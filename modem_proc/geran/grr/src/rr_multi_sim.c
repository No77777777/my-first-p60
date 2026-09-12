/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            GRR MULTI SIM CONTROL MODULE

GENERAL DESCRIPTION
  Provides Multi SIM support for GERAN Radio Resources (GRR)

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2010-2015 Qualcomm Technologies, Inc.

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_multi_sim.c#5 $
$DateTime: 2020/12/16 03:25:40 $ $Author: pwbldsvc $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
31/05/14    SJW     Refactored for latest Multi-SIM/Multi-Standby support
===========================================================================*/
#include "geran_variation.h"
#include "rr_multi_sim.h"
#include "rr_general.h"

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
#include "rr_nv.h"
#include "rr_init.h"
#include "rr_rr.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "rr_l2.h"
#include "rr_l2_g.h"
#include "rr_if.h"
#include "rr_l1_send.h"
#include "rr_l2_send.h"
#include "rr_mm_send.h"
#include "rr_grr.h"
#include "rr_control.h"
#include "rr_pscan.h"
#include "rr_mac_send.h"
#include "rlc_grr.h"
#include "rr_mode.h"
#include "rr_acq_db.h"
#include "rr_resel_calcs.h"
#include "rr_gprs_dsc.h"
#include "trm.h"
#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
#include "task.h"
#include "cm.h"
#endif /* !PERLUTF && !TEST_FRAMEWORK */
#if defined(FEATURE_GTOW_REDIRECTION_AFTER_CONN_RELEASE) || defined(FEATURE_LTE)
#include "rr_g2x_redir.h"
#endif /* FEATURE_GTOW_REDIRECTION_AFTER_CONN_RELEASE || FEATURE_LTE */
#include "rr_sim.h"
#include "rr_ps_access.h"
#include "rr_ps_domain.h"
#include "gs.h"
#include "rr_conn_establish.h"
#include "rr_sys_info_pcell.h"
#include "rr_sys_info_cache.h"
#ifdef FEATURE_GSM_TDS
#include "rr_tdsrrc_if.h"
#endif /* FEATURE_GSM_TDS */
#include "stringl/stringl.h"
#if defined(FEATURE_WCDMA) || defined(FEATURE_LTE) || defined (FEATURE_GSM_TDS)
#include "rr_g2x.h"
#endif /* FEATURE_WCDMA || FEATURE_LTE || FEATURE_GSM_TDS */
#include "rr_cell_selection.h"
#include "rr_plmn_list.h"
#include "geran_multi_sim_api.h"
#include "policyman.h"
#include "rr_gprs_debug.h"
#include "wwan_coex_mgr.h"
#include "cxm_intf_types.h"
#include "geran_multi_sim.h"
#include "rr_gprs_dsc.h"
#include "geran_nv.h"

#define RR_MS_EXTRACT_AS_ID(msg) (msg.as_id)
#define RR_MS_MAX_NUM_DEBUG_EVENTS 32  /* local trace buffer holds this many events per data space */
#define RR_MINIMUM_PROCEDURE_DURATION  (1800) /* 1.8 seconds, slightly shorter than a full TC cycle */
#define RR_CELL_SELECTION_DEFERRAL_MS  (RR_CELL_SELECTION_ABORT_TIMEOUT)    /* 45 seconds */
#define RR_OPTIONAL_SI_PROCEDURE_DURATION (2*RR_MINIMUM_PROCEDURE_DURATION) /* Wait for 2 TC Cycles */
#define RR_SYS_INFO_MAX_ALLOWED_TRM_DENIAL   1
#define RR_PLMN_LIST_MIN_DRX_COUNT           0
#define RR_PLMN_LIST_INITIAL_DRX_COUNT       1
#define RR_PLMN_LIST_MAX_DRX_COUNT           5
#define RR_PLMN_LIST_INVALID_DRX_COUNT       0xFF
/* Maximun page cycle for which page skip is allowed: 3*235 = 705ms */
#define RR_PAGE_SKIP_MAX_BS_PA_MFRMS         3

typedef enum
{
  RR_MS_NULL_DEBUG_EVENT = 0,
  RR_MS_BLOCKING_MSG_SENT,
  RR_MS_INTERFACE_EXPIRED,
  RR_MS_DS_ABORT_WAS_SENT,
  RR_MS_RESPONSE_RECEIVED,
  RR_MS_INTERFACE_SUSPEND,
  RR_MS_ENTER_HIBERNATION,
  RR_MS_INTERFACE_RESUMED,
  RR_MS_GRR_ABORT_BY_IMSG,
  RR_MS_GRR_ABORT_BY_FUNC,
  RR_MS_GRR_TIMER_EXPIRED,
  RR_MS_POLICY_MAN_FAILED,
  RR_MS_GRR_ABORT_BY_NAS,
  RR_MS_LAST_DEBUG_EVENT /* add any new events ABOVE this line */
} rr_ms_debug_event_t;

typedef struct
{
  rr_ms_debug_event_t event_list[RR_MS_MAX_NUM_DEBUG_EVENTS];
  int                 num_events;
  int                 num_wrapped;
} rr_ms_debug_info_t;

typedef enum
{
  RR_MS_NO_ACTION,
  RR_MS_PREPARE_FOR_SUSPENSION,
  RR_MS_CHECK_FOR_SUSPENSION,
  RR_MS_PREPARE_FOR_RESUMPTION,
  RR_MS_INVALID_ACTION
} rr_ms_action_t;

typedef enum
{
  RR_MS_POLL_FOR_RESPONSE,
  RR_MS_ABORT_IS_REQUIRED
} rr_ms_interface_check_t;

typedef struct
{
  rr_ms_action_t          next_action;
  rr_process_msg_action_e process_in_rr;
} rr_ms_executor_t;

typedef enum
{
  RR_MS_INTERFACE_ACTIVE,
  RR_MS_INTERFACE_SUSPENDED,
  RR_MS_INTERFACE_HIBERNATED
} rr_ms_gl1_if_state_t;

typedef enum
{
  RR_MS_PRIORITY_INVERSION_INIT,
  RR_MS_GAS_ID_STARTS_ACQUIRING,
  RR_MS_GAS_ID_SERVICE_RECOVERY,
  RR_MS_GAS_ID_RESPONSE_PENDING,
  RR_MS_GAS_ID_SERVICE_NOTIFIED,
  RR_MS_GAS_ID_CS_NOW_CONNECTED,
  RR_MS_GAS_ID_BLOCKING_MSG_SENT,
  RR_MS_GAS_ID_PRIORITY_NW_LIST,
  RR_MS_GAS_ID_ACQ_STATE_RUNNING,
  RR_MS_GAS_ID_ACQ_STATE_WAITING,
  RR_MS_GAS_ID_DUTY_CYCLE_EXPIRY,
  RR_MS_GAS_ID_POWER_SCAN_ENDED,
  RR_MS_GAS_ID_SEARCH_STARTED,
  RR_MS_GAS_ID_SEARCH_COMPLETED,
  RR_MS_PRIORITY_EVENT_UNDEFINED
} pri_inv_event_t;

typedef enum
{
  RR_MS_ACQ_NULL,
  RR_MS_ACQ_NOMINAL,
  RR_MS_ACQ_PENDING_INVERSION,
  RR_MS_ACQ_INVERTED
} acq_pri_state_t;

typedef enum
{
  RR_MS_SYS_INFO_NULL,
  RR_MS_SYS_INFO_NOMINAL,
  RR_MS_SYS_INFO_INVERTED
} sys_info_pri_state_t;

typedef enum
{
  RR_MS_SYS_INFO_EV_RESET,
  RR_MS_SYS_INFO_EV_BCCH_SELECTED,
  RR_MS_SYS_INFO_EV_SI_GUARD_TIMEOUT,
  RR_MS_SYS_INFO_EV_FULL_CAMPED,
  RR_MS_SYS_INFO_EV_OPTIONAL_SI_STARTED,
  RR_MS_SYS_INFO_EV_SI_STOPPED 
} sys_info_pri_event_t;


typedef enum
{
  RR_MS_PROC_LOCK_INACTIVE,
  RR_MS_PROC_LOCK_ACTIVE
} rr_proc_lock_status_t;

typedef enum
{
  RR_MS_PROC_LOCK_NONE,
  RR_MS_PROC_LOCK_CSFB,
  RR_MS_PROC_LOCK_CS_CALL
} rr_proc_lock_cause_t;

typedef struct
{
  rr_ms_gl1_if_state_t    gl1_interface_state;
  rr_l1_message_id_enum_T required_l1_message;
  rr_plmn_select_req_T    buffered_svc_request;
  dl_unit_data_ind_T      buffered_page_msg;
  rr_ms_debug_info_t      debug_events;
  rr_duration_t           ps_abort_delay;
  rr_duration_t           procedure_duration;
  rr_duration_t           pscan_time_inverted;
  acq_pri_state_t         acq_mgr_state;
  sys_info_pri_state_t    sys_info_mgr_state;
  uint16                  sys_info_denial_count;
  timetick_type           buffered_page_time;
  timetick_type           missed_page;
  timetick_type           page_cycle;
  boolean                 gprs_test_mode;
  boolean                 aborted_l1_interface;
  boolean                 cs_connected;
  uint16                  seconds_since_suspension;
  uint16                  svc_rqst_time;
  uint16                  svc_loss_time;
  word                    paging_group;
  rr_proc_lock_status_t   proc_lock_status;
  rr_proc_lock_cause_t    proc_lock_cause;
  boolean                 cell_selection_timeout;
  rr_ms_svc_recovery_t    ms_service_recovery;
  boolean                 drdsds_enabled;
  uint16                  no_resource_count;
  uint16                  nominal_grants;
  uint16                  inverse_grants;
  uint16                  deferral_count;
  boolean                 volte_call_status;
  uint32                  drx_count;
#ifdef FEATURE_GSM_DECODE_ALL_FREQS
  boolean                 scan_more_freq;
#endif /* FEATURE_GSM_DECODE_ALL_FREQS */
} gas_ctrl_data_t;

typedef struct
{
  rr_multimode_subs_chgd_req_T subscription_caps;
  gas_ctrl_data_t              gas_data[NUM_GERAN_DATA_SPACES];
} rr_multisim_info_t;

typedef enum
{
  RR_TECH_GSM1 = 0,
  RR_TECH_GSM2,
  RR_TECH_GSM3,
  RR_TECH_LTE,
  RR_TECH_TDSCDMA,
  RR_TECH_WCDMA,
  RR_TECH_ONEX,
  RR_TECH_HDR,
  RR_NUM_OF_TECHS
} rr_tech_id_e_t;

#define RR_TECH_RECENT_ACTIVITY_TICK_DURATION   100  // timer ticks every 100ms
#define RR_TECH_RECENT_ACTIVITY_MAX_TICKS        30  // 30x100 = 3000ms

typedef struct
{
  boolean tx_active;
  uint16  inactive_counter;
} rr_ms_tech_info_t;

/* private data structure used by Multi-SIM controller functions */
static rr_multisim_info_t rr_multisim_info;

static rr_ms_tech_info_t  rr_ms_tech_info[RR_NUM_OF_TECHS];

#define RR_MULTI_SIM_MODE_ENABLED    ((boolean)(SYS_MODEM_DEVICE_MODE_SINGLE_SIM != geran_get_multi_sim_mode()))
#define RR_PAGE_SOURCE_PRIORITY_LOW  (0)
#define RR_PAGE_SOURCE_PRIORITY_MED  (1)
#define RR_PAGE_SOURCE_PRIORITY_HIGH (2)

/*===========================================================================

FUNCTION      rr_ms_store_debug_event

DESCRIPTION   Stores a Multi-SIM debug event in a local buffer per GAS ID

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
static void rr_ms_store_debug_event(const rr_ms_debug_event_t new_event, const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
    int count;

    /* check for debug buffer wraparound */
    count = rr_multisim_info.gas_data[as_index].debug_events.num_events;
    if (count >= RR_MS_MAX_NUM_DEBUG_EVENTS)
    {
      rr_multisim_info.gas_data[as_index].debug_events.num_events = 0;
      rr_multisim_info.gas_data[as_index].debug_events.num_wrapped++;
      count = 0;
    }

    /* add the new event at the next available position and increment the count */
    rr_multisim_info.gas_data[as_index].debug_events.event_list[count] = new_event;
    rr_multisim_info.gas_data[as_index].debug_events.num_events++;

    /* blank out the next entry to make the last valid event more obvious */
    count++;
    if (count < RR_MS_MAX_NUM_DEBUG_EVENTS)
    {
      rr_multisim_info.gas_data[as_index].debug_events.event_list[count] = RR_MS_NULL_DEBUG_EVENT;
    }
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_read_policyman_config

DESCRIPTION   Reads the "sys subs" feature support flag from PolicyManager

DEPENDENCIES  None

RETURN VALUE  TRUE if all PolicyManager operations completed successfully

SIDE EFFECTS  Blocking call to PM interface

===========================================================================*/
static boolean rr_ms_read_policyman_config(sys_subs_feature_t *sys_subs_ptr)
{
  policyman_status_t status;

  if (sys_subs_ptr != NULL)
  {
    const policyman_item_id_t pm_id = POLICYMAN_ITEM_DEVICE_CONFIGURATION;
    const policyman_item_t   *pItem = NULL;

    /* request a PolicyMan item pointer using a blocking call */
    status = policyman_get_items_block(&pm_id, 1, &pItem);

    /* continue if the status is OK */
    if (POLICYMAN_SUCCEEDED(status))
    {
      /* retrieve the data associated with the item into the output parameter */
      status = policyman_device_config_get_subs_feature(pItem, 0, 0, sys_subs_ptr);
    }

    /* always release the item memory allocated by PolicyMan */
    /* NULL pointer use is OK, as PM performs its own checks */
    policyman_item_release(pItem);
  }
  else
  {
    /* default the status to "invalid args" */
    status = POLICYMAN_STATUS_ERR_INVALID_ARGS;
  }

  /* return value is TRUE if all PM operations were successful */
  /* and the output parameter was updated with the result data */
  return((boolean)POLICYMAN_SUCCEEDED(status));
}

/*===========================================================================

FUNCTION      rr_ms_reset_internal_variables

DESCRIPTION   Resets all Multi SIM-specific control flags that are NOT
              associated with any external interface. This should only be
              called when the RR task is first started.

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
static void rr_ms_reset_internal_variables(void)
{
  int as_index;

  /*Reset the interface control flags*/
  for (as_index = GERAN_DATA_SPACE_INDEX_1; as_index < NUM_GERAN_DATA_SPACES; as_index++)
  {
    rr_multisim_info.gas_data[as_index].gl1_interface_state      = RR_MS_INTERFACE_ACTIVE;
    rr_multisim_info.gas_data[as_index].required_l1_message      = RR_MS_INVALID_L1_MESSAGE;
    rr_multisim_info.gas_data[as_index].aborted_l1_interface     = FALSE;
    rr_multisim_info.gas_data[as_index].cs_connected             = FALSE;
    rr_multisim_info.gas_data[as_index].acq_mgr_state            = RR_MS_ACQ_NULL;
    rr_multisim_info.gas_data[as_index].sys_info_mgr_state       = RR_MS_SYS_INFO_NULL;
    rr_multisim_info.gas_data[as_index].pscan_time_inverted      = 0;
    rr_multisim_info.gas_data[as_index].sys_info_denial_count    = 0;
    rr_multisim_info.gas_data[as_index].seconds_since_suspension = 0;
    rr_multisim_info.gas_data[as_index].svc_rqst_time            = 0;
    rr_multisim_info.gas_data[as_index].svc_loss_time            = 0;
    rr_multisim_info.gas_data[as_index].cell_selection_timeout   = FALSE;
    rr_multisim_info.gas_data[as_index].ms_service_recovery      = RR_SERVICE_RECOVERY_NONE;
    rr_multisim_info.gas_data[as_index].drdsds_enabled           = FALSE;
    rr_multisim_info.gas_data[as_index].no_resource_count        = 0;
    rr_multisim_info.gas_data[as_index].nominal_grants           = 0;
    rr_multisim_info.gas_data[as_index].inverse_grants           = 0;
    rr_multisim_info.gas_data[as_index].deferral_count           = 0;
    rr_multisim_info.gas_data[as_index].volte_call_status        = FALSE;
    rr_multisim_info.gas_data[as_index].drx_count                = RR_PLMN_LIST_INVALID_DRX_COUNT;
#ifdef FEATURE_GSM_DECODE_ALL_FREQS
    rr_multisim_info.gas_data[as_index].scan_more_freq           = FALSE;
#endif /* FEATURE_GSM_DECODE_ALL_FREQS */
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_buffer_service_request

DESCRIPTION   Buffers a RR_PLMN_SELECT_REQ received while the AS is suspended

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
static void rr_ms_buffer_service_request(const rr_cmd_bdy_type *cmd_ptr)
{
  if (cmd_ptr != NULL)
  {
    const rr_plmn_select_req_T *request_ptr;
    sys_modem_as_id_e_type      nas_id;
    gas_id_t                    gas_id;
    int                         as_index;

    /* service request received during suspended state */
    /* buffer it here and record the arrival timestamp */
    request_ptr = &(cmd_ptr->mm.rr_plmn_select_req);
    nas_id      = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_plmn_select_req);
    gas_id      = geran_map_nas_id_to_gas_id(nas_id);
    as_index    = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    memscpy((void *)&(rr_multisim_info.gas_data[as_index].buffered_svc_request),
            sizeof(rr_multisim_info.gas_data[as_index].buffered_svc_request),
            (void *)request_ptr, sizeof(rr_plmn_select_req_T));

    /* use a +1 here, since time=0 is used to indicate "no occurrence" */
    rr_multisim_info.gas_data[as_index].svc_rqst_time = rr_multisim_info.gas_data[as_index].seconds_since_suspension+1;

    if (rr_multisim_info.gas_data[as_index].gl1_interface_state == RR_MS_INTERFACE_HIBERNATED)
    {
      MSG_GERAN_HIGH_0_G("Buffer svc request whilst AS is hibernated");
    }
    else
    {
      MSG_GERAN_HIGH_0_G("Buffer svc request whilst AS is suspended");
    }
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_start_heartbeat

DESCRIPTION  Starts the RR Dual SIM heartbeat timer with an appropriate heartrate

DEPENDENCIES None

RETURN VALUE TRUE if the heartbeat was started successfully

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_start_heartbeat(const rr_ms_gl1_if_state_t interface_state, const gas_id_t gas_id)
{
  rr_duration_t heart_rate;

  /* start a timer that will generate a heartbeat while the AS is unavailable */
  if (interface_state == RR_MS_INTERFACE_HIBERNATED)
  {
    /* during hibernation, heart rate is one beat per 4 seconds */
    heart_rate = RR_MS_HIBERNATE_SECONDS * GRR_TIMEOUT_1_SECOND;
  }
  else
  {
    /* normal suspension, heart rate is one beat per second */
    heart_rate = RR_MS_HEARTBEAT_SECONDS * GRR_TIMEOUT_1_SECOND;
  }

  /* heartbeat timer is started for the currently suspended GAS ID */
  return(rr_timer_start(RR_DUAL_SIM_HEARTBEAT_TIMER, heart_rate, gas_id));
}

/*===========================================================================

FUNCTION     rr_ms_set_aborted_interface

DESCRIPTION  Sets a flag to indicate whether the specified GAS ID has sent
             the Dual SIM abort signal to L1.

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_set_aborted_interface(const boolean l1_aborted, const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].aborted_l1_interface = l1_aborted;
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_recover_blocked_interface

DESCRIPTION  Notifies GL1 that a pending interface should be aborted so that
             it cleans up and returns the required response.

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_recover_blocked_interface(const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    rr_l1_message_id_enum_T pending_msg;
    boolean                 l1_should_complete;

    /* mark the interface as aborted */
    rr_ms_set_aborted_interface(TRUE, gas_id);

    /* log an appropriate event in the debug buffer */
    rr_ms_store_debug_event(RR_MS_DS_ABORT_WAS_SENT, gas_id);

    /* to resync the interface, send MPH_L1_DS_ABORT_IND to GL1 indicating which signal is pending */
    pending_msg = rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].required_l1_message;

    /* GL1 should complete certain operations if it has already been granted the RF chain */
    /* because it is not possible to track every lock grant and release action separately */
    l1_should_complete = (MPH_POWER_SCAN_CNF == pending_msg) ? FALSE : TRUE;
    
    rr_send_mph_l1_ds_abort_ind(RR_DS_ABORT_TO_NULL, pending_msg, l1_should_complete, gas_id);
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_prepare_for_suspension

DESCRIPTION  Performs operations necessary to suspend a GRR interface

DEPENDENCIES Input GAS ID parameter must be valid

RETURN VALUE TRUE if the specified GAS ID has successfully suspended

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_prepare_for_suspension(const rr_ms_gl1_if_state_t new_interface_state, const gas_id_t gas_id)
{
  boolean now_suspended;
  int     as_index;

  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  if (rr_multisim_info.gas_data[as_index].gl1_interface_state == RR_MS_INTERFACE_ACTIVE)
  {
    /* reset the suspended GAS ID, its abort flag and the heartbeat counter */
    rr_multisim_info.gas_data[as_index].seconds_since_suspension = 0;
    rr_ms_set_aborted_interface(FALSE, gas_id);

    /* reset counters that track NAS activity during suspension */
    rr_multisim_info.gas_data[as_index].svc_rqst_time = 0;
    rr_multisim_info.gas_data[as_index].svc_loss_time = 0;

    /* if the heartbeat starts up OK, then suspension has succeeded */
    now_suspended = rr_ms_start_heartbeat(new_interface_state, gas_id);
    if (now_suspended == TRUE)
    {
      /* store the suspension type for later use */
      rr_multisim_info.gas_data[as_index].gl1_interface_state = new_interface_state;

      /* log an appropriate event in the debug buffer */
      if (RR_MS_INTERFACE_HIBERNATED == new_interface_state)
      {
        rr_ms_store_debug_event(RR_MS_ENTER_HIBERNATION, gas_id);
        MSG_GERAN_HIGH_0_G("GRR hibernating interface, started heartbeat");
      }
      else
      {
        rr_ms_store_debug_event(RR_MS_INTERFACE_SUSPEND, gas_id);
        MSG_GERAN_HIGH_0_G("GRR suspending interface, started heartbeat");
      }
    }
    else
    {
      /* abandon the suspension attempt */
      MSG_GERAN_ERROR_0_G("Unable to suspend due to heartbeat failure");
    }
  }
  else
  {
    now_suspended = FALSE;
    if (RR_MS_INTERFACE_HIBERNATED == new_interface_state)
    {
      MSG_GERAN_HIGH_0_G("Unable to hibernate, already in non-active state");
    }
    else
    {
      MSG_GERAN_ERROR_0_G("Unexpected suspension, already in non-active state");
    }
  }

  return(now_suspended);
}

/*===========================================================================

FUNCTION     rr_ms_prepare_for_resumption

DESCRIPTION  Performs operations necessary to resume a GRR interface

DEPENDENCIES Input GAS ID parameter must be valid

RETURN VALUE VOID

SIDE EFFECTS May reprocess a previously buffered RR_PLMN_SELECT_REQ

===========================================================================*/
static void rr_ms_prepare_for_resumption(const boolean action_svc_request, const gas_id_t gas_id)
{
  int as_index;

  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  if (rr_multisim_info.gas_data[as_index].gl1_interface_state != RR_MS_INTERFACE_ACTIVE)
  {
    rr_cmd_bdy_type *cmd_ptr;

    /* cancel the heartbeat and termination timers */
    (void)rr_timer_cancel(RR_DUAL_SIM_HEARTBEAT_TIMER, gas_id);
    (void)rr_timer_cancel(RR_DUAL_SIM_TERMINATE_TIMER, gas_id);
    MSG_GERAN_HIGH_0_G("GRR resuming suspended interface");

    /* check whether any service requests were buffered during suspension */
    if ((rr_multisim_info.gas_data[as_index].svc_rqst_time != 0) &&
        (action_svc_request == TRUE))
    {
      if (rr_multisim_info.gas_data[as_index].svc_rqst_time >= rr_multisim_info.gas_data[as_index].svc_loss_time)
      {
        /* NAS has requested again since RR lost service */
        cmd_ptr = (rr_cmd_bdy_type *)&(rr_multisim_info.gas_data[as_index].buffered_svc_request);
      }
      else
      {
        /* RR went out of service after a request was buffered */
        cmd_ptr = NULL;
      }
      MSG_GERAN_HIGH_3_G("NAS request at %d, OOS at %d, action=%d",
                         (int)(rr_multisim_info.gas_data[as_index].svc_rqst_time),
                         (int)(rr_multisim_info.gas_data[as_index].svc_loss_time),
                         (int)(cmd_ptr != NULL));
    }
    else
    {
      /* no service requests from NAS during suspension/hibernation */
      cmd_ptr = NULL;
    }

    /* reset the suspension flags and counters */
    rr_multisim_info.gas_data[as_index].gl1_interface_state      = RR_MS_INTERFACE_ACTIVE;
    rr_multisim_info.gas_data[as_index].seconds_since_suspension = 0;
    rr_multisim_info.gas_data[as_index].svc_rqst_time            = 0;
    rr_multisim_info.gas_data[as_index].svc_loss_time            = 0;

    /* log an appropriate event in the debug buffer */
    rr_ms_store_debug_event(RR_MS_INTERFACE_RESUMED, gas_id);

    /* if a request needs to be serviced, process it now */
    if (cmd_ptr != NULL)
    {
      /* post the buffered request to RR's own task queue */
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
      if (GS_SUCCESS != gs_send_message(GS_QUEUE_RR, (void *)cmd_ptr, FALSE))
      {
        MSG_GERAN_ERROR_0_G("Unable to requeue pending service request");
      }
    }
  }
  else
  {
    MSG_GERAN_HIGH_0_G("Resumption not required, interface is active");
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_interface_was_aborted

DESCRIPTION  Checks the flag that indicate whether the specified GAS ID sent
             the MPH_L1_DS_ABORT_IND signal to L1.

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_interface_was_aborted(const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    return(rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].aborted_l1_interface);
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_activate_procedure_lock

DESCRIPTION  Starts the chain locking procedure. Following steps are done in sequence.
             Starts a Guard Timer, to track the chain lock procedure.
             Sets internal data to remember that procedure lock is now active.
             Updates GL1 with MPH_SET_PRIORITY_IND for procedure GERAN_LOCK_HOLD

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_activate_procedure_lock(const rr_proc_lock_cause_t proc_lock_cause, const gas_id_t gas_id)
{
#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
  if (geran_in_multi_standby_mode())
  {
    int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    if (RR_MS_PROC_LOCK_INACTIVE == rr_multisim_info.gas_data[as_index].proc_lock_status)
    {
      /* if the guard timer was started successfully, advise GL1 to hold the lock */
      if (rr_timer_start(RR_PROC_LOCK_GUARD_TIMER, RR_PROC_LOCK_TIMEOUT, gas_id))
      {
        rr_priority_info_t rr_priority_info;

        rr_multisim_info.gas_data[as_index].proc_lock_status = RR_MS_PROC_LOCK_ACTIVE;

        rr_priority_info.procedure_type = GERAN_LOCK_HOLD;
        rr_priority_info.new_priority   = GERAN_PRIORITY_NOT_CHANGED;
        rr_priority_info.new_procedure  = TRUE;
        if (RR_MS_PROC_LOCK_CSFB == proc_lock_cause)
        {
          rr_priority_info.sys_proc_type = SYS_PROC_TYPE_CSFB;
          rr_multisim_info.gas_data[as_index].proc_lock_cause = RR_MS_PROC_LOCK_CSFB;
        }
        else if (RR_MS_PROC_LOCK_CS_CALL == proc_lock_cause)
        {
          rr_priority_info.sys_proc_type = SYS_PROC_TYPE_CS_CALL;
          rr_multisim_info.gas_data[as_index].proc_lock_cause = RR_MS_PROC_LOCK_CS_CALL;
        }
        else
        {
          MSG_GERAN_ERROR_1_G("Procedure lock Invoked with Invalid Cause = %d", proc_lock_cause);
          rr_multisim_info.gas_data[as_index].proc_lock_cause  = RR_MS_PROC_LOCK_NONE;
          rr_multisim_info.gas_data[as_index].proc_lock_status = RR_MS_PROC_LOCK_INACTIVE;
          (void)rr_timer_cancel(RR_PROC_LOCK_GUARD_TIMER, gas_id);
          return;
        }

        MSG_GERAN_HIGH_1_G("Procedure lock activated for cause %d", (int)proc_lock_cause);
        rr_send_mph_set_priority_ind(&rr_priority_info, gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_0_G("Procedure lock failed since guard timer cannot be started");
      }
    }
    else
    {
      MSG_GERAN_ERROR_0_G("Procedure lock invoked when already active");
    }
  }
#else
  #error code not present
#endif /* !PERLUTF && !TEST_FRAMEWORK */

  return;
}

/*===========================================================================

FUNCTION     rr_ms_deactivate_procedure_lock

DESCRIPTION  Deactivates Procedure Locking, if Active, else does nothing                      

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_deactivate_procedure_lock(const gas_id_t gas_id)
{
#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  if (RR_MS_PROC_LOCK_INACTIVE != rr_multisim_info.gas_data[as_index].proc_lock_status)
  {
    rr_priority_info_t rr_priority_info;

    /* cancel the guard timer and reset local state */
    (void)rr_timer_cancel(RR_PROC_LOCK_GUARD_TIMER, gas_id);
    rr_multisim_info.gas_data[as_index].proc_lock_status = RR_MS_PROC_LOCK_INACTIVE;
    MSG_GERAN_HIGH_0_G("Procedure lock deactivated");

    /* advise GL1 it can now cancel the lock hold instruction */
    rr_priority_info.procedure_type = GERAN_LOCK_RELEASE;
    rr_priority_info.new_priority   = GERAN_PRIORITY_NOT_CHANGED;
    rr_priority_info.new_procedure  = FALSE;
    if (RR_MS_PROC_LOCK_CSFB == rr_multisim_info.gas_data[as_index].proc_lock_cause)
    {
      rr_priority_info.sys_proc_type = SYS_PROC_TYPE_CSFB;
    }
    else if (RR_MS_PROC_LOCK_CS_CALL == rr_multisim_info.gas_data[as_index].proc_lock_cause)
    {
      rr_priority_info.sys_proc_type = SYS_PROC_TYPE_CS_CALL;
    }
    else
    {
      rr_priority_info.sys_proc_type = SYS_PROC_TYPE_NONE;
    }

    rr_multisim_info.gas_data[as_index].proc_lock_cause = RR_MS_PROC_LOCK_NONE;
    rr_send_mph_set_priority_ind(&rr_priority_info, gas_id);
  }
#else
  #error code not present
#endif /* !PERLUTF && !TEST_FRAMEWORK */

  return;
}
/*===========================================================================

FUNCTION     rr_ms_update_acquisition_priority

DESCRIPTION  Updates GL1 with MPH_SET_PRIORITY_IND. The parameters of this
             function are:
                new_priority  - the priority that GL1 should store and use
                new_procedure - TRUE if GRR is expecting to send a further
                                cell selection primitive *immediately* after
                                this signal (e.g. a MPH_POWER_SCAN_REQ)

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
void rr_ms_update_acquisition_priority(const geran_priority_t new_priority, const boolean new_procedure, const gas_id_t gas_id)
{
  rr_priority_info_t rr_priority_info;

  rr_priority_info.procedure_type = GERAN_ACQ_PROCEDURE;
  rr_priority_info.new_priority   = new_priority;
  rr_priority_info.sys_proc_type  = SYS_PROC_TYPE_ACQUISITION;

  if (rr_ms_interface_was_aborted(gas_id) && (TRUE != new_procedure))
  {
    /* pass the priority but always set the new_procedure flag to TRUE here */
    /* which will advise GL1 to store the new priority but NOT to action it */
    rr_priority_info.new_procedure = TRUE;
  }
  else
  {
    /* pass the priority and the new procedure flag to GL1 in the MPH_SET_PRIORITY_IND */
    rr_priority_info.new_procedure = new_procedure;
  }

  rr_send_mph_set_priority_ind(&rr_priority_info, gas_id);
  return;
}

