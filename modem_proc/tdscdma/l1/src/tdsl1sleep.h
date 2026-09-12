#ifndef TDSL1SLEEP_H
#define TDSL1SLEEP_H
/*===========================================================================
                 TDSCDMA L1 SLEEP MODULE

GENERAL DESCRIPTION
  This file contains the code for controlling the operation of TDSCDMA L1 SLEEP

Copyright (c) 2009 by Qualcomm Technologies Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdsl1sleep.h#1 $
$DateTime: 2019/08/19 10:51:23 $
$Author: pwbldsvc $

when       who    what, where, why
--------   ---    --------------------------------------------------------- 
11/07/14   cliu   Wakeup GFW at TS0 instead of in OLS
06/21/12   weis   fix CR: 371283, extend rude wake-up timeline
15/04/11   weis   remove FEE; calculate sleep duration; update TD register
===========================================================================*/


/* ==========================================================================
**
** Header Includes
**
** ========================================================================*/

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"

#include "msm.h"
#include "slpc.h"
#include "tdsl1def.h"

#include "tdsl1lsmif.h"
#include "wmgr.h"

#include "tdsl1m_rcinit.h"

/* ==========================================================================
**
** Constants
**
** ========================================================================*/

/* Value to ARM the ST_EVENT state machine */
#define TDSSTMR_ARM_ST_EVENT_ENA   (0x1 << HWIO_SHFT(TDSSTMR_ST_EVENT_CMD_REG, ARM_ST_EVENT))

/* Send GO_TO_SLEEP signal when STMR_ST_EVENT occurs */
#define TDSSTMR_SLEEP_ARM_ENA      (0x1 << HWIO_SHFT(TDSSTMR_ST_EVENT_CMD_REG, SLEEP_ARM))

/*------------------------------------------------------------*/
/*  Defintions used by the sleep controller SW                */
/*------------------------------------------------------------*/
/*used by gl1_hw_sleep_ctl.c, once it's useless, remove it*/
#define TDSL1_SLEEP_CLK_NOMINAL_mHZ  (32765442)

/*for extended timeline or rude wakeup, mcpm warmup might take at most 25 ms
  so set min-sub-frames to 10 in the both cases*/
#define TDSL1_MIN_SUB_FRAMES_FOR_SLEEP  (tdsl1sleep_optimized_timeline ? 5:10)

#define TDSL1_MCPM_CONFIG_CX8             8000  /*0.8 ms */

#ifdef FEATURE_BOLT_MODEM  
#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_RXD  20000   /*2.0 ms */
#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_DIFF_FREQ 30000 /*3.0 ms*/
#else
#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_RXD  5000   /*0.5 ms */
#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_DIFF_FREQ 5000 /*0.5 ms*/
#endif

#if 0 // #ifdef FEATURE_SGLTE
#define TDSL1_LATE_OLS_TRIGGER_CX8       10000 /*1.0 ms*/
#define TDSL1_PLUS_FOR_SGLTE_CX8         5000 /* 0.5 ms*/
#endif

#define TDSL1_GSM_INIT_CX8               153600 /* 15 msec */

#ifdef FEATURE_TDSCDMA_VSTMR
#define TDSL1_SLEEP_GUARD_PERIOD         15000 /*1.5 ms, changed by Kefeng for Bolt bringup tempto 3.5 ms*/
#endif

#define TDSL1_RF_ENTER_TDS_MODE_CX8      25000 /*2.5 ms*/
#if defined (FEATURE_TRITON_MODEM) || defined (FEATURE_DIMEPM_MODEM)
#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_ENTER_MODE 18000  /*1.8 ms*/
#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_ENABLE_RX  12000  /*1.2 ms*/
#else
#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_ENTER_MODE 20000  /*2.0 ms*/
#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_ENABLE_RX  20000  /*2.0 ms*/
#endif

#define TDSL1_RF_WARMUP_CX8_PLUS_FOR_DR_DSDS      30000 /*2.0 ms extra for DR*/

#define TDSL1_WAKEUP_GUARD_PERIOD         18000  /*1.8 ms*/
#define TDSL1_MWU_TO_OLS_GUARD_PERIOD     18000  /*1.8 ms*/

/* in DSDA mode, there are 2 things may extend L1 wakeup timeline:
   1. G wake-up conflict with T wake-up and consequentially cause T wake-up delay.
      to avoid T wake-up late, we extend 1 msec for the delay.
      
   2. CxM registrition requires 2.5 msec ahead of TS0, and it's triggered by OLS (ReACQ) for now .
      we extend 1 msec for this requirement .
 
   in order NOT to extend the timeline, some optimizations were done for above 2 issues respectively.
   MCPM CR 581033 address 1st one, and L1 CR 618885 and FW CR 637191 are for the 2nd one .
   for now MCPM CR 581033 was checked in to DIME, but not in TRITON 3.0; and
   L1 CR 618885 and FW CR 637191 were checked in to TR.3.0, but not in DIME. */
