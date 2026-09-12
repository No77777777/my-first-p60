/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            L1 Logging Facilities

GENERAL DESCRIPTION
   This module contains functions for logging L1 packets.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2014 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_log_qsh.c#4 $
$DateTime: 2020/05/20 06:23:58 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
20/05/20   sc      CR2677798 Changing variable last_state to gl1_t2g_last_state
28/01/20   rks     CR2611286 minidum related change
11/06/19   rv      CR2293366: GL1 changes for minidump Heap optimization for HANA
27/01/20   rks     CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
20/09/16   snjv    CR1069107 Rectify Enum Name for GL1 Metrics 
29/09/16   snjv    CR1072266 Avoid QSH Error F3 if GL1 isnt Active 
08/11/16   snjv    CR1087399 Use QSH_LOG inplace of QSH_LOG_HIGH_*
11/29/16   sm      CR1089963 Added state transition events and BCCH abort in W2G stare handler, to reproduce issue CR/1082619 
09/03/16   snjv    CR984386 QSH: GL1 to avoid writing QSH metrics when not Active
22/04/16   ksb     CR1005869 To mainlne FEATURE_GL1_SLEEP_DEBUG_STRUCT
30/06/16   sm      CR1035750 Added common event to logging all failure events.
09/06/16   mn      CR1026351 Resolve linking error in QTF mob build
31/05/16   mn      CR1022253 Idle DTF/Handlers
12/05/16   mk      CR1010068 Add DTF events in dedicated mode
21/12/15   snjv    CR954530 QSH: GL1 should populate QSH Metrics only when applicable/available
14/01/16   sm      CR962228 Masking QSH_ACTION_CFG if only FEATURE_QSH_EVENT_NOTIFY_TO_QSH feature is enabled 
30/12/15   sk      CR956838 GERAN compilation error fix when enabling only feature FEATURE_QSH_EVENT_NOTIFY_TO_QSH 
11/12/15   snjv    CR950902 QSH: Get recent Tx metric on Timer expiry after state change
14/10/15   snjv    CR919955 FR38046:QSH Call End Data Collection
30/09/15   sk      CR916227 Disable frequent QSH events and add PCH rsv handler
10/09/15   sk      CR899847 GERAN changes to trigger QSH events and handler for debugging
26/08/15   ws      CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
11/06/15   sjv     CR852907 Initialize QSH metric array row by row  
12/05/15   pjr     CR836741 Move gl1_qsh_dump_tag_x_ptr definition to l1_log_qsh.c
12/05/15   pjr     CR836628 Move variable declaration out of for loop
19/02/15   ab      Initial Revision.
20/02/15   ab      CR773948 GL1 Support for provding Dumps and Metrics for QSH
===========================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE
===========================================================================*/
#include "qsh_util.h"
#include "geran_variation.h"
#include "ms.h"
#include "l1_log.h"         // This module's header file
#include "log.h"            // For log_commit() and log_alloc() and log codes
#include "diag.h"           // Needed for DIAG_DIAGVER prior to calling event.h
#include "event.h"          // For logging RAR event and event codes
#include "log_codes_gsm.h"
#include "log_codes_gsm_v.h"  // For GSM log codes

#include "l1i.h"            // For TSKISR struct (for l1_state)
#include "l1_sc.h"          // for l1_sc_globals
#include "gl1_sys_params.h" // for SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16
#include <string.h>         // for memset
#include "gtmrs.h"
#include "gtmrs_g.h"        // For GSTMR_GET_FN_GERAN( gas_id) & SUB_FN()

#ifdef FEATURE_QSH_MDUMP

#include "gpl1_grm_intf.h"
#include "l1_fm.h"
#include "gl1_voice_server.h"
#include "mdsp_debug.h"
#include "gl1_hw.h"
#include "gpl1_gprs_if_g.h"
#include "gl1_hw_clk_ctl_g.h"
#ifdef FEATURE_GSM_RX_DIVERSITY
  #include "gl1_hw_rxdiversity.h"
#endif
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
  #include "gl1_asdiv.h"
#endif
#endif


#include "gpl1_gprs_log.h"  // for convert_txlev_to_pwr_dBm()
#include "l1_utils.h"
#include "gl1_msgi.h"
#include <stringl/stringl.h>

#include "l1_task.h"


#include "l1_isr.h"
#include "gl1_hw_sleep_ctl.h"


#include "geran_multi_sim.h"
#include "geran_dual_sim.h"
#include "l1_log_qsh.h"
#include "gl1_msg_g.h"

//QSH Debug Struct
gl1_qsh_dump_tag_data_s   *qsh_dbg_dump_ptr;//Addr from where GL1 dump starts

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];  // For access to l1_state (in l1_task.c)
extern l1_serving_cell_meas_T l1_serving_cell_meas[];
extern l1_sc_wcdma_data_t l1_sc_wcdma_data[];
extern l1_sc_globals_T l1_sc_globals_store[];

#ifdef FEATURE_QSH_MDUMP

extern boolean gl1_mcpm_tx_on[];
extern gl1_hw_qta_event_data_t gl1_hw_qta_event_data[];
extern uint8 curr_cmc[];
extern uint8 new_cmc[];
extern uint8 curr_cmr[];
extern uint8 computed_cmr[];
extern uint8 curr_ul_cmi[];
extern uint8 curr_dl_cmi[];
extern Frequency_List_T *l1_Frequency_List_Store[];
extern sys_modem_device_mode_e_type gl1_msg_multi_sim_mode;
extern gl1_hw_x2g_data_type gl1_hw_x2g_data[];
extern gl1_dtm_if_data_t gl1_dtm_if_data[];
extern l1_isr_data_t l1_isr_data[];
extern voice_server_session_data_t server_session_data[];
extern channel_type_T amr_channel[];
extern mdsp_debug_private_data_t mdsp_debug_private_data[];
extern uint16 t2g_rf_api_profiling_index;
extern uint16 t2g_api_profiling_index;
extern t2g_api_profiling_type t2g_api_profiling_struct[];
extern t2g_rf_api_profiling_type t2g_rf_api_profiling_struct[];
extern gl1_hw_x2g_startup_info_t gl1_hw_x2g_startup_info;
extern gl1_hw_x2g_cleanup_info_t gl1_hw_x2g_cleanup_info;
extern gl1_hw_x2g_pwr_meas_info_t gl1_hw_x2g_pwr_meas_info;
extern cm_acq_state_machine_type cm_acq_state_machine[];
extern cm_pwr_meas_state_machine_type cm_pwr_meas_state_machine[];
extern uint8 cm_pwr_sm;
extern uint8 num_of_pwr_measures;

#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
extern uint8 cm_acq_state_machine_active_array[];
extern uint8 cm_acq_state_machine_num_active;
#endif

extern boolean send_err_msg;
extern gl1_hw_x2g_api_type gl1_t2g_last_state;
extern boolean queued_x2g_startup;
extern boolean cm_startup_issued;
extern gas_id_t gl1_hw_t2g_gas_id;
extern gl1_x2g_command_info_t X2g_Cmd_Info[];
extern boolean gl1_hw_force_set_app_mode_cmd;
extern ARFCN_T cm_rssi_arfcn[];
extern acq_decode_rpt gl1_hw_cm_fcch_rpt;
extern gl1_hw_cm_sch_tags_type gl1_hw_cm_sch_tags[];
extern boolean gl1_hw_tx_on[];
#ifdef FEATURE_GSM_NB_JAMMER_DETECTION
extern gl1_hw_nb_jdet_data_t gl1_hw_nb_jdet_data[];
#endif
extern boolean gl1_mcpm_idle[];
extern gl1_mcpm_state_type gl1_mcpm_state[];
extern gl1_mcpm_update_gsm_dpc_param_type mcpm_gsm_param[];
extern dedicated_data_T l1_dedicated_data[];
#ifdef FEATURE_GSM_RX_DIVERSITY
extern gl1_hw_rxdiv_data_t gl1_hw_rxdiv_data[];
extern boolean gl1_hw_msim_rxdiv_data;
#endif
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern gl1_trm_asdiv_traffic_mode_T gl1_trm_asdiv_traffic_store[];
extern gl1_trm_asdiv_drds_mode_t gl1_trm_asdiv_drds_mode_var[];
extern gl1_hw_asdiv_type1_T gl1_hw_asdiv_type1;
#endif
#endif

#if defined(FEATURE_QSH_EVENT_METRIC)
static gl1_qsh_metric_bufffer_t gl1_qsh_metric_buffer[NUM_GERAN_DATA_SPACES][GL1_QSH_METRIC_MAX];
#endif

/*Add failure Event to array */
static uint32 gl1_qsh_failure_event_enums[] = {
                                               GL1_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE,
                                               GL1_QSH_EVENT_OOS, 
                                               GL1_QSH_EVENT_G2W_HO_FAILURE,                        
                                               GL1_QSH_EVENT_RESEL_FAILURE,                       
                                               GL1_QSH_EVENT_CON_REL_L2_RESET,                    
                                               GL1_QSH_EVENT_PAGE_DEC_FAIL,
                                               GL1_QSH_EVENT_EMPTY_QTA_GAP,
                                               GL1_QSH_EVENT_TRM_LOCK_WAIT_TIMEOUT,
                                               GL1_QSH_EVENT_RX_HIGH_TIMING_ERROR,
                                               GL1_QSH_EVENT_RX_HIGH_FREQ_ERROR,
                                               GL1_QSH_EVENT_HIGH_RXLEV_DIFF_PRI_DIV,
                                               GL1_QSH_EVENT_CRC_FAIL_GOOD_SNR_RXPWR,
                                               GL1_QSH_EVENT_CRC_FAIL_BAD_SNR_GOOD_RXPWR,
                                               GL1_QSH_EVENT_CRC_FAIL_GOOD_SNR_BAD_RXPWR,
                                               GL1_QSH_EVENT_HRXQUAL_DROP_GOOD_RXPWR,
                                               GL1_QSH_EVENT_DS_ABORT,
                                               GL1_QSH_EVENT_HIGH_BLER
											   };


#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)

void gl1_qsh_debug_callback(qsh_client_cb_params_s *param);
void gfw_qsh_debug_callback(qsh_client_cb_params_s *param);

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR void gl1_qsh_mdump_collect_data(qsh_client_cb_params_s *param);

QSH_MDUMP_FN_ATTR static void gl1_qsh_mdump_collect_high(gas_id_t gas_id);

QSH_MDUMP_FN_ATTR static void gl1_qsh_mdump_collect_low(gas_id_t gas_id);

QSH_MDUMP_FN_ATTR static void gl1_qsh_mdump_collect(gas_id_t gas_id);