#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
/*===========================================================================

FUNCTION     rr_ms_restart_duty_cycle

DESCRIPTION  Starts a fresh instance of the duty cycle timer

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_restart_duty_cycle(const acq_pri_state_t acq_state, const rr_duration_t reduce_by, const gas_id_t gas_id)
{
  rr_duration_t tmr_duration;

  /* stop any running instance of the duty cycle timer */
  (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);

  /* reset resource cnt, when it reaches max count (10) */
  /**************************************************************************/
  /* where it inversion timer reach to 3s and nominal timer reach to 1s     */
  /* No resouce cnt  0   1   2   3   4   5   6   7   8   9  10              */
  /* Nominal    3s  3.0 2.8 2.6 2.4 2.2 2.0 1.8 1.6 1.4 1.2 1.0             */
  /* Inversion  1s  1.0 1.2 1.4 1.6 1.8 2.0 2.2 2.4 2.6 2.8 3.0             */
  /**************************************************************************/
  if ( rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].no_resource_count > 10)
  {
    rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].no_resource_count = 0;
    MSG_GERAN_HIGH_0_G("PIMMS: no resouce conunt was reached max value, now reset to 0");
  }

  /* determine the next cycle duration based on the state parameter */
  if (RR_MS_ACQ_NULL == acq_state)
  {
    /* in NULL state, assume that the first acquisition primitive will be a power scan */
    /* since the local store "required_l1_message" may not be intialised in this state */
    tmr_duration = RR_ACQ_NOMINAL_TIMEOUT_PSCAN;
  }
  else
  {
    rr_l1_message_id_enum_T pending_msg;

    /* check which acquisition primitive GRR is currently waiting for */
    if (rr_plc_bplmn_pscan_active(gas_id))
    {
      /* power scan is not explicitly tracked by the multi-SIM controller in background mode */
      pending_msg = MPH_POWER_SCAN_CNF;
    }
    else
    {
      pending_msg = rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].required_l1_message;
    }

    if ((RR_MS_ACQ_PENDING_INVERSION == acq_state) ||
        (RR_MS_ACQ_INVERTED          == acq_state))
    {
      if (MPH_POWER_SCAN_CNF == pending_msg)
      {
        tmr_duration = RR_ACQ_INVERSE_TIMEOUT_PSCAN;
      }
      else if (rr_ms_drdsds_enabled(gas_id))
      {
        tmr_duration = RR_ACQ_INVERSE_TIMEOUT_OTHER_DR;
      }
      else
      {
              
        tmr_duration = RR_ACQ_INVERSE_TIMEOUT_OTHER;      
        if ( rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].no_resource_count > 0)
        {
          uint16  failed_count = rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].no_resource_count;
          /*In case if  last failed due to lack of resource increase by 200ms% */
          tmr_duration = RR_ACQ_INVERSE_TIMEOUT_OTHER + (failed_count*RR_ACQ_DELTA_TIMEOUT_OTHER);
          MSG_GERAN_HIGH_0_G("PIMMS: duty cycle timer increased"); 
        }             
      }
    }
    else
    {
      if (MPH_POWER_SCAN_CNF == pending_msg)
      {
        tmr_duration = RR_ACQ_NOMINAL_TIMEOUT_PSCAN;
      }
      else if (rr_ms_drdsds_enabled(gas_id))
      {
        tmr_duration = RR_ACQ_NOMINAL_TIMEOUT_OTHER_DR;
      }
      else
      {
        tmr_duration = RR_ACQ_NOMINAL_TIMEOUT_OTHER;
        if ( rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].no_resource_count > 0)
        {
          uint16  failed_count = rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].no_resource_count;
          /*In case if  last failed due to lack of resource decrease the nominal  by 200 */
          tmr_duration = RR_ACQ_NOMINAL_TIMEOUT_OTHER - (failed_count*RR_ACQ_DELTA_TIMEOUT_OTHER);
        }
      }
    }
  }

  /* reduce the duration by any non-zero amount specified by the caller */
  if ((reduce_by > 0) &&
      (reduce_by < tmr_duration))
  {
    tmr_duration -= reduce_by;
  }

  /* and then start the timer with the defined duration */
  if (rr_timer_start(RR_ACQ_DUTY_CYCLE_TIMER, tmr_duration, gas_id))
  {
    MSG_GERAN_HIGH_1_G("PIMMS: started duty cycle timer for %dms", (int)tmr_duration);
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_log_pimms_event

DESCRIPTION  Emits debug for an event arriving in the specified state

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_log_pimms_event(const acq_pri_state_t acq_state, const pri_inv_event_t new_event, const gas_id_t gas_id)
{
  switch(acq_state)
  {
    case RR_MS_ACQ_NULL:
      MSG_GERAN_HIGH_1_G("PIMMS: event %d in state NULL", (int)new_event);
      break;

    case RR_MS_ACQ_NOMINAL:
      MSG_GERAN_HIGH_1_G("PIMMS: event %d in state NOMINAL", (int)new_event);
      break;

    case RR_MS_ACQ_PENDING_INVERSION:
      MSG_GERAN_HIGH_1_G("PIMMS: event %d in state PENDING_INVERSION", (int)new_event);
      break;

    case RR_MS_ACQ_INVERTED:
      MSG_GERAN_HIGH_1_G("PIMMS: event %d in state INVERTED", (int)new_event);
      break;
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_reset_grant_statistics

DESCRIPTION  Resets local flags & statistics for acquisition grants

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_reset_grant_statistics(const gas_id_t gas_id)
{
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  rr_multisim_info.gas_data[as_index].nominal_grants = 0;
  rr_multisim_info.gas_data[as_index].inverse_grants = 0;
  rr_multisim_info.gas_data[as_index].deferral_count = 0;
  return;
}
#endif /* !PERLUTF && !TEST_FRAMEWORK */

/*===========================================================================

FUNCTION     rr_ms_acquisition_manager

DESCRIPTION  Priority Inversion Management for Multi-SIM acquisition

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_acquisition_manager(const pri_inv_event_t new_event, const gas_id_t gas_id)
{

#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  /* handle reset/completion events the same regardless of state */
  if ((RR_MS_PRIORITY_INVERSION_INIT == new_event) ||
      (RR_MS_GAS_ID_SERVICE_NOTIFIED == new_event) ||
      (RR_MS_GAS_ID_CS_NOW_CONNECTED == new_event))
  {
    (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);
    rr_multisim_info.gas_data[as_index].pscan_time_inverted = 0;

    if ((new_event != RR_MS_PRIORITY_INVERSION_INIT) && (new_event != RR_MS_GAS_ID_SERVICE_NOTIFIED))
    {
      rr_multisim_info.gas_data[as_index].no_resource_count = 0;
    }

    if ((RR_MS_ACQ_PENDING_INVERSION == rr_multisim_info.gas_data[as_index].acq_mgr_state) ||
        (RR_MS_ACQ_INVERTED          == rr_multisim_info.gas_data[as_index].acq_mgr_state))
    {
      rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, TRUE, gas_id);
    }

    if (RR_MS_ACQ_NULL != rr_multisim_info.gas_data[as_index].acq_mgr_state)
    {
      rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NULL;
      MSG_GERAN_HIGH_1_G("PIMMS: state reset by event %d", (int)new_event);
    }
    return;
  }
  else
  {
    rr_ms_log_pimms_event(rr_multisim_info.gas_data[as_index].acq_mgr_state, new_event, gas_id);
  }

  switch(rr_multisim_info.gas_data[as_index].acq_mgr_state)
  {
    case RR_MS_ACQ_NULL:
      if (RR_MS_GAS_ID_STARTS_ACQUIRING == new_event)
      {
        if (geran_api_is_alt_sub_in_data(gas_id))
        {
          MSG_GERAN_HIGH_1_G("Other stck in BG Traafic, start with inv for event %d", (int)new_event);        
          /* move to pending inversion state and wait for GL1 to confirm */
          rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_PENDING_INVERSION;

          /* update GL1 with inverted priority before the recovery procedure starts */
          rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, TRUE, gas_id);
        }
        else
        {
          boolean new_proc_expected;

          /* move to nominal state and start the duty cycle timer */
          rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NOMINAL;
          rr_ms_reset_grant_statistics(gas_id);
 
          /* use NULL state here to differentiate initial sequence */
          rr_ms_restart_duty_cycle(RR_MS_ACQ_NULL, 0, gas_id);

          /* initialise GL1 with nominal priority, indicate new procedure if stack is NOT online already */
          new_proc_expected = !rr_mode_gsm_active_or_starting(gas_id);
          rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, new_proc_expected, gas_id);
        }
      }
      else if (RR_MS_GAS_ID_DUTY_CYCLE_EXPIRY == new_event)
      {
        /* no action necessary */;
      }
      else if (RR_MS_GAS_ID_SERVICE_RECOVERY == new_event)
      {
        /* move to pending inversion state and wait for GL1 to confirm */
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_PENDING_INVERSION;

        /* update GL1 with inverted priority before the recovery procedure starts */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, TRUE, gas_id);
      }
      else if (RR_MS_GAS_ID_RESPONSE_PENDING == new_event)
      {
        /* prompt GL1 to resubmit any pending request with the existing priority */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOT_CHANGED, FALSE, gas_id);
      }
      else if (RR_MS_GAS_ID_BLOCKING_MSG_SENT == new_event)
      {
        if (geran_api_is_alt_sub_in_data(gas_id))
        {
          MSG_GERAN_HIGH_1_G("Other stck in BG Traafic, start with inv for event %d", (int)new_event);        
          /* move to pending inversion state and wait for GL1 to confirm */
          rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_PENDING_INVERSION;
          /* update GL1 with inverted priority before the recovery procedure starts */
          rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, FALSE, gas_id);
        }
        else
        {
          /* move to nominal state and start the duty cycle timer */
          rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NOMINAL;
          rr_ms_reset_grant_statistics(gas_id);
          rr_ms_restart_duty_cycle(RR_MS_ACQ_NOMINAL, 0, gas_id);
          rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, TRUE, gas_id);
        }
      }
      else if (RR_MS_GAS_ID_PRIORITY_NW_LIST == new_event)
      {
        /* cancel duty cycle timer and move to inverted state */
        (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_INVERTED;

        /* update GL1 with inverted priority before the search starts */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, TRUE, gas_id);
      }
      else if (RR_MS_GAS_ID_ACQ_STATE_RUNNING == new_event)
      {
        if (geran_api_is_alt_sub_in_data(gas_id))
        {
          MSG_GERAN_HIGH_1_G("Other stck in BG Traafic, start with inv for event %d", (int)new_event);        
          /* move to pending inversion state and wait for GL1 to confirm */
          rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_PENDING_INVERSION;

          /* update GL1 with inverted priority before the recovery procedure starts */
          rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, TRUE, gas_id);
        }
        else
        {      
          /* move to nominal state and remain there */
          rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NOMINAL;
          rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, TRUE, gas_id);
        }
      }
      else if (RR_MS_GAS_ID_ACQ_STATE_WAITING == new_event)
      {
        if (RR_MS_PROC_LOCK_INACTIVE == rr_multisim_info.gas_data[as_index].proc_lock_status)
        {
          /* if procedure lock is not in effect these events might indicate a race condition */
          MSG_GERAN_ERROR_1_G("PIMMS: unexpected state change notification %d", (int)new_event);
        }
      }
      else if (RR_MS_GAS_ID_POWER_SCAN_ENDED == new_event)
      {
        rr_multisim_info.gas_data[as_index].pscan_time_inverted = 0;
      }
      else if (RR_MS_GAS_ID_SEARCH_STARTED == new_event)
      {
        (void)rr_ms_plmn_search_init_drx_count(gas_id);
        (void)rr_ms_plmn_search_update_priority_rqrd(FALSE, TRUE, gas_id); 
      }
      else if (RR_MS_GAS_ID_SEARCH_COMPLETED == new_event)
      {
       (void)rr_ms_plmn_search_invalidate_drx_count(gas_id);
      }  
      break;

    case RR_MS_ACQ_NOMINAL:
      if (RR_MS_GAS_ID_STARTS_ACQUIRING == new_event)
      {
        MSG_GERAN_HIGH_0_G("PIMMS: restarted, wait for event in nominal state");
      }
      else if (RR_MS_GAS_ID_DUTY_CYCLE_EXPIRY == new_event)
      {
        /* move to pending inversion state and wait for GL1 to confirm */
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_PENDING_INVERSION;

        /* update GL1 with inverted priority for existing procedure */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, FALSE, gas_id);
      }
      else if (RR_MS_GAS_ID_SERVICE_RECOVERY == new_event)
      {
        /* move to pending inversion state and wait for GL1 to confirm */
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_PENDING_INVERSION;

        /* update GL1 with inverted priority before the recovery procedure starts */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, TRUE, gas_id);
      }
      else if (RR_MS_GAS_ID_RESPONSE_PENDING == new_event)
      {
        /* trigger GL1 to retry the lock with the same (i.e. nominal) priority */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, FALSE, gas_id);
      }
      else if (RR_MS_GAS_ID_BLOCKING_MSG_SENT == new_event)
      {
        rr_ms_reset_grant_statistics(gas_id);
        MSG_GERAN_HIGH_0_G("PIMMS: already started, wait for event in nominal state");
      }
      else if (RR_MS_GAS_ID_PRIORITY_NW_LIST == new_event)
      {
        /* cancel duty cycle timer and move to inverted state */
        (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_INVERTED;

        /* update GL1 with inverted priority before the search starts */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, TRUE, gas_id);
      }
      else if (RR_MS_GAS_ID_ACQ_STATE_RUNNING == new_event)
      {
        /* stop the duty cycle timer since nominal priority is granted */
        (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);
        MSG_GERAN_HIGH_0_G("PIMMS: nominal grant - stop duty cycle timer");
      }
      else if (RR_MS_GAS_ID_ACQ_STATE_WAITING == new_event)
      {
        if (RR_MS_PROC_LOCK_INACTIVE == rr_multisim_info.gas_data[as_index].proc_lock_status)
        {
          /* restart the duty cycle timer since nominal priority is granted */
          MSG_GERAN_HIGH_0_G("PIMMS: GL1 waiting for nominal grant");
          if (rr_timer_is_running(RR_ACQ_DUTY_CYCLE_TIMER,gas_id))
          {
            MSG_GERAN_HIGH_0_G("PIMMS: timer is already running no restart");
          }
          else
          {
            rr_ms_restart_duty_cycle(RR_MS_ACQ_NOMINAL, 0, gas_id);
          }
        }
        else
        {
          /* if procedure lock is in effect no further inversion should be required */
          MSG_GERAN_HIGH_0_G("PIMMS: return to NULL state, procedure lock active");
          (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);
          rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NULL;
        }
      }
      else if (RR_MS_GAS_ID_POWER_SCAN_ENDED == new_event)
      {
        rr_multisim_info.gas_data[as_index].pscan_time_inverted = 0;
      }
      else if (RR_MS_GAS_ID_SEARCH_STARTED == new_event)
      {
        (void)rr_ms_plmn_search_init_drx_count(gas_id);
        (void)rr_ms_plmn_search_update_priority_rqrd(FALSE, TRUE, gas_id); 
      }
      else if (RR_MS_GAS_ID_SEARCH_COMPLETED == new_event)
      {
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NULL;
        (void)rr_ms_plmn_search_invalidate_drx_count(gas_id);
      }  
      break;

    case RR_MS_ACQ_PENDING_INVERSION:
    case RR_MS_ACQ_INVERTED:
      if (RR_MS_GAS_ID_STARTS_ACQUIRING == new_event)
      {
        MSG_GERAN_HIGH_0_G("PIMMS: restarted, wait for event in inverted state");
      }
      else if (RR_MS_GAS_ID_DUTY_CYCLE_EXPIRY == new_event)
      {
        /* move to nominal state and start the duty cycle timer */
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NOMINAL;
        rr_ms_restart_duty_cycle(RR_MS_ACQ_NOMINAL, 0, gas_id);

        /* update GL1 with nominal priority */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, FALSE, gas_id);
      }
      else if (RR_MS_GAS_ID_RESPONSE_PENDING == new_event)
      {
        /* trigger GL1 to retry the lock with the same (i.e. inverted) priority */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, FALSE, gas_id);
      }
      else if (RR_MS_GAS_ID_SERVICE_RECOVERY == new_event)
      {
        /* update GL1 with the same priority before the recovery procedure starts */
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, TRUE, gas_id);
      }
      else if (RR_MS_GAS_ID_BLOCKING_MSG_SENT == new_event)
      {
        rr_ms_reset_grant_statistics(gas_id);
        MSG_GERAN_HIGH_0_G("PIMMS: already started, wait for event in inverted state");
      }
      else if (RR_MS_GAS_ID_PRIORITY_NW_LIST == new_event)
      {
        (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_INVERTED;
      }
      else if (RR_MS_GAS_ID_ACQ_STATE_RUNNING == new_event)
      {
        /* Layer 1 has signalled that it has been granted permission to run */
        if (RR_MS_ACQ_PENDING_INVERSION == rr_multisim_info.gas_data[as_index].acq_mgr_state)
        {
          rr_duration_t used_time;

          /* determine whether this is an accumulated inversion period */
          used_time = rr_multisim_info.gas_data[as_index].pscan_time_inverted;
          rr_multisim_info.gas_data[as_index].pscan_time_inverted = 0;

          /* transition to inverted state and start the duty cycle timer */
          rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_INVERTED;
          MSG_GERAN_HIGH_0_G("PIMMS: chain granted for inverted priority");
          rr_ms_restart_duty_cycle(RR_MS_ACQ_INVERTED, used_time, gas_id);
        }
      }
      else if (RR_MS_GAS_ID_ACQ_STATE_WAITING == new_event)
      {
        if (RR_MS_PROC_LOCK_INACTIVE == rr_multisim_info.gas_data[as_index].proc_lock_status)
        {
          /* if procedure lock is not in effect this event might indicate a GL1 problem */
          MSG_GERAN_ERROR_1_G("PIMMS: unexpected state change notification %d", (int)new_event);
        }
      }
      else if (RR_MS_GAS_ID_POWER_SCAN_ENDED == new_event)
      {
        /* reset the stored inversion time before recalculating below */
        rr_multisim_info.gas_data[as_index].pscan_time_inverted = 0;

        if ((RR_MS_ACQ_INVERTED == rr_multisim_info.gas_data[as_index].acq_mgr_state) &&
            (rr_timer_is_running(RR_ACQ_DUTY_CYCLE_TIMER, gas_id)))
        {
          rr_duration_t unused_time = rr_timer_get_time_remaining(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);

          /* cancel the duty cycle timer now before deciding on the next state */
          (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);

          /* determine whether there is still some usable inversion period left */
          if (unused_time < (RR_SINGLE_FRAME_TICK_DURATION << 1))
          {
            /* less than 2 frames left, not enough to do anything meaningful, return to NOMINAL */
            rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NOMINAL;

            /* request nominal priority, next WAITING event from GL1 will restart the timer */
            rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, TRUE, gas_id);
          }
          else
          {
            /* calculate the amount of time consumed by the last inverted grant */
            if (unused_time < RR_ACQ_INVERSE_TIMEOUT_PSCAN)
            {
              rr_multisim_info.gas_data[as_index].pscan_time_inverted = (RR_ACQ_INVERSE_TIMEOUT_PSCAN - unused_time);
            }

            /* maintain inverted priority, next RUNNING event from GL1 will restart the timer */
            rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_PENDING_INVERSION;
            MSG_GERAN_HIGH_0_G("PIMMS: waiting for continuation of inverted grant");
          }
        }
      }
      else if (RR_MS_GAS_ID_SEARCH_STARTED == new_event)
      {
        (void)rr_ms_plmn_search_init_drx_count(gas_id);
        (void)rr_ms_plmn_search_update_priority_rqrd(FALSE, TRUE, gas_id); 
      }
      else if (RR_MS_GAS_ID_SEARCH_COMPLETED == new_event)
      {
        /* return to NULL state and restore nominal priority in Layer 1 */
        rr_multisim_info.gas_data[as_index].acq_mgr_state = RR_MS_ACQ_NULL;
        (void)rr_ms_plmn_search_invalidate_drx_count(gas_id);
        rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, TRUE, gas_id);
      }

      break;

    default:
      MSG_GERAN_ERROR_1_G("PIMMS: unhandled state, event %d ignored", (int)new_event);
      break;
  }
#endif /* !PERLUTF && !TEST_FRAMEWORK */

  return;
}

/*===========================================================================

FUNCTION     rr_ms_update_sys_info_priority

DESCRIPTION  Updates GL1 with MPH_SET_PRIORITY_IND for System Information Reading. The parameters of this
             function are:
             new_priority  - the priority that GL1 should store and use
             new_procedure - TRUE if if this is sent at start of System Information Procedure, FALSE otherwsie.

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_update_sys_info_priority(const geran_priority_t new_priority, const boolean new_procedure, const gas_id_t gas_id)
{
  rr_priority_info_t rr_priority_info;

  rr_priority_info.procedure_type = GERAN_SIB_PROCEDURE;
  rr_priority_info.new_priority   = new_priority;
  rr_priority_info.sys_proc_type  = SYS_PROC_TYPE_SYSTEM_INFO_READING;
  rr_priority_info.new_procedure  = new_procedure;

  rr_send_mph_set_priority_ind(&rr_priority_info, gas_id);
  return;
}

/*===========================================================================

FUNCTION     rr_ms_any_stack_in_sys_info_inversion_state

DESCRIPTION  Check whether any G stack is already in CHM IN state

DEPENDENCIES None

RETURN VALUE TRUE if already  FALSE otherwise

SIDE EFFECTS None

===========================================================================*/

static boolean rr_ms_any_stack_in_sys_info_inversion_state(void)
{
#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
  gas_id_t gas_id;

  for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
  {
    if (rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].sys_info_mgr_state == RR_MS_SYS_INFO_INVERTED)
    {
       MSG_GERAN_HIGH_0("G stack already in inversion, wait!!!!! ");      
       return(TRUE);
    }
  }
#endif /* !PERLUTF && !TEST_FRAMEWORK */

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_is_sys_info_inversion_required

DESCRIPTION  Check whether sys information read priority can be inverted or not

DEPENDENCIES None

RETURN VALUE TRUE if Inversion required, FALSE otherwise

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_is_sys_info_inversion_required(const rr_duration_t procedure_duration, const uint16 sys_info_denial_count)
{

  if (rr_ms_any_stack_in_sys_info_inversion_state())
  {
    return(FALSE);
  }

  if ((sys_info_denial_count > RR_SYS_INFO_MAX_ALLOWED_TRM_DENIAL) ||
      (procedure_duration < RR_MINIMUM_PROCEDURE_DURATION))
  {
    return TRUE;
  }
  else if ((procedure_duration > RR_MINIMUM_PROCEDURE_DURATION) &&
           (procedure_duration < RR_OPTIONAL_SI_PROCEDURE_DURATION) &&
           (sys_info_denial_count >= RR_SYS_INFO_MAX_ALLOWED_TRM_DENIAL))
  {
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION     rr_ms_sys_info_manager

DESCRIPTION  Priority Handling for Multi-SIM  System Information Reception

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_sys_info_manager(const sys_info_pri_event_t new_event, const gas_id_t gas_id)
{
#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  MSG_GERAN_HIGH_2_G("System Info. Mgr:Event =%d, Existing State = %d", (int)new_event, (int)rr_multisim_info.gas_data[as_index].sys_info_mgr_state);

  /* State Independent Handling at the start of the function */
  if ((RR_MS_SYS_INFO_EV_RESET      == new_event) ||
      (RR_MS_SYS_INFO_EV_SI_STOPPED == new_event))
  {
    (void)rr_timer_cancel(RR_SYS_INFO_GUARD_TIMER, gas_id);
    /* If Sys Info Acquisition is terminated, then Reset GL1 priority, if Inverted */
    if (RR_MS_SYS_INFO_INVERTED == rr_multisim_info.gas_data[as_index].sys_info_mgr_state)
    {
      rr_ms_update_sys_info_priority(GERAN_PRIORITY_NOMINAL, FALSE, gas_id);
    }

    /* reset state machine variables to defaults */
    rr_multisim_info.gas_data[as_index].sys_info_mgr_state    = RR_MS_SYS_INFO_NULL;
    rr_multisim_info.gas_data[as_index].sys_info_denial_count = 0;
    rr_multisim_info.gas_data[as_index].procedure_duration    = RR_MINIMUM_PROCEDURE_DURATION;
    return;
  }

  switch(rr_multisim_info.gas_data[as_index].sys_info_mgr_state)
  {
    case RR_MS_SYS_INFO_NULL:
      if (RR_MS_SYS_INFO_EV_BCCH_SELECTED == new_event)
      {
        /* GL1 has already started SI Acquisition with Nominal Priority */
        rr_ms_check_and_start_sys_info_guard_timer(TRUE, gas_id);
        rr_multisim_info.gas_data[as_index].sys_info_mgr_state = RR_MS_SYS_INFO_NOMINAL;
      }
      else if (RR_MS_SYS_INFO_EV_OPTIONAL_SI_STARTED == new_event)
      {
        /* GL1 has already started SI Acquisition with Nominal Priority */
        rr_multisim_info.gas_data[as_index].procedure_duration = RR_OPTIONAL_SI_PROCEDURE_DURATION;
        rr_ms_check_and_start_sys_info_guard_timer(TRUE, gas_id);
        rr_multisim_info.gas_data[as_index].sys_info_mgr_state = RR_MS_SYS_INFO_NOMINAL;
      }
      break;

    case RR_MS_SYS_INFO_NOMINAL:
      if (RR_MS_SYS_INFO_EV_SI_GUARD_TIMEOUT == new_event)
      {
        (void)rr_timer_cancel(RR_SYS_INFO_GUARD_TIMER, gas_id);

        if (rr_ms_is_sys_info_inversion_required(rr_multisim_info.gas_data[as_index].procedure_duration, rr_multisim_info.gas_data[as_index].sys_info_denial_count))
        {
          /* Now Reset Denial Count Evaluation */
          rr_multisim_info.gas_data[as_index].sys_info_denial_count = 0;
          
          rr_ms_update_sys_info_priority(GERAN_PRIORITY_INVERTED, FALSE, gas_id);
          rr_multisim_info.gas_data[as_index].sys_info_mgr_state = RR_MS_SYS_INFO_INVERTED;        
        }
        else
        {
          /* System Information Acquisiton not complete yet, RESTART the Duty Cycle Timer */
          rr_ms_check_and_start_sys_info_guard_timer(TRUE, gas_id);
        }
      }
      else if (RR_MS_SYS_INFO_EV_FULL_CAMPED == new_event)
      {
        (void)rr_timer_cancel(RR_SYS_INFO_GUARD_TIMER, gas_id);
        rr_multisim_info.gas_data[as_index].sys_info_denial_count = 0;
        rr_multisim_info.gas_data[as_index].procedure_duration = RR_MINIMUM_PROCEDURE_DURATION;
      }
      else if (RR_MS_SYS_INFO_EV_BCCH_SELECTED == new_event)
      {
        /* GL1 has already started SI Acquisition with Nominal Priority, ensure Sys Information Guard Timer is Running */
        rr_ms_check_and_start_sys_info_guard_timer(TRUE, gas_id);
      }
      else if (RR_MS_SYS_INFO_EV_OPTIONAL_SI_STARTED == new_event)
      {
        rr_multisim_info.gas_data[as_index].procedure_duration = RR_OPTIONAL_SI_PROCEDURE_DURATION;
        rr_ms_check_and_start_sys_info_guard_timer(TRUE, gas_id);
      }
      break;

    case RR_MS_SYS_INFO_INVERTED:
      if (RR_MS_SYS_INFO_EV_FULL_CAMPED == new_event)
      {
        (void)rr_timer_cancel(RR_SYS_INFO_GUARD_TIMER, gas_id);
        rr_multisim_info.gas_data[as_index].sys_info_denial_count = 0;
        rr_multisim_info.gas_data[as_index].procedure_duration = RR_MINIMUM_PROCEDURE_DURATION;

        /* Revert to Nominal Priority to begin with for remaining Optional SI's */
        rr_ms_update_sys_info_priority(GERAN_PRIORITY_NOMINAL, FALSE, gas_id);
        rr_multisim_info.gas_data[as_index].sys_info_mgr_state = RR_MS_SYS_INFO_NOMINAL;
      }
      else if (RR_MS_SYS_INFO_EV_OPTIONAL_SI_STARTED == new_event)
      {
        rr_multisim_info.gas_data[as_index].procedure_duration = RR_OPTIONAL_SI_PROCEDURE_DURATION;
        rr_ms_check_and_start_sys_info_guard_timer(TRUE, gas_id);
      }
      else if (RR_MS_SYS_INFO_EV_SI_GUARD_TIMEOUT == new_event)
      {
        (void)rr_timer_cancel(RR_SYS_INFO_GUARD_TIMER, gas_id);
        rr_multisim_info.gas_data[as_index].procedure_duration = RR_OPTIONAL_SI_PROCEDURE_DURATION;
        rr_ms_check_and_start_sys_info_guard_timer(TRUE, gas_id);
        rr_ms_update_sys_info_priority(GERAN_PRIORITY_NOMINAL, FALSE, gas_id);
        rr_multisim_info.gas_data[as_index].sys_info_mgr_state = RR_MS_SYS_INFO_NOMINAL;
      }
      break;

    default:
      MSG_GERAN_ERROR_1_G("rr_ms_sys_info_manager Invalid State = %d", (int)rr_multisim_info.gas_data[as_index].sys_info_mgr_state);
      break;
  }
#else
  #error code not present
#endif /* !PERLUTF && !TEST_FRAMEWORK */
  return;
}

/*===========================================================================

FUNCTION     rr_ms_sys_info_status_update

DESCRIPTION  Receives External System Information Updates from GRR module and injects Internal events into
             sys info manager

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
void rr_ms_sys_info_status_update(const rr_ms_sys_info_acq_status_t sys_info_acq_status, const gas_id_t gas_id)
{
#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
  switch(sys_info_acq_status)
  {
    case RR_SYS_INFO_ACQ_STARTED:
      rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_BCCH_SELECTED, gas_id);
      break;

    case RR_SYS_INFO_ACQ_OPTIONAL_SI:
      rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_OPTIONAL_SI_STARTED, gas_id);
      break;

    case RR_SYS_INFO_ACQ_STOPPED:
      rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_SI_STOPPED, gas_id);
      break;

    default:
      MSG_GERAN_ERROR_1_G("Invalid SysInfo Status = %d", (int)sys_info_acq_status);
      break;
  }
#else
  #error code not present
#endif /* !PERLUTF && !TEST_FRAMEWORK */

  return;
}

/*===========================================================================

FUNCTION      rr_ms_validate_service_request

DESCRIPTION   Checks for discrepancies in the RR_PLMN_SELECT_REQ signal from NAS

DEPENDENCIES  None

RETURN VALUE  ASID to be used by main DSDS controller

SIDE EFFECTS  May reject NAS signal and cause the message to be ignored

===========================================================================*/
static sys_modem_as_id_e_type rr_ms_validate_service_request(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  sys_modem_as_id_e_type nas_id;
  sys_modem_as_id_e_type bad_id;
  gas_id_t               gas_id;

  /* extract the ASID which NAS has used in this message */
  nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_plmn_select_req);
  gas_id = geran_map_nas_id_to_gas_id(nas_id);

  /* check for invalid service domain types and respond immediately */
  if ((cmd_ptr->mm.rr_plmn_select_req.service_domain == SYS_SRV_DOMAIN_NONE) ||
      (cmd_ptr->mm.rr_plmn_select_req.service_domain == SYS_SRV_DOMAIN_NO_SRV))
  {
    bad_id = nas_id;
    MSG_GERAN_ERROR_0_G("NAS sent service request for NO_SERVICE");
  }
  else
  {
    /* request is OK so far */
    bad_id = SYS_MODEM_AS_ID_NONE;

    /* perform sanity checks for single standby mode */
    if (RR_MULTI_SIM_MODE_ENABLED == FALSE)
    {
      if (rr_ms_any_other_active_gas_id(gas_id))
      {
        bad_id = nas_id;
        MSG_GERAN_ERROR_0_G("NAS trying to start multiple stacks in Single Standby mode");
      }
    }
  }

  /* if the request has been rejected, set output parameters accordingly */
  if (bad_id != SYS_MODEM_AS_ID_NONE)
  {
    /* no need to process this primitive further */
    exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
    exec_ptr->next_action   = RR_MS_NO_ACTION;
    nas_id                  = SYS_MODEM_AS_ID_NONE;

    /* explicitly reject the NAS request */
    rr_ms_send_rr_plmn_select_cnf(MM_AS_CONNECTED, MM_RR_OTHER_FAILURE, FALSE, bad_id);
  }
  else
  {
    /* otherwise check whether this stack needs to start priority inversion */
    if (RR_MM_MO_CS_CALL == cmd_ptr->mm.rr_plmn_select_req.service_req_cause)
    {
      MSG_GERAN_HIGH_0_G("NAS has sent service request for CS_CALL");
      if (rr_mode_gsm_active_or_starting(gas_id))
      {
        /* already online, reset the cause */
        rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].proc_lock_cause = RR_MS_PROC_LOCK_NONE;
      }
      else
      {
        /* enable macro-lock once START_GSM_MODE_CNF is received*/
        rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].proc_lock_cause = RR_MS_PROC_LOCK_CS_CALL;
      }
    }
    rr_ms_acquisition_manager(RR_MS_GAS_ID_STARTS_ACQUIRING, gas_id);
  }

  return(nas_id);
}

