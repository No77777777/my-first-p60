#ifndef QMI_MMODE_TASK_CMD_H
#define QMI_MMODE_TASK_CMD_H
/*===========================================================================

                        QMI_MMODE_TASK_CMD.H

DESCRIPTION

  QMI Mmcp Task header file. All definitions, structures, and functions
  needed for performing QMI Modem task to a specific services.

EXTERNALIZED DATA
  qmi_mmode_cmd_enum_type
    Supported commands for QMI_MMODE task.

  qmi_mmode_cmd_handler_type
    Callback prototype for command handler.

  qmi_mmodei_cmd_type
    QMI_MMODE Task Command queues and Command Data Buffers

  qmi_mmode_cmd_type
    Struct used for all QMI_MMODE Task commands

  qmi_mmode_cmd_data_buf_type
    QMI_MMODE Task command user data type

EXTERNALIZED FUNCTIONS
  qmi_mmode_get_cmd_data_buf()
    Allocates a new command buffer to be used by caller to post commands to
    QMI_MMODE task.

  qmi_mmode_free_cmd_data_buf()
    Deallocates the command buffer.

  qmi_mmode_send_cmd()
    Post a command to QMI_MMODE task

  qmi_mmode_set_cmd_handler()
    Set a handler to process a cmd in QMI_MMODE task context

Copyright (c) 2004-2014 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/qmi/src/qmi_mmode_task_cmd.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/08/12    sk     Created module.
===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */

#ifdef __cplusplus
extern "C"
{
#endif

#include "rex.h"
#include "queue.h"
#include "task.h"
#include "msg.h"
#include "nv.h"
#include "sio.h"
#include "rdevmap.h"
#include "qmi_mmode_task_svc.h"
#include "qmi_voice_cm_if.h"
#include "dsm.h"
#include "qmi_framework.h"
#include "lte_rrc_ext_msg.h"
#ifdef FEATURE_TDSCDMA
#include "tds_rrc_msg.h"
#endif

#include "lte_cphy_rssi_msg.h"
#include "lte_cphy_msg.h"

#include "cm_msgr_msg.h"
#include "qmi_mmode_msgr_msg.h"

#include "emm_ext_msg.h"

#include "ds3gpp_ext_msg.h"

#include "tds_rrc_ext_if.h"
#include "tds_rrc_umid.h"
#include "lte_rrc_ext_msg.h"
#include "geran_grr_msg.h"
#include "wcdma_rrc_msg.h"

/*===========================================================================

                      PUBLIC DATA DECLARATIONS

===========================================================================*/
/*===========================================================================
  QMI NV item and status.
===========================================================================*/
extern nv_item_type         qmi_mmode_nv_item;
extern nv_stat_enum_type    qmi_mmode_nv_status;


#define QMI_MMODE_MAX_LOG_MSG_SIZE 100

/*===========================================================================
TYPEDEF QMI_MMODE_CMD_ENUM_TYPE

DESCRIPTION
  This enum lists all the commands that can be sent to QMI_MMODE task for
  processing.
===========================================================================*/
typedef enum
{
  QMI_MMODE_CMD_MIN_CMD                       = -1,

  /* Voice Service cmds */
  QMI_MMODE_CMD_VOICE_FW_CB                   = 1,
  QMI_MMODE_CMD_VOICE_CB,
  QMI_MMODE_CMD_VOICE_CM_IF_CB,
  QMI_MMODE_CMD_VOICE_MAX,

  /* NAS Service cmds */
  QMI_MMODE_CMD_NAS_MIN,
  QMI_MMODE_CMD_NAS_ALLOC_CLID,
  QMI_MMODE_CMD_NAS_DEALLOC_CLID,
  QMI_MMODE_CMD_NAS_INIT_CBACK,
  QMI_MMODE_CMD_NAS_CMD_HDLR,
  QMI_MMODE_CMD_NAS_CMSS_CMD_CB,
  QMI_MMODE_CMD_NAS_CMSS_MSIM_EVT_CB,
  QMI_MMODE_CMD_NAS_CMPH_CMD_CB,
  QMI_MMODE_CMD_NAS_CMPH_EVT_CB,
  QMI_MMODE_CMD_NAS_CMSTATS_EVT_CB,
  QMI_MMODE_CMD_NAS_CMSUBS_EVT_CB,
  QMI_MMODE_CMD_NAS_MMGSDI_CMD_CB,
  QMI_MMODE_CMD_NAS_MMGSDI_EVT_CB,
  QMI_MMODE_CMD_NAS_HDR_EVT_CB,
  QMI_MMODE_CMD_NAS_TIMER_EVT_CB,
  QMI_MMODE_CMD_NAS_INTERNAL_CB,
  QMI_MMODE_CMD_NAS_MCFG_REFRESH_EVT_CB,
  QMI_MMODE_CMD_NAS_MAX,

  /* Common */
  QMI_MMODE_TIMER_EVT_CB,

  /* Always must be last */
  QMI_MMODE_MAX_DEFINED_CMD_TYPES
} qmi_mmode_cmd_enum_type;

typedef enum
{
  QMI_MMODE_MIN,

  QMI_MMODE_FW_INIT_CB,
  QMI_MMODE_FW_ALLOC_CLID_CB,
  QMI_MMODE_FW_DEALLOC_CLID_CB,
  QMI_MMODE_FW_REQ_CB,
  QMI_MMODE_NAS_INT_BUILTIN_PLMN_CB,
  QMI_MMODE_MAX,

} qmi_mmode_cmd_id_type;

#define QMI_MMODE_ENHANCED_F3_DBG_MED          BM(0)
/* To enable MMODE_QMI MEDIUM debug F3 messages */


/*===========================================================================
TYPEDEF QMI_MMODE_CMD_HANDLER_TYPE

DESCRIPTION
  Each command that is listed in qmi_mmode_cmd_enum_type should have a command
  handler specified by the caller. The function qmi_mmode_set_cmd_handler()
  should be used for setting the handler function. The following typedef
  lists the prototype of the handler function.
===========================================================================*/
typedef void (*qmi_mmode_cmd_handler_type)
(
  qmi_mmode_cmd_enum_type cmd,           /* Actual command to be processed       */
  void             *user_data_ptr  /* Command specific user parameters     */
);

/*===========================================================================
TYPEDEF QMI_MMODEI_CMD_TYPE

DESCRIPTION
  QMI Task Command queues and Command Data Buffers
===========================================================================*/
typedef struct
{
  q_link_type       link;            /* For linking onto cmd queue          */
  qmi_mmode_cmd_enum_type cmd;             /* Actual command to be processed      */
  void             *user_data_ptr;   /* Command specific user parameters    */
} qmi_mmodei_cmd_type;

/*===========================================================================
TYPEDEF QMI_MMODE_CMD_TYPE

DESCRIPTION
  Struct used for all QMI commands
===========================================================================*/
typedef struct
{
  uint16  id;
  union
  {
    union
    {
      struct
      {
        uint16 num_instances;
      } init_cb;
      struct
      {
        qmi_framework_common_msg_hdr_type msg_hdr;
      } alloc_clid;
      struct
      {
        qmi_framework_common_msg_hdr_type msg_hdr;
      } dealloc_clid;
      struct
      {
        qmi_framework_msg_hdr_type msg_hdr;
        dsm_item_type             *sdu_in;
      } req_cb;

    } qmi_fw_info;
    union
    {
      struct
      {
        uint8 as_id;
      } bplmn_list_cb;
    } nas_int_info;
  } data;
} qmi_mmode_common_cmd_type;


typedef struct
{
  //msgr_hdr_s              msg_hdr;
  union
  {
    geran_grr_mtc_drx_rsp_t              g;
    wcdma_rrc_qmi_idle_drx_info_rsp_type w;
    lte_rrc_idle_drx_info_cnf_s          l;
    tds_rrc_qmi_drx_level_rsp_type       t;
  } info;
} qmi_nas_mmode_get_negotiated_drx_resp_type;

// Messages QMI_NAS is expecting
union qmi_nas_msgr_msg_u
{

  ds_ims_pdn_info_msg_s                ims_pdn_info_ind;

  cm_mm_set_rpm_parameters_rsp_s_type  set_rpm_parameters_resp;
  cm_mm_get_rpm_parameters_rsp_s_type  get_rpm_parameters_resp;

  lte_rrc_cell_lock_rsp_s              set_cell_lock_config_resp;
  lte_rrc_mtc_cfg_cnf_s                drx_scaling_resp;

#ifdef FEATURE_TDSCDMA
  tds_rrc_gps_pos_cell_info_rsp_type   tds_cell_and_nbr_pos_info_rsp;
  tds_rrc_scell_signal_status_ind_type tds_rrc_scell_sig_ind;
#endif

#ifdef FEATURE_LTE_EMBMS
  lte_rrc_embms_status_change_ind_s    embms_status_change_ind;
  lte_cphy_mbsfn_signal_strength_cnf_s embms_sig_cnf;
#endif

  lte_cphy_ca_event_ind_s cphy_ca_event_ind;
  cm_domain_sel_ims_pref_chg_ind_msg_type   ims_pref_chg_ind;
  cm_domain_sel_ims_pref_rsp_msg_type_ext   ims_pref_rsp;

  cm_ac_info_ind_s_type                e911_state_ind;
  lte_rrc_utc_time_update_ind_s        lte_sib16_nw_time;
  lte_rrc_band_pri_change_cnf_s        lte_band_pri_cnf;
  lte_rrc_embms_signal_strength_report_cnf_s embms_sig_cnf_ext;
  lte_rrc_get_band_pri_list_cnf_s      lte_get_band_pri_list_cnf;
  lte_rrc_get_serv_cell_sib_cnf_s      lte_get_serv_cell_sib_cnf;

  cm_subsc_chgnd_ind_msg_type          subscription_change_ind;
  lte_rrc_barring_update_ind_s lte_barring_ind;
  mmode_qmi_nas_access_barring_status_ind_msg_type lte_barring_status_ind;
  emm_t3402_changed_ind_s_type emm_t3402_changed_ind;

  cm_t3346_info_s_type                 t3346_timer_status_change;

  cm_call_mode_ind_s_type         call_mode_msg;

  cm_mm_get_ssac_hyst_timer_rsp_s_type  get_hys_timer_rsp;

  qmi_nas_mmode_get_negotiated_drx_resp_type     get_negotiated_drx_resp;

  cm_emerg_mode_status_ind_s_type emergency_mode_status_ind;
  lte_rrc_ue_cap_cfg_cnf_s      lte_rrc_ue_cap_cfg;
  geran_grr_qmi_cell_info_ind_t geran_cell_info_ind;
  geran_grr_qmi_cell_info_rsp_t geran_cell_info_rsp;
  cm_emerg_srch_fail_ind_s_type e911_srch_fail_ind;
  #ifdef FEATURE_DISABLED_HABANERO
  lte_rrc_rach_failure_ind_s    lte_rrc_rach_fail_ind;
  lte_cphy_b2_thresh_adj_update_cnf_s  b2_thresh_adj_update_resp;
  #endif
  mmode_qmi_nas_arfcn_list_ind_msg_type     geran_arfcn_list;
  wcdma_rrc_qmi_current_freqs_info_rsp_type wcdma_arfcn_list;
  lte_rrc_current_cells_info_cnf_s          lte_arfcn_list;
 
  #ifndef REMOVE_QMI_NAS_FORCE_LTE_SCAN_V01
  wcdma_rrc_qmi_prioritize_lte_rsp_type prioritize_lte_scan_resp;
  mmode_qmi_nas_access_barring_status_ind_msg_type cm_ssac_access_barring_status_ind;
  #endif
};

// Messages QMI_VOICE is expecting
union qmi_voice_msgr_msg_u
{
  mmode_qmi_voice_conf_participants_info_cmd_msg_type   parsed_conf_cmd;
  cm_srvcc_call_context_rsp_s_type     srvcc_call_context_resp;
  cm_mid_srvcc_ho_comp_list_s_type     mid_call_srvcc_handover_ind;
  mmode_qmi_voice_tty_mode_info_cmd_msg_type   tty_info_cmd;
  mmode_qmi_voice_audio_session_rat_change_cmd_msg_type   audio_rat_change_info_cmd;
  mmode_qmi_voice_participant_status_cmd_msg_type   participant_status_info_cmd;
  mmode_qmi_voice_ecall_status_ind_msg_type    ecall_status_ind;
  cm_mm_get_t_wwan_911_rsp_s_type      get_wwan_911_timer_resp;
  mmode_qmi_voice_viceDialog_info_ind_msg_type     vice_dialog_info_cmd;
  cm_mt_call_end_auto_reject_modem_ind_s_type mt_call_end_auto_reject_modem_ind;

};

/*===========================================================================
TYPEDEF QMI_MMODEI_MSGR_TYPE

DESCRIPTION
  MMODE Task MSGR queue data type
===========================================================================*/
typedef struct
{
  q_link_type       link;           /* For linking onto msgr queue         */

  union qmi_mmmode_msgr_cmd_u
  {
    msgr_hdr_s hdr;   /* Message router header */

    union  qmi_nas_msgr_msg_u qmi_nas; // contains list of msgs QMI_NAS is registered to
    union  qmi_voice_msgr_msg_u qmi_voice; // contains list of msgs QMI_VOICE is registered to

    // MSGR support for other MMODE QMI Service go here

  } cmd;
} qmi_mmodei_msgr_type;

/*===========================================================================
TYPEDEF QMI_MMODE_CMD_TYPE

DESCRIPTION
  QMI command user data type
===========================================================================*/
typedef struct
{
  union
  {
    qmi_mmode_common_cmd_type   *qmi_ptr;
    qmi_voice_cm_if_info_item_s *cm_if_data_ptr;
    void           *user_data_ptr;
  } cmd;
} qmi_mmode_cmd_data_buf_type;


/* QMI MMODE LOGS */
void qmi_mmode_format_log_msg
(
  char *buf_ptr,
  int buf_size,
  char *fmt,
  ...
);

#define QMI_MMODE_LOG_MSG( lvl, fmt, ... )                                           \
  {                                                                              \
    char buf[ QMI_MMODE_MAX_LOG_MSG_SIZE ];                                          \
                                                                                 \
    /* Format message for logging */                                             \
    qmi_mmode_format_log_msg( buf, QMI_MMODE_MAX_LOG_MSG_SIZE, fmt, __VA_ARGS__ );       \
                                                                                 \
    /* Log message to Diag */                                                    \
    MSG_SPRINTF_1( MSG_SSID_DS, lvl, "%s", buf );                     \
  }


// /* Log error level message */
// #define QMI_MSG_ERROR( fmt, ... )   QMI_MMODE_LOG_MSG( MSG_LEGACY_ERROR, fmt, __VA_ARGS__ )

// /* Log fatal level message */
// #define QMI_MSG_FATAL( fmt, ... )   QMI_MMODE_LOG_MSG( MSG_LEGACY_FATAL, fmt, __VA_ARGS__ )

// /* Log debug level message */
// #define QMI_MSG_HIGH( fmt, ... )    QMI_MMODE_LOG_MSG( MSG_LEGACY_HIGH, fmt, __VA_ARGS__ )

// /* Log info level message */
// #define QMI_MSG_MED( fmt, ...  )    QMI_MMODE_LOG_MSG( MSG_LEGACY_MED, fmt, __VA_ARGS__ )

// /* Log verbose level message */
// #define QMI_MSG_LOW( fmt, ... )     QMI_MMODE_LOG_MSG( MSG_LEGACY_LOW, fmt, __VA_ARGS__ )



/* New QMI MSG macros. */
#define QM_MSG_FATAL(x_fmt) \
    MSG (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt)

#define QM_MSG_ERROR(x_fmt) \
    MSG (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt)

#define QM_MSG_HIGH(x_fmt) \
    MSG (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt)

#define QM_MSG_LOW(x_fmt) \
    MSG (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt)

#define QM_MSG_FATAL_1(x_fmt, a) \
    MSG_1 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a)

