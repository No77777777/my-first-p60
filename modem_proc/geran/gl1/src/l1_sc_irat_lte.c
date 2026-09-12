/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        INTERRAT (LTE) SURROUND CELL

GENERAL DESCRIPTION
   This module contains routines specific to performing LTE interRAT
   measurements while in GSM.

EXTERNALIZED FUNCTIONS

      l1_sc_lte_process_timed_search_results
      l1_sc_save_meas_lte_cells
      l1_sc_save_det_lte_cells
      l1_sc_prune_meas_lte_cells
      l1_sc_populate_g2l
      l1_sc_cm_g2l_state
      l1_sc_idle_g2l_state
      l1_sci_g2l_idle_aborted
      l1_g2l_cm_abort
      l1_sc_set_ml1_init
      g2l_init_cm
      g2l_set_cm_inactive
      g2l_start_idle_lte_search
      l1_g2l_trigger_mdsp
      l1_sc_g2l_idle_abort
      l1_sc_g2l_fast_search_start
      l1_sc_g2l_fast_search_restart
      l1_sc_g2l_fast_search_abort
      l1_sc_g2l_fast_search_top3
      l1_sc_g2l_fast_search_blacklist
      l1_sc_g2l_fast_search_dedicated
      l1_sc_g2l_fast_search_expiry


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2002-2015 Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_irat_lte.c#8 $
$DateTime: 2020/02/10 05:49:45 $ $Author: pwbldsvc $


when       who       what, where, why
--------   --------  ----------------------------------------------------------
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
04/02/20   nv		CR2615257 Reverting the hack made to handle L1_TIME_LINE_DELAY scenario in HA as part of FR:51554
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
06/08/19   sal       CR2488908 If Cleanup cmd is not immediate to GFW then wait one frame to let cleanup complete. 
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
17/09/19   sc        CR2439599 FR 56377 Elevator Sensor Ind for G2L 
28/03/17   ng        CR2214222 Modify handling of g2l abort from g2l_trigger_mdsp as per TA.3.0 
28/02/18   km        CR2189789 Handling g2l abort from g2l_trigger_mdsp
20/02/18   shm       Change Request 2192580 : KW P1 errors on TA.3.0 fix 
15/02/18   ksb       CR2189644 Avoid giving immediate cleanup before startup offset when G2L aborted early 
09/02/18   km        CR2184116 Set g2l_abort to TRUE in case of QTA gap abort
13/12/17   ksb       CR2152375 Schedule two G2L meas in S+M gap before the reselection  timer expiry
18/01/18   km        CR2172348 Addition of GTOL_SRCH_STATE_ABORT_CNF_WAIT in IRAT sm
09/01/18   km        CR2166879 G2L abort handling in QTA/non-QTA scenarios
16/08/17   km        CR2089992 DTF handler to abort G2L IRAT search in idle mode
02/01/18   km        CR2139831 In idle mode, schedule G2L search on same freq if LTE aborted search internally
06/10/16   dg        CR1073625 Don't set a LTE Frequency to no cell found if it was measured previously and if its band is invalid during current G2L search
15/11/16   bg        CR1089997 Fix for KW errors
15/12/17   km        CR1049586 Send G2X cleanup through ASYNC to avoid executing before G2X startup
06/12/17   km        CR973889 Idle Timeline Opt - Sleep between PCH and IRAT
05/12/17   km        CR987605 G2X immediate cleanup
07/12/17   km        CR2152244 G2L search is getting delayed after entering into idle mode from PTM
07/12/17   km        CR2125715 G2L Time transfer Improvement, skip MEAS only gaps if no cell is detected in S+M
07/12/17   km        CR2101400 G2L Time transfer Improvement, do S+M every 20/40 secs and M every 5/10 secs
30/11/17   km        CR2150931 Acquire ISR_LOCK while processing G2L timed search results
04/08/17   akm       CR2086464 Clear empty QTA flag for G2L search in QTA gap
13/04/17   zf        CR2033453: Fix KW error.
15/11/17   ng        CR1021532 Make G2L Null2X (and changes for all G2X as Null2X)
10/11/17   akm       CR2141440 QTA start didn't happen properly in G2L QTA gap
23/01/17   cc        CR1114369 Reset freq age after leaving xfer mode
29/10/17   km        CR2133917 GL1 support for L+L; FR37926 non-DDS sub IRAT optimization
30/08/17   ksb       CR2099611 Improvement over Time tansfer CR 1010614 to reduce the search inetrval to 20 sec
16/02/17   cc        CR1010614 G2L idle time transfer and MEAS only support
27/07/16   zf        CR1046704: Using MSGR variant
17/11/16   akm       CR1090724 JO.3.0 P1 compilation issues due to new KW version
19/09/16   dg        CR1066652 Make MCVS request of 384 mhz 1 frame in advance before initiating G2L search in dedicated mode
25/08/16   dg        CR1056221 Send MCVS req of 384 MHZ while initiating any G2X IRAT search in dedicated mode
17/06/16   ng        CR1030473 Send MCVS request of 384 MHZ at the time of initiating G2X search when GL1 is in Transfer mode (modify fix of CR 1021833 & 1025293)
10/06/16   ng        CR1025293 Extend fix of CR 1021833: Send MCVS request of 384 MHZ at the time of initiating G2X search with HMSC ON
10/06/16   dg        CR1022165 Don't blacklist top3 LTE EARFCN's during G2L fast search
31/05/16   dg        CR1021833 Send MCVS request of 384 MHZ at the time of initiating G2L search with HMSC ON
06/03/2016 ng        CR1022465 Clear drx_ref once IRAT activity gets scheduled
26/05/16   sk        CR1020757 Add DTF events for G2X IRAT scenarios
17/05/16   dp        CR1012917 When waking up for G2L, do load FW/RF in current context rather than task context
30/12/15   js        CR956837 Set DRX reference only when IRAT activity is reserved
25/11/15   cja       CR942064 Add mcvs speed bump for G2L and G2T in dedicated mode 
23/11/15   cgc       CR944459 Fix regression due to CR937488, add tuneback_to_gsm= FALSE also requires change in GFW 
16/11/15   cgc       CR937488 Add l1_sc_g2l_schedule_abort() idle abort functionality when in scheduled state
14/10/15   dg        CR919577 Don't measure blacklisted EARFCN in Packet Transfer Mode
17/09/15   dg        CR907489 Try to dynamically request frames for G2L fast search in idle mode
26/08/15   dg        CR895008 Set active rat to LTE while doing index recovery at the time of initiating G2L search
01/09/15   cgc       CR897441 Add missing TRM band registration, renamed to, MAX_FRAMES_FOR_IRAT_CONNECTED_MODE
26/08/15   br        CR896799 Reset variable"ml1_gap_init" if measurement is skipped.
10/08/15   cgc       CR889913 Abort mechanism, G2L_CM_STOP must also check for timed_cnf pending
06/08/15   cgc       CR886693 Corrections to CR882971 ensure grm_check_ptm_irat_sc_activity_ok() only active once.
05/08/15   cgc       CR885259 race condition timed_cnf received during gap cleanup ensure l1_sc_set_gtow_srch_state_null() gets called.
31/07/15   cja       CR881129 Fix deadlock due gl1_drx_require_next_tick inside irat mutex lock
31/07/15   cah/cc    CR882971 Gl1 Crash in SDCCH due to invalid diversity dev_id
29/07/15   cgc       CR865560 For Fast Search run G2X_cleanup frame earlier using mdsp_async_intf_send_immediate_cmd()
21/07/15   cgc       CR876899 revert CR866735, make G2L sm behave as was on bolt.
22/07/15   ap        CR877707: Use TRM_RX_BEST/TRM_RX_ANY/TRM_TX_BEST instead of TRM_RXTX_ANY and TRM_RXTX_BEST
18/07/15   zf        CR875612: Don't reset g2l gap state in gap CNF.
13/07/15   cja       CR866307 G2T and G2L in QTA gap
06/07/15   jj        CR866735 make wsrch state  GTOW_SRCH_STATE_CLEANUP once mdsp_g2x_cleanup_cmd send
12/07/15   og        CR867635. Decrement the cnf index if the sending of the G2L search req fails.
24/06/15   dg        CR860598 Clear FM entry for G2L in the same frame on which cleanup would be issued to GFW
06/07/15   zf        CR865792: Start ID search when current frame is equal or later than ID search FN
01/07/15   cgc       CR853993 fix reprune totals error reset num_detected_cells
28/06/15   zf        CR860341: Apply mutex protection on fast search state
17/06/15   jj        CR 856570  fix the regression 853609
12/06/15   cgc       CR853609 moved g2l_irat_x2l_meas_enter() to after api call lte_ml1_irat_setup_schedule()
17/06/15   sv        CR856025 Revert CR853609
12/06/15   cgc       CR853609 moved g2l_irat_x2l_meas_enter() to after api call lte_ml1_irat_setup_schedule()
08/06/15   cgc       CR849570 l1_g2l_connected_status() add check for change to SDCCH each gap.
04/06/15   cja       CR848107 G2L to use cleanup state, so extra frame for GFW cleannup
26/03/15   cgc       CR814333 Fix linker errors for non FEATURE_GSM_TO_LTE build
20/05/15   cgc       CR840322 modify g2l_get_next_lte_freq_with_band_chk() add G2L idle reprune
14/05/15   cgc       CR833569 in l1_g2l_trigger_mdsp() use g2l_set_cm_inactive() for mdsp_end state, add G2L_MDSP_NULL
13/05/15   ar        CR837482 Aborting G2L search causes srch state to return to  NULL before CNF is received from ML1
07/05/15   zf        CR833569: When G2L gap ends, only changing srch state to NULL if active RAT is LTE.
05/05/15   cgc       CR833161 update g2l_idle_frame frame value for each connected mode G2L gap.
24/04/15   cja       CR800657 QTA gap for G2W IRAT
12/03/15   br        CR801829 GL1 to discard the measurement, if the RAT freq_idx is not correct
14/04/15   aga       CR799530 Reserve W/TD/L IRAT ID search with TRM in advance
10/04/15   cgc       CR820213 Add g2l_get_next_lte_freq_with_band_chk() add trm band registration for fast search.
09/04/15   cgc       CR819956 Thor 2.x RF API change, G2W use MSGR
31/03/15   cja       CR800650 Optimise irat state machine
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
11/03/15   cgc       CR798584 G2L dedicated, only reprune 1 measurement result, remove 2nd reprune.
05/03/15   sjv       CR804008 Populate support_gap_abort_before_tune flag in LTE search meas. req. in connected mode
06/03/15   cgc       CR794323 FR25799 support x8 LTE with fast idle search.
03/04/15   br        CR763734 GL1 logging improvement to ease debugging
24/01/15   jj        CR 799441 Active irat should not reset until id state becomes NULL
17/02/15   sjv       CR796616 Moving g2l_rf_meas_exit handling as part of G2L Abort inside l1_sc_lte_process_timed_search_results function
13/02/15   pa        CR756156:G2X RF device handling using TRM apis.
11/02/15   sjv       CR791268 G2L Abort support in case ML1 aborts
02/02/15   cgc       CR788618 update gap_start_asap flag while sending meas_req to LTE to TRUE for Idle meas.
27/01/15   npt       CR697236 Mainline FEATURE_GERAN_VSTMR
27/01/15   cgc       CR782109 G2L cm abort only check for G2L_CM_CNF_PENDING.
23/01/15   npt       CR786002 Fix compilation errors when FEATURE_DUAL_SIM is enabled
21/01/15   jj        CR784462 remove FEATURE_THOR_G2X_MSGR_REWORK
21/01/15   npt       CR783249 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled
16/01/15   cgc       CR782093 G2L Idle xfer mode overlap clears common control variable l1_sc_trigger_g2l_gap in error
12/01/15   sjv       CR756154 Enhance Band Registration for Activities
22/12/14   cgc       CR774325 THOR_G2L_REWORK fix omission reset receive_in_progress
16/12/14   cgc       CR765347 Call g2l_reset_stored_det_cnts() once per measurement session for timed searches.
12/12/14   cgc       CR769137 protect against G2L idle/ded xxx_cnf overlaps.
12/12/14   cgc       CR769790 Fix typo for GL1_ISR_UNLOCK -> GL1_IRAT_UNLOCK
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
05/12/14   cgc       CR766117 GFW requires another frame to complete dedicated G2L cleanup
03/12/14   npt       CR759467 Fix compiler warnings
03/12/14   cgc       CR715394 G2L code Idle/Dedicated rework FEATURE_THOR_G2L_REWORK
13/10/14   jk        CR723769:Set rfTune2x to FALSE while sending G2X start up command for G2L timed searches
09/10/14   cgc       CR734430 G2L pruning algorithm to consider detection only scenarios
07/11/14   jj        CR752569 Fix thor bring up issues
05/11/14   cgc       CR743421 add G2L_MDSP_FINISH_EARLY, mdsp_g2x_cleanup_cmd() is only sent from isr for G2L cleanup.
30/10/14   pa        CR748275: In DRDSDS (Transfer Mode): allow IRAT if band registration is successful.
17/10/14   jj        CR741205 moving  G2X CNF from l1 task to MSGR
14/10/14   cgc       CR738236 G2L idle measurement finishes early, add GTOW_SRCH_STATE_LTE_CLEANUP_WAIT to prevent sleep.
03/10/14   cgc       CR733923 KW fix in g2l_lte_list_index_recovery()
25/09/14   cgc       CR728533 cell list change causes G2L stuck in GTOW_SRCH_STATE_SCHEDULE_LTE
16/09/14   cgc       CR723214 Avoid forced stale irat measurements when sub1 is blocked by sub2 MO/MT
22/09/14   cgc       CR727137 LTE api additions add gl1_hw_get_lte_irat_rf_device() to set lte_device_id
09/09/14   cgc       CR721718 FEATURE_GSM_TO_LTE changes to fix compile errors for non-lte build
03/09/14   cja       CR717826 Prevent dedicated G2X and WTR hop at the same time
28/08/14   cgc       CR717278 G2L dedicated scheduling changes,refinement of CR700929
26/08/14   pa        CR715657 Band deregistration on finishing IRAT activity.
29/08/14   cah       CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
22/08/14   cgc       CR713634 obsoleted G2L code and featurisations FEATURE_GSM_G2L_TIMED_SRCH and FEATURE_CM_G2L removed
20/08/14   pa        CR712437: Clear FM activity after G2L Idle abort.
13/08/14   dg        CR690573 Remove compilation error
16/07/14   dg        CR690573 Don't schedule G2L IRAT meas. on the warm-up ISR
08/08/14   cgc       CR706185 additions to CR682787 for BO2.0 G2L
08/08/14   cgc       CR682787 Hopping conflict fix for G2X on SDCCH propagation from DI2.3
07/08/14   cgc       CR700929 update ded_tick after G2L measurement completed
05/08/15   cgc       CR704038 Missing var in merge causing G2L/ncell clash
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
24/06/14   dv/cah    CR697365 ensure wcdma_srch_in_progress flag is reset
08/07/14   dg        CR690007 WHile moving from IDLE to CM mode, don't skip the current freq. offset
22/07/14   cah       CR697586  trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
16/06/14   cgc       CR680118 remove LTE_GAP_LEAVE_CM_REQ code not applicable to Bolt
29/05/14   cgc       CR669540 Modify GTOW_SRCH_STATE_MULTI_SIM_FW_WAIT to use G2L timed search
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS: Scell measurement changes
14/02/14   cgc       CR639886 Changes for FR19574 add priority search for dedicated FEATURE_SGLTE_G2X_CONN_MODE_RESEL.
24/03/14   sd        CR630989 Pass ML1 RF  Device ID for G2L
23/04/14   pa        CR650458:G2L measurement results are not getting processed due to bad index
11/04/14   pg        CR645875 Fix Bolt multisim compilation errors
10/02/14   cja       CR612218 For G2L add meas enter and meas exit for LTE RF
25/11/13   pa        CR579842:Incorporate CR533574 and issue cleanup immediately after receiving results.
11/03/02   tb        Initial version.

===========================================================================*/


#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

#ifdef FEATURE_GSM_TO_LTE

#include "l1_sc_irat.h"
#include "l1_sc_lte_irat.h"

#include "gpl1_dual_sim.h"
#include "gl1_hw_g.h"
#include "gpl1_grm_intf.h"
#include "gl1_msg_wtr_hop.h"
#include "l1i.h"
#include "gpl1_gprs_task.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

#define G2L_CM_ACTIVE_AND_CNF_PENDING    (G2L_CM_ACTIVE | G2L_CM_CNF_PENDING)

/*
** allow a margin for delayed start of GFW isr
*/
#define G2X_FAST_START_START_OF_FRAME_QS    (625)
#define G2X_FAST_START_END_OF_FRAME_QS     (4375)
#define G2X_CLEANUP_OFFSET_MARGIN           (600)
#define G2L_FAST_RESTART_CLEANUP_OFFSET    (G2X_FAST_START_END_OF_FRAME_QS - G2X_CLEANUP_OFFSET_MARGIN)

/* G2L search will hapeen at every 20/40 sec */
#define G2L_SEARCH_INTERVAL                 (4)

/*
** used only by ML1 and passed into each timed req/cnf
*/
lte_ml1_irat_gap_schedule_gap_state_s     ml1_gap_state[NUM_GERAN_DATA_SPACES];                    /*  {0}; */

boolean l1_sc_g2l_build_script_pending[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(FALSE)};
boolean l1_sc_g2l_empty_qta_gap[NUM_GERAN_DATA_SPACES]        = {INITIAL_VALUE(FALSE)};

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
/*Track cleanUp command sent to GFW is Sync or Async*/
boolean cleanup_cmd_sent[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(FALSE)};
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */


/*
** FEATURE_NEW_RF_API  new data stores
*/
static lte_cphy_irat_meas_rf_param_s         g2l_meas_enter_params[NUM_GERAN_DATA_SPACES];

static l1_rat_time_s ggap[NUM_GERAN_DATA_SPACES] =
{
  {
      {INVALID_FN, G2L_GAP_START_OFFSET},       /* G2L gap start offset is fixed */
      {INVALID_FN, G2L_GAP_END_OFFSET},         /* G2L gap end   offset is fixed */
      {INVALID_FN, G2L_GAP_CLOCK_OFFSET}        /* G2L gap clock offset is fixed */
  }
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  , {
      {INVALID_FN, G2L_GAP_START_OFFSET},       /* G2L gap start offset is fixed */
      {INVALID_FN, G2L_GAP_END_OFFSET},         /* G2L gap end   offset is fixed */
      {INVALID_FN, G2L_GAP_CLOCK_OFFSET}        /* G2L gap clock offset is fixed */
    }
#if defined(FEATURE_TRIPLE_SIM)
  , {
      {INVALID_FN, G2L_GAP_START_OFFSET},       /* G2L gap start offset is fixed */
      {INVALID_FN, G2L_GAP_END_OFFSET},         /* G2L gap end   offset is fixed */
      {INVALID_FN, G2L_GAP_CLOCK_OFFSET}        /* G2L gap clock offset is fixed */
    }
#endif
#endif
};


