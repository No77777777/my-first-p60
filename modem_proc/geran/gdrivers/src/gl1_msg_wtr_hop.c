/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             WTR HOPPING FEATURE

GENERAL DESCRIPTION

   This modules provides the mechanisms to hop from an RF transceiver to the 
   other during an active voice call

EXTERNALIZED FUNCTIONS



INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_wtr_hop.c#2 $
  $DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
09/05/15   ch/pa    CRXXXXXX Split RX/TX changes for MSIM/SSIM
21/04/15   sjw       CR822898: removal of deprecated ACQUISITION reasons
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
24/03/15   sjw       CR812407: update for new MPH_SET_PRIORITY_IND payload
16/03/15   pg        CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
11/02/15   jj        CR794406 set correct band in WTR  hop sequence 
05/01/15   npt       CR770381 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled  
22/12/14   pa        CR773689: Set RF device to Invalid on RF exit. 
12/12/14   og        CR758453. Race condition when updating gl1_hw_trm_rf_device.
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   ws        CR768376 Q6 compile warning fixes
03/12/14   pjr       CR765326 Ensure SCE is not restarted after connected mode hopping if
                              previously stopped
04/11/14   nk        CR750248 Reset WTR Hop state machine at Change state
07/11/14   pjr       CR751950 Ensure wtr hopping is inactive when state changes from transfer to idle.
06/11/14   jj        CR750218 remove tx config cmd before wtr hop cmd 
03/11/14   cws       CR749364 Correct logic to start wtr hop
20/10/14   cws       CR734670 Prevent WTR hop for PS non-MMS call and enable WTR hop in SVLTE+G
03/09/14   pjr       CR718235 Check for stale hop when starting new hop and reset variables
11/09/14   pg        CR721587 Call Tx_Disable before exit_mode after completing a WTR hop 
10/09/14   cs        CR722344 Remove old deprecated TRM API usage
05/09/14   dg        CR699913 Resolve KW Error
03/09/14   cja       CR717826 Prevent G2X and WTR hop at the same time
20/08/14   pjr       CR711604 Added check to avoid continuing incomplete sequence after mode change
26/08/14   dg        CR699913 Resolve KW Error
25/07/14   dg        CR699913 IN RACH mode , WTR hopping machine should be called only when handlers for
                     of the current frame are executed
06/05/14   dv        CR660054 RXLM enhancement to ensure that no tech de-allocates another techs buffers  
08/08/14   cgc       CR682787 Hopping conflict fix for G2X on SDCCH propagation from DI2.3
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
21/07/14   cs        CR697245 New GRM API support for WTR Hopping
09/07/14   pjr       CR685199 Added extra check to ensure WTR hop is not triggered after ACCESS MODE is aborted
18/07/14   pg/pa     CR694601:GERAN L1 support for dedicated WTR hopping with FED architecture
03/06/14   ap        CR671684 Spurious Dedicated Hop starting
30/05/14   smd       CR672754 Called vreg_on for new rf device at the end of connected hopping 
28/05/14   pjr       CR670220 SCE abort did not have enough time to finish aborting. 
                              Start aborting 5 frames before instead of 3
23/05/14   us        CR665959 Access correct frame no. in SDCCH4 table
22/05/14   smd       CR668255 Called vreg_off for old rf device at the end of connected hopping.
13/05/14   og        CR663995 Use the correct rflm index when deallocating the txlm buffer.
02/04/14   pjr       CR641721 Enable connected mode WTR hopping during DTM
27/03/14   pjr       CR639777 Configure TX chain Id correctly after connected wtr hopping
17/03/14   pjr       CR633698 Dedicated WTR Hop Bring Up changes
19/02/14   pjr       CR600612 Dedicated WTR Hop initial code drop
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"

#ifdef FEATURE_GSM_WTR_HOP

#include "diagcmd.h"
#include "comdef.h"

#include "gl1_msg_wtr_hop.h"

#include "l1i.h"
#include "geran_msgs.h"
#include "gs.h"

#include "geran_dual_sim.h"
#include "gpl1_dual_sim.h"
#include "gpl1_grm_intf.h"

#include "gtmrs_g.h"
#include "gtmrs.h"

#include "rxlm_intf.h"
#include "txlm_intf.h"

#include "rfgsm_msg.h"
#include "msgr.h"
#include "gl1_mutex.h"
#include "gl1_msg_wcdma_meas.h"
#include "gl1_hw_g.h"
#include "mdsp_intf_g.h"

#include "gl1_hw_clk_ctl_g.h"
#include "l1_task.h"
#include "geran_dual_sim_g.h"
#include "l1_isr.h"
#include "subs_prio.h"
#include "gpl1_gprs_isr.h"

