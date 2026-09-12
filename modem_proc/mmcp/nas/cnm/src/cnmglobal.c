/*===========================================================================
                       COPYRIGHT INFORMATION

Copyright (c) 2001 Qualcomm Technologies, Incorporated.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

                       EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/cnm/vcs/cnmglobal.c_v   1.7   22 Apr 2002 10:24:12   cdealy  $   
$Header: //components/rel/mmcp.mpss/7.9.0/nas/cnm/src/cnmglobal.c#1 $    $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
23-Mar-2001  VT      Created

23-May-2001  VT      Removed some unwanted include(s)

05-Jun-2001  VT      Added q_type mn_cm_cmd_q (as it is extern now in mn_cc.h)

08/06/01     CD      Rearranged header files

08/14/01     AB      Renamed connection manager references to CNM from CM.

27-Sep-2001  VT      Added the declaration for the following global data (after
                       making it extern in cnm_init_data.c) :
                          rex_timer_type mn_cnm_sleep_timer;

04/19/02     CD      Deleted mm_connections_active definition and added 
                     mm_rel_req_sent flag

07/13/05     HS      CNM_mt_cp_message and CNM_mt_cp_data_retransmit_counter
                     exanded to save CP data for all call ids

07/14/06     NR      Adding changes for SMS optimization CR 93099 
===========================================================================*/
/*===========================================================================
                          MN_CNM Interface Library Functions

DESCRIPTION

 This module contains the MN_CNM global variables and structures. 
 
   
EXTERNALIZED FUNCTIONS

  
INITIALIZATION AND SEQUENCING REQUIREMENTS
  

===========================================================================*/

#include "mmcp_variation.h"
#include "customer.h"
#include "rex.h"
#include "queue.h"
#include "task.h"
#include "environ.h"
#include "mn.h"
#include "cmlib.h"
#include "cnm.h"
#include "cm_sms.h"
#include "qsh.h"
#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)
#include "mncnm_qsh.h"
#endif

#include "mn_cnmtask_v.h"
#include "mn_cnmtask_int.h"
#include "ms.h"
#include "qsh.h"
#include "nasutils.h"


q_type                    mn_cnm_cmd_q;

/* Internal high priority CMD Q for MN_CNM task */
q_type                    mn_cnm_internal_cmd_q;

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)

cc_transaction_information_T *  
                          cc_transaction_information_p_sim[MAX_AS_IDS][MAXNO_CC_TRANSACTIONS];
sms_transaction_information_T   
                       sms_transaction_information_sim[MAX_AS_IDS][MAXNO_SMS_TRANSACTIONS];
ss_transaction_information_T   
                       ss_transaction_information_sim[MAX_AS_IDS][MAXNO_SS_TRANSACTIONS];

cnm_request_id_T          mm_connection_pending_sim[MAX_AS_IDS];
boolean                   mm_link_reestablishing_sim[MAX_AS_IDS];
boolean                   mm_rel_req_sent_sim[MAX_AS_IDS];

cc_state_T                last_state_sim[MAX_AS_IDS]; 
byte                      cc_progress_information_sim[MAX_AS_IDS][MAXNO_CC_TRANSACTIONS];
timer_store_T             timer_store_sim[MAX_AS_IDS][MAX_CNM_TIMER];
byte                      timer_store_pointer_sim[MAX_AS_IDS];

/* TCH assignment */
mmcc_sync_ind_T           CNM_mmcc_sync_ind_sim[MAX_AS_IDS] = {{{0, 0, 0, 0}, 0, 0, 0, 0, (byte)WCDMA_RAB_RELEASED},
                                                               {{0, 0, 0, 0}, 0, 0, 0, 0, (byte)WCDMA_RAB_RELEASED}};
 

mmcnm_data_req_T         CNM_mo_cp_message_sim[MAX_AS_IDS];

mmcnm_data_req_T        * CNM_mt_cp_message_sim[MAX_AS_IDS][MAXNO_SMS_TRANSACTIONS-8]; /* only for MT */


