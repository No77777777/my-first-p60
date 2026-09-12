/**
  @file IWCDMA_ext.h

  @brief  This is the header file that describes an interface to a component

*/
/*
  Copyright (c) 2013 - 2015 Qualcomm Technologies, Inc.
          All Rights Reserved.
    Qualcomm Confidential and Proprietary
*/

/*===========================================================================
                      EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/wcdma.mpss/8.4.0/api/IWCDMA_ext.h#3 $
  $DateTime: 2020/01/24 03:56:36 $


 when         who     what, where, why
 ----------   ---     ------------------------------------------------------------
 12/24/15     svh     Removed SS MMCP CTCH APIs.
 07/22/15     ac      Fix MMCP CTCH MSIM macros to use correct function name 
 06/17/15     ac      Added MSIM version of MMCP CTCH APIs
 04/09/15     mk      Made changes to clean up QCHAT specific APIs
                      for removal of seq.h inclusion.
 03/13/15     aa      Made changes to clean up QCHAT specific APIs
 07/01/14     hk      Header file inclusion cleanup
 10/17/13     hzhi    Support new FEATURE_DUAL_SIM on DIME3.0. 
 10/09/13     hzhi    Moved private api pointers for RFTECH_WCDMA/MMCP/Qchat here.
 01/01/13     ngeng   Created for segment loading.

===========================================================================*/
#ifndef _IWCDMA_EXT_H_
#define _IWCDMA_EXT_H_

#include "mcfg_seg_load.h"

/*======================================================================================
  editted by hzhi on 09/23/2013 to update WCDMA interface on DIME3.0 for segment loading. 
  ======================================================================================*/
#include "wcdma_common_api.h"
#include "l1lsmif.h"
#include "l1task.h"
#include "rrcintif.h"
#include "rrccmd.h"
#include "l1extif.h"

/*=====================================================================================*/

/*  The component that is being used provides typedefs for pointers to each
    of its exported functions. 
*/

typedef void 
  (*fn_dl_dec_deregister_ctch_pdu_decode_error_cb_msim)
    (
      sys_modem_as_id_e_type as_id
    );	//private

typedef void 
  (*fn_dl_dec_register_ctch_pdu_decode_error_cb_msim)
    (
      DL_DEC_CTCH_PDU_DECODE_ERROR_CB_FUNC_TYPE ctch_pdu_decode_error_cb,
      sys_modem_as_id_e_type as_id
    );	//private


typedef void (*fn_drx_manager_wake_from_sleep)(void);	//private
typedef void (*fn_drx_sleep_abort)(void);		//private
typedef void (*fn_l1m_setup_next_cmd) ( void );		//private
typedef rrc_new_cell_status_e_type (*fn_rrc_ccm_get_serv_plmn_cellid)
(
 rrc_int_cell_id_type *cell_id, 
 rrc_plmn_identity_type *plmn_id
);	//private
typedef rrc_cmd_type * ( *fn_rrc_get_int_cmd_buf) (void);	//private
typedef rrc_state_e_type (*fn_rrc_get_state) ( void );		//private
typedef void (*fn_rrc_put_int_cmd)
(
  rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
);	//private

#ifdef FEATURE_HSPA_CALL_STATUS_IND
typedef void (*fn_rrc_send_hspa_call_status_info) (void);   //private
#endif /* FEATURE_HSPA_CALL_STATUS_IND */

typedef void (*fn_wl1_send_msg_to_controller) (rrc_cmd_type *cmd_ptr);  //private
typedef void 
  (*fn_sleep_abort)
    (
      void
    );  //private

typedef void (*fn_l1_free_cmd_buf)
(
  /* Command buffer to be freed */
  l1_ext_cmd_type* cmd_ptr
);

/*  The component then provides a vtable beginning with add_ref, release,
    and qi, followed by the functions it exports.
*/
/*  The component then provides a vtable beginning with add_ref, release,
    and qi, followed by the functions it exports.
*/
typedef struct
{
  add_ref_fn  add_ref;
  release_fn  release;
  qi_fn       qi;
  fn_dl_dec_deregister_ctch_pdu_decode_error_cb_msim dl_dec_deregister_ctch_pdu_decode_error_cb_msim;
  fn_dl_dec_register_ctch_pdu_decode_error_cb_msim dl_dec_register_ctch_pdu_decode_error_cb_msim;
} iWCDMA_mmcp_vtbl;