/*===========================================================================

                     Private Defines & Constants

===========================================================================*/
#define NUM_FRAMES_STOP_SCE_TO_HOP     5

wtr_hop_start_result_t gl1_msg_hop_start(
                         rfm_device_enum_type rf_device, 
                         gas_id_t gas_id );

/*===========================================================================

                     Private Prototypes

===========================================================================*/
static gas_id_t gas_to_hop = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
static uint32 rxlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
static uint32 txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID;
static uint32 frame_to_hop = GL1_DEFS_INVALID_FN;
static rfm_device_enum_type target_rf_device = RFM_INVALID_DEVICE;
static rfm_device_enum_type old_rf_device = RFM_INVALID_DEVICE;
static wtr_status_type wtr_hop_status = WTR_HOP_INACTIVE;
static uint32 wtr_hop_init_fn = 0;
static byte wtr_hop_l1_state;
static boolean wtr_hop_restart_sce_after_hop = TRUE;

static void wtr_clean_up(gas_id_t gas_id);
static uint32 get_wtr_hop_frame_ded(channel_information_T *chnl_1_data, gas_id_t gas_id);
static uint32 get_wtr_hop_frame_transfer(gas_id_t gas_id);
static uint32 get_wtr_hop_frame_sdcch(channel_information_T *chnl_1_data, gas_id_t gas_id);
static uint32 get_wtr_hop_frame_access(gas_id_t gas_id);
static void reset_wtr_hop_globals(void);


/*===========================================================================

                     FUNCTIONS DEFINITION

===========================================================================*/

/*===========================================================================

FUNCTION reset_wtr_hop_globals

DESCRIPTION
  Reset all connected mode hopping variables to default settings

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void reset_wtr_hop_globals(void)
{
  /* Reset all connected mode hopping variables */
  gas_to_hop       = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
  rxlm_buf_idx     = MDSP_RXLM_BUF_IDX_INVALID;
  txlm_buf_idx     = MDSP_TXLM_BUF_IDX_INVALID;
  frame_to_hop     = GL1_DEFS_INVALID_FN;
  target_rf_device = RFM_INVALID_DEVICE;
  old_rf_device    = RFM_INVALID_DEVICE;
  wtr_hop_status   = WTR_HOP_INACTIVE;
  wtr_hop_init_fn  = 0;
}

/*===========================================================================

FUNCTION gl1_msg_hop_start

DESCRIPTION
  Triggers a radio transceiver hop operation on a sub in dedicated mode

DEPENDENCIES
  None.

RETURN VALUE
  WTR_HOP_START_SUCCESS if operation successfully started
  or an error code otherwise

SIDE EFFECTS
  None.

===========================================================================*/
wtr_hop_start_result_t gl1_msg_hop_start(
                         rfm_device_enum_type rf_device, 
                         gas_id_t             gas_id )
{
  boolean rach_for_ps = FALSE;
  gas_id = check_gas_id(gas_id);
  (void)geran_get_traffic_reason(&rach_for_ps, gas_id);

  /* Check preconditions are met */
  if ( ( l1_tsk_buffer[gas_id].l1_state == L1_DEDICATED_MODE ) ||                             /* Allow hop in dedicated mode                        */
       ( l1_tsk_buffer[gas_id].l1_state == L1_RANDOM_ACCESS_MODE && rach_for_ps == FALSE ) || /* Allow hop in access mode, only when raching for CS */
#ifdef FEATURE_DUAL_DATA
       ( l1_tsk_buffer[gas_id].l1_state == L1_TRANSFER_MODE &&                                /* Allow hop in transfer mode,                        */
         grm_get_data_sub_priority(gas_id) == GERAN_PRIORITY_INVERTED ) ||                     /* only when transmitting MMS                         */
#endif                                                                     
       ( l1_tsk_buffer[gas_id].l1_state == L1_DTM_MODE ) )                                    /* Allow hop in DTM                                   */
  {  
    if (gl1_msg_wcdma_srch_active(gas_id))
    {
      grm_send_wtr_hop_async_rsp(FALSE, gas_id);
      MSG_GERAN_ERROR_0_G("WTR No Swap g2x in progress");
      return WTR_HOP_START_GAS_NOT_ACTIVE;
    }

    if (wtr_hop_status != WTR_HOP_INACTIVE)
    { 
      if ( SUB_FN(GSTMR_GET_FN_GERAN(gas_to_hop), wtr_hop_init_fn) > 100 )
      {
        MSG_GERAN_ERROR_2_G( "Stale hop procedure detected, hop started at FN: %, state: %d. Reseting state machine", 
            wtr_hop_init_fn, wtr_hop_l1_state);

        reset_wtr_hop_globals();
      }
      else
      {
        MSG_GERAN_ERROR_0_G("WTR Swap already in progress");
        return WTR_HOP_START_HOP_IN_PROGRESS;
      }
    }

    if (rf_device != RFM_DEVICE_0 && rf_device != RFM_DEVICE_2)
    { 
      MSG_GERAN_ERROR_0_G("WTR Swap invalid RF device");
      grm_send_wtr_hop_async_rsp(FALSE, gas_id);
      return WTR_HOP_START_INVALID_RF_DEVICE;
    }

    if (l1_tsk_buffer[gas_id].l1_state == L1_RANDOM_ACCESS_MODE &&
        gl1_msg_get_rach_state(gas_id) == RACH_STATE_NULL)
    { 
      MSG_GERAN_ERROR_0_G("RANDOM ACCESS MODE is being aborted");
      grm_send_wtr_hop_async_rsp(FALSE, gas_id);
      return WTR_HOP_START_GAS_NOT_ACTIVE;
    }

    if (l1_tsk_buffer[gas_id].l1_state == L1_TRANSFER_MODE &&
        gpl1_gprs_awaiting_release(gas_id) )
    { 
      MSG_GERAN_ERROR_0_G("TRANSFER MODE is being aborted");
      grm_send_wtr_hop_async_rsp(FALSE, gas_id);
      return WTR_HOP_START_GAS_NOT_ACTIVE;
    }

    /* Set global variables */
    target_rf_device = rf_device;
    gas_to_hop = gas_id;
    wtr_hop_l1_state = l1_tsk_buffer[gas_id].l1_state;

    /* Start wtr hop state machine */
    wtr_hop_status = WTR_HOP_INIT;
    wtr_hop_init_fn = GSTMR_GET_FN_GERAN(gas_id);
    return WTR_HOP_START_SUCCESS;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Sub can not start hop in state: %d", l1_tsk_buffer[gas_id].l1_state);
    grm_send_wtr_hop_async_rsp(FALSE, gas_id);
    return WTR_HOP_START_GAS_NOT_ACTIVE;
  }
}