/*===========================================================================

FUNCTION      rr_ms_validate_connection_request

DESCRIPTION   Checks whether a RR_EST_REQ signal from NAS can be handled

DEPENDENCIES  None

RETURN VALUE  ASID to be used by main DSDS controller

SIDE EFFECTS  May reject NAS signal and cause the message to be ignored

===========================================================================*/
static sys_modem_as_id_e_type rr_ms_validate_connection_request(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  sys_modem_as_id_e_type nas_id;
  gas_id_t               gas_id;

  /* extract the ASID which NAS has used in this message */
  nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_est_req);
  /* and convert it to a GERAN access stratum ID */
  gas_id = geran_map_nas_id_to_gas_id(nas_id);

  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    int as_index;

    /* establish a data space index for the supplied NAS ID */
    as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /* check whether this stack is waiting for a cell selection primitive in DSDS mode */
    if (MPH_SELECT_SPECIFIC_BCCH_CNF == rr_multisim_info.gas_data[as_index].required_l1_message)
    {
      /* GRR is waiting for an existing cell acquisition to complete */
      /* so prevent this call request from interrupting this process */
      rr_send_abort_ind(RR_ABORT_CON_EST_NOT_ATTEMPTED, MM_RR_OTHER_FAILURE, gas_id);

      /* no need to process this primitive further */
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      exec_ptr->next_action   = RR_MS_NO_ACTION;

      MSG_GERAN_ERROR_0_G("Call establishment rejected - cell acquisition incomplete");
    }
  }

  return(nas_id);
}

/*===========================================================================

FUNCTION      rr_ms_validate_plmn_search_request

DESCRIPTION   Checks whether a RR_PLMN_LIST_REQ signal from NAS can be handled

DEPENDENCIES  None

RETURN VALUE  ASID to be used by main DSDS controller

SIDE EFFECTS  NA

===========================================================================*/
static sys_modem_as_id_e_type rr_ms_validate_plmn_search_request(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  sys_modem_as_id_e_type nas_id;
  gas_id_t               gas_id;

  /* extract the ASID which NAS has used in this message */
  nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_plmn_list_req);
  /* and convert it to a GERAN access stratum ID */
  gas_id = geran_map_nas_id_to_gas_id(nas_id);
  /* check whether other stack is in connected mode */
  if (geran_in_multi_standby_mode() && rr_ms_other_stack_in_dedicated(gas_id))
  {
    /*Send reject for Automatic BPLMN mode */
    if (SYS_NETWORK_SELECTION_MODE_AUTOMATIC == cmd_ptr->mm.rr_plmn_list_req.network_selection_mode)
    {
      MSG_GERAN_HIGH_0_G("PLMN search for auto mode rejected other G sub in connected mode");
      rr_ms_send_rr_plmn_list_cnf(&(cmd_ptr->mm.rr_plmn_list_req), MM_AS_CONNECTED);
      /* no need to process this primitive further */
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      exec_ptr->next_action   = RR_MS_NO_ACTION;
    }
  }
  return(nas_id);
}

#ifdef FEATURE_MM_PROVIDES_CALL_STATUS_IND
/*===========================================================================
FUNCTION      rr_ms_update_volte_call_status

DESCRIPTION   Handle the CALL_STATUS_IND from NAS

DEPENDENCIES  None

RETURN VALUE  ASID to be used by main DSDS controller

SIDE EFFECTS  None

===========================================================================*/
static sys_modem_as_id_e_type rr_ms_update_volte_call_status(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  sys_modem_as_id_e_type nas_id;
  gas_id_t               gas_id;
  boolean                volte_call_status;

  /* extract the ASID which NAS has used in this message */
  nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_mmtel_call_status_ind);

  /* and convert it to a GERAN access stratum ID */
  gas_id = geran_map_nas_id_to_gas_id(nas_id);

  volte_call_status = cmd_ptr->mm.rr_mmtel_call_status_ind.voice_call_status;

  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].volte_call_status = volte_call_status;
    exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
    exec_ptr->next_action   = RR_MS_NO_ACTION;
  }

  MSG_GERAN_HIGH_1_G("VoLTE call status set to %d", (int)volte_call_status);
  return(nas_id);
}
#endif /* FEATURE_MM_PROVIDES_CALL_STATUS_IND */

/*===========================================================================

FUNCTION      rr_ms_volte_call_active_on_other_stack

DESCRIPTION   Determines whether VoLTE may be active on the other stack

DEPENDENCIES  None

RETURN VALUE  TRUE if VoLTE status is set by NAS for another subscription

===========================================================================*/
static boolean rr_ms_volte_call_active_on_other_stack(const gas_id_t gas_id)
{
  gas_id_t other_gas_id;

  for (other_gas_id = GERAN_ACCESS_STRATUM_ID_1; other_gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; other_gas_id++)
  {
    if (other_gas_id != gas_id)
    {
      if (rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(other_gas_id)].volte_call_status)
      {
        MSG_GERAN_HIGH_0_G("VoLTE is active on another stack");
        return(TRUE);
      }
    }
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION      rr_ms_set_connection_priority

DESCRIPTION   Sets the Connection priority based on received procedure type from NAS.

DEPENDENCIES  None

RETURN VALUE Void

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_set_connection_priority(const sys_proc_type_e_type proc_type, const boolean is_ps_procedure, const gas_id_t gas_id)
{
#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
  rr_priority_info_t rr_priority_info;

  rr_priority_info.new_procedure = TRUE;
  rr_priority_info.sys_proc_type = proc_type;

  if (is_ps_procedure)
  {
    rr_priority_info.procedure_type = GERAN_GPRS_PROCEDURE;
    if (MAC_TRANSFER == rr_macinfo_get_mac_status(gas_id))
    {
      rr_priority_info.new_procedure = FALSE;
    }
  }
  else
  {
    /* Procedure is CS procedure */
    rr_priority_info.procedure_type = GERAN_CONN_PROCEDURE;
    if (rr_in_dedicated(gas_id))
    {
      rr_priority_info.new_procedure = FALSE;
    }
  }

  switch(proc_type)
  {
    case SYS_PROC_TYPE_CS_REGISTRATION:
    case SYS_PROC_TYPE_SMS:
      rr_priority_info.new_priority = GERAN_PRIORITY_NOMINAL;
      break;

    case SYS_PROC_TYPE_CS_CALL:
    case SYS_PROC_TYPE_SUPPLEMENTARY_SERVICE:
    case SYS_PROC_TYPE_CS_HANDOVER:
    case SYS_PROC_TYPE_SRVCC:
      rr_priority_info.new_priority = GERAN_PRIORITY_INVERTED;
      break;

    case SYS_PROC_TYPE_PS_REGISTRATION:
    case SYS_PROC_TYPE_PS_CALL_SIGNALING:
    case SYS_PROC_TYPE_HIGH_PRIORITY_PS_CALL:
    case SYS_PROC_TYPE_MMS: 
    case SYS_PROC_TYPE_NON_IMS_OTHERS: /* Any non-IMS procedures on non-DDS sub that use Internet PDN */
    case SYS_PROC_TYPE_RS_RA: /* Last  attempt of Route Solicitation/Route Advertisement */
    case SYS_PROC_TYPE_IMS_REGISTRATION:
      rr_priority_info.new_priority = GERAN_PRIORITY_INVERTED;
      break;

    case SYS_PROC_TYPE_PS_CALL_USER_PLANE:
      rr_priority_info.new_priority = GERAN_PRIORITY_NOMINAL;
      break;

    case SYS_PROC_TYPE_NONE:
      rr_priority_info.new_priority = GERAN_PRIORITY_NOMINAL;

      /* Exception case, Default NONE is PS however, NAS can indicate None for CS also */
      rr_priority_info.new_procedure = FALSE;
      if (rr_in_dedicated(gas_id))
      {
        rr_priority_info.procedure_type = GERAN_CONN_PROCEDURE;
      }
      break;

    default:
      MSG_GERAN_ERROR_1_G("Invalid procedure type received from NAS = %d", (int)proc_type);
      return;
  }

  rr_send_mph_set_priority_ind(&rr_priority_info, gas_id);
#else
  #error code not present
#endif /* !PERLUTF && !TEST_FRAMEWORK */

  return;
}

/*===========================================================================

FUNCTION      rr_ms_channel_sharing_allowed

DESCRIPTION   checks if channel sharing is allowed or not

DEPENDENCIES  None

RETURN VALUE  TRUE if channel sharing is possible on the specified GAS ID

SIDE EFFECTS  None

===========================================================================*/
boolean rr_ms_channel_sharing_allowed(const gas_id_t gas_id)
{
  boolean allow_sharing = FALSE;

  /* check whether this access stratum is online */
  if (rr_mode_gsm_active_or_starting(gas_id))
  {
    /* sharing is allowed in DSDS/TSTS modes */
    if (geran_in_multi_standby_mode())
    {
      allow_sharing = TRUE;
    }
#if defined(FEATURE_SGLTE_DUAL_SIM) || defined(FEATURE_SVLTE_DUAL_SIM)
    /* DSDA mode implied (more than one GAS ID active, but not in multi-standby) */
    else
    {
      /* In DSDA mode sharing is allowed only when SGLTE mode is active and service domain is CS only */
      if (rr_is_sglte_mode_active(GERAN_ACCESS_STRATUM_ID_1) &&
          (rr_sel_get_service_domain(GERAN_ACCESS_STRATUM_ID_1) == SYS_SRV_DOMAIN_CS_ONLY))
      {
        allow_sharing = TRUE;
      }
    }
#endif /* FEATURE_SGLTE_DUAL_SIM || FEATURE_SVLTE_DUAL_SIM */
  }

  return(allow_sharing);
}

/*===========================================================================

FUNCTION      rr_ms_two_stacks_have_same_BSIC

DESCRIPTION   checks if two stacks have same BSIC

DEPENDENCIES  None

RETURN VALUE  TRUE if two stacks have same BSIC, the provided gas_id and 
              the other one

SIDE EFFECTS  None

===========================================================================*/
static boolean rr_ms_two_stacks_have_same_BSIC(const gas_id_t gas_id)
{
  gas_id_t other_gas_id;

  for (other_gas_id = GERAN_ACCESS_STRATUM_ID_1; other_gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; other_gas_id++)
  {
    if (rr_mode_gsm_active_or_starting(other_gas_id) && other_gas_id != gas_id)
    {
      gprs_scell_info_t *scell_ptr_1 = rr_gprs_get_scell_info(gas_id);
      gprs_scell_info_t *scell_ptr_2 = rr_gprs_get_scell_info(other_gas_id);

      return ((scell_ptr_1->gsm.camped_on_flag) &&
              (scell_ptr_2->gsm.camped_on_flag) &&
              SAME_BSIC(scell_ptr_1->gsm.BSIC, scell_ptr_2->gsm.BSIC));
    }  
  }

  return FALSE;
}

/*===========================================================================

FUNCTION      rr_ms_share_ota_message

DESCRIPTION   Demultiplexer callback from Layer 2 for downlink data blocks

DEPENDENCIES  None

RETURN VALUE  TRUE if the specified GAS ID wants the block on the specified ARFCN

SIDE EFFECTS  None

===========================================================================*/
boolean rr_ms_share_ota_message(const l2_mux_data_t rcvd_type, ARFCN_T channel, const gas_id_t gas_id)
{
  boolean allow_sharing = FALSE;

  /* check whether channel sharing is allowed */
  if (rr_ms_channel_sharing_allowed(gas_id))
  {
    gprs_scell_info_t *scell_ptr = rr_gprs_get_scell_info(gas_id);
    gprs_scell_info_t *pcell_ptr = rr_gprs_get_pcell_info(gas_id);

    if (L2_MUX_CBCH == rcvd_type)
    {
      /* determine whether CBCH is enabled on this stack (regardless of mode) */
      if (scell_ptr->gsm.cbch_channel_information_valid)
      {
        allow_sharing = (scell_ptr->gsm.camped_on_flag) && ARFCNS_EQUAL(scell_ptr->gsm.BCCH_ARFCN, channel);
      }
    }
    else if (L2_MUX_CCCH == rcvd_type)
    {
      if (ARFCNS_EQUAL(pcell_ptr->gsm.BCCH_ARFCN, channel))
      {
        allow_sharing = TRUE;
      }
      else
      {
        if (ARFCNS_EQUAL(scell_ptr->gsm.BCCH_ARFCN, channel) &&
            rr_ms_two_stacks_have_same_BSIC(gas_id))
         {
           allow_sharing = TRUE;
         }
      }
    }
  }

  return(allow_sharing);
}

/*===========================================================================

FUNCTION      rr_ms_register_channel_sharing

DESCRIPTION   Registers a set of callback functions that can be used for
              demultiplexing of CCCH or CBCH blocks.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
static void rr_ms_register_channel_sharing(void)
{
  MSG_GERAN_HIGH_0("Registering callback functions for channel sharing");
  rr_send_dl_data_mux_ind(
    rr_ms_share_ota_message,
    rr_ms_share_ota_message,
    GERAN_ACCESS_STRATUM_ID_1
  );

  return;
}

/*===========================================================================

FUNCTION      rr_ms_status_change_handler

DESCRIPTION   Handle the multi-SIM mode flags within RR_DS_STATUS_CHANGE_IND

DEPENDENCIES  None

RETURN VALUE  SYS_MODEM_AS_ID_NONE (message carries no ASID tag)

SIDE EFFECTS  May trigger device mode updates to Layer 1

===========================================================================*/
static sys_modem_as_id_e_type rr_ms_status_change_handler(const rr_ds_status_change_ind_T *status_ptr)
{
  if (status_ptr != NULL)
  {
    sys_modem_dual_standby_pref_e_type ms_standby_pref = status_ptr->multi_standby_mode_pref;
    sys_subs_feature_t                 subs_feature    = SYS_SUBS_FEATURE_MODE_NORMAL;
    boolean device_mode_changed;

    if (rr_ms_read_policyman_config(&subs_feature))
    {
      geran_set_multimode_subs_feature(subs_feature);
    }
    else
    {
      /* log an appropriate event in the debug buffer */
      rr_ms_store_debug_event(RR_MS_POLICY_MAN_FAILED, GERAN_ACCESS_STRATUM_ID_1);
      MSG_GERAN_ERROR_0("Unable to read sys subs feature from PolicyMan");
    }

    /* Update GCOMMON storage with device mode and standby pref */
    MSG_GERAN_HIGH_1("RR_DS_STATUS_CHANGE_IND: new multi-standby pref=%d", (int)ms_standby_pref);
    device_mode_changed = geran_set_sys_multi_sim_config(status_ptr->device_mode, ms_standby_pref);

    /* NAS interface is permitted to make changes between DSDS and DSDA after activation */
    if (device_mode_changed)
    {
      gas_id_t gas_id;

      /* device mode has changed, check whether any G stacks are already active */
      for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
      {
        if (rr_mode_gsm_active_or_starting(gas_id))
        {
          /* need to update GL1 with new device mode (once only) */
          rr_send_mph_device_mode_ind(GERAN_DEVICE_MODE_CAUSE_MMCP, gas_id);
          break;
        }
      }
    }
  }

  return(SYS_MODEM_AS_ID_NONE);
}

/*===========================================================================

FUNCTION      rr_ms_store_subscription_capabilities

DESCRIPTION   Stores the latest subscription capabilities received from NAS

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
static void rr_ms_store_subscription_capabilities(const rr_multimode_subs_chgd_req_T *msg_ptr)
{
  if (msg_ptr != NULL)
  {
    uint32 subs_cap_mask;
    int    as_index;

    /* copy all of the payload from the provided message */
    memscpy(&(rr_multisim_info.subscription_caps), sizeof(rr_multisim_info.subscription_caps),
            msg_ptr, sizeof(rr_multimode_subs_chgd_req_T));

    /* publish the array of capabilities to GCOMMON */
    geran_store_subscription_capabilities(rr_multisim_info.subscription_caps.subs_capability_mask);

    /* post-process the information */
    for (as_index = 0; as_index < MAX_AS_IDS; as_index++)
    {
      /* check for special marker values that indicate finer granularity of capability info */
      if (SUBS_CAPABILITY_NONE == rr_multisim_info.subscription_caps.subs_capability[as_index])
      {
        /* more detailed capability mask is valid */
        subs_cap_mask = rr_multisim_info.subscription_caps.subs_capability_mask[as_index];

        /* update the generic capability based on support for techs that allow inter-RAT with GERAN */
        if (subs_cap_mask & (SYS_SYS_MODE_MASK_WCDMA | SYS_SYS_MODE_MASK_LTE | SYS_SYS_MODE_MASK_TDS))
        {
          rr_multisim_info.subscription_caps.subs_capability[as_index] = SUBS_CAPABILITY_MULTIMODE;
        }
        else
        {
          rr_multisim_info.subscription_caps.subs_capability[as_index] = SUBS_CAPABILITY_GSM_ONLY;
        }
      }
    }
  }
  else
  {
    /* no message provided, initialise the storage to NULL defaults */
    memset(&(rr_multisim_info.subscription_caps), 0, sizeof(rr_multisim_info.subscription_caps));
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_process_multimode_subs_change

DESCRIPTION   Performs processing for RR_MULTIMODE_SUBS_CHGD_REQ message

DEPENDENCIES  None

RETURN VALUE  SYS_MODEM_AS_ID_NONE (message does not carry any ASID)

SIDE EFFECTS  None

===========================================================================*/
static sys_modem_as_id_e_type rr_ms_process_multimode_subs_change(const rr_multimode_subs_chgd_req_T *msg_ptr)
{
  /* process the message and store the subscription capabilities */
  rr_ms_store_subscription_capabilities(msg_ptr);

  /* send a response to the NAS primitive immediately */
  rr_mm_send_multimode_subs_chgd_cnf();

  return(SYS_MODEM_AS_ID_NONE);
}

/*===========================================================================

FUNCTION     rr_ms_check_pending_response

DESCRIPTION  Checks whether a GL1 response is still pending and notifies the
             Priority Inversion Manager if necessary

DEPENDENCIES None

RETURN VALUE TRUE if a GL1 response is outstanding

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_check_pending_response(const rr_ms_interface_check_t interface_check, const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    rr_l1_message_id_enum_T pending_msg;

    /* determine which message is pending from GL1 */
    pending_msg = rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].required_l1_message;

    if ((MPH_POWER_SCAN_CNF           == pending_msg) ||
        (MPH_SELECT_SPECIFIC_BCCH_CNF == pending_msg) ||
        (MPH_RESELECTION_CNF          == pending_msg))
    {
      /* GRR is waiting for (re)selection to complete, so trigger */
      /* the Priority Inversion Manager with an appropriate event */
      /* or signal to Layer 1 that it should abort this procedure */
      if (RR_MS_ABORT_IS_REQUIRED == interface_check)
      {
        MSG_GERAN_HIGH_1_G("GRR still pending GL1 msg 0x%02X, abort required", (int)pending_msg);
        rr_ms_recover_blocked_interface(gas_id);
      }
      else
      {
        MSG_GERAN_HIGH_1_G("GRR still pending GL1 msg 0x%02X, notify PIMMS", (int)pending_msg);
        rr_ms_acquisition_manager(RR_MS_GAS_ID_RESPONSE_PENDING, gas_id);
      }
      return(TRUE);
    }
    else if (RR_MS_INVALID_L1_MESSAGE != pending_msg)
    {
      MSG_GERAN_ERROR_1_G("Not expecting GRR to wait for GL1 msg 0x%02X", (int)pending_msg);
    }
  }

  return(FALSE);
}


/*===========================================================================

FUNCTION     rr_ms_abort_pending_interface

DESCRIPTION  Called when a GRR abort of cell (re)selection is about to occur
             and checks whether it will affect a pending interface

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static rr_ms_action_t rr_ms_abort_pending_interface(const rr_ms_debug_event_t debug_event, const gas_id_t gas_id)
{
  /* log the specified event in the debug buffer */
  rr_ms_store_debug_event(debug_event, gas_id);

  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /* ensure that aborts are only processed once  */
    if (FALSE == rr_ms_interface_was_aborted(gas_id))
    {
      /* check the interface for a pending response, and abort it if necessary */
      if (rr_ms_check_pending_response(RR_MS_ABORT_IS_REQUIRED, gas_id))
      {
        MSG_GERAN_HIGH_1_G("GRR internal abort event %d while response pending from GL1", (int)debug_event);
      }
    }

    /* if the interface has been suspended previously, resume it now */
    if (RR_MS_INTERFACE_ACTIVE != rr_multisim_info.gas_data[as_index].gl1_interface_state)
    {
      return(RR_MS_PREPARE_FOR_RESUMPTION);
    }
  }

  return(RR_MS_NO_ACTION);
}

/*===========================================================================

FUNCTION      rr_ms_process_nas_msg

DESCRIPTION   Performs Dual SIM preprocessing for an incoming NAS message

DEPENDENCIES  None

RETURN VALUE  GERAN access stratum ID

SIDE EFFECTS  None

===========================================================================*/
static gas_id_t rr_ms_process_nas_msg(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  sys_modem_as_id_e_type nas_id;
#ifdef FEATURE_PRIORITIZE_LTE_FREQ
  sys_modem_as_id_e_type curr_dds_id;
#endif /* FEATURE_PRIORITIZE_LTE_FREQ */

  /* most NAS messages have an Active Subscription ID in them */
  switch(cmd_ptr->message_header.message_id)
  {
    case RR_ABORT_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_abort_req);
      break;

    case RR_ACT_REQ:
      /* this primitive has been deprecated by NAS */
      nas_id = SYS_MODEM_AS_ID_NONE;
      MSG_GERAN_ERROR_0("GRR has received unexpected RR_ACT_REQ from NAS");
      break;

#if defined(FEATURE_WCDMA) || defined(FEATURE_GSM_TDS) || defined(FEATURE_LTE)
    case RR_ACTIVATION_RSP:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_activation_rsp);
      MSG_GERAN_HIGH_2(
        "ASID 0x%02X is in dual RAT mode, PS=%d",
        (int)nas_id,
        (int)(cmd_ptr->rr_activation_rsp.PS_Attached)
      );
      break;
#endif /* FEATURE_WCDMA || FEATURE_GSM_TDS || FEATURE_LTE */

    case RR_DATA_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_data_req);
      break;

    case RR_DEACT_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_deact_req);
      /* Abort interface, if GRR is waiting for any pending message */
      (void) rr_ms_abort_pending_interface(RR_MS_GRR_ABORT_BY_NAS, geran_map_nas_id_to_gas_id(nas_id));
      break;

    case RR_EST_REQ:
      nas_id = rr_ms_validate_connection_request(cmd_ptr, exec_ptr);
      rr_ms_set_connection_priority(cmd_ptr->mm.rr_est_req.proc_type, FALSE, geran_map_nas_id_to_gas_id(nas_id));
      break;

    case RR_PLMN_LIST_REQ:
      nas_id = rr_ms_validate_plmn_search_request(cmd_ptr, exec_ptr);
      break;

    case RR_PLMN_SELECT_REQ:
      /* extract the NAS ID and validate the message content */
      nas_id = rr_ms_validate_service_request(cmd_ptr, exec_ptr);
      break;

    case RR_FORBIDDEN_LAI_UPDATE_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_forbidden_lai_update_req);
      break;

    case RR_SIM_INSERTED_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_sim_inserted_req);
      break;

    case RR_INVALIDATE_SIM_DATA_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_invalidate_sim_data_req);
      break;

    case RR_STOP_GSM_MODE_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_stop_gsm_mode_req);
      /* additional debug useful to trap ASID mismatches etc. */
      MSG_GERAN_HIGH_3("RR_STOP_GSM_MODE_REQ: reason=%d (MODE=%d) for ASID 0x%02X",
                       (int)(cmd_ptr->mm.rr_stop_gsm_mode_req.rr_mm_gsm_stop_reason),
                       (int)geran_get_multi_sim_mode(),
                       (int)nas_id);
      /* Abort interface, if GRR is waiting for any pending message */
      (void) rr_ms_abort_pending_interface(RR_MS_GRR_ABORT_BY_NAS, geran_map_nas_id_to_gas_id(nas_id));
      break;

    case RR_SIM_UPDATE_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_sim_update_req);
      break;

    case RR_EQ_PLMN_LIST_CHANGE_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_eq_plmn_list_change_ind);
      break;

    case RR_BPLMN_SEARCH_ABORT_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_bplmn_search_abort_req);
      break;

    case RR_CHANGE_MODE_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_change_mode_ind);
      break;

    case RR_IRAT_ALLOWED_STATUS_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_irat_allowed_status_ind);
      break;

    case RR_LAI_REJECT_LIST_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_lai_reject_list_ind);
      break;

    case RR_CSFB_BLIND_REDIR_TO_LTE_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_csfb_blind_redir_to_lte_req);
      break;

    case RR_CSFB_CALL_STATUS_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_csfb_call_status_ind);
      break;

    case RR_UE_MODE_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_ue_mode_ind);
      break;

    case RR_CSG_WHITE_LIST_UPDATE_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_csg_white_list_update_ind);
      break;

    case RR_GMM_GPRS_STATE_CHANGE_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_gprs_state_change_req);
      break;

    case RR_GMM_GPRS_TLLI_ASSIGNMENT_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_gprs_tlli_assignment_req);
      break;

    case RR_GMM_READY_TIMER_STATE_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_ready_timer_state_req);
      break;

    case RR_GMM_GPRS_MS_RA_CAP_IE_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_gprs_ms_ra_cap_ie_ind);
      break;

    case RR_GMM_START_MM_NON_DRX_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_start_mm_non_drx_ind);
      /* revoke all CBCH priority inversion now */
      break;

    case RR_GMM_STOP_MM_NON_DRX_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_stop_mm_non_drx_ind);
      break;

    case RR_GMM_GPRS_SERVICE_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_gprs_service_req);
      break;

    case RR_GMM_GPRS_NV_PARAMS_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_gprs_nv_params_ind);
      break;

    case RR_GMM_GPRS_AUTH_FAIL_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_gmm_gprs_auth_fail_ind);
      break;

#ifdef FEATURE_GPRS_PS_HANDOVER
    case RR_PSHO_CNF:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_psho_cnf);
      break;
#endif /* FEATURE_GPRS_PS_HANDOVER */

    case RR_DS_STATUS_CHANGE_IND:
      /* process the new indication and check for changes */
      nas_id = rr_ms_status_change_handler(&(cmd_ptr->mm.rr_ds_status_change_ind));
      break;

    case RR_PS_ABORT_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_ps_abort_req);
      break;

    case RR_PS_RESUME_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_ps_resume_ind);
      break;

    case RR_NW_SEL_MODE_RESET_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_nw_sel_mode_reset_ind);
      break;

    case RR_SERVICE_STATUS_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_service_status_ind);
      break;

    case RR_MULTIMODE_SUBS_CHGD_REQ:
      nas_id = rr_ms_process_multimode_subs_change(&(cmd_ptr->mm.rr_multimode_subs_chgd_req));
      break;