QSH_MDUMP_FN_ATTR static void gl1_qsh_mdump_collect_inactive(uint8 gas_id);
#endif


#ifdef FEATURE_QSH_DUMP

// Define a structure with QSH dump header and all TLV structures
typedef struct
{
  qsh_dump_hdr_s hdr;
  gl1_qsh_dump_tag_data_s gl1_qsh_dump_tag_data_1;
  gl1_qsh_dump_tag_data_s gl1_qsh_dump_tag_data_2;
} gl1_qsh_tc_dump_collect_s;

typedef struct
{
  qsh_dump_hdr_s hdr;
  gfw_qsh_dump_tag_data_s gl1_qsh_dump_tag_data_1_;
  gfw_qsh_dump_tag_data_s gl1_qsh_dump_tag_data_2;
} gfw_qsh_tc_dump_collect_s;

/* Static Vars */
/*Copy/Cache of the Tx Power when reset on State Changes*/
static int32 gl1_qsh_tx_metric_copy_on_reset[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
static gl1_qsh_metric_tch_rx_params_s gl1_qsh_tch_rx_params_cache[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE_S( {0} ) };

static qsh_client_dump_info_s gl1_qsh_dump_init(void);
static qsh_client_dump_info_s gfw_qsh_dump_init(void);

static void gl1_populate_qsh_dump_data(qsh_client_cb_params_s *param);
static void gfw_populate_qsh_dump_data(qsh_client_cb_params_s *cb_param);

static void gl1_qsh_dump_data_get(gl1_dump_tag_data_s *data_ptr, gas_id_t gas_id);
void gfw_qsh_dump_data_get(gfw_qsh_dump_s *data_ptr1, gfw_qsh_dump_s *data_ptr2,uint8 *gfw_result);

static void gl1_get_qsh_dump_data
(
  gl1_qsh_dump_tag_data_s *dump_data_ptr,
  gl1_qsh_dump_tag_e       tag
);

void gfw_get_qsh_dump_data
(
  gfw_qsh_dump_tag_data_s *dump_data_ptr1,
  gfw_qsh_dump_tag_data_s *dump_data_ptr2,
  uint8                   *gfw_result

);

static void  gl1_state_snapshot( gl1_dump_tag_data_s *pBuf, gas_id_t gas_id);
static void  gl1_msg_snapshot( gl1_dump_tag_data_s *pBuf, l1_msg_traffic_log_T *pLog, uint16 msgIndex );
static void  gl1_irat_snapshop(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id);
static void  gl1_mdsp_snapshot(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id);
static void  gl1_sleep_snapshot(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id);
static void  gl1_paging_snapshot(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id);
static void  gl1_transfer_snapshot(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id);

// Pointer declared as type of QSH data structure for parser
gl1_qsh_dump_tag_data_s  *gl1_qsh_dump_tag_0_ptr;
gl1_qsh_dump_tag_data_s  *gl1_qsh_dump_tag_1_ptr;
#endif /*  FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_EVENT_METRIC

qsh_ext_metric_cfg_s gl1_qsh_metric_cfg_arr[NUM_GERAN_DATA_SPACES][GL1_QSH_METRIC_MAX];

static qsh_client_metric_info_s gl1_qsh_metric_init(void);

static void gl1_qsh_process_metrics_cfg(qsh_client_cb_params_s *cb_param);
static void gl1_qsh_process_metrics_timer_expiry(qsh_client_cb_params_s *cb_param);

static void gl1_call_in_task_qsh_metric_ind(qsh_client_cb_params_s *cb_params);
static void gfw_qsh_metric_callback(qsh_client_cb_params_s *cb_param);
static boolean gl1_qsh_get_metric_data(gl1_qsh_metric_e metric_idx,gas_id_t gas_id);
static void gl1_qsh_write_tch_rx_params (gl1_qsh_metric_tch_rx_params_s* qsh_ptr, gas_id_t gas_id);
static int32 gl1_qsh_get_tx_metric_on_reset(gas_id_t gas_id);

#endif /* FEATURE_QSH_EVENT_METRIC */

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
gl1_qsh_dtf_event_data_t gl1_qsh_dtf_event_data[NUM_GERAN_DATA_SPACES] = {0};
int64 rem_time_ms;/* QSH IDLE*/
boolean set_event_id_to_failure[NUM_GERAN_DATA_SPACES] = {FALSE};

static void gl1_qsh_dtf_event_data_init(void);
#endif 

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP) || defined(FEATURE_QSH_MDUMP)


#ifdef FEATURE_QSH_DUMP
/*===========================================================================


FUNCTION  gl1_qsh_dump_init

DESCRIPTION
  GFW init for Dumps

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
qsh_client_dump_info_s gl1_qsh_dump_init(void)
{
  qsh_client_dump_info_s dump_info;

  dump_info.max_size_bytes = NUM_GERAN_DATA_SPACES * sizeof(gl1_qsh_dump_tag_data_s);

  return dump_info;
}

/*===========================================================================


FUNCTION  gfw_qsh_dump_init

DESCRIPTION
  GFW init for Dumps

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
qsh_client_dump_info_s gfw_qsh_dump_init(void)
{
  qsh_client_dump_info_s dump_info;

  dump_info.max_size_bytes = NUM_GERAN_DATA_SPACES * sizeof(gfw_qsh_dump_tag_data_s);

  return dump_info;
}

#endif /*  FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_EVENT_METRIC
/*===========================================================================


FUNCTION  gl1_qsh_metric_init

DESCRIPTION
  GL1 Init for metrics

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
qsh_client_metric_info_s gl1_qsh_metric_init(void)
{
  uint8 sub_index;
  qsh_client_metric_info_s metric_info;
  qsh_ext_metric_cfg_s *metric_cfg_ptr;

  for (sub_index = 0; sub_index < NUM_GERAN_DATA_SPACES; sub_index++)
  {
    qsh_client_metric_cfg_init(gl1_qsh_metric_cfg_arr[sub_index], GL1_QSH_METRIC_MAX);

    metric_cfg_ptr = &gl1_qsh_metric_cfg_arr[sub_index][GL1_QSH_METRIC_RSSI];

    metric_cfg_ptr->id = GL1_QSH_METRIC_RSSI;
    metric_cfg_ptr->subs_id = geran_map_gas_id_to_nas_id((gas_id_t)sub_index);
    metric_cfg_ptr->fifo.element_size_bytes = sizeof(gl1_qsh_metric_rssi_s);
    metric_cfg_ptr->fifo.element_count_total = 100;
    metric_cfg_ptr->sampling_period_ms = 100;

    metric_cfg_ptr = &gl1_qsh_metric_cfg_arr[sub_index][GL1_QSH_METRIC_RACH_TX_PWR];

    metric_cfg_ptr->id = GL1_QSH_METRIC_RACH_TX_PWR;
    metric_cfg_ptr->subs_id = geran_map_gas_id_to_nas_id((gas_id_t)sub_index);
    metric_cfg_ptr->fifo.element_size_bytes = sizeof(gl1_qsh_metric_rach_tx_pwr_s);
    metric_cfg_ptr->fifo.element_count_total = 10;
    metric_cfg_ptr->sampling_period_ms = 100;

    
    metric_cfg_ptr = &gl1_qsh_metric_cfg_arr[sub_index][GL1_QSH_METRIC_TCH_RX_PARAMS];

    metric_cfg_ptr->id = GL1_QSH_METRIC_TCH_RX_PARAMS;
    metric_cfg_ptr->subs_id = geran_map_gas_id_to_nas_id((gas_id_t)sub_index);
    metric_cfg_ptr->fifo.element_size_bytes = sizeof(gl1_qsh_metric_tch_rx_params_s);
    metric_cfg_ptr->fifo.element_count_total = 25;
    metric_cfg_ptr->sampling_period_ms = 480;
  }

  metric_info.metric_cfg_arr_ptr = &gl1_qsh_metric_cfg_arr[0][0];
  metric_info.metric_cfg_count = NUM_GERAN_DATA_SPACES * GL1_QSH_METRIC_MAX;

  return metric_info;
}

#endif /* FEATURE_QSH_EVENT_METRIC */

/*===========================================================================

FUNCTION  gl1_qsh_register

DESCRIPTION
  GL1 Registration with QSH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif

void gl1_qsh_register(void)
{
  qsh_client_reg_s client_reg;
  qsh_action_mask_t     cb_action_support_mask = 0;
  qsh_client_reg_init(&client_reg);
  client_reg.client = QSH_CLT_GL1;
  client_reg.major_ver = GL1_QSH_MAJOR_VER ;
  client_reg.minor_ver = GL1_QSH_MINOR_VER;
  client_reg.client_cb_ptr = gl1_qsh_debug_callback;

#ifdef FEATURE_QSH_DUMP
  cb_action_support_mask |= QSH_ACTION_DUMP_COLLECT;

  // Populate the dump info
  client_reg.dump_info = gl1_qsh_dump_init();
#endif /* FEATURE_QSH_DUMP */
#ifdef FEATURE_QSH_MDUMP
	cb_action_support_mask |= QSH_ACTION_MDUMP;
#endif /* FEATURE_QSH_MDUMP */

#ifdef FEATURE_QSH_EVENT_METRIC
  // Populate the metric config info
  client_reg.metric_info = gl1_qsh_metric_init();

  cb_action_support_mask |= (QSH_ACTION_METRIC_CFG|QSH_ACTION_METRIC_TIMER_EXPIRY);
#endif /* FEATURE_QSH_EVENT_METRIC */

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  /* default set all events as rare events, enable all GSM events*/
  gl1_qsh_dtf_event_data_init();
  cb_action_support_mask |= ( QSH_ACTION_EVENT_NOTIFY);
  cb_action_support_mask |= ( QSH_ACTION_CFG );
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  client_reg.cb_action_support_mask = (cb_action_support_mask);

  qsh_client_reg(&client_reg);
} /* gl1_qsh_register */

/*===========================================================================

FUNCTION  gl1_gfw_qsh_register

DESCRIPTION
  GL1  Registration for GFW with QSH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_gfw_qsh_register(void)
{
  qsh_client_reg_s client_reg;
  qsh_action_mask_t     cb_action_support_mask = 0;
  qsh_client_reg_init(&client_reg);
  client_reg.client = QSH_CLT_GFW;
  client_reg.major_ver = GFW_QSH_MAJOR_VER;
  client_reg.minor_ver = GFW_QSH_MINOR_VER;
  client_reg.client_cb_ptr = gfw_qsh_debug_callback;

#ifdef FEATURE_QSH_DUMP
  cb_action_support_mask = QSH_ACTION_DUMP_COLLECT;
  // Populate the dump info
  client_reg.dump_info = gfw_qsh_dump_init();
#endif /* FEATURE_QSH_DUMP */

  client_reg.cb_action_support_mask = (cb_action_support_mask);
  qsh_client_reg(&client_reg);
} /* gl1_gfw_qsh_register */

