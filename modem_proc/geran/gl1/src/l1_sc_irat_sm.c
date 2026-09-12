
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        INTERRAT (WCDMA) SURROUND CELL

GENERAL DESCRIPTION
   This module contains routines specific to performing LTE/TDS/WCDMA interRAT
   measurements while in GSM.

EXTERNALIZED FUNCTIONS
   l1_sc_irat_check_active
   l1_sc_g2x_pending_shutdown
   l1_sc_check_irat_active
   l1_sc_wcdma_init
   l1_sc_wcdma_enter_idle
   l1_sc_wcdma_leave_idle
   l1_sc_wcdma_enter_xfer
   l1_sc_wcdma_enter_dedi
   l1_sc_wcdma_ded_pending
   l1_sc_wcdma_ded_ready
   l1_sc_wcdma_receive
   l1_sc_wcdma_srch_active
   l1_sci_tick_wcdma
   l1_sci_abort_wcdma_tick
   l1_sci_wcdma_abort_reconfirm
   l1_wcdma_abort_scheduled_reconfirm
   l1_sci_wcdma_paging_block_tick
   l1_sc_wcdma_ded_tick
   l1_sc_wcdma_sdcch_tick
   l1_sc_wcdma_abort_ded_search
   l1_sc_wcdma_ded_tick_abort_stuck_srch
   l1_sc_wcdma_set_mode


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_irat_sm.c#13 $
$DateTime: 2020/06/07 04:13:53 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
05/06/20   sc        CR2703314 TDS Removal on Agatti- Moving G2T/T2G code under TDS feature
05/03/20   sal       CR2635669 Don't queue MCPM_G2W_MEAS_STOP request to DPC if G2X init confirmation still pending to receive.
20/02/20   sal       CR2624970 Ensure gas_id is validated before the use
19/02/20   sal       CR2624019 Don't increment g2x_abort_count counter in case of other GSub is doing BPLMN search.
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
06/08/19   sal       CR2488908 If Cleanup cmd is not immediate to GFW then wait one frame to let cleanup complete. 
06/01/20   sc      CR2561139 Update MCPM about Start/Stop for every G2X IRAT search gap in Idle mode.
06/01/20   sc      CR2584400 Don't send G2X DEINIT_REQ during QTA gap active.
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
17/09/19   sc        CR2439599 FR 56377 Elevator Sensor Ind for G2L
15/06/18   ng        CR2261488 Remove 'l1_rcvd_non_drx_ind' related handling, it is no more needed
06/04/18   akm       CR2216412 Moving G2X de-init in QTA cleanup
28/02/18   km        CR2189789 Handling g2l abort from g2l_trigger_mdsp
11/04/17   rv        CR2031827 Excessive F3 log reduction for Oppo
31/08/17   ng        CR2101566 Get correct freq index while applying MCVS boost 1 frame in advance before intiating G2L search 
05/02/18   akm       CR2183853 Set receive_in_progress false while calling search state to NULL in GTOL_SRCH_STATE_ABORT_CNF_WAIT state
17/11/17   ng        CR2145414 De-Init X RAT on receiving Early camping request after Ded->Idle transition 
01/02/18   km        CR2157534 Debug crash to root cause G2L INIT stuck
13/12/17   ksb       CR2152375 Correcting the conditional check for scheduling second G2L before the reselection  timer expiry
23/01/17   km        CR2171441 Debug crash to capture active_irat going UNDEFINED in CLEANUP_WAIT state
18/01/18   km        CR2172348 Addition of GTOL_SRCH_STATE_ABORT_CNF_WAIT in IRAT sm
09/01/18   km        CR2166879 G2L abort handling in QTA/non-QTA scenarios
12/07/17   ng        CR2074605 Do not leave idle mode until G2X init confirm is received
05/10/16   ng        CR1093500 Irat Init/Deinit Code Change
23/09/16   zf        CR1069807: Allow to search blacklisted cells if fast search has not completed.
27/12/17   km        CR2164223 Schedule G2L from post-tick whether or not fast search is inactive
19/12/17   km        CR1021898 Allow G2W reconfirm to schedule before going to sleep in "PCH Decoded" async interrupt
06/12/17   km        CR973889 Idle Timeline Opt - Sleep between PCH and IRAT
05/12/17   km        CR987605 G2X immediate cleanup
13/07/17   ng        CR2075022 Reset G2W ID search scheduling cycle only for a particular freq when reconf attempt on cell belonging to that freq fails
15/11/17   ng        CR1021532 Make G2L Null2X (and changes for all G2X as Null2X)
23/01/17   cc        CR1114369 Reset freq age after leaving xfre mode
29/10/17   km        CR2133917 GL1 support for L+L; FR37926 non-DDS sub IRAT optimization
25/10/16   dg        CR1081835 Apply G2L BIMC boost even if G2X tune away is happening
13/09/16   dg        CR1062850 G2X IRAT search should happen on all the indexes irrespective of fastsearchindex
29/09/16   dg        CR1071675 Don't check for sc tick  while sending MVCS request to MCPM for G2L search
19/09/16   dg        CR1066652 Make MCVS request of 384 mhz 1 frame in advance before initiating G2L search in dedicated mode
25/08/16   dg        CR1056221 Send MCVS req of 384 MHZ while initiating any G2X IRAT search in dedicated mode
04/08/16   dg        CR1049086 Do MVCS boost at the time of doing G2L search in PTM
29/07/16   dg        CR1047372 Allow loop for search all wcell feature to run for 1 more duration so as a W cell can be reported to higher layer
15/07/16   dg        CR1041727 Do BIMC boost at the time of starting G2L search if not done 1 frame before
01/07/16   dg        CR1033789 Send MCVS request for G2L search one frame in advance in PTM
17/06/16   ng        CR1030473 Send MCVS request of 384 MHZ at the time of initiating G2X search when GL1 is in Transfer mode (modify fix of CR 1021833 & 1025293)
10/06/16   ng        CR1025293 Extend fix of CR 1021833: Send MCVS request of 384 MHZ at the time of initiating G2X search with HMSC ON
31/05/16   dg        CR1021833 Send MCVS request of 384 MHZ at the time of initiating G2L search with HMSC ON
09/05/16   dg        CR1012577 Set g2x init flag to TRUE on receiving a new irat list if target tech is not initialized
30/11/15   cja/hd    CR944880 Check if G2X MCPM stop request queued up in DPC task after start request
12/05/16   dp        CR1007422 Implement wakeup cancellation of G2X IRAT (during sleep error recovery)
03/05/16   dg        CR1010494 Check for Forced Index while boosting BIMC clock for G2L CM measurements in packet transfer mode
27/04/16   zf        CR1009191: Dont start NULL2X IRAT if firmware loaded 
07/04/16   dg        CR1000307 Rework CR983630.Add missing wcdma irat mcpm active check
30/03/16   dg        CR990072 Call MCPM API for BIMC boost before starting G2L search in packet transfer mode with AEQ ON
21/03/2016 db        CR992014 Back out CR986247 
14/03/2016 ng        CR986247 Handling G2W Frame manager abort in schedule state also for G2W reconf search
02/03/16   dg        CR983630 Back-out CR972783
08/02/16   ng        CR972783 Schedule Ded mode G2W search when Mcpm clock init is also over
30/11/15   cja/hd    CR944880 Check if G2X MCPM stop request queued up in DPC task after start request
25/11/15   cja       CR942064 Add mcvs speed bump for G2L and G2T in dedicated mode 
23/11/15   cgc       CR944459 Fix regression due to CR937488, also requires change in GFW 
20/11/15   dg        CR940444 Set receive in progress flag while moving to wait state for G2W reconfirm search
16/11/15   dg        CR936934 Abort NCELL activity when received MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ for G2X forced search
16/11/15   cgc       CR937488 Add l1_sc_g2l_schedule_abort() idle abort functionality when in scheduled state
10/11/15   zf        CR934630 Abort pending WCDMA search when entering transfer
30/10/15   br        CR929138 GL1 to book TRM for g2t reconfirm search after gbta gap.
27/10/15   nm        CR922518 Decouple the WReconf search from Page when PCHXFER is enabled
14/10/15   dg        CR919577 Don't measure blacklisted EARFCN in Packet Transfer Mode
20/10/15   snjv      CR927604 Update duration for DR Band registration in G2W Ded search
14/10/15   cc        CR917262 Delay release IRAT band to search done for G2W dedicated search
17/09/15   dg        CR907489 Try to dynamically request frames for G2L fast search in idle mode
05/10/15   dg        CR916160 Call g2l shutdown on receiving init conf from ML1 instead of calling g2x pending shutdown
01/10/15   cgc       CR917158 Add num_freqs=0 check for early exit from l1_sc_wcdma_ded_ready()
29/09/15   cgc       CR915396 Add missing l1_sc_g2l_fast_search_expiry() check to l1_sc_wcdma_ded_ready()
11/09/15   hd        CR905576 Check for receive_in_progress flag is set before G2W reconfirm search
09/09/15   dv        CR905027 Abandon scheduled IRAT search if update list occurs
02/09/15   cja       CR892470 G2X to cope with overlapping init/deinit/init
01/09/15   cgc       CR897441 Add missing TRM band registration to l1_sc_wcdma_ded_ready() when g2l_inhibit_next_update is set.
26/08/15   hd        CR896113: Check for receive_in_progress before setting it for reconfirm search
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
20/08/15   dg        CR892911 Remove API l1_g2x_terminate
20/08/15   cgc       CR893193 back to back cell updates, wait for IRAT init_cnf/deinit_cnf's before next is actioned.
17/08/15   dg        CR891408 Clear start wcdma receive flag on aborting/completion of G2X dedicated mode search
17/08/15   cja       CR890396 In G2T dedicated abort do cleanup before setting state to NULL
15/08/15   zf        CR890992: Don't change srch state to wait state when abort G2W reconf
14/08/14   ws        CR889273 Fix Static declarations of variables which are not Dual data spaced
06/08/15   cgc       CR886693 Corrections to CR882971 ensure grm_check_ptm_irat_sc_activity_ok() only active once.
31/07/15   cja       CR881129 Fix deadlock due gl1_drx_require_next_tick inside irat mutex lock
31/07/15   cah/cc    CR882971 Gl1 Crash in SDCCH due to invalid diversity dev_id
27/07/15   cja       CR880009 Do not abort schedule TDS from dedicated abort
27/07/15   zf        CR880078: Don't allow ID and reconf activity in parallel
23/07/15   aga       CR878462 Avoid seting wcdma_srch_in_progress flag for TD-reconf during the page bursts
21/07/15   cgc       CR876899 fix CR866735 regression, make G2L sm wait for cleanup completed.
15/07/15   cja       CR871031 Remove forcing gas id 1 for T2G/G2T (required for W+W).
15/07/15   aga/zf    CR865337: NULL2T IRAT support.
26/06/15   dg        CR862078 Ensure Gl1 doesn't go to sleep when it is going to initialize G2X IRAT
29/04/15   dg        CR829595 In dedicated mode,don't start G2X IRAT measurements on a frame on which TS adjustment is happening
10/07/15   cja       CR868963 Correct abort of id in tbf search when leaving transfer
08/07/15   zf        CR868329: When start W reconf, set rcv_in_progress flag
09/07/15   cgc       CR866775 G2L SDCCH remove else clause in l1_sc_wcdma_ded_ready wcdma_srch_in_progress = FALSE.
01/07/15   cgc       CR853993 fix reprune totals error, reset num_detected_cells
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
25/06/15   zf        CR861092: Do not schedule NULL2X IRAT in transfer mode
09/06/15   cja       CR844096 RF meas gsm parameters to use xtoy param type
09/06/15   cgc       CR850131 add else clause g2l_set_cm_inactive() to l1_sc_wcdma_leave_xfer()
08/06/15   cgc       CR849570 l1_g2l_connected_status() renamed l1_g2l_connected_restart()
06/06/15   zf        CR849759: Fix decrement of IRAT cleanup wait count
04/06/15   cja       CR848107 G2L to use cleanup state, so extra frame for GFW cleannup
03/06/15   cws       CR846011 Clear all irat fm activity when srch state goes to NULL
30/05/15   zf        CR844855: Only clear receive_in_progress flag in IRAT if srch is in progress
04/06/15   ab        CR817146 : DSDS: GBTA - Apply Generic Mutex Level Protection for GBTA concurrency
26/03/15   cgc       CR814333 Fix linker errors for non FEATURE_GSM_TO_LTE build
14/04/15   hd        CR821910: Call G2T and G2W init once MCPM is configured.
20/05/15   aga       CR840984 NULL2X IRAT changes for G2W IRAT
19/05/15   cja       CR839842 For G2T do not changes search type during search.
11/05/15   cja       CR827065 Handle G2W initialisation requested when G2W shutdown in progress
11/05/15   cja       CR835701 Correct abort for G2W reconfirm active
30/03/15   br        CR808419 GL1 to make sure SCE is in active state before starting new IRAT activity
23/03/15   jj        CR 811970 Mutex dead lock l1_sci_tick_wcdma
                        L1_send_LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ caused by merge  of CR  765859
06/05/15   aga       CR833610 dual data space gl1_wcdma_rxlm_buf_idx and gl1_hw_g2w_rf_enter_called_last
28/04/15   jj        CR829554 mutex dead lock in ded_srch_reconf_cb
05/05/15   dp        CR830787 ensure srch handlers complete before SCE cleans up
03/04/15   zf        CR817252: Reset IRAT GBTA flag after IRAT completed.
24/04/15   cja       CR800657 QTA gap for G2W IRAT
23/04/15   cjl       CR827562 Correct G2W ID Search frames available reporting
21/04/15   cja       CR826384 For reconf do not use idle monitors in progress flag
21/04/15   cgc       CR826504 Reset ForcedMeas on entering dedicated mode.
21/04/15   cja       CR826376 When shutdown G2W only call wcdma inact once.
16/04/15   cja       CR823510 When abort in SCHEDULE_TDS state do not call rf meas exit
15/04/15   cja       CR822684 Changes for NULL2G RF API
03/02/15   cjl       CR790433 Create separate TRM activity for WCDMA reconfirm when needed
14/04/15   aga       CR799530 Reserve W/TD/L IRAT ID search with TRM in advance
14/04/15   cja       CR822143 Check srch type in addition to srch state for detecting IRAT stuck.
09/04/15  dp/cgc     CR819956 Thor 2.x RF API change, G2W use MSGR
30/03/15   cja       CR800650 Tidy-up SC IRAT state machine
                     Initial version - files moved from l1_sc_irat.
===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "l1_sc_irat.h"
#include "l1_sc.h"
#include "l1i.h"
#include "l1_utils.h"
#include "gl1_sys_params.h"
#include "geran_msgs.h"
#include "gl1_msg_wcdma_meas.h"
#include "l1_drx.h"
#include "l1_fm.h"
#include "mcpm_api.h"
#include "gpl1_gprs_utils.h"
#include "gtmrs_g.h"

#ifdef  FEATURE_GSM_TO_LTE
#include "l1_sc_lte_irat.h"
#include "lte_as.h"
#include "lte_l1_types.h"
#include "lte_cphy_irat_meas_msg.h"
#include "lte_ml1_irat_ext_api.h"
#endif

#include "msgr.h"
#include "msgr_geran.h"

#include "rr_l1.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "gpl1_grm_intf.h"

#include "l1_log.h"
#include "wl1x2wsrchapi.h"

#include "gl1_hw_clk_ctl_g.h"
#include "mcpm_api.h"
#include "gl1_hw_sleep_ctl.h"
#include "gl1_hwi.h"

#ifdef FEATURE_GSM_TDS
#include "tdsiratif.h"
#include "tds_l1_api.h"
#endif

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT*/

#include "gl1_hw_g.h"
#include "geran_dual_sim_g.h"
#include "gl1_msg_wtr_hop.h"

#ifdef FEATURE_SEGMENT_LOADING
#include "mcfg_seg_load.h"
/* from gtmrs.c */
extern interface_t *pi_gl1_wcdma;
#endif

#include "gl1_msgrif_init.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/


/************************** enums / structures ***************************************/


/************************** local data ***************************************/


l1_sc_wcdma_data_t l1_sc_wcdma_data[NUM_GERAN_DATA_SPACES] = {
                     INITIAL_VALUE_S( 0 ) };

extern dedicated_data_T l1_dedicated_data[];

static boolean l1_is_rat_change_active[NUM_GERAN_DATA_SPACES] = {
                     INITIAL_VALUE( FALSE ) };

static int16   gl1_sc_cleanup_wait_count = 0;


boolean g2l_bimc_boost_done[NUM_GERAN_DATA_SPACES] = {
                     INITIAL_VALUE( FALSE ) };

/*Flag indicates active scenario for g2x mcpm request*/
uint8 g2w_active_mcpm_scenario[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE( 0 ) };
uint8 g2l_active_mcpm_scenario[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE( 0 ) };
uint8 g2t_active_mcpm_scenario[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE( 0 ) };

/* MCPM Sceanrio based on RAT(W/L/T) and mode(ded/xfer/idle) */
static const uint8 gl1_mcpm_irat_scenario[TOTAL_NGHBR_RAT_TYPE][TOTAL_GL1_MODES]=
{
  {L1_DED_G2W, L1_PTM_G2W, L1_IDLE_G2W},
  {L1_DED_G2L, L1_PTM_G2L, L1_IDLE_G2L},
  {L1_DED_G2T, L1_PTM_G2T, L1_IDLE_G2T}
};

#define MCVS_Q6_CLK_KHZ_AEQ_ON       480000
#define TBF_MIN_DUR_FRAMES 9

boolean l1_sc_g2x_timeline_opt_enable[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };

uint8  check_irat_active[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
/*****************************************************************/
/* Array of values used to convert the idle ID tick between IDLE */
/* and TRANFER modes. So with a BS_PG_MFRM of 2 there will need  */
/* to be 4 ticks in transfer to increment the page_cycle_tick.   */
/* There are two ticks per 52 multiframe...                      */
/*          i.e. 1 xfer tick = 0.119secs                         */
/*****************************************************************/

static const uint16  xfer_ticks_per_idle_tick[MAX_PG_MFRMS-1] =
{
  4,  /*BS_PG_MFRM 2*/
  6,  /*BS_PG_MFRM 3*/
  8,  /*BS_PG_MFRM 4*/
  10, /*BS_PG_MFRM 5*/
  12, /*BS_PG_MFRM 6*/
  14, /*BS_PG_MFRM 7*/
  16, /*BS_PG_MFRM 8*/
  18  /*BS_PG_MFRM 9*/
};

/* Bit mapped Flag to check if INIT_REQ is yet to be issued to the other RAT */
static uint16      gl1_hw_g2x_send_init_req[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
static uint16      gl1_hw_g2x_send_deinit_req[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };

static void l1_sc_clear_filters_stored_meas(gas_id_t gas_id);

static void l1_sci_abort_wcdma_tick (uint16 freq_idx, gas_id_t gas_id );

/* G2L CM additions */
static void    g2l_configure_for_cm(gas_id_t gas_id);
static boolean l1_sc_irat_check_gbta_conflict(gas_id_t gas_id);

static void l1_sc_is_g2x_timeline_opt_enabled( gas_id_t gas_id );

/************************** external function definitions ***************************************/

/**
 *  CR346062 : Workaround to avoid T IRAT abort.
 *  Should be deleted after abort feature available.
 */
#ifdef FEATURE_GSM_TDS
boolean l1_sc_irat_performing_ded_meas(gas_id_t gas_id);
#endif


/************************** global data ***************************************/
#ifndef FEATURE_GSM_TO_WCDMA_MSGR
static boolean l1_sc_g2w_mutex_init = FALSE;
extern void wl1_x2w_srch_init_g2w_crit_sect(void);
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
/************************** function prototypes ***************************************/

static rfm_meas_common_param_type     gl1_irat_rf_common_structure[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION init_sc_wcdma_data

DESCRIPTION
             Initializes sc wcdma data values .

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
void init_sc_wcdma_data( gas_id_t gas_id )
{
  memset ( &l1_sc_wcdma_data[gas_id], 0 , sizeof (l1_sc_wcdma_data_t) );
  l1_sc_wcdma_data[gas_id].first_init = TRUE;

#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
  l1_sc_wcdma_data[gas_id].rr_listed_wcdma_cell_not_found      = FALSE;
  l1_sc_wcdma_data[gas_id].number_of_expanded_wcdma_cell_scan  = -1 ;
#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */

  l1_sc_wcdma_data[gas_id].requested_ded_id_srchs              = 0;
  l1_sc_wcdma_data[gas_id].ded_srch_aborted                    = FALSE;
  l1_sc_wcdma_data[gas_id].ded_time_out                        = 0;
  l1_sc_wcdma_data[gas_id].wcdma_tries                         = 0;
  l1_sc_wcdma_data[gas_id].wcdma_irat_active                   = FALSE;
  l1_sc_wcdma_data[gas_id].wcdma_irat_mcpm_active              = FALSE;
  l1_sc_wcdma_data[gas_id].wcdma_update_list_irat_active       = FALSE;
  l1_sc_wcdma_data[gas_id].scheduled_wcdma_reconf_fn           = NOT_A_FRAME_NUMBER;
#ifdef  FEATURE_GSM_TO_LTE
  l1_sc_wcdma_data[gas_id].lte_irat_active                     = FALSE;
  l1_sc_wcdma_data[gas_id].lte_irat_mcpm_active                = FALSE;
  l1_sc_wcdma_data[gas_id].lte_update_list_irat_active         = FALSE;
#endif /*FEATURE_GSM_TO_LTE*/

#ifdef FEATURE_GSM_TDS
  l1_sc_wcdma_data[gas_id].tds_update_list_irat_active         = FALSE;
  l1_sc_wcdma_data[gas_id].tds_irat_active                     = FALSE;
  l1_sc_wcdma_data[gas_id].tds_irat_mcpm_active                = FALSE;
  l1_sc_wcdma_data[gas_id].reconf_req_fn                       = 0;
  l1_sc_wcdma_data[gas_id].tds_state                           = TDS_NULL;
  l1_sc_wcdma_data[gas_id].toggle_id_reconf                    = 0xFFFF;
  l1_sc_wcdma_data[gas_id].l1_tds_issue_cleanup                = FALSE;
#endif /*FEATURE_GSM_TDS*/

  l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);
  l1_sc_wcdma_data[gas_id].l1_skip_g2x_pkt_txfr                = FALSE;

  l1_sc_wcdma_data[gas_id].id_search_fn                        = L1_GPRS_INVALID_FN;
  l1_sc_wcdma_data[gas_id].l1_irat_reserve_at_sent             = FALSE;
  l1_sc_wcdma_data[gas_id].drx_ref                             = 0;

  l1_sc_wcdma_data[gas_id].sc_wcdma_count                      = 0;
  l1_sc_wcdma_data[gas_id].sc_lte_count                        = 0;
  l1_sc_wcdma_data[gas_id].sc_tds_count                        = 0;

  l1_sc_wcdma_data[gas_id].searching_active                    = FALSE;
  l1_sc_wcdma_data[gas_id].first_init                          = TRUE;
  l1_sc_wcdma_data[gas_id].frame_count                         = 0;

  gl1_hw_g2x_send_init_req[gas_id]                             = 0;
  gl1_hw_g2x_send_deinit_req[gas_id]                           = 0;

}

/*===========================================================================

FUNCTION L1_SC_G2W_SHUTDOWN

DESCRIPTION
  This function is called to shutdown G2W when no W neighbours.


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_g2w_shutdown (gas_id_t gas_id)
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

    /* Wait for WFW to be suspended before shutting down WFW */
     if(g2w_is_wfw_active(gas_id))
     {
        MSG_GERAN_MED_0_G("G2W: Wait for WFW suspended/disabled before shutdown");
        l1_sc_globals_ptr->g2w_shutdown_pending = TRUE;
     }
     else
     {
        /* Shutdown g2w */
        gl1_hw_wcdma_inact(gas_id);
        l1_sc_globals_ptr->g2w_shutdown_pending = FALSE;

        l1_sc_set_wcdma_irat_active(FALSE,gas_id);
        gl1_hw_deallocate_wcdma_rxlm_buf(gas_id);

    #ifndef FEATURE_GSM_TO_WCDMA_MSGR
        l1_sc_mode_cnf_pending(FALSE, bWCDMA_RAT_INACTIVE, gas_id);
    #endif /* !FEATURE_GSM_TO_WCDMA_MSGR */

        MSG_GERAN_HIGH_0_G("G2W shut down ");
     }
}

/*===========================================================================

FUNCTION l1_sc_g2l_shutdown

DESCRIPTION
  This function is called to shutdown G2L when no L neighbours exist
  following cell update list or state change to gsm_only=1.


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_GSM_TO_LTE

void l1_sc_g2l_shutdown (gas_id_t gas_id)
{
    l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

    uint16 curr_init_mode;
    curr_init_mode  = g2l_get_Init_Sent_status(gas_id);

    if ((curr_init_mode & LTE_G2L_MASK )== LTE_G2L_READY)    /* init_req sent and cnf'd and no meas_cnf's outstanding */
    {
        l1_sc_globals_ptr->g2l_shutdown_pending  = FALSE;

        g2l_set_cm_inactive(gas_id);

       /* Shutdown g2l */
        L1_send_LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ(gas_id);

        gl1_hw_deallocate_lte_rxlm_buf(gas_id);

        MSG_GERAN_HIGH_0_G("G2L: l1_sc_g2l_shutdown initiated");
    }
    else
    if (curr_init_mode & LTE_G2L_INIT_SENT)    /* init_req sent and waitng for a cnf */
    {
        l1_sc_globals_ptr->g2l_shutdown_pending = TRUE;
        MSG_GERAN_HIGH_1_G("G2L: l1_sc_g2l_shutdown pending init_sent=0x%X", curr_init_mode);
    }
    else
    {
        /*
        ** not expected, no INIT_REQ has been sent
        */
        l1_sc_globals_ptr->g2l_shutdown_pending  = FALSE;

        MSG_GERAN_HIGH_1_G("G2L: NOT sending DEINIT_REQ init_sent=0x%X", curr_init_mode);
    }
}
#endif /* FEATURE_GSM_TO_LTE */

#ifdef FEATURE_GSM_TDS
/*===========================================================================

FUNCTION L1_SC_G2T_SHUTDOWN

DESCRIPTION
  This function is called to shutdown G2T when no TDS neighbours.


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sc_g2t_shutdown (gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_globals_ptr->g2t_shutdown_pending = FALSE;

   /* Shutdown g2t */
   L1_send_TDS_IRAT_STOP_CMD( gas_id);

   gl1_hw_deallocate_tds_rxlm_buf();
   l1_sc_wcdma_data_ptr->tds_irat_active = FALSE;

   MSG_GERAN_HIGH_0_G("G2T shut down ");
}
#endif /*FEATURE_GSM_TDS*/

/*===========================================================================

FUNCTION L1_SC_G2X_PENDING_SHUTDOWN

DESCRIPTION
  This function is called to shutdown G2X if shutdown is pending.


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_g2x_pending_shutdown(gas_id_t gas_id)
{
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
   /* If G2x shutdown pending, then do it now */

   gl1_irat_mutex_lock( gas_id);

   if(l1_sc_globals_ptr->g2w_shutdown_pending == TRUE)
   {
      l1_sc_g2w_shutdown(gas_id);
   }
#ifdef FEATURE_GSM_TO_LTE
   if(l1_sc_globals_ptr->g2l_shutdown_pending == TRUE)
   {
      l1_sc_g2l_shutdown(gas_id);
   }
#endif
#ifdef FEATURE_GSM_TDS
   if (l1_sc_globals_ptr->g2t_shutdown_pending == TRUE)
   {
      l1_sc_g2t_shutdown(gas_id);
   }
#endif

  gl1_irat_mutex_unlock( gas_id);

}


/*===========================================================================

FUNCTION L1_SC_IRAT_CHECK_ACTIVE

DESCRIPTION
  This function is called when MPH_WCDMA_CELL_UPDATE_LIST
  message from RR. It checks if any RATs have become active
  or inactive.
  update for G2L:
  Added cm_mode for starting LTE in connected mode for G2L measurements in PTM/DED.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_irat_check_active(byte     wcdma_num_of_cells,
                             uint8    lte_freq_count,
                             boolean  update_sc_list,
                             byte     tds_freq_count,
                              boolean cm_mode,
                             gas_id_t gas_id
                            )
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
   boolean             gsm_only             = gl1_hw_get_gsm_only_mode(gas_id);
   boolean             mcpm_start_reqd       = FALSE;
#ifdef  FEATURE_GSM_TO_LTE
   uint16              check_status         = 0;
#endif

   /* sanity check the multimode capability of the specified GAS ID */
   if (FALSE == geran_is_multi_mode_gas_id(gas_id))
   {
     if ( !gsm_only )
     {
        /* irat should never be active on a non-multimode GAS */
        MSG_GERAN_ERROR_2_G("l1_sc_irat_check_active: gas_id %d should not map to MM sub,wcdma_irat_active:%d",
                          gas_id, l1_sc_wcdma_data_ptr->wcdma_irat_active);
     }
   }

   /* debug code that should crash if W cells are added when W segment isn't loaded */
   #ifdef FEATURE_SEGMENT_LOADING
   /* if updating list with non-zero W cells */
   if(update_sc_list &&
      wcdma_num_of_cells > 0)
   {
     /* this interface pointer will be null if W segment isn't loaded */
     if(!pi_gl1_wcdma)
     {
#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       ERR_FATAL("Adding W cells when W segment isn't loaded", 0, 0, 0);
     }
   }
   #endif

   /* If list update, then store cell count */
   if (update_sc_list)
   {
      l1_sc_wcdma_data_ptr->sc_wcdma_count = wcdma_num_of_cells;
      l1_sc_wcdma_data_ptr->sc_lte_count = lte_freq_count;
      l1_sc_wcdma_data_ptr->sc_tds_count = tds_freq_count;
   }
   else
   {
      /* If GSM resume, set cell count to store value */
      if (wcdma_num_of_cells != 0)
      {
         wcdma_num_of_cells = l1_sc_wcdma_data_ptr->sc_wcdma_count;
      }
      if (lte_freq_count != 0)
      {
         lte_freq_count = l1_sc_wcdma_data_ptr->sc_lte_count;
      }
      if (tds_freq_count != 0)
      {
         tds_freq_count = l1_sc_wcdma_data_ptr->sc_tds_count;
      }
   }

   MSG_GERAN_HIGH_3_G("SC WCDMA IRAT change, num of w_freqs %d, active %d, %d",
                           wcdma_num_of_cells,
                           l1_sc_wcdma_data_ptr->wcdma_update_list_irat_active,
                           l1_sc_wcdma_data_ptr->wcdma_irat_active
                    );

   /* WCDMA IRAT becoming active */
   if (!(l1_sc_wcdma_data_ptr->wcdma_update_list_irat_active) && (wcdma_num_of_cells != 0) && !gsm_only)
   {
     gl1_irat_mutex_lock( gas_id);
#ifndef FEATURE_GSM_TO_WCDMA_MSGR
     /* Init the g2w mutex if it hasn't been done already.
     (g2w only happens on one sim right now,
     so we don't need one variable per sub) */
     if(!l1_sc_g2w_mutex_init)
     {
       MSG_GERAN_HIGH_0_G("Initializing g2w critical section (once) at Wcell-ADD");
       wl1_x2w_srch_init_g2w_crit_sect();
       l1_sc_g2w_mutex_init = TRUE;
     }
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

     l1_sc_wcdma_data_ptr->wcdma_update_list_irat_active = TRUE;
     if(l1_sc_globals_ptr->g2w_shutdown_pending == TRUE || (gl1_hw_g2x_send_deinit_req[gas_id] & (1 << RR_L1_IRAT_RAT_WCDMA)))
     {
       /* If shutdown pending no need to re-initialise G2W */
       l1_sc_globals_ptr->g2w_shutdown_pending = FALSE;
       gl1_hw_g2x_send_deinit_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_WCDMA);
     }
     else
     {
       if (l1_sc_wcdma_data_ptr->wcdma_irat_mcpm_active == FALSE)
       {
          /* Start mcpm and send init (if de-init not pending)  */
          l1_sc_wcdma_data_ptr->wcdma_irat_mcpm_active = TRUE;

          gl1_hw_g2x_send_init_req[gas_id] |= (1 << RR_L1_IRAT_RAT_WCDMA);

          /*Set next tick required to true and Increment active task count*/
          gl1_drx_require_next_tick(gas_id);
          l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_WCDMA, GL1_MODE_IDLE, gas_id);
       }
	    /*If G2W init is pending, send the below boolean so as G2W init can continue*/
	   else if ((l1_sc_wcdma_data_ptr->wcdma_irat_mcpm_active == TRUE) && (gl1_check_if_g2w_init_pend(gas_id)))
	   {
		 gl1_hw_g2x_send_init_req[gas_id] |= (1 << RR_L1_IRAT_RAT_WCDMA);
	   }

#if defined(FEATURE_GSM_TO_WCDMA_MSGR)
       /* this only needs to be done in dedicated or xfer mode, but won't hurt if set in idle */
       gl1_msg_cfg_wcdma_meas_ded(ded_srch_id_cb,
                                  ded_srch_abort_cb,
                                  ded_srch_reconf_cb,
                                  srch_handler_done_cb,
                                  gas_id);
