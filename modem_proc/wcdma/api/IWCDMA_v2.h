/**
  @file IWCDMA_V2.h

  @brief  This is the header file that describes an interface to a component

*/
/*
  Copyright (c) 2013 Qualcomm Technologies, Inc.
          All Rights Reserved.
    Qualcomm Confidential and Proprietary
*/

/*===========================================================================
                      EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/wcdma.mpss/8.4.0/api/IWCDMA_v2.h#3 $
  $DateTime: 2020/01/24 03:56:36 $


  when        who     what, where, why
  ---------   ---     ------------------------------------------------------------
04/06/15      ks      Create file


===========================================================================*/
#ifndef _IWCDMA_V2_H_
#define _IWCDMA_V2_H_

#include "mcfg_seg_load.h"

#include "wl1x2wsrchapi.h"

/*=====================================================================================*/

/*  The component that is being used provides typedefs for pointers to each
    of its exported functions. 
 */

typedef void (*fn_wl1_x2w_rf_script_enter)
(
  wl1_irat_x2w_rf_enter_ind_struct_type *rf_enter_params
);

typedef void (*fn_wl1_x2w_rf_script_exit)
(
  wl1_irat_x2w_rf_exit_ind_struct_type *rf_exit_params
);
 
typedef void (*fn_wl1_l2w_config_deinit)
(
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_conn_full_window_srch_req)
(
  /*! Cells for which measurements need to be made. */
  wl1_l2w_freq_info_s  *freq_cell_list_ptr,
  int32 wfw_freq_err,
  lte_l1_stmr_w_s wstmr_rf_tune,
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_conn_step1_nastt_srch_req)
(
  /* Freq on which the step1 search has to be performed */
  uint16 freq,
  /* Indication the W driver that this is the first task and
  subsequent ones results will all be accumulated in the fw */
  boolean first_task,
  /* Indication that Step1 searching should end with this.
  Additionally W driver should program the FW to start performing
  the NASTT search once the Step1 results arrive */
  boolean last_task,
  int32 wfw_freq_err,
  lte_l1_stmr_w_s wstmr_rf_tune,
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_config_init)
(
  /*! FTM mode flag in L2W */
  boolean is_ftm_mode
, sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_register_callbacks)
(
  /*Pass in the function pointers which will be registered by the driver.
  These call backs would be called at appropriate times by the W driver*/
  wl1_l2w_nastt_srch_done_cb            nastt_done_ptr,
  wl1_l2w_pn_srch_done_cb               pn_srch_ptr,
  wl1_l2w_abort_srch_meas_cnf_cb        abort_cnf_ptr,
  wl1_l2w_cleanup_done_cnf_cb           cleanup_cnf_ptr,
  wl1_l2w_rf_script_build_done_cnf_cb   rf_script_build_done_ptr,
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_abort_srch_meas)
(
  sys_modem_as_id_e_type as_id
);

typedef lm_handle_type (*fn_wl1_l2w_driver_get_l2w_handle)
(
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_cell_id_srch_req)
(
  /*! lets us know if ue is SS, DSDS, or DSDA */
  wl1_l2w_multisim_state_e multisim_state,
  /*! Frequency on which the cell id search has to be performed */
  /*!Previously detected cells on that frequency if any. These previously
  detected cells help in WCDMA driver performing the Step1 peak elimination*/
  wl1_l2w_freq_info_s       *freq_info_ptr,
  int32 wfw_freq_err,
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_pn_srch_req)
(
  /*! lets us know if ue is SS, DSDS, or DSDA */
  wl1_l2w_multisim_state_e multisim_state,
  /*! TRUE indicates that RF has to be tuned. It will be in the case
  when only the measurements have to be done and there was no
  search prior to this.*/
  boolean                             tune_wcdma_rf,
  /*! Cells for which measurements need to be made. */
  wl1_l2w_freq_info_s        *freq_cell_list_ptr,
  int32 wfw_freq_err,
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_conn_pn_srch_req)
(
  /*! Cells for which measurements need to be made. */
  wl1_l2w_freq_info_s  *freq_cell_list_ptr,
  int32 wfw_freq_err,
  lte_l1_stmr_w_s wstmr_rf_tune,
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_initiate_conn_search)
(
  /*! lets us know if ue is SS, DSDS, or DSDA */
  wl1_l2w_multisim_state_e multisim_state
, sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_cleanup)
(
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_driver_send_panic_cmd)
(
  sys_modem_as_id_e_type as_id
);

typedef void (*fn_wl1_l2w_build_rf_scripts)
(
   rfm_meas_common_param_type* common_ptr,
   boolean build_script_in_offload_thread
, sys_modem_as_id_e_type as_id
);
/*==================================== end of edit ====================================*/

/*  The component then provides a vtable beginning with add_ref, release,
    and qi, followed by the functions it exports.
 */
