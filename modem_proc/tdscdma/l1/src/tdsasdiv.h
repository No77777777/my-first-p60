#ifndef TDSASDIV_H
#define TDSASDIV_H
/*===========================================================================
   FILE: tdsasdiv.h

GENERAL DESCRIPTION:
      This file contains implementation of external functions and utility
      functions of antenna switch diversity.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  
Copyright(c) 2000-2014 by Qualcomm Technologies Incorporated. All Rights Reserved.
        
===========================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

==============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "customer.h"
#include "comdef.h"

#ifdef FEATURE_TDSCDMA_ANTENNA_SWITCH_DIVERSITY
#include "tdsl1_nv.h"
#include "trm.h"

/* 
 bit7(MSB) bit6 bit5 .........bit0(LSB)
 bit0 ...................1 band A
 bit1 ...................1 band E
 bit2 ...................1 band F 
 bit3-7 ...................reserved */

#define band_A_algorithm_type 0x01
#define band_E_algorithm_type 0x02
#define band_F_algorithm_type 0x04

#define ASDIV_IS_BAND_A(band_mask) \
  (((band_mask) & band_A_algorithm_type) ? TRUE : FALSE) 

#define ASDIV_IS_BAND_E(band_mask) \
  (((band_mask) & band_E_algorithm_type) ? TRUE : FALSE) 

#define ASDIV_IS_BAND_F(band_mask) \
  (((band_mask) & band_F_algorithm_type) ? TRUE : FALSE) 

  
#define TDSASDIV_ANTENNA_ARRAY_SIZE 80
#define TDSASDIV_MIN_T1_LENGTH 50
#define TDSASDIV_MAX_PROBE_CYCLE 10
#define TDSASDIV_T2_COOLDOWN 2000
#define TDSASDIV_MIN_TEST_MODE_IDLE_SWITCH_PERIOD 2

typedef enum
{
	TDSSRCHSET_IDLE_PHASE,
	TDSSRCHSET_PROBING_PHASE,
	TDSSRCHSET_WAITING_PHASE
} tdssrchset_ante_switch_phase_enum_type;

typedef enum
{
	STEP0_FIRST_ANT,
	STEP0_SECOND_ANT,
	STEP1_FIRST_ANT,
	STEP1_SECOND_ANT
} tdssrchset_asdiv_acq_state;