#endif /* defined(FEATURE_GSM_TO_WCDMA_MSGR) */
     }
     gl1_irat_mutex_unlock( gas_id);
   }

   /* WCDMA IRAT becoming inactive */
   if( ( l1_sc_wcdma_data_ptr->wcdma_update_list_irat_active )&& (wcdma_num_of_cells == 0))
   {
      gl1_irat_mutex_lock( gas_id);
      l1_sc_wcdma_data_ptr->wcdma_update_list_irat_active = FALSE;

      /*Update mcpm about IRAT neighbor so as clock gets boosted before deinit request is sent
       *It will return TRUE in case of ded/xfer mode when no search is currently active */
      mcpm_start_reqd = l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_WCDMA, GL1_MODE_IDLE, gas_id);

      if (gl1_hw_g2x_send_init_req[gas_id] & (1 << RR_L1_IRAT_RAT_WCDMA))
      {
         /* If MCPM G2W startup in progress, clear flag so G2W INIT not sent */
         gl1_hw_g2x_send_init_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_WCDMA);
      }
      /*Only case when in ded/xfer/dtm mode and no search is ongoing*/
      else if(mcpm_start_reqd)
      {
        gl1_hw_g2x_send_deinit_req[gas_id] |= (1 << RR_L1_IRAT_RAT_WCDMA);
      }
      else
      {
      /* If srch in progress, wait until it completes. Otherwise shutdown g2w now */
        if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL) 
      {
         if (!gl1_hw_qta_gap_active(gas_id) || !gl1_hw_get_qta_gap_for_g2x_irat(gas_id))
         {
           l1_sc_g2w_shutdown(gas_id);
         }
         else
         {
           l1_sc_globals_ptr->g2w_shutdown_pending = TRUE;
         }
      }
      else
      {
         l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
         l1_sc_globals_ptr->g2w_shutdown_pending = TRUE;
      }
      }
      gl1_irat_mutex_unlock( gas_id);
   }

#ifdef  FEATURE_GSM_TO_LTE

   check_status = 0;

   /* LTE IRAT becoming active */
   if (!l1_sc_wcdma_data_ptr->lte_update_list_irat_active && (lte_freq_count != 0) && !gsm_only)
   {
     gl1_irat_mutex_lock( gas_id);
     l1_sc_wcdma_data_ptr->lte_update_list_irat_active = TRUE;
     if(l1_sc_globals_ptr->g2l_shutdown_pending == TRUE || (gl1_hw_g2x_send_deinit_req[gas_id] & (1 << RR_L1_IRAT_RAT_LTE)))
     {
        /* If shutdown pending no need to re-initialise G2L */
        l1_sc_globals_ptr->g2l_shutdown_pending = FALSE;
        gl1_hw_g2x_send_deinit_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_LTE);
        check_status = 1;
     }
     else
     {
       if (l1_sc_wcdma_data_ptr->lte_irat_mcpm_active == FALSE)
       {
          l1_sc_set_lte_irat_mcpm_active(TRUE, gas_id);
          gl1_hw_g2x_send_init_req[gas_id] |= (1 << RR_L1_IRAT_RAT_LTE);
   
          /*Set next tick required to true and Increment active task count*/
          gl1_drx_require_next_tick(gas_id);
          l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_LTE, GL1_MODE_IDLE, gas_id);
          check_status = 2;
       }
	   /*If G2L init is pending, send the below boolean so as G2L init can continue*/
	   else if ((l1_sc_wcdma_data_ptr->lte_irat_mcpm_active == TRUE) && (gl1_check_if_g2l_init_pend(gas_id)))
	   {
		 gl1_hw_g2x_send_init_req[gas_id] |= (1 << RR_L1_IRAT_RAT_LTE);
		 check_status = 3;
	   }

     }
     gl1_irat_mutex_unlock( gas_id);
  }

   MSG_GERAN_MED_3_G("SC LTE IRAT change, freq count %d, active %d status %d",
                                           lte_freq_count,
                                           l1_sc_wcdma_data_ptr->lte_irat_active,
                                           check_status );

   /* LTE IRAT becoming inactive */
   if (l1_sc_wcdma_data_ptr->lte_update_list_irat_active && (lte_freq_count == 0))
   {
     gl1_irat_mutex_lock( gas_id);
     l1_sc_wcdma_data_ptr->lte_update_list_irat_active = FALSE;
     
      /*Update mcpm about IRAT neighbor so as clock gets boosted before deinit request is sent
       *It will return TRUE in case of ded/xfer mode when no search is currently active */
      mcpm_start_reqd = l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_LTE, GL1_MODE_IDLE, gas_id);
     
      if (gl1_hw_g2x_send_init_req[gas_id] & (1 << RR_L1_IRAT_RAT_LTE))
      {
         /* If MCPM G2L startup in progress, clear flag so G2L INIT not sent */
         gl1_hw_g2x_send_init_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_LTE);
      }
      /*Only case when in ded/xfer/dtm mode and no search is ongoing*/
      else if(mcpm_start_reqd)
      {
        gl1_hw_g2x_send_deinit_req[gas_id] |= (1 << RR_L1_IRAT_RAT_LTE);
      }
      else
      {
      /* If srch in progress, wait until it completes. Otherwise shutdown g2l now */
        if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
      {
         if (!gl1_hw_qta_gap_active(gas_id) || !gl1_hw_get_qta_gap_for_g2x_irat(gas_id))
         {
           l1_sc_g2l_shutdown(gas_id);
         }
         else
         {
           l1_sc_globals_ptr->g2l_shutdown_pending = TRUE;
         }
      }
      else
      {
         uint16  curr_init_mode = g2l_get_Init_Sent_status(gas_id);
         l1_sc_wcdma_data_ptr->wcdma_srch.abort  = TRUE;
         l1_sc_globals_ptr->g2l_shutdown_pending = TRUE;
         MSG_GERAN_HIGH_2_G("G2L: Abort set in l1_sc_irat_check_active CM=0x%x Init=0x%X ",l1_sc_wcdma_data_ptr->lte_cm_irat_active, curr_init_mode);
      }
      }
      gl1_irat_mutex_unlock( gas_id);
    }
#else

    (void)cm_mode;                      /* prevent unused compile warning */

#endif /* FEATURE_GSM_TO_LTE */

#ifdef  FEATURE_GSM_TDS

   // In tdd, RR can send frequency info or cell info. In both the cases, activate TDD RAT.
   /* F3 only if non-zero */
   if(tds_freq_count || (l1_sc_wcdma_data_ptr->tds_update_list_irat_active) || (l1_sc_wcdma_data_ptr->tds_irat_active) )
   {
      // In tdd, RR can send frequency info or cell info. In both the cases, activate TDD RAT.
      MSG_GERAN_MED_3_G("SC TD IRAT change, num of frequencies | cells %d, tds_update_list_irat_active %d tds_irat_active %d",
           tds_freq_count, l1_sc_wcdma_data_ptr->tds_update_list_irat_active, l1_sc_wcdma_data_ptr->tds_irat_active);

   }
   /* TDS IRAT becoming active */
   if (!l1_sc_wcdma_data_ptr->tds_update_list_irat_active && (tds_freq_count != 0) && !gsm_only)
   {
     gl1_irat_mutex_lock( gas_id);
     l1_sc_wcdma_data_ptr->tds_update_list_irat_active = TRUE;
     if(l1_sc_globals_ptr->g2t_shutdown_pending == TRUE || (gl1_hw_g2x_send_deinit_req[gas_id] & (1 << RR_L1_IRAT_RAT_UTRAN_TDD)))
     {
        /* If shutdown pending no need to re-initialise G2T */
        l1_sc_globals_ptr->g2t_shutdown_pending = FALSE;
        gl1_hw_g2x_send_deinit_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_UTRAN_TDD);
     }
     else
     {
       if (l1_sc_wcdma_data_ptr->tds_irat_mcpm_active == FALSE)
       {
          l1_sc_wcdma_data_ptr->tds_irat_mcpm_active = TRUE;
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT) )
          MSG_GERAN_HIGH_0_G("GPLT DBG: Not calling gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_START)");
#else
          gl1_hw_g2x_send_init_req[gas_id] |= (1 << RR_L1_IRAT_RAT_UTRAN_TDD);

          /*Set next tick required to true and Increment active task count*/ 
          gl1_drx_require_next_tick(gas_id);
          l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_UTRAN_TDD, GL1_MODE_IDLE, gas_id);

#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
       }
	   /*If G2T init is pending, send the below boolean so as G2T init can continue*/
   	   else if((l1_sc_wcdma_data_ptr->tds_irat_mcpm_active == TRUE) && (gl1_check_if_g2t_init_pend(gas_id)))
	   {
	     gl1_hw_g2x_send_init_req[gas_id] |= (1 << RR_L1_IRAT_RAT_UTRAN_TDD);
	   }

     }
     gl1_irat_mutex_unlock( gas_id);
  }

   /* TDS IRAT becoming inactive */
   if (l1_sc_wcdma_data_ptr->tds_update_list_irat_active && (tds_freq_count == 0))
   {
      gl1_irat_mutex_lock( gas_id);
      /* Make tds_update_list_irat_active to handle another WCDMA_UPDATE_LIST_REQ*/
      l1_sc_wcdma_data_ptr->tds_update_list_irat_active = FALSE;

      /*Update mcpm about IRAT neighbor so as clock gets boosted before deinit request is sent
       *It will return TRUE in case of ded/xfer mode when no search is currently active */
      mcpm_start_reqd = l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_UTRAN_TDD, GL1_MODE_IDLE, gas_id);

      if (gl1_hw_g2x_send_init_req[gas_id] & (1 << RR_L1_IRAT_RAT_UTRAN_TDD))
      {
         /* If MCPM G2T startup in progress, clear flag so G2T INIT not sent */
         gl1_hw_g2x_send_init_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_UTRAN_TDD);
      }
      /*Only case when in ded/xfer/dtm mode and no search is ongoing*/
      else if(mcpm_start_reqd)
      {
        gl1_hw_g2x_send_deinit_req[gas_id] |= (1 << RR_L1_IRAT_RAT_UTRAN_TDD);
      }
      else
      {
      /* If srch in progress, wait until it completes. Otherwise shutdown g2w now */
        if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
      {
         if (!gl1_hw_qta_gap_active(gas_id) || !gl1_hw_get_qta_gap_for_g2x_irat(gas_id))
         {
           l1_sc_g2t_shutdown(gas_id);
         }
         else
         {
           l1_sc_globals_ptr->g2t_shutdown_pending = TRUE;
         }
      }
      else
      {
        l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
        l1_sc_globals_ptr->g2t_shutdown_pending = TRUE;
      }
      }
      gl1_irat_mutex_unlock( gas_id);
    }
#endif /* FEATURE_GSM_TDS */

} /* l1_sc_irat_check_active */

/*===========================================================================

FUNCTION L1_SC_WCDMA_ENTER_IDLE

DESCRIPTION
  This function is called at entry to IDLE state to let the WCDMA search
  code initialize/reset variables.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_enter_idle( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  uint16 idx;



  /* Called in Task context */
  MSG_GERAN_MED_3_G("G2X: SC enter_idle Fn=%d srch_state %d num_freqs=%d",
                     gl1_get_FN( gas_id ),
                     l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                     l1_sc_wcdma_data_ptr->rr_params.num_freqs );
  

  if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
  {
     if(g2w_is_wfw_active(gas_id) && l1_sc_globals_ptr->g2w_shutdown_pending)
     {
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
       gl1_msg_wcdmadrv_abort_search(gas_id);
#else /* FEATURE_GSM_TO_WCDMA_MSGR */
       wl1_x2w_srch_wcdmadrv_abort_search(gas_id);
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */
     }

     if (!gl1_hw_qta_gap_active(gas_id) || !gl1_hw_get_qta_gap_for_g2x_irat(gas_id))
     {
       /* If G2x shutdown pending, then do it now */
       l1_sc_g2x_pending_shutdown(gas_id);
     }
  }
  gl1_irat_mutex_lock( gas_id);
  for (idx=0; idx<MAX_WCDMA_FREQS; idx++)
  {

    /*************************************************************************/
    /* For cells that were no longer ID'd last time in IDLE make sure that   */
    /* dedicated or transfer mode hasn't changed this and decided that cells */
    /* now need to be ID'd.                                                  */
    /* Cells in the stop_cell_id_srch will only be honoured if they are in   */
    /* the stop_ded_cell_id_srch as well.                                    */
    /*************************************************************************/
    l1_sc_wcdma_data_ptr->stop_cell_id_srch[idx] = l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[idx];

    /*
     * If a particular UARFCN has a valid cache (i.e. we are entering non-DRX
     * mode with stored cells from transfer), then schedule the reconfirmation
     * immediately. Else, wait for the default 5 paging blocks
     */

    /* CR77995:
    Only if there are valid cached Xfer cells load them back into the stored_meas */
    if ( l1_sc_wcdma_data_ptr->stored_meas[idx].cached_xfer_cells )
    {
      /* @@TODO:
       * Make sure the cached_xfer_cells include LTE.
       */
      l1_sc_wcdma_data_ptr->stored_meas[idx].num_cells = l1_sc_wcdma_data_ptr->stored_meas[idx].cached_xfer_cells;
    }


   if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL )
   {
    if(l1_sc_wcdma_data_ptr->stored_meas[idx].num_cells)
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[idx] = 0;
    }
    else
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[idx] = 5;
    }
   }
    /********************************************************************/
    /* If this cell is marked as one that needs to be ID'd ASAP in IDLE */
    /* as it got aborted last time in IDLE - check that all the cells   */
    /* didn't get ID'd in transfer/dedicated. If they did then this cell*/
    /* is no longer in need of a fast-tracked ID search                 */
    /********************************************************************/
    if ( (l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[idx] == TRUE) &&
         (l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[idx] == l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].arfcn.UARFCN)
       )
    {
      MSG_GERAN_HIGH_2_G("Cancel Fast-tracked Cell ID for arfcn %d, RAT %d",
               l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].arfcn.UARFCN,
               l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].rat);

      l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[idx] = FALSE;
    }
    else if (l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[idx] == TRUE)
    {
    /**************************************************************/
    /* Make sure if an ID was aborted last time in IDLE then this */
    /* gets scheduled staright away upon returning to IDLE mode   */
    /**************************************************************/

      MSG_GERAN_HIGH_2_G("Enforce Fast-tracked Cell ID for arfcn %d, RAT %d",
               l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].arfcn.UARFCN,
               l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].rat);

      l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[idx] = FALSE;
      /*
      ** by resetting id_tick, is < page_cycle_tick comparison so is measured on next search interval.
      */
      l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[idx] = 0;
    }

  } /* End of for (idx=0; idx<MAX_WCDMA_FREQS; idx++) */

  /* don't clear rr list here as workaround for GPRS timing issue
     where this function is called after the list is sent from RR */

  l1_sc_wcdma_data_ptr->wcdma_srch.state = GtowStateIdle;
  l1_sc_wcdma_data_ptr->wcdma_srch.prev_state = GtowStateIdle;

  /* Don't reset abort flag to allow pending abort to be done when entering idle. */
  /* l1_sc_wcdma_data_ptr->wcdma_srch.abort = FALSE; */

  /*When returning to IDLE if this flag is set it will abort an IDLE mode ID - clear it now*/
  l1_sc_wcdma_data_ptr->ded_srch_aborted = FALSE;

  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
     /*
     ** if we re-enter idle from idle, W search may have already started
     */
     if(l1_sc_globals_ptr->wcdma_srch_in_progress == TRUE )
     {
        MSG_GERAN_MED_3_G("G2X Search Already running: srch_state %d, type %d, rat %d",
                           l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                           l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                           l1_sc_globals_ptr->active_irat);
     }
     else
     {
        MSG_GERAN_HIGH_3_G("Re-enter idle mode srch_state %d, type %d, rat %d",
                             l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                             l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                             l1_sc_globals_ptr->active_irat);

        l1_sc_set_gtow_srch_state_null(gas_id);
     }
  }
  else /* l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL */
  {
     /******************************************************/
     /* Reconfigure the correct call backs for IDLE mode.  */
     /* There is a chance that the transfer mode searches  */
     /* are still aborting, and so don't do this in such   */
     /* instances. This will happen in the abort callback  */
     /******************************************************/

     if( !l1_sc_wcdma_data_ptr->rr_params.gsm_only )
     {
        l1_sc_wcdma_data_ptr->wcdma_srch.send_report = TRUE;

        gl1_msg_cfg_wcdma_meas( id_report_cb,
                                id_abort_cb,
                                reconfirm_report_cb,
                                reconfirm_abort_cb,
                                #ifdef FEATURE_GSM_TO_WCDMA_MSGR
                                srch_handler_done_cb,
                                #endif
                                gas_id
                              );
     }
  }
  

   gl1_irat_mutex_unlock( gas_id);

} /* l1_sc_wcdma_enter_idle */

/*===========================================================================

FUNCTION L1_SC_WCDMA_LEAVE_IDLE

DESCRIPTION
  This function is called at exit from IDLE state to let WCDMA search code
  abort any outstanding searches.

DEPENDENCIES
  none

RETURN VALUE
  Boolean to tell is WCDMA has a receive_in_progress

SIDE EFFECTS
  None

===========================================================================*/

boolean l1_sc_wcdma_leave_idle( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  uint16 idx;

  for (idx=0; idx<MAX_WCDMA_FREQS; idx++)
  {
    /* Clear the cache for non-DRX mode */
    l1_sc_wcdma_data_ptr->stored_meas[idx].cached_xfer_cells = 0;

    /********************************************************************/
    /* Copy the IDLE ID stopped cell list into the dedi ID stopped list */
    /* so that if ID's were no longer required in IDLE and this is a    */
    /* transition for a brief period for a single block transfer they   */
    /* restored. This will take place in l1_sc_wcdma_enter_idle. If this*/
    /* is a full transition to transfer or dedicated then the           */
    /* stop_ded_cell_id_srch gets reset in l1_sc_wcdma_enter_dedi       */
    /* and l1_sc_wcdma_enter_xfer                                       */
    /********************************************************************/
    l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[idx] = l1_sc_wcdma_data_ptr->stop_cell_id_srch[idx];

  } /* for( idx = 0; idx < MAX_WCDMA_FREQS; idx++ ) */

  


  l1_sc_irat_reset_g2x_abort_count(gas_id);
  l1_sc_wcdma_data_ptr->wcdma_srch.state = GtowStateNull;
  l1_sc_wcdma_data_ptr->wcdma_srch.prev_state = GtowStateNull;

  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
     l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
  }

  MSG_GERAN_HIGH_3_G("G2X: SC leave_idle: srch_state %d abort=%d Fn=%d",
                     l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                     l1_sc_wcdma_data_ptr->wcdma_srch.abort,
                     gl1_get_FN( gas_id ));

  l1_sc_wcdma_data_ptr->wcdma_srch.send_report = FALSE;
  /*
   * As the transfer implementation only gets a new WCDMA
   * neighbour list from GRR when it actually changes in the P/SI
   * (cell reselection, PSI refresh, etc.) then we need to cache
   * the state of the original list on idle to transfer transitions.
   */

  l1_sc_wcdma_data_ptr->rr_report.num_cells = 0;

#ifdef FEATURE_GSM_TO_LTE
  /*Abort any overlapping G2L CM search*/

   if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE )
   {
     l1_g2l_cm_abort(gas_id);
   }
#endif

  return (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL);

} /* l1_sc_wcdma_leave_idle */

/*===========================================================================

FUNCTION l1_sc_check_priority_threshold

DESCRIPTION
  Check whether only higher priority freqs should be searched.
  use_priority_reselection_algo is only relevant when LTE freqs are present.

DEPENDENCIES
  none

RETURN VALUE
  Boolean TRUE if search is allowed

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_check_priority_threshold( uint16 freq_idx, gas_id_t gas_id)
{

  uint16  srch_criteria = 0;         /* for F3 debug */
  dBx16_T thrsh_pwr     = 0;

  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  boolean schedule_is_allowed = FALSE;

  if( l1_sc_wcdma_data_ptr->rr_params.gsm_only)
  {
      srch_criteria = 1;
  }
  else
  if( G2X_PRIORITY_FALSE != l1_sc_wcdma_data_ptr->wcdma_srch.use_priority_reselection_algo )
  {
      /*
      ** Check higher or lower than Scell.
      **
      ** 45.008 6.6.4  Applies to both Idle mode and PTM xfer mode
      ** if scell < threshold search low priority as well
      ** if scell > threshold search only high priority
      */
      thrsh_pwr= l1_sc_wcdma_data_ptr->wcdma_srch.priority_search_threshold;

      /*
      ** check for fast search blacklisted freq and skip this freq measurement
      */
      if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].freq_status == G2X_FS_BLACKLISTED )
      {
        /* If fast search has not completed, allow search on blacklisted cells. */
        if(l1_sc_wcdma_data_ptr->rr_params.fastSearchState == FAST_SEARCH_ARMED)
        {
          srch_criteria = 9;      /* is blacklisted */
          schedule_is_allowed = TRUE;
        }
        else
        {
          srch_criteria = 8;      /* is blacklisted */
        }
      }
      else
      if( l1_sc_wcdma_data_ptr->serving_RLA_C < l1_sc_wcdma_data_ptr->wcdma_srch.priority_search_threshold )
      {
         /*
         ** scell is below the threshold, allow ALL, higher or lower priority rats to be measured
         */
         schedule_is_allowed = TRUE;
         srch_criteria = 2;
      }
      else
      {
        /*
        ** scell is > threshold, only allow higher priority rat to be measured
        */
        srch_criteria = 3;
        if( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].priority > l1_sc_wcdma_data_ptr->rr_params.geran_priority)
        {
           srch_criteria = 4;
           schedule_is_allowed = TRUE;
        }
      }
  }
  else
  {
    /*
    ** legacy ranking based search uses Qsearch_I ==> only W (or T) cells, no LTE
     */
    srch_criteria = 5;

    thrsh_pwr = l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[0];

    if(l1_sc_wcdma_data_ptr->rr_params.wcdma_QSearch != QSEARCH_NEVER)
    {
      if(  (  ( l1_sc_wcdma_data_ptr->serving_RLA_C >= l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[0] )
            &&( l1_sc_wcdma_data_ptr->serving_RLA_C <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[0] ))
/* In G2T, don't consider Femto cells and don't schedule IRAT if Qsearch condition doesn't meet*/
    #ifdef FEATURE_GSM_TDS
       || ((femto_search_state == FEMTO_SEARCH_IN_PROGRESS ) && (!l1_sc_wcdma_data_ptr->tds_irat_active))
    #else
       ||( femto_search_state == FEMTO_SEARCH_IN_PROGRESS )
    #endif
       )
      {
         srch_criteria = 6;
         schedule_is_allowed = TRUE;
     }
    }
    /* The pwr check should catch "always" but just in case */
    if(l1_sc_wcdma_data_ptr->rr_params.wcdma_QSearch == QSEARCH_ALWAYS)
    {
       srch_criteria = 7;
       schedule_is_allowed = TRUE;
    }
  }

  MSG_GERAN_MED_4_G("G2X:srch is pending for index=%d criteria=%d sc_pwr=%d thrsh=%d", freq_idx, srch_criteria, l1_sc_wcdma_data_ptr->serving_RLA_C, thrsh_pwr );

  return schedule_is_allowed;

}  /* l1_sc_check_priority_threshold */

/*===========================================================================

FUNCTION L1_SC_WCDMA_RECEIVE

DESCRIPTION
  This function is called from task level to check if a WCDMA id search
  is ready to be initiated.

  This function is called when:

  52 idle - The monscan process is done with one reporting period in the
            rxlev callback.

            Every paging block period.

  52 idle - When there are no GSM neighbours to monitor and hence rxlev
            callback is not going to occur.

            Every paging block period.

  51 idle - When there are no GSM neighbours to monitor.

            Every paging block period.

  51 idle - The monscan process is done with one reporting period in the
            rxlev callback.

            Every paging block period.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_receive( void ( *done )( void*,gas_id_t gas_id ),gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  idle_data_T        *l1_idle_data         = &l1_idle_data_store[gas_id];

  boolean  irat_scheduled                  = FALSE;
  boolean  try_to_schedule                 = FALSE;
  boolean  max_freq_count_rchd             = FALSE;
  uint16   freq_idx = 0;
  uint16   loop_idx = 0;

  /* Do not Schedule reconfirm or id if one of them is already going on */
  if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
      /* This scenario will happen in G2T, if GL1 has already schedule G2T MEAS and then after performing monscan, it checks
      to perform NCELL IRAT search. If both states are not null, then return.*/
      MSG_GERAN_HIGH_2_G("G2X already active, srch_state %d, srch_type %d",
         l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
         l1_sc_wcdma_data_ptr->wcdma_srch.srch_type);
      return;
  }

#ifdef FEATURE_G2X_TUNEAWAY
  /* Do not schedule GTW search when GTA ongoing */
  if (gpl1_gprs_get_gta_config(gas_id))
  {
    MSG_GERAN_HIGH_0_G("No G2X search when GTA ongoing");
    return;
  }
#endif
   /* make sure SCE is in active state before starting new IRAT activity */
   if ( !l1_sc_globals_ptr->active )
   {
     MSG_GERAN_ERROR_1_G("called l1_sc_wcdma_receive() with SCE is not active :%d ", l1_sc_globals_ptr->active);
     return;
   }

  if(l1_tsk_buffer[gas_id].l1_state == L1_INACTIVE_MODE)
  {
   MSG_GERAN_ERROR_0_G("Already in inactive mode. Don't schedule anything");
   return;
  }

  /* If NCELL activity is running and G2X forced search is attempted from higher layers, then prioritize forced search and abort the NCELL activity */
  if(( l1_sc_globals_ptr->receive_in_progress ) &&
     ( l1_sc_wcdma_data_ptr->rr_params.ForcedMeas & FORCE_MEAS_ACTIVE) && 
     ( l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx] <= l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick ) )
  {
    abort_receive(gas_id);
    MSG_GERAN_MED_0_G("Aborting NCELL activity to allow G2X forced measurements");
  }

  /* Called in Task context */
  if ( l1_sc_globals_ptr->receive_in_progress )
  {
     MSG_GERAN_ERROR_0_G("called l1_sc_wcdma_receive() with rx in progress");
     return;
  }

  gl1_irat_mutex_lock( gas_id);

  l1_sc_wcdma_data_ptr->wcdma_tries = 0;

   MSG_GERAN_MED_2_G( "l1_sc_wcdma_receive Fn=%d l1_idle_data->mm_non_drx_mode %d",gl1_get_FN( gas_id ),l1_idle_data->mm_non_drx_mode);

   if(   (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
        || (l1_sc_wcdma_data_ptr->wcdma_srch.state != GtowStateIdle)
        || (l1_sc_wcdma_data_ptr->rr_params.gsm_only)
        || (l1_idle_data->mm_non_drx_mode)
     )
   {

     MSG_GERAN_HIGH_3_G("receive tick: -early return srch_state %d state %d mm_non_drx_mode %d",
                        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                        l1_sc_wcdma_data_ptr->wcdma_srch.state,
                        l1_idle_data->mm_non_drx_mode);
     gl1_irat_mutex_unlock( gas_id);

     return;
   }
   

   if( (l1_sc_wcdma_data_ptr->rr_params.ForcedMeas  &  FORCE_MEAS_ACTIVE)
     ||(l1_sc_wcdma_data_ptr->rr_params.fastSearchState == FAST_SEARCH_ARMED))
   {
      freq_idx = l1_sc_wcdma_data_ptr->rr_params.num_freqs; /* force a check in loop */
   }
   else
   {
      /*
      ** To prevent keep searching on the same freq
      ** Start next search from where the last finished.
      */
      freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
   }

   for (loop_idx=0; loop_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; loop_idx++)
   {
      freq_idx++;
      if( freq_idx >= l1_sc_wcdma_data_ptr->rr_params.num_freqs )
      {
        /* only check once per loop,
        ** continue with any other freqs if search criteria is not met
        */
        if((l1_sc_wcdma_data_ptr->rr_params.fastSearchState == FAST_SEARCH_ARMED) && (max_freq_count_rchd == FALSE))
        {
           freq_idx = l1_sc_wcdma_data_ptr->rr_params.fastSearchIndex;  /* force check in loop so this index is searched */
           l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = 0;      /* ensure id tick is expired */
           max_freq_count_rchd = TRUE;
        }
        else
        if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas & FORCE_MEAS_ACTIVE)
        {
           freq_idx = l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx; /* force check in loop */
        }
        else
        {
           freq_idx=0;  /* wrap around */
        }
      }

      irat_scheduled   = l1_sc_check_priority_threshold(freq_idx, gas_id);
      try_to_schedule |= irat_scheduled;
      /* check must be rat dependant */

      if(irat_scheduled &&  ( l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] <= l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick ))
      {
          /*
           * Don't start any more WCDMA cell id searches, if we
           * have identified all the cells from RR
           * Comparing with the same freq_idx is because we init the stop_cell_id_srch
           * as soon as we receive a new list from RR
           */

        if( ( ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN == l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx] )
              &&
              ( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat //do we need this check at all?
#ifdef FEATURE_GSM_TDS
                || RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat //do we need this check at all?
#endif
              )
            )
#ifdef  FEATURE_GSM_TO_LTE

          ||
            (  ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.euarfcn == l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx] )
             &&( RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )&& (!l1_sc_wcdma_data_ptr->elevator_flag)//do we need this check at all?
            )
#endif
          )
        {
          MSG_GERAN_MED_2_G("Don't Schedule Cell ID Srch on UARFCN=%d, RAT %d",
                  l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx],l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat);

          if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat
#ifdef FEATURE_GSM_TDS
            || RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat
#endif
            )
          {
            if (l1_sc_g2x_check_for_extend_srch_period(gas_id))
            {
              l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + (l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle << 1);
            }
            else
            {
            l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle;
            }
          }
          else
          {
          /*FR37926 non-DDS sub IRAT optimization, G2L search period should be made 10 sec (id_cycle_lte * 2) (if mfrm=2)
            But if Forced search is active, then should use normal periodicity, as intention is to do search every 2.5 secs */
            if (l1_sc_g2x_check_for_extend_srch_period(gas_id) && (l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL))
            {
              l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + (l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte << 1);
            }
            else
            {
              l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte;
            }
          }
        }
        else
        {
          l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = freq_idx;

#ifdef FEATURE_GSM_TDS
          if( ( RR_L1_IRAT_RAT_WCDMA     == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) ||
              ( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat ) )
#else
          if( RR_L1_IRAT_RAT_WCDMA     == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
#endif
          {
             if (l1_sc_g2x_check_for_extend_srch_period(gas_id))
             {
                l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + (l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle << 1);
             }
             else
             {
                l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle;
             }
#ifdef FEATURE_GSM_TDS_DEBUG
             MSG_GERAN_HIGH_3_G("id search freq_idx %d page_cycle_tick %d id_tick %d", freq_idx, l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick, l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx]);
#endif /*FEATURE_GSM_TDS_DEBUG*/
          }
          else
          {
             /*FR37926 non-DDS sub IRAT optimization, G2L search period should be made 10 sec (id_cycle_lte * 2) (if mfrm=2)
               But if Forced search is active, then should use normal periodicity, as intention is to do search every 2.5 secs */
             if (l1_sc_g2x_check_for_extend_srch_period(gas_id) && (l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL))
             {
                l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + (l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte << 1);
             }
             else
             {
                l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle_lte;
             }
          }
          l1_sc_wcdma_data_ptr->wcdma_srch.abort    = FALSE;
          l1_sc_wcdma_data_ptr->wcdma_srch.done     = done;
          l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_ID, gas_id);
          l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_WAIT;

          l1_sc_globals_ptr->receive_in_progress = TRUE;
          L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_IRAT,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);

          l1_isr_femto_timer_stop();

          /* This break statement needs to be inside the else{ }.
           * This is a bug caused by change list 104635.
           */
           break;
         } /* else */

      } /* End of if (l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] <= l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick) */
   }   /* for (loop_idx=0; loop_idx<l1_sc_wcdma_data_ptr->rr_params.num_freqs; loop_idx++) */

   if(!try_to_schedule)  /* maintain original logic for this */
   {
     /*
     ** Is this the right place for this now ?
     */
#ifdef FEATURE_GSM_TDS
     l1_sc_tds_report(gas_id);
#endif
     MSG_GERAN_MED_2_G( "l1_sc_wcdma_receive Fn=%d NO meas scheduled RLA_C=%d ",gl1_get_FN(gas_id), l1_sc_wcdma_data_ptr->serving_RLA_C);
   }
   else
   {
      MSG_GERAN_MED_3_G( "l1_sc_wcdma_receive end: state=%d srch_state=%d freq_idx=%d ", l1_sc_wcdma_data_ptr->wcdma_srch.state,l1_sc_wcdma_data_ptr->wcdma_srch.srch_state, l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset );
   }

    gl1_irat_mutex_unlock( gas_id);

    /* we want to check the receive_in_progress flag if optimization is off OR G2L fast search is active */
    if (l1_sc_globals_ptr->receive_in_progress == TRUE
        #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
        && (!l1_sc_g2x_timeline_opt_enable[gas_id] ||
            (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_LTE && 
             l1_sc_wcdma_data_ptr->rr_params.fastSearchState != FAST_SEARCH_INACTIVE))
        #endif
       )
    {
       gl1_drx_require_next_tick(gas_id);
    }

} /* l1_sc_wcdma_receive */


/*===========================================================================

FUNCTION L1_SC_SET_GTOW_SRCH_STATE_NULL
DESCRIPTION
  This function is called when SRCH state is going to null.

===========================================================================*/
void l1_sc_set_gtow_srch_state_null (gas_id_t gas_id)
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T     *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  gl1_irat_mutex_lock( gas_id);

  MSG_GERAN_HIGH_3_G("SC IRAT srch state set to NULL, active rat %d, srch_state %d, srch_in_progress %d ",
                      l1_sc_globals_ptr->active_irat,
                      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                      l1_sc_globals_ptr->wcdma_srch_in_progress);

  l1_sc_irat_fm_clear(l1_sc_globals_ptr->active_irat, l1_sc_wcdma_data_ptr->wcdma_srch.srch_type, gas_id);

  /* FM is clear, so grm will remove our reservation with TRM. Clear the IRAT 
     variable indicating the reservation state */
  l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = FALSE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* Release/Flush IRAT band now */
  grm_transaction_manager(GRM_TM_FLUSH, SC_IRAT,l1_tskisr_blk->client_id , gas_id);