/* moved these 2 lte specific functions from l1_sc_irat.c */
static void   l1_sc_save_det_lte_cells(  uint32 freq_idx, uint32 num_cells, lte_cphy_irat_meas_detected_cell_s *det_ptr,  gas_id_t gas_id );
static void   l1_sc_save_meas_lte_cells( uint32 freq_idx, uint32 num_cells, lte_cphy_irat_meas_meas_results_s  *meas_ptr, gas_id_t gas_id );

static void   l1_sc_prune_meas_lte_cells( uint32 freq_idx, boolean g2l_idle, boolean rePrune, boolean cmSkipFirst, gas_id_t gas_id);

static void   g2l_tdd_best_sss_corr(boolean searchPerformed, uint32 freq_idx, gas_id_t gas_id);
static void   g2l_move_next_freq(gas_id_t gas_id);
static void   g2l_set_next_freq(uint16 freq_idx, gas_id_t gas_id);
uint32        l1_sc_get_g2l_frame_number(uint32  g2l_idle_offset_in, gas_id_t gas_id);
static void   g2l_reset_stored_det_cnts(gas_id_t gas_id);
static void   g2l_lte_list_index_recovery(gas_id_t gas_id);
static void   g2l_start_idle_lte_search(gas_id_t gas_id);
static void   g2l_init_cm(gas_id_t gas_id);

static boolean g2l_request_or_reserve_lte_meas(gas_id_t gas_id);
static boolean g2l_can_lte_meas_go(uint32 frames_required, gas_id_t gas_id);

static void   g2l_gap_cnf( boolean g2l_idle , boolean aborted, gas_id_t gas_id);
static void   g2l_process_state_cnf( boolean g2l_idle, uint16 freq_idx ,boolean aborted, gas_id_t gas_id);

static void   g2l_get_lte_device( lte_cphy_irat_meas_target_rf_param_s *ptr_trgt , lte_earfcn_t lte_earfcn, gas_id_t gas_id);
static uint16 g2l_get_next_lte_freq_with_band_chk( uint16 freq_idx, gas_id_t gas_id);

/*
** additions for FR25799
*/
#define   FS_SEARCH_OVERHEAD (4)

/*
** frames for +30sec
*/
#define   FS_FRAMES_FOR_30S    (6500)

static boolean                 l1_sc_g2l_fast_search_restart( gas_id_t gas_id);
static void                    l1_sc_g2l_fast_search_top3(gas_id_t gas_id);
static void                    l1_sc_g2l_fast_search_dedicated(gas_id_t gas_id);
static g2x_fast_search_state_e l1_sc_g2l_fast_search_blacklist( gas_id_t gas_id);
static uint16                  g2l_get_next_lte_freq(uint16 last_freq , gas_id_t gas_id);

/*===========================================================================

FUNCTION l1_sc_get_g2l_frame_number

DESCRIPTION
  Calculates the absolute frame number from now + g2l_idle_offset, the point in the future.
  This is the frame number passed to ML1 to trigger the gap measurement.

DEPENDENCIES
  none

RETURN VALUE
  frame_number + g2l_idle_offset

SIDE EFFECTS
  None

===========================================================================*/

uint32 l1_sc_get_g2l_frame_number(uint32  g2l_idle_offset_in , gas_id_t gas_id)
{
    uint32 next_g2l_frame;

#ifdef FEATURE_DUAL_SIM
    next_g2l_frame = gstmr_rd_hw_frame_count(geran_map_gas_id_to_nas_id(gas_id));
#else
    next_g2l_frame = gstmr_rd_hw_frame_count();

    (void)gas_id;

#endif /*FEATURE_DUAL_SIM*/

    next_g2l_frame = ADD_FN(next_g2l_frame, g2l_idle_offset_in );  /* idle-1 frame is +2 from now */

    return next_g2l_frame;

} /* l1_sc_get_g2l_frame_number */

/*===========================================================================

FUNCTION g2l_set_next_freq

DESCRIPTION
  For multiple L freq, set the next freq to be measured

DEPENDENCIES
  none

RETURN VALUE
  frame_number + g2l_idle_offset

SIDE EFFECTS
  None

===========================================================================*/
static void g2l_set_next_freq(uint16 freq_idx, gas_id_t gas_id)
  {
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint16 f_off;

  f_off = l1_sc_wcdma_get_ded_next_srch_freq(FALSE, gas_id);

  if (f_off >= MAX_WCDMA_FREQS)
  {   f_off  = MAX_WCDMA_FREQS - 1; }

  /*Don't schedule EARFCN whose status is blacklisted */
  if( (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_LTE )
      && (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].freq_status != G2X_FS_BLACKLISTED))
  {
     l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx = f_off;   /* else no change, use same */
     MSG_GERAN_HIGH_2_G("G2L: Change to next freq idx=%d next new idx=%d", l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset, l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx);
  }
  else
  {
    l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx = freq_idx;   /* use the same freq */
  }

} /* g2l_set_next_freq */

/*===========================================================================

FUNCTION  l1_sc_g2l_init

DESCRIPTION
             new G2L init for both idle and connected if needed
             This is a one off call from l1_sc_wcdma_init()

DEPENDENCIES
  none

RETURN VALUE
  various

SIDE EFFECTS
  None

===========================================================================*/

void l1_sc_g2l_init(gas_id_t gas_id )
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    /* add any Idle init required */
    l1_sc_wcdma_data_ptr->lte_cm_irat_active = G2L_CM_INACTIVE;

    g2l_init_cm(gas_id);
}



/*===========================================================================

FUNCTION  g2l_move_next_freq

DESCRIPTION
             Move to next frequency after receiving last CNF.

DEPENDENCIES
  none

RETURN VALUE
  various

SIDE EFFECTS
  None

===========================================================================*/
static void g2l_move_next_freq(gas_id_t gas_id)
{

  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  if(TRUE==l1_sc_wcdma_data_ptr->rr_params.use_next_freq)
  {
    uint16  cur_idx  = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;
    uint16  next_idx = l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx;

    l1_sc_wcdma_data_ptr->rr_params.use_next_freq = FALSE;

    /*
    ** if the next freq is not the same move to next LTE freq and is not blacklisted
    */
    if(( next_idx != l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset) 
    &&(l1_sc_wcdma_data_ptr->rr_params.freq_list[next_idx].freq_status != G2X_FS_BLACKLISTED))
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset = next_idx;
      l1_sc_wcdma_data_ptr->rr_params.currFreqIdx                = next_idx;

      /*
      ** we have used this "nextFreqIdx" so set the next+1
      */
      g2l_set_next_freq(next_idx, gas_id);

      l1_sc_wcdma_data_ptr->g2l_inhibit_next_update = TRUE;

      l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx].best_lte_tdd_cell_idx = INVALID_IDX; /* reset for TDD sss_corr */
    }
    else
    {
      l1_sc_wcdma_data_ptr->g2l_inhibit_next_update = FALSE;
    }

    MSG_GERAN_HIGH_3_G("G2L: Move to next freq: prev idx=%d next idx=%d inhibit=%d", cur_idx, next_idx, l1_sc_wcdma_data_ptr->g2l_inhibit_next_update );
  }

} /* g2l_move_next_freq */

/*===========================================================================

FUNCTION g2l_tdd_best_sss_corr

DESCRIPTION
             For LTE TDD freq , determines the best cell for the next gap measurement.

DEPENDENCIES
  none

RETURN VALUE
  various

SIDE EFFECTS
  None

===========================================================================*/
static void   g2l_tdd_best_sss_corr(boolean searchPerformed, uint32 freq_idx, gas_id_t gas_id)
{

 uint32  i          =0;
 uint32  best_so_far=0;

 l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    if( searchPerformed
         && (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells)
         && (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].lte_type == RR_L1_EUTRAN_TYPE_TDD)
      )
    {
       if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].best_lte_tdd_cell_idx == INVALID_IDX)
       {
          /*
          ** on the last gap of seq, no det is carried out, so ML1 reset sss_corr to 0
          ** so we don't want to use 0 for the first gap.
          */
          if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[0].lte_cell.state.sss_corr)
          {
            i = 0;   /* start with sss_corr of first entry */
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].best_lte_tdd_cell_idx = 0;
          }
       }
       else
       {
         /*
         ** get last value index and update with next best value.
         */
         i = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].best_lte_tdd_cell_idx;
       }

       best_so_far = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.state.sss_corr;

       MSG_GERAN_HIGH_3_G("G2L: Start Vcell_info for idx=%d Best TDD cell=%d sss_corr=%d",
                                i,
                                l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cell_id,
                                best_so_far);

       /*
       ** search for new best cell
       */
       for( i=0; i < l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells; i++ )
       {
          /*
          ** update for best value
          */
          if(best_so_far < l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.state.sss_corr)
          {
             best_so_far = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.state.sss_corr;
             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].best_lte_tdd_cell_idx = i;
          }
       }

       if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].best_lte_tdd_cell_idx != INVALID_IDX)
       {
          i = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].best_lte_tdd_cell_idx;

          MSG_GERAN_HIGH_3_G("G2L: Vcell_info for Freq=%d Best TDD cell=%d sss_corr=%d",
                                l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn,
                                l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cell_id,
                                best_so_far);
       }
       else
       {
          MSG_GERAN_HIGH_3_G("G2L: Vcell_info NOT set for Freq=%d num_cells=%d performed=%d",
                                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn,
                                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells,
                                            searchPerformed);
       }
    }
    else
    {
        if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].lte_type == RR_L1_EUTRAN_TYPE_TDD)
        {
           MSG_GERAN_HIGH_3_G("G2L: NO Vcell_info for Freq=%d num_cells=%d performed=%d",
                                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn,
                                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells,
                                            searchPerformed);
        }
    }
} /* g2l_tdd_best_sss_corr */

/*===========================================================================

FUNCTION l1_sc_prune_meas_lte_cells

DESCRIPTION
             freq_idx is for LTE rat
             any det cell present in meas list to be removed from det list, adjust counts, update state for det cell

DEPENDENCIES
  none

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/

static void l1_sc_prune_meas_lte_cells(uint32 freq_idx,  boolean g2l_idle, boolean rePrune, boolean cmSkipFirst, gas_id_t gas_id)
{
 l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

 uint32 PruneLoopCount;

 uint16 det_cells, cell_id, total_cells,i,j,k;

 uint16 meas_cells = 0;

 uint8 id_tick_update_critreria = 0;

 total_cells = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;          /* those requested to be measured + new detected */
 det_cells   = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells; /* only newly detected cells */

  /* Keep KW happy */
 if( total_cells > G2L_MEAS_DET_MAX_CELLS  )
   { total_cells = G2L_MEAS_DET_MAX_CELLS; }

 if( det_cells > G2L_MEAS_DET_MAX_CELLS  )
   { det_cells = G2L_MEAS_DET_MAX_CELLS; }

 if(total_cells > det_cells)
 {
    meas_cells  = total_cells - det_cells;   /* number of existing measured cells */
 }
 else
 {
    /*
    ** only detected cells present, so no pruning occurs
    */
    if(rePrune == FALSE)  /* is not a reprune request */
    {
      /*
      ** if det_cells=0 then update any existing meas_cnts
      */
      if(!det_cells && !cmSkipFirst)  /* skip on first gap as only AGC is performed in cm, n/a to idle */
      {
         /*
         ** check for any existing cells in store_meas,
         ** this is not very likely unless state changes/cellupdate lists have occurred
         ** the num_cells for the two data stores should always tally after a measurement.
         */
         meas_cells  = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
 
         if( meas_cells > G2L_MEAS_DET_MAX_CELLS  )  /* KW */
           { meas_cells = G2L_MEAS_DET_MAX_CELLS; }
 
         /*
         ** if only detected cells =0, tick any existing meas_cnts so that prune will occur for any stale cells
         */
         for(i=0; i < meas_cells; i++)
         {
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].meas_cnt++;
         }
 
         /* No cell is detected so, skip remaining MEAS only gaps and re-issue SRCH+MEAS every 20/40 secs. It should come at this point only during SRCH+MEAS gap so, freq_age is 0 */
         #ifdef FEATURE_G2L_TIME_TRANSFER
         if (g2l_idle && (!l1_sc_wcdma_data_ptr->elevator_flag))
         {
           if (l1_sc_g2x_check_for_extend_srch_period(gas_id))
           {
             l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] += (G2L_SEARCH_INTERVAL-1) * (l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte << 1);
           }
           else
           {
             l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] += (G2L_SEARCH_INTERVAL-1) * (l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte);
           }
           id_tick_update_critreria = 1;
         }
         #endif
      }
 
      MSG_GERAN_MED_5_G("G2L: Only Detected Cells No Prune req'd Total=%d Meas=%d Det=%d id_tick=%d update_critreria=%d",
                        total_cells,
                        meas_cells,
                        det_cells,
                        l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx],
                        id_tick_update_critreria);
 
      return;
    }
 }

 if(rePrune == FALSE)  /* for normal gap updates */
 {
  #ifdef FEATURE_G2L_TIME_TRANSFER
  if (g2l_idle)
  {
    /* Counter for age, we need to fall back to do SEARCH_AND_MEAS for every 5 MEAS */
    l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age++;

    /* freq age = 1 will be SRCH + MEAS */
    if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age == 1)
    {
      /* To check if there are cells not being detected in this SRCH + MEAS cycle,
       we will increment the detection fail count on the failed cells. In case 
       there is any cell not being detected, we re-issue SRCH + MEAS in the next gap. */
      /* Loop through meas list */
      for (i = 0; i < meas_cells; i++)
      {
        cell_id = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cell_id;

        /* Loop through det list */
        for (j = meas_cells; j < total_cells; j++)
        {
          if (cell_id == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[j].lte_cell.cell_id)
          {
            /* Found a matched cell in det list, reset counter */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].consecutive_fail_det_cnt = 0;
            break;
          }
        }

        /* Not finding a matched cell in det list, increase the counter and reset freq_age to re-issue SRCH + MEAS */
        if (j == total_cells)
        {
          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].consecutive_fail_det_cnt++;

          /* If any measured cell is not detected then skip remaining MEAS only gaps
             and re-issue SERCH+MEAS every 20/40 sec */
          if ((l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age != 0)&& !((l1_sc_wcdma_data_ptr->rr_params.ForcedMeas & FORCE_MEAS_ACTIVE)&&(l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index & (1<< freq_idx)))&&(!l1_sc_wcdma_data_ptr->elevator_flag))
          {
             if (l1_sc_g2x_check_for_extend_srch_period(gas_id))
             {
                l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] += (G2L_SEARCH_INTERVAL-1)*(l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte << 1);
             }
             else
             {
                l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] += (G2L_SEARCH_INTERVAL-1)*(l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte);
             }
             
             id_tick_update_critreria = 2;

          }
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age = 0;
        }
      }
    }
    /* Schedule SRCH + MEAS at every 20 sec or 40 sec if other sub is DDS and in data call */
    else if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age >= G2L_SEARCH_INTERVAL)
    {
       /* We've done 3 MEAS only gaps, now fall back to SRCH + MEAS */
       l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age = 0;
    }
  }
  #endif
  for(j=meas_cells; j<total_cells; j++)
  {
      if( j >= MAX_WCDMA_CELLS_PER_FREQ)
        break;

      cell_id    = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[j].lte_cell.cell_id;

      /* check for duplicate cell ids in meas/det */
      for(i=0; i < meas_cells ; i++)
      {
         /* det found in exsiting meas so delete duplicate */
         if(cell_id == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cell_id)
         {
             det_cells--;
             /*
             ** Each detect, update the meas state for latest timing info
             */
             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.state = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[j].lte_cell.state;
             l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].lte_cell.state =       l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[j].lte_cell.state;

             /*
             ** this cell has been detected so update meas location det_cnt
             */
             l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].det_cnt++;


             MSG_GERAN_MED_5_G("G2L: removing duplicate detected cell=%d idx=%d total=%d id_tick=%d update_critreria=%d",
                                cell_id, j, total_cells, l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx], id_tick_update_critreria);

             for(k=j+1; k < total_cells ; k++)  /* move up remaining cells */
             {
               l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[j] = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[k];
               l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[j]       = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[k];
               l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[j]       = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[k];
             }
             j--;            /* j now has new content so repeat the loop again */
             total_cells--;  /* account for removed cell                       */
             break;
         }
      }
  }

  MSG_GERAN_MED_3_G("G2L: Meas/Det after prune Freq_idx=%d new Total=%d det=%d", freq_idx,total_cells,det_cells);

  for(j=0; j<total_cells; j++)
  {
      MSG_GERAN_MED_5_G("G2L: meas cell id=%d i=%d meas_cnt=%d det_cnt=%d rsrp=%d ",
            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[j].lte_cell.cell_id, j,
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[j].meas_cnt,
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[j].det_cnt,
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[j].lte_cell.rsrp_avg );
  }
 } /* reprune == FALSE */

 if(rePrune == TRUE) /* final gap completed remove any stale cells before updating RR */
 {
   /*
   ** we have existing stored cells but no new cells for this new session
   */
   meas_cells   = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
 }

 if(meas_cells > MAX_WCDMA_CELLS_PER_UARFCN )  /* KW fix */
 {  meas_cells = MAX_WCDMA_CELLS_PER_UARFCN;}

 if(meas_cells)
 {
    MSG_GERAN_MED_3_G("G2L: start: remove any stale cells for Total=%d Measured=%d Det=%d", total_cells, meas_cells, det_cells);
 }

 PruneLoopCount = meas_cells;   /* only need to prune the existing meas cells, not any newly detected */
 i=0;

 while(PruneLoopCount--)
 {
   cell_id = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cell_id;   /* this is only for F3 */

   #ifdef FEATURE_G2L_TIME_TRANSFER
   if (g2l_idle)
   {
     /* Start pruning stale cells:
        1. Cell RSRP <= -130
        2. Cell RSRQ <= -30 
        3. Cell not being detected (but measured) for 2 consecutive SRCH + MEAS gaps 
     */
     if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrp_avg <= LTE_PRUNE_LOW_RSRP_LEVEL ||
         l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrq_avg <= LTE_PRUNE_LOW_RSRQ_LEVEL ||
         l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].consecutive_fail_det_cnt >= 2)
     {
       MSG_GERAN_MED_5_G("G2L: removing stale cell_id=%d rsrp=%d rsrq=%d fail_det_cnt=%d total=%d", 
                          cell_id,
                          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrp_avg,
                          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrq_avg,
                          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].consecutive_fail_det_cnt,
                          total_cells);

       for(j=i+1; j < total_cells ; j++)  /* move up any remaining cells */
       {
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i] = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[j];
         l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i]       = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[j];
         l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i]       = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[j];
         l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i]          = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[j];
       }

       if (total_cells > 0)     /* account for removed cell */
       {  
         total_cells--;
       }
       
       if (total_cells == 0)
       {
         /* G2L search should be scheduled, so id_tick accordingly */
         if ((l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age != 0)&& (!l1_sc_wcdma_data_ptr->elevator_flag))
         {
           if (l1_sc_g2x_check_for_extend_srch_period(gas_id))
           {
             l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] += (G2L_SEARCH_INTERVAL-l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age)*(l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte << 1);
           }
           else
           {
             l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] += (G2L_SEARCH_INTERVAL-l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age)*(l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte);
           }

           id_tick_update_critreria = 3;

         }
         /* If no cell to be measured, issue SRCH + MEAS */
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age = 0;
       }

       if(i)            /* repeat next check on same index location after move from j index */
       {  
         i--; 
       }
     }
   }
   else
   #endif
   {
     /*
     ** After 5 measurements check the number of detections
     */
     if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].meas_cnt > 4)
     {
        if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].det_cnt < 3)
        {
           MSG_GERAN_MED_3_G("G2L: removing stale cell_id=%d det_cnt=%d total=%d", cell_id,l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].det_cnt ,total_cells);

           for(j=i+1; j < total_cells ; j++)  /* move up any remaining cells */
           {
              l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i] = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[j];
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i]       = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[j];
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i]       = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[j];
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i]          = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[j];
           }
           l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[j].meas_cnt=0;  /*always reset last cell */
           l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[j].det_cnt =0;

           if(total_cells)     /* account for removed cell */
           {  total_cells--;}

           if (total_cells == 0)
           {
             /* If no cell to be measured, issue SRCH + MEAS in the next coming IDLE gap */
             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age = 0;
           }

           if(i)            /* repeat next check on same index location after move from j index */
           {  i--; }
        }
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].meas_cnt = 0;      /*reset after 5 measurements have occurred */
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].det_cnt  = 0;
     }
   }
   i++;
 } /* while */
 /*
 ** Update num_cells to account for removed duplicates and any stale cells removed
 */
 l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells          = total_cells;
 l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells = det_cells;
 l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells                  = total_cells;

 MSG_GERAN_MED_5_G("G2L: Meas/Det after stale cell rePrune=%d total=%d det=%d id_tick=%d update_critreria=%d",
                    rePrune, total_cells, det_cells, l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx], id_tick_update_critreria);

} /* l1_sc_prune_meas_lte_cells */

