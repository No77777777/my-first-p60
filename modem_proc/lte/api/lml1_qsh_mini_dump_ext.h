/*!
    @file
    lte_ml1_qsh_mini_dump_ext.h
    
    @brief
    QSH frame work mini dump

    @detail
*/

/*===========================================================================

    Copyright (c) 2015 Qualcomm Technologies Incorporated. All Rights Reserved

    Qualcomm Proprietary

    Export of this technology or software is regulated by the U.S. Government.
    Diversion contrary to U.S. law prohibited.

    All ideas, data and information contained in or disclosed by
    this document are confidential and proprietary information of
    Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
    By accepting this material the recipient agrees that this material
    and the information contained therein are held in confidence and in
    trust and will not be used, copied, reproduced in whole or in part,
    nor its contents revealed in any manner to others without the express
    written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                                                EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/lte.mpss/6.6.1/api/lml1_qsh_mini_dump_ext.h#1 $

when         who     what, where, why
--------   ---     ---------------------------------------------------------- 
10/15/15   cb      Created to remove external header dependencies on internal 
                   header files 
===========================================================================*/
#ifndef _LML1_QSH_MINI_DUMP_EXT_H_
#define _LML1_QSH_MINI_DUMP_EXT_H_

typedef enum
{
    LML1_QSH_SUCCESS,
    LML1_QSH_FAIL
} lte_ml1_qsh_dump_result_e;

typedef PACK(struct)
{
  int32       current_state;    
  int32       curr_input; 
} lte_ml1_qsh_dump_tag_mini_stm_state_machine_t;

