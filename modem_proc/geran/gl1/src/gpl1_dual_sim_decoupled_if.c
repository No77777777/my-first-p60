/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     L 1 DUAL SIM MODE FIND BCCH STATE PROCESSING

GENERAL DESCRIPTION


EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dual_sim_decoupled_if.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when        who      what, where, why
--------   --------  ----------------------------------------------------------
15/06/18   ng       CR2261488 Remove 'l1_rcvd_non_drx_ind' related handling, it is no more needed
25/05/17   hd       CR2051434 Make the variable l1_rcvd_non_drx_ind dual spaced
02/01/17   sk       CR2001022  Delay RANE for RACH till we get QTA Cleanup
23/11/16   bg       CR1089997 Fix for KW errors
17/11/16  akm       CR1090724 JO.3.0 P1 compilation issues due to new KW version
18/11/15   cah     CR900214 - [WLAN+WAN] Failed in acquiring the Rxd chain in Single SIM Mode during Voice Call Attempt
15/10/15   ap       CR923459 GSM logging procedure for Attach seen as CS instead of PS registration
10/09/15   sk       CR899847 GERAN changes to trigger QSH events and handler for debugging
02/09/15   nm       CR897078 Change the reason from Acces to TRAFFIC if RACH for CS is received
16/09/15   cjl      CR908527 improve system procedures used for TRM access
21/07/15   cjl      CR876903 Use system procedure types for TRM subreasons 
21/07/15   cjl      CR876741 Replace TRM subreasons with system procedure types
14/07/15   cjl      CR872092 Fix uninitialised pointer
17/06/15   cjl      CR853555 Use dynamic connected mode reasons for CS/PS traffic
10/06/15   sjw      CR850737: variable connection priority for TRM-Unification
30/04/15   ap       CR811057 FR22272 - TRM Unification  - Logging Changes
21/04/15   sjw      CR822898: removal of deprecated ACQUISITION reasons
30/03/15   ap       CR811753 FR22272 - TRM Unification - GL1 changes to support
                                        SysInfo priority inversion during early camping
30/03/15   nm       CR765891 Allowing TA for TDS during PS RACH
24/03/15   ap       CR811053:FR22272 - TRM Unification  - Access and Traffic attempts
24/03/15   sjw      CR812407: update for new MPH_SET_PRIORITY_IND payload
16/03/15   mko      CR803066 In MPH_ABORT_RA_REQ msg  handling, add wait_for_next_tick to avoid FW LOAD & UNLOAD in back to back frames
12/03/15   pa       CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
26/02/15   ak       CR800702 Correct the check for trm status to not allow rach in GTA gap
19/02/15   nk       CR787044 Dont allow RACH processing in GTA gap
17/02/15   nk       CR787185 Use pointer to l1_tsk_buffer i.e. l1_tskisr_blk when trying to access qta_in_progress flag.
17/02/15   cah      CR777305 Replace depricated TRM type TRM_DENIED with TRM_DENIAL
11/02/15   ap       CR781758:Handling race condition when receiving Start Idle Req in Decoupled State
21/01/15   pa       CR758792:RxD support for multi-sim.
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
04/12/14   ap       CR766527 Handle MPH_WCDMA_CELL_UPDATE_LIST_REQ in Decoupled RACH state
21/10/14   ap       CR743519 Add Handling of CBCH in L1 Decoupled State.
12/03/14   js       CR761902 Ensuring TRM lock should not be retained for access in QTA gap.
25/11/14   ap       CR761941 Handle MPH_UE_MODE_CHANGE_REQ in Decoupled RACH state
07/10/14   ap       CR735010 Ensure GL1 state change is handled correctly when leaving decoupled idle mode
01/10/14   dv       CR728756 Regression of CR728756 for SGLTE mode
01/10/14   ap       CR731657 Handling all race condition that occur due to receiving RA abort followed by GRR msg
01/10/14   js       CR731597 Removing call in task trm release TRM while aborting RACH from dual sim decoupled.
03/10/14   ap       CR730903 Handling race conditions of receiving RA req following RA abort and select specific BCCH req in decoupled state
23/09/14   dv       CR728756 Don't incorrectly register IFM band for RACH
11/09/14   pa     CR723113 Handling of MPH_SET_PRIORITY_IND/MPH_STOP_MM_NON_DRX_IND.
02/09/14   ap       CR717210 Fix compilation warning
26/08/14   mc       CR635105 Enforce minimum RACH delay of 1 frame to allow COEX registrations to complete
05/08/14   ap       CR702609 When MMS initiated need to simulate CS call TRM flow
05/08/14   ap       CR704010 Handle stop gsm mode in decoupled state
02/07/14   ap       Start
===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "l1_isr.h"
#include "l1i.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "l1_sc.h"
#include "l1_smscb.h"
#include "l1_drx.h"
#include "l1_utils.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_task.h"
#include "geran_dual_sim_g.h"
#include "gpl1_dual_sim.h"
#include "gpl1_gprs_utils.h"


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif 

