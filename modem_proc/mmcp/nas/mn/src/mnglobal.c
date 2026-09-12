/*===========================================================================
            COPYRIGHT INFORMATION

Copyright (c) 2001 Qualcomm Technologies, Incorporated and its licensors.  All Rights 
Reserved.  QUALCOMM Proprietary.  Export of this technology or software 
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mn/vcs/mnglobal.c_v   1.5   17 Aug 2001 15:21:30   abanh  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/mn/src/mnglobal.c#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
7/19/01    CD      Initial version.  Extracted global data declarations from 
                   mn_initialize_mn_data.c

8/10/01     CD     Removed unnecessary queue.h

08/16/01    AB     Renamed MMI references to CM.

08/12/04    AB     Added support for the SMS link control feature. 

11/02/04    AB     Added user user data buffers & queue to support the
                     SS call related User-to-User Signaling feature.

02/24/05    AB     Added a new MN client, ATCOP.

11/09/05    AB      Added supports for FEATURE_REL5 via NV_FORCE_UE_MSCR_R99_I

09/14/06    NR     Adding changes for passing raw byte stream to CATAPPS 
 
05/25/11    AM     Replacing the value with macro

05/25/11    AM     Fixing the MOB compilation 

===========================================================================*/

/* Includes */

#include "mmcp_variation.h"
#include "customer.h"
#include "rex.h"
#include "timers_v.h"
#include "mn.h"
#include "mnglobal.h"
#include "qsh.h"
#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)
#include "mncnm_qsh.h"
#endif

#include "mn_cnmtask_v.h"
#include "mn_cnmtask_int.h"
#include "ms.h"
#include "cnm.h"
#include "nasutils.h"

extern void cnm_populate_qsh_dump_data( qsh_client_cb_params_s *param );
/* Globals */

uint32                           mn_asid_2_vsid_map[MAX_AS_IDS] = {0,0};

#if defined(FEATURE_DUAL_SIM) 

#if defined(FEATURE_DUAL_ACTIVE)
mn_dsda_as_id_e_type           mn_dsda_as_id;

/* DUAL SIM : AS INDEX position in Timer IDs*/
#define TIMER_ID_SIM_POS 6


#endif

sys_modem_as_id_e_type           mn_as_id;
#endif
byte                             mn_message_as_id = 1;


/*      sip_config_timer_f
 * This variable used to store the timer f value which is received from WMS when SMS is over IMS.
 * when the SMS is over IMS, NAS has to start Timer f instead of TR1M timer
 * If the SM-over-IP sender uses SMR entity timers as specified in 3GPP TS 24.011 [8], then TR1M is set to a value greater than Timer F (see 3GPP TS 24.229 [10])
 */

#ifdef FEATURE_DUAL_SIM
uint32                           sip_config_timer_f_sim[MAX_AS_IDS];
#else
uint32                           sip_config_timer_f;
#endif

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)

mn_call_information_T *           mn_call_information_p_sim[MAX_AS_IDS][MAXNO_CC_TRANSACTIONS];
#ifdef FEATURE_NAS_MN_CB_SERVICES
mn_cb_services_T                 mn_cb_services_sim[MAX_AS_IDS][MN_MAX_CLIENT];
#endif /* FEATURE_NAS_MN_CB_SERVICES */
/* Traffic Channel Configuration */
tch_user_connections_store_T     tch_user_connections_store_sim[MAX_AS_IDS][MAX_NUM_SPEECH_BEARERS];
boolean                          mn_waiting_for_tch_in_srvcc_sim[MAX_AS_IDS];

/* Global for SS and SS call related */
ss_invoke_data_T                 ss_invoke_data_sim[MAX_AS_IDS][MAXNO_INVOKE_IDS];
Multicall_Status_T               Multicall_Status_sim[MAX_AS_IDS][MULTICALL_STATUS_SIZE];

byte                             suppsvc_l3_data_sim[MAX_AS_IDS][MAX_SS_PARAMETER_LENGTH];
word                             suppsvc_l3_data_length_sim[MAX_AS_IDS];