byte                     CNM_mo_cp_data_retransmit_counter_sim[MAX_AS_IDS];
byte                     CNM_mt_cp_data_retransmit_counter_sim[MAX_AS_IDS][MAXNO_SMS_TRANSACTIONS-8]; /* only for MT */
CNM_cp_user_data_T       CNM_mo_cp_user_data_sim[MAX_AS_IDS];
byte                     cp_cause_value_sim[MAX_AS_IDS];
boolean cp_data_l2_ack_sim[MAX_AS_IDS];

#ifdef FEATURE_QSH_EVENT_METRIC
cnm_qsh_debug_metrics_s_type cnm_qsh_debug_metrics_sim[MAX_AS_IDS];
#define cnm_qsh_debug_metrics cnm_qsh_debug_metrics_sim[mn_dsda_as_id]
#endif
#else

cc_transaction_information_T *  
                          cc_transaction_information_p[MAXNO_CC_TRANSACTIONS];
sms_transaction_information_T   
                       sms_transaction_information[MAXNO_SMS_TRANSACTIONS];
ss_transaction_information_T   
                       ss_transaction_information[MAXNO_SS_TRANSACTIONS];
cnm_request_id_T          mm_connection_pending;

boolean                   mm_link_reestablishing;

/* ** mm_rel_req_sent ** 
 * This variable flags whether CNM waits for MMCNM_REL_CNF in response to CNM's MMCNM_REL_REQ message to MM.
 * Whenever CNM sends MMCNM_REL_REQ, CNM does following as well
 * starts timer CNM_MM_REL_TIMER
 * resets mn_as_id to NONE in DSDS.
 * */
boolean                   mm_rel_req_sent;

/* 
 * state prior to processing a msg 
 */

cc_state_T                last_state; 

byte                      cc_progress_information[MAXNO_CC_TRANSACTIONS];

timer_store_T             timer_store[MAX_CNM_TIMER];
byte                      timer_store_pointer;


/* TCH assignment */
mmcc_sync_ind_T           CNM_mmcc_sync_ind = {{0, 0, 0, 0}, 0, 0, 0, 0, (byte)WCDMA_RAB_RELEASED};
 

mmcnm_data_req_T         CNM_mo_cp_message;

mmcnm_data_req_T        * CNM_mt_cp_message[MAXNO_SMS_TRANSACTIONS-8]; /* only for MT */


byte                     CNM_mo_cp_data_retransmit_counter;
byte                     CNM_mt_cp_data_retransmit_counter[MAXNO_SMS_TRANSACTIONS-8]; /* only for MT */
CNM_cp_user_data_T       CNM_mo_cp_user_data;
byte                     cp_cause_value;




boolean cp_data_l2_ack;

#ifdef FEATURE_QSH_EVENT_METRIC
cnm_qsh_debug_metrics_s_type cnm_qsh_debug_metrics;
#endif

#endif

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)

#ifdef FEATURE_QSH_DUMP
cnm_qsh_dump_tag_as_id_1_s *cnm_qsh_dump_tag_0_ptr;
cnm_qsh_dump_tag_as_id_2_s *cnm_qsh_dump_tag_1_ptr;

size_t cnm_get_qsh_dump_data
(
  cnm_qsh_dump_data_s_type *dump_ptr,
  cnm_qsh_dump_tag_e tag 
) __attribute__((section(".uncompressible.text")));

void cnm_populate_qsh_dump_data 
(
  qsh_client_cb_params_s *param
)__attribute__((section(".uncompressible.text")));

void cnm_post_qsh_message_ind
(
  qsh_client_cb_params_s *param 
)__attribute__((section(".uncompressible.text")));

void cnm_qsh_cb
(
  qsh_client_cb_params_s  *param
)__attribute__((section(".uncompressible.text")));



/*===========================================================================
Function: mn_populate_qsh_dump_data()

Description:
CNM task QSH dump data for for Dump collection action.

============================================================================*/

