
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              SCE: MAIN

GENERAL DESCRIPTION
   This module contains:
   - initialization and shutdown of the SCE and modification of it's states
   - abort condition handling
   - change, sort and inquiry of BA list
   - callback handling (callbacks from monscan)

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_main.c#3 $
$DateTime: 2021/06/28 08:40:41 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
30/01/20   ktg       CR2589703 Change to handle continuous fcch retries on ncell when rssi fluctuations are seen
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
26/05/17   mk        CR2020690 Wait for 2 complete SACCH periods before scheduling NCell FCCH and SCH after channel transition in dedicated mode
10/05/17   sk        CR2024440 Add an explicit intlock for DivRx meas buffer update
22/01/18   sid       CR2175231 While moving from IDLE to DEDICATED mode, update SC next tick value based on current mode instead of previous mode.
04/05/17   sp        CR2025199: Carry forward next NCELL SB reconfirmation time between state changes - Enhancement
27/03/17   sk        CR2021952 Do not reset the SeNS flag when inside the dedicated mode if BSIC was known and 
                     reset the BSIC known flag while leaving dedicated mode if SeNS flag was set
11/08/17   rc        CR2138578  ML logging - GL1 Changes
06/12/17   km        CR973889 Idle Timeline Opt - Sleep between PCH and IRAT
06/07/17   mk        CR2071218 FR44121 -Synchronous Ncell Quality Monitor - synq algo
14/02/17   mk        CR2004610 : FR 40251 (SeNS) - Skip reporting NCELL if SCH reconfirmation failed with good GDET score
03/01/16   mn        CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
06/10/16   sk        CR1005973 Do not report Ncell that in top 10 and if SCH is not re-confirmed in the last 30 seconds
23/09/16   mk        CR1066877 Check if BA list cell has valid info before overwrting from previous cell info blindly
26/05/16   sp        CR956607: Deblaclist Neighbour cells if serving cell is going bad.
06/11/15   ab        CR935174 : Perform G IDLE Freq after all RX done to avoid X desense the G PCH RX.
14/10/15   hd        CR915037 Don't inform a surround cell to GRR as soon as FCB starts failing
05/10/15   sp        CR898457 Ignore next tick req for SCH/FCCH state configurations in next_cell_bcch
29/09/15   br        CR904002 Adjust freq error on ncells after cell reselction
24/08/15   zf        CR895041 Apply mutex protection in l1_sc_stop
30/07/15   am        CR882081 Ensure SCH reconfirmation happens as specified by 3GPP
22/07/15   nm        CR872802 Adjust fn lag when moving from transfer to DTM
13/07/15   ab        CR832798 FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
29/06/15   am        CR861464 Ensure fn_when_surround_tick_changed is up-to-date
20/02/15   ab        CR773948 GL1 Support for provding Dumps and Metrics for QSH
25/02/14   cah       CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good serving cell conditions
24/02/15   am        CR799427 Ensure serving cell when added to BA list has correct frequency error
24/02/15   ak        CR792387 Correctly report sorted cells to RR on BA List update
16/02/15   nm        CR789023 start SCE in SINGAL_BLOCK CONFIG as well for correct FN lag adjustment for ncells
19/02/15   mko       CR790480 Don't call compute_ccch_event_qbit_lag() if timeslot is not changed
12/02/15   am        CR771762 BCCH Decode Failures at Good RSSI when the NCell is drifted by 1.5KHz
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs.
26/01/15   ak        CR783606 Return correct BSIC to RR for old serving cell on reselection
22/12/14   cjl       CR774983 send ncell BCCH SU fail to RR when we cancel a scheduled BCCH in sc_balist_reset_counters
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
03/12/14   npt       CR759467 Fix compiler warnings
03/12/14   pjr       CR765326  Ensure SCE is not restarted after connected mode hopping if
                               previously stopped
27/11/14   ak        CR748234 Problems with legacy SCE Opt 16/17/20
20/10/14   mko       CR741124: Ensure L1 task always have intlock for new_ba_list_pending (l1_sc_main_data)
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
08/08/14   og        CR690180. Dual space the multiband cell array.
08/08/14   og        CR688965. Adjust the ncell count out of range if( ) check.
19/05/14   og        CR663535. Reduce cell array allocations on the stack in
                     l1_sc_balist_multiband_sort( ).
03/04/14   cja       CR641428 Correct serving cell qbit lag for ccch_group when add to ba list
19/12/13   jk        CR503359 - GL1 changes to support Rx diversity
08/10/13   sp        CR561007: Start SC meas based on mode
15/08/13   sk        CR524039 GBTA bringup changes
25/06/13   cs        Major Triton TSTS Syncup
13/06/13   sai       CR495383: Add support for therm read in DSDS mode.
06/06/13   cs        TSTS Updates
26/03/13   cja       CR448231 Correct CR398356 for trans mode NCell SCH outside gap
07/11/12   ss/cgc    CR404843: DSDS version check cell ptr l1_sc_balist_check_cell() is within pool address range
29/01/12   ss        CR398356  Adjust qbit lag and fn lag when qbit lab is negetive or greater than 5000QS
28/11/12   br        CR383165 Boundary checks to fix Klockwork errors
03/09/12   zf/npt    CR339131: blacklist Ncell which fails FCB receive - Opt17
16/08/12   zf        CR390228: Featurize Opt 20.
13/08/12   og        CR366888: Ensure the copying of BA list updates is mutex protected.
13/08/12   ss        CR380614: clear buffer and set buf_len while reinitialise the ba.cell_pool meas_buf pointers to their 'fixed' values
28/06/12   zf        CR318881: Preserve serving cell BSIC.
22/05/12   og        CR362773: extend mutex protection around BA structure access
23/04/12   pg        CR314929: Reduce F3 for BA list update
11/01/12   ap        CR329579 with FEATURE_GERAN_MEM_OPT prevent deferencing NULL ptr, l1_sc_main_data_ptr.
28/04/11   pg        Fix compiler warnings
08/04/11   og        CR280835. Add mutex protection as a replacement for the fix of CR276511.
29/03/11   cja       CR 218213. Do not clear tone_detected if bsic_known.
02/03/11   og        Work around for CR276511. Sanity check a local copy of the ba.cells[n]
                     pointer.
22/11/10   ky        Panic reset mechanism is changed. For all errors gl1 first
                     sees an nv item set or not. if this nv item is set then does
                     error fatal immediately other wise gl1 sends signal to grr for
                     recovery
25/06/10   pg        Move L1_SC*BUF_LEN* defs and ba struct to l1_sc.h and remove static
                     from ba and ra_bufs for Q6 (required for CR244125)
18/06/10   cja       In data transfer adjust qbit lag if tone detected (not just if BSIC known)
16/06/10   ws        Fix compile error with CR 242521 move BA_CELL_POOL_LEN  to l1_sc.h
16/09/09   ps        Fix Klocwork ABR errors
09/09/09   cs        Fix Klocwork bounds checking for sorted _cells
17/04/09   nt        CR179739 - DTM/CS uses different acq window sizes
09/04/09   og        Seperate the dedicated and transfer SCH scheduling decisions
                     CR174438.
23/10/08   cs        Show arfcn when we map freq error for better debug
02/10/08   cs        Lint fixes
28/07/98   hv        Added KxMutex support
28/04/08   nt        CR144860: - Made the multiband sort ignore the cell->try_no
24/04/08   og        Esnure the frequency offset value is set to zero if valid afc
                     information is not available when handing over.
20/02/08   og        CR139102-Ensure the TCH->TCH non-blind handover uses the frequency
                     error for the target cell before camping on it.
26/10/07   og        Added unused_cells[] buffer overflow protection, resolves
                     CR131116 / SR991451.
17/10/07   og        Ensure the wcdma g2w srch is cleared correctly when leaving
                     DTM mode. Resolves CR130313.
20/08/07   og        Fix to SR952099 / CR126426. Fix the use of the current/new ba
                     lists and addition of pptr / ptr NULL checks. Merge from 83.01.
12/07/07   ws        Fixed klocwork error Suspicious dereference of pointer 'cell'
                     before NULL check
11/07/07   og        Filter adjacent arfcns according to their rx power delta in
                     idle ba list. Resolves CR122895.
06/06/07   ws        CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                     is called when we reselect to PBCCH cell stopping NC2 reporting
05/03/07   cs        Making CR112989 calculation consistent with sc updates
28/02/07   nt        CR112989: Turned on FEATURE_GSM_TRANSFER_DED_SERVING_CELL_INFO, by
                     removing switch from around the code.
28/02/07   nt        CR112987: Removed the check of cell->try_no==0 before reporting
                     to RR.
05/02/07   nt        CR:109831 If we have performed a LAU on PBCCH, then this combination is
                     possible, and requires handling. Otherwise on return to PBCCH the g2w
                     drivers will think that they are in dedicated mode.
24/01/07   nt        109818:Added l1_sc_check_for_ncc_permitted_update to update the ncc
                     permitted if the BAlist has not changed but ncc_permitted has
08/12/06   dv        CR96461:No 3G measurements scheduled after entering packet Idle
20/09/06   nt        CR100061:Rework of l1_sc_balist_multiband_sort() function
                     to re-rank the BSICs of the ncells according to multiband rules.
24/08/06   nt        CR97167: Removed the multiband sort from IDLE and TRANSFER
                     this now only takes place in CS dedicated modes.
14/08/06   ws        Allow dedicated to be timeslot slammed to lowest received tn
                     and also to place the CS timeslot on TN 2
05/06/06   nt        CR78045: Total code rework for multiband reporting within
                     the function l1_sc_balist_multiband_sort();
02/01/06   ip        CR85687 & CR85686 FCH and SCH corrections to receiver window
                     size in DTM mode
26/01/06   og        Neighbour cell acquisition changes to handle extended dynamic.
23/01/06   og        Removal of QLint high errors.
17/11/05   nt        Added setting of first_rx_tn=2 and last_tx_tn=5 for calls to
                     gl1_msg_ncell_sch_pos_in_acq_window under FEATURE_GSM_DTM.
                     use by the function call gl1_msg_rx_ncell_fcch().
02/11/05   nt        Added L1SCModeDediDTM into l1_sc_ba_cell_set_sync
31/08/05   nt        Added the DTM sce mode into the switch statement within the
                     l1_sc_report_period_done() function.
08/12/05   ws        Added initial FEATURE_GSM_DTM support
06/16/05   sv        Fixed lint error due to header file include.
06/13/05   yh        Implementing qsort.
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
04/05/05   nt        Set the default for the cell->sb_count_to_bcch = BCCH_DONT_DO
                     within the function l1_sc_balist_new_cell.
01/26/05   sv        Add support for Frame Manager.
25/01/05   og        Fix to pkt transfer <-> idle Ncell qbit/fn lag calculations.
01/05/05   dp        Merged dedicated timeline changes for audio optimizations
                     to mainline. Lint cleanup
08/12/04   kf        Canges for reselection in transfer.
10/28/04   ws        Downgraded Debug msgs relating to frame slamming
10/13/04   kf        Changes for time slot slamming in transfer.
08/26/04   kf        Removed l1_sc_balist_gprs_set_sync for ts slamming.
07/14/04   gw        Added call to gpl1_gprs_sc_trans_post_sort_process().
                     Moved call to l1_sc_post_process() after call to
                     l1_sc_balist_multiband_sort(). Added check on number of
                     sorted cells in l1_sc_balist_multiband_sort().
07/21/04   bk        Merged in changes for Egprs Qual measurements handling.
06/12/04   gfr       Control slew rate limiting in PL1.
06/30/04   hg        Added missing featurization around GtoW code
06/22/04   dp        Lint cleanup
06/22/04   gsc       Removed a F3 message.
06/21/04   gsc       Added l1_sci_ok_to_schedule_cbch/l1_sci_abort_ncell_activity
                     functions. Updated aborter to handle conflicting priorities
                     between the NCELL and Serving Cell activities.
06/21/04   dp        Changes to incorporate multiband reporting when sorting
                     neighbor cell list
05/26/04   sv        Lint Cleanup.
05/24/04   ws        corrected setting of highest_tn in l1_sc_balist_gprs_set_sync()
05/14/04   gw        Changes to support ccch_group != 0.
03/19/04   dp        Fix to not include a cell in the measurement report once
                     a BSIC reconfirm fails and before it is flagged at lost
02/25/04   gw        Lint clean up.
01/14/04   gw        Added check for NULL pointer in l1_sc_balist_add_serving_cell.
11/05/03   gfr       Support for quad-band.
09/24/03   tb        Removed references to fcch_decode_failures.
09/23/03   gw        Modified l1_sc_balist_add_serving_cell() to prevent duplicate
                     BA list entries.  Added code to detect duplicate BA list
                     entries under DEBUG_BALIST_CHECK_FOR_DUPS.
09/12/03   tb        Added initialization of fcch_decode_failures.
09/02/03   gw        Changed l1_sc_balist_transfer_power_info() to set meas_state
                     to the appropriate value when transfering power info.
08/31/03   gw        Don't reset the sorted_cell_count when changing modes or
                     updating the BA list.
08/28/03   dp        Surround cell optimizations
08/27/03   gw        Increased size of BA cell pool to ensure there is always
                     space to add serving cell.  Added some extra debug messages
                     when BA list is changed.  Changed handling of case when
                     adding cell but there are no unused cells.
08/24/03   dp        Fixed bug in ranking cells in l1_sc_balist_sort
08/15/03   dp        Initialized prev_rx_power_sum and prev_rx_power_count
29/07/03   kf        Added case L1SCModeGprsIdle: to l1_sc_balist_set_sync.
07/17/93   ws        Added FN to msg_med in l1_sc_start() and l1_sc_stop()
07/15/03   bk        Added a check in l1_sci_go() to catch the case in where l1_sc_start()
                     is followed by l1_sc_stop() and then followed by l1_sci_monscan_go()
07/14/03   gw        Removed unneeded ASSERTs.  Enabled one needed ASSERT.
07/14/03   gw        Added support for active field in l1_sc_globals.
07/11/03   gfr       Lint cleanup.
07/11/03   gw        Changes to speed up reselection.
07/09/03   bk        Got rid of merge error
07/08/03   bk        Cleaned up the function that is used to calculate the position of
                     SCH when BSIC info is transferred.
07/08/03   gw        GRR/L1 SCE cleanup. Changes to support GRR scheduled ncell BCCH decodes.
06/23/03   gfr       Compiler warning cleanup.
06/19/03   gw        Added more debug messages to SCE start/stop functions.
06/06/03   tb        Support for dedicated mode gtow searching.
06/03/03   gw        Changes to make updating of BA list more robust.  Added
                     support for detecting when ncells enter/leave to top 6.
05/29/03   gw        Added check for existing new BA list in function
                     l1_sc_balist_prepare_new().
05/23/03   ws        Updated MSG level for BSIC TRANSFER feature
05/12/03   dp        Modified l1_sc_balist_gprs_set_sync to handle
                     timeslot reconfig
04/29/03   dp        Changes to preserve Ncell BSIC across transitions
05/02/03   gw        Merged in power transfer changes from branch.
05/01/03   ag        Updates to ncell pbcch reading
04/28/03   gw        Minor fixes to code for preserving power measure info.
09/04/03   ws        Increased L1_SC_MAX_MEAS_BUF_LEN_GPRS
04/09/03   gw        Improved mechanism for adding serving cell to BA list on
                     reselection.  Removed some unneeded code.
04/08/03   gw        Added debug messages.  Removed l1_sc_balist_get_new_cells().
                     Modified l1_sc_balist_get_cells() to return new BA list if
                     one is pending.
04/04/03   gw        Changes to support transferring ncell power info between
                     modes.  Also added new debug messages and removed obsolete
                     debug code.
04/02/03   gw        Changes to reflect modifications in monscan module.
28/02/03   kf        Changes for transfer measurements
17/02/03   kf        Changes for running average in transfer.
01/28/03   tb        Added l1_sci_check_ok_to_sleep().
01/10/03   gw        Clean up / re-organisation of BA list sub-module.  Added
                     QC style function header comments.
12/03/02   mk        General GPRS featurization cleanup, added GPRS state vars
                     init to l1_sc_init(), merged l1_sc_mon_scan_done_idle()
                     for GSM/GPRS.
11/27/02   ws        Updated GPRS baseline
11/25/02   mk        Updated GPRS baseline.
11/21/02   mk        Deleted unsupported code under FEATURE_GSM_ALTERNATE_PLMN_SEARCH.
11/18/02   mk        Updated GPRS baseline.
11/15/02   dp        Fix to trigger sending up surround_meas_ind even when
                     BA list is empty.  Added support for multiband
                     reporting
11/03/02   tb        Added support for INTERRAT_GTOW
10/08/02   kf        Added PL1 GPRS measurement code hooks under switch
                     FEATURE_GPRS_MEAS.
10/01/02   ATM       Added central get and set FN routines
09/12/02   dp        Cleaned up unused code
08/23/02   plm       Removed local variable count in the function
                     l1_sc_mon_scan_done_idle() which was declared by not used
                     to eliminate compiler warning.  Added void argument to the
                     function l1_sc_rank_period_done_ded() to eliminate compiler
                     warnings.
08/12/02   gw        Featurized code for alternate PLMN searching.
07/26/02   dp        Support for a different averaging period to sort the cells
                     in dedicated mode.  Averaging is done over 2 SACCH
                     reporting periods as mandated in the specs.  Also fixed
                     some surround cell bugs, fn_when_lost (*26) and set
                     tone_detected = FALSE in new_cell()
07/15/02   dp        Support to share Ncell info across dedicated modes
07/02/02   gw        Added code to manage running averages of ncell
                     measurements in Idle Mode.
06/27/02   dp        Changed default BCCH mask from BCCH_MASK_34 to BCCH_MASK_3
06/27/02   gw        Changes to support running averages of ncell power
                     measurements in Idle Mode.  Added function
                     l1_sc_mon_scan_done_idle.
05/30/02   pjr/mk    Added PL1 GPRS code hooks under switch FEATURE_GSM_GPRS_L1.
05/10/02   JC/gw     Changes s.t all cellsin BA are logged.
04/21/02   dp        Initialized cell->fn_when_lost in l1_sc_reset_cell_counters
04/17/02   ATM       Removed obsolete call to trace_meas_rpt
04/01/02   ATM       Added calls to NCELL logging
03/26/02   dp        Fixed bug in sort_ba_list.  Use
                     SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 to flag an entry
                     as minimum power(instead of 0)
03/20/02   JC        Added QCT header blocks to several functions amd modified
                     new_cell to support AGC. Included gl1_sys_xxx.h files.
02/21/02   dp        Initialized cell->idle_frames_to_sb in reset_cell_counters
02/20/02   gw        Fixed bug in l1_sc_start.
02/07/02   gw        Changes to improve starting and stopping of SCE.
12/21/01   gw        Changes to support DRX scheduling.  Added code to notify
                     DRX manager when a callback to the task is made in Idle
                     Mode.
12/18/01   dp        Modified l1_sc_reset_counters to reset the surround tick
10/24/01   dp        Modified l1_sc_reset_cell_counters to reset bsic_known
                     so that SC info on idle and dedicated mode is decoupled
                     (Temporary fix). Mainlined OLD_SCE for dedicated mode
10/16/01   gw        Mainlined feature OLD_SCE.  Changed CCWW types to QCT
                     types.  Added function l1_sci_get_available_frames().
09/25/01   mk        Using OLD_SCE for non-supported code.
08/31/01   JC        Removed refs to obsoleted compile switch.
08/16/01   mk        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "l1_drx.h"
#include "l1_isr.h"
#include "l1_log.h"   /* For Logging */
#include "l1_sc.h"
#include "l1_sc_irat.h"
#include "l1_utils.h"
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
#include "ms.h"
#include "geran_msgs.h"
#include "l1_fm.h"
#include "l1_task.h"
#include "l1_sc_int.h"
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_task.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"

#include "gpl1_gprs_task.h"

#ifdef FEATURE_GSM_WTR_HOP
#include "gl1_msg_wtr_hop.h"
#endif

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include <stdlib.h>     /* For qsort */


#include "mcfg_nv_api.h"
#define TICKS_PER_FCB_DEBLACKLIST_CELLS         4   /* 1 sec */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

boolean l1_sc_balist_check_cell( gas_id_t gas_id, cell_T*  cell_ptr);


static int compare_band( const void *arg1, const void *arg2 );
 

static void qsort_band(  ARFCN_T *loc_band , uint8 num);


/*
 * Prototypes for local Functions
 * ------------------------------
 */

static void    l1_sc_post_sort_process( gas_id_t gas_id );