/*===========================================================================

FUNCTION l1_sc_lte_process_timed_search_results

DESCRIPTION
   This very similar to l1_sc_lte_process_meas_results.
   This has both search detected results and meas_meas results.
   Save the DED parameters and call common meas results func.


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_lte_process_timed_search_results(lte_cphy_irat_meas_timed_srch_meas_cnf_s*  msg_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32  freq_idx;

  boolean g2l_idle       = FALSE;
  boolean g2l_allow_done = FALSE;
  boolean cmSkipFirst    = FALSE;

  GL1_ISR_LOCK(gas_id);
  gl1_irat_mutex_lock(gas_id);

  /* Call back to ML1 as per new interface*/
  lte_ml1_irat_gap_srch_meas_cnf_handling(geran_map_gas_id_to_nas_id(gas_id), msg_ptr, &ml1_gap_state[gas_id]);
  gl1_concman_config_params_type gl1_concman_config_params;
  cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);
  
  gl1_concman_config_params.type = CMGR_TRANSIENT_END;
  gl1_concman_config_params.scenario = CMGR_GERAN_PTM_G2L_GAP;
  gl1_concman_config_params.update_scenario = 0;
  gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_IRAT_MEASUREMENT);
  gl1_concman_config_params.start_timestamp = 0;
  gl1_concman_config_params.duration_msec = 0;
  gl1_concman_config_params.flow_ctrl_time_msec = 0;
  gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);

  /*
  ** this _srch_meas_cnf is for idle or connected mode measurement ?
  */
  if(l1_sc_wcdma_data_ptr->lte_cm_irat_active & G2L_CM_CNF_PENDING)
  {
     freq_idx    = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;

     cmSkipFirst = l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req < 2;  /* first gap is ML1 AGC only, so set skip */


  }
  else
  {
     g2l_idle       = TRUE;
     freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;

     if (!msg_ptr->gap_aborted)
     {
       g2l_allow_done = TRUE;
       /*
       ** record each idle result that gets measured in the search interval.
       */
       l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas |= (1<<freq_idx);   /* for xfer/ded only record as measured on the 5th gap */
     }
  }

  MSG_GERAN_HIGH_4_G("G2L:LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_CNF srch_state=%d idx=%d idle=%d gap_aborted=%d",
                                                            l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                                                            freq_idx,
                                                            g2l_idle,
                                                            msg_ptr->gap_aborted);

  l1_sc_irat_inc_dec_g2x_abort_count(msg_ptr->gap_aborted, gas_id);

 /* check for valid rat index */
  if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat != RR_L1_IRAT_RAT_LTE )
  {
     g2l_lte_list_index_recovery(gas_id);
     freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
  }

  if (msg_ptr->gap_aborted)
  {
    freq_idx = MAX_WCDMA_FREQS;
  }

  if(freq_idx < MAX_WCDMA_FREQS)
  {
    if(msg_ptr->meas.num_cells )
    {
       /*
       ** G2L Idle/Ded use common function for saving the same structure
       */
       MSG_GERAN_HIGH_3_G("G2L: Process Meas results idx=%d freq=%d num_meas=%d", freq_idx, msg_ptr->freq, msg_ptr->meas.num_cells );

       l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells = 0;   /* reset for the func that follows */
       l1_sc_save_meas_lte_cells(freq_idx, msg_ptr->meas.num_cells, (lte_cphy_irat_meas_meas_results_s*)&msg_ptr->meas.cells[0], gas_id );
    }
    /*
    ** if num detected > 12 and num meas > 8 truncated meas results to 8 before we add the 12 newly detected cells
    ** values for TDD are less.
    */

    if( msg_ptr->search.num_detected_cells )
    {
      l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells = msg_ptr->search.num_detected_cells;

      if (  ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells > G2L_CM_LIMIT_DET_CELLS  )
         && ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells          > G2L_CM_LIMIT_MEAS_CELLS ))
      {
         update_stored_measurements_for_top_8(freq_idx, gas_id);

         MSG_GERAN_HIGH_3_G("G2L Ded Top8 Truncating meas in=%d out=%d num_det=%d ", l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells, l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells, msg_ptr->search.num_detected_cells );

         // G2L Need s rework //
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = G2L_CM_LIMIT_MEAS_CELLS;
         l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells = G2L_CM_LIMIT_DET_CELLS;
      }
      /*
      ** G2L Idle/Ded use common function for saving the same structures
      ** for ded the detected cells are appended the the meas results.
      **
      */
      MSG_GERAN_HIGH_3_G("G2L: Process Det  results idx=%d freq=%d num_det=%d", freq_idx, msg_ptr->freq, l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells );

      l1_sc_save_det_lte_cells(freq_idx, l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells, (lte_cphy_irat_meas_detected_cell_s*)&msg_ptr->search.det_cells[0], gas_id );
    }
    else
    {
        /*
        ** ensure it gets cleared when no cells are detected.
        */
        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells = 0;
    }

    l1_sc_prune_meas_lte_cells(freq_idx, g2l_idle, FALSE, cmSkipFirst, gas_id);

    /*
    ** if TDD freq, select best cell sss_corr for vcell_info for next gap
    */
    g2l_tdd_best_sss_corr( msg_ptr->search.search_performed , freq_idx, gas_id);

    /*  stored_meas->lte_cell is type lte_cphy_irat_meas_meas_params_s */
    /*  should it be lte_cphy_irat_meas_meas_results_s ??              */
    /*  stored_meas[freq_idx].cell_params[i].lte_cell                  */
  }

  if(g2l_idle && !msg_ptr->gap_aborted)
  {
    /*
    ** Idle: always update freq_status
    */

	#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
	gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2L_IRAT_SEARCH_END_IDLE); 
	#endif
	
    if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells )
    {
       l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_status = G2X_FS_NEWLY_MEASURED;
       /* add bit mask to uniquely count only once per freq */
       l1_sc_wcdma_data_ptr->rr_params.fastSearchNumMeasured++;      /* Idle number of freq with detected/measured cells */
    }
    else
    {
       l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_status = G2X_FS_NO_RESULT;
    }
    if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState & FAST_SEARCH_ACTIVE )   /* is in idle mode */
    {
       /*
       ** G2L idle mode _TIMED_SRCH_MEAS_CNF
       */
       g2l_allow_done = l1_sc_g2l_fast_search_restart(gas_id);      /* if active daisy chain next G2L idle search, true on completion */
       /* set false when restarting, inhibits wcdma_srch.done() that follows */
     }
  }
  else if(g2l_idle && msg_ptr->gap_aborted)
  {
    /* Scheduling G2L search on the same frequency immediately */
    MSG_GERAN_HIGH_1_G("Next G2L search should be freq idx %d", l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset);

    l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick;

    /* subtract 1 modulo num-frequencies */
    l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = (l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset + 
                                                            l1_sc_wcdma_data_ptr->rr_params.num_freqs - 1) %
                                                            l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  }
  else
  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2L_IRAT_SEARCH_END_DED); 
#endif
  }

  /* must come after the l1_sc_g2l_fast_search_restart() to get id_state cleanup sequence correct */
  g2l_process_state_cnf( g2l_idle, freq_idx , msg_ptr->gap_aborted, gas_id);

  gl1_irat_mutex_unlock(gas_id);
  GL1_ISR_UNLOCK(gas_id);

  if(g2l_allow_done==TRUE)            /* for fast restart this is reset to prevent wcdma_srch.done() from running */
  {
     /*
     ** added missing idle functionality for
     ** scheduling call to next_cell_bcch()
     */
      if (l1_sc_wcdma_data_ptr->wcdma_srch.done)
      {
         l1_scdrv_call_in_task( l1_sc_wcdma_data_ptr->wcdma_srch.done, NULL, gas_id );
      }
  }

   /*
   ** in dedicated Call RF meas exit for G2L after gap complete
   */
  gl1_irat_mutex_lock( gas_id);

  if(msg_ptr->gap_aborted)
  {
     /*if g2x clean up commmand is already given, possible in case of connected mode scenarios*/
     switch (l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap)
     {
        case G2L_MDSP_CLEANUP_COMPLETED:
        case G2L_MDSP_END:
        case G2L_MDSP_NULL:
        {
          /*This shouldnt happen, as ML1 should be sending us CNF before gap_end in case of Abort in all cases, But if it happens GL1 handles it*/
          MSG_GERAN_ERROR_1_G("G2L: Abort: l1_sc_trigger_g2l_gap:%d",l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap);

          /*Issue Rf Exit with Abort here*/
          l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;
          if (gl1_hw_get_g2x_rf_enter_called_last(gas_id))
          {
             g2l_irat_x2l_meas_exit(TRUE, gas_id);
             gl1_hw_set_g2x_rf_enter_called_last(FALSE, gas_id);
          } 
        }
         break;
        default:
           /*Set G2L Aborted flag to TRUE, rf_meas_exit will be called after sending G2X clean up command, in ISR*/
           l1_sc_wcdma_data_ptr->g2x_aborted = TRUE;
           MSG_GERAN_MED_1_G("G2L: Search is Aborted by ML1: Abort flag: %d",l1_sc_wcdma_data_ptr->g2x_aborted);
         break;
     }
  }
  else
  {
     l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;
     if (gl1_hw_get_g2x_rf_enter_called_last(gas_id))
     {
        g2l_irat_x2l_meas_exit(FALSE, gas_id);
        gl1_hw_set_g2x_rf_enter_called_last(FALSE, gas_id);
     }
  }

  gl1_irat_mutex_unlock( gas_id);
  
  return;

} /* l1_sc_lte_process_timed_search_results */


/*===========================================================================

FUNCTION l1_sc_populate_cm_g2l

DESCRIPTION
  Function Timed CM gap measurement for G2L
  Populates the structure lte_cphy_irat_meas_timed_srch_meas_req_s for timed srch/meas for G2L.
  ML1 handles all the Rf processing as we pass in the rxlm buffers.

DEPENDENCIES
  none

  lte_cphy_irat_meas_startup_rf_params_s rf_params;
  Is replaced with
  rfm_meas_common_param_type *common;

  Pointer to initialized target and source tech RF parameters.
  Points to NULL if the lte_ml1_irat_x2l_meas_enter() function
  wasn't first called and source tech is following legacy RF call flow


RETURN VALUE
  boolean     TRUE if message was sent

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_populate_g2l( uint16 freq_idx, lte_cphy_irat_meas_timed_srch_meas_req_s *msg, lte_ml1_irat_gap_schedule_hint_e *schedule_hint, boolean g2l_idle, gas_id_t gas_id )
{
  uint32  i;
  uint16  num_cells,tdd_cell_idx;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  msg->rf_param = g2l_meas_enter_params[gas_id];   /* copy, is Not a pointer, passed to and populated by meas_enter */

  /* Payload freq and bw */

  num_cells                                = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;     /* is new combined total meas+det */
  /* sanity check */

  /* limit has been reduced to 9, not as spec'd in HLD  */

  if(num_cells > G2L_CM_MAX_MEAS_CELLS)
  {  num_cells = G2L_CM_MAX_MEAS_CELLS;}

  msg->freq                                = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn;
  msg->meas.num_cells                      = num_cells;
  msg->meas.meas_freq                      = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn;
  msg->dl_bandwidth                        = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].dl_bandwidth;
  msg->dl_bandwith_present                 = TRUE;

  msg->timing_adjustment = 0;

  #ifdef FEATURE_G2L_TIME_TRANSFER
  if (g2l_idle)
  {
    if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age == 0)
    {
      *schedule_hint = LTE_ML1_IRAT_GAP_SCHEDULE_HINT_SEARCH_AND_MEAS;
    }
   else if (l1_sc_wcdma_data_ptr->elevator_flag==TRUE)
    {
       l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age = 0;
       *schedule_hint = LTE_ML1_IRAT_GAP_SCHEDULE_HINT_SEARCH_AND_MEAS;
       MSG_GERAN_HIGH_1_G("G2L: Scheduling S+M as elevator_flag: %d", l1_sc_wcdma_data_ptr->elevator_flag);
    }
    else
    {
      *schedule_hint = LTE_ML1_IRAT_GAP_SCHEDULE_HINT_MEAS_ONLY;
    }
  }
  #else
  /* search flags, the ML1 api func actually sets this */
  msg->search.perform_search               = TRUE;

  if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_detected_cells)
  {
     msg->search.measure_newly_detected_cells = TRUE;
  }
  else
  {
     msg->search.measure_newly_detected_cells = FALSE;
  }
  #endif

  /* this is not by LTE */
  msg->freq_state_included                 = FALSE;
  msg->freq_state.dummy                    = 0;

  /*
  ** Only relevant for TDD
  ** Virtual LTE cell is the cell with the best SSS_CORR in current frequency.
  ** TRUE if the vcell info is present and the next gap is used for TDD measurement.
  */
  tdd_cell_idx = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].best_lte_tdd_cell_idx;

  if((tdd_cell_idx != INVALID_IDX)&&(tdd_cell_idx < MAX_WCDMA_CELLS_PER_FREQ))
  {
     msg->vcell_info_present  = TRUE;
     msg->vcell_info          = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[tdd_cell_idx].lte_cell.state;
  }
  else
  {
     msg->vcell_info_present = FALSE;
  }

  /*
  ** get the stored cells to be measured,
  ** timing info is passed back from the cnf meas of previous in "state" struct.
  ** also includes any newly detected cells to be measured.
  **
  */

  for (i = 0; i < num_cells; i++)
  {
      msg->meas.cells[i].cell_id           = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cell_id;
      msg->meas.cells[i].cp_type           = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cp_type;
      msg->meas.cells[i].state             = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.state;
  }

  /*
  ** gap timing passed to ML1, offsets are all fixed, only FN changes
  ** new api/struct from ML1 , clock offet not required.
  */
  msg->gap.time_rat_type                   = LTE_CPHY_IMEAS_RAT_GSM;

  msg->gap.gap_start.g.frame               = ggap[gas_id].start.frame_number;
  msg->gap.gap_start.g.qs_offset           = ggap[gas_id].start.qs_offset;

  msg->gap.gap_start.g.raw_13M             = 0;

  if (g2l_idle) {
     msg->gap.asap_gap_start = TRUE;
     msg->gap.gap_start.g.frame            = 0;
     msg->gap.gap_start.g.qs_offset        = 0;
     msg->gap.gap_start.g.prescale         = 0;
     msg->support_gap_abort_before_tune    = TRUE;
  }
  else
  {
     msg->gap.gap_start.g.frame            = ggap[gas_id].start.frame_number;
     msg->gap.gap_start.g.qs_offset        = ggap[gas_id].start.qs_offset;
     msg->gap.gap_start.g.prescale         = 0;
     msg->support_gap_abort_before_tune    = TRUE;
  }

  #ifdef FEATURE_G2L_TIME_TRANSFER
  msg->timing_adjustment                   = gl1_msg_get_g2l_tt_acc(gas_id);
  #endif

  MSG_GERAN_HIGH_5_G("G2L gap start frame %d offset %d start %d sched_hint %d freq age %d", msg->gap.gap_start.g.frame, msg->gap.gap_start.g.qs_offset, msg->gap.asap_gap_start, *schedule_hint, l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age);
  msg->gap.gap_end.g.frame                 = ggap[gas_id].end.frame_number;
  msg->gap.gap_end.g.qs_offset             = ggap[gas_id].end.qs_offset;
  msg->gap.gap_end.g.raw_13M               = 0;


  MSG_GERAN_HIGH_3_G("G2L gap end frame %d offset %d FN %d", msg->gap.gap_end.g.frame, msg->gap.gap_end.g.qs_offset, gl1_get_FN(gas_id));
  msg->gap.clock_off_time.g.frame          = ggap[gas_id].clock.frame_number;

/*
**  msg->gap.clock_off_time.g.qs_offset  This is not required to be set
*/
  msg->gap.clock_off_time.g.qs_offset      = 0;
  msg->gap.clock_off_time.g.raw_13M        = 0;
} /* l1_sc_populate_g2l */


/*===========================================================================

FUNCTION g2l_init_cm

DESCRIPTION
             init state control data for G2L CM measurements.

DEPENDENCIES
  none

RETURN VALUE
  various

SIDE EFFECTS
  None

===========================================================================*/

static void g2l_init_cm(gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   l1_sc_wcdma_data_ptr->rr_params.use_next_freq  = FALSE;

   l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req   = 0;
   l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf   = 0;
   l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx    = 0;
   l1_sc_wcdma_data_ptr->g2l_inhibit_next_update  = 0;

}

/*===========================================================================

FUNCTION g2l_set_cm_inactive

DESCRIPTION
             for G2L Idle and CM measurements states are set inactive
             to be called after a measurement has occurred.
             resumes pwr_meas

DEPENDENCIES
  none

RETURN VALUE
  various

SIDE EFFECTS
  None

===========================================================================*/
void   g2l_set_cm_inactive(gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
    l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_wcdma_data_ptr->lte_cm_irat_active = G2L_CM_INACTIVE;

    g2l_init_cm(gas_id);
    gl1_msg_resume_pwr_meas(gas_id);

    if(l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE)
    {
       l1_sc_set_gtow_srch_state_null (gas_id);
    }


} /* g2l_set_cm_inactive */

/*===========================================================================

FUNCTION g2l_reset_stored_det_cnts

DESCRIPTION
             Clear Stored LTE detected list.

DEPENDENCIES
  none

RETURN VALUE
  various

SIDE EFFECTS
  None

===========================================================================*/
static void g2l_reset_stored_det_cnts(gas_id_t gas_id)
{
  uint32 i,j;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  for(i=0; i< l1_sc_wcdma_data_ptr->rr_params.num_freqs ; i++)
  {
     if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat == RR_L1_IRAT_RAT_LTE )
     {
        for (j = 0; j < G2L_MEAS_DET_MAX_CELLS; j++)
        {
          l1_sc_wcdma_data_ptr->stored_meas[i].cell_det[j].det_cnt = 0;
          l1_sc_wcdma_data_ptr->stored_meas[i].cell_det[j].meas_cnt =0;
          l1_sc_wcdma_data_ptr->stored_meas[i].cell_det[j].consecutive_fail_det_cnt = 0;
        }

        l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_age = 0;
     }
  }
} /* g2l_reset_stored_det_cnts */

/*===========================================================================

FUNCTION l1_sc_cm_g2l_state

DESCRIPTION
             The main event controller for G2L CM measurements.
             more TBA
DEPENDENCIES
  none

RETURN VALUE
  various

SIDE EFFECTS
  None

===========================================================================*/
uint16  l1_sc_cm_g2l_state(  g2l_cm_events_e gapEvent , uint16 freq_idx , boolean gapBool, gas_id_t gas_id)
{
    uint16 return_val=0;
    uint16 local_ml1_gap_init =FALSE;
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
    l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

    uint16  curr_init_mode = g2l_get_Init_Sent_status(gas_id);
    uint16  gap_for_sdcch  = 0;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    boolean is_drdsds_band_registered = FALSE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

    (void)gapBool;

    gl1_irat_mutex_lock(gas_id);

    switch(gapEvent)
    {
       /*
       ** There is no "abort" for CM mode or cleanup required.
       ** If started the measurement is allowed to complete, wait for any cnf's then we deinit
       */

       case  LTE_GAP_ABORT:
             break;


       case  LTE_GAP_START_REQ:
       {

          if(l1_sc_wcdma_data_ptr->lte_irat_active == FALSE)
          {
              MSG_GERAN_HIGH_1_G("G2L: waiting !lte_irat_active, skip this measurement init=0x%x ", curr_init_mode);

              gl1_irat_mutex_unlock(gas_id);
              return return_val;
          }

          l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req = 0;          /* The first request of 5 */
          l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf = 0;          /* The first request of 5 */
          l1_sc_wcdma_data_ptr->rr_params.currFreqIdx  = freq_idx;   /* the freq we start with */

          g2l_set_next_freq(freq_idx, gas_id);         /* if the next freq is also LTE set here */

          /*
          ** For xfer the scheduling trigger point is FNmod26== 22 and remains fixed.
          */
          if(l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
          {
             l1_sc_wcdma_data_ptr->g2l_start_frame = 23;        /* frame-1 when we open  the GFW gap + qs offset */
             l1_sc_wcdma_data_ptr->g2l_end_frame   = 24;        /* frame-1 when we close the GFW gap + qs offset */
             l1_sc_wcdma_data_ptr->g2l_idle_offset =  2;        /* trigger frame to the idle frame-1             */
          }
          else
          if((l1_sc_globals_ptr->mode == L1SCModeDediTCh)
#ifdef FEATURE_GSM_DTM
          || (l1_sc_globals_ptr->mode == L1SCModeDediDTM)
#endif
            )
          {
            uint16 g2l_idle_frame = l1_sc_get_tch_idle_frame(gas_id);   /* this is mod26 */
            /*
            ** idle frame can be 12 or 25
            */
            MSG_GERAN_HIGH_3_G("G2L: Ded frame FN=%d FNmod26=%d idle FN %d", gl1_get_FN(gas_id), frame_counters[gas_id].FNmod26, g2l_idle_frame );
            /*
            ** For Ded the scheduling trigger point is FNmod26== 8 or 21
            */

             l1_sc_wcdma_data_ptr->g2l_start_frame = SUB_MOD(g2l_idle_frame, 2, 26);
             l1_sc_wcdma_data_ptr->g2l_end_frame   = SUB_MOD(g2l_idle_frame, 1, 26);
             l1_sc_wcdma_data_ptr->g2l_idle_offset =  3;                              /* trigger frame to the idle-1 frame */
          }

          /*
          ** SDCCH uses consectutive frames and requires "fat gap" implementation from ML1 similar now to idle .
          */
          else
          if(l1_sc_globals_ptr->mode == L1SCModeDediSDCCh)
          {
            uint16 g2l_pseudo_idle_frame = frame_counters[gas_id].FNmod26;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            /*
            ** this returns a gap that is +1 too long for G2L measurements in sdcch/4.
            */
            uint16 gap_frames_to_ccch    = l1_fm_get_ccch_gap(gas_id);

            if(gap_frames_to_ccch)
            {  gap_frames_to_ccch--; }      /* reduce by 1 */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            /* For GAP_START in G2L the _irat_ptm_sc_activity_ok is called twice in the code flow
             * so the below call is to flush out the 1st booking
             * so it can be rebooked for 10 frames, or when not used.
             */
            grm_transaction_manager(GRM_TM_FLUSH, SC_IRAT,l1_tskisr_blk->client_id , gas_id);
#endif
            if(gap_frames_to_ccch < 9)
            {
                MSG_GERAN_HIGH_1_G("G2L: ### Sdcch gap is too small, skip this measurement gap=%d", gap_frames_to_ccch );

                /*If MCVS req of 384 MHZ already sent, then send de-boost request as well*/
                l1_sc_g2x_stop_mcpm_irat_ngbr(gas_id);
                gl1_irat_mutex_unlock(gas_id);
                return  return_val;
            }

            l1_sc_wcdma_data_ptr->g2l_idle_offset = 2;
            gap_for_sdcch                         = gap_frames_to_ccch;      /* idle offset */

            if(gap_for_sdcch > 10)
            {
               gap_for_sdcch = 10;  /* we don't need more than 10 frames so limit */
            }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            /*
            ** rebook TRM for 10 frames
            */
            if(! grm_check_ptm_irat_sc_activity_ok(l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset,
                                                   gap_for_sdcch, CMGR_GERAN_PTM_G2L_GAP, gas_id) )
            {
                MSG_GERAN_HIGH_1_G("G2L: ### Sdcch TRM denied gap, skip this measurement gap=%d", gap_for_sdcch );

                /*If MCVS req of 384 MHZ already sent, then send de-boost request as well*/
                l1_sc_g2x_stop_mcpm_irat_ngbr(gas_id);

                gl1_irat_mutex_unlock(gas_id);

                return  return_val;
            }
#endif
            /*
            ** we have min of next 9 frames to measure LTE,
            ** so make the the frame +2 behave as idle frame
            ** mdsp_g2x_startup_cmd() must be sent now.
            */
            l1_sc_wcdma_data_ptr->g2l_start_frame = ADD_MOD(g2l_pseudo_idle_frame, 1, 26);  /* prevent the trigger from re-running */
            l1_sc_wcdma_data_ptr->g2l_end_frame   = ADD_MOD(g2l_pseudo_idle_frame, (gap_for_sdcch-1), 26);
            /* note the mdsp offsets used here are arbitary  and just re-use the idle frame values */

            /*
            ** preset counts as for sdcch we only do 1 "fat gap" measurement, so stops at cntTimed_Cnf=5
            */
            l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req = 4;
            l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf = 4;
            l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx  = freq_idx;     /* always the same for sdcch */

            /*
            ** reset scheduling tick for next sdcch
            */
            l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick = 0;
            l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick       =  NUM_IDLE_FRAMES_PER_DED_SRCH_G2L;

            MSG_GERAN_HIGH_3_G("G2L: Ded Sdcch frame gap=%d FNmod26 Start=%d End=%d", gap_frames_to_ccch , l1_sc_wcdma_data_ptr->g2l_start_frame, l1_sc_wcdma_data_ptr->g2l_end_frame );

          }
          else
          {
            // get frames available count
            MSG_GERAN_HIGH_1_G("G2L: ### Ded mode=%d not supported yet ###", l1_sc_globals_ptr->mode );

            gl1_irat_mutex_unlock(gas_id);

            return  return_val;
          }

          if(l1_sc_wcdma_data_ptr->g2l_inhibit_next_update == FALSE)          /* already started next freq */
          {
             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].best_lte_tdd_cell_idx = INVALID_IDX; /* reset for TDD sss_corr */
          }
          
         /*
         ** if LTE is active always ready by default
         */
         local_ml1_gap_init     = l1_sc_wcdma_data_ptr->rr_params.ml1_gap_init;
 
         l1_sc_wcdma_data_ptr->rr_params.ml1_gap_init = FALSE;      /* Init sent only required once per session */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
          /* No need to check DRDSDS is enabled or mode is GPRS Transfer. In these cases,
             GL1 should have already performed band registration in l1_sc_wcdma_ded_ready*/
          is_drdsds_band_registered = TRUE;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
       }
       /* fall thru */

       case  LTE_GAP_NEXT_REQ:
       {
          if((l1_sc_globals_ptr->mode == L1SCModeDediTCh)
#ifdef FEATURE_GSM_DTM
          || (l1_sc_globals_ptr->mode == L1SCModeDediDTM)
#endif
            )
          {
            uint16 g2l_idle_frame = l1_sc_get_tch_idle_frame(gas_id);
            /*
            ** check the scheduling FN
            */
            if(frame_counters[gas_id].FNmod26 != (g2l_idle_frame-4))
            {
               MSG_GERAN_HIGH_3_G("G2L: Wait for Ded Idle frame FN=%d FNmod26=%d idle FN %d", gl1_get_FN(gas_id), frame_counters[gas_id].FNmod26, g2l_idle_frame );

               gl1_irat_mutex_unlock(gas_id);

               return  return_val;
            }
            /*
            ** update for each gap incase value changes as seen with TCH<->DTM  FNmod26== 8 or 21
            */
            l1_sc_wcdma_data_ptr->g2l_start_frame = SUB_MOD(g2l_idle_frame, 2, 26);
            l1_sc_wcdma_data_ptr->g2l_end_frame   = SUB_MOD(g2l_idle_frame, 1, 26);
          }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
          /*Before proceeding further check the IRAT measurements are OK with
          TRM DR-DSDS by registering frequency bands with TRM*/
          /* Not sure, whether ML1 will be happy if GL1 doesn't schedule
             measurement in one of the five "consecutive" gaps.
             Future task: If ML1 complains, then reset G2L IRAT state machine
             and start 5 consecutive gaps again.*/
          if(FALSE ==  is_drdsds_band_registered)
          {
            if(TRUE == grm_check_ptm_irat_sc_activity_ok(freq_idx,
                                                         MAX_FRAMES_FOR_IRAT_CONNECTED_MODE,
                                                         CMGR_GERAN_PTM_G2L_GAP,
                                                         gas_id))
            {
              is_drdsds_band_registered = TRUE;
            }
            else
            {
              MSG_GERAN_MED_0_G(GL1_DRDS_DBG_HDR"Use of idle frame for IRAT denied by TRM");

              gl1_irat_mutex_unlock( gas_id);

              return  return_val;
            }
          }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

          if(l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req == l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf)
          {
            uint32 frame_number;

            lte_earfcn_t   nextFreq;

            l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req++;

            frame_number = l1_sc_get_g2l_frame_number(l1_sc_wcdma_data_ptr->g2l_idle_offset, gas_id);   /* frame now +g2l_idle_offset frame offset is 2 or 3 */

            ggap[gas_id].start.frame_number = frame_number;
            ggap[gas_id].clock.frame_number = frame_number;

            nextFreq = l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx].arfcn.euarfcn;

            if(l1_sc_globals_ptr->mode == L1SCModeDediSDCCh)
            {
               /*
               ** the frame number here is already +2 for g2l_idle_offset, so allow for this at the end frame
               */
               frame_number            = ADD_FN(frame_number, (gap_for_sdcch-(l1_sc_wcdma_data_ptr->g2l_idle_offset)));  /* for sdcch "fat gap"  */
               ggap[gas_id].end.frame_number   = frame_number;

               MSG_GERAN_HIGH_3_G("G2L: Schedule SDCCH Timed search freq=%d Start Fn=%d End FN=%d", nextFreq, ggap[gas_id].start.frame_number, ggap[gas_id].end.frame_number );

            }
            else
            {
               ggap[gas_id].end.frame_number   = ADD_FN(frame_number, 2);
            }

            /* used by idle mode, so initialise qs_offset to values applicable in dedicated mode*/
            ggap[gas_id].start.qs_offset = G2L_GAP_START_OFFSET;
            ggap[gas_id].end.qs_offset   = G2L_GAP_END_OFFSET;

            l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_START_CMD_IDX]   = G2L_MDSP_START_OFFSET;
            l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_CLEANUP_CMD_IDX] = G2L_MDSP_END_OFFSET;

			#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
			gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2L_IRAT_SEARCH_START_DED); 
			#endif
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
            /*Inform MCPM about critical scenario now if needed*/
            if((l1_sc_irat_get_g2x_abort_count(gas_id) !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
            {
              gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
              critical_scn_updated_to_mcpm[gas_id] = TRUE;
            }
#endif

            if( TRUE == L1_send_LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ( freq_idx, local_ml1_gap_init, nextFreq, gas_id, FALSE ))
            {
               MSG_GERAN_HIGH_4_G("G2L:LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ Started earfcn=%d idx=%d for Fn=%d, gap_count=%d",
                                   l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn,
                                   freq_idx,
                                   ggap[gas_id].start.frame_number,
                                   l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req );


               g2l_set_Init_Sent_status(LTE_G2L_CNF_PENDING, gas_id);                     /* Indicate cnf is outstanding */
               /*
               ** This is used to discriminate between Idle and Connected mode xxx_cnf response
               */
               l1_sc_wcdma_data_ptr->lte_cm_irat_active    |= G2L_CM_CNF_PENDING;     /* Indicate cm cnf is outstanding */
               l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_DEDICATED, gas_id);
               l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ACTIVE;
               l1_sc_globals_ptr->wcdma_srch_in_progress = TRUE;
               L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
               l1_sc_globals_ptr->active_irat               = RR_L1_IRAT_RAT_LTE;

               l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap  = G2L_MDSP_START;

               return_val = l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req;   /* non-zero next measurement has started <1..5> */
            }
            else
            {
               MSG_GERAN_HIGH_2_G("LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ ### Start failed earfcn=%d idx=%d",
                                   l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn, freq_idx );
               /*In case if GL1 comes in this leg, then clear the transaction*/

               /* restore flag to original value */ 
                
               l1_sc_wcdma_data_ptr->rr_params.ml1_gap_init = local_ml1_gap_init;

               if( l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req != 0 )
               {
                 l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req--;
               }
            }
          }
          else
          {
#ifdef FEATURE_QSH_MDUMP
              QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
              ERR_GERAN_FATAL_3_G("LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ ### search not confirmed idx %d gap req=%d cnf=%d",
                                    freq_idx, l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req, l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf );
          }
       }
       break;


      default:
      break;
    }

    gl1_irat_mutex_unlock( gas_id);

    return return_val;


} /* l1_sc_cm_g2l_state */

/*===========================================================================

FUNCTION l1_sc_idle_g2l_state

DESCRIPTION
             G2L idle mode controller.

             For Thor rework this is reduced to 2 events

             LTE_IDLE_GAP_WAIT and LTE_IDLE_GAP_SCHEDULE, this could be split into 2 functions later.

DEPENDENCIES
  none

RETURN VALUE
  various

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_idle_g2l_state( g2l_idle_events_e gapEvent, gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

   uint16  curr_init_mode    = g2l_get_Init_Sent_status(gas_id);
   uint16 local_ml1_gap_init = FALSE;


   gl1_irat_mutex_lock( gas_id);

   switch(gapEvent)
   {
      case LTE_IDLE_GAP_WAIT:
      {
         /* Check for TIMED_MEAS_SEARCH_CNF pending for dedicated mode*/
         if (l1_sc_wcdma_data_ptr->lte_cm_irat_active & G2L_CM_CNF_PENDING)
         {
            MSG_GERAN_HIGH_0_G("Waiting for TIMED_MEAS_SEARCH_CNF against dedicated mode REQ ");
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
            if(l1_sc_g2x_timeline_opt_enable[gas_id])
            {
              gl1_drx_require_next_tick(gas_id);
            }
#endif
         }
         /* Check INIT_REQ and CNF completed */
         else
         if ((curr_init_mode & LTE_G2L_MASK )== LTE_G2L_READY)
         {
            g2l_start_idle_lte_search(gas_id);
         }
         else
         {
            MSG_GERAN_HIGH_1_G("Waiting for Init cnf 0x%x",  curr_init_mode);
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
            if(l1_sc_g2x_timeline_opt_enable[gas_id])
            {
              gl1_drx_require_next_tick(gas_id);
            }
#endif
         }
      }
      break;

      case LTE_IDLE_GAP_SCHEDULE:
      {
          /*
          ** LTE index recovery: check for cell list update changed and index nolonger matches LTE rat.
          ** If arfcn still in list find new index
          ** If not found, find any LTE index to allow measurement to continue.
          ** else
          **   ERR_FATAL at least we stop near the problem.
          */

          uint32 freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;

          if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat != RR_L1_IRAT_RAT_LTE )
          {
             g2l_lte_list_index_recovery(gas_id);
             freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;   /* re-read recovered index */
          }

          if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_LTE )
          {
             uint32 frame_number = 0;
             lte_earfcn_t   nextFreq;
             int32 rf_setup_time = 0;

             /* Send command to mdsp and ML1*/
             local_ml1_gap_init     = l1_sc_wcdma_data_ptr->rr_params.ml1_gap_init;

             l1_sc_wcdma_data_ptr->rr_params.ml1_gap_init = FALSE;      /* Init sent only required once per session */

             /* Current frame is FN-1 and gap starts at next frame, therefore offset is 1*/
             l1_sc_wcdma_data_ptr->g2l_idle_offset = 1;
             frame_number = l1_sc_get_g2l_frame_number(l1_sc_wcdma_data_ptr->g2l_idle_offset, gas_id);   /* frame now +g2l_idle_offset  */

             /* ggap is passed to ML1. provide next frame number FN and qs offset*/
             ggap[gas_id].start.frame_number = frame_number;
             /* mdsp startup command add mdsp_ftsm() in the offset and in the existing code GL1_EXTRA_LTE_STARTUP_MARGIN is used.*/
             ggap[gas_id].start.qs_offset = (mdsp_ftsm(gas_id) + GL1_EXTRA_LTE_STARTUP_MARGIN);

             ggap[gas_id].clock.frame_number = frame_number;

             /* frame_number is FN and GAP ends at FN+14. New frame_number will be FN+14*/
             frame_number = ADD_FN(frame_number, (MAX_LTE_SEARCH_FRAMES - l1_sc_wcdma_data_ptr->g2l_idle_offset));
             ggap[gas_id].end.frame_number = frame_number;

             /* Time need to start RX at TS0*/
             rf_setup_time = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

             /* qs_offset is total qs in a frame - time required to setup rf for TS0 (FN+15) - cleanup time to release queues */
             ggap[gas_id].end.qs_offset = (QS_PER_FRAME - rf_setup_time - GL1_HW_G2L_CLEANUP_CMD_PROCESS_QS);

             /* ggap_g2l_mdsp represents offset passed to mdsp command*/
             l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_CLEANUP_CMD_IDX] = ((ggap[gas_id].end.qs_offset >= mdsp_ftsm(gas_id)) ? (ggap[gas_id].end.qs_offset - mdsp_ftsm(gas_id)) : 0);


             /* Idle mode, next freq is not used by ML1 for fat gap, so set as same freq */
             nextFreq = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn;

