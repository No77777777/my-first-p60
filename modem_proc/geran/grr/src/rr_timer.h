/**
  @file rr_timer.h

  @brief Provides timer IDs, durations and APIs to support their use within RR.
*/

/*============================================================================

  Copyright (c) 2010-2015 Qualcomm Technologies, Inc.

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

============================================================================*/

#ifndef __RR_TIMER_H__
#define __RR_TIMER_H__

/* $Header: //components/rel/geran.mpss/7.4.0/grr/src/rr_timer.h#4 $ */
/* $DateTime: 2020/07/14 23:56:54 $$Author: pwbldsvc $ */

/*----------------------------------------------------------------------------
 * Include Files
 *--------------------------------------------------------------------------*/

#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "sys_cnst.h"
#include "sys_stru.h"
#include "ms_timer.h"
#include "timers.h"
#include "msg.h"
#include "ms.h"
#include "gs.h"
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

#define RR_DEFAULT_TIMER_CONTEXT 0x00
#define RR_SINGLE_FRAME_TICK_DURATION (5)

/*----------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *--------------------------------------------------------------------------*/

/* Time conversions */

/* 1 decihour is equal to 6 minutes */
#define CONVERT_DECIHOURS_TO_SECONDS(x)      (x*6*60)
#define CONVERT_DECIHOURS_TO_MILLISECONDS(x) (x*6*60*1000)

#define CONVERT_HOURS_TO_SECONDS(x)          (x*60*60)
#define CONVERT_HOURS_TO_MILLISECONDS(x)     (x*60*60*1000)
#define CONVERT_MINUTES_TO_MILLISECONDS(x)   (x*60*1000)
#define CONVERT_SECONDS_TO_MILLISECONDS(x)   (x*1000)
#define CONVERT_MILLISECONDS_TO_SECONDS(x)   (x/1000)

/* Timer Durations. */

#define RR_INVALID_BAND_PREF_TIMEOUT             1000
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
#define RR_GSM_SYS_INFO_TIMEOUT                  8000  /* 8 seconds   */
#define RR_DUAL_SIM_BPLMN_LIST_TIMEOUT          60000  /* 60 seconds  */
#define RR_DUAL_SIM_MM_PS_ABORT_TIMEOUT           200  /* 200 ms      */
#define RR_DUAL_SIM_L1_PS_ABORT_TIMEOUT           200  /* 200 ms      */
#define RR_EPD_TIMER_DURATION            (235 * 8 * 5) /* five TC cycles = 9400ms */
#else
#define RR_GSM_SYS_INFO_TIMEOUT                  4000  /* 4 seconds   */
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
#define RR_GSM_RES_SI_REFRESH_TIMEOUT            5500  /* 5.5 seconds    */
#define RR_GPRS_SYS_INFO_TIMEOUT                 8000  /* 8 seconds  */
#define T3110_TIMEOUT                            5000  /* 5 seconds  */
#define RR_RESET_FORBIDDEN_CELL_TIMEOUT          5000  /* 5 seconds  */
#define RR_CELL_RESELECTION_ABORT_TIMEOUT       11000 /* 11 seconds */
#define RR_RECENT_CELL_RESELECTION_TIMEOUT      15000 /* 15 seconds */
#define SIGNAL_STRENGTH_TIMEOUT                  3000  /* 3 seconds  */
#define NO_CHANNELS_TIMEOUT                     30000 /* 30 seconds */
#define RR_FIVE_SEC_TIMEOUT                      5000  /* 5 seconds */
#define RR_CELL_SELECTION_ABORT_TIMEOUT         120000 /* 120 seconds */

/* 50ms, the timer is guarding the delay of system info from L2 */
#define RR_BCCH_DECODE_COMPLETE_TIMEOUT            50

#define RR_INVALIDATE_AVAILABLE_PLMN_DB_TIMEOUT 15000 /* 15 seconds */
#define RR_INVALIDATE_PSCAN_RESULTS_TIMEOUT     15000 /* 15 seconds */

