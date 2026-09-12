#ifndef SRCHSCHED_H
#define SRCHSCHED_H

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srchpch.c_v   1.12   11 Jul 2002 21:20:38   ajudet  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/src/srchsched.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/11/19   ac      Searcher Mini Dump Changes
07/21/16   vn      API to clear search HB and SRCH OPS from queue
07/19/16   bj      F3 reduction in WL1 searcher module phase-II and code cleanup
03/30/16   bj      NASTT search power optimization changes.
11/27/15   rn      Don't register the timeout handler client, if VSTMR is not up.
11/04/15   vn      FR 30623 MCVS searcher optimizations initial check in
10/07/15   rn      Externing ASYNC NASTT init function
09/03/15   amj     Reset async search parameter if NASTT search is aborted 
08/17/15   ap      parallelizing suspending periodic ops
07/21/15   amj     Enhance NAP feature to RxD and DC cases.
06/26/15   bj      FD-NASTT to be issued once every 4 180 ms full search period and the remaining 3 will be Async-NASTT searches.
06/19/15   bj      [FR 27497] Async NASTT – Power feature in Connected Mode.
06/15/15   jd      CM optimization changes.
06/12/15   da      TA Featurization for CR 846021: Changes to abort all searches in QTA gap 
                   if the winning client is 1x
06/05/15   amj     Changes for Step1 Peak Pruner algorithm.
05/30/15   da      Changes to abort all searches in QTA gap if the winning client is 1x
05/07/15   skk     FR24494: BCH Inter SIB sleep initial check in.
02/27/15   ap      Initial check-in for WL1 QSH framework 
01/28/15   pr      Global reorg for W + W.
01/20/15   jd      Global re-org for W + W
12/22/14   jd      CM optimization changes.
10/29/14   jd      Complete mode change on FW search abort
10/20/14   jd      Changes to complete mode change when FW aborts search.
10/10/14   amj     Code Cleanup.
09/10/14   amj     Nastt Power Optimization changes.
09/22/14   amj     Add a Mode change request timeout client.
09/22/14   amj     Changes such that SRCH ABORT DONE is routed to legacy abort done processor if EDRX is active.
09/19/14   da      Jolokia specific changes related to search parameters in ACQ
05/20/14   bj      Added support for Pause and Resume searches in QTA gap and disable RxD searches in QTA gap
08/07/14   jk      Fixed search compiler warnings.
06/17/14   sm      Srch changes for RxD fast teardown
05/14/14   jd      Changes to add frame reservation in ACQ and BCH.
06/08/14   amj     Initialize and reset async and pn search counts only on state transitions.
05/27/14   jd      Changes to protect rxd async param update when step1 is in progress.
04/27/14   amj     Create macro for checking if async search is in progress
03/31/14   jd      Fix for conflict between scch deactivation and parallel step1.
03/26/14   jd      Moving the cme scch order callback to wl1 task context.
03/17/14   jd      Fix to use mutex in scheduler to avoid race condition during mode change.
02/28/14   sad     SCCH order optimization check in
04/02/14   amj     Created API for resuming CPHY operations upon search abort done.
04/01/14   km      Rxd/ACQ changes
03/27/14   jk      Adding a MACRO for run_triage flag in srchsched_ext_cntrl_params[wsub_id].srchsched_db.
03/26/14   bj      Freezed sample server for step-1/NASTT search in BPLMN state.
01/10/14   jd      Searcher clean-up
01/07/14   amj     Cleaned up unused variables.
01/06/14   sr      Suspending periodic ops before CPHY req processing
12/16/13   as      Porting G2W TA from Triton to Dime.
12/12/13   amj     Corrected Scheduler Busy macro 
12/06/13   jd      Fix for Incorrect pn result processing when cell is removed during search. 
11/06/13   gj      Added 3C searcher code
10/22/13   ar      Added prototype for srchsched_init_db 
10/18/13   ar      Support for setting the allowed searches mask
10/17/13   bs      Preliminary 3C Support
11/20/12   amj     Created file

===========================================================================*/

#include "srchcmd.h"
#include "comdef.h"
#include "srchsetutil.h"
#include "l1def.h"
#include "mcalwcdma_srch.h"
#include "wl1m.h"

#define SRCH_STEP1_MAX_PEAK_CNT_C0 12
#define SRCH_STEP1_MAX_PEAK_CNT_WITH_CM_C0 8
#define SRCH_STEP1_MAX_PEAK_CNT_C1 8
#define SRCH_STEP1_MAX_PEAK_CNT_C2 8

/*  NASTT Power Optimization Defines  */

#define SRCH_NASTT_SKIP_COND1_PN_POS_DIFF_CX8 16
#define SRCH_NASTT_SKIP_COND1_ENG_DIFF 3
#define SRCH_NASTT_SKIP_COND2_ENG_DIFF 5
#define SRCH_MAX_NUM_RXAGC_VAL       15
#define SRCH_STEP1_VARIANCE_COMP        24
#define SRCH_STEP1_ENG_THRESH_DB       15
#define SRCH_STEP1_ENG_THRESH_DB_WITH_DIV   12

#define SRCH_NASTT_UNMON_PERIOD 4 /*3 out of 4 NASTT type is ASYNC NASTT*/


/* Largest QTA gap possible is 180ms (although rare) */
#define SRCH_MODE_CHANGE_REQ_TIMEOUT 19

#define IS_PARALLEL_STEP1_ISSUED()    \
         (wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.srch_step1_sync_parms.carrierCount == MAX_PARALLEL_SRCH )

typedef void SRCHSCHED_RXD_ASYNC_PARAM_UPDATE_CB_FUNC_TYPE(wsub_id_e_type wsub_id);

typedef void SRCHSCHED_MODE_CHG_SRCH_ABORT_POSTCB (l1m_suspend_client_enum_type module, wsub_id_e_type wsub_id);