/*===========================================================================

FUNCTION  gl1_qsh_debug_callback

DESCRIPTION
  QSH CALLBACK for GL1 DUMP/METRICS collection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif

void gl1_qsh_debug_callback(qsh_client_cb_params_s *cb_param)
{
  ASSERT(cb_param != NULL);

  switch(cb_param->action)
  {
#ifdef FEATURE_QSH_EVENT_METRIC
    case QSH_ACTION_METRIC_CFG:
    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      gl1_call_in_task_qsh_metric_ind(cb_param); //Post a message to L1
      break;
#endif

#ifdef FEATURE_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
      gl1_populate_qsh_dump_data(cb_param);
      break;
#endif

#ifdef FEATURE_QSH_MDUMP
		case QSH_ACTION_MDUMP:
		{
		  gl1_qsh_mdump_collect_data(cb_param);
		}
		  break;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH  
    case QSH_ACTION_CFG:
     {
       qsh_client_action_done_s action_done;  
       gl1_qsh_configure_event_data(cb_param->action_params.cfg);

       /* Notify QSH that the required action is handled */
       qsh_client_action_done_init(&action_done);
       action_done.cb_params_ptr = cb_param;
       action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
       qsh_client_action_done(&action_done);		   
     }
	 break;
#endif
	  
    default:
      break;
  }
} /* gl1_qsh_debug_callback */



#ifdef FEATURE_QSH_MDUMP

QSH_MDUMP_FN_ATTR void gl1_qsh_mdump_collect_data(qsh_client_cb_params_s *param)
{
  qsh_client_action_done_s client_action_done;

  int i;

  for(i=0;i<NUM_GERAN_DATA_SPACES;i++)
  {
    if((l1_tsk_buffer[i].l1_state != L1_INACTIVE_MODE) && (l1_tsk_buffer[i].l1_state != L1_MULTI_SIM_INACTIVE) && (l1_tsk_buffer[i].l1_state != 0))
        gl1_qsh_mdump_collect(i);
    else
        gl1_qsh_mdump_collect_inactive(i);
  }

  qsh_mdump_prioritize_client(QSH_CLT_GL1,QSH_CLT_GRR);
}



/*===========================================================================

FUNCTION  gl1_qsh_mdump_collect

DESCRIPTION
  Function to collect GL1 mini dump.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR void gl1_qsh_mdump_collect(gas_id_t gas_id)
{
  gl1_qsh_mdump_collect_high(gas_id);
  gl1_qsh_mdump_collect_low(gas_id);
}

QSH_MDUMP_FN_ATTR void gl1_qsh_mdump_collect_high(gas_id_t gas_id)
{
  int i;
  grm_info_t *grm_info_p = get_grm_info_ptr();

  qsh_mdump_collect_high(grm_info_p,(sizeof(grm_info_t) * MAX_NUM_GRM_CLIENTS));
  for(i=0;i<MAX_NUM_GRM_CLIENTS;i++)
  {
     if((grm_info_p + i)->tx_info_p != NULL)
        qsh_mdump_collect_high((grm_info_p + i)->tx_info_p,sizeof(grm_tx_info_t));
  }

  qsh_mdump_collect_high((void *)(&l1_tsk_buffer[gas_id]),(sizeof(ISRTIM_CMD_BLK)));

  qsh_mdump_collect_high(&gtmrs_data[gas_id],(sizeof(gtmrs_data_t)));
  qsh_mdump_collect_high(gtmrs_data[gas_id].geran_view,sizeof(vstmr_geran_view_s));

  qsh_mdump_collect_high(&l1_idle_data_store[gas_id],(sizeof(idle_data_T)));

  qsh_mdump_collect_high(&l1_nv_log[gas_id],(sizeof(l1_NVLog_struct_T)));

  qsh_mdump_collect_high(&gl1_mcpm_tx_on[gas_id],(sizeof(boolean)));
  qsh_mdump_collect_low(&gl1_hw_tx_on[gas_id],(sizeof(boolean)));

  qsh_mdump_collect_high(&gl1_hw_sleep_struct[gas_id],(sizeof(gl1_hw_sleep_type) * NUM_OF_GL1_DEBUG_CYCLES));

  qsh_mdump_collect_high(&l1_sc_globals_store[gas_id],(sizeof(l1_sc_globals_T)));
  qsh_mdump_collect_high(l1_sc_globals_store[gas_id].receiving_cell,sizeof(cell_T));
  qsh_mdump_collect_high(l1_sc_globals_store[gas_id].sorted_cells,(sizeof(cell_T)*(BA_MAX_LEN + 1)));

  qsh_mdump_collect_high(&gl1_msg_multi_sim_mode,sizeof(sys_modem_device_mode_e_type));

  qsh_mdump_collect_high(&l1_fm_data[gas_id],(sizeof(l1_fm_data_t)));

  qsh_mdump_collect_high(&gl1_hw_qta_event_data[gas_id],(sizeof(gl1_hw_qta_event_data_t)));

  qsh_mdump_collect_high(&gl1_hw_x2g_data[gas_id],(sizeof(gl1_hw_x2g_data_type)));
  
  if( gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr != NULL )
  {
    qsh_mdump_collect_high(gl1_hw_x2g_data[gas_id].gl1_hw_rfm_meas_common_ptr,sizeof(rfm_meas_common_param_type));
  }
  qsh_mdump_collect_high(&l1_transfer_data[gas_id],(sizeof(transfer_data_T)));

  qsh_mdump_collect_high(&gl1_dtm_if_data[gas_id],(sizeof(gl1_dtm_if_data_t)));

  qsh_mdump_collect_high(&grm_gbta_info[gas_id],(sizeof(grm_gbta_info_t)));

  qsh_mdump_collect_high(&l1_isr_data[gas_id],(sizeof(l1_isr_data_t)));

  qsh_mdump_collect_high(&gpl1_g2x_ta_data[gas_id],(sizeof(gpl1_g2x_ta_data_t)));

  qsh_mdump_collect_high(&server_session_data[gas_id].vocoder_state,sizeof(l1_voice_server_state_t));

  qsh_mdump_collect_high(&amr_channel[gas_id],(sizeof(channel_type_T)));

  qsh_mdump_collect_high(&mdsp_debug_private_data[gas_id],(sizeof(mdsp_debug_private_data_t)));

  qsh_mdump_collect_high(&l1_sc_wcdma_data[gas_id],sizeof(l1_sc_wcdma_data_t));

  qsh_mdump_collect_high(&l1_Frequency_List_Store[gas_id],(sizeof(Frequency_List_T *)));
  if (l1_Frequency_List_Store[gas_id] != NULL)
  {
     qsh_mdump_collect_high(l1_Frequency_List_Store[gas_id],(sizeof(Frequency_List_T)));
  }

#ifdef FEATURE_GSM_NB_JAMMER_DETECTION
  qsh_mdump_collect_low(&gl1_hw_nb_jdet_data[gas_id],(sizeof(gl1_hw_nb_jdet_data_t)));
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
  qsh_mdump_collect_high(&gl1_hw_rxdiv_data[gas_id],sizeof(gl1_hw_rxdiv_data_t));

  qsh_mdump_collect_high(&gl1_hw_msim_rxdiv_data,(sizeof(boolean) * (GL1_RXDIV_MAX_MULTIMODE * GL1_RXDIV_MAX_COMBINATION * GL1_RXDIV_MAX_DEVICE_MODE * GL1_RXDIV_MAX_SIM_TYPE * GL1_RXDIV_MAX_SUB_STATE)));
#endif

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING

  //  qsh_mdump_collect_high(&gl1_asdiv_data,sizeof(gl1_asdiv_data_T));
  
  qsh_mdump_collect_high(&gl1_hw_asdiv_type1,sizeof(gl1_hw_asdiv_type1_T));
  
  //  qsh_mdump_collect_high(&gl1_asdiv_type2[gas_id],sizeof(gl1_asdiv_type2_T));
  
  qsh_mdump_collect_high(&gl1_trm_asdiv_traffic_store[gas_id],sizeof(gl1_trm_asdiv_traffic_mode_T));
  
  qsh_mdump_collect_high(&gl1_trm_asdiv_drds_mode_var[gas_id],sizeof(gl1_trm_asdiv_drds_mode_t));

#endif


}


QSH_MDUMP_FN_ATTR void gl1_qsh_mdump_collect_low(gas_id_t gas_id)
{
  qsh_mdump_collect_low(&curr_cmc[gas_id],(sizeof(uint8)));

  qsh_mdump_collect_low(&new_cmc[gas_id],(sizeof(uint8)));

  qsh_mdump_collect_low(&curr_cmr[gas_id],(sizeof(uint8)));

  qsh_mdump_collect_low(&computed_cmr[gas_id],(sizeof(uint8)));

  qsh_mdump_collect_low(&curr_ul_cmi[gas_id],(sizeof(uint8)));

  qsh_mdump_collect_low(&l1_dedicated_data[gas_id],(sizeof(dedicated_data_T)));

  qsh_mdump_collect_low(&mcpm_gsm_param[gas_id],(sizeof(gl1_mcpm_update_gsm_dpc_param_type) * (MCPM_DPC_INDEX_MAX + 1)));

  qsh_mdump_collect_low(&gl1_mcpm_state[gas_id],(sizeof(uint8)));

  qsh_mdump_collect_low(&gl1_mcpm_idle[gas_id],(sizeof(boolean)));

}


QSH_MDUMP_FN_ATTR void gl1_qsh_mdump_collect_inactive(gas_id_t gas_id)
{
  int i,j;
  qsh_mdump_collect_high((void *)(&l1_tsk_buffer[gas_id]),(sizeof(ISRTIM_CMD_BLK)));

  qsh_mdump_collect_low(&l1_nv_log[gas_id],(sizeof(l1_NVLog_struct_T)));

  qsh_mdump_collect_low(&gtmrs_data[gas_id],(sizeof(gtmrs_data_t)));




  
  //IRAT variables
  qsh_mdump_collect_low(&mdsp_debug_private_data[gas_id],(sizeof(mdsp_debug_private_data_t)));
  if( gl1_hw_cm_status[gas_id].ActionValue != NoneAI )
  {
   qsh_mdump_collect_high(&gl1_hw_cm_status[gas_id],(sizeof(gl1_hw_cm_status_t)));

   qsh_mdump_collect_high(&gl1_hw_x2g_data[gas_id],(sizeof(gl1_hw_x2g_data_type)));

   qsh_mdump_collect_low(&t2g_api_profiling_index,(sizeof(uint16)));

   for(i=0; i<MAX_PROFILING_LOG; i++)
   {
    qsh_mdump_collect_low(&t2g_api_profiling_struct[i],(sizeof(t2g_api_profiling_type)));
   }
  
   qsh_mdump_collect_high(&gl1_hw_x2g_startup_info,(sizeof(gl1_hw_x2g_startup_info_t)));
  
   qsh_mdump_collect_high(&gl1_hw_x2g_cleanup_info,(sizeof(gl1_hw_x2g_cleanup_info_t)));
  
   qsh_mdump_collect_high(&gl1_hw_x2g_pwr_meas_info,(sizeof(gl1_hw_x2g_pwr_meas_info_t)));
  
   for(i=0; i<ACQ_MEAS_NUM_SM; i++)
   {
    qsh_mdump_collect_high(&cm_acq_state_machine[i],(sizeof(cm_acq_state_machine_type)));
   }
  
   for(i=0; i<POWER_MEAS_NUM_SM; i++)
   {
    qsh_mdump_collect_high(&cm_pwr_meas_state_machine[i],(sizeof(cm_pwr_meas_state_machine_type)));
   }
  
   qsh_mdump_collect_high(&cm_pwr_sm,(sizeof(uint8)));
  
   qsh_mdump_collect_high(&num_of_pwr_measures,(sizeof(uint8)));
   
#ifdef FEATURE_GSM_T2G_PARALLEL_FCCH_SCH
   for(i=0; i<ACQ_MEAS_NUM_SM; i++)
   {
   qsh_mdump_collect_high(&cm_acq_state_machine_active_array[i],(sizeof(uint8)));
   }
  
   qsh_mdump_collect_high(&cm_acq_state_machine_num_active,(sizeof(uint8)));
#endif
   qsh_mdump_collect_high(&send_err_msg,(sizeof(boolean)));
  
   qsh_mdump_collect_high(&gl1_t2g_last_state,(sizeof(gl1_hw_x2g_api_type)));
  
   qsh_mdump_collect_high(&queued_x2g_startup,(sizeof(boolean)));
  
   qsh_mdump_collect_high(&cm_startup_issued,(sizeof(boolean)));
  
   qsh_mdump_collect_high(&gl1_hw_t2g_gas_id,(sizeof(gas_id_t)));
  
   for(i=0; i<GL1_HW_X2G_CMD_MAX; i++)
   {
    qsh_mdump_collect_high(&X2g_Cmd_Info[i],(sizeof(gl1_x2g_command_info_t)));
   }
  
   qsh_mdump_collect_high(&gl1_hw_force_set_app_mode_cmd,(sizeof(boolean)));
  
   qsh_mdump_collect_high(&cm_rssi_arfcn[gas_id],(sizeof(ARFCN_T) * MAX_CM_RSSI_MEASUREMENTS));
  
   qsh_mdump_collect_high(&gl1_hw_cm_fcch_rpt,(sizeof(acq_decode_rpt)));
  
   qsh_mdump_collect_high(&gl1_hw_cm_sch_tags[gas_id],(sizeof(gl1_hw_cm_sch_tags_type) * Gl1_HW_CM_NUM_SCH_METRICS));
  }
}
#endif /* FEATURE_QSH_MDUMP */



