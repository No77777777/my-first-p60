/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            GRR MULTI SIM HEADER

GENERAL DESCRIPTION
  Provides Dual SIM support for GERAN Radio Resources (GRR)

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2010-2015 Qualcomm Technologies, Inc.

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_multi_sim.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when        who     what, where, why
--------    ---     ----------------------------------------------------------
31/05/13    SJW     Created from legacy rr_dual_sim.h
dd/mm/yy    SJW     Sample text for edit history
===========================================================================*/
#ifndef __RR_MULTI_SIM_H__
#define __RR_MULTI_SIM_H__

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "geran_dual_sim.h"
#include "gs.h"
#include "rr_defs.h"
#include "rr_control.h"

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
#include "rr_gprs_defs.h"
#include "rr_task.h"
#include "cause.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "cb_rr.h"
#include "rr_main.h"
#include "mm_rr.h"
#include "rr_plmn_list.h"


#define RR_MS_INVALID_L1_MESSAGE ((rr_l1_message_id_enum_T)MPH_INVALID_MESSAGE_ID)

/* other DSDS specific definitions */
#define RR_MS_INTERFACE_TIMEOUT         4000
#define RR_MS_HEARTBEAT_SECONDS            1
#define RR_MS_HIBERNATE_SECONDS            4
#define RR_MS_HEARTBEAT_LIMIT          16384
#define RR_MS_INVALID_PAGING_GROUP    0xFFFF
#define RR_MS_INVALID_TIME_STAMP           0
#define RR_MS_SERVICE_REQUEST_TIMEOUT     10 /* 10 seconds */

typedef enum
{
  RR_PS_ABORT_NOT_ACTIVE = 0,     /* no PS abort procedure is in progress */
  RR_PS_ABORT_ACTIVE_WILL_RESUME, /* PS abort is in progress and will resume PS when it completes */
  RR_PS_ABORT_ACTIVE_TBF_BLOCKED  /* PS abort is in progress but only affects TBF establishments  */
} rr_ps_abort_block_t;

typedef enum
{
  RR_SERVICE_RECOVERY_AUTO_CELL_SELECTION,
  RR_SERVICE_RECOVERY_DOWNLINK_SIGNALLING,
  RR_SERVICE_RECOVERY_TRANSCEIVER_DENIALS,
  RR_SERVICE_RECOVERY_CELL_REESTABLISHING,
  RR_SERVICE_RECOVERY_NONE = 0xFF
} rr_ms_svc_recovery_t;

typedef enum
{
  RR_SYS_INFO_ACQ_STARTED,
  RR_SYS_INFO_ACQ_OPTIONAL_SI,
  RR_SYS_INFO_ACQ_STOPPED
} rr_ms_sys_info_acq_status_t;

