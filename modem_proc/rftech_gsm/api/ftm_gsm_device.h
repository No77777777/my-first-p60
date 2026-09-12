#ifndef FTM_GSM_DEVICE_H
#define FTM_GSM_DEVICE_H

/*===========================================================================

               G S M   FTM  DEVICE   D E F I N I T I O N S
                         H E A D E R  F I L E

DESCRIPTION
   This file contains GSM definitions which are device specific


   Copyright (c) 2015 by Qualcomm Technologies Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  Initially, comments will be entered manually. Later, as the module is
  placed under version control, the version control tool will automatic-
  ally insert change comments into this section. [Delete this comment].

  $Header: //components/rel/rftech_gsm.mpss/3.11/api/ftm_gsm_device.h#1 $ 
  $DateTime: 2021/03/09 05:10:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/02/20   mpa     To support MiniDump Feature
08/26/15   tsr      Clear rfctl vars/chain vars for assoc tx/rx devices
07/31/15   hoh     adapt frame sweep cal to work with Rx/Tx device split architecture
08/05/15   zc      Support for new RF Test Framework
07/24/15   tsr     Fix Idle Frame processing in FTM
07/16/15   tsr     Add GSM Support for FTM Device manager
07/13/15   tsr     Move GSM Non-Signalling state variables under common chain variables
07/07/15   tsr     Added new api to get preferred rx device from RFC for Non-Signalling 
6/19/15    tsr     Added new api to retrieve sub id from a tx/rx device by querying ftm chain vars 
06/10/15   tsr     Added FTM and Non-Signalling Support on a chain if TX device is not supported 
05/27/15   tsr     Add new apis to check ftm state in RFGSM CORE LAYER  
04/24/15   tsr     Add api to query if the device passed in is a Primary RX device 
04/08/15   tsr     Initial version created.Introduced ftm gsm chains based framework for split RX/TX devices 

=============================================================================*/
#include "comdef.h"
#include "ftm.h"
#include "ftm_gsm.h"
#include "rfgsm_core_types.h"
#include "rfgsm_mdsp_cal.h"
#include "gl1_defs.h"
#include "rfgsm_ftm_msg.h"
#include "rfcom.h"
#include "rfm_device_types.h"
#include "ftm_gsm_rfctl.h"
#include "ftm_common_control.h"

#ifdef FEATURE_QSH_MDUMP 
#include "qsh.h"
#endif


#define FTM_GSM_START_INIT_CNT                 1      /* Wait 0.1 seconds to start Rx/Tx init   */


typedef enum
{
  FTM_GSM_C0,
  //FTM_GSM_C1,
  FTM_GSM_C2,
  //FTM_GSM_C3, 
  FTM_GSM_C4,
  //FTM_GSM_C5,
  FTM_GSM_MAX_CHAINS,
  FTM_GSM_INVALID_CHAIN = 0xFF
}ftm_gsm_chains_enum_type;

typedef enum
{
  FTM_SUB_ID_0,
  FTM_SUB_ID_1,
  FTM_SUB_ID_MAX,
  FTM_INVALID_SUB_ID = 0xFF
}ftm_gsm_subs_enum_type;


/*ftm gsm cmn chain vars*/
typedef struct
{
  boolean init_once;

  ftm_mode_id_type mode;

  uint16 ftm_channel;   /* Current Channel */

  sys_band_T ftm_gl1_hw_gsm_band; /* Current band */

  /* flag to indicate whether the FTM cal is in progress or not */
  boolean ftm_gsm_cal_flag;

  /* flag to determine idle frame processing */
  boolean ftm_gsm_idle_proc_flag;

  /*Variable to store whether GSM is FTM Non-Signalling, FTM state or FTM IRAT state */
  ftm_rfstate_enum_type gsm_current_state;
  
  boolean gsm_frame_sweep_enabled;
  
  uint16 frame_sweep_fn_count;
   
  uint8 gsm_frame_sweep_paused_count;
   
  uint8 *frame_sweep_header_ptr;

  /*! @brief Variables and queues for Non-Signalling */

  /* Used to map this device instance to a GL1 as_id - mapped at init */
  sys_modem_as_id_e_type                 ns_as_id;

  /* Status of GSM BER */
  ftm_gsm_ber_status_type                ftm_gsm_ber_status;

  /* current state of FTM EGPRS scenario */
  ftm_egprs_ber_state_type           current_egprs_state;
  ftm_egprs_ber_reconfig_state_type  current_egprs_reconfig_state;

  /* Buffer for DL and UL TBF configuration parameters that are controllable via FTM interface */
  ftm_egprs_ber_dl_tbf_status_type   current_egprs_dl_tbf_info;
  ftm_egprs_ber_ul_tbf_status_type   current_egprs_ul_tbf_info;

  /* Buffers the DL and UL TBF configurations that get sent to L1 */
  mac_dynamic_tbf_information_T      current_egprs_l1_dynamic_tbf_info;
  mac_l1_dl_tbf_config_t             current_egprs_l1_dl_tbf_info;
}ftm_gsm_cmn_vars_s;


