/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                          GL1 MSGR Interface
                          RF Messages Handler

GENERAL DESCRIPTION
   This module handles messages received from RF task. 

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmsgrif/src/gl1_msgrif_rfmsg.c#2 $ $DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   ---       ---------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
09/05/18   rv       CR2238600 F3 reduction TA.3.0.c1
23/04/18   sn       CR2220807 Corrected VSWR functionality
29/01/18   rv       CR2026588 prevent a race condition between rf msg handler task and ISR sending tx band cmd to gfw
11/08/17   rc        CR2138578  ML logging - GL1 Changes
28/04/17   og/snjv   CR2048710 For MCS STX PWR IND: Avoid IIR Filter and PCL Conversion when averaging
24/03/17   nm       CR2023665 Correct the Tx power averaging required for SAR reporting
12/10/16   gmr      CR1036802 Gl1 to use actual Tx power in MCS power reports for RT SAR feature
22/04/16   ksb      CR1005869 To mainlne FEATURE_GL1_SLEEP_DEBUG_STRUCT
15/05/17   akm      CR2047050 Avoid GL1 going to sleep during rude wakeup if RF wakeup CNF is not received
16/12/16   sk       CR1100657  Send GERAN_FW_RFM_ENTER_MODE_CMD only for PRX if wakeup Cnf has come only for it
24/05/16   hd       CR1020082 Send RF_GSM_SET_TX_BAND_REQ in access mode, only if tx sleep request is not sent after wakeup
08/02/16   jj       CR872002 GL1 changes for VSWR feature 
12/01/16   sp       CR945229: Ensure Async Command issued only after Enter Mode to GFW in Normal ISR
22/10/15   zf       CR923858 Rework Sleep error recovery in QTA 
26/08/15   sp       CR744909 On TRM grant after GTA/G2W TA gap - Ensure Tx Enable is configured first and then Tx Band Config 
15/08/15   df       CR882861 Improve commit threshold logic to use GRF estimate HW tran exec time 
29/07/15   nk       CR881383 Update gain tags only if DTM is in running sub state
23/07/15   pg       CR877745 Reorder multisim FW RF init to reduce critical path through RF wakeup 
26/06/15   nm       CR862218 wait to get task mutex before calling GFW enter mode
15/06/15   jk        CR854422:GL1 does not send Tx band request to GFW in FTM mode.
04/06/15   pg       CR848279 GERAN TxLM processing occurs before Tx clock enable due to move to MSGR based interface 
29/05/15   pa       CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD. 
15/04/15   pa       CR822841: Split Rx/Tx GL1 TRM changes for Single SIM.
07/05/15   npt      CR821672 GERAN power logging update  
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
16/03/15   pg       CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
12/03/15   pa       CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
12/02/15   cja      CR794676 Only set tx band iss to GFW false if RF set Tx band is sent to RF
27/01/15   jj       CR 787210  set the band invalid before tx enable 
15/01/15   jj       CR 782129 dont send two set tx band  to FW after dedicated wtr hop
21/01/15   jk       CR782488:Use updated gain tags for PRx and DRx in DTM mode
21/01/15   pa       CR758792:RxD support for multi-sim.
07/01/15   zc       CR776264 GL1 updating gain_ptr while rf is processing rx burst
16/12/14   zf       CR771365: Need to return message to msgr free queue after skipping a RF wakeup message
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
05/12/14   cja      CR752567 Always send set tx band after Tx enable cnf
04/12/14   ap       CR727984: GL1 calls enter mode for GFW whilst GFW is in IDLE which is illegal
06/11/14   jj       CR 750218 remove tx config cmd before wtr hop cmd  
14/11/14   br       CR746260 On receiving TX BAND CNF from RF, GL1 need to check fw loaded also before crash if mcpm tx is not turned on.
14/10/14   df       CR676084 Sleep debug structure improvements 
01/11/14   df       CR748602 Rework and enable adaptive timeline
14/10/14   df       CR676084 Sleep debug structure improvements 
10/10/14   pg       CR729410 make DRX exit mode MSGR RSP handling non-blocking 
10/10/14   cos      CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes 
26/09/14   zf       CR732436: Delay starting ACQ when RF wakes up late
26/09/14   zf       CR730930: Cancel pending GFW commands when RF does not wakeup before next frame to mitigate TRM/RF concurrency delay issue.
17/07/14   dg       CR695785 GL1 to wait for MCPM to be turned on only in case of RACH on receiving TX BAND
                    CNF from RF and crash if MCPM is turned ON within 8ms
20/05/14   dg       CR613013 Long Term fix on GL1 for TS 34.123 & TC 8.3.11.12 GCF Testcase
                    failure due to RACH failure on GSM cell
