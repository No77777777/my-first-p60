/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     L 1 DUAL SIM MODE NULL STATE PROCESSING

GENERAL DESCRIPTION
This module handles the NULL mode from TASK context, when in DUAL SIM mode of
operation.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dual_sim_null_if.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
11/05/16   mk        CR1010068 Add DTF events in dedicated mode
24/03/16   km        CR979072 Ensure Reselction req handled in MULTI SIM NULL mode
17/03/16   dg        CR988771 Store desired BSIC sent by GRR in select specific bcch req
27/02/16   sk        CR981473 Correction of handling DS ABORT INDICATION message, gpl1_gprs_handle_ds_abort_ind() called
27/09/15   dv        CR915710 For MPH_BECCH_DECODE_LIST_REQ ensure only 
                              required bands are registered with TRM  
11/09/15   br        CR903740 Add mutex protection for rach abort,to prevent removal of handler while handler is executing
01/09/15   hd        CR899231 Remove the rach_tx_handler once MPH_RECONNECT_CHANNEL_REQ is received
06/07/15   nk        CR866473 handle DS ABORT INDICATION message in NULL mode and RACH mode.
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
15/05/15   sp        CR838865: Fix compile errors on TA20 build.
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
08/04/15   pjr       CR818535 FR25951 Changes to support Single Voice Session 
11/03/15   pjr       CR806250 Fix featurisation for changes submitted under CR787199
24/02/15   pjr       CR787199 Acquire vocoder immediately after IMS releases it during srvcc handover
27/02/15   am        CR646584 WLAN coex
28/05/14   ap        CR671971: Change SGLTE mode on reception of UE_MODE_CHANGE_REQ
28/03/14   sjw       CR637346: Handle new_procedure flag in MPH_SET_PRIORITY_IND           
12/12/13   pjr       CR588712 Call mn_has_nas_already_acquired_voc when FEATURE_DUAL_ACTIVE is enabled
23/08/13   sjw       CR380044 - handle MPH_DECODE_BCCH_LIST_ABORT_REQ in DSDS null state
21/08/13   npt       CR531633 - Remove FEATURE_DIME_MODEM dependencies 
21/06/13   cs        Majopr Triton TSTS Syncup
10/04/13   ap        CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
10/02/13   ip        CR448394
02/07/10   dmb       FEATURE_GERAN_MEM_OPT - l1_dedicated_data structure can be allocated
19/05/10   njt       CR237053 Set the GSTMR FN the same as the WCDMA GSTMR FN
19/05/10   njt       Ensure that L1 is awake for MPH_STOP_GSM_MODE_REQ processing
12/04/10   nt        CR232391 - Add the changes to handle iRAT MPH_RECONNECT_REQ
===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "ms.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "l1.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "l1_drx.h"

#include "l1_os.h"

#include "ftm.h"

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gsmdiag_v.h"

#include "gpl1_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "gpl1_gprs_task.h"
#include "gl1_voice_server.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

extern dedicated_data_T l1_dedicated_data[];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

extern void    L1_setup_blind_ho_ACQ( ARFCN_T  arfcn,
                               const sys_algo_agc_T*  agc_ptr, gas_id_t gas_id );