#define QM_MSG_ERROR_1(x_fmt, a) \
    MSG_1 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a)

#define QM_MSG_HIGH_1(x_fmt, a) \
    MSG_1 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a)

#define QM_MSG_LOW_1(x_fmt, a) \
    MSG_1 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a)

#define QM_MSG_FATAL_2(x_fmt, a, b) \
    MSG_2 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a, b)

#define QM_MSG_ERROR_2(x_fmt, a, b) \
    MSG_2 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a, b)

#define QM_MSG_HIGH_2(x_fmt, a, b) \
    MSG_2 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a, b)

#define QM_MSG_LOW_2(x_fmt, a, b) \
    MSG_2 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a, b)

#define QM_MSG_FATAL_3(x_fmt, a, b, c) \
    MSG_3 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a, b, c)

#define QM_MSG_ERROR_3(x_fmt, a, b, c) \
    MSG_3 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a, b, c)

#define QM_MSG_HIGH_3(x_fmt, a, b, c) \
    MSG_3 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a, b, c)

#define QM_MSG_LOW_3(x_fmt, a, b, c) \
    MSG_3 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a, b, c)

#define QM_MSG_FATAL_4(x_fmt, a, b, c, d) \
    MSG_4 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a, b, c, d)

#define QM_MSG_ERROR_4(x_fmt, a, b, c, d) \
    MSG_4 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a, b, c, d)