#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*===========================================================================
   FUNCTION     gl1_qsh_dtf_event_data_init
  
   DESCRIPTION
   init the event data mask for all the data spaces
  
   DEPENDENCIES
     None.
  
   PARAMETERS
    gas_id, event_id
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void gl1_qsh_dtf_event_data_init(void)
{
  /*Disable all the GL1 Events(Rare and Frequent). 
   We can enable particular event by using CFG command */ 
    memset(gl1_qsh_dtf_event_data, 0, sizeof(gl1_qsh_dtf_event_data));

}


/*===========================================================================
   FUNCTION     gl1_qsh_event_notify
  
   DESCRIPTION
   Notifies an event to QSH if configured
  
   DEPENDENCIES
     None.
  
   PARAMETERS
    gas_id, event_id
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void gl1_qsh_event_notify(uint8 gas_id, gl1_qsh_dtf_event_id_enum_T event_id)
{

  qsh_client_event_notify_params_s  event_notify_params;
  byte evt_div_64 = event_id >> 6;
  byte evt_mod_64 = event_id & 0x3F;
  
  if(gl1_qsh_dtf_event_data[gas_id].gl1_qsh_dtf_event_id_mask[evt_div_64] & ((uint64)1 << evt_mod_64))
  {
    MSG_GERAN_HIGH_1_G(" QSH DEBUG: EVENT_NOTIFY %d ", event_id);
    if ((TRUE == set_event_id_to_failure[gas_id]) && gl1_qsh_check_failure_event(event_id,gas_id))
    {
      MSG_GERAN_HIGH_1_G(" QSH DEBUG: GL1_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE Failure event : %d ", event_id);
      event_id = GL1_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE;
    }
    qsh_client_event_notify_init(&event_notify_params);
    event_notify_params.client = QSH_CLT_GL1;
    event_notify_params.id = event_id;
    event_notify_params.event_data = (event_id==GL1_QSH_EVENT_EXTENDED_PAGE_REM_TIME)?((void *)rem_time_ms):NULL;
    event_notify_params.subs_id = geran_map_gas_id_to_nas_id ( gas_id );
    qsh_client_event_notify(&(event_notify_params));

    QSH_LOG(QSH_CLT_GL1,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"gs%d: GL1 QSH Event Notify %d",gas_id,event_id);
  }
  
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
}

/*===========================================================================
   FUNCTION     gl1_qsh_check_failure_event
  
   DESCRIPTION
     Check for GL1 failure event 
  
   DEPENDENCIES
     None.
  
   PARAMETERS
   event_id, gas_id
  
   RETURN VALUE
    boolean 
  
   SIDE EFFECTS
  
  ===========================================================================*/

boolean gl1_qsh_check_failure_event(gl1_qsh_dtf_event_id_enum_T event_id,uint8 gas_id)
{
  uint32 num_of_failure_events = (sizeof(gl1_qsh_failure_event_enums)/sizeof(uint32));
    while(num_of_failure_events--)
    {
      if(event_id ==  gl1_qsh_failure_event_enums[num_of_failure_events] )
      {
        MSG_GERAN_HIGH_1_G("QSH DEBUG failure event detected :  event id = %d",event_id);
        return TRUE;
      }
    }
  return FALSE;
}

#endif

/*===========================================================================
   FUNCTION     gl1_qsh_configure_event
  
   DESCRIPTION
     Populate the handler data as given by the QSH
  
   DEPENDENCIES
     None.
  
   PARAMETERS
   gas_id, event_cfg
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
#if defined ( FEATURE_QSH_EVENT_NOTIFY_TO_QSH ) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )

void gl1_qsh_configure_event_data( qsh_client_cfg_s cfg)
{

    qsh_event_action_e action = (byte)(cfg.cmd_code & 0x000000FF);
    byte evt = (byte)((cfg.cmd_code >> 8) & 0x000000FF);
    gas_id_t gas_id = geran_map_nas_id_to_gas_id ( (byte)((cfg.cmd_code >> 16) & 0x000000FF));
    byte evt_div_64 = evt >> 6;
    byte evt_mod_64 = evt & 0x3F;
    uint32 num_of_failure_events = (sizeof(gl1_qsh_failure_event_enums)/sizeof(uint32));
    if( action == QSH_EVENT_ACTION_ENABLE && evt == GL1_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE)
    {
     while(num_of_failure_events--)
      {
        evt_div_64 = gl1_qsh_failure_event_enums[num_of_failure_events] >> 6;
        evt_mod_64 = gl1_qsh_failure_event_enums[num_of_failure_events] & 0x3F;
        gl1_qsh_dtf_event_data[gas_id].gl1_qsh_dtf_event_id_mask[evt_div_64] |=  ((uint64)1 << evt_mod_64);
      }
     set_event_id_to_failure[gas_id] = TRUE;
    }
    else if ((action == QSH_EVENT_ACTION_DISABLE ) && (TRUE == set_event_id_to_failure[gas_id]) && (evt == GL1_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE) )
    {
      while(num_of_failure_events--)
      {
        evt_div_64 = gl1_qsh_failure_event_enums[num_of_failure_events] >> 6;
        evt_mod_64 = gl1_qsh_failure_event_enums[num_of_failure_events] & 0x3F;
        gl1_qsh_dtf_event_data[gas_id].gl1_qsh_dtf_event_id_mask[evt_div_64] &=  ~((uint64)1 << evt_mod_64); 
      }
      set_event_id_to_failure[gas_id] = FALSE;
    }
    else if( action == QSH_EVENT_ACTION_ENABLE )
    {
      gl1_qsh_dtf_event_data[gas_id].gl1_qsh_dtf_event_id_mask[evt_div_64] |=  ((uint64)1 << evt_mod_64);
    }
    else if ( action == QSH_EVENT_ACTION_DISABLE )
    {
      gl1_qsh_dtf_event_data[gas_id].gl1_qsh_dtf_event_id_mask[evt_div_64] &= ~((uint64)1 << evt_mod_64);
    }

#if 0
    /* TODO: Populate the command data */
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif
#endif
}
#endif

/*===========================================================================

FUNCTION  gfw_qsh_debug_callback

DESCRIPTION
  QSH CALLBACK for GFW DUMP/METRICS collection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gfw_qsh_debug_callback(qsh_client_cb_params_s *cb_param)
{
  ASSERT(cb_param != NULL);

  switch(cb_param->action)
  {
#ifdef FEATURE_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
      gfw_populate_qsh_dump_data(cb_param);
      break;
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
    case QSH_ACTION_METRIC_CFG:
    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      MSG_ERROR("GFW action %d not supported",cb_param->action,0,0);
      gfw_qsh_metric_callback(cb_param);
      break;
#endif

    default:
      break;
  }
} /* gfw_qsh_debug_callback */