#ifdef FEATURE_G2L_TIME_TRANSFER
              if((l1_sc_wcdma_data_ptr->rr_params.ForcedMeas & FORCE_MEAS_ACTIVE)&&(l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index & (1<< freq_idx)))
              {
                 l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_age = 0;
              }
#endif

			 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
			 gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2L_IRAT_SEARCH_START_IDLE); 
			 #endif
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
             /*Inform MCPM about critical scenario now if needed*/
             if((l1_sc_irat_get_g2x_abort_count(gas_id) !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
             {
               gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0, L1_TIME_LINE_DELAY,gas_id);
               critical_scn_updated_to_mcpm[gas_id] = TRUE;
             }
#endif

             if( TRUE == L1_send_LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ( freq_idx, local_ml1_gap_init, nextFreq, gas_id, TRUE ))
             {
               /* Move to Active state*/
               l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_ID, gas_id);
               l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ACTIVE;
               l1_sc_globals_ptr->wcdma_srch_in_progress   = TRUE;
               L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
               l1_sc_globals_ptr->active_irat               = RR_L1_IRAT_RAT_LTE;

               /*Clear drx_ref for this IRAT activity (G2L-FM_PRI_LTE_IDSEARCH) now*/
               l1_sc_wcdma_data_ptr->drx_ref =0;
     
               /* Set flag to receive TIMED_SRCH_MEAS_CNF*/
               g2l_set_Init_Sent_status(LTE_G2L_CNF_PENDING, gas_id);

               if (gl1_hw_qta_gap_active(gas_id))
               {
                 l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;
                 set_qta_cleanup_pending(FALSE, gas_id);
                 l1_sc_g2l_empty_qta_gap[gas_id] = FALSE;
               }

               MSG_GERAN_HIGH_4_G("G2L:LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ Started earfcn=%d idx=%d for Fn=%d, gap_count=%d",
                                   l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn,
                                   freq_idx,
                                   ggap[gas_id].start.frame_number,
                                   0 );

             }
             else
             {
               MSG_GERAN_HIGH_2_G("LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ ### Start failed earfcn=%d idx=%d",
                                   l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn, freq_idx );


                 /*In case if GL1 comes in this leg, then clear the transaction*/

             }
           }
           else
           {
#ifdef FEATURE_QSH_MDUMP
              QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
              ERR_GERAN_FATAL_1_G("LTE_CPHY_IRAT_MEAS_G2L_TIMED_SRCH_MEAS_REQ ### g2l_lte_list_index_recovery failed idx=%d", freq_idx );
           }
      }
      break;


   default:
      MSG_GERAN_ERROR_1_G("Unhandled G2L Idle event %d", gapEvent);
      break;
   }

   gl1_irat_mutex_unlock( gas_id);

   return;
} /*l1_sc_idle_g2l_state*/

/*===========================================================================

FUNCTION g2l_start_idle_lte_search

DESCRIPTION
  Change G2L idle state machine to SCHEDULE state if frames are available.
  For fast search we use a longer gap to allow multiple searches to take place.


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void g2l_start_idle_lte_search(gas_id_t gas_id)
{
    /* Check if the frames are available now or reserve */
    if(g2l_request_or_reserve_lte_meas(gas_id))
    {
      l1_sc_schedule_G2L_timed_search(FALSE, gas_id);
      gl1_drx_require_next_tick(gas_id);
    }

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/

    return;
} /* g2l_start_idle_lte_search */

/*===========================================================================

FUNCTION l1_sci_g2l_idle_aborted

DESCRIPTION
  Callback to be called when an LTE search has to be aborted because
  layer1 needs frames that were previously available to the surround
  cell engine.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_g2l_idle_aborted( void* dummy, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  __UNUSED_ARG( dummy );

  MSG_GERAN_HIGH_3_G( "l1_sci_g2l_idle_aborted() in state=%d Fn=%d RAT %d" ,
           l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,gl1_get_FN(gas_id),l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset].rat);

  /* Called in Interrupt context */

   /* If active abort the message layer */

  gl1_irat_mutex_lock( gas_id);

  l1_sc_g2l_fast_search_abort(gas_id);

  switch(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state )
  {
     case GTOW_SRCH_STATE_ACTIVE:
     {
       if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE )
       {
          l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES;
          if (l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MEAS_RESTART)
          {
             /* In this place, we only handle quick cleanup for abort if we 
                haven't sent TIMED_SRCH_MEAS_REQ but have sent G2X STARTUP to GFW. 
                ** For those ABORT after sending TIMED_SRCH_MEAS_REQ, we will 
                handle cleanup in the l1_sc_lte_process_timed_search_results function */
             l1_sc_g2l_schedule_abort(gas_id );
          }
          else if (l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_CONTINUE && !g2l_get_abort_cnf_pending_status(gas_id))
          {
             L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ( gas_id );
             MSG_GERAN_HIGH_0_G("G2L TimedSrch: Abort req is sent in GTOW_SRCH_STATE_ACTIVE");
          }
       }
       else
       {
         MSG_GERAN_ERROR_1_G("LTE RAT is not active rat %d  in Active state",l1_sc_globals_ptr->active_irat);
       }
     }
     break;


     case GTOW_SRCH_STATE_SCHEDULE_LTE:
     {
       if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE )
       {
          if (l1_sc_g2l_empty_qta_gap[gas_id])
          {
            l1_sc_g2l_empty_qta_gap[gas_id] = FALSE;
            l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOL_SRCH_STATE_ABORT_CNF_WAIT;
            L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ(gas_id);
          }
          else
          {
            /* In this place, we only handle quick cleanup for abort if we haven't sent TIMED_SRCH_MEAS_REQ but have sent G2X STARTUP to GFW. 
             ** For those ABORT after sending TIMED_SRCH_MEAS_REQ, we will handle cleanup in the l1_sc_lte_process_timed_search_results function */
            l1_sc_g2l_schedule_abort(gas_id );
            l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES;
          }
       }
       else
       {
         MSG_GERAN_ERROR_1_G("LTE RAT is not active rat %d  in Active state",l1_sc_globals_ptr->active_irat);
       }
     }
     break;

     case GTOW_SRCH_STATE_CLEANUP_WAIT:
     {
       MSG_GERAN_HIGH_0_G("G2L TimedSrch: Abort is not supported. Stay in GTOW_SRCH_STATE_CLEANUP_WAIT and wait for sending REQ and receiving CNF from ML1");
     }
     break;

     case GTOW_SRCH_STATE_CLEANUP:
     {
       MSG_GERAN_HIGH_0_G("G2L TimedSrch: Abort is not supported. Stay in GTOW_SRCH_STATE_CLEANUP");
     }
     break;

     case GTOW_SRCH_STATE_ABORTING:
     {
        MSG_GERAN_HIGH_0_G("G2L TimedSrch: After abort from Active state, ran out of frames. Wait for CNF to clear states");
     }
     break;

     case GTOL_SRCH_STATE_ABORT_CNF_WAIT:
     {
       MSG_GERAN_HIGH_0_G("G2L TimedSrch: Waiting for ABORT_CNF. Stay in GTOL_SRCH_STATE_ABORT_CNF_WAIT");
     }
     break;

     default:
     {
       l1_sc_set_gtow_srch_state_null(gas_id);
       if ( (l1_sc_wcdma_data_ptr->drx_ref != 0) &&
          (IS_FRAME_NUM_LATER(l1_sc_wcdma_data_ptr->id_search_fn,gl1_get_FN( gas_id ))) )
        {
          /* remove old frame */
          gl1_drx_release_fn_tick( l1_sc_wcdma_data_ptr->id_search_fn, gas_id );
          l1_sc_wcdma_data_ptr->drx_ref = 0;
        }
       l1_fm_clear_required_frames(FM_PRI_LTE_IDSEARCH, gas_id);
     }
     break;
  }

  gl1_irat_mutex_unlock( gas_id);

  return;
} /*l1_sci_g2l_idle_aborted*/

/*===========================================================================

FUNCTION l1_sc_set_ml1_init

DESCRIPTION
  Set/Reset ml1_gap_init to decide whether GL1 has to call
  lte_ml1_irat_gap_init function or not.
  Only called once per G2L session on _G2L_INIT_CNF

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_set_ml1_init(boolean in_ml1_init, gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   l1_sc_wcdma_data_ptr->rr_params.ml1_gap_init = in_ml1_init;

   /*
   ** reset meas/det only once per G2L session, this brings BOLT into line with THOR implementation
   ** G2L results remain valid across Idle/Dedicated (unlike Dime).
   */
   g2l_reset_stored_det_cnts(gas_id);
   /* Reset G2L TT */
   gl1_msg_get_g2l_tt_acc(gas_id);

   return;
} /*l1_sc_set_ml1_init*/

/*===========================================================================

FUNCTION l1_sc_schedule_G2L_timed_search

DESCRIPTION
  Prepare the gap for the next measurement and schedule the measurement
  for G2L Idle mode timed search on the next tick.
  For fast restart, initiated from meas_cnf of preceding G2L measurement

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void  l1_sc_schedule_G2L_timed_search( boolean fast_restart , gas_id_t gas_id)
{
     l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
     l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

     uint16 g2l_idle_frame = frame_counters[gas_id].FNmod51;

     /* copy scheduled arfcn details */
     l1_sc_wcdma_data_ptr->wcdma_srch.sched_earfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[ l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset];

     l1_sc_globals_ptr->active_irat              = RR_L1_IRAT_RAT_LTE;
     l1_sc_globals_ptr->wcdma_srch_in_progress   = TRUE;
     L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_LTE_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);

     /*
     ** As per comments from ML1, The idle mode interface would start ASAP, so GL1 should have finished G2X Startup before
     ** sending the messages to ML1. If you want to specify a time you can but it should be ~10ms in advance of the start time.
     ** If you have a use case in between we can discuss.  We should probably try to limit the change in the first version.
     ** Issue command to mdsp
     **   FN-2      FN-1         FN ....     FN+14     FN+15
     ** Current   send TIMED   GAPstart      GAPend    Call back from FM if activity doesn't complete
     **
     ** g2l_start_frame and g2l_end_frame are used to issue mdsp command.
     ** While issuing startup and clean FNmod51 is compared with g2l_start_frame and g2l_end_frame
     */
     l1_sc_wcdma_data_ptr->g2l_end_frame   = ADD_MOD(g2l_idle_frame, (MAX_LTE_SEARCH_FRAMES), 51);   /* mdsp end point is +gap -1 from now   */

     gl1_msg_suspend_pwr_meas(gas_id);

     l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_ID, gas_id);

     if(fast_restart)
     {
        /* restart next measuremnt on next isr, large FM gap is left open */
        l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap            = G2L_MEAS_RESTART;
        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state            = GTOW_SRCH_STATE_ACTIVE;
        l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_START_CMD_IDX] = G2X_FAST_START_END_OF_FRAME_QS;  /* for fast start cleanup/startup OFFSETS REVERSED sent in same frame */
     }
     else
     {
        l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap            = G2L_MDSP_CONTINUE;
        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state            = GTOW_SRCH_STATE_SCHEDULE_LTE;   /* next tick_wcdma -> LTE_IDLE_GAP_SCHEDULE */
        /* ggap_g2l_mdsp represents offset passed to mdsp command*/
        l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_START_CMD_IDX] = GL1_EXTRA_LTE_STARTUP_MARGIN;

     }
     /*
     ** Idle/Connected mode are different in how the gap is opened relative to the timed_meas_req to ML1
     ** Idle: in advance of the timed_meas_req          asap_gap_start == TRUE
     ** cm  : is sent 2 frames before the gap is opened asap_gap_start == FALSE
     */

     /* Don't send the startup command to GFW here if this is a multisim idle
        gap (QTA or normal G2X).  In QTA, the startup is sent in QTA procedure,
        and in all other multisim idle it's NULL2X and startup isn't needed. */
     if(!l1_sc_x2g_ta_irat_gap(gas_id))
     {
       /* Make sure this flag is reset at the beginning of gap.
          Set it to TRUE once G2L RF enter is issued. */
       l1_sc_wcdma_data_ptr->rfTuneback_required = FALSE;
        
       mdsp_g2x_startup_cmd(l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_START_CMD_IDX], 
                            FALSE, 
                            MDSP_RXLM_BUF_IDX_INVALID, 
                            FALSE,
                            gas_id);    /* open the GFW gap in advance of idle frame */
     }

} /* l1_sc_schedule_G2L_timed_search */


/*===========================================================================

FUNCTION l1_g2l_connected_restart

DESCRIPTION
  return gap count cntTimed_Req, if non-zero initiates next gap measurement

  If moved to SDCCH , return 0 to restart measurement on same freq in sdcch mode.

DEPENDENCIES
  none

RETURN VALUE
  rr_params.cntTimed_Req

SIDE EFFECTS
  None

===========================================================================*/


uint16 l1_g2l_connected_restart(gas_id_t gas_id)
{
   l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   uint16 gap_status = l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req;      /* non-zero when G2L in progress */


   if(gap_status && (l1_sc_globals_ptr->mode == L1SCModeDediSDCCh))       /* restart measurement again as SDCCH */
   {
       gap_status = 0;

       l1_sc_wcdma_data_ptr->g2l_inhibit_next_update = TRUE;              /* start again on same freq, l1_sc_wcdma_ded_ready() */
   }

   return gap_status;
}

/*===========================================================================

FUNCTION l1_g2l_trigger_mdsp

DESCRIPTION
  Trigger function to send g2x startup/cleanup to mdsp to open/close gap.

  g2l_start_frame = INVALID for idle
  This must run every l1_isr frame tick when search is active idle or connected mode
  tuneback_to_gsm is only FALSE for G2L schedule abort sceanrio.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void l1_g2l_trigger_mdsp( boolean g2l_idle, gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   uint16  g2l_mod_FN;

   if(g2l_idle == TRUE)
   {
     g2l_mod_FN = frame_counters[gas_id].FNmod51;
   }
   else
   {
     g2l_mod_FN = frame_counters[gas_id].FNmod26;
   }

   /*
   ** runs from isr every frame tick when search is active idle or connected mode
   */
   
   if (l1_sc_wcdma_data_ptr->wcdma_srch.abort)
   {
      if (!l1_sc_g2l_build_script_pending[gas_id])
      {
         l1_sc_g2l_abort(g2l_idle, gas_id);
      }
      else
      {
         return;
      }
   }

   if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MEAS_RESTART && g2l_idle )
   {
      /*
      ** restart now on this isr, do not wait for l1_sci_tick_wcdma() calls gl1_hw_g2l_rf_meas_enter and TIMED_SRCH_MEAS_REQ
      */
      l1_sc_idle_g2l_state(LTE_IDLE_GAP_SCHEDULE, gas_id);
      l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_CONTINUE;
   }
   else
   if((l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_START) && (g2l_mod_FN==l1_sc_wcdma_data_ptr->g2l_start_frame))
   {
      gl1_msg_suspend_pwr_meas(gas_id);

      /* Don't send the startup command to GFW here if this is a multisim idle
         gap (QTA or normal G2X).  In QTA, the startup is sent in QTA procedure,
         and in all other multisim idle it's NULL2X and startup isn't needed. */
      if(!l1_sc_x2g_ta_irat_gap(gas_id))
      {
         mdsp_g2x_startup_cmd(l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_START_CMD_IDX], 
                              FALSE, MDSP_RXLM_BUF_IDX_INVALID, FALSE, gas_id  );    /* open the GFW gap in advance of idle frame */
      }
      l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_CONTINUE;
   }
   else
   if(   (l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_FINISH_EARLY)
      ||((l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_CONTINUE) && (g2l_mod_FN==l1_sc_wcdma_data_ptr->g2l_end_frame))
     )
   {
     /* we've finished early or our frames are exhausted */

      uint32 frame_pos_qs = 0xFFFFFFFF;

     /* Don't send the cleanup command to GFW here if this is a multisim idle
        gap (QTA or normal G2X).  In QTA, the cleanup is sent in QTA procedure,
        and in all other multisim idle it's NULL2X and cleanup isn't needed. */
      if (!l1_sc_x2g_ta_irat_gap(gas_id))
      {
         if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState & FAST_SEARCH_START && g2l_idle ) /* adjust offset for cleanup */
         {
           /*
           ** run G2X_cleanup/startup in this current frame giving more margin before running gl1_hw_g2l_rf_meas_enter() in the next frame
           */
           frame_pos_qs = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));

           /* offset margin between G2X cleanup and startup in the same frame */
           if( frame_pos_qs < G2L_FAST_RESTART_CLEANUP_OFFSET )
           {
             /* use current frame pos as new offset and send immediately */
             l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_CLEANUP_CMD_IDX] =  frame_pos_qs;   /* +625 FTSM gets added to this offset by mdsp */
           }
         }

         mdsp_g2x_cleanup_cmd(l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_CLEANUP_CMD_IDX] , 
                              gl1_hw_get_gsm_rxlm_buf_idx(gas_id), 
                              l1_sc_wcdma_data_ptr->rfTuneback_required,
                              #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
                              (g2l_idle && l1_sc_g2x_timeline_opt_enable[gas_id] && l1_sc_wcdma_data_ptr->rfTuneback_required),
                              #else
                              FALSE,
                              #endif
                              gas_id);  /* close the GFW gap end of idle frame  */

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
         if (g2l_idle && l1_sc_g2x_timeline_opt_enable[gas_id] && l1_sc_wcdma_data_ptr->rfTuneback_required)
         {
           mdsp_async_intf_send_immediate_cmd(GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);
           /*here clean up command sent immediately*/
           cleanup_cmd_sent[gas_id] = FALSE;
         }
         else
         {
           /*Mark clean cmd sent true if we are not sending immediate command*/
           if(g2l_idle)
           {
              cleanup_cmd_sent[gas_id] = TRUE;
              MSG_GERAN_HIGH_3_G("G2L cleanup: Issue cleanup here for next frame srch_state %d, g2l gap %d, cleanup_cmd_sent %d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                               l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap,cleanup_cmd_sent[gas_id]);
           }
         }
         
         l1_sc_wcdma_data_ptr->rfTuneback_required = FALSE;
         
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
      }

      gl1_irat_mutex_lock( gas_id);

      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP;          /* check when gl1_hw_qta_gap_active() ? */
      l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_CLEANUP_COMPLETED;       /* check startup/cleanup both sent on cnf */

      gl1_irat_mutex_unlock( gas_id);
   }
   else
   if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_CLEANUP_COMPLETED)
   {
      if(!(l1_sc_wcdma_data_ptr->rr_params.fastSearchState & FAST_SEARCH_START))
      {
         gl1_msg_resume_pwr_meas(gas_id);
      }

      gl1_irat_mutex_lock( gas_id);

      /*If Gap has been aborted by ML1, then we need to issue rf_meas_exit here after mdsp_g2x_cleanup_cmd */
      if(l1_sc_wcdma_data_ptr->g2x_aborted)
      {
        if (gl1_hw_get_g2x_rf_enter_called_last(gas_id))
        {
           g2l_irat_x2l_meas_exit(TRUE, gas_id);
           gl1_hw_set_g2x_rf_enter_called_last(FALSE, gas_id);
        }
        l1_sc_wcdma_data_ptr->g2x_aborted = FALSE;
      }

      if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState & FAST_SEARCH_START && g2l_idle ) /* daisy chain next measurement */
      {
         cleanup_cmd_sent[gas_id] = FALSE;
         MSG_GERAN_HIGH_1_G("G2L cleanup: Reset cleanup_cmd_sent to %d ",cleanup_cmd_sent[gas_id]);
         /*
         ** on the last fast search gap measurement , FAST_SEARCH_START is not set so normal cleanup sequence follows to close gap
         ** sets active srch state and new end frame,
         */
         l1_sc_wcdma_data_ptr->rr_params.fastSearchState = FAST_SEARCH_ACTIVE;
         
         /* 
            G2X_startup required for ML1 sent at the end of frame
            This call updates l1_sc_trigger_g2l_gap=G2L_MEAS_RESTART,
            id_state=GTOW_SRCH_STATE_ACTIVE and runs mdsp_g2x_startup_cmd()
         */
         l1_sc_schedule_G2L_timed_search( TRUE , gas_id);
         
         /* This is a blocking call, send G2X startup in this frame */
         mdsp_async_intf_send_immediate_cmd(GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);
      }
      else
      {
         l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_END;

         #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
         if (g2l_idle && l1_sc_g2x_timeline_opt_enable[gas_id])
         {
           l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_NULL;
           l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
         }
         #endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
       }

      gl1_irat_mutex_unlock( gas_id);
   }
   else
   if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_END)
   {
      /* gap cleanup completed */
      gl1_irat_mutex_lock( gas_id);

      /* action any cm abort */
      /* cm active with abort and no cm mode cnf pending */
      if((l1_sc_wcdma_data_ptr->lte_cm_irat_active & (G2L_CM_ACTIVE_AND_CNF_PENDING)) == G2L_CM_ACTIVE)
      {
          /*
          ** action only if no cnf is pending, else handled in g2l_process_state_cnf()
          */
          if(l1_sc_wcdma_data_ptr->lte_cm_irat_active & G2L_CM_STOP)
          {
              /* cm aborted */
              g2l_set_cm_inactive(gas_id);
          }
          else
          {
              /*
              ** In idle the sm will tick thru GTOW_SRCH_STATE_CLEANUP_WAIT, so only apply in cm.
              ** 2 scenarios, both checked in common gap state G2L_MDSP_NULL
              **    i) timed_cnf arrives after  gap cleanup has finished, so _cnf processing calls state_null immediately
              **   ii) timed_cnf arrives during gap cleanup so is called here after cleanup completes.
              */
              l1_sc_set_gtow_srch_state_null (gas_id);
          }
      }

      l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_NULL;

      gl1_irat_mutex_unlock( gas_id);
   }
   else
   if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MEAS_RESTART_DELAY)
   {
      /* Fast restart was delayed by 1 frame tick */
      l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MEAS_RESTART;
   }

#if defined(FS_EXTRA_DEBUG)
   if( (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)  /* F3 only when active */
     &&(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap > G2L_MDSP_CONTINUE))
   {
      MSG_GERAN_HIGH_2_G("G2L: trig fast search active =0x%X g2l_gap=%d" , l1_sc_wcdma_data_ptr->rr_params.fastSearchState , l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap );
   }
#endif

} /* l1_g2l_trigger_mdsp */


/*===========================================================================

FUNCTION g2l_process_state_cnf

DESCRIPTION
  process gap_cnf states
  runs within gl1_irat_mutex_lock()


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
static void  g2l_process_state_cnf( boolean g2l_idle, uint16 freq_idx , boolean aborted,  gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  g2l_gap_cnf(g2l_idle, aborted,gas_id);     /* common mdsp gap state timing */

  if(g2l_idle == TRUE)
  {

     if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState & FAST_SEARCH_ACTIVE)
     {
         MSG_GERAN_HIGH_2_G("G2L: _cnf fast search active =0x%X g2l_gap=%d" , l1_sc_wcdma_data_ptr->rr_params.fastSearchState , l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap );
         /* on last measurement of fast sequence, active is cleared so cleanup follows as normal below */
     }
     else
     {
         /*
         ** for Idle mode, this only occurs when the allocated frames have been exhausted (as for connected mode ).
         ** Normally for Idle (and SDCCH ) _cnf initiates gap closure for G2X cleanup.
         ** If cleanup has already completed then goto NULL state immediately
         */
         if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_NULL)
         {
            /* if active state null has not yet run */
            if(l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE)
            {
               l1_sc_set_gtow_srch_state_null (gas_id);
            }
         }

     } /* !fast search */

     if(freq_idx < MAX_WCDMA_FREQS)
     {
       l1_sc_prune_meas_lte_cells(freq_idx, g2l_idle, TRUE, FALSE, gas_id);         /* Idle reprune, remove any stale cells before sending results */

       l1_sc_send_lte_cell_search_results_to_rr( freq_idx, gas_id );
     }
  }
  else  /* connected mode measurement,usually confirm comes after the gap has closed */
  {
     /*
     ** for connected mode, the gap is already closed and cleanup completed
     ** so then goto NULL state immediately or action cm abort
     */
     if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_NULL)
     {
        if(l1_sc_wcdma_data_ptr->lte_cm_irat_active & G2L_CM_STOP)
        {
           g2l_set_cm_inactive(gas_id);
        }
        else
        if(l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE)
        {
           /* if active state null has not yet run */
           l1_sc_set_gtow_srch_state_null (gas_id);
        }
     }
     /*
     ** if 0 then we may have stopped due to cell update list change or abort
     */
     if(l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req )
     {
        l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf++;
     }
     if(freq_idx < MAX_WCDMA_FREQS)
     {

      /*
      ** ML1 has finished measuring this frequency, flag is set on the timed_req via ML1 api lte_ml1_irat_setup_schedule()
      */
      if(TRUE==l1_sc_wcdma_data_ptr->rr_params.use_next_freq)
      {
        uint16  next_idx = l1_sc_wcdma_data_ptr->rr_params.nextFreqIdx; 
        /*
             ** send results to rr
             */
        MSG_GERAN_HIGH_2_G("G2L: Meas completed on gap=%d freq_idx=%d ", l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req , freq_idx);
        /*
              ** Final prune to account for only detected measurement scenarios
              ** re-prune for stale measurements
              */
        l1_sc_prune_meas_lte_cells(freq_idx, g2l_idle, TRUE, FALSE, gas_id);   /* cm reprune, remove any stale cells before sending results */
   
        /* update only once in cm on final gap after re-prune */
        if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells )
        {
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_status = G2X_FS_NEWLY_MEASURED;
          l1_sc_wcdma_data_ptr->rr_params.fastSearchNumMeasured++;   /* cm number of freq with detected/measured cells */
        }
        else
        {
          l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_status = G2X_FS_NO_RESULT;
        }
        
        l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas |= (1<<freq_idx);   /* for xfer/ded only record as measured on the 5th gap */
        /*
              ** only send when ML1 has finished, not always 5th gap
              */
        l1_sc_send_lte_cell_search_results_to_rr( freq_idx, gas_id );
            
        l1_sc_g2l_fast_search_dedicated(gas_id);
           
        if( (freq_idx == next_idx)||(l1_sc_wcdma_data_ptr->rr_params.freq_list[next_idx].freq_status == G2X_FS_BLACKLISTED))
        {
          l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf = G2L_5_GAPS_USED;  /* finish early */
          l1_sc_wcdma_data_ptr->rr_params.ml1_gap_init = TRUE;             /* if we finish early, ensure next ML1 gap sequence is aligned */
        }
      }

      if(l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf == G2L_5_GAPS_USED)
      {
         uint16 next_ded_tick = l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req;   /* number of gaps used for this measurement */
         /*
         ** finished measurement for this interval
         */
         l1_sc_wcdma_data_ptr->rr_params.cntTimed_Cnf=0;  /* reset for next scheduled series of gaps */
         l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req=0;

         /*
         ** update ded_tick to ensure gap to next TCH measurement uses only 1:4 of idle frames available for irat.
         ** For PTM it remains fixed at 3 idle frames.
         */
         if(l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
         {
            next_ded_tick = 1;       /* for PTM keep next gap fixed at +3 idle frames for GSM */
         }
         l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick = l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick + (next_ded_tick*NUM_IDLE_FRAMES_PER_DED_SRCH_G2L);
      }

      g2l_move_next_freq(gas_id);            /* resets use_next_freq and sets new nextFreqIdx search index */

     }
  }
} /* g2l_process_state_cnf */


/*===========================================================================

FUNCTION g2l_gap_cnf

DESCRIPTION
  g2l measurement confirm , close gap if finished early.
  runs within gl1_irat_mutex_lock()


DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
static void g2l_gap_cnf( boolean g2l_idle ,boolean aborted, gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   gsmtr_fn_qs_type   current_fn_qs;

    if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_CONTINUE)     /* check to see if sdcch finishes early before cleanup is sent */
    {
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
      if (!l1_sc_g2x_timeline_opt_enable[gas_id])
      {
        l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_FINISH_EARLY;
      }
      else if(l1_sc_x2g_ta_irat_gap(gas_id))
      {
        /* Don't send the cleanup command to GFW here if this is a multisim idle
           gap (QTA or normal G2X).  In QTA, the cleanup is sent in QTA procedure,
           and in all other multisim idle it's NULL2X and cleanup isn't needed. */
        l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_CLEANUP_COMPLETED;
      }
      else
      {
        /* read current FN, QS */
        current_fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

        /* We check if task runs before ISR. If HW_FN != SW_FN, then go to option 2 */
		/* Check to avoid giving immediate cleanup when we are  yet not past the startup offset */
        if ( (!aborted) && (current_fn_qs.fn) == GSTMR_GET_FN_GERAN(gas_id)  )
        {
          /* Option1: send cleanup now in task */
          if (current_fn_qs.qs < G2X_MAX_OFFSET_FOR_PROCESS_CLEANUP)
          {
            l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_CLEANUP_COMPLETED;

            mdsp_g2x_cleanup_cmd(0, 
                                 gl1_hw_get_gsm_rxlm_buf_idx(gas_id), 
                                 TRUE, 
                                 TRUE,
                                 gas_id);

            mdsp_async_intf_send_immediate_cmd(GSTMR_GET_FN_GERAN(gas_id), FALSE, gas_id);

            l1_sc_wcdma_data_ptr->rfTuneback_required = FALSE;
            
          }
          else
          {
            /* Option 2: send cleanup in ISR */
            l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_FINISH_EARLY;
          }
        }
        else
        {
          /* Option 2: send cleanup in ISR */
          l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_FINISH_EARLY;
          MSG_GERAN_HIGH_2_G("G2L: Task runs before ISR, sw_fn =%d, hw_fn=%d",
                             GSTMR_GET_FN_GERAN(gas_id), current_fn_qs.fn);
        }
      }
#else
      l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_FINISH_EARLY;
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */

      l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_CLEANUP_CMD_IDX] = G2X_FAST_START_START_OF_FRAME_QS;

      MSG_GERAN_MED_3_G("G2L: gap finished early in idle=%d end=%d Fn=%d", g2l_idle, ggap[gas_id].end.frame_number, gl1_get_FN(gas_id));
    }

    if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_START)
    {
        MSG_GERAN_ERROR_1_G("G2L: G2L_MDSP_START GAP not triggered idle=%d",  g2l_idle );
    }
    g2l_clr_Init_Sent_status(LTE_G2L_CNF_PENDING, gas_id);

    l1_sc_wcdma_data_ptr->lte_cm_irat_active &=~G2L_CM_CNF_PENDING;   /* clear cnf pending */

} /*  g2l_gap_cnf   */


/*===========================================================================

FUNCTION l1_g2l_cm_abort

DESCRIPTION
  Abort or leave dedicated mode.
  stop any active G2L connected mode measurement and cleanup CM.

DEPENDENCIES
  none

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/

void l1_g2l_cm_abort(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  curr_init_mode = g2l_get_Init_Sent_status(gas_id);
  /*
  ** for G2L CM always stop any measurement sequence whether in progress or not
  */

  if(l1_sc_wcdma_data_ptr->lte_cm_irat_active & G2L_CM_ACTIVE)
  {
    MSG_GERAN_HIGH_3_G("G2L:Aborting stopped on gap=%d init=0x%x CM=0x%x", l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req, curr_init_mode, l1_sc_wcdma_data_ptr->lte_cm_irat_active);

    gl1_irat_mutex_lock( gas_id);

    if( l1_sc_wcdma_data_ptr->lte_cm_irat_active & G2L_CM_CNF_PENDING )     /* cm gap_cnf is pending */
    {
        l1_sc_wcdma_data_ptr->lte_cm_irat_active |= G2L_CM_STOP;            /* process when gap_cnf is recieved */
    }
    else
    if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_NULL)
    {
        g2l_set_cm_inactive(gas_id);
        MSG_GERAN_HIGH_3_G("G2L: CM abort completed srch_state=%d abort=%d init=0x%x" , l1_sc_wcdma_data_ptr->wcdma_srch.srch_state , l1_sc_wcdma_data_ptr->wcdma_srch.abort, curr_init_mode );
    }
    else
    {
        l1_sc_wcdma_data_ptr->lte_cm_irat_active |= G2L_CM_STOP;   /* process when G2L_MDSP_END/NULL occurs */
    }

    gl1_irat_mutex_unlock( gas_id);
  }
} /* l1_g2l_cm_abort */



/*===========================================================================

FUNCTION g2l_lte_list_index_recovery

DESCRIPTION

   Until we have an abort mechanism after we have got into state  GTOW_SRCH_STATE_SCHEDULE_LTE
   We will always have a window where we cannot abort idle if the cell list
   changes and we don't have any LTE ncells present.
   Recover new index if arfcn is still in the list
   Or recover any index for an LTE rat entry in cell list.
   Or recover to dummy index using num_freq, as list cannot be full if no LTE are present.
   Use the saved sched_earfcn to recover a valid index and update
   l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset.


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

static void g2l_lte_list_index_recovery(gas_id_t gas_id)
{
      l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
      char* g2l_step ="none for";

      uint32 saved_earfcn = (uint32)l1_sc_wcdma_data_ptr->wcdma_srch.sched_earfcn.arfcn.euarfcn;
      uint16 new_index;

      /* if index has moved , recover index for arfcn */
      new_index = l1_sc_find_freq_index( saved_earfcn, RR_L1_IRAT_RAT_LTE, FALSE , gas_id);

      if(new_index !=INVALID_IDX)
      {
          g2l_step = "found";
          l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = new_index;
      }
      else
      {
         /* recover any index for an LTE RAT */
         new_index = l1_sc_find_freq_index( saved_earfcn, RR_L1_IRAT_RAT_LTE, TRUE , gas_id);

         if((new_index !=INVALID_IDX) && (new_index < MAX_WCDMA_FREQS))  /* KW fix */
         {
            g2l_step = "new rat";
            l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = new_index;
            saved_earfcn = (uint32)l1_sc_wcdma_data_ptr->rr_params.freq_list[new_index].arfcn.euarfcn;
         }
         else
         {
            /* No LTE in new list, but list cannot be full */
            new_index = l1_sc_wcdma_data_ptr->rr_params.num_freqs;
            if((new_index !=INVALID_IDX) && (new_index < MAX_WCDMA_FREQS))  /* KW fix */
            {
              g2l_step = "saved";
              /* copy temporarily into unused location */
              l1_sc_wcdma_data_ptr->rr_params.freq_list[new_index] = l1_sc_wcdma_data_ptr->wcdma_srch.sched_earfcn;
              l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = new_index;
              l1_sc_wcdma_data_ptr->rr_params.freq_list[new_index].rat = RR_L1_IRAT_RAT_LTE;

              saved_earfcn = (uint32)l1_sc_wcdma_data_ptr->rr_params.freq_list[new_index].arfcn.euarfcn;
            }
         }
      }
      MSG_GERAN_ERROR_3_G("G2L: recovered %s index=%d for earfcn=%d " , g2l_step , new_index, saved_earfcn);
} /* l1_sc_lte_list_index_recovery */

/*===========================================================================

FUNCTION l1_sc_lte_process_search_results

DESCRIPTION
    save the detected cells into GL1 structures rr_params and stored_meas

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/

static void  l1_sc_save_det_lte_cells( uint32 freq_idx, uint32 num_cells, lte_cphy_irat_meas_detected_cell_s *det_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint32  num_total_cells = num_cells;
  uint32  i;

  i = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;    /* index starts at 0 for idle, ded the number previously added for meas results */

  num_total_cells +=i;                            /* combined total meas+det */

  if(num_total_cells > G2L_MEAS_DET_MAX_CELLS )
  {
     num_total_cells = G2L_MEAS_DET_MAX_CELLS;
  }

  /* The search results have no duplicates */
  while( i < num_total_cells )
  {
      l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cell_id = det_ptr->cell_id;
      l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cp_type = det_ptr->cp_type;
      l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.state   = det_ptr->state;
      /*
      ** these parameters are saved and passed back for subsequent measurement
      */

      /* check what we add to stored, increase array size */

      /* G2L det HLD mentions snr for ranking ? , but not in det struct ??*/

      /* stored_meas params */
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].lte_cell.cell_id = det_ptr->cell_id;
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].lte_cell.cp_type = det_ptr->cp_type;
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].lte_cell.state   = det_ptr->state;

      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].det_cnt++;    /* this cell has been detected */

      if( l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrp_avg == 0 )
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrp_avg  = -1024;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrp_last = 0;
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrp_last2= 0;
      }
      i++;
      det_ptr++;
  }

  //info_ptr->measure_newly_detected_cells == TRUE
  l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells = num_total_cells;   /* for ded these are appended counts */
  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells         = num_total_cells;   /* for ded these are appended counts */

} /* l1_sc_save_det_lte_cells */

/*===========================================================================

FUNCTION l1_sc_save_meas_lte_cells

DESCRIPTION
    save the measured cells into GL1 structures rr_params and stored_meas
    need to check that we don't overwrite data to be saved , i.e. 2 from 5 before delete
    meas array size is doubled for both meas and det results.
    as we pass back the added detected for the next round of meas.
    This func will update or append the results,as determined by the initial
    value of rr_params.freq_list[freq_idx].num_cells

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
static void  l1_sc_save_meas_lte_cells( uint32 freq_idx, uint32 num_cells, lte_cphy_irat_meas_meas_results_s *meas_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  t_gsm_l1_lte_wcdma_cell_energy_details  bckup_cell_energy [G2L_CM_MAX_MEAS_CELLS];
  lte_phy_cell_id_t                       bckup_cell_id [G2L_CM_MAX_MEAS_CELLS];
  gsm_g2l_det_t                           bckup_cell_det[G2L_CM_MAX_MEAS_CELLS];

  uint32  num_stored_cells;
  uint32  num_total_cells;
  uint32  i;
  uint32  j;

  num_stored_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;  /* existing stored combined total, only 1st 9 are relevant */

  if(num_stored_cells > G2L_CM_MAX_MEAS_CELLS )    /* max 9, LFW limitation */
  {  num_stored_cells = G2L_CM_MAX_MEAS_CELLS;}

  for ( j=0; j < num_stored_cells;j++   )
  {
    bckup_cell_energy[j]= l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[j];
    bckup_cell_id[j]    = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[j].lte_cell.cell_id;
    bckup_cell_det[j]   = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[j];
  }

  /* index starts at 0 for idle, ded the number previously measured to append */
  i = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;

  num_total_cells = num_cells + i;

  if(num_total_cells > G2L_CM_MAX_MEAS_CELLS )    /* max 9, LFW limitation */
  {  num_total_cells = G2L_CM_MAX_MEAS_CELLS;}

  /* The search results have no duplicates */
  while( i < num_total_cells )
  {
    l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cell_id = meas_ptr->cell_id;
    l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.cp_type = meas_ptr->cp_type;
    l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[i].lte_cell.state   = meas_ptr->state;

    if( l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].lte_cell.cell_id != meas_ptr->cell_id )
    {
      MSG_GERAN_ERROR_1_G(" cell id has changed to %d ! ",  meas_ptr->cell_id );

      for (j=0 ; j < num_stored_cells ; j++)
      {
          if (bckup_cell_id [j] == meas_ptr->cell_id  )
          {
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i] = bckup_cell_energy [j];
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i]    = bckup_cell_det[j];
            MSG_GERAN_HIGH_2_G("G2L: bckup RSRP avg=%d found for freq_idx=%d",l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrp_avg, freq_idx );
            break;
          }
      }
    }
    /*
    ** check that we don't need to find existing and update, as above F3
    ** or copy before update and merge ???
    */
    /* stored_meas */

    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].lte_cell.cell_id   = meas_ptr->cell_id;
    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].lte_cell.cp_type   = meas_ptr->cp_type;
    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].lte_cell.state     = meas_ptr->state;

    /* add Antenna and rssi ?? , struct will need updating if added */
    l1_sc_g2x_filter_treatment(NULL, meas_ptr->rsrp, freq_idx, i, RR_L1_IRAT_RAT_LTE, gas_id);

    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrq_last = meas_ptr->rsrq;
    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].lte_cell.rsrq_avg  = meas_ptr->rsrq;

    l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_det[i].meas_cnt++;    /* this cell has been measured */

    i++;
    meas_ptr++;
  }

  /* after the detected cells are added next, this total can exceed G2L_CM_MAX_MEAS_CELLS */
  l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells = num_total_cells;
  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells         = num_total_cells;

} /* l1_sc_save_meas_lte_cells */

/*===========================================================================

FUNCTION l1_sc_g2l_timed_search_active

DESCRIPTION
            If G2L connected mode timed search has started (cntTimed_Req >0)
            then this function returns TRUE
DEPENDENCIES
  none

RETURN VALUE
  TRUE G2L connected search is active

SIDE EFFECTS
  none

===========================================================================*/
boolean l1_sc_g2l_timed_search_active( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  if( l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req > 0 )
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}



/*===========================================================================

FUNCTION g2l_get_next_lte_freq

DESCRIPTION
            get next LTE freq , ignores any priority order
            always > 4 LTE freqs in the list so not expected to fail.

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
static uint16 g2l_get_next_lte_freq(uint16 last_freq , gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   uint16 i,next_index;

   next_index = last_freq;

   for(i=0; i< l1_sc_wcdma_data_ptr->rr_params.num_freqs ; i++)
   {
     next_index++;

     if(l1_sc_wcdma_data_ptr->rr_params.num_freqs <= next_index )
     {
        next_index = 0;   /* wrap around */
     }
     /*
     ** loop until we find next LTE rat, ignores any priority order with other techs present
     */
     if( l1_sc_wcdma_data_ptr->rr_params.freq_list[next_index].rat == RR_L1_IRAT_RAT_LTE )
     {
         return next_index;
     }
  }

  return INVALID_IDX;   /* should never happen list is always valid */

}


/*===========================================================================

FUNCTION l1_sc_start_fast_srch

DESCRIPTION

  if > 4 LTE freq then use fast search to determine and blacklist fake/ghost frequencies from search list.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_sc_g2l_fast_search_start(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint16 freq_idx;

  if(l1_sc_wcdma_data_ptr->rr_params.num_L_freqs > FAST_SEARCH_ENABLE_LIMIT)    /* set to 4 for initial testing */
  {
     freq_idx = g2l_get_next_lte_freq(l1_sc_wcdma_data_ptr->rr_params.num_freqs, gas_id);  /* find the first highest priority LTE freq to be searched */

     if( freq_idx < MAX_WCDMA_FREQS)  /* KW fix */
     {

         l1_sc_wcdma_data_ptr->rr_params.fastSearchIndex     = freq_idx;

         l1_sc_wcdma_data_ptr->rr_params.fastSearchState     = FAST_SEARCH_ARMED;

         /* reset bit mask for each to be measured */
         l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas = 0;

         MSG_GERAN_HIGH_2_G("G2X; Start Fast Search on index=%d for euarfcn=%d", freq_idx ,l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn );
     }
  }
  else
  {
     l1_sc_wcdma_data_ptr->rr_params.fastSearchState     = FAST_SEARCH_INACTIVE;
     l1_sc_wcdma_data_ptr->rr_params.fastSearchIndex     = 0;
  }
  l1_sc_wcdma_data_ptr->rr_params.fastSearchLoopCount    = 0;
  l1_sc_wcdma_data_ptr->rr_params.fastSearchNumMeasured  = 0;
  l1_sc_wcdma_data_ptr->rr_params.FastSearchExpiryFN     = gl1_get_FN( gas_id );

} /* l1_sc_start_fast_srch */


/*===========================================================================

FUNCTION l1_sc_g2l_fast_search_restart

DESCRIPTION
         restarts the next G2L measurement by daisy chaining off the back of the G2X cleanup isr

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/


static boolean l1_sc_g2l_fast_search_restart( gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint32  current_FN = gl1_get_FN( gas_id );
  uint32  bFastSearchFreqMeas;
  uint16  next_freq           = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
  uint16  cur_id_freq         = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
  boolean fs_completed;

    /*get the next LTE freq, ignores any priority ordering so that all LTE freqs are measurered back to back until we finish or run out of frames
        For fast search on one EARFCN, move to next EARFCN in the list
    */
    next_freq = g2l_get_next_lte_freq(next_freq, gas_id);  /* get the next LTE freq index */
    bFastSearchFreqMeas = l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas;
    /*
    ** check fast list entries have all been measured, allow for cell updates removing entries
    */
    if( (bFastSearchFreqMeas & l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList) == l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList)
    {
      fs_completed = TRUE;
      /* all the G2L in freq list have been measured */

      l1_sc_wcdma_data_ptr->rr_params.fastSearchLoopCount++;           /* blacklist search limited              */
      l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas =0;          /* reset incase we need to repeat search */

    }
    else
    {
      fs_completed = FALSE;
    }
    /*
    ** Do not restart if abort is set or insufficiemnt frames remain
    */
    if(  (next_freq != INVALID_IDX) && !fs_completed && !l1_sc_wcdma_data_ptr->wcdma_srch.abort
        && !l1_sc_g2x_check_for_extend_srch_period(gas_id) && !gl1_hw_qta_gap_active(gas_id))
    {
      /* set the next search index, after rr report sent from  LTE_IDLE_GAP_CNF */
      l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = next_freq;

      l1_sc_wcdma_data_ptr->id_search_fn = current_FN;
      l1_fm_set_required_frame_number(FM_PRI_LTE_IDSEARCH, l1_sc_wcdma_data_ptr->id_search_fn , MAX_LTE_SEARCH_FRAMES, gas_id);

      if(g2l_can_lte_meas_go(MAX_LTE_SEARCH_FRAMES, gas_id))
      {
      l1_sc_wcdma_data_ptr->rr_params.fastSearchState |= FAST_SEARCH_START;
      /*
      ** sets active srch state and new end frame, TBD skips g2x startup clarification needed
      ** this is from mesage task context
      */
      MSG_GERAN_HIGH_3_G("G2L: Fast search restart fastSearchState=%d  meas=0x%X list=0x%X",
                            l1_sc_wcdma_data_ptr->rr_params.fastSearchState,
                            bFastSearchFreqMeas,
                            l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList );
    }
    else
    {
        /* Restore LTE IDSEARCH activity back to active if TRM denied request 
        ** since FM automatically flush and clear contents if TRM denied request 
        ** which will cause an error when we check whether current activity is IRAT */
        l1_fm_set_activity_running(FM_PRI_LTE_IDSEARCH, gas_id);
        l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = cur_id_freq;
      }
    }
    else
    {
      /*
      ** blacklist any ghost freqs and arm/rearm next fast search
      ** arm   -> measure again on next tick_wcdma if insufficient meas freqs found.
      ** rearm -> enough meas freqs found, clear blacklist after 30s has elapsed and restart.
      */
      
      l1_sc_wcdma_data_ptr->rr_params.fastSearchState = l1_sc_g2l_fast_search_blacklist(gas_id);     /*  is FAST_SEARCH_ARMED or FAST_SEARCH_REARMED */
      fs_completed = TRUE;

      MSG_GERAN_HIGH_3_G("G2L: Fast search check blacklist fastSearchState=%d  meas=0x%X list=0x%X",
                          l1_sc_wcdma_data_ptr->rr_params.fastSearchState,
                          bFastSearchFreqMeas,
                          l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList );

    }
    if( next_freq < MAX_WCDMA_FREQS)   /* sanity check */
    {
       l1_sc_wcdma_data_ptr->rr_params.fastSearchIndex  = next_freq;   /* update for next restart */
    }


    return  fs_completed;

} /* l1_sc_g2l_fast_search_restart */



/*===========================================================================

FUNCTION g2l_get_next_lte_freq_with_band_chk

DESCRIPTION
         for fast search with multiple measurements.
         get next LTE freq , ignores any priority order
         checks for valid trm band registration.

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/

static uint16 g2l_get_next_lte_freq_with_band_chk( uint16 freq_idx, gas_id_t gas_id)
{

 /*
 ** get the next LTE freq, ignores any priority ordering so that all LTE freqs are measured back to back until we finish or run out of frames.
 */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*
** For DR-DSDS/DSDS we must check TRM band registration on each new G2L measurement
*/
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  sys_sband_lte_e_type lte_band;
  uint16               g2l_count;
  boolean              band_valid = FALSE;
  uint16               next_freq  = freq_idx;


    g2l_count = l1_sc_wcdma_data_ptr->rr_params.num_L_freqs;

    while( !band_valid && --g2l_count)
    {
       next_freq  = g2l_get_next_lte_freq(next_freq, gas_id);              /* get the next LTE freq index */

       if( next_freq < MAX_WCDMA_FREQS)   /* sanity check */
       {
         if( lte_ml1_common_band_get_band_from_dl_earfcn( l1_sc_wcdma_data_ptr->rr_params.freq_list[next_freq].arfcn.euarfcn, &lte_band ))
         {
           /* check and register band */
           band_valid = grm_register_additional_irat_bands((uint32)lte_band, MAX_LTE_SEARCH_FRAMES, gas_id);
         }
         if(!band_valid)
         {
          /*
          ** skip measurement for this earfcn, mark as measured with NO_RESULT so fast search completes
          */
          l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas |= (1<<next_freq);
           /*Don't reset if freq status is newly measured*/
          if(l1_sc_wcdma_data_ptr->rr_params.freq_list[next_freq].freq_status != G2X_FS_NEWLY_MEASURED)
          {
            l1_sc_wcdma_data_ptr->rr_params.freq_list[next_freq].freq_status = G2X_FS_NO_RESULT;
          }

          MSG_GERAN_HIGH_2_G("G2L: Fast search band not valid, skipped index=%d for euarfcn=%d", next_freq ,l1_sc_wcdma_data_ptr->rr_params.freq_list[next_freq].arfcn.euarfcn );

          next_freq = INVALID_IDX; /* set invalid incase its the last loop entry */

         }
       }
    }
#else

  uint16  next_freq;

  next_freq = g2l_get_next_lte_freq(freq_idx, gas_id);         /* for SS just get the next LTE freq index */

#endif

 return next_freq ;

}


/*===========================================================================

FUNCTION l1_sc_g2l_fast_search_dedicated

DESCRIPTION
         checks for blacklisted frequencies in dedicated mode.

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/

static void l1_sc_g2l_fast_search_dedicated( gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint32  bFastSearchFreqMeas = l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas;

  /*
  ** idle maybe interrupted by dedicated, but still check for blacklisting
  */

  if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState == FAST_SEARCH_REARMED )
  {
       l1_sc_g2l_fast_search_expiry(gas_id);
  }
  else
  if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState & (FAST_SEARCH_ACTIVE|FAST_SEARCH_ARMED) )
  {
    /*
    ** check fast list entries have all been measured, allow for cell updates removing entries
    */
    if( (bFastSearchFreqMeas & l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList) == l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList)
    {
      /* all the G2L in freq list have been measured */

      l1_sc_wcdma_data_ptr->rr_params.fastSearchLoopCount++;           /* blacklist search limited              */
      l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas =0;          /* reset incase we need to repeat search */

      /*
      ** blacklist any ghost freqs and arm/rearm next fast search
      ** arm   -> measure again on next tick_wcdma if insufficient meas freqs found.
      ** rearm -> enough meas freqs found, clear blacklist after 30s has elapsed and restart.
      */
      MSG_GERAN_HIGH_3_G("G2L: Fast search check blacklist frames_remaining=%d  meas=0x%X list=0x%X", 0, bFastSearchFreqMeas,  l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList );
      l1_sc_wcdma_data_ptr->rr_params.fastSearchState = l1_sc_g2l_fast_search_blacklist(gas_id);     /*  is FAST_SEARCH_ARMED or FAST_SEARCH_REARMED */
    }
  }
} /* l1_sc_g2l_fast_search_dedicated */

/*===========================================================================

FUNCTION l1_sc_g2l_fast_search_top3

DESCRIPTION

     After measuring the completed list of LTE frequencies
     Determine blacklisted frequencies and mark status.
     If > 3 freq , pick top 3 to be measured normally.
     Only called when > 3 in list.

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
static void   l1_sc_g2l_fast_search_top3(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  top_3_indexlist[3] = {INVALID_IDX, INVALID_IDX,INVALID_IDX };
   int16  top_3_powerlist[3] = { -1024,-1024,-1024 };

  int16  max_pwr = -1024;

  uint16 i,k;
  uint16 num_cells;

  for(i=0; i< l1_sc_wcdma_data_ptr->rr_params.num_freqs ; i++)
  {
     if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat == RR_L1_IRAT_RAT_LTE )
     {

       num_cells = l1_sc_wcdma_data_ptr->rr_params.freq_list[i].num_cells;
       if( num_cells )
       {
           max_pwr = -1024;

           for(k=0; k < num_cells ; k++)
           {
                /*
                ** if multiple cells exist on this freq find max pwr
                */
                if(l1_sc_wcdma_data_ptr->stored_meas[i].cell_energy[k].lte_cell.rsrp_last > max_pwr )
                {
                   max_pwr = l1_sc_wcdma_data_ptr->stored_meas[i].cell_energy[k].lte_cell.rsrp_last;
                }
           }

           if( top_3_powerlist[0] <= max_pwr  )
           {
               top_3_powerlist[2] = top_3_powerlist[1];
               top_3_indexlist[2] = top_3_indexlist[1];

               top_3_indexlist[1] = top_3_indexlist[0];
               top_3_powerlist[1] = top_3_powerlist[0];
               top_3_powerlist[0] = max_pwr;               /* new top power      */
               top_3_indexlist[0] = i;                     /* index of top power */
           }
           else
           if( top_3_powerlist[1] <= max_pwr )
           {
               top_3_powerlist[2] = top_3_powerlist[1];
               top_3_indexlist[2] = top_3_indexlist[1];

               top_3_powerlist[1] = max_pwr;               /* new top power-1       */
               top_3_indexlist[1] = i;                     /* index of top power -1 */

           }
           else
           if( top_3_powerlist[2] <= max_pwr)
           {
               top_3_powerlist[2] = max_pwr;               /* new top power -2      */
               top_3_indexlist[2] = i;                     /* index of top power -2 */
           }
       }
     }
  }


  /*
  ** blacklist remainder
  */
  for(i=0; i< l1_sc_wcdma_data_ptr->rr_params.num_freqs ; i++)
  {
     if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat == RR_L1_IRAT_RAT_LTE )
     {
       if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_status != G2X_FS_BLACKLISTED)
       {
          /*Blacklist those indexes which are not in top 3*/
          if(( i != top_3_indexlist[0]) && ( i != top_3_indexlist[1]) && ( i != top_3_indexlist[2]) )
          {
             l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_status = G2X_FS_BLACKLISTED;
             l1_sc_wcdma_data_ptr->rr_params.freq_list[i].num_cells   = 0;
             l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_age    = 0;
             MSG_GERAN_HIGH_2_G("G2L: top 3  blacklisted index=%d earfcn=%d", i , l1_sc_wcdma_data_ptr->rr_params.freq_list[i].arfcn.euarfcn);

          }
       }
     }
  }


  MSG_GERAN_HIGH_3_G("G2L: top 3 results freq_index %d %d %d", top_3_indexlist[0], top_3_indexlist[1], top_3_indexlist[2] );
  MSG_GERAN_HIGH_3_G("G2L: top 3 results pwr %d %d %d",        top_3_powerlist[0], top_3_powerlist[1], top_3_powerlist[2] );

   /*
   ** not convinced there's any need for this as we currently measure upto 6
   ** and we expecting not all freqs to be present.
   */

}  /* l1_sc_g2l_fast_search_top3 */