#endif

  l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_UNDEFINED;
  if((l1_sc_globals_ptr->wcdma_srch_in_progress == TRUE ||
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT ||
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOL_SRCH_STATE_ABORT_CNF_WAIT) &&
      l1_sc_globals_ptr->receive_in_progress == TRUE)
  {
    l1_sc_globals_ptr->receive_in_progress = FALSE;
    L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_IRAT,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);
  }

  l1_sc_globals_ptr->wcdma_srch_in_progress = FALSE;
  L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);


#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
  /*Inform MCPM about critical scenario OFF now if needed*/
  if(critical_scn_updated_to_mcpm[gas_id] == TRUE)
  {
    gl1_clkdata_speed(FALSE, L1_TIME_LINE_ERROR, 0,L1_TIME_LINE_DELAY, gas_id);
    critical_scn_updated_to_mcpm[gas_id] = FALSE;
  }
#endif
  /*For dedicated mode search, clear start wcdma receive flag*/
 if((l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED) ||
     (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID_IN_TBF) || 
     (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID) ||
     (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM) )
  {
    l1_sc_g2x_stop_mcpm_irat_ngbr(gas_id);
    l1_sc_stop_wcdma_receive(gas_id);
  }

  l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_NULL, gas_id);
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_NULL;
  l1_sc_wcdma_data_ptr->wcdma_srch.abort = FALSE;
  l1_sc_wcdma_data_ptr->ded_srch_aborted = FALSE;

  /* Ensure G2L-specific state machine is set to NULL.  Note this usually 
     happens before l1_sc_set_gtow_srch_state_null() is called, but in some
     scenarios (like wakeup error recovery with IRAT cancellation) it won't be,
     and it doesn't hurt to set it again to ensure no more G2L activity. */
  l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap = G2L_MDSP_NULL;
  gl1_msg_resume_pwr_meas(gas_id);

  if (!gl1_hw_qta_gap_active(gas_id) || !gl1_hw_get_qta_gap_for_g2x_irat(gas_id))
  {
    /* If G2x shutdown pending, then do it now */
    l1_sc_g2x_pending_shutdown(gas_id);
  }

  gl1_irat_mutex_unlock( gas_id);

}

/*===========================================================================

FUNCTION L1_SC_FM_CLEAR
DESCRIPTION
  This function clears requested frames for given IRAT.

===========================================================================*/
void l1_sc_irat_fm_clear(rr_l1_irat_rat_e l_rat, e_wcdma_srch_type srch_type, gas_id_t gas_id)
{
  l1_fm_priority_T pri;

  if (srch_type != GTOW_SRCH_TYPE_DEDICATED)
  {
    switch (l_rat)
    {
    case RR_L1_IRAT_RAT_WCDMA:
       if (srch_type == GTOW_SRCH_TYPE_RECONFIRM)
       {
          pri = FM_PRI_WCDMA_RECONF;
       }
       else
       {
          pri = FM_PRI_WCDMA_IDSEARCH;
       }
       break;

#ifdef FEATURE_GSM_TDS
    case RR_L1_IRAT_RAT_UTRAN_TDD:
       pri = FM_PRI_TDS_ACQ;
       break;
#endif

#ifdef  FEATURE_GSM_TO_LTE
    case RR_L1_IRAT_RAT_LTE:
       pri = FM_PRI_LTE_IDSEARCH;
       break;
#endif

    default:
       pri = FM_PRI_RESERVED;
       l1_fm_clear_irat_activity(gas_id);
       break;

    }

    if (pri != FM_PRI_RESERVED)
    {
       MSG_GERAN_HIGH_1_G("Clear IRAT on FM activity %d",pri);
       l1_fm_clear_required_frames(pri, gas_id);
    }
  }
}
/*===========================================================================

FUNCTION L1_SC_SET_GTOW_SRCH_TYPE
DESCRIPTION
  This function is called to set the SRCH type.

===========================================================================*/
void l1_sc_set_gtow_srch_type (e_wcdma_srch_type new_srch_type, gas_id_t gas_id)
{
   l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type != new_srch_type)
   {
      /* Check for srch type change in the middle of a search */
      if ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_type != GTOW_SRCH_TYPE_NULL) && (new_srch_type != GTOW_SRCH_TYPE_NULL))
      {
         MSG_GERAN_ERROR_3_G("G2X search type change when active: old %d, new %d, search state %d",
                                l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                                new_srch_type,
                                l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);
      }
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_type = new_srch_type;
      MSG_GERAN_HIGH_1_G("Set G2X search type %d", new_srch_type);
   }
}

/*===========================================================================

FUNCTION L1_SCI_ABORT_WCDMA_TICK

DESCRIPTION
  This function is called from l1_sci_tick_wcdma if the abort_srch is set.

===========================================================================*/
static void l1_sci_abort_wcdma_tick (uint16 freq_idx, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T     *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  MSG_GERAN_HIGH_5_G( "l1_sc_sm_irat: ABORTING! State %d, type %d, active RAT %d, CM=0x%x FS %d",
                         l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                         l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                         l1_sc_globals_ptr->active_irat,
                         l1_sc_wcdma_data_ptr->lte_cm_irat_active,
                         l1_sc_wcdma_data_ptr->rr_params.fastSearchState);

#ifdef  FEATURE_GSM_TO_LTE
  l1_sc_g2l_fast_search_abort(gas_id);
#endif /* FEATURE_GSM_TO_LTE */

  switch ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state )
  {
    case GTOW_SRCH_STATE_ACTIVE:
      {
        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;

#ifdef  FEATURE_GSM_TO_LTE
        if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE )
        {
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
#endif /* FEATURE_GSM_TO_LTE */
#ifdef FEATURE_GSM_TDS
        /* GL1 can't abort T measurement in Active state */
        if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UTRAN_TDD )
        {
          MSG_GERAN_ERROR_0_G("Trying to abort G2T activity in GTOW_SRCH_STATE_ACTIVE");
          L1_send_TDS_G2T_ABORT_REQ(gas_id);
          l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[freq_idx] = TRUE;
       }
        else
#endif // FEATURE_GSM_TDS
        if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_WCDMA )
        {
           if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM)
           {
              if(!gl1_msg_abort_wcdma_reconfirm(gas_id))
              {
                 l1_sc_set_gtow_srch_state_null (gas_id);
              }
           }
           else
           {
              /* Set the flag to ensure that a new idle ID search can run
              at the earliest oppertunity in idle mode */
              l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[freq_idx] = TRUE;

              if( !gl1_msg_abort_wcdma_identify(gas_id) )
              {
                 l1_sc_set_gtow_srch_state_null (gas_id);
              }
           }
        }
      }
      break;

    case GTOW_SRCH_STATE_SCHEDULE_WCDMA:
      {
        /* Set the flag to ensure that a new idle ID search can run
           at the earliest oppertunity in idle mode */
        l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[freq_idx] = TRUE;

        if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM)
        {
           if(!gl1_msg_abort_wcdma_reconfirm(gas_id))
           {
              l1_sc_set_gtow_srch_state_null (gas_id);
           }
        }
        else
        {
           if( !gl1_msg_abort_wcdma_identify(gas_id) )
           {
              l1_sc_set_gtow_srch_state_null (gas_id);
           }
        }
      }
      break;

#ifdef  FEATURE_GSM_TO_LTE
    case GTOW_SRCH_STATE_SCHEDULE_LTE:
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
           /* In this place, we only handle quick cleanup for abort if we haven't sent TIMED_SRCH_MEAS_REQ but might have sent G2X STARTUP to GFW. 
           ** For those ABORT after sending TIMED_SRCH_MEAS_REQ, we will handle cleanup in the l1_sc_lte_process_timed_search_results function */
           l1_sc_g2l_schedule_abort(gas_id );
           l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;
         }
       }
       else
       {
         MSG_GERAN_ERROR_1_G("LTE RAT is not active rat %d  in Schedule state",l1_sc_globals_ptr->active_irat);
       }
       break;
#endif

    case GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES:
      {
        /* Already sent abort, just set the flag to ensure that a new idle ID search can run
        at the earliest oppertunity in idle mode */
        l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[freq_idx] = TRUE;
      }
      break;

    case GTOW_SRCH_STATE_WAIT:
#ifdef FEATURE_GSM_TDS
    case GTOW_SRCH_STATE_SCHEDULE_TDS:
#endif
      {
        if(l1_sc_g2l_empty_qta_gap[gas_id])
        {
          l1_sc_g2l_empty_qta_gap[gas_id] = FALSE;
          l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOL_SRCH_STATE_ABORT_CNF_WAIT;
          L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ(gas_id);
        }
        else
        {
          /*
          ** only booking with FM has been done so far, no cleanup required.
          */
          l1_sc_set_gtow_srch_state_null (gas_id);

          /* Set the flag to ensure that a new idle ID search can run
          at the earliest oppertunity in idle mode */
          l1_sc_wcdma_data_ptr->wcdma_srch.last_idle_id_aborted[freq_idx] = TRUE;
        }
      }
      break;

    default:
      {
          MSG_GERAN_MED_2_G( "SC IRAT Aborting in State %d, srch type %d - do nothing",
             l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
             l1_sc_wcdma_data_ptr->wcdma_srch.srch_type);
      }
      break;
    }

  /* If the abort procedure isn't complete, make sure we don't go to sleep */
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
  if(!l1_sc_g2x_timeline_opt_enable[gas_id] ||
     l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
#endif
  {
    gl1_drx_require_next_tick(gas_id);
  }
  l1_sc_wcdma_data_ptr->wcdma_srch.abort = FALSE;
}  /* l1_sci_abort_wcdma_tick */

/*===========================================================================

FUNCTION L1_SCI_TICK_WCDMA

DESCRIPTION
  This function is called from isr level to manage any active WCDMA
  searches.

  This is called every TDMA frame in 51 and 52 idle.

  l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset

  wcdma cell id searches are issued to message layer.

  reconfirm searches are scheduled from this function when they cannot be done
  inside page demod scheduling (e.g. assigned RF device not WCDMA capable)

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_tick_wcdma( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T     *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  uint16              freq_idx              = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;

  /* If reconfrim then set freq idx for reconfirm */
  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM)
  {
    freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset;
  }

 gl1_irat_mutex_lock( gas_id);

#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
    if( (l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found == TRUE) &&( nv_for_search_all_w_cell ) )
    {
      if( l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan < 0)
      {
        l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found = FALSE;
        l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan = -1;
      }
    }
#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */


  if(l1_sc_get_wcdma_list_update_inprogress(gas_id))
  {
    MSG_GERAN_MED_1_G("update_list: wcdma_srch.srch_state:%d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);

    switch ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state )
    {
      case GTOW_SRCH_STATE_WAIT:
      case GTOW_SRCH_STATE_SCHEDULE_WCDMA:
#ifdef FEATURE_GSM_TDS
      case GTOW_SRCH_STATE_SCHEDULE_TDS:
#endif
#ifdef FEATURE_GSM_TO_LTE
      case GTOW_SRCH_STATE_SCHEDULE_LTE:
#endif
        /*Set The Abort Flag, and the state machine will handle it*/
        l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
      default:
        break;
    }
  }


  /*Print search states only when state is not NULL*/
  if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
   MSG_GERAN_HIGH_1_G( "l1_sci_tick_wcdma: srch state %d " ,
                        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);
  }

  /* Called in Interrupt context */
  if (l1_sc_wcdma_data_ptr->wcdma_srch.abort)
  {
    l1_sci_abort_wcdma_tick(freq_idx, gas_id);
  }

  switch ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state )
  {
    case GTOW_SRCH_STATE_WAIT:
    {
#ifdef FEATURE_GSM_TO_LTE
     /* Check the frequency type */
      if( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_LTE )
      {
        l1_sc_idle_g2l_state(LTE_IDLE_GAP_WAIT, gas_id);

        /* if G2X frontend timeline optimization is off, we always want the next frame
           if fast search is active, we always want the next frame */
        #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
        if(!l1_sc_g2x_timeline_opt_enable[gas_id] ||
           l1_sc_wcdma_data_ptr->rr_params.fastSearchState != FAST_SEARCH_INACTIVE)
        #endif
        {
#ifndef FEATURE_IRAT_RESERVE_SLEEP
           gl1_drx_require_next_tick(gas_id);
#endif
        }
      }
      else
#endif /* FEATURE_GSM_TO_LTE */
#ifdef FEATURE_GSM_TDS
      if ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_UTRAN_TDD)
      {
        // if GL1 is still waiting for IRAT_INIT-CNF, then don't schedule TDS and remain in GTOW_SRCH_STATE_WAIT state
        if (l1_sc_wcdma_data_ptr->tds_irat_active)
        {
           l1_sc_g2t_wait_state(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
        }
#ifndef FEATURE_IRAT_RESERVE_SLEEP
        gl1_drx_require_next_tick(gas_id);
#endif
      }
      else
#endif /* FEATURE_GSM_TDS */
      /* explicit RAT check */
      if ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA)
      {
        if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type ==  GTOW_SRCH_TYPE_RECONFIRM)
        {
           l1_sc_g2w_reconf_wait_state(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
        }
        else
        {
           l1_sc_g2w_id_wait_state(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
        }
#ifndef FEATURE_IRAT_RESERVE_SLEEP
        gl1_drx_require_next_tick(gas_id);
#endif
      }
    } /* GTOW_SRCH_STATE_WAIT */
    break;

    case GTOW_SRCH_STATE_SCHEDULE_WCDMA:
      {
         if( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA )
         {
           l1_sc_g2w_schedule(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
         }
         else
         {
           l1_sc_set_gtow_srch_state_null (gas_id);
           MSG_GERAN_HIGH_2_G(" freq %d belonging to RAT %d not being scheduled as update_list occured. ",
                     l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
                     l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat );
         }

         gl1_drx_require_next_tick(gas_id);

     } /* GTOW_SRCH_STATE_SCHEDULE_WCDMA */
    break;

#ifdef FEATURE_GSM_TO_LTE
    case GTOW_SRCH_STATE_SCHEDULE_LTE:
    {
      l1_sc_idle_g2l_state(LTE_IDLE_GAP_SCHEDULE, gas_id);
      /* Make sure we don't go to sleep */
      gl1_drx_require_next_tick(gas_id);
    } /* GTOW_SRCH_STATE_SCHEDULE_LTE */
    break;
#endif

#ifdef FEATURE_GSM_TDS
    case GTOW_SRCH_STATE_SCHEDULE_TDS:
    {
      if( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_UTRAN_TDD )
      {
           l1_sc_g2t_schedule(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_3_G(" RAT %d UARFCN %d scheduled in GTOW_SRCH_STATE_SCHEDULE_TDS - id_freq_off %d ",
                  l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat,
                  l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
                  freq_idx );

        l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
      }

      gl1_drx_require_next_tick(gas_id);
    }
    break;
#endif


    case GTOW_SRCH_STATE_ACTIVE:
      {
#ifdef FEATURE_GSM_TDS
       if((l1_sc_wcdma_data_ptr->tds_irat_active) && (l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UTRAN_TDD))
       {
         if(mdsp_decrement_cleanup_time_offset_flag[gas_id] == FALSE)
         {
           MSG_GERAN_HIGH_0_G("G2T: GL1 is waiting for the response from TL1 after the gap");
         }
       }
#endif
       /* Make sure we don't go to sleep */
       gl1_drx_require_next_tick(gas_id);
       break;
      }

    case GTOW_SRCH_STATE_ABORTING:
    case GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES:

      /* Make sure we don't go to sleep */
      gl1_drx_require_next_tick(gas_id);

      break;

    case GTOL_SRCH_STATE_ABORT_CNF_WAIT:
      {
        if(g2l_get_abort_cnf_pending_status(gas_id))
        {
          gl1_drx_require_next_tick(gas_id);
        }
        else
        {
          l1_sc_set_gtow_srch_state_null(gas_id);
        }
      }
      break;

    case GTOW_SRCH_STATE_CLEANUP:

      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
#ifdef FEATURE_GSM_TDS
      if (l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UTRAN_TDD)
      {
         /* if we've been aborted then l1_tds_issue_cleanup will be set
          * in l1_sci_wcdma_aborted(), ensuring issuance of cleanup.
          * In normal operation if we get response early from TL1 then
          * close the gap in the next frame.
          */

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
          if (l1_sc_g2x_timeline_opt_enable[gas_id])
          {
             if (TRUE == mdsp_decrement_cleanup_time_offset_flag[gas_id])
             {
                l1_sc_wcdma_data_ptr->l1_tds_issue_cleanup = TRUE;
             }
          }
          else
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
          {
             if (TRUE == mdsp_decrement_cleanup_time_offset_flag[gas_id])
             {
                l1_sc_wcdma_data_ptr->l1_tds_issue_cleanup = TRUE;
                gl1_sc_cleanup_wait_count = 2;
             }
             else
             {
                gl1_sc_cleanup_wait_count = 1;
             }
          }

          MSG_GERAN_HIGH_1_G("G2T: GTOW_SRCH_STATE_TDS_CLEANUP - issue any pending cleanup %d", l1_sc_wcdma_data_ptr->l1_tds_issue_cleanup);

         /* Set number of ticks in CLEANUP_WAIT state */
         if (TRUE == gl1_msg_get_multi_sim_mode() )
         {
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
            if (l1_sc_g2x_timeline_opt_enable[gas_id])
            {
               if (l1_sc_x2g_ta_irat_gap(gas_id))
               {
                  /* For multisim G2T is always NULL2T, we don't have to send GFW cleanup 
                     and wait for multiple ticks to shut down IRAT state machine */
                     
                  mdsp_decrement_startup_time_offset_flag[gas_id] = FALSE;
                  mdsp_decrement_cleanup_time_offset_flag[gas_id] = FALSE;
 
                  l1_g2t_gap_end(gas_id);
              }
              else
              {
                 l1_sc_wcdma_data_ptr->l1_tds_issue_cleanup = TRUE;
              }
            }
            else
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
            {
              gl1_sc_cleanup_wait_count = 3;
            }
         }
      }
      else
#endif /* FEATURE_GSM_TDS */
      {
        gl1_sc_cleanup_wait_count = 1;
      }

      /* Make sure we don't go to sleep */
      gl1_drx_require_next_tick(gas_id);
      break;

    case GTOW_SRCH_STATE_CLEANUP_WAIT:
    {

      MSG_GERAN_HIGH_1_G("GTOW_SRCH_STATE_CLEANUP_WAIT, count %d", gl1_sc_cleanup_wait_count);
      gl1_sc_cleanup_wait_count -= 1;
      if(gl1_sc_cleanup_wait_count < 0)
      {
         gl1_sc_cleanup_wait_count = 0;
      }
#ifdef FEATURE_GSM_TDS
      if (l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UTRAN_TDD)
      {
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
         if (l1_sc_g2x_timeline_opt_enable[gas_id])
         {
            if (l1_g2t_delay_for_fw_startup == 0)
            {
               l1_g2t_gap_end(gas_id);
            }
         }
         else
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
        {
            if (gl1_sc_cleanup_wait_count == 0)
            {
               if (mdsp_decrement_startup_time_offset_flag[gas_id])
               {
                  mdsp_decrement_startup_time_offset_flag[gas_id] = FALSE;
                  mdsp_decrement_cleanup_time_offset_flag[gas_id] = FALSE;
                  MSG_GERAN_HIGH_0_G("G2T: Cancel pending startup cmd");
               }
               l1_g2t_gap_end(gas_id);
            }
         }   
      }
      else
#endif /* FEATURE_GSM_TDS */
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
      if(l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_WCDMA)
      {
        if(g2w_is_wfw_active(gas_id))
        {
          MSG_GERAN_HIGH_0_G("GTOW_SRCH_STATE_CLEANUP_WAIT waiting for g2w drivers to get to stopping point");
        }
        else
        {
          l1_sc_set_gtow_srch_state_null(gas_id);
        }
      }
      else
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */

#ifdef FEATURE_GSM_TO_LTE
      /*
      ** this tick follows l1_g2l_trigger_mdsp() in isr, so any change will occur in the same frame
      */
      if(l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE)
      {
          if(l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap != G2L_MDSP_NULL)
          {
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
            if (l1_sc_g2x_timeline_opt_enable[gas_id])
            {
#ifdef FEATURE_QSH_MDUMP
              QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
              ERR_GERAN_FATAL_1_G("G2L_MDSP state not NULL when GTOW_SRCH_STATE_CLEANUP_WAIT  gap=%d" , l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap);
            }
            else
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
            {
              /* G2L cleanup not yet finished, wait another tick */
              MSG_GERAN_HIGH_1_G("GTOW_SRCH_STATE_CLEANUP_WAIT waiting for g2l cleanup  gap=%d" , l1_sc_wcdma_data_ptr->l1_sc_trigger_g2l_gap);
              gl1_sc_cleanup_wait_count = 1;
            }
          }

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
          if (l1_sc_g2x_timeline_opt_enable[gas_id])
          {
            if(cleanup_cmd_sent[gas_id] == FALSE)
            {
            l1_sc_set_gtow_srch_state_null(gas_id);
          }
          else
            {
            /*If FEATURE_GSM_G2X_TIMELINE_REDUCTION feature enable and we didn't send immediate clean command then wait for one more frame
              to change IRAT state to NULL */
              cleanup_cmd_sent[gas_id] = FALSE;
              MSG_GERAN_HIGH_1_G("G2L cleanup: GTOW_SRCH_STATE_CLEANUP_WAIT waiting for one frame cleanup_cmd_sent=%d" , cleanup_cmd_sent[gas_id]);
            }
             
          }
          else
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
          {
            if(gl1_sc_cleanup_wait_count == 0)
            {
              l1_sc_set_gtow_srch_state_null(gas_id);
            }
          }
      }
      else
#endif
      if(l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UNDEFINED)
      {
#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
         ERR_GERAN_FATAL_0_G("active_irat RR_L1_IRAT_RAT_UNDEFINED in srch_state GTOW_SRCH_STATE_CLEANUP_WAIT");
      }
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
      if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_CLEANUP_WAIT || !l1_sc_g2x_timeline_opt_enable[gas_id])
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
      {
        /* Make sure we don't go to sleep */
        gl1_drx_require_next_tick(gas_id);
      }
    }
    break;

    default:
      MSG_GERAN_ERROR_1_G( "G2X SC tick: unknown srch state %d" ,l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);
      break;

  } /* switch ( wcdma_srch.id_state ) */
/* MCPM IRAT clock turned ON 1 frame in advance for IRAT search at idle mode*/
  switch (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state)
  {
#ifdef FEATURE_GSM_TO_LTE
    case GTOW_SRCH_STATE_SCHEDULE_LTE:
      l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_LTE, GL1_MODE_IDLE, gas_id);	
      break;
#endif
    case GTOW_SRCH_STATE_SCHEDULE_WCDMA:
      l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_WCDMA, GL1_MODE_IDLE, gas_id);	
      break;
#ifdef FEATURE_GSM_TDS 
    case GTOW_SRCH_STATE_SCHEDULE_TDS:
      l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_UTRAN_TDD, GL1_MODE_IDLE, gas_id);
      break;
#endif
    default:
      MSG_GERAN_HIGH_1_G("l1_sc_g2x_idle_irat_tick: srch state %d",l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);
  }
  gl1_irat_mutex_unlock( gas_id);
  return;
} /* l1_sci_tick_wcdma */


/*===========================================================================

FUNCTION L1_SCI_POST_TICK_WCDMA

DESCRIPTION
  This function is called from isr level to manage any active WCDMA
  searches.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_post_tick_wcdma( gas_id_t gas_id )
{
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
  uint16 freq_idx;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T     *l1_sc_globals_ptr   = gl1_ms_switch_l1_sc_globals_store(gas_id);
  
  if(l1_sc_g2x_timeline_opt_enable[gas_id])
  {
    gl1_irat_mutex_lock( gas_id);
  
    if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID)
    {
      freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
    }
    else
    {
      freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset;
    }
  
  
    /*Print search states only when state is not NULL*/
    if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT || 
       l1_sc_wcdma_data_ptr->wcdma_srch.abort)
    {
     MSG_GERAN_HIGH_3_G("l1_sci_POST_tick_wcdma: srch state %d (freq %d abort %d)",
                        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                        freq_idx,
                        l1_sc_wcdma_data_ptr->wcdma_srch.abort);
    }
  
    if (l1_sc_wcdma_data_ptr->wcdma_srch.abort)
    {
      l1_sci_abort_wcdma_tick(freq_idx, gas_id);
    }
  
    /* for post_tick, we generally only want to do a reserve_at() for any G2X
       that's in WAIT state and was previously blocked in the regular tick
       function by some other msg-layer activity (which will now be finished here).
       Without doing reserve_at() here, we'd have to wait until next ISR tick, 
       which would delay sleep processing and make us use more power. */
  
    if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT)
    {
  #ifdef FEATURE_GSM_TO_LTE
      if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_LTE)
      {
         l1_sc_idle_g2l_state(LTE_IDLE_GAP_WAIT, gas_id);
      }
      else
  #endif
  #ifdef FEATURE_GSM_TDS
      if (l1_sc_wcdma_data_ptr->tds_irat_active)
      {
         l1_sc_g2t_wait_state(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
      }
      else
  #endif
      if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA)
      {
        if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID)
        {
          l1_sc_g2w_id_wait_state(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
        }
        else if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type ==  GTOW_SRCH_TYPE_RECONFIRM)
        {
           l1_sc_g2w_reconf_wait_state(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
        }
      }
    }
/*MCPM IRAT clock turned ON 1 frame in advance for IRAT search at idle mode */
    switch (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state)
    {
#ifdef FEATURE_GSM_TO_LTE
      case GTOW_SRCH_STATE_SCHEDULE_LTE:
        l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_LTE, GL1_MODE_IDLE, gas_id);	
        break;
#endif
      case GTOW_SRCH_STATE_SCHEDULE_WCDMA:
        l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_WCDMA, GL1_MODE_IDLE, gas_id);	
        break;
#ifdef FEATURE_GSM_TDS 
      case GTOW_SRCH_STATE_SCHEDULE_TDS:
        l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_UTRAN_TDD, GL1_MODE_IDLE, gas_id);
        break;
#endif
      default:
        MSG_GERAN_HIGH_1_G("l1_sc_g2x_post_idle_irat_tick: srch state %d ",l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);
     }

    gl1_irat_mutex_unlock(gas_id);
  }
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */

#ifndef FEATURE_IRAT_RESERVE_SLEEP
#ifndef FEATURE_GSM_G2X_TIMELINE_REDUCTION
 l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */

 if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT)
 {
   /* Check here whether gap is available to complete a WCDMA ID search */
   if( l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset].rat == RR_L1_IRAT_RAT_WCDMA )
   {
     if (l1_fm_get_available_frames(FM_PRI_WCDMA_IDSEARCH, gl1_get_FN( gas_id ), gas_id) > MAX_ID_SEARCH_FRAMES)
     {
       gl1_drx_require_next_tick(gas_id);
     }
   }
#ifdef FEATURE_GSM_TDS
   else if (l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset].rat == RR_L1_IRAT_RAT_UTRAN_TDD)
   {
     /* In G2T idle mode, reconfirmation is also done using FM*/
     if (l1_fm_get_available_frames(FM_PRI_TDS_ACQ, gl1_get_FN( gas_id ), gas_id) > (MAX_TDS_SEARCH_FRAMES +  1))
     {
       gl1_drx_require_next_tick(gas_id);
     }
   }
#endif
#ifdef  FEATURE_GSM_TO_LTE
   else if (l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset].rat == RR_L1_IRAT_RAT_LTE)
   {
     if (l1_fm_get_available_frames(FM_PRI_LTE_IDSEARCH, gl1_get_FN( gas_id ), gas_id) > MAX_LTE_SEARCH_FRAMES)
     {
       gl1_drx_require_next_tick(gas_id);
     }
   }
#endif
 }
#endif /* !FEATURE_IRAT_RESERVE_SLEEP */
}

/*===========================================================================

FUNCTION L1_SCI_WCDMA_ABORTED

DESCRIPTION
  Callback to be called when an WCDMA search has to be aborted because
  layer1 needs frames that were previously available to the surround
  cell engine.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_wcdma_aborted( void* dummy, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  NOTUSED( dummy );

  gl1_irat_mutex_lock( gas_id);

  MSG_GERAN_HIGH_3_G( "G2X aborted in state=%d Fn=%d RAT %d" ,
           l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,gl1_get_FN( gas_id ),l1_sc_globals_ptr->active_irat);

  /* Called in Interrupt context */

   /* If active abort the message layer */

  switch( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state )
  {
  case GTOW_SRCH_STATE_ACTIVE:

    if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_WCDMA )
    {
      if( !gl1_msg_abort_wcdma_identify(gas_id) )
      {
        MSG_GERAN_ERROR_0_G(" abort not issued ");
      }
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES;
    }
#ifdef  FEATURE_GSM_TO_LTE
    else if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE )
    {
      L1_send_LTE_CPHY_IRAT_MEAS_G2L_ABORT_REQ(gas_id);
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES;
    }
#endif /* FEATURE_GSM_TO_LTE */
#ifdef FEATURE_GSM_TDS
    /* TDS activity cannot be aborted. Therefore if thisis called
     * then its because the activity took longer than booked by Frame
     * Manager. Therefore just go to state that tidies wverything up other
     * wise won't be able to do Idle Abort!! */
    /* In TDS, abort is not supported and GTOW_SRCH_STATE_ACTIVE means command
     * has issued to TL1. GL1 is waiting for the response after which GL1
     * clears l1_sc_wcdma_data_ptr->wcdma_srch.id_state. If FM ran out of frames then change state to
     * GTOW_SRCH_STATE_TDS_CLEANUP_WAIT which will clear the l1_sc_wcdma_data_ptr->wcdma_srch.id_state and
     * ignore the response from TL1 */
    else if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UTRAN_TDD )
    {
      /* If gap is finished, then no need to send Abort*/
      /* ISR runs before FM, which will make mdsp_decrement_cleanup_time_offset_flag FALSE*/
      /* Don't send Abort at the last Frame of GAP*/
      if(mdsp_decrement_cleanup_time_offset_flag[gas_id] && gl1_cleanup_decrement_time_offset[gas_id])
      {
       L1_send_TDS_G2T_ABORT_REQ(gas_id);
       l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;
      }
      else
      {
        /* Not changing state. Waiting for response from TL1 to change state*/
        MSG_GERAN_HIGH_0_G("G2T: FM abort after gap finished");
      }
    }
#endif

    break;

  case GTOW_SRCH_STATE_SCHEDULE_WCDMA:

    MSG_GERAN_MED_0_G( "Scheduled WCDMA search, now CLEANUP" );

    l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP;

    break;

  case GTOW_SRCH_STATE_SCHEDULE_LTE:

    MSG_GERAN_MED_0_G( "Scheduled LTE search, wait for abort confirmation" );  /* add state info for debug */

    break;

  case GTOW_SRCH_STATE_SCHEDULE_TDS:
    l1_sc_set_gtow_srch_state_null(gas_id);
    break;

  case GTOW_SRCH_STATE_CLEANUP:
  case GTOW_SRCH_STATE_CLEANUP_WAIT:
  case GTOW_SRCH_STATE_ABORTING:
  case GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES:
    /* In Abort/Cleanup state, Here do nothing and for ABort wait for ABort CNF. */
    break;

  default:

    MSG_GERAN_ERROR_1_G("No active search to abort state=%d ",l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);

    if ( (l1_sc_wcdma_data_ptr->drx_ref != 0) &&
       (IS_FRAME_NUM_LATER(l1_sc_wcdma_data_ptr->id_search_fn,gl1_get_FN( gas_id ))) )
    {
      /* remove old frame */
      gl1_drx_release_fn_tick( l1_sc_wcdma_data_ptr->id_search_fn, gas_id );
      l1_sc_wcdma_data_ptr->drx_ref = 0;
    }

    l1_sc_set_gtow_srch_state_null(gas_id);

    break;
  }
   gl1_irat_mutex_unlock( gas_id);
}

/*===========================================================================

FUNCTION L1_WCDMA_ABORT_SCHEDULED_RECONFIRM

DESCRIPTION
  Callback to be called when a scheduled WCDMA reconfirm has ran out of frames
  to release TRM and wait for the results from WCDMA

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_wcdma_abort_scheduled_reconfirm ( void* dummy, gas_id_t gas_id )
{
  NOTUSED( dummy );

  l1_sci_wcdma_abort_reconfirm( gas_id );
}

/*===========================================================================

FUNCTION L1_SCI_WCDMA_ABORT_RECONFIRM

DESCRIPTION
  Function called when a WCDMA reconfirmation search should be aborted
  (if one is active) because it is being preempted by a serving cell
  read.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_wcdma_abort_reconfirm( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /* Called in Interrupt context */
  gl1_irat_mutex_lock( gas_id);

  /* Abort the message layer */
  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE && l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM)
  {
    MSG_GERAN_MED_0_G( "G2W: abort_reconfirm" );

    l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;
    gl1_msg_abort_wcdma_reconfirm(gas_id);
  }
  gl1_irat_mutex_unlock( gas_id);
}



/*===========================================================================

FUNCTION L1_SCI_WCDMA_PAGING_BLOCK_TICK

DESCRIPTION
  Function called at start of paging block. A WCDMA reconfirmation
  search may be initiated.

  l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset

  reconfirm attemps are not booked with the frame manager because
  they are scheduled to happen during the paging block read.

DEPENDENCIES
  none

RETURN VALUE
  boolean  list_srch_started

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sci_wcdma_paging_block_tick( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
#ifdef FEATURE_GSM_TDS
  idle_data_T       *l1_idle_data = &l1_idle_data_store[gas_id];
#endif

  boolean list_srch_started = FALSE;
  boolean irat_scheduled    = FALSE;
  boolean  try_to_schedule  = FALSE;
  boolean search_allowed    = FALSE;
  uint16  freq_idx;
  uint16  loop_idx;
  uint16 tn_offset = 0;
  boolean activity_can_run = TRUE;
  boolean mcpm_start_required = FALSE;
  boolean pchxfer_supported = FALSE;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  gas_id_t other_gas_id = gl1_get_gas_id_of_other_g_sub( gas_id );
  if ( other_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED )
  {
    pchxfer_supported = gl1_check_ccch_in_transfer_permitted(other_gas_id) ;
  }
#endif
  /* Called in Interrupt context */
  MSG_GERAN_LOW_0_G( "l1_sci_wcdma_paging_block()" );

  gl1_irat_mutex_lock( gas_id);

  l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick++;

  /*
  ** check for expiry of fast search timeout on blacklisted freqs
  */
#ifdef  FEATURE_GSM_TO_LTE
  l1_sc_g2l_fast_search_expiry(gas_id);
#endif

  if (l1_sc_wcdma_data_ptr->wcdma_srch.state != GtowStateIdle)
  {
    gl1_irat_mutex_unlock( gas_id);
    return  FALSE;
  }

  if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
     MSG_GERAN_HIGH_3_G("G2X paging tick: no reconfim possible srch_state %d srch_type %d, rat %d",
                l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                l1_sc_globals_ptr->active_irat);

     gl1_irat_mutex_unlock( gas_id);
     return  FALSE;
  }

  if(l1_sc_globals_ptr->receive_in_progress )
  {
    MSG_GERAN_HIGH_0_G("G2X paging tick: receive_in_progress is already set, so return");
    gl1_irat_mutex_unlock( gas_id);
    return FALSE;
  }


 
  /* @@TODO: Change the checks below
   *
   * When the reslection algorithm is based on priority search
   * then the priority search threshold requirement overrides the
   * Qsearch_I.
   */

  MSG_GERAN_MED_3_G("Reconf params: priority_resel_alg %d, gsm_only %d priority_srch_th %d",
           l1_sc_wcdma_data_ptr->wcdma_srch.use_priority_reselection_algo,
           l1_sc_wcdma_data_ptr->rr_params.gsm_only,l1_sc_wcdma_data_ptr->wcdma_srch.priority_search_threshold );

  freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset;

  for (loop_idx=0; loop_idx<l1_sc_wcdma_data_ptr->rr_params.num_freqs; loop_idx++)
  {
     freq_idx++;
     if( freq_idx >= l1_sc_wcdma_data_ptr->rr_params.num_freqs )
     {
        freq_idx=0;  /* wrap around */
     }

     search_allowed   = l1_sc_check_priority_threshold(freq_idx,gas_id);
     try_to_schedule |= search_allowed;

     if(    search_allowed   /* check must be rat dependant */
         &&( l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[freq_idx] <= l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick )
         &&( l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells > 0 )
        )
     {
#ifdef FEATURE_GPRS_GBTA

        if(IS_GBTA_ACTIVE() && (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA)
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
           && 
           (pchxfer_supported == FALSE)
#endif
          )
        {
          /*dont want G2W in GBTA */
          volatile ISRTIM_CMD_BLK *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
          grm_info_t              *info_ptr;

          info_ptr = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id, gas_id);
          if ( info_ptr )
          {
             info_ptr->wreconf = TRUE;
          }
          else
          {
#ifdef FEATURE_QSH_MDUMP
            QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
            ERR_GERAN_FATAL_1_G("Invalid clientID!! gas-id = %d",gas_id);
          }

          MSG_GERAN_HIGH_0_G("Do GTA next PCH");
          break;
        }
#endif /* FEATURE_GPRS_GBTA */

        l1_sc_wcdma_data_ptr->searching_active = TRUE;
#ifdef FEATURE_GSM_TDS_DEBUG
        MSG_GERAN_HIGH_3_G("going for reconfirmation freq_idx %d reconfirm cycle %d reconfirm_tick %d", freq_idx,
                 l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle,l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[freq_idx]);
#endif /*FEATURE_GSM_TDS_DEBUG*/


#ifdef  FEATURE_GSM_TO_LTE
        if( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_LTE )
        {
           /* No reconfirmation for G2L - do nothing */
          continue;
        }
        else
#endif  /*FEATURE_GSM_TO_LTE */
#ifdef FEATURE_GSM_TDS
        if ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_UTRAN_TDD )
        {
           if (l1_sc_wcdma_data_ptr->tds_irat_active)
           {
              uint16 num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
              uint16 num_jdscells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells;
              if( num_cells > MAX_WCDMA_CELLS_PER_FREQ )
              {
                 num_cells = MAX_WCDMA_CELLS_PER_FREQ;
              }

              MSG_GERAN_HIGH_4_G("TDS:re-conf. search: ARFCN %d num_cells %d, num_jdscells %d, freq_idx %d",
                     l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,  num_cells, num_jdscells, freq_idx);

               /**
                *  CR346062 : Workaround to avoid T IRAT abort.
                *  Should be deleted after abort feature available.
                *  Since this code is running in the ISR refernce while
                *  mm_non_drx_mode is set in the task refernce, therefore to
                *  make ensure NON DRX mode is not set before initiating IRAT
                */
              if ( (0 != num_cells) && (l1_idle_data->mm_non_drx_mode == FALSE) && (num_jdscells > 0) )
              {
                l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_RECONFIRM, gas_id);
                l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_WAIT;
                l1_sc_globals_ptr->receive_in_progress = TRUE;
                L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_IRAT,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);

                l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset = freq_idx;

                 // To make sure that GL1 should send report to RR
                list_srch_started = TRUE;
                l1_sc_wcdma_data_ptr->reconf_req_fn = GSTMR_GET_FN_GERAN( gas_id);
              }
              else
              {
                 MSG_GERAN_ERROR_1_G("G2T: num_cells in UARFCN %d is zero",l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
              }
           }/*tds_irat_acive */
        }
        else
#endif
        /* Do not schedule W reconfirm if WFW not active yet */
        if (l1_sc_wcdma_data_ptr->wcdma_irat_active)
        {
           uint16  num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
           uint16  i = 0;

           /* check rf device compatability in multisim mode and ensure we are not
              trying to tuneaway from GSM during a tuneaway in e.g. LGW+GW multimode
            */
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
           if ( gl1_msg_get_multi_sim_mode() &&
                (( TRUE == l1_tsk_buffer[gas_id].qta_in_progress) 

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
                 || 
                  (pchxfer_supported == TRUE)
#endif
                ))
           {
              activity_can_run = FALSE;
           }
           else if(mcpm_start_required == FALSE)
#endif
           {

              /* optimised for value 32 */
              while( i < MAX_WCDMA_CELLS_PER_FREQ )
              {
                 l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i]   = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].wcdma_cell;
                 l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+1] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i+1].wcdma_cell;
                 l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+2] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i+2].wcdma_cell;
                 l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+3] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i+3].wcdma_cell;

                 i += 4;
              }

              if( num_cells > MAX_WCDMA_CELLS_PER_FREQ )
              {
                 num_cells = MAX_WCDMA_CELLS_PER_FREQ;
              }

              activity_can_run = gl1_msg_wcdma_reconfirm(
                                   l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
                                   num_cells,
                                   &l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[0],
                                   &l1_sc_wcdma_data_ptr->recon_rpt,
                                   (gl1_defs_tn_type)tn_offset,
                                   freq_idx,
                                   gas_id);
           }


           if (activity_can_run)
           {
 /*MCPM Update for WCDMA Reconf IRAT search */
              l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_WCDMA, GL1_MODE_IDLE, gas_id);
              l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_RECONFIRM,gas_id);
              irat_scheduled = TRUE;
              l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ACTIVE;
              l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_WCDMA;
           }
           else
           {
              l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_RECONFIRM,gas_id);
              /* Perform W reconf as a separate scheduled activity */
              l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_WAIT;
              l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset = freq_idx;
              l1_sc_wcdma_data_ptr->scheduled_wcdma_reconf_fn = NOT_A_FRAME_NUMBER;

              l1_sc_globals_ptr->receive_in_progress = TRUE;
              L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_IRAT,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);

              /* Make sure we stay awake to book scheduled W reconf */
              gl1_drx_require_next_tick(gas_id);

              gl1_irat_mutex_unlock( gas_id);
              return FALSE;
           }
        }
        if (irat_scheduled)
        {
          l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset = freq_idx;

            if (l1_sc_g2x_check_for_extend_srch_period(gas_id) && (l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL))
           {
             l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + 
                                                                           (l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle << 1);
           }
           else
           {
             l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + 
                                         l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle;
           }

           MSG_GERAN_HIGH_3_G("srch_in_progress %d set to TRUE, active rat %d, freq_list rat %d ",
                             l1_sc_globals_ptr->wcdma_srch_in_progress,
                             l1_sc_globals_ptr->active_irat,
                             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat);

           l1_sc_globals_ptr->active_irat = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat;
           l1_sc_globals_ptr->wcdma_srch_in_progress = TRUE;
           L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);

           l1_sc_globals_ptr->receive_in_progress = TRUE;
           L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_IRAT,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);

           list_srch_started = TRUE;

           /* CR 361192 don't go into sleep as GL1 is doing TDS IRAT MEAS*/
           gl1_drx_require_next_tick(gas_id);

           break;
        }
     } /* reconfirm_tick < page_cycle_tick && num_cells != 0 */
  }   /* End of for (freq_idx=0; freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++) */

  /* If we are not performing list searches, make sure we aren't
       reporting stale data */
  if(!try_to_schedule && l1_sc_wcdma_data_ptr->searching_active)
  {
     MSG_GERAN_HIGH_0_G("G2X searching no longer active");

     l1_sc_wcdma_data_ptr->searching_active = FALSE;

     /* clear the RR report struct */
     l1_sc_wcdma_data_ptr->rr_report.num_cells = 0;

     /* clear the energy moving avgs */
     for (freq_idx=0; freq_idx<MAX_WCDMA_FREQS; freq_idx++)
     {
        /* only clear WCDMA cells */
        if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA)
        {
           l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = 0;     /* WCDMA only */
        }
     }
  }
  /* The following checks are for providing info on when id searches
   * are not being performed.
   * l1_sc_check_priority_threshold() above replaces threshold checks, removed
   */
  if(try_to_schedule)
  {
     /* Check if id searches are due and report if they are */
     for (freq_idx=0; freq_idx<l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++)
     {
        if (l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] < l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick)
        {
           if ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT)
           {
              MSG_GERAN_HIGH_1_G("G2X: id waiting UARFCN:%d",
                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
           }
           else if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
           {
              if (l1_sc_globals_ptr->receive_in_progress)
              {
                 if (l1_sc_globals_ptr->receiving_cell )
                 {

                    MSG_GERAN_HIGH_2_G("G2X id on UARFCN:%d blocked by arfcn:%d",
                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
                            l1_sc_globals_ptr->receiving_cell->bs.arfcn.num);
                 }
                 else
                 {
                    MSG_GERAN_HIGH_1_G("G2X id on UARFCN:%d blocked",
                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
                 }
              }
              else
              {
                 MSG_GERAN_HIGH_1_G("G2X id on UARFCN:%d unscheduled",
                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
              }
           }
           break;
        } /* if (l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] < l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick) */
     }  /* for (freq_idx=0; idx<l1_sc_wcdma_data_ptr->rr_params.num_freqs; idx++) */
  }    /* if in automatic mode and the serving cell RLA_C is within
        * the required reporting range.
        */

  if ((!list_srch_started) && (l1_sc_wcdma_data_ptr->wcdma_srch.send_report))
  {
     /* Send the WCDMA report to RR */
     l1_scdrv_call_in_task(l1_sc_wcdma_send_rr_report, (char*)0, gas_id );
  }
  gl1_irat_mutex_unlock( gas_id);
  return  list_srch_started;

} /* l1_sci_wcdma_paging_block_tick */


/*===========================================================================

FUNCTION L1_SCI_IS_WCDMA_READY

DESCRIPTION
  Function called before deciding to go to sleep in idle mode.

DEPENDENCIES
  Can get rid of this function if FEATURE_GSM_G2X_TIMELINE_REDUCTION's NV control is mainlined

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sci_is_wcdma_ready( uint32 gap_to_next_activity, gas_id_t gas_id )
{
  boolean sc_irat_not_ready_to_sleep = TRUE;

  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /* If an id search is ready, and there is room in this paging cycle */

  /* @@TODO: Add a check for LTE MAX search frames here
   *
   * This most likely will require updating to involve a more dynamic check.
   */

  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
  {
     sc_irat_not_ready_to_sleep = FALSE;
  }

  if ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT) &&
       (gap_to_next_activity <= MAX_ID_SEARCH_FRAMES))
  {
     sc_irat_not_ready_to_sleep = FALSE;
  }

  return (sc_irat_not_ready_to_sleep);
}


/*===========================================================================

FUNCTION L1_SC_WCDMA_INIT

DESCRIPTION
  This function is called at initialization to let the WCDMA search code
  initialize variables.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_init( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr     = gl1_ms_switch_l1_sc_globals_store(gas_id);

  /* Called in Task context */
  MSG_GERAN_MED_1_G("G2X: SC init Fn=%d", GSTMR_GET_FN_GERAN(gas_id));

  /* function gets called at boot time, and then also at various state
   * transitions, so use first_init variable to determine which condition
   * this is.
   */
  gl1_irat_mutex_lock( gas_id);

  if (l1_sc_wcdma_data_ptr->first_init)
  {
    l1_sc_globals_ptr->g2w_shutdown_pending      = FALSE;
    l1_sc_globals_ptr->g2l_shutdown_pending      = FALSE;
#ifdef FEATURE_GSM_TDS
    l1_sc_globals_ptr->g2t_shutdown_pending      = FALSE;
#endif

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
    l1_sc_is_g2x_timeline_opt_enabled(gas_id);
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */
  }

  l1_sc_wcdma_data_ptr->first_init = FALSE;
  l1_sc_set_wcdma_list_update_inprogress(FALSE, gas_id);

#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
  if ( nv_for_search_all_w_cell )
  {
   l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found = FALSE;
   l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan = -1 ;
  }


#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */


  l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms = 2;

  l1_sc_wcdma_data_ptr->wcdma_srch.state       = GtowStateNull;
  l1_sc_wcdma_data_ptr->wcdma_srch.prev_state  = GtowStateNull;
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state  = GTOW_SRCH_STATE_NULL;
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_type   = GTOW_SRCH_TYPE_NULL;
  l1_sc_wcdma_data_ptr->wcdma_srch.send_report = FALSE;

  l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode            = FALSE;
  l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled = FALSE;
  l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick      = 0;
  /* add */
  l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick       = 0;
  l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick             = NUM_IDLE_FRAMES_PER_DED_SRCH;
  l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);

  l1_sc_wcdma_data_ptr->wcdma_srch.priority_search_threshold = gsm_l1_priority_search_threshold[ THRESH_ALWAYS ];

  l1_sc_wcdma_data_ptr->ded_srch_aborted = FALSE;

  l1_sc_wcdma_data_ptr->rr_params.num_freqs         = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs       = 0;
  l1_sc_wcdma_data_ptr->rr_params.cached_idle_freqs = 0;
  l1_sc_wcdma_data_ptr->rr_params.geran_priority    = 0;
  l1_sc_wcdma_data_ptr->rr_params.THRESH_priority_search = THRESH_ALWAYS;

  l1_sc_wcdma_data_ptr->rr_report.num_cells = 0;

  l1_sc_wcdma_data_ptr->rr_ignore_list.num_cells = 0;

  l1_sc_globals_ptr->wcdma_srch_in_progress    = FALSE;
  L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);

  l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_UNDEFINED;
  l1_sc_wcdma_data_ptr->requested_ded_id_srchs = 0;

  l1_sc_wcdma_data_ptr->rr_params.cached_idle_freqs = 0;

#ifdef FEATURE_GSM_TO_LTE
  l1_sc_wcdma_data_ptr->g2l_inhibit_next_update = FALSE;

  l1_sc_wcdma_data_ptr->g2l_idle_offset  = 2;
  l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[0] = G2L_MDSP_START_OFFSET;
  l1_sc_wcdma_data_ptr->ggap_g2l_mdsp[1] = G2L_MDSP_END_OFFSET;
  l1_sc_wcdma_data_ptr->g2l_start_frame  = 23;
  l1_sc_wcdma_data_ptr->g2l_end_frame    = 24;

  l1_sc_g2l_init(gas_id);

  l1_sc_g2l_fast_search_start(gas_id);                   /* num_L_freqs =0 so clears internals */

  l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqList = 0;
  l1_sc_wcdma_data_ptr->rr_params.bFastSearchFreqMeas = 0;
  l1_sc_wcdma_data_ptr->elevator_flag   =  FALSE;

#endif

  l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending = 0;


  g2w_active_mcpm_scenario[gas_id] = 0;
  g2l_active_mcpm_scenario[gas_id] = 0;
  g2t_active_mcpm_scenario[gas_id] = 0;
  
  l1_sc_irat_reset_g2x_abort_count(gas_id);
  l1_sc_clear_filters_stored_meas(gas_id);

  l1_sc_wcdma_data_ptr->rr_params.ForcedMeas = FORCE_MEAS_NULL;

  l1_sc_restart_cell_id_srch(gas_id);
  l1_sc_restart_ded_cell_id_srch(gas_id);

  gl1_irat_mutex_unlock( gas_id);

} /* l1_sc_wcdma_init */

/*===========================================================================

FUNCTION l1_sc_wcdma_set_mode_from_init

DESCRIPTION
  Wrapper for l1_sc_wcdma_set_mode() when called from init rather
  than from an actual MPH_UE_MODE_CHANGE_REQ msg from RR

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_sc_wcdma_set_mode_from_init(boolean gsm_only, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   /*
   ** option was to add another param to existing function, but would touch many files
   */
   l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending  = bMODE_FromInit;

   l1_sc_wcdma_set_mode(gsm_only, gas_id);

   l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending &= ~bMODE_FromInit;
}


/*===========================================================================

FUNCTION L1_SC_WCDMA_SET_MODE

DESCRIPTION
  Function called when gsm_only mode info is received from RR.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_set_mode(boolean gsm_only, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  MSG_GERAN_HIGH_1_G( "G2X set_mode gsm_only=%d", gsm_only);

  GL1_ISR_LOCK(gas_id);
  
  check_irat_active[gas_id] &= ~(1 << MODE_CHANGE_REQ_RECEIVED);
  
  if (gsm_only)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.send_report = FALSE;

    l1_sc_wcdma_data_ptr->rr_params.num_freqs    = 0;
    l1_sc_wcdma_data_ptr->rr_params.num_L_freqs  = 0;
    l1_sc_wcdma_data_ptr->rr_params.num_W_freqs  = 0;

    if (!l1_sc_wcdma_data_ptr->rr_params.gsm_only)
    {
      l1_sc_wcdma_data_ptr->rr_params.gsm_only = gsm_only;

      /* changing from automatic to gsm_only */
      if (l1_sc_wcdma_data_ptr->wcdma_srch.state == GtowStateIdle)
      {
        if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
        {
          /* a search is in progress, so don't shutdown isrs yet */

          /* Abort the search if it is not already aborting */
          if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_ABORTING)
          {
            l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
          }
          /* when searches end, the isrs will get deregistered */
        }
      }
      else
      if (l1_sc_wcdma_data_ptr->wcdma_srch.state == GtowStateDedicated)
      {
         l1_sc_wcdma_abort_ded_search(gas_id);    /* CgC ded search is active */
      }
	  
      /* Set the flag to show that GSM only req had come and we need to deactivate other RATs */
      l1_sc_wcdma_data_ptr->sc_wcdma_count = 0;
      l1_sc_wcdma_data_ptr->sc_tds_count = 0;
      l1_sc_wcdma_data_ptr->sc_lte_count = 0;
      check_irat_active[gas_id] |= (1 << MODE_CHANGE_REQ_RECEIVED);

      l1_sc_set_rat_change_active ( TRUE, gas_id );
      /*Clear stored_meas and filters also*/
      l1_sc_clear_filters_stored_meas(gas_id);
    }
  }
  else
  {
    if(  ( l1_sc_wcdma_data_ptr->wcdma_srch.state == GtowStateIdle)
       &&( l1_sc_wcdma_data_ptr->rr_params.gsm_only )
      )
    {
      uint16  idx;
      /*
      ** if we get overlapping requests send any pending confirm now, rats maybe still closing down
      */
      MSG_GERAN_HIGH_1_G( "Exit gsm_only - enable IRAT, mode_cnf_pending was =0x%x", l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending);

      l1_sc_mode_cnf_pending(FALSE, bALL_RATS_INACTIVE , gas_id );

      /* changing from gsm_only to automatic, and in idle so register isrs */
      gl1_msg_cfg_wcdma_meas( id_report_cb,
                              id_abort_cb,
                              reconfirm_report_cb,
                              reconfirm_abort_cb,
                              #ifdef FEATURE_GSM_TO_WCDMA_MSGR
                              srch_handler_done_cb,
                              #endif
                              gas_id
                            );

      /* wait for new cell update list */
      l1_sc_wcdma_data_ptr->rr_params.num_freqs    = 0;
      l1_sc_wcdma_data_ptr->rr_params.num_L_freqs  = 0;
      l1_sc_wcdma_data_ptr->rr_params.num_W_freqs  = 0;

      for (idx=0; idx<MAX_WCDMA_FREQS; idx++)
      {
        /* enable scheduling of measurements */
        l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick;
        l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick;

        /* reset the freq list as now stale */
        l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].num_cells          = 0;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].num_detected_cells = 0;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].freq_age           = 0;
        l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].rat                = RR_L1_IRAT_RAT_UNDEFINED;

      }
       l1_sc_wcdma_data_ptr->wcdma_srch.send_report = TRUE;
       //Clear out any IDs in stop list.
       l1_sc_restart_cell_id_srch(gas_id);

       l1_sc_restart_ded_cell_id_srch(gas_id);
    }
  }

  /*
  ** gsm_only  0 -> 1  shutdown rats and wait completion before sending confirm (SGLTE).
  ** gsm_only  1 -> 0  always send _cnf immediately
  */
  l1_sc_wcdma_data_ptr->rr_params.gsm_only = gsm_only;

  l1_sc_mode_cnf_pending(TRUE, 0 , gas_id );

  /*
   * Always set this so we track the GSM only status for clock disabling
   * etc. This avoids the problem with the startup not registering the state
   * correctly and also removes any conditional calls to message layer
   * functions that previously resulted in the setting being out of step
   * with the true configuration.
   */
  gl1_msg_set_gsm_only( gsm_only,gas_id );


 GL1_ISR_UNLOCK(gas_id);

} /* l1_sc_wcdma_set_mode */



/*===========================================================================

FUNCTION retrieve_reconf_cells

DESCRIPTION
  This function parses the stored_meas database and fills those cells
  which need to be reconfirmed into the requested_reconf_meas structure.

  The top 8 strongest cells will be returned as needing to be reconfirmed.

  freq_idx - frequency index used to identify which freq to use

DEPENDENCIES
  none

RETURN VALUE
  None

  reconf_cells - is filled with data of cells that have been reconfirmed.

SIDE EFFECTS
  None

===========================================================================*/

/* @@TODO: This function needs to be completely updated to support LTE
 *
 * Change the implementation to return a pointer to the filled in structure.
 */

static void retrieve_reconf_cells(uint16 freq_idx, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  i, num_cells;

  uint16  r_idx = 0;

  /* store the requested reconf_cells */
  l1_sc_wcdma_data_ptr->requested_reconf_meas.num_cells     = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
  l1_sc_wcdma_data_ptr->requested_reconf_meas.cached_UARFCN = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN;

  if( l1_sc_wcdma_data_ptr->requested_reconf_meas.num_cells > MAX_WCDMA_CELLS_PER_DED_RECONF )
  {
    l1_sc_wcdma_data_ptr->requested_reconf_meas.num_cells = MAX_WCDMA_CELLS_PER_DED_RECONF;
  }

  for( i = 0; i < l1_sc_wcdma_data_ptr->requested_reconf_meas.num_cells; i++ )
  {
    /* Make sure LTE cells are handled correctly */
    if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA  )
    {
      l1_sc_wcdma_data_ptr->requested_reconf_meas.scr_code[i] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].wcdma_cell.scr_code;
    }
#ifdef FEATURE_GSM_TDS
    else if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat == RR_L1_IRAT_RAT_UTRAN_TDD  )
    {
      l1_sc_wcdma_data_ptr->requested_reconf_meas.scr_code[i] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].tds_cell.cell_parameter_id;
    }
#endif
  }

  num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;

  for( i = 0; i < num_cells; i++ )
  {
    if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA  )
    {
    if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].wcdma_cell.RSCP_last)
    {
      if ( r_idx >= MAX_WCDMA_CELLS_PER_DED_RECONF )
      {
        break;
      }

      /* Make sure LTE cells are handled correctly */
      l1_sc_wcdma_data_ptr->reconf_cells.cells[r_idx].wcdma_cell.diversity = DIVERSITY_OFF;
      l1_sc_wcdma_data_ptr->reconf_cells.cells[r_idx].wcdma_cell.scr_code  = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].wcdma_cell.scr_code;
      l1_sc_wcdma_data_ptr->reconf_cells.cells[r_idx].wcdma_cell.PN_pos    = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].wcdma_cell.PN_pos;

      r_idx++;
      }
    }
#ifdef FEATURE_GSM_TDS
    else if ( l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat == RR_L1_IRAT_RAT_UTRAN_TDD )
    {
      if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[i].wcdma_cell.RSCP_last)
      {
        if ( r_idx >= MAX_WCDMA_CELLS_PER_DED_RECONF )
        {
          break;
        }

        /* Make sure LTE cells are handled correctly */
        l1_sc_wcdma_data_ptr->reconf_cells.cells[r_idx].tds_cell.diversity  = DIVERSITY_OFF;

        l1_sc_wcdma_data_ptr->reconf_cells.cells[r_idx].tds_cell.cell_parameter_id =
           l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].tds_cell.cell_parameter_id;

        l1_sc_wcdma_data_ptr->reconf_cells.cells[r_idx].tds_cell.cell_position =
           l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].tds_cell.cell_position;

        l1_sc_wcdma_data_ptr->reconf_cells.cells[r_idx].tds_cell.diversity =
           l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].tds_cell.diversity;

        l1_sc_wcdma_data_ptr->reconf_cells.cells[r_idx].tds_cell.sync_case =
           l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].tds_cell.sync_case;

        r_idx++;
      }
    }
#endif
  }

  l1_sc_wcdma_data_ptr->reconf_cells.num_cells = r_idx;
}

/*===========================================================================

FUNCTION L1_SC_WCDMA_ENTER_DEDI

DESCRIPTION
  This function is called at entry to Dedicated state to let the WCDMA search
  code initialize/reset variables.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_enter_dedi(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  uint16 idx;

  /* Called in Task context */
  MSG_GERAN_HIGH_0_G("G2X: SC enter_dedi");

  l1_sc_restart_ded_cell_id_srch(gas_id);

  /*Reset skip G2X ded mode flag and set temp gstm fn to invalid value while entering dedicated mode*/
  l1_sc_wcdma_data_ptr->l1_skip_g2x_ded_mode = FALSE;
  ded_gstmr_cha_fn[gas_id] = GL1_DEFS_INVALID_FN;

  if (l1_sc_wcdma_data_ptr->wcdma_srch.state == GtowStateDedicated)
  {
    /* Do not initialize twice */
    MSG_GERAN_HIGH_0_G("early exit from enter dedi");
    return;
  }

  for (idx=0; idx<MAX_WCDMA_FREQS; idx++)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.next_ded_srch_cell_offset[idx] = 0;

    /* keep only the top 8 cells */
    update_stored_measurements_for_top_8(idx, gas_id);
  }

  l1_sc_wcdma_data_ptr->rr_params.cached_idle_freqs = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_freqs   = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs = 0;

  /* reset any outstanding forced request */
  l1_sc_wcdma_data_ptr->rr_params.bForcedMeas2Index = 0;
  l1_sc_wcdma_data_ptr->rr_params.ForcedMeas        = FORCE_MEAS_NULL;


  l1_sc_wcdma_data_ptr->rr_report.num_cells   = 0;

  l1_sc_wcdma_data_ptr->wcdma_srch.state = GtowStateDedicated;
  l1_sc_wcdma_data_ptr->wcdma_srch.prev_state = GtowStateDedicated;
  l1_sc_wcdma_data_ptr->wcdma_srch.abort = FALSE;
  l1_sc_wcdma_data_ptr->wcdma_srch.ded_timeout_count = 0;
  l1_sc_wcdma_data_ptr->wcdma_srch.sdcch_meas_frame_count = L1_SC_DURATION_WCDMA_DED_SRCH;

  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
    MSG_GERAN_ERROR_3_G("G2X search not null when enter dedicated mode srch_state %d, srch_type %d, rat",
                         l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                         l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                         l1_sc_globals_ptr->active_irat);

    l1_sc_set_gtow_srch_state_null(gas_id);
  }
  
  /*FR 47064 Stop MCPM for ded/xfer mode
  ** Turn OFF clocks for XRAT when entering ded/xfer mode, to be turned ON when Gap starts */
  l1_sc_g2x_stop_mcpm_irat_ngbr(gas_id);

  /* If G2x shutdown pending, then do it now */
  l1_sc_g2x_pending_shutdown(gas_id);

#ifdef FEATURE_GSM_TDS
  l1_sc_wcdma_data_ptr->toggle_id_reconf = 0xFFFF;
#endif /* FEATURE_GSM_TDS*/
}

/*===========================================================================

FUNCTION L1_SC_WCDMA_LEAVE_DEDI

DESCRIPTION
  This function is called at exit from Dedicated state to let WCDMA search code
  abort any outstanding searches.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_leave_dedi(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  MSG_GERAN_HIGH_0_G("G2X: SC leave_dedi");

  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
    /* Any searches should have already been aborted */
    MSG_GERAN_ERROR_2_G("Leaving dedicated but srch active-%d, rat %d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state, l1_sc_globals_ptr->active_irat);

    if (l1_sc_wcdma_data_ptr->wcdma_srch.abort == FALSE)
    {
       if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_WCDMA )
       {
         /* no need to abort twice - in case we already aborted but not yet finished */
         l1_sc_wcdma_abort_ded_search(gas_id);
       }
#ifdef FEATURE_GSM_TDS
       else if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UTRAN_TDD )
       {
         l1_sc_wcdma_abort_ded_search(gas_id);
       }
#endif
#ifdef  FEATURE_GSM_TO_LTE
       else
       if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE )
       {
          /*
          ** for G2L CM always stop any measurement sequence whether in progress or not
          */
          l1_g2l_cm_abort(gas_id);
       }
       else
       {
          g2l_set_cm_inactive(gas_id);
       }
#endif
       l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
    }
  }
#ifdef FEATURE_GSM_TO_LTE
  else
  {
    g2l_set_cm_inactive(gas_id);
  }
#endif

  update_stored_measurements( l1_sc_wcdma_data_ptr->rr_params.num_freqs,
                              &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                              &l1_sc_wcdma_data_ptr->rr_params,
                              gas_id
                            );

  l1_sc_gsm_l1_set_ecio_rssi_levels( l1_sc_wcdma_data_ptr->rr_params.wcdma_QSearch,
                                     l1_sc_wcdma_data_ptr->rr_params.wcdma_QSearch,
                                     l1_sc_wcdma_data_ptr->rr_params.wcdma_EcIo_threshold_code,
                                     gas_id);

  /* Reset the 3G cells list,
   * i.e. we don't want to use the 3G list in dedicated mode to IDLE mode
   * CR 52721
   */
  l1_sc_wcdma_data_ptr->rr_params.num_freqs   = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs = 0;
  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs = 0;

  /* Do not carry Dedicated mode parameters to TBF as well */
  l1_sc_wcdma_data_ptr->rr_params.cached_idle_freqs = 0;
  l1_sc_irat_reset_g2x_abort_count(gas_id);

  l1_sc_wcdma_data_ptr->wcdma_srch.state = GtowStateNull;
  l1_sc_wcdma_data_ptr->wcdma_srch.prev_state = GtowStateNull;

  /*Reset skip G2X ded mode flag and set temp gstm fn to invalid value while leaving dedicated mode*/
  l1_sc_wcdma_data_ptr->l1_skip_g2x_ded_mode = FALSE;
  ded_gstmr_cha_fn[gas_id] = GL1_DEFS_INVALID_FN;

} /* l1_sc_wcdma_leave_dedi */

/*===========================================================================

FUNCTION L1_SC_WCDMA_LOG_FILL_SAMPLE_RAM

DESCRIPTION
  This function is called from the L1 task to log the
  Fill Sample Ram packet.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_wcdma_log_fill_sample_ram(void* unused, gas_id_t gas_id)
{
  uint16   f_off;
  uint16   c_off;
  uint16   num_cells;
  dword    frame_num;
  boolean  granted;

  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C_type*  log_ptr;

  GL1_ISR_LOCK(gas_id);

  f_off = l1_sc_wcdma_data_ptr->log_fsr_freq_off;
  c_off = l1_sc_wcdma_data_ptr->log_fsr_cell_off;
  num_cells = l1_sc_wcdma_data_ptr->log_fsr_num_cells;
  frame_num = l1_sc_wcdma_data_ptr->log_fsr_frame_num;
  granted   = l1_sc_wcdma_data_ptr->log_fsr_granted;

  GL1_ISR_UNLOCK(gas_id);

  /* pointer to log buffer */
  log_ptr =
      (LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C_type *)log_alloc( LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C,
                                                         LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C_LEN(num_cells)
                                                       );

  /* Try to allocate memory for the log packet */
  if( NULL != log_ptr )
  {
    /* Fill packet and commit it */
    log_ptr->frame_number = frame_num;
    log_ptr->granted      = (uint8) granted;
    log_ptr->UARFCN = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN,
    log_ptr->num_cells    = (uint8) num_cells;

    switch( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat )
    {
#ifdef FEATURE_GSM_TDS
      case  RR_L1_IRAT_RAT_UTRAN_TDD:
      {
        uint16 count;

        for( count=0; count < num_cells; count++ )
        {
          log_ptr->scr_code[count] = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].cell_list[c_off+count].tds_cell.cell_parameter_id;
        }
      }
      break;
#endif

      default:
      {
        uint16 count;

        for( count=0; count < num_cells; count++ )
        {
          log_ptr->scr_code[count]= l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].cell_list[c_off+count].wcdma_cell.scr_code;
        }
      }
      break;
    }

    log_commit((PACKED void *)log_ptr);

  } /* end of if( NULL != log_ptr ) */
  else
  {
    MSG_GERAN_ERROR_0_G("Null GSM_WCDMA_FILL_SAMPLE_RAM log pkt ptr ");
  }

/*lint -esym(715,unused)*/ /* unused is not used */
}

/*===========================================================================

FUNCTION L1_SC_WCDMA_DED_PENDING

DESCRIPTION
  Function called to determine if L1 has a dedicated mode search
  to schedule on the current idle frame.

DEPENDENCIES
  none

RETURN VALUE
  returns TRUE, if a wcdma measurement would like to use the current
  idle frame.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_wcdma_ded_pending(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr  = &l1_dedicated_data[gas_id];

  boolean srch_pending = FALSE;
  ARFCN_T ded_arfcn;
  ARFCN_T serving_cell;

  dBx16_T scell_avg_pwr = l1_sc_wcdma_data_ptr->serving_RLA_C;

#ifdef FEATURE_GSM_TDS
  // GL1 is waiting for INIT_CNF
  if(gl1_TDS_IRAT_INIT_CNF_pending)
  {
    MSG_GERAN_HIGH_0_G("TDS l1_sc_wcdma_ded_pending early exit init_cnf pending");
    return srch_pending;
  }
#endif

  if ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL) &&
      (l1_sc_wcdma_data_ptr->wcdma_srch.state     == GtowStateDedicated) &&
      (l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode == FALSE) &&
      (!l1_sc_wcdma_data_ptr->rr_params.gsm_only))
  {
    /*
    ** Only check if cycle time has expired to initiate a new search on a frequency.
    */
    if (l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick <= l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick)
    {
      /*
      ** get the serving cell, but may not be is ba list, not yet updated on ded HO
      */
      serving_cell = gl1_get_serving_cell_arfcn(gas_id);
      ded_arfcn    = serving_cell;  /* set as default for hopping case */

      if (l1_tskisr_blk->sub_state == L1_DED_BEFORE )
      {
        if (!l1_dedicated_data_ptr->channel_specification.channel_info_1_before.hopping_flag)
        {
              ded_arfcn = l1_dedicated_data_ptr->channel_specification.channel_info_1_before.frequency_list.channel[0];
        }
      }
      else
      if (l1_tskisr_blk->sub_state == L1_DED_AFTER )
      {
        if (!l1_dedicated_data_ptr->channel_specification.channel_info_1_after.hopping_flag)
        {
              ded_arfcn = l1_dedicated_data_ptr->channel_specification.channel_info_1_after.frequency_list.channel[0];
        }
      }

        /* As per GP041540, if in dedicated and scell is NOT in BA list
         * AND dedicated ARFCN is not BCCH Carrier AND Qsearch_C is not
         * equal to 15 then we cal disregard Qsearch_C and always search
         * for 3G ncells
         * GP041540 addresses the situation where we haven't
         * got the scell in the BA list and therefore we don't measure
         * scell power - therfore how are we supposed to decide on when
         * to trigger 3g ncell searches
         */
      if ( (!ARFCNS_EQUAL(ded_arfcn,serving_cell)) &&
           ( NULL == l1_sc_balist_get_cell_data(serving_cell,gas_id)) )   /* ==> is !known */
      {
        /*
        ** Implied threshold is Always
        */
        if(  l1_sc_wcdma_data_ptr->rr_params.qSearch_C[0] != QSEARCH_NEVER )
        {
           MSG_GERAN_MED_3_G("G2X:ded pending set ALWAYS  ded_arfcn=%d cell_afrcn=%d pwr=%d ", ded_arfcn.num , serving_cell.num, l1_sc_wcdma_data_ptr->serving_RLA_C) ;
           /*
           ** Index 0 is for W
           ** set to always, as is done for qsearch=7 in assign_search_params()
           */
           srch_pending = TRUE;

           l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[0] = MIN_LVL_dBm_x16;
           l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[0] = 0;
        }
        if(  l1_sc_wcdma_data_ptr->rr_params.qSearch_C[1] != QSEARCH_NEVER )
        {

           MSG_GERAN_MED_3_G("G2L:ded pending set ALWAYS ded_arfcn=%d cell_afrcn=%d pwr=%d ", ded_arfcn.num , serving_cell.num, l1_sc_wcdma_data_ptr->serving_RLA_C) ;
           /*
           ** Index 1 is for LTE
           ** set to always, as is done for qsearch=7 in assign_search_params()
           */
           srch_pending = TRUE;

           l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[1] = MIN_LVL_dBm_x16;
           l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[1] = 0;
        }
      }
      else
      {
         /*
         ** for ded there are now x2 Qsearch_C thresholds, W + L, to check
         */
         uint32 i;
         for (i=0;i<2;i++)  /* check both rats */
         {
             if(   (scell_avg_pwr >= l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[i] )
                 &&(scell_avg_pwr <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[i] )
                 )
             {
                MSG_GERAN_MED_3_G("G2X:ded pending Thresh  rat=%d RLA_C=%d > thresh=%d ", i , l1_sc_wcdma_data_ptr->serving_RLA_C,  l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[i]) ;
                srch_pending = TRUE;
                break;
             }
         }
         if(!srch_pending)
         {
             if(l1_sc_wcdma_data_ptr->rr_params.num_W_freqs )
             { MSG_GERAN_HIGH_3_G("G2W:NOT ded pending Thresh W RLA_C=%d < thresh min=%d max=%d", l1_sc_wcdma_data_ptr->serving_RLA_C,  l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[0], l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[0] ) ;}

             if(l1_sc_wcdma_data_ptr->rr_params.num_L_freqs)
             { MSG_GERAN_HIGH_3_G("G2L:NOT ded pending Thresh L RLA_C=%d < thresh min=%d max=%d ", l1_sc_wcdma_data_ptr->serving_RLA_C,  l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[1], l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[1] ) ;}
         }
      } /* if ded */
    } /* if ded tick */
  }  /* if */
  else
  {
    if (l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick <= l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick)
    {
       MSG_GERAN_HIGH_3_G("G2X:NOT ded pending srch_state=%d type=%d xfer=%d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state , l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,  l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode);
    }
  }
  return  srch_pending;

} /* l1_sc_wcdma_ded_pending */


/*===========================================================================

FUNCTION l1_sc_wcdma_get_ded_next_srch_freq

DESCRIPTION
  Function is used in dedicated mode to get the next UARFCN to perform a
  search on.  The next frequency selected is based on a round robin algorithm.
  There are a max of MAX_WCDMA_FREQS.   l1_sc_wcdma_data_ptr->rr_params contains the number of
  frequencies passed to L1.  and wcdma_srch indicates which frequency was
  searched last.

DEPENDENCIES
  none

RETURN VALUE
  returns next frequency offset to use for searching 0->(MAX_WCDMA_FREQS-1)

SIDE EFFECTS
  None

===========================================================================*/
uint16   l1_sc_wcdma_get_ded_next_srch_freq( boolean inhibit_update, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 f_off;
  uint16 num_freq;

  /*
  ** G2L if we already started a new freq measurement on previous idle gap, prevent update here on first call
  */

  if(inhibit_update)    /* only true if already started a G2L CM measurement seq */
  {
    MSG_GERAN_HIGH_1_G("G2L no update of search index required freq_idx=%d", l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset);

    return l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;
  }


  /* get the number of UARFCNs that RR passed to L1             */
  /* and don't go over the max as this will be used as an index */
  num_freq = l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  num_freq = num_freq > MAX_WCDMA_FREQS ? MAX_WCDMA_FREQS : num_freq;

  /* just return the next freq in the list */
  if (num_freq)
  {
    f_off = (l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset + 1) % num_freq;
  }
  else
  {
    f_off = 0;

    MSG_GERAN_ERROR_0_G("No UARFCNS indexed to search");
  }

  return  f_off;

}

/*===========================================================================

FUNCTION l1_sc_wcdma_get_ded_next_cell

DESCRIPTION
  Function is used in dedicated mode to get the next cell for a given
  UARFCN to search.


DEPENDENCIES
  none

RETURN VALUE
  returns next cell offset to use for searching 0->31

SIDE EFFECTS
  None

===========================================================================*/
static uint16   l1_sc_wcdma_get_ded_next_cell ( uint16 f_off, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 c_off;
  uint16 max_cells;

  max_cells = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells;

  if (max_cells)
  {
    c_off = l1_sc_wcdma_data_ptr->wcdma_srch.next_ded_srch_cell_offset[f_off] % max_cells;
  }
  else
  {
    c_off = 0;
    MSG_GERAN_ERROR_0_G("No cells indexed to search");
  }
  return (c_off);
}

/*===========================================================================

FUNCTION L1_SC_WCDMA_DED_READY

DESCRIPTION
  Function called when L1 has an open idle frame for a wcdma measurement.

DEPENDENCIES
  none

RETURN VALUE
  returns TRUE, if a wcdma measurement will use the idle frame.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_wcdma_ded_ready(boolean xfer_mode_in, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
#ifdef FEATURE_GSM_TDS
  idle_data_T        *l1_idle_data         = &l1_idle_data_store[gas_id];
#endif
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  boolean using_this_frame = FALSE;
  uint16  ded_meas_count=0;

  boolean xfer_mode = xfer_mode_in;

#ifdef FEATURE_GSM_TDS
  /*CR359646*/
  boolean tds_perform_acq = TRUE;
#endif /*FEATURE_GSM_TDS*/

  uint16     f_off;
  uint16     num_freqs;

  gl1_irat_mutex_lock( gas_id);
#ifdef  FEATURE_SGLTE_G2X_CONN_MODE_RESEL
/* force xfer mode so as to use priority based searches in dedicated */
  if( l1_sc_wcdma_data_ptr->wcdma_srch.use_priority_reselection_algo == G2X_DEDICATED_PRIORITY)
  {
     xfer_mode = TRUE;
  }
#endif

  num_freqs = l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  f_off     = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;

  /* No need to check Rf device as Gl1 won't perfrom IRAT
     if band registration fails later. */

  MSG_GERAN_HIGH_3_G("G2X: ded_ready xfer=%d Fn=%d ded_idx=%d", xfer_mode, GSTMR_GET_FN_GERAN(gas_id), l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset );

  if( (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
    ||(!num_freqs))
  {
    MSG_GERAN_LOW_3_G("G2X: ded_ready early exit srch_state %d, srch_type %d num_freqs=%d",
                          l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                          l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                          num_freqs);
    gl1_irat_mutex_unlock( gas_id);
    return  FALSE;
  }

#if defined(FEATURE_GSM_WTR_HOP)
  /* check if hop is active */
  if(gl1_msg_wtr_hop_get_status() != WTR_HOP_INACTIVE)
  {
      MSG_GERAN_HIGH_0_G("G2X: WTR hopping conflict, skip this measurement");
      gl1_irat_mutex_unlock( gas_id);
      return  FALSE;
  }
#endif
  if(l1_sc_wcdma_data_ptr->rr_params.gsm_only)
  {
     MSG_GERAN_HIGH_1_G("G2X: ded_ready called when mode is gsm_only=1 FN=%d" , GSTMR_GET_FN_GERAN(gas_id) );

     /* final check for back to back init/deinit pending */
     l1_sc_g2x_pending_shutdown( gas_id);

     gl1_irat_mutex_unlock( gas_id);
     return FALSE;
  }

  l1_sc_irat_set_gbta_not_allowed(TRUE, gas_id);
  if(l1_sc_irat_check_gbta_conflict(gas_id))
  {
   /* Idle sub has already activated GBTA, skip IRAT */
    l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);
    /* Skip IRAT*/
    gl1_irat_mutex_unlock( gas_id);
    return FALSE;
  }

  /*if this is the frame on which TS adjustment has happened, don't schedule the measurements*/
  if((l1_sc_wcdma_data_ptr->l1_skip_g2x_ded_mode == TRUE) && (ded_gstmr_cha_fn[gas_id] == GSTMR_GET_FN_GERAN(gas_id)))
  {
    /*Reset skip G2X ded mode flag and set temp gstm fn to invalid value*/
    l1_sc_wcdma_data_ptr->l1_skip_g2x_ded_mode = FALSE;
    ded_gstmr_cha_fn[gas_id] =GL1_DEFS_INVALID_FN;
    MSG_GERAN_HIGH_0_G("Skip this frame as TS adjustment has happened");

    /*If MCVS req of 384 MHZ already sent, then send de-boost request as well*/
    l1_sc_g2x_stop_mcpm_irat_ngbr(gas_id);
    gl1_irat_mutex_unlock( gas_id);

    return  FALSE;
  }

#ifdef FEATURE_GSM_TO_LTE
    /*
    ** check for expiry of fast search timeout on blacklisted freqs
    */
    l1_sc_g2l_fast_search_expiry(gas_id);

    /*
    ** If we have LTE cells present then configure for CM mode measurements
    */
    g2l_configure_for_cm(gas_id);

    /*
    ** Current search is LTE and has already started then advance to next gap
    */
    if( l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset].rat == RR_L1_IRAT_RAT_LTE )
    {
      if( l1_g2l_connected_restart(gas_id))
      {      
        /*if LTE MCPM nghbr START request & boost not applied 1 frame in advance, apply now*/
        l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_LTE, (uint8)xfer_mode, gas_id);

        l1_sc_cm_g2l_state( LTE_GAP_NEXT_REQ, l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset, l1_sc_wcdma_data_ptr->rr_params.use_next_freq , gas_id );

        gl1_irat_mutex_unlock( gas_id);
        /*wcdma_srch_in_progress will handle gbta.*/
        l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);
        return  TRUE;
      }
    } /* LTE CM is already in progress */
#endif /*FEATURE_GSM_TO_LTE*/

    /*
    ** start next measurement only after ded_tick has expired
    */
    if( (l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick <= l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick) &&
        (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL) &&
        (l1_sc_wcdma_data_ptr->wcdma_srch.state == GtowStateDedicated) &&
        (!l1_sc_wcdma_data_ptr->rr_params.gsm_only))
    {
      /*
      ** force the next search index to use ForcedFreqIdx
      */
      if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_ACTIVE)
      {
        f_off = l1_sc_wcdma_data_ptr->rr_params.num_freqs;       /* force reset in for loop */
      }
#ifdef  FEATURE_GSM_TO_LTE
      if(l1_sc_wcdma_data_ptr->g2l_inhibit_next_update == TRUE)   /* Lte CM is already in progress, re-start on same index */
      {

         f_off = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               /* check for PTM */
         if(grm_check_ptm_irat_sc_activity_ok(f_off ,
                                              MAX_FRAMES_FOR_IRAT_CONNECTED_MODE,
                                              CMGR_GERAN_PTM_G2W_GAP,
                                              gas_id) )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
         {
            using_this_frame = TRUE;
         }
      }
      else
#endif
      if(xfer_mode == TRUE
#ifdef FEATURE_GSM_TDS
       &&(gl1_TDS_IRAT_INIT_CNF_pending== FALSE)
#endif /*FEATURE_GSM_TDS*/
        )
      {
        /*
        ** priority based search criteria:
        ** This is for xfer only, uses 3gpp "idle" procedures
        */
        uint16  i;
        uint16  num_cells =0;
        /*
        ** this should start from last search index and wrap, so we also search the next
        */
        num_freqs = l1_sc_wcdma_data_ptr->rr_params.num_freqs;

        /*
        ** search from current location to find next search index with wrap around
        */
        for (i=0;i<num_freqs; i++)
        {
          f_off++;                    /* next index to check */

          if(f_off >= num_freqs)
          {
            /* check the forced idx is meets the search criteria */
            if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_ACTIVE)
            {
              f_off = l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx;
            }
            else
            {
              f_off=0;                 /* wrap around */
            }
          }

          if(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_WCDMA)
          {
            num_cells = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells;
          }
          else
          {
            num_cells = 1; /* In LTE/TDS we detect cells so force */
          }
          /*
          ** its not clear when we should search for TDS freq in PTM when no cells exist
          ** the original code defaulted to always search, probably in error due to
          ** feature FEATURE_GSM_TDS_PSEUDO_FAST_RETURN which is only used in DED.
          */
          if( num_cells && l1_sc_check_priority_threshold(f_off, gas_id) )
          {

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               /* check for PTM */
               if(grm_check_ptm_irat_sc_activity_ok(f_off ,
                                                    MAX_FRAMES_FOR_IRAT_CONNECTED_MODE,
                                                    CMGR_GERAN_PTM_G2W_GAP,
                                                    gas_id) )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
               {
                 /*For AEQ ON, if Next index is LTE, do BIMC boost if not done*/
                 if((l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_LTE)
                   && (gl1_get_mcvs_speed(gas_id) == MCVS_Q6_CLK_KHZ_AEQ_ON)
                   && (g2l_bimc_boost_done[gas_id] == FALSE)) 
                  {
                    if(gas_id == GERAN_ACCESS_STRATUM_ID_1)
                    { 
                      MCPM_Service_Boost_Request(MCPM_GERAN_TECH, MCPM_BIMC_BOOST_MASK, NULL, NULL, 25000);
                    }
#if defined  ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                    else if(gas_id == GERAN_ACCESS_STRATUM_ID_2)
                    {
                      MCPM_Service_Boost_Request(MCPM_GERAN1_TECH, MCPM_BIMC_BOOST_MASK, NULL, NULL, 25000);
                    }
#if defined ( FEATURE_TRIPLE_SIM )
                    else
                    {
                      MCPM_Service_Boost_Request(MCPM_GERAN2_TECH, MCPM_BIMC_BOOST_MASK, NULL, NULL, 25000);
                    }
#endif
#endif
                    g2l_bimc_boost_done[gas_id] = TRUE;
                  }/*For normal PTM search, MVCS request of next higher level is required*/

                  /*Make MCVS request of 384 MHZ if not done in last frame*/
                   l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_LTE, (uint8)xfer_mode, gas_id);
                  using_this_frame = TRUE;
                  break;
               }
          }
        } /* for */

        if(!using_this_frame)
        {
          MSG_GERAN_MED_2_G("G2W:Xfer no cells found num freqs=%d num_cells= %d", l1_sc_wcdma_data_ptr->rr_params.num_freqs, num_cells);
        }
      }
      else   /* Dedicated */
      {
        /*
        ** Threshold based search criteria:
        ** ded_pending now checks each rat threshold to see if something can be scheduled
        ** the ded_tick is also now checked here first, so no further checks needed
        ** This is for dedicated only, xfer is now handled seperately above
        */
        if (l1_sc_wcdma_ded_pending(gas_id))
        {
          uint16                i;
          dBx16_T               scell_avg_pwr = l1_sc_wcdma_data_ptr->serving_RLA_C;
         /*
         ** new functionality starts here
         ** loop from current index and find the next that meets dedicated search criteria
         */

         num_freqs = l1_sc_wcdma_data_ptr->rr_params.num_freqs;
         /*
         ** search from current location to find next search index with wrap around
         */
         for (i=0;i<num_freqs; i++)
         {
            f_off++;                    /* next index to check */

            if(f_off >= num_freqs)
            {
                 /* check the forced idx is meets the search criteria */
                 if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_ACTIVE)
                 {
                    f_off = l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx;
                 }
                 else  /* refine this so we don't get stuck if fails */
                 {
                    f_off=0;                 /* wrap around */
                 }
            }
            /*
            ** break on first that meets current threshold criteria
            */
            if(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_LTE)
            {
               if(   (scell_avg_pwr >= l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[1] )
                   &&(scell_avg_pwr <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[1] )
                 )
               {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                  /* check for DED */
                  if(grm_check_ptm_irat_sc_activity_ok(f_off ,
                                                       MAX_FRAMES_FOR_IRAT_CONNECTED_MODE,
                                                       CMGR_GERAN_PTM_G2W_GAP,
                                                       gas_id) )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
                  {
                    /*Make MCVS request of 384 MHZ if not done in last frame*/
                     l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_LTE, (uint8)xfer_mode, gas_id);
                     using_this_frame = TRUE;
                     break;
                  }
               }
            }
            else
            if(      (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_WCDMA)
                  && (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells > 0)
              )
            {
               if(   (scell_avg_pwr >= l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[0] )
                   &&(scell_avg_pwr <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[0] )
                 )
               {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                  /* check for DED */
                  if(grm_check_ptm_irat_sc_activity_ok(f_off ,
                                                       MAX_FRAMES_FOR_IRAT_CONNECTED_MODE,
                                                       CMGR_GERAN_PTM_G2W_GAP,
                                                       gas_id) )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
                  {
                     using_this_frame = TRUE;
                     break;
                  }
               }
            }
#ifdef FEATURE_GSM_TDS
            else  /* num_cells not used for TDS only reselection_freq_list used in dedicated  */
            if(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_UTRAN_TDD )
            {
               if(   (scell_avg_pwr >= l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[0] )
                   &&(scell_avg_pwr <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[0] )
                 )
               {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                  /* check for DED */
                  if(grm_check_ptm_irat_sc_activity_ok(f_off ,
                                                       MAX_FRAMES_FOR_IRAT_CONNECTED_MODE,
                                                       CMGR_GERAN_PTM_G2W_GAP,
                                                       gas_id) )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
                  {
                     using_this_frame = TRUE;
                     break;
                  }
               }
            }
#endif /*FEATURE_GSM_TDS*/
#if defined(T_RUMI_EMULATION) && defined(FEATURE_GL1_GPLT)
          /* Force G2W irrespective of service cell power level */
          using_this_frame = TRUE;
          MSG_GERAN_HIGH_1_G("GPLT: using_this_frame  %d", using_this_frame);
          break;
#endif /*T_RUMI_EMULATION || FEATURE_GL1_GPLT*/
         }  /* for */
         /*
         ** no rat met the current search criteria threshold
         */
         if(!using_this_frame)
         {
             MSG_GERAN_MED_1_G("G2W:Ded no cells found num freqs=%d", l1_sc_wcdma_data_ptr->rr_params.num_freqs);
         }
        } /* ded_pending */
      } /* else dedicated */
      /*
      ** common code: start new search ded or xfer, unchanged as it was.
      */
      if( using_this_frame )
      {
        uint16                max_cells;
        uint16                c_off      = 0;
        gl1_defs_tn_type      tn_offset  = GL1_DEFS_TN_0;
        boolean irat_scheduled           = FALSE;

         /* now update the state variable indicating current search */
        l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset = f_off;

        /*FR 47064: if MCPM IRAT clock not turned ON 1 frame in advance, try to turn on now*/
        l1_sc_g2x_start_mcpm_irat_ngbr(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat, (uint8)xfer_mode, gas_id);

        /* once CM has started for G2L use next 4 idle frames */

#ifdef FEATURE_GSM_TO_LTE

        if( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_LTE )
        {
           /* Start PTM G2L CM here, 1st of 5 idle frame gaps are used */

           /*
           ** If LTE is not yet in CM mode, returns 0 so inhibit update and use same index on next gap
           */
           boolean g2l_success = TRUE;
           if( l1_sc_cm_g2l_state( LTE_GAP_START_REQ, f_off , FALSE,gas_id ))
           {
             ded_meas_count = 1; /* not used for LTE */
             MSG_GERAN_MED_3_G("Scheduling G2L MEAS in dedicated/xfer freq %d idx=%d FN=%d",
                                 l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.euarfcn,
                                 f_off,
                                 gl1_get_FN(gas_id));
           }
           else
           {
             l1_sc_wcdma_data_ptr->g2l_inhibit_next_update = TRUE;  /* Lte CM is already in progress, re-start on same index */
             MSG_GERAN_MED_3_G("Waiting to Schedule G2L MEAS in dedicated/xfer freq %d idx=%d FN=%d",
                                 l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.euarfcn,
                                 f_off,
                                 gl1_get_FN(gas_id));
             g2l_success = FALSE;
           }
           /*G2L Success: wcdma_srch_in_progress will handle gbta.
             In both cases, set gbta_not_allowed to FALSE. */
           l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);

           if (!g2l_success)
           {
             gl1_irat_mutex_unlock( gas_id);
             return g2l_success;
           }
        }
        else  /* do WCDMA or TDS */
#endif
        {
          /*
          ** As LTE searches take 5 idle gaps, only update tick in W as the interval has already lapsed
          */
          /* schedule next time to do a search every 4th multiframe */
          l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick = l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick + NUM_IDLE_FRAMES_PER_DED_SRCH;

          /* check for the case when ded_cycle_tick rolls over */
          /* in which case just reset it now                   */
          if (l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick < NUM_IDLE_FRAMES_PER_DED_SRCH)
          {
            l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick = 0;
            l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick =  NUM_IDLE_FRAMES_PER_DED_SRCH;
          }

          c_off = l1_sc_wcdma_get_ded_next_cell ( f_off, gas_id );

          /* Bounds check to resolve Klocwork ABR errors */
          if (c_off >= MAX_WCDMA_CELLS_PER_FREQ)
          {
            MSG_GERAN_ERROR_1_G( "Too many WCDMA freqs %d clamp to max ", c_off );
            c_off = MAX_WCDMA_CELLS_PER_FREQ - 1;
          }

          max_cells = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells;
          ded_meas_count =  max_cells - c_off;

          if ( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_WCDMA)
          {
            /* only do max of 4 id searches at a time */
            if (ded_meas_count > 4)
            {
              ded_meas_count = 4;
              l1_sc_wcdma_data_ptr->wcdma_srch.next_ded_srch_cell_offset[f_off] = (c_off + 4);
            }
            else
            {
              l1_sc_wcdma_data_ptr->wcdma_srch.next_ded_srch_cell_offset[f_off] = 0;
            }
          }
#ifdef FEATURE_GSM_TDS
          else
          if (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_UTRAN_TDD)
          {
            /* Can not measure over more than 1 TDS Slot 0 in dedicated mode */
            if (ded_meas_count > 1)
            {
              ded_meas_count = 1;
              l1_sc_wcdma_data_ptr->wcdma_srch.next_ded_srch_cell_offset[f_off] = (c_off + 1);
            }
            else
            {
              l1_sc_wcdma_data_ptr->wcdma_srch.next_ded_srch_cell_offset[f_off] = 0;
            }
          }
#endif
          /* get the top 8 strongest cells and pass to driver */
          retrieve_reconf_cells(f_off, gas_id);

#ifdef FEATURE_GSM_TDS
          /* CR 340669: TL1 can measure all cells which have acquired during acquisition
          therefore remove the limitation set by ded_meas_count and c_oof
          ded_meas_count should be equal to max_cells passed by RR */
          if (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_UTRAN_TDD)
          {
            ded_meas_count = max_cells;
            l1_sc_wcdma_data_ptr->wcdma_srch.next_ded_srch_cell_offset[f_off] = 0;
          }

#endif /* FEATURE_GSM_TDS*/
          /*
           * Don't do anymore cell Id searches if we found all the cells on a UARFCN
           * stop_ded_cell_id_srch gets initialized as soon as we get new list from RR
           * so comparing with same f_off is good
           */
          if (l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[f_off] == l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN)
          {
              ded_meas_count =0;
#ifdef FEATURE_GSM_TDS
              /*CR359646*/
              if(l1_sc_wcdma_data_ptr->tds_irat_active)
              {
                tds_perform_acq = FALSE;
              }
#endif
              MSG_GERAN_HIGH_2_G("Stop Cell ID Searches on UARFCN=%d, passing 0 id cells to driver RAT = %d",
                      l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN,l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat);
          }
          /* CR 340669: TDD it may be possible that RSCP field is not valid in the ACQ_RSP(
           * RSCP is valid only if acq state is MEAS_SUCCESS).In that case num_reconf_cells
           * will be zero.*/
          if ( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_WCDMA)
          {
            if((ded_meas_count==0)&&(l1_sc_wcdma_data_ptr->reconf_cells.num_cells==0))
            {
              MSG_GERAN_MED_0_G("l1_sc_wcdma_ded_ready early exit using_this_frame =0");
              gl1_irat_mutex_unlock( gas_id);
              l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);
              return (FALSE); /* no wcdma searches, not using this frame */
            }
          }

        } /* !LTE_RAT , WCDMA or TDS only */
        /* safety check to timeout a search that is stuck */
        l1_sc_wcdma_data_ptr->wcdma_srch.ded_timeout_count = 4;

        if( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat != RR_L1_IRAT_RAT_LTE )
        {
          /* save off parameters for log packet, and log it from task context */
          l1_sc_wcdma_data_ptr->log_fsr_freq_off = f_off;
          l1_sc_wcdma_data_ptr->log_fsr_cell_off = c_off;
          l1_sc_wcdma_data_ptr->log_fsr_num_cells = ded_meas_count;
          l1_sc_wcdma_data_ptr->log_fsr_frame_num = gl1_get_FN( gas_id ) + 3;
          l1_sc_wcdma_data_ptr->log_fsr_granted = TRUE;

          l1_scdrv_call_in_task(l1_sc_wcdma_log_fill_sample_ram, (char*)0, gas_id );
        }
        else
        {
          MSG_GERAN_ERROR_0_G(" No fsr log pkt for LTE ");
        }

        if(l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode)
        {
           tn_offset = (gl1_defs_tn_type)gprs_serv_cell_meas[gas_id].lowest_tn.tn;
        }

        l1_sc_wcdma_data_ptr->requested_ded_id_srchs = ded_meas_count;

#ifdef FEATURE_GSM_TDS
        if( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_UTRAN_TDD )
        {
            /* removed the +800 lines of TDS into new func */
            irat_scheduled = l1_sc_wcdma_ded_ready_tds(tds_perform_acq, f_off , ded_meas_count, gas_id );  /* how easy was that YLB */
        }
        else
#endif
        if( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_WCDMA )
        {
          if (l1_sc_wcdma_data_ptr->wcdma_irat_active)
          {
            static gsm_l1_wcdma_cell_params_type  gl1_msg_wcdma_reconf_cells_buffer[ MAX_WCDMA_CELLS_PER_DED_RECONF ];
            uint16                                num_reconf_cells = l1_sc_wcdma_data_ptr->reconf_cells.num_cells;

            uint16  i = 0;

            MSG_GERAN_HIGH_3_G("Scheduling G2W Ded search freq %d idx=%d FN=%d",l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN, f_off, gl1_get_FN(gas_id));

            /* optimised for value 32 */
            if( 0 != ded_meas_count )
            {
              while( i < MAX_WCDMA_CELLS_PER_FREQ )
              {
                l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i]   = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].cell_list[i].wcdma_cell;
                l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+1] = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].cell_list[i+1].wcdma_cell;
                l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+2] = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].cell_list[i+2].wcdma_cell;
                l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+3] = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].cell_list[i+3].wcdma_cell;

                i += 4;
              }

              if( ded_meas_count > MAX_WCDMA_CELLS_PER_FREQ )
              {
                ded_meas_count = MAX_WCDMA_CELLS_PER_FREQ;
              }
            }

            if( 0 != num_reconf_cells )
            {
              i = 0;

              while( i < MAX_WCDMA_CELLS_PER_DED_RECONF )
              {
                gl1_msg_wcdma_reconf_cells_buffer[i]   = l1_sc_wcdma_data_ptr->reconf_cells.cells[i].wcdma_cell;
                gl1_msg_wcdma_reconf_cells_buffer[i+1] = l1_sc_wcdma_data_ptr->reconf_cells.cells[i+1].wcdma_cell;
                gl1_msg_wcdma_reconf_cells_buffer[i+2] = l1_sc_wcdma_data_ptr->reconf_cells.cells[i+2].wcdma_cell;
                gl1_msg_wcdma_reconf_cells_buffer[i+3] = l1_sc_wcdma_data_ptr->reconf_cells.cells[i+3].wcdma_cell;

                i += 4;
              }

              if( num_reconf_cells > MAX_WCDMA_CELLS_PER_DED_RECONF )
              {
                num_reconf_cells = MAX_WCDMA_CELLS_PER_DED_RECONF;
              }
            }
            /* only call driver function if we have cells in either ded_meas_count or num_reconf_cells */
            if ( (num_reconf_cells != 0) || (ded_meas_count != 0) )
            {
               gl1_msg_wcdma_ded_srch( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN,
                                       ded_meas_count,
                                       &(l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[c_off]),
                                       &l1_sc_wcdma_data_ptr->ded_srch_rpt,
                                       num_reconf_cells,
                                       &(gl1_msg_wcdma_reconf_cells_buffer[0]),
                                       &l1_sc_wcdma_data_ptr->ded_recon_rpt,
                                       tn_offset,
                                       gas_id
                                     );
               irat_scheduled = TRUE;
            }
          }
        }
        if (irat_scheduled)
        {
            l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
            l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_DEDICATED, gas_id);
            l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ACTIVE;

            l1_sc_start_wcdma_receive(gas_id);

            l1_sc_wcdma_data_ptr->wcdma_srch.sdcch_meas_frame_count = 0;

            l1_sc_wcdma_data_ptr->ded_srch_aborted = FALSE;

            /* Neighbour RAT Activity is going on */
            l1_sc_globals_ptr->wcdma_srch_in_progress = TRUE;
            L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);

            l1_sc_globals_ptr->active_irat = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat;
        }
      } /* using_this_frame == TRUE */
      else
      {
          l1_sc_globals_ptr->wcdma_srch_in_progress = FALSE;
          L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
      }
    }

  /* wcdma_srch_in_progress will take care of check_gbta_allowed if IRAT started */
  l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);

  gl1_irat_mutex_unlock( gas_id);

  return  using_this_frame;
} /* l1_sc_wcdma_ded_ready */

/*===========================================================================

FUNCTION L1_SC_WCDMA_DED_TICK

DESCRIPTION
  Function called every time SC engine gets an idle frame available.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_ded_tick(boolean srch_pending, boolean srch_scheduled, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /* increment dedicated frame counter */