#ifdef FEATURE_QSH_DUMP
/*===========================================================================

FUNCTION  gl1_populate_qsh_dump_data

DESCRIPTION
  GL1 to perform DUMP QSH Data.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_populate_qsh_dump_data(qsh_client_cb_params_s *param)
{
  qsh_client_action_done_s client_action_done;
  size_t total_size_written = 0;
  size_t remaining_size = param->action_params.dump_collect.dump_iovec.size_bytes;
  gl1_qsh_dump_tag_data_s   *dump_ptr;         //ptr to qsh dump loaction for GL1
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;

  dump_ptr = (gl1_qsh_dump_tag_data_s *)(param->action_params.dump_collect.dump_iovec.addr);
  //Storing GL1 dump ptr Addr for debug purposes
  qsh_dbg_dump_ptr = dump_ptr;

  if(QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask,GL1_QSH_DUMP_TAG_AS_ID_1))
  {
    if(remaining_size >= sizeof(gl1_qsh_dump_tag_data_s))
    {
      qsh_client_dump_tag_hdr_init(
         (qsh_dump_tag_hdr_s *)dump_ptr,
         (uint16)GL1_QSH_DUMP_TAG_AS_ID_1,
         sizeof(gl1_qsh_dump_tag_data_s) );

      gl1_get_qsh_dump_data(dump_ptr, GL1_QSH_DUMP_TAG_AS_ID_1);

      dump_ptr = dump_ptr + 1;
      remaining_size -= sizeof(gl1_qsh_dump_tag_data_s);
      total_size_written += sizeof(gl1_qsh_dump_tag_data_s);
      QSH_LOG(QSH_CLT_GL1, QSH_CAT_OTHER, QSH_MSG_TYPE_INFO, "GL1 qsh_cb written %u in SYNC mode [0]", total_size_written);
    }
  }

  if(QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask,GL1_QSH_DUMP_TAG_AS_ID_2))
  {
    if(remaining_size >= sizeof(gl1_qsh_dump_tag_data_s))
    {
      gas_id = GERAN_ACCESS_STRATUM_ID_2;

      qsh_client_dump_tag_hdr_init(
         (qsh_dump_tag_hdr_s *)dump_ptr,
         (uint16)GL1_QSH_DUMP_TAG_AS_ID_2,
         sizeof(gl1_qsh_dump_tag_data_s) );

      gl1_get_qsh_dump_data(dump_ptr,GL1_QSH_DUMP_TAG_AS_ID_2);
      dump_ptr = dump_ptr + 1;
      remaining_size -= sizeof(gl1_qsh_dump_tag_data_s);
      total_size_written += sizeof(gl1_qsh_dump_tag_data_s);
     
      QSH_LOG(QSH_CLT_GL1, QSH_CAT_OTHER, QSH_MSG_TYPE_INFO, "GL1 qsh_cb written %u in SYNC mode [1]", total_size_written);
    }
  }

  qsh_client_action_done_init(&client_action_done);
  client_action_done.cb_params_ptr = param;
  client_action_done.params.dump_collect.size_written_bytes = total_size_written;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
  qsh_client_action_done(&client_action_done);
} /* gl1_populate_qsh_dump_data */

/*===========================================================================

FUNCTION  gfw_populate_qsh_dump_data

DESCRIPTION
  GL! to act on behalf of GFW  to perform DUMP QSH Data.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gfw_populate_qsh_dump_data(qsh_client_cb_params_s *param)
{
  qsh_client_action_done_s client_action_done;
  gas_id_t gas_id;

  size_t total_size_written = 0;
  size_t remaining_size = param->action_params.dump_collect.dump_iovec.size_bytes;
  uint8 gl1_gfw_dump_data_result[2] = {0,0};

  gfw_qsh_dump_tag_data_s *gfw_dump_ptr1 = NULL;
  gfw_qsh_dump_tag_data_s *gfw_dump_ptr2 = NULL;

  gfw_qsh_dump_tag_data_s   *dump_ptr;         //ptr to qsh dump loaction for GFW

  dump_ptr = (gfw_qsh_dump_tag_data_s *)(param->action_params.dump_collect.dump_iovec.addr);


  if(QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask,GFW_QSH_DUMP_TAG_0))
  {
    if(remaining_size >= sizeof(gfw_qsh_dump_tag_data_s))
    {
      qsh_client_dump_tag_hdr_init(
        (qsh_dump_tag_hdr_s *)dump_ptr,
        (uint16)GFW_QSH_DUMP_TAG_0,
        sizeof(gfw_qsh_dump_tag_data_s) );

      gfw_dump_ptr1  = dump_ptr;
      dump_ptr = dump_ptr + 1;
      remaining_size -= sizeof(gfw_qsh_dump_tag_data_s);
      total_size_written += sizeof(gfw_qsh_dump_tag_data_s);
    }
  }

  if(QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask,GFW_QSH_DUMP_TAG_1))
  {
    if(remaining_size >= sizeof(gfw_qsh_dump_tag_data_s))
    {
      qsh_client_dump_tag_hdr_init(
        (qsh_dump_tag_hdr_s *)dump_ptr,
        (uint16)GFW_QSH_DUMP_TAG_1,
        sizeof(gfw_qsh_dump_tag_data_s) );

      gfw_dump_ptr2  = dump_ptr;
      dump_ptr = dump_ptr + 1;
      remaining_size -= sizeof(gfw_qsh_dump_tag_data_s);
      total_size_written += sizeof(gfw_qsh_dump_tag_data_s);
    }
  }

  gas_id =  GERAN_ACCESS_STRATUM_ID_1;
  gfw_get_qsh_dump_data(gfw_dump_ptr1,gfw_dump_ptr2,&gl1_gfw_dump_data_result[0]);
  MSG_GERAN_HIGH_3_G ("GL1 gfw_get_qsh_dump_data size_written %d [Tag_0] %d [Tag_1] %d",
                      total_size_written,gl1_gfw_dump_data_result[0],gl1_gfw_dump_data_result[1]);

  qsh_client_action_done_init(&client_action_done);
  client_action_done.cb_params_ptr = param;
  client_action_done.params.dump_collect.size_written_bytes =  total_size_written;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
  qsh_client_action_done(&client_action_done);
}  /* gfw_populate_qsh_dump_data */


/*===========================================================================
Function: gl1_get_qsh_dump_data()

Description:
GL1 getting QSH dump data for for Dump collection action.
It initialises qsh_dump_tag_hdr_s
============================================================================*/
void gl1_get_qsh_dump_data
(
  gl1_qsh_dump_tag_data_s *dump_data_ptr,
  gl1_qsh_dump_tag_e      tag
)
{
  gas_id_t gas_id  = GERAN_ACCESS_STRATUM_ID_1;

  if(tag == GL1_QSH_DUMP_TAG_AS_ID_2)
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_2;
  }
  gl1_qsh_dump_data_get(&(dump_data_ptr->gl1_dump_data),gas_id);
}  /* gl1_get_qsh_dump_data */


/*===========================================================================
Function: gfw_get_qsh_dump_data()

Description:
GFW getting QSH dump data for for Dump collection action.
It initialises qsh_dump_tag_hdr_s
============================================================================*/
void gfw_get_qsh_dump_data
(
  gfw_qsh_dump_tag_data_s *dump_data_ptr1,
  gfw_qsh_dump_tag_data_s *dump_data_ptr2,
  uint8                   *gfw_result

)
{
  gfw_qsh_dump_s *gfw_qsh_dump_s_ptr1 = NULL;
  gfw_qsh_dump_s *gfw_qsh_dump_s_ptr2 = NULL;

  if (dump_data_ptr1 != NULL )
  {
     gfw_qsh_dump_s_ptr1 = &(dump_data_ptr1->gfw_dump_data);
  }

  if (dump_data_ptr2 != NULL )
  {
     gfw_qsh_dump_s_ptr2 = &(dump_data_ptr2->gfw_dump_data);
  }

  gfw_qsh_dump_data_get(gfw_qsh_dump_s_ptr1,gfw_qsh_dump_s_ptr2,gfw_result);

}  /* gfw_get_qsh_dump_data */

/*===========================================================================
FUNCTION gl1_state_snapshot

DESCRIPTION
  QSH support : Function to populate QSH reports with L1 state data

RETURN VALUE
  None
===========================================================================*/
void  gl1_state_snapshot( gl1_dump_tag_data_s *pBuf, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *pIn = &l1_tsk_buffer[gas_id];
  l1_NVLog_struct_T *l1_nv_log_ptr=&l1_nv_log[gas_id];
  uint8             l1_state_log_ix = l1_nv_log_ptr->l1_state_log_buf_index;
  uint8             i;

  pBuf->gstmr_FN       = gl1_get_FN(gas_id);

  pBuf->main_command   = pIn->main_command;
  pBuf->command        = pIn->command;
  pBuf->l1_state       = pIn->l1_state;
  pBuf->sub_state      = pIn->sub_state;

  pBuf->sub_sub_state  = pIn->sub_sub_state;
  pBuf->pscan_state    = pIn->pscan_state;
  pBuf->acq_bcch_state = pIn->acq_bcch_state;
  pBuf->trm_client_id  = pIn->client_id;

#if defined (FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

  pBuf->l1_prev_state                 = pIn->l1_prev_state;
  pBuf->multi_sim_pscan_state         = pIn->multi_sim_pscan_state;
  pBuf->multi_sim_acq_bcch_list_state = pIn->multi_sim_acq_bcch_list_state;
  pBuf->multi_sim_acquiring_state     = pIn->multi_sim_acquiring_state;

  pBuf->multi_sim_find_sch_state      = pIn->multi_sim_find_sch_state;
  pBuf->multi_sim_reselection_state   = pIn->multi_sim_reselection_state;
  pBuf->multi_sim_decoupled_state     = pIn->multi_sim_decoupled_state;
  pBuf->firmware_loaded               = pIn->firmware_loaded;

  pBuf->first_multi_sim_init          = pIn->first_multi_sim_init;
  pBuf->firmware_load_status          = pIn->firmware_load_status;
  pBuf->firmware_unload_in_progress   = pIn->firmware_unload_in_progress;
  pBuf->going_active                  = pIn->going_active;

  pBuf->qta_in_progress               = pIn->qta_in_progress;
  pBuf->reservation_for_bcch          = pIn->reservation_for_bcch;
  pBuf->rf_in_deep_sleep_whilst_forced_wakeup = pIn->rf_in_deep_sleep_whilst_forced_wakeup;
  pBuf->g2x_resel_in_progress         = pIn->g2x_resel_in_progress;
#endif 
  
  /// Fill the state history 
  if ( l1_state_log_ix == 0)
  {
    l1_state_log_ix = L1_STATE_LOG_LEN -1;
  }
  else
  {
    l1_state_log_ix--;
  }

  for (i = 0; i < GL1_QSH_STATE_LOG_LEN ; i++)
  {
    pBuf->gl1_state_log_buf[i].FN = l1_nv_log_ptr->l1_state_log_buf[l1_state_log_ix].FN;
    pBuf->gl1_state_log_buf[i].state =  l1_nv_log_ptr->l1_state_log_buf[l1_state_log_ix].state;
    pBuf->gl1_state_log_buf[i].sub_state =  l1_nv_log_ptr->l1_state_log_buf[l1_state_log_ix].sub_state;
    pBuf->gl1_state_log_buf[i].main_command = l1_nv_log_ptr->l1_state_log_buf[l1_state_log_ix].main_command;
    pBuf->gl1_state_log_buf[i].command = l1_nv_log_ptr->l1_state_log_buf[l1_state_log_ix].command;

   if (l1_state_log_ix == 0)
   {
      l1_state_log_ix = L1_STATE_LOG_LEN;
   }

   l1_state_log_ix -- ;
  }
} 


