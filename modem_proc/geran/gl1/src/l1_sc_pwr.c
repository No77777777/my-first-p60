/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            SCE: MONITOR and POWERSCAN

GENERAL DESCRIPTION
   This module contains driver functions for SCE power measurements.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when       who       what, where, why
--------   --------  ----------------------------------------------------------
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
04/02/20   nv		CR2615257 Reverting the hack made to handle L1_TIME_LINE_DELAY scenario in HA as part of FR:51554
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
26/05/17   mk        CR2020690 Wait for 2 complete SACCH periods before scheduling NCell FCCH and SCH after channel transition in dedicated mode
10/05/17   sk        CR2024440 Add an explicit intlock for DivRx meas buffer update
22/05/17   hd        CR2045286 Ignore the NCELL activity on a cell whose power level is less than or equal to -110 dBm
01/08/17   sid       CR2083607 Remove dual logging of 0x5A71 log packet in every alternate instance
12/12/17   km        CR2158090 Compilation warnings fix
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
21/07/16   mn        CR1044590: Fix KW issue and exception caused from 1038564.
11/07/16   mn        CR1038564:Abandon power measurment if current Sub is in transfer and other G sub is not in idle.
12/05/16    rv       CR998395: Excessive F3 logging in  GERAN L1 
23/02/16   sk        CR973190 Excessive F3 logging reduction
21/12/15   sk        CR938638 Reducing no of F3s
06/11/15   ab        CR927639 : CXM In QDTA QDA use desense_band_id as freq id to avoid X blanking.
06/11/15   ab        CR935174 : Perform G IDLE Freq after all RX done to avoid X desense the G PCH RX.
14/10/15   cah       CR921655 DR-DSDS: grm band registrations not cleared in PTM
06/08/15   sk        CR849293 Add an explicit ISR lock for l1_sci_monscan_power_cb_idle
04/09/15   akh       CR890101 Weak ncells adjacent to blacklisted cell getting removed incorrectly
26/08/15   jk        CR896003:Fix KW error in TH2.1
19/08/15   mc        CR874662 : CXM Freq ID support for X2G startup and X2G preload
14/08/15   cah       CR888912: Stale persistent denial PM band registartion in GRM. Causes flooring on X tech
12/08/15   cws       CR864883 FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
16/07/15   akh/br    CR871074 Change scell monitoring from frame 1 to frame 5 for ASDIV in G transfer mode
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
01/06/15   cgc       CR738835 Fix compile error with  FEATURE_GSM_DED_SCELL_MONITOR disabled.
28/05/15   cws       CR843724 Fix compile error with ASDIV feature enable
20/05/15   akh       CR841030 Debug code to check that pwr mon results come back within 4 frames for idle sub
12/05/15   akh       CR813711 Improvements to power monitor debug structure
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
16/04/15   ak        CR820145 Problems with empty BA list during ded mode and with HMSC tests
03/24/15   ak        CR810490 Incorrect Skip Monitor scheduling for MFRM=2 when num_afrcn to measure are less than 2
16/04/15   smd       CR822202 Dual space data for IRAT and QTA
02/04/15   ab        CR815867 : Reset power meas handlers after callback is issued.
                     ****it supercedes CR 801858****
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
13/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
06/03/15   ak        CR803976 Re-implement CR792349 (abort power measure instead of stop from l1_sc_monscan_abort_gprs())
19/02/15   nm        CR796796 abort pwr meas if idle sub mon gbta status is scheduled on aborting trans mons
18/02/15   am        CR790448 Ensure transfer sub points to idle gas id on mon result
15/02/15   sp        CR729492 Remove unimportant logging
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs.
12/01/15   sjv       CR756154 Enhance Band Registration for Activities   
23/12/14   ggu       CR772195: No need to continue check gbta_allowed status when there is no any G sub is under TRANS state
19/12/14   cws       CR772318 ASDIV: Do not count in GBTA for ant meas 
12/12/14   zc        CR769907: Traffic mode asdiv test mode crash
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
20/11/14   cgc       CR766576 FR23109 Adaptive power monitors x3 in TCH with FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
10/12/14   ggu       CR763966 Only suspend the TRM release when make sure the IRAT activity will happen at next frame
03/12/14   npt       CR759467 Fix compiler warnings
04/12/14   cah       CR760172  Update denied band handling for persistent denial algorithm.
21/11/14   ws        CR760942  potential mod zero operation which can lead to divide by zero
19/11/14   ak        CR751852: Realign power monitors req_pos/cnf_pos when aborted
14/11/14   sp        CR725953: Reset req_pos back when Idle Power monitors were aborted by Packet transfer sub.
11/11/14   npt       CR751383 Avoid mutex dead lock. When the Idle ISR runs we shouldn’t post the W rcv to task instead do it inline in the ISR itself  
10/11/14   zc        CR748647 ASDIV Type1 Switching Test Mode API
04/11/14   js        CR749833 Restricting power monitors to be schedule in first frame i.e., first frame in full QBTA
29/08/14   cah       CR710699: DR-DSDS Do not update RSSI running avg for skipped ARFCN's
24/09/14   hd        CR725334: Added the function l1_sci_monscan_meas_in_progress 
08/10/14   pa        CR732823: Hybrid Tuneaway Fallback: Avg RSSI is an avergae of last 3 PCH blocks.
23/09/14   ab        CR727993 : GBTA - update Power mon state when callback are installed.
30/05/14   zf        CR662056: CMCC power test opt
17/09/14   jj        CR724140 dont add power meas handler when no measurement configured
17/09/14   sjv       CR724558 For PM, TRM band-reg and release at frame level granularity for perf. optimization
12/09/14   fan       CR636420: Run Sleep commit after GSTMR ISR at wakeup
04/09/14   am        CR682760 Addressing KW GL1 P1 errors
03/09/14   ssh       CR712967: Do not suspend TRM release for 2 frames if in low monitor mode
02/09/14   cah       CR717580 trm_client.cpp:5900   Assertion TRM_IS_PRIMARY_RF_DEVICE(activity_ptr->primary_device) failed
06/08/14   jj        CR 705509 reducing scell meaurement when BA list is empty in dedicated mode
29/08/14   cah      CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
19/08/14   dv        CR711414: Reset monscan_in_progress flag when aborting monitors
16/08/14   cah       CR707445: overlap between PM periods in PTM causing crash TRM crash due to incorrect trans id. 
05/08/14   zf        CR704506: Tick next_cell_bcch after PCH decode
02/05/14   nk        CR657820 Check for null bs pointer in mon results when we see arfcn mismatch and adjust pointers
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
31/07/14   aga       CR701248: Stop scheduled monitors when imminent TBF suspension  
22/07/14   cah       CR697586  trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
17/17/14   sjv       CR694717 : clearing call backs after aborting when no measurements are in progress
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
07/05/14   jj        CR 660790  fix merge issues on 424199
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
25/04/14   ss        CR655735 Reset req_pos_coex_unprotected and  cnf_pos_coex_unprotected 
                     while starting monscan after receiving new monscan cells 
23/05/14   cah       CR651795: FEATURE_GERAN_DRDSDS: Scelll measurement changes  
20/05/14   ip/cs     CR667871 Monitor Scan done TRM release for Multi SIM only
23/04/14   zf        CR653611: Reset reduced power monitor duty cycle count when missing a page
22/04/14   zf        CR643773: Alternate Power Measurements in Single-SIM
29/04/14   cs        CR655732 Do not suspend TRM release for pending SC activity in QTA
25/04/14   nm        CR652385 Clear idle_monitors_in_progress, monscan_in_progress, meas_in_progress in power cb idle
                     even if monscan.active is not true
18/04/14   nm        CR612938 dont skip pwr mons (cr601230) if it transfer sub has scheduled mons for idle subs
04/04/14   jj        CR 643780 schedule scell monitors for asdiv when ncell list is empty
08/04/14   gk        CR641603 Ignore yielded power monitor results
08/04/14   cs        CR638933 Allow equal priority event to be booked when suspend is set
08/04/14   cs        CR624044 If pending task SC activity in idle then delay TRM release
01/04/14   ip        CR601230: Skipping alternative power measurement on multi-SIM mode with MFRM 2
27/02/14   ab        CR.622172 : DSDS GBTA Reset Idle Power Moniter handler after every attempt
28/01/14   ka        CR605088 Move counter back to original if unable to schedule
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
11/10/13   pjr       CR.555300 : Changes to correctly register freqid during monitors  and acq
11/11/13   ws        CR 573757 Featurise GSMTR-1 registers and Voice services driver
08/11/13   cja       CR555989 Do not add extra power monitors until first complete TCH multiframe.
08/11/13   cja       CR555376 Check for null bs when collecting mon results.
05/11/13   cja       CR523987 Add FEATURE_GSM_DED_SCELL_MONITOR for extra SCell monitor in voice call
26/11/12   jj        CR 424199 Adding Scell measurement for tx-diversity when BA list is empty
02/11/12   jj        CR 402579 Support of Tx Diversity in GERAN.
10/10/13   js        CR481292 ps abort type 4 not finished even after getting the trm
14/09/13   ap/aga    CR545423 - G2W Tune Away Feature
15/08/13   sk        CR524039 GBTA bringup changes
11/07/13   nm        CR510055 - trm_suspension when pwr meas aborted in Retrieving state
25/06/13   cs        TSTS Updates
26/03/13   ws        CR466973 - DSDS BU, Added missing gas_id's from rx_in_progress
                     and mons_in_progress
6/7/12     ws        CR 375550 - Allow power measurements NCELL and SCELL to be sceduled
                     test modes.
13/04/12   ws        CR332457 - updated scheduling to allow SCELL BCCH on FN 0,5,9 FNmod13.
                     8 NCELL and 3 SCELL measurements scheduled every 13 frames
12/04/12   ws        CR332457 - Reintroduce change to schedule SCELL BCCH on frames 0,4,8 mod 12
24/01/11   cja       CR270529 Add recovery from extra pwr meas result (+ for missed Ncell for DTM mode).
21/01/11   cs        CR271591 Ncell monitor timer needs gas_id in DSDS
09/12/10   cs        CR264682 Add support for asynch dsp completion interrupt
09/09/10   cs        Further enhancement to 234959 to use min MON of 4/3
25/06/10   pg        CR244125: Add running avg meas_buf ptr sanity check for Q6
29/06/10   cja       238389 - Fix sign overflow in req_pos calculation for array index
14/06/10   og        Add monscan.req_pos range checking. Resolves CR242521.
20/05/10   nt        Additional NULL pointer check to CR234959
28/04/10   nt        CR234959 Optimisation to NCELL power monitors in the idle mode
24/12/09   cja       CR217174. Fix for monitor not scheduled in multi-frame.
12/03/09   ws        CR 173134 Added Test mode A to L1 interface and calculate
                     RLA_P for test modes from PDCH
02/10/08   cs        Lint fixes
04/09/08   dv        CR153510: Fix for Manual NW search failure
28/07/98   hv        Added KxMutex support
20/05/08   cs        Fix for Klocwork array bounds checking
09/01/08   og        Fix for CR130311. Ensure the power measurements are aborted
                     rather than stopped inorder to avoid rssi 0 conditions.
11/07/07   og        Filter adjacent arfcns according to their rx power delta in
                     idle ba list. Resolves CR122895.
06/06/07   ws        CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                     is called when we reselect to PBCCH cell stopping NC2 reporting
26/01/07   og        Fix to CR 109942, increase ncell info array sizes.
23/01/06   og        Removal of QLint high errors.
11/15/05   sv        Lint cleanup.
29/09/05   og        Merge to fixes of CR74437 and CR70332 from the mainline branch
                     Fixes inter RAT TC and 42.4.8.4.5 respectively.
12/08/05   og        Merge change 228472 from saber 5x branch. Fixes CR71702.
08/12/05   ws        Added initial FEATURE_GSM_DTM support
07/29/05   gfr       Support for NPL1 DTM interface changes
06/16/05   sv        Fixed lint error due to header file include.
04/21/05   sv        Set the initialization value for "monscan_in_progress".
04/20/05   og        Addition of filtered bep params to the msg metrics log packet.
01/15/05   kf        Send empty measurement reports in TBF\
10/09/04   kf        Modification of power meas for frame slamming to take into account
                     the updating of results.
06/09/04   kf        changes to power measurement to take into account frame slamming.
08/26/04   sv        Lint cleanup
24/08/04   kf        Fix for data abort when running wcdma meas with no ncells
23/06/04   og        Egprs Qual measurements handling.
05/26/04   sv        Lint Cleanup.
05/20/04   gfr       Mainline FEATURE_COMMON_PWR_HANDLER
02/25/04   gw        Lint clean up.
01/28/04   gw        Changes to support reselection optimizations.
11/05/03   gfr       Support for quad-band.
11/03/03   gw        Moved INTLOCK earlier in l1_sc_monscan_abort() and
                     l1_sc_monscan_abort_gprs() to avoid race condition.
09/01/03   gw        Changes to l1_sc_monscan_done functions to make SCE start
                     acquiring cells before their running averages are full.
08/21/03   gw        Made a couple of changes in running_avg functions to prevent
                     buffer overruns.
08/18/03   dp        Fix for div 0 error.
08/15/03   dp        Fix to report ded measurements after averaging over 1
                     reporting period but averaging over 2 SACCH block periods
                     when ranking for Ncell scheduling
04/08/03   kf        Added check for just_stuffed in l1_running_avg_add_meas().
                     Also changed l1_sc_globals.sorted_cell_count to 10 from 16
                     in transfer.
07/14/03   gw        Removed ASSERTs.
07/14/03   gw        Removed debug message from l1_sci_monscan_go().
07/11/03   gfr       Lint cleanup.
07/11/03   sw        Added WCDMA ID search trigger with zero length BA list
07/09/03   pjr       Removed check for empty BA list in l1_sci_monscan_do_rxlev_gprs_idle
                     to ensure gprs serving cell measurements are still performed.
06/23/03   gfr       Compiler warning cleanup.
06/19/03   pjr       Modified GPRS serving cell measurements RLA_P to running average.
06/19/03   gw        Added debug function to print state.
06/19/03   gw        Changes to debug messaging.
06/17/03   gw        Added check for power measure handler returning zero
                     measurements.
10/06/03   kf        changes number of sorted cells in transfer.
06/03/03   gw        Changes to make updating of BA list more robust.
05/15/03   gfr       Common power handler support.
14/05/03   kf        Changed report period in transfer
05/14/03   gw        Reinstated decrement of num_monitors removed
                     from l1_sci_monscan_rxlev_cb_gprs_idle() during power
                     transfer merge.
05/02/03   gw        Merged in power transfer changes from branch.
05/01/03   ag        Updates to ncell pbcch reading
04/28/03   gw        Changes to preserve power measure info. when entering and
                     leaving Transfer Mode.
04/09/03   gw        Fixed bug in l1_sc_monscan_done_trans() and added logging of
                     BA list to same function.
04/08/03   gw        Modified l1_sc_mon_scan_update() to use l1_sc_balist_get()
                     rather than l1_sc_balist_get_new_cells().  Removed unneeded
                     call to abort callback from l1_sci_monscan_power_cb_dedi().
04/04/03   gw        Added support for transferring ncell power info between GSM
                     modes.  Also added a bunch of debug messages under feature
                     DEBUG_SCE_MSG.
04/02/03   gw        Reorganization / clean up of monscan module.  Renamed some
                     functions (all mon_scan's were changed to monscan), moved
                     some functions here from l1_sc_main.c, removed some unneeded
                     variables and functions.
20/03/03   pjr       Added clearing of l1_sc_globals.mon_scan_in_progress in
                     gpl1_gprs_rxlev_cnf_monscan_idle. Also added handling for
                     serving cell measurements into gpl1_gprs_rxlev_cnf_monscan_idle
14/03/03   pjr       Added gprs serving cell measurements to gpl1_gprs_do_rxlev_idle
                     and gpl1_gprs_rxlev_cnf_monscan_idle functions
28/02/03   kf        Changes for rxlev aquiring in 52 idle
17/02/03   kf        Changes for running average in transfer.
01/10/03   gw        Changed some SCE function calls to reflect renaming of some
                     BA list functions done as part of SCE clean up.  Commented
                     out call to l1_sc_mon_scan_done() as this function no longer
                     exists.
01/02/03   gw        Added a callback function that is called at the end of
                     each reporting period in Dedicated Mode.  This is used
                     to synchronize the sending (to RR) of neighbor cell and
                     serving cell power measurements.
12/18/02   ag        Correct idle abort problems when returning to idle mode
                     from transfer during cell reselection under switch
                     FEATURE_GPRS_TBF_SUSPEND
12/06/02   pjr       Modified GPRS serving cell measurements
12/03/02   mk        General GPRS featurization cleanup, added temporary feature
                     FEATURE_DEBUG_GPRS_OK.
11/25/02   mk        Updated GPRS baseline.
11/21/02   mk        Deleted unsupported code under FEATURE_GSM_ALTERNATE_PLMN_SEARCH.
11/18/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/15/02   dp        Fix to trigger sending up surround_meas_ind even when
                     BA list is empty
11/03/02   tb        Added support for INTERRAT_GTOW
10/11/02   gw        Fix to schedule monitors on a new BA list if available
10/08/02   kf        Added PL1 GPRS measurement code hooks under switch
                     FEATURE_GPRS_MEAS.
09/06/02   plm       Featurized static function and variable forward declarations to
                     eliminate compiler warnings.
08/23/02   plm       Commented out declared but unused function power_scan_msg_abort_callback()
                     to eliminate compiler warning.
                     Added include of string.h to eliminate compiler warnings.
                     Added extern for the function l1_sc_mon_scan_done_ded() to
                     eliminate compiler warning.
08/23/02   plm       Fixed problem with comment line to get rid of compiler warning.
08/12/02   gw        Featurized code for alternate PLMN searching.
07/26/02   dp        Separated a bunch of monscan functions to be specific for
                     dedicated to support averaging over 2 SACCH block periods
                     for sorting cells
07/02/02   gw        Moved most code for managing running averages into
                     l1_sc_main.c.  Modified starting/stopping/updating
                     of monscan.  Added function mon_scan_done_idle.
06/27/02   gw        Changes to support running averages of ncell power
                     measurements in Idle Mode.
05/30/02   pjr/mk    Added PL1 GPRS code hooks under switch FEATURE_GSM_GPRS_L1.
05/21/02   gw        Fixed bug in monscan aborts.  Changed pscan to use latest
                     message layer functions.  Changed references to
                     idle_rx_in_progress to idle_monitors_in_progress.
                     Removed function gl1_dBm_to_l1pwr().
05/10/02   JC        Added support for new gl1_cfg_power_measure API;
                     Required for IDLE monitoring.
04/21/02   dp        Added function l1_sc_rx_power_acceptable
04/17/02   gw        Fixed bug in l1_sci_power_cnf_monscan so that results are
                     applied to the correct ARFCN.
03/28/02   JC/dp     Added to l1_sci_do_monscan_ded as a temp hack till we
                     consolidate dedicated with idle
03/22/02   JC        Added support for RXLEV reporting in NCELL monitors.
02/07/02   gw        Improved abort mechanisms in power_scan and mon_scan.
                     Cleaned up and removed unused variables.  Combined
                     some callback functions.
12/18/01   dp        Changes to support aborting power measurements.  Changed
                     interface for monscan and powerscan cnf functions to
                     take an array of measurements instead of having to be invoked
                     separately for each measurement
11/27/01   mk        Misc cleanup.
10/24/01   gw        Modified scheduling in power scan.
10/16/01   gw        Mainlined features OLD_SCE and FEATURE_FINDBCCH.
                     Changed pscan to use message layer power measurement.
                     Changed CCWW types to QCT types.  Removed conversion
                     to dBm from monscan_msg_callback().  Removed unused
                     code left over from old style monscan and pscan.
                     Changed l1_sci_do_monscan to provide functionality
                     required by dedicated mode.
10/10/01   gw        Added support for message layer power measurements.
09/25/01   mk/gw     General SCE cleanup. Mainlined MONITOR_DEBUG, added
                     PScan/Ncell support using FINDBCCH and MULTIMON.
08/31/01   JC        Placed COMMENT block around reference to Ps_LEV and
                     obsoleted an unnecessary compile switch.
08/20/01   mk/gw     Added Ncell support, featurized via MONITOR_DEBUG.
08/20/01   mk        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "l1_drx.h"
#include "gl1_msgi.h"
#include "gl1_msg_g.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "l1_isr.h"
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"
#include "l1_sc.h"
#include "l1_log.h"
#include "l1_utils.h"
#include "msg.h"
#include "l1_task.h"
#include "task.h"
#include "l1i.h"

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_task.h"
#include "gpl1_gprs_serving_cell.h"
#include "l1_sc_drv.h"

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"

#include "gpl1_grm_intf.h"

#include "l1_os.h"

#ifdef FEATURE_GPRS_GBTA
#include "gpl1_dual_sim.h"
#include "gpl1_gprs_ptcch.h"
#endif /* FEATURE_GPRS_GBTA */

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#include "gl1_arbitrator_utils.h"
#include "gl1_arbitrator_cxm.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#include "gl1_hw_g.h"

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
#include "gl1_asdiv.h"
#endif
#ifdef GL1_GMSK_MOD_DET
#include "gprs_mem.h"
#endif

#define L1_NCEll_PCH_SHUTDOWN_TIMEOUT  60000 /* ms = 1 mins */
#define PWR_MEAS_ABORT_SUSPENSION_FRAME_DELAY 2
extern rex_tcb_type tcb_pl1;
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern gl1_hw_asdiv_type1_T gl1_hw_asdiv_type1;
uint32 packet_scell_meas_count = 0;
uint32 skip_scell_meas =0;
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
#ifdef GL1_GMSK_MOD_DET
extern uint16 *decStat[NUM_GERAN_DATA_SPACES] ;
#endif
typedef enum
{
  NCELL_PCH_MONITOR_NORMAL,
  NCELL_PCH_TIMER_EXPIRED
} ncell_pch_state_enum;

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

#define L1_SC_PWR_MAX_MEAS_PER_CALL        100 /* 100 limit in message layer */

/* Minimum number of meaurements in running average
 * buffer before ncell can be sorted.  Once an ncell
 * is sorted acquistions can begin on it if it is
 * strong enough.
 */
#define L1_SC_PWR_MIN_NUM_MEAS_TO_SORT     1

/*
 * Static Variable Declarations
 * ----------------------------
 */

typedef struct
{
   /* used in Idle Mode to indicate completion of
    * a pass through the BA list
    */
   boolean wrapped;

   /* have made at least req_passes measurements
    * on each cell
    */
   boolean done_req_passes;

   /* used to prevent race condition on abort */
   uint8   id;
   
   /* number of monitors before x3 for adaptive, 0 denotes adaptive inactive */
   uint8   pm_adaptive_original_cnt;
   
} l1_sc_monscan_results_T;


typedef struct
{
   boolean                   active;
   uint8                     id;

   /* data used by ISR */
   uint16                    req_cell_count;
   cell_T**                  req_cells;
   l1_sc_monscan_results_T*  isr_results;
   uint8                     req_cells_id;

   /* data used by task */
   uint16                    cnf_cell_count;
   cell_T**                  cnf_cells;
   l1_sc_monscan_results_T*  task_results;
   uint8                     cnf_cells_id;

   uint8                     req_pos;
   uint8                     cnf_pos;
   uint8                     req_pos_prev; /* used to restart power monitoring from last place if aborted */

#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
   /* In some multi-RAT COEX scenarios a limited number of meas
    * are protected by a COEX interference from a peer RAT.
    *
    * The meas positions protected from COEX interference for the current
    * req / cnf cycle is determined by the GL1 arbitrator.
    *
    * For these scenarios we maintain 2 x req and 2 x cnf indexes
    * on the req list
    *
    * req_pos and cnf_pos track ARFCNs assigned to the COEX protected
    * meas positions.
    *
    * req_pos_coex_unprotected and cnf_pos_coex_unprotected track
    * ARFCNs assigned to COEX unprotected meas positions
    *
    * Each cnf / req independently traverse round-robin through the
    * BA list to assure equal distribution of ARFCNs across the
    * protected and unprotected meas positions.
    *
    * NOTE : This functionality is currently only applied in CS modes.
    *
    */
    uint8                    req_pos_coex_unprotected;
    uint8                    cnf_pos_coex_unprotected;

#endif /* FEATURE_GSM_COEX_REDUCED_MEAS */

   /* new ba list waiting to be picked up */
   uint16                    new_cell_count;
   cell_T**                  new_cells;
   uint8                     new_cells_id;

   /* flags to indicate new BA list is available */
   boolean                   new_balist_pending;
   boolean                   new_cells_valid;

   /* Indicates whether monscan_done_*() should be called from the
    * next l1_sc_monscan_power_cb*().
    */
   boolean                   next_period;

   /* indicates whether the message layer is currently doing a power meas */
   boolean                   meas_in_progress;

   /* indicates if the pwr meas block start flag should be set this frame */
   boolean                   block_start;

   /* required number of passes through BA list before first callback */
   uint8                     req_passes;

   /* number of passes through BA list - saturates to req_passes */
   uint8                     num_passes;

   /* number of meas requested */
   uint8                      num_req;

   /* number of monitors before x3 for adaptive, 0 denotes adaptive inactive */
   uint8                      pm_adaptive_original_cnt;

   /* function to call at end of reporting period */
   void                    (*next_period_ded_cb)(gas_id_t gas_id);

} monscan_T;


typedef struct
{
  ARFCN_T                       arfcn[L1_SC_PWR_MAX_MEAS_PER_CALL];
  sys_algo_agc_T                *agc_ptr[L1_SC_PWR_MAX_MEAS_PER_CALL];
  gl1_msg_pwr_meas_result_type  pwr_meas_cb_param;
  dBx16_T                       pwr_meas_results[L1_SC_PWR_MAX_MEAS_PER_CALL];
  boolean                       yielded[L1_SC_PWR_MAX_MEAS_PER_CALL];
  #ifdef GL1_GMSK_MOD_DET
  uint16                        decstat[L1_SC_PWR_MAX_MEAS_PER_CALL];
#endif
} l1_sci_ncell_info_type;

uint32  antenna_to_use[L1_SC_PWR_MAX_MEAS_PER_CALL];

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
#define  APM_METRICS_LOG_SZ   (5)
static  uint16  apm_metrics_log[APM_METRICS_LOG_SZ] = {0};
#endif

/*
 * Prototypes for local Functions
 * ------------------------------
 */
void l1_sci_monscan_done_idle( gas_id_t gas_id );
static void l1_sci_monscan_done_ded( gas_id_t gas_id );
static void l1_sci_monscan_done_gprs_idle( gas_id_t gas_id );
static void l1_sci_monscan_power_cb_ded_tch( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id );
static void l1_sci_monscan_power_cb_ded_sdcch( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id );
static void l1_sci_monscan_power_cb_idle( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id );
static void l1_sci_monscan_abort_cb_ded( gas_id_t gas_id );
static void l1_sci_monscan_abort_cb_idle( gas_id_t gas_id );
static void l1_sc_monscan_done_idle( void* data, gas_id_t gas_id );
static void l1_sc_monscan_done_ded( void* data, gas_id_t gas_id );

static void l1_sci_monscan_done_trans( gas_id_t gas_id );
static void l1_sci_monscan_rxlev_cb_gprs_idle(gl1_msg_pwr_meas_result_type * rxlev_results_info,gas_id_t gas_id);
static void l1_sci_monscan_rxlev_cb_trans(gl1_msg_pwr_meas_result_type * rxlev_results_info,gas_id_t gas_id);
static void l1_sci_monscan_abort_cb_gprs_idle( gas_id_t gas_id );
static void l1_sci_monscan_abort_cb_trans( gas_id_t gas_id );
static void l1_sc_monscan_done_gprs_idle( void* data, gas_id_t gas_id );
static void l1_sc_monscan_done_trans( void* data, gas_id_t gas_id );
static boolean l1_sc_monscan_get_num_idle_meas(uint8 *num_meas, uint8* meas_per_frame, gas_id_t gas_id);

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED

static l1_sc_bs_T* l1_validate_bs_ptr( uint32 bs_pos, gas_id_t gas_id);
static uint32      l1_get_next_ded_adaptive_result(gl1_msg_pwr_meas_result_type *rpt, uint32 apm_idx, ARFCN_T cell_arfcn, gas_id_t gas_id);

#endif


/* There was a need to speed SCE for new GCF3.18 the MS needs to */
/* react to new BA List cells for PMO NC2 measurements of 7 sec. */
void l1_sci_monscan_swap_cells( gas_id_t gas_id );

#define NO_OF_XFER_VALUE_MEAS 3
/*The frame that has been requested not to include in the C value calculation */
#define EXCLUDE_SCELL_MONITOR_FRAME 9
/*Schedule for the scell monitors in XFER mode */
const uint16 FRAME_FOR_SCELL_MONITOR_TRANS[NO_OF_XFER_VALUE_MEAS] = {0, 5, 9};

#define PM_ADAPTIVE_MULTIPLIER   (3)
#define PM_ADAPTIVE_INCREMENT    (PM_ADAPTIVE_MULTIPLIER-1)
#define NO_OF_DED_SCELL_MON      4
#define MAX_SCELL_MON_DED       14

const uint16 FRAME_FOR_SCELL_MONITOR_DED[NO_OF_DED_SCELL_MON] = {0, 1, 13, MAX_SCELL_MON_DED};

#define TX_DIV_PROBE_CNT   (2)

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
/* 
** FR23109 need to use with x3 values, these are the trigger points to insert scell measurements into list 
*/

#define TX_DIV_APM_PROBE_CNT   (PM_ADAPTIVE_MULTIPLIER*TX_DIV_PROBE_CNT)

const uint16 FRAME_FOR_SCELL_MONITOR_DED_APM[NO_OF_DED_SCELL_MON] = {  0, 
                                                                       1*PM_ADAPTIVE_MULTIPLIER,
                                                                      13*PM_ADAPTIVE_MULTIPLIER,
                                                       MAX_SCELL_MON_DED*PM_ADAPTIVE_MULTIPLIER };
#else

#define TX_DIV_APM_PROBE_CNT   TX_DIV_PROBE_CNT

#endif 

/* 2 or 6 */
#define TX_DIV_PROBE_COUNT   TX_DIV_APM_PROBE_CNT


#ifdef FEATURE_GSM_DED_SCELL_MONITOR
boolean  ba_list_empty = FALSE ;
#endif

#ifdef FEATURE_GSM_DTM
static void l1_sci_monscan_do_dtm( uint8 num_meas, uint8 meas_per_frame, gas_id_t gas_id );
static void l1_sci_monscan_power_cb_dtm( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id );

#define NO_OF_C_VALUE_MEAS 3
const uint16 FRAME_FOR_SCELL_C_VALUE_MONITOR[NO_OF_C_VALUE_MEAS] = {8, 16, 23};
#endif

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING

#ifdef FEATURE_GSM_DED_SCELL_MONITOR
#define FIRST_FRAME_FOR_PROBE_MONITOR   2
#define SECOND_FRAME_FOR_PROBE_MONITOR  3
#else
#define FIRST_FRAME_FOR_PROBE_MONITOR   0
#define SECOND_FRAME_FOR_PROBE_MONITOR  1
#endif
#else
#define FIRST_FRAME_FOR_PROBE_MONITOR   0
#define SECOND_FRAME_FOR_PROBE_MONITOR  1
#endif

const uint8 PROBE_SCELL_MONITOR_DED[2]     = {FIRST_FRAME_FOR_PROBE_MONITOR, SECOND_FRAME_FOR_PROBE_MONITOR};

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
const uint8 PROBE_SCELL_MONITOR_DED_APM[2] = { FIRST_FRAME_FOR_PROBE_MONITOR*PM_ADAPTIVE_MULTIPLIER, 
                                              SECOND_FRAME_FOR_PROBE_MONITOR*PM_ADAPTIVE_MULTIPLIER};
#endif

typedef struct
{
  monscan_T                    monscan ;
  /* Memory for interacting with message layer */
  ARFCN_T                      arfcn[L1_SC_PWR_MAX_MEAS_PER_CALL];
  sys_algo_agc_T              *agc_ptr[L1_SC_PWR_MAX_MEAS_PER_CALL];
#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
  boolean                      coex_protected_meas[L1_SC_PWR_MAX_MEAS_PER_CALL];
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS */
  gl1_msg_pwr_meas_result_type pwr_meas_cb_param;
  dBx16_T                      pwr_meas_results[L1_SC_PWR_MAX_MEAS_PER_CALL];
  boolean                      pwr_meas_yield[L1_SC_PWR_MAX_MEAS_PER_CALL]; /* monitor result was tainted by COEX interference from another RAT */
  uint8                        ncell_info_res_idx ;
  l1_sci_ncell_info_type ncell_info[2];
  uint8                        send_rr_report ;
  l1_sc_monscan_results_T      results[ 2 ];
  uint32                       num_monitors;
  uint32                       monitors_remaining;
  uint8                        sacch_rep_period_count ;
  rex_timer_type               l1_pch_ncell_monitor_shutdown;
  ncell_pch_state_enum         ncell_monitor_shutdown_state;
  uint8                        reduced_mon_sched_period_index;

#ifdef FEATURE_GSM_COEX_SW_CXM
  /* Function passed back from the GL1 arbitrator used to query if a specific monitor
     is in a COEX protected position */
  garb_is_coex_protected_mon_t is_coex_protected_monitor;
#endif /* FEATURE_GSM_COEX_SW_CXM */
#ifdef GL1_GMSK_MOD_DET
   uint16                        *decstat;
#endif
}l1_sc_pwr_data_t;

static void l1_sci_monscan_ba_list_use_new(gas_id_t gas_id);
#ifdef FEATURE_GSM_COEX_SW_CXM
static void l1_coex_report_lowest_power_per_band(int32 *pwr_per_band, gas_id_t gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */
/*===========================================================================
FUNCTION l1_sc_ncell_monitor_timer_deactivate

DESCRIPTION
  Deactivates the ncell monitor shutdown timer
===========================================================================*/
static void l1_sc_ncell_monitor_timer_deactivate(
              l1_sc_pwr_data_t *loc_l1_sc_pwr_data_ptr );

static l1_sc_pwr_data_t  l1_sc_pwr_data[NUM_GERAN_DATA_SPACES];

/*================================================================================*/


/**********************************************************************
 *
 *  Module:  Monscan
 *  -------
 *  Handles making power measurements on cells in BA list.  Separate
 *  functions are provided for Idle Mode, Dedicated Mode (and GPRS
 *  transfer Mode) to support the different monitoring schemes
 *  required in the different modes.
 *
 **********************************************************************/


/*===========================================================================

FUNCTION l1_sc_monscan_init

DESCRIPTION
  Initialize the monscan module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_monscan_init( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

   gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   uint32 i;

   l1_sc_pwr_data_ptr->monscan.active              = FALSE;
   l1_sc_pwr_data_ptr->monscan.id                  = 0;
   l1_sc_pwr_data_ptr->monscan.isr_results         = &l1_sc_pwr_data_ptr->results[ 0 ];
   l1_sc_pwr_data_ptr->monscan.task_results        = &l1_sc_pwr_data_ptr->results[ 1 ];
   l1_sc_pwr_data_ptr->monscan.new_cells_valid     = FALSE;
   l1_sc_pwr_data_ptr->monscan.new_balist_pending  = FALSE;
   l1_sc_pwr_data_ptr->monscan.meas_in_progress    = FALSE;
   l1_sc_pwr_data_ptr->monscan.next_period         = FALSE;
   l1_sc_pwr_data_ptr->monscan.next_period_ded_cb  = NULL;
   l1_sc_pwr_data_ptr->monscan.block_start         = FALSE;

   /* FR23109   extend count per list */
   l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt    = 0;
   l1_sc_pwr_data_ptr->results[0].pm_adaptive_original_cnt = 0;
   l1_sc_pwr_data_ptr->results[1].pm_adaptive_original_cnt = 0;

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
   for(i=0 ; i< APM_METRICS_LOG_SZ; i++)            
   {
      apm_metrics_log[i] = 0;                       /* reset metrics count */
   }
#endif
   /* Supply the buffer for the power measure results */
   l1_sc_pwr_data_ptr->pwr_meas_cb_param.pwr_dBm_x16 = l1_sc_pwr_data_ptr->pwr_meas_results;
   l1_sc_pwr_data_ptr->pwr_meas_cb_param.yielded     = l1_sc_pwr_data_ptr->pwr_meas_yield; /* monitor result was tainted by COEX interference from another RAT */
   #ifdef GL1_GMSK_MOD_DET
   l1_sc_pwr_data_ptr->pwr_meas_cb_param.decstat     = l1_sc_pwr_data_ptr->decstat;
   #endif
   gpl1_gprs_meas_mode_ptr->allowed_to_go = FALSE;

   /* Supply the buffer for the power measure results */
   l1_sc_pwr_data_ptr->ncell_info[0].pwr_meas_cb_param.pwr_dBm_x16 = l1_sc_pwr_data_ptr->ncell_info[0].pwr_meas_results;
   l1_sc_pwr_data_ptr->ncell_info[1].pwr_meas_cb_param.pwr_dBm_x16 = l1_sc_pwr_data_ptr->ncell_info[1].pwr_meas_results;

   l1_sc_pwr_data_ptr->ncell_info[0].pwr_meas_cb_param.yielded = l1_sc_pwr_data_ptr->ncell_info[0].yielded;
   l1_sc_pwr_data_ptr->ncell_info[1].pwr_meas_cb_param.yielded = l1_sc_pwr_data_ptr->ncell_info[1].yielded;

   #ifdef GL1_GMSK_MOD_DET
   l1_sc_pwr_data_ptr->ncell_info[0].pwr_meas_cb_param.decstat = l1_sc_pwr_data_ptr->ncell_info[0].decstat;
   l1_sc_pwr_data_ptr->ncell_info[1].pwr_meas_cb_param.decstat = l1_sc_pwr_data_ptr->ncell_info[1].decstat;
   #endif
   
   l1_sc_globals_ptr->monscan_in_progress = FALSE;
   L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);

#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_GSM_SCE_PWR_MEAS_DBG)
   {
      idle_data_T* l1_idle_data = get_ptr_to_idle_data(gas_id);
      l1_idle_data->do_pwr_meas_FN = L1_GPRS_INVALID_FN;
   }
#endif

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}