/**
 * Enum that shows the different types of clients that Search 
 * Scheduler can have 
 */
typedef enum
{
  SRCHSCHED_ACQ_CLIENT,
  SRCHSCHED_BPLMN_CLIENT,
  SRCHSCHED_BCH_CLIENT,
  SRCHSCHED_PCH_CLIENT,
  SRCHSCHED_FACH_CLIENT,
  SRCHSCHED_DCH_CLIENT,
  SRCHSCHED_NUM_CLIENTS
} srchsched_client_id_enum_type;

/**
 * Enum denoting all the different modes that the scheduler can 
 * operate in. 
 */
typedef enum
{
  SRCHSCHED_INACTIVE_MODE,
  SRCHSCHED_CONFIG_MODE,
  SRCHSCHED_INTRAF_MODE,
  SRCHSCHED_SUSPENDED_MODE,
  SRCHSCHED_FMO_MODE,
  SRCHSCHED_DL_DISABLED_MODE,
  SRCHSCHED_SLEEP_MODE,
  SRCHSCHED_WAKEUP_MODE,
  SRCHSCHED_NUM_MODES
} srchsched_mode_enum_type;

/**
 * Enum denoting the type of result for a particular frequency 
 * hypothesis for Step1/NASTT search 
 */
typedef enum
{
  FREQ_HYPO_NOT_DONE,
  FREQ_HYPO_PASS,
  FREQ_HYPO_FAIL,
} srchsched_step1_nastt_status_enum_type;

typedef enum
{
  SRCH_PRIMARY,
  SRCH_SECONDARY,
  SRCH_TERTIARY,
  SRCH_PARALLEL,
  SRCH_CARRIER_MAX
}srch_carrier_type;

typedef struct
{
  srch_ops_code_enum_type step1_type;
  srch_ops_code_enum_type nastt_type;
}srchsched_map_type_to_carrier_struct;

/**
 * Structure used to store the Step1 and NASTT results 
 */
typedef struct
{
  srchsched_step1_nastt_status_enum_type f0_s1_status;
  srchsched_step1_nastt_status_enum_type f1_s1_status;
  srchsched_step1_nastt_status_enum_type f0_nastt_status;
  srchsched_step1_nastt_status_enum_type f1_nastt_status;
} srchsched_step1_nastt_status_struct_type;

/**
 * Struct showing all the periodic search counts that will be incremented
 * as part of the Heartbeat Handler
 */
typedef struct
{
  /** This variable holds the ASET PN timer expiry count */
  uint16 aset_pn_srch_cnt;
  /** This variable holds the NSET PN timer expiry count */
  uint16 nset_pn_srch_cnt;
} srchsched_search_expiry_counts_struct_type;

/*
 * Struct for all the scheduler mode-related parameters
 */
typedef struct
{
  /** This flag is set to TRUE if a mode change is in progress, FALSE otherwise */
  boolean mode_change_in_progress;

  /** This variable holds the current mode of operation of the search scheduler */
  srchsched_mode_enum_type curr_mode;

  /** This variable holds the next mode of operation that the search scheduler will be put in */
  srchsched_mode_enum_type next_mode;

  /** This variable holds a void callback function which will get initialized when an mode
   * change request has been issued and an abort has been issued to FW. The callback function
   * will execute when the search abort done is complete, if it is not null */
  SRCHSCHED_MODE_CHG_SRCH_ABORT_POSTCB *abort_done_post_cb;
} srchsched_mode_params_struct_type;

/*
 * Struct for all the Search scheduling parameters
 */
typedef struct
{
  /** This variable holds the mask of all searches that will get pended while a mode 
  * change is in progress */
  uint32 pended_searches;

  /** This variable holds the type of the current search that just got issued. Since
  * there can't be two searches issued at the same time, this is unique. */
  srch_ops_code_enum_type curr_srch_issued;

  /** This array holds bit masks of allowed searches on
        *  each queue */
  uint32 allowed_searches_on_queue[SRCH_NUM_Q];
  
  /** Variable which holds a mask with all the searches that are allowed in Intraf mode, depending on the client */
  uint16 intraf_searches_allowed;

  /* variables holds if fet is enabled, so that overlapped pn searches can be issued */
  boolean fet_active;

  /*Variable holds if scch activation search is in progress*/
  boolean scch_act_srch_in_progress;

  /* Variable holds aset cell masks of carriers which
      needs to be issued for hpq search*/
  uint32 scch_act_cell_mask;

  /* Variable holds aset cell masks of carriers which
      needs to be issued for hpq search*/
  uint32 cur_scch_order;

  /* Variable holds the bitmask of carriers which are 
     de-activated by scch order*/
  uint8 scch_deact_bmsk;

  /* Variable is used if PN cell is removed in the from PN set 
  This will help in Pn processing if cell was removed in middle of search*/
  boolean srch_pn_set_invalidated;

  /* In case rxd tear-down happens, the diversity results needs to 
  be invalidated for current searches in progress*/
  uint32 srch_rxd_teardown_invalidate_mask;

  /** Timer Expiry Count for Searches */
  srchsched_search_expiry_counts_struct_type srch_expiry_counts;

} srchsched_srch_params_struct_type;

/*
 * Struct for all the Step1 and NASTT related processing parameters
 */

