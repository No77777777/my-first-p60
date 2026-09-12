#ifndef WL1SLEEP_H
#define WL1SLEEP_H
/*===========================================================================
                 WCDMA L1 SLEEP MODULE

GENERAL DESCRIPTION
  This file contains the code for controlling the operation of WCDMA L1 SLEEP

Copyright (c) 2013 by Qualcomm Technologies, Inc.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/wl1sleep.h#4 $
$DateTime: 2020/01/22 08:56:27 $
$Author: pwbldsvc $

when       who    what, where, why
--------   ---    --------------------------------------------------------- 
01/10/20   ac      Mini dump framework checkin
06/14/18   pb     Removed redundant variable to track CTCH lock reserve/request duration
05/16/18   am     Timeline opts add wait before sending FW start if MCPM time reduced beyond 0.5 msec
03/26/18   sch     Changing entity back off time and maximum time to block sleep in min sleep opts
12/08/17   pb     API added to get cell sfn and cx8 while ue is in sleep.
01/18/17   pb     Support state information update to TRM 
09/21/16   kr      Changes to vote for APS ON/OFF in SR<----> DRD transitions
09/28/16   rs     Correcting Macro MIN_TIME_FOR_PICH_TO_DELAY_SLEEP_ABORT to reflect the correct time in ms
09/19/16   am      Setting delay_sleep_abort flag when PICH is less than 25ms away.
06/30/16   kr      QSH events for low PI SUM
06/10/16   skk/vn  Updating signal info params before going to sleep
06/07/16   skk    Backing out CR964907
05/13/16   skk    Handling FW/RF resources when sleep duration is less.
04/19/16   skk    Increase the max sleep duration from 6sec to 11sec to handle back to back wakeup cancels.
04/20/16   skk    QSH sleep handlers.
02/19/16   rs     Don�t  skip AGC for DIV chain in case of Early RF shutdown - part 2
01/21/16   rs     Idle Opts: trigger FW sleep and RF sleep in parallel
01/14/16   sks    Use correct SLPC ID for GSM for complete wakeu
01/13/16   nga    RnN for pending LTE searches
12/10/15   pv     Created API to post local cmd for reprogramming to Online PICH 
12/03/15   skk    API for CTCH addition in NON DRX.
11/18/15   skk    made num_qta_wakeups as uint32 
10/22/15   sg     Compilation error fix for wl1 qsh changes
10/12/15   kr     CLear RxD BCH variables when exiting BCH 
10/07/15   rs     CTCH lock management for DSDS
10/08/15   skk    Typecast handle_waking_up with correct enum type. 
10/07/15   vs     BSIC Id Reconf optimizations for X+W QTA scenarios.
09/25/15   kcm    RxD ACQ BCH handling in BCH inter sib sleep scenarios
09/20/15   sks    Do not wait for AGC tune done when tuning RF after an early RF shutdwon w/o early FW sleep
09/18/15   pv     Increase RF Warmup time for any non PICH entity by 2ms.
09/15/15   sad    Pend RRC's request and notify if we are close to a meas only gap
                  wakeup in order to avoid race condition
09/16/15   sks    Compilation error fixes for FEATURE DUAL SIM undefined.
08/26/15   sks    Code changes to wakeup early for the first DRX cycle in a QTA gap.
08/14/15   pv     New utility macros.
08/24/15   us     Updates for including rfm_path_type
08/21/15   abs    Backing out cxm based alt sub in data mods
08/13/15   skk    Query srchzz for correct band while cancelling wakeup for Split srch entity. 
07/18/15   kr     Dont disable outerloop timeline when GPS is getting active 
08/05/15   skk    Enable wakeup cancel always in L1M Sleep states. 
08/03/15   rs     Changes for SR-DR DSDS fallback mechanism 
07/23/15   pv     Variable to store the sc at SLPC start.
06/30/15   sks    Do not do a sleep abort for timetag request in any state.
07/13/15   sks/as Code changes for checking if alt sub is in a data call
7/9/15     kr     Set RF state to sleep from TASK context
07/02/15   sks    Do not execute the guard timer expiry call back if not in PCH SLEEP. 
6/28/15    kr     Make sure to call prep tune in a irat/interf  drx cycle
06/24/15   gsk    Adding a macro to check if UE is in a QTA gap
06/11/15   sad    RF state machine changes for handling meas device
06/07/15   kr     dont wait for agc done when waking up rf UNFREEZE cmd
06/02/15   sks    Minimizing RX_TX_SPLIT featurization
05/20/15   sks    Compilation error fix.
05/19/15   sks    Changes to support PICH and measurements in L/W/G + /G.
05/12/15   rs     Removing references to fast cell deconfig API's
05/07/15   skk    FR24494: BCH Inter SIB sleep initial check in.
05/07/15   rs     Rx-Tx Split changes
02/27/15   ap     Initial check-in for WL1 QSH framework 
02/28/15   rsr    W + W Phase 2 Checkin.
02/25/15   sks    Remove exit mode calls from multiple places and place it along with the RF/FW sleep in the FCDC flow.  
02/23/15   pv     New WL1 SLEEP MODE MANAGER support.
02/23/15   kr     call enter mode for dsds in case Rf is waking up from sleep
02/23/15   rs     Code changes to make sure wakeup searches are issued as HPQ searches
02/23/14   rs     Changes to make sure freq correction is applied before starting agc and also setting the 
                  wakeup_search_pending flag appropriately. 
02/19/15   sl     Ensure W Stack is up when RRC calls DL API "dl_get_cell_timing" 
01/19/15   pr     Created api is_sleep_drv_not_in_sleep to return the slp ctrl state.
12/1/14    skk    Add RF_WKUP_SLP mutex while putting RF to sleep in DPC task. 
02/19/15   kr     Reset last int from sleep controller in NO LOCK case such that 
                  When RRC queries WL1 for SFN it returns valid value
01/19/15   sks    Global re-org for W+W
01/14/15   sks    Log the time of prep wakeup done at the end of the call and not at power clock on.
11/25/14   sad    Change to handle modify activity failure
11/04/14   rs     Code changes for DRDSDS fallback mechanism - part 2
11/13/14   gsk    Fix for compilation error
11/11/14   psr    Moved a few externed variables to srchcrutils
11/03/14   rs     Code changes for DRDSDS fallback mechanism - part 2
10/10/14   skk    Reduced entity rf warmup time to 6ms from 7ms as sleep time is recalculated after RF/FW sleep.
10/14/14   rs     Code changes for DRDSDS fallback mechanism
06/20/14   skk    FR19218: ReacqLessPICH(RLP) porting for JO.
08/29/14   sks    Feature cleanup on BOLT.
08/04/14   rs     Fixed compiler warnings
07/03/14   sks    Changes for FCDC
05/29/14   sks    Combined RF wakeup and tune optimizations on BO.2.0
05/14/14   sks    IDLE Wakeup Optimizations and cleanup
01/16/13   sks    GSTMR changes for W2G, removing redundant featurizations, and removing instances of rf_wu_sc.
01/01/13   kr     making a variable externed
12/16/13   as     Porting G2W TA from Triton to Dime.
12/10/13   pv     FR 17754 - Wsleep API for STMR init at wakeup.
11/25/13   rs     Support for adjusting sleep setup time when RxD is enabled in DSDS.
11/20/13   rs     New algo to calculate sleep setup time when RxD enabled. 
11/14/13   pj     Wrapped HWIO access with WL1_HWIO macro
11/12/13   srk    Reducing the RF wakeup time to 5ms to cutdown channel wakeup time.
09/17/13   jkb    Porting Dual Sim from Triton to Dime
04/08/13   kr     Disabling access to USTMR registers while WL1 is asleep with FW suspended, to prevent watchdog bites.
03/30/13   pv     Support for handling reprogrammed micro wakeup int.
02/20/13   hk     Removed FEATURE_INTER_SIB_SLEEP
02/11/13   pv     New API for setting/getting cell/fing decfg status.
01/16/13   dm     Reduce CTCH awake time per DRX cycle.
11/28/12   pv     Fixed compiler warning.
09/17/12   pv     Externs for cell/fing decfg opt.
09/12/12   pv     Change the QTT config from 5*6 to 6*5 
08/29/12   pv     Remove unused variables and added API for PICH timeline opts.
08/23/12   pv     Externed a flag for wakeup srch. 
11/16/12   pv     Dime sleep support. 
08/15/12   gsk    Updating the RxAGC read with new RF api
07/24/12   geg    Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
03/28/12   zr     Added feature definitions for Dime
07/05/12   pv     Parallel reacq changes.
07/05/12   gsk    Idle mode FRs 1782/1705/1947/2152/1950
04/05/12   gsk    Added Rxd Idle profiling and optimization changes
01/30/12   vs     Feature cleanup.
10/03/11   pv     Externed the MCPM_STMR_BLOCK registered callback.
09/07/11   pv     Removed declarations for sleep timer 1.
07/08/11   saik   Mainlining FEATURE_WCDMA_8200_SLEEP
04/25/11   mc     SW decoupling
09/20/10   pv     Adjusted the ticks_rem adjust parameter for Genesis timeline optimization.
08/23/10   ks     Removed inclusion of mdsp header for Genesis (featurized)
06/23/10   saik   Reduced inner loop backoff by 1 ms. 
06/04/10   saik   Optimized 8200A sleep timeline.
05/20/09   rmak   Deleted sleep_drv_get_sc_mHz
05/12/09   rmak   Removed wsleep_relinquish_short_ticks and keypad_scan_keypad
05/05/09   rmak   Merged in content from sleepdrv.h and wsleep.h
09/16/08   mg     Checked in initial version of file

===========================================================================*/

