#ifndef UIMGEN_HAL_ISO_H
#define UIMGEN_HAL_ISO_H
/*===========================================================================

                            U I M G E N  H A L  I S O

DESCRIPTION
  This file contains generic state machine functions that are specific to
  ISO UIM

Copyright (c) 2015 - 2016, 2019 - 2020 by QUALCOMM Technologies, Inc (QTI) and its licensors. All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/uim.mpss/6.1.0/uimdrv/src/hal/uimgen_hal_iso.h#4 $
$DateTime: 2020/02/23 22:25:47 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/20/20   rkg     Add new TLV in Suscription manager
01/31/20   ykr     Disabling slot as per SIMLOCK configuration
09/17/19   mm      Add EID to SimSlotStatus radio config HAL
09/22/16   gm      Added support for set APDU behavior
06/10/16   gm      Remove the test code of Simulate Null byte
11/26/15   sam     Add logic to update EF-TST in recovery
09/02/15   hyo     Initial Revision
===========================================================================*/
#include "uimgen_hal.h"
#include "uimglobals.h"

/*============================================================================= 
  GENERIC COMMAND STATES
=============================================================================*/
/**
 *  This function sends power down command to UIMDRV
 * 
 *  Side Effect: updates command status in response buffer global
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_power_down_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function sends power up command to UIMDRV 
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_power_up_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function sends reset command to UIMDRV
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_reset_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function initiates send pps to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_pps_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function puts UIM task in a sleep for specified time
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_delay_after_atr_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function calls uimdrv function for update op params
 *  operation
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_update_op_params_st
(
  uim_instance_global_type *uim_ptr
);


#ifdef FEATURE_UIM_T_1_SUPPORT
/**
 *  This function builds apdu for ifs neg command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_ifs_neg_st
(
  uim_instance_global_type *uim_ptr
);
#endif /* FEATURE_UIM_T_1_SUPPORT */


/**
 *  This function builds apdu for read iccid command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_read_iccid_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function initiates selection of cdma df
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_check_for_cdma_df_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for check characteristics command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_check_characteristics_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for select iccid command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_select_iccid_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for select EF-DIR command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_select_ef_dir_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for read EF-DIR command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_read_ef_dir_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for read umpc command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_read_umpc_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function retries select
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_select_failed_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for stream select command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_stream_select_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for select command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_select_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for select command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_reselect_curr_dir_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds the command to open channel to send select ISDR command
 *
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
*/
void uimgen_iso_handle_init_manage_channel_isdr_sel_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function handles GET EID command to an eUICC
 *
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
*/
void uimgen_iso_handle_get_eid_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function handles select ISDR in case of an eUICC
 *
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
*/
void uimgen_iso_handle_select_isdr_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for terminal capability command and send it down
   to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_terminal_capability_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for send status command and send it down
 *  to the card
 *
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_send_status_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for status command and send it down
   to the card
 *
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_status_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for send status command and send it down
 *  to the card
 *  
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_send_status_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for read bin mem command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_read_bin_mem_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for update bin mem command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_update_bin_mem_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for read rec mem command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_read_rec_mem_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for update rec mem command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_update_rec_mem_st
(
  uim_instance_global_type *uim_ptr
);

/**
 *  This function builds apdu for inc mem command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_inc_mem_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for verify chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_verify_chv1_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for verify chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_verify_chv2_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for unblock chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_unblock_chv1_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for unblock chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_unblock_chv2_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for change chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_change_chv1_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for change chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_change_chv2_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for enable chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_enable_chv1_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for enable chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_enable_chv2_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for disable chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_disable_chv1_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for disable chv command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_disable_chv2_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for invalidate command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_invalidate_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for rehabilitate command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_rehabilitate_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for seek command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_seek_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for terminal profile command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_terminal_profile_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for envelope command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_envelope_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for fetch command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_fetch_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for terminal response command and send it down
 *  to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_terminal_response_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function modifies poll timer
 *  
 *  Side effect: proactive_time_delay field in UIM global
 *  pointer is modified
 *  
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_polling_interval_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function initiates handling of stream apdu state
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_stream_apdu_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function switches back to the previously saved mode
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_complete_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function initiates the response processing
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_done_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function clears the manage channel command queues and
 *  proceeds to next state
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_flush_manage_channel_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for verify pin command and send it
 *  down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_verify_chv1_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for verify pin command and send it
 *  down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_verify_chv2_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for terminal profile command and
 *  send it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_perform_tp_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function check if rehabilitate is required or not and
 *  proceeds to appropriate state
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_check_rehab_reqd_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for rehabilitate command and send
 *  it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_rehab_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function check if ef-tst write  is required or not and
 *  proceeds to appropriate state
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_check_ef_tst_update_reqd_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function selects ef-tst file
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_select_ef_tst_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for write of EF-tst and sends
 *  it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_write_ef_tst_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for manage channel command and
 *  send it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_manage_channel_for_uicc_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for recovery close channel for
 *  uicc command and send it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_close_channel_for_uicc_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for recovery check for select aid
 *  command and send it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_check_for_select_aid_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for recovery verify upin for uicc
 *  command and send it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_recovery_verify_upin_for_uicc_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for get response command and send
 *  it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_get_response_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function raises command timeout signal
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_silent_recover_uicc_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function builds apdu for manage channel command and
 *  send it down to the card
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_manage_channel_st
(
  uim_instance_global_type *uim_ptr
);


/**
 *  This function set the APDU behavior of modem for warning status
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_set_apdu_behavior_st
(
  uim_instance_global_type *uim_ptr
);

/**
 *  This function process the reset command if card was already up
 * 
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 */
void uimgen_iso_handle_not_pup_if_already_st
(
  uim_instance_global_type *uim_ptr
);