typedef struct
{
  /* Boolean that holds whether all conditions for skipping NASTT are met or not */
  boolean skip_nastt_conditions_met;
  /** This flag holds TRUE if NASTT needs to be tried on a different FREQ HYPO */
  boolean nastt_tries_left;
  /* Current Freq Index sent for NASTT */
  uint8 curr_nastt_freq_idx;
  /* Boolean flag to keep track of the previous status of RxD at the time step1 got issued */
  boolean prev_s1_rxd_active_in_dch;
  /* Toggle to check whether NASTT search got issued with the last Step1 search or not */
  boolean nastt_srch_360ms_toggle;
  /* Boolean flag to keep track of the current status of RxD at the time step1 got issued */
  boolean curr_s1_rxd_active_in_dch;
  /* Boolean flag that keeps track of whether Step1 pruning is enabled through NV 73569 or not */
  boolean step1_pruning_enabled;
  /* variable that keeps track of the number of Step1 searches in connected mode, maxes at 15 */
  uint8 srch_curr_num_step1_in_dch;
  /* Flag indicating that asynchronous neighbor searches is in progress */
  boolean async_full_srch_active;

  boolean async_nastt_enabled;

  /** This flag gets set to TRUE if Step1 search has just been processed and we need to do a NASTT for the carrier 
  * specified, otherwise it stays FALSE and gets set to FALSE after NASTT search completed */
  boolean nastt_srch_due[NUM_CARR];
  /** This global variable will keep track of the status of Step1 and NASTT search results */
  srchsched_step1_nastt_status_struct_type step1_nastt_srch_result_status;
  /* Async count for Step1 search and for NASTT search */
  int16 async_step1_srch_timer[NUM_CARR];
  /* Starting position of PSC peaks to be sent to step_2 searcher */
  uint16 cur_psc_peak_indx[NUM_CARR];
} srchsched_step1_nastt_params_struct_mdump_type;

typedef struct
{

	 

  #ifdef FEATURE_WCDMA_L1_ASYNC_NASTT_ENABLED
  /*previous issued NASTT search type*/
  srch_full_srch_type_enum_type srch_prev_nastt_srch_type;
  #endif

  /* Boolean that reads from NV whether NASTT power optimizations is enabled or not, Default 1 */
  boolean nastt_power_opt_enabled;
  #ifdef FEATURE_WCDMA_L1_ASYNC_NASTT_ENABLED
  /* ASYNC-NASTT Period counter for Primarry carrier */
  uint8 srch_async_nastt_period_cnt;
  #endif

  #ifdef FEATURE_WCDMA_L1_ASYNC_NASTT_ENABLED
  /* Number of cells  sent for ASYNC NASTT search command */
  uint16 srch_nastt_async_cell_cnt;
  #endif

  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif

#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif

  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
    #error code not present
#endif

  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif

  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif

  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif

  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif
 
  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif  

  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif

  /* step1 peak count for each carrier */
  uint16 srch_step1_peak_cnt[NUM_CARR];
  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif
  
  #ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
  #error code not present
#endif

  /* DB used for NASTT power optimizations to keep track of the RxAGC values of the past 15 Step1 searches */
  int16 srch_nastt_eval_rxagc_db[SRCH_MAX_NUM_RXAGC_VAL];
  /* Variable to keep track of the previous top Step1 peak for NASTT Opt evaluations */
  mcalwcdma_srch_step1_result_peak_type prev_top_step1_peak;
  #ifdef FEATURE_WCDMA_L1_ASYNC_NASTT_ENABLED
  /* PSC of the cells sent for ASYNC NASTT srch */
  uint16  srch_nastt_async_cell_psc[SRCH_NASTT_ASYNC_MAX_SCR_CODES];
  #endif

  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchsched_step1_nastt_params_struct_mdump_type step1_nastt_params_mdump;
} srchsched_step1_nastt_params_struct_type;

/*
 * Struct for all the Search Scheduler Database
 */

typedef struct
{
  /** This flag is TRUE if UE is in ASU procedure, FALSE otherwise */
  boolean in_asu_proc;
  /** This counter will trigger trigger every other time a PN search is done */
  boolean run_triage;
  /** This variable will hold the current client ID that is driving the search scheduler */
  srchsched_client_id_enum_type curr_client_id;
  srchsched_mode_params_struct_type mode_params;
  /* This variable is for all the parameters related to search issuing */
  srchsched_srch_params_struct_type srch_params;
} srchsched_db_struct_mdump_type;


typedef struct
{
  /* Callback used to update step1 async parameters*/
  SRCHSCHED_RXD_ASYNC_PARAM_UPDATE_CB_FUNC_TYPE* srchsched_rxd_async_param_update_cb;
  /* This variable is for all the parameters related to Step1 and NASTT results and processing that
   * search scheduler did */
   /*The structure below was further split for Mini Dump, only dumping a part of that structure*/
  srchsched_step1_nastt_params_struct_type step1_nastt_params;

  /*Mini Dump: Variables that need to be dumped moved to this structure*/
  srchsched_db_struct_mdump_type srchsched_db_mdump;
}srchsched_db_struct_type;


/**
 * This structure holds the handlers for each search mode of the scheduler 
 */
typedef struct
{
  void (*init)(wsub_id_e_type wsub_id);
  void (*cleanup)(wsub_id_e_type wsub_id);
  void (*srch_scheduler) (srch_ops_cmd_type *, wsub_id_e_type wsub_id);
  void (*srch_done) (srch_done_cmd_type *, wsub_id_e_type wsub_id);
} srchsched_srch_mode_handler_struct;

/**
 * This struct defines the handling functions for each type of search
 * that the scheduler is going to support
 */
typedef struct
{
  void (*step1_srch_proc)(srchsched_step1_nastt_status_struct_type *step1_nastt_status, wsub_id_e_type wsub_id);
  void (*nastt_srch_proc)(srchsched_step1_nastt_status_struct_type *step1_nastt_status, wsub_id_e_type wsub_id);
  void (*aset_pn_srch_proc)(srch_done_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);
  void (*nset_pn_srch_proc)(srch_done_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);
  #ifdef FEATURE_DUAL_SIM
  void (*wakeup_srch_proc) (srch_done_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);
  #endif
  void (*scch_act_srch_proc) (srch_done_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);
} srchsched_srch_done_cmd_handler_struct;

typedef struct
{
  srchsched_db_struct_type srchsched_db;
}srchsched_ext_cntrl_params_type;

