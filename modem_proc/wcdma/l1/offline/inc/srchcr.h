#ifndef SRCHCR_H
#define SRCHCR_H
/*===========================================================================
   S R C H     C E L L     R E S E L E C T I O N     H E A D E R    F I L E

DESCRIPTION
   This file contains procedures declarations pertinent to WCDMA
   cell reselection evaluation.

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  srchcr_init() should be called once we enter DRX mode.

REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2001-2013 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/l1/vcs/srchcr.h_v   1.5   11 Jul 2002 21:29:46   halbhavi  $
$Header: //components/rel/wcdma.mpss/8.4.0/l1/offline/inc/srchcr.h#3 $ $DateTime: 2020/01/22 08:56:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------\
01/10/20   ac      Mini dump framework checkin
12/11/19    ac      Searcher Mini Dump Change
09/15/17   gmr     Avoid using gap res eval meas pending for filling log packet srch duration
08/30/17   gmr     FR 38044: W2L unified DETECT and MEAS scheduling
06/29/16   nga     QSH Event reporting
05/20/16   vs      Force Eval will now account for measurement time towards tresel
05/13/16   gsk     Force entity to sleep when GSM search is on-going
04/18/16   nga     QSH resel handlers
04/07/16   nga     Code clean up for FEATURE_DELAY_3G_TO_2G_RESEL
04/05/16   nga     0x4005 & 0x4027 enahncements- adding 'resel_status'
04/01/16    zt      Idle meas log pkt 0x41CF Version 2
02/24/16   zt      Code clean up for FEATURE_DELAY_3G_TO_2G_RESEL
03/31/16   zt      Support for new log pkt 0x41CF
03/03/16   nga     Add Check abort timer
02/18/16   nga     Force rank all W cells if at least one  W frequency is measured
09/22/15    us     Fixed compilation errors got after enabling ATLAS and RFM_PATH feature
10/16/15   kr      DOnt wakeup PICH if BSIC is ongoing
10/07/15   vs      BSIC Id Reconf optimizations for X+W QTA scenarios.
08/28/15   nga     Set res-eval_meas_comp flag to intra,inter& IRAT before issuing EcIo ranking as part of finding next best cell      
08/26/15   gsk     CSG + MSIM changes
08/26/15   nga     Avoiiding wrong increment of Tresel by avoiding redundant ranking
08/07/15   zt      Double the resel log packets for W+W
06/29/15   nga     Avoid setting bch on for duplicate cell resel indication.
                   Removing the use of unused global.
06/17/15   nga     Check if inter-f nbrs measured before issuing RSCP ranking, 
                   seaparately for F0 & F1. Also maintain separate Tresel timers 
                   for F0, F1 of inter-f nbrs
06/16/15   nga     Update EcIo & EcIo rank fields of log packet 0x4005 appropriately
                   No need of CR 835198 anymore, required changes of this CR are 
                   clubbed in current CR.
05/19/15   sks     Changes to support PICH and measurements in L/W/G + /G.
03/19/15   gsk     New Idle mode unified neighbor scheduler   
03/05/15   psr     Move coefficient arrays to const type
02/28/15   rsr     W + W Phase 2 Checkin.
02/20/15   nga     Removing Round Robin Algorithm to issue panic searches in G2W TA
02/06/15   psr     Compiler error fixes for Dual sim
01/20/15    jd      Global re-org for W + W
01/19/15   psr     Global re-org for W+W
01/05/15   sks     Compilation fix.
12/15/14   psr     Thor compilation fixes on top of srchcr.h inclusion cleanup
12/11/14   psr     srchcr.h inclusion cleanup
11/26/14   psr     TH Compilation Fixes - externed functions and added feature definitions
11/19/14   gsk     FACH FMO log packet and adding support for 0x4027 for FE-FACH
11/13/14   pr      Added srchcrutils_intraf_ngbr_fail_ecio_check
11/11/14   psr     Srchcrutil Cleanup
10/31/14   gsk     Logic to avoid clear res eval meas pending when CTCH is dropped for search 
10/13/14   ar      Add prototype for reporting IF nbrs to WL1CXM/MCS
10/23/14   gsk     Allow interf ranking in FACH when priorities are defined but FEFACH isnt enabled  
06/20/14   skk     FR19218: ReacqLessPICH(RLP) porting for JO.
10/06/14   ar      Add prototype for posting OOS indication
10/02/14   ar      Moved macros from .c file
09/18/14   mk      Featurizing LTE code.
08/25/14   psr     Added prototype for posting cell resel start event function
08/13/14   ar      Adjust srchcr_res_eval_meas_type to include LTE forced det type
08/12/14   gsk     Remove usage of srchcr_agc_stopped_post_w2x flag.
08/04/14   rs      Fixed compiler warnings
07/23/14   ar      Featurize PWC code with FEATURE_WCDMA_ENH_OOS_RXD_PWC
07/21/14   ar      Remove FEFACH feature definition for some macros
06/05/14   mk      WCDMA FED Feature for DPM_2_0, Flag used FEATURE_WCDMA_L1_DPM_FED_SUPPORT
06/23/14   ar      Changes to support AbsPrio Tresel scaling factor for GSM and LTE
06/24/14   mk      Support for changing EARFCN variable type from uint16 to uint32.
06/19/14   gsk     Idle DR-DSDS changes
05/30/14   ar      Add prototype SRCHCR_FACH_LEGACY_RULES
05/28/14   ar      Add FACH abs priority macro
05/22/14   gsk     Changes for enhanced PN search and excluding USET cell for meas
05/01/14   ar      Add prototype for panic mode check
04/28/14   ar      Add prototype to check if any LTE tresel is running in FACH
04/28/14   ar      Rename srchzz_is_tresel_running_for_any_lte_cells to srchcrutils file
04/10/14   ar      Remove prototype srchcr_clearing_sorted_intra_ngbr_cells_struct
04/03/14   ar      Add nlayers_fach for absolute priority in fach
03/25/14   ar      Add prototype to cleanup srchcr db
03/10/14   ks      Bolt FED API checkin
01/30/14   ar      Add support for distinguishing IRAT READY state between GSM and LTE
01/24/14   ar      Added support for forcing LTE detect searches after a W2L Resel failure
01/22/14   gsk     Ngbr srch opt log packet changes
01/16/14   gsk     Externed oos check function
01/16/14   ar      Change prototype for srchcrutils_prepare_srched_cells
01/13/14   ar      Add SRCHCR_INVALID_PRIORITY_SCHEDULING_MODE
12/16/13   as      Porting G2W TA from Triton to Dime.
12/15/13   ar      FEFACH (FR2759) absolute priority measurements in FACH
11/27/13   hk      Unification of non-intra module cleanup procedures
11/15/13   ar      FEFACH (FR2759) absolute priority measurements in FACH - ranking and evals
10/31/13   ar      Added nastt unmon peak processing function
10/29/13   sr/gsk  During cell reselection, delaying CPHY setup cnf for NBCH drop untill
                   serv cell is configured and nbr cell is deconfigured,                   
10/29/13   gsk     API to enable heartbeat after cell is deconfigured as part of NBCH drop
10/28/13   ar      Removed code under FEATURE_DL_IGNORE_MODE_FACH_MEAS
10/15/13   ar      Support for FEFACH (FR2759) absolute priority measurements in FACH
09/23/13   as      Porting Dual Sim from Triton to Dime.
09/12/13   jd      Searcher redesign
08/27/17   stk     Changed W2L CSG RSRP threshold to -100 as per SDD
08/19/13   vn      Added changes to indicate the reason for barring a band.
07/12/13   ar      Idle RxD enhancement changes for extended 3g coverage
07/05/13   stk/mk  FR 2677 - Initial version. Support for W2LTE CSG.
06/27/13   gsk     Updating the function name
06/22/13   gsk     Externing functions
05/09/13   mk      Support for measuring inter freq cells faster if tresel timer running for any interf cell.
04/11/13   gsk     Idle Mode FR 2741/2743, reducing WL1 wait time between rf_tune_to_chan
                   and search cmd to FW. Also adding support for interf offline PN
03/18/13   mk      Declared a varible as extern
03/04/13   gsk     Adding a function to clear the early rf sleep decision flag
01/21/13   stk     Support for Femto/CSG selection and reselection
09/10/12   stk     Added support for Cell reselection optimizations with HSFACH/HSRACH
10/01/12   gsk     For ngbr_srch_opt, calculating the min of max_tx_pwr of all the cells in a freq.  
11/13/12   gsk     Code cleanup
10/01/12   gsk     For ngbr_srch_opt, calculating the min of max_tx_pwr of all the cells in a freq.  
09/17/12   pv      Changed the return type of early RF response API.
09/10/12   stk     Added support for Cell reselection optimizations with HSFACH/HSRACH
08/30/12   pv      Fixed a variable type.
08/30/12   pv      Added extern.
08/29/12   pv      Remove featurization for early RF sleep resp API.
08/28/12   gsk     Externing the function for clearing the intra ngbr cell structure
08/21/12   stk     Pend the resel flag only when reselection is successfully triggered and RACH not in progress.
07/19/12   gsk     Neigbour Search Optimizations fixes
07/17/12   gsk     Compiler warnings
07/13/11   pv      Offline PICH/Parallel Reacq fixes.
07/05/12   gsk     Idle mode FRs 1782/1705/1947/2152/1950
05/15/12   gsk     FR 1782: WCDMA Inter- Frequency Tuning Optimization.
05/15/12   gsk     Support for feature Inter-freq List search frequency with two 
                   additional frequencies in idle 
06/19/12   gsk     Skipping detect and measurement searches on barred LTE frequency 
                   and setting the timers to max after the freq is unbarred.
11/28/12   gsk     Fix for compiler warnings
10/21/10   nd      Added an element in cell_bar database to indicate if a 
                   cell is reserved for operator use.
10/01/12   gsk     Externing function for checking if the interf is barred
10/01/12   gsk     Externing the function for clearing the intra ngbr cell structure
09/26/12   gsk     Triton fixes for disabling LTE
08/21/12   stk     Pend the resel flag only when reselection is successfully triggered and RACH not in progress.
08/14/12   gsk     Neigbour Search Optimizations fixes
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
07/05/12   gsk     Idle mode FRs 1782/1705/1947/2152/1950
05/15/12   gsk     FR 1782: WCDMA Inter- Frequency Tuning Optimization.
05/15/12   gsk     Support for feature Inter-freq List search frequency with two 
                   additional frequencies in idle 
06/19/12   gsk     Skipping detect and measurement searches on barred LTE frequency 
                   and setting the timers to max after the freq is unbarred.
06/19/12   vr      HSRACH code update
06/07/12   vs      MBMS feature cleanup.
04/16/12   gsk     Extern the variable srchcr_ext_cntrl_params[wsub_id].srchcr_sorted_inter_f_nset_cnt.
03/27/12   hk      Added support for FEATURE_WCDMA_HS_FACH_DRX
02/03/12   dp      Get RSSI Threshold from common external #define
01/20/12   zr      Mainlined FEATURE_L1_DETECTED_CELLS_FACH, FEATURE_L1_DETECTED_CELLS_IDLE, 
                   FEATURE_WCDMA_HCS, FEATURE_T_BARRED_SUPPORT, and FEATURE_L1_CELL_RESELECTION
12/21/11   stk     Added support for Rel9-RSRQ based W2L reselection. 
12/02/11   stk     Added support for FEATURE_WCDMA_RXD_PCH and FEATURE_IDLEMODE_FILTER
11/12/11   pv      New early RF sleep response API.
09/02/11   stk     fix compiler warnings
09/01/11   stk     Moved srchcr_priority_enum_type to srchcr.h to be used by srchcrgsm
04/26/11   mm/uk   Support for PCH Cleanup
04/26/11   dp/uk   Add support for W2X_IDLE_CMD
08/24/10   stk     Added function prototype to fix compiler warning.
08/23/10   uk      changed algorithm for sending LTE INIT/Deint
08/23/10   stk     Added support to handle pending CELL_BAR_REQ received in PCH_SLEEP state, 
                   to avoid any collision with CPHY_SETUP_REQ for NBCH later on.
08/06/10   mm      Compiler warning fix; added prototype for 
                   srchcr_perform_cell_res_eval_abs_prio
06/02/10   stk     Added support for W2LTE measurements and reselection under 
                   FEATURE_WCDMA_ABSOLUTE_PRIORITY & FEATURE_WCDMA_TO_LTE
03/05/10   stk     Handle OOS after W2G failure recovery and Reacq Fails
05/08/09   sup     Changing the includes for the renaming of header files  dec.h, enci.h, l1m.h, srch.h, srchi.h, 
                    srchlog.h to wdec.h, wenci.h, wl1m_v.h, wsrch.h, wsrchi.h, wsrchlog.h
04/22/09   stk     Compiler warning fixes
01/19/09   stk     Added support for R7 spec CR High Mobility Indicator( spec CR 2948(RP-060712))
12/09/08   uk      Mainling FEATURE_WCDMA_DL_ENHANCED, FEATURE_ENHANCED_CELL_RESELECTION, 
                   FEATURE_WCDMA_DRX_MANAGER, FEATURE_WCDMA_OFFLINE_SEARCHER 
                   and FEATURE_WCDMA_OFFLINE_DEMOD_SUPPORT 
12/09/08   uk      Added non_eq_plmn_barred to struct srchcr_barred_cell_struct
10/03/08   hk      Bringing in 7k Mailine fixes
08/13/08   uk      Merging following fixes
                   Cleaned up and added some fn prototypes
                   Fixed warning C2220W: Superfluous ',' in 'enum' declaration
                   Fixed compiler warning
                   Externed fn srchcr_reset_oos_timer()
                   modified feature name for MBMS
01/24/08   sk      Lint/Featurization fixes
01/18/08   uk      Added fn srchcr_start_gsm_srch_on_gsm_clk_en to be called to trigger 
                   IRAT srch when GSM clk and TX sytem time get enabled
12/19/07   sk      Fixed compilation error for FEATURE_MODEM_MBMS undef compilation
12/03/07   sk      Declaration check pl info function
12/05/07   sk      Changes to support PSC scanner with new DRX
11/01/07   ub      MBMS: DRX mode meas filtering when MCCH/MTCH enabled prototypes
10/26/07   sk      Checking in MBMS branch code into mainline with MBMS feature disabled
08/10/07   sk/uk   Added logic to not short tresel for W cells when GSM neigbors are 
                   present and weak reselection condition is entered (certain number 
                   of reselections triggered in a certain amount of time due to S-criteria 
                   failure and  shorting of Tresel under low rssi condition)
10/04/07   sk      Reselection related cleanup
08/23/07   vsr     Made include file change related to removal of dlcmd.* files
07/05/07   ub      Added FLC support
07/16/07   sv      Fixed previous merge issue.
05/23/07  ub/sk    Intergrate GSM meas and code cleanup
05/22/07   sk      Enabled measurement rules
04/26/07   sk      Fixed lint errors
04/25/07  ub/sk    MBMS cell reselection changes
06/04/07   sk      Added FEATURE_SUPPORT_FOR_PARTIAL_ROAMING or CR151
04/05/07   sk      Added extern for srch_cell_bar_update
02/14/07   ub      Define SRCHCR_NV_MAX_TX_PWR_INCREMENT as 1 for correct 
                   Srxlev calculation for power class 3 UE
01/12/07   sk      Unbar frequency if the cell is not highest ranked cell
10/03/06   sk      Defined srchcr_ext_cntrl_params[wsub_id].srchcr_interf_resel_fail
06/20/06   bd      Renamed global vars/functions to avoid conflict with 1x names
05/24/06   ub/mmr  Cleaned up HCS and Rel5 featurization.
05/11/06   sk      Defined extern srchcr_tresel_irat
04/20/06   sk      Changed SRCHCR_QHYST2_ADD to return 2*hist_add to avoid 
                   rounding off
                   Declared variables for common CR timer in FACH and idle.
04/19/06   sk/ub   Declaration of srchcr_ext_cntrl_params[wsub_id].srchcr_ignore_barred_freqs
03/28/06   sk      Declaration for srchcr_get_weakest_uset_idx ()
03/17/06   ub      Lint error fixes
02/20/06   gv 	   Declared extern srchcr_ext_cntrl_params[wsub_id].srchcr_rem_oos_time, this variable stores
				   the remaining OOS time when the OOS timer is stopped.
10/19/05   ub      Declare fast mov when num reselections exceed Ncr
09/13/05   sk      Detected Cells Support in idle mode
08/12/05   sk      Declaration for srchcr_init_gsm_nset()
08/05/05   ub      SFN based full search timers implementation
07/06/05   sk      Declared extern srchcr_ext_cntrl_params[wsub_id].srchcr_strong_cell_ptr
07/15/05   am      removed the srchcr_inter_f_pccpch_n_cc_setup_pending and 
                   srchcr_inter_f_pccpch_n_ph_setup_pending variables
06/16/05   ub      Changes for supporting sfn-sfn observed time difference
05/31/05   am      Merged the latest lint fixes to the mainline
05/27/05   ub      Exclude inter-f and gsm cells using meas rules in FACH
05/24/05   ub      Changed function prototypes for starting inter-f step srch
05/24/05   ub      Updated penalty timer backup and restore functions
05/12/05   sk      T-barred Feature: Removed intraf_barred. Added new enum for
                   bar type (wcdma cell/ wcdma freq/ gsm/ wcdma aset).
05/10/05   sk      T-barred Feature: T-barred variable in barred cell list
                   to hold ms and hence uint32
05/04/05   sk/sh   Added T-barred Feature support. Added barred cell structure
                   and immediate reselection pending flags
05/01/05   ub      Cell reselection improvements and fixes
04/06/05   ub/ba   HCS based cell reselection feature (FEATURE_WCDMA_HCS)
11/19/04   yiz     Optimize FACH GSM BSIC meas scheduling.
09/22/04   sh      Added srchcr_sib_mod_update() which is called to do SIB
                   modification updates.
06/23/04   sh      Add l1m_state_enum_type argument to srchcr_cleanup().
06/04/04   src     Changes concomitant to consolidating certain neighbour-cell
                   parameters into a newly defined aggregate in demod modules.
05/25/04   sh      Added proto srchcr_exclude_cell_from_ranking().
05/10/04   gs      Moved enum srchcr_cell_type_enum_type from its source file
                   Externalized functions srchcr_add_cell_to_srched_list and
                   srchcr_remove_cell_from_srched_list
05/06/04   sh      Added protos for srchcr_intra_f_post_list_srch_dump() and
                   srchcr_inter_f_post_list_srch_dump().
03/26/04   sh      Added flags to sequence inter-frequency nBCH setup.
02/17/04   sh      Removed SRCHCR_OUT_OF_SERV_TMR_PCH_IN_FRAMES.
02/09/04   yiz     Add SRCHCR_GSM_RSSI_THRESHOLD_FOR_BSIC as -110 dBm
12/10/03   yiz     Add srchcr_fach_gsm_calc_rank() for 
                   FEATURE_INTER_RAT_CELL_FACH_RE_SELECTION
12/09/03   scm     Add extern to srchcr_reset(),
11/12/03   sh      Support for inter-frequency cell reselection in idle mode.
10/10/03   sh      Added proto for function void srchcr_reset_t_resel( ).
08/11/03   sh      Correct Qhyst2s_add equation.
07/28/03   sh      Defined full search timer in fach when no meas occasions
                   are specified. Clip step1 peaks to 6 in fach. Added func
                   proto srchcr_check_full_srch_timer_expired().
06/26/03   sh      Modify calculation of additional Qhyst2.
06/23/03   sh      Define SRCHCR_MAX_STEP1_PEAK_CNT to clip step 1 peak count.
                   Declared srchcr_last_known_good_aset_pos to snapshot aset
                   position.
06/11/03   sh      Added flag srchcr_proc_intra_f_srch_due and funtion proto
                   srchcr_reset_proc_start_n_srch().
04/17/03   sh      Changed internal threshold for Sintrasrch to -14.
                   Declared out of service timer for FACH.
                   Added proto srchcr_set_out_of_service_timer_flag().
03/27/03   sh      Added function proto srchcr_start_cell_reselection_eval()
                   to be called by DRX after it completes setting up the
                   downlink channels. This is done only in the case when
                   initial reacquisition has succeeded.
03/25/03   sh      Modified args to function  srchcr_add_pccpch_n().
02/12/03   sh      Added extern srchcr_ext_cntrl_params[wsub_id].srchcr_drx_waiting_on_srch_sleep.
01/29/03   sh      Added extern srchcr_ext_cntrl_params[wsub_id].srchcr_pending_cell_resel_bsic_verif.
                   Changed srchcr_initial_intra_f_srch_in_progress to
                   srchcr_start_initial_srch.
01/06/03   sh      Added flag srchcr_initial_intra_f_srch_in_progress.
10/01/02   ddh     Added type for specifying initial or reacq srch
08/27/02   sh      Added noff to save nbr offset to srchcr_last_nbr_info_type.
07/31/02   sh      Externed srchcr_full_srch_req.
07/11/02   sh      Macro to calculate additional hysterisis based on ecio.
                   Constants for out of service timers during pch/fach.
06/07/02   sh      Added flag srchcr_ext_cntrl_params[wsub_id].srchcr_start_cell_reselection.
06/04/02   sh      Externed variables related to out of service timer and 1
                   sec timer for starting cell reselection after we go to drx.
05/18/02   sh      Removed srch_cell_reselect_check().
04/29/02   sh      Added protos for step1/2/3 and list search functions.
03/27/02   sh      Created file.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "wcdma_variation.h"
#include "comdef.h"
#include "wl1m.h"

#include "srchcmd.h"
#include "srchcrutils.h"
#include "srchset.h"
#include "srchcrgsm.h"
#include "srchfach.h"
#if defined(FEATURE_WCDMA_TO_LTE)
#include "l1rrcif.h"
#endif
#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
#include "wsrchlte.h"

#include "srchzz.h"
#include "srchsetutil.h"

#include "wsrchiratmeas.h"
#endif

#ifdef FEATURE_WCDMA_HS_FACH_DRX
#include "srchfachedrx.h"
#endif
#include "drx.h"
#include "srchinterf.h"
#ifdef FEATURE_WCDMA_REACQLESS_PICH   
#include "wsrchlte.h"
#endif
/*===========================================================================

                        CONSTANT DECLARATIONS

===========================================================================*/