/*===========================================================================
FUNCTION gl1_msg_snapshot

DESCRIPTION
  QSH support : Function to populate QSH reports with L1 msg trace

RETURN VALUE
  None
===========================================================================*/
void  gl1_msg_snapshot( gl1_dump_tag_data_s *pBuf, l1_msg_traffic_log_T *pLog, uint16 msgIndex )
{

  l1_msg_traffic_log_T      *pMsg;
  int i;
  l1_msg_traffic_log_T      *pTop    = &pLog[0];

  l1_msg_traffic_log_T      *pEnd    = &pLog[L1_MSG_LOG_LEN-1];

  gl1_qsh_msg_traffic_log_t *pBufLog = &pBuf->gl1_log_msg[0];

/// Fill the state history 
   if ( msgIndex == 0)
   {
     msgIndex = L1_MSG_LOG_LEN -1;
   }
   else
   {
     msgIndex--;
   }

  pMsg    = &pLog[msgIndex];

  /* copy the last GL1_QSH_MSG_LOG_LEN messages filtering xx */
  for(i =0 ; i < GL1_QSH_MSG_LOG_LEN; i++)
  {
    pBufLog->FN      = pMsg->FN;
    pBufLog->msg_set = pMsg->msg_set;
    pBufLog->msg_ID  = pMsg->msg_ID;

    pMsg--;
    pBufLog++;

    if(pMsg < pTop)
    {
      /* Wrap around */
      pMsg = pEnd;
    }
  }
} /* gl1_msg_snapshot */


/*===========================================================================
FUNCTION gl1_irat_snapshot

DESCRIPTION
  QSH support : Function to populate QSH reports with IRAT state info

RETURN VALUE
  None
===========================================================================*/
void gl1_irat_snapshop(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t* l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   * l1_sc_globals_ptr    = &l1_sc_globals_store[gas_id];

  pBuf->gsm_only = l1_sc_wcdma_data_ptr->rr_params.gsm_only;
  pBuf->sce_mode = l1_sc_globals_ptr->mode;            // This is not IRAT related. Move ?
  pBuf->sce_running = l1_sc_is_sce_running(gas_id);            // This is not IRAT related. Move ?
  pBuf->G2L_INIT_sent = 0; // Incorrect...need change.

  pBuf->receive_in_progress = l1_sc_globals_ptr->receive_in_progress;        // This is not IRAT related. Move ?
  pBuf->monscan_in_progress = l1_sc_globals_ptr->monscan_in_progress;        // This is not IRAT related. Move ?
  pBuf->wcdma_id_srch_in_progress = l1_sc_globals_ptr->wcdma_srch_in_progress;

  pBuf->wcdma_srch_abort = l1_sc_wcdma_data_ptr->wcdma_srch.abort;
  pBuf->wcdma_srch_state = l1_sc_wcdma_data_ptr->wcdma_srch.state;
  pBuf->wcdma_irat_srch_state = l1_sc_wcdma_data_ptr->wcdma_srch.srch_state;
  pBuf->wcdma_irat_srch_type = l1_sc_wcdma_data_ptr->wcdma_srch.srch_type;

  pBuf->num_freqs = l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  pBuf->num_W_freqs = l1_sc_wcdma_data_ptr->rr_params.num_W_freqs;

  pBuf->num_L_freqs = l1_sc_wcdma_data_ptr->rr_params.num_L_freqs;
  pBuf->active_irat = l1_sc_globals_ptr->active_irat;

  pBuf->cm_initialised = mdsp_cm_active(gas_id);
  pBuf->mdsp_awake_flag = mdsp_awake(gas_id);
#ifdef FEATURE_QTA
  pBuf->gl1_qta_fw_rf_initialised = gl1_hw_get_qta_cm_fw_rf_init_status(gas_id);

  pBuf->gl1_qta_wcdma_has_initialised = gl1_hw_cm_get_qta_wcdma_has_initialised(gas_id);
#endif
}

/*===========================================================================
FUNCTION gl1_mdsp_snapshot

DESCRIPTION
  QSH support : Function to populate QSH reports with MDSP cmd trace info

RETURN VALUE
  None
===========================================================================*/
void gl1_mdsp_snapshot(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id)
{
  mdsp_get_log_cmd_entry(pBuf, GL1_QSH_MDSP_LOG_LEN, gas_id);
}

/*===========================================================================
FUNCTION gl1_sleep_snapshot

DESCRIPTION
  QSH support : Function to populate QSH reports with debug info on the last
  wake-up.

RETURN VALUE
  None
===========================================================================*/
void  gl1_sleep_snapshot(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id)
{
  gl1_hw_sleep_type* entry_p = gl1_hw_get_last_sleep_debug_entry(gas_id);

  pBuf->gl1_hw_sleep_buf.async_msg.async_exec = entry_p->async_msg.async_exec;
  pBuf->gl1_hw_sleep_buf.async_msg.async_NV_poss = entry_p->async_msg.async_NV_poss;
  pBuf->gl1_hw_sleep_buf.async_msg.async_option3_NV_poss  = entry_p->async_msg.async_option3_NV_poss;
  pBuf->gl1_hw_sleep_buf.async_msg.async_poss = entry_p->async_msg.async_poss;
  pBuf->gl1_hw_sleep_buf.async_msg.async_sleep = entry_p->async_msg.async_sleep;
  pBuf->gl1_hw_sleep_buf.async_msg.frame_number = entry_p->async_msg.frame_number;
  pBuf->gl1_hw_sleep_buf.async_msg.ignore_next_gstmr = entry_p->async_msg.ignore_next_gstmr;
  pBuf->gl1_hw_sleep_buf.async_msg.int_pend = entry_p->async_msg.int_pend;
  pBuf->gl1_hw_sleep_buf.async_msg.irat_active = entry_p->async_msg.irat_active;
  pBuf->gl1_hw_sleep_buf.async_msg.precheck  = entry_p->async_msg.precheck;
  pBuf->gl1_hw_sleep_buf.async_msg.saic_status = entry_p->async_msg.saic_status;
  pBuf->gl1_hw_sleep_buf.async_msg.start_qsc = entry_p->async_msg.start_qsc;
  pBuf->gl1_hw_sleep_buf.async_msg.start_ustmr = (int32)entry_p->async_msg.start_ustmr;
  pBuf->gl1_hw_sleep_buf.async_msg.tx_on = entry_p->async_msg.tx_on;

  pBuf->gl1_hw_sleep_buf.forced_wakeup = entry_p->forced_wakeup;
  pBuf->gl1_hw_sleep_buf.sleep_info.adaptive_warmup_us = entry_p->sleep_info.adaptive_warmup_us;
  pBuf->gl1_hw_sleep_buf.sleep_info.awake_duration_us = (int32)entry_p->sleep_info.awake_duration_us;
  pBuf->gl1_hw_sleep_buf.sleep_info.commit_gsmclks = entry_p->sleep_info.commit_gsmclks;
  pBuf->gl1_hw_sleep_buf.sleep_info.diag_f3_trace_disabled = entry_p->sleep_info.diag_f3_trace_disabled;
  pBuf->gl1_hw_sleep_buf.sleep_info.diag_mask_disabled = entry_p->sleep_info.diag_mask_disabled;
  pBuf->gl1_hw_sleep_buf.sleep_info.egts_offset_gsm_clks = entry_p->sleep_info.egts_offset_gsm_clks;
  pBuf->gl1_hw_sleep_buf.sleep_info.gsm_clk_sleep_duration = (uint32)entry_p->sleep_info.gsm_clk_sleep_duration;
  pBuf->gl1_hw_sleep_buf.sleep_info.gsm_clk_sleep_duration_frames = entry_p->sleep_info.gsm_clk_sleep_duration_frames;
  pBuf->gl1_hw_sleep_buf.sleep_info.late_ols_offset_usec = entry_p->sleep_info.late_ols_offset_usec;
  pBuf->gl1_hw_sleep_buf.sleep_info.low_monitor_mode = entry_p->sleep_info.low_monitor_mode;

  pBuf->gl1_hw_sleep_buf.sleep_info.msim_sys_mode = entry_p->sleep_info.msim_sys_mode;
  pBuf->gl1_hw_sleep_buf.sleep_info.parallel_warmup = entry_p->sleep_info.parallel_warmup;
  pBuf->gl1_hw_sleep_buf.sleep_info.returned_rf_warmup_usec = entry_p->sleep_info.returned_rf_warmup_usec;
  pBuf->gl1_hw_sleep_buf.sleep_info.rf_wakeup_to_vstmr_us = (int32)entry_p->sleep_info.rf_wakeup_to_vstmr_us;
  pBuf->gl1_hw_sleep_buf.sleep_info.scheduled = entry_p->sleep_info.scheduled;
  pBuf->gl1_hw_sleep_buf.sleep_info.used_warmup_usec = entry_p->sleep_info.used_warmup_usec;
  pBuf->gl1_hw_sleep_buf.sleep_info.warmup_end_to_gstmr_us = (int32)entry_p->sleep_info.warmup_end_to_gstmr_us;

}

/*===========================================================================
FUNCTION gl1_paging_snapshot

DESCRIPTION
  QSH support : Function to populate QSH reports with paging information

RETURN VALUE
  None
===========================================================================*/
void  gl1_paging_snapshot(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id)
{
  idle_data_T *idle_data_ptr = l1_tsk_buffer[gas_id].current_params.L1Data.pIdle_data;

  if (idle_data_ptr != NULL)
  {
    // Paging Params
    pBuf->gl1_paging_data_buf.All_Paging_Blocks = idle_data_ptr->paging_data.all_paging_blocks;
    pBuf->gl1_paging_data_buf.CCCH_Group = idle_data_ptr->paging_data.ccch_group;
    pBuf->gl1_paging_data_buf.Paging_Block_Index = idle_data_ptr->paging_data.paging_block_index;
    pBuf->gl1_paging_data_buf.Paging_Data_Valid = idle_data_ptr->paging_data.paging_data_valid;
    pBuf->gl1_paging_data_buf.Paging_Group = idle_data_ptr->paging_data.paging_group;
    pBuf->gl1_paging_data_buf.Paging_Mode = idle_data_ptr->paging_data.paging_mode;
    pBuf->gl1_paging_data_buf.Pwr_Av_Freq = idle_data_ptr->paging_data.pwr_av_freq;
  }
  else
  {
    QSH_LOG(QSH_CLT_GL1, QSH_CAT_OTHER, QSH_MSG_TYPE_INFO, " paging_snapshot NULL PTR, not camped");
  }
}

