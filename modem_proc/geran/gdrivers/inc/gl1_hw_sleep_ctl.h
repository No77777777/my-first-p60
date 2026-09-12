#ifndef GL1_HW_SLEEP_CTL_H
#define GL1_HW_SLEEP_CTL_H
/*===========================================================================
                      GSM GPRS NPL1 FRAME LAYER HEADER FILE
DESCRIPTION
   This file defines the interfaces to the GSM/GPRS NPL1 FRAME LAYER.
   The GSM/GPRS NPL1 frame layer abstracts all hardware concepts. The frame
   layer functions typically need to be called periodically every frame.

   gl1_hw_sleep_ctl.h - File handling sleep controller and L1 sleep.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2008-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/2.2/gdrivers/inc/gl1_hw_sleep_ctl.h#14
$ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when        who     what, where, why
--------    ---     -------------------------------------------------------- 
22/04/16   ksb      CR1005869 To mainlne FEATURE_GL1_SLEEP_DEBUG_STRUCT
05/12/17    km      CR987605 G2X immediate cleanup
03/012/15   dp      CR929648 reduce power usage when waiting for TRM during find_bcch
21/11/15    ip/mko  CR938381: In DSDS mode, add larger threshold to error recovery, prevents trying to recover if less than half a frame to run
03/09/15    kb      CR863908:Added new EFS file structure for sleep control
15/08/15    df      CR882861 Improve sleep debug structure
29/07/15    fan/akh CR873850 Missed pch notification timestamp correction
24/07/15    df      CR868476 Adaptive timeline bounces in idle 
15/07/15    fan/akh CR857332 FR19329: WMGR support for rescheduling non-PCH activities
08/07/15    df      CR815683 Avoid error recovery when G2W scripts are built during wakeup 
12/06/15    akh     CR849775 Wkup Mgr fixes - wakeup mgr running twice during GTS, debug code
04/06/15    fan/akh CR848455 Fix for multiple handlings of same cancelation request
12/06/15    akh     CR852568 Debug code for wkup mgr cancellation
08/06/15    ip      CR847902 Miss first burst in QTA gap when warm up time is too short instead of missing PCH
01/06/15    df      CR803641 Save pcycle when committing
21/05/15    df      CR801738 Increase adaptive warmup step size during error recovery 
20/05/15    fan     CR837443 - Remove CR788527 (temporary fix until WMGR cancel mainlined)
07/05/15    fan/akh CR834237 FR19329 Full G wake-up cancellation + code cleanup
30/04/15    ip      CR799995 FR26290 Sleep when X tech is in connected state
24/04/15    fan     CR796481 Full G wakeup cancellation
21/04/15    zf/cs   CR808438 Correction on GPS timetag reporting in wakeup
20/04/15    ip      CR811868 Dual Multisim mode code changes + debug overhaul
20/02/15    ab      CR773948 GL1 Support for provding Dumps and Metrics for QSH
16/03/15    pg      CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
20/02/15    kb      CR788527 Modify the minimum adaptive warmup timeline for Extended paging case.
19/01/15    df      CR779661 Enforce upper limit adaptive timeline & add logging
27/11/14    fan     CR758004 Decrease lower limit on adaptive timeline result
01/11/14    df      CR748602 Rework and enable adaptive timeline
21/10/14    fan     CR734490 Rework the wake up recovery mechanism
14/10/14    df      CR676084 Sleep debug structure improvements
26/09/14    fan     CR730788 Moving the start RF timer next to setting RF_WAKEUP flag to the commit mask
                             and correctly abort PWR mon
12/09/14    fan     CR636420: Run Sleep commit after GSTMR ISR at wakeup
25/08/14    df      CR660576 Enhanced timeline debug
11/08/14    fan     CR703945 Adaptive warm up timeline
06/08/14    df      CR536059 Allow Asynchronous sleep for all idle activities
06/08/14    df      CR590772 - Use of NV items to control sleep and features
01/07/14    npt     CR682701 Do not try to run async early go to sleep if normal go to sleep has already started
19/06/14    ssh     CR679781: Clear any pending deferred WMGR events if trm reservation done prior to sleep
20/05/14    ssh     CR667235 Whilst reprogramming sleep on WUP cancel always check if all conditions met
04/04/14    cah     CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
27/02/14    ip      CR608864 Dime3.0 DB timeline changes
05/02/14    ssh     CR608558: Run error recovery even during the first wake up after the collision detection.
03/02/14    ssh     CR610637:FEATURE_MSIM_VSTMR checkin introduced compilation errors
30/01/14    npt     CR608373 - Enable opt3 on Bolt/Remove unused apis 25/09/13
26/11/13    ip/zf   CR581867:Adjust sleep timeline for RF init.
25/11/13    ip/ssh  CR580882:Extend warmup time in MS case when the power opts are disabled
20/11/13    zf      CR579309: Notify FW USTMR count at OLS.
19/11/13    ssh/ip  CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13    nm      CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
08/11/13    ip      CR547955 - Use multi threads on wake from sleep
05/11/13    ssh     CR570747: DINOSTABLE-23645|Optimise the warm up time for LMM when in single sim mode
25/10/13    ssh     CR563918 - Reduce the wake up isr budget from 19200 to 10000
11/10/13    zf      CR552306: Notify GFW OLS USTMR time.
25/09/13    fan     CR537338 - QDSS logging
11/10/13    zf      CR552306: Notify GFW OLS USTMR time.
25/10/13    ssh     CR563918 - Reduce the wake up isr budget from 19200 to 10000
05/10/13    sk      CR554665 Backout the power opt temperorily(CR537690)
24/09/13    zf      CR537690: Reduce G wakeup timeline for MS mode
31/07/13    ip      CR518821 - reduce the warmup time
10/07/13    ip      CR508975 - Adjust thresolds for CS power goals
01/07/13    ip      CR506628 - FC power code changes
20/06/13    cs      Major Triton TSTS Syncup
04/06/13    cs      Use latest SLPC id's for TSTS
24/05/13    kb/npt   CR459207 - Maintain TDSCDMA frame reference count through
                     GSM sleep when there are TDSCDMA neighbours
23/05/13    cs      TSTS Updates
17/05/13    kb/npt  CR385548 - Decode PCH after sleep wakeup error recovery.
03/01/13    npt     CR430322 - Modify sleep debug structure
06/12/12    npt     Further change of recovery thresholds
20/11/12    npt     CR422880 - Modify wake-up recovery thresholds on Dime
09/11/12    npt     CR380795 - Added core CPU vdd NPA client handle
23/10/12    npt     CR413635 - Port sleep code for DIME modem
13/09/12    npt     CR397821 - Reduce WCDMA warm-up time in wakeup recovery mechanism
                    to avoid "wake up in the past" issues
03/09/12    npt     CR394199 - Added WCDMA recovery wakeup callback function
03/08/12    npt     CR383592 - Reduce WCDMA warm-up time to avoid GSM and WCDMA
                    wake-up interrupts firing at the same time
12/07/12    npt     CR377917 - Increase warm-up time to accommodate for extended MCPM activities
11/07/12    npt     CR376737 - Change of thresholds for triggering recovery mechanism
                    and committing to ending interrupt
04/07/12    npt     CR373673 - Provide additional debug information in case of insufficient warm-up time
                    and dynamically modify warm-up time
28/06/12    npt     CR372829 - Change warm-up time to cater for extended RF antenna tune
31/05/12    npt     CR360829 - Print out warning message when MCPM or RF clks on/off
                    takes longer than budgeted for
15/05/12    npt     CR360462 - Modify WCDMA warm-up time in recovery mechanism
24/04/12    npt     CR354790 - Remove old sleep debug (under DEBUG_GL1_SLEEP) as it is not needed
                    after FEATURE_GL1_SLEEP_DEBUG_STRUCT is introduced
20/04/12    npt     CR348900 - Switch RF clks off when the sleepctl is armed
20/03/12    npt     CR344882 - Increase allowed processing time in commit to wake up process
16/03/12    npt     CR343683 - Modification of commit to wakeup logic
15/03/12    cgc     moved existing sleep header to here and added new slpc.h FEATURE_GSM_SLPC_CONTROLLER
08/03/12    ip      CR326699 - Code changed for wake up recovery mechanism
12/01/12    npt     CR311544 - Added sleep debug structure - FEATURE_GL1_SLEEP_DEBUG_STRUCT
12/07/11    npt     CR293222 Modified WCDMA sleep controller timeline
                    in forced wakeup scenario to prevent "wakeup in the past" issues
23/06/11    npt     CR288201 Adding extra warmup time to prevent stability issues
                    for implementation of MIPS relinquishing
14/02/11    npt     CR264682 Add support for asynch dsp completion interrupt
14/01/11    ip      Enabled Option 4 power feature for 7x30
23/07/10    ip      C245474 Added improved Sleep debug
07/06/10    pg      Modify GL1_HW_SAFE_RF_WARMUP_TIME for QDSP6 targets
17/05/10    nf      CR235198 Increased FORCE_WAKEUP_PROC_TIME for SCMM
07/05/10    ip      CR235204 DCVS settings added for Poseidon 2
09/04/10    ip      CR221022 Added extra FEE state to block an estimation if required
27/05/09    ws      Ammendment to CR183660 changed GL1_HW_NUM_FEE_TE_QSYMBOLS to 12
27/05/09    cs/nf   CR183660 Increase the FEE threshold to allow for corrections
                    out of spec sleep crystals during HT/HV testing
30/03/09    ws      Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
30/01/09    ip      CR 167135 Added API call for sleep debug code
30/01/09    ip      Initial version based on gl1_hw.h

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

#include "comdef.h"
#include "timetick.h"

  #include "slpc.h"

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
/*----------------------------------------------------------------------
QDSS logging related includes and defines
----------------------------------------------------------------------*/
/*tracer header files*/
#include "tracer.h"
#include "gdrivers_tracer_event_ids.h"
#include "tracer_config.h"
#endif /* FEATURE_GL1_SLEEP_QDSS_LOG */

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Amount of time it takes to do go to sleep computations
 * in GSM symbols.  This was calibrated on MSM6275
 */