#ifdef FEATURE_FAST_G2L_PSEUDO_SI2Q
    case RR_PSEUDO_LTE_LIST_UPDATE_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_pseudo_lte_list_update_ind);
      break;
#endif /* FEATURE_FAST_G2L_PSEUDO_SI2Q */

#ifdef FEATURE_MM_PROVIDES_CALL_STATUS_IND
    case RR_MMTEL_CALL_STATUS_IND:
      /* extract the NAS ID and store the message content */
      nas_id = rr_ms_update_volte_call_status(cmd_ptr, exec_ptr);
      break;
#endif /* FEATURE_MM_PROVIDES_CALL_STATUS_IND */

    case RR_PROCEDURE_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_procedure_ind);
      break;    

    case RR_MT_CSFB_FAILURE_IND:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->mm.rr_mt_csfb_failure_ind);
      break;

    case RR_DDS_SUB_IND:
#ifdef FEATURE_PRIORITIZE_LTE_FREQ
      curr_dds_id = geran_get_current_dds_sub_id();
#endif /* FEATURE_PRIORITIZE_LTE_FREQ */
      MSG_GERAN_HIGH_1("RR_DDS_SUB_IND: new as_id: %d", (int)cmd_ptr->mm.rr_dds_sub_ind.as_id);
#ifdef FEATURE_PRIORITIZE_LTE_FREQ
      if (curr_dds_id != SYS_MODEM_AS_ID_NONE)
      {
        MSG_GERAN_HIGH_0("[LTE Prio] DDS SUB changed");
        rr_general_stop_lte_prioritization(geran_map_nas_id_to_gas_id(curr_dds_id));
      }
#endif /* FEATURE_PRIORITIZE_LTE_FREQ */
      geran_set_current_dds_sub_id(cmd_ptr->mm.rr_dds_sub_ind.as_id);
      nas_id = SYS_MODEM_AS_ID_NONE;
      break;
	  
    default:
      nas_id = SYS_MODEM_AS_ID_NONE;
      MSG_GERAN_ERROR_1("Unhandled NAS signal 0x%02X", (int)(cmd_ptr->message_header.message_id));
      break;
  }

  /* handle the cases where the signal does not carry any subscription ID */
  if (SYS_MODEM_AS_ID_NONE == nas_id)
  {
    exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
    exec_ptr->next_action   = RR_MS_NO_ACTION;

    /* insert a debug trace record for the primary GAS ID only, */
    /* since the main RR state machine will not log the message */
    rr_debug_log_imsg(cmd_ptr->message_header.message_set,
                      cmd_ptr->message_header.message_id,
                      0, GERAN_ACCESS_STRATUM_ID_1);

    return(GERAN_ACCESS_STRATUM_ID_UNDEFINED);
  }

  return(geran_map_nas_id_to_gas_id(nas_id));
}

/*===========================================================================

FUNCTION     rr_ms_check_pending_service_request

DESCRIPTION  Checks whether a PLMN_SELECT_REQ received during suspension is
             stale, and aborts it if necessary

DEPENDENCIES Should only be called for a suspended access stratum

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_check_pending_service_request(const uint16 time_now, const gas_id_t gas_id)
{
  int as_index;

  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  if ((rr_multisim_info.gas_data[as_index].svc_rqst_time != 0) &&
      (rr_multisim_info.gas_data[as_index].svc_rqst_time < time_now))
  {
    uint16 request_age;

    request_age = time_now - rr_multisim_info.gas_data[as_index].svc_rqst_time;
    if (request_age >= RR_MS_SERVICE_REQUEST_TIMEOUT)
    {
      sys_modem_as_id_e_type nas_id;

      /* abort the last buffered request and reset the staleness counter */
      nas_id = geran_map_gas_id_to_nas_id(gas_id);
      rr_multisim_info.gas_data[as_index].svc_rqst_time = 0;
      MSG_GERAN_HIGH_0_G("Aborting pending NAS svc request");
      rr_ms_send_rr_plmn_select_cnf(MM_AS_CONNECTED, MM_RR_NO_RESOURCE_AVAILABLE, TRUE, nas_id);
    }
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_abort_pending_service_request

DESCRIPTION  Clears down any pending service request from NAS received during
             suspension

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_abort_pending_service_request(const boolean reply_to_nas, const gas_id_t gas_id)
{
  int as_index;

  /* verify that the current access stratum is still suspended */
  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  if (rr_multisim_info.gas_data[as_index].gl1_interface_state != RR_MS_INTERFACE_ACTIVE)
  {
    uint16 effective_time;

    if (TRUE == reply_to_nas)
    {
      /* create a timestamp (in seconds) that is guaranteed to make any */
      /* pending service request look like it is too old, and abort it  */
      effective_time = RR_MS_HEARTBEAT_LIMIT + RR_MS_SERVICE_REQUEST_TIMEOUT + 1;
      rr_ms_check_pending_service_request(effective_time, gas_id);
    }
    else
    {
      effective_time = rr_multisim_info.gas_data[as_index].svc_rqst_time;
      /* reset the counter to mark the request as invalid */
      rr_multisim_info.gas_data[as_index].svc_rqst_time = 0;
      MSG_GERAN_HIGH_1_G("Discarded RR_PLMN_SELECT_REQ cached at time %d", (int)effective_time);
    }
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_connected_gas_id

DESCRIPTION   Returns the GAS ID of the first data space that is CS connected

DEPENDENCIES  None

RETURN VALUE  GAS ID of connected stack, or GERAN_ACCESS_STRATUM_ID_UNDEFINED

SIDE EFFECTS  None

===========================================================================*/
static gas_id_t rr_ms_connected_gas_id(void)
{
  int as_index;

  for (as_index = GERAN_DATA_SPACE_INDEX_1; as_index < NUM_GERAN_DATA_SPACES; as_index++)
  {
    if (rr_multisim_info.gas_data[as_index].cs_connected)
    {
      /* this stack has a CS connection */
      return(GERAN_MAP_DATA_SPACE_INDEX_TO_GAS_ID(as_index));
    }
  }

  return(GERAN_ACCESS_STRATUM_ID_UNDEFINED);
}

/*===========================================================================

FUNCTION     rr_ms_check_required_l1_response

DESCRIPTION  Checks whether GRR was waiting for a specific response from GL1

DEPENDENCIES None

RETURN VALUE TRUE if the specified GAS ID was waiting for this response

SIDE EFFECTS May trigger resumption procedure for the specified GAS ID

===========================================================================*/
static boolean rr_ms_check_required_l1_response(const rr_l1_message_id_enum_T rcvd_msg, rr_ms_executor_t *exec_ptr, const gas_id_t gas_id)
{
  boolean bMatched;
  int     as_index;

  /* check the response only if it is flagged to be processed further */
  if (RR_PROCESS_MSG_NEVER == exec_ptr->process_in_rr)
  {
    MSG_GERAN_HIGH_1_G("GRR is ignoring GL1 msg 0x%02X", (int)rcvd_msg);
    return(FALSE);
  }

  /* determine the appropriate data space index */
  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  /* by default, no additional action needs to occur */
  exec_ptr->next_action = RR_MS_NO_ACTION;

  if (rcvd_msg == rr_multisim_info.gas_data[as_index].required_l1_message)
  {
    /* this response matches the one GRR is waiting for */
    rr_multisim_info.gas_data[as_index].required_l1_message = RR_MS_INVALID_L1_MESSAGE;
    bMatched = TRUE;

    /* if the AS had already been suspended, now is the time to resume it */
    if (rr_multisim_info.gas_data[as_index].gl1_interface_state != RR_MS_INTERFACE_ACTIVE)
    {
      exec_ptr->next_action = RR_MS_PREPARE_FOR_RESUMPTION;
    }

    /* log an appropriate event in the debug buffer */
    rr_ms_store_debug_event(RR_MS_RESPONSE_RECEIVED, gas_id);

    MSG_GERAN_HIGH_1_G("Received GL1 msg 0x%02X", (int)rcvd_msg);
  }
  else
  {
    bMatched = FALSE;
  }

  return(bMatched);
}

/*===========================================================================

FUNCTION     rr_ms_handle_panic_reset_message

DESCRIPTION  Handles one of the GL1->GRR panic reset primitives

DEPENDENCIES None

RETURN VALUE GAS ID where the signal was received

SIDE EFFECTS None

===========================================================================*/
static gas_id_t rr_ms_handle_panic_reset_message(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  gas_id_t gas_id;

  /* extract the GAS ID */
  if (MPH_RESET_IND == cmd_ptr->message_header.message_id)
  {
    gas_id = cmd_ptr->mph_reset_ind.gas_id;
  }
  else if (MPH_RESET_CNF == cmd_ptr->message_header.message_id)
  {
    gas_id = cmd_ptr->mph_reset_cnf.gas_id;
  }
  else
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
  }

  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /* cancel the timers that monitor L1 interface responses */
    (void)rr_timer_cancel(RR_DUAL_SIM_TERMINATE_TIMER, gas_id);
    (void)rr_timer_cancel(RR_DUAL_SIM_INACTIVATE_TIMER, gas_id);
    (void)rr_timer_cancel(RR_DUAL_SIM_BPLMN_LIST_TIMER, gas_id);

    rr_ms_acquisition_manager(RR_MS_PRIORITY_INVERSION_INIT, gas_id);

    /* use this opportunity to clear down any local flags */
    rr_multisim_info.gas_data[as_index].aborted_l1_interface = FALSE;
    rr_multisim_info.gas_data[as_index].required_l1_message  = RR_MS_INVALID_L1_MESSAGE;
    rr_multisim_info.gas_data[as_index].gprs_test_mode       = FALSE;
    rr_multisim_info.gas_data[as_index].ps_abort_delay       = 0;
    (void)rr_timer_cancel(RR_DUAL_SIM_PS_ABORT_TIMER_1, gas_id);
    (void)rr_timer_cancel(RR_DUAL_SIM_PS_ABORT_TIMER_2, gas_id);

    /* ensure that tuneaway blocking status reverts to default */
    geran_api_set_tuneaway_blocked(GERAN_API_CLIENT_RESET, FALSE, geran_map_gas_id_to_nas_id(gas_id));
  }

  /* next action is to ensure the access stratum is resumed */
  exec_ptr->next_action = RR_MS_PREPARE_FOR_RESUMPTION;

  return(gas_id);
}

/*===========================================================================
FUNCTION     rr_ms_handle_preemption_ind

DESCRIPTION  Handles the MPH_PREEMPTION_IND primitive from Layer 1

DEPENDENCIES None

RETURN VALUE GAS ID where the signal was received

SIDE EFFECTS None

===========================================================================*/
static gas_id_t rr_ms_handle_preemption_ind(const mph_preemption_ind_T *ind_ptr, rr_ms_executor_t *exec_ptr)
{
  if (ind_ptr != NULL)
  {
    gas_id_t gas_id = ind_ptr->gas_id;

    if (RR_L1_CS_PREEMPTION == ind_ptr->preemption_type)
    {
      /* forward this CS indication to NAS */
      rr_mm_send_rr_preemption_ind(gas_id);
    }
    else if(RR_L1_PS_PDU_TUNEAWAY_OVERRUN == ind_ptr->preemption_type)
    {
       MSG_GERAN_HIGH_0_G("rr_gmm_retx_pdu_tuneaway_fail_ind forwarded to NAS");

        /* forward this CS indication to NAS */
        rr_gmm_retx_pdu_tuneaway_fail_ind(gas_id);
    }
    else
    {
      /* for future enhancement: a Layer 1 "PS Abort" could also be handled here */
      MSG_GERAN_ERROR_1_G("Unhandled preemption type %d", (int)(ind_ptr->preemption_type));
    }

    /* main state machine does not need to process the message any further */
    if (exec_ptr != NULL)
    {
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      exec_ptr->next_action   = RR_MS_NO_ACTION;
    }

    return(gas_id);
  }

  return(GERAN_ACCESS_STRATUM_ID_UNDEFINED);
}

/*===========================================================================

FUNCTION     rr_ms_type_approval_active

DESCRIPTION  Determines whether TYPE_APPROVAL operations are being exercised
             in Multi-Standby mode (i.e. either DSDS or TSTS)

DEPENDENCIES None

RETURN VALUE TRUE if the software detects G+G (or G+G+G) multi-SIM activity
             with all active stacks configured with Type Approval SIM cards

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_type_approval_active(void)
{
#ifdef FEATURE_DUAL_SIM_TYPE_APPROVAL_SET
  if (geran_in_multi_standby_mode())
  {
    gas_id_t gas_id;
    byte     op_mode_list[NUM_GERAN_DATA_SPACES];
    int      ta_count = 0;

    /* retrieve the MS operation mode values (defined on each SIM) */
    rr_ms_get_sim_operation_modes(op_mode_list, NUM_GERAN_DATA_SPACES);

    for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
    {
      /* check whether GL1 has been activated on this stack */
      if (rr_mode_gsm_active_or_starting(gas_id))
      {
        /* check whether the corresponding SIM has the Type Approval bit set */
        if ((op_mode_list[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)] & 0x80) == 0x00)
        {
          /* active stack but without Type Approval bit set */
          return(FALSE);
        }
        else
        {
          ta_count++;
        }
      }
    }

    /* check if there are 2 or more active GERAN stacks with Type Approval bit set on the SIM */
    if (ta_count >= 2)
    {
      return(TRUE);
    }
  }
#endif /* FEATURE_DUAL_SIM_TYPE_APPROVAL_SET */

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_gprs_test_mode_state_change

DESCRIPTION  Performs DSDS-specific actions when GPRS Test Mode activity occurs

DEPENDENCIES NONE

RETURN VALUE VOID

SIDE EFFECTS May extend the Sys Info refresh timer on the non-GPRS subscription