/* The following are the timeout values equivalent to infinite in the case of
WCDMA to GSM Reselection Reject. */
#define RR_W2GR_GSM_CELL_BARRED_TIMEOUT           300 /* 5 Min ( Units are Secs ) */
#define RR_W2GR_GSM_CELL_LOW_C1_TIMEOUT         65535 /* Max Possible time */
#define RR_W2GR_GSM_CELL_PLMN_MISMATCH_TIMEOUT  65535 /* Max Possible time */
#define RR_W2GR_GSM_CELL_FORBIDDEN_TIMEOUT      65535 /* Max Possible time */
#define RR_W2GR_GSM_BSIC_NOTFOUND_TIMEOUT       65535 /* Max Possible time */
#define RR_W2GR_BCCH_NOTFOUND_TIMEOUT           65535 /* Max Possible time */
#define RR_W2GR_BCCH_DECODE_FAIL_TIMEOUT        65535 /* Max Possible time */

#ifdef FEATURE_GSM_TDS
/* The following are the timeout values equivalent to infinite in the case of
TDS to GSM Reselection Reject. */
#define RR_T2GR_GSM_CELL_BARRED_TIMEOUT           300 /* 5 Min ( Units are Secs ) */
#define RR_T2GR_GSM_CELL_LOW_C1_TIMEOUT         65535 /* Max Possible time */
#define RR_T2GR_GSM_CELL_PLMN_MISMATCH_TIMEOUT  65535 /* Max Possible time */
#define RR_T2GR_GSM_CELL_FORBIDDEN_TIMEOUT      65535 /* Max Possible time */
#define RR_T2GR_GSM_BSIC_NOTFOUND_TIMEOUT       65535 /* Max Possible time */
#define RR_T2GR_BCCH_NOTFOUND_TIMEOUT           65535 /* Max Possible time */
#define RR_T2GR_BCCH_DECODE_FAIL_TIMEOUT        65535 /* Max Possible time */
#endif /*FEATURE_GSM_TDS*/

/* RR_GSM_SYS_INFO_TIMEOUT(2s) + RR_GPRS_SYS_INFO_TIMEOUT(8s)
 * + some guard time for SCH sync and so on
 */
#define RR_MIN_T309_LENGTH_FOR_WTOG_CCO         13000 /* ms */

/* Cell barring due to authentication failure is for T3212 minus 60 seconds
 * (gsm authentication failure) or T3302 minus 60 seconds (gprs authentication
 * and ciphering failure). 3GPP spec ref: 43.022 section 3.5.5.
 *
 * T3212 has a range of 1-255 decihours. 1 decihour is 6 minutes. So, the
 * minimum possible value for T3212 is 1 decihour which is 1*6*60*1000
 * milliseconds. i.e. 360000 mS. Deduct 60 seconds from this, it gives
 * 300000mS. So, min cell bar period for authentication failure is 300000mS.
 *
 * T3302 has a range of 2 seconds to 31 decihours which is 2000ms to
 * 11160000mS. If default value is taken into account, range is 2000mS to
 * 43200000mS. Min of range should be raised to 62000mS if cell bar period
 * equation T3302 - 60 seconds is to be honoured. So, min cell bar period for
 * authentication and ciphering failure is 2000mS (62000 - 60000mS).
 *
 * A single timer is used for tracking max of 6 barred cells which may be barred
 * due to either reason. Therefore, minimum of the two , i.e. 2000mS is selected
 * as timeout value.
 */
#define RR_CELL_BARRED_TIMEOUT                   2000

#define GRR_TIMEOUT_100_MILLISECONDS              100
#define GRR_TIMEOUT_500_MILLISECONDS              500
#define GRR_TIMEOUT_1_SECOND                     1000
#define GRR_TIMEOUT_2_SECONDS                    2000
#define GRR_TIMEOUT_5_SECONDS                    5000
#define GRR_TIMEOUT_10_SECONDS                  10000
#define GRR_TIMEOUT_30_SECONDS                  30000
#define GRR_TIMEOUT_60_SECONDS                  60000
#define GRR_TIMEOUT_120_SECONDS                120000