/*===========================================================================

FUNCTION l1_sc_monscan_update

DESCRIPTION
  Tells monscan that a new BA list is available and should be picked up.
  If monscan is active the new BA list is picked up at the end of the
  current set of measurements, otherwise the new BA list is picked up
  immediately.

  This function does not cause monscan to transition from inactive to
  active.  This transition is triggered by calling the ISR context
  function l1_sci_monscan_go().

DEPENDENCIES
  New BA list must be present in BA module (i.e.
  l1_sc_balist_new() must have been called).

RETURN VALUE
  None

SIDE EFFECTS
  Triggers transfer of new BA list into regular BA list (by causing call
  to l1_sc_balist_use_new()).

===========================================================================*/
void l1_sc_monscan_update( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   ASSERT_TASK();

   GL1_ISR_SAVE_LOCK(gas_id);
   if (l1_sc_pwr_data_ptr->monscan.active)
   {
      l1_sc_pwr_data_ptr->monscan.new_cell_count = l1_sc_balist_get( &l1_sc_pwr_data_ptr->monscan.new_cells, gas_id );
      l1_sc_pwr_data_ptr->monscan.new_cells_id   = l1_sc_balist_get_new_id(gas_id);

      /* Monscan is already active and doing measurements.
       * Set flags so that update is completed when monscan
       * is ready.
       */
      l1_sc_pwr_data_ptr->monscan.new_cells_valid    = TRUE;
      l1_sc_pwr_data_ptr->monscan.new_balist_pending = TRUE;
   }
   else
   {
      /* Monscan is not active yet, start from scratch. */
      l1_sc_pwr_data_ptr->monscan.req_cell_count = l1_sc_balist_get( &l1_sc_pwr_data_ptr->monscan.req_cells, gas_id );
      l1_sc_pwr_data_ptr->monscan.req_cells_id   = l1_sc_balist_get_new_id(gas_id);

      l1_sci_monscan_ba_list_use_new(gas_id);

      l1_sc_pwr_data_ptr->monscan.num_passes         = 0;
      l1_sc_pwr_data_ptr->monscan.new_cells_valid    = FALSE;

      l1_sc_pwr_data_ptr->monscan.meas_in_progress   = FALSE;
      l1_sc_pwr_data_ptr->monscan.next_period        = FALSE;
      l1_sc_pwr_data_ptr->monscan.req_pos            = 0;
      l1_sc_pwr_data_ptr->monscan.cnf_pos            = 0;
      l1_sc_pwr_data_ptr->monscan.req_pos_prev       = 0;

#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
      l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected = 0;
      l1_sc_pwr_data_ptr->monscan.cnf_pos_coex_unprotected = 0;
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS */

   }
   GL1_ISR_SAVE_UNLOCK(gas_id);

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif

}

/*===========================================================================

FUNCTION  l1_sci_monscan_go

DESCRIPTION
  Moves monscan from inactive to active.

DEPENDENCIES
  Monscan must have been told to pick up the current BA list by calling
  l1_sc_monscan_update().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_monscan_go( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   l1_sc_pwr_data_ptr->monscan.req_pos                   = 0;
   l1_sc_pwr_data_ptr->monscan.cnf_pos                   = 0;
   l1_sc_pwr_data_ptr->monscan.req_pos_prev              = 0;
#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
   l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected = 0;
   l1_sc_pwr_data_ptr->monscan.cnf_pos_coex_unprotected = 0;
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS */

   l1_sc_globals_ptr->monscan_in_progress = TRUE;
   L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);
   l1_sc_pwr_data_ptr->monscan.active                    = TRUE;
}

/*===========================================================================

FUNCTION  l1_sc_monscan_abort

DESCRIPTION
  Stops monscan.  Any measurements in progress are aborted.  Monscan moves
  to the inactive state.  One or two more frame ticks may be required to
  allow the abort to complete.  When the abort has completed
  l1_sc_globals.monscan_in_progress will be set to FALSE.

  NOTE:  Do NOT use monscan.active to determine whether the abort has
  completed.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_monscan_abort( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
#ifdef FEATURE_GPRS_GBTA
   gas_id_t idle_gas_id;
#endif /*  FEATURE_GPRS_GBTA */
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   ASSERT_TASK();

   MSG_GERAN_HIGH_4_G("l1_sc_monscan_abort: active %d, meas_in_progress %d, wsrch %d, gl1_msg_meas_in_progress %d",
                       l1_sc_pwr_data_ptr->monscan.active,
                       l1_sc_pwr_data_ptr->monscan.meas_in_progress,
                       l1_sc_globals_ptr->wcdma_srch_in_progress,
                       gl1_msg_meas_in_progress(gas_id));
   if (l1_sc_pwr_data_ptr->monscan.active)
   {
      GL1_ISR_SAVE_LOCK(gas_id);
      l1_sc_pwr_data_ptr->monscan.active = FALSE;
      ++l1_sc_pwr_data_ptr->monscan.id;

      /* Not really needed but would ensure
       * that req_pos and cnf_pos line up
       * if monscan was to somehow restart.
       */
      l1_sc_pwr_data_ptr->monscan.req_pos = l1_sc_pwr_data_ptr->monscan.cnf_pos;

      if (l1_sc_pwr_data_ptr->monscan.meas_in_progress || gl1_msg_meas_in_progress(gas_id))
      {
         if( FALSE == l1_sc_globals_ptr->wcdma_srch_in_progress )
         {
           gl1_msg_stop_pwr_meas(gas_id);
         }
         else
         {
           /* Abort the message layer
            * @@OG: Changed from stop to abort for CR130311.
            * This ensures no rssi 0 condition is encountered
            * when Wcdma clocks are switched off.
            */
           gl1_msg_abort_pwr_meas(gas_id);
         }
      }
      else
      {

         /*for reseting and terminating in case we abort after power scan meas have completed*/
#ifdef FEATURE_GPRS_GBTA
         if( gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED,gas_id,&idle_gas_id))
         {
           gl1_msg_stop_pwr_meas(gas_id);
         }
         else
#endif /*  FEATURE_GPRS_GBTA */
         {
           gl1_msgi_pwr_meas_terminate(gas_id);
         }
      }
      l1_sc_globals_ptr->monscan_in_progress = FALSE;
      L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);

      GL1_ISR_SAVE_UNLOCK(gas_id);

      /* Make sure that if a new BA list was pending it is
       * picked up.
       */
      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);

#ifdef DEBUG_SCE_MSG
         #error code not present
#endif
      }
   }

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}

/* This funtion should go away when we move to the unified power
 * measure handler.  When that happens l1_sc_moncan_abort() will
 * hanlde GPRS too.
 */
void l1_sc_monscan_abort_gprs( gas_id_t gas_id )
{
   gas_id_t idle_gas_id;
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   ASSERT_TASK();

   MSG_GERAN_HIGH_4_G("l1_sc_monscan_abort_gprs: active %d, meas_in_progress %d, wsrch %d, gl1_msg_meas_in_progress %d",
                       l1_sc_pwr_data_ptr->monscan.active,
                       l1_sc_pwr_data_ptr->monscan.meas_in_progress,
                       l1_sc_globals_ptr->wcdma_srch_in_progress,
                       gl1_msg_meas_in_progress(gas_id));

   if (l1_sc_pwr_data_ptr->monscan.active)
   {
      GL1_ISR_SAVE_LOCK(gas_id);
      l1_sc_pwr_data_ptr->monscan.active = FALSE;
      ++l1_sc_pwr_data_ptr->monscan.id;

      /* Not really needed but would ensure
       * that req_pos and cnf_pos line up
       * if monscan was to somehow restart.
       */
      l1_sc_pwr_data_ptr->monscan.req_pos = l1_sc_pwr_data_ptr->monscan.cnf_pos;

      if ((l1_sc_pwr_data_ptr->monscan.meas_in_progress) || gl1_msg_meas_in_progress(gas_id)
#ifdef FEATURE_GPRS_GBTA
         || ( gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED,gas_id,&idle_gas_id)) 
#endif /*  FEATURE_GPRS_GBTA */
         )
      {
         gl1_msg_abort_pwr_meas(gas_id);
      }
      /* We need to reset monscan_in_progress as well. This shouldn't depend on
       * whether meas_in_progress is TRUE or not. More importantly, only if the SCE
       * is not in operation, which is decided by monscan_in_progress, will the TBF
       * be aborted.
       */
      if (l1_sc_globals_ptr->monscan_in_progress)
      {
         l1_sc_globals_ptr->monscan_in_progress = FALSE;
         L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);
      }
      GL1_ISR_SAVE_UNLOCK(gas_id);

      /* Make sure that if a new BA list was pending it is
       * picked up.
       */
      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);

#ifdef DEBUG_SCE_MSG
         #error code not present
#endif
      }
   }

   /* abort interference measurements */
   gl1_msg_abort_int_meas(gas_id);

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif

}

/*===========================================================================

FUNCTION gl1_is_pwr_meas_block_start

DESCRIPTION
  Returns if the pwr measure block start should be set this frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean gl1_is_pwr_meas_block_start ( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   return l1_sc_pwr_data_ptr->monscan.block_start;
}



/*===========================================================================

FUNCTION gl1_set_pwr_meas_block_start

DESCRIPTION
  Sets the pwr meas block start flag

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_set_pwr_meas_block_start (boolean value, gas_id_t gas_id)
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   l1_sc_pwr_data_ptr->monscan.block_start = value;
}


/*===========================================================================

FUNCTION l1_sc_monscan_set_req_passes

DESCRIPTION
  Tell monscan how many measurements per cell to do before signaling the
  end of the first measurement period.  This is only used in Idle Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_monscan_set_req_passes( uint8 req_passes, gas_id_t gas_id)
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   l1_sc_pwr_data_ptr->monscan.req_passes = req_passes;

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}

/*===========================================================================

FUNCTION l1_sci_monscan_do_idle

DESCRIPTION
  Do the next batch of measurements on the BA list.  Invoked whenever
  a PCH block is upcoming.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

/*
 * From this we see an average meas period of 4/3 = 1.33 which is still spec
 * compliant as the minimum MON duty cycle is 1.3
 */
#define DUTY_CYCLE_FOR_REDUCED_MON              12

const uint8 reduced_mon_sched_period[ DUTY_CYCLE_FOR_REDUCED_MON ]  = { 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1 };
const uint8 skipping_mfrm_2_mon_sched_period[ DUTY_CYCLE_FOR_REDUCED_MON ]  = { 5, 0, 5, 0, 5, 0, 5, 0, 5, 0, 5, 0 };
const uint8 skipping_mfrm_2_mon_sched_period_power_opt[ DUTY_CYCLE_FOR_REDUCED_MON ]  = { 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0 };

void l1_sci_monscan_do_idle( uint8 num_meas, uint8 meas_per_frame, gas_id_t gas_id  )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   idle_data_T* l1_idle_data = get_ptr_to_idle_data(gas_id);
   sys_band_T  serving_cell_band;
#endif
 
   uint16        num;
   uint16        max_meas;
   cell_T*       cell;

   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   idle_data_T      *l1_idle_data_ptr = &l1_idle_data_store[gas_id];

   uint8         max_num_frames = 4;
   gas_id_t      pwr_meas_gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
#ifdef FEATURE_GSM_COEX_SW_CXM
   int32         pwr_per_band[INVALID_BAND] = {0};
   dBx16_T       rx_power;
#endif


   /* variables related to FR54762 */
   gl1_concman_config_params_type gl1_concman_config_params;
   cmgr_scenario_status_e status;
   cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);


#ifdef FEATURE_GPRS_GBTA
   gas_id_t      ptm_gas_id      = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

   if( IS_IDLE_EV_PWR_MON_ACTIVE(gas_id) &&
       OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)
      )
   {
     /* get peer gas of of transfer sub */
     ptm_gas_id = gl1_get_gas_id_for_ptm_client();

     /* It is possible to perform 6 Power meas when entire frame is available */
     if(( ptm_gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED )
        ||( !gpl1_gprs_is_ptcch_active(ptm_gas_id) )
       )
     {
        MSG_GERAN_HIGH_1_G("GL1 GBTA monscan_do_idle from num_meas %d to 6 ", num_meas);
        num_meas = 6;
        meas_per_frame = 6;
     }
     max_num_frames =  1;  // Scheduling Power Mon on 1 PTCCH frame.
   }
#endif /*  FEATURE_GPRS_GBTA */
#if defined (FEATURE_QBTA)
  if(gl1_hw_check_in_QBTA(gas_id))
  {
     max_num_frames = 1;
  }