/* Smin value to calculate an internal threshold value for Sintrasrch
 * when Sintrasrch is specified OTA. Tuneable.
 */
#define SRCH_SMIN_FOR_SINTRA -14

/* Minimum value of CPICH Ec/Io that will trigger a full srch. This
 * corresponds to -16dB. This should be compared with Ec/Io returned
 * from the srch interface.
 */
#define SRCHCR_ASET_MIN_ECIO 32

#ifdef FEATURE_FEMTO_CSG
#define SRCHCR_CSG_PANIC_ECIO_THRESH 40 /* Threshold to trigger Panic Intra Full srch when camped on CSG cell */
#define SRCHCR_CSG_RESEL_ECIO_THRESH 36 /* Minimum threshold for CSG cell to qualify as a resel candidate */
#define SRCHCR_CSG_W1_THRESH 20         /* Range W1 = -10.  Constant defined for 2*ECIO */
#define SRCHCR_CSG_W2_THRESH 36         /* Range W2 = -18.  Constant defined for 2*ECIO */
#define SRCHCR_CSG_LTE_RSRP_THRESH -100
#define SRCHCR_CSG_LTE_RSRQ_THRESH -18
#endif

/* Number of frames in 1 second */
#define SRCHCR_1_SEC_TIME_IN_FRAMES 100

/* Number of frames in 30 seconds */
#define SRCHCR_30_SEC_TIME_IN_FRAMES 3000

/* Max number of step 1 peak count to use for step 2 searching */
#define SRCHCR_MAX_STEP1_PEAK_CNT   6

/* Full search timer when no measurement occasions are specified
 * or when no GSM measurements are performed (number of frames).
 */
