/*===========================================================================

                            U I M G E N  H A L

DESCRIPTION
  This file contains functions for UIMGEN HAL interface. UIMGEN HAL maintains
  function pointer tables for generic state machines. It keeps generic
  command process and response process neat. Also, it constructs a framework
  that can be extended if different state machine bahevior is needed.

Copyright (c) 2016, 2019 - 2020 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimgen_hal.c#4 $
$DateTime: 2020/02/23 22:25:47 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/20   rkg     Add new TLV in Suscription manager
01/31/20   ykr     Disabling slot as per SIMLOCK configuration
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
09/22/16   gm      Added support for set APDU behavior
11/26/15   sam     Add logic to update EF-TST in recovery
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "uimgen_hal.h"
#include "uimgen_hal_iso.h"

/*============================================================================= 
  GLOBALS
=============================================================================*/
/* Generic state machine function pointer tables */
uim_gen_cmd_func_t  uim_gen_cmd_table[UIM_MAX_ST];
uim_gen_rsp_func_t  uim_gen_rsp_table[UIM_MAX_ST];

/*===========================================================================
 FUNCTION UIMGEN_HAL_INIT_CMD_FUNC_PTR_TABLE
===========================================================================*/
void uimgen_hal_init_cmd_func_ptr_table(void)
{
  uim_gen_cmd_table[UIM_POWER_UP_ST]        = uimgen_iso_handle_power_up_st;
  uim_gen_cmd_table[UIM_RESET_ST]           = uimgen_iso_handle_reset_st;
  uim_gen_cmd_table[UIM_DELAY_AFTER_ATR_ST] = uimgen_iso_handle_delay_after_atr_st;
  uim_gen_cmd_table[UIM_PPS_ST]             = uimgen_iso_handle_pps_st;
  uim_gen_cmd_table[UIM_SEND_STATUS_ST]     = uimgen_iso_handle_send_status_st;
  uim_gen_cmd_table[UIM_UPDATE_OP_PARAMS_ST]
                                            = uimgen_iso_handle_update_op_params_st;
#if defined( FEATURE_UIM_T_1_SUPPORT )
  uim_gen_cmd_table[UIM_IFS_NEG_ST]         = uimgen_iso_handle_ifs_neg_st;
#endif /* FEATURE_UIM_T_1_SUPPORT */
  uim_gen_cmd_table[UIM_CHECK_CHARACTERISTICS_ST]
                                            = uimgen_iso_handle_check_characteristics_st;
  uim_gen_cmd_table[UIM_TERMINAL_CAPABILITY_ST]
                                            = uimgen_iso_handle_terminal_capability_st;
  uim_gen_cmd_table[UIM_CHECK_FOR_CDMA_DF]  = uimgen_iso_handle_check_for_cdma_df_st;
  uim_gen_cmd_table[UIM_READ_ICCID_ST]      = uimgen_iso_handle_read_iccid_st;
  uim_gen_cmd_table[UIM_SELECT_ST]          = uimgen_iso_handle_select_st;
  uim_gen_cmd_table[UIM_RESELECT_CUR_DIR_ST]
                                            = uimgen_iso_handle_reselect_curr_dir_st;
  uim_gen_cmd_table[UIM_SELECT_FAILED_ST]   = uimgen_iso_handle_select_failed_st;
  uim_gen_cmd_table[UIM_STREAM_SELECT_ST]   = uimgen_iso_handle_stream_select_st;
  uim_gen_cmd_table[UIM_STATUS_ST]          = uimgen_iso_handle_status_st;
  uim_gen_cmd_table[UIM_READ_BIN_MEM_ST]    = uimgen_iso_handle_read_bin_mem_st;
  uim_gen_cmd_table[UIM_UPDATE_BIN_MEM_ST]  = uimgen_iso_handle_update_bin_mem_st;
  uim_gen_cmd_table[UIM_READ_REC_MEM_ST]    = uimgen_iso_handle_read_rec_mem_st;
  uim_gen_cmd_table[UIM_UPDATE_REC_MEM_ST]  = uimgen_iso_handle_update_rec_mem_st;
  uim_gen_cmd_table[UIM_INC_MEM_ST]         = uimgen_iso_handle_inc_mem_st;
  uim_gen_cmd_table[UIM_SEEK_ST]            = uimgen_iso_handle_seek_st;
  uim_gen_cmd_table[UIM_VERIFY_CHV1_ST]     = uimgen_iso_handle_verify_chv1_st;
  uim_gen_cmd_table[UIM_VERIFY_CHV2_ST]     = uimgen_iso_handle_verify_chv2_st;
  uim_gen_cmd_table[UIM_UNBLOCK_CHV1_ST]    = uimgen_iso_handle_unblock_chv1_st;
  uim_gen_cmd_table[UIM_UNBLOCK_CHV2_ST]    = uimgen_iso_handle_unblock_chv2_st;
  uim_gen_cmd_table[UIM_CHANGE_CHV1_ST]     = uimgen_iso_handle_change_chv1_st;
  uim_gen_cmd_table[UIM_CHANGE_CHV2_ST]     = uimgen_iso_handle_change_chv2_st;
  uim_gen_cmd_table[UIM_ENABLE_CHV1_ST]     = uimgen_iso_handle_enable_chv1_st;
  uim_gen_cmd_table[UIM_ENABLE_CHV2_ST]     = uimgen_iso_handle_enable_chv2_st;
  uim_gen_cmd_table[UIM_DISABLE_CHV1_ST]    = uimgen_iso_handle_disable_chv1_st;
  uim_gen_cmd_table[UIM_DISABLE_CHV2_ST]    = uimgen_iso_handle_disable_chv2_st;
  uim_gen_cmd_table[UIM_INVALIDATE_ST]      = uimgen_iso_handle_invalidate_st;
  uim_gen_cmd_table[UIM_REHABILITATE_ST]    = uimgen_iso_handle_rehabilitate_st;
  uim_gen_cmd_table[UIM_TERMINAL_PROFILE_ST]
                                            = uimgen_iso_handle_terminal_profile_st;
  uim_gen_cmd_table[UIM_ENVELOPE_ST]        = uimgen_iso_handle_envelope_st;
  uim_gen_cmd_table[UIM_FETCH_ST]           = uimgen_iso_handle_fetch_st;
  uim_gen_cmd_table[UIM_TERMINAL_RESPONSE_ST]
                                            = uimgen_iso_handle_terminal_response_st;
  uim_gen_cmd_table[UIM_POLLING_INTERVAL_ST]
                                            = uimgen_iso_handle_polling_interval_st;
  uim_gen_cmd_table[UIM_STREAM_APDU_ST]     = uimgen_iso_handle_stream_apdu_st;
  uim_gen_cmd_table[UIM_POWER_DOWN_ST]      = uimgen_iso_handle_power_down_st;
  uim_gen_cmd_table[UIM_RECOVERY_COMPLETE_ST]
                                            = uimgen_iso_handle_recovery_complete_st;
  uim_gen_cmd_table[UIM_RECOVERY_PERFORM_TP_ST]
                                            = uimgen_iso_handle_recovery_perform_tp_st;
  uim_gen_cmd_table[UIM_RECOVERY_FLUSH_MANAGE_CHANNEL_ST]
                                 = uimgen_iso_handle_recovery_flush_manage_channel_st;
  uim_gen_cmd_table[UIM_RECOVERY_MANAGE_CHANNEL_FOR_UICC_ST]
                                 = uimgen_iso_handle_recovery_manage_channel_for_uicc_st;
  uim_gen_cmd_table[UIM_RECOVERY_CLOSE_CHANNEL_FOR_UICC_ST]
                                 = uimgen_iso_handle_recovery_close_channel_for_uicc_st;
  uim_gen_cmd_table[UIM_RECOVERY_CHECK_FOR_SELECT_AID_ST]
                                 = uimgen_iso_handle_recovery_check_for_select_aid_st;
  uim_gen_cmd_table[UIM_RECOVERY_VERIFY_CHV1_ST]
                                 = uimgen_iso_handle_recovery_verify_chv1_st;
  uim_gen_cmd_table[UIM_RECOVERY_VERIFY_CHV2_ST]
                                 = uimgen_iso_handle_recovery_verify_chv2_st;
  uim_gen_cmd_table[UIM_RECOVERY_SEND_STATUS_COMMAND_ST]
                                 = uimgen_iso_handle_recovery_send_status_st;
  uim_gen_cmd_table[UIM_RECOVERY_VERIFY_UPIN_FOR_UICC_ST]
                                 = uimgen_iso_handle_recovery_verify_upin_for_uicc_st;
  uim_gen_cmd_table[UIM_RECOVERY_CHECK_REHAB_REQD_ST]
                                 = uimgen_iso_handle_recovery_check_rehab_reqd_st;
  uim_gen_cmd_table[UIM_RECOVERY_REHAB_ST]  = uimgen_iso_handle_recovery_rehab_st;
  uim_gen_cmd_table[UIM_RECOVERY_CHECK_EF_TST_UPDATE_REQD_ST]
                                 = uimgen_iso_handle_recovery_check_ef_tst_update_reqd_st;
  uim_gen_cmd_table[UIM_RECOVERY_SELECT_EF_TST_ST]
                                 = uimgen_iso_handle_recovery_select_ef_tst_st;
  uim_gen_cmd_table[UIM_RECOVERY_WRITE_EF_TST_ST]
                                 = uimgen_iso_handle_recovery_write_ef_tst_st;
  uim_gen_cmd_table[UIM_GET_RESPONSE_ST]    = uimgen_iso_handle_get_response_st;
  uim_gen_cmd_table[UIM_SILENT_RECOVER_UICC_ST]
                                            = uimgen_iso_handle_silent_recover_uicc_st;
  uim_gen_cmd_table[UIM_MANAGE_CHANNEL_ST]  = uimgen_iso_handle_manage_channel_st;
  uim_gen_cmd_table[UIM_SELECT_ICCID_ST]    = uimgen_iso_handle_select_iccid_st;
  uim_gen_cmd_table[UIM_READ_UMPC_ST]       = uimgen_iso_handle_read_umpc_st;
  uim_gen_cmd_table[UIM_SET_APDU_BEHAVIOR_ST]       
                                            = uimgen_iso_handle_set_apdu_behavior_st;
  uim_gen_cmd_table[UIM_SELECT_EF_DIR_ST]
                                            = uimgen_iso_handle_select_ef_dir_st;
  uim_gen_cmd_table[UIM_READ_EF_DIR_ST]
                                            = uimgen_iso_handle_read_ef_dir_st;											
  uim_gen_cmd_table[UIM_SELECT_ISDR_ST]     = uimgen_iso_handle_select_isdr_st;
  uim_gen_cmd_table[UIM_INIT_MC_ISDR_SEL_ST]= uimgen_iso_handle_init_manage_channel_isdr_sel_st;
  uim_gen_cmd_table[UIM_GET_EID_ST]         = uimgen_iso_handle_get_eid_st;
  uim_gen_cmd_table[UIM_NOT_PUP_IF_ALREADY_ST]
                                            = uimgen_iso_handle_not_pup_if_already_st;
  uim_gen_cmd_table[UIM_DONE_ST]            = uimgen_iso_handle_done_st;
  uim_gen_cmd_table[UIM_SIMULATE_NULL_ST]   = uimgen_iso_handle_simulate_null_st;
} /* uim_init_gen_cmd_func_ptr_table */


