#ifndef WL1ANTSWITCH_H
#define WL1ANTSWITCH_H
/*===========================================================================

                    wl1antswitch header file

DESCRIPTION
  This file contains global declarations and external references
  for the ant switch feature.

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2012-2013 by Qualcomm Technologies, Inc. All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1antswitch.h#4 $
$DateTime: 2024/09/10 05:08:47 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---   ------------------------------------------------------------
01/03/23    gmr     SAR MTPL update
01/10/20   ac      Mini dump framework checkin
04/25/17   svh   Allow switchback even if MTPL based algorithm is disabled.
08/17/16   br    Clear SAR forced block flag during ASDIV cleanup.
08/10/16   svh   ASDIV + idle inter freq search fixes.
03/06/16   svh   Move ASD MDSP command unsuspend to task context.
01/04/16   hdk   Correcting the AGC units for FR 27546
12/29/15   hdk   Changes to handle -ve switch threshold for type 1 with ULDL imbalance.
12/12/14   hdk   ASDiv feature: In DBDC disable ASDiv
09/04/15   hdk   W+W ASDiv feature
08/28/15   hdk   Update state to ASD TRM for all state transitions.
08/19/15   hdk   FR 27544, ASDiv Thr per band & FR 27546 SAR + ASDiv
07/01/15   svh   Update RF antenna config with correct device id.
06/18/15   hdk   Removing FEATURE_WCDMA_JOLOKIA_MODEM from DSDA ASDiv changes.
06/01/15   jhl   Fix compiler issue after ASDiv feature enabled
06/01/15   jhl   Added mutex for asdiv
05/26/15   jhl   Support ASDiv just for low band and disable for hig/mid band 
05/26/15   jhl   Adjust antenna switch block condition if primary 
                 antenna and diversity antenna has large energy delta
04/09/15   mk    Included l1extif_v.h.
02/18/15   jhl   Account for MPR value for MTPL based algorithm 
01/27/15   hdk   ASDiv Global re-org for W + W
01/20/15   jd    Global re-org for W + W
01/13/15   hdk   Updating ACQ state to TRM
11/21/14   hdk   ASDiv Change for DRDSDS
11/10/14   hdk   ASDiv test mode enhanced
10/06/14   sd    Fixed MSIM compilation warnings
10/07/14   sks   Compilation warning fix.
06/26/14   hdk   Aborting Type1 Switch if RxD is off while issuing ASDiv switch
06/24/14   hdk   Featurizing DSDA ASDiv changes under FEATURE_WCDMA_JOLOKIA_MODEM
06/13/14   hdk   ASDiv DSDA feature
03/20/14   hdk   Adding support for Suspended case for DSDS
03/04/14   hdk   Adding support for OOS case for DSDS.
02/08/14   vs    Asdiv DSDS changes.
12/02/13   vs    Added support for the API wcdma_asdiv_disable_mtpl_algorithm.
09/17/13   hk    added Event logs for ASDiv
02/05/13   vs    added the macro WL1AS_AVG_RSCP_THRESH_FOR_LOOK_BACK
01/30/13   dm    Keep the finger profile same when switching antennas for type 2 mode.
01/21/13   vs    Skip antenna switch measurements till the physical channel est completes.
01/12/13   rc    Added code stop As-div algo on DCH cleanup.
01/09/13   rc    Added function prototype.
12/19/12   rc    Added function prototype.
12/12/12   rc    Added into AS database.
12/06/12   vs    Added support for idle mode antenna switch diversity
12/05/12   vs    Added support for the type 2 algorithm.
12/05/12   vs    Prototype for the antenna switch mdsp pre callback.
11/28/12   vs    changed the element "counter_suspend_Ant_switching" to unit16
11/19/12   vs    added support for the MTPL algorithm and test mode 3.
11/17/12   vs    Turning ON RxD even when the RxD is On by default.
11/12/12   vs    Support for the dynamic algorithm changes and loopback probing.
11/12/12   vs    Handling of the condition when we are about to send the command to FW and RxD is not ON.
11/10/12   vs    Modified the arrays rscp_prim and rscp_div to uint16.
10/28/12   vs    created the new file for the antenna switch feature

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#ifdef FEATURE_WCDMA_ANT_SWITCH_SUPPORT
#include "customer.h"
#include "l1const.h"
#include "l1mathutil.h"
#include "nv.h"
#include "wfw_sw_cmd_proc_intf.h"
#include "trm.h"
#include "wl1m.h"
#include "wl1tri.h"
#include "event_defs.h"
#include "l1extif.h"
#include "l1extif_v.h"
#include "wnv_l1.h"
#include "mdspsync.h"
#include "wsrch.h"
#include "rfm.h"




/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
/* TRM client ID for UMTS */
#define WCDMA_ANT_TRM_CLIENT_ID 5
/* logging related : ASDiv metrics logging starts */
#define WCDMA_ANT_SWITCH_METRICS_calltype_BMSK 0x0000000F
#define WCDMA_ANT_SWITCH_METRICS_calltype_SHFT 0

#define WCDMA_ANT_SWITCH_METRICS_rxdstate_BMSK 0x00000010
#define WCDMA_ANT_SWITCH_METRICS_rxdstate_SHFT 4

#define WCDMA_ANT_SWITCH_METRICS_antennaindex_BMSK 0x00000020
#define WCDMA_ANT_SWITCH_METRICS_antennaindex_SHFT 5

#define WCDMA_ANT_SWITCH_METRICS_eul_BMSK 0x00000040
#define WCDMA_ANT_SWITCH_METRICS_eul_SHFT 6

#define WCDMA_ANT_METRICS_SET_VAL(buf, field, val) \
  (buf) &= (~WCDMA_ANT_SWITCH_METRICS_##field##_BMSK); \
  (buf) |= (((val) << WCDMA_ANT_SWITCH_METRICS_##field##_SHFT) & \
            (WCDMA_ANT_SWITCH_METRICS_##field##_BMSK))


#ifndef WCDMA_ANT_SWITCH_METRICS_LOG_PKT
#define WCDMA_ANT_SWITCH_METRICS_LOG_PKT WCDMA_ANT_SWITCH_METRICS_0x41B5_LOG_PKT
#else
#error "Redefinition of the log command code: WCDMA_ANT_SWITCH_METRICS_LOG_PKT"
#endif

#define WCDMA_ANT_SWITCH_METRICS_INVALID_VAL 0xFF
#define WCDMA_ANT_SWITCH_METRICS_LOG_NUM_BUF 2
#define NUM_MAX_WCDMA_ANT_SWITCH_METRICS_SAMPLES 50
#define WCDMA_ANT_SWITCH_METRICS_SAMPLE_SIZE sizeof(wcdma_ant_switch_metrics_info_struct_type)

#define WCDMA_ANT_SWITCH_METRICS_LOG_MAX_BUF_SIZE  ((sizeof(wcdma_ant_switch_metrics_info_struct_type)) * \
                                           NUM_MAX_WCDMA_ANT_SWITCH_METRICS_SAMPLES)

/* uplink API returns txagc/mpr value of 766 if they are invalid*/
#define WCDMA_ANT_SWITCH_TXAGC_MPR_INVALID_VALUE 766

#define ANT_SWITCH_MTPL_SWITCH_THRESHOLD 40
#define ANT_SWITCH_MAX_DELTA_BETWEEN_ANTENNA -9
#define ANT_SWITCH_DB10_TO_DB_RATIO (10)
/* macros for dbm to db10 conversion and vice versa*/
/*AGC = (db*10 -(-700 - (-512)))*/
#define ANT_SWITCH_TX_AGC_DBM10TODBM(x) ((x - 188)/ANT_SWITCH_DB10_TO_DB_RATIO)

#define ASDIV_MTPL_OF_CURR_ANT_EXT (rfm_wcdma_calculate_mtpl_asdiv_pos(wl1_upd_ant_cnf_to_ext(wsub_id)))

/* Header definition since the header definition is defined in log.c */
typedef PACKED struct PACKED_POST
{
  uint16 len;  /* Specifies the length, in bytes of 
                 the entry, including this header. */

  uint16 code; /* Specifies the log code for the 
                  entry as enumerated above.       
                  Note: This is specified as word 
                  to guarantee size.               */

  qword ts;    /* The system timestamp for the log entry. The upper 48 bits
                  represent elapsed time since 6 Jan 1980 00:00:00 
                  in 1.25 ms units. The low order 16 bits represent elapsed 
                  time since the last 1.25 ms tick in 1/32 chip units 
                  (this 16 bit counter wraps at the value 49152).      */
} wcdma_ant_switch_metrics_log_hdr_struct_type;


typedef PACKED struct PACKED_POST
{
  uint8 cfn_info;
  /* Number of frames before probing */
  uint16 frame_cnt2probe;
  /* Gives the info of the call type,antenna index etc */
  uint8 current_frame_stat;
  /* Gives the RSCP of the antenna 0 */
  uint16 rscp_0;
  /* Gives the RSCP of the antenna 1 */
  uint16 rscp_1;
  /* Gives the RxAgc of the antenna 0 */
  int16 rxagc_0;
  /* Gives the RxAgc of the antenna 1 */
  int16 rxagc_1;
  /* indicates if the probe is active for that frame or not */
  uint8 probe_indicator;
  /* indicates the txagc value in that frame */
  int16 txagc_value;
  /* indicates the MTPL value in that frame */
  int16 mtpl_value;
  /* indicates the counter 1 value of the MTPL algorithm */
  uint8 mtpl_counter1;
  /* indicates the counter 2 value of the MTPL algorithm */
  uint8 mtpl_counter2;
  /* indicates the counter 3 value of the MTPL algorithm */
  uint8 mtpl_counter3;
} wcdma_ant_switch_metrics_info_struct_type;

/* structure for the log packet wcdma_ant_switch_metrics */
LOG_RECORD_DEFINE(WCDMA_ANT_SWITCH_METRICS_LOG_PKT)
  /* Version field */
  uint8 version;
  /* Number of samples */
  uint8 num_samples;
  /* wcdma ant switch metrics samples */
  uint8 info[WCDMA_ANT_SWITCH_METRICS_LOG_MAX_BUF_SIZE];
LOG_RECORD_END

/* Globals for all the NV item reads related to the Antenna switching feature */

#define ANT_SWITCH_TIMER_VALUE (45) 
#define WL1AS_AVG_RSCP_THRESH_FOR_LOOK_BACK (80)
#define ANT_SWITCH_ACQ_TIMER_VALUE (30)
#define NUM_RSSI_PROBE_THR (3)
/* The structure is the database which will store all the counters/flag and the state of the Antenna switch algorithm */
typedef struct 
{
  int16 rscp_threshold;
  /* To store the rscp threshold at which the probe 2 will be triggered for non RxD case. This is read from the NV */
  uint8 tx_pwr_headroom;
  /* variable is used to compare the Tx power after the switch to the MTPL */
  uint32 confirm_cnt;
  /* Counter to count the number of successive hits in probe step 2 */
  uint8 probe_time;
  /* this is equal to 2 frames. Time we need to wait for the RxD to turn ON */
  uint8 probe_wait;
  /* Counter we decrement. when it is equal to 0, RxD would be ON */
  uint8 delta_rscp_switch_threshold;
  /* Used to store the power delta between the Pri and Div antennas. This value is read from the NV and saved here*/
  boolean in_probe;
  /* Set to TRUE when we are in probe state */
  uint16 probe_interval;
  /* Saves the number of frames we need to count down so that we can probe again */
  uint16 frame_cnt2probe;
  /* Counter to count down. When it reaches 0, we probe */
  boolean rxd_flag_ant_probe;
  /* this is for info that RxD is ON since Ant Switch has asked it to be ON */
  boolean suspend_Ant_switching_algo;
  /* Flag and counter for the shutting the algorithm down for specific frames after the switch*/
  uint16 counter_suspend_Ant_switching;

  /* Flags related to the searcher interaction */ 
  boolean ant_switch_searcher_probing_in_progress;
  /* This flag is set when the search results are being collected */
  boolean ant_switch_searcher_results_available;
  /* The flag is set to TRUE when the 4 searcher results are available for Pri and Div antennas*/
  uint8 search_results_counter;
  /* Counter used to count the number of searcher results. */
  uint16 rscp_prim[4];
  /* Array to store the Pri rscp values */
  uint16 rscp_div[4];
  /* Array to store the Div rscp values */

  /* Flag and counter for the After switching protection algorithm */ 
  boolean ant_switch_protection_flag;
  
  boolean ant_switch_second_level_protection_flag;
  /* This flag is to avoid the ping pong when we switch back after the recent switch */

  /* array to save the last 50 frame's rscp values */
  uint16 rscp_values[50];
  uint8 rscp_array_index;

  /* Flag related to the FW switch command*/
  boolean ant_switch_command_in_progress;
  boolean ant_switch_command_to_send;
  /* Counter to make sure RxD is indeed ON before actually proceeding with the antenna switch command */
  uint8 make_sure_rxd_on_counter;

  /* Loopback algorithm declarations */
  uint16 rscp_avg;
  int16 rscp_drop;
  uint8 rscp_buffer_size;
  uint8 look_back_dist;
  uint8 look_back_threshold;
  uint16 rscp_avg_stored[7];
  /* Counter to make sure if we have valid 7 readings in the array*/
  uint8 counter_loopback;
  
  /* Dynamic probing */
  int16 probe_delta_prev[NUM_RSSI_PROBE_THR-1];
  
  /* This flag is used to keep the UE in step 2 for a longer time */
  boolean proceed_to_step2_directly;

  /* Counter for waiting for 2 frames before calling the searcher APIs in case of the RxD ON case.*/
  uint8 counter_wait_for_rxd_on;
  boolean wait_for_rxd_on;

  /* MTPL based algorithm*/
  /* To save the ul tx samples of the first set of 50 samples */
  int16 ul_tx_samples1[50];
  /* To save the ul tx samples of the second set of 50 samples */
  int16 ul_tx_samples2[50];
  /* To keep the array index of ul_tx_samples1 */
  uint8 txagc_array1_index;
  /* To keep the array index of ul_tx_samples2 */
  uint8 txagc_array2_index;
  /* Counter for the percentage of time the Tx power touches the MTPL for the first set of samples */
  uint8 ul_tx_percentage_counter1;
  /* Counter for the percentage of time the Tx power touches the MTPL for the second set of samples */
  uint8 ul_tx_percentage_counter2;
  /* Flag to indicate that the ul_tx_samples2 values needs to be accumulated */
  boolean collect_txagc_2nd_array_samples;
  /* to store the percentage of time the tx power  */
  uint8 mtpl_percentage_pre_switch;
  /* To store the pre switch counter values */
  int16 mtpl_avg_pre_switch;
  /* To store the pre switch average value */
  /* post switch MTPL value array */
  int16 ul_tx_samples3[50];
  uint8 ul_tx_percentage_counter3;
  int16 probe_delta_latest;
  boolean skip_RSCP_decision;
  boolean ant_switch_algo_type1_stopped;

  /* Flag to make sure we skip the UL TX analysis when PA was off for even 1 frame */
  boolean pa_not_always_on_in_evaluation;
}wl1_as_db_type;


/* The structure is the database which will store all the counters/flag and the state of the Antenna switch algorithm type 2*/
typedef struct 
{

  uint16 frame_cnt2probe;
  /* Counter to count down. When it reaches 0, we probe */
  boolean suspend_Ant_switching_algo;
  /* Flag and counter for the shutting the algorithm down for specific frames after the switch*/
  uint16 counter_suspend_Ant_switching;

  boolean ant_switch_second_level_protection_flag;
  /* This flag is to avoid the ping pong when we switch back after the recent switch */

  /* array to save the last 50 frame's rscp values */
  uint16 rscp_values[50];
  uint8 rscp_array_index;

  /* Flag related to the FW switch command*/
  boolean ant_switch_command_in_progress;
  boolean ant_switch_command_to_send;

  /* Loopback algorithm declarations */
  uint16 rscp_avg;
  int16 rscp_drop;
  uint8 rscp_buffer_size;
  uint8 look_back_dist;
  uint8 look_back_threshold;
  uint16 rscp_avg_stored[7];
  /* Counter to make sure if we have valid 7 readings in the array*/
  uint8 counter_loopback;
  

  /* MTPL based algorithm*/
  /* To save the ul tx samples of the first set of 50 samples */
  int16 ul_tx_samples1[50];
  /* To save the ul tx samples of the second set of 50 samples */
  int16 ul_tx_samples2[50];
  /* To keep the array index of ul_tx_samples1 */
  uint8 txagc_array1_index;
  /* To keep the array index of ul_tx_samples2 */
  uint8 txagc_array2_index;
  /* Counter for the percentage of time the Tx power touches the MTPL for the first set of samples */
  uint8 ul_tx_percentage_counter1;
  /* Counter for the percentage of time the Tx power touches the MTPL for the second set of samples */
  uint8 ul_tx_percentage_counter2;
  /* Flag to indicate that the ul_tx_samples2 values needs to be accumulated */
  boolean collect_txagc_2nd_array_samples;
  /* to store the percentage of time the tx power  */
  uint8 mtpl_percentage_pre_switch;
  /* To store the pre switch average value */
  int16 mtpl_avg_pre_switch;
  /* To store the pre switch Avg Tx value (TxAGC units dB10) */
  int16 avg_tx_pwr_pre_switch;

  /* post switch MTPL value array */
  int16 ul_tx_samples_post_switch[45];
  uint8 ul_tx_percentage_counter3;
  int16 rscp_values_post_switch[45];

  /* to store rscp avgerage */
  uint16 rscp_avg_pre_switch;

  /* variables for the type 2 algorithm thresholds */
  int8 threshold_lut[9];
  uint8 threshold_lut_array_index;

  boolean ant_switch_algo_type2_stopped;
  /* Flag to make sure we skip the UL TX analysis when PA was off for even 1 frame */
  boolean pa_not_always_on_in_evaluation;
}wl1_as_db_type2;


/* Typedef declaration for the CME callback function */
typedef void ANT_SWITCH_CME_CB_FUNC_TYPE(wsub_id_e_type wsub_id);

/* Enum for the current antenna switch algorithm*/
typedef enum 
{
  WL1_ANT_SWITCH_ALGO_NONE,
  WL1_ANT_SWITCH_ALGO_TYPE1,
  WL1_ANT_SWITCH_ALGO_TYPE2
}wl1_as_algo_type;

/* Enum for the current controller support */
typedef enum 
{
  WL1_ANT_SWITCH_CONTROLLER_NONE,
  WL1_ANT_SWITCH_CONTROLLER_TYPE1,
  WL1_ANT_SWITCH_CONTROLLER_TYPE2,
}wl1_as_controller_type;

/* Enum for clients that block asdiv support */
typedef enum 
{
  WL1_ASDIV_BLOCK_NO_CLIENT,
  WL1_ASDIV_BLOCK_SAR,
  WL1_ASDIV_BLOCK_DBDC,
}wl1_asdiv_block_client_type;

/* ASDiv Config packet */
#ifndef WCDMA_ANT_SWITCH_CONFIG_LOG_PKT
#define WCDMA_ANT_SWITCH_CONFIG_LOG_PKT WCDMA_ANT_SWITCH_CONFIG_0x41B4_LOG_PKT
#else
#error "Redefinition of the log command code: WCDMA_ANT_SWITCH_CONFIG_LOG_PKT"
#endif

#define WCDMA_ANT_SWITCH_CONFIG_LOG_NUM_BUF 1

/* Header definition since the header definition is defined in log.c */
typedef PACKED struct PACKED_POST
{
  uint16 len;  /* Specifies the length, in bytes of 
                 the entry, including this header. */

  uint16 code; /* Specifies the log code for the 
                  entry as enumerated above.       
                  Note: This is specified as word 
                  to guarantee size.               */

  qword ts;    /* The system timestamp for the log entry. The upper 48 bits
                  represent elapsed time since 6 Jan 1980 00:00:00 
                  in 1.25 ms units. The low order 16 bits represent elapsed 
                  time since the last 1.25 ms tick in 1/32 chip units 
                  (this 16 bit counter wraps at the value 49152).      */
} wcdma_ant_switch_config_log_hdr_struct_type;

/* structure for the log packet ant_switch_config */
LOG_RECORD_DEFINE(WCDMA_ANT_SWITCH_CONFIG_LOG_PKT)
  /* Version field */
  uint8 version;
  /* UE state..DCH or FACH */
  uint8 ue_state;
  /* type of the algorithm configured */
  uint8 asdiv_algorithm_type;
  /* rscp power delta for the type 1 algorithm */
  uint8 type1_rscp_power_delta_threshold;
  /* rscp threshold for the type1 switch */
  uint8 type1_rscp_switch_threshold;
  /* rscp threshold for the loopback switching type1  */
  uint8 type1_rscp_loopback_threshold;
  /* type2 rscp switch threshold */
  uint8 type2_rscp_switch_threshold;
  /* threshold for comparing the counter values */
  uint8 type2_mtpl_switch_threshold;
  /* Number of frames for which the algo will be suspended after a switchback */
  uint16 type2_post_switchback_sus_time;
  /* the threshold base used to compare the post switch evaluation */
  int8 type2_post_switch_evaluation_base_threshold;
  /* type 2 algorithm rscp drop threshold */
  uint8 type2_rscp_drop_threshold;
  /* rscp threshold value when comparing the rscp values post switchback */
  uint8 type2_avg_rscp_threshold_loopback;
  /* band 1 bitmask */
  uint64 band1_bitmask;
  /* band 2 bitmask */
  uint64 band2_bitmask;
  /* rxd nv item value */
  uint8 rxd_nv_config;
  /* Switch config . 0:straight and 1:CrossSwitch*/
  uint8 antenna_switch_config;
  /* the test mode control for antenna diversity */
  uint8 antenna_switch_testmode_control;
LOG_RECORD_END

/* STRUCTURE DEFINITIONS related to the EVENTS */
/* --------------------------------------------- */

/* EVENT_ASDIV_TYPE1_SWITCH_RSCP_BASED */ 
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  uint16 pri_rscp_avg_value;
  uint16 div_rscp_avg_value;
  int16 probe_delta;
  int16 probe_threshold;
}
wcdma_asdiv_type1_switch_rscp_based_evt_type;

/* EVENT_ASDIV_TYPE1_SWITCH_TXAGC_BASED */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  uint8 counter1;
  uint8 counter2;
}
wcdma_asdiv_type1_switch_txagc_based_evt_type;


/* EVENT_ASDIV_TYPE1_SWITCHBACK_MTPL_BASED */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  uint8 preswitch_counter_value;
  uint8 postswitch_counter_value;
  uint8 threshold;
}
wcdma_asdiv_type1_switchback_mtpl_based_evt_type;

/* EVENT_ASDIV_TYPE1_SWITCHBACK_TXAGC_BASED */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  int16 preswitch_txagc_avg;
  int16 postswitch_txagc_avg;
  uint8 threshold;
}
wcdma_asdiv_type1_switchback_txagc_based_evt_type;

/* EVENT_ASDIV_TYPE2_SWITCH_RSCP_BASED */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  uint16 rscp_avg;
  uint16 rscp_threshold;
}
wcdma_asdiv_type2_rscp_based_evt_type;

/* EVENT_ASDIV_TYPE2_SWITCH_MTPL_BASED */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  uint8 counter1;
  uint8 counter2;
}
wcdma_asdiv_type2_mtpl_based_evt_type;

/* EVENT_ASDIV_TYPE2_SWITCHBACK_RSCP_LOOPBACK_BASED */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  uint16 rscp_drop;
  uint16 prev_rscp_value;
  uint16 current_rscp_value;
  uint8 loopback_threshold;
  uint8 rscp_threshold;
}
wcdma_asdiv_type2_switchback_rscp_loopback_based_evt_type;

/* EVENT_ASDIV_TYPE2_SWITCHBACK_DEGRADATION_BASED */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  int16 ul_degradation;
  int16 dl_degradation;
  uint16 threshold;
}
wcdma_asdiv_type2_switchback_degradation_evt_type;


/* EVENT_ASDIV_PROBE_BOTH_ANTENNAS */
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint16 pri_avg_rscp;
  uint16 div_avg_rscp;
}
wcdma_asdiv_Probe_both_antennas_evt_type;


/*Structure for switching during IDLE and ACQ (non Connected modes)*/
typedef PACKED struct PACKED_POST
{
  uint8 cfn;
  uint8 switched_antID;
  int16 pri_ant_rxagc;
  int16 div_ant_rxagc;
}wcdma_asdiv_Idl_Acq_sw_evt_type;


/* Global to store the Switch config from MCS. this will be used for TRM usage */

typedef enum 
{
  WL1_ANT_ACQ,
  WL1_ANT_FACH,
  WL1_ANT_DCH,
  WL1_ANT_FACH_EXIT,
  WL1_ANT_DCH_EXIT,
  WL1_ANT_IDLE_WAKEUP,
  WL1_ANT_IDLE_SLEEP,
  WL1_ANT_OOS,
  WL1_ANT_SUSPEND
 } l1_state_information_for_trm_type;

typedef struct
{
  /* Tells if the feature is enabled or disabled */
  boolean enable_disable_ant_switch_feature;

  trm_ant_switch_div_config_type ant_switch_cnfg;
  rex_crit_sect_type asdiv_mutex;
  
}wl1antswitch_ext_cntrl_params_type;

#define WASDIV_WBAND_MIN SYS_BAND_WCDMA_I_IMT_2000
#define WASDIV_WBAND_MAX SYS_BAND_WCDMA_XIX_850


extern wl1antswitch_ext_cntrl_params_type wl1antswitch_ext_cntrl_params[WCDMA_NUM_SUBS];

/* --------------------------------------
***     Function Prototypes 
----------------------------------------*/ 

/*===========================================================================
FUNCTION wl1antswitch_int_cntrl_params_init

DESCRIPTION
  This function initlizes internal parametes

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wl1antswitch_int_cntrl_params_init( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION wl1_as_antenna_switch
DESCRIPTION
  Checks for a number of prerequisites before calling the mdsp command to send the antenna switch command to FW.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_antenna_switch( wsub_id_e_type wsub_id);

extern trm_ant_switch_div_config_type wl1_upd_ant_cnf_to_ext(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_as_antenna_switch_type2
DESCRIPTION
  Checks for a number of prerequisites before calling the mdsp command to send the antenna switch command to FW.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_antenna_switch_type2( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_antenna_switch_done
DESCRIPTION
  Callback function which would be called after the switch is done by the FW.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_as_antenna_switch_pre_callback( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_as_antenna_switch_cmd_done

DESCRIPTION
    This is supposed to be executed after the FW sends the antenna switch command done.
 
RETURN VALUE
   None.
SIDE EFFECTS
  None.
=============================================================================*/

extern void wl1_as_antenna_switch_cmd_done(uint16 status, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_antenna_switch_done
DESCRIPTION
  Callback function which would be called after the switch is done by the FW.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_antenna_switch_done( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION wl1m_as_determine_as_switch_support
DESCRIPTION

DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/

extern void wl1m_as_determine_as_switch_support( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_as_cleanup
DESCRIPTION
 
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/

extern void wl1_as_cleanup( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_as_antenna_switch_done_type2
DESCRIPTION
  Callback function which would be called after the switch is done by the FW.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_antenna_switch_done_type2( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_init
DESCRIPTION
  This initializes all the variables and counters in the global database for antenna switch
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_init( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_init_type2

DESCRIPTION
    AS init
 
RETURN VALUE
   None.
SIDE EFFECTS
  None.
=============================================================================*/
extern void wl1_as_init_type2( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_reset
DESCRIPTION
  This reinitializes all the variables and counters in the global database for antenna switch
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_reset( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_reset_type2

DESCRIPTION
    AS reset
 
RETURN VALUE
   None.
SIDE EFFECTS
  None.
=============================================================================*/
extern void wl1_as_reset_type2( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_controller_type2
DESCRIPTION
  The main function which runs the algorithm for probing in the antenna switch feature algorithm type2.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_controller_type2(uint8 cfn_number, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_controller
DESCRIPTION
  The main function which runs the algorithm for probing in the antenna switch feature.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_controller(uint8 cfn_number, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_ant_nv_read
DESCRIPTION
  Reads the AS NV value and sets the overrides appropriately.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_ant_nv_read( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_ant_switch_band_nv_read
DESCRIPTION
  Reads the AS NV value and sets the overrides appropriately.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_ant_switch_band_nv_read( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION ant_switch_register_with_cme_cb
DESCRIPTION
  The function is used by CME to register the callback with antenna switch module.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void ant_switch_register_with_cme_cb(ANT_SWITCH_CME_CB_FUNC_TYPE *ant_switch_cme_cb, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION ant_switch_deregister_with_cme_cb
DESCRIPTION
  The function is used by CME to deregister the callback with antenna switch module.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void ant_switch_deregister_with_cme_cb( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_as_searcher_results_collection
DESCRIPTION
  The function collects the searcher results.
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_searcher_results_collection( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_searcher_results_verdict
DESCRIPTION
  The function gives the verdict about the probe based on the searcher results
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_as_searcher_results_verdict(int16 temp_probe_delta, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION wl1_as_searcher_results_verdict
DESCRIPTION
  The function gives the verdict about the probe based on the searcher results
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_ant_switch_idle_mode(event_id_enum_type event_id, wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION wl1_ant_switch_acq_mode
DESCRIPTION
  
DEPENDENCIES
  None.
RETURN VALUE
  None
SIDE EFFECTS
  None
 ===========================================================================*/
extern boolean wl1_ant_switch_acq_mode( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1_ant_switch_measurements_skip_for_phy_chan_ext

DESCRIPTION
  This function sets the flag "ant_switch_measurements_skip_for_phy_chan_ext" to TRUE

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
 ===========================================================================*/

extern void wl1_ant_switch_measurements_skip_for_phy_chan_ext(boolean, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1_ant_switch_get_algo_type

DESCRIPTION
  This function returns the variable value of the global "ant_switch_algorithm_type"

DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None
 ===========================================================================*/

extern uint8 wl1_ant_switch_get_algo_type(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION ant_switch_config_submit_log_buffer

DESCRIPTION
  This function is called to post ant switch config submit command. If the 
  buffer is not ready to be submitted to diag the function returns without
  doing anything. This can so happen because multiple logs are instructed
  to be submitted to diag by the same L1 command.
  It checks for the log code in diag and submits the packet.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ant_switch_config_submit_log_buffer( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION ant_switch_config_submit_log_cmd

DESCRIPTION
  This function send ANT SWITCH CONFIG log packet local command to L1M.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void ant_switch_config_submit_log_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_config_log_update

DESCRIPTION
  This function is used for the  ant switch config logging accumulation.
  The accumulation happens one time.
  
DEPENDENCIES
  None

RETURN VALUE
  

SIDE EFFECTS
  None
===========================================================================*/
extern void wcdma_ant_switch_config_log_update( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type1_switch_rscp_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event EVENT_ASDIV_TYPE1_SWITCH_RSCP_BASED

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_ant_switch_type1_switch_rscp_evt_logging(
     int16 probe_delta,int16 probe_threshold, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type1_switch_txagc_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event EEVENT_ASDIV_TYPE1_SWITCH_TXAGC_BASED

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_ant_switch_type1_switch_txagc_evt_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type1_switchback_mtpl_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event EVENT_ASDIV_TYPE1_SWITCHBACK_MTPL_BASED

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_ant_switch_type1_switchback_mtpl_evt_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type1_switchback_txagc_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event EVENT_ASDIV_TYPE1_SWITCHBACK_TXAGC_BASED

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_ant_switch_type1_switchback_txagc_evt_logging(int16 postswitch_txagc, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type2_switch_rscp_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event EVENT_ASDIV_TYPE2_SWITCH_RSCP_BASED

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void wcdma_ant_switch_type2_switch_rscp_evt_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type2_switch_mtpl_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event  EVENT_ASDIV_TYPE2_SWITCH_MTPL_BASED

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_ant_switch_type2_switch_mtpl_evt_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type2_switch_rscp_loopback_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event  EVENT_ASDIV_TYPE2_SWITCH_RSCP_LOOPBACK_BASED

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_ant_switch_type2_switch_rscp_loopback_evt_logging( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type2_switchback_degradation_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event  EVENT_ASDIV_TYPE2_SWITCHBACK_DEGRADATION_BASED

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_ant_switch_type2_switchback_degradation_evt_logging(int16 dl_degradation,int16 ul_degradation, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_type2_probe_both_antennas_evt_logging

DESCRIPTION
  This function generates the event related to ASDiv feature. This is called 
  for logging the event  EVENT_ASDIV_PROBE_BOTH_ANTENNAS

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_ant_switch_type2_probe_both_antennas_evt_logging(uint32 rscp_pri,uint32 rscp_div, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_metrics_log_init
 
DESCRIPTION This function initializes the log states for ant switch metrics 
            log packet
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wcdma_ant_switch_metrics_log_init(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_metrics_log_update

DESCRIPTION
  This function is used for the  wcdma ant switch metrics logging accumulation.
  
  It is called every frame to collect the data corresponding to the sample.
  
DEPENDENCIES
  None

RETURN VALUE
  
SIDE EFFECTS
  None
===========================================================================*/
void wcdma_ant_switch_metrics_log_update( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_metrics_stop_logging

DESCRIPTION
  This function prepares the wcdma ant switch metrics buffer to be submitted to diag
  It calls the function that posts a wcdma periodic stat log submit local command and then
  clears the state of wcdma ant switch metrics logging.
  
DEPENDENCIES
  wcdma_ant_switch_metrics_log_info_buf_idx should be valid while making
  a call to this function.

RETURN VALUE
  None

SIDE EFFECTS
  ===========================================================================*/
void wcdma_ant_switch_metrics_stop_logging( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION wcdma_ant_switch_metrics_send_log_submit_cmd

DESCRIPTION
  This function makes a request to post a local command to submit the 
  currently accumulated log packet to diag. Before it does so, it checks if 
  a previous log packet that was submitted to diag has been  serviced or not.
  If not, it drops the current packet and proceeds further.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void wcdma_ant_switch_metrics_send_log_submit_cmd(
  /* wcdma periodic stat log buffer index to submit */
  uint8 wcdma_ant_switch_metrics_log_info_buf_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_metrics_submit_log_buffer

DESCRIPTION
  This function is called to post wcdma ant switch metrics submit command. If the 
  buffer is not ready to be submitted to diag the function returns without
  doing anything. This can so happen because multiple logs are instructed
  to be submitted to diag by the same L1 command.
  It checks for the log code in diag and submits the packet.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Flag indicating the log buffer is submitted for flushing is reset.
===========================================================================*/
void wcdma_ant_switch_metrics_submit_log_buffer( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_metrics_submit_log_cmd

DESCRIPTION
  This function send ANT SWITCH METRICS submit log packet local command to L1M.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void wcdma_ant_switch_metrics_submit_log_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_algorithm_block

DESCRIPTION
  This function sets the FLAG to block the ASDiv algorithm.
  If Force block is set it deletes the pending mdsp ASDiv switch command 
  and forces block

DEPENDENCIES
  None

RETURN VALUE
  boolean. 
Returns TRUE if the algorithm can be blocked.
Returns FALSE if the algorithm cannot be blocked now.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean wcdma_ant_switch_algorithm_block( boolean force_block_asd, wl1_asdiv_block_client_type client, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_algorithm_unblock

DESCRIPTION
  This function resets the FLAG to unblock the ASDiv algorithm.

DEPENDENCIES
  None

RETURN VALUE
  boolean. 
Returns TRUE if the algorithm can be unblocked.

SIDE EFFECTS
  None
===========================================================================*/

extern boolean wcdma_ant_switch_algorithm_unblock( wl1_asdiv_block_client_type client, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_is_ant_switch_blocked

DESCRIPTION
  This function returns the FLAG VALUE of block_asdiv_flag

DEPENDENCIES
  None

RETURN VALUE
  boolean. 

SIDE EFFECTS
  None
===========================================================================*/

inline boolean wcdma_is_ant_switch_blocked(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_ant_switch_toggled_value

DESCRIPTION
  This function returns the toggled value of the switch config.
  It doesnt toggle the global value of the switch config.

DEPENDENCIES
  None

RETURN VALUE
  trm_ant_switch_div_config_type. 

SIDE EFFECTS
  None
===========================================================================*/

inline trm_ant_switch_div_config_type  wcdma_ant_switch_toggled_value(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wcdma_asdiv_disable_mtpl_algorithm

DESCRIPTION
  This function sets the global flag "asdiv_mtpl_algorithm_disable".
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wcdma_asdiv_disable_mtpl_algorithm(boolean value, wsub_id_e_type wsub_id);


/*=========================================================================

 FUNCTION     :wl1_ant_set_switch_cb
 
 DESCRIPTION  : This is the callback function for the SGLTE ANT SWITCH to switch antenna
              
 DEPENDENCIES : 

 INPUT PARAMETERS: 
 	trm_ant_switch_set_mode_return_type set_mode_status,
 	trm_ant_switch_div_config_type switch_div_config

 RETURN VALUE : 
       
 SIDE EFFECTS : 
==========================================================================*/
extern void wl1_ant_set_switch_cb(trm_ant_switch_cb_info_type switch_info);

/*===========================================================================

FUNCTION wl1_ant_update_trm_about_state_transition

DESCRIPTION
  This function calls the TRM api and registers the callback for the antenna switch

DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None
 ===========================================================================*/
extern void wl1_ant_update_trm_about_state_transition(l1m_state_enum_type state, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1_ant_switch_set_control_with_trm

DESCRIPTION
  This function calls the TRM api and registers the control for the antenna switch

DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None
 ===========================================================================*/

void wl1_ant_switch_set_control_with_trm(trm_ant_switch_set_ctrl_enum req_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1_ant_switch_qta_state_update

DESCRIPTION
  This function updates the QTA state for antenna switch diversity and locks TRM ASDiv Switch.
  If the TRM ASDiv Switch lock fails, it returns FALSE else TRUE.

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None
 ===========================================================================*/

extern boolean wl1_ant_switch_qta_state_update(boolean status, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION wl1_ant_switch_idle_wake_up

DESCRIPTION
  This function is called in wl1sleep module while waking up from Sleep

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None
 ===========================================================================*/
void wl1_ant_switch_idle_wake_up( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION wl1_ant_switch_oos_idle_sleep

DESCRIPTION
This function determines is sleep a result of OOS or Idle sleep. If it is OOS sleep then performs 
an antenna switch.

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None
 ===========================================================================*/
void wl1_ant_switch_oos_idle_sleep( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1_ant_switch_set_mode_switch_permission_with_trm_for_sleep

DESCRIPTION
  This function calls the TRM api and registers the callback for the antenna switch

DEPENDENCIES
  None.

RETURN VALUE
  uint8

SIDE EFFECTS
  None
 ===========================================================================*/

extern void wl1_ant_switch_set_mode_switch_permission_with_trm_for_sleep(l1_state_information_for_trm_type state, wsub_id_e_type wsub_id);

/*=========================================================================

 FUNCTION     :wl1_as_trm_passive_switch_cmd
 
 DESCRIPTION  : This call back function is used by TRM to notify WL1 for INITATING an Antenna 
 Switch and indicating the Antenna Switch is COMPLETE.
              
 DEPENDENCIES : 

 INPUT PARAMETERS: trm_ant_switch_cb_info_type

 RETURN VALUE : void
       
 SIDE EFFECTS : 
==========================================================================*/

void wl1_as_trm_passive_switch_cmd(trm_ant_switch_cb_info_type ant_cfg_info, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1_ant_switch_idle_testmode

DESCRIPTION
  This function toggles the preferred antenna every ant_switch_dwelling_time sleep cycles.

DEPENDENCIES
  None.

RETURN VALUE
  void

SIDE EFFECTS
  None
 ===========================================================================*/

void wl1_ant_switch_idle_testmode(  wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_as_get_txagc_and_mpr_hist

DESCRIPTION
  This function is called from wl1 antenna switch controller and updates history
  table of txAGC values for past 15 slots in TX AGC units, taking into
  account HDET corrections and CM slots

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  The average value of txagc and mpr from the past 15 slots will be calculated 
==========================================================================*/
void wl1_as_get_txagc_and_mpr_hist(int16 *temp_txagc_average, int16 *temp_mpr_average, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1m_as_determine_as_band_support

DESCRIPTION
  This function is to determine ASDiv controller type support based
  on current band information

DEPENDENCIES
  None.

RETURN VALUE
  Return the ASDiv controller type which is supported

SIDE EFFECTS
 
 ===========================================================================*/

extern wl1_as_controller_type wl1m_as_determine_as_band_support(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION wl1m_as_set_rf_config_for_acq

DESCRIPTION
  Set the correct RF config. If W ASD is enabled, set the "current config".
  If W ASD is not enabled, set the "default config".

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
 
 ===========================================================================*/
extern void wl1m_as_set_rf_config_for_acq(wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION wl1_ant_switch_db_dc_block_asdiv

DESCRIPTION
    This function blocks ASDiv when DB DC config is being recived from RRC
    and unblocks once DB DC is deconfigured from L1.

    DB-DC ASDiv disable requirements:
    1) Block ASDiv.
    2) Set the antenna switch config to the required DB-DC config 
       specified through NV.

    This is function is called before dl carrconfig issues RF tune command for
    DB-DC, if ASDiv can be blocked function return TRUE if not FALSE.
    If ASDiv can't be blocked then the DB-DC config is suspended till ASDiv 
    switch is complete. DB-DC config is resumed through the call back passed 
    by dlcarrcfg module.
 
RETURN VALUE
   Boolean.
   if ASDiv can be blocked function returns TRUE if not FALSE.
 
SIDE EFFECTS
  None.
=============================================================================*/
boolean wl1_ant_switch_db_dc_block_asdiv(boolean db_dc_disable_asdiv, 
                                         void(* dlcarrcfg_callback)(wsub_id_e_type wsub_id),
                                         wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1as_register_ant_switch_complete_cb

DESCRIPTION
  Register Antenna switch complete call back
 
RETURN VALUE
   BOOLEAN
 
SIDE EFFECTS
  None.
=============================================================================*/

void wl1as_register_ant_switch_complete_cb( void(*callback_to_register)(wsub_id_e_type wsub_id),
                                            wsub_id_e_type wsub_id );
/*===========================================================================

FUNCTION wcdma_asdiv_qxdm_debug_cmd

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    None.

Example command:
     send_data 75 4 54 0 1 <val> - for datapath_f3_debug
                           <val> - 16 bit value of debug variable (bytes reversed)
for providing hex value append val with 0x

For example, to set datapath_f3_debug to 0x23, use the following command:
send_data 75 4 54 0 1 0x23 0

===========================================================================*/
void wcdma_asdiv_qxdm_debug_cmd ( uint8 debug_var_type,uint16 debug_var_val,wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION wl1_ant_unsuspend_cmd

DESCRIPTION
   function to unsuspend asd cmd to fw.
 
RETURN VALUE
   None.

SIDE EFFECTS
  None.
=============================================================================*/
extern void wl1_ant_unsuspend_cmd(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION wl1_ant_upd_pre_switch_agc

DESCRIPTION
  This function updates pre switch agc value.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_ant_upd_pre_switch_agc(int16 agc_val, wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION  wl1_qsh_mdump_collect_asd

DESCRIPTION
  Collect ASDiv variables.

DEPENDENCIES
  None

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_asd(wsub_id_e_type wsub_id);
#endif

/*===========================================================================

FUNCTION wcdma_asdiv_sar_forced_antenna_config

DESCRIPTION

DEPENDENCIES
    None.

RETURN VALUE
    None.

SIDE EFFECTS
    Conurrency with disable ASDiv with DB-DC will have issues since both use the 
    same Block ASDiv interface. 
    To resolve a notion of which service blocked ASDiv needs to introduced. 
    A voting mechanism

===========================================================================*/
void wcdma_asdiv_sar_forced_antenna_config(trm_ant_switch_div_config_type curr_ant_sw_cnfg, wsub_id_e_type wsub_id);


extern void wcdma_asdiv_forced_ant_sw_cmplt_cb(wsub_id_e_type wsub_id);

#endif /*Antenna Switch feature*/
#endif /* WL1ANTSWITCH_H ends */