#define SRCHCR_FULL_SRCH_TIMER_WITH_NO_MEAS_OCCASION 36

/* -110 dBm as minimum average RSSI for a cell to have BSIC meas */
#define SRCHCR_GSM_RSSI_THRESHOLD_FOR_BSIC  SRCHGSM_MIN_ACCEPTABLE_RX_LVL_dBm_x16

/* Maximum value of PENALTY_TIMER_n that NW can set for a neighbor
 * cell in SIB11/12 (in seconds)
 */
#define L1_MAX_PENALTY_TIMER   60

/* Value mapping tmp_offset to infinity */
#define L1_TMP_OFFSET_INFINITY   50

#define SRCHCR_RESET_RESEL_EVAL_MEAS_PENDING(reset_meas)\
        do { \
             if(srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_gap_res_eval_meas_pending & reset_meas) SRCHCR_RESET_GAP_RESEL_EVAL_MEAS_PENDING(reset_meas); \
             srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_res_eval_meas_pending = (srchcr_res_eval_meas_type) ((uint32) srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_res_eval_meas_pending & \
                                                                                                         (~(uint32)(reset_meas)));\
           } while(FALSE); \

#define SRCHCR_RESET_GAP_RESEL_EVAL_MEAS_PENDING(reset_meas)\
        srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_gap_res_eval_meas_pending = (srchcr_res_eval_meas_type) ((uint32) srchcr_ext_cntrl_params[wsub_id].srchcr_ext_cntrl_params_mdump.srchcr_gap_res_eval_meas_pending & \
              (~(uint32)(reset_meas))); \
        
#define SRCH_INTRAF_NSET_CNT_MAX  (SRCH_SYNC_NSET_MAX_C0 + SRCHCR_USET_MAX)

/* RF Tune Time in 256 Chip units - 10 ticks post RF tune call, before search 
 * command is issued in idle drx mode */
#define RF_TUNE_TIME_DRX   10

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
/* global flag to disable / enable Absolute Priotity + LTE related F3s for debug purpose */



/* Min srch period in High Priority Scheduling mode */
#define SRCHCR_HIGH_PRIO_MIN_SRCH_PERIOD_FRAMES 6000

#define FEFACH_DEBUG_F3 0

#endif

/*Maximum size for the FIFO buffer used for neighbor search opt*/
#define MAX_SRCH_BUFFER_SIZE 375

/*===========================================================================

                        TYPE DECLARATIONS

===========================================================================*/

typedef enum {
  SRCHCR_INTER_F_SRCH_NONE = 0x0,

  SRCHCR_INTER_F_SRCH_LIST = 0x1,
  SRCHCR_INTER_F_SRCH_FULL = 0x2,

  SRCHCR_INTER_F_SRCH_LIST_FREQ0 = 0x1,
  SRCHCR_INTER_F_SRCH_FULL_FREQ0 = 0x2,

  SRCHCR_INTER_F_SRCH_LIST_FREQ1 = 0x4,
  SRCHCR_INTER_F_SRCH_FULL_FREQ1 = 0x8
} srchcr_inter_f_srch_bfield;

typedef enum {
  SRCHCR_INTER_F_0,
  SRCHCR_INTER_F_1
} srchcr_inter_f_freq_bfield;


/* Structure to hold parameters of the last cell RRC set up the N-PCCPCH
 * for after a cell reselection indication, for use during cell transition.
 */
typedef struct
{
/* SFN-SFN difference between the serving cell and the neighbor we
 * may be reselecting to.
 */
  int16 sfn_sfn_diff;

  /* frequency */
  uint16 freq;

  /* nbr pccpch offset from serving cell */
  int32 noff;  

  /* whether this is a inter-frequency neighbor */
  boolean is_inter_f;

} srchcr_last_nbr_info_type;

/* Enumeration to describe what kind of neighbor search is being started.
 * The behaviour varies for performing inter-frequency and inter-RAT 
 * searches depending on the type of neighbor search being performed.
 */
typedef enum
{
  SRCHCR_INITIAL_CR_SRCH,    /* Initial srch for entering DRX */
  SRCHCR_REACQ_FAIL_CR_SRCH, /* Nset srch for a failed reacquisition */
  SRCHCR_RESEL_CR_SRCH       /* Nset srch for normal reselection */
} srchcr_nset_srch_type;






/* Enum to find out previous state and present state                  */
/* F O R M A T     ---  Previousstate_PresentState                    */
/* X - stands for               not measured state                          */
/* L - Low priority             cells measure state                          */
/* E - Equal priority           cells measure state                         */
/* H - High priority            cells measure state                         */
/*                    E X A M P L E                                                  */
/*   LEH_LEH ==> LowEqualHIgh_LowEqualHigh                        */
/* Previous state measured LEH Present state measures LEH     */

typedef enum
{
  LEH_XEH,
  LEH_LEH,
  LEH_LEX,
  LEX_XEH,
  LEX_LEH,
  LEX_LEX,
  XEH_XEH,
  XEH_LEH,
  XEH_LEX

} prio_gp_state;

/*Struct to indicate a combination of 
  CellFACH_Abs_Prio_Indicator from SIB19 and
  UE capability*/
typedef enum
{
  HIGH_HIGH  = 0x0,
  HIGH_ALL   = 0x1,
  ALL_HIGH   = 0x2,
  ALL_ALL    = 0x3,
  LEGACY     = 0x4
} srchcr_fefach_ue_cap_and_sib19_ind_enum_type;

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
typedef struct
{

  /* 25.133 section 4.2.2.5a
     The total number of configured higher priority E-UTRA 
    and UTRA FDD carrier frequencies and is additionally 
    increased by one if one or more groups of GSM frequencies 
    is configured as a higher priority
    Thigher_priority_search = 60 * Nlayers */

  uint32 nlayers;
#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
  uint8 nlayers_fach;
#endif

    /* 25.133 section 4.2.2.3
     The total number of carriers used for FDD cells.
     The UE shall measure CPICH Ec/Io and CPICH RSCP of detected
     Inter-Frequency cells at least every (Ncarrier-1) * TmeasureFDD */

  uint32 ncarrier;

  /* Indicates if priority based reselection is to be used
     for Inter-Freq layers */
  boolean interf_prior_info_present;

  /* Indicates if any of the Inter-Freq layers configured by
     n/w are higher priority then serving cell */
  boolean higher_prior_w_freq_present;

  /* Indicates if priority based reselection is to be used
     for GSM layers */
  boolean gsm_prior_info_present;

  /* Indicates if any of the GSM Arfcns configured by
     n/w are higher priority then serving cell */
  boolean higher_prior_gsm_arfcn_present;

  boolean g_arfcn_found_in_high_prio_srch;

  #ifdef FEATURE_WCDMA_TO_LTE
  /* Indicates if LTE EARFCNs have been configured in the 
     Neighbor list */
  boolean lte_info_present;

  /* Indicates if any of the LTE EArfcns configured by
     n/w are higher priority then serving cell */
  boolean higher_prior_lte_earfcn_present;

  /* Count of the number of higher prior LTE Earfcns */
  uint16 num_higher_prior_lte_earfcn;
  #endif

  /*Variables to store if AbsPrio in FACH measurement are included in SIB19*/
  boolean fach_abs_prio_meas_incl;
  srchcr_fefach_ue_cap_and_sib19_ind_enum_type fach_abs_prio_ue_cap_sib_ind;
  #ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
  int8 highest_interf_prio;
  int8 highest_gsm_prio;
  #endif
}srchcr_prior_info_for_rats_struct_type;

#endif

typedef enum
{
  SRCHCR_IRAT_MEAS_IDLE_STATE,
  SRCHCR_IRAT_MEAS_DECONFIG_STATE,
  SRCHCR_IRAT_MEAS_FW_STOP_AGC_STATE,
  SRCHCR_IRAT_MEAS_READY_FOR_GSM,
  SRCHCR_IRAT_MEAS_READY_FOR_LTE
}
srchcr_irat_meas_state_enum_type;

#define SRCHCR_COND_CNT_MAX 9

typedef struct
{
    uint16 ecio;
    uint16 rscp;
    uint32 total_eng;
    int16 rxagc;
} fifo_buffer_struct_type;

typedef struct
{
  fifo_buffer_struct_type fifo_buffer[MAX_SRCH_BUFFER_SIZE];
  uint8 idx;
  uint32 candngbrecio;
  int32 candngbrrscp;
  boolean pn_srch_performed;
  uint32 pn_performed_in_drx_cycle;
  int16 q_offset1_min, q_offset2_min, q_rxlevmin_min, q_qualmin_min, max_tx_pwr_min;
  boolean algo_evaled;
  boolean cond[SRCHCR_COND_CNT_MAX];
  int16 step1_eng_th;
  uint32 std_dev_square;
  uint8 rscp_vals_considered;


} srchcr_ngbr_srch_opt_strcut_type;

#ifdef FEATURE_FEMTO_CSG
extern srch_interf_cell_struct_type* srchset_get_interf_cell(uint16 freq, uint16 scr_code);

#ifdef FEATURE_WCDMA_TO_LTE
typedef struct
{
  /*physical cell id of CSG cell*/
  uint16 cell_id;
  /* The earfcn for searching LTE cells on */
  uint32 earfcn;
  /* sss correlation value, used for ranking cell detection results in CM */
  int16 rsrp;
  /* Max RSRQ value combined across Tx-Rx pairs. In linear scale. */
  int16 rsrq;
  /* Max RSSI value combined across Tx-Rx pairs. In linear scale. */
  int16 rssi;
  int16 s_rxlev;
  int16 s_qual;
  int8 p_max;
  uint32 csg_id;
  /* LTE priority info structure */
  wsrchlte_priority_info_type prior_info;
} srchcr_csg_lte_cell_struct_type;
#endif


#define MAX_INTRA_ASF_CELLS 6
typedef struct 
{
  boolean intraf_csg_cells_included; 
  boolean interf_csg_cells_included; 
  uint8 num_tot_asf_cells;
  uint8 num_intra_cells;
  uint8 num_inter_cells;
  uint8 num_lte_cells;
  boolean is_csg_meas_req_processed;
  srch_cell_struct_type* w_intraf_csg_list[MAX_INTRA_ASF_CELLS]; 
  srch_interf_cell_struct_type* w_interf_csg_list[MAX_ASF_CELLS];
  #ifdef FEATURE_WCDMA_TO_LTE
  srchcr_csg_lte_cell_struct_type* lte_csg_list[MAX_ASF_CELLS];
  #endif
  /* bitmask per freq to say whether srch is pending / done on all freq's */
  uint32 csg_meas_pending;
  boolean request_trm_lock;
  boolean srch_trm_id_valid[2];
  /* This field will be updated after SW queries TRM for the lock request for corresponding bands.
   * If lock isn’t granted device will be invalid.
   */
  wrm_irat_identifier srch_trm_id[2];
  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_WCDMA_RFM_PATH_SUPPORTED)
  rfm_path_type srch_rfm_path;
  #else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
  rfm_device_enum_type srch_device_id;
  #endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_WCDMA_RFM_PATH_SUPPORTED) */
} srchcr_csg_info_struct_type;

#endif

/*===========================================================================

                        GLOBAL DECLARATIONS

===========================================================================*/

/* Table of Nserv values indexed by K values. K = 0..2 is not valid */
extern const uint8 srch_cell_select_nserv_tab[10];

/* Table for neighbor search optimizations buffer size K = 0..2 is not valid */
extern const uint16 srchcr_ngbr_srch_opt_buffer_size_tab[10];

/* Table of TmeasureFDD values indexed by K values. K = 0..2 is not valid */
extern const uint8 srchcr_t_meas_fdd_tab[10];

/* Table of TmeasureGSM values indexed by K values. K = 0..2 is not valid */
extern const uint8 srchcr_t_meas_gsm_tab[10];

/* Table of TmeasureGSM values inside QTA gap indexed by K values. K = 0..2 is not valid */
extern const uint8 srchcr_t_meas_gsm_tab_inside_qta[10];

#ifdef FEATURE_IDLEMODE_FILTER