/*===========================================================================

FUNCTION gl1_msg_wtr_hop_state_machine

DESCRIPTION
  This state machine is called from L1_control_dedicated function after 
  the execution of the Frame Notification and from L1_access_ISR when 
  scheduling AGCH.
  The state machine performs all the actions to prepare L1 and RF for hopping 
  the WTR. The hop operation is performed in the idle frame before the FTN 
  and is executed by function gl1_msg_wtr_hop_idle_frame_pre_ftn. After the 
  hop has been completed the state machine de-allocate unused resources 
  and restart the SCE.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_msg_wtr_hop_state_machine(channel_information_T *chnl_1_data, gas_id_t gas_id)
{
  int8 temp_rf_band = RFGSM_BAND_NONE;
  /* Check if the given gas_id is hopping rf device */
  if (gas_id != gas_to_hop) return;
  
  /* Check if hop was not completed within 100 frames. This is an unlikely scenario 
   * but can happen if hop has not completed or canceled when moving to idle 
   */
  if ( SUB_FN(GSTMR_GET_FN_GERAN(gas_id), wtr_hop_init_fn) > 100 )
  {
    MSG_GERAN_ERROR_2_G( "Stale hop procedure detected, hop started at FN: %, state: %d", 
        wtr_hop_init_fn, wtr_hop_l1_state);

    reset_wtr_hop_globals();
  }

  /* Check if l1 state changed while hop procedure is active */
  if (wtr_hop_l1_state != l1_tsk_buffer[gas_id].l1_state)
  {
    MSG_GERAN_ERROR_2_G("L1 state change after starting swap procedure, %d -> %d", 
        wtr_hop_l1_state, l1_tsk_buffer[gas_id].l1_state);

    gl1_msg_wtr_hop_abort(FALSE, gas_id);
  }

  /* Abort hop if dedicated channel is stopping */
  if (l1_tsk_buffer[gas_id].l1_state == L1_DEDICATED_MODE &&
      l1_tsk_buffer[gas_id].command == L1_DEDICATED_STOP)
  {
    gl1_msg_wtr_hop_abort(FALSE, gas_id);
  }

  /* Abort hop if transfer mode is being released */
  if (l1_tsk_buffer[gas_id].l1_state == L1_TRANSFER_MODE &&
      gpl1_gprs_awaiting_release(gas_id) )
  {
    gl1_msg_wtr_hop_abort(FALSE, gas_id);
  }

  switch (wtr_hop_status)
  {
    case WTR_HOP_INACTIVE:
      break;

    case WTR_HOP_INIT:
      wtr_hop_status = WTR_HOP_ALLOC_RXLM;

    /* Allocate RxLM buffer, get buffer index (keep trying until allocated) */      
    case WTR_HOP_ALLOC_RXLM:   
      if (!rflm_allocate_rx_handle(LM_GSM, LM_GSM, &rxlm_buf_idx))
      {
        MSG_GERAN_ERROR_0_G("Unable to allocate RxLM buffer");
        break;
      }
      wtr_hop_status = WTR_HOP_ALLOC_TXLM;

    /* Allocate TxLM buffer, get buffer index (keep trying until allocated) */      
    case WTR_HOP_ALLOC_TXLM:    
      if (!rflm_allocate_tx_handle(LM_GSM, LM_GSM, &txlm_buf_idx))
            
      {
        MSG_GERAN_ERROR_0_G("Unable to allocate TxLM buffer");
        break;
      }

      switch (wtr_hop_l1_state)
      {
        case L1_RANDOM_ACCESS_MODE:
          frame_to_hop = get_wtr_hop_frame_access(gas_id);
          wtr_hop_restart_sce_after_hop = FALSE;
          wtr_hop_status = WTR_HOP_RF_ENTER_MODE;
          break;

        case L1_DEDICATED_MODE: 
        case L1_DTM_MODE:   
          /* Determine when is next idle frame (TCH) or when their will be
           * at least three frames before next activity while in SDCCH */
          frame_to_hop = get_wtr_hop_frame_ded(chnl_1_data, gas_id);
          wtr_hop_status = WTR_HOP_STOP_SCE;
          break;
        case L1_TRANSFER_MODE:
          frame_to_hop = get_wtr_hop_frame_transfer(gas_id);
          wtr_hop_status = WTR_HOP_STOP_SCE;
          break;
      }

      MSG_GERAN_HIGH_3_G("New TxLM: %d, RXLM: %d, FN to Hop: %d", 
          txlm_buf_idx, rxlm_buf_idx, frame_to_hop);

    /* Stop surround cell engine to avoid any activity during the idle frame */
    case WTR_HOP_STOP_SCE:
      if (wtr_hop_status  == WTR_HOP_STOP_SCE)
      {
        if (GSTMR_GET_FN_GERAN(gas_id) == SUB_FN(frame_to_hop, NUM_FRAMES_STOP_SCE_TO_HOP))
        {
          MSG_GERAN_HIGH_1_G("Stop SCE at FN %d", GSTMR_GET_FN_GERAN(gas_id));    
          wtr_hop_restart_sce_after_hop = TRUE;
          l1_sc_stop(gas_id);
          wtr_hop_status = WTR_HOP_RF_ENTER_MODE;
        }
        break;
      }

    /* Request RF to set enter mode for the target device */
    case WTR_HOP_RF_ENTER_MODE:
      if (GSTMR_GET_FN_GERAN(gas_id) == SUB_FN(frame_to_hop, 2))
      {
        MSG_GERAN_HIGH_2_G("rf enter mode for rf device %d at FN %d", 
            target_rf_device, GSTMR_GET_FN_GERAN(gas_id));

        /* To avoid blocking ISR while waiting the confirmation for the 
         * enter mode command, the request is send in frame idle-2 and the 
         * confirmation is retrieved in frame idle-1 */
#ifdef FEATURE_GSM_RX_TX_SPLIT
        /* would need to allow device to be passed in to gl1_hw_rf_wakeup for WTR hopping to actually work */
        gl1_hw_rf_wakeup(rxlm_buf_idx, GL1_PRX_WAKEUP_MODE, gas_id);
#else
        GDRV_RFA_MSGR_LOCK(gas_id);
        gl1_hw_rf_task_enter_mode_req(rxlm_buf_idx, target_rf_device, gas_id);
#endif
        wtr_hop_status = WTR_HOP_RF_SET_TX;
      }
      break;

    case WTR_HOP_RF_SET_TX:
      if (GSTMR_GET_FN_GERAN(gas_id) == SUB_FN(frame_to_hop, 1))
      {
        MSG_GERAN_HIGH_1_G("Entering WTR_HOP_RF_SET_TX at QS %d", 
            GSTMR_RD_QSYMBOL_COUNT(geran_map_gas_id_to_nas_id(gas_id)));
#ifndef FEATURE_GSM_RX_TX_SPLIT
        /* Retrieve enter mode confirmation */
        gl1_hw_rf_task_enter_mode_cnf(gas_id);
        GDRV_RFA_MSGR_UNLOCK(gas_id);
#endif

        /* Enable TX and set tx band */
        MSG_GERAN_HIGH_1_G("Enabling tx at QS: %d", 
            GSTMR_RD_QSYMBOL_COUNT(geran_map_gas_id_to_nas_id(gas_id)));
			
		/* save the band before  rf_enable_tx_device as it
     	   will set invalid band forcefully   */	
        temp_rf_band =  gl1_hw_get_rf_gsm_band(gas_id) ;
#ifdef FEATURE_GSM_RX_TX_SPLIT
        gl1_hw_rf_tx_wakeup(txlm_buf_idx,
                            target_rf_device,
                            gas_id);
#else
        gl1_hw_rf_enable_tx_device(txlm_buf_idx, target_rf_device, gas_id);
#endif
        MSG_GERAN_HIGH_2_G("Set TX band %d, at QS: %d", temp_rf_band, 
            GSTMR_RD_QSYMBOL_COUNT(geran_map_gas_id_to_nas_id(gas_id)));

        gl1_hw_rf_set_tx_band_device(temp_rf_band, target_rf_device, gas_id);

        MSG_GERAN_HIGH_1_G("Completed set TX band at QS: %d", 
            GSTMR_RD_QSYMBOL_COUNT(geran_map_gas_id_to_nas_id(gas_id)));

        /* The actual change of device is performed in function 
         * gl1_msg_wtr_hop_idle_frame_pre_ftn */
        wtr_hop_status = WTR_HOP_CHANGE_DEVICE;
      }
      break;

    case WTR_HOP_CHANGE_DEVICE:
      if (wtr_hop_l1_state == L1_RANDOM_ACCESS_MODE)
      {
        gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id);
      }

    /* This state is executed after the change of 
     * device to deallocate unused resources */
    case WTR_HOP_CLEANUP:
      if (GSTMR_GET_FN_GERAN(gas_id) != frame_to_hop)
      {
        wtr_clean_up(gas_id);
      }
      break;

    default:
      MSG_GERAN_HIGH_1_G("ticking trm hop default, FN mod26: %d",  
          frame_counters[gas_id].FNmod26);
  }
}