29/08/14   cos      CR 716812 - Adding missing changes for imrd framework
28/08/14   am       CR716841 enable device id in enter mode command as FW is supporting now
25/08/14   df       CR660576 Enhanced timeline debug 
19/08/14   df       CR658606 RF passes estimated time to download RF scripts accross CCS/RFFE
11/02/14   jk       CR614254:GL1 changes to support IP2 calibration
18/07/14   pg/pa    CR694601:GERAN L1 support for dedicated WTR hopping with FED architecture
11/06/14   npt      CR630396: Wake-up error recovery mechanism
10/06/14   jj       CR 668965 merge ASDIV from DI.3.0.c8 to BO 2.0 
29/04/14   pjr      CR600612 Dedicated WTR Hop initial code drop
17/04/14   cos      CR646420 Changes for enabling Rx Diversity on Bolt - GL1/GFW intf updates
07/04/14   cja      CR644445 Mainline feature FEATURE_GSM_ASYNC_FW_INTF
19/03/14   cja      CR632037 Use RF Task for X2G
05/03/14   npt      CR626661: Re-enable parallel wake-up
03/03/14   npt      CR622060: Rework wake-up
25/02/14   cos      CR608951: GERAN RFLM FED: Pass subscription ID to GERAN RF in all message router payloads
15/02/14   ip/npt   CR547955: Define multi thread usage on wake up from sleep
09/12/13   cja      CR586971: Remove function mdsp_issue_enter_mode_cmd
01/11/13   pa       CR569692: X2G via RF Task. 
14/10/13   pa       CR536820: Added GL1 Msgr If Task. Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "gl1_msgrif_rfmsg.h"
#include "rfcom.h"
#include "gl1_msgrif_init.h"
#include "gl1_msgrif_os.h"
#include "gl1_msgrif_task.h"
#include "rfgsm_msg.h"
#include "rex.h"
#include "geran_tasks.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "gl1_hw.h"
#include "geran_dual_sim_g.h"
#include "gl1_msg_wtr_hop.h"
#include "gl1_hw_sleep_ctl.h"
#include "DALSys.h"
#include "l1_task.h"
#include "l1_utils.h"
#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/
#include "gl1_hw_g.h"
#include "gpl1_gprs_serving_cell.h"

/*===========================================================================

                     GLOBAL DATA

===========================================================================*/

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

/*===========================================================================

                     Functions Declarations

===========================================================================*/
static void gl1_msgrif_handle_drx_wakeup_cnf(gas_id_t gas_id);
static void gl1_msgrif_handle_combined_or_prx_wakeup_cnf(gl1_msgrif_rf_task_msgr_t *cmd_ptr, 
                                            gas_id_t gas_id);
static void gl1_msgrif_handle_rf_wakeup_cnf(gl1_msgrif_rf_task_msgr_t *cmd_ptr, 
                                            gas_id_t gas_id);
#ifdef FEATURE_GSM_RX_TX_SPLIT
static void gl1_msgrif_handle_rf_tx_wakeup_cnf(gl1_msgrif_rf_task_msgr_t *cmd_ptr,
                                               gas_id_t gas_id);
#endif
/*===========================================================================

                     FUNCTIONS DEFINITION

===========================================================================*/

/*===========================================================================

FUNCTION  gl1_msgrif_handle_rf_task_msgr_msgs

DESCRIPTION
  This function handles messages received from RF task.
  queue.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msgrif_handle_rf_task_msgr_msgs(gas_id_t gas_id)
{
  static rf_gain_range_T save_gain_range[NUM_GERAN_DATA_SPACES];
  static int16           save_expected_power[NUM_GERAN_DATA_SPACES];
  gl1_msgrif_rf_task_msgr_t*  cmd_ptr  = NULL;
  rfa_rf_gsm_cmd_outcome_t result = RFA_RF_GSM_SUCCESS;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  
#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
  int32 last_tx_pwr_in_dBm100 = 0;
  uint8 round_last_tx_pwr_in_dBm = 0;
#endif
  
  gas_id = check_gas_id(gas_id);
  /* Clear the OS signal */
  (void)rex_clr_sigs( gl1_msgrif_task_tcb_read(gas_id),
                      GL1_MSGRIF_RF_TASK_SIG);

  /* get messages from the RF Task MSGR queue*/
  while ((cmd_ptr = (gl1_msgrif_rf_task_msgr_t *) q_get(&gl1_msgrif_rf_task_msgr_q[gas_id])) != NULL)
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
    /* Check the UMID */
    switch(cmd_ptr->msg.hdr.id)
    {
  case RFA_RF_GSM_RX_BURST_CNF:
#ifdef FEATURE_DUAL_SIM
  case RFA_RF_GSM_RX_BURST_SUB2_CNF:
#endif /*FEATURE_DUAL_SIM*/
#ifdef FEATURE_TRIPLE_SIM
  case RFA_RF_GSM_RX_BURST_SUB3_CNF:
#endif /* FEATURE_TRIPLE_SIM */
      {
        /* set num bursts to 0xFF to mark buffer as completed */
        cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->num_rx_bursts = 0xFF;

        if(cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].burst_type == RF_RX_BURST)
        {
          GL1_HANDLER_PERFORMANCE_LOGGING(STOP_RX_RF,gas_id);

          /* check for RF Task signalling requested API completed successfully */
          result = cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.req_result;
          /* For normal burst debug, if GS changes print F3. */
          if (save_gain_range[gas_id] != cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].gain_ptr->range ||
              save_expected_power[gas_id] != cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].rx_lvl_dbm)
          {
            MSG_GERAN_HIGH_3_G( "RFAGC=%d,GS=%d,FN=%d",
                      cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].rx_lvl_dbm/16,
                      cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].gain_ptr->range,
                      GSTMR_GET_FN_GERAN(gas_id) );
          }
          /* Save last transition. */
          save_gain_range[gas_id] = cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].gain_ptr->range;
          save_expected_power[gas_id] = cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].rx_lvl_dbm;