/*===========================================================================

FUNCTION l1_sc_g2l_fast_search_blacklist

DESCRIPTION

     After measuring the completed list of LTE frequencies
     Determine blacklisted frequencies and mark status.
     If > 3 freq , pick top 3 to be measured normally.
     If 0 or more freq after 2 loops of whole list , stop fast search and set 30s timer.

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
static g2x_fast_search_state_e l1_sc_g2l_fast_search_blacklist( gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  g2x_fast_search_state_e fast_state;

  uint32 i;
  uint16 num_blacklisted =0;
  uint16 num_newlymeas   =0;
  uint16 search_cnt;


  if(  (l1_sc_wcdma_data_ptr->rr_params.fastSearchNumMeasured > 2 )     /* 3 or more freqs with cells to be measured normally */
    || (l1_sc_wcdma_data_ptr->rr_params.fastSearchLoopCount   > 1))     /* looped twice on whole list, 0 or more freqs with cells to be measured normally */
  {
      /* blacklist remainder */

      for(i=0; i< l1_sc_wcdma_data_ptr->rr_params.num_freqs ; i++)
      {
         if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat == RR_L1_IRAT_RAT_LTE )
         {
           if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_status == G2X_FS_NO_RESULT )
           {
             l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_status = G2X_FS_BLACKLISTED;
             l1_sc_wcdma_data_ptr->rr_params.freq_list[i].num_cells   = 0;
             l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_age    = 0;

             num_blacklisted++;
#if defined(FS_EXTRA_DEBUG)
             MSG_GERAN_HIGH_2_G("G2L: Fast search blacklist index=%d for euarfcn=%d", i ,l1_sc_wcdma_data_ptr->rr_params.freq_list[i].arfcn.euarfcn );
#endif
           }
           else
           if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_status == G2X_FS_NEWLY_MEASURED )
           {
               num_newlymeas++;
           }
         }
      }
   }
   /*
   ** sanity check num_meas, possible on 2 passes that counts don't tally.
   */
   if(l1_sc_wcdma_data_ptr->rr_params.fastSearchNumMeasured != num_newlymeas )
   {
      MSG_GERAN_HIGH_2_G("G2L: Fast search num_meas=%d newly_meas=%d", l1_sc_wcdma_data_ptr->rr_params.fastSearchNumMeasured ,num_newlymeas );
   }

   if(num_newlymeas > 3)
   {
      l1_sc_g2l_fast_search_top3(gas_id);
   }

   search_cnt = l1_sc_wcdma_data_ptr->rr_params.fastSearchLoopCount; /* copy for F3  */
   /*
   ** stop on first pass if sufficient have been measured already, else stop on second pass regardless
   */
   if((num_newlymeas > 2) || (l1_sc_wcdma_data_ptr->rr_params.fastSearchLoopCount > 1))
   {
      fast_state = FAST_SEARCH_REARMED;    /* stop and set 30s timer */

      l1_sc_wcdma_data_ptr->rr_params.FastSearchExpiryFN    = ADD_FN( gl1_get_FN( gas_id), FS_FRAMES_FOR_30S );
      l1_sc_wcdma_data_ptr->rr_params.fastSearchNumMeasured = 0;
      l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas   = 0;  /* reset for next interval, can also be set by xfer meas */
      l1_sc_wcdma_data_ptr->rr_params.fastSearchLoopCount   = 0;
   }
   else
   {
      /*
      ** continue thru LTE freq list once more on next tick_wcdma
      ** list not yet all measured may have been aborted or ran out of frames.
      */
      fast_state = FAST_SEARCH_ARMED;
      l1_sc_wcdma_data_ptr->rr_params.FastSearchExpiryFN = gl1_get_FN( gas_id );
   }

   MSG_GERAN_HIGH_4_G("G2L: Fast search fast_state=0x%X num_meas=%d blacklisted=%d search_cnt=%d", fast_state ,num_newlymeas, num_blacklisted, search_cnt );

   return fast_state;

} /* l1_sc_g2l_fast_search_blacklist */

/*===========================================================================

FUNCTION l1_sc_g2l_fast_search_expiry

DESCRIPTION


DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_g2l_fast_search_expiry( gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   uint32 i;
   uint32 frame_now = gl1_get_FN(gas_id);

   if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState == FAST_SEARCH_REARMED )
   {
       if( IS_FRAME_NUM_LATER( frame_now, l1_sc_wcdma_data_ptr->rr_params.FastSearchExpiryFN ))
       {
           /* timer expired , clear blacklisted freqs and re-start fast search */

           for(i=0; i< l1_sc_wcdma_data_ptr->rr_params.num_freqs ; i++)
           {
              if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].rat == RR_L1_IRAT_RAT_LTE )
              {
                if( l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_status == G2X_FS_BLACKLISTED )
                {
                  l1_sc_wcdma_data_ptr->rr_params.freq_list[i].freq_status = G2X_FS_NOT_MEASURED;
                }
              }
           }

           l1_sc_wcdma_data_ptr->rr_params.fastSearchState = FAST_SEARCH_ARMED;
           l1_sc_wcdma_data_ptr->rr_params.fastSearchNumMeasured = 0;

           MSG_GERAN_HIGH_1_G("G2L: Fast search ARMED timer expiry blacklist cleared  FN=%d", frame_now );
       }
   }
} /* l1_sc_g2l_fast_search_expiry */

/*===========================================================================

FUNCTION l1_sc_g2l_fast_search_abort

DESCRIPTION
          abort current fast search sequence and re-arm for next wcdma_tick
DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_g2l_fast_search_abort( gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState & FAST_SEARCH_ACTIVE)
    {
        /* current measurement completes and stops as normal */
        l1_sc_wcdma_data_ptr->rr_params.fastSearchState = FAST_SEARCH_ARMED;   /* measure again on next tick_wcdma if we remain in idle */
    }
}

/*===========================================================================

FUNCTION l1_sc_g2l_idle_abort

DESCRIPTION
          g2l abort, placeholder pending new functionality, once ML1 release abort support.

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/

void l1_sc_g2l_abort(boolean g2l_idle, gas_id_t gas_id )
{
   l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   l1_sc_globals_T     *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

   l1_sc_g2l_fast_search_abort(gas_id);

   if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_SCHEDULE_LTE)
   {
     if (l1_sc_g2l_empty_qta_gap[gas_id])
     {
       l1_sc_g2l_empty_qta_gap[gas_id] = FALSE;
       l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOL_SRCH_STATE_ABORT_CNF_WAIT;
       L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ(gas_id);
     }
     else
     {
       /* In this place, we only handle quick cleanup for abort if we haven't sent TIMED_SRCH_MEAS_REQ but have sent G2X STARTUP to GFW. 
        ** For those ABORT after sending TIMED_SRCH_MEAS_REQ, we will handle cleanup in the l1_sc_lte_process_timed_search_results function */
       l1_sc_g2l_schedule_abort(gas_id );
       l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;
     }
   }
   else if (    (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE) 
             && (l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE)) 
   {
     l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;
     
     if (l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MEAS_RESTART)
     {
       /* In this place, we only handle quick cleanup for abort if we haven't sent TIMED_SRCH_MEAS_REQ but have sent G2X STARTUP to GFW. 
       ** For those ABORT after sending TIMED_SRCH_MEAS_REQ, we will handle cleanup in the l1_sc_lte_process_timed_search_results function */
       l1_sc_g2l_schedule_abort(gas_id );
     }
     else if (l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap == G2L_MDSP_CONTINUE)
     {
       L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ( gas_id );
       MSG_GERAN_HIGH_0_G("G2L TimedSrch: Abort req is sent in GTOW_SRCH_STATE_ACTIVE");
     }
   }
 
   if (!g2l_idle)
   {
     /* Reset abort flag for connected mode. Idle mode will be cleared in SM module */
     l1_sc_wcdma_data_ptr->wcdma_srch.abort = FALSE;
   }
}

/*===========================================================================

FUNCTION l1_sc_g2l_schedule_abort

DESCRIPTION
          
          Only called in GTOW_SRCH_STATE_SCHEDULE_LTE state by sm from l1_sci_tick_wcdma()
          G2L has been scheduled but no G2L timed search has been issued to ML1 so we can cleanup G2L (> Thor).
          g2l_irat_x2l_meas_enter() has not yet been called either.
          
          This runs in isr "frame tick" context after l1_g2l_trigger_mdsp() has already run for this frame.
          

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_g2l_schedule_abort(gas_id_t gas_id )
{
   l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    MSG_GERAN_HIGH_2_G("G2L: Schedule g2x_cleanup for abort srch state %d mdsp state %d", 
                        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state, 
                        l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap);
     
   /* schedule state specific actions for G2L idle abort  */
     
   /*
   ** move gap state directly to the "finish early" to initiate G2X cleanup.
   */
   l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_FINISH_EARLY;
    
   /* issue cleanup start of next frame */
   l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[G2L_CLEANUP_CMD_IDX] = G2X_FAST_START_START_OF_FRAME_QS;  

} /* l1_sc_g2l_schedule_abort */

/*===========================================================================

FUNCTION g2l_irat_x2l_meas_enter

DESCRIPTION

 sets up gap parameters for common rf api and x2l meas_enter()

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  none

===========================================================================*/

void g2l_irat_x2l_meas_enter(lte_earfcn_t lte_earfcn, gas_id_t gas_id)
{
  rfm_meas_common_param_type* g2x_common_ptr;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  g2x_common_ptr = gl1_get_irat_rf_structure_ptr(gas_id);
  g2l_meas_enter_params[gas_id].common = g2x_common_ptr;              /* pass the common pointer */

  if(l1_sc_x2g_ta_irat_gap(gas_id))
  {
    if(gl1_hw_qta_gap_active(gas_id))
  {
      /* in QTA case, RF source parameters are populated by the tech that holds
         RF before the QTA.  Aka if it's a <tech y>-to-G QTA and G is performing
         G2L, the source params will be from the <tech y>.  Populating these
         parameters is handled in another function, so we don't have to populate
         anything here. */

     MSG_GERAN_HIGH_1_G("G2L RF Enter: QTA gap, src tech %d", g2x_common_ptr->header.source_tech);

      /* Populate script type and scenario type again just in case */
      g2x_common_ptr->script_type    = RFM_MEAS_PROGRAM_STARTUP;
     g2x_common_ptr->scenario_type  = RFM_MEAS_SCENARIO_IRAT;
  }
  else
  {
      /* If not a QTA and GSM is in multisim idle, nobody is currently utilizing
         RF on this device so no source parameters need to be populated
         (aka Null2X gap).*/

      MSG_GERAN_HIGH_0_G("G2L RF Enter: NULL2X gap");

      rfm_meas_common_init_params(g2x_common_ptr);
      g2x_common_ptr->header.source_sub_id = geran_map_gas_id_to_nas_id(gas_id);
      /* (gsm_params.sub_id might not be needed now since header has same info) */
      g2x_common_ptr->source_param.gsm_params.sub_id = geran_map_gas_id_to_nas_id(gas_id);
      g2x_common_ptr->script_type = RFM_MEAS_PROGRAM_NULL2X;
      g2x_common_ptr->scenario_type = RFM_MEAS_SCENARIO_IRAT;
    }
  }
  else
  {
    /* Non multisim idle:
       This is a gap where GSM is currently utilizing RF on the allocated
       device, so source parameters need to be populated by GSM. */

     MSG_GERAN_HIGH_0_G("G2L RF Enter: G2X gap");

    /* populates parameters */
     g2x_set_common_rf_enter(g2x_common_ptr, gas_id);

    /* To be used at time of issuing mdsp_g2x_cleanup */
    l1_sc_wcdma_data_ptr->rfTuneback_required = TRUE;
  }
  /*
  ** LTE specific target params
  */
  g2l_meas_enter_params[gas_id].target.earfcn            = lte_earfcn;
  g2l_meas_enter_params[gas_id].target.rxlm_buf_idx_ant0 = gl1_hw_get_lte_rxlm_buf0_idx(gas_id);        /* is now lm_handle_type --> uint32 */
  g2l_meas_enter_params[gas_id].target.rxlm_buf_idx_ant1 = gl1_hw_get_lte_rxlm_buf1_idx(gas_id);

  /*If in QTA Gap, G2L Target Dev Params are Initialized in QTA Start CB*/
  if(!gl1_hw_qta_gap_active(gas_id))
  {
     g2l_get_lte_device( &g2l_meas_enter_params[gas_id].target, lte_earfcn , gas_id);
  }
  
  g2l_meas_enter_params[gas_id].flags.is_gap_aborted     = FALSE;

  lte_ml1_irat_x2l_meas_enter(geran_map_gas_id_to_nas_id(gas_id), &g2l_meas_enter_params[gas_id]);

  gl1_hw_set_g2x_rf_enter_called_last(TRUE, gas_id);

} /* g2l_irat_x2l_meas_enter */

/*===========================================================================

FUNCTION g2l_irat_x2l_meas_exit

DESCRIPTION

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  none

===========================================================================*/

void g2l_irat_x2l_meas_exit(boolean aborted, gas_id_t gas_id)
{

    g2l_meas_enter_params[gas_id].flags.is_gap_aborted = aborted;       /* pass back the result that ML1 has already given us */

    lte_ml1_irat_x2l_meas_exit(geran_map_gas_id_to_nas_id(gas_id), &g2l_meas_enter_params[gas_id]);
}

/*===========================================================================

FUNCTION      g2l_get_lte_device

DESCRIPTION
              Code section taken from existing gl1_hw_g2l_rf_meas_enter()
              and revised for new Rf API to get LTE device ids

DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  none

===========================================================================*/
static void g2l_get_lte_device( lte_cphy_irat_meas_target_rf_param_s *ptr_trgt , lte_earfcn_t lte_earfcn, gas_id_t gas_id)
{
   sys_sband_lte_e_type  lte_band;
   sys_band_class_e_type trm_band = SYS_BAND_CLASS_NONE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  trm_set_irat_band_device_info target_device;

#else
   /*this is a union*/
   trm_get_info_data     grm_get_device_input;
#endif


   /* Get LTE band from earfcn */
   if( lte_ml1_common_band_get_band_from_dl_earfcn(lte_earfcn, &lte_band))
   {
     trm_band = lte_ml1_convert_lte_band_to_sys_band(lte_band);
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

   target_device        = grm_get_irat_rf_dev_from_grm_info( trm_band, l1_tskisr_blk->client_id, gas_id );

   ptr_trgt->device     = target_device.primary_device;
   ptr_trgt->div_device = target_device.diversity_device;

#else

   grm_get_device_input.band_mapping_info.resource  = TRM_RX_BEST;
   grm_get_device_input.band_mapping_info.band[0]   = trm_band;
   grm_get_device_input.band_mapping_info.num_bands = 1;

   /* get device mapping for primary LTE and secondary LTE */
   ptr_trgt->device     =  grm_get_device_mapping( GRM_LTE, &grm_get_device_input, gas_id );

   /*get device mapping for lte secondary/diversity resource*/
   grm_get_device_input.band_mapping_info.resource = TRM_RX_DIVERSITY;
   ptr_trgt->div_device =  grm_get_device_mapping( GRM_LTE_SECONDARY, &grm_get_device_input, gas_id );

#endif

} /* g2l_get_lte_device */
/*===========================================================================

FUNCTION g2l_request_or_reserve_lte_meas

DESCRIPTION

Schedule using L1 Frame Manager L ID search activity then call DRX Manager

DEPENDENCIES
  none.

RETURN VALUE
  TRUE if L ID can go be scheduled now

SIDE EFFECTS
  None
===========================================================================*/

static boolean g2l_request_or_reserve_lte_meas(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32 frames_required;
  uint32 current_FN = gl1_get_FN( gas_id );

  /*
  ** normal search window ~15 frames
  */
  frames_required = MAX_LTE_SEARCH_FRAMES;

  /* If possible attempt to do it ASAP */
  if( FM_STATE_USING_FRAMES == l1_fm_get_fm_table_state(gas_id,FM_PRI_CCCH))
  {
    MSG_GERAN_HIGH_1_G("LTE_IDSEARCH Paging still ongoing at FN: %d", current_FN);
    return FALSE;
  }

  if(!l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent)
  {
    l1_sc_wcdma_data_ptr->id_search_fn = ADD_FN(current_FN, LTE_IDSEARCH_ADVANCE_NOTICE);

    if(!gl1_msg_get_multi_sim_mode())
    {
      /* Give DRX manager the reservation FN */
      l1_sc_wcdma_data_ptr->drx_ref = gl1_drx_require_tick( l1_sc_wcdma_data_ptr->id_search_fn, gas_id );
    }

    /* Reserve ID search with L1 FM */
    l1_fm_set_required_frame_number(FM_PRI_LTE_IDSEARCH, l1_sc_wcdma_data_ptr->id_search_fn , frames_required, gas_id);
    l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = TRUE;

    MSG_GERAN_HIGH_1_G("LTE_IDSEARCH reserved at FN: %d", l1_sc_wcdma_data_ptr->id_search_fn);
    return FALSE;
  }
  else if(IS_FRAME_NUM_LATER_OR_EQUAL(current_FN, l1_sc_wcdma_data_ptr->id_search_fn))
  {
      l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = FALSE;
      l1_sc_wcdma_data_ptr->drx_ref = 0;
      return g2l_can_lte_meas_go(frames_required, gas_id);
  }

  MSG_GERAN_HIGH_2_G("LTE_IDSEARCH reserved at FN: %d current FN:%d" , l1_sc_wcdma_data_ptr->id_search_fn, current_FN);
  return FALSE;

}

/*===========================================================================

FUNCTION g2l_can_lte_meas_go

DESCRIPTION

Check using L1 Frame manager if L ID search activity can go ahead

DEPENDENCIES
  none.

RETURN VALUE
  TRUE if L ID can go be started now

SIDE EFFECTS
  None
===========================================================================*/

static boolean g2l_can_lte_meas_go(uint32 frames_required, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif/*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/

  if ((l1_sc_wcdma_data_ptr->lte_irat_active) && (l1_sc_frames_available(FM_PRI_LTE_IDSEARCH, frames_required, l1_sci_g2l_idle_aborted, 0, gas_id)))
  {
    MSG_GERAN_HIGH_0_G("LTE_IDSEARCH can go ahead");

    if((l1_sc_wcdma_data_ptr->rr_params.fastSearchState == FAST_SEARCH_ARMED) ||
       (l1_sc_wcdma_data_ptr->rr_params.fastSearchState & FAST_SEARCH_ACTIVE) )
      {
        /*
        ** frames available so set active
        */
        if (l1_sc_wcdma_data_ptr->rr_params.fastSearchState == FAST_SEARCH_ARMED)
        {
        l1_sc_wcdma_data_ptr->rr_params.fastSearchState = FAST_SEARCH_ACTIVE;
        }
      }

    return TRUE;
  }
  else
  {
    l1_fm_clear_required_frames( FM_PRI_LTE_IDSEARCH, gas_id );
    MSG_GERAN_HIGH_1_G("LTE_IDSEARCH TRM denied l1_sc_wcdma_data_ptr->lte_irat_active:%d", l1_sc_wcdma_data_ptr->lte_irat_active);
    return FALSE;
  }

}

/*===========================================================================

FUNCTION g2l_irat_populate_rf_param

DESCRIPTION


DEPENDENCIES
  none.

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/
void g2l_irat_populate_rf_param ( lte_cphy_irat_meas_rf_param_s *rf_param, gas_id_t gas_id )
{
  memscpy(rf_param, sizeof(lte_cphy_irat_meas_rf_param_s),
          &g2l_meas_enter_params[gas_id], sizeof(lte_cphy_irat_meas_rf_param_s));
}

/*===========================================================================

FUNCTION gl1_hw_set_g2l_devinfo_for_qta

DESCRIPTION

On Receiving QTA Start for G2L , This Function Caches Device Details for G2L Enter


DEPENDENCIES
  none.

RETURN VALUE
  void

SIDE EFFECTS
  None
===========================================================================*/

boolean gl1_hw_set_g2l_devinfo_for_qta(const geran_gl1_qta_msg_type* qta_data_p, gas_id_t gas_id)
{
  /* Validate LTE target diversity chain before populating RF, primary chain has been validated before this function
  ** This is for L->G2L QTA, LTE RF can't build tune away script in case DRX chain is not valid */
  if (qta_data_p->dev_allocated[1] == RFM_INVALID_DEVICE)
  {
    MSG_GERAN_HIGH_0_G("gl1_hw_set_g2l_devinfo_for_qta: Invalid DrxDev, skip X2L in QTA");
    return FALSE;
  }

  g2l_meas_enter_params[gas_id].target.device     = qta_data_p->dev_allocated[0];
  g2l_meas_enter_params[gas_id].target.div_device = qta_data_p->dev_allocated[1];
  
  MSG_GERAN_MED_2_G("gl1_hw_set_g2l_devinfo_for_qta: PrxDev:%d DrxDev:%d",
                     g2l_meas_enter_params[gas_id].target.device,
                     g2l_meas_enter_params[gas_id].target.div_device);
  return TRUE;
}

#endif /*FEATURE_GSM_TO_LTE*/
/* EOF */