/* BA list */
static cell_T* l1_sc_balist_new_cell( ARFCN_T arfcn, gas_id_t gas_id );
static void    l1_sc_balist_delete_cell( cell_T* cell, gas_id_t gas_id );
static void    l1_sc_balist_set_sync( uint32 frame_decrease, int16 qbit_decrease, gas_id_t gas_id );
static void    l1_sc_balist_update_serving_cell( gas_id_t gas_id );
#ifdef DEBUG_BALIST_CHECK_FOR_DUPS
#error code not present
#endif

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST_DEBUG
static void l1_sc_dbg_set_bmask_reduced_ncell_list( uint8 bmask_bef, uint8 reduced_ncell_bmask, gas_id_t gas_id );
static void l1_sc_dbg_clear_bmask_reduced_ncell_list( uint8 bmask_bef, uint8 reduced_ncell_bmask, gas_id_t gas_id );

#define L1_SC_DBG_REDUCED_NCELL_LIST_SET_BMASK(bmask_bef, reduced_ncell_bmask, gas_id) \
        l1_sc_dbg_set_bmask_reduced_ncell_list(bmask_bef, reduced_ncell_bmask, gas_id)
#define L1_SC_DBG_REDUCED_NCELL_LIST_CLEAR_BMASK(bmask_bef, reduced_ncell_bmask, gas_id) \
           l1_sc_dbg_clear_bmask_reduced_ncell_list(bmask_bef, reduced_ncell_bmask, gas_id)
#else  /* #ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST_DEBUG */
#define L1_SC_DBG_REDUCED_NCELL_LIST_SET_BMASK(bmask_bef, reduced_ncell_bmask, gas_id)
#define L1_SC_DBG_REDUCED_NCELL_LIST_CLEAR_BMASK(bmask_bef, reduced_ncell_bmask, gas_id)
#endif /* #ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST_DEBUG */


extern void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id );
extern boolean L1_background_HPLMN_search_in_progress( gas_id_t gas_id );

/*
 * Local  Defines
 * --------------
 */
extern dBx16_T* l1_get_idle_serv_meas_buf( gl1_power_meas_buffer_type prx_drx_buffer, 
                                           gas_id_t gas_id );

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
extern void gpl1_gprs_compute_ccch_event_qbit_lag(gas_id_t gas_id);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

#define TEN_SECONDS  21

typedef struct
{
   uint32 fn;
   uint32 cnt;
} l1_running_avg_meas_buf_err_T;

typedef struct
{
    cell_T                   cell_pool[ 2 * BA_CELL_POOL_LEN ];
    cell_T*                  cell_lists[ 2 ][ BA_CELL_POOL_LEN ];

    cell_T*                  unused_cells[ 2 * BA_CELL_POOL_LEN ];
    byte                     unused_cell_count;

    ARFCN_T                  serving_cell_arfcn;
    byte                     ncc_permitted;

    /* cells in current BA and their count */

    cell_T**                 cells;
    byte                     cell_count;

    /* cells in new BA and their count */

    cell_T**                 new_cells;
    byte                     new_cell_count;
    byte                     new_ncc_permitted;
    boolean                  new_ba_list_pending;
    uint8                    multiband_reporting;

    /* serving cell info for Idle->Idle transitions */
    /* NOTE 1: No running avg buffer is allocated to
     * this cell.
     * NOTE 2: Resetting of fields in this cell is
     * very sloppy.  If you want something reset
     * make sure you do it yourself.
     */
    boolean                  serving_cell_valid;
    cell_T                   serving_cell;
    ARFCN_T*                 pre_scell_arfcn;

    /* IDs to make sure results reported by
     * monscan correspond to a current BS list.
     */
    uint8                   old_id;
    uint8                   new_id;
} ba_T;

/*
 * Global Variable Declarations
 * ----------------------------
 */
l1_sc_globals_T  l1_sc_globals_store[NUM_GERAN_DATA_SPACES];

extern dedicated_data_T l1_dedicated_data[];

typedef struct
{
  /* Running average buffers.  These are assigned to
   * ba's by l1_sc_balist_init().
   */
  dBx16_T ra_bufs[ 2 * BA_CELL_POOL_LEN ][ L1_SC_MAX_MEAS_BUF_LEN ];

  ba_T ba;
  boolean sce_running;
  l1_running_avg_meas_buf_err_T l1_running_avg_meas_buf_err;
} l1_sc_main_data_t;

static l1_sc_main_data_t  l1_sc_main_data[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION gl1_ms_switch_l1_sc_main_data

DESCRIPTION
             Only used for Dual SIM/Dual data space.  Switches the working
             data pointer to the data space indicated by gas_id.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
l1_sc_main_data_t* gl1_ms_switch_l1_sc_main_data(gas_id_t gas_id)
{
  return ( &l1_sc_main_data[gas_id] );
}

/*===========================================================================

FUNCTION gl1_ms_switch_l1_sc_globals_store

DESCRIPTION
             Only used for Dual SIM/Dual data space.  Switches the working
             data pointer to the data space indicated by gas_id.

PARAMS       gas_id - geran access stratum id.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/
l1_sc_globals_T  * gl1_ms_switch_l1_sc_globals_store(gas_id_t gas_id)
{
  return ( &l1_sc_globals_store[gas_id] );
}



/*
** Check address bounds of cell pointer
*/

boolean l1_sc_balist_check_cell(  gas_id_t gas_id, cell_T*  cell_ptr)
{

#ifndef DISABLE_BA_CELL_CHECK

    uint16 lower_limit,upper_limit;
    cell_T*  cell_pool_lower_addr_ptr;
    cell_T*  cell_pool_upper_addr_ptr;

    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    lower_limit = 0;
    upper_limit = (2 * BA_CELL_POOL_LEN) - 1;

    /* check is the obtained cell_ptr is within the address range of the cell_pool */
    cell_pool_lower_addr_ptr = &(l1_sc_main_data_ptr->ba.cell_pool[lower_limit]);
    cell_pool_upper_addr_ptr = &(l1_sc_main_data_ptr->ba.cell_pool[upper_limit]);


    if( (cell_ptr >= cell_pool_lower_addr_ptr) && (cell_ptr <= cell_pool_upper_addr_ptr) )
    {
       return TRUE;
    }
    else
    {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_3_G(" cell_ptr : 0x%x is out of cell_pool range: lower=0x%x upper 0x%x ",cell_ptr, &(l1_sc_main_data_ptr->ba.cell_pool[lower_limit]), &(l1_sc_main_data_ptr->ba.cell_pool[upper_limit]) );
    }
#endif /*DISABLE_BA_CELL_CHECK*/

    return FALSE;

}


/**********************************************************************
 *
 *  Module:   SCE main
 *  -------
 *  Handles starting, stopping and changing mode of SCE.
 *
 **********************************************************************/

/*===========================================================================

FUNCTION l1_sc_init

DESCRIPTION
  Initialise the surround cell engine.
  Sets the mode to L1SCModeNull.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_init( gas_id_t gas_id )
{   
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

    gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr =
        gl1_ms_switch_meas_mode_data(gas_id);
    gpl1_gprs_meas_mode_ptr->allowed_to_go = FALSE;
    l1_sc_globals_ptr->autonomous_bcch     = FALSE;
    gpl1_gprs_meas_mode_ptr->leave_trans_adjust_sync = FALSE;
    gpl1_gprs_meas_mode_ptr->enter_trans_adjust_sync = FALSE;
    l1_sc_globals_ptr->mode      = L1SCModeNull;
    l1_fm_init(gas_id);
    l1_sc_receive_init(gas_id);
    l1_sc_fcb_init(gas_id);
    l1_sc_sb_init(gas_id);
    l1_sc_bcch_init(gas_id);
    l1_sc_monscan_init(gas_id);
    l1_sc_balist_init(gas_id);
    l1_sc_wcdma_init(gas_id);

    l1_sc_main_data_ptr->sce_running = FALSE;
    l1_sc_main_data_ptr->l1_running_avg_meas_buf_err.fn = 0;
    l1_sc_main_data_ptr->l1_running_avg_meas_buf_err.cnt = 0;

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    /* disable sce top-2 optimisation on init */
    l1_sc_clear_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_ENABLED, gas_id);
    if( (!mcfg_gcf_nv_get_status())                                        /* not gcf */
     && (!(gl1_hw_efs_get_debug(gas_id) & GL1_EFS_DEBUG_DISABLE_REDUCED_NCELL)) ) /* dbg flag is not set */
    {
       /* enable top-2 mode if not in gcf test mode, and feature not disabled by EFS */
       l1_sc_set_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_FEATURE_EN, gas_id);
    }
#endif

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif
}

/*===========================================================================

FUNCTION l1_sc_shutdown

DESCRIPTION
  Finally shutdown and free all resources.
  Calling l1_sc_shutdown() followed by l1_sc_init() completely resets
  the engine, but that shuld not be needed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_shutdown( gas_id_t gas_id )
{
    l1_sc_bcch_shutdown(gas_id);
    l1_sc_sb_shutdown(gas_id);
    l1_sc_fcb_shutdown();
    l1_sc_receive_shutdown();
}

/*===========================================================================

FUNCTION l1_sc_start, l1_sc_stop

DESCRIPTION
  These functions are used to start and stop the
  SCE and also to control the mode.

  The mode is set by l1_sc_start.  This function
  starts the SCE processing for the given mode.
  l1_sc_start may also perform final clean up of
  a previous mode.

  l1_sc_stop will abort all SCE operations.  It
  does not change the mode so l1_sc_start may be
  used to restart in the same mode.  Also
  l1_sc_stop does not completely clean up dedicated
  mode.  This is done so that a stop-start sequence
  can be used to transition between SDCCH and TCH
  operation (or vice-versa) without completely
  leaving dedicated mode.

DEPENDENCIES
  SCE must have been initialized by calling l1_sc_init().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_start( l1_sc_mode_T mode, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    l1_sc_mode_T prev_sc_mode            = l1_sc_globals_ptr->mode;

    MSG_GERAN_HIGH_3_G("Preparing to start SCE (%d->%d).[%d]", l1_sc_globals_ptr->mode, mode, gl1_get_FN( gas_id ));

#ifdef FEATURE_GSM_DTM
    /*Mode switching from DTM to Dedicated whilst the SCE is running is permitted*/
    if( l1_sc_main_data_ptr->sce_running &&
      ((l1_sc_globals_ptr->mode == L1SCModeDediDTM) && ( mode == L1SCModeDediTCh)))
    {
      MSG_GERAN_MED_0_G("SCE dynamic mode change DTM->DED");
    }
    else
#endif
    /* Error message if SCE started when already running, except for
     * Dedi->Dedi transitions since these are OK.
     */
    if( l1_sc_main_data_ptr->sce_running && (mode!=L1SCModeDediSDCCh) && (mode!=L1SCModeDediTCh) )
    {
        MSG_GERAN_ERROR_0_G("Start SCE when already running.");
    }

    l1_sc_main_data_ptr->sce_running = TRUE;

    if(mode != L1SCModeSameAsBefore)
    {
        l1_sc_globals_ptr->prev_mode   = l1_sc_globals_ptr->mode;    
        l1_sc_globals_ptr->mode = mode;
//     l1_sc_globals_ptr->sorted_cell_count = 0;
    }

    switch( mode )
    {
    case L1SCModeNull:
        if( (prev_sc_mode == L1SCModeDediTCh) ||
#ifdef FEATURE_GSM_DTM
            (prev_sc_mode == L1SCModeDediDTM) ||
#endif
            (prev_sc_mode == L1SCModeDediSDCCh) )
        {
            l1_sc_leave_dedi(gas_id);
        }
        break;

    case L1SCModeIdle:
        if( (prev_sc_mode == L1SCModeDediTCh) ||
#ifdef FEATURE_GSM_DTM
            (prev_sc_mode == L1SCModeDediDTM) ||
#endif
            (prev_sc_mode == L1SCModeDediSDCCh) )
        {
            l1_sc_leave_dedi(gas_id);
        }
        else if(prev_sc_mode == L1SCModeIdle)
        {
            /* We probably just re-selected.  If we did
             * the serving cell info will be stored in
             * the BA list and we can add it to the BA
             * list proper.  This is good because the
             * old serving cell is probably in the new
             * BA list.
             */
            l1_sc_balist_add_serving_cell(gas_id);
        }

        l1_sc_balist_update_serving_cell(gas_id);

        l1_sc_balist_set_serving_cell_valid( FALSE, gas_id ); /* paranoia */

        l1_sc_enter_idle(gas_id);
        break;

    case L1SCModeDediSDCCh:
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
        /* go back to top-6 mode when we transition from ded to idle mode */
        l1_sc_clear_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_RR_EN, gas_id);
#endif
        if( (prev_sc_mode != L1SCModeDediTCh) &&
            (prev_sc_mode != L1SCModeDediSDCCh) )
        {
            l1_sc_enter_dedi(gas_id);
        }
        l1_sc_enter_dedi_sdcch(gas_id);
        l1_sc_globals_ptr->last_dedi_sc_start_FN = GSTMR_GET_FN_GERAN(gas_id);
        l1_sc_globals_ptr->start_ncell_rep_n_acq_dedi = FALSE;
        break;

#ifdef FEATURE_GSM_DTM
    case L1SCModeDediDTM:
#endif
    case L1SCModeDediTCh:
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
        /* go back to top-6 mode when we transition from ded to idle mode */
        l1_sc_clear_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_RR_EN, gas_id);
#endif
        if( (prev_sc_mode != L1SCModeDediTCh) &&
            (prev_sc_mode != L1SCModeDediSDCCh) )
        {
            l1_sc_enter_dedi(gas_id);
        }
        l1_sc_enter_dedi_tch(gas_id);
        l1_sc_globals_ptr->last_dedi_sc_start_FN = GSTMR_GET_FN_GERAN(gas_id);
        l1_sc_globals_ptr->start_ncell_rep_n_acq_dedi = FALSE;
        break;


    case L1SCModeGprsTrans:
        if(prev_sc_mode == L1SCModeDediSDCCh)
        {
            l1_sc_leave_dedi_sdcch(gas_id);
        }

        if(prev_sc_mode != L1SCModeGprsTrans)
        {
          gpl1_gprs_sc_enter_trans(gas_id);
        }

        break;

    case L1SCModeSameAsBefore:
        switch(prev_sc_mode)
        {
        case L1SCModeNull:
            break;

        case L1SCModeIdle:
            l1_sc_enter_idle(gas_id);
            break;

        case L1SCModeDediSDCCh:
            l1_sc_enter_dedi_sdcch(gas_id);
            break;

        case L1SCModeDediTCh:
            l1_sc_enter_dedi_tch(gas_id);
            break;

        case L1SCModeGprsTrans:
            gpl1_gprs_sc_enter_trans(gas_id);
            break;

        default:
            MSG_GERAN_ERROR_1_G("Unknown mode %d", l1_sc_globals_ptr->mode);
            break;
        }
        break;

    default:
        MSG_GERAN_ERROR_1_G("Unknown mode %d", mode);
        break;
    }
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
}

void l1_sc_stop( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    MSG_GERAN_HIGH_2_G("Stopping SCE (%d).[%d]", l1_sc_globals_ptr->mode, gl1_get_FN( gas_id ));
    if (l1_sc_main_data_ptr == NULL)
    {
      MSG_GERAN_ERROR_0_G("NULL ptr!! return");
      return;
    }
    GL1_ISR_SAVE_LOCK(gas_id);
    if(!l1_sc_main_data_ptr->sce_running)
    {
        MSG_GERAN_ERROR_0_G("Stopping SCE when not running.");
#ifdef FEATURE_GSM_WTR_HOP
        /* Check if connected mode hopping is in progress and 
         * make sure SCE is not restarted afterward */
        gl1_msg_wtr_hop_do_not_restart_sce(gas_id);
#endif
    }

    l1_sc_globals_ptr->active = FALSE;
    l1_sc_main_data_ptr->sce_running          = FALSE;

    switch( l1_sc_globals_ptr->mode )
    {
    case L1SCModeNull:
        break;

    case L1SCModeIdle:
        l1_sc_leave_idle(gas_id);
        break;

    case L1SCModeDediSDCCh:
        l1_sc_leave_dedi_sdcch(gas_id);
        break;

#ifdef FEATURE_GSM_DTM
    case L1SCModeDediDTM:
#endif
    case L1SCModeDediTCh:
        l1_sc_leave_dedi_tch(gas_id);
        break;

    case L1SCModeGprsTrans:
        gpl1_gprs_sc_leave_trans(gas_id);
        break;

    default:
        break;
    }
    GL1_ISR_SAVE_UNLOCK(gas_id);
}

void l1_sci_go( gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    byte    n; 

    if( !l1_sc_main_data_ptr->sce_running )
    {
        MSG_GERAN_HIGH_0_G("SCE has been stopped, not starting SCE");
        return;
    }
    /* Loop through BA list and translate offsets for each cell */   
    for( n = 0; n < l1_sc_main_data_ptr->ba.cell_count; ++n )   
    {       
        cell_T* cell = l1_sc_main_data_ptr->ba.cells[ n ];  /*check_ba_exception*/  
        if( cell->afc_info.valid && cell->bs.bsic_known && l1_sc_globals_ptr->mode == L1SCModeIdle)
        {
            cell->afc_info.valid = TRUE;
            cell->afc_info.freq_error = 
                    gl1_hw_normxofreqtofreq( cell->bs.arfcn ,
                                     (cell->afc_info.afc_freq - gl1_hw_get_xo_acc_freq_err(gas_id)), gas_id );
            cell->afc_info.afc_freq = 
                    ( gl1_hw_freqtonormxofreq( cell->bs.arfcn ,
                                     cell->afc_info.freq_error,
                                     gas_id ) + gl1_hw_get_xo_acc_freq_err(gas_id));
            MSG_GERAN_HIGH_3_G("freq_error %dHz updated for ncellcell %d serving %d", cell->afc_info.freq_error, cell->bs.arfcn.num,gl1_hw_get_xo_acc_freq_err(gas_id));
        }    
    }

    l1_sc_globals_ptr->active = TRUE;
    l1_sci_monscan_go(gas_id);

    MSG_GERAN_HIGH_1_G("Starting SCE (%d)", l1_sc_globals_ptr->mode);
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
}


/*===========================================================================

FUNCTION  l1_sc_set_ba_list

DESCRIPTION
  Set the BA list used by the surround cell engine.
  This function dispatches to a state specific handler.

  IN - ba_list       - New BA list.
  IN - ncc_permitted - Bitmask with permitted NCCs.

DEPENDENCIES
  SCE must have been initialized and started by calling l1_sc_init() and
  l1_sc_start().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_set_ba_list( inter_task_BCCH_allocation_T* ba_list, byte ncc_permitted, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    ASSERT_TASK();

    switch( l1_sc_globals_ptr->mode )
    {
    case L1SCModeIdle:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       /*for DR-DSDS sort the BA list by band so we can cluster nter-band power monitors*/              
        qsort_band( ba_list->BCCH_ARFCN, ba_list->no_of_entries);
#endif 
        l1_sc_set_ba_list_idle( ba_list, 0xFF, gas_id );
        break;

    case L1SCModeDediSDCCh:
        l1_sc_set_ba_list_dedi_sdcch( ba_list, ncc_permitted, gas_id );
        break;

    case L1SCModeDediTCh:
        l1_sc_set_ba_list_dedi_tch( ba_list, ncc_permitted, gas_id );
        break;

    #ifdef FEATURE_GSM_DTM
    case L1SCModeDediDTM:
        l1_sc_set_ba_list_dedi_tch( ba_list, ncc_permitted, gas_id );
        break;
    #endif /* FEATURE_GSM_DTM */


    case L1SCModeGprsTrans:
        gpl1_sc_set_ba_list_trans( ba_list, ncc_permitted, gas_id );
        break;

    default:
        MSG_GERAN_ERROR_1_G("Unknown mode %d", l1_sc_globals_ptr->mode);
        break;
    }
}

/*===========================================================================

FUNCTION  l1_sc_set_sync

DESCRIPTION
  Inform the surround cell engine about a change in synchronisation or
  time slot.

  IN - frame_decrease - How much frame number has been decreased.
  IN - qbit_decrease  - How much quarter bit number has been decreased.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_set_sync( uint32 frame_decrease, int16 qbit_decrease, gas_id_t gas_id )
{
    /*lint -esym(715, time_slot) suppress deep_sleep not referenced */

    ASSERT_TASK();

    l1_sc_balist_set_sync( frame_decrease, qbit_decrease, gas_id );

    l1_sci_update_surround_tick_fn( frame_decrease, gas_id );
}