/*===========================================================================
FUNCTION gl1_transfer_snapshot

DESCRIPTION
  QSH support : Function to populate QSH reports with transfer information

RETURN VALUE
  None
===========================================================================*/
void  gl1_transfer_snapshot(gl1_dump_tag_data_s *pBuf, gas_id_t gas_id)
{
  transfer_data_T* transfer_data_ptr = l1_tsk_buffer[gas_id].current_params.L1Data.pTransfer_data;

  if (transfer_data_ptr != NULL)
  {
    pBuf->gl1_transfer_buf.abort_fn = transfer_data_ptr->abort_fn;
    pBuf->gl1_transfer_buf.confirm_pending = transfer_data_ptr->confirm_pending;
    pBuf->gl1_transfer_buf.current_alloc_type = transfer_data_ptr->current_alloc_type;
    pBuf->gl1_transfer_buf.dl_st = transfer_data_ptr->dl_st;
    pBuf->gl1_transfer_buf.egprs_or_gprs_tbf = transfer_data_ptr->egprs_or_gprs_tbf;
    pBuf->gl1_transfer_buf.mac_mode = transfer_data_ptr->mac_mode;
    pBuf->gl1_transfer_buf.mac_ul_ctrl_available = transfer_data_ptr->mac_ul_ctrl_available;
    pBuf->gl1_transfer_buf.release_in_progress = transfer_data_ptr->release_in_progress;
    pBuf->gl1_transfer_buf.release_pending = transfer_data_ptr->release_pending;
    pBuf->gl1_transfer_buf.rlc_mode = transfer_data_ptr->rlc_mode;
    pBuf->gl1_transfer_buf.ul_st = transfer_data_ptr->ul_st;
    pBuf->gl1_transfer_buf.wait_for_pca_fn = transfer_data_ptr->wait_for_pca_fn;
 }
 else
 {
   MSG_GERAN_ERROR_0_G(" transfer_snapshot NULL PTR, transfer is INACTIVE");
 }
}

/*===========================================================================

FUNCTION  gl1_qsh_dump_data_get

DESCRIPTION
  Populate GL1 Data dump for QSH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gl1_qsh_dump_data_get( gl1_dump_tag_data_s *pBuf,  gas_id_t gas_id)  // gl1_qsh_dump_tag_data_s
{
  {
    gl1_state_snapshot(pBuf, gas_id);
  }

  {
    l1_msg_traffic_log_T *pMsg=&l1_nv_log[gas_id].l1_log_msg[0];
    uint16 index = l1_nv_log[gas_id].l1_log_msg_index;
    gl1_msg_snapshot( pBuf, pMsg, index);
  }

  {
    gl1_irat_snapshop( pBuf, gas_id);
  }

 
  {
    gl1_mdsp_snapshot(pBuf, gas_id);
  }

  {
    gl1_sleep_snapshot(pBuf, gas_id);
  }

  {
    gl1_paging_snapshot(pBuf, gas_id);
  }

  {
    gl1_transfer_snapshot((gl1_dump_tag_data_s*)pBuf, gas_id);
  }

  
}

/*===========================================================================

FUNCTION  gfw_qsh_dump_data_get

DESCRIPTION
  Populate GL1 Data dump for QSH , send dump address via IPC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gfw_qsh_dump_data_get(gfw_qsh_dump_s *data_ptr1, gfw_qsh_dump_s *data_ptr2, uint8  *gfw_result)
{
   gl1_msg_qsh_get_gfw_data_dump((uint8*)data_ptr1,(uint8*)data_ptr2,gfw_result);

} /* gfw_qsh_dump_data_get */

#endif /* FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_EVENT_METRIC

/*===========================================================================

FUNCTION  gl1_process_qsh_metric_ind

DESCRIPTION
  handle L1_QSH_METRIC_IND

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_process_qsh_metric_ind
(
  l1_qsh_metric_ind_T *gl1_qsh_ind,
  gas_id_t             gas_id
)
{
  switch(gl1_qsh_ind->cb_params.action)
  {
    case QSH_ACTION_METRIC_CFG:
      gl1_qsh_process_metrics_cfg(&gl1_qsh_ind->cb_params);
      break;

    case QSH_ACTION_METRIC_TIMER_EXPIRY:
     gl1_qsh_process_metrics_timer_expiry(&gl1_qsh_ind->cb_params);
     break;

    default :
      break;
  }
} /* gl1_process_qsh_metric_ind */

/*===========================================================================

FUNCTION  void gpl1_call_in_task_qsh_metric_ind

DESCRIPTION
  Process  metrics callback from QSH  and post in TASK.

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS

===========================================================================*/
void gl1_call_in_task_qsh_metric_ind(qsh_client_cb_params_s *cb_params)
{
  l1_qsh_metric_ind_T msg;

  msg.header.message_set = MS_L1_L1;
  msg.header.message_id  = L1_QSH_METRIC_IND;
  msg.gas_id             = GERAN_ACCESS_STRATUM_ID_1; // not known at this time.
  msg.cb_params          = (*cb_params);
  PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

  ( void )L1_isr_send_message( l1_queues[GERAN_ACCESS_STRATUM_ID_1], &msg, TRUE, GERAN_ACCESS_STRATUM_ID_1 );
} /* gl1_call_in_task_qsh_metric_ind */

/*===========================================================================

FUNCTION  gl1_qsh_process_metrics_timer_expiry

DESCRIPTION
   Process l1-QSH_METRIC at the time of  QSH timer Expiry

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_qsh_process_metrics_timer_expiry(qsh_client_cb_params_s *param)
{
  qsh_client_metric_log_done_s client_metric_log_done;
  qsh_metric_id_t metric_id;
  gas_id_t gas_id;
  uint8 i;
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk;

  for(i = 0; i < param->action_params.metric_timer_expiry.metric_id_count; i++)
  {
      metric_id = param->action_params.metric_timer_expiry.params[i].metric_id;
      gas_id    = geran_map_nas_id_to_gas_id(param->action_params.metric_timer_expiry.params[i].subs_id);
      l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    if ((l1_tskisr_blk->l1_state != L1_INACTIVE_MODE))
    {
      /*If Start Addr is not NULL and GL1 is not in Inactive Mode*/
      if ((metric_id < GL1_QSH_METRIC_MAX ) && 
          (gl1_qsh_metric_buffer[gas_id][metric_id].start_addr != NULL)
         )
      {
        /*Only If Metric if Valid in this state, populate QSH Metric else Ignore*/
        if(gl1_qsh_get_metric_data((gl1_qsh_metric_e)metric_id,gas_id))
        {
          qsh_client_metric_log_done_init(&client_metric_log_done);
          client_metric_log_done.client = QSH_CLT_GL1;
          client_metric_log_done.metric_id = (qsh_metric_id_t)metric_id;
          client_metric_log_done.metric_context_id = gl1_qsh_metric_buffer[gas_id][metric_id].metric_context_id;
          client_metric_log_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL;
          gl1_qsh_metric_buffer[gas_id][metric_id].start_addr = (uint8 *)qsh_client_metric_log_done(&client_metric_log_done);
        }/*If Get metric*/
      }
      else
      {
        MSG_GERAN_ERROR_1_G("GL1_QSH: start_addr NULL or Metric ID:%d Invalid", metric_id);
    }
    }/*If GL1 isnt INACTIVE*/
  }/*For Loop*/
} /* gl1_qsh_process_metrics_timer_expiry */



/*===========================================================================

FUNCTION  gl1_qsh_process_metrics_cfg

DESCRIPTION
  handle L1_QSH_METRIC_IND

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_qsh_process_metrics_cfg(qsh_client_cb_params_s *param)
{
  qsh_client_action_done_s client_action_done;
  qsh_client_metric_cfg_s *metrics_cfg = &(param->action_params.metric_cfg);

  gl1_qsh_metric_e metric_id = (gl1_qsh_metric_e)metrics_cfg->id;
  gas_id_t gas_id =  geran_map_nas_id_to_gas_id(metrics_cfg->subs_id);

  MSG_GERAN_MED_1_G("GL1 qsh_process_metrics_cfg metric %d ",metric_id);

  switch(metric_id)
  {
    case GL1_QSH_METRIC_RSSI:
    case GL1_QSH_METRIC_RACH_TX_PWR:
    case GL1_QSH_METRIC_TCH_RX_PARAMS:
    {
      gl1_qsh_metric_buffer[gas_id][metric_id].metric_context_id = metrics_cfg->metric_context_id;
      gl1_qsh_metric_buffer[gas_id][metric_id].gas_id = gas_id;

      if(metrics_cfg->action == QSH_METRIC_ACTION_START)
      {
        gl1_qsh_metric_buffer[gas_id][metric_id].start_addr = metrics_cfg->start_addr;
      }
      else
      {
        gl1_qsh_metric_buffer[gas_id][metric_id].start_addr = NULL;
      }
    }
     break;

    default:
     break;
  }

  qsh_client_action_done_init(&client_action_done);
  client_action_done.cb_params_ptr = param;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
  qsh_client_action_done(&client_action_done);
}  /* gl1_qsh_process_metrics_cfg */