#ifdef FEATURE_TDSCDMA_DSDA
#ifdef FEATURE_TRITON_MODEM
#define TDSL1_OLS_TO_TARGET_CX8_PLUS_FOR_DSDA   10000 /*1 ms*/
#else
#define TDSL1_OLS_TO_TARGET_CX8_PLUS_FOR_DSDA   10000 /*1 ms*/
#endif
#endif

#ifdef FEATURE_TDSCDMA_ADAPTIVE_WAKEUP_TIMELINE
#define TDSL1_MAX_OLS_TO_TARGET_CX8_ADP     102400 /*10 ms*/
#define TDSL1_MIN_OLS_TO_TARGET_CX8     10000 /*1.0 ms*/
// The time from TFW TS6 mid-amble point to sub-frame boundary
#define TDSL1_TFW_TS6_SP_TO_BOUNDARY_CX8                  4096  /*0.4 ms*/
// The threshold of the margin from wakeup done to TFW TS6 mid-amble point 
#define TDSL1_WAKEUP_DONE_TO_TFW_SP_MARGIN_THRESHOLD_CX8 102400  /* 10 ms */ 
#define TDSL1_WAKEUP_DONE_TO_TFW_SP_MARGIN_THRESHOLD_TICK 192000  /* 10 ms */
#define TDSL1_ADAPTIVE_OLS_TO_TARGET_GUARD_PERIOD        20000   /* 2 ms*/
#define TDSL1_MAX_USTMR_NUM                              0x8000000 /* 27 bits*/
/* The minimum adjust up size is 2500 chipx8 = 0.25 ms */
#define TDSL1_ADAPTIVE_WAKEUP_UP_SIZE_MIN       2500
#else
#define TDSL1_MIN_OLS_TO_TARGET_CX8     18000 /*1.8 ms*/
#endif /*FEATURE_TDSCDMA_ADAPTIVE_WAKEUP_TIMELINE*/

#define TDSL1_MAX_OLS_TO_TARGET_CX8     50000 /*5.0 ms*/

#define TDSL1_OLS_TO_TARGET_CX8_PLUS_FOR_RUDE_WAKEUP   50000

#ifdef FEATURE_TABASCO_MODEM
#define TDSL1_OLS_TO_TARGET_CX8_PLUS_FOR_PCH_BCH    35000 /*3.5 ms for TA temp, need to remove later*/
#else
#define TDSL1_OLS_TO_TARGET_CX8_PLUS_FOR_PCH_BCH    20000 /*plus 2 ms for PCH/BCH decoding*/
#endif /* FEATURE_TABASCO_MODEM */

#define TDSL1_RPM_DELAY_CX8             204800 /*20 ms*/

#define TDSL1_MIN_GAP_PROG_OLS_USEC     200
#define TDSL1_SLEEP_ABORT_INTLOCK()  REX_ISR_LOCK(&tdsl1sleep_sleep_abort_mutex)
#define TDSL1_SLEEP_ABORT_INTFREE()  REX_ISR_UNLOCK(&tdsl1sleep_sleep_abort_mutex)

#define TDSL1_RF_SLEEP_INTLOCK()  REX_ISR_LOCK(&tdsl1sleep_rf_sleep_mutex)
#define TDSL1_RF_SLEEP_INTFREE()  REX_ISR_UNLOCK(&tdsl1sleep_rf_sleep_mutex)


/* ==========================================================================
**
** Type Definitions
**
** ========================================================================*/
/* Definition of the states of the Sleep module state machine */
typedef enum
{
  /* Sleep module inactive; tdsl1sleep_go_to_sleep() hasn't been called yet */
  TDSSLP_INACTIVE,

  /* RF is in sleep but micro is powered up (no consensus to power down) */
  TDSSLP_RF_IN_SLEEP_MICRO_UP,

  /* RF in warm up (micro is up) */
  TDSSLP_RF_IN_WARM_UP,

  /* In deep sleep.  RF sleeping.  CPU (and TCXO) are currently up, as required
   * by other subsystem(s).
   */
  TDSSLP_RF_IN_SLEEP_MICRO_UP_IN_DEEP_SLEEP

} tdsslp_sleepModuleState_type;


/* Sleep duration in milliseconds */
typedef uint32 tdsslpDurationMillisec_type;

#if defined (DEBUG_TDSSLEEP_PROFILE_TIME_LINE) || defined(FEATURE_TDSCDMA_L1_LOGGING_ENABLED)
/*
 * Type definition for various profile points in the time line.
 */