extern srchsched_ext_cntrl_params_type srchsched_ext_cntrl_params[WCDMA_NUM_SUBS];

#define IS_SRCHSCHED_ACTIVE()                 (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.mode_params.curr_mode != SRCHSCHED_INACTIVE_MODE)

#define SRCHSCHED_IN_INTRAF_MODE()            (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.mode_params.curr_mode == SRCHSCHED_INTRAF_MODE)

#define SRCHSCHED_IS_MODE_CHG_IN_PROG()    (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.mode_params.mode_change_in_progress)

#define SRCHSCHED_IS_CURR_SRCH_FULL_SRCH()   (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued & srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.allowed_searches_on_queue[SRCH_S1_NASTT_Q])
 
#define SRCHSCHED_IS_SRCH_IN_PROG(srch_type)   (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued & srch_type)

#define SRCHSCHED_SRCH_DONE_THROUGH_SCHED() ((WL1_GET_L1M_STATE() == L1M_ACQ) || WL1_L1M_IS_IN_CELL_SELECTION() || (WL1_GET_L1M_STATE() == L1M_DCH) || (WL1_GET_L1M_STATE() == L1M_PCH_BPLMN) || (WL1_GET_L1M_STATE() == L1M_FACH))

#define SRCHSCHED_GET_SRCH_ISSUED_ON_QUEUE(queue) (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued & srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.allowed_searches_on_queue[queue])

#define SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(srch_type, queue)   (SRCHSCHED_GET_SRCH_ISSUED_ON_QUEUE(queue) & srch_type)

#define SRCHSCHED_IS_ASYNC_SRCH_IN_PROG(queue)   \
                  (SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(STEP1_SRCH, queue) ||                   \
                   SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(DC_STEP1_SRCH, queue) ||                \
                   SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(W_3C_STEP1_SRCH, queue) ||              \
                   SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(PARALLEL_STEP1_SRCH, queue) ||          \
                   SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(NASTT_SRCH, queue) ||                   \
                   SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(DC_NASTT_SRCH, queue) ||                \
                   SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(W_3C_NASTT_SRCH, queue) ||              \
                   SRCHSCHED_IS_SRCH_IN_PROG_IN_QUEUE(PARALLEL_NASTT_SRCH, queue))

#define SRCHSCHED_CLEAR_SRCH_ON_QUEUE(queue) (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued ^= SRCHSCHED_GET_SRCH_ISSUED_ON_QUEUE(queue))

#define SRCHSCHED_CLEAR_ALL_SEARCHES() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued = NO_SRCH)

#define SRCHSCHED_SET_SRCH(srch_type) (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued |= srch_type)

#define SRCHSCHED_IS_CURR_SRCH_NASTT_SRCH()    \
                ((srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued & NASTT_SRCH) ||    \
                 (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued & DC_NASTT_SRCH) ||    \
                 (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued & W_3C_NASTT_SRCH) ||    \
                 (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued & PARALLEL_NASTT_SRCH))

#define SRCHSCHED_IS_OVERLAPPED_SRCH_ENABLED() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.fet_active)

#ifdef FEATURE_WCDMA_DUAL_SYNC_INTERFACE
#define SRCHSCHED_GET_QUEUE_FOR_SRCH(srch_type) ( (srch_type  == MCALWCDMA_SRCH_STEP1) ? SRCH_S1_Q : SRCH_NASTT_PN_Q)
#else
#define SRCHSCHED_GET_QUEUE_FOR_SRCH(srch_type) ( (srch_type  == MCALWCDMA_SRCH_PN) ? SRCH_NASTT_PN_Q : SRCH_S1_Q)
#endif

#define SRCHSCHED_IS_QUEUE_BUSY(queue) SRCHSCHED_GET_SRCH_ISSUED_ON_QUEUE(queue)

#define SRCHSCHED_IS_BUSY()     (SRCHSCHED_IS_QUEUE_BUSY(SRCH_S1_Q) && SRCHSCHED_IS_QUEUE_BUSY(SRCH_NASTT_PN_Q))

#define SRCHSCHED_IS_SRCH_ONGOING() ( srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.curr_srch_issued != NO_SRCH )

extern const srchsched_srch_mode_handler_struct *srchsched_srch_handler[SRCHSCHED_NUM_MODES];

#define SRCHSCHED_INVOKE_SCHEDULER(mode, param) (srchsched_srch_handler[mode]->srch_scheduler(param, wsub_id))

#define SRCHSCHED_IS_CURR_CLIENT_DCH_CLIENT() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.curr_client_id == SRCHSCHED_DCH_CLIENT)

#define SRCHSCHED_IS_CURR_CLIENT_BCH_CLIENT() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.curr_client_id == SRCHSCHED_BCH_CLIENT)

#define SRCHSCHED_IS_CURR_CLIENT_BPLMN_CLIENT() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.curr_client_id == SRCHSCHED_BPLMN_CLIENT)

#define SRCHSCHED_IS_CURR_CLIENT_ACQ_CLIENT() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.curr_client_id == SRCHSCHED_ACQ_CLIENT)

#define SRCHSCHED_IN_SUSPENDED_MODE()         (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.mode_params.curr_mode == SRCHSCHED_SUSPENDED_MODE)

#define SRCHSCHED_IN_CONFIG_MODE()         (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.mode_params.curr_mode == SRCHSCHED_CONFIG_MODE)

#define SRCHSCHED_IN_INTRAF_MODE()         (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.mode_params.curr_mode == SRCHSCHED_INTRAF_MODE)

#define SRCHSCHED_IS_CLIENT_ACQ()  (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.curr_client_id == SRCHSCHED_ACQ_CLIENT)

#define SRCHSCHED_IS_CURR_CLIENT_FACH_CLIENT() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.curr_client_id == SRCHSCHED_FACH_CLIENT)