rach_type_t g_rach_type[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( NO_RACH ) };

/*=======================EXTERNAL DATA=========================*/
extern boolean re_calculate_initial_rach[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION  gpl1_gprs_handle_multi_sim_decoupled

DESCRIPTION
  This function handles the L1 Task in dual decoupled mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_multi_sim_decoupled(IMH_T *msg_header)
{
   gas_id_t gas_id;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;


   switch (msg_header->message_set)
   {
      case MS_L2_L1:
        {

          switch (msg_header->message_id)
          {
            /********************************************************
            *                                                      *
            *                PH_RANDOM_ACCESS_IND                  *
            *                                                      *
            ********************************************************/
          case PH_RANDOM_ACCESS_IND:
            {
              ph_random_access_ind_T *message_ptr = (ph_random_access_ind_T *)msg_header;
              gas_id = check_gas_id(message_ptr->gas_id);
              l1_tskisr_blk = &l1_tsk_buffer[gas_id];
              if (message_ptr->rach_for_ps_traffic)
              {
                MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for PS");
              } else
              {
                MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for CS");
              }
              GL1_ISR_LOCK(gas_id);
              /* mark the interface as READY_TO_SEND */
              l1_tskisr_blk->rach_ready_to_send = TRUE;
              GL1_ISR_UNLOCK(gas_id);
              L1_send_PH_READY_TO_SEND_IND(L2_RACH, message_ptr->gas_id);
              break;
            }
            /********************************************************
            *                                                      *
            *                PH_RANDOM_ACCESS_REQ                  *
            *                                                      *
            ********************************************************/
          case PH_RANDOM_ACCESS_REQ:
            {
              ph_random_access_req_T   *message_ptr;
              idle_data_T               *l1_idle_data;
              volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
              gas_id_t                  ptm_gas_id;
              message_ptr = (ph_random_access_req_T *)msg_header; /*lint !e740 */
              gas_id = check_gas_id(message_ptr->gas_id);

              l1_idle_data = &l1_idle_data_store[gas_id];
              l1_tskisr_blk = &l1_tsk_buffer[gas_id];

              GL1_ISR_LOCK(gas_id);
              gl1_set_rach_type(message_ptr->rach_for_ps_traffic ? PS_RACH : CS_RACH, gas_id);
              
              /* check whether READY_TO_SEND has been flagged to GL2 */
              /* because GRR may have cancelled this RACH since then */
              if (FALSE == l1_tskisr_blk->rach_ready_to_send)
              {
                GL1_ISR_UNLOCK(gas_id);
                MSG_GERAN_ERROR_0_G("RACH READY_TO_SEND has been revoked");
                return;
              }


              /*Check to see whether we need to calculate RACH delay(PS HO enhancement)*/
              if (message_ptr->l2_channel_type == L2_RACH)
              {
                /*
                * Add 1 to the value returned as the return value corresponds to
                * the number of RACH slots NOT used, so the delay to the first is
                * the return value plus 1
                */

                l1_idle_data->random_access_data.ra_delay =
                  L1_calculate_RA_first_delay(l1_idle_data->campedon_cell_data.TX_integer);

#ifdef FEATURE_GSM_COEX
                /* This is to ensure a minimum of 1 frame delay to allow COEX registrations to complete */
                if(gl1_msg_get_multi_sim_mode() && (l1_idle_data->random_access_data.ra_delay==0))
                {
                  l1_idle_data->random_access_data.ra_delay = 1;
                }
#endif /* FEATURE_GSM_COEX */

              }
              else
              {
#ifdef FEATURE_GSM_COEX
                /* This is to ensure a minimum of 1 frame delay to allow COEX registrations to complete */
                if(gl1_msg_get_multi_sim_mode())
                {
                  l1_idle_data->random_access_data.ra_delay = 1;
                }
                else
#endif /* FEATURE_GSM_COEX */
                {
                  l1_idle_data->random_access_data.ra_delay = 0;
                }
                MSG_GERAN_MED_2_G("Fast RACH requested - zero delay used FN=%d, chan_type = %d",
                  GSTMR_GET_FN_GERAN( message_ptr->gas_id),message_ptr->l2_channel_type);
              }

              /* Map txlev to power index before passing to msg layer */
              l1_idle_data->random_access_data.txlev =
                l1_idle_data->campedon_cell_data.MS_TXPWR_MAX_CCH;

              /* Extract RACH content to transmit on access burst */
              l1_idle_data->random_access_data.ra_content =
                message_ptr->random_access_content;

              /* Extract RACH power reduction info */
              l1_idle_data->random_access_data.pwr_reduction_valid = message_ptr->pwr_reduction_valid;
              l1_idle_data->random_access_data.pwr_reduction_value = message_ptr->pwr_reduction_value;
              l1_idle_data->random_access_data.emergency_call = message_ptr->emergency_call;
              /* Check if this is an EGPRS packet channel request */
              if (message_ptr->egprs_ra_content_present)
              {
                /* Extract RACH content and TSC to transmit on 11 bit access burst */
                l1_idle_data->random_access_data.egprs_ra = TRUE;
                l1_idle_data->random_access_data.egprs_tsc = message_ptr->egprs_tsc;
                l1_idle_data->random_access_data.ra_content =
                  message_ptr->egprs_ra_content;
              } else
              {
                l1_idle_data->random_access_data.egprs_ra = FALSE;
              }
#ifdef FEATURE_GTA_DURING_PS_RACH
              /*populate the rach for ps service flag from the message ptr*/
              l1_idle_data->random_access_data.rach_for_ps_service = message_ptr->rach_for_ps_traffic;
              l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;

              MSG_GERAN_MED_2_G("GTA_PS_RACH : rach_for_ps_service %d delay %d", l1_idle_data->random_access_data.rach_for_ps_service,
                l1_idle_data->random_access_data.ra_delay);

              l1_idle_data->random_access_data.updated_ra_delay = l1_idle_data->random_access_data.ra_delay;

#endif /*FEATURE_GTA_DURING_PS_RACH*/
              l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;

              l1_tskisr_blk->main_command = L1_ACCESS_COMMAND;

              GL1_ISR_UNLOCK(gas_id);
              break;
            }

          default:
            {
              MSG_ERROR("L1(Decoupled) - Unknown message id.%d", msg_header->message_id,0,0);
              break;
            }
          }
          break;
        }
      case MS_RR_L1:
        {
          switch (msg_header->message_id)
          {
             case MPH_START_GPRS_IDLE_MODE_REQ:
               {
                 mph_start_gprs_idle_mode_req_t *message_ptr;
                 message_ptr = (mph_start_gprs_idle_mode_req_t *)msg_header;

                 gas_id = check_gas_id(message_ptr->gas_id);

                 GL1_ISR_LOCK(gas_id);
                 l1_rach_timer_deactivate(gas_id);
                 gl1_handle_mph_start_gprs_idle_mode_req(message_ptr);
                 gpl1_gprs_setup_start_idle((void *)0, gas_id);
                 GL1_ISR_UNLOCK(gas_id);
                 gl1_drx_require_and_wait_for_next_tick(gas_id);
                 GL1_ISR_LOCK(gas_id);
                 GL1_ISR_UNLOCK(gas_id);
                 break;
               }

             case MPH_ABORT_RA_REQ:
               {
                 mph_abort_ra_req_T  *message_ptr;
                 message_ptr = ( mph_abort_ra_req_T *)msg_header;
                 gas_id = check_gas_id(message_ptr->gas_id);
                 l1_tskisr_blk = &l1_tsk_buffer[gas_id];

                 gl1_drx_require_and_wait_for_next_tick(gas_id);

                 GL1_ISR_LOCK(gas_id);
                 gl1_set_rach_type(NO_RACH, gas_id);

                 l1_rach_timer_deactivate(gas_id);
                 /*
                  * Stop any pending RA bursts and revoke READY_TO_SEND
                  */
                 l1_tskisr_blk->rach_ready_to_send = FALSE;

                 re_calculate_initial_rach[gas_id] = TRUE;
                 /* In case of race condition. making TRM status to GRANTED
                  * so that while aborting RACH TRM lock is released from l1_isr_random_access_mode
                  */
                 if(GL1_TRM_RETAINED_FOR_ACCESS == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id))
                 {
                   grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);
                 }

                 switch(l1_tskisr_blk->l1_prev_state)
                 {
                    case L1_MULTI_SIM_FIND_BCCH:
                      L1_send_MPH_ABORT_RA_CNF(gas_id, GL1_IN_NULL_MODE);
                      break;
                    case L1_MULTI_SIM_IDLE:
                      l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
                      L1_send_MPH_ABORT_RA_CNF(gas_id, GL1_IN_DECOUPLED_MODE);
                      break;
                    default:
                       MSG_GERAN_HIGH_1_G("Unknown state %d", l1_tskisr_blk->l1_prev_state);
                       break;
                 }
                 GL1_ISR_UNLOCK(gas_id);
                 gl1_drx_require_and_wait_for_next_tick(gas_id);
                 GL1_ISR_LOCK(gas_id);
                 GL1_ISR_UNLOCK(gas_id);
                 break;
               }
             case MPH_START_IDLE_REQ:
               {
                 mph_start_idle_req_T *message_ptr = (mph_start_idle_req_T *)msg_header;

                 gas_id = check_gas_id(message_ptr->gas_id);
                 l1_tskisr_blk = &l1_tsk_buffer[gas_id];
                 GL1_ISR_LOCK(gas_id);

                 l1_rach_timer_deactivate(gas_id);
                 L1_setup_START_IDLE(msg_header);

                 /* Change the ISR mode */
                 l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

                 (void)l1_log_paging_data(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                 l1_clear_global_override_trm_reason(gas_id);
#endif

                 GL1_ISR_UNLOCK(gas_id);
                 break;
               }
             case MPH_STOP_GSM_MODE_REQ:
               {
                 mph_stop_gsm_mode_req_T *message_ptr;
                 message_ptr = (mph_stop_gsm_mode_req_T *)msg_header;

                 gas_id = message_ptr->gas_id;
                 GL1_ISR_LOCK(gas_id);

                 l1_rach_timer_deactivate(gas_id);

                 /* Stop L1 ISR */
                 L1_setup_DEACTIVATE(msg_header);
                 GL1_ISR_UNLOCK(gas_id);

                 break;
               }
              /********************************************************
               *                                                      *
               *                MPH_STOP_MM_NON_DRX_IND:              *
               *                                                      *
               ********************************************************/
             case MPH_STOP_MM_NON_DRX_IND:
               {
                 mph_stop_mm_non_drx_ind_t* message_ptr = ( mph_stop_mm_non_drx_ind_t* )msg_header;
                 idle_data_T               *l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];

                 gas_id = message_ptr->gas_id;
                 MSG_GERAN_MED_0_G("MPH_STOP_MM_NON_DRX_IND");
                 /* Disable the MM non-drx mode period */
                 l1_idle_data->mm_non_drx_mode = FALSE;
                 l1_idle_data->non_updated_lai_in_nmo1 = FALSE;
                 
                 break;
               }
              /*******************************************************
               *                                                      *
               *                MPH_SET_PRIORITY_IND                  *
               *                                                      *
               ********************************************************/
             case MPH_SET_PRIORITY_IND:
               {
                 mph_set_priority_ind_T* message_ptr = (mph_set_priority_ind_T* )msg_header;
                 gas_id = check_gas_id(message_ptr->gas_id);
                 l1_tskisr_blk = &l1_tsk_buffer[gas_id];

                 if (message_ptr->procedure_type == GERAN_CONN_PROCEDURE)
                 {
                   grm_check_connected_reason(l1_tskisr_blk->client_id,GRM_CS_CONNECTION,gas_id);
                 }
                 else if (message_ptr->procedure_type == GERAN_GPRS_PROCEDURE)
                 {
                   grm_check_connected_reason(l1_tskisr_blk->client_id,GRM_PS_CONNECTION,gas_id);
                 }
                 break;
               }
              /********************************************************
              *                                                       *
              *                MPH_UE_MODE_CHANGE_REQ                 *
              *                                                       *
              ********************************************************/
             case MPH_UE_MODE_CHANGE_REQ:
             {
               mph_ue_mode_change_req_T  *message_ptr;
               message_ptr = ( mph_ue_mode_change_req_T *) msg_header; /*lint !e740 unusual pointer cast */
               gas_id = check_gas_id(message_ptr->gas_id);
#ifdef FEATURE_SGLTE
               /* Set SGLTE mode */
               gl1_hw_set_sglte_mode(message_ptr->sglte_mode_active, gas_id);
#endif
               l1_sc_wcdma_set_mode(message_ptr->gsm_only,gas_id);
               break;
             }
             /********************************************************
             *                                                       *
             *                MPH_WCDMA_CELL_UPDATE_LIST_REQ         *
             *                                                       *
             ********************************************************/

             case MPH_WCDMA_CELL_UPDATE_LIST_REQ:
             {
               mph_wcdma_cell_update_list_req_T* msg;
               msg = ( mph_wcdma_cell_update_list_req_T* )msg_header;
               l1_sc_set_wcdma_list_update_inprogress( TRUE , msg->gas_id );
               l1_sc_wcdma_update_list( msg, msg->gas_id );
               l1_sc_set_wcdma_list_update_inprogress( FALSE , msg->gas_id );
               break;
             }
             default:
               {
                 MSG_ERROR("L1(Decoupled) - Unknown message id.%d", msg_header->message_id,0,0);
                 break;
               }

          }
          break;
        }
      case MS_CB_L1:
        {
          switch (msg_header->message_id)
          {
             case CB_SCHEDULING_REQ:
               {
                 cb_scheduling_req_type *message_ptr;
                 message_ptr = (cb_scheduling_req_type *)msg_header;

                 l1_smscb_process_scheduling_info
                    (message_ptr->channel_ind,
                     message_ptr->scheduling_mode,
                     message_ptr->scheduling_descriptor.scheduling_period_fn_list,
                     message_ptr->scheduling_descriptor.scheduling_period_length,
                     geran_map_nas_id_to_gas_id(message_ptr->as_id)
                     );
                 break;
               }
             case CB_SKIP_REQ:
               {
                 cb_skip_req_type *message_ptr;
                 message_ptr = (cb_skip_req_type *)msg_header;

                 l1_smscb_process_skip_info
                    (message_ptr->channel_ind,
                     message_ptr->scheduling_descriptor.scheduling_period_fn_list,
                     message_ptr->scheduling_descriptor.scheduling_period_length,
                     message_ptr->skip_ind,
                     geran_map_nas_id_to_gas_id(message_ptr->as_id)
                     );
                 break;
               }
             case CB_DEACTIVATE_REQ:
               {
                 cb_deactivate_req_type *message_ptr;
                 message_ptr = (cb_deactivate_req_type *)msg_header;

                 l1_smscb_process_deactivate_info(message_ptr->channel_ind, geran_map_nas_id_to_gas_id(message_ptr->as_id));
                 break;
               }
             default:
               {
                 MSG_ERROR("L1(Decoupled) - Unknown message id.%d", msg_header->message_id, 0, 0);
                 break;
               }
          }
          break;
        }
      default:
        {
          MSG_ERROR("L1(Decoupled) - Unknown message set %d", msg_header->message_set,0,0);
          break;
        }
   }

}
void gl1_set_rach_type(rach_type_t rach_type, gas_id_t gas_id)
{
     g_rach_type[gas_id] = rach_type;
}

rach_type_t gl1_get_rach_type(gas_id_t gas_id)
{
   return g_rach_type[gas_id];
}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