typedef enum
{
  /* MWU - BEF */
  TDSSP_BEF_MWU,       // 0

  /* MCPM CONFIG for wakeup - BEF & AFT */
  TDSSP_BEF_MCPM_CFG,  // 1
  TDSSP_AFT_MCPM_CFG,  // 2

  /* RF WU - BEF */
  TDSSP_BEF_RF_ON,     // 3
  /* RF WU - AFT */
  TDSSP_AFT_RF_ON,     // 4

  /* MWU - AFT */
  TDSSP_AFT_MWU,       // 5

  /* OLS - BEF & AFT */
  TDSSP_BEF_OLS,       // 6
  TDSSP_AFT_OLS,       // 7

  /* DSP Stand by - BEF*/
  TDSSP_BEF_DSP_TRK,   // 8

  /* Reacq - BEF */
  TDSSP_BEF_REACQ,     // 9

  TDSSP_BEF_INTRA_RSCP_MEAS, //10

  /* DL AWAKE */
  TDSSP_BEF_PICH_SETUP,  // 11
  //TDSSP_AFT_PICH_SETUP, remove it because DL always send this msg at the sub-frame boundary.

  /* DSP Stand by - AFT */
  TDSSP_AFT_DSP_TRK,     // 12

  /* PICH SUB-FRAME BOUNDARY */
  TDSSP_PICH_SUBFRM_BDY,  // 13

  /* Reacq - BEF & AFT */
  TDSSP_AFT_REACQ,        // 14

  /* PICH DECODE DONE */
  TDSSP_PICH_DEC_DONE,    // 15

  TDSSP_AFT_INTRA_RSCP_MEAS, //16

  /* DL Pich Tear Down - BEF & AFT */
  TDSSP_BEF_PICH_TEAR_DOWN,   // 17
  TDSSP_AFT_PICH_TEAR_DOWN,   // 18
  
  /* SRCH Ready to Sleep */
  TDSSP_BEF_SRCH_READY_FOR_SLEEP,   // 19
  TDSSP_AFT_SRCH_READY_FOR_SLEEP,   // 20

  /* DSP Stand by - BEF & AFT */
  TDSSP_BEF_TFW_STANDBY,            // 21
  TDSSP_AFT_TFW_STANDBY,            // 22

  TDSSP_BEF_GFW_STANDBY,            //23
  TDSSP_AFT_GFW_STANDBY,            //24

  /* RF Off - BEF & AFT */
  TDSSP_BEF_RF_OFF,                 // 25
  TDSSP_AFT_RF_OFF,                 // 26

  /* Sleep_drv_setup - Finish */
  TDSSP_SLP_DRV_STP_FINISH,         // 27

  /* SLEEP_START - AFT */
  TDSSP_BEF_SLP_START,              // 28

  /* MCPM CONFIG for sleep - BEF & AFT */
  TDSSP_BEF_MCPM_DECFG,             // 29
  TDSSP_AFT_MCPM_DECFG,             // 30

  /* SLEEP_START - AFT */
  TDSSP_AFT_SLP_START,              // 31

  /*ABORT from real sleep*/
  TDSSP_SLEEP_ABORT,                // 32

  //#if (defined FEATURE_TRITON_MODEM) || (defined FEATURE_BOLT_MODEM)
    /* RF WU - BEF RF_step0 */
  TDSSP_BEF_RF_ON_STEP0,     // 33
  /* RF WU - AFT RF_step0*/
  TDSSP_AFT_RF_ON_STEP0,     // 34
  /* Total Events */
  //#endif
  TDSSP_NUM_POINTS
} tdsslp_profile_name_T;

/* Length of each data point */
#define TDSL1SLP_LOG_CYCLES 20

/* The following data structure contains TDSL1Sleep Profiling info in sleep-clocks w.r.t PICH */

typedef struct {
  uint32 sc[TDSSP_NUM_POINTS]; /*sleep clock counter at each time point */
  
  tdsl1_time_info_struct_type sdt_rsc; /*rx system time at SDT*/
  tdsl1_time_info_struct_type sdt_frc; /*wall time at SDT*/

  tdsl1_time_info_struct_type gts_rsc; /*rx system time at GTS*/
  tdsl1_time_info_struct_type gts_frc; /*wall time at GTS*/

  tdsl1_time_info_struct_type slamed_ols_rsc; /*restored rx system time at OLS*/
  tdsl1_time_info_struct_type slamed_ols_frc; /*restored wall time at OLS*/

  tdsl1_time_info_struct_type read_ols_rsc; /*read rx system time at OLS isr after slamming*/
  tdsl1_time_info_struct_type read_ols_frc; /*read wall time at OLS isr after slamming*/

  uint32 sleep_duration_Cx8; /*time inverval from GTS to OLS in unit of chipx8*/

  /*timing error in unit of chipx8 with univeral stmr as reference
      1. calculate the time length (t1) from SDT to OLS with sdt_rsc and read_ols_rsc
      2. calculate the time length (t2) with univ_stmr_cnt
      3. sleep_duration_error = t2*10.24/19.2 - t1

      note: universal stmr rans at 19.2 MHz, and chipx8 freq is 10.24 MHz*/
  int32  sleep_duration_error; 

  uint64 gts_tick;
  uint64 mwu_tick;
  uint64 ols_tick;

  /*pich rx system time, which is calculated by DL mgr according to PICH NW parameters */
  tdsl1_time_info_struct_type pich_rsc;
} tdsl1slp_timeline_profile ;