#ifdef FEATURE_GSM_RFA_TASK_DEBUG
          MSG_GERAN_HIGH_2_G("GSM L1 <- RF Task MSGR queue msg; RFA_RF_GSM_RX_BURST_CNF; Fn=%d; QS=%d",
                   GSTMR_GET_FN_GERAN(gas_id),
                   gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
#endif

          if(gl1_is_current_state_dtm(gas_id) && L1_DTM_RUNNING == gl1_get_current_sub_state(gas_id))
           {
            gl1_hw_update_gain_tags(cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].gain_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY
                                    cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_diversity_chan_params[0].gain_ptr,
#endif 
#ifdef GERAN_L1_HLLL_LNASTATE
                                    cmd_ptr->msg.rfa_rf_gsm_rx_burst_cnf.rx_burst_params->rx_chan_params[0].high_lin_mode_active,
#endif
                                    gas_id);
           }
        }
        else
        {
          GL1_HANDLER_PERFORMANCE_LOGGING(STOP_PWR_MEAS_RF,gas_id);
#ifdef FEATURE_GSM_RFA_TASK_DEBUG
          MSG_GERAN_HIGH_2_G("GSM L1 <- RF Task MSGR queue msg; RFA_RF_GSM_PWR_MEAS_BURST_CNF; Fn=%d; QS=%d",
                   GSTMR_GET_FN_GERAN(gas_id),
                   gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
#endif
        }
        if ((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE)
            && (gl1_hw_is_forced_wakeup(gas_id)==FALSE)
#ifdef FEATURE_QTA
            && (gl1_hw_qta_gap_active(gas_id)==FALSE)
#endif
           )
        {
            gl1_hw_sleep_set_commit_check (RX_BURST_COMMIT, gas_id);
        }
      }
      break;

    case RFA_RF_GSM_SET_TX_BAND_CNF:
#ifdef FEATURE_DUAL_SIM
    case RFA_RF_GSM_SET_TX_BAND_SUB2_CNF:
#endif /*FEATURE_DUAL_SIM*/
#ifdef FEATURE_TRIPLE_SIM
    case RFA_RF_GSM_SET_TX_BAND_SUB3_CNF:
#endif /* FEATURE_TRIPLE_SIM */
      {
        GL1_HANDLER_PERFORMANCE_LOGGING(STOP_TX_RF_SET_BAND,gas_id);

        MSG_GERAN_HIGH_0_G("Got tx band confirmation from RF");

        /* check for RF Task signalling requested API completed successfully */
        result = cmd_ptr->msg.rfa_rf_gsm_tx_band_cnf.req_result;

#ifdef FEATURE_GSM_RFA_TASK_DEBUG
        MSG_GERAN_HIGH_2_G("GSM L1 <- RF Task MSGR queue msg; RFA_RF_GSM_SET_TX_BAND_CNF; Fn=%d; QS=%d",
                 GSTMR_GET_FN_GERAN(gas_id),
                 gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
#endif

#ifdef FEATURE_GSM_WTR_HOP
        /* Do not automatically call fw enter mode during wtr hopping */
        if (wtr_hop_in_progress( gas_id))
        {
          break; 
        }
#endif
        MSG_GERAN_HIGH_3_G( "SET TX BAND CNF returned sub_state %d state %d command %d",
        l1_tskisr_blk->sub_state,l1_tskisr_blk->l1_state,l1_tskisr_blk->main_command);
        gl1_hw_async_intf_set_tx_band_cmd(geran_map_gas_id_to_nas_id(gas_id));
      }
      break;

  case RFA_RF_GSM_TX_BURST_CNF:
#ifdef FEATURE_DUAL_SIM
  case RFA_RF_GSM_TX_BURST_SUB2_CNF:
#endif /*FEATURE_DUAL_SIM*/
#ifdef FEATURE_TRIPLE_SIM
    case RFA_RF_GSM_TX_BURST_SUB3_CNF:
#endif /* FEATURE_TRIPLE_SIM */
      {
        int32                 tmp_tx_pwr_dBm;
        int16 pcl;

        gl1_defs_coding_type cs = GL1_DEFS_CS1_CODING;
        gprs_pl1_serving_cell_meas_T*  gprs_serv_cell_meas_ptr   = &gprs_serv_cell_meas[gas_id];
        int16 power_slots;      //number of slots to report power
        uint8 i = 0;

        GL1_HANDLER_PERFORMANCE_LOGGING(STOP_TX_RF,gas_id);

#ifdef FEATURE_VSWR
       if( cmd_ptr->msg.rfa_rf_gsm_set_tx_burst_cnf.vswr_meas_requested)
	    {
				 gl1_vswr_meas_requested[gas_id] = TRUE ; 
	    }
	    else
	    {
				 gl1_vswr_meas_requested[gas_id] = FALSE ;
				 gl_hw_use_extended_tx_alpha [gas_id] = FALSE ; 
	    }
#endif /*FEATURE_VSWR*/

        if( cmd_ptr->msg.rfa_rf_gsm_set_tx_burst_cnf.tx_burst_params->num_tx_slots <  GERAN_POWER_MAX_UL_SLOTS )
	    {
          power_slots = cmd_ptr->msg.rfa_rf_gsm_set_tx_burst_cnf.tx_burst_params->num_tx_slots;
	    }
	    else
	    {
          power_slots = GERAN_POWER_MAX_UL_SLOTS;
	    }

        for( i=0; i < power_slots; i++ )
        {
           if( cmd_ptr->msg.rfa_rf_gsm_set_tx_burst_cnf.tx_pwr_in_dBm100[i] != GERAN_POWER_UNUSED_UL_SLOT_MASK ) 
           {
              last_tx_pwr_in_dBm100 = cmd_ptr->msg.rfa_rf_gsm_set_tx_burst_cnf.tx_pwr_in_dBm100[i];
              
              /* If Power % 100 is >50 Then Round it to Next integer post Dividing by 100 */
              tmp_tx_pwr_dBm = (((last_tx_pwr_in_dBm100 % 100) > 50 )? last_tx_pwr_in_dBm100 / 100 + 1 : last_tx_pwr_in_dBm100 / 100); 

              pcl = L1_convert_tx_pwr_from_dBm_to_pcl( gprs_serv_cell_meas_ptr->ul_power_band,
                                                       tmp_tx_pwr_dBm, gas_id
                                                      );
              
              MSG_GERAN_DEBUG_OPT_3_G("TX Burst CNF dBm100 %d dBm %d, pcl %d", last_tx_pwr_in_dBm100, tmp_tx_pwr_dBm, pcl);

              gpl1_update_tx_pwr_avg( &pcl,
                                      gprs_serv_cell_meas_ptr->ul_power_band,
                                      cs,
                                      gas_id
                                    );

              gpl1_update_coex_tx_pwr_avg( tmp_tx_pwr_dBm,
                                           gas_id
                                         );
           }/*If Used Slot*/
        }/*For Loop*/
        round_last_tx_pwr_in_dBm = ((last_tx_pwr_in_dBm100 % 100) ? last_tx_pwr_in_dBm100 / 100 + 1 : last_tx_pwr_in_dBm100 / 100);

#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
        l1_physical_layer_power_data( UPLINK_POWER, round_last_tx_pwr_in_dBm, gas_id );
#endif /* FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO */

        /* check for RF Task signalling requested API completed successfully */
        result = cmd_ptr->msg.rfa_rf_gsm_set_tx_burst_cnf.req_result;

#ifdef FEATURE_GSM_RFA_TASK_DEBUG
        MSG_GERAN_HIGH_2_G("GSM L1 <- RF Task MSGR queue msg; RFA_RF_GSM_TX_BURST_CNF; Fn=%d; QS=%d",
                 GSTMR_GET_FN_GERAN(gas_id),
                 gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
#endif
        if ((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE) && (gl1_hw_is_forced_wakeup(gas_id)==FALSE))
        {
            gl1_hw_sleep_set_commit_check (TX_BURST_COMMIT, gas_id);
        }
      }
      break;

#ifdef  FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
/*  case RFA_RF_GSM_SET_ANTENNA_CNF:
     {
      if (cmd_ptr->msg.rfa_rf_gsm_set_antenna_cnf.req_result == RFA_RF_GSM_SUCCESS )
      {
       MSG_GERAN_HIGH_3_G(" antenna switch suceess re_result =%d ", cmd_ptr->msg.rfa_rf_gsm_set_antenna_cnf.req_result,0,0);
      }
      else
      {
       MSG_GERAN_HIGH_3_G(" antenna switch failed re_result =%d ", cmd_ptr->msg.rfa_rf_gsm_set_antenna_cnf.req_result,0,0);
      }

      break;
     }*/
#endif /*FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/

#ifdef FEATURE_GSM_RX_TX_SPLIT
    case RFA_RF_GSM_RX_WAKEUP_CNF:
#ifdef FEATURE_DUAL_SIM
    case RFA_RF_GSM_RX_WAKEUP_SUB2_CNF:
#endif /*FEATURE_DUAL_SIM*/
#ifdef FEATURE_TRIPLE_SIM
    case RFA_RF_GSM_RX_WAKEUP_SUB3_CNF:
#endif /* FEATURE_TRIPLE_SIM */
#else
    case RFA_RF_GSM_WAKEUP_CNF:
#ifdef FEATURE_DUAL_SIM
    case RFA_RF_GSM_WAKEUP_SUB2_CNF:
#endif /*FEATURE_DUAL_SIM*/
#ifdef FEATURE_TRIPLE_SIM
    case RFA_RF_GSM_WAKEUP_SUB3_CNF:
#endif /* FEATURE_TRIPLE_SIM */
#endif
    {
      gl1_msgrif_handle_rf_wakeup_cnf(cmd_ptr, gas_id);
      break;
    }

#ifdef FEATURE_GSM_RX_TX_SPLIT
    case RFA_RF_GSM_TX_WAKEUP_CNF:
#ifdef FEATURE_DUAL_SIM
    case RFA_RF_GSM_TX_WAKEUP_SUB2_CNF:
#endif /*FEATURE_DUAL_SIM*/
#if defined ( FEATURE_TRIPLE_SIM )
    case RFA_RF_GSM_TX_WAKEUP_SUB3_CNF:
#endif
    {
      gl1_msgrif_handle_rf_tx_wakeup_cnf(cmd_ptr, gas_id);
    }
    break;
#else
    case RFA_RF_GSM_TX_ENABLE_CNF:
#ifdef FEATURE_DUAL_SIM
    case RFA_RF_GSM_TX_ENABLE_SUB2_CNF:
#endif /*FEATURE_DUAL_SIM*/
#if defined ( FEATURE_TRIPLE_SIM )
    case RFA_RF_GSM_TX_ENABLE_SUB3_CNF:
#endif
      {

#ifdef FEATURE_GSM_WTR_HOP
        if (! wtr_hop_in_progress( gas_id))
        {
#endif /* FEATURE_GSM_WTR_HOP */
#ifdef FEATURE_GSM_RX_TX_SPLIT
            gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_ACTIVE,
                                             (uint32)-1,
                                             gl1_hw_get_txlm_buff_index(gas_id),
                                             GFW_ENTER_MODE_TX_BIT,
                                             gl1_hw_get_gsm_rf_id(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id),
                                             geran_map_gas_id_to_nas_id(gas_id));
#else
            /*Call mcpm_tx_config_cmd_pending using the below wrapper function*/
            gl1_hw_mcpm_call_config_cmd(gas_id);
#endif
            /* Issue Set TX BAND command to RF here only for RACH state*/
            if((l1_tskisr_blk->sub_state == L1_ACCESS_INIT)|| (l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE)
                   || (l1_tskisr_blk->main_command == L1_ACCESS_COMMAND))
            {
               gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(gl1_get_serving_cell_arfcn(gas_id)),gas_id);
            }


           if (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE && l1_tskisr_blk->sub_state != L1_TRANSFER_INIT)
           {
             transfer_data_T *transfer_data_ptr=l1_tskisr_blk->current_params.L1Data.pTransfer_data;
             frequency_information_T *freq_info_ptr = (transfer_data_ptr->frequency_info_ptr);
             if (freq_info_ptr != NULL)
             {
               gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]),gas_id);
             }
           }            