#define NUM_SYMBOLS_FOR_GO_TO_SLEEP          (136)      /* 500us */

/* NOTE: MAX_SYMBOL_COUNT_TO_SLEEP must be much greater than
 * NUM_SYMBOLS_FOR_GO_TO_SLEEP.
 */
#define MAX_SYMBOL_COUNT_TO_SLEEP_NON_DSDA   (850)      /* ~1.5ms from end of frame */
#define MAX_SYMBOL_COUNT_TO_SLEEP_DSDA       (700)      /* ~2.0ms from end of frame */
#define MAX_SYMBOL_COUNT_TO_SLEEP            (850)      /* 1.5ms from end of frame */
#define MIN_SYMBOL_COUNT_TO_SLEEP            (3)
#define GL1_HW_WARMUP_SLEEPCTL_MARGIN_US     (1000)     /* 1.0ms */
#define FORCE_WAKEUP_PROC_TIME               (20000)    /* 1.5ms gsm clks */
/*
   For LCU we have precise tcxo and rf warmup times, need to allow time
   for clock switching and other processing time, from UDAS plots
   around 2mS required
*/
#define GL1_HW_WARMUP_EXTRA_MARGIN_US        (500)      /* 0.5ms */



/*
 * The reduced MPM force wakeup time is due to the MAO handling the normal wakeup stuff
 * which now only takes 4 msecs, so as RF warmup is 3msecs we only need an extra 1msecs margin
 */