typedef struct {
  uint32 idx; /* drx cycle index */
  tdsl1slp_timeline_profile timeline[TDSL1SLP_LOG_CYCLES];
  
  uint32 max_mcpm_cfg_dur;
  uint32 max_mcpm_decfg_dur;

  uint32 max_rf_warmup_dur;
  uint32 max_rf_sleep_dur;

  uint32 max_state_cfg_track_dur;
  uint32 max_state_cfg_standby_dur;
} tdsl1slp_log_struct_type;
extern tdsl1slp_log_struct_type tdsl1slp_log;

#endif

/*
 * Type definition for Sleep Contoller Interrupt.
 */
typedef enum
{
  TDSSLP_CTLR_INT_NONE,
  TDSSLP_CTLR_BEF_SLEEP_START,
  TDSSLP_CTLR_SLEEP_START_INT,
  TDSSLP_CTLR_MICRO_WAKE_UP_INT,
} tdsslp_ctlr_int_type;

typedef enum
{
  TDSSLP_CTLR_INT_STATUS_ONLINE = 0x01,
  TDSSLP_CTLR_INT_STATUS_SLEEP  = 0x02,
  TDSSLP_CTLR_INT_STATUS_WARMUP = 0x04
} tdsslp_ctlr_int_status_type;

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#ifdef FEATURE_TDSCDMA_ADAPTIVE_WAKEUP_TIMELINE

typedef enum
{
   TDSL1SLEEP_ADAPTIVE_INACTIVE,
   TDSL1SLEEP_ADAPTIVE_INITIAL,
   TDSL1SLEEP_ADAPTIVE_TRACKING,
   TDSL1SLEEP_ADAPTIVE_NUM_MODES,
}tdsl1sleep_adaptive_mode_type;

typedef struct
{
   /*The number of total DRX cycles since last entry to IDLE mode*/
   uint32 drx_total_num;
   /*The number of PICH miss cycles since last entry to IDLE mode*/
   uint32 drx_miss_num;
   /*Current mode: inactive/initial/tracking*/
   tdsl1sleep_adaptive_mode_type mode;
   /*Variables need to run IIR filter calculation */
   /*The value of OLS_TO_TARGET time for the next DRX cycle*/
   uint32 ols_to_target_Cx8;
   /*The final value of OLS_TO_TARGET with RF/MCPM offset added for RxD state change/ 
     wakeup at different freq/Potential Device change in DR mode. */
   uint32 ols_to_target_Cx8_final;
   /*The mean of OLS_TO_TARGET time for all past DRX cycles*/
   uint32 mean_est;
   /*The standard variance of OLS_TO_TARGET time for all past DRX cycles*/
   uint64 sigma_est;
   /*Flag of pich miss for current DRX cycle*/
   boolean pich_miss_flag;
   /* indicate if the wakeup timeline can be applied to next wakeup timeline adjustment, 
      if its' not applicable, e.g. RF/FW not woken-up due to TRM denial, 
         skip updating the timeline info to adaptive timeline database
      if it's for PCH/BCH wakeup, it's not applicable either, don't reset/overwrite 
         this flag since the previous one might be applicable .
      this flag can be set TRUE at most once in one drx cycle, it should 
         be reset to FALSE after the timeline data is used for next wakeup timeline calculation to 
         avoid it being repeatedly used.
      */
   boolean wakeup_timeline_applied_to_adaptive_adjustment;
   /* The timestamp whether receving both RF CNF and TFW state transition RSP or not during wakeup*/
   boolean wakeup_done_flag;
   /* The timestamp when receving both RF CNF and TFW state transition RSP */
   tdsl1_time_info_struct_type wakeup_done_tsp;
   /* The scheduling point of tfw (currently at the start of TS6 midamble before the PICH subframe bounary*/
   tdsl1_time_info_struct_type tfw_sp;
#if 0 // Keep it temp fro future use
   // For the target without VSTMR, need to use USTMR instead since TSTMR could be not restored 
   /* The timestamp when receving both RF CNF and TFW state transition RSP */
   uint32 wakeup_done_tsp;
   /* The scheduling point of tfw (currently at the start of TS6 midamble before the PICH subframe bounary*/
   uint32 tfw_sp;
#endif 
   // The margin from FW/RF cnf to the TS6 midamble start point
   int32 margin;
}tdsl1sleep_adaptive_struct_type;