#define GRR_FIRST_SYS_INFO_SUSPEND_TIMEOUT        100
#define T3174_TIMEOUT                           15000
#define T3176_TIMEOUT                           15000
#define CELL_RESELECTION_TIMEOUT                12000
#define RESELECTION_HYSTERESIS_TIMEOUT          15000
#define PATH_LOSS_TIMEOUT                        5000
#define GPRS_PATH_LOSS_TIMEOUT                   1000

 /* A single 2s timer is use to track the timeout of all de-prioritized cells */
#define RR_CELL_DEPRIORITIZED_TIMEOUT            2000

/* GRR_SURROUND_MEAS_TIMEOUT set to 5200ms because the max RACH timeout is 5000ms */
#define GRR_SURROUND_MEAS_TIMEOUT 5200
#if defined(FEATURE_GSM_DTM) && defined(FEATURE_GSM_EDTM)
#define EDTM_STATUS_TIMEOUT      GRR_TIMEOUT_1_SECOND
/* T3196 is run in GMAC for 1.5 seconds, so use a guard period of twice that amount */
#define T3196_GUARD_TIMEOUT             (GRR_TIMEOUT_1_SECOND + GRR_TIMEOUT_2_SECONDS)
#endif /* FEATURE_GSM_DTM && FEATURE_GSM_EDTM */

#define SYSTEM_INFO_CLOCK_TIMEOUT 10000 /* 10 secs */
#define SYSTEM_INFO_TIMEOUT 300000 /* 5 min timeout */

#define T3206_TIMEOUT 400
#define T3208_TIMEOUT 960
#define T3210_TIMEOUT 300

#define RR_PLMN_LIST_SEARCH_GUARD_TIMER_DEFAULT_DURATION 150000 /* 150 seconds */
/* FR 42544: Support for frequency/channel level MPLMN scanning */
#define RR_PLMN_LIST_SEARCH_GUARD_TIMER_DURATION 20000 /* 20 seconds */
/* Timer used to ensure L1 responds to start / stop requests in a timely manner. */
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
/* When multi-SIM feaure is enabled this timer is doubled.  Note that it MUST
exceed the 5 seconds that the RR_DUAL_SIM_INACTIVATE_TIMER is assigned. */
#define RR_L1_GUARD_TIMEOUT 6000 /* 6 seconds */
#else
#define RR_L1_GUARD_TIMEOUT 3000 /* 3 seconds */
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

#define RR_ACQ_NOMINAL_TIMEOUT_PSCAN                   900  /* 900ms */
#define RR_ACQ_INVERSE_TIMEOUT_PSCAN                   100  /* 100ms */
#define RR_ACQ_NOMINAL_TIMEOUT_OTHER_DR                705  /* 705ms */
#define RR_ACQ_INVERSE_TIMEOUT_OTHER_DR                235  /* 235ms */
#define RR_ACQ_NOMINAL_TIMEOUT_OTHER                  3000  /*  3 seconds */
#define RR_ACQ_INVERSE_TIMEOUT_OTHER                  1000  /*  1 second  */
#define RR_ACQ_DELTA_TIMEOUT_OTHER                     200  /* 200 milliseconds */ 
#define RR_PROC_LOCK_TIMEOUT                         10000  /* 10 seconds */

/* Timeout used to detect when grr_control() state machine gets stuck in
transitory states such as STARTING_IDLE or {ENABLING|DISABLING}_PS_ACCESS. */
#define RR_GRR_CONTROL_STATE_TRANSITION_TIMEOUT       5000U
#define RR_RESEL_CONTROL_STATE_TRANSITION_TIMEOUT     5000U
#define RR_CELL_ACQ_CONTROL_STATE_TRANSITION_TIMEOUT  5000U
#define RR_MM_ACTIVATION_RSP_TIMEOUT                  3000U /*3 second*/
#define RR_CONN_REL_CONTROL_WAIT_FOR_REL_CNF_TIMEOUT  3000U /*3 second*/
#define RR_CONNECTION_RELEASE_STATE_TRANSITION_TIMEOUT  5000U

#ifdef FEATURE_LOCK_ARFCN
#define RR_WAIT_FOR_ARFCN_TO_LOCK_TIME_OUT            10000  /* 10.0 seconds */
#endif /*FEATURE_LOCK_ARFCN*/

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif //FEATURE_QSH_EVENT_NOTIFY_HANDLER