/* Table of idle mode filtering coefficients, indexed by DRX cycle length */
extern const uint32 srch_idle_mode_filt_coeff_q10[10];

/* Table of idle mode filtering coefficients, indexed by DRX cycle length for neighbor srch done.
* This is different from ext_srchcr_cntrl->srch_idle_mode_filt_coeff_q10 only for DRX length 640ms where we used filt_coeff as 483 */
extern const uint32 srch_idle_mode_filt_coeff_ngbr_q10[10];

#endif

typedef struct
{
  /* This field will be updated as part of measurement request with the neighbor info */  
  srchcr_meas_rules_type srch_rat_type;

  /* This field will be updated as part of measurement request with the neighbor info */  
  uint8 srch_duration;

  uint32 freq;

  /* This field will be updated as part of measurement request with the neighbor info. The reason the band is a structure is to maintain 
  band info for all EARFCNs.*/  
  wrm_freq_input_type srch_band_info;

} srchcr_drx_nxt_gap_info_struct_type;

typedef struct
{
  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  /* Used to maintain the # frames since last cell reselection evaluation */
  uint16 srchcr_frames_since_prev_lte_eval;
  #endif
  #ifdef FEATURE_WCDMA_ENH_OOS_RXD_PWC
  uint8 srch_serv_cell_pwc_failed_drx_cycles;
  #endif
  
  
  boolean srchcr_resp_early_rf_slp_allowed;
  /* Tresel value to be used for intraf resel*/
  uint16 srchcr_tresel_intraf;
  srchcr_irat_meas_state_enum_type srchcr_irat_meas_state;
  #ifdef FEATURE_REL5  
  /* Default Tresel value to be used for intraf resel*/
  uint16 srchcr_tresel_interf_def;
  #endif
  /* Used to maintain the # of frames since last inter freq full srch */
  uint16 srchcr_num_frames_since_inter_full_srch;
  srch_cell_struct_type *srchcr_sorted_intra_f_nset_ptr[SRCH_SYNC_NSET_MAX_C0 + SRCHCR_USET_MAX];
  #ifdef FEATURE_IGNORE_TRESEL_ON_S_FAIL
  dword srchcr_weak_resel_condition_end_time;
  #endif
  /* Used to maintain fast moving status and end time */
  srchcr_ue_fast_moving_struct_type srchcr_ue_fast_moving;
  /* Tresel value to be used for inter rat resel*/
  uint16 srchcr_tresel_lte;
  /* First entry into barred cell list*/
  uint16 srchcr_barred_cell_list_first_index;
  
  uint16 srchcr_sorted_inter_f_nset_cnt[SRCH_OFREQ_FREQ_MAX];
  
  
  /* Cumulative sum of all expired barred cell timers' initial values */
  uint32 srchcr_barred_cell_elapsed_time;
  /* Global to indicate which meas have been completed for reselection evaluation */
  srchcr_res_eval_meas_type srchcr_res_eval_meas_completed;
  uint16 srchcr_frames_since_prev_eval;
  #ifdef FEATURE_IGNORE_TRESEL_ON_S_FAIL
  boolean srchcr_weak_resel_condition;
  #endif
  /* Default Tresel value to be used for irat resel*/
  uint16 srchcr_tresel_lte_def;
  
  #ifdef FEATURE_REL5  
  /* For REL5 CR 136 rev1*/
  uint16 srchcr_tresel_intraf_def;
  #endif
  /* Flag to indicate we are tuned away from serving frequency */
  boolean srchcr_rf_tuned_away;
  /* Global to indicate reselection evaluation needs to be performed or not */
  srchcr_res_eval_status_type srchcr_res_eval_status;
  /* flag to indicate that a BSIC verification is due for the GSM
 * neighbor before sending the reselection indication.
 */
  boolean srchcr_pending_cell_resel_bsic_verif;
  /* Flag to indicate if cell reselection related processing
 * should be done.
 */
  boolean srch_cell_reselect_enabled;
  /* Number of drx cycles the cell selection criteria has failed continously */
  uint8 srch_cell_select_failed_drx_cycles;
  
  #ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  srchcr_prior_scheduler_mode_enum_type srchcr_curr_prior_scheduling_mode;
  #endif
  /* The number of monitored cells to evaluate for reselection */
  uint8 srchcr_inter_f_eval_list_num[SRCH_OFREQ_FREQ_MAX];
  /* Cell pointer to the WCDMA neighbor cell reported to RRC via 
 * RRC_CPHY_CELL_RESELECTION_IND 
 */
  srch_cell_struct_type *srch_cr_cell_ptr;
  /* Holds the no. of barred cells*/
  uint16 srchcr_barred_cell_cnt;
  /* Flag to indicate if a cell reselection is pending */
  boolean srchcr_pending_cell_resel;
  /* Holds the barred cell information*/
  srchcr_barred_cell_struct srchcr_barred_cell_list[WL1_MAX_BARRED_CELLS];
  /* Flag to indicate that W2G failed and out of service timer has expired */
  boolean srchcr_resume_after_w2g_fail_OOS_expired;
  
  /* Cell reselection timestamps used to calculate fast moving */
  srchcr_res_times_struct_type srchcr_res_times;
  
  #ifdef FEATURE_IGNORE_TRESEL_ON_S_FAIL
  srchcr_res_times_struct_type srchcr_fdd_weak_res_times;
  #endif
  
  srch_ofreq_cell_list_struct_type srchcr_sorted_inter_f_nset_ptr[SRCH_OFREQ_FREQ_MAX];
  /*variable holding the remaining Cell Resel timer value used 
  during state transition*/
  rex_timer_cnt_type srchcr_rem_CR_time_ms;
  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  /* Cell pointer to the LTE neighbor cell to be reported to RRC via
 * RRC_CPHY_CELL_RESELECTION_IND.
 */
  wsrchlte_cell_struct_type *srchcr_lte_cell_ptr;
  #endif
  
  /* Store the value of the OOS time remaining when suspending
** W stack of FACH to PCH transition.*/
  rex_timer_cnt_type srchcr_rem_oos_time;
  uint16 srchcr_srched_cells_cnt;

  srchcr_rank_cell_struct_type srchcr_srched_cells[MAX_NUM_SRCHED_CELLS];
  
  /* Flag to indicate if drx is waiting on srch ready to sleep */
  boolean srchcr_drx_waiting_on_srch_sleep;
  
  /* Cell pointer to the GSM neighbor cell to be reported to RRC via
  * RRC_CPHY_CELL_RESELECTION_IND.*/
  srch_gsm_cell_struct_type *srchcr_gsm_cell_ptr;
  
  /* Flag to indicate 1s has elapsed since DRX to start cell reselection */
  boolean srchcr_start_cell_reselection;
  
  #ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  srchcr_prior_info_for_rats_struct_type srchcr_prior_info_for_rats;
  #endif
  
  /* Sorted list of GSM neighbor cells, sorted according to RSSI */
  srch_gsm_cell_struct_type *srchcr_sorted_gsm_nset_ptr[SRCH_GSM_NSET_MAX];
  
  uint8 srchcr_intra_f_eval_list_num;
  
  /* Flag to indicate that the out of service timer has expired */
  boolean srchcr_out_of_service_timer_expired;
  /* Tresel value to be used for GSM resel*/
  uint16 srchcr_tresel_gsm;
  
  /* GLobal to store the current meas rules */
  srchcr_meas_rules_type srchcr_current_meas_rules;

  /* Count of the number of entries in the sorted list */
  uint16 srchcr_sorted_intra_f_nset_cnt;
  uint16 srchcr_cell_res_eval_sfn;
   /* Cell pointer to the WCDMA Inter-frequency neighbor cell to be
  * reported to RRC via RRC_CPHY_CELL_RESELECTION_IND.
  */
  srch_interf_cell_struct_type * srchcr_inter_f_cell_ptr;
  /* Flags to indicate type of search in progress during cell reselection */
   boolean srchcr_inter_s_srch_in_progress;
  /* Indicates that serving cell has become barred and immediate reselection
   is to be triggered*/
  boolean srchcr_pending_immdt_resel;
  srchcr_res_eval_meas_type srchcr_gap_res_eval_meas_pending;
    /* Global to inidcate which meas are pending for reselection evaluation 
  */

	  /*Structure which carries the values for Neighbor search optimizations(FR_1947) Algo*/
  srchcr_ngbr_srch_opt_strcut_type srchcr_values_for_full_srch[3];
  srchcr_res_eval_meas_type srchcr_res_eval_meas_pending;

  srchcr_drx_nxt_gap_info_struct_type srchcr_drx_nxt_gap_info;
  #ifdef FEATURE_FEMTO_CSG
  srchcr_csg_info_struct_type srchcr_csg_info;
  #endif



 
   

}srchcr_ext_cntrl_params_mdump_type;