#define MPM_FORCE_WAKEUP_WARMUP_EXTRA_MARGIN_US (1000)
#define FORCE_WAKEUP_WARMUP_EXTRA_MARGIN_US     (3000)

/* If the returned rf_warmup_time is ever 0 then use this instead */
#define GL1_HW_SAFE_RF_WARMUP_TIME              (20000)

/* Constants for WCDMA time line */
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
#define GL1_HW_SLEEP_WCDMA_GTS_MARGIN        (6144)     /* ~200us in chipx8 */
#define GL1_HW_SLEEP_WCDMA_WARMUP_TIME_US    (1000)
#define GL1_HW_SLEEP_WCDMA_DURATION_OFFSET   (0)
#endif

/* When choosing this value remember that there may be three attempts
 * to sleep in the same frame - two from the ISR and one from task.
 */
#define MAX_NOT_SLEEPING_COUNT_MDSP          (10)


/* After this number of consequitive tries to sleep while not in awake
 * sleep state - give up and ERROR_FATAL.
 */
#define MAX_NOT_READY_TO_SLEEP_COUNT         (10)


#define ASYNCH_INT_MAX_SYMBOL_COUNT_TO_SLEEP_INCREASE (275) /* Extra time for fast pch */

/* these have been added for wake up error recovery mechanism */
#define USTMR_CLK_FREQUENCY              19.2
#define GSM_CLK_FREQUENCY                13
#define ERROR_RECOVERY_G_GUARD_GSM_CLKS  13000
#define ERROR_RECOVERY_G_GUARD_US        (ERROR_RECOVERY_G_GUARD_GSM_CLKS/GSM_CLK_FREQUENCY)
/* from profiling code, sleepctl needs 500us to program new timeline, this is 6500 GSM clks */
#define ERROR_RECOVERY_TIMELINE_GSM_CLKS    (6500)
#define ERROR_RECOVERY_G_TRESHOLD           (3200)
#define ERROR_RECOVERY_G_COMMIT_PROC_TIME   (120)
#define G_COMMIT_PROC_TIME                  (120)

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

#define ERROR_RECOVERY_G_GUARD_DSDS_GSM_CLKS  30000
/* Changed 10000 gsm clocks is ~770us, time it to run isr in warm up interrupt */
#define NORMAL_WAKEUP_GSTMR_ISR_BUDGET_GSM_CLKS (10000)
/* Dime 3.0 2.2ms from commit 1.4mS RF warm up + 800 us CCS write time - minus gstrm init ( 200us) */
/* 2ms = 26000 GSM clocks */
/* In Eng 4 this was 36000  */
#define NORMAL_WAKEUP_GSTMR_GG_DSDS_ISR_BUDGET_GSM_CLKS (34000)
#define NORMAL_WAKEUP_GSTMR_GX_DSDS_ISR_BUDGET_GSM_CLKS (38000)
/* In Eng 4 this was 38000 change to match warm up reduction */
#define NORMAL_WAKEUP_GSTMR_DSDA_ISR_BUDGET_GSM_CLKS NORMAL_WAKEUP_GSTMR_MAX_ISR_BUDGET_GSM_CLKS
/* 28600 gsm clocks = 2200us */
#else
#define NORMAL_WAKEUP_GSTMR_ISR_BUDGET_GSM_CLKS    (10000)
#define NORMAL_WAKEUP_GSTMR_MS_ISR_BUDGET_GSM_CLKS (28600)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#define NORMAL_WAKEUP_GSTMR_G_ISR_BUDGET_GSM_CLKS (26000)
#define NORMAL_WAKEUP_GSTMR_MAX_ISR_BUDGET_GSM_CLKS (7800)
#define FORCED_WAKEUP_GSTMR_ISR_BUDGET_GSM_CLKS    (7800)
/* warm-up components budget in USTMR units*/
#define SLEEP_WARMUP_MCPM_BUDGET            (19200)
#define SLEEP_WARMUP_RF_VREG_ON_BUDGET      (13440)
#define SLEEP_WARMUP_INT_DELAY_BUDGET       (10000)
#if defined (FEATURE_DUAL_SIM) || defined ( FEATURE_TRIPLE_SIM )
/* Dime 3.0; 30720 1.6ms */
#define SLEEP_WARMUP_RF_RX_ON_BUDGET        (30720)
#else
/* 9x25 */
#define SLEEP_WARMUP_RF_RX_ON_BUDGET        (8650)
#endif
#define ERROR_RECOVERY_CONSECUTIVE_COUNTER_THRESHOLD (3)