/* This structure holds the tx variables based on the device */
typedef struct
{
  rfm_device_enum_type rfm_dev;

  /* Dispatch device is required in order to retrieve the chain vars */
  rfm_device_enum_type dispatch_device;

  ftm_gsm_state_type ftm_gsm_state;  

  volatile uint32 ftm_frame_count;

  /* OPLL BW related stuff... temporary until these values are fixed*/
  /* whether it's continuous Tx */
  boolean ftm_cont_tx;

  /* the number of tx bursts */
  uint32  ftm_num_bursts_tx;

  /* Kv Cal V2 - Global Variables */
  uint16 ftm_gsm_rfctl_kv_cal_per_tx_on;
  uint16 ftm_gsm_rfctl_kv_tx_on_off;
  boolean ftm_gsm_rfctl_kv_cal_v2_state;

  boolean tx_cont_on;

  boolean tx_frame_matrix_persistent;

  int16 last_power[FTM_MAX_GSM_SLOT_PER_FRAME];

  rfa_rf_gsm_ftm_set_pa_range_type_rsp_s  rfa_rf_gsm_ftm_set_pa_range_rsp;

  rfa_rf_gsm_ftm_set_tx_pow_dbm_type_rsp_s  rfa_rf_gsm_ftm_set_tx_pow_dbm_rsp;

  rfa_rf_gsm_ftm_tuner_override_type_rsp_s  rfa_rf_gsm_ftm_tuner_override_rsp;

  uint16  data_mod;
  volatile uint16 ftm_FNmod4;

  gl1_hw_tx_signal_type ftm_tx_signal[FTM_MAX_GSM_TX_SLOT];

  ftm_slot_type ftm_slot_assign[FTM_MAX_GSM_SLOT_PER_FRAME];

  uint8 frames_to_wait;

  /*! @brief Flag used to enable stop Tx count*/
  uint8 stop_tx_triggered;

  int32 ftm_gsm_tx_freq_offset;


  /*! @brief TX burst parameters */
  ftm_tx_burst_type tx_brst;

  boolean gsm_tx_cal_start;

  boolean do_tx_da_cal;

  boolean ftm_gsm_predist_override;
  
  boolean gl1_freq_error_saved;
  int32 gl1_freq_error;
  int32 prev_freq_error;
  int32 new_freq_error;

  /* pointer to the TX command function */
  void (*ftm_tx_funct)(rfm_device_enum_type rfm_dev );
  /* pointer to the TX stop command function */
  void (*ftm_tx_stop_funct)(rfm_device_enum_type rfm_dev );

  /*! @brief MDSP cal variables */
  rfgsm_mdsp_cal_variables_s mdsp_cal_vars;

} ftm_gsm_device_tx_vars_s;