/*----------------------------------------------------------------------------
 * Type Declarations
 *--------------------------------------------------------------------------*/

/* Timer IDs. */
typedef enum
{
  RR_TIMER_UNDEFINED,

  RR_GSM_SYS_INFO_TIMER,
  T3110,
  T3126,
  T3122,
  T3124,
  SIGNAL_STRENGTH_TIMER,

  /*! Timer while a cell selection operation is in progress. */
  RR_CELL_SELECTION_ABORT_TIMER,

  /*! Timeout value for waiting for GPRS system information. */
  RR_GPRS_SYS_INFO_TIMER,

  /*! Timer for completing a BCCH decode operation after the
  MPH_DECODE_BCCH_LIST_CNF. */
  RR_BCCH_DECODE_COMPLETE_TIMER,

  /*! Timer for invalidating the available PLMN information when it gets too
  old. */
  RR_INVALIDATE_AVAILABLE_PLMN_DB_TIMER,

  /*! Timer for invalidating the power scan results when they get too old. */
  RR_INVALIDATE_PSCAN_RESULTS_TIMER,

  /*! Timers used to check C1/C2 criteria is valid for at least 5 seconds. */
  RR_FIVE_SEC_C2_TIMER_1,
  RR_FIVE_SEC_C2_TIMER_2,
  RR_FIVE_SEC_C2_TIMER_3,
  RR_FIVE_SEC_C2_TIMER_4,
  RR_FIVE_SEC_C2_TIMER_5,
  RR_FIVE_SEC_C2_TIMER_6,

  RR_SERV_FIVE_SEC_C1_TIMER,

  /*! RR W2G Service Redirection Wait Timer. */
  RR_W2G_SERVICE_REDIRECTION_WAIT_TIMER,

  /*! Timer used for barring cells due to authentication or authentication and
  ciphering failure. */
  RR_CELL_BARRED_TIMER,

  RR_RESET_FORBIDDEN_CELL_TIMER_1,
  RR_RESET_FORBIDDEN_CELL_TIMER_2,
  RR_RESET_FORBIDDEN_CELL_TIMER_3,
  RR_RESET_FORBIDDEN_CELL_TIMER_4,
  RR_RESET_FORBIDDEN_CELL_TIMER_5,
  RR_RESET_FORBIDDEN_CELL_TIMER_6,
  RR_RESET_FORBIDDEN_CELL_TIMER_7,

  /*! Timer used to periodically print out some F3 indicating that RGSM bit has
  been set in RR_PLMN_SELECT_REQ.band_pref. */
  RR_INVALID_BAND_PREF_TIMER,

  T3178,
  T3158,
  T3174,
  T3176,
  CELL_RESELECTION_TIMER,
  RESELECTION_HYSTERESIS,
  PATH_LOSS_TIMER,
  T3142,
  T3146,
  TMULTIFRM,
  GRR_GUARD_TIMER,
  GRR_SYSTEM_INFO_TIMER,
  GRR_SURROUND_MEAS_TIMER,
  GRR_PENALTY_TIMER_1,
  GRR_PENALTY_TIMER_2,
  GRR_PENALTY_TIMER_3,
  GRR_PENALTY_TIMER_4,
  GRR_PENALTY_TIMER_5,
  GRR_PENALTY_TIMER_6,
  GRR_SYS_INFO_REFRESH_TIMER,
  GRR_SYS_INFO_ACQUISITION_TIMER,
  GRR_SYS_INFO_RETRY_TIMER,
  GPRS_RECENT_ACTIVITY_TIMER,

  /* NACC related timers. */
  T3208,
  T3210,
  GRR_NACC_CONTAINER_TIMER_0,
  GRR_NACC_CONTAINER_TIMER_1,
  GRR_NACC_CONTAINER_TIMER_2,
  GRR_NACC_CONTAINER_TIMER_3,

  /* SI Status related timers. */
  GRR_SYS_INFO_STATUS_TIMER_1,
  GRR_SYS_INFO_STATUS_TIMER_2,

  /* DTM / ETM timers. */
  T3148,
  T3196,
  RR_GRR_CHN_ASSIGN_REQ_GUARD_TIMER,

  /* IRAT timers. */

  RR_G2X_BARRED_FREQUENCY_TIMER_1,
  RR_G2X_BARRED_FREQUENCY_TIMER_2,
  RR_G2X_BARRED_FREQUENCY_TIMER_3,
  RR_G2X_BARRED_FREQUENCY_TIMER_4,
  RR_G2X_BARRED_FREQUENCY_TIMER_5,
  RR_G2X_BARRED_FREQUENCY_TIMER_6,

  RR_G2X_BARRED_CELL_TIMER_1,
  RR_G2X_BARRED_CELL_TIMER_2,
  RR_G2X_BARRED_CELL_TIMER_3,
  RR_G2X_BARRED_CELL_TIMER_4,
  RR_G2X_BARRED_CELL_TIMER_5,
  RR_G2X_BARRED_CELL_TIMER_6,

  /*! Timers used to check if WCDMA Cell is strong for at least 5 seconds. */
  RR_G2WR_FIVE_SEC_TIMER_1,
  RR_G2WR_FIVE_SEC_TIMER_2,
  RR_G2WR_FIVE_SEC_TIMER_3,
  RR_G2WR_FIVE_SEC_TIMER_4,
  RR_G2WR_FIVE_SEC_TIMER_5,
  RR_G2WR_FIVE_SEC_TIMER_6,

  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_1,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_2,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_3,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_4,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_5,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_6,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_7,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_8,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_9,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_10,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_11,
  RR_IRAT_PRIORITY_RESEL_THRESH_HIGH_TIMER_12,

  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_1,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_2,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_3,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_4,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_5,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_6,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_7,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_8,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_9,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_10,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_11,
  RR_IRAT_PRIORITY_RESEL_THRESH_LOW_TIMER_12,

  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_1,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_2,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_3,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_4,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_5,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_6,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_7,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_8,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_9,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_10,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_11,
  RR_IRAT_PRIORITY_RESEL_H_PRIO_TIMER_12,

  RR_EOOS_SEARCH_TIMER,
  RR_EOOS_FULL_PSCAN_TIMER_1,
  RR_EOOS_FULL_PSCAN_TIMER_2,

  T3230,

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  RR_DUAL_SIM_INTERFACE_TIMER,
  RR_DUAL_SIM_HEARTBEAT_TIMER,
  RR_DUAL_SIM_TERMINATE_TIMER,
  RR_DUAL_SIM_BPLMN_LIST_TIMER,
  RR_DUAL_SIM_INACTIVATE_TIMER,
  RR_DUAL_SIM_PS_ABORT_TIMER_1,
  RR_DUAL_SIM_PS_ABORT_TIMER_2,
  RR_EPD_TIMER,
  RR_TECH_RECENT_ACTIVITY_TIMER,
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  /* timers used by multi-SIM priority inversion */
  RR_ACQ_DUTY_CYCLE_TIMER,
  RR_SYS_INFO_GUARD_TIMER,
 /* timer used for Chain Lock Procedure */
  RR_PROC_LOCK_GUARD_TIMER,
  
  /**
    @brief Timer to guard the PLMN list search procedure.
  */
#ifdef FEATURE_MPLMN_PARTIAL_UPDATING
  RR_PLMN_LIST_SEARCH_GUARD_TIMER,            // acq_db_tmri_val / MaxManualPLMNTimer
  RR_PLMN_LIST_SEARCH_PARTIAL_RESULTS_TIMER,  // FirstPLMNTimer / MorePLMNTimer
#else
#ifdef FEATURE_LTE
  RR_PLMN_LIST_SEARCH_GUARD_TIMER,            // acq_db_tmri_val
#endif // FEATURE_LTE
#endif // FEATURE_MPLMN_PARTIAL_UPDATING

#ifdef FEATURE_LTE_TO_GSM_CGI
  RR_X2G_CGI_TIMER,
#endif /* FEATURE_LTE_TO_GSM_CGI */

  RR_L1_GUARD_TIMER,

  RR_GRR_CONTROL_STATE_TRANSITION_GUARD_TIMER,
  RR_RESEL_CONTROL_STATE_TRANSITION_GUARD_TIMER,
  RR_CELL_ACQ_CONTROL_STATE_TRANSITION_GUARD_TIMER,

  RR_SGLTE_BAND_COEX_DO_NOT_DISABLE_B9_TIMER,
  RR_MM_ACTIVATION_RSP_TIMER,  
  RR_CONN_REL_CONTROL_WAIT_FOR_REL_CNF_TIMER,
  RR_CONNECTION_RELEASE_STATE_TRANSITION_GUARD_TIMER,

#ifdef FEATURE_LOCK_ARFCN
 RR_WAIT_FOR_ARFCN_TO_LOCK_TIMER,
#endif /*FEATURE_LOCK_ARFCN*/
  RR_SERV_CELL_BAD_SNR_TIMER,

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif //FEATURE_QSH_EVENT_NOTIFY_HANDLER
#ifdef FEATURE_LTE
  RR_PRIORITIZE_LTE_ABORT_TIMER,
  RR_PRIORITIZE_LTE_DEFER_TIMER,
#endif /* FEATURE_LTE */

  /*! Timer used for deprioritizing */
  RR_CELL_DEPRIORITIZED_TIMER,
  
  RR_TIMER_MAX
} rr_timer_id_e;