===========================================================================*/
static void rr_ms_gprs_test_mode_state_change(const boolean gtm_started, const gas_id_t gas_id)
{
  /* record the new state of GPRS Test Mode on the specified subscription */
  rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].gprs_test_mode = gtm_started;

  /* GPRS Test Mode activity has occurred on a GERAN stack */
  /* so GRR must also avoid sys info refresh activities on */
  /* any other stack which needs access to the transceiver */
  if (RR_MULTI_SIM_MODE_ENABLED)
  {
    gas_id_t other_gas_id;

    for (other_gas_id = GERAN_ACCESS_STRATUM_ID_1; other_gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; other_gas_id++)
    {
      if (other_gas_id != gas_id)
      {
        /* cancel the sys info refresh timer on the other stack */
        if (rr_timer_cancel(GRR_SYS_INFO_REFRESH_TIMER, other_gas_id))
        {
          /* and then restart it with an extended duration if it was already running */
          if (rr_timer_start(GRR_SYS_INFO_REFRESH_TIMER, GRR_TIMEOUT_120_SECONDS, other_gas_id))
          {
            MSG_GERAN_HIGH_1("gs%1d: Deferring sys info refresh timer", 1+(int)other_gas_id);
          }
        }
      }
    }
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_get_timestamp

DESCRIPTION  Gets the current slow clock tick count, adjusted for local use

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static timetick_type rr_ms_get_timestamp(const timetick_type input_time)
{
  timetick_type output_time;

  /* query the current timetick (slow clock) count */
  output_time = input_time;
  if (output_time == RR_MS_INVALID_TIME_STAMP)
  {
    /* increment the timestamp so that zero can be used as an invalid value */
    output_time++;
  }

  return(output_time);
}

/*===========================================================================

FUNCTION     rr_ms_two_stacks_have_same_paging_group

DESCRIPTION  Checks whether two stacks have the same (valid) paging group

DEPENDENCIES Relies on paging group data stored in this module

RETURN VALUE TRUE if the stored paging groups are equal, and valid

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_two_stacks_have_same_paging_group(const gas_id_t gas_id_1st, const gas_id_t gas_id_2nd)
{
  int as_index_1 = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id_1st);
  int as_index_2 = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id_2nd);

  if ((rr_multisim_info.gas_data[as_index_1].paging_group == rr_multisim_info.gas_data[as_index_2].paging_group) &&
      (rr_multisim_info.gas_data[as_index_1].paging_group != RR_MS_INVALID_PAGING_GROUP))
  {
    return(TRUE);
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION      rr_ms_two_stacks_have_same_lai

DESCRIPTION   Checks whether two GERAN access strata are in the same Location Area

DEPENDENCIES  None

RETURN VALUE  TRUE if two stacks have the same Location Area Information

SIDE EFFECTS  None

===========================================================================*/
boolean rr_ms_two_stacks_have_same_lai(const gas_id_t gas_id_1st, const gas_id_t gas_id_2nd)
{
  boolean same_lai = FALSE;

  if (rr_mode_gsm_active_or_starting(gas_id_1st) &&
      rr_mode_gsm_active_or_starting(gas_id_2nd))
  {
    gprs_scell_info_t *scell_1_ptr = rr_gprs_get_scell_info(gas_id_1st);
    gprs_scell_info_t *scell_2_ptr = rr_gprs_get_scell_info(gas_id_2nd);

    if ((scell_1_ptr != NULL) && (scell_2_ptr != NULL))
    {
       same_lai = rr_same_lai(&(scell_1_ptr->gsm.location_area_identification),
                              &(scell_2_ptr->gsm.location_area_identification));
    }
  }

  return(same_lai);
}
 
/*===========================================================================

FUNCTION     rr_ms_other_stack_has_same_lai

DESCRIPTION  Briefly checks if there are any other satck with Same LAI

DEPENDENCIES None

RETURN VALUE TRUE id other stacks in same LAI else FALSE

SIDE EFFECTS None

===========================================================================*/
boolean rr_ms_other_stack_has_same_lai(const gas_id_t gas_id)
{
  gas_id_t other_gas_id;

  for (other_gas_id = GERAN_ACCESS_STRATUM_ID_1; other_gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; other_gas_id++)
  {
    if (other_gas_id != gas_id)
    {
      if (rr_ms_two_stacks_have_same_lai(gas_id, other_gas_id))
      {
        return(TRUE);
      }
    }
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_page_in_sharing_window

DESCRIPTION  Calculates delta between missed page and a PCH decode event

DEPENDENCIES None

RETURN VALUE TRUE if the missed page event occurred within 80ms of the PCH

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_page_in_sharing_window(const timetick_type pch_decode_time, const timetick_type pch_missed_time, const timetick_type page_window)
{
  if ((pch_decode_time != RR_MS_INVALID_TIME_STAMP) &&
      (pch_missed_time != RR_MS_INVALID_TIME_STAMP) &&
      (page_window     != 0))
  {
    timetick_type elapsed_ms;

    if (pch_missed_time > pch_decode_time)
    {
      /* missed page was reported for a DRX cycle in the future */
      elapsed_ms = timetick_diff(pch_decode_time, pch_missed_time, T_MSEC);
    }
    else
    {
      /* missed page was reported prior to the last PCH time */
      elapsed_ms = timetick_diff(pch_missed_time, pch_decode_time, T_MSEC);
    }

    /* output debug for calculations */
    MSG_GERAN_HIGH_3("Page at time %d Miss at time %d Elapsed ms %d",
             (int)pch_decode_time, (int)pch_missed_time, (int)elapsed_ms);

    return((boolean)(elapsed_ms < page_window));
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_share_paging_message

DESCRIPTION  Checks whether a page message can be shared between stacks

DEPENDENCIES Page must have already been cached for the source GAS ID

RETURN VALUE VOID

SIDE EFFECTS TRUE if a page message was shared

===========================================================================*/
static boolean rr_ms_share_paging_message(const gas_id_t dest_gas_id, const gas_id_t source_gas_id)
{
  /* page sharing is only valid for GERAN stacks camped in the same Location Area */
  if (rr_ms_two_stacks_have_same_lai(dest_gas_id, source_gas_id))
  {
    /* determine a data space index for each GAS ID */
    int source_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(source_gas_id);
    int dest_index   = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(dest_gas_id);

    /* determine whether the missed page event lies in the acceptable window */
    /* which has been calculated using the destination client's paging cycle */
    if (rr_ms_page_in_sharing_window(rr_multisim_info.gas_data[source_index].buffered_page_time,
                                     rr_multisim_info.gas_data[dest_index].missed_page,
                                     rr_multisim_info.gas_data[dest_index].page_cycle))
    {
      dl_unit_data_ind_T *msg_ptr;
      dl_unit_data_ind_T  shared_page;
      gprs_scell_info_t  *scell_ptr = rr_gprs_get_scell_info(dest_gas_id);

      /* copy the page message so that the other stack can use it */
      msg_ptr = &(rr_multisim_info.gas_data[source_index].buffered_page_msg);
      shared_page = *(msg_ptr);

      shared_page.shared_msg = TRUE;
      shared_page.gas_id     = dest_gas_id;

      if (scell_ptr != NULL)
      {
        /* This code copes with two scenarios affecting OPB (Own Paging Block): */
        /* 1) the SIMs are camped on the same cell, but have different paging groups */
        /* => in this case the OPB setting must be FALSE */
        /* 2) the SIMs are on two different cells */
        /* => in this case the OPB setting must also be FALSE, and the serving ARFCN must be set correctly */
        /* The OPB setting is important because it influences paging mode - it is NOT desirable to force both */
        /* stacks into paging reorganisation if a cached paged is shared due to this DSDS optimisation */
        if (ARFCNS_EQUAL(msg_ptr->channel_number, scell_ptr->gsm.BCCH_ARFCN))
        {
          /* both stacks are on the same serving cell, but page groups could be different */
          if (rr_ms_two_stacks_have_same_paging_group(dest_gas_id, source_gas_id) == FALSE)
          {
            shared_page.own_paging_block = FALSE;
          }
        }
        else
        {
          /* cached page was received on a different cell (but in the same Location Area) */
          shared_page.channel_number   = scell_ptr->gsm.BCCH_ARFCN;
          shared_page.own_paging_block = FALSE;
        }
      } /* end if valid scell */

      /* clear the missed page event and decrement the unknown block count */
      rr_multisim_info.gas_data[dest_index].missed_page = RR_MS_INVALID_TIME_STAMP;

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
      /* and then post the shared page to the RR task queue */
      if (GS_SUCCESS == gs_send_message(GS_QUEUE_RR, (void *)&shared_page, FALSE))
      {
        return(TRUE);
      }
      else
      {
        MSG_ERROR("Unable to send msg for shared PCH decode",0,0,0);
      }
    } /* end if in page sharing window */
  } /* end if same LAI */

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_page_message_received

DESCRIPTION  Checks whether a DL_UNIT_DATA_IND can be shared between stacks

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_page_message_received(const dl_unit_data_ind_T *msg_ptr)
{
  if (msg_ptr != NULL)
  {
    /* filter out any message types that cannot be idle mode paging */
    if ((msg_ptr->own_paging_block == TRUE)  &&
        (msg_ptr->shared_msg       == FALSE) &&
        (msg_ptr->short_header     == FALSE) &&
        (msg_ptr->l2_channel_type  != SACCH))
    {
      byte msg_type;

      /* check whether this is actually a page message */
      msg_type = msg_ptr->layer3_message[2];
      if ((PAGING_REQUEST_TYPE_1 == msg_type) ||
          (PAGING_REQUEST_TYPE_2 == msg_type) ||
          (PAGING_REQUEST_TYPE_3 == msg_type))
      {
        gas_id_t source_gas_id; /* the stack for which the message was actually decoded by GL1 */
        gas_id_t gas_id;        /* access stratum IDs for one of the other possible recipients */
        int      source_index;

        /* buffer the incoming page and the record the time it arrived */
        source_gas_id = msg_ptr->gas_id;
        source_index  = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(source_gas_id);
        rr_multisim_info.gas_data[source_index].buffered_page_time = rr_ms_get_timestamp(timetick_get());
        rr_multisim_info.gas_data[source_index].buffered_page_msg  = *(msg_ptr);

        /* loop through all other subscriptions and check whether any of them require the newly cached message */
        for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
        {
          if (gas_id != source_gas_id)
          {
            if (rr_ms_share_paging_message(gas_id, source_gas_id))
            {
              MSG_GERAN_HIGH_1_G("Using PCH read by gs%1d", 1+(int)source_gas_id);
            }
          } /* end GAS ID check */
        } /* end for loop */
      } /* end if paging message */
    } /* end if channel criteria */
  } /* end if null pointer */

  return;
}

#ifdef FEATURE_TRIPLE_SIM
/*===========================================================================

FUNCTION     rr_ms_prioritise_page_sources

DESCRIPTION  Generates a prioritised list of GAS IDs. The first ID in the
             list represents the best candidate for page sharing for the
             "destination" GAS ID specified as the second parameter.

DEPENDENCIES No prioritisation is required for trivial DSDS scenarios so
             this function only exists when TSTS is compiled in the build

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_prioritise_page_sources(gas_id_t *gas_id_list, const gas_id_t gas_id)
{
  gas_id_t *best_ptr;
  gas_id_t *curr_ptr;
  gas_id_t  source_gas_id;
  int       best_pri;
  int       curr_pri;
  int       as_index;

  /* start with the list of GAS IDs in default numerical order */
  for (as_index = 0; as_index < NUM_GERAN_DATA_SPACES; as_index++)
  {
    gas_id_list[as_index] = GERAN_MAP_DATA_SPACE_INDEX_TO_GAS_ID(as_index);
  }

  /* initialise "best so far" tracking variables */
  best_ptr = NULL;
  best_pri = RR_PAGE_SOURCE_PRIORITY_LOW;
  curr_ptr = gas_id_list;

  /* check whether any particular GAS ID has criteria that makes it high priority */
  for (as_index = 0; as_index < NUM_GERAN_DATA_SPACES; as_index++, curr_ptr++)
  {
    source_gas_id = *curr_ptr;

    /* calculate priority for the current entry if it is NOT the requesting stack */
    if (source_gas_id != gas_id)
    {
      gprs_scell_info_t *src_scell_ptr = rr_gprs_get_scell_info(source_gas_id);
      gprs_scell_info_t *dst_scell_ptr = rr_gprs_get_scell_info(gas_id);

      /* default priority is LOW */
      curr_pri = RR_PAGE_SOURCE_PRIORITY_LOW;

      if ((src_scell_ptr != NULL) && (dst_scell_ptr != NULL))
      {
        if (ARFCNS_EQUAL(src_scell_ptr->gsm.BCCH_ARFCN, dst_scell_ptr->gsm.BCCH_ARFCN))
        {
          if (rr_ms_two_stacks_have_same_paging_group(source_gas_id, gas_id))
          {
            /* camped on same cell and same paging group, high priority */
            curr_pri = RR_PAGE_SOURCE_PRIORITY_HIGH;
          }
          else
          {
            /* camped on same cell, different paging groups, medium priority */
            curr_pri = RR_PAGE_SOURCE_PRIORITY_MED;
          }
        }
      }

      /* check whether this is the highest priority found so far */
      if (curr_pri > best_pri)
      {
        best_pri = curr_pri;
        best_ptr = curr_ptr;
      }
    }
  }

  /* if the highest priority entry is NOT at the top of the list, swap it */
  curr_ptr = gas_id_list;
  if ((best_ptr != NULL) &&
      (curr_ptr != NULL) &&
      (best_ptr != curr_ptr))
  {
    source_gas_id = *best_ptr;
    *best_ptr     = *curr_ptr;
    *curr_ptr     = source_gas_id;
    MSG_GERAN_HIGH_1_G("Prioritised gs%1d as shared page source", 1+(int)source_gas_id);
  }

  return;
}
#endif /* FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION     rr_ms_page_message_missed

DESCRIPTION  Checks whether PCH missed on one stack can be supplied by another

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_page_message_missed(const timetick_type pch_missed_time, const gas_id_t gas_id)
{
  gas_id_t source_list[NUM_GERAN_DATA_SPACES];
  gas_id_t source_gas_id;
  int      as_index;

  /* record the timestamp provided by GL1 within the MPH_BLOCK_QUALITY_IND message */
  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  rr_multisim_info.gas_data[as_index].missed_page = rr_ms_get_timestamp(pch_missed_time);

  MSG_GERAN_HIGH_1_G("Missed PCH at time %d", (uint32)pch_missed_time);

#ifdef FEATURE_TRIPLE_SIM
  /* generate prioritised list of source GAS IDs */
  rr_ms_prioritise_page_sources(source_list, gas_id);
#else
  /* list of GAS IDs always in default numerical order */
  for (as_index = 0; as_index < NUM_GERAN_DATA_SPACES; as_index++)
  {
    source_list[as_index] = GERAN_MAP_DATA_SPACE_INDEX_TO_GAS_ID(as_index);
  }
#endif /* FEATURE_TRIPLE_SIM */

  for (as_index = 0; as_index < NUM_GERAN_DATA_SPACES; as_index++)
  {
    /* extract the next candidate GAS ID from the prioritised list */
    source_gas_id = source_list[as_index];

    if (source_gas_id != gas_id)
    {
      if (rr_ms_share_paging_message(gas_id, source_gas_id))
      {
        MSG_GERAN_HIGH_1_G("Using PCH read by gs%1d instead", 1+(int)source_gas_id);
        /* only need to acquire the data from one source, exit as soon as found */
        return;
      }
    } /* end GAS ID check */
  } /* end for loop */

  return;
}

/*===========================================================================

FUNCTION     rr_ms_configure_page_skip

DESCRIPTION  Calls GRM to enable/disable adaptive page skipping feature.

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
#if defined(PERLUTF) || defined(TEST_FRAMEWORK)
#error code not present
#else
static void rr_ms_configure_page_skip(const boolean aps_state, const gas_id_t gas_id)
{
  /* Interfaces to TRM are owned by Layer 1 (using GRM wrappers in GCOMMON) */
  grm_update_aps_support(aps_state, gas_id);

  return;
}
#endif /* PERLUTF || TEST_FRAMEWORK */

/*===========================================================================

FUNCTION     rr_ms_check_page_skipping

DESCRIPTION  Calls TRM API to enable/disable adaptive page skipping feature.

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_check_page_skipping(const gl1_block_quality_t block_quality, const gas_id_t gas_id)
{
  gprs_scell_info_t *scell_ptr = rr_gprs_get_scell_info(gas_id);

  if ((GL1_BLOCK_QUALITY_GOOD == block_quality)
    && (NORMAL_PAGING == rr_ce_get_ota_page_mode(gas_id))
    && !rr_gprs_non_drx_mode_requested(gas_id)
    && !rr_is_control_state(RR_CONNECTION_PENDING, gas_id)
    && !rr_gprs_is_dsc_below_max_value(gas_id)
    && (scell_ptr->gsm.control_channel_description.BS_PA_MFRMS <= RR_PAGE_SKIP_MAX_BS_PA_MFRMS))
  {
    rr_ms_configure_page_skip(TRUE, gas_id);
  }
  else
  {
    rr_ms_configure_page_skip(FALSE, gas_id);
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_check_and_start_sys_info_guard_timer

DESCRIPTION  Starts the Sys Info Priority Inversion Guard timer, if not already started, if BCCH is Found

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
void rr_ms_check_and_start_sys_info_guard_timer(const boolean BCCH_found, const gas_id_t gas_id)
{
  if (BCCH_found)
  {
    if (rr_timer_is_running(RR_SYS_INFO_GUARD_TIMER, gas_id))
    {
      MSG_GERAN_HIGH_0_G("Sys info guard timer already running");
    }
    else
    {
      rr_duration_t guard_duration;
      int           as_index;

      /* retrieve the current procedure duration and set the guard timer duration accordingly */
      as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
      if (rr_multisim_info.gas_data[as_index].procedure_duration < RR_MINIMUM_PROCEDURE_DURATION)
      {
        /* procedure is unusually short - set a two frame timer */
        guard_duration = (RR_SINGLE_FRAME_TICK_DURATION << 1);
      }
      else if ((rr_multisim_info.gas_data[as_index].procedure_duration > RR_MINIMUM_PROCEDURE_DURATION ) &&
               (rr_multisim_info.gas_data[as_index].procedure_duration < RR_OPTIONAL_SI_PROCEDURE_DURATION)
              )
      {
        /*Use half duration to check if there is collision*/
        guard_duration = rr_multisim_info.gas_data[as_index].procedure_duration/2;
      }
      else if (rr_multisim_info.gas_data[as_index].procedure_duration == RR_OPTIONAL_SI_PROCEDURE_DURATION)
      {
        /*Use 2*TC cycle duration*/
        guard_duration = RR_OPTIONAL_SI_PROCEDURE_DURATION;      
      }
      else
      {
        /*Use the minimum duration*/
        guard_duration = RR_MINIMUM_PROCEDURE_DURATION;
      }

      if (rr_timer_start(RR_SYS_INFO_GUARD_TIMER, guard_duration, gas_id))
      {
        MSG_GERAN_HIGH_1_G("Started sys info guard timer for %dms", (int)guard_duration);
      }
      else
      {
        MSG_GERAN_ERROR_0_G("Unable to start sys info guard timer");
      }
    }
  }
  else if (rr_timer_cancel(RR_SYS_INFO_GUARD_TIMER, gas_id))
  {
    MSG_GERAN_HIGH_0_G("Sys info guard timer stopped, BCCH not acquired");
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_process_block_quality_report

DESCRIPTION  Handles MPH_BLOCK_QUALITY_IND reports from GSM Layer 1

DEPENDENCIES None

RETURN VALUE GAS ID specified by GSM L1 within the incoming message

SIDE EFFECTS May deliberately prevent the signal reaching the main GRR task

===========================================================================*/
static gas_id_t rr_ms_process_block_quality_report(const mph_block_quality_ind_T *block_ptr, rr_ms_executor_t *exec_ptr)
{
  gas_id_t rcvd_id;

  if ((block_ptr != NULL) && (exec_ptr != NULL))
  {
    gas_id_t gas_id = block_ptr->gas_id;

    /* discard idle block quality reports during Type Approval testing */
    if ((rr_ms_type_approval_active() == TRUE)    &&
        (block_ptr->l2_channel_type   != L2_RACH) &&
        (block_ptr->l2_channel_type   != SACCH))
    {
      /* all GSM stacks have been put into TYPE_APPROVAL mode, */
      /* so prevent the block quality message from reaching RR */
      /* since a DSF procedure in this scenario is undesirable */
      rcvd_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    }
    else
    {
      /* use the GAS ID from the incoming message */
      rcvd_id = gas_id;
    }

    /* if there is no valid GAS ID, avoid further processing */
    if (rcvd_id > GERAN_ACCESS_STRATUM_ID_MAX)
    {
      /* set up the output parameters to avoid further processing */
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      exec_ptr->next_action   = RR_MS_NO_ACTION;
      MSG_GERAN_HIGH_0_G("MPH_BLOCK_QUALITY_IND ignored in multi-SIM mode");
    }
    else
    {
      if (GL1_BLOCK_QUALITY_UNKNOWN == block_ptr->block_quality)
      {
        if (block_ptr->l2_channel_type == BCCH)
        {
          /* increment the system info denial count for this stack */
          rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].sys_info_denial_count++;
          rr_decrement_read_count_for_optional_SI2bis_SI2ter_SI13(gas_id);
          MSG_GERAN_HIGH_0_G("MPH_BLOCK_QUALITY_IND with BCCH type rcvd");
          /* set up the output parameters to avoid further processing */
          exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
          exec_ptr->next_action   = RR_MS_NO_ACTION;
        }
        else
        {
          rr_log_page_missed(gas_id);

          /* this may use a cached page to serve the missing PCH */
          rr_ms_page_message_missed(block_ptr->block_timestamp, rcvd_id);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
          /* For PCH read during own transfer case , GRR should not honour UNKNOWN block so stop it being sent to RR control*/
          if (MAC_TRANSFER == rr_macinfo_get_mac_status(gas_id))
          {
            /* set up the output parameters to avoid further processing */
            exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
            exec_ptr->next_action   = RR_MS_NO_ACTION;
            MSG_GERAN_MED_0_G("CCCH read while in PS transfer, ignore unknown block");
          }
        }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
      }

      if ((block_ptr->l2_channel_type != L2_RACH) && (block_ptr->l2_channel_type != SACCH))
      {
        rr_ms_check_page_skipping(block_ptr->block_quality, gas_id);
      }
    }
  }
  else
  {
    /* Pointer validation failed, return an undefined GAS ID to indicate an error */
    rcvd_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
  }

  return(rcvd_id);
}

/*===========================================================================

FUNCTION       rr_ms_exit_other_stack_from_hibernation

DESCRIPTION    Exit any other stack if in hibernation and heartbeat running

DEPENDENCIES   None

RETURN VALUE   VOID

SIDE EFFECTS   None

===========================================================================*/
static void rr_ms_exit_other_stack_from_hibernation(const gas_id_t connected_gas_id)
{
  gas_id_t gas_id;

  for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
  {
    if (gas_id != connected_gas_id)
    {
      if ((rr_timer_is_running(RR_DUAL_SIM_HEARTBEAT_TIMER, gas_id)) &&
          (RR_MS_INTERFACE_HIBERNATED == rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].gl1_interface_state)) 
      {
        MSG_GERAN_HIGH_0_G("Exiting from hibernation");
        (void)rr_timer_cancel(RR_DUAL_SIM_HEARTBEAT_TIMER, gas_id);
        rr_ms_prepare_for_resumption(TRUE, gas_id);
      }
    }
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_cs_released

DESCRIPTION   Used internally by this module to monitor CS connection release

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
static void rr_ms_cs_released(const gas_id_t gas_id)
{
  /* this GAS ID has just released a dedicated mode connection */
  rr_ms_cs_connected(FALSE, gas_id);

  /* process any buffered serv request if any*/
  rr_ms_exit_other_stack_from_hibernation(gas_id);

  return;
}

/*===========================================================================

FUNCTION     rr_ms_process_power_scan_confirm

DESCRIPTION  Pre-processes the MPH_POWER_SCAN_CNF

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS May extend the cell selection abort timer

===========================================================================*/
static gas_id_t rr_ms_process_power_scan_confirm(const mph_power_scan_cnf_T *cnf_ptr, rr_ms_executor_t *exec_ptr)
{
  gas_id_t gas_id;

  if (cnf_ptr != NULL)
  {
    /* extract the GAS ID and determine whether GRR was waiting for this response */
    gas_id = cnf_ptr->gas_id;
    (void)rr_ms_check_required_l1_response(MPH_POWER_SCAN_CNF, exec_ptr, gas_id);

    /* if the CNF is not an abort response, check the grant statistics */
    if (FALSE == cnf_ptr->invalidate)
    {
      /* update PIMMS to allow any remaining inversion period to be consumed */
      (void)rr_ms_acquisition_manager(RR_MS_GAS_ID_POWER_SCAN_ENDED, gas_id);

      if (rr_timer_is_running(RR_CELL_SELECTION_ABORT_TIMER, gas_id))
      {
        int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

        if ((rr_multisim_info.gas_data[as_index].nominal_grants == 0) &&
            (rr_multisim_info.gas_data[as_index].inverse_grants != 0) &&
            (rr_multisim_info.gas_data[as_index].deferral_count == 0))
        {
          /* only inverted acquisition was successful */
          /* so extend the cell selection timeout now */
          /* if it has less than a specified duration */
          if (rr_timer_get_time_remaining(RR_CELL_SELECTION_ABORT_TIMER, gas_id) < RR_CELL_SELECTION_DEFERRAL_MS)
          {
            /* restart the timer with the extension duration */
            if (rr_timer_start(RR_CELL_SELECTION_ABORT_TIMER, RR_CELL_SELECTION_DEFERRAL_MS, gas_id))
            {
              /* reset the grant statistics for the next phase of acquisition */
              rr_multisim_info.gas_data[as_index].nominal_grants = 0;
              rr_multisim_info.gas_data[as_index].inverse_grants = 0;

              /* increment the count to ensure the timer is not deferred ad infinitum */
              rr_multisim_info.gas_data[as_index].deferral_count++;
              MSG_GERAN_HIGH_1_G("PIMMS: defer cell selection timeout for %d seconds", RR_CELL_SELECTION_DEFERRAL_MS/1000);
            }
          }
        }
        else
        {
          /* report the grant statistics */
          MSG_GERAN_HIGH_3_G("PIMMS: nominal grants %d inverse grants %d deferrals %d",
                             (int)(rr_multisim_info.gas_data[as_index].nominal_grants),
                             (int)(rr_multisim_info.gas_data[as_index].inverse_grants),
                             (int)(rr_multisim_info.gas_data[as_index].deferral_count));
        }
      }
    }
  }
  else
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
  }
 
  return(gas_id);
}

/*===========================================================================

FUNCTION     rr_ms_update_grant_statistics

DESCRIPTION  Updates local statistics for acquisition grants

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_update_grant_statistics(const geran_priority_t grant_priority, const gas_id_t gas_id)
{
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  if (GERAN_PRIORITY_NOMINAL == grant_priority)
  {
    rr_multisim_info.gas_data[as_index].nominal_grants++;
  }
  else if (GERAN_PRIORITY_INVERTED == grant_priority)
  {
    rr_multisim_info.gas_data[as_index].inverse_grants++;
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_handle_acquisition_state_ind

DESCRIPTION  Process the MPH_ACQUISITION_STATE_IND from GL1

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS May generate a new event for PIMMS

===========================================================================*/
static gas_id_t rr_ms_handle_acquisition_state_ind(const mph_acquisition_state_ind_T *ind_ptr, rr_ms_executor_t *exec_ptr)
{
  if (ind_ptr != NULL)
  {
    gas_id_t gas_id = ind_ptr->gas_id;

    if (RR_L1_ACQ_RUNNING == ind_ptr->rr_l1_acq_state)
    {
      /* update the grant statistics */
      rr_ms_update_grant_statistics(ind_ptr->current_priority, gas_id);

      /* update PIMMS with the "running" event only if its not the case of PLMN search  */
      if (!rr_plmn_search_in_progress( gas_id))	
     {
        rr_ms_acquisition_manager(RR_MS_GAS_ID_ACQ_STATE_RUNNING, gas_id);
      }
      
    }
    else if (RR_L1_ACQ_WAITING == ind_ptr->rr_l1_acq_state)
    {
      /* update PIMMS with the "waiting" event only if its not the case of PLMN search*/
      if (!rr_plmn_search_in_progress( gas_id))
      {
        rr_ms_acquisition_manager(RR_MS_GAS_ID_ACQ_STATE_WAITING, gas_id);
      }
    }
    else
    {
      /* future expansion: add handler for STOPPED event here */
      MSG_GERAN_ERROR_1_G("Unable to handle GL1 acq state %d", (int)(ind_ptr->rr_l1_acq_state));
    }

    /* main state machine does not need to process the message any further */
    if (exec_ptr != NULL)
    {
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      exec_ptr->next_action   = RR_MS_NO_ACTION;
    }

    return(gas_id);
  }

  return(GERAN_ACCESS_STRATUM_ID_UNDEFINED);
}

/*===========================================================================

FUNCTION     rr_ms_process_gl1_msg

DESCRIPTION  Extracts the access stratum ID from an incoming GL1 message

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS Cancels the L1 interface guard timer

===========================================================================*/
static gas_id_t rr_ms_process_gl1_msg(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  gas_id_t gas_id;

  switch(cmd_ptr->message_header.message_id)
  {
  case MPH_FREQUENCY_REDEFINITION_CNF:
  case MPH_SPECIFIC_BCCH_DECODE_FAILURE_IND:
    /* legacy interface, no longer supported */
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    break;

  case MPH_L1_DS_ABORT_IND:
    gas_id = cmd_ptr->mph.l1_ds_abort_ind.gas_id;
    break;

  case MPH_CHANNEL_ASSIGN_CNF:
    gas_id = cmd_ptr->mph_channel_assign_cnf.gas_id;
    break;

  case MPH_BLOCK_QUALITY_IND:
    gas_id = rr_ms_process_block_quality_report(&(cmd_ptr->mph_block_quality_ind), exec_ptr);
    break;

  case MPH_CHANNEL_MODE_MODIFY_CNF:
    gas_id = cmd_ptr->mph_channel_mode_modify_cnf.gas_id;
    break;

  case MPH_CHANNEL_RELEASE_CNF:
    gas_id = cmd_ptr->mph_channel_release_cnf.gas_id;
    rr_ms_cs_released(gas_id);
    break;

  case MPH_HANDOVER_CNF:
    gas_id = cmd_ptr->mph_handover_cnf.gas_id;
    break;

  case MPH_HANDOVER_IND:
    gas_id = cmd_ptr->mph_handover_ind.gas_id;
    break;

  case MPH_IMMED_ASSIGN_CNF:
    gas_id = cmd_ptr->mph_immed_assign_cnf.gas_id;
    rr_ms_cs_connected(TRUE, gas_id);
    rr_ms_cs_establishment_confirmed(gas_id);
    break;

  case MPH_RECONNECT_CHANNEL_CNF:
    gas_id = cmd_ptr->mph_reconnect_channel_cnf.gas_id;
    rr_ms_cs_connected(TRUE, gas_id);
    break;

  case MPH_SERVING_DED_MEAS_IND:
    gas_id = cmd_ptr->mph_serving_ded_meas_ind.gas_id;
    break;

  case MPH_SERVING_IDLE_MEAS_IND:
    gas_id = cmd_ptr->mph_serving_idle_meas_ind.gas_id;
    break;

  case MPH_SET_CIPHERING_CNF:
    gas_id = cmd_ptr->mph_set_ciphering_cnf.gas_id;
    break;

  case MPH_STOP_HANDOVER_ACCESS_CNF:
    gas_id = cmd_ptr->mph_stop_handover_access_cnf.gas_id;
    break;

  case MPH_SURROUND_MEAS_IND:
    gas_id = cmd_ptr->mph_surround_meas_ind.gas_id;
    break;

  case MPH_RESELECTION_CNF:
    gas_id = cmd_ptr->mph_reselection_cnf.gas_id;
    (void)rr_ms_check_required_l1_response(MPH_RESELECTION_CNF, exec_ptr, gas_id);
    if (TRUE == cmd_ptr->mph_reselection_cnf.BCCH_found)
    {
      rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_BCCH_SELECTED, gas_id);
      /* Reset PIMMS */
      rr_ms_acquisition_manager(RR_MS_PRIORITY_INVERSION_INIT, gas_id);
    }
    break;

  case MPH_DECODE_BCCH_LIST_CNF:
    gas_id = cmd_ptr->mph_decode_bcch_list_cnf.gas_id;
    break;

  case MPH_DECODE_BCCH_LIST_FAILURE_IND:
    gas_id = cmd_ptr->mph_decode_bcch_list_failure_ind.gas_id;
    break;

  case MPH_POWER_SCAN_CNF:
    gas_id = rr_ms_process_power_scan_confirm(&(cmd_ptr->mph_power_scan_cnf), exec_ptr);
    break;

  case MPH_SELECT_SPECIFIC_BCCH_CNF:
    gas_id = cmd_ptr->mph_select_specific_bcch_cnf.gas_id;
    if (rr_ms_check_required_l1_response(MPH_SELECT_SPECIFIC_BCCH_CNF, exec_ptr, gas_id) == FALSE)
    {
      /* GL1 may have responded to a reselection, so perform further check */
      (void)rr_ms_check_required_l1_response(MPH_RESELECTION_CNF, exec_ptr, gas_id);
    }
    if (TRUE == cmd_ptr->mph_select_specific_bcch_cnf.BCCH_found)
    {
      rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_BCCH_SELECTED, gas_id);
      /* Reset PIMMS */
      rr_ms_acquisition_manager(RR_MS_PRIORITY_INVERSION_INIT, gas_id);
    }
    break;

  case MPH_FCCH_SCH_DECODE_IND:
    gas_id = cmd_ptr->mph_fcch_sch_decode_ind.gas_id;
    MSG_GERAN_HIGH_3_G("MPH_FCCH_SCH_DECODE_IND for ARFCN %d BSIC <%d,%d>",
                       (int)(cmd_ptr->mph_fcch_sch_decode_ind.BCCH_ARFCN.num),
                       (int)(cmd_ptr->mph_fcch_sch_decode_ind.BSIC.PLMN_colour_code),
                       (int)(cmd_ptr->mph_fcch_sch_decode_ind.BSIC.BS_colour_code));
    break;

  case MPH_SURROUND_UPDATE_FAILURE_IND:
    gas_id = cmd_ptr->mph_surround_update_failure_ind.gas_id;
    break;

  case MPH_SERVING_AUX_MEAS_IND:
    gas_id = cmd_ptr->mph_serving_aux_meas_ind.gas_id;
    break;

  case MPH_READ_SYS_INFO_FAILURE_IND:
    gas_id = cmd_ptr->mph_read_sys_info_failure_ind.gas_id;
    break;

  case MPH_FREQUENCY_REDEFINITION_IND:
    gas_id = cmd_ptr->mph_frequency_redefinition_ind.gas_id;
    break;

  case MPH_START_GSM_MODE_CNF:
    gas_id = cmd_ptr->mph_start_gsm_mode_cnf.gas_id;
    if (TRUE == rr_is_csfb_call_in_progress(gas_id))
    {
      rr_ms_activate_procedure_lock(RR_MS_PROC_LOCK_CSFB, gas_id);
    }
    else if (RR_MS_PROC_LOCK_CS_CALL == rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].proc_lock_cause)
    {
      rr_ms_activate_procedure_lock(RR_MS_PROC_LOCK_CS_CALL, gas_id);
    }

    rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].drdsds_enabled =
      cmd_ptr->mph_start_gsm_mode_cnf.drdsds_enabled;

    break;

  case MPH_STOP_GSM_MODE_CNF:
    gas_id = cmd_ptr->mph_stop_gsm_mode_cnf.gas_id;
    if (rr_timer_cancel(RR_DUAL_SIM_INACTIVATE_TIMER, gas_id))
    {
      MSG_GERAN_HIGH_0_G("Cancelled GRR deactivation timer");
    }
    break;

  case MPH_NC_MEASUREMENT_IND:
    gas_id = cmd_ptr->mph_nc_measurement_ind.gas_id;
    break;

  case MPH_SERVING_MEAS_IND:
    gas_id = cmd_ptr->mph_serving_meas_ind.gas_id;
    break;

#ifdef FEATURE_WCDMA
  case MPH_SURROUND_WCDMA_IDLE_MEAS_IND:
    gas_id = cmd_ptr->mph_surround_wcdma_idle_meas_ind.gas_id;
    break;

  case MPH_SURROUND_WCDMA_DEDICATED_MEAS_IND:
    gas_id = cmd_ptr->mph_surround_wcdma_dedicated_meas_ind.gas_id;
    break;

  case MPH_SURROUND_WCDMA_XFER_MEAS_IND:
    gas_id = cmd_ptr->mph_surround_wcdma_xfer_meas_ind.gas_id;
    break;
#endif /* FEATURE_WCDMA */

#if defined(FEATURE_WCDMA) || defined(FEATURE_LTE) || defined(FEATURE_GSM_TDS)
  case MPH_SURROUND_IRAT_MEAS_IND:
    gas_id = cmd_ptr->mph.surround_irat_meas_ind.gas_id;
    break;
#endif /* FEATURE_WCDMA | FEATURE_LTE | FEATURE_GSM_TDS */

  case MPH_START_IDLE_MODE_CNF:
    gas_id = cmd_ptr->mph_start_idle_mode_cnf.gas_id;
    break;

  case MPH_SUSPEND_GSM_MODE_CNF:
    gas_id = cmd_ptr->mph_suspend_gsm_mode_cnf.gas_id;
    geran_api_set_tuneaway_blocked(GERAN_API_CLIENT_G2X, TRUE, geran_map_gas_id_to_nas_id(gas_id));
    break;

  case MPH_RESUME_GSM_MODE_CNF:
    gas_id = cmd_ptr->mph_resume_gsm_mode_cnf.gas_id;
    geran_api_set_tuneaway_blocked(GERAN_API_CLIENT_G2X, FALSE, geran_map_gas_id_to_nas_id(gas_id));
    break;

  case MPH_INTERRAT_LIST_SRCH_TIME_IND:
    gas_id = cmd_ptr->mph_interrat_list_srch_time_ind.gas_id;
    break;

  case MPH_SUSPEND_GSM_MODE_IND:
    gas_id = cmd_ptr->mph_suspend_gsm_mode_ind.gas_id;
    break;

  case MPH_START_GSM_MODE_FOR_BPLMN_CNF:
    gas_id = cmd_ptr->mph_start_gsm_mode_for_bplmn_cnf.gas_id;
    break;

  case MPH_RESUME_GSM_MODE_FOR_BPLMN_CNF:
    gas_id = cmd_ptr->mph_resume_gsm_mode_for_bplmn_cnf.gas_id;
    break;

#ifdef FEATURE_GSM_DTM
  case MPH_DTM_CHANNEL_ASSIGN_CNF:
    gas_id = cmd_ptr->mph_dtm_channel_assign_cnf.gas_id;
    rr_ms_cs_connected(TRUE, gas_id);
    break;

  case MPH_DTM_CHANNEL_RELEASE_IND:
    gas_id = cmd_ptr->mph_dtm_channel_release_ind.gas_id;
    if (cmd_ptr->mph_dtm_channel_release_ind.cs_release == TRUE)
    {
      rr_ms_cs_released(gas_id);
    }
    break;
#endif /* FEATURE_GSM_DTM */

  case MPH_L1_TEST_MODE_START_IND:
    gas_id = cmd_ptr->mph_l1_test_mode_start_ind.gas_id;
    rr_ms_gprs_test_mode_state_change(TRUE, gas_id);
    break;

  case MPH_L1_TEST_MODE_STOP_IND:
    gas_id = cmd_ptr->mph_l1_test_mode_stop_ind.gas_id;
    rr_ms_gprs_test_mode_state_change(FALSE, gas_id);
    break;

  case MPH_ABORT_RA_CNF:
    gas_id = cmd_ptr->mph_abort_ra_cnf.gas_id;
    break;

  case MPH_UE_MODE_CHANGE_CNF:
    gas_id = cmd_ptr->mph_ue_mode_change_cnf.gas_id;
    break;

  case MPH_COEX_MSC_CHANGE_IND:
    gas_id = cmd_ptr->mph.coex_msc_change_ind.gas_id;
    break;

  case MPH_RESET_IND:
  case MPH_RESET_CNF:
    gas_id = rr_ms_handle_panic_reset_message(cmd_ptr, exec_ptr);
    break;

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  case MPH_NV_REFRESH_CNF:
    gas_id = cmd_ptr->mph.nv_refresh_cnf.gas_id;
    break;
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

  case MPH_PREEMPTION_IND:
    gas_id = rr_ms_handle_preemption_ind(&(cmd_ptr->mph.preemption_ind), exec_ptr);
    break;

  case MPH_VOICE_SAMPLE_RATE_CHANGE_IND:
    gas_id = cmd_ptr->mph.voice_sample_rate_change_ind.gas_id;
    break;

  case MPH_ACQUISITION_STATE_IND:
    gas_id = rr_ms_handle_acquisition_state_ind(&(cmd_ptr->mph.acq_state_ind), exec_ptr);
    break;

  case MPH_IDLE_COLL_FALLBACK_IND:
    gas_id = cmd_ptr->mph.idle_coll_fallback_ind.gas_id;
    break;
  
  default:
    /* this will trap any unhandled messages */
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    break;
  }

  return(gas_id);
}

/*===========================================================================

FUNCTION     rr_ms_process_grr_msg

DESCRIPTION  Extracts the access stratum ID from an incoming GRR message

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS May abort a pending TRM transaction in GL1

===========================================================================*/
static gas_id_t rr_ms_process_grr_msg(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  gas_id_t gas_id;
  boolean  is_abort;

  /* extract the GAS ID from the header */
  gas_id = cmd_ptr->rr.header.gas_id;

  if ((cmd_ptr->rr.header.rr_message_set == RR_CELL_ACQ_IMSG) &&
      (cmd_ptr->rr.header.imh.message_id == RR_CELL_ACQ_IMSG_ABORT_REQ))
  {
    is_abort = TRUE;
  }
  else if ((cmd_ptr->rr.header.rr_message_set == RR_L1_IDLE_IMSG) &&
           (cmd_ptr->rr.header.imh.message_id == RR_L1_IDLE_IMSG_START_IDLE_REQ))
  {
    rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_FULL_CAMPED, gas_id);
    is_abort = FALSE;
  }
  else
  {
    is_abort = FALSE;
  }

  if (is_abort)
  {
    /* check whether this internal abort affects a pending interface */
    exec_ptr->next_action = rr_ms_abort_pending_interface(RR_MS_GRR_ABORT_BY_IMSG, gas_id);
  }

  return(gas_id);
}

/*===========================================================================

FUNCTION     rr_ms_process_mac_msg

DESCRIPTION  Determines the access stratum ID for an incoming GMAC message

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS None

===========================================================================*/
static gas_id_t rr_ms_process_mac_msg(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  gas_id_t gas_id;

  switch(cmd_ptr->message_header.message_id)
  {
  case MAC_GRR_MAC_STATUS:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_mac_status.gas_id;
    break;

  case MAC_GRR_DATA_IND:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_data_ind.gas_id;
    break;

  case MAC_GRR_CELL_RESEL_STATUS:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_cell_resel_status.gas_id;
    break;

  case MAC_GRR_PAGING_NOTIFICATION:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_paging_notification.gas_id;
    break;

  case MAC_GRR_ABNORMAL_RELEASE:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_abnormal_release.gas_id;
    break;

  case MAC_GRR_MEAS_RPT_CNF:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_meas_rpt_cnf.gas_id;
    break;

  case MAC_GRR_PCCF_SENT:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_pccf_sent.gas_id;
    break;

  case MAC_GRR_TBF_REJ:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_tbf_rej.gas_id;
    break;

  case MAC_GRR_TBF_ACCEPT:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_tbf_accept.gas_id;
    break;

  case MAC_GRR_UL_TBF_REQ:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_ul_tbf_req.gas_id;
    break;

  case MAC_GRR_STORE_UPDATE:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_store_update.gas_id;
    break;

  case MAC_GRR_RACH_CNF:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_rach_cnf.gas_id;
    break;

  case MAC_GRR_RACH_REJ:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_rach_rej.gas_id;
    break;

  case MAC_GRR_NCELL_DATA_IND:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_ncell_data_ind.gas_id;
    break;

  case MAC_GRR_NCELL_DATA_CNF:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_ncell_data_cnf.gas_id;
    break;

  case MAC_GRR_RESET_CNF:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_reset_cnf.gas_id;
    break;

#ifdef FEATURE_GSM_DTM
  case MAC_GRR_DTM_CHANNEL_ASSIGN_REJ:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_dtm_channel_assign_rej.gas_id;
    break;

#ifdef FEATURE_GSM_EDTM
  case MAC_GRR_EDTM_PKT_CS_EST_REJ:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_edtm_pkt_cs_est_rej.gas_id;
    break;

  case MAC_GRR_EDTM_CS_RELEASE_IND:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_edtm_cs_release_ind.gas_id;
    break;
#endif /* FEATURE_GSM_EDTM */
#endif /* FEATURE_GSM_DTM */

#ifdef FEATURE_GPRS_PS_HANDOVER
  case MAC_GRR_PSHO_CNF:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_psho_cnf.gas_id;
    break;
#endif /* FEATURE_GPRS_PS_HANDOVER */

  case MAC_GRR_PS_ACCESS_CNF:
    gas_id = cmd_ptr->mac_grr_sig.mac_grr_ps_access_cnf.gas_id;
    break;

  default:
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    MSG_GERAN_ERROR_1("Missing GAS ID from GMAC msg %d", (int)(cmd_ptr->message_header.message_id));
    break;
  }

  /* output useful debug that shows where TLLIs are assigned */
#ifdef FEATURE_TRIPLE_SIM
  MSG_GERAN_HIGH_3_G("Rcvd GMAC msg(TLLI0=%1d,TLLI1=%1d,TLLI2=%1d)",
                     rr_ps_tlli_is_assigned(GERAN_ACCESS_STRATUM_ID_1),
                     rr_ps_tlli_is_assigned(GERAN_ACCESS_STRATUM_ID_2),
                     rr_ps_tlli_is_assigned(GERAN_ACCESS_STRATUM_ID_3));
#else
  MSG_GERAN_HIGH_2_G("Rcvd GMAC msg(TLLI0=%1d,TLLI1=%1d)",
                     rr_ps_tlli_is_assigned(GERAN_ACCESS_STRATUM_ID_1),
                     rr_ps_tlli_is_assigned(GERAN_ACCESS_STRATUM_ID_2));
#endif /* FEATURE_TRIPLE_SIM */

  return(gas_id);
}