#ifdef FEATURE_UUS
MN_q_T      mn_user_user_data_q_sim[MAX_AS_IDS][USER_USER_DATA_Q_ID+1];  /* No. MO connection ID + 1 user buffer q */
MN_qlink_T  mn_user_user_data_pool_sim[MAX_AS_IDS][MN_MAXNO_USER_USER_DATA+1];  /* q link of the buffer pool */
cm_mo_user_data_req_T   mn_user_user_data_buffers_sim[MAX_AS_IDS][MN_MAXNO_USER_USER_DATA+1];   /* user data buffer pool */
#endif

mn_alternate_procedure_status_T  mn_alternate_proc_status_sim[MAX_AS_IDS];

/* Globals for SMS */
rl_mo_mr_T                       rl_mo_mr_sim[MAX_AS_IDS];
sm_rl_transaction_information_T  sm_rl_transaction_information_sim[MAX_AS_IDS][MAXNO_SMS_TRANSACTIONS];
nas_msg_transport_type_e_type    mo_sms_transport_type_sim[MAX_AS_IDS];

mo_rpdu_store_T                  mo_rpdu_store_sim[MAX_AS_IDS];
sms_link_control_T               sms_link_control_sim[MAX_AS_IDS];
mo_rpdu_store_T                  mo_memory_available_rpdu_store_sim[MAX_AS_IDS];
#ifdef FEATURE_REL5
ue_force_rel_mode_type           mn_nv_force_mscr_r99_sim[MAX_AS_IDS];
#endif

/*        last_allocated_mo_sms_connection_id
 * This variable is used to track the last allocated connection_id for MO SMS 
 * by NAS. So, for next mo sms conn_id allocation, connection ids are iterated
 * from last_allocated_mo_sms_connection_id + 1 to last_allocated_mo_sms_connection_id .
 *  */
connection_id_T                   last_allocated_mo_sms_connection_id_sim[MAX_AS_IDS];

/*                  waiting_for_next_network_ussd_trigger 
 *
 *  Scenario:     For NW initiated USSD, after user gives a response, NW releases the transaction.
 *                For some OS, after user response is given for MT USSD, a USSD running menu is thrown
 *                on UI which gets cleared only if UI get a transaction release message from LL (NAS in this case).
 *                So, this variable tracks whether such a response to CM/and above is pending from NAS.
 *
 *  Purpose:      This variable indicates if following message from MN to CM is pending or not.
 *                CM_RELEASE_UNSTRUCTURED_SS_IND
 * 
 *  
 *  Setting:       MN makes it TRUE after getting a response for CM_UNSTRUCTURED_SS_IND
 *  Checking:      When MN receives END_IND, REJ_IND, ABORT_SS_REQ or RELEASE_SS_REQ
 *  Resetting:     MN resets this variable after sending CM_RELEASE_UNSTRUCTURED_SS_IND
 * 
*/
boolean waiting_for_next_network_ussd_trigger_sim[MAX_AS_IDS];


nas_msg_transport_type_e_type    mt_sms_transport_type_sim[MAX_AS_IDS];


/* Store the Destination Address of the MO SMS here */
cm_called_party_bcd_no_T         mo_sms_destination_addr_sim[MAX_AS_IDS];

#ifdef FEATURE_QSH_EVENT_METRIC
cnm_qsh_debug_metrics_s_type cnm_qsh_debug_metrics_sim[MAX_AS_IDS];
#define cnm_qsh_debug_metrics cnm_qsh_debug_metrics_sim[mn_dsda_as_id]
#endif

#else

mn_call_information_T *           mn_call_information_p[MAXNO_CC_TRANSACTIONS];
#ifdef FEATURE_NAS_MN_CB_SERVICES 
mn_cb_services_T                 mn_cb_services[MN_MAX_CLIENT];
#endif /* FEATURE_NAS_MN_CB_SERVICES */

/* Traffic Channel Configuration */
tch_user_connections_store_T     tch_user_connections_store[MAX_NUM_SPEECH_BEARERS];

/* mn_waiting_for_tch_in_srvcc - This variable indicates if SRVCC call context transfer 
 * happened from IMS to CS domain without SYNC_IND (HO indication reason) indication received
 * from RRC/RR */
boolean                          mn_waiting_for_tch_in_srvcc;

/* Global for SS and SS call related */
ss_invoke_data_T                 ss_invoke_data[MAXNO_INVOKE_IDS];
Multicall_Status_T               Multicall_Status[MULTICALL_STATUS_SIZE];