#ifdef FEATURE_GSM_TDS_DEBUG
  MSG_GERAN_HIGH_2_G("G2X: ded_tick %d ded_cycle_tick %d", l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick, l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick);
#endif /*FEATURE_GSM_TDS_DEBUG*/

  l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick++;

  /* timeout searches here, and issue srch continue if needed */

  if (srch_pending && !srch_scheduled)
  {
    uint16 f_off, c_off, meas_count;

    /* If there is a search pending but it was not scheduled, log the
     * ungranted fill sample ram request.
     */
    f_off = l1_sc_wcdma_get_ded_next_srch_freq(FALSE, gas_id);

    /* Bounds check to resolve Klocwork ABR errors */
    if (f_off >= MAX_WCDMA_FREQS)
    {
      MSG_GERAN_ERROR_1_G( "Too many G2X freqs %d clamp to max ", f_off);
      f_off = MAX_WCDMA_FREQS - 1;
    }

    c_off = l1_sc_wcdma_get_ded_next_cell(f_off, gas_id);

    meas_count = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells - c_off;

    if (meas_count > 4)
    {
      meas_count = 4;
    }
#ifdef FEATURE_GSM_TDS
    if (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_UTRAN_TDD)
    {
      /* Can not measure over more than 1 TDS Slot 0 in dedicated mode */
      if (meas_count > 1)
      {
        meas_count = 1;
      }
    }
#endif

    if( l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat != RR_L1_IRAT_RAT_LTE )
    {
    /* save off parameters for log packet, and log it from task context */
    l1_sc_wcdma_data_ptr->log_fsr_freq_off = f_off;
    l1_sc_wcdma_data_ptr->log_fsr_cell_off = c_off;
    l1_sc_wcdma_data_ptr->log_fsr_num_cells = meas_count;
    l1_sc_wcdma_data_ptr->log_fsr_frame_num = gl1_get_FN( gas_id ) + 3;
    l1_sc_wcdma_data_ptr->log_fsr_granted = FALSE;

    l1_scdrv_call_in_task(l1_sc_wcdma_log_fill_sample_ram, (char*)0, gas_id );
    }
    else
    {
      MSG_GERAN_ERROR_0_G(" No fsr log pkt for LTE ");
    }
  }
  if ((!srch_scheduled) && (l1_sc_wcdma_data_ptr->wcdma_srch.ded_timeout_count > 0))
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.ded_timeout_count--;

    if (l1_sc_wcdma_data_ptr->wcdma_srch.ded_timeout_count == 0)
    {
      if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE)
      {
        MSG_GERAN_ERROR_0_G("Timing out dedicated search");
        /* Issue an abort here, just to recover */

        /*
         * We reach here if for some reason MDSP is not responding/reporting back to ARM
         * calling l1_sc_wcdma_abort_ded_search() will not help here, as aborting of driver
         * is coupled with MDSP responding back with respective SRCH_DONE interrupts.
         * For example calling l1_sc_wcdma_abort_ded_search() will not help in the following case:
         * Say, We timed out because we did not receive CMD_DONE interrupt from MDSP and
         * G2WSRCHDRV is in WAITING_FOR_CMD_DONE state (If the G2WSRCHDRV is in state WAITING_FOR_CMD_DONE
         * then abort is delayed until we receive CMD_DONE interrupt from MDSP)
         */

        l1_sc_wcdma_abort_ded_search(gas_id);
        wcdma_ded_srch_timout(gas_id);
      }
    }
  }
} /* l1_sc_wcdma_ded_tick */

/*===========================================================================

FUNCTION L1_SC_WCDMA_SDCCH_TICK

DESCRIPTION
  Function called every frame while on SDDCH.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_sdcch_tick( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  static uint16 frame_count[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };


  if (l1_sc_wcdma_data_ptr->wcdma_srch.sdcch_meas_frame_count < L1_SC_DURATION_WCDMA_DED_SRCH)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.sdcch_meas_frame_count++;

    if (l1_sc_wcdma_data_ptr->wcdma_srch.sdcch_meas_frame_count == L1_SC_DURATION_WCDMA_DED_SRCH)
    {
      /* We are on sdcch, so we had to wait for the frame tick handler
       * to finish (4 frames) before allowing the SC engine to schedule
       * some other activity.
       */

      if(l1_sc_globals_ptr->receive_in_progress == TRUE)
      {
          MSG_GERAN_ERROR_0_G("l1_sc_globals_ptr->receive_in_progress is TRUE when G2X search is about to start");
          return;
      }
    }
  }

  if (++frame_count[gas_id] > 26)
  {
    frame_count[gas_id] = 0;
    l1_sc_wcdma_ded_tick( l1_sc_wcdma_ded_pending(gas_id), FALSE, gas_id);
  }
}

/*===========================================================================

FUNCTION L1_SC_WCDMA_ABORT_DED_SEARCH

DESCRIPTION
  Function called when leaving tch (l1_sc_stop).

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_abort_ded_search(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  MSG_GERAN_HIGH_2_G( "l1_sci_tick_wcdma_abort_ded_search: state %d active rat %d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
  l1_sc_globals_ptr->active_irat);

  gl1_irat_mutex_lock( gas_id);
  /* abort any active searches */
  if ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL) && (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_CLEANUP_WAIT) && (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_ABORTING))
  {
    l1_sc_wcdma_data_ptr->ded_srch_aborted = TRUE;
    l1_sc_wcdma_data_ptr->ded_time_out = 8;

    if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_WCDMA )
    {
      if (gl1_msg_abort_wcdma_ded_srch(gas_id) == FALSE)
      {
        /* msg layer did not actually abort - clean up here instead */
        ded_srch_abort_cb(FALSE, gas_id);
      }
      else
      {
        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;
      }
    }
#ifdef FEATURE_GSM_TDS
    else if((l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_UTRAN_TDD)
            &&(l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED))
    {
      L1_send_TDS_G2T_ABORT_REQ(gas_id);
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;
      l1_sc_globals_ptr->wcdma_srch_in_progress  = TRUE;   /* prevent state change until abort completed */
      L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
    }
#endif /*FEATURE_GSM_TDS*/

#ifdef FEATURE_GSM_TO_LTE
    else if(l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE)
    {
       /*
       ** for G2L CM always stop any measurement sequence whether in progress or not
       */
       l1_g2l_cm_abort(gas_id);
    }
#endif
  }
  else
  {
    MSG_GERAN_HIGH_1_G("G2X: No abort, gap is already completing (srch state %d)", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);
  }

  gl1_irat_mutex_unlock( gas_id);
} /* l1_sc_wcdma_abort_ded_search */

/*===========================================================================

FUNCTION l1_sc_wcdma_ded_tick_abort_stuck_srch

DESCRIPTION
  Function called when leaving tch to abort any stuck searches.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_ded_tick_abort_stuck_srch(gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    if (l1_sc_wcdma_data_ptr->ded_srch_aborted)
    {
        l1_sc_wcdma_data_ptr->ded_time_out--;

        if(l1_sc_wcdma_data_ptr->ded_time_out == 0)
        {
            wcdma_ded_srch_timout(gas_id);
            l1_sc_wcdma_data_ptr->ded_srch_aborted = FALSE;
            l1_sc_wcdma_data_ptr->ded_time_out = 8;
        }
    }
}

/*===========================================================================

FUNCTION L1_SCI_START_WCDMA_SRCH

DESCRIPTION
  Function called on idle frame -1.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_start_wcdma_srch(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  gl1_set_ncell_block_start(TRUE,gas_id);

  if (l1_sc_wcdma_data_ptr->wcdma_srch.sdcch_meas_frame_count != 0)
  {
    /* We are on sdcch so we need to wait for for the frame tick handler
     * to finish up before calling l1_sci_receive_done().
     */
    /* MSG_GERAN_HIGH_0_G("Hold off on receive done", 0,0,0); */
  }
  else
  {
    /* We are on tch so we can clean up now, so next idle frame can
     * be used by the SC engine.
     */
      if(l1_sc_globals_ptr->receive_in_progress == TRUE)
      {
          MSG_GERAN_ERROR_0_G("l1_sc_globals_ptr->receive_in_progress is TRUE when G2X search is about to start");
      }
  }

  MSG_GERAN_MED_1_G("triggering block start FN=%d", gl1_get_FN(gas_id));

}

/*===========================================================================

FUNCTION l1_sc_restart_ded_cell_id_srch

DESCRIPTION
  This function will initialize stop_ded_cell_id_srch, which will trigger us
  to do cell id seraches.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_restart_ded_cell_id_srch(gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    uint16 idx;

    for (idx=0; idx<MAX_WCDMA_FREQS; idx++)
    {
      l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[idx] = 0;
    }
}


/*===========================================================================

FUNCTION l1_sc_restart_ded_specific_cell_id_srch

DESCRIPTION
  This function will initialize stop_ded_cell_id_srch, which will trigger us
  to do cell id serach on this freq index afresh.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_restart_ded_specific_cell_id_srch(uint16 freq_idx, gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[freq_idx] = 0;

}


/*===========================================================================*/
/*===========================================================================*/
/*===========================================================================*/
/*===========================================================================*/
/*===========================================================================*/
/*===============           START OF XFER MODE CODE        ==================*/
/*===========================================================================*/
/*===========================================================================*/
/*===========================================================================*/
/*===========================================================================*/
/*===========================================================================*/
/*===========================================================================*/

/*===========================================================================

FUNCTION l1_sc_wcdma_tick_idle_id_in_tbf

DESCRIPTION
  This function is called by the SCE when in GPRS transfer mode. The function
  will get called twice every 52frame multiframe, and this is key to the operation
  of the fucntion and the table of values "xfer_ticks_per_idle_tick" defined
  within.
  Using this 52multiframe tick and using the page_cycle_tick table
  the function translates this into what would have been PCH ticks, and
  increments the page_cycle_tick if required. This keeps the timing
  of the cell ID's in IDLE evenly spaced throughout transitions into transfer mode,
  and also allows scheduling of TBF suspends for cell ID's at the correct timing
  from the last IDLE ID.

  NB: Suspension for Cell ID's in transfer only take place if there are no PSC's
  defined.

DEPENDENCIES
  none

RETURN VALUE
  Boolean to allow the TBF suspension for cell ID.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_wcdma_tick_idle_id_in_tbf( uint16 transfer_tick_counter, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  boolean  start_id = FALSE;

  /* the num_scr_codes and curr_id_in_tbf_freq_offset elements of rr_params are
   * featured under FEATURE_GTOW_RESELECTION_XFER_CELL_ID
   */

  if ( (transfer_tick_counter % xfer_ticks_per_idle_tick[l1_sc_wcdma_data_ptr->wcdma_srch.pg_mfrms-2]) == 0 )
  {
    uint16  index;

    /* Increment the page_cycle_tick value */
    l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick++;

      /* See if any ID's need to be scheduled on this tick. ID's via tbf suspension */
      /* only happen if no PSC's are defined.                                       */
      for (index=0; index<l1_sc_wcdma_data_ptr->rr_params.num_freqs; index++)
      {
        if (  (l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[index] <= l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick)
           && (l1_sc_wcdma_data_ptr->rr_params.freq_list[index].num_scr_codes == 0)
           &&  l1_sc_check_priority_threshold (index, gas_id)
           )
        {
          switch( l1_sc_wcdma_data_ptr->rr_params.freq_list[index].rat )
          {
          case RR_L1_IRAT_RAT_LTE:

            MSG_GERAN_MED_1_G(" Not scheduling LTE earfcn %d search during TBF suspension ",l1_sc_wcdma_data_ptr->rr_params.freq_list[index].arfcn.UARFCN );

            break;
#ifdef FEATURE_GSM_TDS
      case RR_L1_IRAT_RAT_UTRAN_TDD:
           /* TBF suspension should always be scheduled for TDS search
            * as only the simplest TS allocations would give enough time
            */
           /* For G2T in xfer, don't want to invoke TBF suspension mechanism as we
            * don't do anything for that at the moment
            */
            MSG_GERAN_HIGH_0_G("Not scheduling G2T during TBF suspension");
           break;
#endif
      case RR_L1_IRAT_RAT_WCDMA:
            {
              l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_in_tbf_freq_offset = index;
              start_id = TRUE;
              MSG_GERAN_MED_2_G("G2W scheduling WCDMA ID search for earfcn %d during TBF suspension idx=%d",l1_sc_wcdma_data_ptr->rr_params.freq_list[index].arfcn.UARFCN ,index );

            }
            break;

          default:
              MSG_GERAN_ERROR_1_G(" RR_L1_IRAT_RAT_UNDEFINED  index %u ",index);
              continue;
          } /* switch */
        }
        /*Break the loop to schedule the first index being searched*/
        if(start_id == TRUE)
        break;
      }  /* End of for (index=0; index<l1_sc_wcdma_data_ptr->rr_params.num_freqs; index++) */
  }

  return  start_id;
}

/*===========================================================================

FUNCTION l1_sc_wcdma_abort_id_tbf

DESCRIPTION
  This function is called to abort any ongoing ID searches in TBF

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_abort_id_tbf(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

    MSG_GERAN_HIGH_3_G( "l1_sci_tick_wcdma_abort_id_tbf: state %d, rcv %d active rat %d",
                         l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                         l1_sc_globals_ptr->receive_in_progress,
                         l1_sc_globals_ptr->active_irat );

    if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
    {
       if( gl1_msg_abort_wcdma_identify(gas_id))
       {
          l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ABORTING;
       }
       else
       {
          l1_sc_set_gtow_srch_state_null(gas_id);
       }
    }
}


/*===========================================================================

FUNCTION l1_sc_wcdma_start_id_in_tbf

DESCRIPTION
  This function will decide whether to start an ID search or not

DEPENDENCIES
  none

RETURN VALUE
  TRUE, if we want to start IDserach
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_wcdma_start_id_in_tbf(gas_id_t gas_id)
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    uint16  freq_idx   = 0;
    boolean id_started = FALSE;

    for(freq_idx=0; freq_idx<l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++)
    {
      if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
      {
        if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_scr_codes == 0)
        {
            id_started=TRUE;
        }
      }
    }

    MSG_GERAN_HIGH_2_G("G2W: Started=%d ID search in TBF idx=%d", id_started, freq_idx);

    return id_started;
}



/*===========================================================================

FUNCTION l1_sc_wcdma_receive_id_in_tbf

DESCRIPTION
  This function will trigger IDsearches in TBF if number of cells are not
  specified in UPDATE_LIST
DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_receive_id_in_tbf( void ( *done )( void*, gas_id_t gas_id ), gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr  = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr     = gl1_ms_switch_l1_sc_globals_store(gas_id);

  uint16 freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_in_tbf_freq_offset;
  /* Called in Task context */

    /* make sure SCE is in active state before starting new IRAT activity */
   if ( !l1_sc_globals_ptr->active )
   {
     MSG_GERAN_ERROR_1_G("called l1_sc_wcdma_receive() with SCE is not active :%d" , l1_sc_globals_ptr->active);
     return;
   }

  if( l1_sc_globals_ptr->receive_in_progress )
  {
     MSG_GERAN_ERROR_1_G("called l1_sc_wcdma_receive_id_in_tbf() with rx in progress  idx=%d" , freq_idx);
     return;
  }

  gl1_irat_mutex_lock( gas_id);

  MSG_GERAN_LOW_0_G("l1_sc_wcdma_receive");

  if(  ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL )
     &&( l1_sc_globals_ptr->wcdma_srch_in_progress )
    )
  {
    /********************************************************************/
    /* If the search state is not GTOW_SRCH_STATE_NULL then either a    */
    /* search is still running or something has got stuck. To recover   */
    /* check to see if the drivers think a serach is still in progress. */
    /* If there is then abort this and then the search state will return*/
    /* to GTOW_SRCH_STATE_NULL. If the drivers think that there is no   */
    /* search in progress just move the state and carry on with this ID */
    /********************************************************************/
     MSG_GERAN_ERROR_3_G("!!receive_id_in_tbf: srch_state = %d srch_type = %d, idx=%d",
                           l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                           l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                           freq_idx);

    gl1_msg_cfg_wcdma_meas( id_in_tbf_report_cb,
                            id_in_tbf_abort_cb,
                            reconfirm_report_cb,
                            reconfirm_abort_cb,
                            #ifdef FEATURE_GSM_TO_WCDMA_MSGR
                            srch_handler_done_cb,
                            #endif
                            gas_id );

    l1_sc_wcdma_abort_id_tbf(gas_id);
    gl1_irat_mutex_unlock( gas_id);
    return;

  }
  else if ( l1_sc_wcdma_data_ptr->rr_params.gsm_only )
  {
    MSG_GERAN_ERROR_0_G("!!Mode is GSM only for id in tbf");
    gl1_irat_mutex_unlock( gas_id);
    return;
  }
   l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled = FALSE ;
   gl1_msg_cfg_wcdma_meas( id_in_tbf_report_cb,
                          id_in_tbf_abort_cb,
                          reconfirm_report_cb,
                          reconfirm_abort_cb,
                          #ifdef FEATURE_GSM_TO_WCDMA_MSGR
                          srch_handler_done_cb,
                          #endif
                          gas_id );

  /* @@TODO: Change the checks below
   *
   * When the reslection algorithm is based on priority search
   * then the priority search threshold requirement overrides the
   * Qsearch_I.
   */

  if(l1_sc_check_priority_threshold(freq_idx, gas_id))
  {
    if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[ freq_idx].rat )
    {
      l1_sc_wcdma_data_ptr->wcdma_srch.abort = FALSE;
      l1_sc_wcdma_data_ptr->wcdma_srch.done  = done;
      l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_ID_IN_TBF, gas_id);
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_WAIT;

      /*WCDMA MCPM nghbr START request apply now*/
      l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_WCDMA, (uint8)l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode, gas_id);

      l1_sc_globals_ptr->receive_in_progress = TRUE;
      l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_WCDMA;
      L1_LOG_VAR(l1_sc_globals_ptr->receive_in_progress ,L1_FNAME_L1_SC_IRAT,L1_VAR_RECEIVE_IN_PROGRESS,gas_id);

     /*Update the IDLE ID tick for this chosen freq so it gets spaced out in IDLE */
     if (l1_sc_g2x_check_for_extend_srch_period(gas_id))
     {
        l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + (l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle << 1);
     }
     else
     {
        l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + l1_sc_wcdma_data_ptr->wcdma_srch.id_cycle;
     }

      MSG_GERAN_HIGH_3_G( "G2W: l1_sc_wcdma_receive_id_in_tbf: state %d, rcv %d idx=%d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                                                                                         l1_sc_globals_ptr->receive_in_progress,
                                                                                         freq_idx );
    }
    else
    {
      MSG_GERAN_ERROR_2_G( " G2X search not supported for RAT %d in TBF suspension idx=%d",
                                                                                  l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat,
                                                                                  freq_idx );
    } /* else if RAT is not WCDMA */
  }   /* if RLA_C is within range */

   gl1_irat_mutex_unlock( gas_id);

   if (l1_sc_globals_ptr->receive_in_progress == TRUE)
   {
      gl1_drx_require_next_tick(gas_id);
   }

} /* l1_sc_wcdma_receive_id_in_tbf */

/*===========================================================================

FUNCTION l1_sci_tick_wcdma_id_in_tbf

DESCRIPTION
  This function is called from isr level to manage any active WCDMA
  searches during TBF suspension.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_tick_wcdma_id_in_tbf( uint32 available_frames, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T     *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  uint16  freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_in_tbf_freq_offset;

  gl1_irat_mutex_lock( gas_id);

  if (l1_sc_wcdma_data_ptr->wcdma_srch.abort)
  {
    MSG_GERAN_HIGH_3_G( "l1_sc_sm_irat: ABORTING id_in_tbf! State %d, active RAT %d, CM=0x%x",
                         l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
                         l1_sc_globals_ptr->active_irat,
                         l1_sc_wcdma_data_ptr->lte_cm_irat_active);

    l1_sci_abort_wcdma_tick(freq_idx, gas_id);
  }

  switch ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state )
  {
    case GTOW_SRCH_STATE_WAIT:
      {
        if ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA)
        {
           if(l1_sc_wcdma_data_ptr->wcdma_irat_active)
           {
             if (l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
             {
                gl1_msg_wcdma_identify( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
                                        &l1_sc_wcdma_data_ptr->id_rpt,
                                        gas_id
                                       );

                gl1_set_ncell_block_start(TRUE,gas_id);

                l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_ID_IN_TBF, gas_id);
                l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ACTIVE;
                /* Set Active RAT to WCDMA  */
                l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_WCDMA;
                l1_sc_globals_ptr->wcdma_srch_in_progress   = TRUE;
                L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
             }
             else
             {
#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
                MSG_GERAN_HIGH_0_G("l1_sci_tick_wcdma_id_in_tbf: schedule g2w in non-L1SCModeGprsTrans mode");
                
                if(l1_sc_g2x_timeline_opt_enable[gas_id])
                {
                  l1_sc_wcdma_data_ptr->wcdma_srch.srch_type = GTOW_SRCH_TYPE_ID_IN_TBF;
                }
#endif
                l1_sc_g2w_id_wait_state(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
                if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_SCHEDULE_WCDMA)
                {
                   l1_sc_wcdma_data_ptr->wcdma_srch.srch_type  = GTOW_SRCH_TYPE_ID_IN_TBF;
                   l1_sc_g2w_schedule(l1_sc_wcdma_data_ptr, l1_sc_globals_ptr, gas_id);
                }
             }
           }
           else
           {
              MSG_GERAN_ERROR_0_G(" G2W not active for search in TBF suspension ");
              l1_sc_set_gtow_srch_state_null(gas_id);
           }
        }
#ifdef  FEATURE_GSM_TO_LTE
        else if( l1_sc_wcdma_data_ptr->rr_params.freq_list[ freq_idx ].rat == RR_L1_IRAT_RAT_LTE )
        {
           MSG_GERAN_ERROR_0_G(" G2L search not supported in TBF suspension ");
           l1_sc_set_gtow_srch_state_null(gas_id);
        }
#endif
#ifdef FEATURE_GSM_TDS
        else if (l1_sc_wcdma_data_ptr->rr_params.freq_list[ l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset ].rat == RR_L1_IRAT_RAT_UTRAN_TDD  )
        {
           MSG_GERAN_ERROR_0_G(" G2T search not supported in TBF suspension ");
           l1_sc_set_gtow_srch_state_null(gas_id);
        }
#endif
        gl1_drx_require_next_tick(gas_id);
      }
      break;

    case GTOW_SRCH_STATE_ACTIVE:
    case GTOW_SRCH_STATE_ABORTING:
    case GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES:
      /* Make sure we don't go to sleep */
      gl1_drx_require_next_tick(gas_id);
      break;

    case GTOW_SRCH_STATE_NULL:
       /* If G2x shutdown pending, then do it now */
       l1_sc_g2x_pending_shutdown(gas_id);
    break;

    default:
        MSG_GERAN_HIGH_0_G( "l1_sci_tick_wcdma_id_in_tbf: unknown id state" );
        l1_sc_set_gtow_srch_state_null(gas_id);
      break;
  }
  gl1_irat_mutex_unlock( gas_id);
  return;
} /* l1_sci_tick_wcdma_id_in_tbf */


/*===========================================================================

FUNCTION L1_SC_WCDMA_ENTER_XFER

DESCRIPTION
  Function called when entering packet transfer mode from packet idle

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_enter_xfer(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  uint16 f_idx;

  f_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;

  gl1_irat_mutex_lock( gas_id);
  if (l1_sc_wcdma_data_ptr->wcdma_srch.abort)
  {
    l1_sci_abort_wcdma_tick(f_idx, gas_id);
  }
  gl1_irat_mutex_unlock( gas_id);

  l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode = TRUE;

  /* if we are not currently aborting an idle mode search */
  if ((!l1_sc_wcdma_data_ptr->rr_params.gsm_only) &&
     (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL))
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled = TRUE;

    /* may be a G2X abort ongoing - only reset flag if there isn't*/
    l1_sc_globals_ptr->wcdma_srch_in_progress = FALSE;
    L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
  }

  MSG_GERAN_HIGH_3_G("G2X: SC enter_xfer: num_freqs=%d, L=%d W|T=%d",
                                  l1_sc_wcdma_data_ptr->rr_params.num_freqs ,
                                  l1_sc_wcdma_data_ptr->rr_params.num_L_freqs,
                                  l1_sc_wcdma_data_ptr->rr_params.num_W_freqs);

  /*
  ** If we have LTE cells present then configure for CM mode measurements
  */
  g2l_configure_for_cm(gas_id);

  if (l1_sc_wcdma_data_ptr->wcdma_srch.state == GtowStateDedicated)
  {
    /* Do not initialize twice */
    MSG_GERAN_HIGH_0_G("early exit from enter xfer");
    return;
  }

  /*
   * leave the ded_tick count, as resetting it
   * had an adverse effect, as tick never expired on short transitions to xfer/idle
   * so that there's more chance of getting a measurement before we
   * dip out of transfer again.
   */
  l1_sc_wcdma_data_ptr->wcdma_srch.state = GtowStateDedicated;
  l1_sc_wcdma_data_ptr->wcdma_srch.ded_timeout_count = 0;

  /* l1_sc_wcdma_data_ptr->wcdma_srch.abort = FALSE; */
  l1_sc_wcdma_data_ptr->rr_report.num_cells = 0;

  /*FR 47064 Stop MCPM for ded/xfer mode
  ** Turn OFF clocks for XRAT when entering ded/xfer mode, to be turned ON when Gap starts */
  l1_sc_g2x_stop_mcpm_irat_ngbr(gas_id);

  /* If G2x shutdown pending, then do it now */
  l1_sc_g2x_pending_shutdown(gas_id);

  /*
   * Retrieve the original WCDMA list in rr_params in case it has
   * been cleared on idle mode (default behaviour for dedicated mode
   * implementation, as the WCDMA neighbour list is resent once
   * dedicated is entered).
   */

  /*
   * Merge in any older data from Packet Idle that may be useful during
   * Transfer, as per the Dual Mode Cell Reslection Feature Overview (3.3.3):
   *
   * "When entering packet transfer mode. L1 shall not forget the PN
   *  positions of the UTRAN cells it considers known and have a
   *  valid moving average from idle mode."
   *
   *
   * Copy in the WCDMA PSC and Diversity parameters obtained from
   * idle for "frequency only" parameters
   */

  for (  f_idx = 0;
         f_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs;
       ++f_idx     )
  {

    if(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].num_cells == 0)
    {
      /*
       * ASSUMPTION :
       *
       * In the case where there is a "frequency only" 3G neighbour cell list
       * from we can consider *all* previously detected cells in idle mode as
       * "known" and use them in transfer mode.
       */

      uint16 c_idx;

      /*
       * Tag this frequency as a special case so that we can "undo" the merge
       * on return to Packet Idle and let the Cell ID process discover any
       * new cells.
       */

      l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].freq_only_data = TRUE;
      l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].num_cells = l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells;

      for(  c_idx = 0;
            c_idx < l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells;
          ++c_idx       )
      {
#ifdef  FEATURE_GSM_TO_LTE
        if( RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].rat )
        {
          l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].lte_cell.cell_id =
                l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].lte_cell.cell_id;

          l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].lte_cell.cp_type =
                l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].lte_cell.cp_type;

          l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].lte_cell.state =
                l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].lte_cell.state;
        }
        else
#endif /* FEATURE_GSM_TO_LTE */
#ifdef FEATURE_GSM_TDS
        /* CR 342232 Handling of TDD in Packet Transfer */
        if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].rat )
        {
          l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].tds_cell =
                l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].tds_cell;
        }
        else
#endif
        if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].rat )
        {
          l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].wcdma_cell.scr_code =
                l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].wcdma_cell.scr_code;

          l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].wcdma_cell.diversity =
                l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].wcdma_cell.diversity;

          /* PN_pos in transfer in chipx8 */

          l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].wcdma_cell.PN_pos =
                l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].wcdma_cell.PN_pos;
        }
      } /* End of for( c_idx = 0; c_idx < l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells; ++c_idx ) */
    }
    else
    {
      /*
       * Normal case where UARFCN + PSCs are given for 3G neighbour cells -
       * Merge all PN Positions for cells that are already known. Any other
       * can be discovered during transfer.
       */

      uint16 c_idx;

      for(  c_idx = 0;
            c_idx < l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].num_cells;
          ++c_idx           )
      {
        uint16 find_idx;
        boolean found = FALSE;

#ifdef  FEATURE_GSM_TO_LTE
      if( RR_L1_IRAT_RAT_LTE == l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].rat )
      {
          uint16  cell_id = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].lte_cell.cell_id;

          for(  find_idx = 0;
                find_idx < l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells;
              ++find_idx      )
          {

            if( cell_id == l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[find_idx].lte_cell.cell_id )
            {
              l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].lte_cell.state =
                 l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[find_idx].lte_cell.state;

              found = TRUE;
              break;
            }
          } /* for (find_idx ... */
      }
      else
#endif /* FEATURE_GSM_TO_LTE */
#ifdef FEATURE_GSM_TDS
      /* CR 342232 Handling of TDD in Packet Transfer */
      if( RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].rat )
      {
          uint16  cell_id = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].tds_cell.cell_parameter_id;

          for(  find_idx = 0;
                find_idx < l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells;
              ++find_idx      )
          {

            if( cell_id == l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[find_idx].tds_cell.cell_parameter_id )
            {
              l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].tds_cell =
                 l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[find_idx].tds_cell;

              found = TRUE;
              break;
            }
          } /* for (find_idx ... */
      }
      else
#endif
      if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].rat )
      {
        /*
         * Attempt to find the Cell's PN position in the stored_meas
         * database from idle mode.
         */

          uint16 temp_scr_code = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].wcdma_cell.scr_code;

          for(  find_idx = 0;
                find_idx < l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells;
              ++find_idx      )
          {

            if(temp_scr_code == l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[find_idx].wcdma_cell.scr_code )
            {
               l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].wcdma_cell.PN_pos =
                  l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[find_idx].wcdma_cell.PN_pos;

              found = TRUE;
              break;
            }
          } /* for (find_idx ... */

          if(found == FALSE)
          {
             l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].cell_list[c_idx].wcdma_cell.PN_pos = PN_POSITION_UNKNOWN;
          }
        }
      } /* for (c_idx ... */
    } /* else */

    /* now keep only the top 8 cells */
    update_stored_measurements_for_top_8((uint16)f_idx, gas_id);

  } /* for (f_idx ... */

  l1_sc_wcdma_data_ptr->l1_skip_g2x_pkt_txfr = FALSE;

#ifdef FEATURE_GSM_TDS
  l1_sc_wcdma_data_ptr->toggle_id_reconf = 0xFFFF;
#endif /* FEATURE_GSM_TDS*/

  l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);
  return;
} /* l1_sc_wcdma_enter_xfer */

/*===========================================================================

FUNCTION L1_SC_WCDMA_LEAVE_XFER

DESCRIPTION
  Function called when leaving packet transfer mode

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_leave_xfer(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr     = gl1_ms_switch_l1_sc_globals_store(gas_id);


  uint16 f_idx;

  MSG_GERAN_HIGH_0_G("G2X: SC leave_xfer");

  l1_sc_irat_reset_g2x_abort_count(gas_id);
  l1_sc_wcdma_data_ptr->wcdma_srch.state = GtowStateNull;

  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
    /* Search must be aborting, so wait to tell the srch driver that
     * we are no longer in dedicated mode until after the abort completes
     */
    MSG_GERAN_HIGH_3_G("Leaving xfer but g2x srch active %d, rat %d type %d",l1_sc_wcdma_data_ptr->wcdma_srch.srch_state, l1_sc_globals_ptr->active_irat,l1_sc_wcdma_data_ptr->wcdma_srch.srch_type);

    /* Scenario: G2L measurement is going in xfer mode. Gl1 decides to leave xfer mode but TIMED_SRCH_MEAS_CNF
       hasn't received from ML1. If GL1 calls here l1_sc_cm_g2l_state( LTE_GAP_ABORT, 0 , TRUE, gas_id );
      like dedicated mode then on receiving CNF, GL1 will clear rr_cell_list and stored_result which is not required );
      in xfer mode as GL1 uses same list in xfer and idle mode and retains result.
      Instead of that on scheduling G2L measurement in Idle mode, reset CM mode specific parameters */

    /* In prev CM, on entering idle mode, GL1 calls DEINIT and on CNF reset CM variables.
       GL1 is leaving xfer mode and CNF hasn't received yet. Set CM_STOP using below function call which will
       reset CM varaiables when CNF is received*/

#ifdef FEATURE_GSM_TO_LTE
    if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE )
    {
      /*
      ** for G2L CM always stop any measurement sequence whether in progress or not
      */
      l1_g2l_cm_abort(gas_id);
    }
    else
    {
      g2l_set_cm_inactive(gas_id);
    }
#endif
  }
  else
  {
#ifdef FEATURE_GSM_TO_LTE
    g2l_set_cm_inactive(gas_id);
#endif
    /* Let driver know we are no longer doing dedicated mode searches. */
    l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled = FALSE;

  }
  /*
   * Remove any cells that were introduced at the transition to transfer
   * so that Cell ID can discover any new ones.
   */
  for(  f_idx = 0;
        f_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs;
      ++f_idx      )
  {
    if(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].freq_only_data == TRUE)
    {
      l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].freq_only_data = FALSE;
      l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].num_cells = 0;
      l1_sc_wcdma_data_ptr->rr_params.freq_list[f_idx].freq_age = 0;
    }
  }
  /*
   * Now entering non-DRX mode, so cache any PN_pos values that we
   * gathered during transfer for use by the RECONF driver in non-DRX mode
   *
   * The PN_positions have already been saved in chipx8 format, and
   * the "last" EcNo and RSCP will be non-zero if the cell has been
   * sucessfully synced on the last attempt during xfer.
   */
  for (f_idx=0; f_idx<MAX_WCDMA_FREQS; f_idx++)
  {
    l1_sc_wcdma_data_ptr->stored_meas[f_idx].cached_xfer_cells = l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells;
  }

  l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode = FALSE;
  l1_sc_wcdma_data_ptr->l1_skip_g2x_pkt_txfr = FALSE;
  l1_sc_irat_set_gbta_not_allowed(FALSE, gas_id);

  /*
  ** for Xfer/Idle we use the same cell list so may not get an update on mode change, whereas in dedicated we get a new list
  ** update for next idle search index.
  */
  l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;

  return;
} /* l1_sc_wcdma_leave_xfer */