/* Mask for USTMR timetick count */
#define SLEEP_USTMR_TIMETICK_COUNT_MASK            0x0000000007FFFFFFuLL

/* Use a few SCLK to absorb inaccurate TRM booking. */
#define NUM_SCLKS_PER_GSM_FRAME             (148)

/* Delta treshold (in sclks) between WCDMA and GSM sleep controler timelines
 * for modification of WCDMA sleep controler timeline in forced wakeup scenario
 */
#define SLEEP_START_DELTA_TRESHOLD             (100)

#ifdef FEATURE_GSM_GPRS_SLEEP_CANCEL_MIPS
#define NORMAL_WARMUP_TIME_ADJUSTMENT          (1000)
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define GL1_SLEEP_WUP_CANCEL_MIN_SLEEP_FRAMES               (4)
#define CONV_13M_TO_19_2M(x)                                (((x) * 6049) >> 12)
/* 4 GSM frames in USTMR ticks */
#define GL1_SLEEP_MIN_GAP_TO_WUP_CANCEL                     (CONV_13M_TO_19_2M(GL1_HW_SLEEP_WAKEUP_CONFLICT_RANGE * NUM_GSM_CLKS_PER_FRAME))
#define GL1_HW_SLEEP_WAKEUP_CONFLICT_RANGE                  (4)
#define GL1_HW_SLEEP_WAKEUP_NUM_CONFLICT_DEFER_FRAMES_X_G   (1)
#define GL1_HW_SLEEP_WAKEUP_NUM_CONFLICT_DEFER_FRAMES_G_G   (1)
#endif

/* States for gl1_hw_sleep_cont. */
typedef enum {
   GL1_HW_SLEEP_NULL = 0,
   GL1_HW_SLEEP_AWAKE,
   GL1_HW_SLEEP_GOING_TO_SLEEP,
   GL1_HW_SLEEP_ASLEEP,
   GL1_HW_EARLY_WAKEUP,
   GL1_HW_SLEEP_WARMUP
} gl1_hw_sleep_state_T;

/* FEE states */
typedef enum {
   GL1_HW_FEE_NULL = 0,
   GL1_HW_FEE_WAITING,
   GL1_HW_FEE_HOLDING,
   GL1_HW_FEE_USING_TE,
   GL1_HW_FEE_BLOCK
} gl1_hw_fee_state_T;


typedef struct
{
  boolean gl1_hw_latency_sent;
  uint32 gl1_hw_latency_sclk;
} gl1_hw_corecpu_latency_type;
#ifdef FEATURE_GSM_ADAPTIVE_TMLN
#define ADAPTIVE_TMLN_SAMPLE_COUNT 8
#define ADAPTIVE_TMLN_RIGHT_SHIFT_COUNT 3  // will be used to right shift for div by 8
#define WARMUP_STEP_SIZE 200
#define ADAPTIVE_ERR_RECOVERY_STEP_SIZE_US 1400 /* Increase in warmup time when 
                                                  2 error recoveries occur */
#define ADDITIONAL_G2W_SCRIPT_TIME_US 200
#define ERROR_COUNT_THRESHOLD 2
#define INITIAL_WARMUP_TIME 6400
#define CCS_DEFAULT_WRITE_TIME 800
#define MAX_GAP_2_STMR_TIME INITIAL_WARMUP_TIME
#define MIN_WARMUP_TIME 2500 /* Lower bound on get_adaptive_warmup_time return value */
#define QTA_WARMUP_TIME 5000 /* In QTA gap, if warm up too short set it to this value */
#define MAX_WARMUP_TIME 10000/* Upper bound on get_adaptive_warmup_time return value */