#define QM_MSG_HIGH_4(x_fmt, a, b, c, d) \
    MSG_4 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a, b, c, d)

#define QM_MSG_LOW_4(x_fmt, a, b, c, d) \
    MSG_4 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a, b, c, d)

#define QM_MSG_FATAL_5(x_fmt, a, b, c, d, e) \
    MSG_5 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a, b, c, d, e)

#define QM_MSG_ERROR_5(x_fmt, a, b, c, d, e) \
    MSG_5 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a, b, c, d, e)

#define QM_MSG_HIGH_5(x_fmt, a, b, c, d, e) \
    MSG_5 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a, b, c, d, e)

#define QM_MSG_LOW_5(x_fmt, a, b, c, d, e) \
    MSG_5 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a, b, c, d, e)

#define QM_MSG_FATAL_6(x_fmt, a, b, c, d, e, f) \
    MSG_6 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a, b, c, d, e, f)

#define QM_MSG_ERROR_6(x_fmt, a, b, c, d, e, f) \
    MSG_6 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a, b, c, d, e, f)

#define QM_MSG_HIGH_6(x_fmt, a, b, c, d, e, f) \
    MSG_6 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a, b, c, d, e, f)

#define QM_MSG_LOW_6(x_fmt, a, b, c, d, e, f) \
    MSG_6 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a, b, c, d, e, f)

