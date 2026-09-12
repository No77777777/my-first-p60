#ifndef GL1_DRDSDS_H
#define GL1_DRDSDS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

      GERAN L1 DRDSDS HEADER FILE

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

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gl1_drdsds.h#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ---------------------------------------------------------- 
20/04/15   sjv      CR823566 Set number of TRM denials/Xcch failures count through debug NV 
27/03/15   dv/pa    CR813849: In X+G DR-DSDS, revert back to SR after 3 consequtive TRM denials  
10/03/15   sjv      CR805970: SCell RSSI Threshold set through a new NV for DR to SR Fallback
31/03/15   pa       CR815621:GRM restructuring for TH 2.0. 
29/01/15   pa       CR785021: Disable DR on TRM denial to activate GBTA on next PCH decode.  
05/01/15   pa       CR774166: Enable DR on inactivating GL1 task.
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
08/10/14   pa       CR732823: Hybrid Tuneaway Fallback: Avg RSSI is an avergae of last 3 PCH blocks. 
25/09/14   pa       CR719024: Part2: Disable DRDSDS on receiving DSC_IND in DUAL_RX Tuneaway Mode.
09/09/14   pa       CR719024: Implementation of Hybrid Tuneaway Fallback Algorithm

===========================================================================*/

/*===========================================================================

           INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "ms.h"

#include "l2_l1.h"
#include "l2_l1_g.h"
#include "msg.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "geran_nv.h"

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
/*===========================================================================

           Structures, Enums and Hash definitions

===========================================================================*/
#define GL1_DRDSDS_THIRD_PAGING_BLOCK       (3)
#define GL1_DRDSDS_FOURTH_PAGING_BLOCK        (4)

#define GL1_DRDSDS_TRM_DENIAL_XCCH_FAIL_DEF_CNT (4)

/*6dB => 96 dBx16*/
#define GL1_DRDSDS_SNR_THRESHOLD          (96)
/* Average RSSI of last 3 PCH blocks */
#define GL1_DRDSDS_AVG_NUM_OF_RSSI           (3)
/* Dual Receive Mode (Request Diversity with High/Low Priority */
typedef enum
{
  DRDSDS_RXD_NULL,
  DRDSDS_RXD_LP,
  DRDSDS_RXD_HP
}gl1_drdsds_drmode_rxd_priority;

/* Hybrid Tuneaway States to handle Hybrid Tuneaway Fallback Algorithm */
typedef enum
{
  DRDSDS_TUNEAWAY_MODE_STATE,
  /* 1 in 4 PB Dual Rx Tuneaway Mode state*/
  DRDSDS_DUAL_RX_TUNEAWAY_MODE_STATE,
  DRDSDS_LOW_PRIORITY_MODE_STATE,
  DRDSDS_HIGH_PRIORITY_MODE_STATE
}gl1_drdsds_hybrid_tuneaway_state;

/*Events which triggers Hybrid Tuneaway State machine*/
typedef enum
{
  DRDSDS_IDLE_INIT_EVENT, /* Gl1 eneters into L1_IDLE_INIT substate*/
  DRDSDS_MPH_BLOCK_QUALITY_IND_EVENT, /* Every decode of PCH block (Good, Bad, Unknown)*/
  DRDSDS_MPH_DSC_THRESHOLD_IND_EVENT, /*MPH_DSC_THRESHOLD_IND from RR*/
  DRDSDS_STOP_GSM_EVENT,
  DRDSDS_TRM_DENIED_EVENT
}gl1_drdsds_hybrid_tuneaway_event;

/* Messages passed from L1/RR to Hybrid Tuneaway State Machine */
/* Union: if another message handling requires in future*/
typedef union
{
  mph_block_quality_ind_T  block_quality_ind;
}gl1_drdsds_hybrid_tuneaway_msg;

/*===========================================================================

           Functions declarations

===========================================================================*/
void gl1_drdsds_set_hybrid_tuneaway_state(grm_client_enum_t client_id,
                                          gas_id_t          gas_id);
void gl1_drdsds_hybrid_tuneaway_trigger(gl1_drdsds_hybrid_tuneaway_event event, 
                                        gl1_drdsds_hybrid_tuneaway_msg *msg,
                                        grm_client_enum_t client_id,
                                        gas_id_t gas_id);
void gl1_drdsds_handle_idle_init_event(grm_client_enum_t  client_id,
                                       gas_id_t           gas_id);
void gl1_drdsds_handle_blk_quality_ind_event(mph_block_quality_ind_T *msg,
                                             grm_client_enum_t client_id,
                                             gas_id_t gas_id);
void gl1_drdsds_handle_dsc_threshold_ind_event(grm_client_enum_t client_id,
                                               gas_id_t gas_id);
void gl1_drdsds_process_blk_ind_tuneaway_mode(grm_client_enum_t client_id,
                                              gas_id_t gas_id);
void gl1_drdsds_process_blk_ind_dual_rx_tuneaway_mode(gl1_block_quality_t blk_quality,
                                                      grm_client_enum_t client_id,
                                                      gas_id_t gas_id);
void gl1_drdsds_process_blk_ind_lp_mode(mph_block_quality_ind_T *msg,
                                        grm_client_enum_t client_id,
                                        gas_id_t gas_id);
void gl1_drdsds_process_blk_ind_hp_mode(mph_block_quality_ind_T *msg,
                                        grm_client_enum_t client_id,
                                        gas_id_t gas_id);
void gl1_drdsds_process_dsc_ind_tuneaway_mode(grm_client_enum_t client_id,
                                              gas_id_t gas_id);
void gl1_drdsds_process_dsc_ind_dual_rx_tuneaway_mode(grm_client_enum_t client_id,
                                                      gas_id_t gas_id);
void gl1_drdsds_process_dsc_ind_lp_mode(grm_client_enum_t client_id, gas_id_t gas_id);
void gl1_drdsds_process_dsc_ind_hp_mode(grm_client_enum_t client_id, gas_id_t gas_id);
dBx16_T gl1_drdsds_get_average_rssi(uint8 num_meas, gas_id_t gas_id);
void gl1_drdsds_handle_stop_gsm_event(grm_client_enum_t client_id,
                                      gas_id_t gas_id);

void gl1_drdsds_handle_trm_denial_event(grm_client_enum_t client_id,
                                                  gas_id_t gas_id);
void gl1_drdsds_process_trm_denial(grm_client_enum_t client_id, gas_id_t gas_id);

uint8 gl1_drdsds_get_denial_or_fail_count(gas_id_t gas_id);

#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
#endif /* GL1_DRDSDS_H */