#ifdef FEATURE_GSM_WTR_HOP          
        }
        else
        {
          MSG_GERAN_HIGH_0_G( "postponed set tx band and tx config after wtr hop cmd ");          
        }
#endif /*FEATURE_GSM_WTR_HOP*/

        /*Set the below flag to TRUE as TX ENABLE CNF is received from RF*/
        l1_rf_tx_enable_cnf_rcv[gas_id] = TRUE;

        MSG_GERAN_HIGH_2_G( "RFA GSM TX ENABLE CNF returned sub_state %d state %d",l1_tskisr_blk->sub_state,l1_tskisr_blk->l1_state);
      }
      break;
#endif
      case RFA_RF_GSM_ENTER_MODE_CNF:
      case RFA_RF_GSM_EXIT_MODE_CNF:
      case RFA_RF_GSM_SLEEP_CNF:
      case RFA_RF_GSM_TX_DISABLE_CNF:
      case RFA_RF_GSM_CM_ENTER_CNF:
      case RFA_RF_GSM_CM_EXIT_CNF:

#if defined ( FEATURE_DUAL_SIM )|| defined ( FEATURE_TRIPLE_SIM )
       /* RFA Device 2 UMID's*/
      case RFA_RF_GSM_ENTER_MODE_SUB2_CNF:
      case RFA_RF_GSM_EXIT_MODE_SUB2_CNF:
      case RFA_RF_GSM_SLEEP_SUB2_CNF:
      case RFA_RF_GSM_TX_DISABLE_SUB2_CNF:
      case RFA_RF_GSM_CM_ENTER_SUB2_CNF:
      case RFA_RF_GSM_CM_EXIT_SUB2_CNF:
#endif
#if defined ( FEATURE_TRIPLE_SIM )
      case RFA_RF_GSM_ENTER_MODE_SUB3_CNF:
      case RFA_RF_GSM_EXIT_MODE_SUB3_CNF:
      case RFA_RF_GSM_SLEEP_SUB3_CNF:
      case RFA_RF_GSM_TX_DISABLE_SUB3_CNF:
      case RFA_RF_GSM_CM_ENTER_SUB3_CNF:
      case RFA_RF_GSM_CM_EXIT_SUB3_CNF:
#endif /* FEATURE_TRIPLE_SIM */
      {
        MSG_GERAN_LOW_1_G("Unexpected RFA Task MSGR msg id %d ",cmd_ptr->msg.hdr.id);
      }
      break;

#ifdef GERAN_L1_IP2CAL
      case RFA_RF_GSM_START_IP2_CAL_CNF:
      case RFA_RF_GSM_START_IP2_CAL_SUB2_CNF:
#if defined ( FEATURE_TRIPLE_SIM )
      case RFA_RF_GSM_START_IP2_CAL_SUB3_CNF:
#endif
      {
        /* check for RF Task signalling requested API completed successfully */
        result = cmd_ptr->msg.rfa_rf_gsm_start_ip2_cal_cnf.req_result;
        MSG_GERAN_HIGH_0_G( "Start IP2Cal CNF returned");
        break;
      }
#endif /*GERAN_L1_IP2CAL*/

#ifdef GERAN_L1_HLLL_LNASTATE
      case RFA_RF_GSM_PROCESS_BURST_METRICS_CNF:
      case RFA_RF_GSM_PROCESS_BURST_METRICS_SUB2_CNF:
#if defined ( FEATURE_TRIPLE_SIM )
      case RFA_RF_GSM_PROCESS_BURST_METRICS_SUB3_CNF:
#endif
      {
        /* check for RF Task signalling requested API completed successfully */
        result = cmd_ptr->msg.rfa_rf_gsm_get_metrics_cnf.req_result;
        MSG_GERAN_HIGH_0_G( "Start HLin LLin Metrics CNF returned");
        break;
      }
#endif /*GERAN_L1_HLLL_LNASTATE*/

      default:
        MSG_GERAN_ERROR_1_G("Unknown RFA Task MSGR msg id %d ",cmd_ptr->msg.hdr.id);
      break;
    }

    /* Indicate error if RF Task confirmation message does not indicate success */
    if(result != RFA_RF_GSM_SUCCESS)
    {
      MSG_GERAN_ERROR_1_G("GSM L1 MSGR <- RF Task MSGR request failed: %d",result);
    }

    /* Return message to the free queue */
    q_put( &gl1_msgrif_rf_task_msgr_free_q[gas_id], &cmd_ptr->link);
  }
}

#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================

FUNCTION  gl1_msgrif_handle_drx_wakeup_cnf