/*This strucue holds RX variables for a device*/
typedef struct
{
  rfm_device_enum_type rfm_dev;

  /* Dispatch device is required in order to retrieve the chain vars */
  rfm_device_enum_type dispatch_device;

  ftm_gsm_state_type ftm_gsm_state;  

  boolean ftm_gsm_idle_proc_flag;

  ftm_gsm_frame_sweep_rx_tag_type  gsm_frame_sweep_tags[FTM_MAX_GSM_BURST_TAGS];

  uint8 rx_slot_num;

  volatile uint32 ftm_frame_count;

  ftm_gsm_rx_cmd_rx_burst_metrics_type burst_metrics;

  /* whether it's continuously issued Rx bursts*/
  boolean ftm_cont_rx;

  /* the number of rx bursts */
  uint32  ftm_num_bursts_rx;

  boolean do_multi_slot_rx_cal_flag;

  /* mDSP Rx debug buffer */          
  gl1_defs_metrics_rpt gsm_rx_burst_metrics;
  gl1_msg_metrics_rpt  gsm_rx_burst_msg_rpts[FTM_MAX_GSM_TX_SLOT];
  gl1_msg_metrics_rpt* gsm_rx_burst_msg_ptrs[FTM_MAX_GSM_TX_SLOT];

  gl1_hw_rx_tags_type  ftm_gsm_rx_tags[FTM_MAX_GSM_BURST_TAGS];
  uint16               tags_rd_index;
  uint16               tags_wr_index;


  gl1_defs_rx_pkt_metrics_type gsm_rx_multislot_metrics[GERAN_API_MAX_GL1_DEFS_MAX_DL_MSGS];
  gl1_defs_rx_pkt_metrics_type *gsm_rx_multislot_metrics_ptr[GERAN_API_MAX_GL1_DEFS_MAX_DL_MSGS];

  gl1_hw_rx_signal_type  signal_info[FTM_GSM_RFCTL_SIGNAL_INFO_ARR_SIZE];
  gl1_hw_rx_tags_type  ftm_gsm_rx_multislot_tags[3][GERAN_API_MAX_GL1_DEFS_MAX_DL_MSGS];

  boolean average_rssi_available;

  uint16 num_rx_slots;
  boolean multislot_rx_on;

  int16 last_power[FTM_MAX_GSM_SLOT_PER_FRAME];

  rfa_rf_gsm_ftm_tuner_override_type_rsp_s  rfa_rf_gsm_ftm_tuner_override_rsp;

  rf_gain_T ftm_gain_range;
  rf_gain_T ftm_gain_range_drx;

  uint16  data_mod;
  volatile uint16 ftm_FNmod4;

  ftm_slot_type ftm_slot_assign[FTM_MAX_GSM_SLOT_PER_FRAME];

  /*! @brief No of frames over which RX RSSI is averaged. */
  uint8 ftm_gsm_rssi_no_frames_avg;

  boolean ftm_gsm_lna_range_in_rx_lvl_parm;

  boolean ftm_gsm_rfctl_reset_rssi;

  //for burst metrics
  int32 local_frame_cnt;
  boolean wait_for_next_lna_range;
  int8 next_lna_range_order;
  uint32 first_call_fn;

  /*! @brief inittialize the sequence of lna ranges in rx cal for a particular band and RX level */
  uint16 ftm_gsm_next_lna_range[FTM_MAX_GSM_SLOT_PER_FRAME];
  boolean ftm_gsm_lna_range_ind_flag;
  uint8 frames_to_wait;

  /*! @brief Flags used to specify when to start sending rx burst commands to MDSP and when to stop */
  uint8 beginning_rx;
  uint8 stop_rx;
  
  /*! @brief flag to reset the transition RX frame count in the burst metrics function */
  boolean ftm_gsm_rfctl_reset_transit_rx_frame_cnt;

  /*! @brief to check whether Rx Burst has started */
  boolean rxBurstOn;

  /* flag to indicate whether the FTM cal is in progress or not */
  boolean ftm_gsm_cal_flag;
  
  boolean gl1_freq_error_saved;
  int32 gl1_freq_error;
  int32 prev_freq_error;
  int32 new_freq_error;

  void (*ftm_rx_funct)(rfm_device_enum_type rfm_dev );  

  /* ftm mode id */
  ftm_mode_id_type ftm_mode_id;

  /*diversity enabled */
  boolean diversity_on;

} ftm_gsm_device_rx_vars_s;


/*FTM GSM Chain Variables structure*/

/*! @brief This structure holds the ftm gsm chain variables for GSM RF calibration */
typedef struct
{
  uint8 gsm_chain;

  uint8 sub_id;

  rfm_device_enum_type prx_dev;

  rfm_device_enum_type associated_drx_dev;

  rfm_device_enum_type associated_tx_dev;

  ftm_gsm_cmn_vars_s* cmn_vars_ptr;

  /*Temporarily pointing the tx device variables ptr to legacy structure*/
  ftm_gsm_rfctl_vars_s* rx_vars_ptr; //ftm_gsm_device_rx_vars_s* rx_vars_ptr;

  /*Temporarily pointing the tx device variables ptr to legacy structure*/
  ftm_gsm_rfctl_vars_s* tx_vars_ptr; //ftm_gsm_device_tx_vars_s* tx_vars_ptr;

} ftm_gsm_chain_vars_s;