size_t cnm_get_qsh_dump_data( cnm_qsh_dump_data_s_type *dump_ptr, cnm_qsh_dump_tag_e tag )
{
  int i;

  mn_dsda_as_id_e_type as_id = MN_AS_ID_1;
  if(tag == CNM_QSH_DUMP_TAG_AS_ID_2)
  {
    as_id = MN_AS_ID_2;
  }
  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_ptr,(uint16)tag, sizeof(cnm_qsh_dump_data_s_type));

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
  //next_state_debug
  dump_ptr->next_state_debug = get_next_state(as_id);

  //last_state_debug
  dump_ptr->last_state_debug = last_state_sim[as_id];

  //sms_transaction_information_debug
  for ( i = 0; i < MAXNO_SMS_TRANSACTIONS; i++ )
  {
    if ( sms_transaction_information_sim[as_id][i].sms_state != SMS_IDLE ) 
    {
      memscpy((void *) &dump_ptr->sms_transaction_information_debug, sizeof(nas_cnm_sms_transaction_information_T_debug),
              (void *) &sms_transaction_information_sim[as_id][i], sizeof(sms_transaction_information_T));

      //sms_transaction_id_debug
      dump_ptr->sms_transaction_id_debug = i;

      //CNM_mt_cp_data_retransmit_counter_debug
      if ( i >= 8)
      {
        dump_ptr->CNM_mt_cp_data_retransmit_counter_debug = CNM_mt_cp_data_retransmit_counter_sim[as_id][i-8];
      }
      break;
    }
  }

  for ( i = 0; i < MAXNO_CC_TRANSACTIONS; i++ )
  {
    if ( cc_transaction_information_p_sim[as_id][i] != NULL ) 
    {
      memscpy((void *) &dump_ptr->cc_transaction_information_debug, sizeof(dump_ptr->cc_transaction_information_debug),
              (void *) cc_transaction_information_p_sim[as_id][i], sizeof(*cc_transaction_information_p_sim[as_id][i]));

      //cc_transaction_id_debug
      dump_ptr->cc_transaction_id_debug = i;
      break;
    }
  }

  for ( i = 0; i < MAXNO_SS_TRANSACTIONS; i++ )
  {
    if ( ss_transaction_information_sim[as_id][i].ss_state != SMS_IDLE ) 
    {
      memscpy((void *) &dump_ptr->ss_transaction_information_debug, sizeof(nas_cnm_ss_transaction_information_T_debug),
              (void *) &ss_transaction_information_sim[as_id][i], sizeof(ss_transaction_information_T));

      //ss_transaction_id_debug
      dump_ptr->ss_transaction_id_debug = i;
      break;
    }
  }

  //CNM_mmcc_sync_ind_debug
  dump_ptr->CNM_mmcc_sync_ind_debug = CNM_mmcc_sync_ind_sim[as_id];

  //mm_connection_pending_debug
  memscpy((void *) &dump_ptr->mm_connection_pending_debug, sizeof(nas_cnm_CNM_request_id_T_debug),
          (void *) &mm_connection_pending_sim[as_id], sizeof(cnm_request_id_T));

  //no_of_IEs_debug
  dump_ptr->no_of_IEs_debug = get_no_of_IEs(as_id); 

  //nas_cnm_IE_info_T_debug         cc_IE_info_debug; 


  //cc_progress_information_debug
  for (i=0; i < MAXNO_CC_TRANSACTIONS; i++)
  {
    if (cc_progress_information_sim[as_id][i] != 0 )
    {
      dump_ptr->cc_progress_information_debug = cc_progress_information_sim[as_id][i];
      break;
    }
 }

  //CNM_mo_cp_data_retransmit_counter_debug
  dump_ptr->CNM_mo_cp_data_retransmit_counter_debug = CNM_mo_cp_data_retransmit_counter;

  //cp_data_l2_ack_sim_debug
  dump_ptr->cp_data_l2_ack_sim_debug = cp_data_l2_ack_sim[0];

  //cp_data_l2_ack_sim_debug
  dump_ptr->cp_data_l2_ack_sim_debug =  mm_link_reestablishing_sim[as_id]; 

  //mm_rel_req_sent_debug
  dump_ptr->mm_rel_req_sent_debug =  mm_rel_req_sent_sim[as_id]; 