#endif
   ASSERT_INT();

   if ( l1_sc_pwr_data_ptr->reduced_mon_sched_period_index >= DUTY_CYCLE_FOR_REDUCED_MON )
   {
     l1_sc_pwr_data_ptr->reduced_mon_sched_period_index = 0;
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* reset duty cycle if missed page to avoid skipping too many power measurement. */
   if ( l1_idle_data_ptr->idle_rx_missed_page )
   {
     l1_sc_pwr_data_ptr->reduced_mon_sched_period_index = 0;
     l1_idle_data_ptr->idle_rx_missed_page = FALSE;
   }
#endif

   if (  ( l1_idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 2 )
      && ( l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state != NCELL_PCH_TIMER_EXPIRED )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_GPRS_GBTA
      /*skip pwr monitors in idle +idle +idle case only, not when transfer sub is scheduling pwr monitors for idle sub*/
      && !(IS_IDLE_EV_PWR_MON_ACTIVE(gas_id) && OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
#endif
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      )
   {
     /* When BS_PA_MFRMS is 2, to skip ncell monitors at alternative PCH decode */
     if (l1_idle_data_ptr->idle_power_opt > 0)
     {
       num_meas = meas_per_frame = skipping_mfrm_2_mon_sched_period_power_opt[ l1_sc_pwr_data_ptr->reduced_mon_sched_period_index++ ];
     }
     else
     {
       num_meas = meas_per_frame = skipping_mfrm_2_mon_sched_period[ l1_sc_pwr_data_ptr->reduced_mon_sched_period_index++ ];
     }
   }
   else
   if ( l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state == NCELL_PCH_TIMER_EXPIRED )
   {
     /* alternate each period between performing one then two ncell monitors per PCH decode */
     num_meas = meas_per_frame = reduced_mon_sched_period[ l1_sc_pwr_data_ptr->reduced_mon_sched_period_index++ ];
   }
   
   if ( num_meas > L1_SC_PWR_MAX_MEAS_PER_CALL )
   {
      MSG_GERAN_ERROR_2_G("Requested %d measurements, doing %d.", num_meas, L1_SC_PWR_MAX_MEAS_PER_CALL);
      num_meas = L1_SC_PWR_MAX_MEAS_PER_CALL;
   }

   if ( ( l1_sc_pwr_data_ptr->monscan.active ) && (!l1_sc_pwr_data_ptr->monscan.meas_in_progress) )
   {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     if ( gl1_drdsds_enabled( gas_id ) )
     {
       /*capture serving cell band this is to check if we are making non-serving band measurements in the next three blocks.
        Non-serving cell band measurements must be checked with TRM before proceeding when DR-DSDS is enabled. RF tuning has an impact on the other tech
       */
       serving_cell_band = gl1_get_band(l1_idle_data->campedon_cell_ARFCN);
      }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

      /* If current BA list is empty  just invoke callback */
      if ( l1_sc_pwr_data_ptr->monscan.req_cell_count == 0 )
      {
         l1_sci_monscan_done_idle(gas_id);
         MSG_GERAN_MED_0_G("l1_sci_monscan_do_idle req_cell_count = 0");
         return;
      }

      /* Only measure each cell once per paging block even
       * if the BA list length is less than the number of
       * measurements per paging block.
       */
      if (l1_sc_pwr_data_ptr->monscan.req_cell_count < num_meas)
      {
         num_meas = (uint8)l1_sc_pwr_data_ptr->monscan.req_cell_count;
      }
      if(num_meas >0)
      {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY 
        /*Inform MCPM about critical scenario now if needed*/
        if((timeline_error_recovery_needed[gas_id] !=0) && (critical_scn_updated_to_mcpm[gas_id] == FALSE))
        {
          gl1_clkdata_speed(TRUE, L1_TIME_LINE_ERROR, 0, L1_TIME_LINE_DELAY,gas_id);
          critical_scn_updated_to_mcpm[gas_id] = TRUE;
        }
#endif
      }

#ifdef FEATURE_GPRS_GBTA
      MSG_GERAN_HIGH_3_G("monscan_do_idle num_meas %d meas_per_frame %d max_num_frames %d", num_meas, meas_per_frame,max_num_frames);
#endif /* FEATURE_GPRS_GBTA */

#ifdef FEATURE_GPRS_GBTA
            pwr_meas_gas_id = (ptm_gas_id == GERAN_ACCESS_STRATUM_ID_UNDEFINED)? gas_id : ptm_gas_id;
#else
            pwr_meas_gas_id = gas_id;
#endif /* FEATURE_GPRS_GBTA */

      /* store prev req_pos to restart from same location when aborted */
      l1_sc_pwr_data_ptr->monscan.req_pos_prev = l1_sc_pwr_data_ptr->monscan.req_pos;

      /* Do a loop to setup num_meas monitors in the next frames.
       * num: counts number of measures to be done in this power measure command.
       * max_meas: counts upto num_meas (5 generally).
       * This is done in order to skip power measurement on certain cells.
       */
      for( num=0, max_meas=0; max_meas<num_meas; max_meas++ )
      {
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
         antenna_to_use[num]= 2;
#endif
         cell = l1_sc_pwr_data_ptr->monscan.req_cells[ l1_sc_pwr_data_ptr->monscan.req_pos ];
         ++l1_sc_pwr_data_ptr->monscan.req_pos;

         if ( l1_sc_pwr_data_ptr->monscan.req_pos == l1_sc_pwr_data_ptr->monscan.req_cell_count )
         {
            l1_sc_pwr_data_ptr->monscan.req_pos = 0;
         }

#ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS
         if(  (cell->in_blacklist)  /* cell is blacklisted */
              &&                    /* cur time < pwr msr disable time (note, don't use "<=" as condition will become wrong ) */
              ( gl1_ms_switch_l1_sc_globals_store(gas_id)->surround_tick < cell->pwr_msr_disabled_till_tick) )
         {
            /* don't do power monitor on blacklisted ncell for 1.5mins */
            cell->pwr_msr_enabled = FALSE;
         }
         else
         {
            /* normal case (good ncell): power monitor enabled only for non-blacklisted cells */
            cell->pwr_msr_enabled = TRUE;
#endif

            l1_sc_pwr_data_ptr->arfcn[num]   = cell->bs.arfcn;
            l1_sc_pwr_data_ptr->agc_ptr[num] = &cell->bs.agc;

#ifdef FEATURE_GSM_COEX_SW_CXM
            rx_power           = cell->bs.rx_power;
            // Check NEW CELL ADDED, Dont use floor power as it will lead to X tech blanking.
            if ( rx_power == SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16)
            {
               rx_power = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16_COEX;
            }
            if((cell->bs.arfcn.band != INVALID_BAND) && ((pwr_per_band[cell->bs.arfcn.band] > rx_power)))
            {
              pwr_per_band[cell->bs.arfcn.band] = rx_power;
            }
#endif /* FEATURE_GSM_COEX_SW_CXM */

            /* increase counter to indicate power measure needs to be done on this cell */
            num++;
#ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS
         } /* if(  (cell->in_blacklist) */
#endif
      } /* for( num=0, max_meas=0; max_meas<num_meas; max_meas++ ) */

      if (num > 0)
      {
	  #ifdef GL1_GMSK_MOD_DET
	   if(decStat[gas_id] == NULL){
	   
		 decStat[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(L1_SC_PWR_MAX_MEAS_PER_CALL+1));
		 }
#endif
         /* Reset handlers as in every attempt it will get re-instated below */
         /* gas_id is always - IDLE SUB gas_id , there is no harm in init to NULL ,
            in ALL modes of operation , as it will ne always PWR_MEAS_SM[SUB_ID][ZERO] 
            in NON GBTA */
         /* For GBTA - you want them as NULL , so callback and management done by Transfer SUB */
         (void)gl1_msg_cfg_pwr_meas(NULL, NULL, gas_id);

         /* FR54762 Inform MCPM about this scenario and check if we can go ahead with scheduling idle power MON */
          gl1_concman_config_params.type = CMGR_STEADY_START;
          gl1_concman_config_params.scenario = CMGR_GERAN_PSCAN;
          gl1_concman_config_params.update_scenario = 0;
          gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_ACQUISITION);
          gl1_concman_config_params.start_timestamp = 0;
          gl1_concman_config_params.duration_msec = 0;
          gl1_concman_config_params.flow_ctrl_time_msec = 0;
          status = gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);


         if (gl1_msg_cfg_pwr_meas(l1_sci_monscan_power_cb_idle, l1_sci_monscan_abort_cb_idle, pwr_meas_gas_id)&& (status == CMGR_ACCEPTED))
         {
            l1_sc_pwr_data_ptr->monscan.num_req = num;
#ifdef GL1_GMSK_MOD_DET
            l1_sc_pwr_data_ptr->pwr_meas_cb_param.decstat= decStat[gas_id] ;
#endif
#ifdef FEATURE_GSM_COEX_SW_CXM
            l1_coex_report_lowest_power_per_band(&pwr_per_band[0], gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

            gl1_msg_do_pwr_meas
            (
               l1_sc_pwr_data_ptr->arfcn,
               l1_sc_pwr_data_ptr->agc_ptr,
               num,
               meas_per_frame,
               max_num_frames,     /* Measure on max 4 frames */
               &l1_sc_pwr_data_ptr->pwr_meas_cb_param,
               pwr_meas_gas_id
            );

             /* Should set the gas_id of the transfer SUB  */
            gl1_set_pwr_meas_block_start(TRUE,pwr_meas_gas_id);
            l1_increment_idle_monitors_in_progress(gas_id);
            L1_LOG_VAR(l1_get_idle_monitors_in_progress(gas_id),L1_FNAME_L1_SC_PWR,L1_VAR_IDLE_MONITORS_IN_PROGRESS,gas_id);
            l1_sc_pwr_data_ptr->monscan.meas_in_progress = TRUE;

#ifdef FEATURE_GPRS_GBTA
#ifdef FEATURE_GSM_SCE_PWR_MEAS_DBG
            if(pwr_meas_gas_id != gas_id)
            {
               GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_GBTA_DO_PWR_MEAS, gas_id);
            }
#endif
#endif

#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_GSM_SCE_PWR_MEAS_DBG)
            /* store FN from idle gas_id (on GBTA) */
            if(l1_idle_data->do_pwr_meas_FN != L1_GPRS_INVALID_FN)
            {
               MSG_GERAN_ERROR_2_G("SCE: WARN: More than one pwr mon pending in Idle mode! Q_FN:%d, Cur_FN:%d",
                  l1_idle_data->do_pwr_meas_FN, GSTMR_GET_FN_GERAN(gas_id));
            }
            l1_idle_data->do_pwr_meas_FN = GSTMR_GET_FN_GERAN(gas_id);
#endif

#ifdef DEBUG_SCE_MSG
            #error code not present
#endif
         }
         else
         {
            MSG_GERAN_ERROR_0_G("Pwr meas config not succesfull, pwr meas not intialised or MCPM returned REJECT for Idle Power Meas");

            /* restore prev req_pos to restart from same location */
            l1_sc_pwr_data_ptr->monscan.req_pos = l1_sc_pwr_data_ptr->monscan.req_pos_prev;
         }
      }
   }
}

/*===========================================================================

FUNCTION l1_sc_monscan_get_num_idle_meas

DESCRIPTION
  The function gets the number of IDLE meas and IDLE meas per frame

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if this was a reduced monitor period

SIDE EFFECTS
  None
===========================================================================*/
static boolean l1_sc_monscan_get_num_idle_meas(uint8 *num_meas, uint8* meas_per_frame, gas_id_t gas_id)
{
   idle_data_T      *l1_idle_data_ptr = &l1_idle_data_store[gas_id];
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   boolean rc = FALSE; /* monitor reduction was applied during this ccch */

   uint8 reduced_mon_sched_period_index = l1_sc_pwr_data_ptr->reduced_mon_sched_period_index;

   if ( reduced_mon_sched_period_index >= DUTY_CYCLE_FOR_REDUCED_MON )
   {
     reduced_mon_sched_period_index = 0;
   }
 
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* reset duty cycle if missed page to avoid skipping too many power measurement. */
   if ( l1_idle_data_ptr->idle_rx_missed_page )
   {
     reduced_mon_sched_period_index = 0;
   }
#endif

   if (  ( l1_idle_data_ptr->campedon_cell_data.BS_PA_MFRMS == 2 )
      && ( l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state != NCELL_PCH_TIMER_EXPIRED )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_GPRS_GBTA
      /*skip pwr monitors in idle +idle +idle case only, not when transfer sub is scheduling pwr monitors for idle sub*/
      && !(IS_IDLE_EV_PWR_MON_ACTIVE(gas_id) && OTHER_GSM_CLIENT_IN_TRANSFER(gas_id))
#endif
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      )
   {
     /* When BS_PA_MFRMS is 2, to skip ncell monitors at alternative PCH decode */
     if (l1_idle_data_ptr->idle_power_opt > 0)
     {
       *num_meas = *meas_per_frame = skipping_mfrm_2_mon_sched_period_power_opt[ reduced_mon_sched_period_index ];
     }
     else
     {
       *num_meas = *meas_per_frame = skipping_mfrm_2_mon_sched_period[ reduced_mon_sched_period_index ];
     }

     rc = TRUE;
   }
   else
   if ( l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state == NCELL_PCH_TIMER_EXPIRED )
   {
     /* alternate each period between performing one then two ncell monitors per PCH decode */
     *num_meas = *meas_per_frame = reduced_mon_sched_period[ reduced_mon_sched_period_index ];

     rc = TRUE;
   }

   if ( *num_meas > L1_SC_PWR_MAX_MEAS_PER_CALL )
   {
      MSG_GERAN_ERROR_2_G("Requested %d measurements, doing %d.", *num_meas, L1_SC_PWR_MAX_MEAS_PER_CALL);
      *num_meas = L1_SC_PWR_MAX_MEAS_PER_CALL;
   }

   /* Only measure each cell once per paging block even
    * if the BA list length is less than the number of
    * measurements per paging block.
    */
   if (l1_sc_pwr_data_ptr->monscan.req_cell_count < *num_meas)
   {
      *num_meas = (uint8)l1_sc_pwr_data_ptr->monscan.req_cell_count;
   }

   return rc;

}

#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION l1_sc_check_idle_mon_allowed

DESCRIPTION
  Checks of Mon active and NCELL exists

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_sc_check_idle_mon_allowed(gas_id_t gas_id)
{
  boolean mon_active = FALSE;
  l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

  if  ( (   l1_sc_pwr_data_ptr->monscan.active ) &&
        ( ! l1_sc_pwr_data_ptr->monscan.meas_in_progress) &&
        (   l1_sc_pwr_data_ptr->monscan.req_cell_count != 0 )
      )
    {
      mon_active = TRUE;
    }

  return mon_active;
} /* l1_sc_check_mon_allowed */

/*===========================================================================

FUNCTION  l1_sc_get_monscan_active

DESCRIPTION
  This is called to know if sub is allowed to undergo power mon.

DEPENDENCIES
  None

RETURN VALUE
  Returns if monsacn is active.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_get_monscan_active( gas_id_t gas_id )
{
  l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
  return l1_sc_pwr_data_ptr->monscan.active;
}   
#endif /* FEATURE_GPRS_GBTA */

/*===========================================================================

FUNCTION l1_sci_monscan_do_ded

DESCRIPTION
  Do the next batch of measurements on the BA list.  Invoked after each
  idle or SACCH frame.

  meas_per_frame is only ever 1 for dedicated.
  
DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sci_monscan_do_ded( uint8 num_meas, uint8 meas_per_frame, boolean monscan_ded_tch_mode, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

#if defined (FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) || defined (FEATURE_GSM_DED_SCELL_MONITOR)
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   boolean                   monscan_ded_tch_full_26_mf  = ( monscan_ded_tch_mode && (num_meas==L1_SC_DED_MAX_MEAS_PER_MF) );
#endif

   uint16        num;
   cell_T*       cell;

#ifdef FEATURE_GSM_DED_SCELL_MONITOR
   uint16        sched_no_of_ded_scell_monitors = 0;
#endif

#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
   uint32        FnMod_102_104 = ADD_FN(gl1_get_FN(gas_id), 1) % ( (monscan_ded_tch_mode) ? 104 : 102 ); /* Position on the TCH 104 or SDCCH 102 MF */
#endif

   ASSERT_INT();
   MSG_GERAN_HIGH_2_G ( "l1_sci_monscan_do_ded num_meas=%d per frame=%d", num_meas, meas_per_frame);
   
   if ( num_meas > L1_SC_PWR_MAX_MEAS_PER_CALL )
   {
      MSG_GERAN_ERROR_2_G("Requested %d measurements, doing %d.", num_meas, L1_SC_PWR_MAX_MEAS_PER_CALL);
      num_meas = L1_SC_PWR_MAX_MEAS_PER_CALL;
   }

#ifdef FEATURE_GSM_DTM
   /* If in DTM there may be a requirement to take measurements on the scell's bcch */
   if ( (l1_sc_globals_ptr->mode == L1SCModeDediDTM) &&
        (gprs_l1_idle_data->gprs_campedon_cell_data.power_control.pc_meas_chan == FALSE) )
   {
     /* C Values must be calculated on BCCH of the scell */
     l1_sci_monscan_do_dtm(num_meas, meas_per_frame, gas_id);
   }
   else

#endif
   if(( l1_sc_pwr_data_ptr->monscan.active ) && (!l1_sc_pwr_data_ptr->monscan.meas_in_progress) )
   {
       
    uint16        num_plus3,max_scell_ded;
    const uint16 *pInsert_SCELL;  
    const uint8  *pInsert_PROBE;

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
     /*
     ** FR23109 update numbers for x3 adaptive only for TCH when peer tech is active in TDD
     */
     if((meas_per_frame == 1) && (monscan_ded_tch_mode == TRUE) && garb_is_peer_tech_active_tdd(gas_id) && (l1_sc_pwr_data_ptr->monscan.req_cell_count > 0))
     {
        l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt = num_meas;   /* save original, used as active indicator */
        /*
        ** increase x3 for apm                     
        */
        meas_per_frame  = PM_ADAPTIVE_MULTIPLIER;              /* x3 per frame */
        num_meas       *= PM_ADAPTIVE_MULTIPLIER;              /* x3 total     */  
        max_scell_ded   = (MAX_SCELL_MON_DED+2)*PM_ADAPTIVE_MULTIPLIER;
        
        pInsert_SCELL = &FRAME_FOR_SCELL_MONITOR_DED_APM[0];   /* increase scell monitor insert indices */
        pInsert_PROBE = &PROBE_SCELL_MONITOR_DED_APM[0];
        
        MSG_GERAN_HIGH_2_G ( "APM: is active num_meas=%d per frame=%d", num_meas, meas_per_frame);
     }
     else
#endif
     {
        l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt = 0;          /* disabled, adaptive not in use */
        pInsert_SCELL   = &FRAME_FOR_SCELL_MONITOR_DED[0];                 /* use original values */
        pInsert_PROBE   = &PROBE_SCELL_MONITOR_DED[0];
        max_scell_ded   = (MAX_SCELL_MON_DED+2);
     }

      /* Do a loop to setup num_meas monitors in the next frames. */

     if( num_meas > L1_SC_PWR_MAX_MEAS_PER_CALL )
     {   num_meas = L1_SC_PWR_MAX_MEAS_PER_CALL;}   /* KW */
      
      for( num=0; num<num_meas; num++ )
      {
       antenna_to_use[num]= 0;

#ifdef FEATURE_GSM_DED_SCELL_MONITOR
       /* 
       ** for adaptive will only match on the first of 3 with x3 values used in lut 
       */
       if ( (num == pInsert_SCELL[sched_no_of_ded_scell_monitors]) && (monscan_ded_tch_full_26_mf == TRUE) )
       {
         /* Place a scell monitor in at this location */
          num_plus3 = num + meas_per_frame;
          
          if( num_plus3 > L1_SC_PWR_MAX_MEAS_PER_CALL )
          {   num_plus3 = L1_SC_PWR_MAX_MEAS_PER_CALL;}   /* KW */

          /* add once for normal or x3 for adaptive active */
          for(  ; num < num_plus3 ; num++ )
          {
             l1_sc_pwr_data_ptr->arfcn[num]   =  l1_idle_data->campedon_cell_ARFCN;
             l1_sc_pwr_data_ptr->agc_ptr[num] = &l1_idle_data->campedon_cell_agc;
          }
          num--;  /* finishes at +1 */

          MSG_GERAN_MED_3_G("APM: adding scell monitor at idx=%d for arfcn=%d x3=%d ",num,l1_sc_pwr_data_ptr->arfcn[num].num, num_plus3);

          sched_no_of_ded_scell_monitors++;                                 /* adaptive only incremented 1 in 3 */
          if ( sched_no_of_ded_scell_monitors >= NO_OF_DED_SCELL_MON )      /* is 4 */
          {
             sched_no_of_ded_scell_monitors = 0;
          }
       }
       else
#endif
       {
         if( l1_sc_pwr_data_ptr->monscan.req_pos < (2 * BA_CELL_POOL_LEN) )
         {
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING

           uint16 prb_idx;

           if (( (num==pInsert_PROBE[0])||(num==pInsert_PROBE[1]) )
              && (gl1_hw_asdiv_type1.start_alg == TRUE )
              && (monscan_ded_tch_full_26_mf == TRUE))
           {

            MSG_GERAN_HIGH_2_G( "start asdiv type1 algorithm for ARFCN = %d monscan.req_pos = %d ",
                                                                        l1_idle_data->campedon_cell_ARFCN.num,
                                                                        l1_sc_pwr_data_ptr->monscan.req_pos);
            /* Place a scell mointor in at this location */

           if (num==pInsert_PROBE[0])
           { prb_idx=2; } else { prb_idx=1; }


            num_plus3 = num + meas_per_frame;

            if( num_plus3 > L1_SC_PWR_MAX_MEAS_PER_CALL )
            {   num_plus3 = L1_SC_PWR_MAX_MEAS_PER_CALL;}   /* KW */

           /* add once for normal or x3 for adaptive active */
            for(  ; num < num_plus3 ; num++ )
            {
               l1_sc_pwr_data_ptr->arfcn[num]   = l1_idle_data->campedon_cell_ARFCN;
               l1_sc_pwr_data_ptr->agc_ptr[num] = &l1_idle_data->campedon_cell_agc;
               antenna_to_use[num]              = prb_idx;
            }
            num--;  /* finishes at +1 */

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
            MSG_GERAN_HIGH_3_G( "APM:Probe monitor arfcn[num]=%d num=%d x3=%d", l1_sc_pwr_data_ptr->arfcn[num].num, num, num_plus3);
#endif
           }
           else
           {
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
           if( l1_sc_pwr_data_ptr->monscan.req_cell_count > 0  )
           {
#ifdef FEATURE_GSM_DED_SCELL_MONITOR 
             ba_list_empty = FALSE ; 
#endif /* #ifdef FEATURE_GSM_DED_SCELL_MONITOR  */          
    
#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
                       
            if(!l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt )
            {
               l1_sc_pwr_data_ptr->coex_protected_meas[num] =  l1_sc_pwr_data_ptr->is_coex_protected_monitor(FnMod_102_104 + num, gas_id);
            }
            else
            {
              /*
              ** for apm always use monscan.req_pos for all results 
              */ 
              l1_sc_pwr_data_ptr->coex_protected_meas[num] = TRUE;
            }
            if(!l1_sc_pwr_data_ptr->coex_protected_meas[num])
            {
               cell = l1_sc_pwr_data_ptr->monscan.req_cells[ l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected ];

               ++l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected;

               if ( l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected >= l1_sc_pwr_data_ptr->monscan.req_cell_count )
               {
                 l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected = 0;
               }
            }
            else
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS */
            {
              cell = l1_sc_pwr_data_ptr->monscan.req_cells[ l1_sc_pwr_data_ptr->monscan.req_pos ];

              ++l1_sc_pwr_data_ptr->monscan.req_pos;

              if ( l1_sc_pwr_data_ptr->monscan.req_pos >= l1_sc_pwr_data_ptr->monscan.req_cell_count )
              {
                l1_sc_pwr_data_ptr->monscan.req_pos = 0;
              }
            }

            num_plus3 = num + meas_per_frame;
            if( num_plus3 > L1_SC_PWR_MAX_MEAS_PER_CALL )
            {   num_plus3 = L1_SC_PWR_MAX_MEAS_PER_CALL;}   /* KW */
            
            /* add once for normal or x3 for adaptive active */
            for(  ; num < num_plus3 ; num++ )
            {
              l1_sc_pwr_data_ptr->arfcn[num] = cell->bs.arfcn;
              l1_sc_pwr_data_ptr->agc_ptr[num] = &cell->bs.agc;
             }
            num--;  /* finishes at +1 */
             
           } /* if req_cell_count > 0 */   
           else
           {
#ifdef FEATURE_GSM_DED_SCELL_MONITOR           
             if (monscan_ded_tch_full_26_mf == TRUE)
             {
             
              ba_list_empty = TRUE ;       
              if (num >= max_scell_ded )
              {
                break;
              } 
               
              num_plus3 = num + meas_per_frame;
              /* add once for normal or x3 for adaptive active */
              
              if( num_plus3 > L1_SC_PWR_MAX_MEAS_PER_CALL )
              {   num_plus3 = L1_SC_PWR_MAX_MEAS_PER_CALL;}   /* KW */
              
              for(  ; num < num_plus3 ; num++ )
              {
                 /* BA list empty, stuff SCELL ARFCN instead so monitor on every frame */
                 l1_sc_pwr_data_ptr->arfcn[num].num   = DUMMY_ARFCN_FOR_SC_MEAS;
                 l1_sc_pwr_data_ptr->agc_ptr[num] = &l1_idle_data->campedon_cell_agc;
              }
              num--;  /* finishes at +1 */

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
                 MSG_GERAN_DEBUG_OPT_3_G("APM:adding stuffed scell monitor %d at %d x3=%d",l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num].num,num, num_plus3);
#endif
             }           
#else
            break;
#endif /* FEATURE_GSM_DED_SCELL_MONITOR */            
           }
            
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
           }
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
         }
         else
         {
             MSG_GERAN_ERROR_3_G(" invalid req_pos %d req_cell_count %d ",
                       l1_sc_pwr_data_ptr->monscan.req_pos,
                       l1_sc_pwr_data_ptr->monscan.req_cell_count,
                       0 );

             break;
         }
      }
     }  /* for num_meas */

      /* Update scell and ncell arfcn for timeline reporting */
      garb_update_scell_ncell_arfcns(l1_sc_pwr_data_ptr->arfcn , num, meas_per_frame, gas_id);

#if defined (FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) || defined (FEATURE_GSM_DED_SCELL_MONITOR)
      if (((monscan_ded_tch_full_26_mf == TRUE) ||(l1_sc_pwr_data_ptr->monscan.req_cell_count > 0) )&&( num>0))
#else
      if ((l1_sc_pwr_data_ptr->monscan.req_cell_count > 0)&&( num>0))
#endif
      {
         boolean cfg_success;
#ifdef GL1_GMSK_MOD_DET
		 if(decStat[gas_id] == NULL){
		 decStat[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(L1_SC_PWR_MAX_MEAS_PER_CALL+1));
		 }
#endif
         /* Register the callback for message layer every time */
         /* pscan may have changed it.                         */
         if (monscan_ded_tch_mode == TRUE)
         {
           cfg_success = gl1_msg_cfg_pwr_meas(l1_sci_monscan_power_cb_ded_tch, l1_sci_monscan_abort_cb_ded,gas_id);
         }
         else
         {
           cfg_success = gl1_msg_cfg_pwr_meas(l1_sci_monscan_power_cb_ded_sdcch, l1_sci_monscan_abort_cb_ded,gas_id);
         }
         if (cfg_success)
         {
            l1_sc_pwr_data_ptr->monscan.num_req = num;
#ifdef GL1_GMSK_MOD_DET
			l1_sc_pwr_data_ptr->pwr_meas_cb_param.decstat = decStat[gas_id];
#endif
            gl1_msg_do_pwr_meas( l1_sc_pwr_data_ptr->arfcn,
                                 l1_sc_pwr_data_ptr->agc_ptr,
                                 num,  /* number of arfcns */
                                 meas_per_frame,
                                 num,  /* schedule for at most "num" frames */
                                &l1_sc_pwr_data_ptr->pwr_meas_cb_param,
                                 gas_id
                               );

            l1_sc_pwr_data_ptr->monscan.meas_in_progress = TRUE;
            gl1_set_pwr_meas_block_start(TRUE,gas_id);

#ifdef DEBUG_SCE_MSG
            #error code not present
#endif
         }
         else
         {
            MSG_GERAN_ERROR_0_G("Pwr meas config not succesfull, pwr meas not intialised");
         }
      }
      else
      {
        MSG_GERAN_ERROR_0_G(" l1_sci_monscan_do_ded num 0 ");
      }
   }
}

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION l1_sci_monscan_do_dtm

DESCRIPTION
  Do the next batch of measurements on the BA list. Invoked after each
  idle or SACCH frame. This function will include extra scell bcch measurements
  required to perform the C value calculations. PC_MEAN_CHAN == FALSE for this
  function to be executed.
  The spread of measurements is fixed using predefined positions of frames
  8, 16, 23 throughout the multiframe as defined within the array
  FRAME_FOR_SCELL_C_VALUE_MONITOR.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_sci_monscan_do_dtm( uint8 num_meas, uint8 meas_per_frame, gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];

   uint16        num;
   uint16        sched_no_of_scell_bcch_monitors = 0;
   cell_T*       cell;

   MSG_GERAN_MED_0_G ( " l1_sci_monscan_do_dtm ");

   if ( ( l1_sc_pwr_data_ptr->monscan.active ) && (!l1_sc_pwr_data_ptr->monscan.meas_in_progress) )
   {
     /************************************************************/
     /* Do a loop to setup num_meas monitors in the next frames. */
     /* The scell will need to be evenly distrubuted throughout  */
     /* the list to calculate the C value                        */
     /************************************************************/
     for( num=0; (num<num_meas) ; num++ )
     {
      antenna_to_use[num]= 0;
 #ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
           if (( (num==0)||(num==1) )&&gl1_hw_asdiv_type1.start_alg)
           {

             MSG_GERAN_HIGH_2_G ( "  Probe monitor ARFCN = %d monscan.req_pos = %d ",l1_idle_data->campedon_cell_ARFCN.num, l1_sc_pwr_data_ptr->monscan.req_pos);
             /* Place a scell mointor in at this location */
             l1_sc_pwr_data_ptr-> arfcn[num] = l1_idle_data->campedon_cell_ARFCN;
             l1_sc_pwr_data_ptr-> agc_ptr[num] = &l1_idle_data->campedon_cell_agc;

            if (num==0)
            {
             antenna_to_use[num]= 2;
            }

            if (num==1)
            {
             antenna_to_use[num]= 1;
            }

             MSG_GERAN_MED_1_G ( "  arfcn[num] index = %d ",num);
           }
           else

#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
       if ( num == FRAME_FOR_SCELL_C_VALUE_MONITOR[sched_no_of_scell_bcch_monitors] )
       {

          if( l1_sc_pwr_data_ptr->monscan.req_cell_count <= 0  )
          {
          MSG_GERAN_HIGH_1_G ( " num of power meas  = %d ",num);
          break;
          }
         /* Place a scell mointor in at this location */
         l1_sc_pwr_data_ptr->arfcn[num] = l1_idle_data->campedon_cell_ARFCN;
         l1_sc_pwr_data_ptr->agc_ptr[num] = &l1_idle_data->campedon_cell_agc;
         sched_no_of_scell_bcch_monitors++;

         /* We've run off the end of available frames so wrap around for next set */
         if ( sched_no_of_scell_bcch_monitors >= NO_OF_C_VALUE_MEAS )
         {
           sched_no_of_scell_bcch_monitors = 0;
         }
       }
       else
       {
          if( l1_sc_pwr_data_ptr->monscan.req_cell_count <= 0  )
          {
          MSG_GERAN_HIGH_1_G( " num of power meas  = %d ",num);
          break;
          }
         /* Place a ncell monitor in at this location */
         cell = l1_sc_pwr_data_ptr->monscan.req_cells[ l1_sc_pwr_data_ptr->monscan.req_pos ];
         ++l1_sc_pwr_data_ptr->monscan.req_pos;
         if ( l1_sc_pwr_data_ptr->monscan.req_pos == l1_sc_pwr_data_ptr->monscan.req_cell_count )
         {
           l1_sc_pwr_data_ptr->monscan.req_pos = 0;
         }
         l1_sc_pwr_data_ptr->arfcn[num] = cell->bs.arfcn;
         l1_sc_pwr_data_ptr->agc_ptr[num] = &cell->bs.agc;
       }
     }

     /* Update scell and ncell arfcn for timeline reporting */
     //garb_update_scell_ncell_arfcns(l1_sc_pwr_data_ptr->arfcn,num,1,gas_id);

     if (num > 0)
     {
#ifdef GL1_GMSK_MOD_DET
	   if (decStat[gas_id] == NULL){
         decStat[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(L1_SC_PWR_MAX_MEAS_PER_CALL+1));
		 }
#endif
       /* Register the callback for message layer every time */
       /* pscan may have changed it.                         */
       if (gl1_msg_cfg_pwr_meas(l1_sci_monscan_power_cb_dtm,
                                  l1_sci_monscan_abort_cb_ded,gas_id))
       {
          l1_sc_pwr_data_ptr->monscan.num_req = num;
#ifdef GL1_GMSK_MOD_DET
		  l1_sc_pwr_data_ptr->pwr_meas_cb_param.decstat = decStat[gas_id];
#endif
          gl1_msg_do_pwr_meas
          (
             l1_sc_pwr_data_ptr->arfcn,
             l1_sc_pwr_data_ptr->agc_ptr,
             num,  /* number of arfcns */
             meas_per_frame,
             num,  /* schedule for at most "num" frames */
             &l1_sc_pwr_data_ptr->pwr_meas_cb_param,
             gas_id
          );

          l1_sc_pwr_data_ptr->monscan.meas_in_progress = TRUE;
          gl1_set_pwr_meas_block_start(TRUE,gas_id);

#ifdef DEBUG_SCE_MSG
          #error code not present
#endif
       }
       else
       {
          MSG_GERAN_ERROR_0_G("Pwr meas config not succesfull, pwr meas not intialised");
       }
     }
   }
}
#endif/*FEATURE_GSM_DTM*/
/*===========================================================================

FUNCTION l1_sci_monscan_check_monitors_remaining

DESCRIPTION
  Called by l1_sci_monscan_power_cb to check that all arfcns have been scheduled.

DEPENDENCIES
  IN - num_monitors - Number of monitors in the power measure report.

===========================================================================*/
static void l1_sci_monscan_check_monitors_remaining(uint32 num_monitors, gas_id_t gas_id)
{
   int32   monitors_remaining;
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   /* Check that all arfcns have been scheduled, correct req_pos if not */
   monitors_remaining = l1_sc_pwr_data_ptr->monscan.num_req - num_monitors;
   if (monitors_remaining > 0)
   {
      if( l1_sc_pwr_data_ptr->monscan.req_cell_count > 0 )
      {
        monitors_remaining = monitors_remaining % l1_sc_pwr_data_ptr->monscan.req_cell_count;
      }

      if ( l1_sc_pwr_data_ptr->monscan.req_pos >= monitors_remaining)
      {
         l1_sc_pwr_data_ptr->monscan.req_pos -= monitors_remaining;
      }
      else
      {
         if( monitors_remaining <= l1_sc_pwr_data_ptr->monscan.req_cell_count )
         {
           uint32  diff = l1_sc_pwr_data_ptr->monscan.req_cell_count - monitors_remaining;

           l1_sc_pwr_data_ptr->monscan.req_pos += diff;
         }
         else
         {
           MSG_GERAN_ERROR_3_G(" monitors_remaining %d req_cell_count %d ",
                     monitors_remaining,
                     l1_sc_pwr_data_ptr->monscan.req_cell_count,
                     0 );
         }
      }

      if( l1_sc_pwr_data_ptr->monscan.req_pos >= (2 * BA_CELL_POOL_LEN) )
      {
        MSG_GERAN_ERROR_3_G(" invalid req_pos %d reset to %d",
                  l1_sc_pwr_data_ptr->monscan.req_pos,
                  ((2 * BA_CELL_POOL_LEN) - 1),
                  0 );

        l1_sc_pwr_data_ptr->monscan.req_pos = ((2 * BA_CELL_POOL_LEN) - 1);
      }
   }
}

/*===========================================================================

FUNCTION l1_sci_monscan_ded_power_results_update

DESCRIPTION
  Called by l1_sci_monscan_power_cb to update the  power results.

===========================================================================*/
static void l1_sci_monscan_ded_power_results_update(gl1_msg_pwr_meas_result_type *rpt, uint32 i, gas_id_t gas_id)
{
   uint8    temp_monscan_cnf_pos;
   l1_sc_bs_T       *bs;
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
   uint8 *cnf_pos_p = l1_sc_pwr_data_ptr->coex_protected_meas[i] ? &l1_sc_pwr_data_ptr->monscan.cnf_pos : &l1_sc_pwr_data_ptr->monscan.cnf_pos_coex_unprotected;
#else
   uint8 *cnf_pos_p = &l1_sc_pwr_data_ptr->monscan.cnf_pos;
#endif

   if(l1_sc_pwr_data_ptr->monscan.req_cell_count == 0)
   {
      return;
   }

   bs = &(l1_sc_pwr_data_ptr->monscan.req_cells[ *cnf_pos_p ]->bs);
   if (bs == NULL)
   {
     MSG_GERAN_ERROR_0_G("Null bs pointer, ignore mon results");
     return;
   }

   /*
    Match up the results with the ARFCN's to make sure they don't
    get mixed up for any reason
   */
   if ( ARFCNS_EQUAL(rpt->arfcns[i],bs->arfcn) )
   {
     /* ignore yielded measuremnt results */
     if( (rpt->yielded != NULL) && (!rpt->yielded[i]) )
     {
       bs->rx_power_sum += rpt->pwr_dBm_x16[i];
       bs->rx_power_count++;
     }
     /* inc monscan.cnf_pos if ncell pwr correctly added. */
     (*cnf_pos_p)++;
     if ( *cnf_pos_p >= l1_sc_pwr_data_ptr->monscan.req_cell_count )
     {
       *cnf_pos_p = 0;
     }
   }
   else
   {
     MSG_GERAN_ERROR_2_G("Unexpected ncell monitor exp:%d act:%d",bs->arfcn.num,rpt->arfcns[i].num);
     /* If one report lost, then re-align pointers */
     temp_monscan_cnf_pos = *cnf_pos_p + 1;
     if (temp_monscan_cnf_pos >= l1_sc_pwr_data_ptr->monscan.req_cell_count)
     {
        temp_monscan_cnf_pos = 0;
     }
     bs = &(l1_sc_pwr_data_ptr->monscan.req_cells[temp_monscan_cnf_pos]->bs);
     if (bs == NULL)
     {
       MSG_GERAN_ERROR_0_G("Null bs pointer, ignore mon results");
       return;
     }
     if ( ARFCNS_EQUAL(rpt->arfcns[i],bs->arfcn) )
     {
       /* ignore yielded measuremnt results */
       if( (rpt->yielded != NULL) && !rpt->yielded[i])
       {
         bs->rx_power_sum  += rpt->pwr_dBm_x16[i];
         bs->rx_power_count++;
       }

       *cnf_pos_p = temp_monscan_cnf_pos + 1;
       if ( *cnf_pos_p >= l1_sc_pwr_data_ptr->monscan.req_cell_count )
       {
         *cnf_pos_p = 0;
       }
     }
   }
} /* l1_sci_monscan_ded_power_results_update */


/*===========================================================================

FUNCTION l1_validate_bs_ptr

DESCRIPTION
     returns valid bs to proceed

===========================================================================*/
#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED

static l1_sc_bs_T* l1_validate_bs_ptr( uint32 bs_pos, gas_id_t gas_id)
{
   
   l1_sc_bs_T       *bs;
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   if(l1_sc_pwr_data_ptr->monscan.req_cell_count == 0)
   {
      return NULL;
   }

   bs = &(l1_sc_pwr_data_ptr->monscan.req_cells[bs_pos]->bs);

   return bs;

}

/*===========================================================================

FUNCTION l1_get_next_ded_adaptive_result

DESCRIPTION
     Search the next three results for the 1st that has not yielded

===========================================================================*/

static uint32 l1_get_next_ded_adaptive_result(gl1_msg_pwr_meas_result_type *rpt, uint32 apm_idx, ARFCN_T cell_arfcn, gas_id_t gas_id)
{
   uint32   metrics_index = 0;
   uint32   yield_cnt     = 0;
   uint32   loop_cnt;
  
   loop_cnt = apm_idx + PM_ADAPTIVE_MULTIPLIER;            /* +3 */

   /*
   ** find the next arfcn that matches and has not yielded
   */
   for( ; apm_idx < loop_cnt; apm_idx++)           /* check next two location so finishes on 3rd index */
   {
     if( ARFCNS_EQUAL(rpt->arfcns[apm_idx], cell_arfcn) )
     {
       if( (rpt->yielded != NULL) && (!rpt->yielded[apm_idx]) )
       {
           apm_metrics_log[metrics_index]++;     /* count occurrences of 1st non-yield 0,1,2 */
           return apm_idx;                       /* first index that did not yield */
       }
       else
       {
         apm_metrics_log[3]++;                   /* total number of yields */
         yield_cnt++;
       }
       metrics_index++;
     }
     else
     {
        apm_metrics_log[4]++;                    /* total number of unequal afrcns */
        MSG_GERAN_ERROR_3_G("APM: Unequal arfcn's for index=%d rpt->arfcn=%d bs->arfcn=%d", apm_idx, rpt->arfcns[apm_idx].num, cell_arfcn.num );
     }
   }

   apm_idx--;                                    /* point to last of the 3 results */

   if(yield_cnt > 2)
   {
      MSG_GERAN_HIGH_3_G("APM: all results yielded for index=%d rpt->arfcn %d cell arfcn=%d ", apm_idx, rpt->arfcns[apm_idx].num , cell_arfcn.num);
}
   
   return apm_idx;                              /* if all 3 yielded point to last expected result */
} /* l1_get_next_ded_adaptive_result */


/*===========================================================================

FUNCTION  l1_sc_monscan_adaptive_active

DESCRIPTION
  api for garb_intf_arbitrate_monitors() to determine if current power monitors are running in adaptive mode.

DEPENDENCIES
  None

RETURN VALUE
  TRUE of monscan is not running currently

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_monscan_adaptive_active( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   if(!l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt )
   {
      return FALSE;
   }
   else
   {
      return TRUE;
   }
}
#endif  /* FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED */

/*===========================================================================

FUNCTION ll1_sci_monscan_ded_period_done

DESCRIPTION
  Called by l1_sci_monscan_power_cb for period complete.

===========================================================================*/
static void l1_sci_monscan_ded_period_done(gas_id_t gas_id)
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   if ( l1_sc_pwr_data_ptr->monscan.next_period )
   {
      l1_sc_pwr_data_ptr->monscan.next_period = FALSE;
       l1_sci_monscan_done_ded(gas_id);

      /* Call callback to help synchronize serving
       * cell report to RR with ncell report.
       */
      if (l1_sc_pwr_data_ptr->monscan.next_period_ded_cb != NULL)
      {
          l1_sc_pwr_data_ptr->monscan.next_period_ded_cb(gas_id); /*lint !e746 no prototype */
      }
   }
   l1_sc_pwr_data_ptr->monscan.meas_in_progress = FALSE;
}

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION l1_sci_monscan_power_cb_dtm

DESCRIPTION
  Called on completion of a power measurement initiated by
  l1_sci_monscan_do_dtm(). The scell measurements that have been taken
  for the C value calculation are all stripped out of the results.

DEPENDENCIES
  IN - rpt - Power measure report generated by the message layer.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_sci_monscan_power_cb_dtm( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
   uint32   num_monitors;
   uint32   i;
   uint16   sched_no_of_scell_bcch_monitors = 0;
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
   int extra_meas_count = 0;
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
   ASSERT_INT();

   num_monitors = rpt->num_pwr_meas;

   l1_sci_monscan_check_monitors_remaining (num_monitors, gas_id);

   if ( l1_sc_pwr_data_ptr->monscan.active )
   {
      /* Check the ID, if it doesn't match then don't process
       * the results, but still call callback.
       */
     if ( l1_sc_pwr_data_ptr->monscan.req_cells_id == l1_sc_balist_get_old_id(gas_id) )
     {

       /*************************************************************/
       /* Go through the results stripping out extra scell monitors */
       /* that have been inserted in order to calculate the C value */
       /* Match all results using the ARFCN check before using them */
       /*************************************************************/
       for(i=0;i<num_monitors;i++)
       {

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
        if (ARFCNS_EQUAL(rpt->arfcns[i], l1_idle_data->campedon_cell_ARFCN)&&(extra_meas_count<2)&&gl1_hw_asdiv_type1.start_alg)
         {
            MSG_GERAN_HIGH_2_G( "Probe burst %d scell power = %d dBmx16", extra_meas_count,rpt->pwr_dBm_x16[i]);
            extra_meas_count++;
         }
         else
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

         if ( (i == FRAME_FOR_SCELL_C_VALUE_MONITOR[sched_no_of_scell_bcch_monitors]) &&
              (ARFCNS_EQUAL(rpt->arfcns[i],l1_idle_data->campedon_cell_ARFCN))
            )
         {
           /* ignore yielded measuremnt results */
           if( (rpt->yielded != NULL) && !rpt->yielded[i])
           {
             /*  If PC_MEAS_CHAN is not set update power control parameters  */
             gprs_serv_cell_meas_ptr->power_cntrl_samples += rpt->pwr_dBm_x16[i];
             gprs_serv_cell_meas_ptr->power_cntrl_count++;
           }
           sched_no_of_scell_bcch_monitors++;

           /* We've run off the end of available frames so wrap around for next set */
           if ( sched_no_of_scell_bcch_monitors >= NO_OF_C_VALUE_MEAS )
           {
             sched_no_of_scell_bcch_monitors = 0;
           }
         }
         else
         {
            /* The yield flag is checked inside this function */
            l1_sci_monscan_ded_power_results_update(rpt, i, gas_id);
         }
       }//end of for loop serve
     }
     l1_sci_monscan_ded_period_done(gas_id);
   }

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}  /* l1_sci_monscan_power_cb_dtm */

#endif/*FEATURE_GSM_DTM*/
/*===========================================================================

FUNCTION l1_sci_monscan_power_cb_ded_tch

DESCRIPTION
  Called on completion of a power measurement initiated by
  l1_sci_monscan_do_ded().

DEPENDENCIES
  IN - rpt - Power measure report generated by the message layer.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_sci_monscan_power_cb_ded_tch( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
#if defined(FEATURE_GSM_TX_DIV_ANTNA_SWITCHING) || defined(FEATURE_GSM_DED_SCELL_MONITOR)
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/
   uint32   num_monitors;
   uint32   i;
#ifdef FEATURE_GSM_DED_SCELL_MONITOR
   uint16   sched_no_of_ded_scell_monitors = 0;
#endif
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
   int extra_meas_count = 0  ;
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/

   uint32 probe_cnt; /* or 2 or 6 */
   uint32 apm_idx;
#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
   num_monitors = rpt->num_pwr_meas;

   l1_sci_monscan_check_monitors_remaining (num_monitors, gas_id);

   if ( l1_sc_pwr_data_ptr->monscan.active )
   {
      /* Check the ID, if it doesn't match then don't process
       * the results, but still call callback.
       */
      if ( l1_sc_pwr_data_ptr->monscan.req_cells_id == l1_sc_balist_get_old_id(gas_id) )
      {
       const uint16 *pInsert_SCELL;  

        /* adaptive active ? */  
        if(!l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt)
        {   /* 
            ** not active use original valuues 
            */
            pInsert_SCELL  = &FRAME_FOR_SCELL_MONITOR_DED[0];
            probe_cnt      = TX_DIV_PROBE_CNT;
        }
#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
        else
        {
            pInsert_SCELL  = &FRAME_FOR_SCELL_MONITOR_DED_APM[0];
            probe_cnt      = TX_DIV_APM_PROBE_CNT;
        }
#endif
        for(i=0; i < num_monitors; i++)   /* for adaptive this is x3 results */
        {
           apm_idx = i;  /* copy current */

#ifdef FEATURE_GSM_DED_SCELL_MONITOR
          if ( (i == pInsert_SCELL[sched_no_of_ded_scell_monitors]) &&
               (ARFCNS_EQUAL(rpt->arfcns[i],l1_idle_data->campedon_cell_ARFCN))
            )
          {
#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
             /* get 1 of the next 3 results */
             if(l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt)
             {  
                i = l1_get_next_ded_adaptive_result(rpt, i, l1_idle_data->campedon_cell_ARFCN , gas_id);   /* this picks 1 from next 3 results */
                apm_idx += PM_ADAPTIVE_INCREMENT;                                                          /* adds 2 each iteration, for loop ++ adds further +1 */
             }
#endif                         
             MSG_GERAN_HIGH_3_G("Ded scell monitor: frame %d, arfcn %d, level %d dBm_x16 ",i, rpt->arfcns[i].num, rpt->pwr_dBm_x16[i]);
             sched_no_of_ded_scell_monitors++;
             if ( sched_no_of_ded_scell_monitors >= NO_OF_DED_SCELL_MON )
             {
               sched_no_of_ded_scell_monitors = 0;
             }
          }
          else if (rpt->arfcns[i].num == DUMMY_ARFCN_FOR_SC_MEAS)
          {
#ifdef DEBUG_FEATURE_GSM_DED_SCELL_MONITOR          
            MSG_GERAN_HIGH_3_G("DUMMY_ARFCN_FOR_SC_MEAS cb ",0, 0, 0);
#endif /*DEBUG_FEATURE_GSM_DED_SCELL_MONITOR */            
          }
          else
#endif
          {
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING

           if (ARFCNS_EQUAL(rpt->arfcns[i], l1_idle_data->campedon_cell_ARFCN)
              && (i< probe_cnt)   
              && gl1_hw_asdiv_type1.start_alg)
           {
             if(probe_cnt < TX_DIV_PROBE_COUNT)
             {
                MSG_GERAN_HIGH_2_G( "Probe burst %d scell power = %d dBmx16", extra_meas_count,rpt->pwr_dBm_x16[i]);
                extra_meas_count++;
             }
           }
           else
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
           {
#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
             /* 
             ** find next "i" that has not yieled so as to pass into l1_sci_monscan_ded_power_results_update() only once
             */
             if(l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt)
             {  
               l1_sc_bs_T  *bs = l1_validate_bs_ptr(l1_sc_pwr_data_ptr->monscan.cnf_pos, gas_id); 
               if(bs)
               {
                  i = l1_get_next_ded_adaptive_result(rpt, i, bs->arfcn, gas_id);   /* this picks 1 from next 3 results */
               }
               apm_idx += PM_ADAPTIVE_INCREMENT;    /* adds 2 each iteration, for loop ++ adds further +1 */
             }
#endif
             /* The yield flag is checked inside this function */
             l1_sci_monscan_ded_power_results_update(rpt, i, gas_id);
           }
          }
          i = apm_idx;   /* next "i" is the same "i" value or i+2 */
        }
      }
      else
      {
         MSG_GERAN_MED_2_G("SCE: l1_sci_monscan_power_cb_ded_tch result id mismatch %d %d", l1_sc_pwr_data_ptr->monscan.req_cells_id, l1_sc_balist_get_old_id(gas_id) );
      }
      
      l1_sci_monscan_ded_period_done(gas_id);
   }

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}
/*===========================================================================

FUNCTION l1_sci_monscan_power_cb_ded_sdcch

DESCRIPTION
  Called on completion of a power measurement initiated by
  l1_sci_monscan_do_ded().

DEPENDENCIES
  IN - rpt - Power measure report generated by the message layer.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void l1_sci_monscan_power_cb_ded_sdcch( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   uint32   num_monitors;
   uint32   i;


#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
   int extra_meas_count = 0;
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

   num_monitors = rpt->num_pwr_meas;

   l1_sci_monscan_check_monitors_remaining (num_monitors, gas_id);

   if ( l1_sc_pwr_data_ptr->monscan.active )
   {
      /* Check the ID, if it doesn't match then don't process
       * the results, but still call callback.
       */
      if ( l1_sc_pwr_data_ptr->monscan.req_cells_id == l1_sc_balist_get_old_id(gas_id) )
      {
         for(i=0; i < num_monitors; i++)
         {


#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
            if (ARFCNS_EQUAL(rpt->arfcns[i], l1_idle_data->campedon_cell_ARFCN)&&(extra_meas_count<2))
            {
            MSG_GERAN_HIGH_3_G( "  scell power from cb ........ = %d ", rpt->pwr_dBm_x16[i],0,0);
            extra_meas_count++;
            }
            else

#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
            {
             /* The yield flag is checked inside this function */
             l1_sci_monscan_ded_power_results_update(rpt, i, gas_id);
            }

         }
      }
      l1_sci_monscan_ded_period_done(gas_id);
   }

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}  /* l1_sci_monscan_power_cb_ded_sdcch */

/*===========================================================================

FUNCTION  l1_sci_monscan_power_cb_idle

DESCRIPTION
  Called on completion of a power measurement initiated by
  l1_sci_monscan_do_idle().

DEPENDENCIES
  IN - rpt - Power measure report generated by the message layer.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_power_cb_idle( gl1_msg_pwr_meas_result_type *rpt, gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr;

   uint32                    num_monitors, monitors_remaining;
   uint32                    i;
   boolean                   wrapped=FALSE;
   l1_sc_bs_T               *bs;
   cell_T                   *cell;
   boolean                   req_conf_mismatch = FALSE;
#ifdef FEATURE_GPRS_GBTA
   gas_id_t                  idle_gas_id;
   gas_id_t                  other_g_gas_id = OTHER_G_SUB(gas_id);
   boolean                   gas_id_flipped = FALSE;
#endif /*FEATURE_GPRS_GBTA*/
   volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   ASSERT_INT();


#ifdef FEATURE_GPRS_GBTA
   /* Swap gas_id as pwr mon scheduled by ptm sub */
   if( gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED, gas_id,&idle_gas_id))
   {
     MSG_GERAN_HIGH_2_G("GBTA cb_idle gas_id will flip from %d to gas_id %d",gas_id,idle_gas_id);

     /* Flip gas_id as scheduled in other sub gas_id */
     gas_id = idle_gas_id;
     gas_id_flipped = TRUE;
   }
   else if (l1_tsk_buffer[gas_id].l1_state == L1_TRANSFER_MODE)
   {
     if ( other_g_gas_id != GERAN_ACCESS_STRATUM_ID_UNDEFINED )//KW issue
     {
       l1_sc_globals_T   *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(other_g_gas_id);
	   idle_data_T* l1_idle_data = get_ptr_to_idle_data(other_g_gas_id);
	   l1_sc_pwr_data_ptr = &l1_sc_pwr_data[other_g_gas_id];
       l1_decrement_idle_monitors_in_progress(other_g_gas_id);
       L1_LOG_VAR(l1_get_idle_monitors_in_progress(other_g_gas_id),L1_FNAME_L1_SC_PWR,L1_VAR_IDLE_MONITORS_IN_PROGRESS,other_g_gas_id);
       l1_sc_pwr_data_ptr->monscan.meas_in_progress = FALSE;
       l1_sc_globals_ptr->monscan_in_progress = FALSE;
       L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,other_g_gas_id);
       MSG_GERAN_HIGH_1_G("abonding meas_in_progress & monscan_in_progress gas_id:%d",other_g_gas_id);     
       l1_idle_data->do_pwr_meas_FN = L1_GPRS_INVALID_FN;
       /* Reset when Pwr mon done */
       grm_notify_event(GBTA_EV_NONE,FALSE,other_g_gas_id);
       return;
     }
   }
   /* Reset when Pwr mon done */
   grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);
#endif /* FEATURE_GPRS_GBTA */

   GL1_ISR_SAVE_LOCK(gas_id);

   gas_id             = check_gas_id(gas_id);
   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   num_monitors = rpt->num_pwr_meas;
   /* Check that all arfcns have been scheduled, correct req_pos if not */
   monitors_remaining = l1_sc_pwr_data_ptr->monscan.num_req - num_monitors;
   if (monitors_remaining > 0)
   {
      monitors_remaining = monitors_remaining % l1_sc_pwr_data_ptr->monscan.req_cell_count;
      if ( l1_sc_pwr_data_ptr->monscan.req_pos >= monitors_remaining)
      {
         l1_sc_pwr_data_ptr->monscan.req_pos -= monitors_remaining;
      }
      else
      {
         l1_sc_pwr_data_ptr->monscan.req_pos += (l1_sc_pwr_data_ptr->monscan.req_cell_count - monitors_remaining);
      }
   }

   if ( l1_sc_pwr_data_ptr->monscan.active )
   {
      /* Check the ID, if it doesn't match then don't process
       * the results, but still call callback.
       */
      if ( l1_sc_pwr_data_ptr->monscan.req_cells_id == l1_sc_balist_get_old_id(gas_id) )
      {

         i=0; /* idx for number of pwr msr results (normally 5 results available per pwr msr) */
         while(i < num_monitors) /* go through all pwr msr results */
         {
            cell = l1_sc_pwr_data_ptr->monscan.req_cells[ l1_sc_pwr_data_ptr->monscan.cnf_pos ];

#ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS
            if( cell->pwr_msr_enabled )
            {
               /* set cell->pwr_msr_enabled back to TRUE, if other code uses this. 
                * not reqd for now as its only done from l1_sci_monscan_do_idle()
                *
                * cell->pwr_msr_enabled = TRUE;
                *
                */
#endif

               /* update running averages */
               if( (rpt->yielded != NULL) && !rpt->yielded[i])
               {
                  bs      = &(cell->bs);
                  if (ARFCNS_EQUAL(bs->arfcn, rpt->arfcns[i]) )
                  {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                     /* if the arfcn is from a denied band then the skip flag is set prior to measuringand GFW 
                      * will report 0dB for RSSI meas .This will impact RSSI averaging for this ARFCN so do 
                      * not update the running average
                      */
                     if (grm_is_band_registered(rpt->arfcns[i].band, l1_tskisr_blk->client_id, gas_id))
                     {
#endif
                        /* all good: power measured for the right arfcn */
                        l1_running_avg_add_meas( &(bs->ra), 
                                                  rpt->pwr_dBm_x16[i],
                                                  GL1_INVALID_POWER_LEVEL,
                                                  PRX_MEAS,
                                                  gas_id );
                        bs->meas_state = L1SCMeasNew;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                     }
                     else
                     {
                        MSG_GERAN_MED_2_G(GL1_DRDS_DBG_HDR"l1_sci_monscan_power_cb_idle:Skipped arfcn %d pwr_dBm_x16 %d do not update running avg!",
                        rpt->arfcns[i].num, rpt->pwr_dBm_x16[i] );
                     } /* else - if (!grm_is_band_registered(rpt->arfcns[i].band, gas_id)) */
#endif
                  } /* if (ARFCNS_EQUAL(bs->arfcn, rpt->arfcns[i]) ) */
                  else
                  {
                     /* serious error: power measured on arfcn x, but storing results on arfcn y */
                     if( req_conf_mismatch == FALSE )
                     {
                        /* don't print error message in a loop */
                        req_conf_mismatch = TRUE;
                        MSG_GERAN_ERROR_5_G("ERROR: Idle mode: Pwr measured on arfcn: %d, but storing results on arfcn: %d, req_pos: %d, cnf_pos: %d, num_monitors=%d",
                           rpt->arfcns[i].num,
                           bs->arfcn.num, 
                           l1_sc_pwr_data_ptr->monscan.req_pos,
                           l1_sc_pwr_data_ptr->monscan.cnf_pos,
                           num_monitors);
                     } /* if( req_conf_mismatch == FALSE )                     */
                  }    /* else - if (ARFCNS_EQUAL(bs->arfcn, rpt->arfcns[i]) ) */
               }       /* if( (rpt->yielded != NULL) && !rpt->yielded[i])      */

               /* If the ncell_monitor shutdown is currently active then check here for the measurements
              instantaneous power levels, and if they are above the set threshold then revert to noraml
              ncell monitor behaviour. */
               if ( ( l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state == NCELL_PCH_TIMER_EXPIRED ) &&
                    ( rpt->pwr_dBm_x16[i] > SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 ) )
               {
                  l1_sc_ncell_monitor_timer_start(gas_id);
               }

               /* power monitor processed, so increment the counter */
               i++;
#ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS
            } /* if( cell->pwr_msr_enabled ) */
#endif

            ++l1_sc_pwr_data_ptr->monscan.cnf_pos;
            if ( l1_sc_pwr_data_ptr->monscan.cnf_pos == l1_sc_pwr_data_ptr->monscan.req_cell_count )
            {
               l1_sc_pwr_data_ptr->monscan.cnf_pos = 0;
               wrapped         = TRUE;

               if (l1_sc_pwr_data_ptr->monscan.num_passes < l1_sc_pwr_data_ptr->monscan.req_passes)
               {
                  l1_sc_pwr_data_ptr->monscan.num_passes++;
               }
            }
         } /* while(i < num_monitors) */
      } /* if ( l1_sc_pwr_data_ptr->monscan.req_cells_id == l1_sc_balist_get_old_id(gas_id) ) */

      /* Now pass results to task to complete processing */
      if (l1_sc_pwr_data_ptr->monscan.num_passes >= l1_sc_pwr_data_ptr->monscan.req_passes)
      {
         l1_sc_pwr_data_ptr->monscan.isr_results->done_req_passes = TRUE;
      }
      else
      {
         l1_sc_pwr_data_ptr->monscan.isr_results->done_req_passes = FALSE;
      }

      /* indicate whether just completed pass through BA list */
      l1_sc_pwr_data_ptr->monscan.isr_results->wrapped = wrapped;

      /* Synchronise power monitor pointers (req/conf) if they go out of sync */
      if(req_conf_mismatch == TRUE)
      {
         /* reset req/conf pointers if req/conf not in sync */
         l1_sc_pwr_data_ptr->monscan.req_pos           = 0;
         l1_sc_pwr_data_ptr->monscan.cnf_pos           = 0;
         l1_sc_pwr_data_ptr->monscan.req_pos_prev      = 0;
      }

      l1_sci_monscan_done_idle(gas_id);

      l1_decrement_idle_monitors_in_progress(gas_id);
      L1_LOG_VAR(l1_get_idle_monitors_in_progress(gas_id),L1_FNAME_L1_SC_PWR,L1_VAR_IDLE_MONITORS_IN_PROGRESS,gas_id);
      l1_sc_pwr_data_ptr->monscan.meas_in_progress = FALSE;

   }
#ifdef FEATURE_GPRS_GBTA
   else
   {
      /*reset these flags only in case gas_id_was_flipped .i.e PT sub scheduled idle monitors for idle sub
      * as in that case it might be possible that Idle sub would have already cleared its monscan.active flag
      * on getting RACH req (stop SCE)*/
      if(gas_id_flipped)
      {

         l1_sc_globals_T   *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);
         l1_decrement_idle_monitors_in_progress(gas_id);
         L1_LOG_VAR(l1_get_idle_monitors_in_progress(gas_id),L1_FNAME_L1_SC_PWR,L1_VAR_IDLE_MONITORS_IN_PROGRESS,gas_id);
         l1_sc_pwr_data_ptr->monscan.meas_in_progress = FALSE;
         l1_sc_globals_ptr->monscan_in_progress = FALSE;
         L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);
         MSG_GERAN_HIGH_0_G("reset meas_in_progress & monscan_in_progress");
      }
   }
#endif

#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_GSM_SCE_PWR_MEAS_DBG)
   {
      idle_data_T* l1_idle_data = get_ptr_to_idle_data(gas_id);
      l1_idle_data->do_pwr_meas_FN = L1_GPRS_INVALID_FN;
   }
#endif

   GL1_ISR_SAVE_UNLOCK(gas_id);
#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}  /* l1_sci_monscan_power_cb_idle */

/*===========================================================================

FUNCTION  l1_sci_monscan_abort_cb_idle

DESCRIPTION
  Callback called by message layer when power measurements are aborted in
  Idle Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_abort_cb_idle( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr;
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr;
#ifdef FEATURE_GPRS_GBTA
      gas_id_t idle_gas_id;
#endif



#ifdef FEATURE_GPRS_GBTA
   /* Swap gas_id as pwr mon scheduled by ptm sub */
   if( gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED,gas_id,&idle_gas_id))
   {
     MSG_GERAN_HIGH_2_G("GBTA abort_cb_idle gas_id %d will flip gas_id idle %d",
                       gas_id,
                       idle_gas_id);

     /* Flip gas_id as scheduled in other sub gas_id */
     /* using transfer sub gas)id to derive actual idle sub gas_id */
     gas_id = idle_gas_id;
   }

   /* Reset when Pwr mon done */
   grm_notify_event(GBTA_EV_NONE,FALSE,gas_id);
#endif /* FEATURE_GPRS_GBTA */

   gas_id             = check_gas_id(gas_id);
   l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   /* restore prev req_pos to restart from same location */
   l1_sc_pwr_data_ptr->monscan.req_pos = l1_sc_pwr_data_ptr->monscan.req_pos_prev;

   l1_sci_monscan_swap_cells(gas_id);

   l1_decrement_idle_monitors_in_progress(gas_id);
   L1_LOG_VAR(l1_get_idle_monitors_in_progress(gas_id),L1_FNAME_L1_SC_PWR,L1_VAR_IDLE_MONITORS_IN_PROGRESS,gas_id);
   l1_sc_pwr_data_ptr->monscan.meas_in_progress          = FALSE;
   l1_sc_globals_ptr->monscan_in_progress = FALSE;
   L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);

   /*check if the last pwr meas abort was done while it was in SM_PWR_MEAS_RETRIEVING state
    *as then we need to suspend TRM release else the TRM release would trigger a APP mod change cmd
    *to FW while PWR meas is in progress
    */
#ifdef FEATURE_DUAL_SIM
   if(gl1_msg_pwr_meas_aborted_while_retrieving(gas_id))
   {
      MSG_GERAN_MED_0_G("SCE: l1_sci_monscan_abort_cb_idle(), suspend trm_release");
      /*to suspend the TRM release by 2 frames for PWR MEAS to finish*/
      l1_fm_set_suspend_trm_release( PWR_MEAS_ABORT_SUSPENSION_FRAME_DELAY,
                                     FM_PRI_RESERVED, gas_id );
   }

#endif

#if (defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )) && defined (FEATURE_GSM_SCE_PWR_MEAS_DBG)
   {
      idle_data_T* l1_idle_data = get_ptr_to_idle_data(gas_id);
      l1_idle_data->do_pwr_meas_FN = L1_GPRS_INVALID_FN;
   }
#endif

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_sci_monscan_abort_cb_ded

DESCRIPTION
  Callback called by message layer when power measurements are aborted in
  Dedicated Mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_abort_cb_ded( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   l1_sci_monscan_swap_cells(gas_id);

   l1_sc_pwr_data_ptr->monscan.meas_in_progress          = FALSE;
   l1_sc_globals_ptr->monscan_in_progress = FALSE;
   L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}


/*===========================================================================

FUNCTION  l1_sci_monscan_done_ded

DESCRIPTION
  Sets up the results pointers for the next set of measurements and calls
  l1_sc_monscan_done_ded().


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_done_ded( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   ASSERT_INT();

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif

   l1_sci_monscan_swap_cells(gas_id);

   /* reset counters to start of BA list */
   l1_sc_pwr_data_ptr->monscan.cnf_pos        = 0;
   l1_sc_pwr_data_ptr->monscan.req_pos        = 0;

   /* callback to task context for rest of processing */
   l1_sc_pwr_data_ptr->monscan.task_results->id = l1_sc_pwr_data_ptr->monscan.id;
   
   /* only for dedicated, used to indicate if x3 monitors were measured */
   l1_sc_pwr_data_ptr->monscan.task_results->pm_adaptive_original_cnt = l1_sc_pwr_data_ptr->monscan.pm_adaptive_original_cnt;
   
   l1_scdrv_call_in_task( l1_sc_monscan_done_ded, (void*) l1_sc_pwr_data_ptr->monscan.task_results, gas_id );
}


/*===========================================================================

FUNCTION  l1_sci_monscan_done_idle

DESCRIPTION
  Sets up the results pointers for the next set of measurements and calls
  l1_sc_monscan_done_idle().

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_monscan_done_idle( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   ASSERT_INT();


#ifdef DEBUG_SCE_MSG
   #error code not present
#endif

   l1_sci_monscan_swap_cells(gas_id);

   /* callback to task context for rest of processing */
   l1_sc_pwr_data_ptr->monscan.task_results->id = l1_sc_pwr_data_ptr->monscan.id;
   l1_scdrv_call_in_task( l1_sc_monscan_done_idle, (void*) l1_sc_pwr_data_ptr->monscan.task_results, gas_id);
}

/*===========================================================================

FUNCTION  l1_sci_monscan_next_period_ded

DESCRIPTION
   Called to notify monscan that end of reporting period is approaching.
   Calls l1_sci_monscan_done_ded if monitors are already done.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_monscan_next_period_ded( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   ASSERT_INT();

   /* If a series of measurements is still in progress then
    * set the next_period flag so that monscan_done_ded()
    * is called when the measurements complete, otherwise
    * call it now.
    *
    * Also ensure that callback function is called at the
    * end of the ISR processing for the reporting period.
    * (The callback function will trigger the sending of
    * the serving cell measurements.)
    */

   if ( l1_sc_pwr_data_ptr->monscan.active )
   {
      if ( l1_sc_pwr_data_ptr->monscan.meas_in_progress )
      {
         l1_sc_pwr_data_ptr->monscan.next_period = TRUE;
      }
      else
      {
         l1_sci_monscan_done_ded(gas_id);

         if (l1_sc_pwr_data_ptr->monscan.next_period_ded_cb != NULL)
         {
            l1_sc_pwr_data_ptr->monscan.next_period_ded_cb(gas_id); /*lint !e746 no prototype */
         }
      }
   }
   else
   {
       if (l1_sc_pwr_data_ptr->monscan.next_period_ded_cb != NULL)
       {
          l1_sc_pwr_data_ptr->monscan.next_period_ded_cb(gas_id); /*lint !e746 no prototype */
       }
   }

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif

}

/*===========================================================================

FUNCTION l1_sci_monscan_set_next_period_ded_cb

DESCRIPTION
   Install a call back funtion that will be called when the ISR part of
   the processing for a reporting period is completed.

   Mon scan is told that the reporting period is ending by a call to
   l1_sci_monscan_next_period_ded().

   The call back provides a mechanism to synchronise the sending of
   surrouding and serving meaurement reports.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sci_monscan_set_next_period_ded_cb( void (*cb)(gas_id_t gas_id), gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

    l1_sc_pwr_data_ptr->monscan.next_period_ded_cb = cb;
}


/*===========================================================================

FUNCTION

DESCRIPTION
  - Redundant function. need to be removed during the cleanup.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_monscan_do_rxlev_gprs_idle( uint8 num_meas, uint8 meas_per_frame, gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];

   uint8         num;
   cell_T*       cell;

   ASSERT_INT();

   /* One less is max as we have the extra entry after looping for the SCELL */
   if ( num_meas >= L1_SC_PWR_MAX_MEAS_PER_CALL )
   {
      MSG_GERAN_ERROR_2_G("Requested %d measurements, doing %d.", num_meas, ( L1_SC_PWR_MAX_MEAS_PER_CALL - 1 ));

      num_meas = ( L1_SC_PWR_MAX_MEAS_PER_CALL - 1);
   }


   if ( ( l1_sc_pwr_data_ptr->monscan.active ) && (!l1_sc_pwr_data_ptr->monscan.meas_in_progress) )
   {

      if (l1_sc_pwr_data_ptr->monscan.req_cell_count < num_meas)
      {
         num_meas = (uint8)l1_sc_pwr_data_ptr->monscan.req_cell_count;
      }

      /* store prev req_pos to restart from same location when aborted */
      l1_sc_pwr_data_ptr->monscan.req_pos_prev = l1_sc_pwr_data_ptr->monscan.req_pos;

      /* Do a loop to setup num_meas monitors in the next frames. */
      for( num=0; num<num_meas; num++ )
      {
         cell = l1_sc_pwr_data_ptr->monscan.req_cells[ l1_sc_pwr_data_ptr->monscan.req_pos ];
         ++l1_sc_pwr_data_ptr->monscan.req_pos;

         if ( l1_sc_pwr_data_ptr->monscan.req_pos == l1_sc_pwr_data_ptr->monscan.req_cell_count )
         {
            l1_sc_pwr_data_ptr->monscan.req_pos = 0;
         }

         l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num] = cell->bs.arfcn;
         l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].agc_ptr[num] = &cell->bs.agc;
         l1_sc_pwr_data_ptr->arfcn[num]              = cell->bs.arfcn;
         l1_sc_pwr_data_ptr->agc_ptr[num]            = &cell->bs.agc;
         MSG_GERAN_MED_2_G("Mon Arfcn=%i dBm=%i ", cell->bs.arfcn.num,
                                        (cell->bs.agc.pwr_dBm_x16 >> 0x4));
      }

      l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num] = l1_idle_data->campedon_cell_ARFCN;
      l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].agc_ptr[num] = &l1_idle_data->campedon_cell_agc;
      MSG_GERAN_LOW_2_G("ARFCN = %d POWER = %d ", l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num].num, l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].agc_ptr[num]->pwr_dBm_x16);
      num++;
      num_meas++;

      /* Register the callback for message layer every time */
      /* pscan may have changed it. */
#ifdef GL1_GMSK_MOD_DET
	  if(decStat[gas_id] == NULL)
	  {
         decStat[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(L1_SC_PWR_MAX_MEAS_PER_CALL+1));
	  }
#endif
      if(gl1_msg_cfg_pwr_meas(l1_sci_monscan_rxlev_cb_gprs_idle, l1_sci_monscan_abort_cb_gprs_idle, gas_id))
      {
         l1_sc_pwr_data_ptr->monscan.num_req = num_meas;
#ifdef GL1_GMSK_MOD_DET
		 l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].pwr_meas_cb_param.decstat= decStat[gas_id] ;
#endif
         gl1_msg_do_pwr_meas ( l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn,
                               l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].agc_ptr,
                               num_meas,
                               meas_per_frame,
                               num_meas,
                              &l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].pwr_meas_cb_param,
                               gas_id
                             );

        /* The lazy way to toggle 1 bit
         l1_sc_pwr_data_ptr->ncell_info_res_idx = (l1_sc_pwr_data_ptr->ncell_info_res_idx + 1) % 2; */
         l1_sc_pwr_data_ptr->ncell_info_res_idx ^=1;   
 
         l1_increment_idle_monitors_in_progress(gas_id);
         L1_LOG_VAR(l1_get_idle_monitors_in_progress(gas_id),L1_FNAME_L1_SC_PWR,L1_VAR_IDLE_MONITORS_IN_PROGRESS,gas_id);
         l1_sc_pwr_data_ptr->monscan.meas_in_progress = TRUE;
         gl1_set_pwr_meas_block_start(TRUE,gas_id);

#ifdef DEBUG_SCE_MSG
         #error code not present
#endif
      }
      else
      {
         MSG_GERAN_ERROR_0_G("Pwr meas config not succesfull, pwr meas not intialised");

         /* restore prev req_pos to restart from same location */
         l1_sc_pwr_data_ptr->monscan.req_pos = l1_sc_pwr_data_ptr->monscan.req_pos_prev;
      }
   }
}

/*===========================================================================

FUNCTION

DESCRIPTION
  - Redundant function. need to be removed during the cleanup.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_rxlev_cb_gprs_idle(gl1_msg_pwr_meas_result_type * pwr_results_info, gas_id_t gas_id)
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
   uint32        i;
   boolean       wrapped=FALSE;
   l1_sc_bs_T    *bs;
   boolean       req_conf_mismatch = FALSE;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

   ASSERT_INT();

   l1_sc_pwr_data_ptr->num_monitors = pwr_results_info->num_pwr_meas;
   /* Check that all arfcns have been scheduled, corret req_pos if not */
   l1_sc_pwr_data_ptr->monitors_remaining = l1_sc_pwr_data_ptr->monscan.num_req - l1_sc_pwr_data_ptr->num_monitors;
   if (l1_sc_pwr_data_ptr->monitors_remaining > 0)
   {
      MSG_GERAN_HIGH_2_G("Not all monitors scheduled: requested: %d, scheduled: %d",l1_sc_pwr_data_ptr->monscan.num_req, l1_sc_pwr_data_ptr->num_monitors);
   }
   /* Last power measurement is actually for serving cell. */
   if(l1_sc_pwr_data_ptr->num_monitors > 0)
   {
     l1_sc_pwr_data_ptr->num_monitors--;
   }


   if ( l1_sc_pwr_data_ptr->monscan.active )
   {

     /* Check the ID, if it doesn't match then don't process
      * the results, but still call callback.
      */
     if ( l1_sc_pwr_data_ptr->monscan.req_cells_id == l1_sc_balist_get_old_id(gas_id) )
     {
        for(i=0; i < l1_sc_pwr_data_ptr->num_monitors; i++)
        {
           /* update running averages */
           if( (pwr_results_info->yielded != NULL) && !pwr_results_info->yielded[i])
           {
              bs = &(l1_sc_pwr_data_ptr->monscan.req_cells[l1_sc_pwr_data_ptr->monscan.cnf_pos]->bs);
              if(ARFCNS_EQUAL(pwr_results_info->arfcns[i], bs->arfcn))
              {
           /* all good: power measured for the right arfcn */
                 l1_running_avg_add_meas( &(bs->ra),
                                          pwr_results_info->pwr_dBm_x16[i], 
                                          GL1_INVALID_POWER_LEVEL,
                                          PRX_MEAS,
                                          gas_id);
                 bs->meas_state = L1SCMeasNew;
              }
              else
              {
                 /* serious error: power measured on arfcn x, but storing results on arfcn y */
                 if(req_conf_mismatch == FALSE)
                 {
                    /* don't print error message in a loop */
                    req_conf_mismatch = TRUE;

                    MSG_GERAN_ERROR_5_G("ERROR: Gprs Idle: Pwr measured on arfcn: %d, but storing results on arfcn: %d, req_pos: %d, cnf_pos: %d, num_monitors=%d",
                       pwr_results_info->arfcns[i].num,
                       bs->arfcn.num, 
                       l1_sc_pwr_data_ptr->monscan.req_pos,
                       l1_sc_pwr_data_ptr->monscan.cnf_pos,
                       l1_sc_pwr_data_ptr->num_monitors);
                 } /* if(req_conf_mismatch == FALSE)                                            */
              }    /* else - if(ARFCNS_EQUAL(pwr_results_info->arfcns[i], bs->arfcn))           */
           }       /* if( (pwr_results_info->yielded != NULL) && !pwr_results_info->yielded[i]) */

           /* Monitor Call back results */
           MSG_GERAN_MED_2_G("CallBck Arfcn=%i dBm=%i", pwr_results_info->arfcns[i].num,
                                             (pwr_results_info->pwr_dBm_x16[i] >> 0x4));
           ++l1_sc_pwr_data_ptr->monscan.cnf_pos;
           if ( l1_sc_pwr_data_ptr->monscan.cnf_pos == l1_sc_pwr_data_ptr->monscan.req_cell_count )
           {
              l1_sc_pwr_data_ptr->monscan.cnf_pos = 0;
              wrapped         = TRUE;

              if (l1_sc_pwr_data_ptr->monscan.num_passes < l1_sc_pwr_data_ptr->monscan.req_passes)
              {
                 l1_sc_pwr_data_ptr->monscan.num_passes++;
              }
           }
        }
     }
     else
     {
        i = l1_sc_pwr_data_ptr->num_monitors;
     }

     /*  Serving cell measurements */
     /*  RLA_P measurements always taken on BCCH */
     if( (pwr_results_info->yielded != NULL) && !pwr_results_info->yielded[i])
     {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       /*if the arfcn is from a denied band then the skip flag is set prior to measuringand GFW 
         will report 0dB for RSSI meas .This will impact RSSI averaging for this ARFCN so do not
         update the running average*/
       if (grm_is_band_registered(pwr_results_info->arfcns[i].band, l1_tskisr_blk->client_id, gas_id))
       {
#endif
         /* Function is redundant, therefore no need to handle DivRX power measurement.*/
         l1_running_avg_add_meas( &(gprs_serv_cell_meas_ptr->rla_p_buffer), 
                                  pwr_results_info->pwr_dBm_x16[i], 
                                  GL1_INVALID_POWER_LEVEL,
                                  PRX_MEAS,
                                  gas_id);
                  
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       }
       else
       {
         MSG_GERAN_HIGH_2_G("l1_sci_monscan_rxlev_cb_gprs_idle:Skipped arfcn %d pwr_dBm_x16 %d do not update running avg!",
         pwr_results_info->arfcns[i].num, pwr_results_info->pwr_dBm_x16[i] );
       }
#endif                  

     }
     gprs_serv_cell_meas_ptr->rla_p_count++;

     /*  Power control measurements taken on BCCH if indicated in the PC_MEAS_CHAN parameter */
     if (!gprs_l1_idle_data->gprs_campedon_cell_data.power_control.pc_meas_chan)
     {
       /*  If PC_MEAS_CHAN is not set update power control parameters  */
       gprs_serv_cell_meas_ptr->power_cntrl_samples += pwr_results_info->pwr_dBm_x16[i];
       gprs_serv_cell_meas_ptr->power_cntrl_count++;
     }

     MSG_GERAN_LOW_3_G(" CALLBACK SERV ARFCN = %d POWER = %d AVG = %d ", pwr_results_info->arfcns[i].num, pwr_results_info->pwr_dBm_x16[i], l1_running_avg_get_avg(&gprs_serv_cell_meas_ptr->rla_p_buffer));

     /* Now pass results to task to complete processing */
     if (l1_sc_pwr_data_ptr->monscan.num_passes == l1_sc_pwr_data_ptr->monscan.req_passes)
     {
       l1_sc_pwr_data_ptr->monscan.isr_results->done_req_passes = TRUE;
     }
     else
     {
       l1_sc_pwr_data_ptr->monscan.isr_results->done_req_passes = FALSE;
     }

     /* indicate whether just completed pass through BA list */
     l1_sc_pwr_data_ptr->monscan.isr_results->wrapped = wrapped;

     /* Synchronise power monitor pointers (req/conf) if they go out of sync */
     if(req_conf_mismatch == TRUE)
     {
        /* reset req/conf pointers if req/conf not in sync */
        l1_sc_pwr_data_ptr->monscan.req_pos           = 0;
        l1_sc_pwr_data_ptr->monscan.cnf_pos           = 0;
        l1_sc_pwr_data_ptr->monscan.req_pos_prev      = 0;
     }

     l1_sci_monscan_done_gprs_idle(gas_id);

     l1_decrement_idle_monitors_in_progress(gas_id);
     L1_LOG_VAR(l1_get_idle_monitors_in_progress(gas_id),L1_FNAME_L1_SC_PWR,L1_VAR_IDLE_MONITORS_IN_PROGRESS,gas_id);
     l1_sc_pwr_data_ptr->monscan.meas_in_progress = FALSE;

   }

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}

/*===========================================================================

FUNCTION  l1_sci_monscan_done_gprs_idle

DESCRIPTION
  Sets up the results pointers for the next set of measurements and calls
  l1_sc_monscan_done_gprs_idle().
  - Redundant function. need to be removed during the cleanup.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_done_gprs_idle( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);

   ASSERT_INT();

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif

   /* There was a need to speed up SCE for GCF 3.18 the MS needs to */
   /* react to new BA List cells for PMO NC2 measurements of 7 sec. */
   /* This test prevents the early new cell swap from another swap. */
   if (gpl1_gprs_meas_mode_ptr->nc_new_ba_list_pending == FALSE)
   {
      l1_sci_monscan_swap_cells(gas_id);
   }
   else
   {
     gpl1_gprs_meas_mode_ptr->nc_new_ba_list_pending = FALSE;
   }

   /* callback to task context for rest of processing */
   l1_sc_pwr_data_ptr->monscan.task_results->id = l1_sc_pwr_data_ptr->monscan.id;
   l1_scdrv_call_in_task( l1_sc_monscan_done_gprs_idle, (void*) l1_sc_pwr_data_ptr->monscan.task_results, gas_id );


}

/*===========================================================================

FUNCTION

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_monscan_do_rxlev_trans( uint8 num_meas, uint8 meas_per_frame, gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   idle_data_T      *l1_idle_data=&l1_idle_data_store[gas_id];

   uint8         num;
   uint8         req_pos_backup;
   cell_T*       cell;
   uint16        sched_no_of_scell_bcch_monitors = 0;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   sys_band_T  serving_cell_band;
#endif
   uint32        FnMod104 = ADD_FN(gl1_get_FN(gas_id), 1) % 104;

   ASSERT_INT();

   MSG_GERAN_MED_2_G( "l1_sci_monscan_do_rxlev_trans  num_meas=%d meas_per_frame=%d " , num_meas, meas_per_frame);

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
   if( skip_scell_meas)
   {
     skip_scell_meas = 0;
   }
   else
   {
     skip_scell_meas = 1;
   }

   MSG_GERAN_HIGH_3_G(" start asdiv type1 algorithm =%d delay_for_ante_switching =%d skip_scell_meas =%d",gl1_hw_asdiv_type1.start_alg,gl1_hw_asdiv_type1.wait_count,skip_scell_meas);

   if (gl1_hw_asdiv_type1.wait_count > -6 )
   {
     gl1_hw_asdiv_type1.wait_count -- ;
   }

#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

   if ( num_meas > L1_SC_PWR_MAX_MEAS_PER_CALL )
   {
      MSG_GERAN_ERROR_2_G("Requested %d measurements, doing %d.", num_meas, L1_SC_PWR_MAX_MEAS_PER_CALL);
      num_meas = L1_SC_PWR_MAX_MEAS_PER_CALL;
   }

   if( l1_sc_pwr_data_ptr->monscan.active )
   {
     /*
     l1_sci_monscan_do_rxlev_trans gets called every 13 frames
     Make sure to only force switch for test mode switch every 26 frames.
    */