/*===========================================================================

FUNCTION L1_SC_WCDMA_XFER_TICK

DESCRIPTION
  Function called every time SC engine gets an idle frame available.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_xfer_tick( boolean  srch_pending,
                            boolean  srch_scheduled, gas_id_t gas_id
                          )
{
  l1_sc_wcdma_ded_tick( srch_pending,
                        srch_scheduled
                      , gas_id);
}



/*===========================================================================

FUNCTION L1_SC_WCDMA_XFER_READY

DESCRIPTION
  Function called when L1 has an open idle frame for a wcdma measurement.

  Calls l1_sc_wcdma_ded_ready( ).

  The higher_priority boolean refers to clashes of WCDMA search and GSM
  acquisition and SCH reconfirm.

DEPENDENCIES
  none

RETURN VALUE
  returns TRUE, if a wcdma measurement will use the idle frame.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_wcdma_xfer_ready(boolean high_priority_only, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr     = gl1_ms_switch_l1_sc_globals_store(gas_id);
  boolean search_started = FALSE;

  /* No need to check Rf device as Gl1 won't perfrom IRAT
     if band registration fails later. */

  /* exit if sce rx is already in progress */
  if(l1_sc_globals_ptr->receive_in_progress)
  {
    l1_sci_rcv_state_debug_msg(gas_id);
    MSG_GERAN_HIGH_0_G("don't schedule IRAT - rx in progress");
    return FALSE;
  }

  /* In PTM, GL1 schedules G2L/G2T at FNmod26 =22,
  ** if TS is reconfigured in Blk 21-24, don't schedule G2T or G2L
  ** as this corrupts the snapshot timestamps used by the other techs.
  */
  if((l1_sc_globals_ptr->mode == L1SCModeGprsTrans) && (l1_sc_wcdma_data_ptr->l1_skip_g2x_pkt_txfr))
  {
    if(l1_sc_wcdma_data_ptr->wcdma_irat_active)
    {
       MSG_GERAN_HIGH_0_G("G2W: TS reconfigured, don't schedule IRAT");
       return FALSE;
    }
#ifdef FEATURE_GSM_TDS
    if(l1_sc_wcdma_data_ptr->tds_irat_active)
    {
      MSG_GERAN_HIGH_0_G("G2T: TS reconfigured, don't schedule IRAT");
      return FALSE;
    }
#endif

#ifdef FEATURE_GSM_TO_LTE
    if(l1_sc_wcdma_data_ptr->lte_irat_active)
    {
       MSG_GERAN_HIGH_0_G("G2L: TS reconfigured, don't schedule IRAT");
       return FALSE;
    }
#endif
  }

   /* If we entered xfer mode, but were in the process of aborting an idle
   * mode srch, the driver would not have been enabled (to allow the srch
   * driver to abort the search). Before issuing dedicated search make
   * sure driver knows we are in dedicated.
   */
  if (!l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled = TRUE;
  }


  /* CHECK1:that there is not a pending TBF suspension for an ID search.
  This can be seen when trans.wcdma_cell_id_go is set.If set then
  don't allow the start of an IDLE frame search as this may cause a
  clash between the two tasks.*/
  if ( trans[gas_id].wcdma_cell_id_go == FALSE )
  {
#ifdef FEATURE_GPRS_GBTA
      MSG_GERAN_HIGH_3_G("dg:g+g active %d, other G client has lock %d other G client missed last page %d",
                         GBTA_ALLOWED_CAN_BE_SET(gas_id), !gl1_check_any_other_client_has_lock(gas_id),
                         OTHER_SUB_MISSED_LAST_PAGE(gas_id) );

     if ( (GBTA_ALLOWED_CAN_BE_SET(gas_id)) && !gl1_check_any_other_client_has_lock(gas_id)   )
     {
        if (OTHER_SUB_MISSED_LAST_PAGE(gas_id))
        {
           search_started = FALSE;
           MSG_GERAN_HIGH_0_G("dg:Do not allow IRAT search if page on other SUB was missed");
        }
        else
#endif
        {
          search_started = l1_sc_wcdma_ded_ready(high_priority_only,gas_id);
        }
#ifdef FEATURE_GPRS_GBTA
     }/*Regression of CR569122*/
     /*If all the other subs are in inactive state, try to schedule IRAT*/
      else if((gl1_check_all_other_subs_l1_state(gas_id, L1_INACTIVE_MODE) ) ||(gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE))
     {
       search_started = l1_sc_wcdma_ded_ready(high_priority_only,gas_id);
     }
#endif

  }
  else
  {
    MSG_GERAN_MED_0_G("IDLE frame iRAT ded search forbidden due to iRAT TBF susp");

    /* CHECK2: As an ID in TBF is now pending, check to make sure that there
    is not an IDLE frame search still active. If there is then Abort this now to
    prevent any lock-up due to unexpected state transitions. Also check that it
    is not already in the process of aborting */
    if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE
       && l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED
       && (!l1_sc_wcdma_data_ptr->ded_srch_aborted) )
    {
      MSG_GERAN_HIGH_0_G("Terminate active IDLE frame search");

      l1_sc_wcdma_abort_xfer_search(gas_id);
    }
  }
  MSG_GERAN_HIGH_1_G("G2X SC xfer_ready returning %d",search_started);
  return  search_started;


} /* l1_sc_wcdma_xfer_ready */



/*===========================================================================

FUNCTION L1_SC_WCDMA_XFER_PENDING

DESCRIPTION
  Function called to determine if L1 has a packet transfer mode search
  to schedule on the current idle frame.

DEPENDENCIES
  none

RETURN VALUE
  returns TRUE, if a wcdma measurement would like to use the current
  idle frame.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_wcdma_xfer_pending(gas_id_t gas_id)
{
  return l1_sc_wcdma_ded_pending(gas_id);
}


/*===========================================================================

FUNCTION L1_SC_WCDMA_ABORT_XFER_SEARCH

DESCRIPTION
  Aborts all active WCDMA searches during packet transfer mode

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_abort_xfer_search( gas_id_t gas_id )
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  MSG_GERAN_HIGH_3_G("G2X: abort_xfer_search: x-rat %d, srch_state %d, srch_type %d",
          l1_sc_globals_ptr->active_irat,
          l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
          l1_sc_wcdma_data_ptr->wcdma_srch.srch_type);

  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID_IN_TBF)
  {
     l1_sc_wcdma_abort_id_tbf(gas_id);
  }
  else
  {
     l1_sc_wcdma_abort_ded_search(gas_id);
  }

}



#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION l1_sc_wcdma_init_gsm_only_mode

DESCRIPTION
  Function used to set the gsm_only flag up upon a startup.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_wcdma_init_gsm_only_mode(boolean gsm_only, gas_id_t gas_id)
{
  int as_index;

  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  l1_sc_wcdma_data_ptr->rr_params.gsm_only = gsm_only;

  if ( gl1_msg_get_multi_sim_mode() )
  {
    /* print the current setting for each data space */
    for ( as_index = GERAN_DATA_SPACE_INDEX_1; as_index < NUM_GERAN_DATA_SPACES; as_index++ )
    {
      MSG_GERAN_HIGH_2_G( "MULTISIM: gsm_only[%d]:%d",
                          as_index,
                          (int)(l1_sc_wcdma_data[as_index].rr_params.gsm_only ) );
    }
  }
  else
  {
    MSG_GERAN_HIGH_1_G( "gsm_only is %d",
                        (int)l1_sc_wcdma_data_ptr->rr_params.gsm_only);
  }

  return;
}

/*===========================================================================

FUNCTION l1_sc_wcdma_get_gsm_only_mode

DESCRIPTION
  Function used to access the gsm_only setup for this data space

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_wcdma_get_gsm_only_mode( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  return (l1_sc_wcdma_data_ptr->rr_params.gsm_only);
}

#ifdef FEATURE_WCDMA
/*===========================================================================

FUNCTION l1_sci_trm_blocked_inc_wcdma_page_cycle_tick

DESCRIPTION
  Used to increment the page_cycle_tick, if the lock wasn't granted in dual sim
  builds. This will ensure that the scheduling remains on track.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_trm_blocked_inc_wcdma_page_cycle_tick( gas_id_t gas_id )
{
  l1_sc_wcdma_data[gas_id].wcdma_srch.page_cycle_tick++;
}
#endif /* FEATURE_WCDMA */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION l1_sc_wcdma_srch_active

DESCRIPTION
  Used by gdrivers to be able to identify when a W search is really being
  activated.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean l1_sc_wcdma_srch_active( gas_id_t gas_id )
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  return ( l1_sc_globals_ptr->wcdma_srch_in_progress );
}


/*===========================================================================

FUNCTION gl1_sc_irat_is_lte_active

DESCRIPTION
  Check if LTE Idle measuremnt is currently active

DEPENDENCIES
  none

RETURN VALUE
  TRUE LTE Idle search is active

SIDE EFFECTS
  none

===========================================================================*/
#ifdef  FEATURE_GSM_TO_LTE

boolean gl1_sc_irat_is_lte_active( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  boolean  active = FALSE;

  if( ( l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_ACTIVE )
        &&(l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_LTE)
        &&(l1_sc_globals_ptr->wcdma_srch_in_progress == TRUE)
    )
  {
    active = TRUE;
  }

  return  active;
}
#endif /* FEATURE_GSM_TO_LTE */

/*===========================================================================

FUNCTION l1_sc_get_rat_change_active

DESCRIPTION
  Check if we are in process of change to GSM only mode.

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
boolean l1_sc_get_rat_change_active( gas_id_t gas_id )
{
  return ( l1_is_rat_change_active [gas_id] );
}

/*===========================================================================

FUNCTION l1_sc_set_rat_change_active

DESCRIPTION
  Set the flag if we are in process of change to GSM only

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  none

===========================================================================*/
void l1_sc_set_rat_change_active( boolean active, gas_id_t gas_id )
{
  l1_is_rat_change_active [gas_id] = active;
}

#ifdef FEATURE_GSM_TDS


/*===========================================================================

FUNCTION l1_sc_irat_performing_acq_meas

DESCRIPTION
  This function is introduced to support work around for delaying NON-DRX operation
  in G2T scenario. Once T supports Abort IRAT feature, this function will be
  redundant and should be deleted.

DEPENDENCIES
  none

RETURN VALUE
  If T is active then TRUE or FALSE depending upon the IRAT search state.
  Otherwise FALSE.

SIDE EFFECTS
  none

===========================================================================*/
boolean l1_sc_irat_performing_acq_meas(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  boolean status = FALSE;

  /**
   * CR 346062 Applicable for TDD
   */
  if((l1_sc_wcdma_data_ptr->tds_irat_active) && (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL))
  {
     status = TRUE;
  }

  return status;
}
/*===========================================================================

FUNCTION l1_sc_irat_performing_ded_meas

DESCRIPTION
  This function returns whether GL1 is measuring dedicated G2T IRAT or not.

DEPENDENCIES
  none

RETURN VALUE
  If T is active then TRUE or FALSE depending upon the IRAT search state.
  Otherwise FALSE.

SIDE EFFECTS
  none

===========================================================================*/
boolean l1_sc_irat_performing_ded_meas(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  boolean status = FALSE;

  if(l1_sc_wcdma_data_ptr->tds_irat_active)
  {
    if((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL) && (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED))
    {
      status = TRUE;
    }
  }

  return status;
}
/*===========================================================================

FUNCTION tds l1 sc set/get functions for:
         l1_tds_issue_cleanup
         tds_irat_active
         tds_state

===========================================================================*/
boolean l1_sc_get_tds_issue_cleanup(gas_id_t gas_id)
{
  return(l1_sc_wcdma_data[gas_id].l1_tds_issue_cleanup);
}

void l1_sc_set_tds_issue_cleanup(boolean value, gas_id_t gas_id)
{
  l1_sc_wcdma_data[gas_id].l1_tds_issue_cleanup = value;
}

boolean l1_sc_get_tds_irat_active(gas_id_t gas_id)
{
  return(l1_sc_wcdma_data[gas_id].tds_irat_active);
}

void l1_sc_set_tds_irat_active(boolean value, gas_id_t gas_id)
{
  if ((value == TRUE) && (FALSE == geran_is_multi_mode_gas_id(gas_id)))
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0_G("Attempt to enable T iRAT with bad GAS ID");
  }
  else
  {
    MSG_GERAN_MED_1_G("tds_irat_active set to %d", value);
    l1_sc_wcdma_data[gas_id].tds_irat_active = value;
  }
}

void l1_sc_set_tds_state(byte value,gas_id_t gas_id)
{
  l1_sc_wcdma_data[gas_id].tds_state = value;
}

byte l1_sc_get_tds_state(gas_id_t gas_id)
{
  return(l1_sc_wcdma_data[gas_id].tds_state);
}

#endif /*FEATURE_GSM_TDS*/

/*===========================================================================

FUNCTION l1_sc_ts_changed()

DESCRIPTION

     If time slot changed on FNmod26==22, inhibit G2L PTM scheduling.
     gstmr isr gets elongated and causes snapshot timing errors in ML1.
     This function captures both the ReConfig and Release TS change.


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_ts_changed(gas_id_t gas_id)
{
   if(frame_counters[gas_id].FNmod26==22)
   {
      l1_sc_set_skip_g2x_pkt_txfr( TRUE, gas_id);
   }
}

/*===========================================================================

FUNCTION l1_sc_set_skip_g2x_pkt_txfr()

DESCRIPTION

     Set/Clear flag to inhibit G2X PTM measurements when TS reconfig/release occurs.
     This is set on FNmod26==21 or 22 and cleared on FNmod26=23.


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_set_skip_g2x_pkt_txfr(boolean value, gas_id_t gas_id)
{
  l1_sc_wcdma_data[gas_id].l1_skip_g2x_pkt_txfr = value;
}

#ifdef  FEATURE_GSM_TO_LTE
boolean l1_sc_get_lte_irat_active(gas_id_t gas_id)
{
  return(l1_sc_wcdma_data[gas_id].lte_irat_active);
}

/*===========================================================================

FUNCTION l1_sc_set_lte_irat_mcpm_active/l1_sc_get_lte_irat_mcpm_active

DESCRIPTION

     lte_irat_mcpm_active is set:
       TRUE when IRAT requests G2L MCPM start (when G2l starts)
       FALSE when IRAT requests G2L MCPM stop (in DEINT CNF)

   In additon to give the G2l init state (for init/deint) it is used to
   determine whether MPH_INTERRAT_LIST_SRCH_TIME_REQ can request G2L MCPM stop

===========================================================================*/
void l1_sc_set_lte_irat_mcpm_active(boolean value, gas_id_t gas_id)
{
    l1_sc_wcdma_data[gas_id].lte_irat_mcpm_active = value;
}

boolean l1_sc_get_lte_irat_mcpm_active(gas_id_t gas_id)
{
    return (l1_sc_wcdma_data[gas_id].lte_irat_mcpm_active);
}

#endif /* FEATURE_GSM_TO_LTE */

void l1_sc_set_wcdma_irat_mcpm_active(boolean value, gas_id_t gas_id)
{
    l1_sc_wcdma_data[gas_id].wcdma_irat_mcpm_active = value;
}

boolean l1_sc_get_wcdma_irat_mcpm_active(gas_id_t gas_id)
{
    return (l1_sc_wcdma_data[gas_id].wcdma_irat_mcpm_active);
}


#ifdef  FEATURE_GSM_TDS
void l1_sc_set_tds_irat_mcpm_active(boolean value, gas_id_t gas_id)
{
    l1_sc_wcdma_data[gas_id].tds_irat_mcpm_active = value;
}

boolean l1_sc_get_tds_irat_mcpm_active(gas_id_t gas_id)
{
    return (l1_sc_wcdma_data[gas_id].tds_irat_mcpm_active);
}

#endif

boolean l1_sc_get_wcdma_update_list_irat_active(gas_id_t gas_id)
{
  return(l1_sc_wcdma_data[gas_id].wcdma_update_list_irat_active);
}

void l1_sc_set_wcdma_irat_active(boolean value, gas_id_t gas_id)
{
    l1_sc_wcdma_data[gas_id].wcdma_irat_active = value;
}

/*===========================================================================

FUNCTION l1_sc_check_irat_active()

DESCRIPTION

     Returns TRUE if there is any ongoing IRAT activity still in progress


DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean l1_sc_check_irat_active(gas_id_t gas_id)
{
 l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);

 /* sanity check the multimode capability of the specified GAS ID */
 if (FALSE == geran_is_multi_mode_gas_id(gas_id))
 {
   // irat should never be active on a non-multimode GAS ID
   MSG_GERAN_LOW_0_G("check_irat_active: this GAS ID is not multimode");

   return(FALSE);
 }
 else
 {
   /*check_irat_active indicates pending G2X Init/Deinit activity which will be done at the end of ISR*/
   return( g2x_irat_cnf_pending(gas_id)                        /* irat message  g2x_cnf outstanding   */
           || l1_sc_globals_ptr->wcdma_srch_in_progress 
           || gl1_hw_g2x_send_init_req[gas_id] || gl1_hw_g2x_send_deinit_req[gas_id]
           || ((check_irat_active[gas_id] & (1 << MODE_CHANGE_REQ_RECEIVED)) != 0)
           || ((check_irat_active[gas_id] & (1 << UPDATE_LIST_REQ_RECEIVED)) != 0));
 }
}


/*===========================================================================

FUNCTION l1_sc_frames_available

DESCRIPTION
             wrapper for l1_fm_are_frames_available
             and if update list is in progress inhibit new search from starting
DEPENDENCIES
  none

RETURN VALUE
  boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_frames_available( l1_fm_priority_T pri, uint32 frames_required,
                                    void ( *abort_callback )( void*,gas_id_t ),
                                    void* abort_data,
                                    gas_id_t gas_id )
{
  boolean frames_available = TRUE;

  if (l1_sc_get_wcdma_list_update_inprogress(gas_id) == TRUE)
  {
    frames_available = FALSE;
  }
#if defined(FEATURE_GSM_WTR_HOP)
  /* check if hop is active */
  else if(gl1_msg_wtr_hop_get_status() != WTR_HOP_INACTIVE)
  {
    MSG_GERAN_HIGH_0_G("G2X: WTR hopping conflict, no frames available");
    frames_available = FALSE;
  }
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  else if (gl1_msg_get_multi_sim_mode())
  {
    if ( pri == FM_PRI_WCDMA_IDSEARCH )
    {
      if (l1_fm_are_frames_available(pri, frames_required, abort_callback, abort_data, gas_id))
      {
        l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
        uint16 W_chn = l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset].arfcn.UARFCN;
        /*Safe check. validate RF dev id*/
        frames_available = gl1_hw_check_granted_device_is_irat_compatabile(W_chn, gas_id);
      }
      else
      {
        frames_available = FALSE;
      }
    }
    else if ( pri == FM_PRI_WCDMA_RECONF )
    {

      if (l1_fm_are_frames_available(pri, frames_required, abort_callback, abort_data, gas_id))
      {
        l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
        uint16 W_chn = l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset].arfcn.UARFCN;
        frames_available = gl1_hw_check_granted_device_is_irat_compatabile(W_chn, gas_id);
      }
      else
      {
        frames_available = FALSE;
      }
    }
#ifdef  FEATURE_GSM_TO_LTE
    else if ( pri == FM_PRI_LTE_IDSEARCH )
    {
      if (l1_fm_are_frames_available(pri, frames_required, abort_callback, abort_data, gas_id))
      {
        l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
        lte_earfcn_t L_chn = l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset].arfcn.euarfcn;
        /*Safe check. validate RF dev id*/
        frames_available = gl1_hw_check_granted_device_is_LTE_irat_compatabile(L_chn, gas_id);
      }
      else
      {
        frames_available = FALSE;
      }
    }
#endif /*FEATURE_GSM_TO_LTE*/
#ifdef FEATURE_GSM_TDS
    else if ( pri == FM_PRI_TDS_ACQ )
    {
      if (l1_fm_are_frames_available(pri, frames_required, abort_callback, abort_data, gas_id))
      {
        /* No need to check rf device compatibility for irat operation,
           as GL1 has performed band registration in l1_fm_are_frames_available */
        frames_available = TRUE;
      }
      else
      {
        frames_available = FALSE;
      }
    }
#endif /*FEATURE_GSM_TDS*/
    else
    {
      frames_available = FALSE;
    }
  }
#endif /* FEATURE_DUAL_SIM  || FEATURE_TRIPLE_SIM  */
  else
  {
    frames_available = l1_fm_are_frames_available(pri, frames_required, abort_callback, abort_data, gas_id);
  }
  return  frames_available;
}

/*
** when we enter dedicated or transfer mode.
** If we have LTE cells then configure for CM mode measurements
*/

static void g2l_configure_for_cm(gas_id_t gas_id)
{

#ifdef  FEATURE_GSM_TO_LTE
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  if(l1_sc_wcdma_data_ptr->lte_cm_irat_active == G2L_CM_INACTIVE)
  {
      /*  if lte_irat_active always ready */
      if(   !l1_sc_wcdma_data_ptr->rr_params.gsm_only
          && l1_sc_wcdma_data_ptr->lte_irat_active
          && l1_sc_wcdma_data_ptr->rr_params.num_L_freqs )              /*   we have L ncells and not active  */
      {
         l1_sc_wcdma_data_ptr->lte_cm_irat_active = G2L_CM_ACTIVE;      /* Allow G2L on next idle frame tick to start CM mode */
      }
      else
      {
         l1_sc_wcdma_data_ptr->lte_cm_irat_active = G2L_CM_INACTIVE;
      }
  }

#else
  (void)gas_id;
#endif
}


boolean g2l_cm_xfer_state(gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    if(l1_tskisr_blk->l1_state == L1_TRANSFER_MODE )
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }

} /* g2l_cm_xfer_state */


/*===========================================================================

FUNCTION l1_sc_irat_get_gbta_not_allowed

DESCRIPTION
   whether we don't allow gbta in xfer idle frame


DEPENDENCIES
  none

RETURN VALUE
  returns TRUE, if a IRAT measurement will use the idle frame.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_irat_get_gbta_not_allowed(gas_id_t gas_id)
{
  boolean gbta_not_allowed = FALSE;
#ifdef FEATURE_GPRS_GBTA
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  if (l1_sc_wcdma_data_ptr->gbta_not_allowed)
  {
    MSG_GERAN_MED_0_G("GBTA is not allowed by IRAT");
  }
  gbta_not_allowed = l1_sc_wcdma_data_ptr->gbta_not_allowed;
#endif /*FEATURE_GPRS_GBTA*/
  return gbta_not_allowed;
}

/*===========================================================================

FUNCTION l1_sc_irat_get_gbta_not_allowed

DESCRIPTION
   Clear not allow GBTA flag


DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_irat_set_gbta_not_allowed(boolean gbta_not_allowed,
                                     gas_id_t gas_id)
{
#ifdef FEATURE_GPRS_GBTA
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_wcdma_data_ptr->gbta_not_allowed = gbta_not_allowed;
#endif /*FEATURE_GPRS_GBTA*/
}

/*
** Indicate cnf is pending
** to be sent after all active rats are shutdown
*/
void l1_sc_mode_cnf_pending(boolean SetPending , uint16 ratInactiveBit, gas_id_t gas_id)
{

#ifdef FEATURE_SGLTE
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  if(l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending == bMODE_FromInit)      /* only send if called from actual MPH_UE_MODE_CHANGE_REQ msg */
  {
     return;
  }

  if(SetPending)
  {
    /*
    ** gsm_only  0 -> 1  shutdown rats and wait before sending confirm
    ** gsm_only  1 -> 0  we send _cnf immediately.
    */
    l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending = bMODE_CNF_PENDING;
    /*
    ** RR legacy behaviour for NON-SGLTE mode causes problems with back to back UE changes.
    ** so only check rats active in the off transition condition.
    */
    if(l1_sc_wcdma_data_ptr->rr_params.gsm_only)
    {
      /*
      **  flag all active rats for shutdown only
      */
      if(l1_sc_wcdma_data_ptr->lte_irat_active)
      {
         l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending |= bLTE_RAT_INACTIVE;
      }

      if(l1_sc_wcdma_data_ptr->wcdma_irat_active)
      {
         l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending |= bWCDMA_RAT_INACTIVE;
      }

      /* tds_irat_active is not set until int_cnf, but maybe in progress */
      if((l1_sc_wcdma_data_ptr->tds_irat_active) || (TRUE == gl1_TDS_IRAT_INIT_CNF_pending))
      {
         l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending |= bTDS_RAT_INACTIVE;
      }
    }
  }
  else
  {
     /*
     ** clear each bit as each rat is deactivated
     */
     l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending &= ~ratInactiveBit;
  }

  /*
  ** all rats inactive and cnf is pending
  */
  if(l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending == bMODE_CNF_PENDING)
  {
    l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending = bMODE_NULL;

    MSG_GERAN_HIGH_3_G( "G2L: mode change sending MPH_UE_MODE_CHANGE_CNF", 0, 0, 0 );

    L1_send_MPH_UE_MODE_CHANGE_CNF(gas_id);

  }
  else
  if(l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending & bMODE_CNF_PENDING)
  {
    MSG_GERAN_HIGH_3_G( "G2L: mode change cnf pending 0x%x", l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending, 0, 0 );
  }
#endif

} /* l1_sc_mode_cnf_pending */


boolean l1_sc_g2x_shutdown_cnf_pending(gas_id_t gas_id)
{
#ifdef FEATURE_SGLTE
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   return ( (l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending & bLTE_RAT_INACTIVE) ||
            (l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending & bWCDMA_RAT_INACTIVE) ||
            (l1_sc_wcdma_data_ptr->l1_sc_send_mode_cnf_pending & bTDS_RAT_INACTIVE)
           );
#else

   return(FALSE);
#endif
}
/*===========================================================================

FUNCTION l1_isr_G2L_trigger

DESCRIPTION
  When CM measurement is active this function is called to trigger G2L GFW startup/cleanup
  from the l1 isr handlers l1_isr_transfer_mode(),l1_isr_dedicated_mode(),l1_isr_dtm_mode()
  on each frame tick.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_isr_G2L_trigger(gas_id_t gas_id)
{

#ifdef  FEATURE_GSM_TO_LTE

   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   /*
   ** For G2L PTM , trigger GFW startup and cleanup
   */
   if(l1_sc_wcdma_data_ptr->lte_cm_irat_active)
   {
       l1_g2l_trigger_mdsp( FALSE, gas_id );
   }
#else
  (void)gas_id;
#endif

} /*  l1_isr_G2L_trigger */

/*===========================================================================

FUNCTION l1_sc_irat_state_machine_active

DESCRIPTION
  Check to see IRAT state machine is inactive


DEPENDENCIES
  none.

RETURN VALUE
  TRUE is irat compatable device

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_sc_irat_state_machine_active(gas_id_t gas_id)
{
  boolean irat_state_machine_active = FALSE;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];


  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
     l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

     MSG_GERAN_HIGH_3_G("IRAT active %d, srch_state %d, srch_type %d",
          l1_sc_globals_ptr->active_irat,
          l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,
          l1_sc_wcdma_data_ptr->wcdma_srch.srch_type);

     irat_state_machine_active = TRUE;
  }

  return(irat_state_machine_active);

}
/*===========================================================================

FUNCTION  l1_sc_reset_state_machine_in_leave_idle

DESCRIPTION
  Clean up the irat state machine if abort is 0 and srch state is wait state.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_reset_state_machine_in_leave_idle( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  gl1_irat_mutex_lock( gas_id);
  if((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_WAIT) && (!l1_sc_wcdma_data_ptr->wcdma_srch.abort))
  {
     l1_sc_set_gtow_srch_state_null (gas_id);
  }
  gl1_irat_mutex_unlock( gas_id);
} /* l1_sc_reset_state_machine_in_leave_idle */


/*===========================================================================

FUNCTION  l1_sc_g2x_sdcch_inprogress

DESCRIPTION

        Returns TRUE if G2X is actively doing measurement in SDCCH gap,
        this requires 10 frames with no abort.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_g2x_sdcch_inprogress(gas_id_t gas_id)
{

    l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

    if(l1_sc_globals_ptr->mode == L1SCModeDediSDCCh)
    {
        /* G2L or G2T active ? */
        return (l1_sc_wcdma_srch_active(gas_id));
    }
    else
    {
       return FALSE;
    }
}

/*===========================================================================

FUNCTION l1_isr_G2L_idle_trigger

DESCRIPTION
  When G2L Idle measurement is active this function is called to trigger G2L GFW startup/cleanup
  from the l1 isr handlers l1_isr_idle_mode() on each frame tick.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_isr_G2L_idle_trigger(gas_id_t gas_id)
{
#ifdef  FEATURE_GSM_TO_LTE
   /*
   ** For G2L PTM , trigger GFW startup and cleanup
   */
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   if(l1_sc_wcdma_data_ptr->lte_irat_active)
   {
       l1_g2l_trigger_mdsp( TRUE, gas_id );
   }
#else
  (void)gas_id;
#endif
   return;
} /*  l1_isr_G2L_idle_trigger */

/*===========================================================================

FUNCTION l1_sc_clear_filters_stored_meas

DESCRIPTION

Clear Irat stored meas and filters

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_sc_clear_filters_stored_meas(gas_id_t gas_id)
{

l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
uint16 idx,cell_idx;

  for (idx=0; idx<MAX_WCDMA_FREQS; idx++)
  {
      /* Clear the non-DRX cache */
      l1_sc_wcdma_data_ptr->stored_meas[idx].cached_xfer_cells = 0;
      l1_sc_wcdma_data_ptr->stored_meas[idx].num_cells=0;
      l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].num_cells = 0;
      l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].freq_age = 0;

      /*initialise the frequency lists*/
      l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].rat = RR_L1_IRAT_RAT_UNDEFINED;
      l1_sc_wcdma_data_ptr->stored_meas[idx].rat = RR_L1_IRAT_RAT_UNDEFINED;
      for(cell_idx = 0; cell_idx < MAX_WCDMA_CELLS_PER_UARFCN; cell_idx++)
      {
        l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].wcdma_cell.RSSI_avg    = 0;
        l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].wcdma_cell.RSSI_last   = 0;
        l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].wcdma_cell.RSSI_last2  = 0;
      }

#ifdef FEATURE_GSM_TDS
      for(cell_idx = 0; cell_idx < MAX_WCDMA_CELLS_PER_UARFCN; cell_idx++)
      {
        l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].wcdma_cell.RSSI_avg   = 0;      /* ensure empty list is detected */
        l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].wcdma_cell.RSCP_avg   = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].wcdma_cell.RSCP_last  = MIN_TDD_CELL_RSCP_VALUE;
        l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].wcdma_cell.RSCP_last2 = MIN_TDD_CELL_RSCP_VALUE;
      }
#endif


#ifdef FEATURE_GSM_TO_LTE
     for(cell_idx = 0; cell_idx < MAX_WCDMA_CELLS_PER_UARFCN; cell_idx++)
     {
       l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].lte_cell.rsrp_avg    = 0;
       l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].lte_cell.rsrp_last   = 0;
       l1_sc_wcdma_data_ptr->stored_meas[idx].cell_energy[cell_idx].lte_cell.rsrp_last2  = 0;
     }
#endif
  }
}

/*===========================================================================

FUNCTION l1_sc_is_irat_state_null

DESCRIPTION

Check to see Dedicated AND Idle searches are NULL

DEPENDENCIES
  none.

RETURN VALUE
  TRUE if id state and dedicated state are NULL
  FALSE Otherwise

SIDE EFFECTS
  None
===========================================================================*/

boolean l1_sc_is_irat_state_null(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
  {
      return TRUE;
  }
  else
  {
      return FALSE;
  }

}/*function l1_sc_is_irat_state_null*/

/*===========================================================================

FUNCTION l1_sc_irat_check_gbta_conflict

DESCRIPTION

Find GBTA conflicts with IRAT.

DEPENDENCIES
  none.

RETURN VALUE
  TRUE if IRAT conflicts with GBTA
  FALSE Otherwise

SIDE EFFECTS
  None
===========================================================================*/
static boolean l1_sc_irat_check_gbta_conflict(gas_id_t gas_id)
{
  boolean conflict = FALSE;
#ifdef FEATURE_GPRS_GBTA
  /*GBTA is not activated by other SUB, set gbta_not_allowed to True so
    that check_gbta_allowed should fail*/
  if (GRM_GBTA_ACTIVE == GET_GBTA_STATE())
  {
    /*Skip IRAT. Other SUB is already in GBTA*/
    conflict = TRUE;
    MSG_GERAN_MED_0_G("Skipped IRAT, GBTA is going on");
  }
#endif /*FEATURE_GPRS_GBTA*/
  return conflict;
}