extern boolean gl1_is_interrat_handover_msg_valid (mph_handover_req_T *message_ptr);
extern boolean gl1_is_interrat_handover_blind     (mph_handover_req_T *message_ptr);
extern void    gl1_store_interrat_handover_details(mph_handover_req_T *message_ptr);
extern sys_algo_agc_T* l1_ds_get_w2g_blind_handover_agc(gas_id_t gas_id);
/*===========================================================================

FUNCTION  gpl1_gprs_handle_multi_sim_null

DESCRIPTION
  This function handles the L1 Task in dual sim null mode, and the messages
  received within the task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_multi_sim_null( IMH_T * msg_header )
{
  gas_id_t gas_id;
   /* code start */
   if (msg_header->message_set == MS_RR_L1)
   {
     switch (msg_header->message_id)
     {

     /********************************************************
      *
      *                MPH_POWER_SCAN_REQ
      *
      ********************************************************/
      case MPH_POWER_SCAN_REQ:
      {
        mph_power_scan_req_T *message_ptr;
        message_ptr = (mph_power_scan_req_T *)msg_header; /*lint !e740 unusual pointer cast */
        gas_id = check_gas_id(message_ptr->gas_id);

        if( FALSE == L1_setup_PSCAN( msg_header ) )
         {
           /* (Re)-Initialize the SCE */
           l1_sc_init(gas_id);

           GL1_ISR_LOCK(gas_id);
           /* Command the  the ISR to start measuring */
           L1_send_ISRTIM_SCAN_REQ(L1_FIND_BCCH_COMMAND,gas_id);
           GL1_ISR_UNLOCK(gas_id);

           /* Rude wakeup from SLEEP, the other SIM may be in IDLE so L1 is sleeping */
           gl1_drx_require_next_tick(gas_id);
         }
         else
         {
           /* Something is not right about the power scan request.
            * Send back a confirmation to avoid any upper layer
            * timeouts.
            */
           L1_send_MPH_POWER_SCAN_CNF( FALSE, gas_id ); /* suspending boolean */
         }
         break;
      }

     /********************************************************
      *
      *          MPH_SELECT_SPECIFIC_BCCH_REQ
      *
      ********************************************************/
      case MPH_SELECT_SPECIFIC_BCCH_REQ:
      {
         mph_select_specific_bcch_req_T *message_ptr;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         sys_band_mask_type             band_mask;
         uint8 bsic_requested;
         /* align pointers */
         message_ptr = (mph_select_specific_bcch_req_T *)msg_header; /*lint !e740 unusual pointer cast */
         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];

         bsic_requested = (message_ptr->bsic.PLMN_colour_code << 3) | message_ptr->bsic.BS_colour_code;

         /* If we are about to camp on a 1800 band cell ensure we are running at
          * low clock speed, otherwise set some other band to clock at high speed
          */
         if(message_ptr->specific_channel_no.band == SYS_BAND_DCS_1800)
         {
           band_mask = SYS_BAND_MASK_GSM_DCS_1800;
         }
         else
         {
           band_mask = SYS_BAND_MASK_GSM_850;
         }

         gl1_set_band_group(band_mask, gas_id);


         /* (Re)-Initialize the SCE */
         l1_sc_init(gas_id);

         GL1_ISR_LOCK(gas_id);

         l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;

         L1_send_ISRTIM_ACQ_REQ(message_ptr->specific_channel_no, gas_id);

         l1_tskisr_blk->next_params.bsic_expected = bsic_requested;

         MSG_GERAN_HIGH_2_G("SpecificBCCH Req with ARFCN:%d, BSIC %2x",message_ptr->specific_channel_no.num, bsic_requested);

         GL1_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_COEX_SW_CXM
         garb_intf_notify_find_bcch_update_arfcn(message_ptr->specific_channel_no,gas_id);
         garb_intf_notify_find_bcch_update_rx_power(message_ptr->specific_channel_no,
              (int32)(l1_tskisr_blk->next_params.L1Data.pAgc_data->pwr_dBm_x16),gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

         /* Rude wakeup from SLEEP, the other SIM may be in IDLE so L1 is sleeping */
         gl1_drx_require_next_tick(gas_id);

         break;
      }

      /********************************************************
       *
       *            MPH_DECODE_BCCH_LIST_REQ
       *
       ********************************************************/
      case MPH_DECODE_BCCH_LIST_REQ:
      {
        mph_decode_bcch_list_req_T  *message_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        BCCH_List_T *l1_BCCH_List_ptr;

        /* Align pointers */
        message_ptr = ( mph_decode_bcch_list_req_T *)msg_header; /*lint !e740 unusual pointer cast */
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];


        /* If RR sends a null list, just send back a cnf and do nothing */
        if ( message_ptr->frequency_list.num_frequencies == 0 )
        {
           MSG_GERAN_ERROR_0_G("RR->L1 BCCH decode list empty");
           /* Send CNF(ERROR) back to RR */
           L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
        }
        else
        {
          L1_setup_DECODE_BCCH_LIST( msg_header );

          /*Only setup bands for use that are required.*/
          gpl1_select_band_to_req_trm(message_ptr->requested_bands, l1_tskisr_blk->client_id, gas_id);

          /* (Re)-Initialize the SCE */
          l1_sc_init(gas_id);

          GL1_ISR_LOCK(gas_id);

          l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
          l1_tskisr_blk->command = L1_ACQUIRE_BCCH_LIST_COMMAND;

          L1_send_ISRTIM_ACQ_LIST_REQ( l1_BCCH_List_ptr->current_sch_index,gas_id );

          GL1_ISR_UNLOCK(gas_id);

          /* Rude wakeup from SLEEP, the other SIM may be in IDLE so L1 is sleeping */
          gl1_drx_require_next_tick(gas_id);

        }

        break;
      }

       /********************************************************
       *                                                      *
       *        MPH_DECODE_BCCH_LIST_ABORT_REQ                *
       *                                                      *
       ****************************************************** */
      case MPH_DECODE_BCCH_LIST_ABORT_REQ:
      {
        mph_decode_bcch_list_abort_req_T* message_ptr = ( mph_decode_bcch_list_abort_req_T* )msg_header;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
           
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
         /* Change the ISR mode */
         GL1_ISR_LOCK(gas_id);
         l1_tskisr_blk->main_command = L1_ENTER_NULL_COMMAND;
         /* 
            Indicate that a decode bcch list cnf is to be sent.
            Only sent when aborted by a MPH_DECODE_BCCH_LIST_ABORT_REQ
          */
            
         l1_tskisr_blk->next_params.L1_confirm_ID = L1_DECODE_BCCH_LIST_CNF;  
         GL1_ISR_UNLOCK(gas_id);

         break;
      }

      /********************************************************
       *                                                      *
       *                MPH_HANDOVER_REQ                      *
       *                                                      *
       ********************************************************/

      case MPH_HANDOVER_REQ:
      {
         /* InterRAT handover */
         mph_handover_req_T  *message_ptr = (mph_handover_req_T *)msg_header; /*lint !e740 unusual pointer cast */
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         ARFCN_T arfcn;

         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
         gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_IRAT_HO);
#endif

#ifdef FEATURE_INTER_RAT_HO_OPT
         /* Notify Voice Agent that GSM will need acces to the vocoder */
         l1_vs_request_vocoder_ownership(gas_id);
#endif

         /* (Re)-Initialize the SCE */
         l1_sc_init(gas_id);

         arfcn =  message_ptr->BCCH_ARFCN;

         MSG_GERAN_HIGH_1_G("MPH_HANDOVER_REQ to GSM Cell %d ",arfcn.num);

         /*Check if handover is possible - validate timing advance */
         if ( ! gl1_is_interrat_handover_msg_valid(message_ptr) )
         {
             L1_send_MPH_HANDOVER_IND( HANDOVER_OUT_OF_RANGE, gas_id );
             break; // Stay here -- wait for GSM_STOP_REQ from RR
         }

         gl1_store_interrat_handover_details(message_ptr);

         if( gl1_is_interrat_handover_blind(message_ptr) )
         {    /* BLIND CASE */
             MSG_GERAN_HIGH_0_G("Performing Blind Handover ");

             L1_setup_blind_ho_ACQ( arfcn,
                                    l1_ds_get_w2g_blind_handover_agc(gas_id), message_ptr->gas_id ); /* list of one freq */

             GL1_ISR_LOCK(gas_id);
             l1_tskisr_blk->main_command = L1_GET_TIMING_COMMAND;
             L1_send_ISRTIM_ACQ_REQ(arfcn, gas_id);
             GL1_ISR_UNLOCK(gas_id);
         }
         else
         {   /* COMPRESSED MODE CASE */
             MSG_GERAN_HIGH_0_G("Performing Compressed Mode Handover ");
             GL1_ISR_LOCK(gas_id);
             l1_tskisr_blk->main_command = L1_CM_HANDOVER_COMMAND;
             GL1_ISR_UNLOCK(gas_id);
         }
#ifdef FEATURE_WLAN_COEX_SW_CXM
         garb_cxm_high_priority_event_ind(GSTMR_GET_FN_GERAN(gas_id),GL1_DEFS_INVALID_FN,CXM_HIGH_PRIO_HO,gas_id);
#endif
         break;
      }
      /********************************************************
       *                                                         *
       *                MPH_RECONNECT_CHANNEL_REQ                *
       *                                                         *
       ********************************************************/
      case MPH_RECONNECT_CHANNEL_REQ:
      {
         /* GtoW handover failed */
         mph_reconnect_channel_req_T  *message_ptr;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         dedicated_data_T *l1_dedicated_data_ptr;

         message_ptr = (mph_reconnect_channel_req_T *)msg_header; /*lint !e740 unusual pointer cast */
         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];
         l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

         MSG_GERAN_HIGH_0_G("MPH_RECONNECT_CHANNEL_REQ to GSM Cell");

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
         gl1_msg_set_iq_state( FALSE, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES || FEATURE_GSM_LOG_IQ_SAMPLES */

#ifdef FEATURE_INTER_RAT_HO_OPT
         /* Notify Voice Agent that GSM will need acces to the vocoder */
         l1_vs_request_vocoder_ownership(gas_id);
#endif

         l1_sc_init(gas_id);

         /* Update Dedicated params to that of target cell*/
         l1_dedicated_data_ptr->channel_specification
             = message_ptr->channel_spec;
         l1_dedicated_data_ptr->starting_time
             = (message_ptr->channel_spec.starting_time[0]<<8)
              + message_ptr->channel_spec.starting_time[1];

         /*
         If when previously on this cell the starting time had expired then
         invalidate this starting time here. The value of the starting time must also
         be cleared as this is sometimes used without the check for starting_time_valid
         in l1_ded.c
         */
         if ( l1_dedicated_data_ptr->previous_cell_on_after_channels == TRUE )
         {
           l1_dedicated_data_ptr->channel_specification.starting_time_valid = FALSE;
           l1_dedicated_data_ptr->starting_time = 0;
         }

         gl1_load_current_cell_data(gas_id);

         l1_dedicated_data_ptr->no_of_ab_to_send = 0;
         GL1_ISR_LOCK(gas_id);
         gl1_msg_abort_rach(gas_id);
         l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND;
         GL1_ISR_UNLOCK(gas_id);
         break;
      }

      /********************************************************
       *                                                      *
    *                MPH__GPRS_RESELECTION_REQ              *
    *                                                       *
    ********************************************************/

   case MPH_GPRS_RESELECTION_REQ:
    {
      gpl1_gprs_handle_reselection_bcch_mode((mph_gprs_reselection_req_t *)msg_header);
      break;
    }	  

      /********************************************************
       *                                                      *
       *                MPH_UE_MODE_CHANGE_REQ                *
       *                                                      *
       ********************************************************/

      case MPH_UE_MODE_CHANGE_REQ:
      {
        mph_ue_mode_change_req_T  *message_ptr;
        message_ptr = (mph_ue_mode_change_req_T *)msg_header; /*lint !e740 unusual pointer cast */
        gas_id = check_gas_id(message_ptr->gas_id);
#ifdef FEATURE_SGLTE
        /* Set SGLTE mode */
        gl1_hw_set_sglte_mode(message_ptr->sglte_mode_active, gas_id);        
#endif
        l1_sc_wcdma_set_mode(message_ptr->gsm_only, gas_id);
        break;
      }

      /********************************************************
       *
       *              MPH_STOP_GSM_MODE_REQ
       *
       ********************************************************/
      case MPH_STOP_GSM_MODE_REQ:
      {
        mph_stop_gsm_mode_req_T  *message_ptr;
        message_ptr = ( mph_stop_gsm_mode_req_T *) msg_header; /*lint !e740 */
        gas_id = check_gas_id(message_ptr->gas_id);

        /* Stop L1 ISR */
        L1_setup_DEACTIVATE( msg_header );

        /* Rude wakeup from SLEEP, so we respond faster */
        gl1_drx_require_next_tick(gas_id);

        break;
      }

      case MPH_SET_PRIORITY_IND:
      {
        /* no specific action required */
        break;
      }      

      case MPH_L1_DS_ABORT_IND:
      {
           gpl1_gprs_handle_ds_abort_ind((mph_l1_ds_abort_ind_T *)msg_header);
           break;
      }

      default:
      {
        MSG_GERAN_ERROR_1("RR->L1(NULL) message %d ignored.",msg_header->message_id);
      }

     }/*end switch msgid */
   }/*end if MS_RR_L1*/
   else
   {
      MSG_GERAN_ERROR_2("L1(MULTI_SIM_NULL) - Unknown message <%d,%d>.",
                msg_header->message_set,msg_header->message_id);
   }

}

#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */


