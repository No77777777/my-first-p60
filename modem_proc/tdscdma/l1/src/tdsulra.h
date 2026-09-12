#ifndef TDS_ULRA_H
#define TDS_ULRA_H

/*===========================================================================
                            T D S U L R A . H

GENERAL DESCRIPTION
  This header file contains the MACRO definitions, variables and functions
  statement for tdsulra.c
  
EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS  
  

Copyright (c) 2011 by Qualcomm Technologies Incorporated.  All Rights Reserved.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsulra.h#1 $ 
  $DateTime: 2019/08/19 10:51:23 $ 
  $Author: pwbldsvc $

when        who     what, where, why
--------    ---     --------------------------------------------------------
07/06/11    cdf     File created.
                    
===========================================================================*/
#include "tdscdma_variation.h"
#include "tdsl1rrcif.h"
#include "tfw_sw_intf_msg.h"

#ifdef FEATURE_QSH_EVENT_METRIC
#include "tdsl1_qsh_ext.h"
#endif

/* ----------------------------------------------------------------------- */
/*                          Macro and Constant                             */
/* ----------------------------------------------------------------------- */

/* The maximum pending response from FW. Because UL sends one more probe
 * to FW ahead, so there may be 2 FPACH responses from FW at most 
 */
#define TDS_UL_RA_MAX_ISR_PENDING_RSP_CNT       2

/* The maximum number of subframe to wait for FPACH_DEC_DONE ISR, in case of 
 * FW doesn't send any response in some exception condition
 */
#define TDS_UL_RA_MAX_SUBFRM_TO_WAIT            30

/* The timing advance of the UpPCH default value */
#define TDS_DEFAULT_UPPCH_TIMING_ADVANCE        48
/* MAX value of timing advance in 1/8 chip: (96*8 - 1)= 767 */
#define TDS_MAX_TIMING_ADVANCE_CX8              767

#define TDSULRA_TA_SUCCESS_RATE_ACCURACY_LEVEL (14)
#define TDSULRA_MEDIAN_TA_SUCCESS_RATE (1<<(TDSULRA_TA_SUCCESS_RATE_ACCURACY_LEVEL-1))

#define TDSULRA_MAX_TA_OPT_TABLE_SIZE (7)
#define TDSULRA_MAX_TADV  TDS_DEFAULT_UPPCH_TIMING_ADVANCE
#define TDSULRA_FPACH_ACK (1)
#define TDSULRA_FPACH_NACK (0)


#define TDS_UL_RA_ISR_LOCK()                    REX_ISR_LOCK(&tdsulra_isr_mutex)
#define TDS_UL_RA_ISR_UNLOCK()                  REX_ISR_UNLOCK(&tdsulra_isr_mutex)


/* ----------------------------------------------------------------------- */
/*                        Enumeration Definition                           */
/* ----------------------------------------------------------------------- */
typedef enum
{
  TDS_UL_RA_ACTIVE_MODE_NONE,
  TDS_UL_RA_ACTIVE_MODE_PRACH,
  TDS_UL_RA_ACTIVE_MODE_HHO_RESYNC,
  TDS_UL_RA_ACTIVE_MODE_ERUCCH
} tdsulra_active_mode_enum_type;


typedef enum
{
  TDS_UL_RA_STATE_INACTIVE,
  TDS_UL_RA_STATE_READY,
  TDS_UL_RA_STATE_SYNC,
  TDS_UL_RA_STATE_TX,
  TDS_UL_RA_STATE_ERROR
} tdsulra_state_enum_type;

typedef enum
{
  TDS_UL_RA_NONE_ACTION,
  TDS_UL_RA_DATA_TX_FRIST_RF,   /* To transmit first 10ms RACH or E-RUCCH data */
  TDS_UL_RA_DATA_TX_SECOND_RF,  /* To transmit second 10ms RACH data, for 20ms TTI RACH only */
  TDS_UL_RA_DATA_TX_DONE        /* To report RACH_DONE to MAC or E-RUCCH done to HSUPA */
} tdsulra_data_tx_action_enum_type;

/* ----------------------------------------------------------------------- */
/*                         Structure Definition                            */
/* ----------------------------------------------------------------------- */

typedef struct
{
  /* Indicates the count of fpach success (FPACH success at the 1st UpPCH. */
  uint32 fpach_success_on_first_uppch;
  /* Indicates the count of fpach success (FPACH success at the Max Number of UpPCH. */
  uint32 fpach_success_on_max_uppch_cnt;
  /* Indicates the count of fpach failure */
  uint32 fpach_failure_cnt;
  /* Indicates the count of uppch attempts */
  uint32 uppch_attemp_cnt;
  /* Indicates the count of RACH on max uppch pwr count */
  uint32 rach_on_max_uppch_pwr_cnt;
  /* Indicates the average tx power upon RACH success */
  int32 avg_tx_pwr_on_rach_success_q4;

  int8 avg_prach_tx_pwr_dbm_q4;
}tdscdma_rach_phy_stats_type;


typedef struct
{
  /*----------------------------------------------------------
   * general control information 
   *----------------------------------------------------------*/

  /* The purpose for this random access procedure:
   * RACH transmission, HHO-resync or E-RUCCH
   */
  tdsulra_active_mode_enum_type     active_mode;

  /* State of random access procedure */
  tdsulra_state_enum_type           curr_state;

  /* Phychan abort pending flag. If PRACH/E-RUCCH is requested to
   * be dropped, but the proc is ongoing (waiting for FPACH DEC DONE or waiting
   * for Tx Done, this flag is set to hold on the drop action until the proc
   * is aborted done
   */
  boolean                           abort_pending_flag;

  /* Whether the random access procedure is suspend.
   * It's for PRACH only. It will be set to TRUE to suspend RACH TX when perform
   * FACH->DCH state change, CELL_FACH cell transition, or Intra- or Inter-freq 
   * neighbor PCCPCH decoding
   */
  boolean                           suspend_flag;
  
  /*if the probe is conflict with QTA gap, this flag will be set to TRUE*/
  boolean                           qta_conflict_flag;
  /*probe index in uppch_tx_subfrm_list, for qta query */
  uint8                               probe_index;
  /* UpPCH tx subframe (OTA) for the all the probes */
  uint16                             uppch_tx_subfrm_list[8];
  /* WT value used for all the probes*/
  uint8                               wt_list[8];
  
  /*----------------------------------------------------------
   * static parameters from higher layer configurations
   *----------------------------------------------------------*/

  /* available SYNC_UL codes for this random access proc */
  uint8                             avail_sync_code;

  /* sub-channel size */
  uint8                             sub_chan_size;

  /* available sub-channels for this random access proc */
  uint8                             avail_sub_chan;

  /* desired UpPCH received power, in dBm (-120..-58) */
  int8                              prx_uppch_des;

  /* power ramp step for next probe when no FPACH received after sent SYNC_UL */
  uint8                             pwr_ramp_step;

  /* maximum numbers of SYNC_UL transmissions */
  uint8                             max_sync_ul_tx;

  /* UpPCH shift, in 16chips (0..127) */
  uint8                             uppch_shift;

  /*----------------------------------------------------------
   * dynamic parameters used in SYNC state
   *----------------------------------------------------------*/
  /* The flag to indicate whether use initial time adv when sending 
   * UPPCH_TX_PWR_TIMING_CONFIG_CMD. 
   * When any one of the following four events happens, UL shall
   * not use initial timeAdv:
   * 1. UL DPCH setup
   * 2. UL non-schedule E-PUCH setup
   * 3. First E-AGCH grant decoded (added later in Rel-9)
   * 4. First HS-SCCH decoded (added later in Rel-9)
   * When any of the following happens, UL shall use initial timeAdv
   * 1. All 4 channels above are released
   * 2. HHO
   * 3. Cell Update after cell reselection to a new cell.
   * And if OTD is not applied, the initial TimeAdv is reset to 48chips;
   * If OTD is available, fill in correct value based on DL observed time difference.
   */
  boolean                           ingore_initial_time_adv;
  
  /*! \brief Initial timing advance for UpPCH in Cx8. Default 48*8 Cx8.*/
  int16                             time_adv;

  /* How many probes whose config have been sent to FW, regardless of the reception
   * of FPACH ACK/NoACK responses */
  uint8                             probe_cnt;

  /* How many FPACH responses UL are waiting for */
  uint8                             pending_rsp_cnt;

  /* How many sub-frames that are passed during FPACH responses waiting */
  uint8                             waiting_subfrm_cnt;

  /* Record the sub-frame number of latest GET_DATA event, used to determine the 
   * FPACH_DEC_DONE ISR is before or after this GET_DATA event
   */
  uint16                            get_data_event_occur_subfrm;

  /* UpPCH tx power for the upcoming probe, in 1/16 dBm */
  int16                             uppch_tx_pwr;

  /* UpPCH tx subframe (OTA) for the upcoming probe */
  uint16                            uppch_tx_subfrm;

  /* The base of SYNC-UL ID of given code group, determined by scrambling code ID
   * or SYNC-DL ID.
   * See 25.223/8.3 for the relationship between the SYNC-DL and SYNC-UL sequences,
   * the scrambling codes and the midamble codes
   */
  uint8                             sync_code_base;

  /* randomly selected SYNC_UL code for the upcoming probe */
  uint8                             sync_code_id;

  /* fpach info that are used for the upcoming probe */
  tdsl1_ul_fpach_info_struct_type  *fpach_info_ptr;

  /*----------------------------------------------------------
   * dynamic parameters used in TX state
   *----------------------------------------------------------*/

  /* The action to perform in TX state */
  tdsulra_data_tx_action_enum_type  data_tx_action;

  /* The subframe at which data_tx_action would be performed */
  uint16                            data_tx_action_trigger_time;

  /* PRACH to be used for RACH TX, determined by SYNC_UL code responsed */
  tdsl1_ul_prach_fpach_struct_type *prach_info_ptr;

  /* Channelisation code of PRACH to be used, determined by Subframe 
   * that FPACH received 
   */
  uint8                             prach_chan_code_id;

  /* Transmit power level for RACH message, get from FPACH */
  int16                             prx_prach_des;

  /* PRACH tx subframe (OTA) */
  uint16                            prach_tx_subfrm;
  
  /* Received starting position of the UpPCH from FPACH in 1/8 chip */
  uint16                            uppch_pos;

  /* UPPCH pwr accumulation from last pwr rampping in 1/16 dB*/
  int16                             uppch_tx_pwr_accumulation;
} tdsulra_ctrl_struct_type;

#ifdef FEATURE_QSH_EVENT_METRIC
/* This structure contains the uplink counters to support FR3833.*/

typedef struct
{
  /* Counter for FPACH success on the first UPPCH attempt */
  uint32 fpach_success_on_1st_uppch_cnt; 

  /* Counter for FPACH success on maximum UPPCH attemp */
  uint32 fpach_success_on_max_uppch_cnt;

  /* Counter for total FPACH failure */
  uint32 fpach_failure_cnt;

  /* Counter for total UPPCH attempt */
  uint32 uppch_attemp_cnt;

  /* Counter for RACH attempt on maximum UPPCH power */
  uint32 rach_on_max_uppch_pwr_cnt;

  /* average TX power on RACH */
  int32  avg_prach_tx_pwr_dbm_q4;

  /* Counter for FPACH ACK */
  uint32 fpach_ack_cnt;

  /* Counter for FPACH NO ACK */
  uint32 fpach_no_ack_cnt;

  /* the accumulated prach power in the history */
  int32  prach_pwr_hist_q4;
  
}tdsulra_statistic_logging_struct_type;
#endif /*FEATURE_QSH_EVENT_METRIC*/

/*dynamic memory allocation struct*/
typedef struct
{
  /* Control center of random access procedure */
  tdsulra_ctrl_struct_type tdsulra_ctrl_base;

  tfw_uppch_tx_pwr_timing_config_cmd_t tdsulra_uppch_tx_pwr_timing_config;

  /* Record two latest UPPCH_FPACH_CONFIG messages which were sent
   * to FW. It's available in UL SYNC procedure. 
   */
  tfw_fpach_config_cmd_t tdsulra_uppch_fpach_config[2];

  /* Record latest PRACH_CONFIG message which was sent to FW in
   * PRACH TX procedure.
   */
  tfw_prach_config_cmd_t tdsulra_prach_config;

  uint8 tdsulra_success_rate_sorted_index[TDSULRA_MAX_TA_OPT_TABLE_SIZE];
  uint8 tdsulra_success_rate_table_size;

  boolean tdsulra_is_init_rex_crit_sect;

  #ifdef FEATURE_QSH_EVENT_METRIC
  /* uplink RA statistics structure */
  tdsulra_statistic_logging_struct_type tdsulra_stats_logging;
  #endif
  
  #ifdef FEATURE_TDSCDMA_PLT
  /* Add flag to hack FPACH ISR or not */
  boolean tdsulra_hack_fpach_isr;
  #endif

  /*set this flag to true when disable RF in drop all,will not set it in suspend_rach*/
  boolean       tdsulra_disable_rf_in_drop_all;
  
}tdsulra_global_struct_type;

extern tdsulra_global_struct_type *tdsulra_global;

/* ----------------------------------------------------------------------- */
/*                          Variable Statement                             */
/* ----------------------------------------------------------------------- */


/* ----------------------------------------------------------------------- */
/*                          Function Statement                             */
/* ----------------------------------------------------------------------- */

/*=========================================================================
FUNCTION        tdsulra_init

DESCRIPTION     This function initialize the variables defined in this
                module.

DEPENDENCIES    

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsulra_init(void);

/*===========================================================================
FUNCTION        tdsulra_set_active_mode

DESCRIPTION     This function set the active mode of random access procedure.
                The active mode is set whenever PRACH initial setup, or 
                HHO-resync or E-RUCCH procedure is triggered.

DEPENDENCIES    N/A

RETURN VALUE    void

SIDE EFFECTS    None
===========================================================================*/
void tdsulra_set_active_mode (tdsulra_active_mode_enum_type active_mode);

/*===========================================================================
FUNCTION        tdsulra_get_active_mode

DESCRIPTION     This function get current active mode.

DEPENDENCIES    N/A

RETURN VALUE    The active mode of random access procedure

SIDE EFFECTS    None
===========================================================================*/
tdsulra_active_mode_enum_type tdsulra_get_active_mode (void);

/*===========================================================================
FUNCTION        tdsulra_set_state

DESCRIPTION     This function set the state of random access procedure.
                Whenever state machine transits, this function is invoked to
                update current state.

DEPENDENCIES    N/A

RETURN VALUE    void

SIDE EFFECTS    None
===========================================================================*/
void tdsulra_set_curr_state (tdsulra_state_enum_type state);

/*===========================================================================
FUNCTION        tdsulra_get_curr_state

DESCRIPTION     This function get current state.

DEPENDENCIES    N/A

RETURN VALUE    The active mode of random access procedure

SIDE EFFECTS    None
===========================================================================*/
tdsulra_state_enum_type tdsulra_get_curr_state (void);

/*===========================================================================
FUNCTION        tdsulra_set_ingore_initial_time_adv_flg

DESCRIPTION     This function is used to set tdsulra_global->tdsulra_ctrl_base.ingore_initial_time_adv

DEPENDENCIES    N/A

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsulra_set_ingore_initial_time_adv_flg (boolean ingore_initial_time_adv);


/*===========================================================================
FUNCTION        tdsulra_get_sync_code

DESCRIPTION     This function returns the actual available SYNC_UL codes index
                for specified ASC.
                For example, suppose SYNC_UL codes 0,5,6,7 are given in PRACH info
                (i.e. code_bitmap=0b11100001), and the SYNC_UL  codes indices
                (code_mask) is 0b00001100, then it means SYNC_UL codes 0 and 5
                are not available, while SYNC_UL codes 6 and 7 are available
                for this ASC.
                See 3GPP/25.331 8.6.6.29 for details.

--------------------------------------------------------
Example:
Available SYNC_UL codes:  1110 0001 (i.e. 0,5,6,7)
codes indices in ASC:     0000 1100
                               ||||
                               |||code0(unavailable)
                               ||code5(unavailable)
                               |code6(available)
                               code7(available)
--------------------------------------------------------

DEPENDENCIES    

RETURN VALUE    The available SYNC_UL codes for this ASC.

SIDE EFFECTS    None
===========================================================================*/
uint8 tdsulra_get_sync_code
(
  /* bit map of SYNC_UL codes */
  uint8 code_bitmap,
  /* available index in ASC */
  uint8 code_mask
);

/*===========================================================================
FUNCTION        tdsulra_process_prach_data

DESCRIPTION     This is the main handler of PRACH processing. It checks 
                whether PRACH transmission is ongoing, handles PRACH state 
                machine, and gets data from MAC.
  
DEPENDENCIES    It's invoked in GET_DATA event.

RETURN VALUE    None.

SIDE EFFECTS    None.
===========================================================================*/
void tdsulra_process_prach_data(void);

/*===========================================================================
FUNCTION        tdsulra_process_hho_resync

DESCRIPTION     This is the main handler of HHO-resync processing. It checks
                whether HHO re-sync is ongoing, handle state machine transition, 
                and config FW/HW.
  
DEPENDENCIES    It's invoked in GET_DATA event.

RETURN VALUE    None.

SIDE EFFECTS    None.
===========================================================================*/
void tdsulra_process_hho_resync(void);

#ifdef FEATURE_TDSCDMA_HSUPA
/*===========================================================================
FUNCTION        tdsulra_process_erucch_data

DESCRIPTION     This is the main handler of ERUCCH processing. It checks 
                whether ERUCCH transmission is ongoing, handle state machine
                transition, and config FW/HW.
  
DEPENDENCIES    It's invoked in GET_DATA event.

RETURN VALUE    None.

SIDE EFFECTS    None.
===========================================================================*/
void tdsulra_process_erucch_data(void);

/*=========================================================================
FUNCTION        tdsulra_fpach_dec_done_isr_for_erucch

DESCRIPTION     This ISR is registered to FW when E-RUCCH Tx is initiated and 
                triggered by FW after FPACH is decoded or not received within 
                WT. It is also used by FW to response UPPCH/FPACH abort command.

DEPENDENCIES    N/A

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsulra_fpach_dec_done_isr_for_erucch(void);

/*===========================================================================
FUNCTION        tdsulra_is_erucch_in_progress

DESCRIPTION     This function checks whether E-RUCCH procedure is in progress.

DEPENDENCIES    None

RETURN VALUE    boolean

SIDE EFFECTS    None
==========================================================================*/
boolean tdsulra_is_erucch_in_progress(void);


/*=========================================================================
FUNCTION        tdsulra_get_erucch_config

DESCRIPTION     This function is invoked by HSUPA to get the ERUCCH_CONFIG 
                for calculation of power headroom for EPUCH transmission.

DEPENDENCIES    the ERUCCH_CONFIG is valid only when ERUCCH state is 
                ERUCCH_TX or CNT_FOR_DONE.

RETURN VALUE    The pointer to the ERUCCH_CONFIG.

SIDE EFFECTS    None.
=========================================================================*/
tfw_erucch_config_cmd_t * tdsulra_get_erucch_config(void);

/*=========================================================================
FUNCTION        tdsulra_get_uppch_fpach_config

DESCRIPTION     This function is invoked by HSUPA to get the UPPCH_CONFIG 
                for channel conflict with EUPCH determination.

DEPENDENCIES    Only available when in SYNC state in E-RUCCH active mode.

RETURN VALUE    The pointer to the UPPCH_CONFIG.

SIDE EFFECTS    None.
=========================================================================*/
tfw_fpach_config_cmd_t * tdsulra_get_uppch_fpach_config(void);

/*=========================================================================
FUNCTION        tdsulra_get_prach_config

DESCRIPTION     This function is to get the prach config info 

DEPENDENCIES    

RETURN VALUE    The pointer to the PRACH_CONFIG.

SIDE EFFECTS    None.
=========================================================================*/
tfw_prach_config_cmd_t * tdsulra_get_prach_config(void);

#endif

/*=========================================================================
FUNCTION        tdsulra_fpach_dec_done_isr_for_prach

DESCRIPTION     This ISR is registered to and triggered by FW after FPACH 
                is decoded or not received within WT. It is also used by FW
                to response UPPCH/FPACH abort command.

DEPENDENCIES    N/A

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsulra_fpach_dec_done_isr_for_prach(void);

/*=========================================================================
FUNCTION        tdsulra_fpach_dec_done_isr_for_hho_resync

DESCRIPTION     This ISR is registered to FW when HHO-resync proc is started
                and triggered by FW after FPACH is decoded or not received 
                within WT. It is also used by FW to response UPPCH/FPACH 
                abort command.

DEPENDENCIES    N/A

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsulra_fpach_dec_done_isr_for_hho_resync(void);

/*===========================================================================
FUNCTION        tdsulra_is_rach_in_progress

DESCRIPTION     This function tells L1M if RACH procedure is in progress.

DEPENDENCIES    This function is called by L1M before proceeding PhyChan Drop 
                for FACH to DCH transition.

RETURN VALUE    boolean

SIDE EFFECTS    None
==========================================================================*/
boolean tdsulra_is_rach_in_progress(void);

/*===========================================================================
FUNCTION        tdsulra_prach_drop_preprocessing

DESCRIPTION     This function checks whether PRACH is in progress. 
                PRACH is in progress if the state is SYNC or TX. 
                In this case, the function will
                  1) notify FW to abort it if in SYNC state;
                  2) wait until RACH transmission done if in TX state;
                and then return TRUE.
                Otherwise, it returns FALSE.

DEPENDENCIES    This function is called by L1M before proceeding PRACH drop.
                If it returns TRUE, L1M shall wait for the RACH_DONE from
                UL; otherwise it can continue the handling of PRACH drop.

RETURN VALUE    boolean

SIDE EFFECTS    None
==========================================================================*/
boolean tdsulra_prach_drop_preprocessing(void);

/*===========================================================================
FUNCTION        tdsulra_resume_rach

DESCRIPTION     This function resumes RACH procedure

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
==========================================================================*/
void tdsulra_resume_rach(void);

/*===========================================================================
FUNCTION        tdsulra_suspend_rach

DESCRIPTION     This function suspends RACH procedure without dropping

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
==========================================================================*/
void tdsulra_suspend_rach(void);

/*===========================================================================
FUNCTION        tdsulra_abort_random_access_proc

DESCRIPTION     This function aborts RACH procedure or E-RUCCH procedure. 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
==========================================================================*/
void tdsulra_abort_random_access_proc(void);

/*=========================================================================
FUNCTION        tdsulra_cleanup

DESCRIPTION     Disable the interrupt and event that are set for random
                access procedure, and cleanup variables as well as send
                notification to other modules.

DEPENDENCIES    

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsulra_cleanup(void);

/*===========================================================================
FUNCTION    
  tdsl1_alloc_dynamic_mem
DESCRIPTION
  when receive TDSL1_CPHY_START_TDSCDMA_MODE_REQ, L1M start to malloc memory for L1 modules, and initialize them.
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsulra_alloc_dynamic_mem(void);

/*===========================================================================
FUNCTION    
  tdsl1dec_free_dynamic_mem
DESCRIPTION
  when receive TDSL1_CPHY_STOP_TDSCDMA_MODE_REQ, L1M start to free memory for L1 modules.
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/
void tdsulra_free_dynamic_mem(void);

#ifdef FEATURE_TDSCDMA_DSDA
/*=========================================================================
FUNCTION tdsulra_get_hho_resync_fpach_timeslot_bitmask

DESCRIPTION
  L1M/DSDA use this interface to query hho resync fpach timeslot configuration

DEPENDENCIES
  None.

RETURN VALUE
  HHO resync FPACH slot bitmap.

SIDE EFFECTS
  None.
=========================================================================*/
uint16 tdsulra_get_hho_resync_fpach_timeslot_bitmask(void);

/*=========================================================================
FUNCTION tdsulra_get_prach_timeslot_bitmask

DESCRIPTION
  L1M/DSDA use this interface to query prach timeslot configuration

DEPENDENCIES
  None.

RETURN VALUE
  PRACH slot bitmap.

SIDE EFFECTS
  None.
=========================================================================*/
uint16 tdsulra_get_prach_timeslot_bitmask(void);

/*=========================================================================
FUNCTION tdsulra_get_prach_fpach_timeslot_bitmask

DESCRIPTION
  L1M/DSDA use this interface to query fpach timeslot configuration

DEPENDENCIES
  None.

RETURN VALUE
  FPACH slot bitmap.

SIDE EFFECTS
  None.
=========================================================================*/
uint16 tdsulra_get_fpach_timeslot_bitmask(void);
#endif

/*=========================================================================
FUNCTION tdsulra_is_prach_erucch_tranmission

DESCRIPTION
  Check if the subframe is PRACH or ERUCCH OTA subframe

DEPENDENCIES
  have to call after FPACH ACK received.

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None.
=========================================================================*/
boolean tdsulra_is_prach_erucch_in_tranmission(uint16 sub_sfn);

#ifdef FEATURE_TDSCDMA_FATAL_RECOVERY
/*=========================================================================
FUNCTION tdsulra_fw_error_detected

DESCRIPTION
  TDSL1M will notify UL that FW error detected

DEPENDENCIES
  Should only happen in DCH state

RETURN VALUE
  None

SIDE EFFECTS
  None.
=========================================================================*/
void tdsulra_fw_error_detected(void);
#endif /*FEATURE_TDSCDMA_FATAL_RECOVERY*/