/* masks used to manipulate 32-bit timer handles */
#define RR_TIMER_HANDLE_UID_BIT  (1UL << 16UL)
#define RR_TIMER_CONTEXT_BIT_SHIFT 20UL

typedef rr_timer_id_e rr_timer_id_T;    /* legacy timer ID type */

typedef enum
{
  RR_TIMER_STOPPED = 0x0,
  RR_TIMER_RUNNING = 0x1,
  RR_TIMER_EXPIRED = 0x2
} rr_timer_state_T;

typedef time_timetick_type rr_duration_t;

/*----------------------------------------------------------------------------
 * Function Declarations and Documentation
 *--------------------------------------------------------------------------*/

/*!
  @brief Initialises the RR timer module.

  @note This function should only be called at task initialisation.
*/
extern void rr_timer_init(void);

/*!
  @brief Returns a string containing the name of the specified timer ID.
*/
extern const char * rr_timer_name(rr_timer_id_e);

extern void rr_timer_expiry_callback(unsigned long);

extern boolean rr_timer_start(rr_timer_id_T, rr_duration_t, const gas_id_t);
extern boolean rr_timer_cancel(rr_timer_id_T, const gas_id_t);
extern boolean rr_timer_truncate(rr_timer_id_T, rr_duration_t, const gas_id_t);