/*******************************************************************************
 *
 * Gapmgr dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  boolean                              gap_cfg_enabled;
  uint8                                pattern_id;
  uint8                                gap_offset;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_status_s;

typedef PACK(struct)
{
  int8                              rat_id;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_rat_reg_info_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_rat_reg_info_s        rat_reg_info;
  int8                                abort_status;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_rat_data_s;


typedef PACK(struct)
{
  int32    last_rat_assigned;
  int32    cur_gap_owner;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_rat_data_s   rat_data[8];
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_rat_info_s;

typedef PACK(struct)
{
  boolean                     is_allocated;
  uint16                      gap_start_time;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_ota_gap_time_info_s;


typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_status_s             gapmgr_status;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_rat_info_s           rat_info;
  int32                                               curr_action;
  uint16                                              alloc_obj_act_time;
  uint16                                              suspend_mod_mask;
  uint16                                              resume_mod_mask;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_ota_gap_time_info_s  ota_gap_info;
  boolean                                             aborting_current_gap;
  boolean                                             is_resume_pending;
  boolean                                             awaiting_gap_completion;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_gapmgr_data_s;


/*******************************************************************************
 *
 * Schdlr dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  uint16    cell_id;
  uint32         freq;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_cell_info_s;

typedef PACK(struct)
{
  uint32 id:8;
  uint32 sfn_time:16;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_header_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_header_s header;
  uint32                            obj_id:16;
  uint32                            obj_group:4;
  uint32                            obj_prior:4;
  uint32                            prev_state:3;
  uint32                            new_state:3;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_obj_state_change_s;


typedef PACK(struct)
{
    lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_header_s      header;
    uint32 obj_id:16;
    uint32 start_time_type:8;
    uint32 start_time_sf:16;
    uint16 cell_info_cell_id:16;
    uint32 win_size:16;
    uint32 start_state:3;
    uint32 end_state:3;
    uint32 p_overriden:1;
    uint32 new_prior:4;
    uint32 asap_timeout:1;
    uint16 min_act_win:16;
    int16  asap_timeout_val:16;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_service_req_s;

typedef PACK(union)
{ 
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_obj_state_change_s  obj_state_change;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_service_req_s       event_service_req;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_u;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_event_u schdlr_events[20]; // last 20 entries
} lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_log_buffer_s;

typedef PACK(struct)
{
  int32 running_mode;
  int32 arb_mode;
  boolean sched_asap;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_mode_data_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_cell_info_s          scell;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_cell_info_s          ccell;
  uint16            sf_now;
  uint16            plus_delta_to_scell;
  uint16            scell_sf_now;
  uint64                              dl_subf_ref_time[3];
  uint32                              sclk_val;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_systime_info_s;



/*******************************************************************************
 *
 * ML/GM dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  uint16     sfn_subfn_cnt;
  uint32     umid;
  uint8      sclk_diff;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_logged_msg_s;

typedef PACK(struct)
{
  uint32 in_last_msg;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_logged_msg_s in_msgs[20];
  uint32 out_last_msg;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_logged_msg_s out_msgs[20];
} lte_ml1_qsh_dump_tag_mini_lte_ml1_msg_debug_log_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_msg_debug_log_s lte_ml1_mgr_msg_log;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_msg_debug_log_s lte_ml1_gm_msg_log;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_debug_log_s;

/*******************************************************************************
 *
 * Search conn dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  boolean enable;
} lte_ml1_qsh_dump_tag_mini_lte_l1_subfm_pattern_cfg_param_s;

typedef PACK(struct)
{
  boolean                              gap_cfg_enabled;
} lte_ml1_qsh_dump_tag_mini_lte_cphy_meas_gap_cfg_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_cphy_meas_gap_cfg_s     gap_cfg;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_meas_cfg_data_s;

typedef PACK(struct)
{
  boolean                   active;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_tl_mgr_stm_wb_meas_data_s;

typedef PACK(struct)
{
  uint32                       needed_cc_mask;
  uint32                       needed_cc_mask_ic_srch;
} lte_ml1_qsh_dump_tag_mini_cdrx_info_s;

typedef PACK(struct)
{
  int32   status;
} lte_ml1_qsh_dump_tag_mini_gapmgr_suspend_resume_info_s;


typedef PACK(struct)
{
  uint32                                             abort_bitmask;
  boolean                                            con_stm_abort_in_progress;
  lte_ml1_qsh_dump_tag_mini_lte_l1_subfm_pattern_cfg_param_s          meas_subfn_pattern_srv_cell;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_meas_cfg_data_s           meas_cfg_data;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_tl_mgr_stm_wb_meas_data_s wb_meas_data;
  lte_ml1_qsh_dump_tag_mini_cdrx_info_s                               cdrx;
  lte_ml1_qsh_dump_tag_mini_gapmgr_suspend_resume_info_s              gapmgr_suspend_resume_info;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_tl_mgr_stm_s;

typedef PACK(struct)
{
  boolean                 trigger;
  boolean                 active;
} lte_ml1_qsh_dump_tag_mini_nb_data_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_nb_data_s data_per_nb[2];
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_pbch_mgr_stm_s;

typedef PACK(struct)
{
  uint32     num_objs;
  int32      meas_state;
  boolean    relinquish_next_gap;
  uint32     num_opportunities_passed;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_meas_inter_freq_meas_s;


typedef PACK(struct)
{
  int32       prev_state;
  boolean     abort_ongoing;
  uint32      ongoing_activites; 
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_conn_inter_freq_stm_info_s;

/*******************************************************************************
 *
 * IRAT dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  struct
  {
    int32 onex_conn_state;
    int32 hrpd_conn_state;
  } cdma;
  struct
  {
    int32 state;
  }tds;
  struct
  {
    int32 state;
  }wcdma;
  struct
  {
    int32 state;
  } sched;
  struct
  {
    int32 state;
  } idle_irat;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_irat_l2x_s;

typedef PACK(struct)
{
  struct
  {
     int32 state;
  } stm;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_irat_x2l_s;


typedef PACK(struct)
{
  boolean x2l;
  union
  {
    lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_irat_x2l_s x2l;
    lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_irat_l2x_s l2x;
  } irat_sm;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_irat_meas_s;

/*******************************************************************************
 *
 * MCLK dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  int32    scenario_idx;
  uint8    obj_bitmask;                  
  int32    target_freq;
  uint16   start_sf;
  uint16   end_sf;
  boolean  pre_scale_needed;
  boolean  pre_scale_done;
  boolean  mcvs_called;
  boolean  mcvs_done;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_mcvs_trigger_s;

typedef PACK(struct)
{
  int32          mcvs_state;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mcvs_trigger_s active_trigger;
  boolean        resched_pended;
  boolean        always_on; 
  int32          base_vpe_setting;
  int32          muxed_vpe_setting;
  int32          vpe_voting[4];
  int32          muxed_q6_setting;
  int32          q6_voting[7];
  boolean        mcvs_enabled;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_mcvs_ctrl_s;

typedef PACK(struct)
{
  uint32                             cur_state;
  uint32                             pcc_bw;
  uint8                              num_tx_ant;  
  uint8                              pcc_num_crs_ic;  
  uint32                             highest_tx_mode;
  uint32                             pcc_tx_mode;
  uint32                             scc_bw[2];  
  boolean                            scc_activated[2];
  uint8                              scc_num_tx_ant[2];
  uint32                             scc_tx_mode[2];
  uint8                              scc_num_crs_ic[2]; 
  boolean                            ca_activated;
  boolean                            ca_configured;
  uint32                             cur_mcpm_bw;
  uint8                              max_data_tb;  
} lte_ml1_qsh_dump_tag_mini_lte_ml1_mcpm_s;

/*******************************************************************************
 *
 * RFMGR dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  int16               lte_trm_client; 
  int16               trm_action; 
  uint32              timetick; 
//  char                file[16]; 
} lte_ml1_qsh_dump_tag_mini_lte_ml1_rfmgr_trm_log_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_rfmgr_trm_log_s  buffer[3];
} lte_ml1_qsh_dump_tag_mini_lte_ml1_rfmgr_trm_dbg_buffer_s;

typedef PACK(struct)
{
  boolean chain_granted[6];
  boolean chain_waiting[6];
  uint32  band_tune_info[5];
  int32   ml1_state;
  int32   rrc_state;
  boolean acquiring_carrier[5];
  int16  cc_state[5];   
  int32  ml1_state_before_sleep;
  uint32                        unlock_by_mask;
  uint32                        unlock_imm_mask;
  uint32                        unlock_cancel_mask;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_rfmgr_trm_dbg_buffer_s  trm_dbg_buffer[6];
} lte_ml1_qsh_dump_tag_mini_lte_ml1_rfmgr_trm_s;

/*******************************************************************************
 *
 * Sleepmgr dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  uint16                        num_samples_req;
  uint16                        num_samples;
  uint16                        worst_rf_warmup_usec;
  uint32                        rf_warmup_start;
  uint64                        rf_warmup_start_ustmr;
  uint16                        mcpm_ext_overhead_usec;
  uint16                        curr_rf_warmup_usec;
  uint16                        worst_rf_only_warmup_usec;
  uint32                        rf_only_warmup_start;
  uint64                        rf_only_warmup_start_ustmr;
  uint16                        last_sched_wakeup_rf_warmup_usec;
} lte_ml1_qsh_dump_tag_mini_sleepmgr_dyn_rf_warmup_t;

typedef PACK(struct)
{
  boolean                                     sleep_enabled; 
  uint16                                      schdlr_gap; 
  uint16                                      sleep_duration; 
  uint16                                      obj_start_time;  
  uint16                                      wakeup_obj_time; 
  boolean                                     ll1_sleep_cnf_rcvd; 
  boolean                                     ll1_wakeup_cnf_rcvd; 
  boolean                                     rfmgr_wakeup_cnf_rcvd; 
  boolean                                     rfmgr_tx_tune_ind_rcvd; 
  boolean                                     rfmgr_enter_cnf_rcvd;  
  int32                                       wakeup_type; 
  uint16                                      rf_warmup_usec; 
  uint16                                      dls_rf_warmup_usec; 
  uint16                                      long_dls_rf_warmup_usec;
  lte_ml1_qsh_dump_tag_mini_sleepmgr_dyn_rf_warmup_t           dyn_rf_warmup; 
  lte_ml1_qsh_dump_tag_mini_sleepmgr_dyn_rf_warmup_t           dls_dyn_rf_warmup; 
  boolean                                     is_curr_wakeup_rude_wakeup; 
  boolean                                     offline_goto_sleep_rcvd; 
  boolean                                     light_sleep_obj_end_pending; 
  uint16                                      light_sleep_end_time; 
  uint16                                      dis_deep_sleep_mod_mask; 
  uint16                                      dis_light_sleep_mod_mask; 
  uint16                                      dls_exit_time; 
  boolean                                     dls_exit_pending;
  int32                                       wakeup_obj_id; 
  boolean                                     is_tao_scheduled; 
  boolean                                     is_wakeup_timeline_chest_reset;
  boolean                                     rf_less_wakeup; 
  boolean                                     is_wakeup_timeline_missed; 
  boolean                                     rude_wakeup_pull_in; 
  boolean                                     rxlm_cnf_rcvd;
  uint64                                      wakeup_cb_recv_delay;
  uint64                                      rfmgr_wakeup_delay;  
  boolean                                     is_rf_exit;  
  boolean                                     wakeup_on_pcell_only;
  int32                                       ttl_correction;  
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sleepmgr_t; 

/*******************************************************************************
 *
 * Search idle dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  int32            band;
  uint32           freq;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_rat_lte_per_layer_params_s;

typedef PACK(struct) 
{
  int32                       type;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_mdb_rat_data_s;

typedef PACK(struct) 
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mdb_rat_data_s      rat_ptr;
  boolean                         for_son_only;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_rat_lte_per_layer_params_s   lte;
  uint8                           num_cells;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_layer_s;

typedef PACK(struct)
{
  uint32       acq_stage1_timeout;
  int8         rrc_timeout_type;
  int32        scheduled_object;
  int32        pending_cnf;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_acq_s;

typedef PACK(struct)
{
  uint16 phy_cell_id;
  uint32 dl_cell_frequency;
} lte_ml1_qsh_dump_tag_mini_lte_cphy_cell_info_s;

typedef PACK(struct)
{
  uint16               pci;
  boolean              csg_cell;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_rat_lte_cell_info_s;


typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_rat_lte_cell_info_s     cell_info;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_rat_lte_cell_struct_s;


typedef PACK(union)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_rat_lte_cell_struct_s   lte;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_cell_struct_u;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_cell_struct_u        cell_data;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_layer_s              layer_ptr;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_mdb_cell_struct_s;


typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_cphy_cell_info_s  lte_cell_info;
} lte_ml1_qsh_dump_tag_mini_lte_cphy_rat_specific_cell_info_u;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_cphy_rat_specific_cell_info_u  rat_cell_info;
} lte_ml1_qsh_dump_tag_mini_lte_cphy_resel_target_cell_info_s;

typedef PACK(struct)
{
  int32                                       resel_status;
  lte_ml1_qsh_dump_tag_mini_lte_cphy_resel_target_cell_info_s  resel_target;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_idle_resel_info_s;

typedef PACK(struct)
{
  int32                                                             req_type;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_idle_resel_info_s            resel_info;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_layer_s                      active_layer_ptr;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_idle_db_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_idle_db_s idle_db;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_idle_s;


typedef PACK(struct)
{
  uint16    lte_active_srch_meas;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_idle_lte_search_meas_pbch_info_s;

typedef PACK(struct)
{
  boolean pbch_complete;
  uint32  earfcn;
  uint16  cell_id;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_sm_acq_pbch_info_s;

/*******************************************************************************
 *
 * DLM dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  uint8                        cc_id;
  uint32                      mode;
  uint16                      cell_id;
  uint32                      dl_earfcn;
  int32                       dl_bw;
  uint32                      cur_dl_earfcn; 
  int32                       cur_dl_bw; 
  uint16                      start_rec_offset_Ts;      
  uint16                      samp_rec_time_ms;     
  uint16                      samp_rec_time_frac_Ts;  
  uint32                      min_duration_in_ms;
  uint32                      timeout_in_ms;         
}lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_rx_cfg_params_per_carr_s;

typedef PACK(struct){
  boolean                        is_explicit_req; 
  lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_rx_cfg_params_per_carr_s req_param;
  uint8                          rx_tune_type_mask;
  uint16                         ll_msg_disable_mask;
  uint16                         ll_msg_enable_mask;
  uint16                         ll_msg_resume_mask;
  int32                          ll_msg_rxagc_mode; 
  uint8                          ll_msg_rxagc_start_lna_state[2]; 
  int32                          ll_msg_rxagc_start_dvga_dB[2];
  boolean                        pending_trm_acq;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_rx_cfg_proc_cfg_per_carr_s;

typedef PACK(struct)
{
  boolean                            in_progress;  
  lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_rx_cfg_proc_cfg_per_carr_s cfg_app_params[1]; // PCC
  int32                          ll_msg_proc_mode;              
  int32                          ll_msg_resource_mode;  
  int32                          ll_msg_sys_mode;
  uint16                         pending_actions; 
  boolean                        pending_rx_dis_cnf;  
  boolean                        pending_rx_sys_time_en_cnf; 
  boolean                        pending_rx_rf_cnf; 
  boolean                        pending_rx_en_cnf; 
  boolean                        pending_rx_en_dis_cnf;
  boolean                        pending_rx_sys_time_dis_cnf;
  boolean                        pending_pmch_suspend_cnf;
  boolean                        pending_rsc_mode_chng_cnf;
  boolean                        pending_rf_script_exec_cnf;
  boolean                        pending_scripts_based_tune_cnf;
  boolean                        pending_rx_b2b_cnf;
  boolean                        pending_tune_impact_check_cnf;
  boolean                        pending_ulm_blank_tx_cnf;
  uint8                          pending_cfg_split; 
  uint8                          pending_rx_seq_id;    
  uint32                         trm_acq_pend_mask;      
  int32                          ret_status;  
  boolean                        apply_action_time;
  uint16                         action_subfn_cnt;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_rx_cfg_proc_status_s;

typedef PACK(struct) {
  int16               cur_state;
  int16               next_state;
  uint8               ml_id;
  uint8               ll_id;
  uint32              proc_cnf_wait_mask;
  uint32              scc_cleanup_cfg; 
} lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_per_scc_data_s;


typedef PACK(struct) {
  uint32              scc_del_pend_mask;
  uint32              scc_add_pend_mask;
  uint32              scc_recfg_pend_mask;
  uint32              scc_recfg_done_mask;   
  uint32              scc_trm_pend_mask;
  uint32              scc_act_pend_mask;
  uint32              scc_deact_pend_mask;
  uint32              scc_add_done_mask; 
  uint32              scc_activate_done_mask; 
  uint32              scc_rf_tune_pend_mask; 
  uint32              scc_rf_exit_pend_mask; 
  uint32              scc_vrlf_deact_done_mask;
  uint32              scc_vrlf_cell_deact_trigger_mask;
  uint32              scc_sus_res_pend_mask;
  uint32              scc_handover_trig_deconfig_mask;
  uint32              scc_handover_trig_trm_rel_mask;
  uint32              scc_rf_not_tuned_mask;
  uint32              scc_init_search_done_mask;    
  uint8                           num_scc_cfg;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_per_scc_data_s      scc_db_list[4];     
} lte_ml1_qsh_dump_tag_mini_lte_ml1_dlm_ca_data_s;

/*******************************************************************************
 *
 * Manager dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  uint32 free_map[12];
  uint32 usage_cnt;  
} lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_pq_mem_usage_state_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_pq_mem_usage_state_s mem_state;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_pq_mem_s;

typedef PACK(struct)  
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_pq_mem_s                  lte_ml1_mgr_pq_mem;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_pq_data_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_pq_data_s                 pq_data;
} lte_ml1_qsh_dump_tag_mini_lte_mgr_static_data_s;

typedef PACK(struct)
{
  uint32              dlm_dedicated_cfg_req_mask;
  uint16                 dlm_common_cfg_req_mask;
  uint32              ulm_dedicated_cfg_req_mask;
  uint16                 ulm_common_cfg_req_mask;
  uint32              gm_dedicated_cfg_req_mask;
  uint16                 gm_common_cfg_req_mask;
  uint16        dlm_common_mobility_cfg_req_mask;  
}lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_modules_cfg_s;

typedef PACK(struct)
{
  int32              sub_proc;
  int32              ho_cfg_error_status;
  int32              last_sub_proc;
  boolean            tx_tune_cnf_pending;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_ho_proc_data_s;

typedef PACK(struct)
{
  uint16                     ml1_mod_block_mask; 
  uint8                                  dlm_mod_block_mask; 
  uint32                                 upr_lyr_req_rcvd_umid; 
  uint32                                 int_req_trigger_umid; 
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_modules_cfg_s     modules_cfg; 
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_ho_proc_data_s    ho_proc_data; 
  uint16                    modules_cnf_wait_mask; 
}lte_ml1_qsh_dump_tag_mini_l1m_stm_data_s;

typedef PACK(struct)
{
  boolean   is_valid;
  int32     state;
  int32     ta_type;
  uint64    tao_start_time_rtc;
} lte_ml1_qsh_dump_tag_mini_tam_taw_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_tam_taw_s         taws[3]; 
} lte_ml1_qsh_dump_tag_mini_tam_ta_info_s;

typedef PACK(struct)
{
  int32          slte_state;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_tam_suspend_resume_data_s;

typedef PACK(struct)
{
  int32                 status;
  int32                 state;
  boolean               unlock_requested;
  boolean               grant_received;
} lte_ml1_qsh_dump_tag_mini_tam_chain_buffer_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_tam_chain_buffer_s      chains[4];
} lte_ml1_qsh_dump_tag_mini_tam_buffer_s;

typedef PACK(struct) 
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_tam_suspend_resume_data_s     suspend_resume_data;
  int32                                                 tam_state;
  lte_ml1_qsh_dump_tag_mini_tam_ta_info_s                         ta_info;
  lte_ml1_qsh_dump_tag_mini_tam_buffer_s                          buffer;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_tam_data_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_tam_data_s tam_data;  
}lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_tam_db_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_mgr_static_data_s         mgr_static_data_ptr;
  lte_ml1_qsh_dump_tag_mini_l1m_stm_data_s                stm_ptr;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_mgr_tam_db_s          tam_db_ptr;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_manager_data_s;

/*******************************************************************************
 *
 * ULM dependencies
 *
 ******************************************************************************/