DESCRIPTION
  This function handles DRX RF wakeup CNF..

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msgrif_handle_drx_wakeup_cnf(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  grm_client_enum_t divrx_client = grm_map_prx_to_divrx_client_id(l1_tskisr_blk->client_id, gas_id);

  MSG_GERAN_HIGH_0_G("GSM L1 <- RF Task MSGR queue msg; RFA_RF_GSM_WAKEUP_CNF : DRX WakeUP");
  gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_ACTIVE, 
                                   gl1_get_imrd_rxdiv_rxlm_buff_idx(gas_id),
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                   (uint32)-1,
                                   GFW_ENTER_MODE_DRX_BIT,
#else
                                   GFW_ENTER_MODE_DRX,
#endif
                                   gl1_hw_get_gsm_rf_id(divrx_client, gas_id), 
                                   geran_map_gas_id_to_nas_id(gas_id) );
  gl1_hw_set_gfw_enter_status(GL1_GFW_PRX_DIVRX_ENTER,gas_id);
  /*Wait for start of block to enable RxD*/
  gl1_hw_set_rxdiv_wait_for_block_start(TRUE, gas_id);
  return;
}
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifndef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_msgrif_handle_combined_or_prx_wakeup_cnf

DESCRIPTION
  This function handles Combined or PRX RF wakeup CNF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msgrif_handle_combined_or_prx_wakeup_cnf(gl1_msgrif_rf_task_msgr_t *cmd_ptr, 
                                            gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  rfa_rf_gsm_cmd_outcome_t result = RFA_RF_GSM_SUCCESS;

  gl1_hw_sleep_type *gl1_hw_sleep_cycle   = NULL;

uint8 wakeup_mode = cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.wakeup_mode;



  gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]]; 
  gl1_hw_sleep_cycle->wakeup.rf_cnf_to_vstmr_us = gl1_get_time_next_vsmtr_us(gas_id);
  gl1_hw_sleep_cycle->wakeup.rf_script_dload_us = cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.ccs_time_us;

  gl1_hw_sleep_rf_wakeup_end(gas_id);



#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if(ftm_get_mode() != FTM_MODE)
  {
    MSG_GERAN_HIGH_3_G("RFA_RF_GSM_WAKEUP_CNF: prx_device_req_result=%i, "
                       "drx_device_req_result=%i, req_result=%i",
                       cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.prx_device_req_result,
                       cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.drx_device_req_result,
                       cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.req_result);
  }

  result = cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.req_result;
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

#ifdef FEATURE_GSM_ADAPTIVE_TMLN
  gl1_hw_adaptive_set_valid_flag(gas_id, 
                                 TRUE,
                                 cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.ccs_time_us);

#endif
  gl1_hw_save_ccs_write_time_estimate(cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.ccs_time_us,
                                      gas_id);

  gl1_task_mutex_lock( gas_id );

  /* Print RF's estimated time (microseconds) for the HW transactions
     to be executed. This works for both SSBI and RFFE. */
  MSG_GERAN_HIGH_3_G("GSM L1 <- RF Task MSGR queue msg; RFA_RF_GSM_WAKEUP_CNF; gas id=%d; QS=%d; HW tran exec %dusecs",
                     gas_id,
                     gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)),
                     cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.ccs_time_us);


  /* Profile RF ENTER MODE */
  gl1_hw_sleep_cycle->wakeup.rf_enter_mode.start_ustmr = slpc_get_tick();

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if ( GFW_APP_MODE_ACTIVE != gl1_get_gfw_app_mode(gas_id))
  {
    MSG_GERAN_HIGH_0_G("Skip enter mode");
    gl1_task_mutex_unlock(gas_id );
    return;
  }
#endif /* FEATURE_DUAL_SIM OR FEATURE_TRIPLE_SIM */
  gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_ACTIVE, 
                                   gl1_hw_get_rxlm_buff_index(gas_id),
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                   (uint32)-1,
#endif
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE 
                                   gl1_hw_get_gfw_entermode_type(wakeup_mode, gas_id),
#else
                                   GFW_ENTER_MODE_PRX,
