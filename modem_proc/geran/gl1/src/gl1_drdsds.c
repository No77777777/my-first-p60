/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     GERAN L1 DRDSDS FILE

GENERAL DESCRIPTION
This module handles DRDSDS specific functions.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

            EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_drdsds.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when      who       what, where, why
--------  --------  ---------------------------------------------------------- 
30/06/15   ab       CR851337 : DR-DSDS. Design to allow SR(GBTA) to DR Transition.
20/04/15   sjv      CR823566  Set number of TRM denials/Xcch failures count through debug NV 
07/04/15   sjv      CR817252  Fallback from DR->SR happens after 3 TRM denials for G+G as well similar to X+G 
27/03/15   dv/pa    CR813849: In X+G DR-DSDS, revert back to SR after 3 consequtive TRM denials 
10/03/15   sjv      CR805970: SCell RSSI Threshold set through a new NV for DR to SR Fallback
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
04/02/15   sjv      CR790894: Disable DR->SR Fallback using mask set for NV 65770 (l1_debug)
29/01/15   pa       CR785021: Disable DR on TRM denial to activate GBTA on next PCH decode.
05/01/15   pa       CR774166: Enable DR on inactivating GL1 task.
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   ws       CR768376 Q6 compile warning fixes
14/11/14   pa       CR756157: Keep DR enabled for fourth Paging block in Tuneaway Mode. 
21/10/14   pa       CR743690: Maintain bad paging block counter on transition from LP to HP state. 
17/10/14   cs       CR735918: Assign supported bands for initial ACQ based on UE support
08/10/14   pa       CR732823: Hybrid Tuneaway Fallback: Avg RSSI is an avergae of last 3 PCH blocks.
25/09/14   pa       CR719024: Part2: Disable DRDSDS on receiving DSC_IND in DUAL_RX Tuneaway Mode.
09/09/14   pa       CR719024: Implementation of Hybrid Tuneaway Fallback Algorithm

===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "ms.h"

#include "l1_isr.h"
#include "gl1_drdsds.h"
#include "gpl1_dual_sim.h"
#include "l1_sc_int.h"
#include "l1_task.h"
#include "gl1_mutex.h"
#include "gmutex.h"
#include "gpl1_dual_sim.h"

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
/*===============DUAL DATA VARIABLES DECLARED HERE================*/


/*===============EXTERNALISED FUNCTION DECLARATIONS================*/

/*===============FUNCTION DEFINITIONS================*/

/*===========================================================================

  FUNCTION  gl1_drdsds_set_hybrid_tuneaway_state

  DESCRIPTION
  This function sets hybrid tuneaway state based upon dr status.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_set_hybrid_tuneaway_state(grm_client_enum_t client_id,
                                          gas_id_t gas_id )
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
    /* DR is enabled and supported by UE */
    if ( grm_info_p->dr_enabled && grm_device_config_is_drdsds() )
    {
      gl1_drdsds_hybrid_tuneaway_state prev_state = grm_info_p->hybrid_tuneaway_state;
      if(grm_info_p->dualRx_request) 
      {
        grm_info_p->hybrid_tuneaway_state = DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE;
        grm_info_p->dualRx_request = FALSE;
      }
      else
      {
        /* Set state based upon rxd_priority.
          RxD priority is set on handling of Paging Block and DSC Threshold */
        if (DRDSDS_RXD_HP == grm_info_p->rxd_priority)
        {
          grm_info_p->hybrid_tuneaway_state = DRDSDS_HIGH_PRIORITY_MODE_STATE;
        }
        else
        {
          grm_info_p->hybrid_tuneaway_state = DRDSDS_LOW_PRIORITY_MODE_STATE;
        }
      }

      /* Maintain bad paging block counter when move from LP to HP state, else reset*/
      if ((DRDSDS_LOW_PRIORITY_MODE_STATE != prev_state) ||
          (DRDSDS_HIGH_PRIORITY_MODE_STATE != grm_info_p->hybrid_tuneaway_state)) 
      {
        grm_info_p->bad_paging_block_count = 0;
        grm_info_p->trm_denied_count = 0;
      }
    }
    else
    {
      grm_info_p->hybrid_tuneaway_state = DRDSDS_TUNEAWAY_MODE_STATE;

      /* Reset paging block counts */
      grm_info_p->paging_block_count = 0;
      grm_info_p->bad_paging_block_count = 0;
      grm_info_p->dualRx_request = FALSE;
      grm_info_p->trm_denied_count = 0;

      /*Reset dual Rx SNR*/
      grm_info_p->dualRx_SNR = GL1_DRDSDS_SNR_THRESHOLD;
    }

    MSG_GERAN_HIGH_1_G( GL1_DRDS_DBG_HDR"Hybrid Tuneaway State:%d",
                        grm_info_p->hybrid_tuneaway_state );
  }

  return;
}