#define SRCHSCHED_GET_CURR_CLIENT() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.curr_client_id)

#define SRCHSCHED_IS_ASET_PN_SRCH_ALLOWED() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.intraf_searches_allowed & ASET_PN_SRCH)

#define SRCHSCHED_IS_NSET_PN_SRCH_ALLOWED() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.intraf_searches_allowed & NSET_PN_SRCH)

#define SRCHSCHED_IS_STEP1_SRCH_ALLOWED() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.intraf_searches_allowed & STEP1_SRCH)

#define SRCHSCHED_SEND_ABORT_TO_SCHEDULER() ((WL1_GET_L1M_STATE() == L1M_ACQ) || WL1_L1M_IS_IN_CELL_SELECTION()  \
                                            || (WL1_GET_L1M_STATE() == L1M_DCH) || (WL1_GET_L1M_STATE() == L1M_PCH_BPLMN) \
                                            || (SRCHFACH_NONDRX_FACH()) || SRCHSCHED_IS_MODE_CHG_IN_PROG() )

#define SRCHSCHED_TRIAGE_CYCLE_DUE() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.run_triage)

#define SRCHSCHED_IS_REACT_SRCH_IN_PROG() ( srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.scch_act_srch_in_progress )

#define SRCHSCHED_CELL_MASK_FOR_REACT_SRCH(carrier_idx) ( carrier_idx == CARR_1 ? SRCH_PN_DC_ASET_INCLUDED : SRCH_PN_3C_ASET_INCLUDED);

#define SRCHSCHED_SET_REACT_SRCH_IN_PROG() ( srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.scch_act_srch_in_progress = TRUE )

#define SRCHSCHED_CLEAR_REACT_SRCH_IN_PROG() ( srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.scch_act_srch_in_progress = FALSE )

#define SRCHSCHED_IS_CARR_DEACTIVATED(carrier_idx) (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.scch_deact_bmsk & (1 << carrier_idx ))

#define SRCHSCHED_DEACTIVATE_CARR(carrier_idx) ( srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.scch_deact_bmsk |= (1 << carrier_idx ))

#define SRCHSCHED_ACTIVATE_CARR(carrier_idx)  (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.scch_deact_bmsk &= ~((uint8) (1 << (carrier_idx))) )

#define SRCHSCHED_SCCH_ACT_SRCH(srch_ops) ( srch_ops->ops_code.srch_type == SCCH_ACT_SRCH )

#define SRCHSCHED_SKIP_PENDED_SRCH_DURING_SCCH_ACT() (!SRCHSCHED_IS_REACT_SRCH_IN_PROG() || (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & SCCH_ACT_SRCH))

#define SRCHSCHED_SEC_CARR_ISSUED_FOR_PARALLEL_STEP1() ( wsrch_ext_cntrl_params[wsub_id].wsrch_ext_cntrl_params_mdump.srch_step1_sync_parms.carrierSel[CARR_1])

#define SRCHSCHED_GET_STEP1_BMSK() (STEP1_SRCH | DC_STEP1_SRCH | W_3C_STEP1_SRCH | PARALLEL_STEP1_SRCH)

#define SRCHSCHED_SET_RXD_TEARDOWN_INVALIDATION_MASK(mask) (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.srch_rxd_teardown_invalidate_mask |= mask)

#define SRCHSCHED_CLEAR_RXD_TEARDOWN_INVALIDATION_MASK(mask) (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.srch_rxd_teardown_invalidate_mask &= ~ mask)

#define SRCHSCHED_IS_STEP1_DIV_RSLT_INVALID() ( srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.srch_rxd_teardown_invalidate_mask & srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.allowed_searches_on_queue[SRCH_S1_Q] )

#define SRCHSCHED_IS_PN_DIV_RSLT_INVALID() ( srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.srch_rxd_teardown_invalidate_mask & srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.allowed_searches_on_queue[SRCH_NASTT_PN_Q] )

#define SRCHSCHED_RESET_RXD_TEARDOWN_INVALIDATION_MASK() (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.srch_rxd_teardown_invalidate_mask = 0)

#define SRCHSCHED_IS_PENDED_SEARCH_ASYNC_SRCH() ((srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & STEP1_SRCH) || \
                                              (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & DC_STEP1_SRCH) || \
                                            (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & W_3C_STEP1_SRCH) || \
                                        (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & PARALLEL_STEP1_SRCH) || \
                                                 (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & NASTT_SRCH) || \
                                              (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & DC_NASTT_SRCH) || \
                                            (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & W_3C_NASTT_SRCH) || \
                                        (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.srch_params.pended_searches & PARALLEL_NASTT_SRCH))

#ifdef FEATURE_WCDMA_L1_ASYNC_NASTT_ENABLED                                        
#define SRCHSCHED_IS_ASYNC_NASTT_ACTIVE() ((SRCHSCHED_IS_CURR_CLIENT_DCH_CLIENT()) && \
                                           (srchsched_get_full_srch_timer(CARR_0, wsub_id) == SRCHDCH_ASYNC_SRCH_TIME)&& \
                                           (srchsched_ext_cntrl_params[wsub_id].srchsched_db.step1_nastt_params.step1_nastt_params_mdump.async_nastt_enabled == TRUE))
#endif

#define SRCHSCHED_IS_VSTMR_UP()    (srchsched_ext_cntrl_params[wsub_id].srchsched_db.srchsched_db_mdump.mode_params.next_mode != SRCHSCHED_SUSPENDED_MODE) && (WL1_GET_L1M_STATE() != L1M_DEEP_SLEEP)