/*FTM GSM SUB Variables structure*/
typedef struct 
{
  uint8 active_gsm_chain;

  uint8 sub_id;

  volatile uint32 ftm_frame_count;

 /* pointer to the ISR function */
  void (*ftm_tdma_isr_fnc)( sys_modem_as_id_e_type );

  ftm_gsm_chain_vars_s* chain_vars_ptr;

}ftm_gsm_sub_vars_s;


/*-----------------------------------------------------------------------------
                            Function Prototypes
-----------------------------------------------------------------------------*/


ftm_gsm_chain_vars_s* ftm_gsm_device_clear_chain_vars( rfm_device_enum_type dispatched_rx_device );

boolean ftm_gsm_clear_chain_vars( ftm_gsm_chains_enum_type gsm_curr_chain );

ftm_gsm_chains_enum_type ftm_gsm_get_chain_from_dispatch_device( rfm_device_enum_type dispatch_device );

ftm_gsm_chain_vars_s * ftm_gsm_create_and_init_chain_vars( ftm_gsm_chains_enum_type ftm_gsm_chain);

ftm_gsm_chain_vars_s * ftm_gsm_get_chain_vars_ptr( ftm_gsm_chains_enum_type ftm_gsm_chain );

void ftm_gsm_initialise_chains(void);

ftm_gsm_chain_vars_s * ftm_gsm_device_get_and_update_chain_vars( rfm_device_enum_type dispatched_rx_device);

boolean ftm_gsm_device_is_tx_supported(rfm_device_enum_type tx_device);

rfm_device_enum_type ftm_gsm_get_associated_tx_device( rfm_device_enum_type prx_dev );

rfm_device_enum_type ftm_gsm_get_associated_rx_device( rfm_device_enum_type prx_dev );

rfm_device_enum_type ftm_gsm_get_remapped_rx_device_info( rfm_device_enum_type dispatch_device );

rfm_device_enum_type ftm_gsm_allocate_ns_device( void );

rfm_device_enum_type ftm_gsm_get_prx_device (rfm_device_enum_type dispatch_device ,
                                             rfm_device_enum_type device);

void ftm_gsm_create_and_init_sub_vars(ftm_gsm_subs_enum_type sub_id);


void ftm_gsm_device_clear_sub_vars(ftm_gsm_subs_enum_type sub_id);


ftm_gsm_sub_vars_s * ftm_gsm_get_sub_vars_ptr( uint8 active_sub );

void ftm_gsm_assign_sub_vars_chain_ptr( uint8 sub_id, ftm_gsm_chain_vars_s * chain_vars_ptr );

boolean ftm_gsm_update_sub_vars( ftm_gsm_chains_enum_type curr_chain,
                                 uint8 active_sub );

boolean ftm_gsm_sub_vars_created( ftm_gsm_subs_enum_type sub_id );

uint8 ftm_gsm_get_sub_from_chain( ftm_gsm_chains_enum_type gsm_chain );

void ftm_gsm_device_create_cmn_vars_ptr( ftm_gsm_chain_vars_s * chain_vars_ptr );

void ftm_gsm_device_free_cmn_vars_ptr( ftm_gsm_chain_vars_s * chain_vars_ptr );

boolean ftm_gsm_is_device_prx ( rfm_device_enum_type dispatch_device,
                                rfm_device_enum_type remapped_rx_device );

boolean ftm_gsm_tx_device_is_ftm_state(rfm_device_enum_type tx_device);

boolean ftm_gsm_rx_device_is_ftm_state(rfm_device_enum_type rx_device);

uint8 ftm_gsm_get_sub_id_from_device( rfm_device_enum_type device );
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR ftm_gsm_chain_vars_s ** ftm_gsm_get_chain_vars_ptr_address ( void );



#endif /* FEATURE_QSH_MDUMP */


#endif /* FTM_GSM_DEVICE_H */