/*===========================================================================

FUNCTION wtr_clean_up

DESCRIPTION
  This function de-allocate unused resources and restart the SCE.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void wtr_clean_up(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  /* RF exit mode for old RF device */
  MSG_GERAN_HIGH_1_G("Start task exit mode at QS: %d",
      GSTMR_RD_QSYMBOL_COUNT(geran_map_gas_id_to_nas_id(gas_id)));

#ifdef FEATURE_GSM_RX_TX_SPLIT
  gl1_hw_rf_tx_sleep(gl1_hw_get_txlm_buff_index(gas_id), old_rf_device, gas_id);
#else
  /* must call Tx_disable before exit to allow RF to release any FE devices */
  gl1_hw_rf_disable_tx(gl1_hw_get_txlm_buff_index(gas_id), old_rf_device, gas_id);
#endif

#ifdef FEATURE_GSM_RX_TX_SPLIT
  /* would need to allow device to be passed in to gl1_hw_rf_sleep for WTR hopping to actually work */
  gl1_hw_rf_sleep(gl1_hw_get_gsm_rxlm_buf_idx(gas_id), 
                  GL1_PRX_SLEEP_MODE, 
                  gas_id);
#else
  gl1_hw_rf_task_exit_mode(gas_id,
                           RFGSM_PRX_EXIT_MODE,
                           old_rf_device);