/*===========================================================================

FUNCTION  l1_sc_report_period_done

DESCRIPTION
  Callback function called by monscan at the end of each reporting period.
  Based on the SCE mode this function calls the appropriate function to
  schedule ncell receives.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_report_period_done( gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    switch( l1_sc_globals_ptr->mode )
    {
    case L1SCModeIdle:
        l1_sc_report_period_done_idle(gas_id);
        break;

    case L1SCModeDediSDCCh:
        l1_sc_report_period_done_sdcch(gas_id);
        break;

#ifdef FEATURE_GSM_DTM
    case L1SCModeDediDTM:
#endif
    case L1SCModeDediTCh:
        l1_sc_report_period_done_tch(gas_id);
        break;


    case L1SCModeGprsTrans:
        gpl1_gprs_report_period_done_trans(gas_id);
        break;

    default:
        MSG_GERAN_ERROR_1_G("Invalid dedicated SC mode: %d",l1_sc_globals_ptr->mode);
        break;
    }
}

/*===========================================================================

FUNCTION  l1_sc_post_sort_process

DESCRIPTION
  Called to kick of processing to occur each time the BA list is sorted.  The
  exact processing depends on the SCE mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_post_sort_process( gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    switch(l1_sc_globals_ptr->mode)
    {
    case L1SCModeIdle:
        l1_sc_idle_post_sort_process(gas_id);
#ifdef FEATURE_GSM_COEX_SW_CXM
       /* In IDLE , when BPLMN is active , it does CCCH and only bplmn , so no need to register NCELL cells */
        if (L1_background_HPLMN_search_in_progress(gas_id) == FALSE)
        {
          garb_intf_notify_idle_update_freq_list(l1_sc_globals_ptr->sorted_cell_count,
                                                 l1_sc_globals_ptr->sorted_cell_max_count,
                                                 &l1_sc_globals_ptr->sorted_cells[0],
                                                 gas_id );
        }
#endif /* FEATURE_GSM_COEX_SW_CXM */
        break;

    case L1SCModeGprsTrans:
        gpl1_gprs_sc_trans_post_sort_process(gas_id);
        break;

    default:
        break;
    }
}


/*===========================================================================

FUNCTION L1_SC_SET_BCCH_AUTONOMY

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sc_set_bcch_autonomy( boolean autonomous, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

    l1_sc_globals_ptr->autonomous_bcch = autonomous;

    if(autonomous)
    {
        l1_sc_balist_reset_counters(gas_id);
    }

    MSG_GERAN_MED_1_G("BCCH autonomy = %d", autonomous);
}



/**********************************************************************
 *
 *  Module:   BA List
 *  -------
 *  Handles change, sort and inquiry of BA list.
 *
 *  The data for the BA list is held in the ba struct.  This data is
 *  operated on by the functions defined below.  For efficiency
 *  the BA list module may also export pointers to the cells in the
 *  BA list so that other SCE functions can manipulate the data
 *  stored for these cells.
 *
 **********************************************************************/

/*===========================================================================

FUNCTION l1_sc_balist_init

DESCRIPTION
  Initailizes the BA list struct.  This function must be called before
  calling any other BA list funtions.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_init( gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    uint32 n;
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    for( n = 0; n < 2 * BA_CELL_POOL_LEN; ++n )
    {
        l1_sc_main_data_ptr->ba.unused_cells[ n ] = &l1_sc_main_data_ptr->ba.cell_pool[ n ];
    }

    l1_sc_main_data_ptr->ba.unused_cell_count            = (uint8)n;

    l1_sc_main_data_ptr->ba.cells                        = l1_sc_main_data_ptr->ba.cell_lists[ 0 ];
    l1_sc_main_data_ptr->ba.cell_count                   = 0;

    l1_sc_main_data_ptr->ba.new_cells                    = l1_sc_main_data_ptr->ba.cell_lists[ 1 ];
    l1_sc_main_data_ptr->ba.new_cell_count               = 0;

    l1_sc_main_data_ptr->ba.new_ba_list_pending          = FALSE;
    l1_sc_main_data_ptr->ba.new_id                       = 1;
    l1_sc_main_data_ptr->ba.old_id                       = 1;
    l1_sc_main_data_ptr->ba.multiband_reporting          = 0;
    l1_sc_globals_ptr->sorted_cell_count = 0;
	l1_sc_globals_ptr->last_top3_fcch_attempted_cell     = NULL;
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr       = 0;
#endif

    /* Allocate buffers to running average structs */
    for(n=0; n<2*BA_CELL_POOL_LEN; n++)
    {
        l1_running_avg_init( &l1_sc_main_data_ptr->ba.cell_pool[n].bs.ra,
                             l1_sc_main_data_ptr->ra_bufs[n],
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
                             NULL,
#endif /*FEATURE_GSM_RX_DIVERSITY || FEATURE_GSM_RX_DIVERSITY_DATA*/
                             L1_SC_MAX_MEAS_BUF_LEN,
                             gas_id);
    }

    l1_sc_main_data_ptr->ba.serving_cell_valid = FALSE;
    l1_sc_main_data_ptr->ba.pre_scell_arfcn = NULL;

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif
}

/*===========================================================================

FUNCTION l1_sc_balist_clear

DESCRIPTION
  Clears the BA list, deleting all the entries in it.
  Must not be called while any cells in the BA list are measured.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_clear( gas_id_t gas_id )
{
    l1_sc_balist_init(gas_id);
}

/*===========================================================================

FUNCTION  l1_sc_balist_set_multiband_reporting

DESCRIPTION
  Set the multiband reporting flag

  IN - multiband_reporting paramter specified by n/w

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_set_multiband_reporting( uint8 multiband_reporting, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    l1_sc_main_data_ptr->ba.multiband_reporting = multiband_reporting;
}

/*===========================================================================

FUNCTION  l1_sc_balist_prepare_new

DESCRIPTION
  Prepare a new BA list from the passed inter_task_BCCH_allocation_T.
  If the BA list changed, set ba.new_ba_list_pending.
  On all cells not in the new ba list, the flag in_new_ba_list
  is cleared.

DEPENDENCIES
  IN - ba_list - New BA list.

RETURN VALUE
  TRUE when BA list changed.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_balist_prepare_new( inter_task_BCCH_allocation_T* ba_list, byte ncc_permitted, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    uint32  added_cnt;
    boolean delete_old;
    word n;

    ASSERT_TASK();

    /* Check if there is already a BA list pending. If
     * there is then delete the previous new cells before
     * adding the current ones.
     */
    GL1_ISR_SAVE_LOCK(gas_id);
    if(l1_sc_main_data_ptr->ba.new_ba_list_pending)
    {
        delete_old = TRUE;

        /* This might look redundant but it is included as a
         * safeguard against race conditions.
         */
        l1_sc_main_data_ptr->ba.new_ba_list_pending = FALSE;
    }
    else
    {
        delete_old = FALSE;
    }
    GL1_ISR_SAVE_UNLOCK(gas_id);

    if(delete_old)
    {
        MSG_GERAN_HIGH_1_G("SCE: Deleting %d new cells", l1_sc_main_data_ptr->ba.new_cell_count);

        /* Only delete cells that aren't in old BA list. */
        for( n = 0; n < l1_sc_main_data_ptr->ba.new_cell_count; ++n )
        {
            cell_T* cell;

            GL1_ISR_SAVE_LOCK(gas_id);

            cell = l1_sc_main_data_ptr->ba.new_cells[ n ];

            l1_sc_balist_check_cell(gas_id, cell);

            if( !cell->in_old_ba_list )
            {
                l1_sc_balist_delete_cell( cell, gas_id );
            }
            GL1_ISR_SAVE_UNLOCK(gas_id);
        }
    }

    /* Now put the new cells into the BA list as new cells. */
    l1_sc_main_data_ptr->ba.new_cell_count = ba_list->no_of_entries;
    l1_sc_main_data_ptr->ba.new_ncc_permitted = ncc_permitted;

    /* Static analysis suggests this check */
    if (l1_sc_main_data_ptr->ba.new_cell_count > ARR_SIZE(ba_list->BCCH_ARFCN))
    {
      l1_sc_main_data_ptr->ba.new_cell_count = ARR_SIZE(ba_list->BCCH_ARFCN);
    }

    for( n = 0; n < l1_sc_main_data_ptr->ba.cell_count; ++n )
    {
        l1_sc_main_data_ptr->ba.cells[ n ]->in_new_ba_list = FALSE;
        l1_sc_main_data_ptr->ba.cells[ n ]->in_old_ba_list = TRUE;
    }

    added_cnt = 0;
    for( n = 0; n < l1_sc_main_data_ptr->ba.new_cell_count; ++n )
    {
        ARFCN_T arfcn;
        cell_T* cell;


  GL1_ISR_SAVE_LOCK(gas_id);

  arfcn = ba_list->BCCH_ARFCN[ n ];
  cell  = l1_sc_balist_get_cell_by_arfcn( arfcn, gas_id );

        if( cell )
        {
            l1_sc_balist_check_cell(gas_id, cell);
            cell->in_new_ba_list = TRUE;
            l1_sc_main_data_ptr->ba.new_cells[ n ]    = cell;
        }
        else
        {
            cell = l1_sc_balist_new_cell( arfcn, gas_id );

            if(cell)
            {
                /*check_ba_exception*/
                l1_sc_balist_check_cell(gas_id, cell);
                added_cnt++;
                l1_sc_main_data_ptr->ba.new_cells[ n ] = cell;
            }
            else
            {
                /* Must have been no more space to add cells.
                 * Abandon BA update and output debug info.
                 */
                MSG_GERAN_ERROR_1_G("SCE: Couldn't add cell(%d) to BA list.", arfcn.num);
                MSG_GERAN_ERROR_3_G("old=%d, new=%d, added=%d", l1_sc_main_data_ptr->ba.cell_count, l1_sc_main_data_ptr->ba.new_cell_count, added_cnt);
            }
        }

  GL1_ISR_SAVE_UNLOCK(gas_id);
    }

    /*
     * See if there are changes
     */

    if( (added_cnt > 0) || (l1_sc_main_data_ptr->ba.new_cell_count != l1_sc_main_data_ptr->ba.cell_count) || delete_old )
    {
        l1_sc_main_data_ptr->ba.new_ba_list_pending = TRUE;
        l1_sc_main_data_ptr->ba.new_id++;
    }

    MSG_GERAN_MED_2_G("SCE: New BA list. %d cells, %d new.", l1_sc_main_data_ptr->ba.new_cell_count, added_cnt);

    return l1_sc_main_data_ptr->ba.new_ba_list_pending;
}

/*===========================================================================

FUNCTION l1_sc_check_for_ncc_permitted_update

DESCRIPTION
  Used to see if the ncc_permitted has been updated but the BAlist has not.
  Previously the code would not update the ncc_permitted in such circumstances
  this function checks for this specific case and updates the value here.

DEPENDENCIES
  Should be called upon a MPH_DEDICATED_UPDATE_REQ to check for update.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_check_for_ncc_permitted_update(gas_id_t gas_id)
{
  l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

  /*
  If the ncc_permitted has changed but the BAlist has not then update the
  ncc_permitted here. This may happen if RR sends the dedicated_update_req
  before receiving SI6, and also if RR detected a change in SI6 only.
  See 3gpp 05.08 8.4.4 "The current ncc permitted is the latest ncc_permitted
  received on the SACCH"
  */
  if ( (l1_sc_main_data_ptr->ba.ncc_permitted != l1_sc_main_data_ptr->ba.new_ncc_permitted) &&
       !l1_sc_main_data_ptr->ba.new_ba_list_pending)
  {
    MSG_GERAN_HIGH_2_G("Update NCC Permitted prev:%d new:%d",l1_sc_main_data_ptr->ba.ncc_permitted,l1_sc_main_data_ptr->ba.new_ncc_permitted);
    l1_sc_main_data_ptr->ba.ncc_permitted = l1_sc_main_data_ptr->ba.new_ncc_permitted;
  }
}

/*===========================================================================

FUNCTION l1_sc_balist_use_new

DESCRIPTION
  Used to signal that it is safe to pick up the new BA list.

  NOTE: Do NOT call this function from the ISR context.  Several
  task context functions cannot tolerate the new BA list being
  picked up part way through their execution.

DEPENDENCIES
  New BA list must have been previously set up by a call to
  l1_sc_balist_prepare_new().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_use_new( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    cell_T** tmp;
    word n;

#ifdef DEBUG_SCE_MSG
    #error code not present
#else
    if(l1_sc_main_data_ptr->ba.new_ba_list_pending)
    {
        MSG_GERAN_MED_0_G("SCE: Using new BA list.");
    }
#endif

    l1_sc_main_data_ptr->ba.ncc_permitted = l1_sc_main_data_ptr->ba.new_ncc_permitted;
    if( l1_sc_main_data_ptr->ba.new_ba_list_pending )
    {
        /* This is used to buffer the current cell count */
        byte  current_cell_count;

        /* delete cells not in new BA list any more */
        for( n = 0; n < l1_sc_main_data_ptr->ba.cell_count; ++n )
        {
            cell_T* cell = l1_sc_main_data_ptr->ba.cells[ n ];
            if( !cell->in_new_ba_list )
            {
                l1_sc_balist_delete_cell( cell, gas_id );
            }
        }

        /* Use new BA list. This involves buffering the pptr and cell count */
        tmp                    = l1_sc_main_data_ptr->ba.cells;
        current_cell_count     = l1_sc_main_data_ptr->ba.cell_count;

        l1_sc_main_data_ptr->ba.cells               = l1_sc_main_data_ptr->ba.new_cells;
        l1_sc_main_data_ptr->ba.cell_count          = l1_sc_main_data_ptr->ba.new_cell_count;

        /* Complete the swap correctly by setting the cell count and pptr */
        l1_sc_main_data_ptr->ba.new_cells           = tmp;
        l1_sc_main_data_ptr->ba.new_cell_count      = current_cell_count;

        l1_sc_main_data_ptr->ba.old_id              = l1_sc_main_data_ptr->ba.new_id;

        l1_sc_main_data_ptr->ba.new_ba_list_pending = FALSE;
    }

    /* We've just updated the BA list so if
     * the old serving cell is meant to be
     * in it then it will be already.
     */
    l1_sc_main_data_ptr->ba.serving_cell_valid = FALSE;
    l1_sc_main_data_ptr->ba.pre_scell_arfcn = NULL;

    /* Reset the sorted cell count. */
//   l1_sc_globals.sorted_cell_count = 0;
}

/*===========================================================================

FUNCTION  l1_sc_balist_set_ra_length

DESCRIPTION
  Sets the number of measurements to include in the running average for
  cells in the BA list.  Thi is done by setting the running avg length
  for each cell in the BA list.

DEPENDENCIES
  l1_sc_ba_list_init() must have been called.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_set_ra_length( uint8 len, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    int32 i;

    GL1_ISR_SAVE_LOCK(gas_id);
    /* Do new BA list if one is pending. */
    if(l1_sc_main_data_ptr->ba.new_ba_list_pending)
    {
        for(i=0; i<l1_sc_main_data_ptr->ba.new_cell_count; i++)
        {
          l1_running_avg_set_avg_len( &(l1_sc_main_data_ptr->ba.new_cells[i]->bs.ra), len, gas_id );
        }
    }

    /* This function is called from the task and the code below
     * operates on data shared with the ISR.  However, for each
     * cell it writes only one value.  Since this operation is
     * atomic there should be no problem with race conditions.
     */

    /* Do BA list */
    for(i=0; i<l1_sc_main_data_ptr->ba.cell_count; i++)
    {
       /*check_ba_exception*/
      l1_sc_balist_check_cell(gas_id, l1_sc_main_data_ptr->ba.cells[i]);

      l1_running_avg_set_avg_len( &(l1_sc_main_data_ptr->ba.cells[i]->bs.ra), len, gas_id );
    }
    GL1_ISR_SAVE_UNLOCK(gas_id);

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_sc_balist_multiband_sort

DESCRIPTION
  Loops through the sorted list and ensures that the top 6 has n cells in
  the non-serving cell band and 6 - n cells in the serving cell's band


DEPENDENCIES
  l1_sc_ba_list_init() must have been called.
  Most recent rx power average for each cell must be present in
  the bs.rx_power field of each cell.  This is the field that
  is used in the sort.

RETURN VALUE
  None

SIDE EFFECTS
  Sorted cell list is modified

===========================================================================*/

static  cell_T* temp_multiband_sorted_cells[NUM_GERAN_DATA_SPACES][BA_MAX_LEN + 1];

  typedef struct{
    cell_T* multiband_cells[MAX_NUMBER_OF_BEST_CELLS];
    uint16 number_of_cells;
    uint16 ranked_cells;
  }multiband_store_t;

static  multiband_store_t  SYS_BAND_EGSM_900_store[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE({{0}}) };
static  multiband_store_t  SYS_BAND_PGSM_900_store[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE({{0}}) };
static  multiband_store_t  SYS_BAND_DCS_1800_store[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE({{0}}) };
static  multiband_store_t  SYS_BAND_PCS_1900_store[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE({{0}}) };
static  multiband_store_t  SYS_BAND_CELL_850_store[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE({{0}}) };