/*===========================================================================
 FUNCTION UIMGEN_HAL_INIT_RSP_FUNCTION_POINTER_TABLES
===========================================================================*/
void uimgen_hal_init_rsp_func_ptr_table(void)
{
  uim_gen_rsp_table[UIM_POWER_UP_ST]        = uimgen_iso_handle_power_up_st_rsp;
  uim_gen_rsp_table[UIM_RESET_ST]           = uimgen_iso_handle_reset_st_rsp;
  uim_gen_rsp_table[UIM_DELAY_AFTER_ATR_ST] = uimgen_iso_handle_delay_after_atr_st_rsp;
  uim_gen_rsp_table[UIM_PPS_ST]             = uimgen_iso_handle_pps_st_rsp;
  uim_gen_rsp_table[UIM_SEND_STATUS_ST]     = uimgen_iso_handle_send_status_st_rsp;
  uim_gen_rsp_table[UIM_UPDATE_OP_PARAMS_ST]
                                            = uimgen_iso_handle_update_op_params_st_rsp;
#if defined( FEATURE_UIM_T_1_SUPPORT )
  uim_gen_rsp_table[UIM_IFS_NEG_ST]         = uimgen_iso_handle_ifs_neg_st_rsp;
#endif /* FEATURE_UIM_T_1_SUPPORT */
  uim_gen_rsp_table[UIM_CHECK_CHARACTERISTICS_ST]
                                            = uimgen_iso_handle_check_characteristics_st_rsp;
  uim_gen_rsp_table[UIM_TERMINAL_CAPABILITY_ST]
                                            = uimgen_iso_handle_terminal_capability_st_rsp;
  uim_gen_rsp_table[UIM_CHECK_FOR_CDMA_DF]  = uimgen_iso_handle_check_for_cdma_df_st_rsp;
  uim_gen_rsp_table[UIM_READ_ICCID_ST]      = uimgen_iso_handle_read_iccid_st_rsp;
  uim_gen_rsp_table[UIM_SELECT_ST]          = uimgen_iso_handle_select_st_rsp;
  uim_gen_rsp_table[UIM_RESELECT_CUR_DIR_ST]
                                            = uimgen_iso_handle_reselect_curr_dir_st_rsp;
  uim_gen_rsp_table[UIM_SELECT_FAILED_ST]   = uimgen_iso_handle_select_failed_st_rsp;
  uim_gen_rsp_table[UIM_STREAM_SELECT_ST]   = uimgen_iso_handle_stream_select_st_rsp;
  uim_gen_rsp_table[UIM_STATUS_ST]          = uimgen_iso_handle_status_st_rsp;
  uim_gen_rsp_table[UIM_READ_BIN_MEM_ST]    = uimgen_iso_handle_read_bin_mem_st_rsp;
  uim_gen_rsp_table[UIM_UPDATE_BIN_MEM_ST]  = uimgen_iso_handle_update_bin_mem_st_rsp;
  uim_gen_rsp_table[UIM_READ_REC_MEM_ST]    = uimgen_iso_handle_read_rec_mem_st_rsp;
  uim_gen_rsp_table[UIM_UPDATE_REC_MEM_ST]  = uimgen_iso_handle_update_rec_mem_st_rsp;
  uim_gen_rsp_table[UIM_INC_MEM_ST]         = uimgen_iso_handle_inc_mem_st_rsp;
  uim_gen_rsp_table[UIM_SEEK_ST]            = uimgen_iso_handle_seek_st_rsp;
  uim_gen_rsp_table[UIM_VERIFY_CHV1_ST]     = uimgen_iso_handle_verify_chv1_st_rsp;
  uim_gen_rsp_table[UIM_VERIFY_CHV2_ST]     = uimgen_iso_handle_verify_chv2_st_rsp;
  uim_gen_rsp_table[UIM_UNBLOCK_CHV1_ST]    = uimgen_iso_handle_unblock_chv1_st_rsp;
  uim_gen_rsp_table[UIM_UNBLOCK_CHV2_ST]    = uimgen_iso_handle_unblock_chv2_st_rsp;
  uim_gen_rsp_table[UIM_CHANGE_CHV1_ST]     = uimgen_iso_handle_change_chv1_st_rsp;
  uim_gen_rsp_table[UIM_CHANGE_CHV2_ST]     = uimgen_iso_handle_change_chv2_st_rsp;
  uim_gen_rsp_table[UIM_ENABLE_CHV1_ST]     = uimgen_iso_handle_enable_chv1_st_rsp;
  uim_gen_rsp_table[UIM_ENABLE_CHV2_ST]     = uimgen_iso_handle_enable_chv2_st_rsp;
  uim_gen_rsp_table[UIM_DISABLE_CHV1_ST]    = uimgen_iso_handle_disable_chv1_st_rsp;
  uim_gen_rsp_table[UIM_DISABLE_CHV2_ST]    = uimgen_iso_handle_disable_chv2_st_rsp;
  uim_gen_rsp_table[UIM_INVALIDATE_ST]      = uimgen_iso_handle_invalidate_st_rsp;
  uim_gen_rsp_table[UIM_REHABILITATE_ST]    = uimgen_iso_handle_rehabilitate_st_rsp;
  uim_gen_rsp_table[UIM_TERMINAL_PROFILE_ST]
                                            = uimgen_iso_handle_terminal_profile_st_rsp;
  uim_gen_rsp_table[UIM_ENVELOPE_ST]        = uimgen_iso_handle_envelope_st_rsp;
  uim_gen_rsp_table[UIM_FETCH_ST]           = uimgen_iso_handle_fetch_st_rsp;
  uim_gen_rsp_table[UIM_TERMINAL_RESPONSE_ST]
                                            = uimgen_iso_handle_terminal_response_st_rsp;
  uim_gen_rsp_table[UIM_POLLING_INTERVAL_ST]
                                            = uimgen_iso_handle_polling_interval_st_rsp;
  uim_gen_rsp_table[UIM_STREAM_APDU_ST]     = uimgen_iso_handle_stream_apdu_st_rsp;
  uim_gen_rsp_table[UIM_POWER_DOWN_ST]      = uimgen_iso_handle_power_down_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_COMPLETE_ST]
                                            = uimgen_iso_handle_recovery_complete_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_PERFORM_TP_ST]
                                            = uimgen_iso_handle_recovery_perform_tp_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_FLUSH_MANAGE_CHANNEL_ST]
                                 = uimgen_iso_handle_recovery_flush_manage_channel_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_MANAGE_CHANNEL_FOR_UICC_ST]
                                 = uimgen_iso_handle_recovery_manage_channel_for_uicc_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_CLOSE_CHANNEL_FOR_UICC_ST]
                                 = uimgen_iso_handle_recovery_close_channel_for_uicc_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_CHECK_FOR_SELECT_AID_ST]
                                 = uimgen_iso_handle_recovery_check_for_select_aid_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_VERIFY_CHV1_ST]
                                 = uimgen_iso_handle_recovery_verify_chv1_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_VERIFY_CHV2_ST]
                                 = uimgen_iso_handle_recovery_verify_chv2_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_SEND_STATUS_COMMAND_ST]
                                 = uimgen_iso_handle_recovery_send_status_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_VERIFY_UPIN_FOR_UICC_ST]
                                 = uimgen_iso_handle_recovery_verify_upin_for_uicc_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_CHECK_REHAB_REQD_ST]
                                 = uimgen_iso_handle_recovery_check_rehab_reqd_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_REHAB_ST]  = uimgen_iso_handle_recovery_rehab_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_CHECK_EF_TST_UPDATE_REQD_ST]
                                 = uimgen_iso_handle_recovery_check_ef_tst_update_reqd_st_rsp;
  uim_gen_rsp_table[UIM_RECOVERY_SELECT_EF_TST_ST]
                                 = uimgen_iso_handle_recovery_select_ef_tst_st_rsp;

  uim_gen_rsp_table[UIM_RECOVERY_WRITE_EF_TST_ST]
                                 = uimgen_iso_handle_recovery_write_ef_tst_st_rsp;
  uim_gen_rsp_table[UIM_GET_RESPONSE_ST]    = uimgen_iso_handle_get_response_st_rsp;
  uim_gen_rsp_table[UIM_SILENT_RECOVER_UICC_ST]
                                            = uimgen_iso_handle_silent_recover_uicc_st_rsp;
  uim_gen_rsp_table[UIM_MANAGE_CHANNEL_ST]  = uimgen_iso_handle_manage_channel_st_rsp;
  uim_gen_rsp_table[UIM_SELECT_ICCID_ST]    = uimgen_iso_handle_select_iccid_st_rsp;
  uim_gen_rsp_table[UIM_READ_UMPC_ST]       = uimgen_iso_handle_read_umpc_st_rsp;
  uim_gen_rsp_table[UIM_SET_APDU_BEHAVIOR_ST]
                                            = uimgen_iso_handle_set_apdu_behavior_rsp;  
  uim_gen_rsp_table[UIM_SELECT_EF_DIR_ST]
                                            = uimgen_iso_handle_select_ef_dir_st_rsp;             
  uim_gen_rsp_table[UIM_READ_EF_DIR_ST]
                                            = uimgen_iso_handle_read_ef_dir_st_rsp;											
  uim_gen_rsp_table[UIM_SELECT_ISDR_ST]     = uimgen_iso_handle_select_isdr_st_rsp;
  uim_gen_rsp_table[UIM_GET_EID_ST]         = uimgen_iso_handle_get_eid_st_rsp;
  uim_gen_rsp_table[UIM_NOT_PUP_IF_ALREADY_ST]
                                            = uimgen_iso_handle_not_pup_if_already_st_rsp;
  uim_gen_rsp_table[UIM_DONE_ST]            = uimgen_iso_handle_done_st_rsp;
  uim_gen_rsp_table[UIM_SIMULATE_NULL_ST]   = uimgen_iso_handle_simulate_null_st_rsp;
} /* uim_init_gen_rsp_func_ptr_table */


/*===========================================================================
 FUNCTION UIMGEN_HAL_HANDLE_GENERIC_CMD_ST
===========================================================================*/
void uimgen_hal_handle_generic_cmd_st
(
  uim_instance_global_type *uim_ptr
)
{
  uim_gen_cmd_table[*uim_ptr->command.generic_state_ptr](uim_ptr);
} /* uimgen_hal_handle_generic_cmd_st */


/*===========================================================================
 FUNCTION UIMGEN_HAL_HANDLE_GENERIC_CMD_ST_RSP
===========================================================================*/
uim_cmd_status_type uimgen_hal_handle_generic_cmd_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
)
{
  return uim_gen_rsp_table[*uim_ptr->command.generic_state_ptr](rsp_ptr, uim_ptr, completed);
}