#endif
  MSG_GERAN_HIGH_1_G("Completed task exit mode at QS: %d", 
      GSTMR_RD_QSYMBOL_COUNT(geran_map_gas_id_to_nas_id(gas_id)));

  /* Deallocate old LM buffers */
  rflm_deallocate_rx_handle(LM_GSM, LM_GSM,  gl1_hw_get_rxlm_buff_index(gas_id));
  rflm_deallocate_tx_handle(LM_GSM, LM_GSM,  gl1_hw_get_txlm_buff_index(gas_id)); 

  /* Update global variables with new LM buffers */
  gl1_hw_set_rxlm_buff_index(rxlm_buf_idx, gas_id);
  gl1_hw_set_txlm_buff_index(txlm_buf_idx, gas_id);

  rxlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;
  txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID;

  if (wtr_hop_restart_sce_after_hop == TRUE)
  {
    MSG_GERAN_HIGH_0_G("Restarting SCE");
    /* Restart surround cell engine in dedicated mode */
    l1_sc_start(L1SCModeSameAsBefore, gas_id);
    l1_sci_go(gas_id);
  }

  /* Report successful hop to TRM */
  grm_send_wtr_hop_async_rsp(TRUE, gas_id);

  /* Update global variable tracking rf device to be used by sub */
  gl1_hw_set_gsm_rf_id(target_rf_device, 
                       l1_tskisr_blk->client_id, 
                       FALSE,
                       gas_id);

  /* Stop WTR hop state machine */
  wtr_hop_init_fn = 0x0000;
  wtr_hop_status = WTR_HOP_INACTIVE;
  gas_to_hop = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
}