#ifdef FEATURE_GSM_QSC_TX_DIV
   if(gl1_trm_asdiv_debug_get_test_mode(gas_id) && skip_scell_meas)
   {
      gl1_trm_asdiv_debug_test_mode_switch_sm(gas_id, TRM_CLIENT_MODE_RXTX);
   }
   else
   {
      gl1_trm_asdiv_type2_trans_alg(gas_id);
   }
#endif /*FEATURE_GSM_QSC_TX_DIV*/
     /************************************************************/
     /* Do a loop to setup num_meas monitors in the next three   */
     /* blocks                                                   */
     /************************************************************/

     /* store the req_pos incase of abort */
     req_pos_backup = l1_sc_pwr_data_ptr->monscan.req_pos;
     l1_sc_pwr_data_ptr->monscan.req_pos_prev = req_pos_backup;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
     if ( gl1_drdsds_enabled( gas_id ) )
     {
       /*capture serving cell band this is to check if we are making non-serving band measurements in the next three blocks.
        Non-serving cell band measurements must be checked with TRM before proceeding when DR-DSDS is enabled. RF tuning has an impact on the other tech
       */
       serving_cell_band = gl1_get_band(l1_idle_data->campedon_cell_ARFCN);
     }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

     for( num=0; num<num_meas; num++ )
     {
        antenna_to_use[num]= 0;

        if ( num == FRAME_FOR_SCELL_MONITOR_TRANS[sched_no_of_scell_bcch_monitors] )
        {
           /* Place a scell mointor in at this location */
           l1_sc_pwr_data_ptr->ncell_info[ l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num]= l1_sc_pwr_data_ptr->arfcn[num] = l1_idle_data->campedon_cell_ARFCN;
           l1_sc_pwr_data_ptr->ncell_info[ l1_sc_pwr_data_ptr->ncell_info_res_idx].agc_ptr[num] = l1_sc_pwr_data_ptr->agc_ptr[num] = &l1_idle_data->campedon_cell_agc;
           MSG_GERAN_LOW_1_G("adding scell monitor %d",l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num].num);

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
           /* First 13 frames, scell at frames [0, 5, 9] will use ant [2, 1, 0]
            * Next 13 frames,  scell at frames [0, 5, 9] will use ant [0, 0, 0]
            * This will repeat
            */
           if (
                   ( (sched_no_of_scell_bcch_monitors==0) || (sched_no_of_scell_bcch_monitors==1) )
                && ( gl1_hw_asdiv_type1.start_alg )
                && ( gl1_hw_asdiv_type1.wait_count <= 0)
                && ( skip_scell_meas )
                && ( !gl1_trm_asdiv_debug_get_test_mode(gas_id) )
              )
           {
              MSG_GERAN_HIGH_3_G ( "Probe Monitor  ARFCN = %d monscan.req_pos = %d index = %d",
                l1_idle_data->campedon_cell_ARFCN.num, l1_sc_pwr_data_ptr->monscan.req_pos, num);
   
              if (sched_no_of_scell_bcch_monitors==0)
              {
                antenna_to_use[num]= 2;
              }
              else if (sched_no_of_scell_bcch_monitors==1)
              {
                antenna_to_use[num]= 1;
              }

              packet_scell_meas_count++;
              if (packet_scell_meas_count == 8 )
              {
                start_tx_div_antenna_alg( TRUE,gas_id);
                packet_scell_meas_count =0;
              }
           }
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

           /* increment count after it has been used above */
           sched_no_of_scell_bcch_monitors++;

           /* We've run off the end of available frames so wrap around for next set */
           if ( sched_no_of_scell_bcch_monitors >= NO_OF_XFER_VALUE_MEAS )
           {
             sched_no_of_scell_bcch_monitors = 0;
           }
        }
        else
        {
           if(l1_sc_pwr_data_ptr->monscan.req_cell_count > 0)
           {

#ifdef FEATURE_GSM_COEX_REDUCED_MEAS_GPRS
             l1_sc_pwr_data_ptr->coex_protected_meas[num] = l1_sc_pwr_data_ptr->is_coex_protected_monitor(FnMod104 + num, gas_id);

             if(!l1_sc_pwr_data_ptr->coex_protected_meas[num])
             {
                /* Place a ncell monitor in at this location */
                cell = l1_sc_pwr_data_ptr->monscan.req_cells[ l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected];
                ++l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected;
                if ( l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected== l1_sc_pwr_data_ptr->monscan.req_cell_count )
                {
                   l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected = 0;
                }
             }
             else
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS_GPRS */
             {
                /* Place a ncell monitor in at this location */
                cell = l1_sc_pwr_data_ptr->monscan.req_cells[ l1_sc_pwr_data_ptr->monscan.req_pos ];
                ++l1_sc_pwr_data_ptr->monscan.req_pos;
                if ( l1_sc_pwr_data_ptr->monscan.req_pos == l1_sc_pwr_data_ptr->monscan.req_cell_count )
                {
                   l1_sc_pwr_data_ptr->monscan.req_pos = 0;
                }
             }

             l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num]   = l1_sc_pwr_data_ptr->arfcn[num]   = cell->bs.arfcn;
             l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].agc_ptr[num] = l1_sc_pwr_data_ptr->agc_ptr[num] = &cell->bs.agc;
             MSG_GERAN_LOW_1_G("adding ncell monitor %d",l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num].num);

           }
           else
           {
             /* BA list empty, stuff SCELL ARFCN instead */
             l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num]   = l1_idle_data->campedon_cell_ARFCN;
             l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].agc_ptr[num] = &l1_idle_data->campedon_cell_agc;
             MSG_GERAN_HIGH_2_G("adding stuffed scell monitor %d at %d",l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn[num].num,num);
           }
        }
     }

     l1_sc_pwr_data_ptr->monscan.num_req = num_meas;

     /* Update scell and ncell arfcn for timeline reporting */
     garb_update_scell_ncell_arfcns(l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn,num_meas,1,gas_id);
#ifdef GL1_GMSK_MOD_DET
	 if(decStat[gas_id] == NULL)
	 {
     decStat[gas_id] = (uint16 *)GPRS_MEM_CALLOC(4*(L1_SC_PWR_MAX_MEAS_PER_CALL+1));
	 }
#endif      
     if (num > 0)
     {
        if(gl1_msg_cfg_pwr_meas( l1_sci_monscan_rxlev_cb_trans,
                                 l1_sci_monscan_abort_cb_trans,gas_id ))
        {
#ifdef GL1_GMSK_MOD_DET
          l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].pwr_meas_cb_param.decstat = decStat[gas_id];
 #endif 
           gl1_msg_do_pwr_meas
           (
              l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].arfcn,
              l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].agc_ptr,
              num_meas,
              meas_per_frame,
              num_meas + 1,  /* Max frames to measure for */
              &l1_sc_pwr_data_ptr->ncell_info[l1_sc_pwr_data_ptr->ncell_info_res_idx].pwr_meas_cb_param,
              gas_id
           );

           l1_sc_pwr_data_ptr->ncell_info_res_idx++;
           l1_sc_pwr_data_ptr->ncell_info_res_idx %= 2;

           l1_sc_pwr_data_ptr->monscan.meas_in_progress = TRUE;
           gl1_set_pwr_meas_block_start(TRUE,gas_id);

           MSG_GERAN_MED_1_G("SCE: l1_sci_monscan_do_rxlev_trans(), %d", num);
        }
        else
        {
           MSG_GERAN_ERROR_2_G("Pwr meas config not succesfull, pwr meas not intialised %i, %i", l1_sc_pwr_data_ptr->monscan.req_pos,
             req_pos_backup);
           l1_sc_pwr_data_ptr->monscan.req_pos = req_pos_backup;
        }
     }
   }
}

/*===========================================================================

FUNCTION

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_rxlev_cb_trans(gl1_msg_pwr_meas_result_type * pwr_results_info, gas_id_t gas_id)
{
   gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

   uint32            num_monitors;
   uint32            i;
   l1_sc_bs_T       *bs;
   dBx16_T           power;
   boolean           wrapped=FALSE;
   uint8             num_ncell_req;
   uint16            sched_no_of_scell_bcch_monitors = 0;
   boolean           req_conf_mismatch = FALSE;

   /* Setup a pointer to the current transfer data parameters */
   transfer_data_T*  transfer_data_ptr =
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;

   ASSERT_INT();

   num_monitors     = pwr_results_info->num_pwr_meas;

   if (num_monitors < 2)
   {
     /* This is not good.  Try to recover. */
     uint32 fn = gl1_get_FN(gas_id);
     MSG_GERAN_ERROR_2_G("Not enough power measure results (%d). FN=%d", num_monitors, fn);

     num_monitors = 0;
   }

   if ( l1_sc_pwr_data_ptr->monscan.active )
   {
     MSG_GERAN_LOW_2_G(" monscan.num_req [%d] pwr_results_info->num_pwr_meas [%d] ",(l1_sc_pwr_data_ptr->monscan.num_req),pwr_results_info->num_pwr_meas);

     if ((l1_sc_pwr_data_ptr->monscan.num_req) != (pwr_results_info->num_pwr_meas))
     {
       /* Calculate number of ncells, take off the 3 SCell monitors */
       num_ncell_req = (l1_sc_pwr_data_ptr->monscan.num_req - 3);

       for(i=0; i < num_ncell_req ; i++)
       {

         ++l1_sc_pwr_data_ptr->monscan.cnf_pos;

         if ( l1_sc_pwr_data_ptr->monscan.cnf_pos == l1_sc_pwr_data_ptr->monscan.req_cell_count )
         {
              l1_sc_pwr_data_ptr->monscan.cnf_pos = 0;
              wrapped         = TRUE;

              if (l1_sc_pwr_data_ptr->monscan.num_passes < l1_sc_pwr_data_ptr->monscan.req_passes)
              {
                 l1_sc_pwr_data_ptr->monscan.num_passes++;
              }

              MSG_GERAN_LOW_2_G("PASSES REQ_PASSES %d %d ",l1_sc_pwr_data_ptr->monscan.num_passes,l1_sc_pwr_data_ptr->monscan.req_passes);
         }
       }

        /* Now pass results to task to complete processing */
      if (l1_sc_pwr_data_ptr->monscan.num_passes >= l1_sc_pwr_data_ptr->monscan.req_passes)
      {
        l1_sc_pwr_data_ptr->monscan.isr_results->done_req_passes = TRUE;
      }
      else
      {
        l1_sc_pwr_data_ptr->monscan.isr_results->done_req_passes = FALSE;
      }

       /* indicate whether just completed pass through BA list */
      l1_sc_pwr_data_ptr->monscan.isr_results->wrapped = wrapped;

      l1_sci_monscan_done_trans(gas_id);

      l1_sc_pwr_data_ptr->monscan.meas_in_progress = FALSE;
      MSG_GERAN_ERROR_0_G("early exit from trans_pwr_rxlev_cb!!");
      return;
     }

     /* Check the ID, if it doesn't match then don't process
      * the results, but still call callback.
      */
     if ( l1_sc_pwr_data_ptr->monscan.req_cells_id == l1_sc_balist_get_old_id(gas_id) )
     {
       gl1_power_meas_type pwr_meas_type = PRX_MEAS;

#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
       if(gl1_get_MSRD_ctl_nv(gas_id) || gl1_get_IMRD_ctl_nv(gas_id))
       {
         pwr_meas_type = PRX_DIVRX_MEAS;
       }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_IDLE*/

       /*************************************************************/
       /* Go through the results stripping out extra scell monitors */
       /* that have been inserted in order to calculate the C value */
       /* Match all results using the ARFCN check before using them */
       /*************************************************************/
       for(i=0;i<num_monitors;i++)
       {
         if ( (i == FRAME_FOR_SCELL_MONITOR_TRANS[sched_no_of_scell_bcch_monitors]) &&
              (ARFCNS_EQUAL(pwr_results_info->arfcns[i],l1_idle_data->campedon_cell_ARFCN))
            )
         {

           if ( FRAME_FOR_SCELL_MONITOR_TRANS[sched_no_of_scell_bcch_monitors] == EXCLUDE_SCELL_MONITOR_FRAME )
           {
             MSG_GERAN_LOW_1_G("Exclude scell BCCH burst %d in XFER",EXCLUDE_SCELL_MONITOR_FRAME);
           }
           else
           {
             /* If we are in test mode don't add the samples into the RLA_P buffer
              * Since samples are derived from PDCH instead */
             if (transfer_data_ptr->l1_test_mode == TEST_MODE_OFF)
             {
               if( (pwr_results_info->yielded != NULL) && !pwr_results_info->yielded[i])
               {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                 /*if the arfcn is from a denied band then the skip flag is set prior to measuringand GFW 
                   will report 0dB for RSSI meas .This will impact RSSI averaging for this ARFCN so do not
                   update the running average*/
                 if (grm_is_band_registered(pwr_results_info->arfcns[i].band, l1_tskisr_blk->client_id, gas_id))
                 {
#endif           
                   /* In GPRS, GL1 can schedule power monitior of serving cell if there is an opportunity
                      to do after scheduling power monitor of neighbour cells. Since RxD is not enabled in
                      monscan, therefore to keep avergaing meas counter consistent for PRX and DIVRX treat
                      PRX power same as DivRX power if RxD Data Transefr is enabled.*/
                   l1_running_avg_add_meas( &(gprs_serv_cell_meas_ptr->rla_p_buffer), 
                                              pwr_results_info->pwr_dBm_x16[i], 
                                              pwr_results_info->pwr_dBm_x16[i],
                                              pwr_meas_type,
                                              gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                 }
                 else
                 {
                   MSG_GERAN_HIGH_2_G("l1_sci_monscan_rxlev_cb_trans:Skipped arfcn %d pwr_dBm_x16 %d do not update running avg!",
                      pwr_results_info->arfcns[i].num, pwr_results_info->pwr_dBm_x16[i] );
                 }
#endif              
               

               }
               MSG_GERAN_LOW_3_G(" CALLBACK SERV ARFCN = %d POWER = %d AVG = %d ", pwr_results_info->arfcns[i].num,
                        pwr_results_info->pwr_dBm_x16[i], l1_running_avg_get_avg(&gprs_serv_cell_meas_ptr->rla_p_buffer));
               gprs_serv_cell_meas_ptr->rla_p_count++;
             }

             /*  Power control measurements taken on BCCH if indicated in the PC_MEAS_CHAN parameter */
             if (!gprs_l1_idle_data->gprs_campedon_cell_data.power_control.pc_meas_chan)
             {
               gprs_serv_cell_meas_ptr->power_cntrl_samples += pwr_results_info->pwr_dBm_x16[i];
               gprs_serv_cell_meas_ptr->power_cntrl_count++;
               MSG_GERAN_LOW_2_G("extract scell monitor %d at %d",pwr_results_info->pwr_dBm_x16[i],i);
             }
           }

           sched_no_of_scell_bcch_monitors++;

           /* We've run off the end of available frames so wrap around for next set */
           if ( sched_no_of_scell_bcch_monitors >= NO_OF_XFER_VALUE_MEAS )
           {
             sched_no_of_scell_bcch_monitors = 0;
           }
         }
         else
         {
           /* Check BA list is not empty as we stuff with Scell ARFCN if it is */
           if(l1_sc_pwr_data_ptr->monscan.req_cell_count !=0)
           {
             uint8 *cnf_pos_p;

#ifdef FEATURE_GSM_COEX_REDUCED_MEAS_GPRS
             if(!l1_sc_pwr_data_ptr->coex_protected_meas[i])
             {
                cnf_pos_p = &l1_sc_pwr_data_ptr->monscan.cnf_pos_coex_unprotected;
             }
             else
#endif
             {
                cnf_pos_p = &l1_sc_pwr_data_ptr->monscan.cnf_pos;
             }

             if( (l1_sc_pwr_data_ptr->monscan.req_cells[*cnf_pos_p]->bs.arfcn.num) !=
                              (pwr_results_info->arfcns[i].num) )
             {
                MSG_GERAN_HIGH_2_G("Unexpected ARFCN %d expected %d",
                l1_sc_pwr_data_ptr->monscan.req_cells[*cnf_pos_p]->bs.arfcn.num, pwr_results_info->arfcns[i].num );
             }

             bs = &(l1_sc_pwr_data_ptr->monscan.req_cells[*cnf_pos_p]->bs); 

             power = pwr_results_info->pwr_dBm_x16[i];

             if( (pwr_results_info->yielded != NULL) && !pwr_results_info->yielded[i])
             {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               /*if the arfcn is from a denied band then the skip flag is set prior to measuringand GFW 
                              will report 0dB for RSSI meas .This will impact RSSI averaging for this ARFCN so do not update the running average*/
              if (grm_is_band_registered(pwr_results_info->arfcns[i].band, l1_tskisr_blk->client_id, gas_id))
              {
#endif
               if(ARFCNS_EQUAL(pwr_results_info->arfcns[i], bs->arfcn))
               {
                  l1_running_avg_add_meas( &(bs->ra), 
                                        power, 
                                        GL1_INVALID_POWER_LEVEL,
                                        PRX_MEAS,
                                        gas_id);
                  bs->meas_state = L1SCMeasNew;

                  MSG_GERAN_LOW_4_G("SCE-PWR: NCELL ARFCN = %d POWER = %d AVG = %d i=%d", pwr_results_info->arfcns[i].num,
                     power/16, l1_running_avg_get_avg(&(bs->ra))/16, i);
               }
               else
               {
                  /* serious error: power measured on arfcn x, but storing results on arfcn y */
                  if(req_conf_mismatch == FALSE)
                  {
                     /* don't print error message in a loop */
                     req_conf_mismatch = TRUE;
            
                     MSG_GERAN_ERROR_5_G("Gprs Trfr: Pwr measured on arfcn: %d, but storing results on arfcn: %d, req_pos: %d, cnf_pos: %d, i=%d (SKIPPED)",
                        pwr_results_info->arfcns[i].num,
                        bs->arfcn.num, 
                        l1_sc_pwr_data_ptr->monscan.req_pos,
                        l1_sc_pwr_data_ptr->monscan.cnf_pos,
                        i);
                  } /* if(req_conf_mismatch == FALSE)                                            */
               }    /* else - if(ARFCNS_EQUAL(pwr_results_info->arfcns[i], bs->arfcn))           */
                                    
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
              }
              else
              {
#ifdef DEBUG_SCE_MSG
                #error code not present
#endif
              }
#endif  


             }

             MSG_GERAN_LOW_2_G("extract ncell monitor %d at %d",pwr_results_info->pwr_dBm_x16[i],i);

             (*cnf_pos_p)++;

             if ( *cnf_pos_p == l1_sc_pwr_data_ptr->monscan.req_cell_count )
             {
                *cnf_pos_p = 0;

                wrapped         = TRUE;

                if (l1_sc_pwr_data_ptr->monscan.num_passes < l1_sc_pwr_data_ptr->monscan.req_passes)
                {
                  l1_sc_pwr_data_ptr->monscan.num_passes++;
                }

                MSG_GERAN_LOW_2_G("PASSES REQ_PASSES %d %d ",l1_sc_pwr_data_ptr->monscan.num_passes,l1_sc_pwr_data_ptr->monscan.req_passes);
             }
           }
           else
           {
             /* no need to use this as we already have 8 Ncell samples per multiframe */
             MSG_GERAN_LOW_3_G("Ignore scell monitor %d at %d pwr cnt %d",pwr_results_info->pwr_dBm_x16[i],i,gprs_serv_cell_meas_ptr->power_cntrl_count);
           }
         }
       }
     }//end of for loop serve

     /* Now pass results to task to complete processing */
     if (l1_sc_pwr_data_ptr->monscan.num_passes >= l1_sc_pwr_data_ptr->monscan.req_passes)
     {
        l1_sc_pwr_data_ptr->monscan.isr_results->done_req_passes = TRUE;
     }
     else
     {
        l1_sc_pwr_data_ptr->monscan.isr_results->done_req_passes = FALSE;
     }

     /* indicate whether just completed pass through BA list */
     l1_sc_pwr_data_ptr->monscan.isr_results->wrapped = wrapped;

     /* Synchronise power monitor pointers (req/conf) if they go out of sync */
     if(req_conf_mismatch == TRUE)
     {
        /* reset req/conf pointers if req/conf not in sync */
        l1_sc_pwr_data_ptr->monscan.req_pos           = 0;
        l1_sc_pwr_data_ptr->monscan.cnf_pos           = 0;
        l1_sc_pwr_data_ptr->monscan.req_pos_prev      = 0;
     }

     grm_clear_registered_bands(gl1_map_gas_id_to_client_id(gas_id), gas_id);
     
     l1_sci_monscan_done_trans(gas_id);

     l1_sc_pwr_data_ptr->monscan.meas_in_progress = FALSE;

   }
#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}

/*===========================================================================

FUNCTION

DESCRIPTION
  - Redundant function. need to be removed during the cleanup.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_abort_cb_gprs_idle( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   /* restore prev req_pos to restart from same location */
   l1_sc_pwr_data_ptr->monscan.req_pos = l1_sc_pwr_data_ptr->monscan.req_pos_prev;

   l1_sci_monscan_swap_cells(gas_id);

   l1_decrement_idle_monitors_in_progress(gas_id);
   L1_LOG_VAR(l1_get_idle_monitors_in_progress(gas_id),L1_FNAME_L1_SC_PWR,L1_VAR_IDLE_MONITORS_IN_PROGRESS,gas_id);
   l1_sc_pwr_data_ptr->monscan.meas_in_progress          = FALSE;
   l1_sc_globals_ptr->monscan_in_progress = FALSE;
   L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);

   /*check if the last pwr meas abort was done while it was in SM_PWR_MEAS_RETRIEVING state
    * as then we need to suspend TRM release else the TRM release would trigger a APP mod change cmd
    * to FW while PWR meas is in progress */
#ifdef FEATURE_DUAL_SIM
   if(gl1_msg_pwr_meas_aborted_while_retrieving(gas_id))
   {
      MSG_GERAN_MED_0_G("SCE: l1_sci_monscan_abort_cb_gprs_idle(), suspend trm_release");
      /*to suspend the TRM release by 2 frames for PWR MEAS to finish*/
      l1_fm_set_suspend_trm_release( PWR_MEAS_ABORT_SUSPENSION_FRAME_DELAY,
                                     FM_PRI_RESERVED, gas_id );
   }

#endif
   return;
}
/*===========================================================================

FUNCTION

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_abort_cb_trans( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   /* restore prev req_pos to restart from same location */
   l1_sc_pwr_data_ptr->monscan.req_pos = l1_sc_pwr_data_ptr->monscan.req_pos_prev;

   l1_sci_monscan_swap_cells(gas_id);

   l1_sc_pwr_data_ptr->monscan.meas_in_progress          = FALSE;
   l1_sc_globals_ptr->monscan_in_progress = FALSE;
   L1_LOG_VAR(l1_sc_globals_ptr->monscan_in_progress,L1_FNAME_L1_SC_PWR,L1_VAR_MONSCAN_IN_PROGRESS,gas_id);

   grm_clear_registered_bands(gl1_map_gas_id_to_client_id(gas_id), gas_id);
   
#ifdef DEBUG_SCE_MSG
   #error code not present
#endif


}

/*===========================================================================

FUNCTION  l1_sci_monscan_done_trans

DESCRIPTION
  Called when a scan finishes or is aborted.
  Sets up the next scan and calls l1_sc_monscan_done().
  l1_sc_monscan_done_trans() is responsible for nulling the result
  data after use.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sci_monscan_done_trans( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   ASSERT_INT();

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif

   l1_sci_monscan_swap_cells(gas_id);

   /* callback to task context for rest of processing */
   l1_sc_pwr_data_ptr->monscan.task_results->id = l1_sc_pwr_data_ptr->monscan.id;
   l1_scdrv_call_in_task( l1_sc_monscan_done_trans, l1_sc_pwr_data_ptr->monscan.task_results, gas_id );


}


/*===========================================================================

FUNCTION  l1_sci_monscan_swap_cells

DESCRIPTION
  Swaps cnd cells and req cells so that task can process new results while
  ISR gets more.  Also brings in new cells if they are present.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_monscan_swap_cells( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   l1_sc_monscan_results_T* tmp;

   /* ISR uses req cells, task uses cnf cells. */
   l1_sc_pwr_data_ptr->monscan.cnf_cell_count = l1_sc_pwr_data_ptr->monscan.req_cell_count;
   l1_sc_pwr_data_ptr->monscan.cnf_cells      = l1_sc_pwr_data_ptr->monscan.req_cells;
   l1_sc_pwr_data_ptr->monscan.cnf_cells_id   = l1_sc_pwr_data_ptr->monscan.req_cells_id;

   /* swap result structures so new results don't overwrite old ones */
   tmp                    = l1_sc_pwr_data_ptr->monscan.task_results;
   l1_sc_pwr_data_ptr->monscan.task_results   = l1_sc_pwr_data_ptr->monscan.isr_results;
   l1_sc_pwr_data_ptr->monscan.isr_results    = tmp;

   /* if BA list has been updated then load new cells
    * into monscan
    */
   if ( l1_sc_pwr_data_ptr->monscan.new_cells_valid )
   {
      MSG_GERAN_MED_0_G("Swap/Monitor New Cells ");
      l1_sc_pwr_data_ptr->monscan.req_cell_count  = l1_sc_pwr_data_ptr->monscan.new_cell_count;
      l1_sc_pwr_data_ptr->monscan.req_cells       = l1_sc_pwr_data_ptr->monscan.new_cells;
      l1_sc_pwr_data_ptr->monscan.req_cells_id    = l1_sc_pwr_data_ptr->monscan.new_cells_id;
      l1_sc_pwr_data_ptr->monscan.req_pos         = 0;
      l1_sc_pwr_data_ptr->monscan.cnf_pos         = 0;
      l1_sc_pwr_data_ptr->monscan.req_pos_prev    = 0;

#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
      l1_sc_pwr_data_ptr->monscan.req_pos_coex_unprotected = 0;
      l1_sc_pwr_data_ptr->monscan.cnf_pos_coex_unprotected = 0;
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS */

      l1_sc_pwr_data_ptr->monscan.new_cells_valid = FALSE;
      l1_sc_pwr_data_ptr->monscan.num_passes      = 0;
   }
}

/**********************************************************************
 *
 *  MODULE:   Monscan Done
 *  -------
 *  Takes results from monscan and puts them in the BA list.
 *  Initiates sorting of BA list and calls back to SCE main
 *  to signal end of measurements / reporting period.
 *
 **********************************************************************/