#endif  /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                                   gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id), 
                                   geran_map_gas_id_to_nas_id(gas_id) );

  /* DO NOT MOVE - for accurate commit decision this must be
     called immediately after the enter_mode cmd is sent */
  gl1_hw_save_time_enter_mode_completed(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
  if (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id)) 
  {
    gl1_hw_set_rxdiv_wait_for_block_start(TRUE, gas_id);
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/


  gl1_hw_sleep_debug_duration ( &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]].wakeup.rf_enter_mode);


  l1_tskisr_blk->rf_wakeup_pending = FALSE;

  gl1_task_mutex_unlock(gas_id );

  if ((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE) && (gl1_hw_is_forced_wakeup(gas_id)==FALSE))
  {
    gl1_hw_sleep_set_commit_check (RF_WAKEUP_COMMIT, gas_id);
  }

  return;
}
/*===========================================================================

FUNCTION  gl1_msgrif_handle_rf_wakeup_cnf

DESCRIPTION
  This function handles RF wakeup CNF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msgrif_handle_rf_wakeup_cnf(gl1_msgrif_rf_task_msgr_t *cmd_ptr, 
                                            gas_id_t gas_id)
{
  switch (cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.wakeup_mode) 
  {
#ifdef FEATURE_GSM_RX_DIVERSITY
    case RFGSM_DRX_WAKEUP_MODE:
      gl1_msgrif_handle_drx_wakeup_cnf(gas_id);
      break;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    case RFGSM_PRX_WAKEUP_MODE:
    case RFGSM_PRX_DRX_WAKEUP_MODE:
      gl1_msgrif_handle_combined_or_prx_wakeup_cnf(cmd_ptr, gas_id);
      break;

    case RFGSM_INVALID_WAKEUP_MODE:
    default:
      MSG_GERAN_ERROR_1_G("Invalid RF Wakeup CNF mode %d", cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.wakeup_mode);
      break;
  }
  gl1_hw_sleep_set_rf_cnf_pending(FALSE, gas_id);
  return;
}
#endif

#ifdef FEATURE_GSM_RX_TX_SPLIT
/*===========================================================================

FUNCTION  gl1_msgrif_handle_combined_or_prx_wakeup_cnf

DESCRIPTION
  This function handles Combined or PRX RF wakeup CNF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msgrif_handle_combined_or_prx_wakeup_cnf(gl1_msgrif_rf_task_msgr_t *cmd_ptr, 
                                            gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  rfa_rf_gsm_cmd_outcome_t result = RFA_RF_GSM_SUCCESS;


  gl1_hw_sleep_type *gl1_hw_sleep_cycle   = NULL;


#ifdef FEATURE_GSM_RX_TX_SPLIT
uint8 wakeup_mode = cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.op_mode;
#else
  uint8 wakeup_mode = cmd_ptr->msg.rfa_rf_gsm_wakeup_cnf.wakeup_mode;
#endif



  gl1_hw_sleep_cycle = &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]]; 
  gl1_hw_sleep_cycle->wakeup.rf_cnf_to_vstmr_us = gl1_get_time_next_vsmtr_us(gas_id);
  gl1_hw_sleep_cycle->wakeup.rf_script_dload_us = cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.ccs_time_us;

  gl1_hw_sleep_rf_wakeup_end(gas_id);

  /* Profile RF ENTER MODE */
  gl1_hw_sleep_cycle->wakeup.rf_enter_mode.start_ustmr = slpc_get_tick();

  gl1_task_mutex_lock( gas_id );

  gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_ACTIVE, 
                                   gl1_hw_get_rxlm_buff_index(gas_id),
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                   (uint32)-1,
#endif
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE 
                                   gl1_hw_get_gfw_entermode_type(wakeup_mode, gas_id),
#else
                                   GFW_ENTER_MODE_PRX,
#endif  /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
                                   gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id), 
                                   geran_map_gas_id_to_nas_id(gas_id) );

  /* DO NOT MOVE - for accurate commit decision this must be
     called immediately after the enter_mode cmd is sent */
  gl1_hw_save_time_enter_mode_completed(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if(ftm_get_mode() != FTM_MODE)
  {
    MSG_GERAN_HIGH_3_G("RFA_RF_GSM_WAKEUP_CNF: prx_device_req_result=%i, "
                       "drx_device_req_result=%i, req_result=%i",
                       cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.prx_device_req_result,
                       cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.drx_device_req_result,
                       cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.req_result);
  }

  result = cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.req_result;
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

#ifdef FEATURE_GSM_ADAPTIVE_TMLN
  gl1_hw_adaptive_set_valid_flag(gas_id, 
                                 TRUE,
                                 cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.ccs_time_us);

#endif
  gl1_hw_save_ccs_write_time_estimate(cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.ccs_time_us,
                                      gas_id);

  /* Print RF's estimated time (microseconds) for the HW transactions
     to be executed. This works for both SSBI and RFFE. */
  MSG_GERAN_HIGH_3_G("GSM L1 <- RF Task RFA_RF_GSM_WAKEUP_CNF; QS=%d; vreg/wakeup=%dus; HW tran exec %dusecs",
                     gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)),
                     gl1_hw_sleep_cycle->wakeup.rf_awake.duration_us,
                     cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.ccs_time_us);

#ifdef FEATURE_GSM_RX_DIVERSITY
  if (GL1_GFW_PRX_DIVRX_ENTER == gl1_hw_get_gfw_enter_status(gas_id)) 
  {
    gl1_hw_set_rxdiv_wait_for_block_start(TRUE, gas_id);
  }