/*===========================================================================

FUNCTION gl1_msg_wtr_hop_idle_frame_pre_ftn

DESCRIPTION
  This function is called every frame when in dedicated mode but return 
  immediately if the conditions are not ready for hopping the RF device.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_msg_wtr_hop_idle_frame_pre_ftn(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* Check if it is necessary to hop transceiver */
  if (gas_id != gas_to_hop ||
      wtr_hop_status != WTR_HOP_CHANGE_DEVICE ||
      GSTMR_GET_FN_GERAN(gas_id) != frame_to_hop) 
    return;

  MSG_GERAN_HIGH_2_G("WTR hop, pre FTN processing, FN mod26: %d, QS: %d", 
      frame_counters[gas_id].FNmod26, 
      GSTMR_RD_QSYMBOL_COUNT(geran_map_gas_id_to_nas_id(gas_id)));

  /* Pass new RXLM buffer to firmware */
  gl1_hw_async_intf_wtr_cfg_cmd(rxlm_buf_idx, target_rf_device, 
      geran_map_gas_id_to_nas_id(gas_id));

  /* Call FW enter mode for new device */
  gl1_hw_async_intf_enter_mode_cmd(GFW_APP_MODE_ACTIVE, 
                                   rxlm_buf_idx,
#ifdef FEATURE_GSM_RX_TX_SPLIT
                                   (uint32)-1,
                                   GFW_ENTER_MODE_PRX_BIT,
#else
                                   GFW_ENTER_MODE_PRX,
#endif
                                   target_rf_device,
                                   geran_map_gas_id_to_nas_id(gas_id));

  /* Set TX band */
  gl1_hw_async_intf_set_tx_band_cmd(geran_map_gas_id_to_nas_id(gas_id));

  /* Keep track of old rf device to call exit mode during clean up */
  old_rf_device = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);
  
  /* Update global variable tracking rf device to be used by sub */
  gl1_hw_set_gsm_rf_id(target_rf_device, 
                       l1_tskisr_blk->client_id, 
                       FALSE,
                       gas_id);

  /* Configure firmware for TX */
  gl1_hw_wtr_hop_config_tx(txlm_buf_idx, target_rf_device, gas_id);

  MSG_GERAN_HIGH_1_G("WTR hop, pre FTN processing, completed at QS: %d", 
                     GSTMR_RD_QSYMBOL_COUNT(geran_map_gas_id_to_nas_id(gas_id)));

  /* Change of the state machine to perform clean up during next frame */
  wtr_hop_status = WTR_HOP_CLEANUP;
}