void l1_sc_balist_multiband_sort( byte multiband_parameter, gas_id_t gas_id )
{
  #define UNRANKED_NCELL 0xff

  l1_sc_globals_T   *l1_sc_globals_ptr   = NULL;
  l1_sc_main_data_t *l1_sc_main_data_ptr = NULL;

  byte        i;
  cell_T*     cell;
  sys_band_T  serving_cell_band;
  sys_band_T  ncell_band;

  uint16   check_count;
  boolean  already_used;
  boolean  cell_allowed;

  uint16  multiband_ranking = 0;
  uint16  mb_cells_count   = 0;
  uint16  last_mb_cell_end = 0;

  gas_id = check_gas_id( gas_id );

  l1_sc_globals_ptr   = gl1_ms_switch_l1_sc_globals_store(gas_id);
  l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

  //boundary check for multiband parameter to fix klockwork error
  if(multiband_parameter > 6)
  {
     multiband_parameter = 6;
  }

  SYS_BAND_EGSM_900_store[gas_id].multiband_cells[0] = NULL;
  SYS_BAND_EGSM_900_store[gas_id].multiband_cells[1] = NULL;
  SYS_BAND_EGSM_900_store[gas_id].multiband_cells[2] = NULL;
  SYS_BAND_EGSM_900_store[gas_id].multiband_cells[3] = NULL;
  SYS_BAND_EGSM_900_store[gas_id].multiband_cells[4] = NULL;
  SYS_BAND_EGSM_900_store[gas_id].multiband_cells[5] = NULL;
  SYS_BAND_EGSM_900_store[gas_id].number_of_cells = 0;
  SYS_BAND_EGSM_900_store[gas_id].ranked_cells = 0;

  SYS_BAND_PGSM_900_store[gas_id].multiband_cells[0] = NULL;
  SYS_BAND_PGSM_900_store[gas_id].multiband_cells[1] = NULL;
  SYS_BAND_PGSM_900_store[gas_id].multiband_cells[2] = NULL;
  SYS_BAND_PGSM_900_store[gas_id].multiband_cells[3] = NULL;
  SYS_BAND_PGSM_900_store[gas_id].multiband_cells[4] = NULL;
  SYS_BAND_PGSM_900_store[gas_id].multiband_cells[5] = NULL;
  SYS_BAND_PGSM_900_store[gas_id].number_of_cells = 0;
  SYS_BAND_PGSM_900_store[gas_id].ranked_cells = 0;

  SYS_BAND_PCS_1900_store[gas_id].multiband_cells[0] = NULL;
  SYS_BAND_PCS_1900_store[gas_id].multiband_cells[1] = NULL;
  SYS_BAND_PCS_1900_store[gas_id].multiband_cells[2] = NULL;
  SYS_BAND_PCS_1900_store[gas_id].multiband_cells[3] = NULL;
  SYS_BAND_PCS_1900_store[gas_id].multiband_cells[4] = NULL;
  SYS_BAND_PCS_1900_store[gas_id].multiband_cells[5] = NULL;
  SYS_BAND_PCS_1900_store[gas_id].number_of_cells = 0;
  SYS_BAND_PCS_1900_store[gas_id].ranked_cells = 0;


  SYS_BAND_DCS_1800_store[gas_id].multiband_cells[0] = NULL;
  SYS_BAND_DCS_1800_store[gas_id].multiband_cells[1] = NULL;
  SYS_BAND_DCS_1800_store[gas_id].multiband_cells[2] = NULL;
  SYS_BAND_DCS_1800_store[gas_id].multiband_cells[3] = NULL;
  SYS_BAND_DCS_1800_store[gas_id].multiband_cells[4] = NULL;
  SYS_BAND_DCS_1800_store[gas_id].multiband_cells[5] = NULL;
  SYS_BAND_DCS_1800_store[gas_id].number_of_cells = 0;
  SYS_BAND_DCS_1800_store[gas_id].ranked_cells = 0;


  SYS_BAND_CELL_850_store[gas_id].multiband_cells[0] = NULL;
  SYS_BAND_CELL_850_store[gas_id].multiband_cells[1] = NULL;
  SYS_BAND_CELL_850_store[gas_id].multiband_cells[2] = NULL;
  SYS_BAND_CELL_850_store[gas_id].multiband_cells[3] = NULL;
  SYS_BAND_CELL_850_store[gas_id].multiband_cells[4] = NULL;
  SYS_BAND_CELL_850_store[gas_id].multiband_cells[5] = NULL;
  SYS_BAND_CELL_850_store[gas_id].number_of_cells = 0;
  SYS_BAND_CELL_850_store[gas_id].ranked_cells = 0;


  MSG_GERAN_HIGH_1_G("l1_sc_balist_multiband_sort %d ", multiband_parameter );

  if( multiband_parameter != 0 )
  {
    /* Determine serving and surround cell bands */
    serving_cell_band = gl1_get_band ( gl1_get_serving_cell_arfcn(gas_id) );

    /*************************************************************************/
    /* Go through the complete list of ncells and strip out from the list those
       cells that are not in the same band as the serving cell. But only take out
       to a maximum of those required for the multiband reporting. e.g. for
       multiband reporting = 2 then take out the best 2 of every ncell that is
       in a different band to the scell. See below...

        If scell = PGSM
        l1_sc_sorted_cells = ARFCN:525 BAND:DCS PWR:-43   <- TAKE THIS DCS
        l1_sc_sorted_cells = ARFCN:975 BAND:EGSM PWR:-50  <- TAKE THIS EGSM
        l1_sc_sorted_cells = ARFCN:666 BAND:DCS PWR:-50   <- TAKE THIS DCS
        l1_sc_sorted_cells = ARFCN:10 BAND:PGSM PWR:-53
        l1_sc_sorted_cells = ARFCN:1000 BAND:EGSM PWR:-56 <- TAKE THIS EGSM
        l1_sc_sorted_cells = ARFCN:770 BAND:DCS PWR:-56
        l1_sc_sorted_cells = ARFCN:590 BAND:DCS PWR:-62
        l1_sc_sorted_cells = ARFCN:1010 BAND:PGSM PWR:-62
        l1_sc_sorted_cells = ARFCN:50 BAND:PGSM PWR:-62
        l1_sc_sorted_cells = ARFCN:30 BAND:PGSM PWR:-62
        l1_sc_sorted_cells = ARFCN:1010 BAND:PGSM PWR:-62
        l1_sc_sorted_cells = ARFCN:40 BAND:PGSM PWR:-68
        l1_sc_sorted_cells = ARFCN:585 BAND:DCS PWR:-78
        l1_sc_sorted_cells = ARFCN:585 BAND:DCS PWR:-78


       These cells are placed in a specific store for its respective band. This
       store will only hold up to the multiband reporting parameter entries */
    /*************************************************************************/
    for(i=0;i<l1_sc_globals_ptr->sorted_cell_count;i++)
    {

      cell_allowed = FALSE;

      cell = l1_sc_globals_ptr->sorted_cells[ i ];



      if ( cell == NULL )
      {
        MSG_GERAN_ERROR_0_G("NULL PTR in l1_sc_globals_ptr->sorted_cells");
        continue;
      }

      /*Unrank all the cells at this point - They will be ranked later according to
      the multiband rules. These are that the top out of band ncells (number of these
      is determined by the multiband parameter) shall be ranked higher than the in band
      ncells*/
      cell->rank = UNRANKED_NCELL;

      ncell_band = gl1_get_band(cell->bs.arfcn);

      /*
      Set the cell_allowed flag if the BSIC is known and allowed.
      This is referenced later to check if ncells are to be reported to RR
      */
      if( cell->bs.bsic_known )
      {
        byte ncc = ( cell->bs.bsic >> 3 ) & 7;
        if( ( l1_sc_main_data_ptr->ba.ncc_permitted & ( 1 << ncc ) ) != 0 )
        {
          cell_allowed = TRUE;
        }
      }

      /*
      If cell is not in serving band, and has an acceptable power level then use it. At this point it
      is either added to the store of out of band cells with BSIC known and allowed, in preparation for
      reporting to RR, or just ranked highly as it needs BSIC decoding, so it can be reported.
      */
      if( ( ncell_band != serving_cell_band) &&
          (l1_sc_rx_power_acceptable(cell->bs.rx_power))
        )
      {
        switch (ncell_band)
        {
        /****************** FOR SYS_BAND_EGSM_900 ****************/
        case SYS_BAND_EGSM_900:
          if ( (SYS_BAND_EGSM_900_store[gas_id].number_of_cells < multiband_parameter) && cell_allowed )
          {

            if (SYS_BAND_EGSM_900_store[gas_id].ranked_cells < multiband_parameter)
            {
              /*
              Only rank the top "multiband_parameter" number of cells. Don't actually perform
              the re-ranking here, it will be done when this cell is added to the
              temp_multiband_sorted_cells
              */
              SYS_BAND_EGSM_900_store[gas_id].ranked_cells++;
            }

            SYS_BAND_EGSM_900_store[gas_id].multiband_cells[SYS_BAND_EGSM_900_store[gas_id].number_of_cells++] = cell;
          }
          else
          /* If the BSIC isn't known and not all cells in this band have been ranked yet then
          this must be one of the strongest ncells in this band and needs the bsic to be decoded. */
          if ((!cell->bs.bsic_known) &&
              (SYS_BAND_EGSM_900_store[gas_id].ranked_cells < multiband_parameter)
             )
          {
            cell->rank = multiband_ranking++;
            SYS_BAND_EGSM_900_store[gas_id].ranked_cells++;
          }
          break;

        /****************** FOR SYS_BAND_PGSM_900 ****************/
        case SYS_BAND_PGSM_900:
          if ( (SYS_BAND_PGSM_900_store[gas_id].number_of_cells < multiband_parameter) && cell_allowed)
          {

            if (SYS_BAND_PGSM_900_store[gas_id].ranked_cells < multiband_parameter)
            {
              /*
              Only rank the top "multiband_parameter" number of cells. Don't actually perform
              the re-ranking here, it will be done when this cell is added to the
              temp_multiband_sorted_cells
              */
              SYS_BAND_PGSM_900_store[gas_id].ranked_cells++;
            }

            SYS_BAND_PGSM_900_store[gas_id].multiband_cells[SYS_BAND_PGSM_900_store[gas_id].number_of_cells++] = cell;
          }
          else
          /* If the BSIC isn't known and not all cells in this band have been ranked yet then
          this must be one of the strongest ncells in this band and needs the bsic to be decoded. */
          if ((!cell->bs.bsic_known) &&
              (SYS_BAND_PGSM_900_store[gas_id].ranked_cells < multiband_parameter)
             )
          {
            cell->rank = multiband_ranking++;
            SYS_BAND_PGSM_900_store[gas_id].ranked_cells++;
          }
          break;

        /****************** FOR SYS_BAND_DCS_1800 ****************/
        case SYS_BAND_DCS_1800:
          if ( (SYS_BAND_DCS_1800_store[gas_id].number_of_cells < multiband_parameter) && cell_allowed)
          {

            if (SYS_BAND_DCS_1800_store[gas_id].ranked_cells < multiband_parameter)
            {
              /*
              Only rank the top "multiband_parameter" number of cells. Don't actually perform
              the re-ranking here, it will be done when this cell is added to the
              temp_multiband_sorted_cells
              */
              SYS_BAND_DCS_1800_store[gas_id].ranked_cells++;
            }

            SYS_BAND_DCS_1800_store[gas_id].multiband_cells[SYS_BAND_DCS_1800_store[gas_id].number_of_cells++] = cell;
          }
          else
          /* If the BSIC isn't known and not all cells in this band have been ranked yet then
          this must be one of the strongest ncells in this band and needs the bsic to be decoded. */
          if ((!cell->bs.bsic_known) &&
              (SYS_BAND_DCS_1800_store[gas_id].ranked_cells < multiband_parameter)
             )
          {
            cell->rank = multiband_ranking++;
            SYS_BAND_DCS_1800_store[gas_id].ranked_cells++;
          }
          break;

        /****************** FOR SYS_BAND_PCS_1900 ****************/
        case SYS_BAND_PCS_1900:
          if ( (SYS_BAND_PCS_1900_store[gas_id].number_of_cells < multiband_parameter) && cell_allowed)
          {

            if (SYS_BAND_PCS_1900_store[gas_id].ranked_cells < multiband_parameter)
            {
              /*
              Only rank the top "multiband_parameter" number of cells. Don't actually perform
              the re-ranking here, it will be done when this cell is added to the
              temp_multiband_sorted_cells
              */
              SYS_BAND_PCS_1900_store[gas_id].ranked_cells++;
            }

            SYS_BAND_PCS_1900_store[gas_id].multiband_cells[SYS_BAND_PCS_1900_store[gas_id].number_of_cells++] = cell;
          }
          else
          /* If the BSIC isn't known and not all cells in this band have been ranked yet then
          this must be one of the strongest ncells in this band and needs the bsic to be decoded. */
          if ((!cell->bs.bsic_known) &&
              (SYS_BAND_PCS_1900_store[gas_id].ranked_cells < multiband_parameter)
             )
          {
            cell->rank = multiband_ranking++;
            SYS_BAND_PCS_1900_store[gas_id].ranked_cells++;
          }
          break;

        /****************** FOR SYS_BAND_CELL_850 ****************/
        case SYS_BAND_CELL_850:
          if ( (SYS_BAND_CELL_850_store[gas_id].number_of_cells < multiband_parameter) && cell_allowed)
          {

            if (SYS_BAND_CELL_850_store[gas_id].ranked_cells < multiband_parameter)
            {
              /*
              Only rank the top "multiband_parameter" number of cells. Don't actually perform
              the re-ranking here, it will be done when this cell is added to the
              temp_multiband_sorted_cells
              */
              SYS_BAND_CELL_850_store[gas_id].ranked_cells++;
            }

            SYS_BAND_CELL_850_store[gas_id].multiband_cells[SYS_BAND_CELL_850_store[gas_id].number_of_cells++] = cell;
          }
          else
          /* If the BSIC isn't known and not all cells in this band have been ranked yet then
          this must be one of the strongest ncells in this band and needs the bsic to be decoded. */
          if ((!cell->bs.bsic_known) &&
              (SYS_BAND_CELL_850_store[gas_id].ranked_cells < multiband_parameter)
             )
          {
            cell->rank = multiband_ranking++;
            SYS_BAND_CELL_850_store[gas_id].ranked_cells++;
          }
          break;


        default:
          MSG_GERAN_ERROR_1_G("Band is not known %d",ncell_band);
        }
      }
    }

    /******************************************************************************************/
    /* Begin to populate the temp_multiband_sorted_cells. This will eventually overwrite the  */
    /* l1_sc_globals.sorted_cells. It is safe to just grab what is in each store or until the */
    /* best six of the temp_multiband_sorted_cells array has been filled up.                  */
    /* The scell bands array will contain nothing and so will automatically be skipped.       */
    /******************************************************************************************/
    for(i=0;(i<SYS_BAND_EGSM_900_store[gas_id].number_of_cells)&&(mb_cells_count<MAX_NUMBER_OF_BEST_CELLS);i++)
    {

      SYS_BAND_EGSM_900_store[gas_id].multiband_cells[i]->rank = multiband_ranking++;
      temp_multiband_sorted_cells[gas_id][mb_cells_count++] = SYS_BAND_EGSM_900_store[gas_id].multiband_cells[i];

    }

    for(i=0;(i<SYS_BAND_PGSM_900_store[gas_id].number_of_cells)&&(mb_cells_count<MAX_NUMBER_OF_BEST_CELLS);i++)
    {

      SYS_BAND_PGSM_900_store[gas_id].multiband_cells[i]->rank = multiband_ranking++;
      temp_multiband_sorted_cells[gas_id][mb_cells_count++] = SYS_BAND_PGSM_900_store[gas_id].multiband_cells[i];

    }

    for(i=0;(i<SYS_BAND_PCS_1900_store[gas_id].number_of_cells)&&(mb_cells_count<MAX_NUMBER_OF_BEST_CELLS);i++)
    {

      SYS_BAND_PCS_1900_store[gas_id].multiband_cells[i]->rank = multiband_ranking++;
      temp_multiband_sorted_cells[gas_id][mb_cells_count++] = SYS_BAND_PCS_1900_store[gas_id].multiband_cells[i];

    }

    for(i=0;(i<SYS_BAND_DCS_1800_store[gas_id].number_of_cells)&&(mb_cells_count<MAX_NUMBER_OF_BEST_CELLS);i++)
    {

      SYS_BAND_DCS_1800_store[gas_id].multiband_cells[i]->rank = multiband_ranking++;
      temp_multiband_sorted_cells[gas_id][mb_cells_count++] = SYS_BAND_DCS_1800_store[gas_id].multiband_cells[i];

    }

    for(i=0;(i<SYS_BAND_CELL_850_store[gas_id].number_of_cells)&&(mb_cells_count<MAX_NUMBER_OF_BEST_CELLS);i++)
    {

      SYS_BAND_CELL_850_store[gas_id].multiband_cells[i]->rank = multiband_ranking++;
      temp_multiband_sorted_cells[gas_id][mb_cells_count++] = SYS_BAND_CELL_850_store[gas_id].multiband_cells[i];

    }


    /*************************************************************************************/
    /* Go through the l1_sc_globals.sorted_cells and pull out the best ncells within the */
    /* scell's band and fill remaining places in the best six of the                     */
    /* temp_multiband_sorted_cells with these. The Ranking of these cells is performed   */
    /* here too.                                                                         */
    /*************************************************************************************/
    for(i=0;i<l1_sc_globals_ptr->sorted_cell_count;i++)
    {
      cell = l1_sc_globals_ptr->sorted_cells[i];

      if ( cell == NULL )
      {
        MSG_GERAN_ERROR_0_G("NULL PTR in l1_sc_globals_ptr->sorted_cells");
        continue;
      }

      ncell_band = gl1_get_band(cell->bs.arfcn);

      if (gl1_get_band(cell->bs.arfcn) == serving_cell_band)
      {
        cell->rank = multiband_ranking++;
        temp_multiband_sorted_cells[gas_id][mb_cells_count++] = cell;

        if (mb_cells_count >= MAX_NUMBER_OF_BEST_CELLS)
          /*Top six is full jump out of this loop*/
          break;
      }
    }

    /* Remember where the best six ends as there may not be enough ncells to fill it all up.*/
    last_mb_cell_end = mb_cells_count;

    /*************************************************************************/
    /* Go through the list again and add any entries not yet included to the */
    /* list.                                                                 */
    /*************************************************************************/
    for(i=0;i<l1_sc_globals_ptr->sorted_cell_count;i++)
    {
      cell = l1_sc_globals_ptr->sorted_cells[i];

      if ( cell == NULL )
      {
        MSG_GERAN_ERROR_0_G("NULL PTR in l1_sc_globals_ptr->sorted_cells");
        continue;
      }

      already_used = FALSE;

      /*Check that each cell isn't already in the top six then just add then below*/
      for (check_count=0;check_count<last_mb_cell_end;check_count++)
      {
        if (ARFCNS_EQUAL(cell->bs.arfcn,temp_multiband_sorted_cells[gas_id][check_count]->bs.arfcn))
        {
          already_used = TRUE;
          break;
        }
      }

      if (!already_used)
      {
          if(mb_cells_count < (BA_MAX_LEN+1))
          {
              temp_multiband_sorted_cells[gas_id][mb_cells_count++] = cell;
          }
          else
          {
              if(geran_get_nv_recovery_restart_enabled(gas_id))
              {
                MSG_GERAN_ERROR_1_G("NCELL:Count out of range %d",mb_cells_count);
                l1_call_panic_reset_in_task(L1_SPLIT_PAGE_CYCLE_ZERO, gas_id);
              }
              else
              {
#ifdef FEATURE_QSH_MDUMP
              QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
              ERR_GERAN_FATAL_1_G("NCELL:Count out of range %d",mb_cells_count);
          }
      }
    }
    }

    /*********************************************************************************/
    /* Copy the temp_multiband_sorted_cells back into the l1_sc_globals.sorted_cells */
    /* list. Then re-rank the list.....job done !                                    */
    /*********************************************************************************/

    /*Always use the index to the list being copied from to prevent problems*/
    l1_sc_globals_ptr->sorted_cell_count = mb_cells_count;
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr = mb_cells_count;
#endif

    for( i=0; i < l1_sc_globals_ptr->sorted_cell_count; i++ )
    {
      cell = temp_multiband_sorted_cells[gas_id][i];

      if ( cell == NULL )
      {
        MSG_GERAN_ERROR_0_G("NULL PTR in temp_multiband_sorted_cells");
        continue;
      }

      l1_sc_globals_ptr->sorted_cells[i] = cell;

      if (cell->rank == UNRANKED_NCELL)
      {

        l1_sc_globals_ptr->sorted_cells[i]->rank = multiband_ranking++;

      }

#if 0
      MSG_4(MSG_SSID_DFLT, MSG_LEGACY_MED,
            "SORTED LIST Arfcn:%d Band:%d Rank:%d Pwr:%d"
            ,l1_sc_globals_ptr->sorted_cells[i]->bs.arfcn.num
            ,l1_sc_globals_ptr->sorted_cells[i]->bs.arfcn.band
            ,l1_sc_globals_ptr->sorted_cells[i]->rank
            ,l1_sc_globals_ptr->sorted_cells[i]->bs.rx_power
            );
#endif
    }
  }
}

/*===========================================================================

FUNCTION  compare_balist

DESCRIPTION
  This function is to be used by qsort as a compare function. This function
  is exclusive to compare an array of pointers to cell_T by the
  bs.rx_power and it should sort it descendently.

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
static int compare_balist (const void * a, const void * b)
{
  cell_T **cell1ptr = (cell_T **)a;
  cell_T **cell2ptr = (cell_T **)b;
  cell_T *cell1 = *cell1ptr;
  cell_T *cell2 = *cell2ptr;
  return (cell2->bs.rx_power - cell1->bs.rx_power);
}

/*===========================================================================

FUNCTION  l1_sc_balist_sort

DESCRIPTION
  Creates list of BA cells sorted by decreasing power.

  If include_serving is TRUE then the serving cell is included in the
  sorted BA list, otherwise the serving cell is excluded.

DEPENDENCIES
  l1_sc_ba_list_init() must have been called.
  Most recent rx power average for each cell must be present in
  the bs.rx_power field of each cell.  This is the field that
  is used in the sort.

RETURN VALUE
  None

SIDE EFFECTS
  Sorted cell list is copied to l1_sc_globals struct.

===========================================================================*/
void l1_sc_balist_sort( boolean include_serving, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    /* Use uint32s because that produces more efficient code - really
     * only need uint8s.
     */
    uint32    i,n;
    uint32    num_sorted_cells;
    uint8 multiband_parameter;

    ASSERT_TASK();

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif

    num_sorted_cells = l1_sc_main_data_ptr->ba.cell_count;
    //ba is a global structrue so bound check has provided
    if(num_sorted_cells >(BA_MAX_LEN + 1))
    {

      num_sorted_cells = (BA_MAX_LEN + 1);
    }

    if(include_serving)
    {
        /* Copy all cells to sorted list. */
        for( n=0, i=0; n < num_sorted_cells; n++ )
        {
#ifdef FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17
            /* If timeout, bring the cell out of the backlist */
            if( l1_sc_main_data_ptr->ba.cells[n]->next_tick <= l1_sc_globals_ptr->surround_tick )
            {
                if(l1_sc_main_data_ptr->ba.cells[n]->in_blacklist)
                {
                  MSG_GERAN_MED_4_G("SCE: De-blacklisting(1): FN: %d, s-tick: %d, BL-end: %d, arfcn: %d",
                    GSTMR_GET_FN_GERAN(gas_id),
                    l1_sc_globals_ptr->surround_tick,
                    l1_sc_main_data_ptr->ba.cells[n]->next_tick,
                    l1_sc_main_data_ptr->ba.cells[n]->bs.arfcn.num);
                }

                l1_sc_main_data_ptr->ba.cells[n]->in_blacklist = FALSE;
            }
            /* remove the blacklist when total number of ncell is less than 6 */
            if ( ( num_sorted_cells < 6 ) && l1_sc_main_data_ptr->ba.cells[n]->in_blacklist )
            {
                l1_sc_main_data_ptr->ba.cells[n]->in_blacklist = FALSE;
            }
            if( !l1_sc_main_data_ptr->ba.cells[n]->in_blacklist )
#endif
            {
                l1_sc_globals_ptr->sorted_cells[ i++ ] = l1_sc_main_data_ptr->ba.cells[n];

            }
        }
    }
    else
    {
        for( n=0, i=0; n < num_sorted_cells; n++ )
        {
#ifdef FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17
            /* If timeout, bring the cell out of the backlist */
            if( l1_sc_main_data_ptr->ba.cells[n]->next_tick <= l1_sc_globals_ptr->surround_tick )
            {
                if(l1_sc_main_data_ptr->ba.cells[n]->in_blacklist)
                {
                  MSG_GERAN_MED_4_G("SCE: De-blacklisting(2): FN: %d, s-tick: %d, BL-end: %d, arfcn: %d",
                    GSTMR_GET_FN_GERAN(gas_id),
                    l1_sc_globals_ptr->surround_tick,
                    l1_sc_main_data_ptr->ba.cells[n]->next_tick,
                    l1_sc_main_data_ptr->ba.cells[n]->bs.arfcn.num);
                }
                l1_sc_main_data_ptr->ba.cells[n]->in_blacklist = FALSE;
            }
            /* remove the blacklist when total number of ncell is less than 6 */
            if ( ( num_sorted_cells < 6 ) && l1_sc_main_data_ptr->ba.cells[n]->in_blacklist )
#endif
            {
                l1_sc_main_data_ptr->ba.cells[n]->in_blacklist = FALSE;
            }

            /* Copy all cells to sorted list except serving
             * cell and cells below power threshold.
             */
            if( !ARFCNS_EQUAL(l1_sc_main_data_ptr->ba.cells[n]->bs.arfcn, l1_sc_main_data_ptr->ba.serving_cell_arfcn) &&
                (l1_sc_main_data_ptr->ba.cells[n]->bs.rx_power > SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16)
                 && (!l1_sc_main_data_ptr->ba.cells[n]->in_blacklist ))
            {
                l1_sc_globals_ptr->sorted_cells[ i++ ] = l1_sc_main_data_ptr->ba.cells[n];
            }
        }
    }

    if ( i > ( sizeof( l1_sc_globals_ptr->sorted_cells ) / sizeof( l1_sc_globals_ptr->sorted_cells[0] ) ) )
    {
      MSG_GERAN_ERROR_3_G( "Too many sorted cells %d clamping to max %d",
                 i,
                 ( sizeof( l1_sc_globals_ptr->sorted_cells ) / sizeof( l1_sc_globals_ptr->sorted_cells[0] ) ),
                 0 );

      i = ( sizeof( l1_sc_globals_ptr->sorted_cells ) / sizeof( l1_sc_globals_ptr->sorted_cells[0] ) );
    }

    num_sorted_cells                = i;
    l1_sc_globals_ptr->sorted_cell_count = (uint8)i;
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr = (uint8) i;
#endif

    /* Save previous ranks so we can spot cells entering or
     * leaving top 6, etc.
     */
    for( n=0; n < num_sorted_cells; n++ )
    {
        l1_sc_globals_ptr->sorted_cells[ n ]->prev_rank = l1_sc_globals_ptr->sorted_cells[ n ]->rank;
    }

    /* Put cells in order of rx_power and fill in rank
     * fields correspondingly.
     */
    if(num_sorted_cells == 1)
    {
        l1_sc_globals_ptr->sorted_cells[0]->rank = 0;
    }
    else if(num_sorted_cells > 1)
    {
        qsort(l1_sc_globals_ptr->sorted_cells, num_sorted_cells, sizeof(cell_T*), compare_balist);
    }

    for( n=0; n < num_sorted_cells; n++ )
    {
        l1_sc_globals_ptr->sorted_cells[ n ]->rank = (uint8)n;
    }

    /* Re-sort for multiband if necessary */
    multiband_parameter = l1_sc_main_data_ptr->ba.multiband_reporting;

   /* Only perform Multiband sort of the BAlist when in dedicated CS operation*/
    if( multiband_parameter &&
        ((l1_sc_globals_ptr->mode == L1SCModeDediSDCCh) ||
         (l1_sc_globals_ptr->mode == L1SCModeDediTCh))
      )
    {
        l1_sc_balist_multiband_sort( multiband_parameter, gas_id );
    }

    l1_sc_post_sort_process(gas_id);

#ifdef DEBUG_BALIST_CHECK_FOR_DUPS
    #error code not present
#endif

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif
}