/*===========================================================================

FUNCTION  l1_sc_monscan_done_idle

DESCRIPTION
  Called at the end of each set of measurements in Idle Mode.  Performs the
  following actions:
  - processes the latest set of ncell measurements,
  - causes the BA list to be sorted each time a new measurement has been
    made for each cell in the BA list,
  - sends a measurement report to RR,
  - makes a callback to the SCE function for scheduling ncell receives,
  - triggers the new BA list to be picked up if one is pending.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_monscan_done_idle( void* data, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
   gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;

   int32                     n;
   l1_sc_monscan_results_T  *results;
   l1_sc_bs_T               *bs;

   ASSERT_TASK();

   results = ( l1_sc_monscan_results_T* )data;
   gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);

   if (results->id != l1_sc_pwr_data_ptr->monscan.id)
   {
      MSG_GERAN_MED_2_G("SCE: l1_sc_monscan_done_idle result id mismatch %d %d", results->id , l1_sc_pwr_data_ptr->monscan.id);
      return;
   }

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif

   if ( (l1_sc_pwr_data_ptr->monscan.cnf_cell_count > 0) &&
        (l1_sc_pwr_data_ptr->monscan.cnf_cells_id == l1_sc_balist_get_old_id(gas_id)) )
   {
      /* @@OG: Use local variable to search through the BA list
       * and filter cells.
       */
      const uint8  ba_cell_count = l1sc_get_ba_cell_count( gas_id );

      uint32    cell_count = 0;
      cell_T**  cells_pptr = l1sc_get_ba_cells( gas_id );

      /* Process latest batch of results. */
      for ( n = 0; n < l1_sc_pwr_data_ptr->monscan.cnf_cell_count; ++n )
      {
         bs = &(l1_sc_pwr_data_ptr->monscan.cnf_cells[ n ]->bs);

         /* Check whether each cell has enough
          * measurements to proceed.  Cells
          * that have just entered the BA list
          * will need some time to fill their
          * running average buffers.
          */
         if ( (bs->meas_state == L1SCMeasNone)  ||
              (l1_running_avg_get_num_meas( &(bs->ra) ) < L1_SC_PWR_MIN_NUM_MEAS_TO_SORT) )
         {
            /* Wait for more measurements. In the meantime
             * make sure cell is not used by setting to
             * 1dB below lowest acceptable rx power level.
             */
            bs->prev_rx_power    = bs->rx_power;
            bs->rx_power         = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16 - 16;
            bs->rx_power_instant = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16 - 16;

#ifdef DEBUG_SCE_MSG
            #error code not present
#endif
         }
         else
         {
            if (bs->meas_state == L1SCMeasNew)
            {
               bs->prev_rx_power    = bs->rx_power;
               bs->rx_power         = l1_running_avg_get_avg( &(bs->ra) );
               bs->rx_power_instant = l1_running_avg_get_last_meas( &(bs->ra) );
               bs->meas_state       = L1SCMeasOld;

#ifdef DEBUG_SCE_MSG
               #error code not present
#endif
            }
         }
      }

      /* @@OG: Check for adjacent channel power where the power difference
       * is higher than/equal to 13 dB and filter the cell out by setting the
       * receive power to a minimum.
       */
      if( NULL != cells_pptr )
      {
        while( cell_count < ba_cell_count )
        {
          /* Use a local comparison counter since we cant guarantee
           * the list in cells is in order of arfcn.
           */
          uint8  compare = 0;

          while( compare < ba_cell_count )
          {
             if( NULL == cells_pptr[ compare ] )
             {
               ++compare;
               continue;
             }

           /* Compare adjacent channel powers and remove accordingly */
             if( ( cells_pptr[ compare ]->bs.arfcn.band == cells_pptr[ cell_count ]->bs.arfcn.band )
#ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS
                 && (!cells_pptr[ cell_count ]->in_blacklist)
#endif
               )
             {
                if(  ( cells_pptr[ compare ]->bs.arfcn.num == (cells_pptr[ cell_count ]->bs.arfcn.num + 1) )
                   ||( ( cells_pptr[ compare ]->bs.arfcn.num + 1) == cells_pptr[ cell_count ]->bs.arfcn.num )
                  )
                {
                  if(  ( cells_pptr[ compare ]->bs.rx_power < 0 )
                     &&( cells_pptr[ cell_count ]->bs.rx_power < 0 )
                    )
                  {
                    if( (cells_pptr[ compare ]->bs.rx_power - cells_pptr[ cell_count ]->bs.rx_power) < SYS_PARAM_ADJACENT_CELL_PWR_RX_LVL_DELTA_dB_x16 )
                    {
                      MSG_GERAN_MED_3_G(" arfcn %u pwr %d filtered out from BA list ",
                                cells_pptr[ compare ]->bs.arfcn.num,
                                cells_pptr[ compare ]->bs.rx_power,
                                0 );

                      cells_pptr[ compare ]->bs.rx_power         = SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 - 16;
                      cells_pptr[ compare ]->bs.rx_power_instant = SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 - 16;
                    }
                  }
                } /* End of if arfcns are adjacent */
             }   /* End of if bands are equal */

             if( 0 == cell_count ) /* This can be optimised */
             {
               /* Now compare against the serving cell which is not
                * necessarily always in the BA list.
                */
               if( cells_pptr[ compare ]->bs.arfcn.band == l1_idle_data->campedon_cell_ARFCN.band )
               {
                 if(  ( cells_pptr[ compare ]->bs.arfcn.num == (l1_idle_data->campedon_cell_ARFCN.num + 1) )
                    ||( ( cells_pptr[ compare ]->bs.arfcn.num + 1) == l1_idle_data->campedon_cell_ARFCN.num )
                   )
                 {
                   if(  ( cells_pptr[ compare ]->bs.rx_power < 0 )
                      &&( l1_serving_cell_meas_ptr->rx_power_instant < 0 )
                     )
                   {
                     if( (cells_pptr[ compare ]->bs.rx_power - l1_serving_cell_meas_ptr->rx_power_instant) < SYS_PARAM_ADJACENT_CELL_PWR_RX_LVL_DELTA_dB_x16 )
                     {
                       MSG_GERAN_MED_3_G(" arfcn %u pwr %d filtered out from BA list ",
                               cells_pptr[ compare ]->bs.arfcn.num,
                               cells_pptr[ compare ]->bs.rx_power,
                               0 );

                       cells_pptr[ compare ]->bs.rx_power         = SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 - 16;
                       cells_pptr[ compare ]->bs.rx_power_instant = SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 - 16;

                     }
                   }
                 }
               } /* End of if bands are equal */
             }   /* End of if cell_count == 0 */

             compare++;
          }     /* End of second while loop */

          /* Continue searching through the list */
          cell_count++;
        }
      }
      /* If there is a new BA list pending then pick
       * it up now, before we sort it. New cells are
       * initialized to very low powers so they will
       * end up at the bottom.
       */
      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);
      }

      if (results->wrapped
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
        || l1_sc_globals_ptr->blacklist_update_pending
#endif
        )
      {
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
         l1_sc_globals_ptr->blacklist_update_pending = FALSE;
#endif
         l1_sc_balist_sort_restrict_ba_len(FALSE, gas_id);
      }

#if defined(FEATURE_GSM_SCE_REDUCED_NCELL_LIST_DEBUG) && defined(DEBUG_SCE_MSG)
      #error code not present
#endif

      /*
      If the top ncells power level is above the defined threshold then ncells power levels are
      acceptable and therefore the ncell power monitor shutdown mechanism should not be employed.
      The timer is restated to prevent expiry.
      Always check that there are valid cells in the sorted cell list. It is possible to have stale
      data within the sorted_cells[0]. Also make sure that the first element is not a NULL pointer.
      */
      if ( ( l1_sc_globals_ptr->sorted_cell_count != 0 ) &&
           ( l1_sc_globals_ptr->sorted_cells[0] != NULL )
         )
      {
        if ( l1_sc_globals_ptr->sorted_cells[0]->bs.rx_power > SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 )
        {
          l1_sc_ncell_monitor_timer_start(gas_id);
        }
      }

      /* Log up to BA_MAX_LEN cells */
      l1_log_ncell_ba_list(gas_id);

      /* Log auxiliary ncell measurements */
      l1_log_ncell_aux_meas(gas_id);

   }
   else
   {
      if (l1_sc_pwr_data_ptr->monscan.cnf_cells_id != l1_sc_balist_get_old_id(gas_id))
      {
         uint8 old_id = l1_sc_balist_get_old_id(gas_id);
         uint8 new_id = l1_sc_balist_get_new_id(gas_id);

         MSG_GERAN_HIGH_3_G("Monscan: ID mismatch %d, %d, %d",
                   l1_sc_pwr_data_ptr->monscan.cnf_cells_id, old_id, new_id);
      }

      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);
      }
   }

   /* send report to RR */
   l1_scdrv_mon_ind(gas_id);

}

/*===========================================================================

FUNCTION  l1_sc_monscan_done_ded

DESCRIPTION
  Called at the end of each reporting period in Dedicated Mode.  Performs the
  following actions:
  - processes the ncell measurements from the last reporting period,
  - causes the BA list to be sorted,
  - sends a measurement report to RR,
  - makes a callback to the SCE function for scheduling ncell receives,
  - triggers the new BA list to be picked up if one is pending.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_monscan_done_ded( void* data, gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

   l1_sc_bs_T              *bs;
   l1_sc_monscan_results_T *results;
   uint32                   n;

   ASSERT_TASK();

   results = (l1_sc_monscan_results_T*)data;

   /* Check if 150 frames reached since last SC start */
   if(FALSE == l1_sc_globals_ptr->start_ncell_rep_n_acq_dedi)
   {
     /* Clear the flag to accumulate power scan results till first valid report */
     l1_sc_pwr_data_ptr->sacch_rep_period_count = 0;
     if( (SUB_FN(GSTMR_GET_FN_GERAN(gas_id), l1_sc_globals_ptr->last_dedi_sc_start_FN)) >  
                       SYS_PARAM_DEDI_NCELL_ACQ_START_FN_THRESHOLD ) 
     {
       l1_sc_globals_ptr->start_ncell_rep_n_acq_dedi = TRUE;
       l1_sc_pwr_data_ptr->sacch_rep_period_count = 1;
     }
   }

   if (results->id != l1_sc_pwr_data_ptr->monscan.id)
   {
      /* Monscan must have aborted - bail out now. */
      MSG_GERAN_MED_2_G("SCE: l1_sc_monscan_done_ded result id mismatch %d %d", results->id , l1_sc_pwr_data_ptr->monscan.id);
      return;
   }

   if ( (l1_sc_pwr_data_ptr->monscan.cnf_cell_count > 0) &&
        (l1_sc_pwr_data_ptr->monscan.cnf_cells_id == l1_sc_balist_get_old_id(gas_id)) )
   {
      GL1_ISR_LOCK(gas_id);

      MSG_GERAN_HIGH_1_G("l1_sc_monscan_done_ded cell_count=%d" , l1_sc_pwr_data_ptr->monscan.cnf_cell_count );

     if(TRUE == l1_sc_globals_ptr->start_ncell_rep_n_acq_dedi)
     {
      /* Sort the cells only every 2 SACCH block periods as mandated in the specs */
      for ( n = 0; n < l1_sc_pwr_data_ptr->monscan.cnf_cell_count; ++n )
      {
         bs                 = &(l1_sc_pwr_data_ptr->monscan.cnf_cells[ n ]->bs);
         bs->prev_rx_power  = bs->rx_power;
         if ( ( bs->rx_power_count - bs->prev_rx_power_count ) > 0 )
         {
            bs->rx_power = (dBx16_T)(( bs->rx_power_sum - bs->prev_rx_power_sum ) /
                                     ( bs->rx_power_count - bs->prev_rx_power_count ));
         }
         else
         {
            bs->rx_power = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;
         }

         bs->prev_rx_power_sum   = bs->rx_power_sum;
         bs->prev_rx_power_count = bs->rx_power_count;
      }
     }

      /* If there is a new BA list pending then pick
       * it up now, before we sort it. New cells are
       * initialized to very low powers so they will
       * end up at the bottom.
       */
      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);
      }

      l1_sc_balist_sort( TRUE, gas_id );

      l1_scdrv_mon_ind(gas_id);

      if ( l1_sc_pwr_data_ptr->sacch_rep_period_count++ )
      {
         for ( n = 0; n < l1_sc_pwr_data_ptr->monscan.cnf_cell_count; ++n )
         {
            bs                 = &(l1_sc_pwr_data_ptr->monscan.cnf_cells[ n ]->bs);
            if ( bs->rx_power_count > 0 )

            {
               bs->rx_power = (dBx16_T)(bs->rx_power_sum / bs->rx_power_count);
            }
            else
            {
               bs->rx_power = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;
            }

            bs->rx_power_sum        = 0;
            bs->prev_rx_power_sum   = 0;
            bs->rx_power_count      = 0;
            bs->prev_rx_power_count = 0;
         }

         l1_sc_balist_sort( TRUE, gas_id );

         l1_sc_pwr_data_ptr->sacch_rep_period_count = 0;
      }

      /* Log up to BA_MAX_LEN cells */
      l1_log_ncell_ba_list(gas_id);

      l1_sc_report_period_done(gas_id);
      GL1_ISR_UNLOCK(gas_id);

   }
   else
   {
      if (l1_sc_pwr_data_ptr->monscan.cnf_cells_id != l1_sc_balist_get_old_id(gas_id))
      {
         uint8 old_id = l1_sc_balist_get_old_id(gas_id);
         uint8 new_id = l1_sc_balist_get_new_id(gas_id);

         MSG_GERAN_HIGH_3_G("Monscan: ID mismatch %d, %d, %d", l1_sc_pwr_data_ptr->monscan.cnf_cells_id, old_id, new_id);
      }

      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);
      }

      l1_scdrv_mon_ind(gas_id);
   }

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
    /* adaptive active results  */  
       /* 
        **  print adaptive metrics apm_metrics_log[]
        **  [0] = 1st burst count not-yielded
        **  [1] = 2nd burst count not-yielded
        **  [2] = 3rd burst count not-yielded
        **  [3] = total yielded count
        **  [4] = unequal arfcns count
        */
        MSG_GERAN_HIGH_5_G("APM: Monscan results found on burst: 1st=%d, 2nd=%d, 3rd=%d, yielded=%d, unequal arfcns=%d", 
                                                                apm_metrics_log[0],
                                                                apm_metrics_log[1],
                                                                apm_metrics_log[2],
                                                                apm_metrics_log[3],
                                                                apm_metrics_log[4] );
#endif

} /* l1_sc_monscan_done_ded */


/*===========================================================================

FUNCTION  l1_sc_monscan_done_gprs_idle

DESCRIPTION
  Called at the end of each set of measurements in GPRS Idle Mode.  Performs
  the following actions:
  - processes the latest set of ncell measurements,
  - causes the BA list to be sorted each time a new measurement has been
    made for each cell in the BA list,
  - sends a measurement report to RR,
  - makes a callback to the SCE function for scheduling ncell receives,
  - triggers the new BA list to be picked up if one is pending.
  - Redundant function. need to be removed during the cleanup.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_monscan_done_gprs_idle( void* data, gas_id_t gas_id )
{
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
#endif
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   int32                     n;
   l1_sc_monscan_results_T  *results;
   l1_sc_bs_T               *bs;

   ASSERT_TASK();

   results = ( l1_sc_monscan_results_T* )data;

   if (results->id != l1_sc_pwr_data_ptr->monscan.id)
   {
      MSG_GERAN_MED_2_G("SCE: l1_sc_monscan_done_gprs_idle result id mismatch %d %d", results->id , l1_sc_pwr_data_ptr->monscan.id);
      return;
   }

   if ( (l1_sc_pwr_data_ptr->monscan.cnf_cell_count > 0)  &&
        (l1_sc_pwr_data_ptr->monscan.cnf_cells_id == l1_sc_balist_get_old_id(gas_id)) )
   {
      /* Process latest batch of results. */
      for ( n = 0; n < l1_sc_pwr_data_ptr->monscan.cnf_cell_count; ++n )
      {
         bs = &(l1_sc_pwr_data_ptr->monscan.cnf_cells[ n ]->bs);

         /* Check whether each cell has enough
          * measurements to proceed.  Cells
          * that have just entered the BA list
          * will need some time to fill their
          * running average buffers.
          */

         /* Allow the cell to be sorted after 2 power monitors
          * have been performed on it. This allows the acquisitions
          * to start and prevents un-necessary delays.
          */
         if(   ( bs->meas_state == L1SCMeasNone )
             ||( bs->ra.num_meas < 2 )
           )
         {
            /* Wait for more measurements. In the meantime
             * make sure cell is not used by setting to
             * 1dB below lowest acceptable rx power level.
             */
            bs->prev_rx_power = bs->rx_power;
            bs->rx_power      = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16 - 16;

#ifdef DEBUG_SCE_MSG
            #error code not present
#endif
         }
         else
         {
            if (bs->meas_state == L1SCMeasNew)
            {
               bs->prev_rx_power = bs->rx_power;
               bs->rx_power   = l1_running_avg_get_avg( &(bs->ra) );
               bs->meas_state = L1SCMeasOld;

#ifdef DEBUG_SCE_MSG
               #error code not present
#endif
            }
         }
      }

      /* If there is a new BA list pending then pick
       * it up now, before we sort it. New cells are
       * initialized to very low powers so they will
       * end up at the bottom.
       */
      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);
      }


      if (results->wrapped 
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
         || l1_sc_globals_ptr->blacklist_update_pending
#endif
         )
      {
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
         l1_sc_globals_ptr->blacklist_update_pending = FALSE;
#endif
         l1_sc_balist_sort_restrict_ba_len( FALSE, gas_id );
      }

      /* Log up to BA_MAX_LEN cells */
      l1_log_ncell_ba_list(gas_id);

      l1_sc_report_period_done(gas_id);
   }
   else
   {

      if ( l1_sc_pwr_data_ptr->monscan.cnf_cell_count == 0)
      {
         /* There are no GPRS ncell monitors to perform this paging
           multiframe, hence WCDMA list search must be kicked
           off manually (rather than started from the rxlev callback)  */
         l1_wcdma_schedule_idle( NULL, gas_id );
      }

      if (l1_sc_pwr_data_ptr->monscan.cnf_cells_id != l1_sc_balist_get_old_id(gas_id))
      {
         uint8 old_id = l1_sc_balist_get_old_id(gas_id);
         uint8 new_id = l1_sc_balist_get_new_id(gas_id);

         MSG_GERAN_HIGH_3_G("Monscan: ID mismatch %d, %d, %d",
                   l1_sc_pwr_data_ptr->monscan.cnf_cells_id, old_id, new_id);
      }

      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);
      }
   }

   /* send report to RR */
   l1_scdrv_mon_ind(gas_id);

}

/*===========================================================================

FUNCTION

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_monscan_done_trans( void* data, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
   gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
   l1_sc_bs_T                *bs;
   uint8                      n;
   l1_sc_monscan_results_T   *results;

   ASSERT_TASK();

   results = (l1_sc_monscan_results_T*)data;

   if (results->id != l1_sc_pwr_data_ptr->monscan.id)
   {
      MSG_GERAN_MED_2_G("SCE: l1_sc_monscan_done_trans result id mismatch %d %d", results->id , l1_sc_pwr_data_ptr->monscan.id);
      return;
   }

   if ( (l1_sc_pwr_data_ptr->monscan.cnf_cell_count > 0) &&
        (l1_sc_pwr_data_ptr->monscan.cnf_cells_id == l1_sc_balist_get_old_id(gas_id)) )
   {
      /* Process latest batch of results. */
      for ( n = 0; n < l1_sc_pwr_data_ptr->monscan.cnf_cell_count; ++n )
      {
         bs = &(l1_sc_pwr_data_ptr->monscan.cnf_cells[ n ]->bs);

         /* Check whether each cell has enough
          * measurements to proceed.  Cells
          * that have just entered the BA list
          * will need some time to fill their
          * running average buffers.
          */
         if ( (bs->meas_state == L1SCMeasNone)  ||
              (l1_running_avg_get_num_meas( &(bs->ra) ) < L1_SC_PWR_MIN_NUM_MEAS_TO_SORT) )
         {
            /* Wait for more measurements. In the meantime
             * make sure cell is not used by setting to
             * 1dB below lowest acceptable rx power level.
             */
            bs->prev_rx_power = bs->rx_power;
            bs->rx_power      = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16 - 16;

#ifdef DEBUG_SCE_MSG
            #error code not present
#endif
         }
         else
         {
           if (bs->meas_state == L1SCMeasNew)
           {
             bs->prev_rx_power = bs->rx_power;
             bs->rx_power   = l1_running_avg_get_avg( &(bs->ra) );
             bs->meas_state = L1SCMeasOld;

#ifdef DEBUG_SCE_MSG
             #error code not present
#endif
            }
         }
      }

      /* If there is a new BA list pending then pick
       * it up now, before we sort it. New cells are
       * initialized to very low powers so they will
       * end up at the bottom.
       */
      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);
      }

      if (results->wrapped)
      {
         l1_sc_balist_sort( FALSE, gas_id );
         if (gpl1_gprs_meas_mode_ptr->nc_mode == FALSE)
         {
            /* Restrict the sorted cell count to 6 in Idle mode */
            if (l1_sc_globals_ptr->sorted_cell_count > 10)
            {
               l1_sc_globals_ptr->sorted_cell_count = 10;

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
               /* Use sorted list size */
               l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr = l1_sc_globals_ptr->sorted_cell_count;
#endif
            }
         }
      }

      /* Log up to BA_MAX_LEN cells */
      l1_log_ncell_ba_list(gas_id);

      l1_sc_pwr_data_ptr->send_rr_report++;

      if(l1_sc_pwr_data_ptr->send_rr_report >= 7)
      {
         l1_scdrv_mon_ind(gas_id);
         l1_sc_pwr_data_ptr->send_rr_report = 0;
      }

      l1_sc_report_period_done(gas_id);
   }
   else
   {
      if (l1_sc_pwr_data_ptr->monscan.cnf_cells_id != l1_sc_balist_get_old_id(gas_id))
      {
         uint8 old_id = l1_sc_balist_get_old_id(gas_id);
         uint8 new_id = l1_sc_balist_get_new_id(gas_id);

         MSG_GERAN_HIGH_3_G("Monscan: ID mismatch %d, %d, %d", l1_sc_pwr_data_ptr->monscan.cnf_cells_id, old_id, new_id);
      }

      if (l1_sc_pwr_data_ptr->monscan.new_balist_pending)
      {
         l1_sci_monscan_ba_list_use_new(gas_id);
      }

      l1_sc_pwr_data_ptr->send_rr_report++;

      if(l1_sc_pwr_data_ptr->send_rr_report >= 7)
      {
         l1_scdrv_mon_ind(gas_id);
         l1_sc_pwr_data_ptr->send_rr_report = 0;
      }
   }

} /* l1_sc_monscan_done_trans */

/**********************************************************************
 *
 *  Module:   MISC
 *  -------
 *
 **********************************************************************/

/*===========================================================================

FUNCTION l1_sc_rx_power_acceptable

DESCRIPTION
  Determines if the passed in rx power is above the noise floor

DEPENDENCIES
  None

RETURN VALUE
  TRUE - If power > noise floor
  FALSE - If power <= noise floor

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_sc_rx_power_acceptable( dBx16_T rx_power)
{
   return ( rx_power > SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16 );
}

/*===========================================================================

FUNCTION l1_sc_rx_power_drop_acceptable

DESCRIPTION
  Determines if the passed in rx power differential is acceptable

DEPENDENCIES
  None

RETURN VALUE
  TRUE - If power > noise floor
  FALSE - If power <= noise floor

SIDE EFFECTS
  None
===========================================================================*/
boolean l1_sc_rx_power_drop_acceptable( dBx16_T rx_power_drop)
{
   return ( rx_power_drop > SYS_PARAM_MAX_NCELL_RX_LVL_DROP_dB_x16 );
}

/**********************************************************************
 *
 *  Module:   Running average
 *  -------
 *  Support for doing running averages.
 *
 **********************************************************************/

/*===========================================================================

FUNCTION L1_RUNNING_AVG_...

DESCRIPTION
  These functions combined with the type running_avg_struct are used to
  implement running averages of power measurements.

  For each running average a seperate running_avg_struct should be declared
  and a buffer should be assigned to the struct using l1_running_avg_init.
  The buffer is not declared as part of the struct to allow its length to
  be varied.

  The length of the running average may be controlled using
  l1_running_avg_set_avg_len.  This function may even be applied to a
  buffer that is in use.  The requested average length cannot be longer
  than the length of the buffer supplied to the init function.

  New measurements are added to a running average using
  l1_running_avg_add_meas.

  An average may be retrieved with l1_running_avg_get_avg.

  The number of measurements and the sum of the measurements may be
  retrieved with l1_running_avg_get_num_meas and
  l1_running_avg_get_total respectively.

  To clear out a set of measurements and start again use
  l1_running_avg_clear_meas.

  The function l1_running_avg_is_full() can be used to test whether
  the running average buffer is full.

  The function l1_running_avg_stuff_buf() can be used to fill a
  running average buffer with a given value.  The length of the
  average (number of samples to average over) is not changed by
  this function.

  The function l1_running_avg_sanity_check_meas_buf() is used to check
  that meas_buf is pointing to a valid running average structure. If the
  pointer address is invalid, the ba.cell_list running average meas_buf
  pointers are reinitialised. This is required for CR242536 and is only
  applicable for Q6 targets.
 
  The function l1_running_avg_get_last_num_meas calculates average rssi of
  last number of measurements specified as a input parameter. 
DEPENDENCIES
  Each running average struct must be initialized before it is used by
  calling l1_running_avg_init(). This function is used to assign the
  buffer memory to the struct.  If this is not done memory will get
  trashed.

RETURN VALUE
  na

SIDE EFFECTS
  None
===========================================================================*/