/*===========================================================================

FUNCTION gl1_check_if_irat_state_stuck

DESCRIPTION
FUnction checks if GL1 is unable to move out of a particular state for 25 frames because either of the flags
receive_in_progress/wcdma_srch_in_progress/wcdma_id_srch_in_progress are not cleared, then crash

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_check_if_irat_state_stuck(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  static uint8 count_idle[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };
  static uint8 count_dedi[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(NULL) };

  /*Check if any of the below IRAT related flags are SET*/
  if((l1_sc_globals_ptr->wcdma_srch_in_progress) || (l1_sc_globals_ptr->receive_in_progress))
   {
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
      /*Check if GL1 is in Idle/Multi SIM Idle mode*/
      if((l1_tskisr_blk->l1_state == L1_IDLE_MODE) || (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE))
#else
       if(l1_tskisr_blk->l1_state == L1_IDLE_MODE)
#endif
      {
        MSG_GERAN_HIGH_2_G("srch_state %d count %d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,count_idle[gas_id]);

        /*If Id/reconfirm state is not NULL , then increase counter count_idle by 1*/
        if ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)&&
            ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM) ||
             (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID)))
        {
          count_idle[gas_id]++;

          /*If GL1 is not able to move out of idle mode for 25 frames then crash*/
          if(count_idle[gas_id] == NO_FRAMES_TO_CHECK_IF_GL1_IS_STUCK)
          {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_GERAN_FATAL_3_G("Crash as Gl1 is unable to leave idle mode",0,0,0);
          }
        }
        else
        {
          count_idle[gas_id] = 0;
        }

      }
      /*Check if GL1 is in Dedicated/Packet Transfer mode*/
      else if((l1_tskisr_blk->l1_state == L1_DEDICATED_MODE) || (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE))
      {
        MSG_GERAN_HIGH_2_G("srch_state %d count %d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state,count_dedi[gas_id]);

        /*Check if either of dedicated/id_tbf state is not NULL,then increase counter count_dedi by 1*/

        if((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL) &&
           ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED) || (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID_IN_TBF)))
        {
          count_dedi[gas_id]++;

          /*If GL1 is not able to move out of dedicated/PTM  mode for 25 frames then crash*/
          if(count_dedi[gas_id] == NO_FRAMES_TO_CHECK_IF_GL1_IS_STUCK)
          {
#ifdef FEATURE_QSH_MDUMP
           QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
           ERR_GERAN_FATAL_3_G("Crash as Gl1 is unable to leave dedicated/transfer mode",0,0,0);
          }
        }
        else
        {
          count_dedi[gas_id] = 0;
        }
      }
   }
   else
   {
     count_idle[gas_id] =0;
     count_dedi[gas_id] =0;
   }
}  /* gl1_check_if_irat_state_stuck */

#ifdef FEATURE_GSM_TO_LTE
/*===========================================================================

FUNCTION gl1_check_if_g2l_init_stuck

DESCRIPTION
  Debug crash if new list with LTE neighbours is received but G2L INIT was not
  sent or confirmed for 50 frames in any mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_check_if_g2l_init_stuck(gas_id_t gas_id)
{
   l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

   static uint8 frames_count[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(0) };

   if((TRUE == l1_sc_wcdma_data_ptr->lte_update_list_irat_active) && (g2l_get_Init_Sent_status(gas_id) != LTE_G2L_READY))
   {
      frames_count[gas_id]++;

      if(frames_count[gas_id] >= 50)
      {
#ifdef FEATURE_QSH_MDUMP
         QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
         ERR_GERAN_FATAL_2_G("DEBUG CRASH:: G2L_INIT_sent %x frames %d", g2l_get_Init_Sent_status(gas_id), frames_count[gas_id]);
      }
   }
   else
   {
      frames_count[gas_id] = 0;
   }
}

#endif /* FEATURE_GSM_TO_LTE */

/*===========================================================================

FUNCTION g2x_rfm_meas_common_init_params

DESCRIPTION
        wrapper for external function

DEPENDENCIES
  none.

RETURN VALUE

SIDE EFFECTS
  None
===========================================================================*/

void g2x_rfm_meas_common_init_params( rfm_meas_common_param_type *g2x_common_rf_ptr, gas_id_t gas_id)
{
    (void)gas_id;
    /*
    ** Initialise default values for common rf_param
    */
    rfm_meas_common_init_params( g2x_common_rf_ptr);
}

/*===========================================================================

FUNCTION g2x_set_common_rf_enter

DESCRIPTION
         Common g2x enter, initialises and sets all the source tech rf_params
         for the local g store gl1_irat_rf_common_structure
 .
         Does this need to be double buffered or dual data spaced ???
DEPENDENCIES
  none.

RETURN VALUE


SIDE EFFECTS
  None
===========================================================================*/

void g2x_set_common_rf_enter( rfm_meas_common_param_type *g2x_common_rf_ptr, gas_id_t gas_id)
{
  ARFCN_T scell_arfcn;
  uint32  gsm_rxlm_buf_idx;
  rfm_device_enum_type gsm_device;

  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  g2x_rfm_meas_common_init_params(g2x_common_rf_ptr, gas_id);         /* init each gap */
  /*
  ** get gsm band and arfcn
  */
  gl1_msg_algs_get_arfcn( &scell_arfcn.num, &scell_arfcn.band, gas_id );

  gsm_rxlm_buf_idx = gl1_hw_get_gsm_rxlm_buf_idx(gas_id);
  gsm_device       = gl1_hw_get_gsm_rf_id(l1_tskisr_blk->client_id, gas_id);  /* returns rfm_device_enum_type */
  /*
  ** header params
  */
  g2x_common_rf_ptr->header.source_tech                                   = RFM_EGSM_MODE;
  g2x_common_rf_ptr->header.num_source_item                               = 1;
  g2x_common_rf_ptr->header.source_param_v2[RFM_MEAS_PCELL].band          = gl1_hw_arfcn_band_to_rfcom_gsm_band(scell_arfcn.band);
  g2x_common_rf_ptr->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].device    = gsm_device;
  g2x_common_rf_ptr->header.source_param_v2[RFM_MEAS_PCELL].rx_dev_handle[DEV_HANDLE_PRX].lm_handle = gsm_rxlm_buf_idx;
  /*
  ** populate gsm source tech common params
  ** GRF have deprecated unused items
  */
  g2x_common_rf_ptr->source_param.gsm_params.sub_id                      = gas_id;
  g2x_common_rf_ptr->source_param.gsm_params.band                        = gl1_hw_arfcn_band_to_rfcom_gsm_band(scell_arfcn.band);
  g2x_common_rf_ptr->source_param.gsm_params.diversity_en                = FALSE;             /* is boolean */

  g2x_common_rf_ptr->source_param.gsm_params.rx_dev_handle[DEV_HANDLE_PRX].device     = gsm_device;
  g2x_common_rf_ptr->source_param.gsm_params.rx_dev_handle[DEV_HANDLE_PRX].lm_handle  = gsm_rxlm_buf_idx;

  g2x_common_rf_ptr->source_param.gsm_params.rx_dev_handle[DEV_HANDLE_DRX1].device     = RFM_INVALID_DEVICE;
  g2x_common_rf_ptr->source_param.gsm_params.rx_dev_handle[DEV_HANDLE_DRX1].lm_handle  = 0;

  /* Populate script type and scenario type */
  g2x_common_rf_ptr->script_type    = RFM_MEAS_PROGRAM_STARTUP;  /* Anything except NULL2X */
  g2x_common_rf_ptr->scenario_type  = RFM_MEAS_SCENARIO_IRAT;

} /* g2x_set_common_rf_enter */

/*===========================================================================

FUNCTION gl1_get_irat_rf_structure_ptr

DESCRIPTION

         Does this need to be double buffered or dual data spaced ???

DEPENDENCIES
  none.

RETURN VALUE
   rfm_meas_common_param_type* to gl1_irat_rf_common_structure

SIDE EFFECTS
  None
===========================================================================*/
rfm_meas_common_param_type* gl1_get_irat_rf_structure_ptr(gas_id_t gas_id)
{
    return &gl1_irat_rf_common_structure[gas_id];
}
/*===========================================================================

FUNCTION l1_sc_x2g_ta_irat_gap

DESCRIPTION Returns whether the X+G TA gap is just for IRAT
===========================================================================*/
boolean l1_sc_x2g_ta_irat_gap(gas_id_t gas_id)
{
   boolean is_irat_gap = FALSE;
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
   volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   is_irat_gap = (gl1_msg_get_multi_sim_mode() &&
                  l1_fm_is_next_activity_IRAT(gas_id) &&
                  l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE &&
                  l1_tskisr_blk->firmware_loaded == FALSE);
#endif

   return is_irat_gap;
}

/*===========================================================================

FUNCTION l1_sc_send_irat_g2x_init

DESCRIPTION
  When MCPM clock update has completed, initialise TDSCDMA/LTE/WCDMA for IRAT.
     i.e the clocks for other RAT must be correct before we can send this request.

  This is executed from the DPC task.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sc_send_irat_g2x_init(rr_l1_irat_rat_e rat, boolean stop_req_after_start, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t* l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  gl1_irat_mutex_lock( gas_id);
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
  switch(rat)
  {
    case RR_L1_IRAT_RAT_WCDMA:
        if(gl1_hw_g2x_send_init_req[gas_id] & (1 << RR_L1_IRAT_RAT_WCDMA))
        {
          if (stop_req_after_start == FALSE)
          {
            gl1_hw_g2x_send_init_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_WCDMA);
            /* takes care of G2W drivers init */
            gl1_hw_wcdma_init(gas_id);
#ifndef FEATURE_GSM_TO_WCDMA_MSGR
            /* Perform G2W initialization */
            wl1_x2w_srch_g2w_wfw_sw_init(gas_id);

            /*Decrement active task count*/
            gl1_drx_task_inactive(gas_id);
#endif
          }
        }
        else if(gl1_hw_g2x_send_deinit_req[gas_id] & (1 << RR_L1_IRAT_RAT_WCDMA))
        {
          gl1_hw_g2x_send_deinit_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_WCDMA);
          /*If QTA gap is active, don't send De-init request here, it will take care while QTA gap exit if g2w_shutdown_pending set*/
          if (!gl1_hw_qta_gap_active(gas_id) || !gl1_hw_get_qta_gap_for_g2x_irat(gas_id))
          {
          /* takes care of G2W drivers deinit */
          l1_sc_g2w_shutdown(gas_id);
          }
          else
          {
            l1_sc_globals_ptr->g2w_shutdown_pending = TRUE;
          }
        }
        else if(!(l1_sc_wcdma_data[gas_id].wcdma_irat_active ))
          {
          /* If going inactive do not turn off WSTMR as need to be continuous for WL1 timing */
          if(l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE && (g2w_active_mcpm_scenario[gas_id] != 0))
          {
            gl1_drx_task_active(gas_id);
            gl1_hw_mcpm_neighbor_update(MCPM_G2W_MEAS_STOP, g2w_active_mcpm_scenario[gas_id], gas_id);
          }
          g2w_active_mcpm_scenario[gas_id] = 0;
          l1_sc_set_wcdma_irat_mcpm_active(FALSE,gas_id);
        }
        break;

     case RR_L1_IRAT_RAT_LTE:
      {
#ifdef  FEATURE_GSM_TO_LTE

        if(gl1_hw_g2x_send_init_req[gas_id] &   (1 << RR_L1_IRAT_RAT_LTE))
        {
          if (stop_req_after_start == FALSE)
          {
            gl1_hw_allocate_lte_rxlm_buf(gas_id);
            register_LTE_irat_messages ( gas_id);
            gl1_hw_g2x_send_init_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_LTE);
            L1_send_LTE_CPHY_IRAT_MEAS_G2L_INIT_REQ(TRUE, gas_id);
          }
        }
           else if(gl1_hw_g2x_send_deinit_req[gas_id] &   (1 << RR_L1_IRAT_RAT_LTE))
        {
           gl1_hw_g2x_send_deinit_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_LTE);
          /*If QTA gap is active, don't send De-init request here, it will take care while QTA gap exit if g2l_shutdown_pending set*/
          if (!gl1_hw_qta_gap_active(gas_id) || !gl1_hw_get_qta_gap_for_g2x_irat(gas_id))
          {
           /* takes care of G2L deinit */
           l1_sc_g2l_shutdown(gas_id);
          }
          else
          {
            l1_sc_globals_ptr->g2l_shutdown_pending = TRUE;
          }
        }
        else if(!(l1_sc_wcdma_data[gas_id].lte_irat_active ))
        {  
          /* If going inactive do not turn off WSTMR as need to be continuous for WL1 timing */
          if(l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE && (g2l_active_mcpm_scenario[gas_id] != 0))
          {
            gl1_drx_task_active(gas_id);
            gl1_hw_mcpm_neighbor_update(MCPM_G2L_MEAS_STOP, g2l_active_mcpm_scenario[gas_id], gas_id);
          }
          g2l_active_mcpm_scenario[gas_id] = 0;
          l1_sc_set_lte_irat_mcpm_active(FALSE,gas_id);
        }
 #endif
      }
      break;

    case RR_L1_IRAT_RAT_UTRAN_TDD:
#ifdef FEATURE_GSM_TDS
        if(gl1_hw_g2x_send_init_req[gas_id] & (1 << RR_L1_IRAT_RAT_UTRAN_TDD))
          {
            if (stop_req_after_start == FALSE)
            {
              gl1_hw_allocate_tds_rxlm_buf();
              register_TDSCDMA_irat_messages(gas_id);
              gl1_hw_g2x_send_init_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_UTRAN_TDD);
              L1_send_TDS_IRAT_INIT_CMD(gas_id);
            }
          }
 else if(gl1_hw_g2x_send_deinit_req[gas_id] & (1 << RR_L1_IRAT_RAT_UTRAN_TDD))
        {
            gl1_hw_g2x_send_deinit_req[gas_id] &= ~(1 << RR_L1_IRAT_RAT_UTRAN_TDD);
            /*If QTA gap is active, don't send De-init request here, it will take care while QTA gap exit if g2l_shutdown_pending set*/
            if (!gl1_hw_qta_gap_active(gas_id) || !gl1_hw_get_qta_gap_for_g2x_irat(gas_id))
            {
            /* takes care of G2T deinit */
            l1_sc_g2t_shutdown(gas_id);
          }
            else
            {
              l1_sc_globals_ptr->g2t_shutdown_pending = TRUE;
            }
        }
        else if (!(l1_sc_wcdma_data[gas_id].tds_irat_active ))
          {
          /* If going inactive do not turn off WSTMR as need to be continuous for WL1 timing */
          if(l1_tsk_buffer[gas_id].l1_state != L1_INACTIVE_MODE && (g2t_active_mcpm_scenario[gas_id] != 0))
          {
            gl1_drx_task_active(gas_id);
            gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_STOP, g2t_active_mcpm_scenario[gas_id], gas_id);
          }
          g2t_active_mcpm_scenario[gas_id] = 0;
          l1_sc_set_tds_irat_mcpm_active(FALSE,gas_id);
        }
 #endif
        break;

     default:
        MSG_GERAN_HIGH_1_G("Unknown RAT %d" , rat );
     break;
  }
  gl1_irat_mutex_unlock( gas_id);
}/* l1_sc_send_irat_g2x_init */

#ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
/*===========================================================================

FUNCTION L1_SC_IS_G2X_TIMELINE_OPT_ENABLED

DESCRIPTION
  Check if feature is enabled or not in EFS file

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_is_g2x_timeline_opt_enabled( gas_id_t gas_id )
{
  l1_sc_g2x_timeline_opt_enable[gas_id] = TRUE;
  if ( GL1_HW_EFS_SLP_ENABLED( GL1_EFS_SLP_DISABLE_G2X_TIMELINE_OPT ) )
  {
    l1_sc_g2x_timeline_opt_enable[gas_id] = FALSE;
  }

  MSG_GERAN_HIGH_1_G("G2X timeline opt enable %d" , l1_sc_g2x_timeline_opt_enable[gas_id] );
}
#endif /* FEATURE_GSM_G2X_TIMELINE_REDUCTION */

/*===========================================================================

FUNCTION l1_sc_check_if_next_cm_search_is_lte

DESCRIPTION
This function checks if GL1 is going to schedule G2L search in the next frame. If yes, it sends a BIMC boost request
to MCPM

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void l1_sc_check_if_next_cm_search_is_lte(gas_id_t gas_id)
{
 uint16 f_off;

 l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
 l1_sc_globals_T   *l1_sc_globals_ptr     = gl1_ms_switch_l1_sc_globals_store(gas_id);

 uint32 next_suspen_FN, next_res_FN;
 gpl1_g2x_ta_priority_t pri;

 /*Get next freq index*/
 f_off     = l1_sc_get_next_ded_freq_idx(gas_id);

  g2l_bimc_boost_done[gas_id] = FALSE;

  
   /*If already receive is in progress or TS adj is happening or gsm_only is SET, no need for BIMC boost*/
  if((l1_sc_wcdma_data_ptr->rr_params.gsm_only) ||
   ((l1_sc_globals_ptr->mode == L1SCModeGprsTrans) && (l1_sc_wcdma_data_ptr->l1_skip_g2x_pkt_txfr))||
     !(l1_sc_wcdma_data_ptr->lte_irat_active))
  {
    MSG_GERAN_HIGH_3_G("Skip boost req bcz of rcv in pro/ts adj rcv %d skip %d or Lte active %d ",
		                                                             l1_sc_globals_ptr->receive_in_progress,
                                                                     l1_sc_wcdma_data_ptr->l1_skip_g2x_pkt_txfr,
                                                                     l1_sc_wcdma_data_ptr->lte_irat_active);
    return;
  }

  
  /*Go for BIMC boost if G2L search is going to be scheduled in the next frame*/
  if(
    ((l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_ACTIVE) &&
      ( l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx].rat == RR_L1_IRAT_RAT_LTE ) &&
      (((l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode == TRUE) && (l1_sc_check_priority_threshold(l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx, gas_id)))
       || 
      ((l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode == FALSE) &&(l1_sc_wcdma_ded_pending(gas_id))))
     )

  ||

 (( l1_sc_wcdma_data_ptr->rr_params.freq_list[l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset].rat == RR_L1_IRAT_RAT_LTE )&&
  ((l1_sc_wcdma_data_ptr->rr_params.cntTimed_Req != 0) ||
     ((l1_sc_wcdma_data_ptr->g2l_inhibit_next_update == TRUE) &&
     (l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick <= l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick))))

  ||
  
  (  (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_LTE) &&
      (l1_sc_wcdma_data_ptr->wcdma_srch.ded_tick <= l1_sc_wcdma_data_ptr->wcdma_srch.ded_cycle_tick) &&
      (((l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode == TRUE) && (l1_sc_check_priority_threshold(f_off, gas_id)))
       || 
      ((l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode == FALSE) &&(l1_sc_wcdma_ded_pending(gas_id))))
   )
   )
    {
    
     /*For AEQ, Do BIMC boost*/
     if(gl1_get_mcvs_speed(gas_id) == MCVS_Q6_CLK_KHZ_AEQ_ON)
     {
        gl1_hw_mcpm_boost_request(L1_DED_G2L_BIMC,MCPM_BIMC_BOOST_MASK, 30000, gas_id);
        g2l_bimc_boost_done[gas_id] = TRUE;
     }
      /*Apply boost 1 frame in advance*/
      l1_sc_g2x_start_mcpm_irat_ngbr(RR_L1_IRAT_RAT_LTE, (uint8)l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode, gas_id);
    }
    else 
    {
        MSG_GERAN_HIGH_0_G("is_next_cm_srch_lte Next index not lte" );
    }

}

/*===========================================================================
FUNCTION l1_sc_get_next_ded_freq_idx

DESCRIPTION
This function checks whether next search can be on W/L/T in ded mode and returns freq index

DEPENDENCIES
  none.

RETURN VALUE
frequency index for next search going to be scheduled

SIDE EFFECTS
  None
===========================================================================*/
uint16 l1_sc_get_next_ded_freq_idx(gas_id_t gas_id)
{
 l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
 uint16 f_off,num_freqs,num_cells, i =0;

 dBx16_T  scell_avg_pwr = l1_sc_wcdma_data_ptr->serving_RLA_C;
 num_freqs = l1_sc_wcdma_data_ptr->rr_params.num_freqs;
 f_off = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;

 if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_ACTIVE)
 {
   f_off = l1_sc_wcdma_data_ptr->rr_params.num_freqs;       /* force reset in for loop */
 }

 for (i=0;i<num_freqs; i++)
 {
   f_off++; 				   /* next index to check */
   if(f_off >= num_freqs)
   {
     /* check the forced idx is meets the search criteria */
     if(l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_ACTIVE)
     {
       f_off = l1_sc_wcdma_data_ptr->rr_params.ForcedFreqIdx;
     }
     else
     {
       f_off=0; 	/* wrap around */
     }
   }
   if(l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode)
   {
     if(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_WCDMA)
     {
       num_cells = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells;
     }
     else
     {
       num_cells = 1; /* In LTE/TDS we detect cells so force */
     }

     if( num_cells && l1_sc_check_priority_threshold(f_off, gas_id) )
     {
	   break;
     }
   }
   else /*dedicated mode */
   {
     /* break on first that meets current threshold criteria */
     if(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_LTE)
     {
       if((scell_avg_pwr >= l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[1])
           &&(scell_avg_pwr <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[1]))
       {
         break;
       }
     }
     else if((l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_WCDMA)
              && (l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells > 0))
     {
       if((scell_avg_pwr >= l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[0] )
           &&(scell_avg_pwr <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[0]))
       {
         break;
       }
     }
#ifdef FEATURE_GSM_TDS
     else if(l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].rat == RR_L1_IRAT_RAT_UTRAN_TDD)
     {
       if((scell_avg_pwr >= l1_sc_wcdma_data_ptr->wcdma_srch.min_gsm_rssi_x16[0])
           &&(scell_avg_pwr <= l1_sc_wcdma_data_ptr->wcdma_srch.max_gsm_rssi_x16[0]))
       {
         break;
       }
     }
#endif
   }
  }
  return f_off;
}


/*===========================================================================

FUNCTION L1_SC_G2X_CHECK_FOR_EXTEND_SRCH_PERIOD

DESCRIPTION
  Check if search period has to be extended if current sub is non-DDS and the other sub is in connected mode.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_g2x_check_for_extend_srch_period( gas_id_t gas_id )
{
    sys_modem_as_id_e_type as_id;
    gas_id_t dds_id;
    uint8 max_threshold;
    boolean alt_sub_in_data, dr_enabled;
    
    volatile ISRTIM_CMD_BLK *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
    grm_info_t *grm_info_p = grm_get_info_ptr_from_client(l1_tskisr_blk->
    client_id, gas_id);
    
    if (!grm_info_p)
    {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       ERR_GERAN_FATAL_1_G("Invalid clientID!! gas-id = %d",gas_id);
    }
    
    as_id = geran_get_current_dds_sub_id();
    dds_id = geran_map_nas_id_to_gas_id(as_id);
    
    alt_sub_in_data = grm_is_alt_sub_in_data(gas_id);
    max_threshold = geran_nv_get_dsc_threshold_for_slow_irat();
    dr_enabled = gl1_drdsds_enabled(gas_id);
    
    MSG_GERAN_HIGH_5_G("G2X extend srch period: dds_sub %d alt_sub_in_data %d DR_enabled %d dsc_threshold %d max threshold %d",
                        dds_id, alt_sub_in_data, dr_enabled, grm_info_p->dsc_threshold, max_threshold);
    
    /* If meet 1 & 2 & 3 & 4 & 5, we extend scheduling period */
    /* 1. MSIM
    ** 2. Not DDS sub
    ** 3. SR mode
    ** 4. The alternative SUB is in DATA
    ** 5. DSC threshold == MAX threshold
    */
    if (gl1_msg_get_multi_sim_mode() && 
       (dds_id != gas_id) && 
       !dr_enabled&&
       alt_sub_in_data &&
       (grm_info_p->dsc_threshold >= max_threshold))
    {
       return TRUE;
    }
    
    return FALSE;

}
/*===========================================================================

FUNCTION l1_sc_abort_irat_at_sleep_err_recovery

DESCRIPTION
  This function resets the IRAT state when a wakeup-for-irat overruns its wakeup 
  timeline.  This state-reset is different from a standard abort in that we put
  the cur-frequency-index and page-cycle-counters to a place such that the same
  frequency will be tried again right away at the next page cycle.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_abort_irat_at_sleep_err_recovery(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /* For the frequency we just failed to search, set the page cycle number so 
     that we'll select it immediately at the next page cycle.  Also make the 
     current-frequency-index the index before the failed-search frequency so 
     we'll retry the failed frequency first at the next page cycle */
  if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM)
  {
    MSG_GERAN_HIGH_1_G("Next avail G2X RECONF search should be freq idx %d", l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset);

    l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick;

    /* subtract 1 modulo num-frequencies */
    l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset = (l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset +
                                                                   l1_sc_wcdma_data_ptr->rr_params.num_freqs - 1) %
                                                                   l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  }
  else
  {
    MSG_GERAN_HIGH_1_G("Next avail G2X ID search should be freq idx %d", l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset);

    l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick;

    /* subtract 1 modulo num-frequencies */
    l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = (l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset + 
                                                            l1_sc_wcdma_data_ptr->rr_params.num_freqs - 1) %
                                                            l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  }

  /* Set the search state back to NULL and clear the FM state.
     (It would be nice to reuse the standard abort procedure for IRAT, but we
     can't since sleep error recovery is sneaky and secretly doesn't send the
     GFW commands that the IRAT module has assumed were sent. Therefore this
     function can just put IRAT state machines back to NULL state since no
     commands have been sent to X tech or GFW yet.) */
  l1_sc_set_gtow_srch_state_null(gas_id);
}

/*===========================================================================

FUNCTION l1_sc_check_for_g2x_mcvs_deboost_req

DESCRIPTION
 If GL1 has sent MCVS request of 384 MHZ at the last frame, then check if MVCS de-boost req needs to be sent
 as no G2X search will happen on the next idle frame

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_check_for_g2x_mcvs_deboost_req(gas_id_t gas_id)
{

 l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
 
 /*if no search happening but Irat nghbor is started at mcpm, go for stop*/
 if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL)
 {
  MSG_GERAN_HIGH_0_G("FR47064:l1_check_for_redun_mcpm_start_req");
    l1_sc_g2x_stop_mcpm_irat_ngbr(gas_id);
 }

}
/*===========================================================================
FUNCTION l1_sc_g2x_start_mcpm_irat_ngbr

DESCRIPTION
This function starts MCPM for X-RAT for G2X search in ded/ptm

DEPENDENCIES
  mode 0 - ded, 1- xfer, 2 - idle

RETURN VALUE
  TRUE: If some mcpm neighbor update Start Request is queued to DPC Task
  FALSE: If not queued

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_sc_g2x_start_mcpm_irat_ngbr(rr_l1_irat_rat_e srch_rat, uint8 mode, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint8 scenario = 0;
  boolean queued_to_dpc = FALSE;

  switch(srch_rat)
  {
    case RR_L1_IRAT_RAT_LTE:
    {
      if(g2l_active_mcpm_scenario[gas_id] == 0)
      {
        /*For G2L search in dedicated/PTM, request for 940Mhz Q6, 232 mpps*/
        scenario = gl1_mcpm_irat_scenario[RR_L1_IRAT_RAT_LTE-1][mode];
        g2l_active_mcpm_scenario[gas_id] = scenario; 
        gl1_drx_task_active(gas_id);
        gl1_hw_mcpm_neighbor_update(MCPM_G2L_MEAS_START,g2l_active_mcpm_scenario[gas_id], gas_id);
        queued_to_dpc = TRUE;
      }
    }
    break;

    case RR_L1_IRAT_RAT_WCDMA:
    {
      if(g2w_active_mcpm_scenario[gas_id] == 0)
      {
        if((mode ==GL1_MODE_XFER) && (gl1_check_no_dl_rx_slots(gas_id)))
        {
          /*For G2W search in PTM when Rx slots >3, apply boost for 595Mhz */ 
          g2w_active_mcpm_scenario[gas_id] = L1_PTM_HMSC_G2W;
        }
        else if((mode ==GL1_MODE_XFER) && (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID_IN_TBF))
        {
          /*For G2W ID search during TBF suspension in Xfer mode */ 
          g2w_active_mcpm_scenario[gas_id] = L1_PTM_TBF_G2W;
        }
        else
        {
          /*For G2W search in Dedicated mode, apply boost for 384 Mhz, 147 mpps */
          scenario = gl1_mcpm_irat_scenario[RR_L1_IRAT_RAT_WCDMA-1][mode];
          g2w_active_mcpm_scenario[gas_id] = scenario;
        }
        gl1_drx_task_active(gas_id);
        gl1_hw_mcpm_neighbor_update(MCPM_G2W_MEAS_START, g2w_active_mcpm_scenario[gas_id], gas_id);
        queued_to_dpc = TRUE;
      }
    }
    break;

    case RR_L1_IRAT_RAT_UTRAN_TDD:
    {
      if(g2t_active_mcpm_scenario[gas_id] == 0)
      {
        /*For G2T search in Dedicated/Data mode, apply boost for 384 Mhz */
        scenario = gl1_mcpm_irat_scenario[RR_L1_IRAT_RAT_UTRAN_TDD-1][mode];
        g2t_active_mcpm_scenario[gas_id] = scenario; 
        gl1_drx_task_active(gas_id);
        gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_START,g2t_active_mcpm_scenario[gas_id], gas_id);
        queued_to_dpc = TRUE;
      }
    }
    break;

    default:
    break;
  }

  return queued_to_dpc;
}


/*===========================================================================
FUNCTION l1_sc_g2x_stop_mcpm_irat_ngbr

DESCRIPTION
This function stops MCPM for X-RAT for G2X search in ded/ptm

DEPENDENCIES
  none.

RETURN VALUE
  none

SIDE EFFECTS
  None
===========================================================================*/
void l1_sc_g2x_stop_mcpm_irat_ngbr(gas_id_t gas_id)
{ 
  l1_sc_globals_T 	*l1_sc_globals_ptr	  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  if((g2l_active_mcpm_scenario[gas_id] != 0) && (l1_sc_globals_ptr->g2l_shutdown_pending == FALSE) &&
      !(gl1_hw_g2x_send_deinit_req[gas_id] & (1 << RR_L1_IRAT_RAT_LTE)) && !(gl1_hw_g2x_send_init_req[gas_id] & (1 << RR_L1_IRAT_RAT_LTE)))
  { 
    gl1_drx_task_active(gas_id);
    gl1_hw_mcpm_neighbor_update(MCPM_G2L_MEAS_STOP, g2l_active_mcpm_scenario[gas_id], gas_id);
    g2l_active_mcpm_scenario[gas_id] = 0; 
  }

  if((g2w_active_mcpm_scenario[gas_id] != 0) && (l1_sc_globals_ptr->g2w_shutdown_pending == FALSE) &&
      !(gl1_hw_g2x_send_deinit_req[gas_id] & (1 << RR_L1_IRAT_RAT_WCDMA)) && !(gl1_hw_g2x_send_init_req[gas_id] & (1 << RR_L1_IRAT_RAT_WCDMA)))
 {
    gl1_drx_task_active(gas_id);
    gl1_hw_mcpm_neighbor_update(MCPM_G2W_MEAS_STOP,  g2w_active_mcpm_scenario[gas_id], gas_id);
    g2w_active_mcpm_scenario[gas_id] = 0; 
  }
#ifdef FEATURE_GSM_TDS

  if((g2t_active_mcpm_scenario[gas_id] != 0) && (l1_sc_globals_ptr->g2t_shutdown_pending == FALSE) &&
      !(gl1_hw_g2x_send_deinit_req[gas_id] & (1 << RR_L1_IRAT_RAT_UTRAN_TDD)) && !(gl1_hw_g2x_send_init_req[gas_id] & (1 << RR_L1_IRAT_RAT_UTRAN_TDD)))
  {  
    gl1_drx_task_active(gas_id);
    gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_STOP,  g2t_active_mcpm_scenario[gas_id], gas_id);
    g2t_active_mcpm_scenario[gas_id] = 0; 
 }
#endif
}

/*===========================================================================
FUNCTION l1_sc_irat_inc_dec_g2x_abort_count

DESCRIPTION
If WCDMA has aborted the gap, increment XRF error count, after 3 such consecutive errors
MCPM has to be informed about CRITICAL situation at start of next gap
for idle/ded/xfer all modes.

DEPENDENCIES
  none.

RETURN VALUE
  none

SIDE EFFECTS
  None
===========================================================================*/
void l1_sc_irat_inc_dec_g2x_abort_count(boolean gap_aborted, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    gas_id_t other_g_gas_id= OTHER_G_SUB(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  if(gap_aborted)
  {
    if( l1_sc_globals_ptr->active_irat == RR_L1_IRAT_RAT_WCDMA )
    {
      l1_sc_wcdma_data_ptr->g2x_aborted = TRUE;
    }
    /*Don't increment g2x_abort_count counter if BPLMN search is ongoing on other Sub*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if(other_g_gas_id < NUM_GERAN_DATA_SPACES && !l1_get_background_wcdma_plmn_scan_active(other_g_gas_id))
#endif
    {

      l1_sc_wcdma_data_ptr->g2x_abort_count++;
    }
    l1_sc_wcdma_data_ptr->g2x_success_count = 0;
  }
  else
  {
    l1_sc_wcdma_data_ptr->g2x_success_count++;
  }
  
  MSG_GERAN_HIGH_2_G("l1_sc_irat_inc_dec_g2x_abort_count g2x_abort_count %d, g2x_success_count %d ",
                                      l1_sc_wcdma_data_ptr->g2x_abort_count,l1_sc_wcdma_data_ptr->g2x_success_count);

  if(l1_sc_wcdma_data_ptr->g2x_success_count >= G2X_GAP_SUCCESS_COUNT )
  {
    /*Reset abort count after 3 successful gaps*/
    l1_sc_wcdma_data_ptr->g2x_abort_count = 0;
    l1_sc_wcdma_data_ptr->g2x_success_count = 0;
  }

  if(l1_sc_wcdma_data_ptr->g2x_abort_count>=G2X_MAX_GAP_ABORT_COUNT)
  {

#ifdef FEATURE_QSH_MDUMP
       QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
       ERR_GERAN_FATAL_0_G("Crash for Debug: G2X getting aborted continuosuly");
  }
}

void l1_sc_irat_reset_g2x_abort_count(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  l1_sc_wcdma_data_ptr->g2x_success_count = 0;
  l1_sc_wcdma_data_ptr->g2x_abort_count = 0;
  
}

uint8 l1_sc_irat_get_g2x_abort_count(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  return(l1_sc_wcdma_data_ptr->g2x_abort_count);
}
/* EOF */