extern void                    rr_ms_cs_connected(const boolean, const gas_id_t);
extern boolean                 rr_ms_other_stack_in_dedicated(const gas_id_t);
extern boolean                 rr_ms_rps_required_before_connection(const gas_id_t);
extern void                    rr_ms_complete_nas_service_ind(rr_service_ind_T *, const gas_id_t);
extern void                    rr_ms_complete_nas_service_cnf(rr_plmn_select_cnf_T *, const gas_id_t);
extern void                    rr_ms_service_recovery_started(const rr_ms_svc_recovery_t, const gas_id_t);
extern void                    rr_ms_cs_establishment_confirmed(const gas_id_t);
extern rr_process_msg_action_e rr_ms_process_task_message(const rr_cmd_bdy_type *, gas_id_t *);
extern void                    rr_ms_task_start_init(void);
extern void                    rr_ms_store_paging_info(layer1_sys_info_T *, word, const gas_id_t);
extern void                    rr_ms_plmn_list_search_starting(rr_PLMN_list_req_info_T *, const rr_plc_search_type_e , const boolean, const gas_id_t);
extern void                    rr_ms_plmn_list_search_complete(const gas_id_t);
extern void                    rr_ms_clear_missed_page(const gas_id_t);
extern void                    rr_ms_l1_blocking_message_sent(const rr_l1_message_id_enum_T, const gas_id_t);
extern void                    rr_ms_l1_shutdown_msg_expected(const rr_l1_message_id_enum_T, const gas_id_t);
extern void                    rr_ms_access_stratum_deactivated(const sys_modem_as_id_e_type, const boolean);
extern boolean                 rr_ms_gprs_active_on_other_stack(const gas_id_t);
extern void                    rr_ms_power_scan_abort_required(const rr_event_T, const gas_id_t);
extern void                    rr_ms_process_ps_abort_req(const gas_id_t);
extern void                    rr_ms_process_l1_ds_abort_ind(mph_l1_ds_abort_ind_T *);
extern rr_ps_abort_block_t     rr_ms_ps_abort_process_active(const gas_id_t);
extern void                    rr_ms_acquisition_started(const gas_id_t);
extern void                    rr_ms_ps_abort_timer_expiry(const rr_timer_id_e, const gas_id_t);
extern boolean                 rr_ms_any_other_active_gas_id(const gas_id_t);
extern boolean                 rr_ms_other_stack_has_same_lai(const gas_id_t);
extern boolean                 rr_ms_two_stacks_have_same_lai(const gas_id_t, const gas_id_t);
extern void                    rr_ms_cs_conn_failed(const gas_id_t);
extern void                    rr_ms_set_connection_priority(const sys_proc_type_e_type, const boolean, const gas_id_t); 
extern void                    rr_ms_store_tech_state(cxm_coex_tech_state_bcast_ind_s *);
extern void                    rr_ms_update_acquisition_priority(const geran_priority_t, const boolean, const gas_id_t);
extern boolean                 rr_ms_plmn_search_update_priority_rqrd(const boolean, const boolean, const gas_id_t);
extern void                    rr_ms_plmn_search_init_drx_count(const gas_id_t);
extern void                    rr_ms_plmn_search_invalidate_drx_count(const gas_id_t);
#ifdef PERLUTF
#error code not present
#endif /* PERLUTF */
extern void                    rr_ms_sys_info_status_update(const rr_ms_sys_info_acq_status_t, const gas_id_t);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/* these functions are always defined, even if the feature is not enabled */
extern boolean                 rr_multi_sim_mode_enabled(void);
extern boolean                 rr_multi_sim_api_valid(const gas_id_t);
extern void                    rr_ms_x2g_procedure_failure(const rr_proc_failure_type_t, const gas_id_t);
extern boolean                 rr_ms_all_stacks_in_state(const rr_state_T rr_state);
extern boolean                 rr_ms_sys_info_refresh_blocked(const gas_id_t);
extern boolean                 rr_ms_sub_capabilities_exclude_sys_mode(const sys_sys_mode_mask_e_type, const gas_id_t);
extern void                    rr_ms_set_procedure_duration(const rr_duration_t, const gas_id_t);
extern void                    rr_ms_check_and_start_sys_info_guard_timer(const boolean, const gas_id_t);
extern boolean                 rr_ms_other_stack_in_traffic(const gas_id_t);
extern boolean                 rr_ms_drdsds_enabled(const gas_id_t);
extern void                    rr_ms_redirected_for_cs_call(const gas_id_t);
extern boolean                 rr_ms_in_slow_irat_mode(const gas_id_t);
extern void                    rr_ms_reset_no_resource_cnt(const gas_id_t);
#ifdef FEATURE_GSM_DECODE_ALL_FREQS
extern void                    rr_ms_set_scan_more_freq_allowed(const gas_id_t gas_id);
extern boolean                 rr_ms_get_scan_more_freq_allowed(const gas_id_t gas_id);
extern void                    rr_ms_reset_scan_more_freq_allowed(const gas_id_t gas_id);
#endif /* FEATURE_GSM_DECODE_ALL_FREQS */

#endif /* __RR_MULTI_SIM_H__ */

/* EOF */