#endif /*FEATURE_GSM_RX_DIVERSITY*/


  gl1_hw_sleep_debug_duration ( &gl1_hw_sleep_struct[gas_id][gl1_sleep_cnt[gas_id]].wakeup.rf_enter_mode);

  l1_tskisr_blk->rf_wakeup_pending = FALSE;

  gl1_task_mutex_unlock(gas_id );
  if (gl1_msg_get_multi_sim_mode() &&
      gl1_hw_sleep_get_in_warmup_check(gas_id) == FALSE &&
      l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
  {
     GL1_ISR_LOCK(gas_id);
     if (mdsp_commands_pending(gas_id))
     {
        MSG_GERAN_HIGH_0_G("Issuing Async Command from RF WakeupCnf");
        mdsp_issue_async_msg( GFW_ASYNC_CMD, gas_id );
     }
     GL1_ISR_UNLOCK(gas_id);
  }

  if ((gl1_hw_sleep_get_in_warmup_check(gas_id)==TRUE) && (gl1_hw_is_forced_wakeup(gas_id)==FALSE))
  {
    gl1_hw_sleep_set_commit_check (RF_WAKEUP_COMMIT, gas_id);
  }
  return;
}
/*===========================================================================

FUNCTION  gl1_msgrif_handle_rf_wakeup_cnf

DESCRIPTION
  This function handles RF wakeup CNF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msgrif_handle_rf_wakeup_cnf(gl1_msgrif_rf_task_msgr_t *cmd_ptr, 
                                            gas_id_t gas_id)
{
#if 0 //under review
#ifdef GERAN_L1_HLLL_LNASTATE
  uint8 i = 0;
  rfcom_device_enum_type device;

  for(device=RFM_DEVICE_0;device<RFM_MAX_WAN_DEVICES;device++)
  {
    for(i=0;i<(RFCOM_NUM_GSM_BANDS-1);i++)
    {
      gl1_hw_data_ptr[gas_id]->gl1_hw_sawless_support[device] |= (cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.sawless_sup[device][i] << i);
      MSG_GERAN_HIGH_3_G("Sawless support %d for Band %d for device:%d",
                         cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.sawless_sup[device][i],
                         (rfcom_gsm_band_type)i,
                         device);
    }
  }
#endif

#ifdef GERAN_L1_IP2CAL
  if(ftm_get_mode() == FTM_MODE)
  {
    gl1_hw_data_ptr[gas_id]->ip2cal_supported = cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.ip2_cal_supported;
    MSG_GERAN_HIGH_1_G("ip2cal_supported:%d",gl1_hw_data_ptr[gas_id]->ip2cal_supported);
  }
#endif /*GERAN_L1_IP2CAL*/
#endif
  switch (cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.op_mode) 
  {
#ifdef FEATURE_GSM_RX_DIVERSITY
    case RFGSM_DRX_OP_MODE:
      gl1_msgrif_handle_drx_wakeup_cnf(gas_id);
      break;
#endif /*FEATURE_GSM_RX_DIVERSITY*/

    case RFGSM_PRX_OP_MODE:
    case RFGSM_PRX_DRX_OP_MODE:
      gl1_msgrif_handle_combined_or_prx_wakeup_cnf(cmd_ptr, gas_id);
      break;

    default:
      MSG_GERAN_ERROR_1_G("Invalid RF Wakeup CNF mode %d", cmd_ptr->msg.rfa_rf_gsm_rx_wakeup_cnf.op_mode);
      break;
  }
  gl1_hw_sleep_set_rf_cnf_pending(FALSE, gas_id);
  return;
}

/*===========================================================================

FUNCTION  gl1_msgrif_handle_rf_tx_wakeup_cnf

DESCRIPTION
  This function handles RF wakeup CNF.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_msgrif_handle_rf_tx_wakeup_cnf(gl1_msgrif_rf_task_msgr_t *cmd_ptr,
                                               gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint32 count_wait_for_mcpm_on = 0;

#ifdef FEATURE_GSM_WTR_HOP
  if (! wtr_hop_in_progress( gas_id))
  {
#endif /* FEATURE_GSM_WTR_HOP */
     /* Must wait for MCPM to turn Tx on before sending Tx enter_mode request to GFW
        otherwise TxLM processing will cause unclocked register access */
     if (!g1l_hw_is_mcpm_tx_on(gas_id))
     {
        MSG_GERAN_HIGH_1_G("Waiting for MCPM to turn Tx on (mcpm_tx_on=%d)",g1l_hw_is_mcpm_tx_on(gas_id));
        /*Wait for MCPM TX to be turned ON*/
        while(!g1l_hw_is_mcpm_tx_on(gas_id))
        {
           DALSYS_BusyWait(50);
           /*Crash if MCPM is not turned ON for 8ms  8ms = 50 us * 160*/
           if( (++count_wait_for_mcpm_on) == 160)
           {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
              ERR_FATAL("MCPM not turned ON yet",0,0,0);
           }
        }
        MSG_GERAN_HIGH_0_G("MCPM turned on NOW - proceed with Tx enter_mode request to GFW");
     }

     gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_ACTIVE,
                                      (uint32)-1,
                                      gl1_hw_get_txlm_buff_index(gas_id),
                                      GFW_ENTER_MODE_TX_BIT,
                                      gl1_hw_get_gsm_rf_id(grm_map_rx_to_tx_client_id(l1_tskisr_blk->client_id, gas_id), gas_id),
                                      geran_map_gas_id_to_nas_id(gas_id));

     /* Issue Set TX BAND command to RF here only for RACH state*/
     if(((l1_tskisr_blk->sub_state == L1_ACCESS_INIT) ||
         (l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE) ||
         (l1_tskisr_blk->main_command == L1_ACCESS_COMMAND)) &&
         is_hw_tx_on(gas_id))
     {
        gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(gl1_get_serving_cell_arfcn(gas_id)),gas_id);
     }
     
     if (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE && l1_tskisr_blk->sub_state != L1_TRANSFER_INIT)
     {
       transfer_data_T *transfer_data_ptr=l1_tskisr_blk->current_params.L1Data.pTransfer_data;
       frequency_information_T *freq_info_ptr = (transfer_data_ptr->frequency_info_ptr);
       if (freq_info_ptr != NULL)
       {
         GL1_ISR_LOCK(gas_id);
         gl1_hw_rf_set_tx_band(gl1_hw_arfcn_to_rfgsm_band(freq_info_ptr->frequency_list.channel[0]),gas_id);
         GL1_ISR_UNLOCK(gas_id);
       }
     } 
#ifdef FEATURE_GSM_WTR_HOP          
  }
  else
  {
    MSG_GERAN_HIGH_0_G( "postponed set tx band and tx config after wtr hop cmd ");          
  }
#endif /*FEATURE_GSM_WTR_HOP*/

  /*Set the below flag to TRUE as TX WAKEUP CNF is received from RF*/
  l1_rf_tx_enable_cnf_rcv[gas_id] = TRUE;

  MSG_GERAN_HIGH_2_G( "RFA GSM TX WAKEUP CNF returned sub_state %d state %d",l1_tskisr_blk->sub_state,l1_tskisr_blk->l1_state);
}
#endif

/* EOF */