/* ==========================================================================
**
** Feature Descriptions
**
** ========================================================================*/

/* -----------------------------------------------------------------------

    FEATURE_DETECT_STMR_SLP_CLK_MISMATCH  - Enabled in all targets

       This feature detects if the SFN goes out of sync with
       network SFN. The elapsed time in system clocks and
       in sleep clocks (reference) is calculated in every DRX cycle.
       Ideally both the elapsed values should be almost same.
       If there is a large difference then the SFN is corrected
       and RRC is nudged to read PCCPCH.


    FEATURE_MAINTAIN_GTIME_THROUGH_WSLEEP - Enabled in all targets

       This feature maintains GFN and GSM time during WCDMA drx cycles.
       Upon wakeup GFN and GSM time are slammed.


    FEATURE_SLEEP_TASK - Enabled in all targets

       This feature enables sleep task.


----------------------------------------------------------------------- */


/* ==========================================================================
**
** Header Includes
**
** ========================================================================*/

#include "wcdma_variation.h"
#include "comdef.h"
#include "customer.h"

#include "msm.h"

#include "wl1api.h"
#include "slpc.h"
#include "wl1drxmanager.h"

#if defined(FEATURE_MMGPS) ||        defined(FEATURE_CGPS_TIME_TAGGING)
#include "l1lsmif_v.h"
#endif /* FEATURE_MMGPS || FEATURE_MMGPS_WL1 || FEATURE_CGPS_TIME_TAGGING */

#include "l1msm.h"

#include "rxdiv_v.h"

#if defined (FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
#include "wmgr.h"
#endif /* defined (FEATURE_QSH_EVENT_NOTIFY_TO_QSH) */

/* ==========================================================================
**
** Constants
**
** ========================================================================*/

/* RF warmup time including rf_wakeup() in microseconds
 * this shdn't be > 65000 because its used as a word16 parameter
 */
#ifdef FEATURE_WCDMA_REACQLESS_PICH
#define DRX_PRLL_REACQ_TRIG_THRESH 5
#endif

#define MIN_TIME_TO_WAKEUP_FOR_ABORT               5
#define MIN_TIME_FOR_SLEEP_ABORT                  15
#define MIN_TIME_FOR_PICH_TO_DELAY_SLEEP_ABORT    (MIN_TIME_FOR_SLEEP_ABORT + (DRX_ENTITY_MIN_ENTITY_DIFF_USEC/1000))

/*
** Increasing the RF wakeup time to account TRM apis also in Min Sleep ops 
** Applicable only for TA
*/
#define RF_WAKE_UP_TIME_IN_MILLISECONDS_NON_EDRX WL1_IN_DS_MODE() ?  11 : 6
#define RF_WAKE_UP_TIME_IN_MILLISECONDS_EDRX     WL1_IN_DS_MODE() ?  2 : 0

#define RF_WAKE_UP_TIMER_IN_MS(entity_to_be_awake)  (entity_to_be_awake == DRX_ENTITY_EDRX) ? RF_WAKE_UP_TIME_IN_MILLISECONDS_EDRX : RF_WAKE_UP_TIME_IN_MILLISECONDS_NON_EDRX

#define RF_WAKE_UP_TIME_FOR_RXD_BCH 2
/* Value to ARM the ST_EVENT state machine */
#define STMR_ARM_ST_EVENT_ENA   (0x1 << WL1_HWIO_SHFT(STMR_ST_EVENT, CTL_ARM_ST_EVENT))

/* Send GO_TO_SLEEP signal when STMR_ST_EVENT occurs */
#define STMR_SLEEP_ARM_ENA      (0x1 << STMR_ST_EVENT_CTL_SLEEP_ARM_SHFT)

/* Capture system time value at ST_EVENT */
#define STMR_STATUS_DUMP_ST_EVENT_V     0x2

#define KEYSENSE_RD MPM_KEYSENSE_GROUP_STATUS
#define STMR_ST_EVENT_CTL_SLEEP_ARM_SHFT WL1_HWIO_RSHFT(STMR_ST_EVENT_CTL_SLEEP_ARM)
#define STMR_ST_EVENT_CTL_EVENT_TIME_SHFT                              \
        WL1_HWIO_RSHFT(STMR_ST_EVENT_CTL_EVENT_TIME)
#define STMR_REF_COUNT_STATUS_MICRO_FRAME_REF_COUNT_BMSK               \
        WL1_HWIO_FMSK(STMR_REF_COUNT_STATUS, MICRO_FRAME_REF_COUNT)

#define MDSP_C_qftEnThreshold MDSP_qftEnThreshold

/* Defintions for bit positions for the debug LED */
#define DEBUG_LED_1_POS 0x2  /* TCXO      */
#define DEBUG_LED_2_POS 0x4  /* DRX state */
#define DEBUG_LED_3_POS 0x8  /* INT       */
#define DEBUG_LED_4_POS 0x10 /* RF        */

/*------------------------------------------------------------*/
/*  Defintions used by the sleep controller SW                */
/*------------------------------------------------------------*/
#define ONE_MS_IN_CHIPS            (3840)
#define ONE_MS_IN_CX8              (30720)
#define ONE_MS_IN_STMR_TICKS       (15)
#define ONE_FRAME_IN_CHIPS         (38400)
#define ONE_SEC_IN_CX8             (30720000)
#define ONE_SEC_IN_CHIPS           (3840000)
#define ONE_SEC_IN_MS              (1000)
#define ONE_SEC_IN_SC              (32768)

#define WSLEEP_MAX_SLEEP_DURATION_SEC                16
#define WSLEEP_MAX_SLEEP_DURATION_IN_CX8             (WSLEEP_MAX_SLEEP_DURATION_SEC * ONE_SEC_IN_CX8)

#define SLEEP_CLK_IN_KHZ           (19200)
#define SLEEP_CLK_NOMINAL_mHZ      (32765442)
#define MIN_SLEEP_XTAL_MILLI_HZ    (30000000)
#define NOMINAL_FEE_NUM_SLEEP_CLKS (10000)
#define TEN_MS_IN_US               (10000) 
/* When a meas grant is received, SLPC has to be reprogrammed
   to start the meas gap. It is reprogrammed to fire 5 ms away. */
#define TIME_NEEDED_TO_REPROGRAM_SLPC_MS (5)

#define ONE_MS_IN_SC          (1 + SLEEP_CLK_NOMINAL_mHZ/1000000)
#define FIVE_MS_IN_SC         (1 + 5 * SLEEP_CLK_NOMINAL_mHZ/1000000)
#define TEN_MS_IN_SC          (1 + 10 * SLEEP_CLK_NOMINAL_mHZ/1000000)
#define M_BY_N_MS_IN_SC(m,n)  (1 + ((m) * (SLEEP_CLK_NOMINAL_mHZ/10000)/((n) * 100)))

#define SLPC_TICKS_TO_CX8(value)   (((value)/(float)SLEEP_CLK_IN_KHZ) * ONE_MS_IN_CX8)

#define WCDMA_QFT_INT_LENGTH_LONG               (30)
#define WCDMA_QFT_INT_LENGTH_SHORT              (15)

#define WCDMA_QFT_THRESHOLD_LONG                (2500000)
#define WCDMA_QFT_THRESHOLD_SHORT               (625000)

#define WCDMA_QTT_COH_INT_LENGTH_STTD_LONG      (4)
#define WCDMA_QTT_COH_INT_LENGTH_LONG           (6)