typedef struct
{
  add_ref_fn  add_ref;
  release_fn  release;
  qi_fn       qi;

  fn_wl1_x2w_rf_script_enter wl1_x2w_rf_script_enter;
  fn_wl1_x2w_rf_script_exit wl1_x2w_rf_script_exit;
  fn_wl1_l2w_config_deinit wl1_l2w_config_deinit;
  fn_wl1_l2w_conn_full_window_srch_req wl1_l2w_conn_full_window_srch_req;
  fn_wl1_l2w_conn_step1_nastt_srch_req wl1_l2w_conn_step1_nastt_srch_req;
  fn_wl1_l2w_config_init wl1_l2w_config_init;
  fn_wl1_l2w_register_callbacks wl1_l2w_register_callbacks;
  fn_wl1_l2w_abort_srch_meas wl1_l2w_abort_srch_meas;
  fn_wl1_l2w_driver_get_l2w_handle wl1_l2w_driver_get_l2w_handle;
  fn_wl1_l2w_cell_id_srch_req wl1_l2w_cell_id_srch_req;
  fn_wl1_l2w_pn_srch_req wl1_l2w_pn_srch_req;
  fn_wl1_l2w_conn_pn_srch_req wl1_l2w_conn_pn_srch_req;
  fn_wl1_l2w_initiate_conn_search wl1_l2w_initiate_conn_search;
  fn_wl1_l2w_cleanup wl1_l2w_cleanup;
  fn_wl1_l2w_driver_send_panic_cmd wl1_l2w_driver_send_panic_cmd;
  fn_wl1_l2w_build_rf_scripts wl1_l2w_build_rf_scripts;
/*====================================== end of edit ====================================*/
} iWCDMA_v2_vtbl;

/*  The component provides a macro to get the pointer to its vtable from
    a pointer to an interface (defined in dynload.h).
 */
#define WCDMA_v2_vtbl(pi)   ((iWCDMA_v2_vtbl *) ((pi)->pVtable))

#define IWCDMA_wl1_x2w_rf_script_enter(pi,rf_enter_params) \
        (WCDMA_v2_vtbl(pi))->wl1_x2w_rf_script_enter(rf_enter_params)

#define IWCDMA_wl1_x2w_rf_script_exit(pi,rf_exit_params) \
        (WCDMA_v2_vtbl(pi))->wl1_x2w_rf_script_exit(rf_exit_params)

#define IWCDMA_wl1_l2w_config_deinit(pi,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_config_deinit(as_id)

#define IWCDMA_wl1_l2w_conn_full_window_srch_req(pi,freq_cell_list_ptr,wfw_freq_err,wstmr_rf_tune,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_conn_full_window_srch_req(freq_cell_list_ptr,wfw_freq_err,wstmr_rf_tune,as_id)

#define IWCDMA_wl1_l2w_conn_step1_nastt_srch_req(pi,freq,first_task,last_task,wfw_freq_err,wstmr_rf_tune,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_conn_step1_nastt_srch_req(freq,first_task,last_task,wfw_freq_err,wstmr_rf_tune,as_id)

#define IWCDMA_wl1_l2w_config_init(pi,ftm_mode,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_config_init(ftm_mode,as_id)

#define IWCDMA_wl1_l2w_register_callbacks(pi,nastt_done_ptr,pn_srch_ptr,abort_cnf_ptr,cleanup_cnf_ptr,rf_script_build_done_ptr,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_register_callbacks(nastt_done_ptr,pn_srch_ptr,abort_cnf_ptr,cleanup_cnf_ptr,rf_script_build_done_ptr,as_id)

#define IWCDMA_wl1_l2w_abort_srch_meas(pi,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_abort_srch_meas(as_id)

#define IWCDMA_wl1_l2w_driver_get_l2w_handle(pi,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_driver_get_l2w_handle(as_id)

#define IWCDMA_wl1_l2w_cell_id_srch_req(pi,multisim_state,freq_info_ptr,wfw_freq_er,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_cell_id_srch_req(multisim_state,freq_info_ptr,wfw_freq_er,as_id)

#define IWCDMA_wl1_l2w_pn_srch_req(pi,multisim_state,tune_wcdma_rf,freq_cell_list_ptr,wfw_freq_err,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_pn_srch_req(multisim_state,tune_wcdma_rf,freq_cell_list_ptr,wfw_freq_err,as_id)

#define IWCDMA_wl1_l2w_conn_pn_srch_req(pi,freq_cell_list_ptr,wfw_freq_err,wstmr_rf_tune,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_conn_pn_srch_req(freq_cell_list_ptr,wfw_freq_err,wstmr_rf_tune,as_id)

#define IWCDMA_wl1_l2w_initiate_conn_search(pi,multisim_state,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_initiate_conn_search(multisim_state,as_id)

#define IWCDMA_wl1_l2w_cleanup(pi,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_cleanup(as_id)

#define IWCDMA_wl1_l2w_driver_send_panic_cmd(pi,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_driver_send_panic_cmd(as_id)

#define IWCDMA_wl1_l2w_build_rf_scripts(pi,l2w_rfm_meas_setup_param,build_script_in_offload_thread,as_id) \
        (WCDMA_v2_vtbl(pi))->wl1_l2w_build_rf_scripts(l2w_rfm_meas_setup_param,build_script_in_offload_thread,as_id)
/*====================================== end of edit ====================================*/

#endif /* _IWCDMA_H_ */