/*===========================================================================

  FUNCTION  gl1_drdsds_hybrid_tuneaway_trigger

  DESCRIPTION
  This function triggers hybrid tuneaway state machine.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_hybrid_tuneaway_trigger(gl1_drdsds_hybrid_tuneaway_event event,
                                        gl1_drdsds_hybrid_tuneaway_msg *msg,
                                        grm_client_enum_t client_id,
                                        gas_id_t gas_id)
{
  /* Only valid if UE configured as DR-DSDS capable */
  if (grm_device_config_is_drdsds() && !GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_DISABLE_HYBRID_FB_FRM_DR))
  {
    switch (event)
    {
      case DRDSDS_IDLE_INIT_EVENT:
        gl1_drdsds_handle_idle_init_event(client_id, gas_id);
        break;

      case DRDSDS_MPH_BLOCK_QUALITY_IND_EVENT:
        gl1_drdsds_handle_blk_quality_ind_event((mph_block_quality_ind_T *)msg,
                                                client_id,
                                                gas_id);
        break;

      case DRDSDS_MPH_DSC_THRESHOLD_IND_EVENT:
        gl1_drdsds_handle_dsc_threshold_ind_event(client_id, gas_id);
        break;

      case DRDSDS_STOP_GSM_EVENT:
        gl1_drdsds_handle_stop_gsm_event(client_id, gas_id);
        break;

      case DRDSDS_TRM_DENIED_EVENT:
        gl1_drdsds_handle_trm_denial_event(client_id, gas_id);
        break;

      default:
        MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR"Invalid Hybrid Tuneaway Event %d",
                           event);
        break;
    }
  }
  else
  {
     MSG_GERAN_MED_2_G(GL1_DRDS_DBG_HDR"grm_device_config_is_drdsds:%d "
                                       "GL1_EFS_DEBUG_DISABLE_HYBRID_FB_FRM_DR:%d",
                       grm_device_config_is_drdsds(), 
                       GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_DISABLE_HYBRID_FB_FRM_DR));
  }
  return;
}