/*===========================================================================

FUNCTION     rr_ms_sys_info_refresh_blocked

DESCRIPTION  Determines whether the specified stack should allow SI refresh

DEPENDENCIES None

RETURN VALUE TRUE if a normal SI refresh can be allowed to take place.

SIDE EFFECTS None

===========================================================================*/
boolean rr_ms_sys_info_refresh_blocked(const gas_id_t gas_id)
{
  uint32 refresh_duration = rr_nv_get_multisim_si_refresh_duration_milliseconds(RR_GAS_ID_TO_AS_ID);

  /* if the timer duration is set to 0ms then SI refresh is disabled*/
  if (refresh_duration == 0)
  {
    return(TRUE);
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_connected_mode_detected

DESCRIPTION  Detects any non-preemptable GSM (or LTE) connected mode

DEPENDENCIES Assumes that GSM voice and VoLTE are not pre-emptable

RETURN VALUE TRUE if connected mode is detected on a different stack

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_connected_mode_detected(const gas_id_t gas_id)
{
  if (rr_ms_other_stack_in_dedicated(gas_id))
  {
    /* a GERAN stack is in a circuit-switched connected mode */
    return(TRUE);
  }
  else if (rr_ms_volte_call_active_on_other_stack(gas_id))
  {
    /* LTE stack is in a voice call */
    return(TRUE);
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_hibernation_required

DESCRIPTION  Determines whether the system is in a state where hibernation
             mode is a useful strategy for optimised power consumption

DEPENDENCIES None

RETURN VALUE TRUE if hibernation is required

SIDE EFFECTS None

===========================================================================*/
static boolean rr_ms_hibernation_required(const gas_id_t gas_id)
{
  if (geran_in_multi_standby_mode())
  {
    return(rr_ms_connected_mode_detected(gas_id));
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_max_heartbeat_reached

DESCRIPTION  Rebaselines the heartbeat time counter

DEPENDENCIES None

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_max_heartbeat_reached(int as_index)
{
  uint16 oldest = RR_MS_HEARTBEAT_LIMIT - 1;

  if (rr_multisim_info.gas_data[as_index].svc_rqst_time < rr_multisim_info.gas_data[as_index].svc_loss_time)
  {
    if (rr_multisim_info.gas_data[as_index].svc_rqst_time != 0)
    {
      oldest = rr_multisim_info.gas_data[as_index].svc_rqst_time-1;
    }
    else if (rr_multisim_info.gas_data[as_index].svc_loss_time != 0)
    {
      oldest = rr_multisim_info.gas_data[as_index].svc_loss_time-1;
    }
  }
  else /* (rr_multisim_info.gas_data[as_index].svc_loss_time <= rr_multisim_info.gas_data[as_index].svc_rqst_time) */
  {
    if (rr_multisim_info.gas_data[as_index].svc_loss_time != 0)
    {
      oldest = rr_multisim_info.gas_data[as_index].svc_loss_time-1;
    }
    else if (rr_multisim_info.gas_data[as_index].svc_rqst_time != 0)
    {
      oldest = rr_multisim_info.gas_data[as_index].svc_rqst_time-1;
    }
  }

  if (rr_multisim_info.gas_data[as_index].svc_rqst_time != 0)
  {
    rr_multisim_info.gas_data[as_index].svc_rqst_time -= oldest;
  }

  if (rr_multisim_info.gas_data[as_index].svc_loss_time != 0)
  {
    rr_multisim_info.gas_data[as_index].svc_loss_time -= oldest;
  }

  rr_multisim_info.gas_data[as_index].seconds_since_suspension = RR_MS_HEARTBEAT_LIMIT - oldest;
  MSG_GERAN_ERROR_1("Max heartbeat, seconds since suspension = %d", (int)(rr_multisim_info.gas_data[as_index].seconds_since_suspension));

  return;
}

/*===========================================================================

FUNCTION     rr_ms_process_heartbeat

DESCRIPTION  Monitors a suspended access stratum

DEPENDENCIES None

RETURN VALUE RR event for main state machine

SIDE EFFECTS Checks for staleness of any buffered service request from NAS

===========================================================================*/
static void rr_ms_process_heartbeat(const gas_id_t gas_id)
{
  rr_ms_gl1_if_state_t interface_state;
  int                  as_index;

  /* verify that the specified GAS ID is still suspended (or hibernated) */
  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  interface_state = rr_multisim_info.gas_data[as_index].gl1_interface_state;
  if (interface_state != RR_MS_INTERFACE_ACTIVE)
  {
    /* determine how long this AS has been suspended */
    if (rr_multisim_info.gas_data[as_index].seconds_since_suspension < RR_MS_HEARTBEAT_LIMIT)
    {
      if (interface_state == RR_MS_INTERFACE_HIBERNATED)
      {
        rr_multisim_info.gas_data[as_index].seconds_since_suspension += RR_MS_HIBERNATE_SECONDS;
      }
      else
      {
        rr_multisim_info.gas_data[as_index].seconds_since_suspension += RR_MS_HEARTBEAT_SECONDS;
      }
    }
    else
    {
      /* not expected, but better cope with it */
      rr_ms_max_heartbeat_reached(as_index);
    }

    /* if the heartbeat timer has expired while RR is in an INACTIVE state */
    /* a recovery needs to be attempted, otherwise service requests cannot */
    /* ever be processed, and this GERAN stack will remain out of service  */
    if ((interface_state != RR_MS_INTERFACE_HIBERNATED) && rr_is_inactive(gas_id))
    {
      MSG_GERAN_ERROR_0_G("Resume suspended interface: inactive but not hibernated");
      rr_ms_prepare_for_resumption(TRUE, gas_id);
      return;
    }

    /* check whether any buffered service request is now stale */
    rr_ms_check_pending_service_request(rr_multisim_info.gas_data[as_index].seconds_since_suspension, gas_id);

    /* restart the heartbeat with the appropriate heartrate */
    if (rr_ms_start_heartbeat(interface_state, gas_id))
    {
      /* heartbeat has restarted, prompt GL1 to complete its last */
      /* activity, unless there a GERAN stack in a connected mode */
      if (FALSE == rr_ms_other_stack_in_dedicated(gas_id))
      {
        (void)rr_ms_check_pending_response(RR_MS_POLL_FOR_RESPONSE, gas_id);
      }
    }
    else
    {
      /* heartbeat failure has occurred, resume now */
      MSG_GERAN_ERROR_0_G("Unable to restart heartbeat, resume now");
      rr_ms_prepare_for_resumption(TRUE, gas_id);
    }
  }
  else
  {
    /* explicitly clear down the heartbeat timer */
    (void)rr_timer_cancel(RR_DUAL_SIM_HEARTBEAT_TIMER, gas_id);
    MSG_GERAN_ERROR_0_G("Heartbeat not expected while interface is active");
  }

  return;
}

static void handle_tech_recent_activity_timer_expiry(void)
{
  rr_ms_tech_info_t *tech_info_ptr;
  boolean            tick_still_required = FALSE;
  uint8              i;

  tech_info_ptr = rr_ms_tech_info;
  for (i = 0; i < ARR_SIZE(rr_ms_tech_info); i++, tech_info_ptr++)
  {
    if (tech_info_ptr->inactive_counter > 0)
    {
      --tech_info_ptr->inactive_counter;

      if (tech_info_ptr->inactive_counter == 0)
      {
        tech_info_ptr->tx_active = FALSE;
      }
      else
      {
        tick_still_required = TRUE;
      }
    }
  }

  if (tick_still_required)
  {
    (void)rr_timer_start(RR_TECH_RECENT_ACTIVITY_TIMER,
                         RR_TECH_RECENT_ACTIVITY_TICK_DURATION,
                         GERAN_ACCESS_STRATUM_ID_1);
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_process_tmr_msg

DESCRIPTION  Determines the access stratum ID for a timer expiry message

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS May determine that L1 is unresponsive and block NAS processing

===========================================================================*/
static gas_id_t rr_ms_process_tmr_msg(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  gas_id_t gas_id;

  /* the only timer messages GRR expects to receive are expiry notifications */
  /* which are only enqueued by its own timer management module (rr_timer.c) */
  if (cmd_ptr->message_header.message_id == (byte)MID_TIMER_EXPIRY)
  {
    timer_id_T timer_id;
    int        as_index;

    /* the timer expiry message should have a GAS ID in its data payload */
    gas_id   = (gas_id_t)(cmd_ptr->mid_timer_expiry.data);
    as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /* extract the timer ID from the expiry message */
    timer_id = cmd_ptr->mid_timer_expiry.timer_id;
    if ((timer_id == RR_CELL_SELECTION_ABORT_TIMER) ||
        (timer_id == RR_DUAL_SIM_TERMINATE_TIMER)   ||
        (timer_id == CELL_RESELECTION_TIMER))
    {
      /* check whether this timer has expired whilst an interface is pending */
      exec_ptr->next_action = rr_ms_abort_pending_interface(RR_MS_GRR_TIMER_EXPIRED, gas_id);

      /* Reset PIMMS */
      rr_ms_acquisition_manager(RR_MS_PRIORITY_INVERSION_INIT, gas_id);

      if (timer_id == RR_DUAL_SIM_TERMINATE_TIMER)
      {
        /* no need to process this timer expiry in the main state machines */
        exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      }

      /* Check whether cell selection timeout has happened or not */
      if (timer_id == RR_CELL_SELECTION_ABORT_TIMER)
      {
        rr_multisim_info.gas_data[as_index].cell_selection_timeout = TRUE;
      }
    }
    else if (timer_id == RR_DUAL_SIM_BPLMN_LIST_TIMER)
    {
      /* BCCH decode for BPLMN list search has not completed */
      exec_ptr->next_action   = RR_MS_NO_ACTION;
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;

      MSG_GERAN_HIGH_0_G("BPLMN guard timer expired, reset DB, abort GL1");

      if (FALSE == rr_limited_service_in_effect(gas_id))
      {
        /* under normal service conditions, update the candidate cell DB */
        /* such that GRR does not try to start another BCCH list search  */
        rr_ms_candidate_db_search_timeout(gas_id);
      }

      /* notify the RR control state machine */
      rr_ms_set_bplmn_failed(gas_id);

      /* notify PLMN List Control that it has been aborted */
      rr_ms_set_plc_gsm_aborted(gas_id);

      /* and ask GL1 to abort the current list decode operation */
      rr_send_mph_decode_bcch_list_abort_req(gas_id);
    }
    else if (timer_id == RR_DUAL_SIM_INACTIVATE_TIMER)
    {
      /* RR mode control not able to complete shutdown */
      exec_ptr->next_action   = RR_MS_NO_ACTION;
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      rr_mode_deactivation_guard_expiry(gas_id);
    }
    else if (timer_id == RR_DUAL_SIM_HEARTBEAT_TIMER)
    {
      if ((RR_MS_INTERFACE_HIBERNATED == rr_multisim_info.gas_data[as_index].gl1_interface_state) &&
          (FALSE                      == rr_ms_hibernation_required(gas_id)))
      {
        /* hibernation condition has ended, resume GRR and process any pending service request */
        exec_ptr->next_action   = RR_MS_PREPARE_FOR_RESUMPTION;
        exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
        MSG_GERAN_HIGH_0_G("Hibernation ending, no blocking client detected");
      }
      else
      {
        rr_ms_process_heartbeat(gas_id);
      }
    }
    else if (timer_id == RR_ACQ_DUTY_CYCLE_TIMER)
    {
      /* cancel the timer because GRR task does not validate the expiry */
      (void)rr_timer_cancel(RR_ACQ_DUTY_CYCLE_TIMER, gas_id);
      exec_ptr->next_action   = RR_MS_NO_ACTION;
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      rr_ms_acquisition_manager(RR_MS_GAS_ID_DUTY_CYCLE_EXPIRY, gas_id);
    }
    else if (timer_id == RR_PROC_LOCK_GUARD_TIMER)
    {
      exec_ptr->next_action   = RR_MS_NO_ACTION;
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      rr_ms_deactivate_procedure_lock(gas_id);
    }
    else if (timer_id == RR_SYS_INFO_GUARD_TIMER)
    {
      exec_ptr->next_action   = RR_MS_NO_ACTION;
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_SI_GUARD_TIMEOUT, gas_id);
    }
    else if (timer_id == RR_EPD_TIMER)
    {
      exec_ptr->next_action   = RR_MS_NO_ACTION;
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
    }
    else if (timer_id == RR_TECH_RECENT_ACTIVITY_TIMER)
    {
      exec_ptr->next_action   = RR_MS_NO_ACTION;
      exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      handle_tech_recent_activity_timer_expiry();
    }
  }
  else
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
    exec_ptr->next_action   = RR_MS_NO_ACTION;
    MSG_GERAN_ERROR_1_G("Unhandled MS_TIMER message ID 0x%02X", (int)(cmd_ptr->message_header.message_id));
  }

  return(gas_id);
}

/*===========================================================================

FUNCTION      rr_ms_any_other_active_gas_id

DESCRIPTION   Determines whether any other GERAN access stratum is active

DEPENDENCIES  None

RETURN VALUE  TRUE if any other GERAN stack is active, FALSE otherwise

SIDE EFFECTS  None

===========================================================================*/
boolean rr_ms_any_other_active_gas_id(const gas_id_t gas_id)
{
  gas_id_t other_gas_id;

  for (other_gas_id = GERAN_ACCESS_STRATUM_ID_1; other_gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; other_gas_id++)
  {
    if (other_gas_id != gas_id)
    {
      if (rr_mode_gsm_active_or_starting(other_gas_id))
      {
        return(TRUE);
      }
    }
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION     rr_ms_process_gl2_msg

DESCRIPTION  Extracts the access stratum ID from an incoming GL2 message

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS None

===========================================================================*/
static gas_id_t rr_ms_process_gl2_msg(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr)
{
  gas_id_t gas_id;

  switch(cmd_ptr->message_header.message_id)
  {
  case DL_ESTABLISH_CNF:
    gas_id = cmd_ptr->dl_establish_cnf.gas_id;
    rr_ms_cs_establishment_confirmed(gas_id);
    break;

  case DL_ESTABLISH_IND:
    gas_id = cmd_ptr->dl_establish_ind.gas_id;
    break;

  case DL_UNIT_DATA_IND:
    gas_id = cmd_ptr->dl_unit_data_ind.gas_id;
    rr_ms_page_message_received(&(cmd_ptr->dl_unit_data_ind));
    break;

  case DL_RANDOM_ACCESS_CNF:
    gas_id = cmd_ptr->dl_random_access_cnf.gas_id;
    rr_ms_cs_establishment_confirmed(gas_id);
    break;

  case DL_RECONNECT_CNF:
    gas_id = cmd_ptr->dl_reconnect_cnf.gas_id;
    break;

  case DL_RELEASE_CNF:
    gas_id = cmd_ptr->dl_release_cnf.gas_id;
    break;

  case DL_RELEASE_IND:
    gas_id = cmd_ptr->dl_release_ind.gas_id;
    break;

  case DL_RESUME_CNF:
    gas_id = cmd_ptr->dl_resume_cnf.gas_id;
    break;

  case DL_SUSPEND_CNF:
    gas_id = cmd_ptr->dl_suspend_cnf.gas_id;
    rr_ms_cs_establishment_confirmed(gas_id);
    break;

  case DL_DATA_ACK_IND:
    gas_id = cmd_ptr->dl_data_ack_ind.gas_id;
    break;

  case DL_DATA_IND:
    gas_id = cmd_ptr->dl_data_ind.gas_id;
    break;

  case MDL_ERROR_IND:
    gas_id = cmd_ptr->mdl_error_ind.gas_id;
    break;

  default:
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    MSG_GERAN_ERROR_1("Unknown GAS ID for GL2 msg 0x%02X", (int)(cmd_ptr->message_header.message_id));
    break;
  }

  return(gas_id);
}

/*===========================================================================

FUNCTION     rr_ms_process_rlc_msg

DESCRIPTION  Determines the access stratum ID for an incoming GRLC message

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS None

===========================================================================*/
static gas_id_t rr_ms_process_rlc_msg(const rr_cmd_bdy_type *cmd_ptr)
{
  gas_id_t gas_id;

#ifdef FEATURE_GSM_DTM
  if (RG_UL_DCCH_REQ == cmd_ptr->message_header.message_id)
  {
    gas_id = cmd_ptr->rlc_grr_ul_dcch_req.gas_id;
  }
  else
#endif /* FEATURE_GSM_DTM */
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    MSG_GERAN_ERROR_1("Unexpected message ID from GRLC %d", (int)(cmd_ptr->message_header.message_id));
  }

  return(gas_id);
}

#ifdef FEATURE_CGPS_UMTS_CP_GSM
/*===========================================================================

FUNCTION     rr_ms_process_gps_msg

DESCRIPTION  Determines which GERAN access stratum to use for GPS messages

DEPENDENCIES None

RETURN VALUE ID of GERAN Access Stratum which serves the GPS task

SIDE EFFECTS None

===========================================================================*/
static gas_id_t rr_ms_process_gps_msg(const rr_cmd_bdy_type *cmd_ptr)
{
  sys_modem_as_id_e_type nas_id;

  switch(cmd_ptr->message_header.message_id)
  {
    case RR_APDU_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_apdu_req);
      break;

    case RR_CGPS_EVENT_REQ:
      nas_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_cgps_event_req);
      break;

    default:
    {
      MSG_GERAN_ERROR_1("Unknown CGPS msg 0x%02X", (int)(cmd_ptr->message_header.message_id));

      // Exit indicating undefined GAS-ID
      return (GERAN_ACCESS_STRATUM_ID_UNDEFINED);
    }
  }

  return (geran_map_nas_id_to_gas_id(nas_id));
}
#endif /* FEATURE_CGPS_UMTS_CP_GSM */

/*===========================================================================

FUNCTION     rr_ms_suspended_as_deactivation

DESCRIPTION  Allows GRR to monitor cell selection activities that might need
             to be aborted before AS shutdown can be completed.

DEPENDENCIES Should only be called for a suspended GERAN access stratum

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_suspended_as_deactivation(const gas_id_t gas_id)
{
  /* validate the subscription ID */
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    if (RR_MS_INTERFACE_HIBERNATED == rr_multisim_info.gas_data[as_index].gl1_interface_state)
    {
      /* if hibernation is in effect, this access stratum must be resumed */
      /* and any pending service request from NAS discarded silently here */
      MSG_GERAN_HIGH_0_G("Deactivated by NAS while hibernated");
      rr_ms_prepare_for_resumption(FALSE, gas_id);
    }
    else
    {
      /* normal suspension is in effect, discard any buffered service request */
      MSG_GERAN_HIGH_0_G("Deactivated by NAS while suspended");
      rr_ms_abort_pending_service_request(FALSE, gas_id);

      /* if the cell selection or reselection timers are inactive, */
      /* initiate a timed DS abort process for this access stratum */
      if ((rr_timer_is_running(RR_CELL_SELECTION_ABORT_TIMER, gas_id) == FALSE) &&
          (rr_timer_is_running(RR_DUAL_SIM_TERMINATE_TIMER, gas_id)   == FALSE) &&
          (rr_timer_is_running(CELL_RESELECTION_TIMER, gas_id)        == FALSE))
      {
        /* access stratum is currently suspended, but has no other */
        /* guard that ensures AS resumption if GL1 cannot respond  */
        if (rr_timer_start(RR_DUAL_SIM_TERMINATE_TIMER, GRR_TIMEOUT_10_SECONDS, gas_id))
        {
          MSG_GERAN_HIGH_0_G("Starting termination timer");
        }
        else
        {
          MSG_GERAN_ERROR_0_G("Unable to start termination timer");
        }
      }
    }
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_check_suspension_response

DESCRIPTION  Determines any response required to a message sent to GRR while
             the active AS is suspended

DEPENDENCIES Input GAS ID parameter must be valid

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_check_suspension_response(const rr_cmd_bdy_type *cmd_ptr, rr_ms_executor_t *exec_ptr, const gas_id_t gas_id)
{
  int as_index;

  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  if ((rr_multisim_info.gas_data[as_index].gl1_interface_state != RR_MS_INTERFACE_ACTIVE) &&
      (cmd_ptr                                                 != NULL))
  {
    byte message_set = cmd_ptr->message_header.message_set;
    byte message_id  = cmd_ptr->message_header.message_id;

    /* access stratum is suspended - certain messages cannot be handled */
    if (message_set == MS_MM_RR)
    {
      if (message_id == (byte)RR_EST_REQ)
      {
        rr_send_abort_ind(RANDOM_ACCESS_FAILURE, MM_RR_NO_RESOURCE_AVAILABLE, gas_id);
        exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      }
      else if (message_id == (byte)RR_PLMN_SELECT_REQ)
      {
        rr_ms_buffer_service_request(cmd_ptr);
        exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      }
      else if (message_id == (byte)RR_PLMN_LIST_REQ)
      {
        /* do not bother trying to process this now, unlikely to succeed */
        rr_ms_send_rr_plmn_list_cnf(&(cmd_ptr->mm.rr_plmn_list_req), MM_AS_CONNECTED);
        exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      }
      else if ((message_id == (byte)RR_STOP_GSM_MODE_REQ) ||
               (message_id == (byte)RR_DEACT_REQ))
      {
        /* check for unguarded cell selection/reselection activity */
        rr_ms_suspended_as_deactivation(gas_id);
      }
    }
    else if (message_set == MS_MAC_RR)
    {
      if (message_id == (byte)MAC_GRR_UL_TBF_REQ)
      {
        /* PS access is not available during suspension */
        rr_send_grr_mac_no_ps_access_ind(DEL_ONE, gas_id);

        /* do not resume GMAC if PS access is blocked */
        if (rr_mac_ps_access_blocked(gas_id))
        {
          MSG_GERAN_ERROR_0_G("Unexpected GMAC request during PS suspension");
        }
        else
        {
          // TODO - this probably will not work, needs to be co-ordinated with GRR main state
          rr_send_grr_mac_ps_access_req(FALSE, gas_id);
        }
        exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      }
    }
#ifdef FEATURE_GSM_DTM
    else if (message_set == MS_RLC_RR)
    {
      /* should never happen, but if it does, close the loop with RLC */
      if (message_id == (byte)RG_UL_DCCH_REQ)
      {
        rr_send_grr_mac_dtm_ul_dcch_cnf(OTHER_ERROR, gas_id);
        exec_ptr->process_in_rr = RR_PROCESS_MSG_NEVER;
      }
    }
#endif /* FEATURE_GSM_DTM */
  }

  return;
}

#ifdef FEATURE_WCDMA
/*===========================================================================

FUNCTION     rr_ms_process_rrc_msg

DESCRIPTION  Called for an inter-task signal from RRC

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS None

===========================================================================*/
static gas_id_t rr_ms_process_rrc_msg(const rr_cmd_bdy_type *cmd_ptr)
{
  sys_modem_as_id_e_type rrc_id;
  gas_id_t               gas_id;

  switch(cmd_ptr->message_header.message_id)
  {
  case RRC_CIPHER_SYNC_IND:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_cipher_sync_ind);
    break;

  case RR_INTERRAT_HANDOVER_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_handover_req);
    break;

  case RR_INTERRAT_HANDOVER_ABORT_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_handover_abort_req);
    break;

  case RR_NAS_DATA_IND:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_nas_data_ind);
    break;

  case RR_INTERRAT_RESELECTION_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_reselect_req);
    break;

  case RR_INTERRAT_RESELECTION_ABORT_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_reselection_abort_req);
    break;

  case RR_INTERRAT_CC_ORDER_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_cc_order_req);
    break;

  case RR_INTERRAT_CC_ORDER_ABORT_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_cc_order_abort_req);
    break;

  case RRC_INTERRAT_RESELECTION_REJ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_reselect_rej);
    break;

  case RRC_INTERRAT_HANDOVER_CNF:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_handover_cnf);
    break;

  case RRC_INTERRAT_HANDOVER_ABORT_CNF:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_handover_abort_cnf);
    break;

  case RR_INTERRAT_REDIRECT_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_redirect_req);
    break;

  case RR_INTERRAT_REDIRECT_ABORT_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_redirect_abort_req);
    break;

  case RRC_INTERRAT_RESELECTION_ABORT_CNF:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_reselection_abort_cnf);
    break;

  case RRC_INTERRAT_PLMN_SRCH_CNF:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_plmn_srch_cnf);
    break;

  case RRC_INTERRAT_PLMN_SRCH_ABORT_CNF:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_plmn_srch_abort_cnf);
    break;

  case RR_INTERRAT_PLMN_SRCH_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_plmn_srch_req);
    break;

  case RR_INTERRAT_PLMN_SRCH_ABORT_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_plmn_srch_abort_req);
    break;

  case RR_INTERRAT_PLMN_SRCH_SUSPEND_IND:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_interrat_plmn_srch_suspend_ind);
    break;

  case RRC_INTERRAT_CC_ORDER_CNF:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_cc_order_cnf);
    break;

#ifdef FEATURE_GTOW_REDIRECTION_AFTER_CONN_RELEASE
  case RRC_INTERRAT_REDIRECT_REJ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_redirect_rej);
    break;

  case RRC_INTERRAT_REDIRECT_ABORT_CNF:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_interrat_redirect_abort_cnf);
    break;
#endif /* FEATURE_GTOW_REDIRECTION_AFTER_CONN_RELEASE */

  case RRC_RR_STOP_WCDMA_CNF:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_rr_stop_wcdma_cnf);
    break;

  case RR_RRC_STOP_GSM_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_rrc_stop_gsm_req);
    break;

  case RR_CLEAR_DEDICATED_PRIORITIES_IND:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_clear_dedicated_priorities_ind);
    break;

  case RR_DEDICATED_PRIORITIES_REQ:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_dedicated_priorities_req);
    break;

  case RR_INTERRAT_WL1_STOP_IND:
    rrc_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rr_wl1_stop_ind);
    break;

  default:
    rrc_id = geran_map_gas_id_to_nas_id(GERAN_ACCESS_STRATUM_ID_1);
    break;
  }

  if ((SYS_MODEM_AS_ID_1 == rrc_id) ||
      (SYS_MODEM_AS_ID_2 == rrc_id)
#ifdef FEATURE_TRIPLE_SIM
   || (SYS_MODEM_AS_ID_3 == rrc_id)
#endif /* FEATURE_TRIPLE_SIM */
      )
  {
    /* map the ASID to GERAN access stratum ID format */
    gas_id = geran_map_nas_id_to_gas_id(rrc_id);

    /* sanity check the subscription capabilities */
    if (rr_ms_sub_capabilities_exclude_sys_mode(SYS_SYS_MODE_MASK_WCDMA, gas_id))
    {
      MSG_GERAN_ERROR_0_G("Unexpected W-RRC message received on this subscription");
    }
  }
  else
  {
    MSG_GERAN_ERROR_2("Missing or invalid ASID %d in RRC msg %d",
                      (int)rrc_id,
                      (int)(cmd_ptr->message_header.message_id));
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
  }

  return(gas_id);
}
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_GSM_TDS
/*===========================================================================

FUNCTION     rr_ms_process_tdsrrc_msg

DESCRIPTION  Called for an inter-task signal from TDSRRC

DEPENDENCIES None

RETURN VALUE GAS ID

SIDE EFFECTS None

===========================================================================*/
static gas_id_t rr_ms_process_tdsrrc_msg(const rr_cmd_bdy_type *cmd_ptr)
{
  gas_id_t               gas_id;

#ifdef FEATURE_TDS_SUB_ID_INTERFACE

  sys_modem_as_id_e_type as_id = SYS_MODEM_AS_ID_NONE;

  switch(cmd_ptr->message_header.message_id)
  {
    case TDSRR_INTERRAT_HANDOVER_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_ho_req);
      break;
    }

    case TDSRR_INTERRAT_HANDOVER_ABORT_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_ho_abort_req);
      break;
    }

    case TDSRR_NAS_DATA_IND:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_nas_data_ind);
      break;
    }

    case TDSRR_INTERRAT_RESELECTION_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_resel_req);
      break;
    }

    case TDSRR_INTERRAT_RESELECTION_ABORT_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_resel_abort_req);
      break;
    }

    case TDSRR_INTERRAT_PLMN_SRCH_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_plmn_srch_req);
      break;
    }

    case TDSRR_INTERRAT_PLMN_SRCH_ABORT_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_plmn_srch_abort_req);
      break;
    }

    case TDSRR_INTERRAT_PLMN_SRCH_SUSPEND_IND:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_plmn_srch_suspend_ind);
      break;
    }

    case TDSRR_INTERRAT_CC_ORDER_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_cco_req);
      break;
    }

    case TDSRR_INTERRAT_CC_ORDER_ABORT_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_cco_abort_req);
      break;
    }

    case TDSRR_INTERRAT_REDIRECT_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_redir_req);
      break;
    }

    case TDSRR_INTERRAT_REDIRECT_ABORT_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_redir_abort_req);
      break;
    }

    case TDSRR_STOP_GSM_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_stop_gsm_req);
      break;
    }

    case TDSRR_INTERRAT_TL1_STOP_IND:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_tl1_stop_ind);
      break;
    }

    case TDSRR_CIPHER_SYNC_IND:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_cipher_sync_ind);
      break;
    }

    case TDSRRC_INTERRAT_HANDOVER_CNF:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_ho_cnf);
      break;
    }

    case TDSRRC_INTERRAT_HANDOVER_ABORT_CNF:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_ho_abort_cnf);
      break;
    }

    case TDSRRC_INTERRAT_RESELECTION_REJ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_resel_rej);
      break;
    }

    case TDSRRC_INTERRAT_RESELECTION_ABORT_CNF:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_resel_abort_cnf);
      break;
    }

    case TDSRRC_INTERRAT_PLMN_SRCH_CNF:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_plmn_srch_cnf);
      break;
    }

    case TDSRRC_INTERRAT_PLMN_SRCH_ABORT_CNF:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_plmn_srch_abort_cnf);
      break;
    }

    case TDSRRC_INTERRAT_CC_ORDER_CNF:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->rrc_cipher_sync_ind);
      break;
    }

    case TDSRRC_INTERRAT_REDIRECT_REJ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_redir_rej);
      break;
    }

    case TDSRRC_INTERRAT_REDIRECT_ABORT_CNF:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_redir_abort_cnf);
      break;
    }

#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
    case TDSRR_DEDICATED_PRIORITIES_REQ:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_dedicated_priorities_req);
      break;
    }

    case TDSRRC_DEDICATED_PRIORITIES_RSP:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrrc_dedicated_priorities_rsp);
      break;
    }

    case TDSRR_CLEAR_DEDICATED_PRIORITIES_IND:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_clear_dedicated_priorities_ind);
      break;
    }
#endif /* FEATURE_TDSCDMA_DEDICATED_PRI_INFO */

    case TDSRRC_STOP_TDS_CNF:
    {
      as_id = RR_MS_EXTRACT_AS_ID(cmd_ptr->tdsrr_stop_tds_cnf);
      break;
    }

    default:
      break;
  }

  if ((SYS_MODEM_AS_ID_1 == as_id) ||
      (SYS_MODEM_AS_ID_2 == as_id)
#ifdef FEATURE_TRIPLE_SIM
   || (SYS_MODEM_AS_ID_3 == as_id)
#endif /* FEATURE_TRIPLE_SIM */
      )
  {
    /* map the ASID to GERAN access stratum ID format */
    gas_id = geran_map_nas_id_to_gas_id(as_id);

    /* sanity check the subscription capabilities */
    if (rr_ms_sub_capabilities_exclude_sys_mode(SYS_SYS_MODE_MASK_TDS, gas_id))
    {
      MSG_GERAN_ERROR_0_G("Unexpected T-RRC message received on this subscription");
    }
  }
  else
  {
    MSG_GERAN_ERROR_2("Missing or invalid ASID %d in TDSRRC msg %d",
                      (int)as_id,
                      (int)(cmd_ptr->message_header.message_id));
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
  }

#else // FEATURE_TDS_SUB_ID_INTERFACE

  /* use the default multimode GAS ID for all TDS messages */
  gas_id = GERAN_ACCESS_STRATUM_ID_1;