byte                             suppsvc_l3_data[MAX_SS_PARAMETER_LENGTH];
word                             suppsvc_l3_data_length;

#ifdef FEATURE_UUS
MN_q_T      mn_user_user_data_q[USER_USER_DATA_Q_ID+1];  /* No. MO connection ID + 1 user buffer q */
MN_qlink_T  mn_user_user_data_pool[MN_MAXNO_USER_USER_DATA+1];  /* q link of the buffer pool */
cm_mo_user_data_req_T   mn_user_user_data_buffers[MN_MAXNO_USER_USER_DATA+1];   /* user data buffer pool */
#endif


/* SS Alternate-Procedure related global variable. This stores whether an alternate procedure 
(2+Send Call Hold/Retrieve), has been initated. It also stores the status of procedure, since this 
requires the synchronization of HOLD Procedure and RETRIEVE Procedure.One event each of Set A and Set B below should 
occur for the alternate procedure to end. Same applies if one of the Calls is a MPTY Call.
A = {HOLD_CNF,HOLD_REJ,HLD_TIMER_EXPIRY}
B = {RETRIEVE_CNF,RETRIEVE_REJ,RETRIEVE_TIMER_EXPIRY}
*/
mn_alternate_procedure_status_T  mn_alternate_proc_status;

/* Globals for SMS */
rl_mo_mr_T                       rl_mo_mr;
sm_rl_transaction_information_T  sm_rl_transaction_information[MAXNO_SMS_TRANSACTIONS];
mo_rpdu_store_T                  mo_rpdu_store;
sms_link_control_T               sms_link_control;
mo_rpdu_store_T                  mo_memory_available_rpdu_store;
nas_msg_transport_type_e_type    mo_sms_transport_type;
nas_msg_transport_type_e_type    mt_sms_transport_type;

/*        last_allocated_mo_sms_connection_id
 * This variable is used to track the last allocated connection_id for MO SMS 
 * by NAS. So, for next mo sms conn_id allocation, connection ids are iterated
 * from last_allocated_mo_sms_connection_id + 1 to last_allocated_mo_sms_connection_id .
 *  */
connection_id_T                   last_allocated_mo_sms_connection_id;


/*                  waiting_for_next_network_ussd_trigger 
 *
 *  Scenario:     For NW initiated USSD, after user gives a response, NW releases the transaction.
 *                For some OS, after user response is given for MT USSD, a USSD running menu is thrown
 *                on UI which gets cleared only if UI get a transaction release message from LL (NAS in this case).
 *                So, this variable tracks whether such a response to CM/and above is pending from NAS.
 *
 *  Purpose:      This variable indicates if following message from MN to CM is pending or not.
 *                CM_RELEASE_UNSTRUCTURED_SS_IND
 * 
 *  
 *  Setting:       MN makes it TRUE after getting a response for CM_UNSTRUCTURED_SS_IND
 *  Checking:      When MN receives END_IND, REJ_IND, ABORT_SS_REQ or RELEASE_SS_REQ
 *  Resetting:     MN resets this variable after sending CM_RELEASE_UNSTRUCTURED_SS_IND
 * 
*/
boolean waiting_for_next_network_ussd_trigger;

/* Store the Destination Address of the MO SMS here */
cm_called_party_bcd_no_T         mo_sms_destination_addr;

#ifdef FEATURE_QSH_EVENT_METRIC
cnm_qsh_debug_metrics_s_type cnm_qsh_debug_metrics;
#endif

#endif

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP)
#ifdef FEATURE_QSH_DUMP
mn_qsh_dump_tag_as_id_1_s *mn_qsh_dump_tag_0_ptr;
mn_qsh_dump_tag_as_id_2_s *mn_qsh_dump_tag_1_ptr;

extern byte mn_als_itc_aux_speech;
extern byte mn_nv_no_auto_answer_on_hold;
extern byte mn_nv_mt_call_reject_cause;

size_t mn_get_qsh_dump_data
(
  mn_qsh_dump_data_s_type *dump_ptr, mn_qsh_dump_tag_e tag
)__attribute__((section(".uncompressible.text")));

void mn_populate_qsh_dump_data
(
  qsh_client_cb_params_s *param
)__attribute__((section(".uncompressible.text")));