/*===========================================================================

FUNCTION  l1_sc_balist_sort_restrict_ba_len

DESCRIPTION
 Restricts sorted BA list size to n in top-2 mode
 Currently, called only in Idle mode

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  

===========================================================================*/
void l1_sc_balist_sort_restrict_ba_len( boolean include_serving, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);

    l1_sc_balist_sort( include_serving, gas_id );

    if (gpl1_gprs_meas_mode_ptr->nc_mode == FALSE)
    {
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
        if(l1_sc_globals_ptr->reduced_ncell_list_bmask == SCE_REDUCED_NCELL_LIST_ENABLED)
        {
           /* top 2 mode */
           uint8 n;
           uint8 max_cell_count = l1_sc_globals_ptr->sorted_cell_count;
           uint8 bsic_known_cells;

           /* only check top 6 cells */
           if(max_cell_count > SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL)
           {
              max_cell_count = SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL;
           }

           /* stop when 2 good cells are found in top 6 */
           for(n=0, bsic_known_cells=0 ; 
               (n<max_cell_count) && (bsic_known_cells < SCE_MAX_SORTED_CELLS_IDLE_MODE_STRONG_SCELL); 
               n++)
           {
              if(l1_sc_globals_ptr->sorted_cells[n]->bs.bsic_known)
              {
                 bsic_known_cells++;
              }
           }

           /* we have atleast 2 good cells in top 6 */
           if(bsic_known_cells == SCE_MAX_SORTED_CELLS_IDLE_MODE_STRONG_SCELL)
           {
              /* use top n (2 to 6) mode */
              l1_sc_globals_ptr->sorted_cell_max_count = n;
              MSG_GERAN_MED_1_G("SCE: Top %d mode", n);
           }
           else
           {
             /* fallback to top 6 mode */
              l1_sc_globals_ptr->sorted_cell_max_count = SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL;
              MSG_GERAN_MED_0_G("SCE: Top 6 mode as insufficient bsic_known cells");
           }
        }
        else
        {
           /* top 6 mode */
           l1_sc_globals_ptr->sorted_cell_max_count = SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL;
           MSG_GERAN_MED_0_G("SCE: Top 6 mode as scell not good");
        }

        /* Report top-6 cells to RR even if we are in top-2 mode */
        if (l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr > SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL)
        {
            l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr = SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL;
        }

#else /* #ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST */
        /* top 6 mode */
        l1_sc_globals_ptr->sorted_cell_max_count = SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL;
#endif

        /* Restrict the sorted cell count to max (<=6) in Idle mode */
        if (l1_sc_globals_ptr->sorted_cell_count > l1_sc_globals_ptr->sorted_cell_max_count)
        {
            l1_sc_globals_ptr->sorted_cell_count = l1_sc_globals_ptr->sorted_cell_max_count;
        }
    }

}


/*===========================================================================

FUNCTION  l1_sc_balist_new_cell

DESCRIPTION
  Allocates a cell from the pool of unused cells and initialises it.

  IN - arfcn - ARFCN of the new cell.

DEPENDENCIES
  Ba list must have been previously initialized by calling
  l1_sc_balist_init().

RETURN VALUE
  cell_T *   - Pointer to the new cell structure.

SIDE EFFECTS
  Reduces the cell count from the unused cell pool in the ba structure.

===========================================================================*/
static cell_T* l1_sc_balist_new_cell( ARFCN_T arfcn, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    cell_T*  cell_ptr = NULL;

    ASSERT_TASK();

  /* Grab a cell from the end of the unused cell pool
   * Protect against buffer overflow.
   * The design requires that one unused_cell array remains
   * empty on index 0.
   */
  if(  ( l1_sc_main_data_ptr->ba.unused_cell_count > 1 )
     &&( l1_sc_main_data_ptr->ba.unused_cell_count <= (2 * BA_CELL_POOL_LEN) )
    )
  {
    --l1_sc_main_data_ptr->ba.unused_cell_count;

    cell_ptr                        = l1_sc_main_data_ptr->ba.unused_cells[ l1_sc_main_data_ptr->ba.unused_cell_count ];

    /*check_ba_exception*/
    l1_sc_balist_check_cell( gas_id, cell_ptr);

    cell_ptr->bs.arfcn              = arfcn;
    cell_ptr->bs.rx_power           = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;
    cell_ptr->bs.prev_rx_power      = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;
    cell_ptr->bs.bsic_known         = FALSE;
    cell_ptr->bs.tone_detected      = FALSE;
    cell_ptr->bs.meas_state         = L1SCMeasNone;
    cell_ptr->bs.rx_power_sum       = 0;
    cell_ptr->bs.prev_rx_power_sum  = 0;
    cell_ptr->bs.rx_power_count     = 0;
    cell_ptr->bs.prev_rx_power_count= 0;

    /* Initialize the cell's AGC structure */
    sys_algo_agc_init(&cell_ptr->bs.agc, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16);
    sys_algo_agc_disable_slew_rate_limiting(&cell_ptr->bs.agc);

#ifdef FEATURE_GSM_RX_DIVERSITY
    sys_algo_agc_init(&cell_ptr->bs.agc_divrx, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16);
    sys_algo_agc_disable_slew_rate_limiting(&cell_ptr->bs.agc_divrx);
#endif

    /* Clear the running average measurement buffer. */
    l1_running_avg_clear_meas( &(cell_ptr->bs.ra) );
    l1_running_avg_set_avg_len( &(cell_ptr->bs.ra), L1_SC_MAX_MEAS_BUF_LEN, gas_id );

    cell_ptr->rank                  = L1_SC_NO_RANK;
    cell_ptr->prev_rank             = L1_SC_NO_RANK;
    cell_ptr->in_new_ba_list        = TRUE;
    cell_ptr->in_old_ba_list        = FALSE;
    cell_ptr->in_fade               = FALSE;
    cell_ptr->cell_stale_sch        = FALSE;
    cell_ptr->next_tick             = 0;
    cell_ptr->try_no                = 0;
    cell_ptr->fcb_try_no            = 0;
    cell_ptr->in_blacklist          = FALSE;
    cell_ptr->reassert              = FALSE;
    cell_ptr->fn_when_lost          = -32 * MAX_TCH_TONE_DETECT_ATTEMPTS * MULTIFRAME_26;
    cell_ptr->sb_count_to_bcch      = BCCH_DONT_DO;
#ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS
    cell_ptr->pwr_msr_enabled       = TRUE;
    cell_ptr->pwr_msr_disabled_till_tick = 0;
#endif
  }
  else
  {
    MSG_GERAN_ERROR_3_G(" New arfcn %d not added index %d ",
              arfcn.num,
              (l1_sc_main_data_ptr->ba.unused_cell_count-1),
              0 );
  }

  return  cell_ptr;
}

/*===========================================================================

FUNCTION  l1_sc_balist_delete_cell

DESCRIPTION
  Returns a cell to the pool of unused cells.

  IN - cell - pointer to the cell to be deleted.

DEPENDENCIES
  Cell must have prciously been allocated by a call to
  l1_sc_balist_new_cell().

RETURN VALUE
  None

SIDE EFFECTS
  Cell is also removed from l1_sc_globals struct sorted cell list.

===========================================================================*/
static void l1_sc_balist_delete_cell( cell_T* cell, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    uint8 sorted_cell_count_to_iterate;
    uint8 n;

    ASSERT_TASK();

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    sorted_cell_count_to_iterate = l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr;
#else
    sorted_cell_count_to_iterate = l1_sc_globals_ptr->sorted_cell_count;
#endif

    /* TODO maybe remove cell from best six earlier */
    n = cell->rank;

    if( (sorted_cell_count_to_iterate > 0) && (n != L1_SC_NO_RANK)
      && (n < sorted_cell_count_to_iterate) )
    {
        --sorted_cell_count_to_iterate;
        for( ; ((n < sorted_cell_count_to_iterate) && (n < BA_MAX_LEN)); ++n )
        {
            l1_sc_globals_ptr->sorted_cells[ n ]       = l1_sc_globals_ptr->sorted_cells[ n + 1 ];
            l1_sc_globals_ptr->sorted_cells[ n ]->rank = (uint8)n;
        }
    }

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr = sorted_cell_count_to_iterate;

    /* if we delete cell 3-6 in top-2 mode, then no need to reduce sorted_cell_count.
     * unless it goes below the number of cells reported to RR.
     */
    if(sorted_cell_count_to_iterate < l1_sc_globals_ptr->sorted_cell_count)
    {
      l1_sc_globals_ptr->sorted_cell_count = sorted_cell_count_to_iterate;
    }
#else
    l1_sc_globals_ptr->sorted_cell_count = sorted_cell_count_to_iterate;
#endif

    /* Make sure we are not over-indexing into this array */
    if( l1_sc_main_data_ptr->ba.unused_cell_count < (2 * BA_CELL_POOL_LEN) )
    {
      l1_sc_main_data_ptr->ba.unused_cells[ l1_sc_main_data_ptr->ba.unused_cell_count ] = cell;

      l1_sc_main_data_ptr->ba.unused_cell_count++;
    }
    else
    {
      /* Print an error message */
      MSG_GERAN_ERROR_3_G(" arfcn %d not added on count %d ",
                cell->bs.arfcn.num,
                l1_sc_main_data_ptr->ba.unused_cell_count,
                0 );

      /* Cap the value so its usable next time
       * Use the array size and not the highest permissible index
       * due to the way the add new cell function works, otherwise
       * one location would be wasted.
       */
      l1_sc_main_data_ptr->ba.unused_cell_count = (2 * BA_CELL_POOL_LEN);
    }

#ifdef DEBUG_BALIST_CHECK_FOR_DUPS
    #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_sc_balist_get_best_six

DESCRIPTION
  Populates the array best_six with pointers to the bs fields of
  the best six cells in the BA list.

DEPENDENCIES
  BA list must have been sorted by calling l1_sc_balist_sort().

RETURN VALUE
  Number of cells in list (might be less than 6).

SIDE EFFECTS
  Accesses l1_sc_globals struct to get sorted BA list.

===========================================================================*/
byte l1_sc_balist_get_best_six( l1_sc_bs_T* best_six[ MAX_NUMBER_OF_SURROUND_CELLS ], gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    word              n;
    volatile word     count              = 0;
    cell_T           *cell;
    uint8             max_num_ncells_to_rr;

    /******************************************************************/
    /* TO OPTIMIZE: HAVE PUT IN A QUICK FIX FOR MULTIBAND REP         */
    /******************************************************************/

    ASSERT_TASK();

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
    /* In idle mode, sorted_cell_count_rpt_to_rr can be different from sorted_cell_count.
     * In other modes, it is same.
     */
    max_num_ncells_to_rr = l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr;
#else
    max_num_ncells_to_rr = l1_sc_globals_ptr->sorted_cell_count;
#endif

    for( n = 0; n < max_num_ncells_to_rr && count < MAX_NUMBER_OF_SURROUND_CELLS; ++n )
    {
        cell = l1_sc_globals_ptr->sorted_cells[ n ];

        if ((l1_sc_globals_ptr->mode == L1SCModeDediSDCCh) ||
            (l1_sc_globals_ptr->mode == L1SCModeDediTCh))
        {
          if( ( cell->bs.bsic_known == TRUE )&& (( cell->next_tick + ( 3*TEN_SECONDS ) ) < l1_sc_globals_ptr->surround_tick ))
          {
            MSG_GERAN_HIGH_0_G("Do not report Ncell if the bsic is not reconfirmed within the last 30 seconds");
            cell->cell_stale_sch = TRUE;
          }
        }

        /* If the BSIC is known then report this cell. Have removed the check on retries */
#ifdef FEATURE_GSM_SENS_SUPPORT
        if (( cell->bs.bsic_known ) && (cell->fcb_try_no == 0) && (cell->cell_stale_sch == FALSE) && (cell->reassert == FALSE))
#else
        if (( cell->bs.bsic_known ) && (cell->fcb_try_no == 0) && (cell->cell_stale_sch == FALSE))
#endif

        {
          byte ncc = ( cell->bs.bsic >> 3 ) & 7;
          if( ( l1_sc_main_data_ptr->ba.ncc_permitted & ( 1 << ncc ) ) != 0 )
          {
            *best_six = &cell->bs;
            ++best_six;
            ++count;

            if( cell->sb_count_to_bcch == BCCH_DO_AFTER_MEAS_RPT )
            {
              if( !l1_sc_globals_ptr->autonomous_bcch )
              {
                cell->sb_count_to_bcch = BCCH_DONT_DO;
              }
              else
              {
                /* RR will learn about this cell soon, so enable receives of BCCh sys infos 3 and 4 */
                cell->bcch_mask        = BCCH_MASK_3;
                cell->bcch_clear_mask  = BCCH_MASK_34;
                cell->sb_count_to_bcch = BCCH_DO_NOW;
              }
            }
          }
        }
    }
    return(uint8)count;
}

/*===========================================================================

FUNCTION  l1_sc_balist_get_cell_by_arfcn

DESCRIPTION
  Returns the cell using the given ARFCN.

  IN - arfcn - ARFCN of the cell to locate.

DEPENDENCIES
  None

RETURN VALUE
  Cell using arfcn or NULL if no cell uses the arfcn.

SIDE EFFECTS
  None

===========================================================================*/
cell_T* l1_sc_balist_get_cell_by_arfcn( ARFCN_T arfcn, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    cell_T*  cell_ptr = NULL;

    ASSERT_TASK();

    /* Sanity check the pptr */
    if( NULL != l1_sc_main_data_ptr->ba.cells )
    {
      word  n;

      for( n = 0; n < l1_sc_main_data_ptr->ba.cell_count; ++n )
      {
        /* Sanity check the pointer */
        if( NULL !=  l1_sc_main_data_ptr->ba.cells[ n ] )
        {

          /*check_ba_exception*/
          l1_sc_balist_check_cell( gas_id, l1_sc_main_data_ptr->ba.cells[ n ]);

          if( ARFCNS_EQUAL(l1_sc_main_data_ptr->ba.cells[ n ]->bs.arfcn, arfcn) )
          {
            cell_ptr = l1_sc_main_data_ptr->ba.cells[ n ];

            /* map the afc_info.freq_err to the frequency_offset
             * to be used in the target cell.
             */
            if( FALSE != cell_ptr->afc_info.valid )
            {
              cell_ptr->bs.frequency_offset = cell_ptr->afc_info.freq_error;
            }
            else
            {
              cell_ptr->bs.frequency_offset = 0;
            }
#ifdef FEATURE_GERAN_REDUCED_F3
            MSG_GERAN_MED_3_G( "mapping arfcn %d freq error %d Hz valid %d",
                     arfcn.num, cell_ptr->bs.frequency_offset, cell_ptr->afc_info.valid );
#endif
            break;
          }
        }
        else
        {
          MSG_GERAN_ERROR_3_G(" NULL cells[] ptr at index %d of count %d",
                    n,
                    l1_sc_main_data_ptr->ba.cell_count,
                    0 );
        }

      } /* End of for( ) loop */

    }
    else
    {
      MSG_GERAN_ERROR_0_G(" NULL l1_sc_main_data_ptr->ba.cells pptr ");
    }

    /* Return the pointer to caller */
    return  cell_ptr;
}

cell_T**  l1sc_get_ba_cells( gas_id_t gas_id )
{
  l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

  return  l1_sc_main_data_ptr->ba.cells;
}

uint8     l1sc_get_ba_cell_count( gas_id_t gas_id )
{
  l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

  return  l1_sc_main_data_ptr->ba.cell_count;
}

/*===========================================================================

FUNCTION  l1sc_get_ba_blacklisted_cell_count

DESCRIPTION
 Returns number of blacklisted cells

DEPENDENCIES
  None

RETURN VALUE
  Number of blacklisted cells

SIDE EFFECTS
  None

===========================================================================*/
uint8     l1sc_get_ba_blacklisted_cell_count( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    uint32    i;
    uint32    num_cells = 0;

    for( i=0; i < l1_sc_main_data_ptr->ba.cell_count; i++ )
    {
        if( l1_sc_main_data_ptr->ba.cells[i]->in_blacklist )
        {
            num_cells++;
        }
    }

    return num_cells;
}

/*===========================================================================

FUNCTION  l1_sc_balist_is_cell_in_new_ba_list

DESCRIPTION
 Determines if the specified cell is in the new BA list

DEPENDENCIES
  None

RETURN VALUE
  Cell using arfcn or NULL if no cell uses the arfcn.

SIDE EFFECTS
  None

===========================================================================*/
cell_T* l1_sc_balist_is_cell_in_new_ba_list( ARFCN_T arfcn, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    cell_T*  cell_ptr = NULL;

    ASSERT_TASK();

    /* Sanity check the pptr */
    if( NULL != l1_sc_main_data_ptr->ba.new_cells )
    {
      word  n;

      for( n = 0; n < l1_sc_main_data_ptr->ba.new_cell_count; ++n )
      {
        /* Sanity check the pointer */
        if( NULL != l1_sc_main_data_ptr->ba.new_cells[ n ] )
        {

    /*check_ba_exception*/
          l1_sc_balist_check_cell( gas_id, l1_sc_main_data_ptr->ba.new_cells[ n ]);

          if( ARFCNS_EQUAL(l1_sc_main_data_ptr->ba.new_cells[ n ]->bs.arfcn, arfcn) )
          {
              cell_ptr = l1_sc_main_data_ptr->ba.new_cells[ n ];
              break;
          }
        }
        else
        {
          MSG_GERAN_ERROR_3_G(" NULL new_cells[] ptr at index %d of count %d",
                    n,
                    l1_sc_main_data_ptr->ba.new_cell_count,
                    0 );
        }

      } /* End of for( ) loop */

    }
    else
    {
      MSG_GERAN_ERROR_0_G(" NULL l1_sc_main_data_ptr->ba.new_cells pptr ");
    }

    /* Return the pointer to caller */
    return  cell_ptr;
}

/*===========================================================================

FUNCTION  l1_sc_balist_get

DESCRIPTION
  Provides a pointer to the array of cells making up the BA list.  If a new
  BA list is pending then the array contains the cells in the new BA list.

DEPENDENCIES
  None

RETURN VALUE
  Number of cells in the BA list or the new BA list.

SIDE EFFECTS
  None

===========================================================================*/
word l1_sc_balist_get( cell_T*** cells, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    ASSERT_TASK();

    if(l1_sc_main_data_ptr->ba.new_ba_list_pending)
    {
        *cells = l1_sc_main_data_ptr->ba.new_cells;
        return l1_sc_main_data_ptr->ba.new_cell_count;
    }
    else
    {
        *cells = l1_sc_main_data_ptr->ba.cells;
        return l1_sc_main_data_ptr->ba.cell_count;
    }
}

/*===========================================================================

FUNCTION  l1_sc_balist_get_best_thirtytwo

DESCRIPTION
  Returns data for the best (up to) thirtytwo cells depending on the mode.

  OUT - best_thirtytwo - Data for best cells.

DEPENDENCIES
  BA list must have been sorted by calling l1_sc_balist_sort().

RETURN VALUE
  Number of cells written to best_thirtytwo.

SIDE EFFECTS
  Accesses l1_sc_globals struct to get sorted BA list.

===========================================================================*/
byte l1_sc_balist_get_best_thirtytwo( l1_sc_bs_T* best_thirtytwo[ 32 ], gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    word n;
    word count = 0;

    ASSERT_TASK();

    for( n = 0; n < l1_sc_globals_ptr->sorted_cell_count && count < 32; ++n )
    {
        cell_T* cell = l1_sc_globals_ptr->sorted_cells[ n ];
        if( cell->bs.bsic_known )
        {
            byte ncc = ( cell->bs.bsic >> 3 ) & 7;
            if( ( l1_sc_main_data_ptr->ba.ncc_permitted & ( 1 << ncc ) ) != 0 )
            {
                *best_thirtytwo = &cell->bs;
                ++best_thirtytwo;
                ++count;

                if( cell->sb_count_to_bcch == BCCH_DO_AFTER_MEAS_RPT )
                {
                    if( !l1_sc_globals_ptr->autonomous_bcch )
                    {
                        cell->sb_count_to_bcch = BCCH_DONT_DO;
                    }
                    else
                    {
                        /* RR will learn about this cell soon, so enable receives of BCCh sys infos 3 and 4 */
                        cell->bcch_mask        = BCCH_MASK_3;
                        cell->bcch_clear_mask  = BCCH_MASK_34;
                        cell->sb_count_to_bcch = BCCH_DO_NOW;
                    }
                }
            }
        }
        else
        {
            *best_thirtytwo = &cell->bs;
            ++best_thirtytwo;
            ++count;
        }
    }

    return(uint8)count;

}


/*===========================================================================

FUNCTION  l1_sc_balist_set_serving_cell

DESCRIPTION
  Sets the serving cell arfcn.

  IN - arfcn - The serving cells ARFCN.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_set_serving_cell( ARFCN_T arfcn, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    ASSERT_TASK();

    l1_sc_main_data_ptr->ba.serving_cell_arfcn = arfcn;

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_sc_balist_get_cell_data

DESCRIPTION
  Returns the bs data of the cell using the given ARFCN.

  IN - arfcn - ARFCN of the cell to locate.

DEPENDENCIES
  None

RETURN VALUE
  BS data or NULL if no cell uses the arfcn.

SIDE EFFECTS
  None

===========================================================================*/
l1_sc_bs_T* l1_sc_balist_get_cell_data( ARFCN_T arfcn, gas_id_t gas_id )
{
    cell_T* cell;

    ASSERT_TASK();

    cell = l1_sc_balist_get_cell_by_arfcn( arfcn, gas_id );
    if( cell )
    {
        return &cell->bs;
    }
    return NULL;
}


/*===========================================================================

FUNCTION  l1_sc_ba_cell_set_sync

DESCRIPTION
  Transfer offset for the specified cell in BA list

  IN - frame_decrease - How much frame number has been decreased.
  IN - qbit_decrease  - How much quarter bit number has been decreased.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_ba_cell_set_sync( l1_sc_bs_T  *bs,
                             uint32 frame_decrease,
                             int16 qbit_decrease,
                             gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
    gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr =
        gl1_ms_switch_meas_mode_data(gas_id);
    int16   old_qbit_lag;
    int16 ncell_sch_pos_in_acq_window;

    ASSERT_TASK();

    switch(l1_sc_globals_ptr->mode)
    {
#ifdef FEATURE_GSM_DTM
    case L1SCModeDediDTM:
#endif
    case L1SCModeDediTCh:

        if( bs->bsic_known )
        {
            /* Instantiate and initialise the lowest and highest timeslot
             * information structures.
             */
            gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, /* relative timeslot */
                                                          FALSE };       /* transmit boolean  */
            gl1_defs_tn_struct  highest_tn_info = { GL1_DEFS_TN_3, /* relative timeslot */
                                                          FALSE };       /* transmit boolean  */
            old_qbit_lag = bs->qbit_lag;
            bs->qbit_lag -= qbit_decrease;
            bs->fn_lag = SUB_FN( bs->fn_lag, frame_decrease );
            bs->fn_when_read = SUB_FN( bs->fn_when_read, frame_decrease );

            while( bs->qbit_lag >= 5000 )
            {
                bs->qbit_lag -= 5000;
                bs->fn_lag = ADD_FN ( bs->fn_lag, 1 );
                bs->fn_when_read = ADD_FN( bs->fn_when_read, 1 );
            }

            while( bs->qbit_lag <= -5000 )
            {
                MSG_GERAN_HIGH_3_G("Cell %d qbit_lag %d < -5000 fn_lag = %d",bs->arfcn.num,bs->qbit_lag, bs->fn_lag );

                bs->qbit_lag += 5000;
                bs->fn_lag = SUB_FN ( bs->fn_lag, 1 );
                bs->fn_when_read = SUB_FN( bs->fn_when_read, 1 );
            }

            /* If there was a positive to negative transition adjust fn_lag */
            if( old_qbit_lag >= 0 && bs->qbit_lag < 0 )
            {
                bs->fn_lag = SUB_FN( bs->fn_lag, 1 );
                bs->fn_when_read = SUB_FN( bs->fn_when_read, 1 );
            }

            /* If there was a negative to positive transition adjust fn_lag */
            if( old_qbit_lag < 0 && bs->qbit_lag >= 0 )
            {
                bs->fn_lag = ADD_FN( bs->fn_lag, 1 );
                bs->fn_when_read = ADD_FN( bs->fn_when_read, 1 );
            }