#define QM_MSG_FATAL_7(x_fmt, a, b, c, d, e, f,g) \
    MSG_7 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a, b, c, d, e, f,g)

#define QM_MSG_ERROR_7(x_fmt, a, b, c, d, e, f, g) \
    MSG_7 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a, b, c, d, e, f, g)

#define QM_MSG_HIGH_7(x_fmt, a, b, c, d, e, f, g) \
    MSG_7 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a, b, c, d, e, f, g)

#define QM_MSG_LOW_7(x_fmt, a, b, c, d, e, f, g) \
    MSG_7 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a, b, c, d, e, f,g)

#define QM_MSG_FATAL_8(x_fmt, a, b, c, d, e, f, g, h) \
    MSG_8 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a, b, c, d, e, f, g, h)

#define QM_MSG_ERROR_8(x_fmt, a, b, c, d, e, f, g, h) \
    MSG_8 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a, b, c, d, e, f, g, h)

#define QM_MSG_HIGH_8(x_fmt, a, b, c, d, e, f, g, h) \
    MSG_8 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a, b, c, d, e, f, g, h)

#define QM_MSG_LOW_8(x_fmt, a, b, c, d, e, f, g, h) \
    MSG_8 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a, b, c, d, e, f, g, h)

#define QM_MSG_FATAL_9(x_fmt, a, b, c, d, e, f, g, h, i) \
    MSG_9 (MSG_SSID_MMODE_QMI, MSG_LEGACY_FATAL, x_fmt, a, b, c, d, e, f, g, h, i)