#else /* (FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE) */
  //next_state_debug
  dump_ptr->next_state_debug = get_next_state(as_id);

  //last_state_debug
  dump_ptr->last_state_debug = last_state;

  //sms_transaction_information_debug
  for ( i = 0; i < MAXNO_SMS_TRANSACTIONS; i++ )
  {
    if ( sms_transaction_information[i].sms_state != SMS_IDLE ) 
    {
      memscpy((void *) &dump_ptr->sms_transaction_information_debug, sizeof(nas_cnm_sms_transaction_information_T_debug),
              (void *) &sms_transaction_information[i], sizeof(sms_transaction_information_T));

      //sms_transaction_id_debug
      dump_ptr->sms_transaction_id_debug = i;

      //CNM_mt_cp_data_retransmit_counter_debug
      if ( i >= 8)
      {
        dump_ptr->CNM_mt_cp_data_retransmit_counter_debug = CNM_mt_cp_data_retransmit_counter[i-8];
      }
      break;
    }
  }

  for ( i = 0; i < MAXNO_CC_TRANSACTIONS; i++ )
  {
    if ( cc_transaction_information_p[i] != NULL ) 
    {
      memscpy((void *) &dump_ptr->cc_transaction_information_debug, sizeof(dump_ptr->cc_transaction_information_debug),
              (void *) cc_transaction_information_p[i], sizeof(*cc_transaction_information_p[i]));

      //cc_transaction_id_debug
      dump_ptr->cc_transaction_id_debug = i;
      break;
    }
  }

  for ( i = 0; i < MAXNO_SS_TRANSACTIONS; i++ )
  {
    if ( ss_transaction_information[i].ss_state != SMS_IDLE ) 
    {
      memscpy((void *) &dump_ptr->ss_transaction_information_debug, sizeof(nas_cnm_ss_transaction_information_T_debug),
              (void *) &ss_transaction_information[i], sizeof(ss_transaction_information_T));

      //ss_transaction_id_debug
      dump_ptr->ss_transaction_id_debug = i;
      break;
    }
  }

  //CNM_mmcc_sync_ind_debug
  dump_ptr->CNM_mmcc_sync_ind_debug = CNM_mmcc_sync_ind;

  //mm_connection_pending_debug
  memscpy((void *) &dump_ptr->mm_connection_pending_debug, sizeof(nas_cnm_CNM_request_id_T_debug),
          (void *) &mm_connection_pending, sizeof(cnm_request_id_T));

  //no_of_IEs_debug
  dump_ptr->no_of_IEs_debug = get_no_of_IEs(as_id); 

  //nas_cnm_IE_info_T_debug         cc_IE_info_debug; 


  //cc_progress_information_debug
  for (i=0; i < MAXNO_CC_TRANSACTIONS; i++)
  {
    if (cc_progress_information[i] != 0 )
    {
      dump_ptr->cc_progress_information_debug = cc_progress_information[i];
      break;
    }
 }

  //CNM_mo_cp_data_retransmit_counter_debug
  dump_ptr->CNM_mo_cp_data_retransmit_counter_debug = CNM_mo_cp_data_retransmit_counter;

  //cp_data_l2_ack_sim_debug
  dump_ptr->cp_data_l2_ack_sim_debug = cp_data_l2_ack;

  //cp_data_l2_ack_sim_debug
  dump_ptr->cp_data_l2_ack_sim_debug =  mm_link_reestablishing; 

  //mm_rel_req_sent_debug
  dump_ptr->mm_rel_req_sent_debug =  mm_rel_req_sent; 

#endif  /* FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE) */

  return sizeof( cnm_qsh_dump_data_s_type );
}

/*===========================================================================
Function: mn_populate_qsh_dump_data()

Description:
CNM task QSH dump data for for Dump collection action.

============================================================================*/
void cnm_populate_qsh_dump_data ( qsh_client_cb_params_s *param )
{
  qsh_client_action_done_s client_action_done;
  qsh_dump_tag_mask_t cnm_qsh_dump_tag_mask = param->action_params.dump_collect.dump_tag_mask;
  size_t  remaining_size = param->action_params.dump_collect.dump_iovec.size_bytes; 
  uint8   *dump_ptr = param->action_params.dump_collect.dump_iovec.addr; 
  size_t  size_written = 0;
  
  if ( dump_ptr != NULL )
  {
    if( QSH_DUMP_TAG_ENABLED(cnm_qsh_dump_tag_mask, CNM_QSH_DUMP_TAG_AS_ID_1) &&
        remaining_size >= sizeof(cnm_qsh_dump_data_s_type) )  
    {
      size_written = cnm_get_qsh_dump_data( (cnm_qsh_dump_data_s_type *)dump_ptr, CNM_QSH_DUMP_TAG_AS_ID_1 );
      remaining_size -= size_written;
      dump_ptr = dump_ptr + size_written;
    }

    if ( QSH_DUMP_TAG_ENABLED(cnm_qsh_dump_tag_mask, CNM_QSH_DUMP_TAG_AS_ID_2) &&
         remaining_size >= sizeof(cnm_qsh_dump_data_s_type) )
    {
      size_written += cnm_get_qsh_dump_data( (cnm_qsh_dump_data_s_type *)dump_ptr, CNM_QSH_DUMP_TAG_AS_ID_2 );
    }
  }

  qsh_client_action_done_init(&client_action_done);

  client_action_done.cb_params_ptr = param; 
  client_action_done.params.dump_collect.size_written_bytes = size_written;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
  qsh_client_action_done(&client_action_done);
}

#endif

/*===========================================================================
Function: mn_post_qsh_message_ind()

Description:
Post a message to own task on QSH call back
============================================================================*/
void cnm_post_qsh_message_ind( qsh_client_cb_params_s *param )
{
  mn_cnm_cmd_type   *cmd_ptr;

  /* Get command buffer from mn_cnm task */
  cmd_ptr = mn_cnm_get_cmd_buf();

  /* Construct message */
  cmd_ptr->cmd.qsh_msg_ind.msg_header.message_set = MS_MN_QSH;
  cmd_ptr->cmd.qsh_msg_ind.msg_header.message_id  = CNM_QSH_MSG_IND;

  PUT_IMH_LEN (sizeof (cmd_ptr->cmd.qsh_msg_ind) - sizeof (cmd_ptr->cmd.qsh_msg_ind.msg_header),
                    &(cmd_ptr->cmd.qsh_msg_ind.msg_header));

  cmd_ptr->cmd.qsh_msg_ind.param = *param;

  /* Put on MNCNM QSH command queue */
  mn_cnm_put_cmd(cmd_ptr);
}

void cnm_qsh_cb( qsh_client_cb_params_s  *param )
{
  if ( param != NULL )
  {
    switch ( param->action ) 
    {
#ifdef FEATURE_QSH_EVENT_METRIC
      case QSH_ACTION_METRIC_CFG:
          cnm_post_qsh_message_ind(param);
        break;
#endif
#ifdef FEATURE_QSH_DUMP
      case QSH_ACTION_DUMP_COLLECT:
         if ( param->action_mode == QSH_ACTION_MODE_SYNC_REQUIRED ) 
         {
           cnm_populate_qsh_dump_data(param);
         }
         else
         {
           cnm_post_qsh_message_ind(param);
         }
         break;
#endif
       default:
         break;
    }
  }
  else
  {
    MSG_ERROR_DS_0(MN_SUB,"=CNM= ERROR. NULL param pointer in cnm_qsh_cb");
  }
}

#ifdef FEATURE_QSH_EVENT_METRIC
void cnm_qsh_metric_log_done( void )
{
  qsh_client_metric_log_done_s client_metric_log_done;
  qsh_client_metric_log_done_init(&client_metric_log_done);
  client_metric_log_done.client = QSH_CLT_CNM;
  client_metric_log_done.metric_id = (qsh_metric_id_t)CNM_QSH_METRIC_SIGNALING;
  client_metric_log_done.metric_context_id = cnm_qsh_debug_metrics.signaling_metric_context_id;
  client_metric_log_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL;
  cnm_qsh_debug_metrics.ota_signaling_ptr = 
                  (cnm_qsh_metric_signaling_s*)qsh_client_metric_log_done(&client_metric_log_done);
}
#endif
#endif


