typedef struct
{



  


  srchcr_ext_cntrl_params_mdump_type srchcr_ext_cntrl_params_mdump;

  /* Flag to indicate whether CR timer has expired and
   srchcr_init() should be called in DRX*/ 
  boolean srchcr_start_CR;

 
  /* Temp struct to store first reacq list srch values */
  srch_cell_struct_type temp_srch_aset_ptr;

  uint8 srchcr_hcs_intra_async_nset_cnt;
  #ifdef FEATURE_WCDMA_ENH_OOS_RXD_PWC
  uint8 srch_serv_cell_pwc_inst_fail_drx_cycles;
  #endif

  uint16 srchcr_num_consecutive_reacq_fails;

  /* stores the previous DRX cycle's filt_eng to track changes between cycles */
  uint32 srch_prev_drx_filt_eng;
  /* Variable that lets us know that interf reselection has failed and we will
     have to pend rach till 20ms ASET search is done in FACH */
  boolean srchcr_interf_resel_fail;
  /* Flag to indicate to DRX to count 1 second before enabling cell reselection */
  boolean srchcr_start_1_sec_timer;
  uint8 srchcr_intra_f_nsrch_reqd_inx;



  /* Number of DRX cycles elapsed since start of cell reselection */
  uint16 srchcr_num_drx_cycles;
  /* HCS inter sync and async set */
  uint8 srchcr_hcs_interrat_nset_cnt;
  /* Used to maintian the SFN of the last cell reselection evaluation */
  uint16 srchcr_gsm_cell_res_eval_sfn;
  /* Used to maintian the SFN of the last interf cell reselection evaluation */
  uint16 srchcr_interf_cell_res_eval_sfn[SRCH_OFREQ_FREQ_MAX];
  /* Flag indication whether reacq has been failed atleast once due to 
barred cell*/
  boolean srchcr_barred_cell_reacq_failed;
  #if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
  uint16 srchcr_lte_cell_res_eval_sfn;
  #endif


  uint8 srchcr_hcs_inter_async_nset_cnt[2];
  srchcr_nset_srch_type srchcr_srch_type;

  /* BSIC statistics - used only during Idle mode */
  srchcr_bsic_stats_struct_type srchcr_bsic_stats;
  /* Flag to indicate whether we have to sort the entire nset.
 * This is set to true whenever we perform step 1/2/3 searches
 * and a new neighbor has been added to the sync nset.
 */
  boolean srchcr_sort_entire_intra_f_list;


  #ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  /* FALSE - Disable (default), TRUE - Enable */
  boolean srchcr_abs_prio_f3_debug;
  #endif
  /* Flag to indicate that only the intra-f neighbors programmed
 * to the LPQ list search should be sorted after the search completes
 */
  boolean srchcr_sort_lpq_nset;

 
  /* Used to maintain the SFN of last serving cell list search */
  uint16 srchcr_serv_list_srch_sfn;
 
  uint16 srchcr_inter_f_start_ts;
  /* 30s timer for BSIC verification of GSM cells in number of DRX Cycles */
  uint16 srchcr_gsm_bsic_verif_timer;
  /* Used to maintain the time of last cell reselection evaluation */
  dword srchcr_cell_res_eval_time;
  uint16 srchcr_frames_since_prev_gsm_eval;

  /* Flag to indicate whether we have to sort the entire intra
 * frequency nset. This is set to true whenever we perform step
 * 1/2/3 searches and a new neighbor has been added to the sync nset.
 */
  boolean srchcr_sort_entire_inter_f_list;
  /* Flag to indicate if a cell reselection is pending */

  /* Holds the barred cell information*/

  /* Flag to indicate if an intra-frequency search should be performed
   during this search cycle
 */
  boolean srchcr_intra_f_srch_req;
  /* Cell reselection timestamps used to calculate fast moving */

 
 

 
  /* Used to maintain the # frames since last interf cell reselection evaluation */
  uint16 srchcr_frames_since_prev_interf_eval[SRCH_OFREQ_FREQ_MAX];

  /* Number of DRX cycles elapsed since last GSM search was done  */
  uint16 srchcr_num_drx_cycles_last_gsm_srch;
  #ifdef FEATURE_FEMTO_CSG
  boolean srchcr_csg_mem_allocated;
  #endif
  /* search timestamp when the cell reselection timer was last updated */
  uint16 srchcr_intra_f_start_ts;
  /* check abort starting timestamp */
  uint16 srchcr_check_abort_timer_interf_resel;
  uint16 srchcr_sch_ecio_thresh_for_step1_peaks;
  /* Cell pointer to a WCDMA Intra-F neighbor cell which became stronger
 * during a intra-freq cell reselection. In case of a cell reselection failure
 * this pointer will be used to trigger fresh reselection.
 */
  srch_cell_struct_type *srchcr_strong_cell_ptr;
  
 
  /* Flag to indicate if a UL_SET_MOD_TIMING cmd has been sent to DSP.
 * This cmd should be sent once before starting GSM measurements
 * when we enter DRX state.
 */
  boolean srchcr_ul_set_mod_timing_cmd_sent;
  uint8 srchcr_hcs_intra_sync_nset_cnt;
  /* Indicates if we ignore barred freqs for cell selection/reselection */
  boolean srchcr_ignore_barred_freqs;
  /* Used to maintain backup info of penalty timers for ngbr cells */
  srch_backup_cell_struct_type srchcr_backup_cell_struct;

  #if defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)
  #error code not present
#endif
 
  /* HCS intra sync and async set */
  uint8 srchcr_hcs_inter_sync_nset_cnt[2];
  boolean srchcr_enh_resel_trig_to_bsic_verfd_cell;
  

  /* Number of inter-f sync neighbors that return zero energies
 * after a list srch
 */
  uint8 srchcr_sync_inter_f_nset_zero_eng_cnt[SRCH_OFREQ_FREQ_MAX];
  /* Sorted list of GSM neighbor cells, sorted according to RSSI */

  /* Used to maintain the # of frames since last intra freq full srch */
  uint16 srchcr_num_frames_since_intra_full_srch;
  /* TmeasureGSM as measured in DRX Cycles */
  uint8 srchcr_t_meas_gsm_drx_cycles;

  /* The previous state of the DRX meas rules */
  srchcr_meas_rules_type srchcr_prev_meas_rules;
  
  /* frequency index that is currently being searched */
  srchcr_inter_f_idx_enum_type srchcr_inter_f_idx;
  /* Flag to indicate that a RSSI scan is pending */
  boolean srchcr_t_meas_gsm_timer_expired;

  /* Flag to indicate if a inter-frequency list search is required */
  boolean srchcr_inter_f_list_srch_req;
 
  /* Cell pointer to the WCDMA Inter-frequency neighbor cell to be
 * reported to RRC via RRC_CPHY_CELL_RESELECTION_IND.
 *
  /* global boolean to indicate an external CELL_BAR_REQ message has arrived from RRC but not processed. */
  boolean srchcr_cell_bar_ext_msg_recd;
  /* Flags to indicate type of search in progress during cell reselection */
 
  /*Tresel value to be used for lte resel*/
  uint16 srchcr_tresel_gsm_def;
  
  SRCH_CELL_RESEL_RANK_LOG_PKT_type srchcr_rank_log_pkt;

  #ifdef FEATURE_FEMTO_CSG
  SRCH_CSG_CELL_RESEL_LOG_PKT_type srchcr_csg_cell_resel_log_pkt;

  #endif
  #ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY
  SRCH_ABS_PRIO_CELL_RESEL_RANK_LOG_PKT_type srchcr_rank_abs_prio_log_pkt;
  #endif

  WCDMA_IDLE_MEAS_LOG_PACKET_type wcdma_idle_meas_log_pkt;
  boolean need_submit_idle_meas_log_pkt;
  tlm_cfn_struct_type srch_start_end_cfn;

  #if defined (FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
  boolean srchcr_reacq_fail_cnt;
  #endif
}srchcr_ext_cntrl_params_type;

extern srchcr_ext_cntrl_params_type srchcr_ext_cntrl_params[WCDMA_NUM_SUBS];

void srchcr_ext_cntrl_params_init( wsub_id_e_type wsub_id);


/*===========================================================================

                   EXTERNAL VARIABLE DECLARATIONS

===========================================================================*/
/* Extern the variable here since its used in srchpch cleanup */


/* Timer to keep track number of frames to send out of service ind */
extern uint8 srchcr_out_of_service_timer;

extern srchcr_last_nbr_info_type srchcr_last_nbr;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

extern void srchcr_reset_rftune_state(boolean skip_if_interf_resel);