#define QM_MSG_ERROR_9(x_fmt, a, b, c, d, e, f, g, h, i) \
    MSG_9 (MSG_SSID_MMODE_QMI, MSG_LEGACY_ERROR, x_fmt, a, b, c, d, e, f, g, h, i)

#define QM_MSG_HIGH_9(x_fmt, a, b, c, d, e, f, g, h, i) \
    MSG_9 (MSG_SSID_MMODE_QMI, MSG_LEGACY_HIGH, x_fmt, a, b, c, d, e, f, g, h, i)

#define QM_MSG_LOW_9(x_fmt, a, b, c, d, e, f, g, h, i) \
    MSG_9 (MSG_SSID_MMODE_QMI, MSG_LEGACY_LOW, x_fmt, a, b, c, d, e, f, g, h, i)

#define QM_MSG_MED(str)                                       \
  do                                                          \
  {                                                           \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                \
    {                                                         \
      QM_MSG_HIGH(str);                                       \
    }                                                         \
    else                                                      \
    {                                                         \
      MSG (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str);          \
    }                                                         \
  } while (0)

#define QM_MSG_MED_1(str, a)                                  \
  do                                                          \
  {                                                           \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                \
    {                                                         \
      QM_MSG_HIGH_1(str, a);                                  \
    }                                                         \
    else                                                      \
    {                                                         \
      MSG_1 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a);     \
    }                                                         \
  } while (0)

#define QM_MSG_MED_2(str, a, b)                               \
  do                                                          \
  {                                                           \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                \
    {                                                         \
      QM_MSG_HIGH_2(str, a, b);                               \
    }                                                         \
    else                                                      \
    {                                                         \
      MSG_2 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a, b);  \
    }                                                         \
  } while (0)

#define QM_MSG_MED_3(str, a, b, c)                               \
  do                                                             \
  {                                                              \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                   \
    {                                                            \
      QM_MSG_HIGH_3(str, a, b, c);                               \
    }                                                            \
    else                                                         \
    {                                                            \
      MSG_3 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a, b, c);  \
    }                                                            \
  } while (0)

#define QM_MSG_MED_4(str, a, b, c, d)                               \
  do                                                                \
  {                                                                 \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                      \
    {                                                               \
      QM_MSG_HIGH_4(str, a, b, c, d);                               \
    }                                                               \
    else                                                            \
    {                                                               \
      MSG_4 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a, b, c, d);  \
    }                                                               \
  } while (0)

#define QM_MSG_MED_5(str, a, b, c, d, e)                               \
  do                                                                   \
  {                                                                    \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                         \
    {                                                                  \
      QM_MSG_HIGH_5(str, a, b, c, d, e);                               \
    }                                                                  \
    else                                                               \
    {                                                                  \
      MSG_5 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a, b, c, d, e);  \
    }                                                                  \
  } while (0)

#define QM_MSG_MED_6(str, a, b, c, d, e, f)                               \
  do                                                                      \
  {                                                                       \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                            \
    {                                                                     \
      QM_MSG_HIGH_6(str, a, b, c, d, e, f);                               \
    }                                                                     \
    else                                                                  \
    {                                                                     \
      MSG_6 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a, b, c, d, e, f);  \
    }                                                                     \
  } while (0)

#define QM_MSG_MED_7(str, a, b, c, d, e, f, g)                               \
  do                                                                         \
  {                                                                          \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                               \
    {                                                                        \
      QM_MSG_HIGH_7(str, a, b, c, d, e, f, g);                               \
    }                                                                        \
    else                                                                     \
    {                                                                        \
      MSG_7 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a, b, c, d, e, f, g);  \
    }                                                                        \
  } while (0)

#define QM_MSG_MED_8(str, a, b, c, d, e, f, g, h)                               \
  do                                                                            \
  {                                                                             \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                                  \
    {                                                                           \
      QM_MSG_HIGH_8(str, a, b, c, d, e, f, g, h);                               \
    }                                                                           \
    else                                                                        \
    {                                                                           \
      MSG_8 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a, b, c, d, e, f, g, h);  \
    }                                                                           \
  } while (0)