/*=============================================================================
  FUNCTION
    SRCHSCHED_MODE_CHANGE_REQ
  
  DESCRIPTION
    This function will get called by an external client ID and it will request
	the scheduler to move into a specific mode. 

  DEPENDENCIES
    None
  
  RETURN VALUE
    TRUE if the request is successful, FALSE if mode change validation fails
  
  SIDE EFFECTS
    None
=============================================================================*/
extern boolean srchsched_mode_change_req(srchsched_mode_enum_type new_mode, wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_CLIENT_CHANGE_REQ
  
  DESCRIPTION
    This function will get called by an external client ID and it will request
    the scheduler to update the new client of the scheduler

  DEPENDENCIES
    None
  
  RETURN VALUE
    TRUE if the request is successful, FALSE if mode change validation fails
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_client_change_req(srchsched_client_id_enum_type client_id, wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_INACTIVE_MODE_INIT
  
  DESCRIPTION
    This function will get called whenever the scheduler enters INACTIVE mode.
    It will perform any variable/database initialization necessary for this mode. 

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_inactive_mode_init(wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_HANDLE_SRCH_OPS_CMD
  
  DESCRIPTION
    This function will get called by the WL1M module when a SRCH OPS local 
		command is posted. It checks whether a mode change is in progress and if yes
		it pends the current search. Otherwise is calls the search scheduler. 

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_handle_srch_ops_cmd(srch_ops_cmd_type *srch_ops_cmd_ptr, wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_PEND_ISSUED_SRCH
  
  DESCRIPTION
    This function will get called if a search command was issued while an abort is pending and it will pend
    the search sent.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_pend_issued_srch( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_START_ASU_PROC
  
  DESCRIPTION
    This function will get called if a search command was issued while an abort is pending and it will pend
    the search sent.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_start_asu_proc(boolean start_asu, wsub_id_e_type wsub_id);

extern void srchsched_handle_srch_done_cmd(srch_done_cmd_type *srch_done_cmd_ptr, wsub_id_e_type wsub_id);
extern void srchsched_abort_done_cmd_handler( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_RESET_SRCH_ISSUED

  DESCRIPTION
    This function will get called from functions such as aborting searches or when a search was aborted from FW. It will reset
    the current srch issued variable to that scheduler does not get stuck

  DEPENDENCIES
    None

  RETURN VALUE
    None

  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_reset_srch_issued( srch_intraf_queue_enum_type srch_queue_idx , wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_RESET_ASET_PN_SRCH_COUNT
  
  DESCRIPTION
    This function will get called from functions such as aborting searches or when a search was aborted from FW. It will reset
    the current srch issued variable to that scheduler does not get stuck

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_reset_aset_pn_srch_count( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_RESET_NSET_PN_SRCH_COUNT
  
  DESCRIPTION
    This function will get called from functions such as aborting searches or when a search was aborted from FW. It will reset
    the current srch issued variable to that scheduler does not get stuck

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_reset_nset_pn_srch_count( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_RESET_FULL_SRCH_COUNT
  
  DESCRIPTION
    This function will get called from functions such as aborting searches or when a search was aborted from FW. It will reset
    the current srch issued variable to that scheduler does not get stuck

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_reset_full_srch_count( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_GET_FULL_SRCH_TIMER_C0
  
  DESCRIPTION
    This function returns the full search timer for carrier 0. It is calculated based on whether WPLT is active or not.

  DEPENDENCIES
    None
  
  RETURN VALUE
    uint16
  
  SIDE EFFECTS
    None
=============================================================================*/
extern uint16 srchsched_get_full_srch_timer_c0(void);

/*=============================================================================
  FUNCTION
    SRCHSCHED_GET_FULL_SRCH_TIMER
  
  DESCRIPTION
    This function returns the full search timer for carrier 0. It is calculated based on whether WPLT is active or not.

  DEPENDENCIES
    None
  
  RETURN VALUE
    uint16
  
  SIDE EFFECTS
    None
=============================================================================*/
extern uint16 srchsched_get_full_srch_timer(uint8 carrier_idx, wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_GET_CURR_MODE
  
  DESCRIPTION
    This function will get called from the INTRAF search scheduler, when the client is DCH_CLIENT and it will send periodic
    measurement reports to RRC if needed.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern uint8 srchsched_get_curr_mode( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_GET_CURR_CLIENT
  
  DESCRIPTION
    This function will get called from the INTRAF search scheduler, when the client is DCH_CLIENT and it will send periodic
    measurement reports to RRC if needed.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern uint8 srchsched_get_curr_client( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_UPDATE_ALLOWED_SRCH_BMSK
  
  DESCRIPTION
   This function will be called from Clients which wants to change the allowed searches bit mask. The func
   will ignore the change if the bmask provided is not valid. It is the clients responsibility to provide the
   correct bitmask

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/

extern void srchsched_update_allowed_srch_bmsk(uint16 new_mask, wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_INIT_DB
  
  DESCRIPTION
    This function is called once during search_init when WL1 task is initialized

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_init_db( wsub_id_e_type wsub_id);

#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
#error code not present
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */

/*===========================================================================

FUNCTION SRCHSCHED_CHECK_IF_PARALLEL_STEP1_CAN_BE_ISSUED

DESCRIPTION
  This function checks if Primary and secondary carrier counters have expired and
  no CM search is going on
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

extern boolean srchsched_check_if_parallel_step1_can_be_issued( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHSCHED_CHECK_IF_PARALLEL_NASTT_CAN_BE_ISSUED

DESCRIPTION
  This function checks if Primary and secondary carrier counters have expired and
  no CM search is going on
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

extern boolean srchsched_check_if_parallel_nastt_can_be_issued( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHSCHED_CHECK_IF_PARALLEL_NASTT_CAN_BE_ISSUED_IN_CPC_MODE

DESCRIPTION
  This function checks if Primary and secondary carrier nastt is due.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
  
===========================================================================*/

extern boolean srchsched_check_if_parallel_nastt_can_be_issued_in_cpc_mode( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_MODE_CHANGE_REQ_FOR_CPHY_SETUP
  
  DESCRIPTION
    This function is when a cphy setup or cell transition mode change request is necessary. It will pass the new mode
    as well as a void function which will get set only if a search abort was issued to FW. This callback functino will get
    executed only at search abort done, if the callback is not NULL.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern boolean srchsched_mode_change_req_for_cphy_setup(srchsched_mode_enum_type new_mode, SRCHSCHED_MODE_CHG_SRCH_ABORT_POSTCB *post_cb, wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_RESUME_CPHY_SETUP_CELL_TRANS_OPERATIONS
  
  DESCRIPTION
    This function checks whether L1 has a callback for resuming cphy or cell trans operations and issues it if yes.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern void srchsched_resume_cphy_setup_cell_trans_operations( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHSCHED_SET_TRIAGE_FLAG

DESCRIPTION
  This function sets the triage flag in srchsched db

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchsched_set_triage_flag(boolean val, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_RXFE_START_INTERRUPT_HANDLER

DESCRIPTION     This function processes the rxfe start interrupt after RF is out of LPM mode

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_rxfe_start_interrupt_handler( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_INIT_SCCH_ACTIVATION_STATE

DESCRIPTION     This function put the scheduler in config mode before issuing scch activation hpq pn search

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/

extern void srchsched_init_scch_activation_state( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_PROCESS_SCCH_ORDER

DESCRIPTION     This function processes the hs scch order .

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_process_scch_order( uint8 scch_order_mask , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_INFORM_CME_UPON_SCCH_ORDER

DESCRIPTION     This function sends the scch order status to CME .

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_inform_cme_upon_scch_order( wsub_id_e_type wsub_id );

/*===========================================================================
FUNCTION        SRCHSCHED_IS_RXD_ASYNC_PARAM_UPDATE_ALLOWED

DESCRIPTION     This function checks if rxd async parameters can be update.
                          This depends on whether step1 search is in progress or not

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_is_rxd_async_param_update_allowed( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_REGISTER_RXD_ASYNC_PARAM_UPDATE_CB

DESCRIPTION     This function register callback for async parameter update in case step1 search is on progress. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_register_rxd_async_param_update_cb( SRCHSCHED_RXD_ASYNC_PARAM_UPDATE_CB_FUNC_TYPE async_upd_cb , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_UPDATE_RXD_ASYNC_PARAM

DESCRIPTION     This function calls rxd async update function 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_update_rxd_async_params( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_SRCH_INVALIDATION_MASK

DESCRIPTION     This function sets search invalidation mask for fast rxd tear down. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_srch_invalidation_mask( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_ASYNC_SRCH_PARAMS_AND_COUNTS

DESCRIPTION     This function initialized all the Step1 and NASTT reservation parameters at the beginning of each L1
                          state transition.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_async_srch_params_and_counts(srchsched_client_id_enum_type client_id, wsub_id_e_type wsub_id);

#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
#error code not present
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */

/*===========================================================================
FUNCTION        SRCHSCHED_CLEAR_INVALIDATION_MASKS

DESCRIPTION     This function invalidation mask for the queue specified. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_clear_invalidation_masks( srch_intraf_queue_enum_type queue_idx , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_COMPLETE_MODE_CHANGE_IF_PROGRESS

DESCRIPTION     This function invokes the scheduler to complete the mode change in case the search was aborted 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_complete_mode_change_if_progress( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_POST_MODE_CHANGE_COMPLETE

DESCRIPTION     This function posts local command to complete the mode change when FW aborts the ongoing search.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_post_mode_change_complete( wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_ABORT_ALL_SEARCHES
  
  DESCRIPTION
    This function will get called whenever a mode change request happens. It 
    will check whether there are any searches in progress and if yes, it will
    send an abort command to FW.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
extern boolean srchsched_abort_all_searches( wsub_id_e_type wsub_id);


/*=============================================================================
  FUNCTION
    SRCHSCHED_ISSUE_PERIODIC_MEAS_RPTS
  
  DESCRIPTION
    This function will get called from the INTRAF search scheduler, when the client is DCH_CLIENT and it will send periodic
    measurement reports to RRC if needed.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/
void srchsched_issue_periodic_meas_rpts(srch_periodic_meas_cmd_type *cmd_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHSCHED_INIT_NASTT_OPT_PARAMS

DESCRIPTION
  This function initializes parameters necessary for NASTT Power Optimization feature. It initializes the previous Step1
  peak, the booleans related to rxd state active for previous and current step1 result, as well as the RxAGC db used
  for variance calculation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchsched_init_nastt_opt_params( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHSCHED_SAVE_CURRENT_TOP_STEP1_PEAK

DESCRIPTION
  This function saves the current top step1 peak for NASTT power optimizations

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchsched_save_current_top_step1_peak( wsub_id_e_type wsub_id);


/*===========================================================================
FUNCTION        SRCHSCHED_GET_FULL_SRCH_TIMER

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern int16 srchsched_get_async_step1_srch_timer(uint8 carr_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_FULL_SRCH_TIMER

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_async_step1_srch_timer(uint8 carr_idx, int16 value, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_CURR_NASTT_FREQ_IDX

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern uint8 srchsched_get_curr_nastt_freq_idx( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_CUR_PSC_PEAK_INDX

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern uint16 srchsched_get_cur_psc_peak_indx(uint8 carr_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_CUR_PSC_PEAK_INDX

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_cur_psc_peak_indx(uint8 carr_idx, uint8 value, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_IS_FULL_SRCH_ACTIVE

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_is_full_srch_active( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_FULL_SRCH_ACTIVE

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_full_srch_active(boolean active, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_NASTT_SRCH_DUE

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_get_nastt_srch_due(uint8 carr_idx, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_NASTT_SRCH_DUE

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_nastt_srch_due(uint8 carr_idx, boolean nastt_due, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_STEP1_PEAK_CNT

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_step1_peak_cnt(uint8 carr_idx, uint16 step1_pk_cnt, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_NASTT_SRCH_360MS_TOGGLE

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_get_nastt_srch_360ms_toggle( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_SKIP_NASTT_CONDITIONS_MET

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_get_skip_nastt_conditions_met( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_SKIP_NASTT_CONDITIONS_MET

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_skip_nastt_conditions_met(boolean skip_nastt_cond_met, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_SRCH_CURR_NUM_STEP1_IN_DCH

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern uint8 srchsched_get_srch_curr_num_step1_in_dch(void);

/*===========================================================================
FUNCTION        SRCHSCHED_INCREMENT_CURR_NUM_STEP1_IN_DCH

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_increment_curr_num_step1_in_dch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_RXAGC_AT_STEP1

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern int16 srchsched_get_rxagc_at_step1(uint8 index, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_RXAGC_AT_STEP1

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_rxagc_at_step1(uint8 index, int16 value, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_IS_PREV_S1_RXD_ACTIVE_IN_DCH

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_is_prev_s1_rxd_active_in_dch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_IS_CURR_S1_RXD_ACTIVE_IN_DCH

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_is_curr_s1_rxd_active_in_dch( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_PREV_STEP1_PK

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern mcalwcdma_srch_step1_result_peak_type srchsched_get_prev_step1_pk( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_CURR_CLIENT

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern srchsched_client_id_enum_type srchsched_get_curr_client( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_ASET_PN_CNT

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern uint16 srchsched_get_aset_pn_cnt( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_ASET_PN_CNT

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern uint16 srchsched_get_nset_pn_cnt( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_INCREMENT_ASET_PN_CNT

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_increment_aset_pn_cnt( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_INCREMENT_NSET_PN_CNT

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_increment_nset_pn_cnt( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_ASET_PN_CNT

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_aset_pn_cnt(uint16 value, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_NSET_PN_CNT

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_nset_pn_cnt(uint16 value, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_SRCH_ISSUED

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern srch_ops_code_enum_type srchsched_get_srch_issued( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_FET_ACTIVE

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_fet_active(boolean active, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_PN_SET_INVALIDATED

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern boolean srchsched_get_pn_set_invalidated( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_PN_SET_INVALIDATED

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_set_pn_set_invalidated(boolean invalidate, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_SET_SKIP_NASTT_CONDITIONS_MET

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern uint8 srchsched_get_curr_num_step1_in_dch( wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION        SRCHSCHED_QSH_DUMP_CB

DESCRIPTION     This function collects srchsched info needed for QSH module 
				during crash. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_qsh_dump_cb(wl1_srcher_mini_debug_info_struct_type *addr_ptr_t, wsub_id_e_type wsub_id);
#endif

/*===========================================================================
FUNCTION        SRCHSCHED_GET_STEP1_TYPE

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern srch_ops_code_enum_type srchsched_get_step1_type(uint8 carr_idx);

/*===========================================================================
FUNCTION        SRCHSCHED_GET_STEP1_TYPE

DESCRIPTION  This function returns the full search timer for the specified carrier IDX.

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern srch_ops_code_enum_type srchsched_get_nastt_type(uint8 carr_idx);

#ifndef FEATURE_WCDMA_TABASCO_MODEM 
/*===========================================================================
FUNCTION        SRCHSCHED_MODE_CHANGE_ON_STOP_QTA

DESCRIPTION     This function will be called only after QTA stop and the 
    		winning client is 1x. It will change the scheduler mode back
		to its appropriate mode depending on its mode prior to QTA gap. 

DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_mode_change_on_stop_qta(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION        SRCHSCHED_CHECK_MODE_CHANGE_TO_SUSPEND_FOR_QTA

DESCRIPTION     This function will take care of moving the scheduler to suspended state
                before QTA gap if the winning client is 1x. This is because
                all searches need to be aborted in QTA gap if the winning client is 1x.
                
DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/
extern void srchsched_check_mode_change_to_suspend_for_qta(wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_TABASCO_MODEM */


#ifdef FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED
#error code not present
#endif /* FEATURE_FULL_SRCH_FRAME_RESERVATION_ENABLED */

/*===========================================================================

FUNCTION SRCHSCHED_INIT_ASYNC_NASTT_PARAMS

DESCRIPTION:
  This fuction is Initialize ASYNC NASTT parameters.
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

extern void srchsched_init_async_nastt_params(wsub_id_e_type wsub_id);

/*=============================================================================
  FUNCTION
    SRCHSCHED_IS_NASTT_SRCH_DUE_IN_INTRAF_MODE
  
  DESCRIPTION
    This function checks the counters for nastt search for all carrier and returns 
    the srch type if nastt is due.

  DEPENDENCIES
    None
  
  RETURN VALUE
    None
  
  SIDE EFFECTS
    None
=============================================================================*/

extern srch_ops_code_enum_type srchsched_is_nastt_srch_due_in_intraf_mode( wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION         SRCHSCHED_CLEAR_HB_AND_OPS_CMD

DESCRIPTION   The function clear pending SRCH OPS from queue and turn off HB.
 
DEPENDENCIES    None

RETURN VALUE    None

SIDE EFFECTS    None
==========================================================================*/

extern void srchsched_clear_hb_and_ops_cmd(wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
/*===========================================================================
  FUNCTION     srchsched_qsh_mdump_collect

  DESCRIPTION
  This function dumps all the required parameters from srchsched external 
  control parameter structure

  DEPENDENCIES
  None

  RETURN VALUE
  None

  SIDE EFFECTS
  None
  ===========================================================================*/
extern QSH_MDUMP_FN_ATTR void srchsched_qsh_mdump_collect(wsub_id_e_type wsub_id);
#endif

#endif /* !SRCHSCHED_H */