/*  The component provides a macro to get the pointer to its vtable from
    a pointer to an interface (defined in dynload.h).
*/
#define WCDMA_mmcp_vtbl(pi)   ((iWCDMA_mmcp_vtbl *) ((pi)->pVtable))
#define IWCDMA_dl_dec_deregister_ctch_pdu_decode_error_cb_msim(pi,as_id) (WCDMA_mmcp_vtbl(pi))->dl_dec_deregister_ctch_pdu_decode_error_cb_msim(as_id)
#define IWCDMA_dl_dec_register_ctch_pdu_decode_error_cb_msim(pi,ctch_pdu_decode_error_cb,as_id) (WCDMA_mmcp_vtbl(pi))->dl_dec_register_ctch_pdu_decode_error_cb_msim(ctch_pdu_decode_error_cb,as_id)

/*  The component then provides a vtable beginning with add_ref, release,
    and qi, followed by the functions it exports.
*/
typedef struct
{
  add_ref_fn  add_ref;
  release_fn  release;
  qi_fn       qi;
} iWCDMA_rftech_vtbl;

/*  The component provides a macro to get the pointer to its vtable from
    a pointer to an interface (defined in dynload.h).
*/
#define WCDMA_rftech_vtbl(pi)   ((iWCDMA_rftech_vtbl *) ((pi)->pVtable))

/*  The component then provides a vtable beginning with add_ref, release,
    and qi, followed by the functions it exports.
*/
typedef struct
{
  add_ref_fn  add_ref;
  release_fn  release;
  qi_fn       qi;
  fn_drx_manager_wake_from_sleep drx_manager_wake_from_sleep;  
  fn_drx_sleep_abort drx_sleep_abort;
  fn_l1m_setup_next_cmd l1m_setup_next_cmd;
  fn_rrc_ccm_get_serv_plmn_cellid rrc_ccm_get_serv_plmn_cellid;
  fn_rrc_get_int_cmd_buf rrc_get_int_cmd_buf;
  fn_rrc_get_state rrc_get_state;
  fn_rrc_put_int_cmd rrc_put_int_cmd;
#ifdef FEATURE_HSPA_CALL_STATUS_IND
  fn_rrc_send_hspa_call_status_info rrc_send_hspa_call_status_info;
#endif /* FEATURE_HSPA_CALL_STATUS_IND */
  fn_wl1_send_msg_to_controller wl1_send_msg_to_controller;
  fn_sleep_abort sleep_abort;
  fn_l1_free_cmd_buf l1_free_cmd_buf;
} iWCDMA_qchat_vtbl;

/*  The component provides a macro to get the pointer to its vtable from
    a pointer to an interface (defined in dynload.h).
*/
#define WCDMA_qchat_vtbl(pi)   ((iWCDMA_qchat_vtbl *) ((pi)->pVtable))
#define IWCDMA_drx_manager_wake_from_sleep(pi)     (WCDMA_qchat_vtbl(pi))->drx_manager_wake_from_sleep()
#define IWCDMA_drx_sleep_abort(pi)                 (WCDMA_qchat_vtbl(pi))->drx_sleep_abort()
#define IWCDMA_l1m_setup_next_cmd(pi)               (WCDMA_qchat_vtbl(pi))->l1m_setup_next_cmd()
#define IWCDMA_rrc_ccm_get_serv_plmn_cellid(pi,cell_id,plmn_id) (WCDMA_qchat_vtbl(pi))->rrc_ccm_get_serv_plmn_cellid(cell_id,plmn_id)
#define IWCDMA_rrc_get_int_cmd_buf(pi) (WCDMA_qchat_vtbl(pi))->rrc_get_int_cmd_buf() 
#define IWCDMA_rrc_get_state(pi)         (WCDMA_qchat_vtbl(pi))->rrc_get_state()
#define IWCDMA_rrc_put_int_cmd(pi,cmd_buf) (WCDMA_qchat_vtbl(pi))->rrc_put_int_cmd(cmd_buf)
#define IWCDMA_rrc_send_hspa_call_status_info(pi)   (WCDMA_qchat_vtbl(pi))->rrc_send_hspa_call_status_info()
#define IWCDMA_wl1_send_msg_to_controller(pi,cmd_ptr) (WCDMA_qchat_vtbl(pi))->wl1_send_msg_to_controller(cmd_ptr)
#define IWCDMA_sleep_abort(pi) (WCDMA_qchat_vtbl(pi))->sleep_abort()
#define IWCDMA_l1_free_cmd_buf(pi,cmd_ptr) (WCDMA_qchat_vtbl(pi))->l1_free_cmd_buf(cmd_ptr)
#endif /* _IWCDMA_EXT_H_ */