extern boolean rr_timer_restart(rr_timer_id_T, rr_duration_t, const gas_id_t);
extern boolean rr_timer_extend(rr_timer_id_T, rr_duration_t, const gas_id_t);
extern boolean rr_timer_is_active(rr_timer_id_T, const gas_id_t);
extern boolean rr_timer_is_running(rr_timer_id_T, const gas_id_t);
extern boolean rr_timer_has_expired(rr_timer_id_T, const gas_id_t);
extern boolean rr_timer_create_expired_timer(rr_timer_id_T, const gas_id_t);
extern boolean rr_timer_validate_expiry_message(rr_timer_id_T, const gas_id_t);

extern rr_duration_t rr_timer_get_time_remaining(rr_timer_id_T, const gas_id_t);
extern rr_timer_state_T   rr_timer_get_state(rr_timer_id_T, const gas_id_t);

extern void rr_timer_reset_all(rr_timer_id_T *, int, const gas_id_t);

extern void rr_start_cell_barred_timer(const gas_id_t);
extern void rr_stop_barred_cell_timer(const gas_id_t);

#ifdef FEATURE_FAKE_BTS_CELL_DEPRIORITIZE
extern void rr_start_cell_deprioritized_timer(const gas_id_t gas_id);
#endif/*FEATURE_GCELL_DEPRIORITIZING*/

extern boolean rr_timer_force_timer_expiry(rr_timer_id_T, const gas_id_t);

#endif /* __RR_TIMER_H__ */