#endif // FEATURE_TDS_SUB_ID_INTERFACE

  return(gas_id);
}
#endif /* FEATURE_GSM_TDS */

/*===========================================================================

FUNCTION      rr_ms_process_msgr_header

DESCRIPTION   Processes the contents of a Message Router primitive

DEPENDENCIES  None

RETURN VALUE  GAS ID for the message

SIDE EFFECTS  None

===========================================================================*/
static gas_id_t rr_ms_process_msgr_header(const msgr_hdr_s *hdr_ptr)
{
  gas_id_t gas_id;
  
  if (hdr_ptr != NULL)
  {
    sys_modem_as_id_e_type as_id = (sys_modem_as_id_e_type) MSGR_GET_VARIANT(hdr_ptr);

      /* Map the extracted ASID to a GAS ID */
      gas_id = geran_map_nas_id_to_gas_id(as_id);
    }
  else
  {
    gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    MSG_GERAN_ERROR_0("Unable to process MSGR header");
  }

  return(gas_id);
}

/*===========================================================================

FUNCTION     rr_ms_process_task_message

DESCRIPTION  Intercepts incoming command message from the RR task

DEPENDENCIES None

RETURN VALUE RR_PROCESS_MSG_NOW if the message should be processed by RR

SIDE EFFECTS None

===========================================================================*/
rr_process_msg_action_e rr_ms_process_task_message(const rr_cmd_bdy_type *cmd_ptr, gas_id_t *gas_id_ptr)
{
  rr_ms_executor_t executor;
  gas_id_t         next_gas_id;

  /* set defaults for the execution controller */
  executor.process_in_rr = RR_PROCESS_MSG_NOW;
  if (RR_MULTI_SIM_MODE_ENABLED)
  {
    executor.next_action = RR_MS_CHECK_FOR_SUSPENSION;
  }
  else
  {
    executor.next_action = RR_MS_NO_ACTION;
  }

  if (cmd_ptr == NULL)
  {
    next_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
    MSG_GERAN_ERROR_0("Null pointer received for inter-task message");

    return(RR_PROCESS_MSG_NEVER);
  }
  else
  {
    /* determine the appropriate GAS ID from the incoming message */
    if (cmd_ptr->message_header.message_set == MS_MM_RR)
    {
      next_gas_id = rr_ms_process_nas_msg(cmd_ptr, &executor);
    }
    else if (cmd_ptr->message_header.message_set == MS_RR_L1)
    {
      next_gas_id = rr_ms_process_gl1_msg(cmd_ptr, &executor);
    }
    else if (cmd_ptr->message_header.message_set == MS_RR_L2)
    {
      next_gas_id = rr_ms_process_gl2_msg(cmd_ptr, &executor);
    }
    else if ((cmd_ptr->message_header.message_set == MS_RR_RR) ||
             (cmd_ptr->message_header.message_set == MS_RR_RR_EXT))
    {
      next_gas_id = rr_ms_process_grr_msg(cmd_ptr, &executor);
    }
    else if (cmd_ptr->message_header.message_set == MS_MAC_RR)
    {
      next_gas_id = rr_ms_process_mac_msg(cmd_ptr, &executor);
    }
    else if (cmd_ptr->message_header.message_set == MS_TIMER)
    {
      next_gas_id = rr_ms_process_tmr_msg(cmd_ptr, &executor);
    }
#ifdef FEATURE_CGPS_UMTS_CP_GSM
    else if (cmd_ptr->message_header.message_set == MS_CGPS_RR)
    {
      next_gas_id = rr_ms_process_gps_msg(cmd_ptr);
    }
#endif /* FEATURE_CGPS_UMTS_CP_GSM */
    else if (cmd_ptr->message_header.message_set == MS_RLC_RR)
    {
      next_gas_id = rr_ms_process_rlc_msg(cmd_ptr);
    }
#ifdef FEATURE_WCDMA
    else if (cmd_ptr->message_header.message_set == MS_RRC_RR)
    {
      next_gas_id = rr_ms_process_rrc_msg(cmd_ptr);
    }
#endif /* FEATURE_WCDMA */
    else if (cmd_ptr->message_header.message_set == MS_GHDI_EXPORT)
    {
      /* this should only occur during dedicated mode */
      next_gas_id = rr_ms_connected_gas_id();
    }
#ifdef FEATURE_GSM_TDS
    else if (cmd_ptr->message_header.message_set == MS_TDSRRC_RR)
    {
      next_gas_id = rr_ms_process_tdsrrc_msg(cmd_ptr);
    }
#endif /* FEATURE_GSM_TDS */
    else if (cmd_ptr->message_header.message_set == MS_MSGR_RR)
    {
      /* process the MSGR header to derive the subscription ID */
      next_gas_id = rr_ms_process_msgr_header(&(cmd_ptr->rr_msgr_msg.message.hdr));
    }
    else
    {
      next_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
      MSG_GERAN_ERROR_0("Unhandled inter-task message set");
    }
  }

  if (executor.next_action != RR_MS_NO_ACTION)
  {
    /* a valid GAS ID should always be available */
    if (next_gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED)
    {
      MSG_GERAN_ERROR_2("GRR is unable to determine the active AS(%d,%d)",
                        (int)(cmd_ptr->message_header.message_set),
                        (int)(cmd_ptr->message_header.message_id));
    }
  }

  if (next_gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    /* check the execution control structure for actions that must occur now */
    switch(executor.next_action)
    {
    case RR_MS_PREPARE_FOR_SUSPENSION:
      if (rr_ms_prepare_for_suspension(RR_MS_INTERFACE_SUSPENDED, next_gas_id) == FALSE)
      {
        /* attempt to start suspension failed, try to clear up the GL1 interface */
        MSG_GERAN_ERROR_1("gs%1d: Unable to suspend, aborting interface now", 1+(int)next_gas_id);
        rr_ms_recover_blocked_interface(next_gas_id);
      }
      break;

    case RR_MS_CHECK_FOR_SUSPENSION:
      rr_ms_check_suspension_response(cmd_ptr, &executor, next_gas_id);
      break;

    case RR_MS_PREPARE_FOR_RESUMPTION:
      rr_ms_prepare_for_resumption(TRUE, next_gas_id);
      break;

    default:
      /* no further action required */
      break;
    }
  }

  /* return the current GAS ID if the caller provided an output parameter */
  if (gas_id_ptr != NULL)
  {
    (*gas_id_ptr) = next_gas_id;
  }

  return(executor.process_in_rr);
}

/*===========================================================================

FUNCTION      rr_ms_service_recovery_started

DESCRIPTION   Informs Priority Inversion Manager that GRR is about to start
              a cell (re)selection action related to recovering service.

DEPENDENCIES  This function relies on the assumption that GRR will send a
              further cell selection primitive to L1 (e.g. MPH_POWER_SCAN_REQ)
              very soon after this call (or even immediately).

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_service_recovery_started(const rr_ms_svc_recovery_t trigger_type, const gas_id_t gas_id)
{
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  /* store the last trigger type for debug purposes */
  rr_multisim_info.gas_data[as_index].ms_service_recovery = trigger_type;

  /* Set System Information FSM to RESET state */
  rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_RESET, gas_id);

  /* pass an appropriate recovery event into the Priority Inversion Manager */
  if ((RR_SERVICE_RECOVERY_TRANSCEIVER_DENIALS == trigger_type) ||
      (RR_SERVICE_RECOVERY_CELL_REESTABLISHING == trigger_type))
  {
    /* for TRM denials OR Cell Reestablishment triggers, inversion starts immediately */
    rr_ms_acquisition_manager(RR_MS_GAS_ID_SERVICE_RECOVERY, gas_id);
  }
  else
  {
    /* for other triggers, PIMMS starts its conventional duty cycle */
    rr_ms_acquisition_manager(RR_MS_GAS_ID_STARTS_ACQUIRING, gas_id);
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_plmn_list_search_starting

DESCRIPTION   Informs Priority Inversion Manager that GRR is about to start
              a PLMN list search. 

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_plmn_list_search_starting(rr_PLMN_list_req_info_T *req_ptr, 
                                     const rr_plc_search_type_e search_type, 
                                     const boolean high_priority, 
                                     const gas_id_t gas_id
                                    )
{
  if (geran_in_multi_standby_mode())
  {
    pri_inv_event_t pimms_event = RR_MS_PRIORITY_EVENT_UNDEFINED;

    if (req_ptr != NULL)
    {
      switch (search_type)
      {
        case RR_PLC_FOREGROUND_SEARCH:
        {
           /* the default event may now be overwritten for manual searches */
           /* as these are treated as high priority acquisition activities */
           /* if the service_search parameter is consistent with that mode */
           if ((SYS_NETWORK_SELECTION_MODE_MANUAL == req_ptr->network_selection_mode))
           {
              if ((SYS_SERVICE_SEARCH_NONE == req_ptr->service_search) || 
                  (SYS_SERVICE_SEARCH_CSG  == req_ptr->service_search))
              {
                MSG_GERAN_HIGH_0_G("PIMMS: high priority acquisition for PLMN search");
                pimms_event = RR_MS_GAS_ID_PRIORITY_NW_LIST;
              }
              else
              {
                MSG_GERAN_HIGH_1_G("PIMMS: manual service_search=%d not prioritised", (int)(req_ptr->service_search));
              }
           }
          break;
        }
        case RR_PLC_BACKGROUND_SEARCH:
        {
          pimms_event = RR_MS_GAS_ID_SEARCH_STARTED;
          break;
        }
#ifdef FEATURE_WCDMA
        case RR_PLC_WTOG_SEARCH:
#endif /* FEATURE_WCDMA */
#ifdef FEATURE_LTE
        case RR_PLC_LTOG_SEARCH:
#endif /* FEATURE_LTE */
#ifdef FEATURE_GSM_TDS
        case RR_PLC_TTOG_SEARCH:
#endif /*FEATURE_GSM_TDS*/
        {
          boolean new_proc_expected;
          new_proc_expected = !rr_mode_gsm_active_or_starting(gas_id);
          if (high_priority)
          {
            rr_ms_update_acquisition_priority(GERAN_PRIORITY_INVERTED, new_proc_expected, gas_id);
          }
          else
          {
            rr_ms_update_acquisition_priority(GERAN_PRIORITY_NOMINAL, new_proc_expected, gas_id);
          }
          break;
        }
        default:
        {
          pimms_event = RR_MS_PRIORITY_EVENT_UNDEFINED;
        }
      }
    }

    if (pimms_event != RR_MS_PRIORITY_EVENT_UNDEFINED)
    {
      /* update PIMMS with the appropriate event */
      rr_ms_acquisition_manager(pimms_event, gas_id);
    }
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_plmn_list_search_complete

DESCRIPTION   Informs PIMMS that a PLMN search has completed

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_plmn_list_search_complete(const gas_id_t gas_id)
{
  rr_ms_acquisition_manager(RR_MS_GAS_ID_SEARCH_COMPLETED, gas_id);
  return;
}

/*===========================================================================

FUNCTION     rr_ms_cs_establishment_confirmed

DESCRIPTION   Informs Priority Inversion Manager that GERAN is entering
              a connection establishment phase.

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_cs_establishment_confirmed(const gas_id_t gas_id)
{
  /* pass the establishment event into the Priority Inversion Manager */
  rr_ms_acquisition_manager(RR_MS_GAS_ID_CS_NOW_CONNECTED, gas_id);

  return;
}

/*===========================================================================

FUNCTION      rr_ms_oos_due_to_no_resource

DESCRIPTION   Checks whether OOS may have been caused by resource availability

DEPENDENCIES  None

RETURN VALUE  TRUE if GERAN should indicate NO_RESOURCE_AVAILABLE;

SIDE EFFECTS  None

===========================================================================*/
static boolean rr_ms_oos_due_to_no_resource(const gas_id_t gas_id)
{
  if (geran_in_multi_standby_mode())
  {
    boolean abort_timeout;
    int     as_index;

    as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /* retrieve and then reset the local timeout flag now */
    abort_timeout = rr_multisim_info.gas_data[as_index].cell_selection_timeout;
    rr_multisim_info.gas_data[as_index].cell_selection_timeout = FALSE;

    /* if a connected mode is detected on another stack, return immediately */
    if (rr_ms_connected_mode_detected(gas_id))
    {
      return(TRUE);
    }

    /* STEP #1: check the cell selection abort timeout */
    if (abort_timeout)
    {
      word num_of_cell_to_decode = rr_candidate_db_get_power_scan_offset(rr_get_candidate_cell_db_ptr(gas_id));

      /* STEP #1.1: check if no. of cell to be decoded is more than max candidate cell */
#ifdef FEATURE_GSM_DECODE_ALL_FREQS
      if ( TRUE == rr_ms_get_scan_more_freq_allowed(gas_id))
      {
        MSG_GERAN_HIGH_1_G("BCCH decoding done on %d cells, treat as genuine OOS", (int)num_of_cell_to_decode);
        return(FALSE);
      }
      else
#endif /* FEATURE_GSM_DECODE_ALL_FREQS */
      if (num_of_cell_to_decode > RR_MAX_CANDIDATE_FREQ_LIST)
      {
        MSG_GERAN_HIGH_1_G("BCCH decoding done on %d cells, treat as genuine OOS", (int)num_of_cell_to_decode);
        return(FALSE);
      }

      /* STEP #1.2: legacy DSDS retry behaviour for GSM-only mode */
      if (rr_gsm_only(gas_id))
      {
        /* subscription only has GSM capabilities, NO_RESOURCE will cause instant rescan */
        MSG_GERAN_HIGH_0_G("Cell selection abort timeout on GSM-only mode");
        return(TRUE);
      }

      /* STEP #1.3: check for total denial cases */
      if ((rr_multisim_info.gas_data[as_index].nominal_grants == 0) &&
          (rr_multisim_info.gas_data[as_index].inverse_grants == 0))
      {
        MSG_GERAN_HIGH_0_G("Cell selection abort timeout, total denial, treat as NO_RESOURCE");
        return(TRUE);
      }

      /* partial denial case, want to allow another RAT to be scanned */
      MSG_GERAN_HIGH_3_G("PIMMS: nominal grants %d inverse grants %d deferrals %d",
                         (int)(rr_multisim_info.gas_data[as_index].nominal_grants),
                         (int)(rr_multisim_info.gas_data[as_index].inverse_grants),
                         (int)(rr_multisim_info.gas_data[as_index].deferral_count));

      /* treat as genuine OOS */
      return(FALSE);
    }
    else
    {
      /* STEP #2: check for BCCH_BLOCKED results in candidate cell database */
      if (rr_candidate_db_failure_due_to_no_resource(gas_id))
      {
        MSG_GERAN_HIGH_0_G("Candidate DB reports blocked BCCH, treat as NO_RESOURCE");
        return(TRUE);
      }
      else
      {
        /* report the last service recovery trigger */
        MSG_GERAN_HIGH_1_G("Last service recovery trigger was %d",
                           (int)(rr_multisim_info.gas_data[as_index].ms_service_recovery));
      }
    }
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION      rr_ms_complete_nas_service_cnf

DESCRIPTION   Intercepts the PLMN_SELECT_CNF before it reaches NAS

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  May invoke priority inversion for OOS

===========================================================================*/
void rr_ms_complete_nas_service_cnf(rr_plmn_select_cnf_T *msg_ptr, const gas_id_t gas_id)
{
  if (msg_ptr != NULL)
  {
    msg_ptr->as_id = geran_map_gas_id_to_nas_id(gas_id);
    msg_ptr->failure_cause = MM_RR_OTHER_FAILURE;

    /* priority inversion can now be disabled, since we're either in service */
    /* or going OOS - for either case there is no GL1 activity to prioritise */
    rr_ms_acquisition_manager(RR_MS_GAS_ID_SERVICE_NOTIFIED, gas_id);

    /* check whether this access stratum has gone OOS due to lack of transceiver lock */
    if (MM_AS_NO_SERVICE == msg_ptr->service_state)
    {
      int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

      if (rr_ms_oos_due_to_no_resource(gas_id))
      {
        /* increment internal tracking counter */
        rr_multisim_info.gas_data[as_index].no_resource_count++;

        MSG_GERAN_ERROR_2_G("PLMN_SELECT_CNF will indicate NO_RESOURCE (OOS=%d, count=%d)",
                            (int)(msg_ptr->ue_oos), rr_multisim_info.gas_data[as_index].no_resource_count);
        msg_ptr->failure_cause = MM_RR_NO_RESOURCE_AVAILABLE;
        msg_ptr->ue_oos        = TRUE;
      }
      else
      {
      
        rr_nv_reset_last_camped_cell(gas_id);
        /* reset internal tracking counter */
        rr_multisim_info.gas_data[as_index].no_resource_count = 0;
      }
    }

    if (MM_AS_SERVICE_AVAILABLE == msg_ptr->service_state)
    {
      /* reset internal tracking counter, Service reported to NAS */
      rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].no_resource_count = 0;
    }

    /* set the aborted interface flag for the active stack */
    rr_ms_set_aborted_interface(FALSE, gas_id);
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_complete_nas_service_ind

DESCRIPTION   Intercepts the RR_SERVICE_IND before it reaches NAS

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  May initiate access stratum suspension/hibernation for OOS

===========================================================================*/
void rr_ms_complete_nas_service_ind(rr_service_ind_T *msg_ptr, const gas_id_t gas_id)
{
  if (msg_ptr != NULL)
  {
    int as_index;

    as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
    msg_ptr->as_id = geran_map_gas_id_to_nas_id(gas_id);
    msg_ptr->failure_cause = MM_RR_OTHER_FAILURE;

    /* priority inversion can now be disabled, since we're either in service */
    /* or going OOS - for either case there is no GL1 activity to prioritise */
    rr_ms_acquisition_manager(RR_MS_GAS_ID_SERVICE_NOTIFIED, gas_id);

    if (msg_ptr->service_state == MM_AS_NO_SERVICE)
    {
      MSG_GERAN_HIGH_0_G("NO SERVICE event being sent to NAS");

      if (rr_multisim_info.gas_data[as_index].gl1_interface_state != RR_MS_INTERFACE_ACTIVE)
      {
        /* loss of service on a suspended access stratum - record the time */
        /* use a +1 here, since time=0 is used to indicate "no occurrence" */
        rr_multisim_info.gas_data[as_index].svc_loss_time = rr_multisim_info.gas_data[as_index].seconds_since_suspension+1;

        /* update the message to indicate the probable cause */
        msg_ptr->failure_cause = MM_RR_NO_RESOURCE_AVAILABLE;

        /* abort any service request that was received during the suspension */
        rr_ms_abort_pending_service_request(TRUE, gas_id);
      }

      /* check whether this access stratum has gone OOS due to lack of transceiver lock */
      if (rr_ms_oos_due_to_no_resource(gas_id))
      {
        msg_ptr->failure_cause = MM_RR_NO_RESOURCE_AVAILABLE;
        MSG_GERAN_ERROR_0_G("RR_SERVICE_IND will indicate NO_RESOURCE_AVAILABLE");
      }
      else
      {
        rr_nv_reset_last_camped_cell(gas_id);
      }

      if ((MM_RR_NO_RESOURCE_AVAILABLE == msg_ptr->failure_cause)   &&
          (TRUE                        == rr_ms_hibernation_required(gas_id)))
      {
        /* putting this OOS access stratum back into a suspended state */
        /* will prevent NAS from bringing it back online straight away */
        if (rr_ms_prepare_for_suspension(RR_MS_INTERFACE_HIBERNATED, gas_id))
        {
          MSG_GERAN_HIGH_0_G("OOS hibernation started");
        }
        else
        {
          MSG_GERAN_ERROR_0_G("Unable to hibernate");
        }
      }
    }

#ifdef FEATURE_GSM_DECODE_ALL_FREQS
    /* reset scan more freq */
    rr_ms_reset_scan_more_freq_allowed(gas_id);
#endif /* FEATURE_GSM_DECODE_ALL_FREQS */

    /* reset the aborted interface flag each time a RR_SERVICE_IND is sent */
    rr_ms_set_aborted_interface(FALSE, gas_id);
  }

  return;
}

/*===========================================================================

FUNCTION       rr_ms_cs_connected

DESCRIPTION    Informs the multi-SIM controller of CS connection state

DEPENDENCIES   None

RETURN VALUE   VOID

SIDE EFFECTS   None

===========================================================================*/
void rr_ms_cs_connected(const boolean is_connected, const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    /* this GAS ID has just started a dedicated mode connection */
    rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].cs_connected = is_connected;
  }
  rr_ms_deactivate_procedure_lock(gas_id);
  return;
}

/*===========================================================================

FUNCTION       rr_ms_cs_conn_failed

DESCRIPTION    Informs the multi-SIM controller of CS connection failure

DEPENDENCIES   None

RETURN VALUE   VOID

SIDE EFFECTS   None

===========================================================================*/
void rr_ms_cs_conn_failed(const gas_id_t gas_id)
{
  rr_ms_deactivate_procedure_lock(gas_id);
  return;
}

/*===========================================================================

FUNCTION       rr_ms_other_stack_in_dedicated

DESCRIPTION    Determines whether any other GAS ID is in CS connected state

DEPENDENCIES   None

RETURN VALUE   TRUE if another GAS ID is connected, FALSE otherwise

SIDE EFFECTS   None

===========================================================================*/
boolean rr_ms_other_stack_in_dedicated(const gas_id_t gas_id)
{
  gas_id_t other_gas_id;

  for (other_gas_id = GERAN_ACCESS_STRATUM_ID_1; other_gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; other_gas_id++)
  {
    if (other_gas_id != gas_id)
    {
      if (rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(other_gas_id)].cs_connected)
      {
        return(TRUE);
      }
    }
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION       rr_ms_rps_required_before_connection

DESCRIPTION    Determines if serving cell has to be resynced before handling a connection request

DEPENDENCIES   None

RETURN VALUE   TRUE if another GAS ID is connected for reason other than LAU or SMS or Supplementary Service, 
               FALSE otherwise

SIDE EFFECTS   None

===========================================================================*/
boolean rr_ms_rps_required_before_connection(const gas_id_t gas_id)
{
  if (geran_in_multi_standby_mode())
  {
    gas_id_t other_gas_id;

    for (other_gas_id = GERAN_ACCESS_STRATUM_ID_1; other_gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; other_gas_id++)
    {
      if (other_gas_id != gas_id)
      {
        /* Check if other sub is in dedicated */
        if (rr_in_dedicated(other_gas_id))
        {
          geran_traffic_reason_t traffic_reason = geran_get_traffic_reason(NULL, other_gas_id);

          if ((traffic_reason != GERAN_TRAFFIC_CS_REGISTRATION) &&
              (traffic_reason != GERAN_TRAFFIC_CS_SIGNALLING)   &&
              (traffic_reason != GERAN_TRAFFIC_MO_SMS))
          {
            return(TRUE);
          }
        }
      }
    }
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION      rr_ms_task_start_init

DESCRIPTION   Initialise the DS control module at task startup

DEPENDENCIES  This function must be called before ANY other RR initialisation

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_task_start_init(void)
{
  int as_index;

  /* call the local function to reset everything internally */
  rr_ms_reset_internal_variables();

  /* initialise everything that affects external interfaces (NAS etc.) */
  for (as_index = GERAN_DATA_SPACE_INDEX_1; as_index < NUM_GERAN_DATA_SPACES; as_index++)
  {
    rr_multisim_info.gas_data[as_index].paging_group       = RR_MS_INVALID_PAGING_GROUP;
    rr_multisim_info.gas_data[as_index].ps_abort_delay     = 0;
    rr_multisim_info.gas_data[as_index].procedure_duration = RR_MINIMUM_PROCEDURE_DURATION;
    rr_multisim_info.gas_data[as_index].buffered_page_time = RR_MS_INVALID_TIME_STAMP;
    rr_multisim_info.gas_data[as_index].missed_page        = RR_MS_INVALID_TIME_STAMP;
    rr_multisim_info.gas_data[as_index].page_cycle         = 0;
    rr_multisim_info.gas_data[as_index].gprs_test_mode     = FALSE;
    rr_multisim_info.gas_data[as_index].proc_lock_status   = RR_MS_PROC_LOCK_INACTIVE;
    rr_multisim_info.gas_data[as_index].proc_lock_cause    = RR_MS_PROC_LOCK_NONE;
    memset(&(rr_multisim_info.gas_data[as_index].debug_events), 0, sizeof(rr_ms_debug_info_t));
  }

  /* reset multimode capability storage */
  rr_ms_store_subscription_capabilities(NULL);

  /* Register a set of callback functions that can be */
  /* used for demultiplexing of CCCH (or CBCH) blocks */
  rr_ms_register_channel_sharing();

  /* Clear the per-tech information */
  memset(rr_ms_tech_info, 0, sizeof(rr_ms_tech_info));

  return;
}

/*===========================================================================

FUNCTION      rr_ms_store_paging_info

DESCRIPTION   Calculates and stores the paging group based on OTA parameters

DEPENDENCIES  Must be called within the active context of one of the stacks

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_store_paging_info(layer1_sys_info_T *sys_info_ptr, word imsi_mod_1000, const gas_id_t gas_id)
{
  timetick_type page_cycle = 0;
  word          page_group = RR_MS_INVALID_PAGING_GROUP;

  if (sys_info_ptr != NULL)
  {
    word n;
    word i;

    if (sys_info_ptr->BS_CCCH_SDCCH_COMB == FALSE)
    {
       n = (9 - sys_info_ptr->BS_AG_BLKS_RES);
    }
    else
    {
       n = (3 - sys_info_ptr->BS_AG_BLKS_RES);
    }

    n *= sys_info_ptr->BS_PA_MFRMS;
    page_cycle = 235 * sys_info_ptr->BS_PA_MFRMS;

    i = (word)(n * sys_info_ptr->BS_CC_CHANS);

    if (i != 0)
    {
      page_group = (imsi_mod_1000 % i) % n;
    }
  }

  /* store the calculated value in this module's data structure */
  rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].paging_group = page_group;
  rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].page_cycle   = page_cycle;
  return;
}

/*===========================================================================

FUNCTION      rr_ms_all_stacks_in_state

DESCRIPTION   Checks the RR control state for all subscriptions

DEPENDENCIES  None

RETURN VALUE  TRUE if all subscriptions are in the specified state

SIDE EFFECTS  None

===========================================================================*/
boolean rr_ms_all_stacks_in_state(const rr_state_T rr_state)
{
  gas_id_t gas_id;

  /* check the state of each data space in turn */
  for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
  {
    if (!rr_is_control_state(rr_state, gas_id))
    {
      return(FALSE);
    }
  }

  return(TRUE);
}

/*===========================================================================

FUNCTION      rr_ms_gprs_active_on_other_stack

DESCRIPTION   Checks whether another GERAN stack has an active GPRS data call

DEPENDENCIES  None

RETURN VALUE  TRUE if GMAC is not idle on any other subscription

SIDE EFFECTS  None

===========================================================================*/
boolean rr_ms_gprs_active_on_other_stack(const gas_id_t calling_gas_id)
{
  gas_id_t gas_id;

  /* check the active GERAN clients first */
  for (gas_id = GERAN_ACCESS_STRATUM_ID_1; gas_id <= GERAN_ACCESS_STRATUM_ID_MAX; gas_id++)
  {
    if ((gas_id   != calling_gas_id) &&
        (MAC_IDLE != rr_macinfo_get_mac_status(gas_id)))
    {
      return(TRUE);
    }
  }

  return(FALSE);
}

/*===========================================================================

FUNCTION      rr_ms_clear_missed_page

DESCRIPTION   Resets the missed page time stamp

DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_clear_missed_page(const gas_id_t gas_id)
{
  int as_index;

  /* determine which access stratum is in use */
  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  if (as_index < NUM_GERAN_DATA_SPACES)
  {
    /* Reset the missed page timestamp */
    rr_multisim_info.gas_data[as_index].missed_page = RR_MS_INVALID_TIME_STAMP;
  }

  return;
}

/*===========================================================================

FUNCTION       rr_ms_l1_blocking_message_sent

DESCRIPTION    Indicates that a message has been sent via the L1 interface
               that may potentially cause L1 to be blocked by TRM

DEPENDENCIES   Device must be operating in DSDx mode

RETURN VALUE   VOID

SIDE EFFECTS   None

===========================================================================*/
void rr_ms_l1_blocking_message_sent(const rr_l1_message_id_enum_T required_msg_id, const gas_id_t gas_id)
{
  if ((required_msg_id != RR_MS_INVALID_L1_MESSAGE)    &&
      (gas_id          <= GERAN_ACCESS_STRATUM_ID_MAX) &&
      RR_MULTI_SIM_MODE_ENABLED)
  {
    int as_index;

    /* establish a data space index for the supplied GAS ID */
    as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /* new primitive being sent to Layer 1, reset the interface flags */
    rr_ms_set_aborted_interface(FALSE, gas_id);

    /* log an appropriate event in the debug buffer */
    rr_ms_store_debug_event(RR_MS_BLOCKING_MSG_SENT, gas_id);

    /* store the required GL1 response to this primitive */
    rr_multisim_info.gas_data[as_index].required_l1_message = required_msg_id;

    /* Inform PIMMS that blocking message is sent so that it can start its cycle only if its not the case for PLMN */
    if (!rr_plmn_search_in_progress(gas_id))	
    {
      rr_ms_acquisition_manager(RR_MS_GAS_ID_BLOCKING_MSG_SENT, gas_id);
    }

    MSG_GERAN_HIGH_1_G("GRR requires GL1 msg 0x%02X", (int)required_msg_id);
  }

  return;
}

/*===========================================================================

FUNCTION       rr_ms_l1_shutdown_msg_expected

DESCRIPTION    Indicates that a message has been sent via the L1 interface
               that requires deactivation of the physical layer

DEPENDENCIES   None

RETURN VALUE   VOID

SIDE EFFECTS   None

===========================================================================*/
void rr_ms_l1_shutdown_msg_expected(const rr_l1_message_id_enum_T shutdown_cnf_id, const gas_id_t gas_id)
{
  if ((shutdown_cnf_id != RR_MS_INVALID_L1_MESSAGE)    &&
      (gas_id          <= GERAN_ACCESS_STRATUM_ID_MAX) &&
      RR_MULTI_SIM_MODE_ENABLED)
  {
    int as_index;

    /* establish a data space index for the supplied GAS ID */
    as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

    /* this access stratum is either shutting down or resetting Layer 1 */
    rr_multisim_info.gas_data[as_index].required_l1_message = RR_MS_INVALID_L1_MESSAGE;

    /* deactivate procedure lock */
    rr_ms_deactivate_procedure_lock(gas_id);

    /* Set System Information FSM to RESET state */
    rr_ms_sys_info_manager(RR_MS_SYS_INFO_EV_RESET, gas_id);

    /* Reset PIMMS */
    rr_ms_acquisition_manager(RR_MS_PRIORITY_INVERSION_INIT, gas_id);

    /* if suspended, behave as though the access stratum is resuming now */
    if (rr_multisim_info.gas_data[as_index].gl1_interface_state != RR_MS_INTERFACE_ACTIVE)
    {
      if ((RR_MS_INTERFACE_HIBERNATED == rr_multisim_info.gas_data[as_index].gl1_interface_state) &&
          (MPH_STOP_GSM_MODE_CNF      == shutdown_cnf_id))
      {
        /* if the access stratum is about to hibernate, don't resume yet */
        MSG_GERAN_HIGH_0_G("GL1 is being shut down for hibernation");
      }
      else
      {
        /* normal suspension is in effect, or a panic reset was triggered */
        rr_ms_prepare_for_resumption(FALSE, gas_id);
      }
    }
    else
    {
      /* ensure that this access stratum is not flagged as being CS connected */
      rr_ms_cs_released(gas_id);
    }

    MSG_GERAN_HIGH_1_G("GRR requires GL1 msg 0x%02X", (int)shutdown_cnf_id);
  }

  return;
}

/*===========================================================================

FUNCTION       rr_ms_access_stratum_deactivated

DESCRIPTION    Completes clean-up in this module when the AS is deactivated

DEPENDENCIES   Should be called just before deactivation is confirmed to NAS

RETURN VALUE   VOID

SIDE EFFECTS   None

===========================================================================*/
void rr_ms_access_stratum_deactivated(const sys_modem_as_id_e_type nas_id, const boolean stop_mode_success)
{
  gas_id_t gas_id;
  int      as_index;

  /* convert the supplied NAS ID to a GERAN access stratum identifier */
  gas_id   = geran_map_nas_id_to_gas_id(nas_id);
  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  /* ensure that suspension or hibernation is cleaned up appropriately */
  if (stop_mode_success == TRUE)
  {
    /* cancel the termination timer if it is running */
    if (rr_timer_cancel(RR_DUAL_SIM_TERMINATE_TIMER, gas_id))
    {
      MSG_GERAN_HIGH_0_G("Cancelled termination timer");
    }
  }
  else
  {
    rr_ms_prepare_for_resumption(FALSE, gas_id);
  }

  /* reset cell selection timeout and service recovery flags */
  rr_multisim_info.gas_data[as_index].cell_selection_timeout = FALSE;
  rr_multisim_info.gas_data[as_index].ms_service_recovery    = RR_SERVICE_RECOVERY_NONE;

  /* reset Layer 1 interface control flags */
  rr_multisim_info.gas_data[as_index].aborted_l1_interface   = FALSE;
  rr_multisim_info.gas_data[as_index].required_l1_message    = RR_MS_INVALID_L1_MESSAGE;

  /* reset GMAC interface control flags and paging group info */
  rr_multisim_info.gas_data[as_index].paging_group           = RR_MS_INVALID_PAGING_GROUP;
  rr_multisim_info.gas_data[as_index].missed_page            = RR_MS_INVALID_TIME_STAMP;

  /* update the Priority Inversion Manager for this subscription */
  rr_ms_acquisition_manager(RR_MS_GAS_ID_SERVICE_NOTIFIED, gas_id);
  MSG_GERAN_HIGH_0_G("Deactivation of GERAN access stratum");

  return;
}

/*===========================================================================

FUNCTION       rr_ms_power_scan_abort_required

DESCRIPTION    Notifies the controller that a power scan needs to abort

DEPENDENCIES   None

RETURN VALUE   VOID

SIDE EFFECTS   May trigger sending of MPH_L1_DS_ABORT_IND

===========================================================================*/
void rr_ms_power_scan_abort_required(const rr_event_T abort_event, const gas_id_t gas_id)
{
  /* log an appropriate event in the debug buffer */
  if (RR_EV_CELL_SELECTION_ABORT_TIMEOUT != abort_event)
  {
    rr_ms_store_debug_event(RR_MS_GRR_ABORT_BY_FUNC, gas_id);
  }

#if !defined(PERLUTF) && !defined(TEST_FRAMEWORK)
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    rr_l1_message_id_enum_T pending_msg;

    /* determine which message is pending from GL1 (this function is intended for power scans only) */
    pending_msg = rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].required_l1_message;

    /* check whether GRR is still waiting for a power scan response (and has not already aborted) */
    if (MPH_POWER_SCAN_CNF == pending_msg)
    {
      if (rr_ms_interface_was_aborted(gas_id))
      {
        /* GRR has already sent a DS_ABORT_IND for this power scan */
        MSG_GERAN_HIGH_0_G("Interface aborted already, waiting for MPH_POWER_SCAN_CNF");
      }
      else
      {
        /* recover the blocked interface in the usual way, and */
        /* allow the power scan to complete if it has TRM lock */
        rr_ms_recover_blocked_interface(gas_id);
      }
    }
    else if (RR_MS_INVALID_L1_MESSAGE != pending_msg)
    {
      MSG_GERAN_ERROR_1_G("Power scan abort required but msg 0x%02X pending", (int)pending_msg);
    }
  }
#endif /* !PERLUTF && !TEST_FRAMEWORK */

  return;
}