#define WCDMA_QTT_COH_INT_LENGTH_STTD_SHORT     (4)
#define WCDMA_QTT_COH_INT_LENGTH_SHORT          (5)

#define WCDMA_QTT_NON_COH_INT_LENGTH_STTD_LONG  (7)
#define WCDMA_QTT_NON_COH_INT_LENGTH_LONG       (5)

#define WCDMA_QTT_NON_COH_INT_LENGTH_STTD_SHORT (3)
#define WCDMA_QTT_NON_COH_INT_LENGTH_SHORT      (3)


#ifdef FEATURE_DUAL_SIM
/* Wakeupmgr cancellation algorithm support primitives */

#define DRX_ENABLE_WKUPMGR_CANCEL_SHFT 7
#define DRX_ENABLE_WKUPMGR_CANCEL_OPT_BMSK 0x80

#define DRX_WKUPMGR_CANCEL_FROM_SLEEP_START_INT_SHFT 0
#define DRX_WKUPMGR_CANCEL_FROM_SLEEP_START_INT_BMSK 0x01

#define DRX_WKUPMGR_CANCEL_IN_PROGRESS_SHFT 1
#define DRX_WKUPMGR_CANCEL_IN_PROGRESS_BMSK 0x02

#define DRX_WKUPMGR_SLEEP_START_INT_IN_PROGRESS_SHFT 2
#define DRX_WKUPMGR_SLEEP_START_INT_IN_PROGRESS_BMSK 0x04

extern boolean sleep_drv_wkup_cancel_pending;
#define WSLEEP_DRV_WAKEUP_CANCEL_PENDING() (wl1sleep_ext_cntrl_params[wsub_id].sleep_drv_wkup_cancel_pending)

#endif /* FEATURE_DUAL_SIM */


#define WSLEEP_SLPC_PROGRAMMED()         (wl1sleep_ext_cntrl_params[wsub_id].last_int_from_sleep_ctlr == SLEEP_CTLR_SLEEP_START_INT)

#define WL1SLEEP_FW_AWAKE()              (wl1sleep_ext_cntrl_params[wsub_id].wl1_fw_status == WSLEEP_FW_AWAKE)
#define WSLEEP_FW_SLEEP_IN_PROG()        (wl1sleep_ext_cntrl_params[wsub_id].wl1_fw_status == WSLEEP_FW_SLEEP_IN_PROGRESS)
#define WL1SLEEP_FW_ASLEEP()             (wl1sleep_ext_cntrl_params[wsub_id].wl1_fw_status == WSLEEP_FW_ASLEEP)
#define WSLEEP_FW_STATUS()               (wl1sleep_ext_cntrl_params[wsub_id].wl1_fw_status)

/* SIG used to wait for FW sleep done cmd */
#define WSLEEP_FW_SLEEP_DONE_SIG DRX_PRL_RQ_DL_WRKLP_DONE_SIG

#ifdef FEATURE_DUAL_WCDMA
#define WSLEEP_GET_OPTIMIZED_TIMELINE_CB()   (((wsub_id == WCDMA_SUB_ID_0) ? (wsleep_set_optimized_timeline_sub0):(wsleep_set_optimized_timeline_sub1)))
#define WSLEEP_GET_BLK_RESTORE_CB()          (((wsub_id == WCDMA_SUB_ID_0) ? (wsleep_ready_for_stmr_wall_time_setting_sub0):(wsleep_ready_for_stmr_wall_time_setting_sub1)))
#else
#define WSLEEP_GET_OPTIMIZED_TIMELINE_CB()   (wsleep_set_optimized_timeline_sub0)
#define WSLEEP_GET_BLK_RESTORE_CB()          (wsleep_ready_for_stmr_wall_time_setting_sub0)
#endif /* FEATURE_DUAL_WCDMA */

#define WSLEEP_CHECK_IF_IN_QTA_GAP()             ((wl1sleep_ext_cntrl_params[wsub_id].sleep_mode == WSLEEP_DUAL_SIM_QTA_MODE) || (wl1sleep_ext_cntrl_params[wsub_id].sleep_mode == WSLEEP_DUAL_SIM_QTA_MODE_NO_LOCK))
#define WSLEEP_CHECK_IF_NOT_IN_QTA_GAP()         (!WSLEEP_CHECK_IF_IN_QTA_GAP())
#define WAKEUP_BUFFER_FOR_IRAT_MS 10

#define WSLEEP_GET_SLEEP_START_TICK()            (wl1sleep_ext_cntrl_params[wsub_id].sleep_start_tick)
#define WLSEEP_GET_PROGRAMMED_SLP_TIME_IN_SC()   (wl1sleep_ext_cntrl_params[wsub_id].sleep_interval_written_sc)

#define WSLEEP_GET_TIME_TO_WKUP_ISR_USEC()       (wl1sleep_ext_cntrl_params[wsub_id].wl1sleep_time_to_wakeup_isr_usec)
#define WSLEEP_SET_TIME_TO_WKUP_ISR_USEC(val)    (wl1sleep_ext_cntrl_params[wsub_id].wl1sleep_time_to_wakeup_isr_usec = val)
/* ==========================================================================
**
** Macros
**
** ========================================================================*/

/*===========================================================================

MACRO SLEEP_DRV_PROGRAM_SLEEP_INTERVAL, and other macros

DESCRIPTION
    This macro programs SLEEP_INTERVAL register with sleep time specified in
    number of sleep cycles

PARAMETERS
    sleep_interval - Sleep duration specified in number of sleep clock cycles
    Sleep clock frequency is about 32 KHz

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
#define SLEEP_DRV_SET_ST_EVENT_STATUS_DUMP() \
    WL1_HWIO_OUT(STMR_STATUS_DUMP_CMD_MICRO, STMR_STATUS_DUMP_ST_EVENT_V)


/* ==========================================================================
**
** Type Definitions
**
** ========================================================================*/

typedef void (*isr_type)(void);

/* Definition of the states of the Sleep module state machine */
typedef enum
{
  /* Sleep module inactive; go_to_sleep() hasn't been called yet */
  SLEEP_INACTIVE,

  /* Sleep module is active; go_to_sleep has been called but RF isn't to be
   * put to sleep
   */
  SLEEP_MICRO_UP,

  /* RF is in sleep but micro is powered up (no consensus to power down) */
  SLEEP_RF_IN_SLEEP_MICRO_UP,

  /* RF in warm up (micro is up) */
  SLEEP_RF_IN_WARM_UP,

  /* In deep sleep.  RF sleeping.  CPU (and TCXO) are currently up, as required
   * by other subsystem(s).
   */
  SLEEP_RF_IN_SLEEP_MICRO_UP_IN_DEEP_SLEEP

} sleepModuleState_type;


/* Sleep duration in milliseconds */
typedef uint32 sleepDurationMillisec_type;

/* Call back function to wake up the protocol stack */
typedef void (*wakeUpCallBackFunction_type)(wsub_id_e_type wsub_id);

#ifdef DEBUG_WSLEEP_PROFILE_TIME_LINE
#error code not present
#endif

/*
 * Type definition for Sleep Contoller Interrupt.
 */
typedef enum
{
  SLEEP_CTLR_INT_NONE,
  SLEEP_CTLR_BEF_SLEEP_START,
  SLEEP_CTLR_SLEEP_START_INT,
  SLEEP_CTLR_MICRO_WAKE_UP_INT,
  SLEEP_CTLR_WU_1_ACT_TIME_0_INT
} sleep_ctlr_int_type;

typedef enum
{
  SLEEP_CTLR_INT_STATUS_ONLINE = 0x01,
  SLEEP_CTLR_INT_STATUS_SLEEP  = 0x02,
  SLEEP_CTLR_INT_STATUS_WARMUP = 0x04
} sleep_ctlr_int_status_type;

/* new state machine to track the FW awake status */
typedef enum
{
  WSLEEP_FW_ASLEEP,
  WSLEEP_FW_AWAKE,
  WSLEEP_FW_SLEEP_IN_PROGRESS
}wsleep_fw_status_enum_type;