typedef PACK(struct)
{
  int32                                  curr_req; 
  uint32                                 cfg_bitmask; 
  uint32                                 cfg_scc_status_update;
  uint8                                  pending_upr_layer_req; 
  uint32                                 cfg_scc_bitmask[7]; 
  uint32                                 scc_decfg_bitmask[7]; 
} lte_ml1_qsh_dump_tag_mini_lte_ml1_ulm_cfg_pending_cnf_s;

typedef PACK(struct)
{
  boolean                              is_rach_env_active;
  uint16                               suspend_mask;
  uint16                   cnf_pend_mask;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_prach_sched_info_s;

typedef PACK(struct)
{  
  int16 curr_rf_tune_reason;
  boolean is_in_conn_mode; 
  lte_ml1_qsh_dump_tag_mini_lte_ml1_ulm_cfg_pending_cnf_s cfg_pending_cnf; 
  lte_ml1_qsh_dump_tag_mini_lte_ml1_prach_sched_info_s    rach_sched_info;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_ulm_s;

/*******************************************************************************
 *
 * GM dependencies
 *
 ******************************************************************************/
typedef PACK(struct) 
{
  int32 subframe_assign;
  int32 special_sub_fn_patterns;
} lte_ml1_qsh_dump_tag_mini_lte_cphy_tdd_cfg_param_s;

typedef PACK(struct)
{
  boolean                                                  tdd_cfg_rcvd;
  lte_ml1_qsh_dump_tag_mini_lte_cphy_tdd_cfg_param_s                 tdd_cfg;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_config_info_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_config_info_s   config;
  boolean                                    sfn_unknown; 
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_info_s;

typedef PACK(struct)
{
  uint16 sfn;
  uint8  subfn;
} lte_ml1_qsh_dump_tag_mini_lte_l1_cell_systime_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_l1_cell_systime_s  last_pdcch_timing;
  uint8                                  pdcch_phich_info_record_cnt;
  uint8                                  virtual_sfn;   
  lte_ml1_qsh_dump_tag_mini_lte_ml1_schdlr_systime_info_s          curr_sys_time;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_gh_data_s;

typedef PACK(struct)
{
    uint16 subframe_num:4;                  
    uint16 frame_num:12;                    
} lte_ml1_qsh_dump_tag_mini_lte_LL1_sys_time_struct;

typedef PACK(struct)
{
    lte_ml1_qsh_dump_tag_mini_lte_LL1_sys_time_struct   action_time;
    int16                                     chan_type;
    boolean                                   srs_present;
    boolean                                   ack_nak_present_flag;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_hp_saved_phychan_sched_req_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_hp_saved_phychan_sched_req_s phychan_sched_buf[10];
  uint8 buf_index;
} lte_ml1_qsh_dump_tag_mini_lte_ml1_hp_saved_phychan_sched_info_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_hp_saved_phychan_sched_info_s       saved_phychan_sched_info;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_hp_data_s;


typedef PACK(struct)
{
  boolean                enable;
  uint16                 cycle_len;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_short_cycle_info_s;

typedef PACK(struct)
{
  uint16                  cycle_len;
  uint16                  offset;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_long_cycle_info_s;

typedef PACK(struct)
{
  uint16                value;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_on_duration_timer_info_s;

typedef PACK(struct)
{
  int16                 state;
  uint16                value;
  uint16                start_time;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_inactivity_timer_info_s;

typedef PACK(struct)
{
  boolean                                   cdrx_cfg_received;
  int16                                     curr_cycle_state;
  int16                                     curr_cycle_type;
  int16                                     next_cycle_type;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_inactivity_timer_info_s   inact_timer_info;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_on_duration_timer_info_s  on_duration_timer_info;  
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_short_cycle_info_s        short_cycle_info;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_long_cycle_info_s         long_cycle_info;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_info_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_info_s            cdrx_info;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_data_s;

typedef PACK(struct)
{
  boolean valid;
  boolean enable;
  int32 sps_interval;
}lte_ml1_qsh_dump_tag_mini_lte_l1_sps_cfg_ul_s;

typedef PACK(struct)
{
  boolean valid;
  boolean enable;
  int32 sps_interval;
}lte_ml1_qsh_dump_tag_mini_lte_l1_sps_cfg_dl_s;


typedef PACK(struct)
{
  uint16 sps_crnti;
  lte_ml1_qsh_dump_tag_mini_lte_l1_sps_cfg_dl_s sps_cfg_dl;
  lte_ml1_qsh_dump_tag_mini_lte_l1_sps_cfg_ul_s sps_cfg_ul;
}lte_ml1_qsh_dump_tag_mini_lte_cphy_sps_cfg_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_cphy_sps_cfg_s  sps_cfg;
  boolean                         ul_sps_activated;
  boolean                         dl_sps_activated;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_sps_data_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_sps_data_s             sps_db;
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_sps_global_data_s;

typedef PACK(struct)
{
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_info_s         info_ptr;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_cdrx_data_s    cdrx_data_ptr;  
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_gh_data_s      gh_data_ptr;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_hp_data_s      hp_data_ptr;
  lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_sps_global_data_s     sps_global_data_ptr;  
}lte_ml1_qsh_dump_tag_mini_lte_ml1_gm_data_s;

#endif /* _LML1_QSH_MINI_DUMP_EXT_H_ */