typedef struct
{
   /* The total number of drx cycles */
   uint32 total_cycles;
   /* The total number of miss drx cycles */
   uint32 total_miss_cycles;
   /* record the statistics of the margin from the time L1 receive both TFW
      state transition CNF and RF wakeup CNF to TS6 midamble start.(unit: cx8)
   [0] -- NO. of cycles with margin < -6000        (late more than 0.6 ms)
   [1] -- NO. of cycles with margin [-6000, -4000) (late 0.4~0.6 ms)                                                                             )
   [2] -- NO. of cycles with margin [-4000, -2000) (late 0.2~0.4 ms)                                                                                                                                                                                                                            )
   [3] -- NO. of cycles with margin [-2000, 0)     (late 0~0.2 ms)                                                                                                                                                                                                                   )
   [4] -- NO. of cycles with margin [0, 2000)      (ealry 0.0~0.2 ms)                                                                                                                                                                                                                                                                                      )
   [5] -- NO. of cycles with margin [2000, 4000)   (ealry 0.2~0.4 ms)                                                                                                                                                                                                                                                                                                                                                          )
   [6] -- NO. of cycles with margin [4000, 6000)   (early 0.4~0.6 ms)                                                                                                                                                                                                                                                                                                                                                                                                                            )
   [7] -- NO. of cycles with margin [6000, 8000)   (early 0.6~0.8 ms)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        )
   [8] -- NO. of cycles with margin [8000, 10000)  (early 0.8~1.0 ms)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           )
   [9] -- NO. of cycles with margin > 10000        (early more than 1.0 ms)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  )
   */
   uint32 dist_tbl[10];
}tdsl1sleep_adaptive_stat_db_type;

typedef struct
{
   // The target miss rate, defualt value: 0.001 * 1000
   uint16 target_pich_miss_rate;
   /* tdsl1sleep_adaptive_trk_down_factor = (1000-tdsl1sleep_adaptive_global.params_static.target_pich_miss_rate)/ \
      tdsl1sleep_adaptive_global.params_static.target_pich_miss_rate*/
   uint16 trk_down_factor;
   // The fast converge rate in tracking mode, default value: 0.1 * 1000
   uint16 converge_rate_fast;
   // The slow converge rate in tracking mode, default value: 0.01 * 1000
   uint16 converge_rate_slow;
   /* The number of DRX cycles when the converge rate from fast to slow in tracking mode default value: 100*/
   uint32 onverge_rate_switch_threshold;
   // The number of Chip8 for step size in initial mode
   uint32 step_size_of_initial_mode;
}tdsl1sleep_adaptive_cfg_params_type;

/* The flag: the static params have been get from NV or not*/
extern boolean tdsl1sleep_adaptive_static_params_init;

/* The packed global for Adaptive wakeup timeline feature*/
typedef struct
{
   /* The run-time data for adaptive wakeup timeline feature*/
   tdsl1sleep_adaptive_struct_type params_runtime;
   /* The statistics data for all the past drx cycles */
   tdsl1sleep_adaptive_stat_db_type stat_db;   
}tdsl1sleep_adaptive_global_type;
extern tdsl1sleep_adaptive_global_type tdsl1sleep_adaptive_global;
#endif /*FEATURE_TDSCDMA_ADAPTIVE_WAKEUP_TIMELINE*/

/* ==========================================================================
**
** Global Variable Definitions
**
** ========================================================================*/
extern boolean tdsdrx_debug_mcpm_idle_rx_at_wakeup;

/* only for debug: keep stmr always on in drx state */
extern boolean tdsl1sleep_debug_not_turn_off_stmr;
extern boolean tdsl1sleep_enable_power_collapse;
/*
 * Whether ok to turn off tcxo from sleep driver's point of view
 */
extern boolean tdsl1sleep_oktcxo;

extern boolean tdsl1sleep_gsm_clk_enabled;

extern volatile boolean tdsl1sleep_drv_ols_has_occured;

extern boolean tdsl1sleep_drv_multiple_int;
extern boolean tdsl1sleep_debug_rx_time_adj_at_reacq_resp;

extern uint16 tdsdrxentity_target_freq;

extern boolean tdsl1sleep_wakeup_for_pich;

extern boolean tdsl1sleep_ols_stmr_restore_failure ;
extern uint32  tdsl1sleep_ols_stmr_restore_fail_cnt;

extern boolean tdsl1sleep_optimized_timeline;

extern boolean tdsl1sleep_rf_sleep_early_event_happened;

/*The flag for whether need to do IRAT or Inter-F Measurement in order to decide whether RF can go to sleep early*/
extern boolean tdsl1sleep_irat_or_interf_meas_prevent_rf_sleep_early;
extern boolean tdsl1sleep_rf_early_sleep_event_enabled;

extern boolean tdsl1sleep_gsm_asleep;