typedef enum
{
  /*Single Sim mode/DUAL SIM DUAL ACTIVE.
  * RF/FW APIs/call flow is different from other modes.*/
  WSLEEP_NORMAL_MODE,
  /* DUAL SIM MODE - RF APIs called are different.*/
  WSLEEP_DUAL_SIM_MODE_WITH_LOCK,
  /* No TRM lock, so no RF/FW calls.*/
  WSLEEP_DUAL_SIM_MODE_NO_LOCK,
  /* X is G/W/L. No direct RF API calls can be made.*/
  WSLEEP_DUAL_SIM_QTA_MODE, 
  /* QTA mode but with no lock, go to sleep can be made via DS no lock code path.
     This will be needed for preventing BPLMN, and not incrementing req and
     notify counter ( until new CXM API can be used ).*/
  WSLEEP_DUAL_SIM_QTA_MODE_NO_LOCK,
  /* Total number of sleep modes.*/
  WSLEEP_NUM_SLEEP_MODES
}wsleep_mode_type;

/* This enum is to keep track of whether FW is to be started or not in any wakeup. 
   No FW wakeup for an IRAT gap owing to NULL->X tech scripting.
*/
typedef enum
{
   /* signifies wakeup is on a W band - PICH/intra f/inter f band*/
   W_BAND,
   /* signifies wakeup is on a non W band - G*/
   G_BAND,
   /* signifies wakeup is on a non W band - L*/
   L_BAND
   
}wsleep_wakeup_reason;

typedef enum
{
  /* curr mode is SR */
  WSLEEP_SR_MODE,
  /* curr mode is DR */
  WSLEEP_DR_MODE,
  /* curr mode is PROBE mode */
  WSLEEP_PROBE_MODE,
  /* Invalid mode*/
  WSLEEP_MAX_MODE
}wsleep_wakeup_mode;

typedef enum
{
  /*Very less sleep time, stay awake (< 8ms).*/
  WSLEEP_STAY_AWAKE,
  /*Only RF/FW sleep but no XO shutdown 
    and slpc programming, 8 - 38ms*/
  WSLEEP_BLOCK_XO_SLEEP,
  /*Trigger full sleep. > 38ms*/
  WSLEEP_ALLOW_XO_SLEEP,
  /*Invalid Sleep type.*/
  WSLEEP_MAX_SLEEP_TYPES
}wsleep_sleep_type;

/* ==========================================================================
**
** Global Variable Definitions
**
** ========================================================================*/

/* ==========================================================================
**
** Function declarations
**
** ========================================================================*/

/* Function pointer definition for Sleep mode true sleep proc type.*/
typedef wsleep_sleep_type (*wsleep_true_sleep_proc_mode_type)
(
  uint32 ms_to_sleep,
  drx_entity_handle_type handle_waking_up,
  boolean *bplmn_started,
  wsub_id_e_type wsub_id 
);


/* Function pointer definition for RFM PREP OPS MODE handler.*/
typedef boolean (*wsleep_rfm_prep_ops_mode_type)
(
  wl1_rfm_wakeup_chain_type rf_wakeup_type,
  wsub_id_e_type wsub_id
);


extern uint32 sleep_drv_mult_div(uint32 a, uint32 b, uint32 c,wsub_id_e_type wsub_id);
extern uint32 diff_sfn_sfn(uint32 sfn, uint32 comb_cnt_cx8,
                           uint32 sfn2, uint32 comb_cnt_cx8_2);