typedef struct
{
  uint16  error_cnt;  // Number of times recovery is hit
  /* gap to ols sample data store. This could be removed in future since the
     running total is what matters for averaging . But keep until feature stabilizes */
  int16   samples[ADAPTIVE_TMLN_SAMPLE_COUNT];
  uint16  index; // sample index
  int32   average_gap_to_ols; // gap_to_ols sample set avg
  uint32  initial_warmup_time; // warm up time used on first wake up
  uint32  last_good_warmup_time; // Last good warm up time
  uint32  current_warmup_time; // Current warm up time as per the last data collected
  /* Highest ever estimate, effectively the likely worst-case for future
     RF_AWAKE_REQs. Used to decide whether to decrease warmup time. */
  uint32  highest_CCS_Write_time_us; 
  /* Valid iteration or not ie RF CNF was called and a CCS_write time passed.
     For example, this would not get set for a rude wakeup */
  boolean valid;
  uint16  step_size; // step size to increase or decrease the current warm up time
} adaptive_warmup_tmln_struct_type;

void gl1_hw_adaptive_set_valid_flag(gas_id_t gas_id, boolean valid, uint32 CCS_write_time);

#endif  /* FEATURE_GSM_ADAPTIVE_TMLN */

/* -----------------------------------------------------------------------
**                             Sleep debug declarations
** ----------------------------------------------------------------------- */

/*
** Dime changes
*/

/*
** map ids for now
*/
#define SLEEPCTL_GSM        SLPC_GSM
#define SLEEPCTL_WCDMA      SLPC_WCDMA
#define SCLK_WCDMA          SLPC_WCDMA

#define GL1_SLPC_GSM_0      SLPC_GSM
#define GL1_SLPC_GSM_1      SLPC_GSM2


#define GL1_SLPC_WCDMA_0    SLPC_WCDMA

#ifdef FEATURE_DUAL_WCDMA
#define GL1_SLPC_WCDMA_1    SLPC_WCDMA2
#endif /* FEATURE_DUAL_WCDMA */

#if defined ( FEATURE_TRIPLE_SIM )
#define GL1_SLPC_GSM_2      SLPC_GSM3
#endif /* FEATURE_TRIPLE_SIM */

typedef slpc_id_type        sclk_enum_type;
typedef int64               sys_err_t;

extern boolean gl1_hw_sleep_disable[NUM_GERAN_DATA_SPACES];

/* CgC temp wrappers for dime */
void   gl1_sleepctl_error_feedback( sclk_enum_type sclkRat, sys_err_t sysclk_error, gas_id_t gas_id);
uint32 gl1_sleepctl_get_sclk_till_wakeup(uint32 ratWakeupId);

uint32 gl1_sleepctl_get_sclk_cnt(uint32 ratWakeupId);
uint32 gl1_sleepctl_get_sys_clk_cnt(uint32 ratWakeupId);

ustmr_type gl1_sleepctl_get_ustmr_at_ols( gas_id_t gas_id );

#ifdef FEATURE_GL1_SLEEP_QDSS_LOG
void QDSS_logging( gas_id_t gas_id,tracer_event_id_t event_id );
#endif /* FEATURE_GL1_SLEEP_QDSS_LOG */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
void gl1_hw_sleep_set_any_pending_deferred_wmgr_event(gas_id_t gas_id);
void gl1_hw_sleep_clear_any_pending_deferred_wmgr_event(gas_id_t gas_id);
boolean gl1_hw_sleep_wup_cancel_reprogram_allowed(gas_id_t gas_id);
boolean gl1_hw_sleep_reprogram_slpc(uint32 num_frames, gas_id_t gas_id);
void gl1_hw_sleep_handle_wakeup_conflict(gas_id_t gas_id);
void gl1_hw_sleep_handle_deferred_wakeup_conflict(gas_id_t gas_id);
void gl1_hw_sleep_wgmr_sm(gas_id_t gas_id);
void gl1_hw_sleep_wake_up_manager_cb_handler(wmgr_client_enum_t client,
                                   wmgr_result_type result,
                                   uint32 transition,
                                   uint32 reserved,
                                   void *user_data );

void gl1_hw_sleep_set_qta_active( gas_id_t gas_id, boolean qta_active );
boolean gl1_hw_sleep_get_qta_active( gas_id_t gas_id );
#endif

#ifdef FEATURE_DUAL_SIM
void gl1_hw_mcpm_restore_edge_block_gas_2_cb( void );
void gl1_hw_mcpm_restore_g_stmr_gas_2_cb( void );
#endif /*FEATURE_DUAL_SIM*/
#if defined ( FEATURE_TRIPLE_SIM )
void gl1_hw_mcpm_restore_edge_block_gas_3_cb( void );
void gl1_hw_mcpm_restore_g_stmr_gas_3_cb( void );
#endif /* FEATURE_TRIPLE_SIM */