#ifdef FEATURE_TDSCDMA_RF_WU_STEP0_SEP
extern boolean tdsl1sleep_rf_step0_called;
extern boolean tdsl1sleep_rf_step0_p1_done;
#endif

/* ==========================================================================
**
** Function declarations
**
** ========================================================================*/

extern uint32 tdsl1sleep_drv_mult_div(uint32 a, uint32 b, uint32 c);

/*===========================================================================

FUNCTION SLEEP_ERROR_FEEDBACK

DESCRIPTION
    once getting the timing error after re-acq, feedback it to sleepctl 
    for next sleep cycle adjustment.
    
DEPENDENCIES
    None
    
RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/

extern void tdsl1sleep_error_feedback(int32 timingerror);


/*===========================================================================

FUNCTION     WSLEEP_INIT

DESCRIPTION This function is called to initialize the TDSCDMA Sleep module,
  and to register a call back function for wake up.
  See further description in the sleep.c file.

DEPENDENCIES
  None

RETURN VALUE
  True if all went well; False otherwise.

SIDE EFFECTS

===========================================================================*/
extern boolean tdsl1sleep_init(void);

/*===========================================================================

FUNCTION     GO_TO_SLEEP

DESCRIPTION This function is called to initiate the the sleep
  process for a specified period (in milliseconds).

DEPENDENCIES
  None

PARAMETERS
  wakeUpWithinMs   - Duration of the sleep period.
  wake_up_for_ctch - TRUE if we're waking up for CTCH.  FALSE for PICH.

RETURN VALUE
  TRUE if all went well; FALSE otherwise.

SIDE EFFECTS

===========================================================================*/
extern boolean tdsl1sleep_go_to_sleep(tdsl1_time_info_struct_type target_time);


/*===========================================================================

FUNCTION       SLEEP_ABORT

DESCRIPTION
  This function aborts a sleep in progress.
  See further description in the sleep.c file.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void tdsl1sleep_abort( void );


#if defined (DEBUG_TDSSLEEP_PROFILE_TIME_LINE) || defined(FEATURE_TDSCDMA_L1_LOGGING_ENABLED)
extern void tdsl1sleep_drv_profile_point( tdsslp_profile_name_T name);
#else
#define tdsl1sleep_drv_profile_point(a) /* do nothing */
#endif

/*===========================================================================

FUNCTION SLEEP_DRV_INIT, SLEEP_DRV_SETUP

DESCRIPTION
    These functions initialize and set up the sleep controller for beginning a sleep cycle

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
extern void tdsl1sleep_drv_init(void);

/*===========================================================================
FUNCTION     SLEEP_DRV_SETUP

DESCRIPTION
  This function programs the sleep controller for sleep at the beginning of
  a sleep cycle in DRX mode.  Generally we wake up to check PICH.  It is
  also possible that we will wake up to receive CTCH broadcast data.

DEPENDENCIES
  None.

PARAMETERS
  sleep_period_ms  - Duration of the sleep period (sometimes ignored and PICH
                     SFN used instead).
  wake_up_for_ctch - TRUE if we're waking up for CTCH.  FALSE for PICH.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdsl1sleep_drv_setup(void);

/*===========================================================================

FUNCTION SLEEP_DRV_ABORT

DESCRIPTION
    This function initiates an abort of the sleep cycle

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
extern void tdsl1sleep_drv_abort(void);

/*===========================================================================

FUNCTION IS_SLEEP_DRV_IN_SLEEP

DESCRIPTION
    This function returns TRUE or FALSE depending on the last interrupt
    received from the sleep controller. Sleep driver is supposed to be in
    sleep if the last interrupt is SLEEP_START_INT

DEPENDENCIES
    None

RETURN VALUE
    TRUE : if last interrupt is SLEEP_START_INT
    FALSE: all other interrupts other than SLEEP_START_INT

SIDE EFFECTS
    None

===========================================================================*/
extern boolean tdsl1sleep_drv_in_sleep(void);

#if defined(FEATURE_CGPS_TIME_TAGGING)


typedef struct
{
  //tdsl1_cell_time_type  cell_time;
  //Arun -Typedef of typedef not working
  cgps_TDScdmaCellTimeType  cell_time;

  uint8  is_valid_flag;

} tdsl1_cell_time_info_type;

/*===========================================================================
FUNCTION        GET_CURRENT_CELL_TIME

DESCRIPTION     Returns the cell time (wl1_cell_time_info_type).

DEPENDENCIES    None

RETURN VALUE:   Returns whether a sleep timetag was performed.
                TRUE:  Sleep Timetag
                FALSE: Not a Sleep Timetag

SIDE EFFECTS    None
===========================================================================*/
extern boolean tdsl1sleep_get_current_cell_time(tdsl1_cell_time_info_type *info);
#endif /* FEATURE_CGPS_TIME_TAGGING */


extern void tdsl1sleep_set_sleep_state(tdsslp_sleepModuleState_type newState);