#ifdef FEATURE_GSM_DTM
            if( (l1_sc_globals_ptr->mode == L1SCModeDediDTM) && 
                (FALSE != gpl1_gprs_meas_mode_ptr->leave_trans_adjust_sync) )
            {
                if(bs->qbit_lag >= 0 )
                {
                    bs->fn_lag = ADD_FN ( bs->fn_lag, 1 );
                }

                MSG_GERAN_MED_1_G("ADJUST_FN_DTM [%d]",bs->fn_lag);
            }
#endif
            /* gl1_msg_ncell_sch_pos_in_acq_window_dedic() determines the position
            of the SCH within the surround cell acquisition window on the
            TCH.  It returns 1 if lies after the end of the acq window,
            -1 if it lies before the start of the acq window and 0 if it
            lies within.  Thus 1 or -1 would indicate that the SCH lies
            outside the acquisition window and we need to wrap around
            */
            l1_sc_highest_lowest_timslot(&(lowest_tn_info), &(highest_tn_info), gas_id);
            highest_tn_info.tn = (gl1_defs_tn_type) ADD_MOD( (uint32) highest_tn_info.tn, 3, GL1_DEFS_SLOTS_IN_FRAME );

            /*The constraints apllied to the acquisition window differ for dedicated mode*/
            if ( l1_sc_globals_ptr->mode == L1SCModeDediTCh)
            {
              ncell_sch_pos_in_acq_window = gl1_msg_ncell_sch_pos_in_acq_window_dedic(
                                                      bs->qbit_lag,
                                                      &(lowest_tn_info),
                                                      &(highest_tn_info),
                                                      gas_id);
            }
            else
            {
              ncell_sch_pos_in_acq_window = gl1_msg_ncell_sch_pos_in_acq_window(
                                                      bs->qbit_lag,
                                                      &(lowest_tn_info),
                                                      &(highest_tn_info),
                                                      gas_id);
            }

            if( ncell_sch_pos_in_acq_window == 1 ) /* Rx and Tx TS */
            {
                /* Slide by a 51 multiframe to handle the wrap but we need
                   to slide by 2*26 TCH multiframes to achieve this
                */
                bs->qbit_lag -= 5000;
                bs->sch_wrap_direction = 1;
            }
            else if( ncell_sch_pos_in_acq_window == -1 )  /* Rx and Tx TS */
            {
                /* Slide by a 51 multiframe to handle the wrap but we need
                   to slide by 2*26 TCH multiframes to achieve this
                */
                bs->qbit_lag += 5000;
                bs->sch_wrap_direction = -1;
            }
            else
            {
                bs->sch_wrap_direction = 0;
            }
        }
        break;


    case L1SCModeDediSDCCh:
    case L1SCModeIdle:

        if( bs->bsic_known )
        {
            MSG_GERAN_LOW_3_G("BEFORE IDLE -> arfcn [%d] qbit_lag [%d] fn_lag[%d] ",bs->arfcn.num, bs->qbit_lag, bs->fn_lag);

            if( FALSE != gpl1_gprs_meas_mode_ptr->leave_trans_adjust_sync )
            {
                if(bs->qbit_lag >= 0 )
                {
                    bs->fn_lag = ADD_FN ( bs->fn_lag, 1 );
                }

                MSG_GERAN_LOW_1_G(" ADJUST_FN_IDLE [%d]",bs->fn_lag);
            }

            old_qbit_lag = bs->qbit_lag;
            bs->qbit_lag -= qbit_decrease;
            bs->fn_lag = SUB_FN( bs->fn_lag, frame_decrease );
            bs->fn_when_read = SUB_FN( bs->fn_when_read, frame_decrease );

            while( bs->qbit_lag >= (int16)(5000U) )
            {
                bs->qbit_lag -= (int16)(5000U);
                bs->fn_lag = ADD_FN ( bs->fn_lag, 1 );
                bs->fn_when_read = ADD_FN( bs->fn_when_read, 1 );
            }

            if( bs->qbit_lag < 0 )
            {
                bs->qbit_lag += 5000;
                bs->fn_lag = SUB_FN( bs->fn_lag, 1 );
                bs->fn_when_read = SUB_FN( bs->fn_when_read, 1 );
            }

            /* If there was a negative to positive transition adjust fn_lag */
            if( old_qbit_lag < 0 && bs->qbit_lag >= 0 )
            {
                bs->fn_lag = ADD_FN( bs->fn_lag, 1 );
                bs->fn_when_read = ADD_FN( bs->fn_when_read, 1 );
            }
        }

        MSG_GERAN_LOW_3_G("AFTER -> arfcn [%d] qbit_lag [%d] fn_lag[%d] ",bs->arfcn.num,bs->qbit_lag,bs->fn_lag);
        break;

    case L1SCModeGprsTrans:

        if( bs->bsic_known || bs->tone_detected )
        {
            /* Instantiate and initialise the lowest and highest timeslot
             * information structures.
             */
            const gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, /* relative timeslot */
                                                          FALSE          /* transmit boolean  */
                                                        };
            gl1_defs_tn_struct        highest_tn_info = { GL1_DEFS_TN_0, /* relative timeslot */
                                                          FALSE          /* transmit boolean  */
                                                        };
            gl1_defs_tn_type          highest_tn;

            MSG_GERAN_LOW_3_G("BEFORE TRANS -> arfcn [%d] qbit_lag [%d] fn_lag[%d] ",bs->arfcn.num,bs->qbit_lag,bs->fn_lag);

            if( FALSE != gpl1_gprs_meas_mode_ptr->enter_trans_adjust_sync )
            {
                bs->fn_lag = SUB_FN( bs->fn_lag, 1 );
                bs->fn_when_read = SUB_FN( bs->fn_when_read, 1 );
                MSG_GERAN_LOW_1_G(" ADJUST_FN_TRANS [%d]",bs->fn_lag);
            }

            bs->qbit_lag -= qbit_decrease;


            /* Adjust FN lag and qbit lag if qbit lag is greater than 5000QS*/
            while( bs->qbit_lag >= 5000 )
            {
                bs->qbit_lag -= 5000;
                bs->fn_lag = ADD_FN ( bs->fn_lag, 1 );
                bs->fn_when_read = ADD_FN( bs->fn_when_read, 1 );
            }
            /*  Adjust FN lag and qbit lag if qbit lag is negetive*/
            while( bs->qbit_lag <= -5000)
            {
                MSG_HIGH("Cell ARFCN num = %d ; qbit_lag = %d (< 0) ; fn_lag = %d",bs->arfcn.num,bs->qbit_lag, bs->fn_lag );

                bs->qbit_lag += 5000;
                bs->fn_lag = SUB_FN ( bs->fn_lag, 1 );
                bs->fn_when_read = SUB_FN( bs->fn_when_read, 1 );
            }

            /* gl1_msg_ncell_sch_pos_in_acq_window() determines the position
            of the SCH within the surround cell acquisition window on the
            TCH.  It returns 1 if lies after the end of the acq window,
            -1 if it lies before the start of the acq window and 0 if it
            lies within.  Thus 1 or -1 would indicate that the SCH lies
            outside the acquisition window and we need to wrap around
            */

            if(gprs_serv_cell_meas_ptr->highest_tn == 3) /*lint !e641 */
            {
                highest_tn = (gl1_defs_tn_type)(gprs_serv_cell_meas_ptr->highest_tn  + 2);
            }
            else
            {
                highest_tn = (gl1_defs_tn_type)(gprs_serv_cell_meas_ptr->highest_tn + 3);
            }

            /* Set the highest timeslot information */
            highest_tn_info.tn =  highest_tn;

            switch( gl1_msg_ncell_sch_pos_in_acq_window( bs->qbit_lag,
                                                         &(lowest_tn_info),
                                                         &(highest_tn_info),
                                                         gas_id ) )
            {
            case  1:  /* Rx and Tx TS */
                /* Slide by a 51 multiframe to handle the wrap but we need
                   to slide by 2*26 TCH multiframes to achieve this
                */
                bs->qbit_lag -= 5000;
                bs->fn_lag = ADD_FN( bs->fn_lag, 1 );
                bs->fn_when_read = ADD_FN( bs->fn_when_read, 1 );
                break;

            case  -1:  /* Rx and Tx TS */
                /* Slide by a 51 multiframe to handle the wrap but we need
                   to slide by 2*26 TCH multiframes to achieve this
                */
                bs->qbit_lag += 5000;
                bs->fn_lag = SUB_FN( bs->fn_lag, 1 );
                bs->fn_when_read = SUB_FN( bs->fn_when_read, 1 );
                break;

            default:
                /* Don't do anything for this case */
                break;
            }
        }
        MSG_GERAN_LOW_3_G("AFTER TRANS-> arfcn [%d] qbit_lag [%d] fn_lag[%d] ",bs->arfcn.num,bs->qbit_lag,bs->fn_lag);

        break;

    default:
        MSG_GERAN_ERROR_1_G("Unexpected mode %d",l1_sc_globals_ptr->mode);
        break;
    }
}
/*===========================================================================

FUNCTION  l1_sc_balist_set_sync

DESCRIPTION
  Inform the surround cell engine about a change in synchronisation or
  time slot.

  IN - frame_decrease - How much frame number has been decreased.
  IN - qbit_decrease  - How much quarter bit number has been decreased.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_balist_set_sync( uint32 frame_decrease, int16 qbit_decrease, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
    word    n;

    ASSERT_TASK();

    /* Loop through BA list and translate offsets for each cell */
    for( n = 0; n < l1_sc_main_data_ptr->ba.cell_count; ++n )
    {
        cell_T* cell = l1_sc_main_data_ptr->ba.cells[ n ];

  /*check_ba_exception*/
        l1_sc_balist_check_cell(gas_id, cell);

        l1_sc_ba_cell_set_sync( &cell->bs, frame_decrease, qbit_decrease, gas_id );
    }

    /* Reset the pkt transfer enter/leave adjust sync flags to
     * prevent further adjustement of ncell qbit/fn lag.
     */
    switch(l1_sc_globals_ptr->mode)
    {
    case L1SCModeDediSDCCh:
    case L1SCModeIdle:
#ifdef FEATURE_GSM_DTM
    case L1SCModeDediDTM:
#endif

        gpl1_gprs_meas_mode_ptr->leave_trans_adjust_sync = FALSE;
        break;

    case L1SCModeGprsTrans:

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
        if((frame_decrease == 0) && (qbit_decrease == 0))
        {
          MSG_GERAN_HIGH_0_G("SCE: No timeslot change, don't call compute_ccch_event_qbit_lag()");
        }
        else
        {
          gpl1_gprs_compute_ccch_event_qbit_lag(gas_id);
        }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

        gpl1_gprs_meas_mode_ptr->enter_trans_adjust_sync = FALSE;
        break;

    default:
        /* MSG Error is already called from within
         * l1_sc_ba_cell_set_sync( ) for the default
         * case.
         */
        break;
    }

    if(l1_sc_main_data_ptr->ba.serving_cell_valid)
    {
        /**********************************
         * add code to do ba.serving_cell *
         **********************************/
    }

}


void l1_sc_balist_maintain_sc_counter(cell_T* cell, gas_id_t gas_id)
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint8 ticks_remaining = 0;
   uint32 time_to_next_tick_curr_mode = 0;
   uint32 time_to_next_tick_new_mode = 0;

   if (cell->bs.bsic_known == FALSE)
   {
     cell->next_tick = 0;
     return;
   }

   /*Moving from Dedicated to Idle Mode 
     Retain the next reconfirmation time as it is. Just need to convert from Ded/Transfer Tick counter to Idle tick counter.

     Logic used:
       Get_Next_Reconfimr_Occassion in msec ( diff(next_tick,curr_tick) * Time_for_each_tick)
       Convert this into ticks of new mode.
   */
    if ((l1_sc_globals_ptr->prev_mode == L1SCModeDediSDCCh|| l1_sc_globals_ptr->prev_mode == L1SCModeDediTCh || l1_sc_globals_ptr->prev_mode == L1SCModeGprsTrans) &&
       l1_sc_globals_ptr->mode == L1SCModeIdle)
    {
       if (l1_sc_globals_ptr->prev_mode == L1SCModeDediTCh ||
           l1_sc_globals_ptr->prev_mode == L1SCModeDediSDCCh)      
       {
          time_to_next_tick_curr_mode = (cell->next_tick - l1_sc_globals_ptr->surround_tick) * TICK_TO_MS_DED;
       }
       else if (l1_sc_globals_ptr->prev_mode == L1SCModeGprsTrans)
       {
          time_to_next_tick_curr_mode = (cell->next_tick - l1_sc_globals_ptr->surround_tick) * TICK_TO_MS_TRANS;
       }
       time_to_next_tick_new_mode = time_to_next_tick_curr_mode;

       MSG_GERAN_HIGH_4_G("Moving from DED to Idle, ARFCN:%d, Surr_Tick:%d, Present Tick:%d, TimeRemaining:%d", 
                           cell->bs.arfcn.num, l1_sc_globals_ptr->surround_tick,
                           cell->next_tick, time_to_next_tick_curr_mode);
       
       if (cell->next_tick < l1_sc_globals_ptr->surround_tick)
       {
          /*Reconfirm at the earliest, surround_tick is reset to 0 later on*/
          cell->next_tick = 0;
       }
       else
       {
         cell->next_tick = (time_to_next_tick_new_mode / TICK_TO_MS_IDLE);
       }
       MSG_GERAN_HIGH_2_G("Moving from DED to Idle, NewTick:%d, TimeRemaining:%d", cell->next_tick, time_to_next_tick_new_mode);
    }
   /*Moving from Idle Mode to Dedicated
     Retain the next reconfirmation time if it was done within last 10secs.
     Anything beyond 10sec, need to reconfirm immediately in dedicated modes.
     
     Logic used:
       Get_Next_Reconfimr_Occassion in msec ( diff(next_tick,curr_tick) * Time_for_each_tick)
       Convert this into ticks of new mode.
   */    
    else if (l1_sc_globals_ptr->prev_mode == L1SCModeIdle &&
        (l1_sc_globals_ptr->mode == L1SCModeDediSDCCh || 
        l1_sc_globals_ptr->mode == L1SCModeDediTCh || 
        l1_sc_globals_ptr->mode == L1SCModeGprsTrans))
    {
      time_to_next_tick_curr_mode = (cell->next_tick - l1_sc_globals_ptr->surround_tick) * TICK_TO_MS_IDLE;
      time_to_next_tick_new_mode = MIN(time_to_next_tick_curr_mode,10000);

      MSG_GERAN_HIGH_4_G("Moving from Idle to Ded, ARFCN:%d, Surr_Tick:%d, Present Tick:%d, TimeRemaining:%d", 
                          cell->bs.arfcn.num, l1_sc_globals_ptr->surround_tick,
                          cell->next_tick, time_to_next_tick_curr_mode);
      
      if (cell->next_tick > l1_sc_globals_ptr->surround_tick)
      {
        /*If reconfirmation is pending beyond 10sec, then limit to 10sec*/
       if (l1_sc_globals_ptr->mode == L1SCModeDediTCh)  
       {   
          cell->next_tick = (time_to_next_tick_new_mode / TICK_TO_MS_DED);
       }
       else if (l1_sc_globals_ptr->mode == L1SCModeGprsTrans) 
       {
         cell->next_tick = (time_to_next_tick_new_mode / TICK_TO_MS_TRANS);
       }
      }
      else
      {
          cell->next_tick              = 0;
      }
      MSG_GERAN_HIGH_2_G("Moving from Idle to Dedicated, NewTick:%d, TimeRemaining:%d", cell->next_tick, time_to_next_tick_new_mode);
    }
    else if (l1_sc_globals_ptr->prev_mode == l1_sc_globals_ptr->mode)
    {
       if (cell->next_tick > l1_sc_globals_ptr->surround_tick)
       {  
         //Surround tick is reset to 0 in l1_sc_balist_reset_counters(). So just carry forward the delta
         cell->next_tick = cell->next_tick - l1_sc_globals_ptr->surround_tick;
         MSG_GERAN_HIGH_1_G("No Change in SC Mode, Retain Tick:%d", cell->next_tick);
       }
       else
       {
          cell->next_tick = 0;
       }
    }
    else
    {
      cell->next_tick              = 0;
    }
}