extern boolean entered_warmup_recovery[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GSM_ADAPTIVE_TMLN
extern adaptive_warmup_tmln_struct_type adaptive_warmup_tmln[NUM_GERAN_DATA_SPACES];
void gl1_hw_set_adaptive_warmup_time( gas_id_t gas_id, uint32 set_warmup_time );
#endif

void gl1_hw_save_time_enter_mode_completed(gas_id_t gas_id);
void gl1_hw_save_ccs_write_time_estimate(uint32 ccs_write_time_us, gas_id_t gas_id);
gl1_hw_sleep_state_T gl1_hw_get_sleep_state(gas_id_t gas_id);
void gl1_hw_rf_result_monitor_timer_init(gas_id_t gas_id );
void gl1_hw_rf_result_monitor_timer_deactivate( gas_id_t gas_id );
void gl1_hw_rf_result_monitor_timer_start(uint32 time_to_wait_for_rf_cnf, gas_id_t gas_id );
void gl1_hw_timer_sleep_commit ( gas_id_t gas_id );
void gl1_hw_abort_idle_act_prog(gas_id_t gas_id );
uint16 gl1_hw_get_frame_number_mdsp_wakeup(gas_id_t gas_id);
uint16 gl1_hw_get_frame_number_ols(gas_id_t gas_id);
void gl1_hw_sleep_update_scheduled_activity(gas_id_t gas_id);
boolean gl1_hw_sleep_try_fw_sleep(gas_id_t gas_id);
boolean gl1_hw_sleep_try_fw_wakeup(gas_id_t gas_id);

#define GL1_HW_EFS_SLP_ENABLED( dEBUG_mASK ) \
  ( ( gl1_hw_efs_get_slp() & ( dEBUG_mASK ) ) == \
    ( dEBUG_mASK ) )

/* *** BITMASK FOR 'l1_sleep' EFS FILE *** 
 * Functionality controlled by each bit should be unique.
 * Each bit should control the same functionality across all targets.
 * Always keep this in order to avoid duplicate settings when mergin etc
 * Don't use FEATURES around these to avoid problems.
 */
#define GL1_EFS_SLP_DISABLE_SLEEP                0x00000001
#define GL1_EFS_SLP_ENABLE_SLEEP_WITH_GCF        0x00000002
#define GL1_EFS_SLP_DISABLE_ASYNCH_DEBUG         0x00000004
/* Enabling ASYNCH SLEEP will always disable OPT3.
 * To enable Opt3 via EFS, may sure you disable
 * ASYNC sleep bit(GL1_EFS_SLP_DISABLE_ASYNCH_SLEEP) 
 */
#define GL1_EFS_SLP_DISABLE_OPTION3              0x00000008
#define GL1_EFS_SLP_DISABLE_ASYNCH_SLEEP         0x00000010
#define GL1_EFS_SLP_DISABLE_SBD                  0x00000020
#define GL1_EFS_SLP_DISABLE_EARLY_DECODE         0x00000040
#define GL1_EFS_SLP_DISABLE_ADAPTIVE_TIMELINE    0x00000080
#define GL1_EFS_SLP_DISABLE_WKUP_CANCEL          0x00000100
#define GL1_EFS_SLP_QDSS_LOG_ENABLE              0x00000200
#define GL1_EFS_SLP_DISABLE_SLEEP_RF             0x00000400
#define GL1_EFS_SLP_DISABLE_SLEEP_GFW            0x00000800
#define GL1_EFS_SLP_DISABLE_G2X_TIMELINE_OPT     0x00001000


/*changes for F.E.R*/

#define GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_ON    0x00000001 //1-bit
#define GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_DELAY 0x00000030 //2-bits
#define GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_CYCLE 0x00000700 //3-bits
#define GL1_EFS_SLP_FORCE_ERROR_RECOVERY_REGULAR_INT_ACTIVITY 0x0000F000 //4-bits

//assuming only for dual-sim
#define GL1_HW_EFS_SLP_ENABLED_ERROR_REC( dEBUG_mASK,gas_id) \
   ( (gl1_hw_efs_get_slp_error_rec()>>(gas_id*16)) & ( dEBUG_mASK ) ) //as gas_id is 0/1
    
uint8 gl1_efs_slp_force_error_wakeup_activity_count[NUM_GERAN_DATA_SPACES] ;



#define NUM_OF_GL1_DEBUG_BURSTS         (4)
/*----------------------------------------------------------------------------
  MCPM state update data
----------------------------------------------------------------------------*/


typedef struct
{
  /* common state update start point, in ustmr counter units */
  ustmr_type                            start_ustmr;

  /* common state update end point, in ustmr counter units */
  ustmr_type                            end_ustmr;

  /* common state update execution duration, in ustmr counter units*/
  ustmr_type                            duration_ustmr;

  /* common state update execution duration, in us */
  ustmr_type                            duration_us;
}gsm_sleep_common_type;

/*----------------------------------------------------------------------------
  Sleep start data
----------------------------------------------------------------------------*/

typedef struct
{
  uint32                                go_to_sleep_frame_number;

  uint32                                wakeup_frame_number;

  gsm_sleep_common_type                    go_to_sleep;

  gsm_sleep_common_type                 deact_sleep;

  /* RF RX OFF data */
  gsm_sleep_common_type                 rf_rx_off;

  /* switching rf vreg off  - duration */
  gsm_sleep_common_type                 rf_vreg_off;
  /* MCPM state update - duration */
  gsm_sleep_common_type                    mcpm;
}
gsm_sleep_start_type;

/*----------------------------------------------------------------------------
  Delay data
----------------------------------------------------------------------------*/

typedef struct
{
  /* Programmed point for wakeup interrupt to run, in ustmr counter units */
  ustmr_type                            programmed_to_run;

  /* Actual point when wakeup interrupt has run, in ustmr counter units */
  ustmr_type                            actually_run;

  /* Delay of wakeup interrupt, in ustmr counter units */
  ustmr_type                            delay;

  /* Delay of wakeup interrupt, in us */
  ustmr_type                            delay_us;
}
gsm_sleep_wakeup_int_delay_type;

/*----------------------------------------------------------------------------
  Delay data
----------------------------------------------------------------------------*/

typedef struct
{
  /* Programmed point for ols interrupt to run, in ustmr counter units */
  ustmr_type                            programmed_to_run;

  /* Actual point when ols interrupt has run, in ustmr counter units */
  ustmr_type                            actually_run;

  /* Delay of ols interrupt, in ustmr counter units */
  ustmr_type                            delay;

  /* Delay of ols interrupt, in us */
  ustmr_type                            delay_us;

  /* Frame number */
  uint32           frame_number;
}
gsm_sleep_ols_int_delay_type;

/*----------------------------------------------------------------------------
  Sleep additional info
----------------------------------------------------------------------------*/

typedef struct
{
  /* Flag indicating scheduled/non-scheduled wakeup */
  boolean                               scheduled;

  /* RF warmup time, in microseconds */
  uint16                                returned_rf_warmup_usec;

#ifdef FEATURE_GSM_ADAPTIVE_TMLN
  uint16                                adaptive_warmup_us;
#endif /* FEATURE_GSM_ADAPTIVE_TMLN */

  /* Warmup duration, in microseconds */
  uint16                                used_warmup_usec;

  /* Sleep duration, in GSM clks */
  uint64                                gsm_clk_sleep_duration;

  /* late ols offset */
  uint32                                gsm_clk_sleep_duration_frames;

  /* egts offset */
  uint32                                egts_offset_gsm_clks;

  /* late ols offset */
  uint32                                late_ols_offset_usec;

  /* Flag indicating LMM */
  boolean                               low_monitor_mode;

  /* Flag indicating diag mask disabled */
  boolean                               diag_mask_disabled;

  /* Flag indicating diag f3 trace diasbled */
  boolean                               diag_f3_trace_disabled;

  /* Flag indicating warm up should be done in parallel mode */
  boolean                               parallel_warmup;

  /* Flag indicating warm up should in dsda mode */
  sys_modem_device_mode_e_type          msim_sys_mode;

  /* Total awake duration exc MCPM config */
  ustmr_type                            awake_duration_us;

  /* slack offset */
  int32                                 warmup_end_to_gstmr_us;

  /* slack offset */
  int32                                 rf_wakeup_to_vstmr_us;

  /* commit offset */
  int32                                 commit_gsmclks;

  /* pcycle count when committing. Useful for debugging preemption issues
     as it makes it easier to match T32 merge buffer log to code flow. */
  uint32                                commit_pcycles;

  /* time from enter_mode completed to OLS */
  int32                                 commit_enter_mode_to_ols_us;

  /* was an enter mode sent on this wakeup */
  boolean                               commit_enter_mode_this_wakeup;
}
gsm_sleep_info_type;

/*----------------------------------------------------------------------------
  GSTMR data
----------------------------------------------------------------------------*/

typedef struct
{
  /* GSTMR event start point, in timetick counter units */
  uint32                                frame_number;

  uint32                                frames_awake;

  ustmr_type                            time_ustmr;

  boolean                               next_tick_req;

  boolean                               active_task_count;

  uint32                                duration_us;
}
gsm_sleep_burst_info_type;


typedef struct
{
  uint32                                frame_number;
  ustmr_type                            start_ustmr;
  uint32                                start_qsc;
  boolean                               ignore_next_gstmr;
  boolean                               async_sleep;
  boolean                               async_exec;
  boolean                               async_poss;
  boolean                               async_NV_poss;
  boolean                               async_option3_NV_poss;
  boolean                               precheck;
  boolean                               int_pend;
  boolean                               irat_active;
  boolean                               tx_on;
  boolean                               saic_status;
}
gsm_sleep_async_type;
/*----------------------------------------------------------------------------
  Sleep wakeup data
----------------------------------------------------------------------------*/

typedef struct
{
  /* wakeup interrupt delay */
  gsm_sleep_wakeup_int_delay_type          wakeup_delay;

  /* duration for activities before MCPM, ie acquire locks, TRM etc */
  uint32                                   pre_mcpm_activities_us;

  /* MCPM state update duration */
  gsm_sleep_common_type                    mcpm;

  gsm_sleep_common_type                    awake_init;

  /* RF RX ON data */
  gsm_sleep_common_type                    rf_awake;

  /* RF RX data */
  gsm_sleep_common_type                    rf_rx_on;

  /* Wakeup ISR data */
  gsm_sleep_common_type                    isr;

  /* RF ENTER MODE execution time*/
  gsm_sleep_common_type                    rf_enter_mode;

  /* RF's estimate of time required for RF scripts to download over the SSBI bus */
  uint32                                   rf_script_dload_us;
  /* Time from receiving rf awake cnf to next GSTMR tick */
  int32                                    rf_rx_to_vstmr_us;
  /* Time from receiving rf awake cnf to next GSTMR tick */
  int32                                    rf_cnf_to_vstmr_us;

  gsm_sleep_common_type                    total_wakeup_time;
}gsm_sleep_wakeup_type;


typedef struct
{
  /* Sleep wakeup event start point, in timetick counter units */
  ustmr_type                        start_ustmr;

  /* Sleep wakeup event end point, in timetick counter units */
  ustmr_type                        end_ustmr;

  /* Sleep ols cb function execution duration, in sleep-clocks */
  ustmr_type                        duration_ustmr;

}gsm_sleep_recovery_wakeup_type;

/*----------------------------------------------------------------------------
  Sleep ending data
----------------------------------------------------------------------------*/

typedef struct
{
  /* ols interrupt delay */
  gsm_sleep_ols_int_delay_type          ols_delay;

  /* Sleep ols cb function start point, in timetick counter units */
  //ustmr_type                            start_ustmr;

  /* Sleep ols cb function end point, in timetick counter units */
  //ustmr_type                            end_ustmr;

  /* Sleep ols cb function execution duration, in sleep-clocks */
  //ustmr_type                            duration_ustmr;

  /* Sleep ols cb function execution duration, in us */
  //ustmr_type                            duration_us;
  gsm_sleep_common_type                 set_terminal_count;
}
gsm_sleep_ols_type;

/*----------------------------------------------------------------------------
  GL1 sleep profile data
----------------------------------------------------------------------------*/

typedef struct
{
  /* Indicates last populated cycle */
  boolean                               active;

  /* Indicates forced wakeup */
  boolean                               forced_wakeup;

  /* Sleep duration, warmup duration, egts, ols, interrupt delays */
  gsm_sleep_info_type                   sleep_info;

  /* Wakeup GSM data */
  gsm_sleep_wakeup_type                 wakeup;

    /* Recovery Wakeup GSM data */
  gsm_sleep_recovery_wakeup_type        rec_wakeup;

  /* End of sleep GSM data */
  gsm_sleep_ols_type                    ols;

  /* Details of each burst in  */
  gsm_sleep_burst_info_type             burst[NUM_OF_GL1_DEBUG_BURSTS];

  /* Async sleep command msg */
  gsm_sleep_async_type                  async_msg;

  /* Start of GSM sleep data */
  gsm_sleep_start_type                  start;

  /* EFS NV Items configuration*/
  uint32                        efs_debug_setting;
}
gl1_hw_sleep_type;

#define NUM_OF_GL1_DEBUG_CYCLES         (8)

extern uint16 gl1_sleep_cnt[];
extern gl1_hw_sleep_type gl1_hw_sleep_struct[NUM_GERAN_DATA_SPACES][NUM_OF_GL1_DEBUG_CYCLES];
void gl1_non_blocking_delay( uint16 delay,rex_sigs_type  wait_sig, rex_tcb_type *tcb_ptr);


/* Debug structure for wakeup cancellation */
typedef struct gl1_hw_sleep_wgmr_dbg_type
{
  boolean   cancel_successful;
  int32     gap;
  uint32    prev_wkpt_fn;
  uint32    new_wkpt_fn;
  uint32    wmgr_result_code;
} gl1_hw_sleep_wgmr_dbg_type;

extern uint16 gl1_hw_sleep_wgmr_dbg_cnt[NUM_GERAN_DATA_SPACES];
extern gl1_hw_sleep_wgmr_dbg_type gl1_hw_sleep_wgmr_dbg[NUM_GERAN_DATA_SPACES][NUM_OF_GL1_DEBUG_CYCLES];

void gl1_hw_sleep_rf_wakeup_end(gas_id_t gas_id);
void gl1_hw_sleep_debug_duration (gsm_sleep_common_type *p);
gl1_hw_sleep_type* gl1_hw_get_last_sleep_debug_entry(gas_id_t gas_id);

#ifdef FEATURE_GSM_ADAPTIVE_TMLN
/* Definitions used to log the decisions made to adjust adaptive tmln after
   each bank of samples is averaged. This makes it possible to determine from
   a crash dump if the tmln had settled or was increasing/decreasing. */
#define GL1_HW_TMLN_LOG_SIZE    (10)

/* Structure filled when gl1_hw_reset_adaptive_tmln_data_store() is called,
   explains why decision to inc/dec tmln was made */
typedef struct
{
  /* Value of first_time_use function parameter */
  boolean first_time_use;

  /* How many samples were logged in bank, will be less than
     ADAPTIVE_TMLN_SAMPLE_COUNT if error_cnt was too great */
  uint16  index;

  /* TMLN data used to make decision */
  uint16  error_cnt;
  uint32  current_warmup_time;
  int32   average_gap_to_ols;
  uint32  highest_CCS_Write_time_us; /* Highest ever estimate, effectively the likely worst-case */
  uint16  step_size;
}
gl1_hw_tmln_log_type;
#endif /* FEATURE_GSM_ADAPTIVE_TMLN */

#endif /* GL1_HW_SLEEP_CTL_H */

/* EOF */