/*--------------------------------------------------------------------------
 * The following macros program TDSCDMA Modem Sleep Controller registers
 * Modem Sleep Controller is used in MSM 6260 and above
 *--------------------------------------------------------------------------*/
/*===========================================================================

FUNCTION     GET_SLEEP_STATE

DESCRIPTION This function is called to get the state of the sleep module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern tdsslp_sleepModuleState_type tdsl1sleep_get_sleep_state(void);

extern uint32 tdsl1sleep_get_target_rx_subfn (void);

extern tdsl1_time_info_struct_type tdsl1sleep_get_target_rsc(void);

/* Function prototypes */
extern uint32 tdsl1sleep_get_current_sleep_interval_cx8(void);

void tdsl1sleep_re_program_slpc_due_to_miss_ols(void);

#ifndef TEST_FRAMEWORK
#ifdef FEATURE_TDSCDMA_VSTMR
#define TDSL1SLEEP_DRV_INSTALL_SLEEP_CTL_ISR() \
  slpc_set_wakeup_callback(SLPC_TDSCDMA, tdsl1sleep_micro_wake_up_isr )

#define TDSL1SLEEP_DRV_UN_INSTALL_SLEEP_CTL_ISR() \
  slpc_set_wakeup_callback( SLPC_TDSCDMA, NULL)

#define TDSL1SLEEP_DRV_SET_SLP_CTL_SLEEP_INTERVAL() \
  slpc_set_duration(SLPC_TDSCDMA, (uint64)tdsl1sleep_interval_Cx8)

#else
#define TDSL1SLEEP_DRV_INSTALL_SLEEP_CTL_ISR()                            \
{                                                                         \
  slpc_set_wakeup_callback( SLPC_TDSCDMA, tdsl1sleep_micro_wake_up_isr ); \
  slpc_set_ols_callback( SLPC_TDSCDMA, tdsl1sleep_on_line_start_isr );    \
}

#define TDSL1SLEEP_DRV_UN_INSTALL_SLEEP_CTL_ISR() \
{                                                 \
  slpc_set_wakeup_callback( SLPC_TDSCDMA, NULL);  \
  slpc_set_ols_callback( SLPC_TDSCDMA, NULL);     \
}

/* The second variable to the functions sleepctl_set_warmup is the wakeup time in us. 
    Convert tdsl1sleep_mwu_to_ols_Cx8 to us */

#define TDSL1SLEEP_DRV_SET_SLP_CTL_SLEEP_INTERVAL()                 \
{                                                                   \
  slpc_set_duration_and_warmup(SLPC_TDSCDMA,                        \
    (uint64)(tdsl1sleep_interval_Cx8 + tdsl1sleep_mwu_to_ols_Cx8),  \
    (uint16)((tdsl1sleep_mwu_to_ols_Cx8 * 1000)/TDSL1_CX8_PER_MS)); \
}

#endif //FEATURE_TDSCDMA_VSTMR
#else /*ifdef TEST_FRAMEWORK */
#error code not present
#endif /* #ifndef TEST_FRAMEWORK */

/*===========================================================================
FUNCTION        SLEEP_DRV_EXIT

DESCRIPTION     This function resets various parameters to their original values.

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void tdsl1sleep_drv_exit(void);

/*===========================================================================
FUNCTION        SLEEP_DRV_SLAM_GFN

DESCRIPTION     This function sets the GFN.

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void tdsl1sleep_drv_slam_gfn(void);

/*===========================================================================
FUNCTION        tdsl1sleep_drv_prepare_for_status_dump

DESCRIPTION     This function sets up in preparation for saving related Time 
                information of last sleep cycle.

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void tdsl1sleep_drv_prepare_for_status_dump(void);


/*===========================================================================

FUNCTION SLEEP_DRV_STMR_STATUS_DUMP

DESCRIPTION
    This is a helper function that does a STMR status dump
    and some associated bookkeeping for GSM time maintenence.

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
extern void tdsl1sleep_drv_stmr_status_dump(void);

/*===========================================================================

FUNCTION SLEEP_DRV_COMPUTE_GFN_gQSYM_AT_OLS

DESCRIPTION
    This function calculates the gfn and gqsym that should be configured at ols

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
extern void tdsl1sleep_drv_compute_gfn_gqsym_at_ols(void);

/*===========================================================================
FUNCTION        SLEEP_DRV_GET_SC_REM_TO_PICH

DESCRIPTION     Returns time in Sleep-Clocks from now to PICH
                This should be used before PICH in a DRX cycle

DEPENDENCIES    None

RETURN VALUE    int32

SIDE EFFECTS    None
===========================================================================*/
extern int32 tdsl1sleep_drv_get_sc_rem_to_pich(void);