/*===========================================================================

FUNCTION  gl1_qsh_get_metric_data

DESCRIPTION
   Populate QSH Metrics Data.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if Metric values are valid, else FALSE
  - If defined FEATURE_CUST_1 , 
    always return TRUE, 
    to preserve behaviour, until requested to alter

SIDE EFFECTS
  None

===========================================================================*/
static boolean gl1_qsh_get_metric_data(gl1_qsh_metric_e metric_idx,gas_id_t gas_id)
{
    uint8 metric_id = (uint8)metric_idx;
    int32 metric_value;
#ifndef FEATURE_CUST_1
    boolean ret_val = TRUE; /*Initialize return value to valid*/
    volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

 
    /*If in Inactive State and not initialized at all then return False*/
    if ((l1_tskisr_blk->l1_state == L1_INACTIVE_MODE)
        && (l1_tskisr_blk->l1_prev_state == L1_INACTIVE_MODE))
    {
       ret_val = FALSE;
       return ret_val;
    }
#endif

    switch (metric_id)
    {
    case GL1_QSH_METRIC_RSSI :
     metric_value = gpl1_get_rx_pwr_avg_dbm10_coex(gas_id);
     ((gl1_qsh_metric_rssi_s*)(gl1_qsh_metric_buffer[gas_id][metric_id].start_addr))-> avg_rssi = metric_value;  
      metric_value = ((gl1_qsh_metric_rssi_s*)(gl1_qsh_metric_buffer[gas_id][metric_id].start_addr))-> avg_rssi;
      break;

    case GL1_QSH_METRIC_RACH_TX_PWR :
      metric_value = gpl1_get_avg_tx_slot_pwr_db_10(gas_id);
      /*If the Metric retrieved is Zero, Then it might have been reset on State Change*/
      if (metric_value == 0)
      {
        metric_value = gl1_qsh_get_tx_metric_on_reset(gas_id);
      }
      /*Reset The Tx metric Copy On each Data Write*/
      gl1_qsh_write_tx_metric_on_reset(0, gas_id);

#ifndef FEATURE_CUST_1
      /*If metric is still Zero and in Non Tx mode then return False*/
      /*2nd condition always TRUE now cause of OR*/
      if ((metric_value == 0)
          && ((l1_tskisr_blk->l1_state != L1_DEDICATED_MODE)
              ||(l1_tskisr_blk->l1_state != L1_TRANSFER_MODE)
              ||(l1_tskisr_blk->l1_state != L1_DTM_MODE)
              ||(l1_tskisr_blk->l1_state != L1_RANDOM_ACCESS_MODE)
              ||(l1_tskisr_blk->l1_state != L1_PACKET_ACCESS_MODE)))
      {
          ret_val = FALSE;
      }
      else
      {
         ((gl1_qsh_metric_rach_tx_pwr_s *)(gl1_qsh_metric_buffer[gas_id][metric_id].start_addr))->rach_tx_pwr = metric_value; 
      }
#else      
      ((gl1_qsh_metric_rach_tx_pwr_s *)(gl1_qsh_metric_buffer[gas_id][metric_id].start_addr))->rach_tx_pwr = metric_value; 
#endif      
      break;

    case GL1_QSH_METRIC_TCH_RX_PARAMS:
      gl1_qsh_write_tch_rx_params (
                      (gl1_qsh_metric_tch_rx_params_s*)(gl1_qsh_metric_buffer[gas_id][metric_id].start_addr), 
                      gas_id);
      break;

    default:
#ifndef FEATURE_CUST_1
      MSG_GERAN_ERROR_1_G("GL1_QSH: Invalid Metric Id %d", metric_id);
      ret_val = FALSE;
#endif
      break;
  }

#ifndef FEATURE_CUST_1 
  return ret_val;
#else
  return TRUE;
#endif
}

/*===========================================================================

FUNCTION  gfw_qsh_metric_callback

DESCRIPTION
  QSH METRIC for GFW DUMP/METRICS collection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gfw_qsh_metric_callback(qsh_client_cb_params_s *cb_param)
{
  qsh_client_action_done_s cb_done;

  qsh_client_action_done_init(&cb_done);
  cb_done.cb_params_ptr = cb_param;
  cb_done.params.dump_collect.size_written_bytes = 0;
  cb_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;  // Not ASYNC as this is an ERROR CASE.
  qsh_client_action_done(&cb_done);
} /* gfw_qsh_metric_callback */

/*===========================================================================

FUNCTION  gl1_qsh_cache_tch_rx_params

DESCRIPTION
  Function To Cache required metric Info at right time for QSH

DEPENDENCIES
  Applicable only in Dedicated Mode, Will Cache data before sending MPH_SERVING_DED_MEAS_IND

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_qsh_cache_tch_rx_params (int32 pri_rxlev_full, int32 pri_rxlev_sub,
                                  int32 sec_rxlev_full, int32 sec_rxlev_sub,
                                  uint8 rxqual_full, uint8 rxqual_sub,
                                  gas_id_t gas_id)
{
  
  gl1_qsh_tch_rx_params_cache[gas_id].tch_pri_rxlev_full = pri_rxlev_full;
  gl1_qsh_tch_rx_params_cache[gas_id].tch_pri_rxlev_sub  = pri_rxlev_sub;
  gl1_qsh_tch_rx_params_cache[gas_id].tch_sec_rxlev_full = sec_rxlev_full;
  gl1_qsh_tch_rx_params_cache[gas_id].tch_sec_rxlev_sub  = sec_rxlev_sub;
  gl1_qsh_tch_rx_params_cache[gas_id].tch_rxqual_full    = rxqual_full;
  gl1_qsh_tch_rx_params_cache[gas_id].tch_rxqual_sub     = rxqual_sub;

}

/*===========================================================================

FUNCTION  gl1_qsh_write_tch_rx_params

DESCRIPTION
  Writes the Values from the Cache to the Address provided

DEPENDENCIES
  Applicable only in Dedicated Mode, Will Cache data before sending MPH_SERVING_DED_MEAS_IND

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_qsh_write_tch_rx_params (gl1_qsh_metric_tch_rx_params_s* qsh_ptr, gas_id_t gas_id)
{
    
    qsh_ptr->tch_pri_rxlev_full = gl1_qsh_tch_rx_params_cache[gas_id].tch_pri_rxlev_full;
    qsh_ptr->tch_pri_rxlev_sub  = gl1_qsh_tch_rx_params_cache[gas_id].tch_pri_rxlev_sub;
    qsh_ptr->tch_sec_rxlev_full = gl1_qsh_tch_rx_params_cache[gas_id].tch_sec_rxlev_full;
    qsh_ptr->tch_sec_rxlev_sub  = gl1_qsh_tch_rx_params_cache[gas_id].tch_sec_rxlev_sub;
    qsh_ptr->tch_rxqual_full    = gl1_qsh_tch_rx_params_cache[gas_id].tch_rxqual_full;
    qsh_ptr->tch_rxqual_sub     = gl1_qsh_tch_rx_params_cache[gas_id].tch_rxqual_sub;

}

/*===========================================================================

FUNCTION  gl1_qsh_get_tx_metric_on_reset

DESCRIPTION
  Gets the Value of Tx Metric before value reset
  This is to cater the scenario when the metric timer expires after L1 changes state
  hence resetting the gpl1_tx_pwr structure
  On Next metric expiry L1 has to send the last valid Tx power instead
 
DEPENDENCIES
  

RETURN VALUE
  Last Available Tx metric 

SIDE EFFECTS
  None

===========================================================================*/
static int32 gl1_qsh_get_tx_metric_on_reset(gas_id_t gas_id)
{
  return gl1_qsh_tx_metric_copy_on_reset[gas_id];
}

/*===========================================================================

FUNCTION  gl1_qsh_write_tx_metric_on_reset

DESCRIPTION
  Writes the Value of Tx Metric to the static var

DEPENDENCIES
  

RETURN VALUE
  None 

SIDE EFFECTS
  None

===========================================================================*/
void gl1_qsh_write_tx_metric_on_reset(int32 metric, gas_id_t gas_id)
{
  MSG_GERAN_LOW_1_G("GL1_QSH: Copying Tx Metric:%d", metric);
  gl1_qsh_tx_metric_copy_on_reset[gas_id] = metric;

}
#endif /* FEATURE_QSH_EVENT_METRIC */


#endif /* FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_DUMP */

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH

/*===========================================================================

FUNCTION  GL1_GET_REM_TIME_IN_MS

DESCRIPTION
  This function converts the remaining Sclks to ms..

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
int64 gl1_get_rem_time_in_ms(uint32 rem_sclk)
{
  int64 rem_time;

  /* Compute time left in ms by converting from sclk ticks */
  rem_time = (int64)rem_sclk - (int64)timetick_get();
  rem_time = (rem_time < 0)? (rem_time + 0xFFFFFFFF):rem_time;
  rem_time = ((rem_time * 1000)/TIMETICK_NOMINAL_FREQ_HZ);

 // MSG_GERAN_HIGH_2_G("DSDS:rem_time_in_ms %d,  current_clk 0x%x", rem_time, (int64)timetick_get());

  return rem_time;
}
/*===========================================================================

FUNCTION gl1_qsh_notify_state_transition

DESCRIPTION
  This function is to notify state transition events to QSH   

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void gl1_qsh_notify_state_transition(gas_id_t gas_id ,uint16 newState)
{
switch (newState)
  {
    case L1_INACTIVE_MODE:
    case L1_MULTI_SIM_INACTIVE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_INACTIVE_STATE_TRANSITION);
    break;
    case L1_NULL_MODE:
    case L1_MULTI_SIM_NULL:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_NULL_STATE_TRANSITION);
    break;
    case L1_SUSPEND_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_SUSPEND_STATE_TRANSITION);
    break;
    case L1_RESET_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_RESET_STATE_TRANSITION);
    break;
    case L1_WTOG_BPLMN_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_WTOG_BPLMN_STATE_TRANSITION);
    break;  
    /* ACQ STATE(S) */
    case L1_FIND_BCCH:
    case L1_MULTI_SIM_FIND_BCCH:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_FIND_BCCH_STATE_TRANSITION);
    break;
    case L1_FIND_SCH:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_FIND_SCH_STATE_TRANSITION);
    break;
    
    /* IDLE STATE(S) */
    case L1_IDLE_MODE:
    case L1_MULTI_SIM_IDLE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_IDLE_STATE_TRANSITION);
    break;
    case L1_GPRS_IDLE_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_GPRS_IDLE_STATE_TRANSITION);
    break;
    /* IDLE STATE(S) */
	
    /* ACCESS STATE(S) */
    case L1_RANDOM_ACCESS_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_RANDOM_ACCESS_STATE_TRANSITION);
    break;
    case L1_PACKET_ACCESS_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_PACKET_ACCESS_STATE_TRANSITION);
    break;
    /* ACCESS STATE(S) */
	
    /* DEDICATED STATE(S) */
    case L1_DEDICATED_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_DEDICATED_STATE_TRANSITION);
    break;
	/* DEDICATED STATE(S) */

	/* TRANSFER STATE(S) */
    case L1_TRANSFER_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_TRANSFER_STATE_TRANSITION);
    break;
    /* TRANSFER STATE(S) */
	
    #ifdef FEATURE_GSM_DTM
    /* DTM STATE */
    case L1_DTM_MODE:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_DTM_STATE_TRANSITION);
    break;
    #endif
    
    #if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    case L1_MULTI_SIM_DUMMY:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_MSIM_DUMMY_STATE_TRANSITION);
    break;
    case L1_MULTI_SIM_DECOUPLED:
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_MSIM_DECOUPLED_STATE_TRANSITION);
    break;
  #endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */  
    default:
     MSG_GERAN_HIGH_0_G("QSH DEBUG: Unknown Change state received");
    break;
  }
}

#endif


/*  EOF */