/*===========================================================================

FUNCTION  l1_sc_balist_reset_counters

DESCRIPTION
 Clears the all the timing information for each cell in the BA list.  This
 means the SCE will need to re-acquire each cell if it wants to regenerate
 the information.

 The power measurements accumulated for each cell are preserved.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Also clears l1_sc_globals.surround_tick.

===========================================================================*/
void l1_sc_balist_reset_counters( gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    word n;
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    for( n = 0; n < l1_sc_main_data_ptr->ba.cell_count; ++n )
    {
        cell_T* cell                 = l1_sc_main_data_ptr->ba.cells[ n ];

        /*check_ba_exception*/
        l1_sc_balist_check_cell(gas_id, cell);

        /* reset next_tick and surround tick on state change,
         * new values will be assigned later based on the new GL1 state
         */
        //cell->next_tick              = 0;
        l1_sc_balist_maintain_sc_counter(cell, gas_id);
        cell->in_blacklist           = FALSE;

#ifdef FEATURE_GSM_SYNQ_SUPPORT
        /* reset synq fr related params during dedicated channel transitions */
        if( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )
        {
          cell->bad_gdet_pwr_mon_cntr  = 0;
          cell->gdet_based_sch_reconfirm = FALSE;
        }
#endif


        if( ( l1_tskisr_blk->l1_state == L1_DEDICATED_MODE ) && ( cell->bs.bsic_known == TRUE ) && ( cell->reassert == TRUE )  )
        {
          MSG_GERAN_MED_0_G(" Do not reset the SeNS flag in dedicated mode, BSIC was known prviously and SeNS flag was set" );
        }
        else if( ( cell->bs.bsic_known == TRUE ) && ( cell->reassert == TRUE ) )
        {
          MSG_GERAN_MED_0_G(" Not in dedicated mode, SeNS flag was set and BSIC was known earlier, reset the BSIC known and SeNS flags ");
          cell->bs.bsic_known          = FALSE;
          cell->reassert               = FALSE;
        }
        else
        {
          cell->reassert				 = FALSE;
        }

        if( cell->try_no != 0 )
        {
            cell->try_no = 0;
            l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                              cell->rank,
                              cell->bs.arfcn,
                              cell->bs.rx_power, gas_id );
        }

        /* doing F&S 3 frame acquisition phase */
        if( cell->fcb_try_no != 0 )
        {
            /* If F&S acquisition had failed once, then do full F&S acquisition */
            cell->fcb_try_no = 0;
            cell->bs.bsic_known = FALSE;
            cell->reassert               = FALSE;
        }


        cell->fn_when_lost           = -32 * MAX_TCH_TONE_DETECT_ATTEMPTS * 26;
        if (cell->bs.bsic_known == FALSE)
        {
          cell->bs.tone_detected     = FALSE;
        }
        cell->in_fade                = FALSE;
        cell->cell_stale_sch         = FALSE;

        cell->idle_frames_to_next_sb = 2;

        /* (re)do all sys infos for ncells after next meas report
         * (if in autonomous mode).
         */
        if( !l1_sc_globals_ptr->autonomous_bcch )
        {
            if (cell->sb_count_to_bcch == BCCH_DO_NOW)
            {
                L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, TRUE, gas_id );
                MSG_GERAN_MED_1_G("Ncell %d SU cancelled", cell->bs.arfcn.num);
            }
            /* GW HACK */
            cell->sb_count_to_bcch    = BCCH_DONT_DO;
        }
        else
        {
            cell->sb_count_to_bcch    = BCCH_DO_AFTER_MEAS_RPT;
            cell->bcch_mask           = BCCH_MASK_3;
            cell->bcch_clear_mask     = BCCH_MASK_34;
        }

        /* Clear the last power monitor results during channel transition */
        if(L1_DEDICATED_MODE == l1_tsk_buffer[gas_id].l1_state)
        {
            cell->bs.rx_power_sum = 0;
            cell->bs.rx_power_count = 0;
            cell->bs.prev_rx_power_sum = 0;
            cell->bs.prev_rx_power_count = 0;
        }
            
    }

    /* Reset surround tick */
    l1_sc_globals_ptr->surround_tick  = 0;

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_sc_balist_reset_powers

DESCRIPTION
 Clears the power measurements for each cell in the BA list.  Also calls
 l1_sc_balist_reset_cell_counters() to clear the timing/BSIC information
 for the cells in the BA list.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_reset_powers( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    word        n;
    l1_sc_bs_T               *bs;
    cell_T     *cell;

    for( n = 0; n < l1_sc_main_data_ptr->ba.cell_count; ++n )
    {
        cell                   = l1_sc_main_data_ptr->ba.cells[ n ];

  /*check_ba_exception*/
        l1_sc_balist_check_cell(gas_id, cell);

        bs                     = &(cell->bs);
        bs->rx_power           = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;
        bs->prev_rx_power      = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;
        bs->meas_state         = L1SCMeasNone;
        bs->rx_power_sum       = 0;
        bs->prev_rx_power_sum       = 0;
        bs->rx_power_count     = 0;
        bs->prev_rx_power_count     = 0;

        /* re-initialize the cell's AGC structure */
        sys_algo_agc_init(&cell->bs.agc, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16);
        sys_algo_agc_disable_slew_rate_limiting(&cell->bs.agc);
#ifdef FEATURE_GSM_RX_DIVERSITY
        sys_algo_agc_init(&cell->bs.agc_divrx, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16);
        sys_algo_agc_disable_slew_rate_limiting(&cell->bs.agc_divrx);
#endif
        /* Clear the running average measurement buffer. */
        l1_running_avg_clear_meas( &(bs->ra) );
        l1_running_avg_set_avg_len( &(bs->ra), L1_SC_MAX_MEAS_BUF_LEN, gas_id );

        cell->rank             = L1_SC_NO_RANK;
        cell->prev_rank        = L1_SC_NO_RANK;
    }

    l1_sc_balist_reset_counters(gas_id);

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_sc_balist_get_num_cells

DESCRIPTION
  Queries length of BA list.  If a new BA list is pending then the length
  of the new BA list is returned, otherwise the length of the current
  BA list is returned.

DEPENDENCIES
  None

RETURN VALUE
  Number of cells in the BA list or the new BA list.

SIDE EFFECTS
  None

===========================================================================*/
uint8 l1_sc_balist_get_num_cells( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    if(l1_sc_main_data_ptr->ba.new_ba_list_pending)
    {
        return l1_sc_main_data_ptr->ba.new_cell_count;
    }
    else
    {
        return l1_sc_main_data_ptr->ba.cell_count;
    }
}

/*===========================================================================

FUNCTION  l1_sc_balist_transfer_power_info

DESCRIPTION
  For each cell in the BA list (or new BA list if one is pending) this
  functions fills the running average buffer with the latest rx_power
  value.  This is used to transfer power information from Dedicated/
  Transfer Modes to Idle Modes.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_transfer_power_info( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    uint32   i, num_cells;
    cell_T **cells;

    if(l1_sc_main_data_ptr->ba.new_ba_list_pending)
    {
        cells     = l1_sc_main_data_ptr->ba.new_cells;
        num_cells = l1_sc_main_data_ptr->ba.new_cell_count;
    }
    else
    {
        cells     = l1_sc_main_data_ptr->ba.cells;
        num_cells = l1_sc_main_data_ptr->ba.cell_count;
    }

    for(i=0; i<num_cells; i++)
    {
        /* Only fill the buffer if rx_power is above the threshold.
         * rx_power < threshold may indicate that this cell never
         * had enough measurements to fill the buffer.
         */
        if(cells[i]->bs.rx_power > SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16)
        {
            l1_running_avg_stuff_buf( &(cells[i]->bs.ra),
                                      cells[i]->bs.rx_power, 
                                      GL1_INVALID_POWER_LEVEL,
                                      PRX_MEAS,
                                      gas_id);

            cells[i]->bs.meas_state = L1SCMeasOld;
        }
        else
        {
            l1_running_avg_clear_meas( &(cells[i]->bs.ra) );

            cells[i]->bs.meas_state = L1SCMeasNone;
        }
    }
}

/*===========================================================================

FUNCTION  l1_sc_balist_set_serving_cell_valid

DESCRIPTION
  Set or clear the field in the BA list that indicated whether the info
  in the serving cell field is valid or not.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_set_serving_cell_valid( boolean serving_cell_valid, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    l1_sc_main_data_ptr->ba.serving_cell_valid = serving_cell_valid;

    if ( serving_cell_valid )
    {
      l1_sc_main_data_ptr->ba.pre_scell_arfcn = &(l1_sc_main_data_ptr->ba.serving_cell.bs.arfcn);
    }
    else
    {
      l1_sc_main_data_ptr->ba.pre_scell_arfcn = NULL;
    }

#ifdef DEBUG_SCE_MSG
    #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_sc_balist_update_serving_cell

DESCRIPTION
  Updates the frequency error for the old serving cell which is just added to BA list
  after cell re-selection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_balist_update_serving_cell( gas_id_t gas_id )
{
  l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
  ARFCN_T* preSrvcell = l1_sc_main_data_ptr->ba.pre_scell_arfcn;

  if ( preSrvcell != NULL )
  {
    cell_T* cell = l1_sc_balist_get_cell_by_arfcn( *preSrvcell, gas_id );

    if ( cell != NULL )
    {
      if( !cell->afc_info.valid && cell->bs.bsic_known )
      {
        cell->afc_info.valid = TRUE;
        cell->afc_info.freq_error = 
                gl1_hw_normxofreqtofreq( cell->bs.arfcn ,
                                     (cell->afc_info.afc_freq - gl1_hw_get_xo_acc_freq_err(gas_id)), gas_id );

        MSG_GERAN_LOW_2_G("freq_error %dHz updated for old scell %d", cell->afc_info.freq_error, cell->bs.arfcn.num);
      }

      if( cell->afc_info.valid )
      {
        cell->bs.fn_when_read  = gl1_get_FN(gas_id);
        cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
      }
    }
  }
}

/*===========================================================================

FUNCTION  l1_sc_balist_get_serving_cell_ptr

DESCRIPTION
  Returns a pointer to the serving cell field in the BA list so that the
  info in this field can be updated.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
cell_T *l1_sc_balist_get_serving_cell_ptr( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    return &(l1_sc_main_data_ptr->ba.serving_cell);
}

/*===========================================================================

FUNCTION  l1_sc_balist_add_serving_cell

DESCRIPTION
  If the serving cell info is valid it is used to create a new cell which is
  added to the BA list.

DEPENDENCIES
  Need to coordinate fields transferred by this function with fields
  populated by l1_sc_idle_add_serving_cell_to_balist().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_add_serving_cell( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    cell_T* cell;
    ARFCN_T arfcn;
    ARFCN_T serving_arfcn = gl1_get_serving_cell_arfcn(gas_id);
    cell_T* preSrvcell = &(l1_sc_main_data_ptr->ba.serving_cell);

    arfcn = preSrvcell->bs.arfcn;

    if( (l1_sc_main_data_ptr->ba.serving_cell_valid) &&
        !ARFCNS_EQUAL(arfcn, serving_arfcn) )
    {
        cell  = l1_sc_balist_get_cell_by_arfcn( arfcn, gas_id );

        if(cell == NULL)
        {
            cell = l1_sc_balist_new_cell( arfcn, gas_id );
            if(cell != NULL)
            {

     /*check_ba_exception*/
                l1_sc_balist_check_cell(gas_id, cell);
                l1_sc_main_data_ptr->ba.cells[l1_sc_main_data_ptr->ba.cell_count++] = cell;
            }
        }

        if(cell != NULL)
        {

            /*check_ba_exception*/
            l1_sc_balist_check_cell(gas_id, cell);

            cell->bs.rx_power   = preSrvcell->bs.rx_power;
#ifdef FEATURE_GSM_CELL_RESELECTION_OPTION20
            cell->bs.bsic_known = TRUE;
            cell->bs.bsic       = preSrvcell->bs.bsic;
            cell->bs.qbit_lag   = preSrvcell->bs.qbit_lag;
            cell->bs.fn_lag     = preSrvcell->bs.fn_lag;
            /* Reconstruct SCH data (BSIC & RFN).
             * SCH data is sent to RR in MPH_SURROUND_MEAS_IND.
             * RR does not use RFN, so only fill in BSIC.
             * BSIC is b2-b7 in byte 0 of SCH data.
             * Remaining bytes are for RFN.
             * SCH data will be refreshed on SCH reconf.
             */
            cell->bs.sb_data[0] = cell->bs.bsic << 2; /* BSIC */
            cell->bs.sb_data[1] = 0;                  /* RFN  */
            cell->bs.sb_data[2] = 0;                  /* RFN  */
            cell->bs.sb_data[3] = 0;                  /* RFN  */
            cell->afc_info.afc_freq = gl1_hw_get_xo_acc_freq_err(gas_id);
            MSG_GERAN_MED_1_G("afc_freq %dXO updated for serving cell %d", cell->afc_info.afc_freq);

#endif /* FEATURE_GSM_CELL_RESELECTION_OPTION20 */

            sys_algo_agc_init(&cell->bs.agc, cell->bs.rx_power);
            sys_algo_agc_disable_slew_rate_limiting(&cell->bs.agc);
            MSG_GERAN_MED_3_G("SCE: Add previous serving cell(%d) to BA list (qbit_lag %d, fn_lag %d)", arfcn.num,preSrvcell->bs.qbit_lag,preSrvcell->bs.fn_lag);
        }
        else
        {
            MSG_GERAN_ERROR_1_G("SCE: Couldn't add serving cell(%d) to BA list.", arfcn.num);
        }

#ifdef DEBUG_SCE_MSG
        #error code not present
#endif
    }

    l1_sc_main_data_ptr->ba.serving_cell_valid = FALSE;
}


/*===========================================================================

FUNCTION  l1_sc_balist_add_cell

DESCRIPTION
  This function adds the specified cell to the BA list

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_add_cell( l1_sc_bs_T *new_bs, gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    cell_T*  cell_ptr;

    /* Check if there is a pending BA list as a result of an RR list update
     * message.
     */
    if( FALSE != l1_sc_main_data_ptr->ba.new_ba_list_pending )
    {
      cell_ptr = l1_sc_balist_is_cell_in_new_ba_list( new_bs->arfcn, gas_id );
    }
    else
    {
      /* There is no pending BA list so use the current list
       * This most likely has just been swapped by the monscan update
       * routine.
       */
      cell_ptr = l1_sc_balist_get_cell_by_arfcn( new_bs->arfcn, gas_id );
    }

    /* If the cell ptr is valid then populate the cell info in */
    if( (NULL != cell_ptr) && !cell_ptr->bs.bsic_known )
    {

        /*check_ba_exception*/
        l1_sc_balist_check_cell(gas_id, cell_ptr);

        /* Populate new cell with passed in info */
        cell_ptr->bs.bsic       = new_bs->bsic;
        cell_ptr->bs.bsic_known = new_bs->bsic_known;
        cell_ptr->bs.qbit_lag   = new_bs->qbit_lag;
        cell_ptr->bs.fn_lag     = new_bs->fn_lag;
        cell_ptr->bs.rx_power   = new_bs->rx_power;
#ifdef FEATURE_GERAN_REDUCED_F3
        /* Print out an F3 message */
        MSG_GERAN_HIGH_3_G( "Cell %d qbit_lag=%d fn_lag=%d in new BA list",
                  cell_ptr->bs.arfcn.num,
                  cell_ptr->bs.qbit_lag,
                  cell_ptr->bs.fn_lag );
#endif
    }
    else
    {
        /* Warn the user that this cell is not added to the list afterall */
        MSG_GERAN_HIGH_3_G( "Cell %d not in new BA list qbit_lag=%d fn_lag=%d",
                  new_bs->arfcn.num,
                  new_bs->qbit_lag,
                  new_bs->fn_lag );
    }
}


/*===========================================================================

FUNCTION  l1_sc_balist_get_old_id, l1_sc_balist_get_new_id

DESCRIPTION
  Return the old/new ID stored in the BA list.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint8 l1_sc_balist_get_old_id( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    return l1_sc_main_data_ptr->ba.old_id;
}

uint8 l1_sc_balist_get_new_id( gas_id_t gas_id )
{
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

    return l1_sc_main_data_ptr->ba.new_id;
}

/*===========================================================================

FUNCTION  l1_sc_highest_lowest_timslot

DESCRIPTION
This function returns the highest and lowest downlink timeslots
in current use. This is for CS and DTM modes only

DEPENDENCIES
This function can only be used for TCH and DTM useage. It should
not be called for transfer state (GPRS) as lowest and highest
timeslot allocation is not implemented.

RETURN VALUE
Update pointer accordingly

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_highest_lowest_timslot(gl1_defs_tn_struct* lo, gl1_defs_tn_struct* hi, gas_id_t gas_id )
{
#ifdef FEATURE_GSM_DTM
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
    gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

    /* For DTM and Dedicated we slam to place the CS TN on relative TN 2 */
    if(l1_tskisr_blk->l1_state != L1_DTM_MODE)
    {
      /* TCH CS call may be slammed to either lowest CS Tn or to place
       * CS TN on TN 2
       */

      if (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
      {
        hi->tn = GL1_DEFS_TN_2;
        lo->tn = GL1_DEFS_TN_2;

      }else
      {
        hi->tn = GL1_DEFS_TN_0;
        lo->tn = GL1_DEFS_TN_0;
      }

    }
    else
    {
      /* returns the highest receive timeslot used includeing CS */
      /* this takes into account CS and Packet timeslots allocated */
      hi->tn = (gl1_defs_tn_type)gprs_serv_cell_meas_ptr->highest_tn;
      lo->tn = (gl1_defs_tn_type)gprs_serv_cell_meas_ptr->lowest_tn.tn;
    }
#else
    /*
      The 0 is the sliding window to accomodate the rx slots.
      Set to 0 for GSM
    */
    lo->tn = GL1_DEFS_TN_0;
    hi->tn = GL1_DEFS_TN_0;
#endif
}
/*
 *     Function name :  l1_sci_check_ok_to_sleep
 *     ----------------------------------------------
 *
 *     Description :
 *     ---------------
 *     Check with surround cell processes before sleeping
 *
 */
boolean l1_sci_check_ok_to_sleep( uint32 gap_to_next_activity, gas_id_t gas_id )
{
    boolean ret_code = TRUE;

	if(l1_sci_is_wcdma_ready(gap_to_next_activity, gas_id)
            #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
            && !l1_sc_g2x_timeline_opt_enable[gas_id]
            #endif
            )
    {
        ret_code = FALSE;
    }

    return ret_code;
}


/**********************************************************************
 *
 *  Module:
 *  -------
 *  Move this function to l1_utils.c and rename.
 *
 **********************************************************************/