/*===========================================================================

FUNCTION SLEEP_ON_LINE_START_ISR

DESCRIPTION
    ISR for the ON_LINE_START interrupt generated.

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/

extern void tdsl1sleep_on_line_start_isr(void);

extern boolean tdsl1sleep_wkup_cancel_pending;

extern boolean tdsl1sleep_debug_keep_lock_for_pch ;
extern boolean tdsl1sleep_debug_need_release_source;
extern boolean tdsl1sleep_keep_lock_flag;
extern boolean tdsl1sleep_rf_enter_tds_mode_at_wakeup;
extern uint16  tdsl1sleep_wakeup_freq;

/*===========================================================================
FUNCTION     TDSL1SLEEP_KEEP_LOCK_DURING_SLEEP

DESCRIPTION
  check whether the LOCK should be kept during this sleep.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern boolean tdsl1sleep_keep_lock_during_sleep(void);
/*===========================================================================
FUNCTION     TDSL1SLEEP_GET_SLEEP_DUR_SC

DESCRIPTION
  return sleep duration in sclk counter.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern uint32 tdsl1sleep_get_mwu_sc(void);
/*=========================================================================

FUNCTION    tdsl1sleep_wakeup_manager_cb

DESCRIPTION
  Call back from wake up manager (WMGR) to inform a wake up collision with
  other active RAT.


PARAMETERS
  Client - Client ID.
  result -The enum with the result for wakeup.
  transition - Layer1 transitions.
  reserved -  Reserved for enhancements.
  user_data- User defined data if any. 
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
Note: This call back can happen in any active RAT (interrupt, task) context where 
trm_reserve is called.
=========================================================================*/
extern void tdsl1sleep_wakeup_manager_cb 
(
  /* RAT ID */
  wmgr_client_enum_t  client,

  /* The enum with the result */
  wmgr_result_type    result,
  
  /* Layer1 transitions */
  uint32              transition,

  /* Reserved for enhancements */
  uint32              reserved,

  /* User defined data */
  void                *user_data
);


extern void tdsl1sleep_drv_cancel_wakeup(void);

#if 0
/*===========================================================================
FUNCTION     TDSL1SLEEP_DSDS_DETERMINE_COLLISION

DESCRIPTION
  Determine if next TDSCDMA wakeup is very shortly before GSM wakeup.  If so,
  wake TDSCDMA a little bit earlier to hopefully avoid collision.  Collisions
  can cause GSTMR update problems.

  GSM is also doing this exact same collision check.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
===========================================================================*/
extern void   tdsl1sleep_dsds_determine_collision(void);
#endif



/*===========================================================================

FUNCTION     TDSL1SLEEP_ASSURE_RF_AWAKE

DESCRIPTION
  This function is called to check whether RF state is awake.
  if not, let's wait in this function until RF wake-up is done.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdsl1sleep_assure_rf_awake(void);

/*===========================================================================

FUNCTION tdsl1sleep_assure_rf_awake_without_waiting

DESCRIPTION
  Assure RF is in the state of TDSRF_WAITING_FOR_WAKEUP_CNF or TDSRF_AWAKE
  In other words, do not need to receive the CNF of RF_WAKEUP command

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void tdsl1sleep_assure_rf_awake_without_waiting(void);
/*===========================================================================

FUNCTION     TDSL1SLEEP_ASSURE_RF_FW_AWAKE

DESCRIPTION
  This function is called to check whether RF is already awake,and FW is tracking/
  track_idles. RF wakeup request and FW state transition request should be sent before
  this function is got called, if we don't receive RF wakeup confirmation or FW state 
  change response, let's wait in this function until the both responses received.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdsl1sleep_assure_rf_fw_awake(void);

/*===========================================================================
FUNCTION     tdsl1sleep_rf_sleep_early

DESCRIPTION
  RF sleep early at TS1. 

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  boolean: TRUE -- RF sleep early at TS1
           FLASE -- otherwise
===========================================================================*/
extern void tdsl1sleep_rf_sleep_early(void);

#ifdef FEATURE_TDSCDMA_ADAPTIVE_WAKEUP_TIMELINE
/*===========================================================================
FUNCTION     tdsl1sleep_adaptive_init

DESCRIPTION
  Initialize the module of adaptive wakeup timeline feature.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None. 
===========================================================================*/
extern void tdsl1sleep_adaptive_init(void);

/*===========================================================================
FUNCTION     tdsl1sleep_adaptive_mode_entry

DESCRIPTION
  Trigger the corresponding procedure when entry to a specific mode. 

DEPENDENCIES
  None.

PARAMETERS
  tdsl1sleep_adaptive_mode_type new_mode.

RETURN VALUE
  None. 
===========================================================================*/
extern void tdsl1sleep_adaptive_mode_entry(tdsl1sleep_adaptive_mode_type new_mode);

#endif /*FEATURE_TDSCDMA_ADAPTIVE_WAKEUP_TIMELINE*/

#endif /* TDSL1SLEEP_H */