/*===========================================================================

  FUNCTION  gl1_drdsds_handle_idle_init_event

  DESCRIPTION
  This function handles DRDSDS_IDLE_INIT_EVENT in all states.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_handle_idle_init_event(grm_client_enum_t client_id, 
                                       gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if(grm_info_p)
  {
    MSG_GERAN_HIGH_3_G(GL1_DRDS_DBG_HDR"gl1_drdsds_handle_idle_init_event "
                                       "dr_enabled %d pending_dr_state %d state %d",
                                       grm_info_p->dr_enabled,
                                       pending_dr_state,
                                       grm_info_p->hybrid_tuneaway_state);

    switch (grm_info_p->hybrid_tuneaway_state)
    {
      case DRDSDS_TUNEAWAY_MODE_STATE:
      case DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE:
      {
        /* If other SUB is in transfer mode, then don't do anything
        Other SUB will change DRDSDS status when it moves into Idle state*/
        if(FALSE == OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
        {
          (void)grm_enable_disable_dual_receive(TRUE, client_id, gas_id);

          /* Reset pending state*/
          pending_dr_state = PENDING_DR_NULL;
        }
        /* Idle Init: reset paging block count*/
        grm_info_p->paging_block_count = 0;
        break;
      }

      case DRDSDS_LOW_PRIORITY_MODE_STATE:
      case DRDSDS_HIGH_PRIORITY_MODE_STATE:
        /* In DRDSDS mode, on entering idle mode reset bad paging block count*/
        grm_info_p->bad_paging_block_count = 0;
        grm_info_p->trm_denied_count = 0;
        break;

      default:
        break;
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_handle_blk_quality_ind_event

  DESCRIPTION
  This function handles DRDSDS_MPH_BLOCK_QUALITY_IND_EVENT in all states.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_handle_blk_quality_ind_event(mph_block_quality_ind_T *msg,
                                             grm_client_enum_t client_id,
                                             gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if(grm_info_p && msg)
  {
    /*Paging Block received*/
    grm_info_p->trm_denied_count = 0;

    switch (grm_info_p->hybrid_tuneaway_state)
    {
      case DRDSDS_TUNEAWAY_MODE_STATE:
        gl1_drdsds_process_blk_ind_tuneaway_mode(client_id, gas_id);
        break;
      case DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE:
        gl1_drdsds_process_blk_ind_dual_rx_tuneaway_mode(msg->block_quality, 
                                                         client_id, 
                                                         gas_id);
        break;
      case DRDSDS_LOW_PRIORITY_MODE_STATE:
        gl1_drdsds_process_blk_ind_lp_mode(msg, client_id, gas_id);
        break;
      case DRDSDS_HIGH_PRIORITY_MODE_STATE:
        gl1_drdsds_process_blk_ind_hp_mode(msg, client_id, gas_id);
        break;
      default:
        break;
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_process_blk_ind_tuneaway_mode

  DESCRIPTION
  This function processes MPH_BLOCK_QUALITY_IND in DRDSDS_TUNEAWAY_MODE_STATE.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_blk_ind_tuneaway_mode(grm_client_enum_t client_id,
                                              gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if (grm_info_p)
  {
    if (gl1_drds_debug(gas_id))
    {
      MSG_GERAN_HIGH_3_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_blk_ind_tuneaway_mode "
                                          "dr_enabled %d paging_block_count %d "
                                          "dualRxSNR %d",
                                          grm_info_p->dr_enabled,
                                          grm_info_p->paging_block_count,
                                          grm_info_p->dualRx_SNR);
    }
    /* Increment paging block_count irrespective of Good, Bad, Unknown*/
    grm_info_p->paging_block_count++;

    /* After third paging block, enable DRDSDS*/
    if(GL1_DRDSDS_THIRD_PAGING_BLOCK == grm_info_p->paging_block_count)
    {
      /* Ignore if other sub is in transfer, reset paging_block_count*/
      if(FALSE == OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
      {
        /*Enable DRDSDS: Schedule 1 page in 4 in DRDSDS mode*/
        MSG_GERAN_HIGH_0_G(GL1_DRDS_DBG_HDR"Changing state to DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE");
        grm_info_p->dualRx_request = TRUE;
        grm_info_p->rxd_priority = DRDSDS_RXD_NULL;

        (void)grm_enable_disable_dual_receive(TRUE, client_id, gas_id);
      }
      else
      {
        /* When other sub is in transfer, don't enable/disable DRDSDS
          Reset paging block count*/
        grm_info_p->paging_block_count = 0;
      }
      /*Reset dual Rx SNR*/
      grm_info_p->dualRx_SNR = GL1_DRDSDS_SNR_THRESHOLD;
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_process_blk_ind_dual_rx_tuneaway_mode

  DESCRIPTION
  This function processes MPH_BLOCK_QUALITY_IND in DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE.
  As per the design, GL1 shall disable DRDSDS after decoding 4th Paging Block and enable
  DRDSDS on receiving DSC_IND if DSC condition matches. Disabling and Enabling of DRDSDS is
  very close to each other and TRM takes 9ms to 18ms to process the request.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_blk_ind_dual_rx_tuneaway_mode(gl1_block_quality_t blk_quality,
                                                      grm_client_enum_t client_id,
                                                      gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  l1_serving_cell_meas_T *l1_serving_cell_meas_ptr = &l1_serving_cell_meas[gas_id];

  if (grm_info_p)
  {
    if (gl1_drds_debug(gas_id))
    {
      MSG_GERAN_HIGH_5_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_blk_ind_dual_rx_tuneaway_mode "
                                          "dr_enabled %d paging_block_count %d "
                                          "dualRxSNR %d serving cell snr %d "
                                          "blk_quality %d",
                                          grm_info_p->dr_enabled,
                                          grm_info_p->paging_block_count,
                                          grm_info_p->dualRx_SNR,
                                          l1_serving_cell_meas_ptr->snr,
                                          blk_quality);
    }
    /*Increment paging block_count irrespective of Good, Bad, Unknown*/
    grm_info_p->paging_block_count++;

    /* For every good/bad PCH block, GL1 receives DSC_IND from RR. If unknown PCH block
      is detected then disable DRDSDS otherwise handle enabling/disabling of DRDSDS on receiving DSC_IND*/
    if (GL1_DRDSDS_FOURTH_PAGING_BLOCK == grm_info_p->paging_block_count)
    {
      /* Save SNR calculated on 4th PB*/
      grm_info_p->dualRx_SNR = l1_serving_cell_meas_ptr->snr;

      /*Disable DRDSDS: Schedule 1 page in 4 in DRDSDS mode*/
      /* Unknown block quality doesn't trigger RR to send DSC_IND. Therefore disable DRDSDS*/
      if(GL1_BLOCK_QUALITY_UNKNOWN == blk_quality)
      {
        MSG_GERAN_HIGH_0_G(GL1_DRDS_DBG_HDR"Changing state to DRDSDS_TUNEAWAY_MODE_STATE");
        grm_info_p->dualRx_request = FALSE;
        (void)grm_enable_disable_dual_receive(FALSE, client_id, gas_id);
      }

      /* After fourth paging block, reset paging block count */
      grm_info_p->paging_block_count = 0;
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_process_blk_ind_lp_mode

  DESCRIPTION
  This function processes DRDSDS_MPH_BLOCK_QUALITY_IND_EVENT in
  DRDSDS_LOW_PRIORITY_MODE_STATE.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_blk_ind_lp_mode(mph_block_quality_ind_T *msg,
                                        grm_client_enum_t client_id,
                                        gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if (grm_info_p && msg)
  {
    if (gl1_drds_debug(gas_id)) 
    {
      MSG_GERAN_HIGH_4_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_blk_ind_lp_mode "
                                          "dr_enabled %d bad_paging_block_count %d "
                                          "pending_dr_state %d paging_block_count %d", 
                                          grm_info_p->dr_enabled,
                                          grm_info_p->bad_paging_block_count,
                                          pending_dr_state,
                                          grm_info_p->paging_block_count); 
    }

    if (GL1_BLOCK_QUALITY_BAD == msg->block_quality)
    {
      /* PCH decode failure, increment bad count*/
      grm_info_p->bad_paging_block_count++;

      /* ccch_metrics hasn't processed yet. Wait for MPH_DSC_THRESHOLD_IND
         and checks 3 consecutive PCH failed condition during the
         handling of MPH_DSC_THRESHOLD_IND */
    }
    else if(GL1_BLOCK_QUALITY_GOOD == msg->block_quality)
    {
      /* Reset bad paging block count on receiving Good*/
      grm_info_p->bad_paging_block_count = 0;
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_process_blk_ind_hp_mode

  DESCRIPTION
  This function processes DRDSDS_MPH_BLOCK_QUALITY_IND_EVENT in
  DRDSDS_HIGH_PRIORITY_MODE_STATE.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_blk_ind_hp_mode(mph_block_quality_ind_T *msg,
                                        grm_client_enum_t client_id,
                                        gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if (grm_info_p && msg)
  {
    if (gl1_drds_debug(gas_id))
    {
      MSG_GERAN_HIGH_4_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_blk_ind_hp_mode "
                                          "dr_enabled %d bad_paging_block_count %d "
                                          "pending_dr_state %d paging_block_count %d", 
                                          grm_info_p->dr_enabled,
                                          grm_info_p->bad_paging_block_count,
                                          pending_dr_state,
                                          grm_info_p->paging_block_count); 
    }
    if (GL1_BLOCK_QUALITY_BAD == msg->block_quality)
    {
      /* PCH decode failure, increment bad count*/
      grm_info_p->bad_paging_block_count++;

      /* ccch_metrics hasn't processed yet. Wait for MPH_DSC_THRESHOLD_IND
         and checks 3 consecutive PCH failed condition during the
         handling of MPH_DSC_THRESHOLD_IND */
    }
    else if(GL1_BLOCK_QUALITY_GOOD == msg->block_quality)
    {
      grm_info_p->bad_paging_block_count = 0;
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_handle_dsc_threshold_ind_event

  DESCRIPTION
  This function handles DRDSDS_MPH_DSC_THRESHOLD_IND_EVENT in all states.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_handle_dsc_threshold_ind_event(grm_client_enum_t client_id,
                                               gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if(grm_info_p)
  {
    switch (grm_info_p->hybrid_tuneaway_state)
    {
      case DRDSDS_TUNEAWAY_MODE_STATE:
        gl1_drdsds_process_dsc_ind_tuneaway_mode(client_id, gas_id);
        break;
      case DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE:
        gl1_drdsds_process_dsc_ind_dual_rx_tuneaway_mode(client_id, gas_id);
        break;
      case DRDSDS_LOW_PRIORITY_MODE_STATE:
        gl1_drdsds_process_dsc_ind_lp_mode(client_id, gas_id);
        break;
      case DRDSDS_HIGH_PRIORITY_MODE_STATE:
        gl1_drdsds_process_dsc_ind_hp_mode(client_id, gas_id);
        break;
      default:
        break;
    }
  }
  return;
}

/*===========================================================================

  FUNCTION  gl1_drdsds_process_dsc_ind_tuneaway_mode

  DESCRIPTION
  This function processes DRDSDS_MPH_DSC_THRESHOLD_IND_EVENT in
  DRDSDS_TUNEAWAY_MODE_STATE.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_dsc_ind_tuneaway_mode(grm_client_enum_t client_id,
                                              gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  gl1_drdsds_hybrid_tuneaway_state next_state = DRDSDS_TUNEAWAY_MODE_STATE;
  gl1_drdsds_drmode_rxd_priority rxd_priority = DRDSDS_RXD_NULL;

  if (grm_info_p)
  {
    if (gl1_drds_debug(gas_id))
    {
      MSG_GERAN_HIGH_5_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_tuneaway_mode "
                                          "state %d dr_enabled %d paging_block_count %d"
                                          "dsc threshold %d snr %d",
                                          grm_info_p->hybrid_tuneaway_state,
                                          grm_info_p->dr_enabled,
                                          grm_info_p->paging_block_count,
                                          grm_info_p->dsc_threshold,
                                          grm_info_p->dualRx_SNR);
    }

    if (GERAN_DSC_THRESHOLD_0 == grm_info_p->dsc_threshold)
    {
      next_state = DRDSDS_LOW_PRIORITY_MODE_STATE;
      rxd_priority = DRDSDS_RXD_LP;
      grm_info_p->dualRx_request = FALSE;
      grm_info_p->dualRx_SNR = GL1_DRDSDS_SNR_THRESHOLD;
    }
    else if ((grm_info_p->dsc_threshold > GERAN_DSC_THRESHOLD_3) &&
          (grm_info_p->dualRx_SNR > GL1_DRDSDS_SNR_THRESHOLD))
    {
      next_state = DRDSDS_HIGH_PRIORITY_MODE_STATE;
      rxd_priority = DRDSDS_RXD_HP;
      grm_info_p->dualRx_request = FALSE;
      /*Reset dual Rx SNR*/
      grm_info_p->dualRx_SNR = GL1_DRDSDS_SNR_THRESHOLD;
    }
    /* Allows transition from SR to DR in G+G when in  SR-DSDS state */
    /* For X+G -> OTHER_GSM_CLIENT_IN_TRANSFER(gas_id) return is = FALSE */
    else if ((OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)) &&
             (grm_info_p->dsc_threshold > GERAN_DSC_THRESHOLD_3))
    {
      next_state = DRDSDS_HIGH_PRIORITY_MODE_STATE;
      rxd_priority = DRDSDS_RXD_HP;
      grm_info_p->dualRx_request = FALSE;
      /*Reset dual Rx SNR*/
      grm_info_p->dualRx_SNR = GL1_DRDSDS_SNR_THRESHOLD;
    }

    if (DRDSDS_TUNEAWAY_MODE_STATE != next_state)
    {
      if (OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
      {
        /* Set DRDSDS pending state to Enable*/
        pending_dr_state = PENDING_DR_ENABLE;
        /*Set Rxd Priority*/
        grm_info_p->rxd_priority = rxd_priority;
      }
      else
      {
        (void)grm_enable_disable_dual_receive(TRUE, client_id, gas_id);
      }
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_process_dsc_ind_dual_rx_tuneaway_mode

  DESCRIPTION
  This function processes DRDSDS_MPH_DSC_THRESHOLD_IND_EVENT in
  DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_dsc_ind_dual_rx_tuneaway_mode(grm_client_enum_t client_id,
                                                      gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  /*Initialise with current state*/
  gl1_drdsds_hybrid_tuneaway_state next_state = DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE;
  gl1_drdsds_drmode_rxd_priority rxd_priority = DRDSDS_RXD_NULL;

  if (grm_info_p)
  {
    if (gl1_drds_debug(gas_id))
    {
      MSG_GERAN_HIGH_5_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_dual_rx_tuneaway_mode "
                                          "state %d dr_enabled %d paging_block_count %d"
                                          "dsc threshold %d snr %d",
                                          grm_info_p->hybrid_tuneaway_state,
                                          grm_info_p->dr_enabled,
                                          grm_info_p->paging_block_count,
                                          grm_info_p->dsc_threshold,
                                          grm_info_p->dualRx_SNR);
    }

    if (GERAN_DSC_THRESHOLD_0 == grm_info_p->dsc_threshold)
    {
      next_state = DRDSDS_LOW_PRIORITY_MODE_STATE;
      rxd_priority = DRDSDS_RXD_LP;
    }
    else if ((grm_info_p->dsc_threshold > GERAN_DSC_THRESHOLD_3) &&
          (grm_info_p->dualRx_SNR > GL1_DRDSDS_SNR_THRESHOLD))
    {
      next_state = DRDSDS_HIGH_PRIORITY_MODE_STATE;
      rxd_priority = DRDSDS_RXD_HP;
    }
    else if(GL1_DRDSDS_THIRD_PAGING_BLOCK == grm_info_p->paging_block_count)
    {
      /*Scenario: DR has already enabled for 4th Paging block i.e. 
        call back has been received before DSC_IND of 3rd Paging block*/
      /* do nothing*/
    }
    else
    {
      next_state = DRDSDS_TUNEAWAY_MODE_STATE;
      rxd_priority = DRDSDS_RXD_NULL;
    }

    /* Reset dualRx SNR*/
    grm_info_p->dualRx_request = FALSE;
    grm_info_p->dualRx_SNR = GL1_DRDSDS_SNR_THRESHOLD;

    if (next_state != grm_info_p->hybrid_tuneaway_state) 
    {
      /* Threshold conditions are not meeting. Disbale DRDSDS for 5th PB*/
      if (DRDSDS_TUNEAWAY_MODE_STATE == next_state)
      {
        /*Disable DRDSDS */
        MSG_GERAN_HIGH_0_G(GL1_DRDS_DBG_HDR"Changing state to DRDSDS_TUNEAWAY_MODE_STATE");
        (void)grm_enable_disable_dual_receive(FALSE, client_id, gas_id);
      }
      else 
      {
        if (gl1_drds_debug(gas_id))
        {
          MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_dual_rx_tuneaway_mode "
                                               "rxd_priority %d", rxd_priority);
        }

        /* Set Rxd Priority*/
        grm_info_p->rxd_priority = rxd_priority;
        /* DRDSDS is already enabled. Change state based upon Rxd priority*/
        gl1_drdsds_set_hybrid_tuneaway_state(client_id, gas_id);
      }
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_process_dsc_ind_lp_mode

  DESCRIPTION
  This function processes DRDSDS_MPH_DSC_THRESHOLD_IND_EVENT in
  DRDSDS_LOW_PRIORITY_MODE_STATE.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_dsc_ind_lp_mode(grm_client_enum_t client_id,
                                        gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  dBx16_T rssi = gsm_dr_to_sr_falback_rssi_thresh_dBmx16(gas_id);
  dBx16_T rssi_thresh = rssi;
  gl1_drdsds_hybrid_tuneaway_state next_state = DRDSDS_LOW_PRIORITY_MODE_STATE;

  if (grm_info_p)
  {
    if (gl1_drds_debug(gas_id))
    {
      MSG_GERAN_HIGH_4_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_lp_mode "
                                          "dr_enabled %d dsc_threshold %d "
                                          "bad_paging_block_count %d "
                                          "trm_den_or_bad_page_cnt_set %d",
                                          grm_info_p->dr_enabled,
                                          grm_info_p->dsc_threshold,
                                          grm_info_p->bad_paging_block_count,
                                          grm_info_p->trm_den_or_bad_page_cnt_set); 
    }

    /* Three consecutive Bad PCH blocks and rssi is greater than threshold*/
    if (grm_info_p->trm_den_or_bad_page_cnt_set == grm_info_p->bad_paging_block_count)
    {
      next_state = DRDSDS_TUNEAWAY_MODE_STATE;
    }
    else if (grm_info_p->dsc_threshold < GERAN_DSC_THRESHOLD_2) 
    {
      next_state = DRDSDS_HIGH_PRIORITY_MODE_STATE;
    }

    if (DRDSDS_LOW_PRIORITY_MODE_STATE != next_state) 
    {
      /* MPH_DSC_THRESHOLD_IND is handled in task's context*/
      /* Lock will ensure power level of last PCH block has been calculated*/
      GL1_ISR_LOCK(gas_id);
      rssi = gl1_drdsds_get_average_rssi(GL1_DRDSDS_AVG_NUM_OF_RSSI, gas_id);
      GL1_ISR_UNLOCK(gas_id);

      if (gl1_drds_debug(gas_id)) 
      {
        MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_lp_mode rssi %d", rssi);
      }

      /* RSSI is greater than threshold, change state depend upon the next_state*/
      if (rssi > rssi_thresh)
      {
        switch (next_state) 
        {
          case DRDSDS_TUNEAWAY_MODE_STATE:
          {
            /*Disable DRDSDS: three consecuitve bad quality paging blocks*/
            (void)grm_enable_disable_dual_receive(FALSE, client_id, gas_id);
            break;
          }

          case DRDSDS_HIGH_PRIORITY_MODE_STATE:
          {
            grm_info_p->rxd_priority = DRDSDS_RXD_HP;
            /*Reset dual Rx SNR*/
            grm_info_p->dualRx_SNR = GL1_DRDSDS_SNR_THRESHOLD;
            /* DRDSDS is already enabled. Change state based upon Rxd priority*/
            gl1_drdsds_set_hybrid_tuneaway_state(client_id, gas_id);
            break;
          }

          default:
            break;
        }
      }
      else
      {
        if (grm_info_p->trm_den_or_bad_page_cnt_set == grm_info_p->bad_paging_block_count)
        {
          /* Discard the last third bad paging block*/
          /* so that next time GL1 will consider last two and next paging block*/
          grm_info_p->bad_paging_block_count--;
          if (gl1_drds_debug(gas_id)) 
          {
            MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_lp_mode discard last "
                                               "bad_paging_block_count %d ", 
                                                grm_info_p->bad_paging_block_count);
          }
        }
      }
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_process_dsc_ind_hp_mode

  DESCRIPTION
  This function handles DRDSDS_MPH_DSC_THRESHOLD_IND_EVENT in
  DRDSDS_HIGH_PRIORITY_MODE_STATE.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_dsc_ind_hp_mode(grm_client_enum_t client_id,
                                        gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);
  dBx16_T rssi = gsm_dr_to_sr_falback_rssi_thresh_dBmx16(gas_id);
  dBx16_T rssi_thresh = rssi;

  if (grm_info_p) 
  {
    if (gl1_drds_debug(gas_id)) 
    {
      MSG_GERAN_HIGH_4_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_hp_mode "
                                          "dr_enabled %d dsc_threshold %d "
                                          "bad_paging_block_count %d "
                                          "trm_den_or_bad_page_cnt_set %d",
                                          grm_info_p->dr_enabled,
                                          grm_info_p->dsc_threshold,
                                          grm_info_p->bad_paging_block_count,
                                          grm_info_p->trm_den_or_bad_page_cnt_set);
    }

    /* Three consecutive Bad PCH blocks or DSC < TH5*/
    if ((grm_info_p->trm_den_or_bad_page_cnt_set == grm_info_p->bad_paging_block_count)
        || (grm_info_p->dsc_threshold < GERAN_DSC_THRESHOLD_5))
    {
      /* MPH_DSC_THRESHOLD_IND is handled in task's context*/
      /* Lock will ensure power level of last PCH block has been calculated*/
      GL1_ISR_LOCK(gas_id);
      rssi = gl1_drdsds_get_average_rssi(GL1_DRDSDS_AVG_NUM_OF_RSSI, gas_id);
      GL1_ISR_UNLOCK(gas_id);

      if (gl1_drds_debug(gas_id)) 
      {
        MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_hp_mode rssi %d", rssi);
      }
      /* RSSI is greater than threshold, change state depend upon the next_state*/
      if (rssi > rssi_thresh)
      {
        /*Disable DRDSDS: three consecuitve bad quality paging blocks*/
        (void)grm_enable_disable_dual_receive(FALSE, client_id, gas_id);
      }
      else
      {
        if (grm_info_p->trm_den_or_bad_page_cnt_set == grm_info_p->bad_paging_block_count)
        {
          /* Discard the last third bad paging block*/
          /* so that next time GL1 will consider last two and next paging block*/
          grm_info_p->bad_paging_block_count--;
          if (gl1_drds_debug(gas_id)) 
          {
            MSG_GERAN_HIGH_1_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_dsc_ind_hp_mode discard last "
                                               "bad_paging_block_count %d ", 
                                                grm_info_p->bad_paging_block_count);
          }
        }
      }
    }
    else if (grm_info_p->dsc_threshold > GERAN_DSC_THRESHOLD_1) 
    {
      grm_info_p->rxd_priority = DRDSDS_RXD_LP;

     /*   DRDSDS is already enabled. Change state based upon Rxd priority*/
      gl1_drdsds_set_hybrid_tuneaway_state(client_id, gas_id);
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_get_average_rssi

  DESCRIPTION
  This function calculates average rssi value of last "num_meas" PCH blocks.

  DEPENDENCIES
  None

  RETURN VALUE
  RSSI value

  SIDE EFFECTS
===========================================================================*/
dBx16_T gl1_drdsds_get_average_rssi(uint8 num_meas, gas_id_t gas_id)
{
  running_avg_struct *ra_ptr = NULL;
  dBx16_T rssi = gsm_dr_to_sr_falback_rssi_thresh_dBmx16(gas_id);

  MSG_GERAN_MED_1_G("dr_to_sr RSSI Threshold Set:%d",rssi);
  ra_ptr = &(l1_tsk_buffer[gas_id].current_params.L1Data.pIdle_data->campedon_cell_pwr_meas_avg); 
  if (ra_ptr)
  {
    rssi =  l1_running_avg_get_last_num_meas(ra_ptr, num_meas, gas_id);
  }
  return rssi;
}

/*===========================================================================

  FUNCTION  gl1_drdsds_handle_stop_gsm_event

  DESCRIPTION
  This function handles STOP GSM event.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_handle_stop_gsm_event(grm_client_enum_t client_id,
                                      gas_id_t gas_id)
{
  (void)grm_enable_disable_dual_receive(TRUE, client_id, gas_id);
  return;
}

/*===========================================================================

  FUNCTION  gl1_drdsds_handle_trm_denial_event

  DESCRIPTION
  This function handles TRM denial event.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_handle_trm_denial_event(grm_client_enum_t client_id,
                                                  gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if(grm_info_p)
  {
    switch (grm_info_p->hybrid_tuneaway_state)
    {
      case DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE:
      case DRDSDS_LOW_PRIORITY_MODE_STATE:
      case DRDSDS_HIGH_PRIORITY_MODE_STATE:
        gl1_drdsds_process_trm_denial(client_id, gas_id);
        break;

      default:
        break;
    }
  }
  return;
}
/*===========================================================================

  FUNCTION  gl1_drdsds_process_trm_denial

  DESCRIPTION
  This function handles TRM denial event.

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
void gl1_drdsds_process_trm_denial(grm_client_enum_t client_id, gas_id_t gas_id)
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if (grm_info_p)
  {
    if (gl1_drds_debug(gas_id))
    {
      MSG_GERAN_HIGH_5_G(GL1_DRDS_DBG_HDR"gl1_drdsds_process_trm_denial "
                                          "dr_enabled %d bad_paging_block_count %d "
                                          "pending_dr_state %d trm_denied_count %d "
                                          "trm_den_or_bad_page_cnt_set %d", 
                                          grm_info_p->dr_enabled,
                                          grm_info_p->bad_paging_block_count,
                                          pending_dr_state,
                                          grm_info_p->trm_denied_count,
                                          grm_info_p->trm_den_or_bad_page_cnt_set); 
    }
        
    if (gl1_msg_get_multi_sim_standby_mode())
    {
      /* Increment TRM denial count*/
      grm_info_p->trm_denied_count++;

      if (grm_info_p->trm_den_or_bad_page_cnt_set == grm_info_p->trm_denied_count)
      {
        /*Disable DRDSDS: three consecuitve bad quality paging blocks*/
        (void)grm_enable_disable_dual_receive(FALSE, client_id, gas_id);
        grm_info_p->trm_denied_count = 0;
      }
    }
    else
    {
       MSG_GERAN_HIGH_0_G(GL1_DRDS_DBG_HDR"TRM Denial in Non MultiSim-StandBy Mode");
    }
    
  }
  return;
}

/*===========================================================================

  FUNCTION  gl1_drdsds_get_denial_or_fail_count

  DESCRIPTION
  Gets the number of denial or XCCH failure count to disable DR.
  It is based of the l1_debug_nv

  DEPENDENCIES
  None

  RETURN VALUE
  Void

  SIDE EFFECTS
===========================================================================*/
uint8 gl1_drdsds_get_denial_or_fail_count(gas_id_t gas_id)
{
   uint8 bit0, bit1;
   uint8 set_val = 0;
   /*Default Value to be returned*/
   uint8 ret_val = GL1_DRDSDS_TRM_DENIAL_XCCH_FAIL_DEF_CNT;
   /*Get the values from l1_debug_nv*/
   bit0 = GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_TRMDEN_FAIL_CNT_BIT0);
   bit1 = GL1_HW_EFS_DEBUG_ENABLED(GL1_EFS_DEBUG_TRMDEN_FAIL_CNT_BIT1);
 
   /* The count can be toggled based on the combination of these 2 bits set in NV 65770
      0 0 -> 0 -> default value of 4
      0 1 -> 1 -> 5
      1 0 -> 2 -> 6
      1 1 -> 3 -> 3*/

   /* calculate the integer value for the 2 bit combo above*/
   set_val = (2* bit1) + bit0;

   switch (set_val)
   {
   case 1:
     ret_val += 1;
     break;
   case 2:
     ret_val += 2;
     break;
   case 3:
     ret_val -= 1;
     break;
   case 0:
   default:
     /*initialized previously*/
     break;
   }

   MSG_GERAN_HIGH_2_G(GL1_DRDS_DBG_HDR"gl1_drdsds_get_denial_or_fail_count: set_val:%d ret_val:%d", set_val, ret_val);

   return ret_val;
}

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
/* EOF */