#define QM_MSG_MED_9(str, a, b, c, d, e, f, g, h, i)                               \
  do                                                                               \
  {                                                                                \
    if (qmi_mmode_is_enhanced_f3_dbg_medium())                                     \
    {                                                                              \
      QM_MSG_HIGH_9(str, a, b, c, d, e, f, g, h, i);                               \
    }                                                                              \
    else                                                                           \
    {                                                                              \
      MSG_9 (MSG_SSID_MMODE_QMI, MSG_LEGACY_MED, str, a, b, c, d, e, f, g, h, i);  \
    }                                                                              \
  } while (0)

/*===========================================================================

                      PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================
FUNCTION QMI_MMODE_GET_CMD_DATA_BUF()

DESCRIPTION
  Allocates a new command buffer to be used by caller to post commands to
  QMI_MMODE task.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  Ptr to new command data buffer.

SIDE EFFECTS
  None.
===========================================================================*/
qmi_mmode_cmd_data_buf_type *qmi_mmode_get_cmd_data_buf
(
  void
);

/*===========================================================================
FUNCTION QMI_MMODE_FREE_CMD_DATA_BUF()

DESCRIPTION
  Frees QMI_MMODE task command buffer.

PARAMETERS
  None.

DEPENDENCIES
  Command buffer should have been allocated by a prior call to
  qmi_mmode_get_cmd_data_buf().

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
#define qmi_mmode_free_cmd_data_buf(p)                               \
           if (p != NULL)                                            \
           {                                                         \
             modem_mem_free(p, MODEM_MEM_CLIENT_QMI_MMODE_CRIT);     \
             p = NULL;                                               \
           }

/*===========================================================================
FUNCTION QMI_MMODE_SEND_CMD()
  Send command to be processed in QMI_MMODE task.

DESCRIPTION
  This method is used to send a command to the QMI_MMODE task for processing.
  The command handler registered with the command being sent is executed
  when the QMI_MMODE task is run the next time. The QMI_MMODE task is serialized
  through a command Q, so any pending commands in the QMI_MMODE task would be
  executed before the posted command.

PARAMETERS
  cmd           - Command to send to QMI MODEM task.
  user_data_ptr - User data sent with the command. The
                  command handler would be called with this user data.
RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void qmi_mmode_send_cmd
(
  qmi_mmode_cmd_enum_type cmd,         /* Actual command to be processed         */
  void *user_data_ptr            /* Command specific user parameters       */
);

/*===========================================================================
FUNCTION QMI_MMODE_SET_CMD_HANDLER()

DESCRIPTION
  This function sets a handler for the specified cmd type.  If a NULL cmd
  handler is passed, then the command handling is defaulted (see
  qmi_mmode_default_cmd_handler). The command handler can be updated on the
  fly by calling this funciton again.

PARAMETERS
  cmd         - Command for which we want to set handler.
  cmd_handler - Callback function to be registered as handler.

DEPENDENCIES
  None.

RETURN VALUE
  Previously registered cmd handler

SIDE EFFECTS
  None.
===========================================================================*/
qmi_mmode_cmd_handler_type qmi_mmode_set_cmd_handler
(
  qmi_mmode_cmd_enum_type cmd,             /* Command name                      */
  qmi_mmode_cmd_handler_type cmd_handler   /* Cmd handler function pointer      */
);

/*===========================================================================
FUNCTION QMI_MMODE_TASK_WAIT()

DESCRIPTION
  This function performs the QMI Modem Task waiting function. This function will
  not return until at least one signal from the requested mask is set.
  The watchdog is also kicked at least once.

PARAMETERS
  requested_mask : QMI Modem Task SIG mask.

DEPENDENCIES
  None.

RETURN VALUE
  The signal mask actually set out of the requested mask which terminated
  the wait.

SIDE EFFECTS
  All the set signals in the requested mask are cleared.
===========================================================================*/
rex_sigs_type qmi_mmode_task_wait
(
  rex_sigs_type requested_mask              /* mask of signals to wait for */
);