void mn_post_qsh_message_ind
(
  qsh_client_cb_params_s *param
)__attribute__((section(".uncompressible.text")));

void mn_process_qsh_message_ind
(
  IMH_T *msg_header 
)__attribute__((section(".uncompressible.text")));

void mn_qsh_cb
(
  qsh_client_cb_params_s  *param
)__attribute__((section(".uncompressible.text")));


/*===========================================================================
Function: mn_get_qsh_dump_data()

Description:
QSH dump data for MN dump collection action.

============================================================================*/
size_t mn_get_qsh_dump_data( mn_qsh_dump_data_s_type *dump_ptr, mn_qsh_dump_tag_e tag )
{
  int i;
  mn_dsda_as_id_e_type as_id = MN_AS_ID_1;
  if(tag == MN_QSH_DUMP_TAG_AS_ID_2)
  {
    as_id = MN_AS_ID_2;
  }

  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_ptr,(uint16)tag, sizeof(mn_qsh_dump_data_s_type));

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)

  //Multicall_Status_debug
  for ( i = 0; i < MULTICALL_STATUS_SIZE; i++)
  {
    if ( Multicall_Status_sim[as_id][i].call_activity != CALL_INACTIVE ) 
    {
        memscpy((void *) &dump_ptr->mn_debug_data.Multicall_Status_debug, sizeof(mn_Multicall_Status_debug_T),
                (void *) &Multicall_Status_sim[as_id][i], sizeof(Multicall_Status_T));
        break;
    }
  }

  //last_allocated_mo_sms_connection_id_debug
  dump_ptr->mn_debug_data.last_allocated_mo_sms_connection_id_debug = (connection_id_debug_T)last_allocated_mo_sms_connection_id_sim[as_id];

  //mn_alternate_proc_status_debug
  memscpy((void *) &dump_ptr->mn_debug_data.mn_alternate_proc_status_debug, sizeof(mn_alternate_procedure_status_debug_T),
                (void *) &mn_alternate_proc_status_sim[as_id], sizeof(mn_alternate_procedure_status_T));

  //mn_as_id_debug
  dump_ptr->mn_debug_data.mn_as_id_debug                             =  mn_as_id; 

  //mn_asid_2_vsid_map_debug
  dump_ptr->mn_debug_data.mn_asid_2_vsid_map_debug                   =  mn_asid_2_vsid_map[as_id];

  //mn_call_information_debug
  for ( i=0; i<MAXNO_CC_TRANSACTIONS; i++ )
  {
    if ( mn_call_information_p_sim[as_id][i] != NULL )
    {
      dump_ptr->mn_debug_data.mn_call_information_debug.connection_id = i;
      memscpy((void *) &dump_ptr->mn_debug_data.mn_call_information_debug, sizeof(mn_call_information_debug_T),
              (void *) mn_call_information_p_sim[as_id][i], sizeof(mn_call_information_T));
      break;
    }
  }

  //mn_dsda_as_id_debug
  dump_ptr->mn_debug_data.mn_dsda_as_id_debug                        = (mn_dsda_as_id_debug_e_type)mn_dsda_as_id;

  //mn_waiting_for_tch_in_srvcc_debug
  dump_ptr->mn_debug_data.mn_waiting_for_tch_in_srvcc_debug          =  mn_waiting_for_tch_in_srvcc_sim[as_id];

  //mo_sms_transport_type_debug
  dump_ptr->mn_debug_data.mo_sms_transport_type_debug                =  mo_sms_transport_type_sim[as_id];

  //sm_rl_transaction_information
  for (i = 0; i < MAXNO_SMS_TRANSACTIONS; i++)
   {
     if ( sm_rl_transaction_information_sim[as_id][i].rl_state != IDLE )
     { 
       dump_ptr->mn_debug_data.sm_rl_transaction_information_debug  = sm_rl_transaction_information_sim[as_id][i]; 
       break;
     }
   }

  //sms_link_control_debug
  memscpy((void *) &dump_ptr->mn_debug_data.sms_link_control_debug, sizeof(mn_sms_link_control_debug_T),
                (void *) &sms_link_control_sim[as_id], sizeof(sms_link_control_T));

  //ss_invoke_data_debug
  for (i=0;i<MAXNO_INVOKE_IDS;i++)
  {
    if ( ss_invoke_data_sim[as_id][i].connection_id != UNKNOWN_CONNECTION_ID ) 
    {
      memscpy((void *) &dump_ptr->mn_debug_data.ss_invoke_data_debug, sizeof(mn_ss_invoke_data_debug_T),
                    (void *) &ss_invoke_data_sim[as_id][i], sizeof(ss_invoke_data_T));
       break;
    }
  }

  //suppsvc_l3_data_length_debug
  dump_ptr->mn_debug_data.suppsvc_l3_data_length_debug = suppsvc_l3_data_length_sim[as_id];

  //tch_user_connections_store_debug   
  memscpy((void *) &dump_ptr->mn_debug_data.tch_user_connections_store_debug, sizeof(mn_tch_user_connections_store_debug_T),
          (void *) &tch_user_connections_store_sim[as_id][DEFAULT_RAB_ID], sizeof(tch_user_connections_store_T));

  //waiting_for_next_network_ussd_trigger_debug
  dump_ptr->mn_debug_data.waiting_for_next_network_ussd_trigger_debug = waiting_for_next_network_ussd_trigger_sim[as_id];

  //mn_als_itc_aux_speech_debug
  dump_ptr->mn_debug_data.mn_als_itc_aux_speech_debug = mn_als_itc_aux_speech;

  //mn_nv_mt_call_reject_cause_debug
  dump_ptr->mn_debug_data.mn_nv_mt_call_reject_cause_debug = mn_nv_mt_call_reject_cause;

  //mn_nv_no_auto_answer_on_hold_debug
  dump_ptr->mn_debug_data.mn_nv_no_auto_answer_on_hold_debug = mn_nv_no_auto_answer_on_hold;

  //mn_rab_status_debug
  dump_ptr->mn_debug_data.mn_rab_status_debug = mn_rab_status_get(as_id);