/*===========================================================================

FUNCTION       SRCHCR_EXCLUDE_CELL_FROM_RANKING

DESCRIPTION
  This function iterates through the searched cells list and if it
  finds the cell pointed to by the passed pointer, will mark this
  cell to be excluded from ranking during the current evaluation.

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  Will set the exclude flag to true for the cell pointed
  to by cell_ptr.
===========================================================================*/
void srchcr_exclude_cell_from_ranking( void* cell_ptr , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_NSET_CELL_RESELECT_EVAL

DESCRIPTION
  This function will rank the serving and neighbor cells according to
  the criteria specified in 25.304 (Section 5.2.6.1.4). If the neighbor
  cell meets the cell selection criteria, the cell reselection timer,
  Treselection, associated with the neighbor cell is incremented.
  If the neighbor cell meets the cell selection criteria for Treselection,
  and if the rank of this neighbor cell is better than the rank of the
  serving cell, it is selected as the best cell suitable for reselection.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the best cell (either the current serving cell or a new
  neighbor cell), NULL if no best cell was found.

SIDE EFFECTS
  None.
===========================================================================*/
extern srch_cell_struct_type* srchcr_nset_cell_reselect_eval( void );

/*===========================================================================

FUNCTION     srchcr_find_best_cell_and_reselect

DESCRIPTION
  This function is called when we first enter DRX and also after List search
  on SYNC_NSET and possibly a Step 1/2/3 search on ASYNC_NSET neighbor cells
  completes. This function will sort all the neighbor cells according to
  their energies and performs nset evaluation for cell reselection.

  If a cell was found better than the serving cell, send a
  RRC_CPHY_CELL_RESELECTION_IND indication to RRC.

  If the serving cell became the most suitable cell, report reacquisition
  success to DRX module.

  If no best cell was found, it means we lost the serving cell and also
  could not find a better neighbor cell. Send reacquisition failed status
  to DRX module.

  If the DRX module is waiting for the reacquisition/cell reselection
  evaluation to complete, notify DRX module we are done.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_find_best_cell_and_reselect( boolean use_legacy,srchcr_rank_cell_struct_type* cell_ptr, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_NSET_LIST_SRCH_DONE

DESCRIPTION
  This function is called when a NSET list search completes. If a step1
  search is due, this function will initiate a step1 search.

  If there is no more searching to be done, send SRCH_NSET_SRCH_SORT_DONE_CMD
  local command to L1M. If this search was started when we first entered DRX,
  sort and return to L1M. If we are performing reacquisition after coming
  out of sleep, L1M will send command to sort the nset and continue with
  cell reselection evaluation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_nset_list_srch_done(srch_done_cmd_type *cmd_ptr);

/*===========================================================================

FUNCTION      SRCHCR_INTRA_F_POST_LIST_SRCH_DUMP

DESCRIPTION
  This function calculates the ECIO and filt ECIO of the cells that
  were searched.

DEPENDENCIES
  This should be called after a list search is done and the
  function srch_list_srch_dump() is called.

RETURN VALUE
  None.

SIDE EFFECTS
  ECIO and Filt ECIO of searched cells are updated.
===========================================================================*/
void srchcr_intra_f_post_list_srch_dump( srch_queue_enum_type que );

/*===========================================================================

FUNCTION       srchcr_sort_intraf_nset

DESCRIPTION
  This function sorts just the neighbor cells that were just searched
  according to decreasing order of their filtered energies.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_sort_intraf_nset( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_ADD_PCCPCH_N

DESCRIPTION
  This function is called to set up the PCCPCH on the neighbor during
	cell reselection. A SFN-SFN difference between the neighbor and
	serving cell is performed after the PCCPCH_N has been successfully
	set up.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean indicating if the channel was added successfully or not.

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcr_add_pccpch_n
(
  dl_phychan_add_type* phch_cmd,
  uint16 freq
);

/*===========================================================================

FUNCTION      SRCHCR_RESET_T_RESEL

DESCRIPTION
  This function the Treselction timers associated with each neighbor cell.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_reset_t_resel( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_BSIC_IDENTIFY_DONE

DESCRIPTION
  This function performs the next step after a BSIC identification has
  been done. If a PCH cleanup is pending, a module cleanup done command
  will be sent to L1M.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_bsic_identify_done( srchcrgsm_local_cmd_type* cmd_ptr , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_INTER_F_NSET_STEP_SRCH_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_inter_f_nset_step_srch_cmd( srchcr_inter_f_idx_enum_type f_idx,
                                        boolean tune_rf );

/*===========================================================================

FUNCTION       SRCHCR_INTER_F_STEP1_SRCH_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_inter_f_step1_srch_cmd( uint16 freq, boolean tune_rf );

/*===========================================================================

FUNCTION       SRCHCR_INTER_F_STEP2_SRCH_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_inter_f_step2_srch_cmd( void );

/*===========================================================================

FUNCTION       SRCHCR_INTER_F_STEP3_SRCH_CMD

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_inter_f_step3_srch_cmd( void );
void srchcr_sort_inter_f_nset( wsub_id_e_type wsub_id);
void srchcr_nset_inter_f_cell_reselect_eval( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCH_INTER_F_LIST_SRCH_DUMP

DESCRIPTION
  This function retrieve List Search results from DSP memory to 
  search_cells[].

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srch_inter_f_list_srch_dump ( srch_queue_enum_type que );

/*===========================================================================

FUNCTION      SRCHCR_INTER_F_POST_LIST_SRCH_DUMP

DESCRIPTION
  This function calculates the ECIO and filt ECIO of the cells that
  were searched.

DEPENDENCIES
  This should be called after a list search is done and the
  function srch_inter_f_list_srch_dump() is called.

RETURN VALUE
  None.

SIDE EFFECTS
  ECIO and Filt ECIO of searched cells are updated.
===========================================================================*/
void srchcr_inter_f_post_list_srch_dump( srch_queue_enum_type que );

/*===========================================================================

FUNCTION SRCH_INTER_F_LIST_WIN_CTR

DESCRIPTION
  This function performs window centering on cells in the referenced search 
  queue, based on most recent search results. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srch_inter_f_list_win_ctr ( srch_queue_enum_type que , wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_INTER_F_NSET_LIST_SRCH_DONE

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_inter_f_nset_list_srch_done( srch_done_cmd_type* srch_cmd );

/*===========================================================================

FUNCTION       SRCHCR_INTER_F_STEP1_DONE

DESCRIPTION

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_inter_f_step1_done( srch_done_cmd_type* srch_cmd );
void srchcr_inter_f_step2_done( srch_done_cmd_type* srch_cmd );
void srchcr_inter_f_step3_done( srch_done_cmd_type* srch_cmd );

/*===========================================================================

FUNCTION   SRCHCR_FACH_GSM_NSET_RANK_SORT    

DESCRIPTION
  This function calculates the RSCP rank for CELL_FACH GSM neighbor cells
  and sor them in ranking order.

DEPENDENCIES
  Can be called in CELL_FACH state

RETURN VALUE
  None.

SIDE EFFECTS
  srch_gsm_nset_ptr[] is updated
  
===========================================================================*/
void srchcr_fach_gsm_nset_rank_sort( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_CALC_SERVING_CELL_RSCP_RANK

DESCRIPTION
  This function calculates the RSCP rank for the serving cell.

DEPENDENCIES
  Can ONLY be called when RF is in serving frequency in L1 task context.

RETURN VALUE
  None.

SIDE EFFECTS
  srch_aset_ptr[0] is updated
  
===========================================================================*/
void srchcr_calc_serving_cell_rscp_rank( wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION        SRCHCR_INIT_SFN_CTRS

DESCRIPTION
  This function initializes the global storing sfn of last aset list
  search and sfn based full search counters

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Full search sfn counters initialized

===========================================================================*/
void srchcr_init_sfn_ctrs( wsub_id_e_type wsub_id);

#ifdef FEATURE_CELL_FACH_RESEL_CHK_THRESH
/*===========================================================================

FUNCTION       SRCHCR_FACH_EXCLUDE_CELLS_USING_MEAS_RULES

DESCRIPTION
  This function limits the candidates for cell reselection in FACH state
  based on the current meas rules

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  srchcr_ext_cntrl_params[wsub_id].srchcr_srched_cells array of cells is updted
  
===========================================================================*/
void srchcr_fach_exclude_cells_using_meas_rules ( wsub_id_e_type wsub_id);
#endif

/*===========================================================================

FUNCTION       SRCHCR_CALC_INTRA_NEIGHBOR_PENALTY_AND_H

DESCRIPTION
  This function calculates updates the penalty timer and calculates H
  for the neighbor cell

DEPENDENCIES
  Can ONLY be called when HCS is used on the current serving cell

RETURN VALUE
  None.

SIDE EFFECTS
  srch_cell_struct_type for the neighbor is updated
  
===========================================================================*/
void srchcr_calc_intra_neighbor_penalty_and_h(srch_cell_struct_type* n_ptr,
                                              uint16 time_elapsed,
                                              boolean apply_penalty, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_CALC_INTER_NEIGHBOR_PENALTY_AND_H

DESCRIPTION
  This function calculates updates the penalty timer and calculates H
  for the neighbor cell

DEPENDENCIES
  Can ONLY be called when HCS is used on the current serving cell

RETURN VALUE
  None.

SIDE EFFECTS
  srch_cell_struct_type for the neighbor is updated
  
===========================================================================*/
void srchcr_calc_inter_neighbor_penalty_and_h(srch_interf_cell_struct_type* n_ptr,
                                              uint16 time_elapsed,
                                              boolean apply_penalty, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_CALC_GSM_NEIGHBOR_PENALTY_AND_H

DESCRIPTION
  This function calculates updates the penalty timer and calculates H
  for the gsm neighbor cell

DEPENDENCIES
  Can ONLY be called when HCS is used on the current serving cell

RETURN VALUE
  None.

SIDE EFFECTS
  srch_cell_struct_type for the neighbor is updated
  
===========================================================================*/
void srchcr_calc_gsm_neighbor_penalty_and_h(srch_gsm_cell_struct_type* n_ptr,
                                            uint16 time_elapsed, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_EXCLUDE_CELLS_USING_H

DESCRIPTION
  This function limits the candidates for cell reselection based
  on H value

DEPENDENCIES
  Can ONLY be called when HCS is used on the current serving cell and 
  UE is not in fast moving state

RETURN VALUE
  None.

SIDE EFFECTS
  srchcr_ext_cntrl_params[wsub_id].srchcr_srched_cells array of cells is updted
  
===========================================================================*/
void srchcr_exclude_cells_using_h ( wsub_id_e_type wsub_id);
      
/*===========================================================================

FUNCTION       SRCHCR_FACH_EXCLUDE_CELLS_USING_MEAS_RULES_HCS

DESCRIPTION
  This function limits the candidates for cell reselection in FACH state
  based on the current HCS meas rules

DEPENDENCIES
  Can ONLY be called after the meas_rules function has correctly set the
  meas_detect flag for the cells.

RETURN VALUE
  None.

SIDE EFFECTS
  srchcr_ext_cntrl_params[wsub_id].srchcr_srched_cells array of cells is updted
  
===========================================================================*/
void srchcr_fach_exclude_cells_using_meas_rules_hcs (void);

/*======================================================================
FUNCTION SRCHCR_RESTORE_NEIGH_CELL_PENALTY_TIMER

DESCRIPTION
    Restores the penalty timer info for all neighbor cells

RETURN VALUE
    void

SIDE EFFECTS
    Neighbor cell list penalty timers are updated from backup info
=======================================================================*/
void srchcr_restore_nbr_penalty_timer(dword elapsed_time, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCR_SET_NUM_FRAMES_SINCE_PREV_EVAL

DESCRIPTION
  Function to set num frames since previous eval

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
=======================================================================*/
extern void srchcr_set_num_frames_since_prev_eval( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       SRCHCR_UPDATE_MEAS_RULES

DESCRIPTION
  This function updates meas rules for all types of meas

DEPENDENCIES
  None.

INPUTS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Following globals updated to reflect the latest measurement rules
    srchcr_ext_cntrl_params[wsub_id].srchcr_current_meas_rules
    srchcr_current_meas_prio
===========================================================================*/
extern void srchcr_update_meas_rules( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       SRCHCR_ASET_S_CRITERIA_PROCEDURES

DESCRIPTION
  This function performs following S criteria related procedures
  (a) Update aset S criteria
  (b) Check against immediate reselection at low EcIo threshold
  (c) Perform S criteria success or failure procedures

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  Serving cell S criteria calculated, Tresel_fdd updated, S criteria
  success or failure procedures performed.
  
===========================================================================*/
extern void srchcr_aset_s_criteria_procedures( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_PERFORM_CELL_RES_EVAL

DESCRIPTION
  This function is called to perform cell reselection evaluation

  It first calls functions to calculate the ngbr cell rank and then 
  If there is no more searching to be done, send SRCHCR_NSET_SRCH_DONE_CMD
  local command to L1M. If this search was started when we first entered DRX,
  sort and return to L1M. If we are performing reacquisition after coming
  out of sleep, L1M will send command to sort the nset and continue with
  cell reselection evaluation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_perform_cell_res_eval(srchcr_res_eval_meas_type res_eval_meas_comp, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       srchcr_perform_cell_res_eval_abs_prio ()

DESCRIPTION
  This function is used only for REL8 UE's when Absolute Priotity 
  is used for resel eval. 
  This is just a wrapper function to which calls the inter-f/gsm/LTE resel eval 
  functions based on the current scheduling mode.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_perform_cell_res_eval_abs_prio( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION       SRCHCR_SET_T_RESEL_FDD

DESCRIPTION
  This function set the Treslection value to be used for FDD neighbor cells
  based on serving cell S criteria and EcIo

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Treselection for FDD neighbors set as per optimizations
  
===========================================================================*/
extern void srchcr_set_t_resel_fdd( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCR_DO_PENDING_PCH_CLEANUP

DESCRIPTION
  This function is called to perform pending PCH cleanup

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_do_pending_pch_cleanup( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCR_DO_IRAT_SRCH

DESCRIPTION
  This function is called to perfom irat srch - RSSI or BSIC

DEPENDENCIES
  None.

RETURN VALUE
  Boolean - TRUE if search is started, FALSE otherwise

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcr_do_gsm_srch(boolean rssi_due, wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION       SRCHCR_CALC_ASET_S_CRITERIA

DESCRIPTION
  This function calculates and updates the S criteria of the serving cell.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchcr_calc_aset_s_criteria( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCR_START_STEP1_SRCH

DESCRIPTION
  This function starts step-1 search in idle mode

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_start_step1_srch(void);

/*===========================================================================
FUNCTION      SRCHCR_START_STEP2_SRCH

DESCRIPTION
  This function starts step-2 search in idle mode

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_start_step2_srch(uint16 peak_idx);

/*===========================================================================
FUNCTION      SRCHCR_START_STEP3_SRCH

DESCRIPTION
  This function starts step-3 search in idle mode

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_start_step3_srch(uint16 peak_idx);

extern void srchcr_update_rssi_all_cell(W_ARFCN_T arfcn, int16 filt_rssi, wsub_id_e_type wsub_id);
extern void srchcr_set_flag_to_sort_entire_nset_after_full_srch(boolean sort, wsub_id_e_type wsub_id);
extern void srchcr_set_flag_to_sort_nset_after_multipath_srch(boolean sort, wsub_id_e_type wsub_id);
void srchcr_eliminate_aset_and_nset_values
(
  srch_cell_struct_type *set_ptr[],
  uint16 max_nof_cells
, wsub_id_e_type wsub_id);
extern void srchcr_eliminate_duplicate_peaks_in_step1_srch( wsub_id_e_type wsub_id);
extern void srchcr_eliminate_duplicate_peaks_in_step1_srch_inter( wsub_id_e_type wsub_id);
void srchcr_clear_srched_cells( wsub_id_e_type wsub_id);
boolean srchcr_aset_squal_meets_gsm_thresh( wsub_id_e_type wsub_id);
/*===========================================================================

FUNCTION       srchcr_interf_rank_ecio

DESCRIPTION
  This function calculates the EcIo rank of the inter-frequency neighbor cells and if the cell meets
  cell selection criteria and is better ranked than the serving cell,
  the Treselection will be incremented.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/

void srchcr_interf_rank_ecio
(
  srchcr_res_eval_meas_type interf_cell_type,
  srch_interf_cell_struct_type* inter_cell_ptr,
  boolean hcs_used_for_cr_eval, 
  boolean is_csg_ranking,
  int16 hist_add,
  uint16 time_elapsed,
  wsub_id_e_type wsub_id
);


srchcr_rank_cell_struct_type* srchcr_find_best_ecno_cell
(
  void* curr_best_cell_ptr,
  srchcr_cell_type_enum_type cell_type,
  boolean skip_evals_do_ranking, 
  boolean is_csg_ranking
, wsub_id_e_type wsub_id);
void srchcr_fach_nset_inter_f_cell_reselect_eval( wsub_id_e_type wsub_id);
srchcr_rank_cell_struct_type* srchcr_find_best_gsm_cell
(
  srchcr_rank_cell_struct_type* cell_ptr
, wsub_id_e_type wsub_id);
srchcr_rank_cell_struct_type* srchcr_find_best_ranked
(
  /* pointer to the current best cell */
  void* curr_best_cell_ptr
, wsub_id_e_type wsub_id);
boolean srchcr_find_next_best_ranked
(
  /* pointer to the current best cell */
  void* curr_best_cell_ptr
, wsub_id_e_type wsub_id);
boolean srchcr_check_gsm( wsub_id_e_type wsub_id);
void srchcr_hcs_det_neigh_srch( wsub_id_e_type wsub_id);
void srchcr_prune_uset_cells( wsub_id_e_type wsub_id);
void srchcr_intraf_nset_cell_reselect_eval( wsub_id_e_type wsub_id);
void srchcr_proc_inter_f_srch( wsub_id_e_type wsub_id);
void srchcr_nset_gsm_cell_reselect_eval( wsub_id_e_type wsub_id);
void srchcr_find_next_bsic_id( wsub_id_e_type wsub_id);
void srchcr_start_bsic_id_srch( wsub_id_e_type wsub_id);
void srchcr_start_n_srch( void );
void srchcr_init_intra_f_nset( wsub_id_e_type wsub_id);
void srchcr_init_inter_f_nset( wsub_id_e_type wsub_id);
void srchcr_init_timers( wsub_id_e_type wsub_id);
boolean srchcr_trigger_cell_resel_ind( wsub_id_e_type wsub_id);
void PRINT_BARRED_CELLS( wsub_id_e_type wsub_id);
void srchcr_add_cell_to_barredlist (l1_cell_bar_cmd_type *barred_cell, wsub_id_e_type wsub_id);
void srchcr_gsm_cell_resel_ind( srch_gsm_cell_struct_type* new_cell_ptr , wsub_id_e_type wsub_id);
void srchcr_exclude_cells_using_prio( int16 included_priority, wsub_id_e_type wsub_id);
void srchcr_update_prio_state(boolean low_prio, boolean hi_prio, prio_gp_state *prio_state);
void srchcr_qualify_intraf_cell( boolean low_prio, boolean eq_prio, boolean hi_prio, wsub_id_e_type wsub_id);
boolean srchcr_qualify_interf_cell( boolean low_prio, boolean eq_prio, boolean hi_prio, wsub_id_e_type wsub_id);
void srchcr_qualify_gsm_cell(boolean low_prio,boolean hi_prio, wsub_id_e_type wsub_id);
void srchcr_check_and_remove_uset_cells( wsub_id_e_type wsub_id);
boolean srchcr_inter_f_check_qualified_async_cells(srchcr_inter_f_idx_enum_type freq_index, wsub_id_e_type wsub_id);
boolean srchcr_inter_f_check_qualified_sync_cells(srchcr_inter_f_idx_enum_type freq_index, wsub_id_e_type wsub_id);
boolean srchcr_inter_f_check_all_sync_cells_below_threshold(srchcr_inter_f_idx_enum_type freq_index, wsub_id_e_type wsub_id);
boolean srchcr_inter_f_list_srch_possible_for_freq(srchcr_inter_f_idx_enum_type freq_index, wsub_id_e_type wsub_id);
extern boolean srchcr_inter_f_full_srch_possible(srchcr_inter_f_idx_enum_type freq_index,boolean serving_cell_reacq_successful, wsub_id_e_type wsub_id);
boolean srchcr_check_inter_f_list_srch_in_gap(uint16 *freq_to_meas, wsub_id_e_type wsub_id);
void srchcr_inter_f_srch_in_gap_send_cmd(void);
void srchcr_update_res_eval_status(void);
void srchcr_set_res_eval_status_on_pich_wakeup(void);
extern void srchcr_add_inter_f_sync_cells_to_eval_list( wsub_id_e_type wsub_id);
extern void srchcr_intraf_nbr_pn_srch_done_handler( wsub_id_e_type wsub_id);
extern void srchcr_interf_nbr_pn_srch_done_handler( wsub_id_e_type wsub_id);
extern void srchcr_intraf_full_srch_done_handler
(
  boolean aset_meas_updated,
  boolean aset_reacquired
, wsub_id_e_type wsub_id);
extern void srchcr_interf_full_srch_done_handler(uint8 prev_sync_cnt, wsub_id_e_type wsub_id);
extern uint16 srchcr_add_intraf_timing_known_nbr_cells
(
  uint16 max_pn_tasks,
  srch_cell_struct_type **cell_list_pptr
, wsub_id_e_type wsub_id);
extern uint16 srchcr_add_inter_f_timing_known_nbr_cells
(
  uint16 max_pn_tasks,
  srch_interf_cell_struct_type **interf_cell_pptr
, wsub_id_e_type wsub_id);
extern uint16 srchcr_add_intraf_timing_known_hcs_nbr_cells
(
  uint16 max_pn_tasks,
  srch_cell_struct_type **cell_list_pptr
);
extern uint16 srchcr_add_inter_f_timing_known_hcs_nbr_cells
(
  uint16 max_pn_tasks,
  srch_interf_cell_struct_type **interf_cell_pptr
);

extern void srchzz_bsic_id_aborted (W_ARFCN_T aborted_arfcn,
   srchcm_gsm_bsic_struct_type aborted_bsic, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCR_TBARRED_TIMER_UPDATE

DESCRIPTION
    This function is called before ranking a cell for cell reselection and
    acquisition to update the t-barred values in the barred cell list based 
    on the time elapsed value.
    Barred cells are unbarred when T-barred timer elapses and holes in the list
    plugged if created.
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_tbarred_timer_update (rex_timer_cnt_type time_elapsed, wsub_id_e_type wsub_id);
/*===========================================================================
FUNCTION       SRCHCR_UPDATE_LOG_PKT_RESEL_STATUS

DESCRIPTION     This function updates 'resel_status' of each cell ptr in 0x4005/0x4027 log packet 

DEPENDENCIES
  

RETURN VALUE

SIDE EFFECTS
  
===========================================================================*/
void srchcr_update_log_pkt_resel_status(wsub_id_e_type wsub_id);
/*============================================================================
FUNCTION: SRCHCR_UPDATE_CR_RANK_LOG_PKT_PARMS

DESCRIPTION: This function is used to update the 0x4005 log packet info for 
all cells. 
      
DEPENDENCIES:
None.

RETURN VALUE:
None.

SIDE EFFECTS:
=============================================================================*/
void srchcr_update_cr_rank_log_pkt_parms(srchcr_cell_type_enum_type cell_type, void *cell_ptr, boolean rerank_fdd, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_UPDATE_ABS_PRIO_LOG_PKT_PARMS

DESCRIPTION: This function is used to update the 0x4027 log packet info for 
all cells. 
      
DEPENDENCIES:
None.

RETURN VALUE:
None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_update_abs_prio_log_pkt_parms(srchcr_cell_type_enum_type cell_type, void *cell_ptr, boolean absolute_criteria_pass,boolean rerank_fdd, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_INTRAF_NGBR_FAIL_ECIO_CHECK

DESCRIPTION: This api checks if all the sync set neighbor's EcIo is below -16 dB or not
      
DEPENDENCIES:
None.

RETURN VALUE:
TRUE: All the intraf sync neighbor's EcIo is below -16dB
FALSE: Atleast one of the neighbor has EcIo better than -16dB

SIDE EFFECTS:
None
=============================================================================*/
extern boolean srchcrutils_intraf_ngbr_fail_ecio_check( wsub_id_e_type wsub_id);

#if defined(FEATURE_WCDMA_TO_LTE) && defined(FEATURE_WCDMA_ABSOLUTE_PRIORITY)
/*============================================================================
FUNCTION: SRCHCR_DO_LTE_SEARCH

DESCRIPTION:
  This function checks if LTE INIT message has been sent or not. If not sent, 
  then it sends the INIT message, in this case, search will be scheduled when
  LTE INIT cnf is received. If LTE INIT message has already been sent, it calls
  the wsrchlte function to schedule the search
      
DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - If either LTE INIT gets sent or a search gets started.
  FALSE - If search start fails

SIDE EFFECTS:
=============================================================================*/
extern boolean srchcr_do_lte_search(wsrchlte_srch_enum_type srch_type, wsrchlte_frequency_info_type *earfcn_info, wsub_id_e_type wsub_id) ;


/*===========================================================================
FUNCTION      SRCHCR_LTE_SEARCH_SENT_TO_DRIVER

DESCRIPTION
  This function checks that if the IRAT state machine indicates 
  that measurement are in progress. If this functions return TRUE 
  and ext_srchzz_cntrl->srchzz_ext_cntrl_params_mdump.srchzz_substate is SRCHZZ_LTE, it indicates that search has 
  been sent to driver 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If search has been sent to driver 
  FALSE - Otherwise 

SIDE EFFECTS
  None.
===========================================================================*/
extern boolean srchcr_lte_search_sent_to_driver( wsub_id_e_type wsub_id);

extern void srchcr_fill_csg_lte_thresholds (srchcr_csg_lte_cell_struct_type* lte_cell_ptr, wsub_id_e_type wsub_id);

#endif


/*===========================================================================
FUNCTION      SRCHCR_IDLE_MODE_W2X_CFG_CNF_CB

DESCRIPTION
  This function should be called in interrupt context when a w2x_idle_cfg_cmd 
  is acknowledged by WFW.  It will update the search state variable and send a
  local message to wl1 indicating it should pickup the search process where it 
  left off.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_idle_mode_w2x_cfg_cnf_cb(uint16 status, wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCR_IRAT_MEAS_STATE_MC

DESCRIPTION
  This function is the IRAT state machine

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_irat_meas_state_mc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCR_RESET_IRAT_MEAS_STATE_MC

DESCRIPTION
  This function re-initializes the IRAT state machine

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_reset_irat_meas_state_mc( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCR_SEND_START_AGC_CMD_TO_FW

DESCRIPTION
  This function sends a start AGC cmd to FW

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_send_start_agc_cmd_to_fw( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCR_SEND_STOP_AGC_CMD_TO_FW

DESCRIPTION
  This function sends the idle mode w2x command to FW and then calls the state 
  machine function so a transition to the next state can occur.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_send_w2x_cmd_to_fw( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_UPDATE_FULL_SRCH_COND_VAL

DESCRIPTION:
  This function updates the values needed for performing additional full search
  conditions
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_update_full_srch_cond_val(srchcr_freq_idx_enum_type freq_idx, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_CHECK_ADDTNL_COND_FOR_FULL_SRCH

DESCRIPTION:
  If the full search on any frequency is satisfied, a set of calculations are
  done, to figure out if the full search results in any cells with good energy.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - If full search would be useful.
  FALSE - If full search not needed.

SIDE EFFECTS:
=============================================================================*/
extern boolean srchcr_check_addtnl_cond_for_full_srch(srchcr_freq_idx_enum_type freq_idx, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_UPDATE_STEP1_THRESHOLDS

DESCRIPTION:
  This function updates the step1 search thresholds based on the state
  of the UE(Stationary/Mobile)
      
DEPENDENCIES:
  None.

RETURN VALUE:

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_update_step1_thresholds(srchcr_freq_idx_enum_type freq_idx, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_UPD_TMR_BASED_ON_CELL_ENG

DESCRIPTION:
  If the energy of the cell is less than threshold value(-25), a 15s timer is
  decremented and when the timer expires the cell is removed from the monitored
  set.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_upd_tmr_based_on_cell_eng(srchcr_freq_idx_enum_type freq_idx, wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_HS_FACH
/*===========================================================================

FUNCTION        srchfach_reset_tresel_ngbr_cells

DESCRIPTION     This function resets Tresel for IF and GSM based on the cell_type passed.   
                This function will be called if FMO for that RAT is skipped for N_FMOSkip times.

DEPENDENCIES    None

RETURN VALUE    None
  
SIDE EFFECTS    Tresel for all IF / GSM ngbr cells will be reset to 0.
                
===========================================================================*/
void srchcr_reset_tresel_ngbr_cells(srchcr_cell_type_enum_type cell_type, wsub_id_e_type wsub_id);
#endif /* FEATURE_WCDMA_HS_FACH */

#ifdef FEATURE_DUAL_SIM
/*===========================================================================
FUNCTION     srchcr_decfg_cell_for_interf_resel_cb

DESCRIPTION
  This is a CB function which triggers a serving cell configure/deconfigure soon 
  after configuring/deconfiguring of the neighbor cell

DEPENDENCIES
  None.

RETURN VALUE

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcr_decfg_cell_for_interf_resel_cb( wsub_id_e_type wsub_id);
#endif

#ifdef FEATURE_FEMTO_CSG

/*============================================================================
FUNCTION: srchcr_perform_csg_eval_procedures()

DESCRIPTION:
  This function 
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
=============================================================================*/
extern void srchcr_perform_csg_eval_procedures (srchcr_res_eval_meas_type res_eval_meas_comp, wsub_id_e_type wsub_id);

#endif

#ifdef FEATURE_DUAL_SIM
/*============================================================================
FUNCTION: SRCHCR_GSM_SRCH_IN_THIS_DRX

DESCRIPTION: This function is used to check if GSM searh is performed in this
DRX cycle(using srchcr_ext_cntrl_params[wsub_id].srchcr_gsm_cell_res_eval_sfn)

      
DEPENDENCIES:
  None.

RETURN VALUE:
  TRUE - If GSM search in this DRX
  FASLE - Otherwise

SIDE EFFECTS:
=============================================================================*/
extern boolean srchcr_gsm_srch_in_this_drx( wsub_id_e_type wsub_id);
#endif /*FEATURE_DUAL_SIM */

#ifdef FEATURE_WCDMA_ENH_OOS_RXD_PWC
/*===========================================================================

FUNCTION     SRCHCR_CHECK_OOS_CRIT_USING_SUM_PRI_DIV

DESCRIPTION
 Computes total energy based on SUM(PRIM,DIV) energies and 
 calculates Srxlev and Squal for OOS determination

DEPENDENCIES
  None.

RETURN VALUE
  True  - If serving cell (Squal >0 and Srxlev > 0)
  False - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
void  srchcr_check_oos_crit_using_sum_pri_div( wsub_id_e_type wsub_id);
#endif
/*===========================================================================

FUNCTION SRCHCR_SORT_NSET_CELLS

DESCRIPTION
  This functions sorts the peaks from the full search/PN search results

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchcr_sort_nset_cells( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_CALC_SERVING_CELL_RANK

DESCRIPTION
  This function calculates the RSCP or EcIo rank for the serving cell 

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  srch_aset_ptr[0] is updated
  
===========================================================================*/
void srchcr_calc_serving_cell_rank( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCRUTILS_UPDATE_FAST_MOVING_CONDITION

DESCRIPTION
  This function checks if UE is in fast mvoing and can exit fast moving

DEPENDENCIES
  

RETURN VALUE
  None.

SIDE EFFECTS
  UE fast moving condition updated
  
===========================================================================*/
void srchcr_update_fast_moving_condition( wsub_id_e_type wsub_id);

#ifdef FEATURE_IGNORE_TRESEL_ON_S_FAIL
/*===========================================================================

FUNCTION       SRCHCR_UPDATE_WEAK_RESEL_CONDITION

DESCRIPTION
  This function checks if weak resel condtion's end time has been reached. If so update reset the weak resel flag.
DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  
===========================================================================*/
void srchcr_update_weak_resel_condition( wsub_id_e_type wsub_id);
#endif

/*===========================================================================

FUNCTION       SRCHCRUTILS_PREPARE_SRCHED_CELLS

DESCRIPTION
  This function clears WCDMA cells in FACH state and all cells otherwise from
  the searched cells list and sets the srched cellcounter accordingly.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void srchcr_prepare_srched_cells( boolean force_clear , wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCRUTILS_CALCULATE_TIME_SINCE_PREV_EVAL

DESCRIPTION
  This function is called to calculate the time since previous resel eval

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_calculate_time_since_prev_intraf_eval( wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION      SRCHCRUTILS_CALCULATE_TIME_SINCE_PREV_INTERF_EVAL

DESCRIPTION
  This function is called to calculate the time since previous InterF resel eval

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchcr_calculate_time_since_prev_interf_eval(srchcr_inter_f_idx_enum_type freq_idx, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCRUTILS_ADD_SERV_CELL_TO_RANK_LOG_PKT

DESCRIPTION
  This function will add the Serving Cell to the Ranking log packet (Serving cell is always placed first)

DEPENDENCIES
  None.

RETURN VALUE
NONE

SIDE EFFECTS
  None.

===========================================================================*/
void srchcr_add_serv_cell_to_rank_log_pkt( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      SRCHCRMEAS_PROCESS_NASTT_UNMON_PEAKS

DESCRIPTION
 This function is called after NASTT is completed. It processes the NASTT peaks and updates
 cell energies if they are already known. Else, we will try to allocate a cell in USET and save
 the newly found cell
 
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchcrmeas_process_nastt_unmon_peaks( boolean *aset_meas_updated, boolean *aset_reacquired, wsub_id_e_type wsub_id);


#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY

/*===========================================================================

FUNCTION      SRCHCRUTILS_GSM_HIGH_PRIO_CELL_FOUND

DESCRIPTION
  Find if GSM HighPrio cells were found in the last HighPrio search

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void  srchcr_gsm_high_prio_cell_found( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_PRIOR_INFO_GIVEN_FOR_FREQ

DESCRIPTION:
  This fn check if a given frequency is already present in the inter frequency 
  priority structure sent as a part of the 3RD_MEAS_REQ.
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:   
  None. 
=============================================================================*/

extern boolean srchcr_prior_info_given_for_freq(uint16 freq,l1_meas_priority_info_type *prior_info,uint32 *freq_idx, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCR_UPDATE_INTERF_PRIORITY_INFO

DESCRIPTION:
  This fn updates the priority info from the 3rd MEAS_REQ, for the passed 
  inter frequency index. 
  
      
DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:   
  None. 
=============================================================================*/
extern void srchcr_update_interf_priority_info(uint32 freq_idx,l1_inter_freq_priority_info_type *prior_info, wsub_id_e_type wsub_id);


#endif

#ifdef FEATURE_WCDMA_ABSOLUTE_PRIORITY_FACH
/*============================================================================
FUNCTION: SRCHCR_MARK_ABS_PRIO_FACH_RESEL_CAND

DESCRIPTION:
  This function loops through the srched_cells array and marks reselection Inter/IRAT candidates based on
  absolute priority rules
      
DEPENDENCIES:
  None.

RETURN VALUE:
NONE

SIDE EFFECTS:
NONE
=============================================================================*/
void srchcr_mark_abs_prio_fach_resel_cand(srchcr_cell_type_enum_type cell_type, 
    srchcr_priority_enum_type priority_type, wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: SRCHCRUTILS_MARK_ALLOWED_FREQ

DESCRIPTION:
  This function is called when scheduling mode changes in FACH. It updates the srch_allowed flag in
  driver search list for the next FMO RAT. Driver uses this information in scheduling the freq for a search
  in the following FMO gap.
      
DEPENDENCIES:
  None.

RETURN VALUE:
NONE

SIDE EFFECTS:
NONE
=============================================================================*/
void srchcr_mark_allowed_freq(srchcr_cell_type_enum_type cell_type, wsub_id_e_type wsub_id);

void srchcr_upd_fach_absprio_parms(l1_meas_priority_eutra_cell_list_info_type *info, wsub_id_e_type wsub_id);

#endif

/*===========================================================================

FUNCTION SRCHCRUTILS_RESET_TRESEL_FOR_INTRAF_CELLS

DESCRIPTION
  This functions resets the Treselection counter and enabled values

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcr_reset_tresel_for_intraf_cells(uint8 carr_idx, srch_set_enum_type set, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCRUTILS_RESET_TRESEL_FOR_INTERF_CELLS

DESCRIPTION
  This functions resets the Treselection counter and enabled values for INTERF cells

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcr_reset_tresel_for_interf_cells(srch_interf_set_enum_type set, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION SRCHCRUTILS_RESET_TRESEL_FOR_GSM_CELLS

DESCRIPTION
  This functions resets the Treselection counter and enabled values for GSM cells

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void srchcr_reset_tresel_for_gsm_cells(srch_gsm_set_enum_type set, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHZZ_BLOCK_MEAS_THRU_NV

DESCRIPTION
  This function pushes a measurement to a new gap if the NV is set accordingly. 
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If the search needs to be pushed to a measurement only gap
  FALSE - Otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean srchcr_block_meas_thru_nv(srchcr_res_eval_meas_type srch_to_be_performed,wsub_id_e_type wsub_id);

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================

FUNCTION WL1_POPULATE_DEBUG_SRCHCR_INFO_STRUCT_TYPE

DESCRIPTION
   This function dumps state info into a global strcuture as part of a crash

DEPENDENCIES
  None.

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
void wl1_populate_debug_srchcr_info_struct_type(wl1_cell_resel_mini_debug_info_struct_type * srchcr_cell_resel_dump_ptr);

/*===========================================================================

FUNCTION WL1_CELL_RESEL_DUMP_CB

DESCRIPTION
   This function passes the dump pointer address to populate the dump info

DEPENDENCIES
  None.

RETURN VALUE
  NONE

SIDE EFFECTS
  None.

===========================================================================*/
void wl1_cell_resel_dump_cb(void* addr_ptr);
#endif /* FEATURE_WCDMA_QSH_DUMP*/

/*===========================================================================

FUNCTION      SRCHCR_CHECK_IF_BSIC_POSSIBLE

DESCRIPTION
  This function sets the global with index on which BSIC has to be attempted.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
boolean srchcr_check_if_bsic_possible( wsub_id_e_type wsub_id);

/*============================================================================
FUNCTION: srchcr_allocate_rf_bufs_and_trm_extn_for_irat

DESCRIPTION: This api checks with TRM if an IRAT search can be performed.
      
DEPENDENCIES:
None.

RETURN VALUE: 
 TRUE - IF the search is allowed by TRM
 FALSE - IF the search request is denied by TRM
SIDE EFFECTS:
None
=============================================================================*/
mcalwcdma_rf_tune_type srchcr_allocate_rf_bufs_and_trm_extn_for_irat(wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHCR_CAN_USET_BE_EXLCUDED

DESCRIPTION
  This function checks if USET cells can be excluded. 
 
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - If we can skip USET cell measurement
  FALSE - Otherwise

SIDE EFFECTS
  None.
===========================================================================*/
boolean srchcr_can_uset_be_exlcuded(wsub_id_e_type wsub_id);

extern void srchcr_debug_crash_check(wsub_id_e_type wsub_id);

/*===========================================================================
FUNCTION     SRCHZZ_FORCE_RANK_W_CELLS

DESCRIPTION
  This function is called at the beginning of check force rank to force rank W cells
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void srchzz_force_rank_w_cells(srchcr_res_eval_meas_type *res_eval_meas_comp, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION     SRCHZZ_CHECK_FORCE_RANKING

DESCRIPTION
  This function is called at the beginning of cell reselection evaluation to check whether or not cells need to be forcefully ranked
  during the current evaluation. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchzz_check_force_ranking(wsub_id_e_type wsub_id, srchcr_res_eval_meas_type *res_eval_meas_comp);

/*===========================================================================

FUNCTION SRCHCR_IS_NEXT_SRCH_BSIC_RECONF

DESCRIPTION
   This function returns if the next srch is bsic reconf or not.

DEPENDENCIES
  None.

RETURN VALUE
  Boolean - next srch is bsic reconf or not

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean srchcr_is_next_srch_bsic_reconf(wsub_id_e_type wsub_id);


/*===========================================================================

FUNCTION       SRCHCR_CHECK_AND_ABORT_ONGOING_RESELECTION

DESCRIPTION
  This function checks if the current reselection needs to be aborted 
  based on the newly ranked best cell. If the rselection needs to be 
  aborted, this function aborts the reselection.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if DL WEAK IND is sent to RRC

SIDE EFFECTS
  Cell reselection aborted using DL_WEAK_IND
  
===========================================================================*/
extern boolean srchcr_check_and_abort_ongoing_reselection ( wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION       SRCHCR_CHECK_AND_ABORT_ONGOING_INTER_RESELECTION

DESCRIPTION
  This function checks if the current interf reselection needs to be aborted 
  based on the newly ranked best cell. If the interf rselection needs to be 
  aborted, this function aborts the interf reselection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Cell reselection aborted using DL_WEAK_IND
  
===========================================================================*/
extern void srchcr_check_and_abort_ongoing_inter_reselection( wsub_id_e_type wsub_id);

/*==========================================================================
FUNCTION      srchzz_fill_idle_meas_log_pkt_paras

DESCRIPTION
  This function fills the parameters for the wcdma idle meas log packet (0x41CF).

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void srchzz_fill_idle_meas_log_pkt_params(srchcr_res_eval_meas_type srch_type, boolean srch_started, wsub_id_e_type wsub_id);

extern void srchzz_fill_srch_duration_for_idle_meas_log_pkt(srchcr_res_eval_meas_type srch_type, wsub_id_e_type wsub_id);

/*===========================================================================

FUNCTION      srchzz_fill_idle_meas_log_pkt_paras_wrapper

DESCRIPTION
  This function fills the parameters for the wcdma idle meas log packet (0x41CF) when srch_type contains more than one srch.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void srchzz_fill_idle_meas_log_pkt_params_wrapper(srchcr_res_eval_meas_type srch_type, skipped_srch_reason_enum_type reason_bitmask, boolean srch_started, wsub_id_e_type wsub_id);

#ifdef FEATURE_QSH_MDUMP 
extern QSH_MDUMP_FN_ATTR void wl1_qsh_mdump_collect_resel(wsub_id_e_type wsub_id);
#endif
#endif /* SRCHCR_H */