/*=========================================================================
FUNCTION        tdsulra_send_hho_resync_done_cmd

DESCRIPTION     This function sends command to L1M to indicate whether 
                HHO resync success or not.

DEPENDENCIES    None.

RETURN VALUE    None.

SIDE EFFECTS    None.
=========================================================================*/
void tdsulra_send_hho_resync_done_cmd(boolean ack_is_received);

#ifdef FEATURE_QSH_EVENT_METRIC

/*===========================================================================

FUNCTION    tdsulra_get_fpach_success_on_1st_uppch_cnt

DESCRIPTION
          Getter for fpach success count on 1st uppch attempt.

DEPENDENCIES
  None

RETURN VALUE
  UINT32  counts of fpach success on 1st uppch attempt.

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsulra_stats_get_fpach_success_on_1st_uppch_cnt(void);
/*===========================================================================

FUNCTION    tdsulra_get_fpach_success_on_max_uppch_cnt

DESCRIPTION
          Getter for fpach success count on max uppch attempt.

DEPENDENCIES
  None

RETURN VALUE
  UINT32  counts of fpach success on max uppch attempt.

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsulra_stats_get_fpach_success_on_max_uppch_cnt(void);
/*===========================================================================

FUNCTION    tdsulra_get_fpach_failure_cnt

DESCRIPTION
          Getter for fpach failure.

DEPENDENCIES
  None

RETURN VALUE
  UINT32  counts of fpach faliures.

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsulra_stats_get_fpach_failure_cnt(void);
/*===========================================================================

FUNCTION    tdsulra_get_uppch_attemp_cnt

DESCRIPTION
          Getter for uppch attempts.

DEPENDENCIES
  None

RETURN VALUE
  UINT32  counts of uppch attempts.

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsulra_stats_get_uppch_attemp_cnt(void);
/*===========================================================================

FUNCTION    tdsulra_get_rach_on_max_pwr_cnt

DESCRIPTION
          Getter for rach on max power.

DEPENDENCIES
  None

RETURN VALUE
  UINT32  counts of rach attempts on max TX power.

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsulra_stats_get_rach_on_max_pwr_cnt(void);
/*===========================================================================

FUNCTION    tdsulra_get_avg_tx_pwr_on_rach_success

DESCRIPTION
          Getter for average tx power on RACH(UPPCH) success.

DEPENDENCIES
  None

RETURN VALUE
  INT32  The average tx power upon rach(uppch) success.

SIDE EFFECTS
  None
===========================================================================*/
int32 tdsulra_stats_get_avg_prach_tx_pwr(void);

/*=========================================================================
FUNCTION        tdsulra_stats_clear

DESCRIPTION   This function clear all uplink RACH subsystem call stats related 
              fields and variables.
                      

DEPENDENCIES    N/A

RETURN VALUE    void.

SIDE EFFECTS    None.
=========================================================================*/
void tdsulra_stats_clear(void);

/*=========================================================================
FUNCTION      tdsulra_get_qsh_metric

DESCRIPTION   This function updates the rach qsh counters.
                      

DEPENDENCIES    N/A

RETURN VALUE    void.

SIDE EFFECTS    None.
=========================================================================*/
boolean tdsulra_get_qsh_metric(tdsl1_qsh_metric_ul_rach_s* qsh_metric_ul_rach_ptr);

/*=========================================================================
FUNCTION      tdsulra_get_qsh_prach_pwr

DESCRIPTION   This function updates the rach tx power.
                      

DEPENDENCIES    N/A

RETURN VALUE    void.

SIDE EFFECTS    None.
=========================================================================*/
boolean tdsulra_get_qsh_prach_pwr (tdsl1_qsh_metric_ul_prach_pwr_s* qsh_metric_ul_prach_pwr_ptr);

/*===========================================================================
FUNCTION        tdsulra_init_prach_stats

DESCRIPTION     This function initialize the uplink random access stats.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
===========================================================================*/
void tdsulra_init_prach_stats(void);

/*===================================================================

FUNCTION    tdsl1ulra_init_qsh_prach_power_reporting

DESCRIPTION
           Set initial PRACH Tx power to be an Invalid value before
           TD start.

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None
====================================================================*/
void tdsl1ulra_init_qsh_prach_power_reporting(void);

#endif /*FEATURE_QSH_EVENT_METRIC */
#endif