/*============================================================================= 
  GENERIC RESPONSE STATES
=============================================================================*/
/**
 *  This function process the response returned from the card for
 *  power down command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_power_down_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  power up command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_power_up_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  reset command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_reset_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  pps command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_pps_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process delay after rsp state
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_delay_after_atr_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  update op params command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_update_op_params_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


#ifdef FEATURE_UIM_T_1_SUPPORT 
/**
 *  This function process the response returned from the card for
 *  ifs neg command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_ifs_neg_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);
#endif /* FEATURE_UIM_T_1_SUPPORT */


/**
 *  This function process the response returned from the card for
 *  read iccid command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_read_iccid_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  check for cdma df command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_check_for_cdma_df_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  check characteristics command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_check_characteristics_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  select iccid command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_select_iccid_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  select EF-DIR command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_select_ef_dir_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  read EF-DIR command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_read_ef_dir_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function process the response returned from the card for
 *  read umpc command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_read_umpc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes select failed state
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_select_failed_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  stream select command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_stream_select_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  select command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_select_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  reselect curr dir command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_reselect_curr_dir_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  terminal capability command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_terminal_capability_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  send status command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_send_status_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  status command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_status_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery send status command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_send_status_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  read bin mem command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_read_bin_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  update bin mem command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_update_bin_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  read rec mem command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_read_rec_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  update rec mem command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_update_rec_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  inc mem command
 *
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_inc_mem_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  verify chv1 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_verify_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  verify chv2 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_verify_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  unblcok chv1 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_unblock_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  unblock chv2 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_unblock_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  change chv1 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_change_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  change chv2 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_change_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card
 *  for enable chv1 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_enable_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  enable chv2 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_enable_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  disable chv1 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_disable_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  disable chv2 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_disable_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  invalidate command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_invalidate_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  rehabilitate command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_rehabilitate_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  seek command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_seek_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  terminal profile
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_terminal_profile_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  envelope command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_envelope_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  fetch command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_fetch_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  terminal response command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_terminal_response_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  polling interval command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_polling_interval_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  stream apdu command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_stream_apdu_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes recovery complete state
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_complete_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes command done state
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_done_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery flush manage channel command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_flush_manage_channel_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery verify chv1 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_verify_chv1_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery verify chv2 command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_verify_chv2_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery perform tp command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_perform_tp_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery check rehab reqd command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_check_rehab_reqd_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery rehab command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_rehab_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery check update ef-tst required command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_check_ef_tst_update_reqd_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery select ef-tst command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_select_ef_tst_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  write ef tst command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_write_ef_tst_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery manage channel for uicc command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_manage_channel_for_uicc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card
 *  forrecovery close channel for uicc command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_close_channel_for_uicc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery check for select aid command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_check_for_select_aid_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  recovery verify pin for uicc command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_recovery_verify_upin_for_uicc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  get response command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_get_response_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  silence recovery uicc command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_silent_recover_uicc_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response returned from the card for
 *  manage channel command
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_manage_channel_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);


/**
 *  This function processes the response for set transmit behavior
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if there is a proactive command
 *                   pending
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_set_apdu_behavior_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed
);

/**
 *  This function processes the response for a GET EID cmd
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if command is completed or not
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_get_eid_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed_ptr
);


/**
 *  This function processes the response for a select ISDR cmd
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if command is completed or not
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_select_isdr_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed_ptr
);

/**
 *  This function processes the response for reset command when card is already up
 *  
 *  @param rsp_ptr[OUT] Pointer to the command response buffer
 *  @param uim_ptr[IN] Pointer to the current instance of the UIM
 *                 global structure
 *  @param completed[OUT] indication if command is completed or not
 *  @return uim_cmd_status_type card response of the command
 *          status
 */
uim_cmd_status_type uimgen_iso_handle_not_pup_if_already_st_rsp
(
  uim_rsp_buf_type         *rsp_ptr,
  uim_instance_global_type *uim_ptr,
  uim_cmd_status_type      *completed_ptr
);

#endif /* UIMGEN_HAL_ISO_H */