typedef struct
{
        // dynamic flag to enable/disable asdiv
        boolean asdiv_enable;
	// ASDiv algorithm used, either type 1 or type 2
	uint8 algorithm_type;
	// Current state
	tdssrchset_ante_switch_phase_enum_type ante_switch_phase;
	// Current state under ACQ
	tdssrchset_asdiv_acq_state acq_state;
	// Current antenna
	uint8 ant_no;
	// T1 probe time (ms)
	uint16 T1_probe_time;
	// Number of probe cycles
	uint8 num_probe_cycles;
	
	// Average RSCP for ant0, Q8, updated every T1 period
	int32 ant0_rscp;
	// Number of received RSCP values in current T1 period
	int16 ant0_rscp_num_in_T1;
	// Average RSCP history for ant0
	int16 ant0_rscp_history[TDSASDIV_ANTENNA_ARRAY_SIZE];
	// Average Tx power for ant0, updated every T1 period
	int32 ant0_txpwr;
	// Number of received Tx power values in current T1 period
	int16 ant0_txpwr_num_in_T1;
	// Average Tx power history for ant0
	int16 ant0_txpwr_history[TDSASDIV_ANTENNA_ARRAY_SIZE];
	// The index for ant0 Tx power history array
	int8 ant0_history_index;

	// Average RSCP for ant1, Q8, updated every T1 period
	int32 ant1_rscp;
	// Number of received RSCP values in current T1 period
	int16 ant1_rscp_num_in_T1;
	// Average RSCP history for ant1
	int16 ant1_rscp_history[TDSASDIV_ANTENNA_ARRAY_SIZE];
	// Average Tx power for ant1, updated every T1 period
	int32 ant1_txpwr;
	// Number of received Tx power values in current T1 period
	int16 ant1_txpwr_num_in_T1;
	// Average Tx power history for ant1
	int16 ant1_txpwr_history[TDSASDIV_ANTENNA_ARRAY_SIZE];
	// The index for ant1 Tx power history array
	int8 ant1_history_index;

	// Timer for individual T1 period
	uint16 t1_timer;
	// Timer for the probing phase
	uint16 probing_timer;
	// Timer for the waiting phase
	uint16 waiting_timer;
	// Number of subframes stayed in current antenna
	uint16 num_sf_stayed_in_current_ant;
	// Average power delta
	int16 avgPwrDelta;

	// Total number of Tx timeslots every T1 period
	int16 ant0_total_tx_slots[TDSASDIV_MAX_PROBE_CYCLE];
	// Number of timeslots which Tx exceeds MTPL every T1 period
	int16 ant0_exceed_mtpl[TDSASDIV_MAX_PROBE_CYCLE];
	//The index for ant0 MTPL array
	int8 ant0_mtpl_index;
	// Total number of Tx timeslots every T1 period
	int16 ant1_total_tx_slots[TDSASDIV_MAX_PROBE_CYCLE];
	// Number of timeslots which Tx exceeds MTPL every T1 period
	int16 ant1_exceed_mtpl[TDSASDIV_MAX_PROBE_CYCLE];
	//The index for ant1 MTPL array
	int8 ant1_mtpl_index;
	// Total uplink timeslots number in T1 period
	uint16 txpwr_total_num;
	// Total uplink timeslots number whose txpower reach mtpl in T1 period
	uint16 txpwr_reach_mtpl_num;
	
	// Save the switch complete parameters
	trm_switch_complete_input_type switch_complete_input;
	// if TRUE, means RF/FW is doing antenna switch
	boolean wait_switch_complete_flag;

	// if FALSE, we still need to initialize the ASDiv config during boot-up
	boolean config_initialized_when_asdiv_is_disabled;
	// The config granted from the callback of trm_asd_set_mode_and_config
	trm_asd_set_mode_and_config_type set_mode_and_config_result;
	// In DSDS mode, L1 will use tdsrf_enter_mode instead of tdsrf_wakeup.
	// So we need this to indicate that TDS is in DRX sleep but not the really doing enter mode. 
	boolean is_drx_sleep;
	// if TRUE, we have RSCP measurement during this DRX cycle
	boolean has_rscp_measurement_during_wakeup;
	// Traffic radio counts how many times its switch request was continuously denied by TRM. 
	// When the counter reach M (M is NV configurable for each tech), notify TRM by setting a “crisis flag” in the switch request
	uint8 crisis_mode_count;
	// Each idle radio shall count how many times it has changed its preferred antenna config continuously (back-to-back). 
	// When the number reaches N (N is NV configurable for each tech), stop changing preferred config for L (NV configurable) paging cycles. 
	uint8 idle_pingpong_count;
	uint8 idle_suspension_count;
	// How many subframes to force a switch in test mode
	uint64 test_mode_switch_period_connected;
	// How many sleep cycles to force a switch in test mode
	uint64 test_mode_switch_period_idle;
	// Current sleep cycle number
	uint64 test_mode_switch_count_idle;
} tdssrchset_active_cell_ante_switch_struct_type;

extern tdssrchset_active_cell_ante_switch_struct_type tdssrchset_scell_antenna_switch_info;
extern tdsl1_nv_antenna_switch_struct_type tdsschset_scell_antenna_switch_timer_threshold_nv;

#define TDSASDIV_TYPE1_ALGO_PROBING_PERIOD 100 //calsulate avgRSCP in every probing period 500ms, 100sfn
#define TDSASDIV_TYPE1_ALGO_RXD_PERIOD 50 //turn on DRx chain for Tp period of every 250ms, 50sfn
#define TDSASDIV_TYPE1_ALGO_LOOKBACK_TIME 600 //lookback time is 3s, 600sfn
#define TDSASDIV_TYPE1_ALGO_ARRAY_SIZE 20
#define TDSASDIV_TYPE1_ALGO_TP_OVERHEAD 6 //set overhead for Tp period as 30ms, 6sfn (worst case of 10ms RxD EVENT fire, 10ms CLK_PREP1 timer, 10ms margin) 

typedef enum
{
	TDSSRCH_IDLE_PHASE,
	TDSSRCH_PROBING_FOR_SWITCH_PHASE,
	TDSSRCH_PROBING_FOR_SWITCH_BACK_PHASE,
	TDSSRCH_HOLDING_OFF_PHASE
} tdssrch_ante_switch_phase_type1_algo_enum_type;

typedef struct
{
	// Current state
	tdssrch_ante_switch_phase_type1_algo_enum_type ante_switch_phase;
	// Indicate PRx chain is on which antenna 
	// 0 means PRx chain on ant0(switch config is straight) 
	// 1 means PRx chain on ant1(switch config is straight)
	// But as "active_cell_ptr->rscp_x256" always means rscp from PRx chain, 
	// "rscp_x256_ant1" always means rscp from DRx chain, need to use this flag??
	uint8 prx_chain_no;
  
	// calcuate avgRSCP0(on PRx chain) in each probe period (500ms), unit is sfn
	uint16 probe_period;
	// turn on-off RxD chain in each rxd period (250ms), unit is sfn
	uint8 rxd_period;
	// calcuate avgRSCP1(on DRx chain) in each Tp period (75ms), unit is sfn
	uint8 tp_period;
	// holding off type1 algorithm period (3s), unit is sfn
	uint16 holding_off_period;
	
	// total RSCP for PRx chain in every IDLE/PROBING period, Q8, updated every TS0 rsp
	int32 prx_rscp;
	// Number of received RSCP values for PRx chain in current IDLE/PROBING period
	int16 prx_rscp_num_in_probing;
	// Average RSCP history for PRx chain, lookback time is 3s, probing period is 500ms, 
	// so we most need to save 6 history values for lookback
	int16 prx_rscp_history[TDSASDIV_TYPE1_ALGO_ARRAY_SIZE];
	// The index for PRx chain RSCP history array
	int8 prx_history_index;

	// Average RSCP for DRx chain in current PROBING period, Q8, updated every probing period
	// finally averaged at the end of PROBING period
	int32 drx_rscp;
	// RSCP for DRx chain is valid or not.
	boolean drx_rscp_is_valid;
	// Number of received RSCP values for DRx chain in current in every IDLE/PROBING period
	int16 drx_rscp_num_in_probing;
	
	// Average Tx power in current PROBING period, updated every subframe, Q4	
	// finally averaged at the end of PROBING period
	int32 txpwr;
	// Number of received Tx power values in current PROBING period
	int16 txpwr_num_in_probing;

	// PowerMetric in the probing period before switch, Q8
	int16 powerMetric_preSwitch;
	// PowerMetric in the probing period after switch, Q8
	int16 powerMetric_postSwitch;

	// Timer for the idle and probing phase
	uint8 probing_timer;
	// Timer for turn on-off RxD chain
	uint8 rxd_timer;
	// Timer for the holding off phase
	uint16 holding_off_timer;

	// Save the switch complete parameters
	trm_switch_complete_input_type switch_complete_input;
	// if TRUE, means RF/FW is doing antenna switch
	boolean wait_switch_complete_flag;
	// Traffic radio counts how many times its switch request was continuously denied by TRM. 
	// When the counter reach M (M is NV configurable for each tech), notify TRM by setting a “crisis flag” in the switch request
	uint8 crisis_mode_count;

	// The config granted from the callback of trm_asd_set_mode_and_config
	//trm_asd_set_mode_and_config_type set_mode_and_config_result;
} tdssrch_ante_switch_type1_algo_struct_type;

extern tdssrch_ante_switch_type1_algo_struct_type tdsasdiv_type1_algo_params;
extern tdsl1_nv_asdiv_type1_algo_struct_type tdsl1_antenna_switch_type1_algo_params;

typedef enum
{
  TDSL1M_ASDIV_SLEEP = 0,
  TDSL1M_ASDIV_RX,
  TDSL1M_ASDIV_TX,
  TDSL1M_ASDIV_RXACQ,
  TDSL1M_ASDIV_INACTIVE
} tdsl1m_asdiv_state_enum_type;
  