/*===========================================================================

FUNCTION   l1_scdrv_call_in_task

DESCRIPTION
   Sends a message to the layer1 task containing a pointer
   to a function to be called.  Allows processing thread to
   move from ISR contect to task context.

   IN - code - The function to be called in the layer1 task
   IN - data - The data to be passed to 'code'

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_scdrv_call_in_task( void ( *code )( void*, gas_id_t gas_id ), void* data, gas_id_t gas_id )
{
    l1_sc_call_in_task_T msg;

    msg.header.message_set = MS_L1_L1;
    msg.header.message_id  = L1_SC_CALL_IN_TASK_CNF;
    msg.code               = code;
    msg.data               = data;
    msg.gas_id             = gas_id;

    PUT_IMH_LEN( sizeof( msg ) - sizeof( msg.header ), &msg ); /*lint !e740 unusual pointer cast */

    /* When we are in idle mode we need to tell the DRX manager
     * when we send a callback to the task so it knows that the
     * task is active.  This call is ignore when we are not in
     * Idle Mode because the DRX manager is off.
     */
    gl1_drx_task_active(msg.gas_id);

    ( void )L1_isr_send_message( l1_queues[msg.gas_id], &msg, TRUE, msg.gas_id );
}


/**********************************************************************
 *
 *  Module:  Functions to Eliminate
 *  -------
 *  These functions should no longer be used
 *
 **********************************************************************/

dword l1_scdrv_get_fn( gas_id_t gas_id )
{
    dword fn;
    do
    {
        fn = gl1_get_FN( gas_id );
    } while( fn != gl1_get_FN( gas_id ) );
    fn = ADD_FN( fn, 1 );
    return fn;
}

dword l1_sc_get_fn( gas_id_t gas_id )
{
    dword fn;

    do
    {
        fn = l1_scdrv_get_fn(gas_id);
    } while( fn != l1_scdrv_get_fn(gas_id) );
    return fn;
}

l1_sc_mode_T l1_sc_get_curr_mode(gas_id_t gas_id)
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  return l1_sc_globals_ptr->mode;
}

#ifdef DEBUG_BALIST_CHECK_FOR_DUPS
#error code not present
#endif /* DEBUG_BALIST_CHECK_FOR_DUPS */

void l1_running_avg_sanity_check_meas_buf( running_avg_struct *ra_ptr, gas_id_t gas_id )
{
   l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

   uint16 index;

   /* sanity check meas_buf is pointing within the ra_bufs structure, or
      to the beginning of any other valid running average structure */
   if( ((ra_ptr->meas_buf < l1_sc_main_data_ptr->ra_bufs[0]) ||
        (ra_ptr->meas_buf > l1_sc_main_data_ptr->ra_bufs[2*BA_CELL_POOL_LEN-1]))
       &&(ra_ptr->meas_buf != (l1_get_idle_serv_meas_buf(PRX_MEAS_BUFFER, gas_id)))
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
       &&(ra_ptr->meas_buf != (l1_get_idle_serv_meas_buf(DIVRX_MEAS_BUFFER, gas_id)))
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
       &&(ra_ptr->meas_buf != (gpl1_gprs_get_serv_cell_meas_buf(PRX_MEAS_BUFFER, gas_id)))
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
       &&(ra_ptr->meas_buf != (gpl1_gprs_get_serv_cell_meas_buf(DIVRX_MEAS_BUFFER, gas_id)))
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
       )
   {
      /* state that meas_buf ptr is invalid, store away last FN when issues occurred and count of total occurrences */
      MSG_GERAN_ERROR_2_G("L1 running avg meas_buf ptr invalid! ra_ptr=0x%x, meas_buf=0x%x",&(ra_ptr->meas_buf),ra_ptr->meas_buf);
      l1_sc_main_data_ptr->l1_running_avg_meas_buf_err.fn = GSTMR_GET_FN_GERAN( gas_id);
      l1_sc_main_data_ptr->l1_running_avg_meas_buf_err.cnt++;

      /* reinitialise the ba.cell_pool meas_buf pointers to their 'fixed' values (pointers should be fixed
         values after original initialisation after running l1_running_avg_init()) */
      for(index=0; index<2*BA_CELL_POOL_LEN; index++)
      {
      l1_running_avg_init( &l1_sc_main_data_ptr->ba.cell_pool[index].bs.ra,
                             l1_sc_main_data_ptr->ra_bufs[index],
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
                             NULL,
#endif /*FEATURE_GSM_RX_DIVERSITY || FEATURE_GSM_RX_DIVERSITY_DATA*/
                             L1_SC_MAX_MEAS_BUF_LEN,
                             gas_id);
      }
      /* indicate that ba.cell_pool running average ptrs have been reinitialised */
      MSG_GERAN_HIGH_2_G("L1 running avg meas_buf ptrs reset: ra_ptr=0x%x, meas_buf=0x%x",&(ra_ptr->meas_buf),ra_ptr->meas_buf);
   }
}


#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST

/*===========================================================================

FUNCTION  l1_sc_update_sorted_ba_list

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_update_sorted_ba_list( boolean include_serving, gas_id_t gas_id )
{
   if ( l1_sc_monscan_is_not_active(gas_id) )
   {
      l1_sc_balist_sort_restrict_ba_len(include_serving, gas_id);

      /* Log up to BA_MAX_LEN cells */
      l1_log_ncell_ba_list(gas_id);
   }
   else
   {
       /* update blacklist on l1_sc_monscan_done_idle() */
       l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

       l1_sc_globals_ptr->blacklist_update_pending = TRUE;

       MSG_GERAN_MED_0_G("SCE: l1_sc_update_sorted_ba_list(): update pending as monscan is active");
   }
}

/*===========================================================================

FUNCTION  l1_sc_reduced_ncell_list_status

DESCRIPTION
  Returns L1's status of top2 mode feature to RR

DEPENDENCIES
  None

RETURN VALUE
  reduced_ncell_list_stored_in_l1, reduced_ncell_list_mode_active, reduced_ncell_list_allowed

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_reduced_ncell_list_status(
   boolean *reduced_ncell_list_stored_in_l1, /* L1's snapshot of reduced_ncell_list which was last sent by RR - for L1 <-> RR sync */
   boolean *reduced_ncell_list_mode_active,  /* T => L1 is using top2mode */
   boolean *reduced_ncell_list_allowed,      /* T in Idle/Gprs mode, F otherwise */
   gas_id_t gas_id )
{
   l1_sc_globals_T *l1_sc_globals_ptr        = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint8            reduced_ncell_list_bmask = l1_sc_globals_ptr->reduced_ncell_list_bmask;  

   /* top-2 mode allowed by RR */
   *reduced_ncell_list_stored_in_l1          = ((reduced_ncell_list_bmask & SCE_REDUCED_NCELL_LIST_RR_EN) != 0);

   /* L1 is using top-2 mode now */
   *reduced_ncell_list_mode_active          = (reduced_ncell_list_bmask == SCE_REDUCED_NCELL_LIST_ENABLED);

   /* T in Idle/Gprs mode, F otherwise */
   if(
      /* L1 is allowed to use top-2 mode when gcf/efs is not set */
      ((reduced_ncell_list_bmask & SCE_REDUCED_NCELL_LIST_STATIC_EN) == SCE_REDUCED_NCELL_LIST_STATIC_EN)
      && /* and, when SCE is in gsm idle/gprs idle/gprs transfer mode of operation */
      (
            (l1_sc_globals_ptr->mode == L1SCModeIdle)
         || (l1_sc_globals_ptr->mode == L1SCModeGprsIdle)
         || (l1_sc_globals_ptr->mode == L1SCModeGprsTrans) ) )
   {
      *reduced_ncell_list_allowed = TRUE;
   }
   else
   {
      *reduced_ncell_list_allowed = FALSE;
   }

   MSG_GERAN_MED_3_G("SCE: L1_Sce status: L1view: %d, L1Mode: %d, Allowed: %d",
      *reduced_ncell_list_stored_in_l1,
      *reduced_ncell_list_mode_active, 
      *reduced_ncell_list_allowed);

}


/*===========================================================================

FUNCTION  l1_sc_config_reduced_ncell_list_rr

DESCRIPTION
RR gives indication on whether L1 can use top-2 mode for SCE.
Called from task context.

DEPENDENCIES
 None

RETURN VALUE
 None
 
SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_config_reduced_ncell_list_rr( boolean reduced_ncell_list_en, gas_id_t gas_id )
{
   l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);

   MSG_GERAN_MED_1_G("SCE: Top-n Mode: MPH_REDUCED_NCELL_MEAS_IND(): Enable=%d", reduced_ncell_list_en);

   if(reduced_ncell_list_en == TRUE)
   {
      l1_sc_set_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_RR_EN, gas_id);
   }
   else
   {
      l1_sc_clear_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_RR_EN, gas_id);
   }

   /* Top-2/top-6 mode has probably been changed by RR.
    * But, we update sorted BA list "only if" we are in idle mode.
    * Note that the serving cell is not included in idle mode.
    */
   if( (l1_sc_globals_ptr->mode == L1SCModeIdle)
       || (l1_sc_globals_ptr->mode == L1SCModeGprsIdle) )
   {
      l1_sc_update_sorted_ba_list(FALSE, gas_id);
   }

}


/*===========================================================================

FUNCTION  l1_sc_set_bmask_reduced_ncell_list

DESCRIPTION
Set flag to enable top-2 SCE optimisation.
Top-2 feature is enabled when all the required flags are set

DEPENDENCIES
 None

RETURN VALUE
 None
 
SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_set_bmask_reduced_ncell_list( uint8 reduced_ncell_bmask, gas_id_t gas_id )
{
   l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);

   GL1_ISR_LOCK(gas_id);

   L1_SC_DBG_REDUCED_NCELL_LIST_SET_BMASK(l1_sc_globals_ptr->reduced_ncell_list_bmask, reduced_ncell_bmask, gas_id);

   l1_sc_globals_ptr->reduced_ncell_list_bmask |= reduced_ncell_bmask;

   GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION  l1_sc_clear_bmask_reduced_ncell_list

DESCRIPTION
Clear flag to disable top-2 SCE optimisation.
Top-2 optimisation is disabled if any of the flags are cleared.

DEPENDENCIES
 None

RETURN VALUE
 None
 
SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_clear_bmask_reduced_ncell_list( uint8 reduced_ncell_bmask, gas_id_t gas_id )
{
   l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);

   GL1_ISR_LOCK(gas_id);

   L1_SC_DBG_REDUCED_NCELL_LIST_CLEAR_BMASK(l1_sc_globals_ptr->reduced_ncell_list_bmask, reduced_ncell_bmask, gas_id);

   l1_sc_globals_ptr->reduced_ncell_list_bmask &= ~reduced_ncell_bmask;

   GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION  l1_sci_set_bmask_reduced_ncell_list

DESCRIPTION
Set flag to enable top-2 SCE optimisation.
Top-2 feature is enabled when all the required flags are set

DEPENDENCIES
 None

RETURN VALUE
 None
 
SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_set_bmask_reduced_ncell_list( uint8 reduced_ncell_bmask, gas_id_t gas_id )
{
   l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);

   L1_SC_DBG_REDUCED_NCELL_LIST_SET_BMASK(l1_sc_globals_ptr->reduced_ncell_list_bmask, reduced_ncell_bmask, gas_id);

   l1_sc_globals_ptr->reduced_ncell_list_bmask |= reduced_ncell_bmask;
}

/*===========================================================================

FUNCTION  l1_sci_clear_bmask_reduced_ncell_list

DESCRIPTION
Clear flag to disable top-2 SCE optimisation.
Top-2 optimisation is disabled if any of the flags are cleared.

DEPENDENCIES
 None

RETURN VALUE
 None
 
SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_clear_bmask_reduced_ncell_list( uint8 reduced_ncell_bmask, gas_id_t gas_id )
{
   l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);

   L1_SC_DBG_REDUCED_NCELL_LIST_CLEAR_BMASK(l1_sc_globals_ptr->reduced_ncell_list_bmask, reduced_ncell_bmask, gas_id);

   l1_sc_globals_ptr->reduced_ncell_list_bmask &= ~reduced_ncell_bmask;
}
#endif /* #ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST */


#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST_DEBUG
/*===========================================================================

FUNCTION  l1_sc_dbg_set_bmask_reduced_ncell_list

DESCRIPTION
Prints a message on which activity is enabling top-n mode, and if top-n mode is getting enabled now.
Called from any context.

DEPENDENCIES
 None

RETURN VALUE
 None
 
SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_dbg_set_bmask_reduced_ncell_list( uint8 bmask_bef, uint8 reduced_ncell_bmask, gas_id_t gas_id )
{
   uint8 bmask_aft;

   bmask_aft = bmask_bef | reduced_ncell_bmask;
   MSG_GERAN_MED_3_G("SCE: Top-n Mode: SET BMASK: Mask: 0x%02x, Bef: 0x%02x, Aft: 0x%02x", reduced_ncell_bmask, bmask_bef, bmask_aft);
   if( (bmask_bef != bmask_aft) && (bmask_aft == SCE_REDUCED_NCELL_LIST_ENABLED) )
   {
      MSG_GERAN_MED_0_G("SCE: Top-n Mode ENABLED");
   }
}

/*===========================================================================

FUNCTION  l1_sc_dbg_clear_bmask_reduced_ncell_list

DESCRIPTION
Prints a message on which activity is disabling top-n mode, and if top-n mode is getting disabled now.
Called from any context.

DEPENDENCIES
 None

RETURN VALUE
 None
 
SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_dbg_clear_bmask_reduced_ncell_list( uint8 bmask_bef, uint8 reduced_ncell_bmask, gas_id_t gas_id )
{
   uint8 bmask_aft;

   bmask_aft = bmask_bef & (~reduced_ncell_bmask);
   MSG_GERAN_MED_3_G("SCE: Top-n Mode: CLR BMASK: Mask: 0x%02x, Bef: 0x%02x, Aft: 0x%02x", reduced_ncell_bmask, bmask_bef, bmask_aft);
   if( (bmask_bef != bmask_aft) && (bmask_bef == SCE_REDUCED_NCELL_LIST_ENABLED) )
   {
      MSG_GERAN_MED_0_G("SCE: Top-n Mode DISABLED");
   }
}

/*===========================================================================

FUNCTION  l1_sc_balist_print_running_avg

DESCRIPTION
Prints scell avg and instant power values.
Also prints ncell avg and instant power values, alongwith misc debug info.

DEPENDENCIES
 None

RETURN VALUE
 None
 
SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_balist_print_running_avg( gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
    l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    idle_data_T        *idle_data_ptr;
    running_avg_struct *ra_ptr;
    uint32    i;
    uint32    num_sorted_cells;

    idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;    
    ra_ptr        = &(idle_data_ptr->campedon_cell_pwr_meas_avg);

    num_sorted_cells = l1_sc_main_data_ptr->ba.cell_count;

    if( num_sorted_cells > 10)
      num_sorted_cells = 10;

    MSG_GERAN_MED_2_G("SCE: PWR: STRT: %d, %d", num_sorted_cells, l1_sc_globals_ptr->sorted_cell_count);
    MSG_GERAN_MED_3_G("SCE: PWR: SCEL: %d, %d, %d, 1, 0", 
      gl1_get_serving_cell_arfcn(gas_id).num,
      l1_running_avg_get_avg( ra_ptr) >> 4,
      l1_serving_cell_meas_ptr->rx_power_instant >> 4) ;
    for(i=0; i<num_sorted_cells; i++)
    {
       MSG_GERAN_MED_5_G("SCE: PWR: NCEL: %d, %d, %d, %d, %d",
         l1_sc_globals_ptr->sorted_cells[i]->bs.arfcn.num,
         l1_sc_globals_ptr->sorted_cells[i]->bs.rx_power >> 4,
         l1_sc_globals_ptr->sorted_cells[i]->bs.rx_power_instant >> 4,
         l1_sc_globals_ptr->sorted_cells[i]->bs.bsic_known,
         l1_sc_globals_ptr->sorted_cells[i]->in_blacklist);
    }
    MSG_GERAN_MED_0_G("SCE: PWR: STOP");

}

#endif /* #ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST_DEBUG */
/*===========================================================================

FUNCTION  qsort_band

DESCRIPTION
Sorts the incoming BA list by band. This APi should ony be called in DR mode. THis is used to group monitors
by band to reduce the intances of diversity tune-away in the X data + G idle configuration.  

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/

static int compare_band( const void *arg1, const void *arg2 )
{
  return ( ((ARFCN_T*)arg1)->band - ((ARFCN_T*)arg2)->band );
} 

static void qsort_band(
  ARFCN_T *loc_band,
  uint8 num
)
{
  qsort((void *)loc_band, num, sizeof(ARFCN_T), compare_band);

}

boolean l1_sc_is_sce_running(gas_id_t gas_id)
{
   l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);

   return l1_sc_main_data_ptr->sce_running;
}

#ifdef FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17
void l1_sc_deblacklist_ba_list (gas_id_t gas_id)
{

   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_main_data_t *l1_sc_main_data_ptr = gl1_ms_switch_l1_sc_main_data(gas_id);
   uint32 n = 0;

   for( n=0; n < l1_sc_main_data_ptr->ba.cell_count; n++ )
   {
      if(l1_sc_main_data_ptr->ba.cells[n]->in_blacklist)
      {
         MSG_GERAN_MED_4_G("SCE: De-blacklisting(3): FN: %d, s-tick: %d, BL-end: %d, arfcn: %d",
           GSTMR_GET_FN_GERAN(gas_id),
           l1_sc_globals_ptr->surround_tick,
           l1_sc_main_data_ptr->ba.cells[n]->next_tick,
           l1_sc_main_data_ptr->ba.cells[n]->bs.arfcn.num);

          /*Deblacklist cell, ensure the FB/SB attempt is resumed within 1sec, and power measurement immediately*/
          l1_sc_main_data_ptr->ba.cells[n]->in_blacklist = FALSE;
          l1_sc_main_data_ptr->ba.cells[n]->next_tick = l1_sc_globals_ptr->surround_tick + TICKS_PER_FCB_DEBLACKLIST_CELLS;
          l1_sc_main_data_ptr->ba.cells[n]->pwr_msr_disabled_till_tick = l1_sc_globals_ptr->surround_tick;
       }
   }
}
#endif

#ifdef FEATURE_GSM_SYNQ_SUPPORT
/*===========================================================================

FUNCTION  l1_sc_run_synq_algo

DESCRIPTION
  This function runs synq algorithm, if 3 consecutive gdet score is bad mark cell for SCH reconfirm.
  If cell is already marked but received good gdet score before reconfirmation, decrement the count and if bad gdet count goes below 2 clear the params

DEPENDENCIES
  GDET FR should be enabled and gdet score needs to be passed

INPUT
  NCELL ARFCN, gdet score, gas_id

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_run_synq_algo( ARFCN_T arfcn, uint16 gdet_dec_stat, gas_id_t gas_id )
{
    cell_T* ncell_balist_ptr = NULL;
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    if(L1_DEDICATED_MODE != l1_tskisr_blk->l1_state)
    {
      return;
    }

    ncell_balist_ptr = l1_sc_balist_get_cell_by_arfcn( arfcn, gas_id );

    if(ncell_balist_ptr != NULL)
    {
      if( TRUE == ncell_balist_ptr->bs.bsic_known )
      {

       uint8 bad_gdet_threshold = 0;
       uint8 gdet_recovering_threshold = 0; 

        if(gdet_dec_stat > gl1_hw_get_synq_fr_gdet_threshold(&bad_gdet_threshold, &gdet_recovering_threshold))
        {
          if(ncell_balist_ptr->bad_gdet_pwr_mon_cntr < bad_gdet_threshold)
          {
            ncell_balist_ptr->bad_gdet_pwr_mon_cntr++;

            if( bad_gdet_threshold == ncell_balist_ptr->bad_gdet_pwr_mon_cntr)
            {
              ncell_balist_ptr->gdet_based_sch_reconfirm = TRUE;
            }
          }
        }
        else
        {
          if( TRUE == ncell_balist_ptr->gdet_based_sch_reconfirm)
          {
            ncell_balist_ptr->bad_gdet_pwr_mon_cntr--;

            if(ncell_balist_ptr->bad_gdet_pwr_mon_cntr < gdet_recovering_threshold)
            {
              ncell_balist_ptr->gdet_based_sch_reconfirm = FALSE;
              ncell_balist_ptr->bad_gdet_pwr_mon_cntr = 0;
            }
          }
          else
          {
            ncell_balist_ptr->bad_gdet_pwr_mon_cntr = 0;
          }
        }
        
        MSG_GERAN_MED_3_G("synq_algo: cell %d, gdet score %d cnt %d ",arfcn.num,  gdet_dec_stat,  ncell_balist_ptr->bad_gdet_pwr_mon_cntr);
      }
      else
      {
          ncell_balist_ptr->gdet_based_sch_reconfirm = FALSE;
          ncell_balist_ptr->bad_gdet_pwr_mon_cntr = 0;
      }
    }

}
#endif