/*===========================================================================

FUNCTION gl1_msg_wtr_hop_get_status

DESCRIPTION
  This function returns the current state of the WTR_HOP state machine

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
wtr_status_type gl1_msg_wtr_hop_get_status(void)
{
  return wtr_hop_status;
}

/*===========================================================================

FUNCTION gl1_msg_wtr_hop_get_gas_to_hop

DESCRIPTION
  This function returns the gas_id of the sub hopping, if no sub is
  hopping GERAN_ACCESS_STRATUM_ID_UNDEFINED is returned

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
gas_id_t gl1_msg_wtr_hop_get_gas_to_hop(void)
{
  return gas_to_hop;
}

/*===========================================================================

FUNCTION get_wtr_hop_frame_ded

DESCRIPTION
  This function calculates the FN where WTR will be hopped to the target
  device. During TCH this will be an idle frame, during SDCCH it will be the 
  first frame where there is no scheduled activity in at least the following
  three frames.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static uint32 get_wtr_hop_frame_ded(channel_information_T *chnl_1_data, gas_id_t gas_id)
{
  uint32 return_value;

  if(chnl_1_data == NULL)
  {
#ifdef FEATURE_QSH_MDUMP
   QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
   ERR_FATAL( "Channel1 Data is NULL. So crash", 0, 0, 0 );
  }
  
  if (chnl_1_data->channel_type == FULL_RATE_TRAFFIC || 
      chnl_1_data->channel_type == HALF_RATE_TRAFFIC)
  {
    int idle_mod26 = l1_sc_get_tch_idle_frame(gas_id);

    if (frame_counters[gas_id].FNmod26 < idle_mod26 &&
        idle_mod26 - frame_counters[gas_id].FNmod26 > NUM_FRAMES_STOP_SCE_TO_HOP)
    {
      return_value = GSTMR_GET_FN_GERAN(gas_id) + idle_mod26 - 
        frame_counters[gas_id].FNmod26;
    }
    else
    {
      return_value = GSTMR_GET_FN_GERAN(gas_id) + idle_mod26 - 
        frame_counters[gas_id].FNmod26 + 26;
    }
  }

  if (chnl_1_data->channel_type == SDCCH_4 || 
      chnl_1_data->channel_type == SDCCH_8)
  {
    return_value = get_wtr_hop_frame_sdcch(chnl_1_data, gas_id);
  }
  return return_value;
}

/*===========================================================================

FUNCTION get_wtr_hop_frame_transfer

DESCRIPTION
  This function calculates the FN where WTR will be hopped to the target
  device. During Transfer Mode this will be an idle frame.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static uint32 get_wtr_hop_frame_transfer(gas_id_t gas_id)
{
  int idle_mod26 = 25;

  if (frame_counters[gas_id].FNmod26 < idle_mod26 &&
      idle_mod26 - frame_counters[gas_id].FNmod26 > NUM_FRAMES_STOP_SCE_TO_HOP)
  {
    return GSTMR_GET_FN_GERAN(gas_id) + idle_mod26 - 
      frame_counters[gas_id].FNmod26;
  }
  else
  {
    return GSTMR_GET_FN_GERAN(gas_id) + idle_mod26 - 
      frame_counters[gas_id].FNmod26 + 26;
  }
}

/*===========================================================================

FUNCTION get_wtr_hop_frame_sdcch

DESCRIPTION
  This function searches the SDCCH tables to determine the first frame
  where there are at least three free frames until next activity

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static uint32 get_wtr_hop_frame_sdcch(channel_information_T *chnl_1_data, gas_id_t gas_id)
{
  int lookup_FN;
  byte frames_to_next_activity = 0;
  uint32 fn_mod_102;
  int offset = 0;

  /* calculate index of current frame in ded sdcch table */
  /* Note that table is advanced by 1 frames wrt real 102 multiframe */
  fn_mod_102 = SUB_MOD(frame_counters[gas_id].FNmod102, 1, 102);

  if (chnl_1_data->channel_type == SDCCH_8)
  {
    /* Boundary check for subchannel */
    if (chnl_1_data->subchannel > 7)
    {
      chnl_1_data->subchannel = 7;
    }

    lookup_FN = (fn_mod_102 + (((7 - chnl_1_data->subchannel) % 4) << 2)) % 102;

    while (frames_to_next_activity < NUM_FRAMES_STOP_SCE_TO_HOP - 1)
    {
      frames_to_next_activity = 
        ded_sdcch_8_table[ADD_MOD(lookup_FN, offset, 102)][chnl_1_data->subchannel >> 2][1];

      if (frames_to_next_activity < NUM_FRAMES_STOP_SCE_TO_HOP - 1) offset++;
    }

    MSG_GERAN_MED_2_G("SDCCH8, sub-channel %d, gap to next activity: %d",
                      chnl_1_data->subchannel, frames_to_next_activity);
  }
  else if (chnl_1_data->channel_type == SDCCH_4)
  {

    /* Boundary check for subchannel */
    if (chnl_1_data->subchannel > 3)
    {
      chnl_1_data->subchannel = 3;
    }

    while (frames_to_next_activity < NUM_FRAMES_STOP_SCE_TO_HOP - 1)
    {
      frames_to_next_activity = ded_sdcch_4_table[ADD_MOD(fn_mod_102, offset, 102)][chnl_1_data->subchannel][1];
      if (frames_to_next_activity < NUM_FRAMES_STOP_SCE_TO_HOP - 1) offset++;
    }
   
    MSG_GERAN_MED_1_G("SDCCH4 channel, gap to next activity: %d", frames_to_next_activity);
  }

  return ADD_FN(GSTMR_GET_FN_GERAN(gas_id), offset + NUM_FRAMES_STOP_SCE_TO_HOP);
}

/*===========================================================================

FUNCTION get_wtr_hop_frame_access

DESCRIPTION
  This function searches for the next FCCH SCH frames to perform hop
  while decoding AGCH in access mode

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static uint32 get_wtr_hop_frame_access(gas_id_t gas_id)
{
  uint32 offset;
  uint32 fn_mod_51;
  uint32 fn_mod_51_mod_10;

  fn_mod_51 = frame_counters[gas_id].FNmod51;
  fn_mod_51_mod_10 = fn_mod_51 % 10;

  /* Calculate the frame number of the following 
   * FCCH frame (frame 0, 10, 20, 30, 40) */
  if (fn_mod_51_mod_10 <= 8 && fn_mod_51_mod_10 > 0)
  {
    offset = 10 - fn_mod_51_mod_10;
  }
  else
  {
    offset = 20 - fn_mod_51_mod_10;
  }

  /* Take into account that 51 is not multiple of 10
   * add one, otherwise will attempt to hop in the idle
   * frame (50) or in the frame before next FCCH */
  /*Change from 40 to 39 as for FNmod51 = 39 also, offset needs to be increased*/
  if (fn_mod_51 >= 39) offset++;

  return ADD_FN(GSTMR_GET_FN_GERAN(gas_id), offset);
}