/*===========================================================================

FUNCTION     rr_ms_confirm_mm_ps_abort_cb_fn

DESCRIPTION  Callback function called when RR-CONTROL confirm PS is blocked.
             RR_DUAL_SIM_PS_ABORT_TIMER_1 is started.

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_confirm_mm_ps_abort_cb_fn(const gas_id_t gas_id)
{
  // Start the PS abort timer using the specified time-out
  // PS will remain blocked as long as this timer is running
  if (rr_timer_start(RR_DUAL_SIM_PS_ABORT_TIMER_1, RR_DUAL_SIM_MM_PS_ABORT_TIMEOUT, gas_id))
  {
    MSG_GERAN_HIGH_1_G("RR_DUAL_SIM_PS_ABORT_TIMER_1 started for %dms", RR_DUAL_SIM_MM_PS_ABORT_TIMEOUT);
  }

  // Send a confirmation to MM
  rr_ms_send_rr_ps_abort_cnf(gas_id);

  return;
}

/*===========================================================================

FUNCTION       rr_ms_process_ps_abort_req

DESCRIPTION    Called from RR-PS-DOMAIN when RR_PS_ABORT_REQ is received from MM

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void rr_ms_process_ps_abort_req(const gas_id_t gas_id)
{
  // Request RR-CONTROL to block PS access
  rr_send_imsg_no_ps_access_req(
    FALSE,                            // del_all_pdus
    rr_ms_confirm_mm_ps_abort_cb_fn,  // confirm_cb_fn
    gas_id
  );

  return;
}

/*===========================================================================

FUNCTION     rr_ms_confirm_l1_ds_abort_cb_fn

DESCRIPTION  Callback function called when RR-CONTROL confirm PS is blocked.
             RR_DUAL_SIM_PS_ABORT_TIMER_1 is started.

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS None

===========================================================================*/
static void rr_ms_confirm_l1_ds_abort_cb_fn(const gas_id_t gas_id)
{
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  // Start the PS abort timer using the specified time-out
  // PS will remain blocked as long as this timer is running
  if (rr_timer_start(RR_DUAL_SIM_PS_ABORT_TIMER_1, rr_multisim_info.gas_data[as_index].ps_abort_delay, gas_id))
  {
    MSG_GERAN_HIGH_1_G("RR_DUAL_SIM_PS_ABORT_TIMER_1 started for %dms", rr_multisim_info.gas_data[as_index].ps_abort_delay);
  }

  return;
}

/*===========================================================================

FUNCTION       rr_ms_process_l1_ds_abort_ind

DESCRIPTION    Called from RR-MAIN from MPH_L1_DS_ABORT_IND is received from L1

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void rr_ms_process_l1_ds_abort_ind(mph_l1_ds_abort_ind_T *msg_ptr)
{
  gas_id_t gas_id;

  RR_NULL_CHECK_FATAL(msg_ptr);

  /* extract the GAS ID from the message */
  gas_id = msg_ptr->gas_id;

  if ((RR_DS_ABORT_TRANSFER == msg_ptr->abort_type) ||
      (RR_DS_ABORT_TIMEDOUT == msg_ptr->abort_type))
  {
    if (rr_mac_ps_access_blocked(gas_id))
    {
      /* GMAC has already been told to disable GPRS activity */
      /* so this new abort indication from GL1 is either:    */
      /* a) a repetition of RR_DS_ABORT_TIMEDOUT, which usually means GL1 is stuck, or */
      /* b) a collision between an internal GL1 abort and some other PS release in GRR */
      /* For each of these scenarios, it should be OK to ignore the new abort from GL1 */
      if (RR_DS_ABORT_TIMEDOUT == msg_ptr->abort_type)
      {
        MSG_GERAN_ERROR_0_G("Duplicate RR_DS_ABORT_TIMEDOUT ignored");
        return;
      }
      else /* RR_DS_ABORT_TRANSFER */
      {
        rr_ps_abort_block_t ps_abort_active;

        /* if no PS abort process is active, start the secondary timer now */
        ps_abort_active = rr_ms_ps_abort_process_active(gas_id);
        if (RR_PS_ABORT_NOT_ACTIVE == ps_abort_active)
        {
          /* while this timer is running, no new UL or DL establishment can take place */
          if (rr_timer_start(RR_DUAL_SIM_PS_ABORT_TIMER_2, msg_ptr->timeout_ms, gas_id))
          {
            MSG_GERAN_HIGH_1_G("RR_DUAL_SIM_PS_ABORT_TIMER_2 started for %dms", msg_ptr->timeout_ms);
          }
        }
        MSG_GERAN_HIGH_1_G("RR_DS_ABORT_TRANSFER, MAC blocked (abort %d)", (int)ps_abort_active);
        return;
      }
    }

    // Store the delay required for the RR_DUAL_SIM_PS_ABORT_TIMER_1 timer
    if (msg_ptr->timeout_ms == 0)
    {
      rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].ps_abort_delay = RR_DUAL_SIM_L1_PS_ABORT_TIMEOUT;;
      MSG_GERAN_HIGH_1_G("PS abort delay corrected to %d", RR_DUAL_SIM_L1_PS_ABORT_TIMEOUT);
    }
    else
    {
      rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].ps_abort_delay = msg_ptr->timeout_ms;
    }

    // Request RR-CONTROL to block PS access
    rr_send_imsg_no_ps_access_req(
      FALSE,                             // del_all_pdus
      rr_ms_confirm_l1_ds_abort_cb_fn,   // confirm_cb_fn
      gas_id                             // gas_id
    );
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Unexpected abort type %d received from GL1", (int)(msg_ptr->abort_type));
  }

  return;
}

/*===========================================================================

FUNCTION     rr_ms_ps_abort_timer_expiry

DESCRIPTION  Called when either RR_DUAL_SIM_PS_ABORT_TIMER_x timer expires.
             PS access is resumed for RR_DUAL_SIM_PS_ABORT_TIMER_1 only

DEPENDENCIES None

RETURN VALUE None

SIDE EFFECTS RR_DUAL_SIM_PS_ABORT_TIMER_2 is cancelled

===========================================================================*/
void rr_ms_ps_abort_timer_expiry(const rr_timer_id_e ps_abort_id, const gas_id_t gas_id)
{
  if (RR_DUAL_SIM_PS_ABORT_TIMER_1 == ps_abort_id)
  {
    // Request PS to be enabled
    rr_send_imsg_ps_access_req(
      FALSE,   // purge_gmm_signalling
      NULL,    // confirm_cb_fn
      gas_id   // gas_id
    );
  }

  /* always cancel the secondary timer if it is still running */
  (void)rr_timer_cancel(RR_DUAL_SIM_PS_ABORT_TIMER_2, gas_id);

  return;
}

/*===========================================================================

FUNCTION     rr_ms_ps_abort_process_active

DESCRIPTION  Determines the status of PS Abort processing

DEPENDENCIES None

RETURN VALUE RR_PS_ABORT_NOT_ACTIVE
             RR_PS_ABORT_ACTIVE_WILL_RESUME
             RR_PS_ABORT_ACTIVE_TBF_BLOCKED

SIDE EFFECTS None

===========================================================================*/
rr_ps_abort_block_t rr_ms_ps_abort_process_active(const gas_id_t gas_id)
{
  if (rr_timer_is_running(RR_DUAL_SIM_PS_ABORT_TIMER_1, gas_id))
  {
    /* this timer will resume PS access when it expires */
    return(RR_PS_ABORT_ACTIVE_WILL_RESUME);
  }
  else if (rr_timer_is_running(RR_DUAL_SIM_PS_ABORT_TIMER_2, gas_id))
  {
    /* this timer only blocks TBF establishment while it is active */
    return(RR_PS_ABORT_ACTIVE_TBF_BLOCKED);
  }

  return(RR_PS_ABORT_NOT_ACTIVE);
}

/*===========================================================================

FUNCTION      rr_ms_acquisition_started

DESCRIPTION   Informs Priority Inversion Manager that GRR is about to
              start GERAN acquisition for an Acquition  procedure
                       
DEPENDENCIES  None

RETURN VALUE  VOID

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_acquisition_started(const gas_id_t gas_id)
{
  /* pass the acquiring event into the Priority Inversion Manager */
  rr_ms_acquisition_manager(RR_MS_GAS_ID_STARTS_ACQUIRING, gas_id);

  return;
}

/*!
 * \brief Converts a CxM tech type into a GERAN index
 * 
 * \param cxm_tech_type (in)
 */
static int rr_ms_get_tech_info_index(const cxm_tech_type cxm_id)
{
  switch(cxm_id)
  {
    case CXM_TECH_GSM1:      return(RR_TECH_GSM1);
    case CXM_TECH_GSM2:      return(RR_TECH_GSM2);
    case CXM_TECH_GSM3:      return(RR_TECH_GSM3);
    case CXM_TECH_LTE:       return(RR_TECH_LTE);
    case CXM_TECH_TDSCDMA:   return(RR_TECH_TDSCDMA);
    case CXM_TECH_WCDMA:
    case CXM_TECH_WCDMA2:    return(RR_TECH_WCDMA);
    case CXM_TECH_ONEX:      return(RR_TECH_ONEX);
    case CXM_TECH_HDR:       return(RR_TECH_HDR);

    default:
      break;
  }

  return(-1);
}

/*!
 * \brief Processes MCS_CXM_COEX_TECH_STATE_BCAST_IND.
 * 
 * \param tech_state_bcast_ind_ptr (in)
 */
void rr_ms_store_tech_state(cxm_coex_tech_state_bcast_ind_s *tech_state_bcast_ind_ptr)
{
  rr_ms_tech_info_t *tech_info_ptr;
  boolean            timer_required;
  boolean            timer_is_running;
  boolean            any_tx_active;
  uint8              cxm_num_active;
  uint8              i;
  int                tech_index;

  /* sanitise input data */
  RR_NULL_CHECK_RETURN_VOID(tech_state_bcast_ind_ptr);
  if (tech_state_bcast_ind_ptr->num_active_techs <= CXM_MAX_CONCURRENT_TECHS)
  {
    cxm_num_active = tech_state_bcast_ind_ptr->num_active_techs;
  }
  else
  {
    cxm_num_active = CXM_MAX_CONCURRENT_TECHS;
    MSG_GERAN_ERROR_1("MCS_CXM_COEX_TECH_STATE_BCAST_IND report capped at %d techs", (int)cxm_num_active);
  }

  // Now replace the information for those techs which are active
  any_tx_active = FALSE;
  for (i = 0; i < cxm_num_active; i++)
  {
    /* map the CxM tech ID to an internal array index */
    tech_index = rr_ms_get_tech_info_index(tech_state_bcast_ind_ptr->tech_state[i].tech_id);

    /* range check the array index before using it */
    if ((tech_index >= 0) && (tech_index < ARR_SIZE(rr_ms_tech_info)))
    {
      tech_info_ptr = &(rr_ms_tech_info[tech_index]);

      if (tech_state_bcast_ind_ptr->tech_state[i].tx_active)
      {
        tech_info_ptr->tx_active = TRUE;
        any_tx_active            = TRUE;

        if (tech_info_ptr->inactive_counter > 0)
        {
          tech_info_ptr->inactive_counter = 0;
        }
      }
      else
      {
        if (tech_info_ptr->tx_active)
        {
          if (tech_info_ptr->inactive_counter == 0)
          {
            tech_info_ptr->inactive_counter = RR_TECH_RECENT_ACTIVITY_MAX_TICKS;
          }
        }
      }
    }
  } // for loop

  /* check whether any entry now has a non-zero inactive_count */
  timer_required = FALSE;
  for (i = 0; i < ARR_SIZE(rr_ms_tech_info); i++)
  {
    if (rr_ms_tech_info[i].inactive_counter > 0)
    {
      timer_required = TRUE;
      break;
    }
  }

  // Check if the timer is already running
  timer_is_running = rr_timer_is_running(RR_TECH_RECENT_ACTIVITY_TIMER, GERAN_ACCESS_STRATUM_ID_1);

  if (timer_required)
  {
    // If the timer is required and isn't already running, then start it now
    if (FALSE == timer_is_running)
    {
      (void)rr_timer_start(RR_TECH_RECENT_ACTIVITY_TIMER,
                           RR_TECH_RECENT_ACTIVITY_TICK_DURATION,
                           GERAN_ACCESS_STRATUM_ID_1);
    }
  }
  else
  {
    // If the timer isn't required and is currently running, then stop it now
    if (timer_is_running)
    {
      (void)rr_timer_cancel(RR_TECH_RECENT_ACTIVITY_TIMER, GERAN_ACCESS_STRATUM_ID_1);
    }
  }

  return;
}
/*===========================================================================

FUNCTION       rr_ms_plmn_search_invalidate_drx_count

DESCRIPTION    Invalidate the DRX count

DEPENDENCIES   None

RETURN VALUE   None

SIDE EFFECTS   None

===========================================================================*/
void rr_ms_plmn_search_invalidate_drx_count(const gas_id_t gas_id)
{
  int as_index;
  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  rr_multisim_info.gas_data[as_index].drx_count = RR_PLMN_LIST_INVALID_DRX_COUNT;
}

/*===========================================================================

FUNCTION      rr_ms_plmn_search_init_drx_count

DESCRIPTION   Initializes  the DRX count to zero

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
void rr_ms_plmn_search_init_drx_count(const gas_id_t gas_id)
{
  int as_index;
  as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);
  rr_multisim_info.gas_data[as_index].drx_count = RR_PLMN_LIST_MIN_DRX_COUNT;
}

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
/*===========================================================================

FUNCTION       rr_ms_plmn_search_update_priority_rqrd

DESCRIPTION    For BPLMN seacrh G2X, G2G, it determines whether  priority  to be used during BPLMN search be 
               nominal or inverted

DEPENDENCIES   None

RETURN VALUE   TRUE if inverted, FALSE otherwise

SIDE EFFECTS   None

===========================================================================*/
boolean rr_ms_plmn_search_update_priority_rqrd(const boolean g2x_srch_in_prgrs, const boolean new_proc_expected, const gas_id_t gas_id)
{
  int                               as_index;
  geran_priority_t                  priority        = GERAN_PRIORITY_NOMINAL;  
  boolean                           update_priority = FALSE;
  rr_PLMN_list_req_info_T           *req_ptr        = rr_get_plmn_list_req_info_ptr(gas_id);
  sys_network_selection_mode_e_type net_sel_mode    = req_ptr->network_selection_mode;
  as_index                                          = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if (!geran_in_multi_standby_mode())
  {
    return FALSE;
  }

  if (RR_PLMN_LIST_INVALID_DRX_COUNT == rr_multisim_info.gas_data[as_index].drx_count)
  {
    MSG_GERAN_ERROR_0_G("Invalid BPLMN initiated , search follow nominal priority");    
    return FALSE;
  }
  
  if ( FALSE == g2x_srch_in_prgrs)
  {
    rr_multisim_info.gas_data[as_index].drx_count++;
  }
  
  if (rr_multisim_info.gas_data[as_index].drx_count == RR_PLMN_LIST_INITIAL_DRX_COUNT)
  {
    if (net_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
    {
      priority        = GERAN_PRIORITY_INVERTED;
      update_priority = TRUE;   
    }
    else if (net_sel_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
    {
      priority        = GERAN_PRIORITY_NOMINAL;
      update_priority = TRUE; 
    }
  }
  else if (RR_PLMN_LIST_MIN_DRX_COUNT == (rr_multisim_info.gas_data[as_index].drx_count % RR_PLMN_LIST_MAX_DRX_COUNT))
  {
    if (net_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
    {
      update_priority = TRUE; 
      priority = GERAN_PRIORITY_NOMINAL;
    }
    else if (net_sel_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
    {
      if (!geran_api_is_alt_sub_in_data(gas_id))
      {
        update_priority = TRUE; 
        priority = GERAN_PRIORITY_INVERTED;
      }
      else
      {
        MSG_GERAN_HIGH_0_G("Auto BPLMN search ,no Inv rqrd, other tech in PS");      
      }
    }
    rr_multisim_info.gas_data[as_index].drx_count = RR_PLMN_LIST_MIN_DRX_COUNT;
  }
  else if (TRUE == g2x_srch_in_prgrs)
  {    
    if (net_sel_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
    {
      update_priority = TRUE; 
      priority = GERAN_PRIORITY_INVERTED;
    }  
  }

  MSG_GERAN_HIGH_3_G("BPLMN search DRx count is =%d Update Rqrd = %d, Priority = %d",
                      rr_multisim_info.gas_data[as_index].drx_count,
                      update_priority,
                      priority);    
  
  if ((update_priority) && (FALSE == g2x_srch_in_prgrs))
  {
    rr_ms_update_acquisition_priority(priority, new_proc_expected, gas_id);
  } 
  
  return (update_priority && (priority == GERAN_PRIORITY_INVERTED));
#else
  return FALSE;
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */  
}

/*===========================================================================

FUNCTION       rr_multi_sim_mode_enabled

DESCRIPTION    Determines whether the GERAN AS is in Multi SIM mode

DEPENDENCIES   None

RETURN VALUE   TRUE if Multi SIM mode is enabled in the GERAN AS

SIDE EFFECTS   None

===========================================================================*/
boolean rr_multi_sim_mode_enabled(void)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  return(RR_MULTI_SIM_MODE_ENABLED);
#else
  return(FALSE);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
}

boolean rr_multi_sim_api_valid(const gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if (SYS_MODEM_AS_ID_1 != geran_map_gas_id_to_nas_id(gas_id))
  {
    return(FALSE);
  }
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(TRUE);
}

/*!
 * \brief Called by inter-RAT modules to report a failure
 *
 * \param failure_type, gas_id (in)
 *
 * \return VOID
 */
void rr_ms_x2g_procedure_failure(const rr_proc_failure_type_t failure_type, const gas_id_t gas_id)
{
  boolean lock_exchange_for_return_to_x_rat;

  /* determine whether a lock exchange is allowed for the return to the X-RAT */
  switch(failure_type)
  {
  case RR_X2G_REDIRECTION_FAILURE:
  case RR_X2G_HANDOVER_FAILURE:
    lock_exchange_for_return_to_x_rat = TRUE;
    break;

  case RR_X2G_REDIRECTION_ABORT_BY_RRC:
    lock_exchange_for_return_to_x_rat = FALSE;
    break;

  case RR_X2G_RESELECTION_FAILURE:
#ifdef FEATURE_LTE
    if (RR_RAT_LTE == rr_get_x2g_resel_source_rat(gas_id))
    {
      lock_exchange_for_return_to_x_rat = TRUE;
    }
    else
#endif /* FEATURE_LTE */
    {
      lock_exchange_for_return_to_x_rat = FALSE;
    }
    break;

  default:
    lock_exchange_for_return_to_x_rat = FALSE;
    break;
  }

  /* arrange TRM lock exchange with other RAT if required */
  if (geran_was_lock_exchanged(GERAN_EXCHANGE_XTOG, gas_id) &&
      (TRUE == lock_exchange_for_return_to_x_rat))
  {
    /* advise GL1 to exchange rather than release this client */
    /* note that the direction indicator is the OPPOSITE here */
    geran_set_exchange_on_release(GERAN_EXCHANGE_GTOX, gas_id);

    /* then clear the X2G flag for future procedures */
    geran_clear_lock_exchanged(GERAN_EXCHANGE_XTOG, gas_id);
  }
  else
  {
    /* clear the release flag so that GL1 doesn't act on it */
    geran_set_exchange_on_release(GERAN_EXCHANGE_NULL, gas_id);

    /* and clear the G2X flag for future procedures */
    geran_clear_lock_exchanged(GERAN_EXCHANGE_GTOX, gas_id);

    MSG_GERAN_HIGH_0_G("Inter-RAT exchange not used for this procedure");
  }

  return;
}

/*===========================================================================

FUNCTION      rr_ms_sub_capabilities_exclude_sys_mode

DESCRIPTION   Determines whether a specific sys mode is excluded

DEPENDENCIES  None

RETURN VALUE  TRUE if the specified sys mode is excluded

SIDE EFFECTS  None

===========================================================================*/
boolean rr_ms_sub_capabilities_exclude_sys_mode(const sys_sys_mode_mask_e_type sys_mode_mask, const gas_id_t gas_id)
{
  uint32 subs_cap_mask;

  /* retrieve the mask of capability information from GCOMMON */
  subs_cap_mask = geran_retrieve_subscription_capabilities(gas_id);

  if (subs_cap_mask == 0UL)
  {
    MSG_GERAN_ERROR_0_G("Subscription caps have not been initialised");
  }

  return(((subs_cap_mask & (uint32)sys_mode_mask) == (uint32)SYS_SYS_MODE_MASK_NONE));
}

/*===========================================================================

FUNCTION     rr_ms_set_procedure_duration

DESCRIPTION  Sets the expected maximum duration of the current procedure
             so that this module can modify its own guard timers

DEPENDENCIES Must be called before the corresponding BCCH operation begins

RETURN VALUE VOID

SIDE EFFECTS None

===========================================================================*/
void rr_ms_set_procedure_duration(const rr_duration_t new_duration, const gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  rr_multisim_info.gas_data[as_index].procedure_duration = new_duration;
#else
  NOTUSED(new_duration);
  NOTUSED(gas_id);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return;
}

/*!
 * \brief Indicates if another stack is in traffic. This is based on the information in the last 
 *        MCS_CXM_COEX_TECH_STATE_BCAST_IND message received.
 * 
 * \param gas_id (in)
 * 
 * \return boolean - TRUE if another stack is in traffic, FALSE otherwise
 */
boolean rr_ms_other_stack_in_traffic(const gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  int ignore_tech_id;
  int i;

  ignore_tech_id = rr_ms_get_tech_info_index(geran_map_gas_id_to_cxm_id(gas_id));

  for (i = 0; i < ARR_SIZE(rr_ms_tech_info); i++)
  {
    if (i != ignore_tech_id)
    {
      if (rr_ms_tech_info[i].tx_active)
      {
        return(TRUE);
      }
    }
  }
#else
  NOTUSED(gas_id);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return(FALSE);
}

/*!
 * \brief Indicates whether Dual Receive DSDS is enabled
 *        MCS_CXM_COEX_TECH_STATE_BCAST_IND message received.
 * 
 * \param gas_id (in)
 * 
 * \return boolean - TRUE if DR-DSDS is enabled, FALSE otherwise
 */
boolean rr_ms_drdsds_enabled(const gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  return(rr_multisim_info.gas_data[as_index].drdsds_enabled);
#else
  NOTUSED(gas_id);
  return(FALSE);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
}

/*!
 * \brief Indicates whether Redirection is for MO/Emergency CS Call
 *
 * 
 * \param gas_id (in)
 * 
 * RETURN VALUE VOID
 *
 * SIDE EFFECTS None
 */
void rr_ms_redirected_for_cs_call(const gas_id_t gas_id)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  MSG_GERAN_HIGH_0_G("RR MS Redirected for MO/Emergency Call");
 
  rr_multisim_info.gas_data[as_index].proc_lock_cause = RR_MS_PROC_LOCK_CS_CALL;
#else
  NOTUSED(gas_id);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
  return;
}

/*!
 * \brief Indicates whether the sub is currently in slow IRAT mode. 
 *        To reduce impact to DDS sub throughput, following checks are made to inhibit higher priority based IRAT reselection.
 *        1. Current sub is non-DDS
 *        2. DSC is equal or above the EFS controlled threshold
 *        3. DDS sub is doing data transfer
 *        4. Slow IRAT Mode EFS Enabled only 
 * 
 * \param gas_id (in)
 * 
 * \return boolean - TRUE if slow IRAT mode checks are satisfied, FALSE otherwise
 */
boolean rr_ms_in_slow_irat_mode(const gas_id_t gas_id)
{
  boolean slow_irat_mode = FALSE;
  
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  int as_index = GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id);

  /* Slow IRAT mode = Current sub is non DDS + DSC is equal or above threshold + DDS sub is doing data + slow IRAT Mode EFS Enabled.
        In this mode, we inhibit "higher" priority based reselection. */
  if ((geran_get_current_dds_sub_id() != RR_GAS_ID_TO_AS_ID) &&
      (rr_gprs_get_dsc_percent(gas_id) >= geran_nv_get_dsc_threshold_for_slow_irat()) &&
      (geran_api_is_alt_sub_in_data(gas_id)) &&
      (TRUE == rr_nv_get_slow_irat_mode_is_enabled(gas_id)))
  {
    slow_irat_mode = TRUE;
  }

  return(slow_irat_mode);
#else
  NOTUSED(gas_id);
  return(slow_irat_mode);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
}

/*!
 * \brief this function used to reset no resource cnt when GSM stack deactivated time.
 *
 * \param gas_id (in)
 * 
 * \return none.
 *
 */
void rr_ms_reset_no_resource_cnt(const gas_id_t gas_id)
{
  rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].no_resource_count = 0;
}

#ifdef FEATURE_GSM_DECODE_ALL_FREQS
/*!
 * \brief this function is used to set scan_more_freq counter, when more than 140 scan was finished.
 *
 * \param gas_id (in)
 * 
 * \return none.
 *
 */
void rr_ms_set_scan_more_freq_allowed(const gas_id_t gas_id)
{
  rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].scan_more_freq = TRUE;
}

/*!
 * \brief this function is used to get scan_more_freq counter value.
 *
 * \param gas_id (in)
 * 
 * \return TRUE if Scan was completed more than 140 freq's else FALSE.
 *
 */
boolean rr_ms_get_scan_more_freq_allowed(const gas_id_t gas_id)
{
  return rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].scan_more_freq;
}
/*!
 * \brief this function is used to reset scan_more_freq counter, when GRR send srv cnf to NAS or GSM stack deactivated time.
 *
 * \param gas_id (in)
 * 
 * \return none.
 *
 */
void rr_ms_reset_scan_more_freq_allowed(const gas_id_t gas_id)
{
  rr_multisim_info.gas_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].scan_more_freq = FALSE;
}
#endif /* FEATURE_GSM_DECODE_ALL_FREQS */

/* EOF */