/*=========================================================================

 FUNCTION     :TDSASDIV_CHECK_STATE_CHANGE
 
 DESCRIPTION  : This function checks if there is a ASDiv state change (SLEEP <-> RX <-> TX <-> SLEEP). If true, 
 inform the config manager and set up the callback functions.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdsasdiv_check_state_change(void);

/*=========================================================================

 FUNCTION     :TDSASDIV_SET_MODE_CB
 
 DESCRIPTION  : This is the callback function for TRM to confirm TDS has set ASDiv mode
              
 DEPENDENCIES : 

 INPUT PARAMETERS: trm_ant_switch_set_mode_return_type set_mode_status

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdsasdiv_set_mode_cb(trm_ant_switch_set_mode_type set_mode_status);

/*=========================================================================

 FUNCTION     :TDSASDIV_ANT_SWITCH_CB
 
 DESCRIPTION  : This is the callback function for TRM to inform TDS to switch antenna
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
 	trm_ant_switch_cb_info_type switch_action

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdsasdiv_ant_switch_cb(trm_ant_switch_cb_info_type switch_action);

/*=========================================================================

 FUNCTION     :TDSASDIV_SET_MODE_AND_CONFIG_CB
 
 DESCRIPTION  : This is the callback function used by TRM to inform TDS the 
 				the grant result of the trm_asd_set_mode_and_config()
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
 	trm_asd_set_mode_and_config_type set_mode_status

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void tdsasdiv_set_mode_and_config_cb(trm_asd_set_mode_and_config_type set_mode_status);

/*=========================================================================

 FUNCTION     :TDSASDIV_CHECK_SAFE_CONDITION
 
 DESCRIPTION  : This function checks the safe condition to allow other tech to switch in DSDA idle/traffic scenario.
 Then send the result to TRM to update status.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
void tdsasdiv_check_safe_condition(void);

/*=========================================================================

 FUNCTION     :TDSASDIV_BLOCK_OTHER_TECH_SWITCH
 
 DESCRIPTION  : This function informs TRM to stop/resume other tech from switching due to certain reasons 
 (TDS is doing QTA, IRAT, etc.)
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
	boolean								lock
	trm_asdiv_switch_state_update_reason_t	reason
 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
void tdsasdiv_block_other_tech_switch(boolean lock, trm_asdiv_switch_state_update_reason_t reason);

/*=========================================================================

 FUNCTION     :TDSASDIV_INIT_ANTENNA_POSITION
 
 DESCRIPTION  : This function initializes antenna position during idle wake up or enter mode
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
void tdsasdiv_init_antenna_position(void);

/*=========================================================================

 FUNCTION     :TDSASDIV_SET_PREFERRED_ANTENNA_BEFORE_SLEEP
 
 DESCRIPTION  : This function sets the preferred antenna and inform TRM before going to sleep
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
void tdsasdiv_set_preferred_antenna_before_sleep(void);


void tdsasdiv_init(void);
void tdsasdiv_dch_init(void);
void tdsasdiv_rscp_filtering(void);
void tdsasdiv_state_update(void);
void tdsasdiv_complete_switch(void);
void tdsasdiv_call_fw_switch(boolean is_rf_ready);
void tdsasdiv_rf_callback(void * cb_data);
void tdsasdiv_init_antenna_position_when_asdiv_is_disabled(void);
void tdsasdiv_test_mode_update(void);
void tdsasdiv_stay_on_current_antenna_before_sleep(void);

void tdsasdiv_test_mode_update_type1_algo(void);

void tdsasdiv_init_type1_algo(void);
void tdsasdiv_rscp_filtering_type1_algo(void);
void tdsasdiv_state_update_type1_algo(void);
void tdsasdiv_complete_switch_type1_algo(void);
void tdsasdiv_call_fw_switch_type1_algo(boolean is_rf_ready);
uint8 tdsasdiv_choose_algorithm(void);

void tdsasdiv_timer_update(void);
void tdsasdiv_timer_update_type1_algo(void);
void tdsasdiv_do_txpwr_filtering(int16 new_measured_txpwr);

boolean tdsasdiv_acq_is_only_one_ant_available(void);
boolean tdsasdiv_step1_decision(uint16 snr, uint32 midambleIdx);
void tdsasdiv_acq_choose_algorithm(uint16 freq);
uint8 tdsasdiv_acq_get_current_antenna(void);
boolean tdsasdiv_acq_switch_antenna(uint8 ant_no);

#endif //FEATURE_TDSCDMA_ANTENNA_SWITCH_DIVERSITY

#endif //TDSASDIV_H