#ifdef FEATURE_NAS_VOC_REL_AFTER_CALL_HOLD
  //MN_turned_on_vocoder_debug
  dump_ptr->mn_debug_data.MN_turned_on_vocoder_debug = MN_turned_on_vocoder_get(as_id);
#endif
#else  /* (FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE) */

 
  //Multicall_Status_debug
  for ( i = 0; i < MULTICALL_STATUS_SIZE; i++)
  {
    if ( Multicall_Status[i].call_activity != CALL_INACTIVE ) 
    {
        memscpy((void *) &dump_ptr->mn_debug_data.Multicall_Status_debug, sizeof(mn_Multicall_Status_debug_T),
                (void *) &Multicall_Status[i], sizeof(Multicall_Status_T));
        break;
    }
  }

  //last_allocated_mo_sms_connection_id_debug
  dump_ptr->mn_debug_data.last_allocated_mo_sms_connection_id_debug = (connection_id_debug_T)last_allocated_mo_sms_connection_id;

  //mn_alternate_proc_status_debug
  memscpy((void *) &dump_ptr->mn_debug_data.mn_alternate_proc_status_debug, sizeof(mn_alternate_procedure_status_debug_T),
                (void *) &mn_alternate_proc_status, sizeof(mn_alternate_procedure_status_T));

  //mn_as_id_debug
#if defined(FEATURE_DUAL_SIM) 
  dump_ptr->mn_debug_data.mn_as_id_debug                             =  mn_as_id; 
#else
  dump_ptr->mn_debug_data.mn_as_id_debug                           =  SYS_MODEM_AS_ID_1; 