void l1_running_avg_init( running_avg_struct *ra_ptr,
                          dBx16_T *buf, 
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
                          dBx16_T *divrx_buf,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
                          uint8 buf_len, 
                          gas_id_t gas_id)
{
  GL1_ISR_LOCK(gas_id);
  ra_ptr->meas_buf = buf;
  ra_ptr->buf_len  = buf_len;
  ra_ptr->avg_len  = buf_len;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  ra_ptr->divrx_meas_buf = divrx_buf;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

  l1_running_avg_clear_meas( ra_ptr );
  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION l1_running_avg_set_avg_len

DESCRIPTION
  Set maximum number of measurements.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_running_avg_set_avg_len( running_avg_struct *ra_ptr, 
                                 uint8 avg_len, 
                                 gas_id_t gas_id )
{
  dBx16_T  power = GL1_INVALID_POWER_LEVEL, divrx_power = GL1_INVALID_POWER_LEVEL;
  dBx16_T *ptr;
  boolean rxd = FALSE;

  /* sanity check meas_buf is valid - CR242536 */
  l1_running_avg_sanity_check_meas_buf(ra_ptr, gas_id);

  if (avg_len > ra_ptr->buf_len) 
  {
    avg_len = ra_ptr->buf_len;
    MSG_GERAN_ERROR_0_G("Buffer too short for avg_len.");
  }

#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
  if ((gl1_get_MSRD_ctl_nv(gas_id) || (gl1_get_IMRD_ctl_nv(gas_id)))
      && (ra_ptr->divrx_meas_buf))
  {
    rxd = TRUE;
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

  GL1_ISR_LOCK(gas_id);

  /* If the buffer was just stuffed with a value then
   * re-stuff it.  This makes SCE mode changes more
   * efficient.
   */
  /* just_stuffed does not set TRUE for CCCH measurement.
     No need to handle DivRx Power*/
  if (ra_ptr->just_stuffed) 
  {
    power = ra_ptr->meas_buf[0];

    if (rxd)
    {
      divrx_power = ra_ptr->divrx_meas_buf[0];
    }

    if (avg_len > ra_ptr->avg_len) 
    {
      /* Add more measurements to buffer. */
      ptr   = &(ra_ptr->meas_buf[ra_ptr->newest_ptr]);
      l1_running_stuff_avg_pwr(ptr, power, (avg_len-ra_ptr->avg_len));
      if (rxd)
      {
        ptr   = &(ra_ptr->divrx_meas_buf[ra_ptr->newest_ptr]);
        l1_running_stuff_avg_pwr(ptr, divrx_power, (avg_len-ra_ptr->avg_len));
      }
    }


    ra_ptr->avg_len      = avg_len;
    ra_ptr->num_meas     = avg_len;
    ra_ptr->total        = avg_len * (int32)power;

    if (rxd)
    {
      ra_ptr->divrx_total        = avg_len * (int32)divrx_power;
    }

    if (avg_len < ra_ptr->buf_len) 
    {
      ra_ptr->newest_ptr = avg_len;
    }
    else 
    {
      ra_ptr->newest_ptr = 0;
    }
  }
  else 
  {
    ra_ptr->avg_len = avg_len;

    /* meas_buf will be updated next time we add
       a measurement.*/
  }
  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION l1_running_add_prx_meas

DESCRIPTION
  Add PRX measurement in the running average buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_running_add_prx_meas(running_avg_struct *ra_ptr,
                             dBx16_T meas,
                             uint8 index,
                             gas_id_t gas_id)
{
  if (ra_ptr->meas_buf) 
  {
    ra_ptr->meas_buf[index] = meas; 
    /* add meas to total */
    ra_ptr->total += meas;
  }
  else
  {
    MSG_GERAN_ERROR_0_G("Unable to add prx meas");
  }
  return;
}

/*===========================================================================

FUNCTION l1_running_add_divrx_meas

DESCRIPTION
  Add DivRX measurement in the running average buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_running_add_divrx_meas(running_avg_struct *ra_ptr,
                               dBx16_T meas,
                               uint8 index,
                               gas_id_t gas_id)
{
  GL1_ISR_LOCK(gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if (ra_ptr->divrx_meas_buf) 
  {
    ra_ptr->divrx_meas_buf[index] = meas; 
    /* add meas to total */
    ra_ptr->divrx_total += meas;
  }
  else
  {
    MSG_GERAN_ERROR_0_G("Unable to add divrx meas");
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
  GL1_ISR_UNLOCK(gas_id);
  return;
}

/*===========================================================================

FUNCTION l1_running_remove_prx_meas

DESCRIPTION
  Remove PRX measurement from the running average buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_running_remove_prx_meas(running_avg_struct *ra_ptr,
                                uint8 index,
                                gas_id_t gas_id)
{
  if (ra_ptr->meas_buf) 
  {
    ra_ptr->total -= ra_ptr->meas_buf[index];
  }
  else
  {
    MSG_GERAN_ERROR_0_G("Unable to remove prx meas");
  }
  return;
}

/*===========================================================================

FUNCTION l1_running_remove_divrx_meas

DESCRIPTION
  Remove DivRX measurement from the running average buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_running_remove_divrx_meas(running_avg_struct *ra_ptr,
                                  uint8 index,
                                  gas_id_t gas_id)
{
  GL1_ISR_LOCK(gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if (ra_ptr->divrx_meas_buf) 
  {
    ra_ptr->divrx_total -= ra_ptr->divrx_meas_buf[index];
  }
  else
  {
    MSG_GERAN_ERROR_0_G("Unable to remove divrx meas");
  }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
  GL1_ISR_UNLOCK(gas_id);
  return;
}

/*===========================================================================

FUNCTION l1_running_avg_add_meas

DESCRIPTION
  Add measurement in the running average buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_running_avg_add_meas( running_avg_struct *ra_ptr, 
                              dBx16_T meas,
                              dBx16_T divrx_meas,
                              gl1_power_meas_type meas_type,
                              gas_id_t gas_id )
{
 
  /* sanity check meas_buf is valid - CR242536 */
  l1_running_avg_sanity_check_meas_buf(ra_ptr, gas_id);

  if (ra_ptr->newest_ptr >= ra_ptr->buf_len)
  {
    ra_ptr->newest_ptr = 0;
    MSG_GERAN_ERROR_0_G("Running average buffer overflow!");
  }

  switch (meas_type) 
  {
    case PRX_MEAS:
      l1_running_add_prx_meas(ra_ptr, meas, ra_ptr->newest_ptr, gas_id);
      break;

    case DIVRX_MEAS:
      l1_running_add_divrx_meas(ra_ptr, divrx_meas, ra_ptr->newest_ptr, gas_id);
      break;

    case PRX_DIVRX_MEAS:
      l1_running_add_prx_meas(ra_ptr, meas, ra_ptr->newest_ptr, gas_id);
      l1_running_add_divrx_meas(ra_ptr, divrx_meas, ra_ptr->newest_ptr, gas_id);
      break;

    default:
      MSG_GERAN_ERROR_1_G("Unknown meas_type %d", meas_type);
      break;
  }

  /* increment write ptr */
  ra_ptr->newest_ptr++;
  if (ra_ptr->newest_ptr >= ra_ptr->buf_len) 
  {
    ra_ptr->newest_ptr = 0;
  }

  ra_ptr->num_meas++;

  /* throw away old measurement if required                        */
  /* done this way to support changing avg_len on the fly          */
  while ( ra_ptr->num_meas > ra_ptr->avg_len ) 
  {
    /* remove oldest meas from total */
    switch (meas_type) 
    {
      case PRX_MEAS:
        l1_running_remove_prx_meas(ra_ptr, ra_ptr->oldest_ptr, gas_id);
        break;

      case DIVRX_MEAS:
        l1_running_remove_divrx_meas(ra_ptr, ra_ptr->oldest_ptr, gas_id);
        break;

      case PRX_DIVRX_MEAS:
        l1_running_remove_prx_meas(ra_ptr, ra_ptr->oldest_ptr, gas_id);
        l1_running_remove_divrx_meas(ra_ptr, ra_ptr->oldest_ptr, gas_id);
        break;

      default:
        MSG_GERAN_ERROR_1_G("Unknown meas_type %d", meas_type);
        break;
    }

    /* increment end ptr */
    ra_ptr->oldest_ptr++;
    if (ra_ptr->oldest_ptr == ra_ptr->buf_len) 
    {
      ra_ptr->oldest_ptr = 0;
    }
    ra_ptr->num_meas--;
  }

  ra_ptr->just_stuffed = FALSE;

  return;
}

/*===========================================================================

FUNCTION l1_running_avg_get_avg

DESCRIPTION
  Get average of PRX measurements.

DEPENDENCIES
  None

RETURN VALUE
  Average power level.

SIDE EFFECTS
  None
===========================================================================*/
dBx16_T l1_running_avg_get_avg( running_avg_struct *ra_ptr )
{
   dBx16_T avg;

   if (ra_ptr->num_meas > 0) {
      avg = (dBx16_T)(ra_ptr->total / ra_ptr->num_meas);
   }
   else {
      MSG_GERAN_ERROR_0("Running Avg: no meas. to average.");
      avg = 0;
   }

   if (avg > 0) {
      MSG_GERAN_ERROR_1("Suspicous avg %d", avg);
   }

   return avg;
}

/*===========================================================================

FUNCTION l1_running_avg_get_divrx_avg

DESCRIPTION
  Get average of DivRX measurements.

DEPENDENCIES
  None

RETURN VALUE
  Average power level.

SIDE EFFECTS
  None
===========================================================================*/
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA) 
dBx16_T l1_running_avg_get_divrx_avg( running_avg_struct *ra_ptr )
{
  dBx16_T avg;

  if ((ra_ptr->divrx_meas_buf) && (ra_ptr->num_meas > 0)) 
  {
    avg = (dBx16_T)(ra_ptr->divrx_total / ra_ptr->num_meas);
  }
  else 
  {
    MSG_GERAN_ERROR_0("Running Avg: no meas. to average.");
    avg = 0;
  }

  if (avg > 0) 
  {
    MSG_GERAN_ERROR_1("Suspicous avg %d", avg);
  }

  return avg;
}
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/

/*===========================================================================

FUNCTION l1_running_avg_get_num_meas

DESCRIPTION
  Get number of measurements in the buffer.

DEPENDENCIES
  None

RETURN VALUE
  Number of measurements.

SIDE EFFECTS
  None
===========================================================================*/
uint8 l1_running_avg_get_num_meas( running_avg_struct *ra_ptr )
{
   return ra_ptr->num_meas;
}

/*===========================================================================

FUNCTION l1_running_avg_get_total

DESCRIPTION
  Get sum of measurements values in the buffer.

DEPENDENCIES
  None

RETURN VALUE
  Total of measurement values.

SIDE EFFECTS
  None
===========================================================================*/
int32 l1_running_avg_get_total( running_avg_struct *ra_ptr )
{
   return ra_ptr->total;
}

/*===========================================================================

FUNCTION l1_running_avg_clear_meas

DESCRIPTION
  Clear measurement buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_running_avg_clear_meas( running_avg_struct *ra_ptr )
{
   ra_ptr->num_meas     = 0;
   ra_ptr->oldest_ptr   = 0;
   ra_ptr->newest_ptr   = 0;
   ra_ptr->total        = 0;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   ra_ptr->divrx_total  = 0;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
   ra_ptr->just_stuffed = FALSE;
}

/*===========================================================================

FUNCTION l1_running_avg_is_full

DESCRIPTION
  Return the status of avergae measurement buffer.

DEPENDENCIES
  None

RETURN VALUE
  TRUE -> Number of measurements equals to avg length of the buffer.
  FALSE -> else
 
SIDE EFFECTS
  None
===========================================================================*/
boolean l1_running_avg_is_full( running_avg_struct *ra_ptr )
{
   if (ra_ptr->num_meas == ra_ptr->avg_len) {
      return TRUE;
   }
   else {
      return FALSE;
   }
}

/*===========================================================================

FUNCTION l1_running_stuff_avg_pwr

DESCRIPTION
  Populate buffer with the passed measurement value.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_running_stuff_avg_pwr(dBx16_T *meas_buf_ptr, dBx16_T power, int32 avg_len)
{
  dBx16_T *ptr = meas_buf_ptr;
  int32 i = 0;
  if (ptr) 
  {
    for (i = avg_len; i != 0; i--) 
    {
      *ptr = power;
      ptr++;
    }
  }
  return;
}

/*===========================================================================

FUNCTION l1_running_avg_stuff_buf

DESCRIPTION
  Populate buffer with the passed measurement value.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
/* This function is not used for camped cell power measurement average on CCCH.
   Therefore no need to handle Div Rx*/
void l1_running_avg_stuff_buf( running_avg_struct *ra_ptr, 
                               dBx16_T power, 
                               dBx16_T divrx_power,
                               gl1_power_meas_type meas_type,
                               gas_id_t gas_id )
{
  int32    avg_len;

  GL1_ISR_LOCK(gas_id);

  /* sanity check meas_buf is valid - CR242536 */
  l1_running_avg_sanity_check_meas_buf(ra_ptr, gas_id);

  /* Clear out the buffer */
  ra_ptr->oldest_ptr = 0;

  /* Now fill buffer to the previous length with
   * the supplied value.
   */

  avg_len = ra_ptr->avg_len;

  switch (meas_type) 
  {
    case PRX_MEAS:
      l1_running_stuff_avg_pwr(ra_ptr->meas_buf, power, avg_len);
      ra_ptr->total        = avg_len * (int32)power;
      break;

    case DIVRX_MEAS:
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
      l1_running_stuff_avg_pwr(ra_ptr->divrx_meas_buf, divrx_power, avg_len);
      ra_ptr->divrx_total        = avg_len * (int32)divrx_power;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
      break;

    case PRX_DIVRX_MEAS:
      l1_running_stuff_avg_pwr(ra_ptr->meas_buf, power, avg_len);
      ra_ptr->total        = avg_len * (int32)power;
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined(FEATURE_GSM_RX_DIVERSITY_DATA)
      l1_running_stuff_avg_pwr(ra_ptr->divrx_meas_buf, divrx_power, avg_len);
      ra_ptr->divrx_total        = avg_len * (int32)divrx_power;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE || FEATURE_GSM_RX_DIVERSITY_DATA*/
      break;

    default:
      MSG_GERAN_ERROR_1_G("Unknown meas_type %d", meas_type);
      break;
  }

  ra_ptr->avg_len      = (uint8)avg_len;
  ra_ptr->num_meas     = (uint8)avg_len;
  ra_ptr->just_stuffed = TRUE;

  if (avg_len < ra_ptr->buf_len) 
  {
    ra_ptr->newest_ptr = (uint8)avg_len;
  }
  else 
  {
    ra_ptr->newest_ptr = 0;
  }

  GL1_ISR_UNLOCK(gas_id);

  return;
}

/*===========================================================================

FUNCTION l1_running_avg_get_last_meas

DESCRIPTION
  Return last measurement value.

DEPENDENCIES
  None

RETURN VALUE
  Power level of the last measurement.

SIDE EFFECTS
  None
===========================================================================*/
dBx16_T l1_running_avg_get_last_meas( running_avg_struct *ra_ptr )
{
   uint32 i;

   if ( ra_ptr->num_meas > 0 ) {
      i = ra_ptr->newest_ptr;

      if ( i==0 ) {
         i = ra_ptr->buf_len - 1;
      }
      else {
         i -= 1;
      }

      return ra_ptr->meas_buf[i];

   }
   else {
      return SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;
   }
}

/*===========================================================================

FUNCTION l1_running_avg_get_last_num_meas

DESCRIPTION
  Calculate average power of last number of measurements specified in the
  input parameter.

DEPENDENCIES
  None

RETURN VALUE
  Average power level of last num of measurements.

SIDE EFFECTS
  None
===========================================================================*/
dBx16_T l1_running_avg_get_last_num_meas(running_avg_struct *ra_ptr, uint8 num_of_meas, gas_id_t gas_id)
{
  uint8 latest_meas_idx = 0, oldest_meas_idx = 0, i = 0, total_meas = 0;
  dBx16_T power = SYS_PARAM_MIN_ACCEPTABLE_NCELL_RX_LVL_dBm_x16;

  if (ra_ptr) 
  {
    if((ra_ptr->num_meas > 0) && (num_of_meas <= ra_ptr->avg_len) 
       && (num_of_meas <= ra_ptr->buf_len) && (num_of_meas != 0)) 
    {
      /* newest_ptr points to the next empty/old meas buf index*/
      /* For example: Five(avg_len) measurements in the buffer of length 10.
         E->Empty L-> Latest O->Old
         O, O, L, L, L, L, L, E, E, E
         newest_ptr = 7, oldest_ptr = 2
         */
      power = 0;
      latest_meas_idx = ra_ptr->newest_ptr;
      oldest_meas_idx = ra_ptr->oldest_ptr;

      for(i = 0; i < num_of_meas; i++) 
      {
        /* Find head of the circular buffer*/
        if(0 == latest_meas_idx) 
        {
          latest_meas_idx = ra_ptr->buf_len - 1;
        }
        else
        {
          latest_meas_idx--;
        }
        
        power += ra_ptr->meas_buf[latest_meas_idx];
        total_meas++;

        /* head of the circular buffer is equal to the
           tail. It means no more valid measurements are left*/
        if (latest_meas_idx == oldest_meas_idx) 
        {
          break;
        }
      }
      power /= (dBx16_T)(total_meas);
    }
    else 
    {
      MSG_GERAN_HIGH_4_G("No measurement done or invalid num_of_meas %d"
                           "num_meas %d avg_len %d buf_len %d ",
                           num_of_meas, 
                           ra_ptr->num_meas, 
                           ra_ptr->avg_len,
                           ra_ptr->buf_len);
    }
  }
  else
  {
    MSG_GERAN_ERROR_0_G("Running average pointer is NULL");
  }
  return power;
}
/*===========================================================================

FUNCTION  l1_sci_monscan_state_debug_msg()

DESCRIPTION
  Print a debug message showing the state of the monscan module.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_sci_monscan_state_debug_msg( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   MSG_GERAN_ERROR_4_G("monscan: active=%d, r_c_cnt=%d, meas_i_p=%d, gl1_msg_meas_in_progress=%d",
             l1_sc_pwr_data_ptr->monscan.active,
             l1_sc_pwr_data_ptr->monscan.req_cell_count,
             l1_sc_pwr_data_ptr->monscan.meas_in_progress,
             gl1_msg_meas_in_progress(gas_id));
}

/*===========================================================================
FUNCTION l1_sc_ncell_monitor_timer_handler

DESCRIPTION
        Handlers the L1_NCELL_MONITOR_SHUTDOWN_TIMER_SIG, and sets the correct
        state upon this.

===========================================================================*/

static void l1_sc_ncell_monitor_timer_handler( unsigned long gas_id )
{
  l1_sc_pwr_data_t *loc_l1_sc_pwr_data_ptr = &l1_sc_pwr_data[(int)gas_id];

  switch ( loc_l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state )
  {
    case NCELL_PCH_MONITOR_NORMAL:
      MSG_GERAN_HIGH_0_G( "ncell monitor shutdown timer EXPIRED");

      loc_l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state = NCELL_PCH_TIMER_EXPIRED;

      l1_sc_ncell_monitor_timer_deactivate( loc_l1_sc_pwr_data_ptr );
    break;

    case NCELL_PCH_TIMER_EXPIRED:
    break;

    default:
      MSG_GERAN_ERROR_1_G( "ncell pch shutdown state %d unknown",
                 loc_l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state);
    break;
  }
}

/*===========================================================================
FUNCTION l1_sc_ncell_monitor_timer_start

DESCRIPTION
  Starts the ncell monitor shutdown timer for the time period defined by
  L1_NCEll_PCH_SHUTDOWN_TIMEOUT
===========================================================================*/
void l1_sc_ncell_monitor_timer_start( gas_id_t gas_id )
{
  l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

  if ( l1_sc_pwr_data_ptr )
  {
    if ( l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state == NCELL_PCH_TIMER_EXPIRED )
    {
      MSG_GERAN_HIGH_0_G( "Leaving ncell_monitor_shutdown_state");
    }

    l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state = NCELL_PCH_MONITOR_NORMAL;

    (void)rex_set_timer( &l1_sc_pwr_data_ptr->l1_pch_ncell_monitor_shutdown,
                         L1_NCEll_PCH_SHUTDOWN_TIMEOUT );
  }
}

/*===========================================================================
FUNCTION l1_sc_init_ncell_monitor_timer

DESCRIPTION
  creates/defines the ncell monitor shutdown timer
===========================================================================*/
void l1_sc_init_ncell_monitor_timer( gas_id_t gas_id )
{
  l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

  /* Create a timer for ncell monitor shutdown */
  memset(&(l1_sc_pwr_data_ptr->l1_pch_ncell_monitor_shutdown), 0, sizeof(rex_timer_type));
  rex_def_timer_ex( &(l1_sc_pwr_data_ptr->l1_pch_ncell_monitor_shutdown),
                    (rex_timer_cb_type)l1_sc_ncell_monitor_timer_handler,
                    (unsigned long)gas_id );
}

/*===========================================================================
FUNCTION l1_sc_ncell_monitor_timer_deactivate

DESCRIPTION
  Deactivates the ncell monitor shutdown timer
===========================================================================*/
static void l1_sc_ncell_monitor_timer_deactivate(
             l1_sc_pwr_data_t *loc_l1_sc_pwr_data_ptr )
{
  if ( loc_l1_sc_pwr_data_ptr )
  {
    /* Disable the timer */
    (void)rex_clr_timer( &loc_l1_sc_pwr_data_ptr->l1_pch_ncell_monitor_shutdown );
  }
}

/*===========================================================================
FUNCTION l1_sc_ncell_pch_low_monitor_active

DESCRIPTION
  Tell the drivers layer if we are in the low monitor state for asynch
  interrupt sleep handling
===========================================================================*/
boolean l1_sc_ncell_pch_low_monitor_active( gas_id_t gas_id )
{
  l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

  boolean ncell_pch_low_monitor_active = FALSE;

  if ( l1_sc_pwr_data_ptr &&
       ( NCELL_PCH_TIMER_EXPIRED == l1_sc_pwr_data_ptr->ncell_monitor_shutdown_state )
     )
  {
    ncell_pch_low_monitor_active = TRUE;
  }

  return ( ncell_pch_low_monitor_active );
}

/*===========================================================================
FUNCTION l1_sc_monscan_is_next_idle_scan_interband

DESCRIPTION
  This function determines if the next IDLE monscan will contain interband
  cells (cells on a band different to the serving BCCH ARFCN).

  This is similar to l1_sc_monscan_idle_check_for_inter_band_cells()
  except that :

  1.) The interband determination is made taking into account the actual
      number of monitors to be scheduled on the next CCCH (which can vary due to
      IDLE monitor skipping feature etc) rather than assuming that the max number
      of monitors (SYS_PARAM_NUM_MEASUREMENTS_PER_FRAME_NCELL_PWR_MON) will be
      scheduled.

  2.) There is no updating of the denied bands list.
  
===========================================================================*/
boolean l1_sc_monscan_is_next_idle_scan_interband(gas_id_t  gas_id )
{
  l1_sc_globals_T  *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
  uint8 num_meas, meas_per_frame, i;
  uint8 cell_index = l1_sc_pwr_data_ptr->monscan.req_pos;
  idle_data_T  *l1_idle_data_ptr = &l1_idle_data_store[gas_id];
  ARFCN_T arfcn;
  boolean rc = FALSE;

  if( (l1_sc_globals_ptr->mode == L1SCModeIdle) || (l1_sc_globals_ptr->mode == L1SCModeGprsIdle) )
  {
     /* Number of measurements to be scheduled on the next CCCH block */
     (void)l1_sc_monscan_get_num_idle_meas(&num_meas, &meas_per_frame, gas_id);

     if(l1_sc_pwr_data_ptr->monscan.req_cell_count > 0)
     {
        for (i = 0; i < num_meas; i++)
        {
           arfcn = l1_sc_pwr_data_ptr->monscan.req_cells[cell_index]->bs.arfcn;
           if(arfcn.band != l1_idle_data_ptr->campedon_cell_ARFCN.band)
           {
              rc = TRUE;
              break;
           }
           cell_index++;
           cell_index %= l1_sc_pwr_data_ptr->monscan.req_cell_count;
        }
     }
  }

  return rc;
}


/*===========================================================================
FUNCTION l1_sci_monscan_ba_list_use_new

DESCRIPTION
  Used to signal that it is safe to pick up the new BA lis and notifies the
  arbitration interface that a new BA list has been loaded into monscan.
===========================================================================*/
static void l1_sci_monscan_ba_list_use_new(gas_id_t gas_id)
{
  l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

  l1_sc_balist_use_new(gas_id);
  l1_sc_pwr_data_ptr->monscan.new_balist_pending = FALSE;

#ifdef FEATURE_GSM_COEX_SW_CXM
  garb_intf_notify_monscan_ba_list(
     l1_sc_pwr_data_ptr->monscan.req_cells,
     l1_sc_pwr_data_ptr->monscan.req_cell_count,
    &l1_sc_pwr_data_ptr->is_coex_protected_monitor,
     gas_id  );

#endif /* FEATURE_GSM_COEX_SW_CXM */
}

/*===========================================================================
FUNCTION l1_sci_monscan_meas_in_progress

DESCRIPTION
  Used to signal that monscan.meas_in_progress is set.
===========================================================================*/
boolean l1_sci_monscan_meas_in_progress(gas_id_t gas_id)
{
  l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

  return (l1_sc_pwr_data_ptr->monscan.meas_in_progress);
}


/*===========================================================================

FUNCTION  l1_sc_monscan_is_not_active

DESCRIPTION
  Tells if monscan is running currently

DEPENDENCIES
  None

RETURN VALUE
  TRUE of monscan is not running currently

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_monscan_is_not_active( gas_id_t gas_id )
{
   l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];

   if ( ( l1_sc_pwr_data_ptr->monscan.active ) && (!l1_sc_pwr_data_ptr->monscan.meas_in_progress) )
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/*===========================================================================

FUNCTION  l1_sc_monscan_idle_check_for_inter_band_cells

DESCRIPTION
  Checks if we're likely to measure non-serving band cells in next PCH period. This API should only be called in 
  DR mode. API used as part of persistent denial algorithm

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_monscan_idle_check_for_inter_band_cells( gas_id_t gas_id )
{
    l1_sc_pwr_data_t *l1_sc_pwr_data_ptr = &l1_sc_pwr_data[gas_id];
    cell_T*       cell;
    int i; 
    int j = l1_sc_pwr_data_ptr->monscan.req_pos; 
    grm_client_enum_t client_id = gl1_map_gas_id_to_client_id(gas_id);

    if (client_id == GRM_NO_CLIENT )
    {
        return;
    }
    
    if(l1_sc_pwr_data_ptr->monscan.req_cell_count > 0)
    {
      for (i = 0; i < SYS_PARAM_NUM_MEASUREMENTS_PER_FRAME_NCELL_PWR_MON; i++)
      {

        if (j >= l1_sc_pwr_data_ptr->monscan.req_cell_count)
        {
          j = 0; 
        }                     
        cell = l1_sc_pwr_data_ptr->monscan.req_cells[j];
        j++; 
          
        /*is this a denied band?*/   
        if (grm_is_denied_band(cell->bs.arfcn.band,gl1_map_gas_id_to_client_id(gas_id),gas_id))
        {
         /*
                 Set flag in the GRM interface that the denied band is due to be measured in next PCH block.
                 If the conditions are met for pesrsistent denial then the band will be added to the next TRM request for DEMOD page
               */

#ifdef FEATURE_PERSISTENT_DENIAL_DEBUG
         MSG_GERAN_HIGH_2_G (GL1_DRDS_DBG_HDR": ARFCN %d from denied band %d to be measured in next PCH mon scan",cell->bs.arfcn.num,cell->bs.arfcn.band);
#else 
         MSG_GERAN_LOW_2_G (GL1_DRDS_DBG_HDR": ARFCN %d from denied band %d to be measured in next PCH mon scan",cell->bs.arfcn.num,cell->bs.arfcn.band);
#endif 
         grm_set_denied_band_in_next_mon_scan_flag(cell->bs.arfcn.band,client_id,gas_id);

        }
        
      }
    }

}

#ifdef FEATURE_GSM_COEX_SW_CXM
/*===========================================================================

FUNCTION l1_report_lowest_power_per_band

DESCRIPTION
  The function reports lowest power per valid band to CXM for COEX Mitigation.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_coex_report_lowest_power_per_band(int32 *pwr_per_band, gas_id_t gas_id)
{
  uint8 sys_band = 0;

  for(sys_band=0 ; sys_band < INVALID_BAND ; sys_band++)
  {
    if( pwr_per_band[sys_band] != 0)
    {
       garb_cxm_band_power_ind((sys_band_T)sys_band, pwr_per_band[sys_band], gas_id);
    }
  }
}
#endif /* FEATURE_GSM_COEX_SW_CXM */

/* EOF */