/*===========================================================================

FUNCTION     WSLEEP_INIT

DESCRIPTION This function is called to initialize the WCDMA Sleep module,
  and to register a call back function for wake up.
  See further description in the sleep.c file.

DEPENDENCIES
  None

RETURN VALUE
  True if all went well; False otherwise.

SIDE EFFECTS

===========================================================================*/
extern boolean wsleep_init(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WSLEEP_ADJ_SLEEP_CLK_FREQ

DESCRIPTION This function conditionally calls a sleep driver function to trigger
computation of sleep clk frequency.

DEPENDENCIES
  None

RETURN VALUE
   None
SIDE EFFECTS

===========================================================================*/
extern void wsleep_adj_sleep_clk_freq( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WSLEEP_FEE_IS_NEEDED

DESCRIPTION This function returns whether FEE is needed.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  TRUE if FEE is required. FALSE otherwise.

SIDE EFFECTS

===========================================================================*/
extern boolean wsleep_fee_is_needed(void);

/*===========================================================================

FUNCTION     WSLEEP_FEE_IS_POSSIBLE

DESCRIPTION This function returns whether FEE will complete within the next sleep
            of duration avail_msec.

DEPENDENCIES
  None

PARAMETERS
  avail_msec - Duration of the next sleep period in msec.

RETURN VALUE
  TRUE if FEE is possible during this sleep period. FALSE otherwise.

SIDE EFFECTS

===========================================================================*/
extern boolean wsleep_fee_is_possible(uint16 avail_msec);

/*===========================================================================

FUNCTION       SLEEP_ABORT_SUB0

DESCRIPTION
  This function aborts a sleep in progress. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void sleep_abort( void);

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
extern void sleep_abort_for_sub( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     EXIT_SLEEP_CYCLE

DESCRIPTION This function is called to exit sleep at the end of a drx cycle.
It clears some signals and cleans up the module and prints some stats.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void exit_sleep_cycle( wsub_id_e_type wsub_id);



/*===========================================================================

FUNCTION     EXIT_SLEEP_MODE

DESCRIPTION This function is called to turn required clock regimes for demodulation.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void exit_sleep_mode(void);



/*===========================================================================

FUNCTION     WSLEEP_DRX_RELEASED

DESCRIPTION This function is called when DRX mode is terminated.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void wsleep_drx_released( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SLEEP_PROCESS_TIMER_SIG

DESCRIPTION This function is called to process the timer signal
received by the sleep task

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void sleep_process_timer_sig( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION     WSLEEP_READY_FOR_WAKE_UP

DESCRIPTION This function checks for conditions that need to be satisfied for
     reacquisition search to proceed (ref counter should be slammed at on-line-start).


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
extern void wsleep_ready_for_wake_up(void);

#ifdef DEBUG_WSLEEP_PROFILE_TIME_LINE
#error code not present
#else
#define sleep_drv_profile_point(a) /* do nothing */
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
extern void sleep_drv_init( wsub_id_e_type wsub_id);

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
extern void sleep_drv_setup
(
  uint32                         sleep_period_ms,
  boolean                        wake_up_for_ctch
, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION GET_NUM_SLEEP_CLKS_FOR_FEE

DESCRIPTION
    This function returns the number of sleep clks required to run SLEEP_FEE
    based on the length of the current DRX cycle.

DEPENDENCIES
    None

RETURN VALUE
    uint32 - Returns the number of sleep clks for SLEEP_FEE

SIDE EFFECTS
    None

===========================================================================*/
extern uint32 get_num_sleep_clks_for_fee(void);

/*===========================================================================

FUNCTION SLEEP_DRV_RUN_FEE

DESCRIPTION
    This function initiates an FEE (frequency error estimation) to estimate the
    sleep xtal frequency currently.

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
extern void sleep_drv_run_fee(void);

/*===========================================================================
FUNCTION        SLEEPDRV_ADJ_SLEEP_CLK_FREQ

DESCRIPTION     This function implements the phase 2 sleep clk calibration algorithm.

DEPENDENCIES

SIDE EFFECTS    None
===========================================================================*/
extern void sleepdrv_adj_sleep_clk_freq( wsub_id_e_type wsub_id);

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
extern void sleep_drv_abort( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION IS_SLEEP_DRV_NOT_IN_SLEEP

DESCRIPTION
    This function returns TRUE or FALSE depending on the last interrupt 
    received from the sleep controller. Sleep driver is supposed to be
    awake if the last interrupt is SLEEP_CTLR_INT_NONE

DEPENDENCIES
    None

RETURN VALUE
    TRUE : if last interrupt is SLEEP_CTLR_INT_NONE
    FALSE: all other interrupts other than SLEEP_CTLR_INT_NONE

SIDE EFFECTS
    None

===========================================================================*/
extern boolean is_sleep_drv_not_in_sleep( wsub_id_e_type wsub_id);

#if defined(FEATURE_MMGPS) ||        defined(FEATURE_CGPS_TIME_TAGGING)
typedef struct
{
  sleep_ctlr_int_type last_int_from_sleep_ctlr;

  uint32 coarse_time;

  uint32 fine_time;

} wl1_slp_counters_type;

typedef struct
{
  uint16 freq;
  uint16 psc;
  
  wl1_cell_time_type  cell_time;

  uint8  is_valid_flag;

} wl1_cell_time_info_type;

typedef struct
{
  sleep_ctlr_int_type last_int_from_sleep_ctlr;
  
  boolean went_to_sleep_without_rf;
  #ifdef FEATURE_MAINTAIN_GTIME_THROUGH_WSLEEP
  boolean gsm_clk_enabled;
  #endif
/*timeline related globals */
  boolean drx_pich_opts_status;
  /* Set to false to disable sleep task from powering down TCXO */
  /* number of frames for which we haven't calibrated the sleep clk */
  uint32 num_frames_sleep_clk_uncalibrated;
  uint32 rf_wu_Cx8;
  uint32 sleep_interval_written_sc;
  uint32 sleep_interval_Cx8;
  boolean wsleep_early_wakeup_last_cycle;
  uint32 early_wakeup_delta_bpgs;
  uint32 wl1sleep_rf_timetaken_prev;
  uint32 num_sleep_ctlr_cycles;
  boolean sleep_drv_multiple_int;
  boolean rf_sleep_dpc_needed;
  boolean wsleep_trm_owned_gps ;
  /*
 * Indicates whether in good or bad DL conditions (vs CPICH EcIo -8 db)
 * so that we can sleep longer
 */
  boolean short_qxt;

  boolean wsleep_wakeup_srch_pending;

  boolean sleep_drv_force_abort;

  #ifdef FEATURE_DUAL_SIM
  uint8 drx_wkupmgr_cancel_bmsk;
  boolean sleep_drv_wkup_cancel_pending;
  #endif

  wsleep_fw_status_enum_type wl1_fw_status;
  #ifdef FEATURE_WCDMA_REACQLESS_PICH
  int32 adjust_ticks_rem_for_rlp;
  #endif
  uint32 wl1sleep_rf_timetaken_cur;
  uint32 rf_wakeup_profile_stop_time_outerloop;  
  uint32 rf_wakeup_profile_start_time_outerloop;
  uint32 rpm_delay_Cx8;
  /* Variable to track if the WCDMA TECH will be using optimized/extended awake timeline.*/
  boolean wsleep_optimized_timeline;

  wsleep_wakeup_reason wakeup_type;

  wsleep_wakeup_reason curr_wakeup_type;

  boolean wsleep_open_meas_gap;

  wsleep_mode_type sleep_mode;  

  timetick_type sleep_start_tick;

  uint32 num_qta_wakeups;

  boolean is_wait_for_agc_needed;

  /*will have the info of how early entity timer expired when compared to wakeup ISR.*/
  uint32 wl1sleep_time_to_wakeup_isr_usec;

  uint8 bsic_reconf_time;
    
  boolean embms_active_on_lte;

  boolean embms_active_on_lte_at_sleep;

}wl1sleep_ext_cntrl_params_type;

extern wl1sleep_ext_cntrl_params_type wl1sleep_ext_cntrl_params[WCDMA_NUM_SUBS];

#define WL1SLEEP_ASSIGN_WAKEUP_TYPE(val)  (wl1sleep_ext_cntrl_params[wsub_id].wakeup_type = val) 
#define WL1SLEEP_GET_WAKEUP_TYPE()        (wl1sleep_ext_cntrl_params[wsub_id].wakeup_type)

#define WL1SLEEP_BACKUP_WAKEUP_TYPE()          (wl1sleep_ext_cntrl_params[wsub_id].curr_wakeup_type = wl1sleep_ext_cntrl_params[wsub_id].wakeup_type)
#define WL1SLEEP_GET_BACKEDUP_WAKEUP_TYPE()        (wl1sleep_ext_cntrl_params[wsub_id].curr_wakeup_type)

#define WL1SLEEP_MEAS_GAP_ALLOWED(val)    (wl1sleep_ext_cntrl_params[wsub_id].wsleep_open_meas_gap = val)
#define WL1SLEEP_IS_MEAS_GAP_ALLOWED()    (wl1sleep_ext_cntrl_params[wsub_id].wsleep_open_meas_gap)
#define WL1SLEEP_RET_NUM_QTA_WAKEUPS()    (wl1sleep_ext_cntrl_params[wsub_id].num_qta_wakeups)
#define WL1SLEEP_INC_NUM_QTA_WAKEUPS()    (wl1sleep_ext_cntrl_params[wsub_id].num_qta_wakeups++)
#define WL1SLEEP_SET_AGC_WAIT_NEEDED(val) (wl1sleep_ext_cntrl_params[wsub_id].is_wait_for_agc_needed = val)
#define WL1SLEEP_IS_AGC_WAIT_NEEDED()     (wl1sleep_ext_cntrl_params[wsub_id].is_wait_for_agc_needed)

/*===========================================================================
FUNCTION        GET_CURRENT_CELL_TIME_VALUES

DESCRIPTION     Returns the cell time (wl1_cell_time_type).

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void get_current_cell_time_values (wl1_slp_counters_type  *wl1_cell_time_values, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        COMPUTE_CURRENT_CELL_TIME

DESCRIPTION     Returns the cell time (wl1_cell_time_info_type).

DEPENDENCIES    None

RETURN VALUE:   Returns whether a sleep timetag was performed.
                TRUE:  Sleep Timetag
                FALSE: Not a Sleep Timetag

SIDE EFFECTS    None
===========================================================================*/
extern boolean compute_current_cell_time(wl1_cell_time_info_type *info,
                               const wl1_slp_counters_type *cell_time_values, boolean allow_sleep_timetag, wsub_id_e_type wsub_id);
#endif /* FEATURE_MMGPS || FEATURE_MMGPS_WL1 || FEATURE_CGPS_TIME_TAGGING */


/*--------------------------------------------------------------------------
 * The following macros program WCDMA Modem Sleep Controller registers
 * Modem Sleep Controller is used in MSM 6260 and above
 *--------------------------------------------------------------------------*/

/* Function prototypes */
extern uint32 sleepdrv_get_coarse_time(void);
extern uint16 sleepdrv_get_fine_time(void);
extern uint16 sleepdrv_get_sleep_timer_1_count(void);
extern uint32 sleepdrv_get_programmed_wakeup_time(void);
extern boolean sleepdrv_set_sleep_timer_1_count(uint32 sclks);
extern sleep_ctlr_int_status_type sleepdrv_get_sleep_int_status( wsub_id_e_type wsub_id);
extern void wsleep_uninstall_wakeup_isr(wsub_id_e_type wsub_id);

/* The second variable to the functions sleepctl_set_warmup and sleepctl_set_warmup_no_wakeup
   is the wakeup time in ms. Convert tcxo_pll_wu_Cx8 to ms */
#define SLEEP_DRV_SET_SLP_CTL_SLEEP_INTERVAL()                                        \
      {                                                                             \
        sleepctl_set_warmup(SLEEPCTL_WCDMA,                                         \
                            (wl1sleep_ext_cntrl_params[wsub_id].rf_wu_Cx8 * ONE_SEC_IN_MS)/ONE_MS_IN_CX8);  \
        sleepctl_set_duration_sys_clk(SLEEPCTL_WCDMA,                               \
                                      (wl1sleep_ext_cntrl_params[wsub_id].sleep_interval_Cx8 + wl1sleep_ext_cntrl_params[wsub_id].rf_wu_Cx8)); \
      }

#define WSLEEP_GET_SLPC_ID() ((wsub_id == WCDMA_SUB_ID_0) ? (SLPC_WCDMA):(SLPC_WCDMA2))
#define WSLEEP_GET_G_SLPC_ID() ((wsub_id == WCDMA_SUB_ID_0) ? (SLPC_GSM):(SLPC_GSM2))

/*===========================================================================
FUNCTION        GET_PICH_CC_CX8

DESCRIPTION     This function returns the combiner counter associated with
                the hardware channel carrying PICH.

DEPENDENCIES    returns the value from the last STMR status dump, NOT the
                dynamic value.

SIDE EFFECTS    None
===========================================================================*/
extern uint32 get_pich_cc_cx8( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SLEEP_DRV_EXIT

DESCRIPTION     This function resets various parameters to their original values.

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void sleep_drv_exit( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SLEEP_DRV_PREPARE_FOR_GTIME_SNAPSHOT

DESCRIPTION     This function sets up in preparation for saving GSM Time information.

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void sleep_drv_prepare_for_gtime_snapshot( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SLEEP_DRV_GTIME_SNAPSHOT

DESCRIPTION     This function snapshots and saves the GSM time at the time of going to sleeep.

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void sleep_drv_gtime_snapshot(void);

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
extern void sleep_drv_stmr_status_dump(void);

/*===========================================================================
FUNCTION        WRITE_SHORT/LONG_QXT_PARAMS

DESCRIPTION     These functions write required parameters for QXT

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void write_long_qxt_params( wsub_id_e_type wsub_id);
extern void write_short_qxt_params( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        WSLEEP_SET_BIT/WSLEEP_CLEAR_BIT

DESCRIPTION     These functions set and clear bits of time test port

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern void wsleep_set_bit(uint32 bit_pos);
extern void wsleep_clear_bit(uint32 bit_pos);

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
extern void sleep_drv_compute_gfn_gqsym_at_ols( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        SLEEP_DRV_ABORT_TIMEOUT_HANDLED

DESCRIPTION     If the action time and on-line-start interrupts haven't arrived,
                call them, and return TRUE; otherwise return FALSE.
                It is expected that this function be called only up on
                sleep abort processing delayed detection.

DEPENDENCIES    None

SIDE EFFECTS    None
===========================================================================*/
extern boolean sleep_drv_abort_timeout_handled(void);

/*===========================================================================
FUNCTION        SLEEP_DRV_GET_SC_REM_TO_PICH

DESCRIPTION     Returns time in Sleep-Clocks from now to PICH
                This should be used before PICH in a DRX cycle

DEPENDENCIES    None

RETURN VALUE    int32

SIDE EFFECTS    None
===========================================================================*/
extern int32 sleep_drv_get_sc_rem_to_pich( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SLEEP_DRV_GET_SC_SINCE_PICH

DESCRIPTION     Returns time in Sleep-Clocks since PICH till now
                This should be used after PICH in a DRX cycle

DEPENDENCIES    None

RETURN VALUE    int32

SIDE EFFECTS    None
===========================================================================*/
extern int32 sleep_drv_get_sc_since_pich( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SLEEP_DRV_CACHE_SC_AT_PICH

DESCRIPTION     Caches the sc at pich so the sleep_drv_get_sc_since_pich uses current
                cycle. This should be called after PICH and before sc for
                next PICH is calculated

DEPENDENCIES    None

RETURN VALUE    void

SIDE EFFECTS    None
===========================================================================*/
extern void sleep_drv_cache_sc_at_pich( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SLEEP_DRV_GET_SC_REM_TO_PICH_FOR_F3

DESCRIPTION     Returns time in Sleep-Clocks from now to PICH
                This should be used before PICH in a DRX cycle
                Will return 0 if tcxo_was_shut_down to avoid overhead

DEPENDENCIES    sleep_drv_get_sc_rem_to_pich

RETURN VALUE    int32

SIDE EFFECTS    None
===========================================================================*/
extern int32 sleep_drv_get_sc_rem_to_pich_for_f3( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SLEEP_DRV_GET_SC_SINCE_PICH_FOR_F3

DESCRIPTION     Returns time in Sleep-Clocks since PICH till now
                This should be used after PICH in a DRX cycle
                Will return 0 if tcxo_was_shut_down to avoid overhead

DEPENDENCIES    sleep_drv_get_sc_since_pich and sleep_drv_cache_sc_at_pich

RETURN VALUE    int32

SIDE EFFECTS    None
===========================================================================*/
extern int32 sleep_drv_get_sc_since_pich_for_f3( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION WSLEEP_READY_FOR_STMR_WALL_TIME_SETTLING

DESCRIPTION
  Callback registered with MCPM for setting STMR slam event.This is also called directly
  if it happens we are not sending WAKEUP_REQ to MCPM during wakeup.

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
extern void wsleep_ready_for_stmr_wall_time_setting(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WSLEEP_READY_FOR_STMR_WALL_TIME_SETTING_SUB0

DESCRIPTION This is a wrapper Function for SUB0 

DEPENDENCIES
  MCPM.

RETURN VALUE
  None.  

SIDE EFFECTS
===========================================================================*/
extern void wsleep_ready_for_stmr_wall_time_setting_sub0(void);

/*===========================================================================
FUNCTION     WSLEEP_READY_FOR_STMR_WALL_TIME_SETTING_SUB1

DESCRIPTION This is a wrapper Function for SUB1 

DEPENDENCIES
  MCPM.

RETURN VALUE
  None.  

SIDE EFFECTS
===========================================================================*/
extern void wsleep_ready_for_stmr_wall_time_setting_sub1(void);

/*===========================================================================
FUNCTION wsleep_stmr_wall_time_cb

DESCRIPTION
  wrapper to call ready_for_stmr_wall_time_setting from inside the srch clock change callback.

DEPENDENCIES
    None

RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/

extern void wsleep_stmr_wall_time_cb( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WSLEEP_SET_OPTIMIZED_TIMELINE_SUB0

DESCRIPTION This function is the callback registered with MCPM to be called for 
notifying if WCDMA should use optimized/extended awake timeline. Accordingly,
the warmup time will be changed.

DEPENDENCIES
  MCPM.

RETURN VALUE
  None.  

SIDE EFFECTS
===========================================================================*/
extern void wsleep_set_optimized_timeline_sub0(boolean optimized_timeline);

/*===========================================================================
FUNCTION     WSLEEP_SET_OPTIMIZED_TIMELINE_SUB1

DESCRIPTION This function is the callback registered with MCPM to be called for 
notifying if WCDMA should use optimized/extended awake timeline. Accordingly,
the warmup time will be changed.

DEPENDENCIES
  MCPM.

RETURN VALUE
  None.  

SIDE EFFECTS
===========================================================================*/
extern void wsleep_set_optimized_timeline_sub1(boolean optimized_timeline);

/*===========================================================================
FUNCTION     WSLEEP_SET_WARMUP_TIME

DESCRIPTION This function is the callback registered with MCPM to be called for 
notifying if WCDMA should use optimized/extended awake timeline. Accordingly,
the warmup time will be changed.

DEPENDENCIES
  MCPM.

RETURN VALUE
  None.  

SIDE EFFECTS
===========================================================================*/
extern void wsleep_set_warmup_time(boolean rude_wakeup, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_SLEEP_SETUP_OPTS

DESCRIPTION This function sets up the DRX optimizations.

DEPENDENCIES
  None.

RETURN VALUE
  None.  

SIDE EFFECTS
===========================================================================*/
extern void drx_sleep_setup_opts( wsub_id_e_type wsub_id);

extern void wsleep_set_cell_fing_decfg_status(boolean decfg_done, wsub_id_e_type wsub_id);

extern boolean wsleep_get_cell_fing_decfg_status( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WSLEEP_POST_RFM_PREP_LOCAL_CMD

DESCRIPTION
  This function posts local cmd to do RFM PREP wakeup in L1 task context.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_post_rfm_prep_local_cmd(wl1_rfm_wakeup_chain_type wkup_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       wsleep_rfm_power_clock_on_at_wakeup

DESCRIPTION     This function calls rfm_power_clock_on() if not in DS mode.

DEPENDENCIES    Called from wakeup callback in L1 context after clocks are enabled.

SIDE EFFECTS    None.
===========================================================================*/
extern void wsleep_rfm_power_clock_on_at_wakeup(wl1_rfm_wakeup_chain_type wkup_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WSLEEP_RFM_PREP_OPS

DESCRIPTION
  This function pretunss to the required frequency and then calls RF API
  to build scripts through RFM PREP API, for the primary chain.
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_rfm_prep_ops(wl1_rfm_wakeup_chain_type rf_wakeup_type, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION SLEEP_COMPUTE_NEW_SLP_SETUP_TIME

DESCRIPTION
    Function used to compute sleep setup time in case of FULL DIVERSITY this cycle.
    Takes into account W2G client status to compute the new setup time.
    
DEPENDENCIES
    None
    
PARAMETERS
    None

RETURN VALUE
    None

SIDE EFFECTS
    None
===========================================================================*/
void sleep_compute_new_slp_setup_time( wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION WSLEEP_PROCESS_WAKEUP_CANCEL_CMD

DESCRIPTION
  Local cmd to offload wkupmgr cancallation processing to WL1 task from
  the cancellation callback.
    
DEPENDENCIES
    None
    
RETURN VALUE
    None

SIDE EFFECTS
    None

===========================================================================*/
extern void wsleep_process_wakeup_cancel_cmd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     DRX_PICH_OPTS_CHK

DESCRIPTION
  This function checks all the conditions for enabling pich opts.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  True - PICH opts can be enabled.
  False - Shouldnt be enabled.
===========================================================================*/
extern boolean drx_pich_opts_chk( wsub_id_e_type wsub_id);

#endif /*FEATURE_DUAL_SIM*/

/*===========================================================================
FUNCTION     WSLEEP_FW_SLEEP_SUSPEND

DESCRIPTION This function puts FW to sleep and suspend and informs drxofflineprocmgr. 

DEPENDENCIES
  None

PARAMETERS
  

RETURN VALUE
  boolean.

SIDE EFFECTS

===========================================================================*/
extern void wsleep_fw_sleep_suspend(boolean cool_down_opts, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WSLEEP_COMPUTE_NEXT_RXD_MODE

DESCRIPTION
  Wrapper function that calls RxD API to compute diversity mode in the
  next drx cycle.
  Diversity mode can be No Diversity or Full diversity
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
void wsleep_compute_next_rxd_mode(void);

/*===========================================================================

FUNCTION     wsleep_early_rf_fw_sleep

DESCRIPTION 
  API used to trigger RF and FW sleep one after the other. This can be called 
  in DPC or L1 context

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_early_rf_fw_sleep(uint32  sub_id);

/*===========================================================================

FUNCTION     WSLEEP_FW_START

DESCRIPTION 
  API used to send start command to firmware.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wsleep_fw_start( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WSLEEP_WAIT_FOR_FW_SLP_DONE

DESCRIPTION 
  This API is used to wait for fw response to sleep cmd. Once FW response is
  received FW app is suspended.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wsleep_wait_for_fw_slp_done( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WSLEEP_SET_FW_SLEEP_STATUS

DESCRIPTION 
  API used to set the current FW state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_set_fw_sleep_status(wsleep_fw_status_enum_type fw_status, wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===========================================================================
FUNCTION     SET_XO_SLAM_FLAG

DESCRIPTION This function resets the WFW XO slam flag.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void set_xo_slam_flag(boolean val, wsub_id_e_type wsub_id );

/*===========================================================================

FUNCTION     WL1_GET_CHANNEL_INFO

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_get_channel_info_for_trm_reserve_request(wrm_freq_input_type *band_res_req_info, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     WSLEEP_CHK_RX_MODE_MODE_AT_NXT_WKUP

DESCRIPTION 
   This function determines whether next wakeup should be HYBRID mode or
   DR DSDS mode
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_chk_rx_mode_at_nxt_wkup( wsub_id_e_type wsub_id);

#endif /* DUAL_SIM*/

extern void wl1sleep_ext_cntrl_params_init( wsub_id_e_type wsub_id);

extern void wl1sleep_int_cntrl_params_init( wsub_id_e_type wsub_id);

extern void wl1sleep_global_params_init(void);

/*===========================================================================
FUNCTION     wsleep_get_Wclk_status

DESCRIPTION 
   Checks if WSTACK & CLOCKs are up
 
DEPENDENCIES
  None

RETURN VALUE
  TRUE if W stack is up & FALSE otherwise

SIDE EFFECTS
  none
===========================================================================*/

extern boolean wsleep_get_Wclk_status(wsub_id_e_type wsub_id);

/*===================================================================================
FUNCTION     WSLEEP_HANDLE_SLEEP_FAILURE

DESCRIPTION
  This function takes care of all proceedures needed for handling sleep failure case.
  This might include unfreezing of sample server, handling pending RRC CMD for call est,
  handling pending sleep abort.
 
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.
======================================================================================*/
extern void wsleep_handle_sleep_failure(wsleep_sleep_type sleep_type, boolean bplmn_started,wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WSLEEP_PROCESS_SLP_OPS

DESCRIPTION 
   This function processes all the sleep operations to put the UE to true sleep.
   This invokes the handler corresponding to 
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none
=============================================================================*/
extern boolean wsleep_process_true_slp_ops(uint32 ms_to_sleep,
                                           drx_entity_handle_type handle_waking_up, 
                                           boolean *bplmn_started,
                                           wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WSLEEP_RFM_WAKEUP_WRAPPER

DESCRIPTION
  Wrapper function for waking up RF. This function also takes care of calling
  agc ttl init to apply the freq error before starting AGC. 
  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_rfm_wakeup_wrapper(wl1_rfm_wakeup_chain_type chain, 
                                      MDSP_WCDMA_L1_EVENT_CB_FUNC_TYPE *cb_func,
                                      boolean call_enter_mode,
                                      boolean prep_needed,
                                      boolean wait_for_agc_tune,
                                      wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
/*===========================================================================
FUNCTION     wsleep_exit_mode_op

DESCRIPTION 
   Checks a couple of conditions and calls exit mode on the PRx
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none
===========================================================================*/
extern void wl1_exit_mode_op(rfm_path_type rfm_path, wsub_id_e_type wsub_id);
#else

/*===========================================================================
FUNCTION     wsleep_exit_mode_op

DESCRIPTION 
   Checks a couple of conditions and calls exit mode on the PRx
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  none
===========================================================================*/

extern void wl1_exit_mode_op(uint16 device_id, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION  WSLEEP_GET_RF_FREQ 
 
DESCRIPTION
  This function gives the RF frequency that we have to use to tune and for other
  TRM reservations/requests etc in Idle Mode.
  
DEPENDENCIES
  None
  
RETURN VALUE
  uint16 freq
 
SIDE EFFECTS
  None
===========================================================================*/
uint16 wsleep_get_rf_freq(wsub_id_e_type wsub_id);
/*===================================================================================
FUNCTION     WSLEEP_POPULATE_PENDING_BANDS 

DESCRIPTION
  This function obtains the pending bands to be measured from srchzz
  and also obtains the RAT type for the measurements.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  No  
 
======================================================================================*/

extern void wl1sleep_populate_pending_bands(wsleep_wakeup_reason* wakeup, wrm_freq_input_type* band_res_req_info, uint8* srch_duration,wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===================================================================================
FUNCTION     WSLEEP_MEAS_GRANT_CB 

DESCRIPTION
  This handles the grant callback from for the req and notify made for 
  a meas gap.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  No  
 
======================================================================================*/

extern void wsleep_meas_grant_cb(boolean success, wsub_id_e_type wsub_id);

/*===================================================================================
FUNCTION     WSLEEP_QTA_CB 

DESCRIPTION
  This is the function called by multisim module at CPHY_START_QTA
  and CPHY_STOP_QTA.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  No  
 
======================================================================================*/

extern void wsleep_qta_cb(trm_state_oper_enum_type qta_state, wsub_id_e_type wsub_id);

#endif

/*===================================================================================
FUNCTION     WSLEEP_MEAS_GUARD_TMR_EXPIRY 

DESCRIPTION
  Posts local cmd for meas guard timer expiry.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  No  
 
======================================================================================*/
extern void wl1sleep_meas_guard_tmr_expiry(timer_cb_data_type cb_data);

/*===================================================================================
FUNCTION     WSLEEP_GET_PEDNING_SRCH_BAND_DURATION 

DESCRIPTION
  This function reads the pending searches db in srchzz to decide
  if a meas gap has to be opened.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  No  
 
======================================================================================*/

extern void wsleep_get_pending_srch_band_duration(wrm_freq_input_type* band, uint8* duration, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION  WSLEEP_RFM_PREP_DS_MODE
 
DESCRIPTION
  This function determines the frequency to tune at wakeup and calls appropriate
  APIs for preparing scripts in SS/DS mode respectively.
 
DEPENDENCIES
  None
  
RETURN VALUE
  None
 
SIDE EFFECTS
  None
===========================================================================*/
extern boolean wsleep_rfm_prep_ds_mode(wl1_rfm_wakeup_chain_type rf_wakeup_type,wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
extern void wl1_sleep_params_get_debug_info(wl1_drx_sleep_mini_debug_info_struct_type* obj);
#endif

extern void wsleep_rfm_pretune( wsub_id_e_type wsub_id);

#ifdef FEATURE_DUAL_SIM
/*===================================================================================
FUNCTION     WSLEE_PROCESS_GUARD_TIMER_EXPIRY 

DESCRIPTION
  A guard timer was set for PICH - 180ms at the time of requesting for lock for meas. It has expired.
  We must reserve for PICH now. Sleep interval and SLPC would have already been programmed for PICH.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  No  
 
======================================================================================*/
extern void wsleep_process_guard_timer_expiry(wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_MSIM_TEST_FRAMEWORK
/*===================================================================================
FUNCTION     WSLEEP_GET_PERIODIC_GAP_INFO_FOR_ELS

DESCRIPTION 
   This will provide the following periodic gap information for ELS:
   - start of PICH frame in sclks
   - start of PI bits in sclks
 
DEPENDENCIES
  None.

PARAMETERS
  wsub_id - wsub id

RETURN VALUE
  pich_start_sclk - start of PICH frame in sclks
  pi_start_sclk   - start of PI bits in sclks

======================================================================================*/
void wsleep_get_periodic_gap_info_for_els(uint32 *pich_start_sclk, uint32 *pi_start_sclk, wsub_id_e_type wsub_id);
#endif

/*===================================================================================
FUNCTION     WSLEEP_PERFORM_LOCK_OPS_AT_SLEEP 

DESCRIPTION 
   This will perform lock operations like reserve/request and notify
    while going to sleep in both normal sleep and wakeup cancel cases.
 
DEPENDENCIES
  None.

PARAMETERS
  wakeup_cancel - if wakeup a reserve needs to be done even it is CTCH ETWS message.

RETURN VALUE
  TRUE  - LOCK is not reserved.
  FALSE - LOCK is reserved.
 
======================================================================================*/
extern boolean wsleep_perform_lock_ops_at_sleep(boolean wakeup_cancel, wsub_id_e_type wsub_id);

extern void wl1sleep_reprogram_for_online_pich(wsub_id_e_type wsub_id);
#endif

/*===================================================================================
FUNCTION     WL1SLEEP_CLOSE_TO_NEXT_WAKEUP 

DESCRIPTION 
   This checks if we are close to the programmed wakeup. This is only called by WRM
   when a reservation for a meas only gap has been made ( hence in PCH_SLEEP only )
   and a RnN from RRC comes in. 
   If the wakeup is not close, the RnN is honored. If close, it is pended to be handled
   at the end of the gap. 5 ms has been chosen as a conservative buffer.
 
DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  TRUE  - less than 5 ms to the reserved meas gap
  FALSE - more than 5 ms away to the reserved meas gap.
 
======================================================================================*/

extern boolean wl1sleep_close_to_next_wakeup(wsub_id_e_type wsub_id);

#if defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
/*===================================================================================
FUNCTION     WSLEEP_PROGRAM_DUMMY_WAKEUP 

DESCRIPTION 
  This API will be called by test framwork with no of frames with in which a 
  dummy wakeup should happen. This is mainly used to test concurrecny scenarios 
  with other Techs.
  
DEPENDENCIES
  None.

PARAMETERS
 wakeup_in_frames - After how many frames a wakeup should happen.

RETURN VALUE
  No  
 
======================================================================================*/
extern boolean wsleep_program_dummy_wakeup(uint16 wakeup_in_frames, wsub_id_e_type wsub_id);

extern boolean wsleep_trigger_dummy_wakeup_cancel(wsub_id_e_type wsub_id);
extern boolean wsleep_trigger_dummy_ext_cmd(l1_cmd_enum_type ext_cmd_id, wsub_id_e_type wsub_id);
extern boolean wsleep_trigger_dummy_local_cmd(l1_local_cmd_enum_type local_cmd_id, wsub_id_e_type wsub_id);
extern boolean wsleep_trigger_dummy_po_ovveran(uint8 num_ms_delay, wsub_id_e_type wsub_id);
extern boolean wsleep_qsh_dummy_enable_rxd(uint8 num_cycles, wsub_id_e_type wsub_id);
extern boolean wsleep_qsh_dummy_enable_qxt(uint8 num_cycles, wsub_id_e_type wsub_id);

extern boolean wsleep_qsh_check_rxd_status(wsub_id_e_type wsub_id);
extern boolean wsleep_qsh_check_qxt_status(wsub_id_e_type wsub_id);
extern void wl1_wakeup_manager_cb (wmgr_client_enum_t client, wmgr_result_type result, uint32 transition,uint32 reserved, void *user_data);

typedef struct
{
   uint8 po_overran_delay_ms;
   uint8 rxd_active_num_cycles;
   uint8 qxt_active_num_cycles;
     /* Consecutive PI SUM 0 in Good RF*/
  uint8 num_cons_pi_sum_zero_goodRF;
  /* Bad PI SUM over a period of 10 cycles*/
  uint8 num_bad_pi_sum;
}wsleep_qsh_info_type;

extern wsleep_qsh_info_type wsleep_qsh_info[WCDMA_NUM_SUBS];

#define DRX_GET_NUM_CONT_PISUM_ZERO_GOOD_RF() (wsleep_qsh_info[wsub_id].num_cons_pi_sum_zero_goodRF)
#define DRX_INC_NUM_CONT_PISUM_ZERO_GOOD_RF() (wsleep_qsh_info[wsub_id].num_cons_pi_sum_zero_goodRF++)
#define DRX_CLR_NUM_CONT_PISUM_ZERO_GOOD_RF() (wsleep_qsh_info[wsub_id].num_cons_pi_sum_zero_goodRF = 0)

#define DRX_GET_NUM_BAD_PISUM() (wsleep_qsh_info[wsub_id].num_bad_pi_sum)
#define DRX_INC_NUM_BAD_PISUM() (wsleep_qsh_info[wsub_id].num_bad_pi_sum++)
#define DRX_CLR_NUM_BAD_PISUM() (wsleep_qsh_info[wsub_id].num_bad_pi_sum = 0)
#endif

/*===================================================================================
FUNCTION     WSLEEP_GET_WAKEUP_TIME 

DESCRIPTION 
  This API returns the next wakeup time in terms of SFN and Cx8 TLM STRUCT type.
  
DEPENDENCIES
  None.

PARAMETERS
  TLM CFN STRUCT TYPE PTR.

RETURN VALUE
  None.
 
======================================================================================*/
extern void wsleep_get_wakeup_time(tlm_cfn_struct_type *wakeup_time,wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     wsleep_set_rxd_bch_status

DESCRIPTION 
  API used to set RxD state used in BCH

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wsleep_set_rxd_bch_status(boolean rxd_status, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     wsleep_get_rxd_bch_status

DESCRIPTION 
  API used to get RxD state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern boolean wsleep_get_rxd_bch_status(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     wsleep_notify_qta_for_idle_tech

DESCRIPTION 
  This API is used to nofity the IDLE TECH of the upcoming QTA in data SUB context.
  We will post local cmd to handle it in IDLE TECH context.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void wsleep_notify_qta_for_idle_tech(wsub_id_e_type wsub_id);


#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION     WSLEEP_UPDATE_LOCK_REQ_DELAY

DESCRIPTION 
  This API is used to compute the time difference between entity timer expiry 
  and MWU ISR in which lock req is made. Need of this is, while going to sleep 
  we run through all entities wakeup times and check for collision and decide a
  ds_proc with which lock is reserved. But at wakeup as entity timer will expire
  first followed by MWU ISR where entity collision is checked. Because of time gap
  between entity expiry and MWU ISR we might a detect new collision at wakeup which 
  was not detected while going to sleep, this will change the proc which shouldn't 
  happen. This API will make sure we dont detect a false collision at wakeup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_update_lock_req_delay(wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION     wsleep_notify_ctch_non_drx_mode_start

DESCRIPTION 
  This API is used by SCCPCH channel manager to inform that CTCH is being 
  added for the first time in NON DRX mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_notify_ctch_non_drx_mode_start(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WL1_LTE_EMBMS_SESSION_HANDLER

DESCRIPTION 
  Function to handle embms indication from LTE 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

extern void wl1_lte_embms_session_handler(msgr_hdr_struct_type* hdr, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION:      wl1sleep_compute_curr_sfn_in_sleep

DESCRIPTION:   This function calculates the current cell sfn and cx8 based on frame timing at ue sleep startand
               the number of ticks elapsed since ue put to sleep.

DEPENDENCIES:  wsub-id

RETURN VALUE:  current cell sfn and cx8

SIDE EFFECTS:  NONE
===========================================================================*/
void wl1sleep_compute_curr_sfn_in_sleep(tlm_tfn_struct_type *cell_sfn, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION     wsleep_is_ta_type_qta

DESCRIPTION 
  Function to decide what kind of TUNE AWAY it will be based on CTCH is active or not.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern trm_tuneaway_enum_type wsleep_determine_ta_type(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     WSLEEP_APS_VOTING

DESCRIPTION 
  Wrapper function to vote for APS ON/OFF based on
  --Target entity
  --DR/SR mode
  --DRX cycle length

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wsleep_aps_voting(drx_entity_handle_type handle,wsub_id_e_type wsub_id);

extern uint64 wsleep_get_mcpm_config_time_cx8(wsub_id_e_type wsub_id);


#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
FUNCTION:      wl1_qsh_mdump_collect_sleep

DESCRIPTION:   UL Minidump

DEPENDENCIES:  None

RETURN VALUE:  NONE

SIDE EFFECTS:  NONE
===========================================================================*/
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_sleep(wsub_id_e_type wsub_id);
#endif
#endif /* WL1SLEEP_H */