/*===========================================================================
FUNCTION QMI_MMODE_GET_NV_ITEM_EXT

DESCRIPTION
  This function retrieves the specified item from NV.

PARAMETERS
  item_code : Item to get.
  data_ptr  : Pointer where to put the item.

RETURN VALUE
  Status returned from the NV read request.  An ERR is logged if status is
  other than:
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active

DEPENDENCIES
  The NV task has been started and is running.
  This functions is Non-reentrant.

SIDE EFFECTS
  While this function is running all other QMI Modem task activities are
  suspended except for watchdog kicking, and until the NV item is
  read in.
===========================================================================*/
nv_stat_enum_type qmi_mmode_get_nv_item_ext
(
  nv_items_enum_type  item_code,       /* Item to get                      */
  nv_item_type        *data_ptr,       /* Pointer where to put the item    */
  uint16              context
);

/*===========================================================================
FUNCTION QMI_MMODE_PUT_NV_ITEM_EXT

DESCRIPTION
  Write an item to NV memory.  Wait until write is completed.

PARAMETERS
  item_code : Item to set.
  data_ptr  : Pointer to data for item.

RETURN VALUE
  Status returned from the NV read request.  An ERR is logged if status is
  other than:
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active

DEPENDENCIES
  This function can only be called from QMI modem task.  Also it is not
  reentrant. Calls rex_wait() and doesn't exit until processing is finished.

SIDE EFFECTS
  While this function is running all other QMI modem task activities are
  suspended except for watchdog kicking, and until the NV item is
  wrote down.
===========================================================================*/
nv_stat_enum_type qmi_mmode_put_nv_item_ext
(
  nv_items_enum_type item_code,                /* which item               */
  nv_item_type *data_ptr,                      /* pointer to data for item */
  uint16              context
);

/*===========================================================================
FUNCTION QMI_MMODE_GET_NV_ITEM

DESCRIPTION
  This function retrieves the specified item from NV.

PARAMETERS
  item_code : Item to get.
  data_ptr  : Pointer where to put the item.

RETURN VALUE
  Status returned from the NV read request.  An ERR is logged if status is
  other than:
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active

DEPENDENCIES
  The NV task has been started and is running.
  This functions is Non-reentrant.

SIDE EFFECTS
  While this function is running all other QMI Modem task activities are
  suspended except for watchdog kicking, and until the NV item is
  read in.
===========================================================================*/
nv_stat_enum_type qmi_mmode_get_nv_item
(
  nv_items_enum_type  item_code,       /* Item to get                      */
  nv_item_type        *data_ptr        /* Pointer where to put the item    */
);

/*===========================================================================
FUNCTION QMI_MMODE_PUT_NV_ITEM

DESCRIPTION
  Write an item to NV memory.  Wait until write is completed.

PARAMETERS
  item_code : Item to set.
  data_ptr  : Pointer to data for item.

RETURN VALUE
  Status returned from the NV read request.  An ERR is logged if status is
  other than:
    NV_DONE_S       - request done
    NV_NOTACTIVE_S  - item was not active

DEPENDENCIES
  This function can only be called from QMI modem task.  Also it is not
  reentrant. Calls rex_wait() and doesn't exit until processing is finished.

SIDE EFFECTS
  While this function is running all other QMI modem task activities are
  suspended except for watchdog kicking, and until the NV item is
  wrote down.
===========================================================================*/
nv_stat_enum_type qmi_mmode_put_nv_item
(
  nv_items_enum_type item_code,                /* which item               */
  nv_item_type *data_ptr                       /* pointer to data for item */
);

/*===========================================================================
FUNCTION QMI_MMODE_IS_ENHANCED_F3_DBG_MEDIUM()

DESCRIPTION
  This function checks whether the enhanced F3 debug rule is to print out
  QM_MSG_MED_x() or not.

PARAMETERS
  None.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE  - print out QM_MSG_MED_x() is enabled;
  FALSE - print out QM_MSG_MED_x() is disabled;

SIDE EFFECTS
  None.
===========================================================================*/
boolean qmi_mmode_is_enhanced_f3_dbg_medium
(
  void
);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* QMI_MMODE_TASK_CMD_H */