/*===========================================================================

FUNCTION gl1_msg_wtr_hop_abort

DESCRIPTION
  This function aborts a hop in progress

DEPENDENCIES
  None.

RETURN VALUE
  Return TRUE if abort was successful or no hop is in progress
  Return FALSE to indicate that wtr hop state machine still needs to be ticked 

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_msg_wtr_hop_abort(boolean restart_sce, gas_id_t gas_id)
{
  if (wtr_hop_status == WTR_HOP_INACTIVE || gas_id != gas_to_hop) return TRUE;

  wtr_hop_restart_sce_after_hop = restart_sce;

  switch (wtr_hop_status)
  {
    case WTR_HOP_CLEANUP:
      MSG_GERAN_HIGH_0_G("WTR abort run wtr_clean_up");
      wtr_clean_up(gas_id);
      break;

    case WTR_HOP_CHANGE_DEVICE:
    case WTR_HOP_RF_SET_TX:
        MSG_GERAN_HIGH_2_G("WTR abort can not abort in state: %d, restart sce after abort: %d", 
            wtr_hop_status, wtr_hop_restart_sce_after_hop);
        /* Return false to indicate that wtr hop state machine still needs to be ticked */
        return FALSE;

    case WTR_HOP_RF_ENTER_MODE:

      if (wtr_hop_restart_sce_after_hop == TRUE)
      {
        MSG_GERAN_HIGH_0_G("WTR abort Restart SCE");
        /* Restart surround cell engine in dedicated mode */
        l1_sc_start(L1SCModeSameAsBefore, gas_id);
        l1_sci_go(gas_id);
      }
      else
      {
        MSG_GERAN_HIGH_0_G("WTR abort NOT Restarting SCE");
      }

    case WTR_HOP_STOP_SCE:
      MSG_GERAN_HIGH_0_G("WTR abort deallocate txlm");
      rflm_deallocate_tx_handle(LM_GSM, LM_GSM, txlm_buf_idx);
      txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID;

    case WTR_HOP_ALLOC_TXLM:
      MSG_GERAN_HIGH_0_G("WTR abort deallocate rxlm");
      rflm_deallocate_rx_handle(LM_GSM, LM_GSM, rxlm_buf_idx);
      rxlm_buf_idx = MDSP_RXLM_BUF_IDX_INVALID;

    case WTR_HOP_ALLOC_RXLM:

    case WTR_HOP_INIT:
      MSG_GERAN_HIGH_0_G("WTR abort complete");

      /* Report HOP was declined */
      grm_send_wtr_hop_async_rsp(FALSE, gas_id);

      /* Stop WTR hop state machine */
      wtr_hop_init_fn = 0x0000;
      wtr_hop_status = WTR_HOP_INACTIVE;
      gas_to_hop = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
      break;

    default:
      MSG_GERAN_HIGH_1_G("Can not abort wtr hop in state: %d", wtr_hop_status);
  }
  return TRUE;
}
/*===========================================================================

FUNCTION wtr_hop_in_progress

DESCRIPTION
  This function checks wtr hop in progress

DEPENDENCIES
  None.

RETURN VALUE
  Return TRUE if wtr hop in progress
  Return FALSE to indicate that wtr hop state machine still needs to be ticked 

SIDE EFFECTS
  None.

===========================================================================*/

boolean wtr_hop_in_progress (gas_id_t gas_id )
{
  boolean  hop_in_progress = FALSE ;
  if (((wtr_hop_status == WTR_HOP_CHANGE_DEVICE) ||(wtr_hop_status == WTR_HOP_RF_SET_TX))
       &&(gas_id == gl1_msg_wtr_hop_get_gas_to_hop()))
  {
     hop_in_progress = TRUE ;
  }
  return hop_in_progress;
}

/*===========================================================================

FUNCTION gl1_msg_wtr_hop_rude_reset

DESCRIPTION
  This function performs a rude reset of WTR Hop state machine and sends an ind to TR about it

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_msg_wtr_hop_rude_reset(gas_id_t gas_id)
{
  if (gas_id == gas_to_hop)
  {
    reset_wtr_hop_globals();
    MSG_GERAN_ERROR_0_G("WTR Hop Rude Reset");
  }
}

/*===========================================================================

FUNCTION gl1_msg_wtr_hop_do_not_restart_sce

DESCRIPTION
  Call this function to prevent restarting SCE after hop is complete

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_msg_wtr_hop_do_not_restart_sce(gas_id_t gas_id)
{
  if (gas_id == gas_to_hop)
  {
    wtr_hop_restart_sce_after_hop = FALSE;
  }
}
#endif /* FEATURE_GSM_WTR_HOP */