#endif

  //mn_asid_2_vsid_map_debug
  dump_ptr->mn_debug_data.mn_asid_2_vsid_map_debug                   =  mn_asid_2_vsid_map[as_id];

  //mn_call_information_debug
  for ( i=0; i<MAXNO_CC_TRANSACTIONS; i++ )
  {
    if ( mn_call_information_p[i] != NULL )
    {
      dump_ptr->mn_debug_data.mn_call_information_debug.connection_id = i;
      memscpy((void *) &dump_ptr->mn_debug_data.mn_call_information_debug, sizeof(mn_call_information_debug_T),
              (void *) mn_call_information_p[i], sizeof(mn_call_information_T));
      break;
    }
  }

  //mn_dsda_as_id_debug
  dump_ptr->mn_debug_data.mn_dsda_as_id_debug                        = 0;

  //mn_waiting_for_tch_in_srvcc_debug
  dump_ptr->mn_debug_data.mn_waiting_for_tch_in_srvcc_debug          =  mn_waiting_for_tch_in_srvcc;

  //mo_sms_transport_type_debug
  dump_ptr->mn_debug_data.mo_sms_transport_type_debug                =  mo_sms_transport_type;             

  //sm_rl_transaction_information
  for (i = 0; i < MAXNO_SMS_TRANSACTIONS; i++)
   {
     if ( sm_rl_transaction_information[i].rl_state != IDLE )
     { 
       dump_ptr->mn_debug_data.sm_rl_transaction_information_debug  = sm_rl_transaction_information[i]; 
       break;
     }
   }

  //sms_link_control_debug
  memscpy((void *) &dump_ptr->mn_debug_data.sms_link_control_debug, sizeof(mn_sms_link_control_debug_T),
                (void *) &sms_link_control, sizeof(sms_link_control_T));

  //ss_invoke_data_debug
  for (i=0;i<MAXNO_INVOKE_IDS;i++)
  {
    if ( ss_invoke_data[i].connection_id != UNKNOWN_CONNECTION_ID ) 
    {
      memscpy((void *) &dump_ptr->mn_debug_data.ss_invoke_data_debug, sizeof(mn_ss_invoke_data_debug_T),
                    (void *) &ss_invoke_data[i], sizeof(ss_invoke_data_T));
       break;
    }
  }

  //suppsvc_l3_data_length_debug
  dump_ptr->mn_debug_data.suppsvc_l3_data_length_debug = suppsvc_l3_data_length;

  //tch_user_connections_store_debug   
  memscpy((void *) &dump_ptr->mn_debug_data.tch_user_connections_store_debug, sizeof(mn_tch_user_connections_store_debug_T),
          (void *) &tch_user_connections_store[DEFAULT_RAB_ID], sizeof(tch_user_connections_store_T));

  //waiting_for_next_network_ussd_trigger_debug
  dump_ptr->mn_debug_data.waiting_for_next_network_ussd_trigger_debug = waiting_for_next_network_ussd_trigger;
  
  //mn_als_itc_aux_speech_debug
  dump_ptr->mn_debug_data.mn_als_itc_aux_speech_debug = mn_als_itc_aux_speech;

  //mn_nv_mt_call_reject_cause_debug
  dump_ptr->mn_debug_data.mn_nv_mt_call_reject_cause_debug = mn_nv_mt_call_reject_cause;

  //mn_nv_no_auto_answer_on_hold_debug
  dump_ptr->mn_debug_data.mn_nv_no_auto_answer_on_hold_debug = mn_nv_no_auto_answer_on_hold;

  //mn_rab_status_debug
  dump_ptr->mn_debug_data.mn_rab_status_debug = mn_rab_status_get( as_id );

#ifdef FEATURE_NAS_VOC_REL_AFTER_CALL_HOLD
  //MN_turned_on_vocoder_debug
  dump_ptr->mn_debug_data.MN_turned_on_vocoder_debug = MN_turned_on_vocoder_get( as_id );
#endif
#endif

  return sizeof( mn_qsh_dump_data_s_type );
}

/*===========================================================================
Function: mn_populate_qsh_dump_data()

Description:
QSH dump data for MN dump collection action.

============================================================================*/
void mn_populate_qsh_dump_data( qsh_client_cb_params_s *param )
{
  qsh_client_action_done_s client_action_done;
  qsh_dump_tag_mask_t mn_qsh_dump_tag_mask = param->action_params.dump_collect.dump_tag_mask;
  size_t  remaining_size = param->action_params.dump_collect.dump_iovec.size_bytes; 
  size_t  size_written = 0;
  uint8   *dump_ptr = param->action_params.dump_collect.dump_iovec.addr; 

  if ( dump_ptr != NULL ) 
  {
    if( QSH_DUMP_TAG_ENABLED(mn_qsh_dump_tag_mask, MN_QSH_DUMP_TAG_AS_ID_1) &&
        remaining_size >= sizeof(mn_qsh_dump_data_s_type) )
    {
      size_written = mn_get_qsh_dump_data( (mn_qsh_dump_data_s_type *)dump_ptr, MN_QSH_DUMP_TAG_AS_ID_1 );
      dump_ptr = dump_ptr + size_written;
      remaining_size -= size_written;
    }

    if( QSH_DUMP_TAG_ENABLED(mn_qsh_dump_tag_mask, MN_QSH_DUMP_TAG_AS_ID_2) &&
        remaining_size >= sizeof(mn_qsh_dump_data_s_type) )   
    {
      size_written += mn_get_qsh_dump_data( (mn_qsh_dump_data_s_type *)dump_ptr, MN_QSH_DUMP_TAG_AS_ID_2 );
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
void mn_post_qsh_message_ind(qsh_client_cb_params_s *param)
{
  mn_cnm_cmd_type   *cmd_ptr;

  /* Get command buffer from mn_cnm task */
  if ( (cmd_ptr = mn_cnm_get_cmd_buf()) != NULL )
  {
  /* Construct message */
  cmd_ptr->cmd.qsh_msg_ind.msg_header.message_set = MS_MN_QSH;
  cmd_ptr->cmd.qsh_msg_ind.msg_header.message_id  = MN_QSH_MSG_IND;

  PUT_IMH_LEN (sizeof (cmd_ptr->cmd.qsh_msg_ind) - sizeof (cmd_ptr->cmd.qsh_msg_ind.msg_header),
                    &(cmd_ptr->cmd.qsh_msg_ind.msg_header));

    cmd_ptr->cmd.qsh_msg_ind.param = *param;

  /* Put on MNCNM QSH command queue */
  mn_cnm_put_cmd(cmd_ptr);
  }
}

void mn_process_qsh_message_ind( IMH_T *msg_header )
{
  qsh_client_cb_params_s  *param = &(((mncnm_qsh_msg_T *)msg_header)->param);
  qsh_client_action_done_s client_action_done;
  sys_modem_as_id_e_type   subs_id;

  cnm_qsh_metric_e metric_id = (cnm_qsh_metric_e)param->action_params.metric_cfg.id;
  subs_id = param->action_params.metric_cfg.subs_id;
  
  switch ( metric_id ) 
  {
    case CNM_QSH_METRIC_SIGNALING:
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
      if ( (subs_id >= SYS_MODEM_AS_ID_1) && (subs_id <= SYS_MODEM_AS_ID_2) ) 
      {
        cnm_qsh_debug_metrics_sim[subs_id].signaling_metric_context_id = 
             param->action_params.metric_cfg.metric_context_id;

        if(param->action_params.metric_cfg.action == QSH_METRIC_ACTION_START)
      {
          cnm_qsh_debug_metrics_sim[subs_id].ota_signaling_ptr = 
             (cnm_qsh_metric_signaling_s *)param->action_params.metric_cfg.start_addr;
        }
        else
        {
          cnm_qsh_debug_metrics_sim[subs_id].ota_signaling_ptr = NULL;
      }
      }
#else
      cnm_qsh_debug_metrics.signaling_metric_context_id = 
           param->action_params.metric_cfg.metric_context_id;

      if(param->action_params.metric_cfg.action == QSH_METRIC_ACTION_START)
      {
        cnm_qsh_debug_metrics.ota_signaling_ptr = 
           (cnm_qsh_metric_signaling_s *)param->action_params.metric_cfg.start_addr;
      }
      else
      {
        cnm_qsh_debug_metrics.ota_signaling_ptr = NULL;
      }
#endif
      break;

    default:
      break;
  }

  qsh_client_action_done_init(&client_action_done);
  client_action_done.cb_params_ptr = param;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
  qsh_client_action_done(&client_action_done);
}               

void mn_qsh_cb( qsh_client_cb_params_s  *param )
{
  if ( param != NULL )
  {
    switch ( param->action ) 
    {
#ifdef FEATURE_QSH_EVENT_METRIC
    case QSH_ACTION_METRIC_CFG:
      /* TBD */
      break;
#endif
#ifdef FEATURE_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
      if ( param->action_mode == QSH_ACTION_MODE_SYNC_REQUIRED ) 
      {
        mn_populate_qsh_dump_data(param);
      }
      else 
      {
        mn_post_qsh_message_ind(param);
      }
      break;
#endif
    default:
      break;
    }
  }
  else
  {
    MSG_ERROR_DS_0(MN_SUB,"=MN= ERROR. NULL param pointer in mn_qsh_cb");
  }
}
#endif











