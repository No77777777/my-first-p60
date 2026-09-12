/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===

                   L 1   U T I L I T Y   S E R V I C E S

GENERAL DESCRIPTION
   This module contains the utility functions for Layer 1.

EXTERNALIZED FUNCTIONS
  L1_sort_signal_strengths
  L1_condition_rxlev
  L1_normalise_signal_strengths
  L1_calculate_paging_data
  L1_calculate_ARFCN
  L1_convert_word
  L1_print_hex

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_utils.c#5 $
$DateTime: 2020/04/29 10:32:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
11/04/17   rv        CR2031827 Excessive F3 log reduction for Oppo
21/08/17   sk        CR2093261 Dual space IMS meas data and report meas as state if TRM not available during previous DRX period
03/03/17   km        CR2006922 Before sending GTS timing info on timer expiry check if current state is DED/DTM/Tranfer
19/01/17   sn        CR1105051 When Unlock Request was Cancelled during RACH state , schedule RACH if pending
09/02/16   mk        CR2002074 Add LB_MS_TXPWR_MAX_CCH for dedicated Tx power level initialization
03/01/18   ksb       CR2164555 Geran support to send Global Time Services Information TO GPS module
17/04/17   nm        CR2032010 make rf power class variable dual data spaced
27/12/16   sm        CR1105461 Resolve Compilation error when QHS flags are enabled.Compilation error is because of CR1089963.
11/29/16   sm        CR1089963 Added state transition events and BCCH abort in W2G stare handler, to reproduce issue CR/1082619 
19/09/16   cws       CR1067531 Use turn tx off API which checks Tx status before sending Tx sleep req to RF
07/07/17   snjv      CR2073295 Integer Overflow When Avg. Tx pwr for MSlot Config in Packet Transfer
28/04/17   og/snjv   CR2048710 For MCS STX PWR IND: Avoid IIR Filter and PCL Conversion when averaging
24/03/17   nm        CR2023665 Correct the Tx power averaging required for SAR reporting
12/10/16   gmr       CR1036802 Gl1 to use actual Tx power in MCS power reports for RT SAR feature
28/12/15   br        CR 955168 GL1 to disable TX during rach to find bcch state transition 
15/12/17   ap        CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
29/03/16   mn        CR995137 Access tune away support: Allowing other tech's paging at GPRS access
17/08/17   br        CR2092505 NV control for logging GDET related F3s
28/09/16   br        CR1071711 Removal of F3 which is causing the ISR overrun.
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
16/03/17   mn        CR2020307 FR40846:Packet transfer Error Recovery mechanism.
03/01/16   mn        CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
07/06/16   sk        CR1028080 Utility to get GSM Serving cell measurements for IMS use
06/09/15   br        CR1063442 FR 38050: AsDiv in Acquisition for CSFB in multisimode.
03/11/15   ss        CR916273 FR 29852: AsDiv in Acquisition.
09/07/16   mn        CR1063231:Changing global variable l1_utils_find_bcch_mcvs_on to mutiple dataspace 
24/05/16   sn        CR1016717:Removing Excess F3 logging for packet transfer
18/5/16    sm        CR1017901 Added GL1 idle mode QSH events 
25/01/15   cws       CR964388 Remove crash if switch is in transition when cancelling switch
11/12/15   snjv      CR950902 QSH: Get recent Tx metric on Timer expiry after state change
03/12/15   dp        CR929648 reduce power usage when waiting for TRM during find_bcch
12/01/15   tsk       CR943455 Transition RxD State from RACH to IDLE when main state transitions from RACH to IDLE.
10/28/15   tsk       CR929961 Update rpush_freq_err_in_ppm during BCCH to IDLE state transition.
09/10/15   akh       CR912421 FN/QS not correct for active ttg, sleep ttg resp delayed by 1s
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
27/08/15   jk        CR892120:Reset Data RxD ON/OFF control variables  introduced in CR 864645 during state transitions and during init
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
12/08/15   cws       CR864883 FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
04/08/15   nm        CR882878 Handle unlock cb when changed the reason to ACCESS in transfer mode
19/06/14   dv        CR857895 Ensure that CS ded channels report TX power correctly
29/07/15   nk        CR881383 Update gain tags only if DTM is in running sub state
21/07/15   jk        CR864645:Data RxD ON/OFF changes for GCF TC 14.18.3 & 14.18.1
23/07/15   cjl       CR878351 fix compiler warning
17/07/15   mc        CR871205 : Incorrect CXM Channel ID assigned to W2G activities in W+W mode
14/07/15   ab        CR870268 : GL1 DSDS : Clear GBTA PWR_MON during G2W SUSPEND/RESUME IRAT
29/05/15   jj        CR842746 Don't reset Asdiv algorithm  when DTM to dedicated transition
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
15/06/15   am        CR854340 Adding simulation support through diag
02/06/15   zc        CR845203 Cancel pending switch on GL1 mode change
25/06/15   dg        CR798863 Adjust GSTMR back to its previous value after X2G HO Fail
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
17/06/15   ap        CR852672 Abort PS Access when receiving event unlock immediately
10/06/15   sjw       CR850737: variable connection priority for TRM-Unification
15/04/15   hd        CR822761: Properly check for maximum no freq in hopping list and set NBIN
28/05/15   akh       CR822523 (FR26957) Modem power tx stats
27/05/15   sjw       CR843300: Framework to support MPH_PREEMPTION_IND
07/05/15   mc        CR830988 : COEX Channel ID selection for X2G IRAT measurements does not support TRM_UMTS2
27/04/15   jj        CR 828973 increase clock speed at find bcch mode
28/04/15   mko       CR828316 Enable G to TD tuneaway during Rach process by default
21/04/15   cs/zf     CR808438 Ensure any pending TT requests are sent before cell
                     information is updated. Correction on GPS timetag reporting in wakeup
21/04/15   shr       CR806878 GPRS DSDS Power Control and WWCoex Power Info do not log the Tx Power in use
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX
13/04/15   jj        CR806504 use default clock speed for FIND BCCH MODE
08/04/15   pjr       CR818535 FR25951 Changes to support Single Voice Session
23/02/15   sjv       CR799313: Using new mcfg APIs replacing efs_get for Geran
07/04/15   shr       CR808932: COEX support for DTM
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
30/03/15   ap        CR811753 FR22272 - TRM Unification - GL1 changes to support
                                        SysInfo priority inversion during early camping
30/03/15   nm        CR765891 Allowing TA for TDS during PS RACH
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic
24/03/15   ap        CR811053:FR22272 - TRM Unification  - Access and Traffic attempts
16/03/15   pg        CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP
05/03/15   mko       CR796626 Updating the GNSS_DETERMINISTIC_TIMING_EXIT MACRO so that
                            MCPM MUTEX will be released from DPC tsk before it returns
12/03/15   pa        CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
05/12/14   sp        CR767083: Override paging multiframe to 2, for PDT tests
17/11/14   dg        CR758006 IRAT improvements to ease debugging
23/02/15   am        CR794260 Reset idle update_pending if not moving to idle/transfer state
02/19/15   zc        CR790902: Initialize Asdiv Acq state only when TRM is granted
18/02/15   zc        CR795410: Support asdiv type2 during data
02/19/15   zc        CR790902: Initialize Asdiv Acq state only when TRM is granted
27/02/15   am        CR646584 WLAN coex
02/17/15   zc        CR778174 ASDIV 1x access/traffic should not be able to switch if G registers as RXACQ
25/11/14   zc        CR762713 Make sure to set asdiv mode to rxtx when entering data transfer
05/02/15   cs        CR791768 Support L1_FIND_SCH in l1_change_state
23/01/15   npt       CR697236 Mainline FEATURE_GERAN_VSTMR
21/01/15   pa        CR758792:RxD support for multi-sim.
07/01/15   jj        CR 766904 reset the type 1 asdiv algorithm during transition from dedicated to packet
05/01/15   zc        CR762713 Make sure to set asdiv mode to rxtx when entering data transfer
15/12/14   pg        CR630281 implementation of RxD support for data/MSRD
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
03/12/14   npt       CR759467 Fix compiler warnings
04/11/14   nk        CR750248 Reset WTR Hop state machine at Change state
13/11/14   pa        CR755557:Support for granted band/rf device in Idle mode RxD.
10/11/14   zc        CR748647 ASDIV Type1 Switching Test Mode API
13/11/14   npt       CR746180 Remove KW errors
10/11/14   zc        CR748647 ASDIV Type1 Switching Test Mode API
10/11/14   zc        CR748647 ASDIV Type1 Switching Test Mode API
06/11/14   cs        CR744751 Correctly send firmware valid imsi during RESUME_REQ
27/10/14   am        CR736553 Consider L1_MULTI_SIM_DECOUPLED also in cell_valid_state_check
17/10/14   ws        CR730418 Correct GPS timetagging being out by 1 TDMA frame
17/10/14   cs        CR735918 Assign supported bands for initial ACQ based on UE support
10/10/14   cos       CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes
18/12/13   ka        CR580134 RLF optimizations for CMCC for SGLTE (FR18608)
22/09/14   ap        CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
18/09/14   ws        CR 726622 Fix P1 Klocwork issue with TX AS DIV feature
15/09/14   sp        CR681169 Log radio block data
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
29/08/14   cos       CR 716812 - Adding missing changes for imrd framework
20/06/14   jj        CR656872 fix critical gl1 kw error
04/08/14   ap        CR702584 trm redundant request was made without the non redundant request
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
10/07/14   jk        CR688587:GL1 changes to support LIF feature
24/07/14   zf        CR500399: Don't schedule PCH receive between PCH and extended PCH
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
17/06/14   am        CR680643 Ensure power reporting is active while moving to dedicated state
23/07/14   npt       CR698143: Do not notify arbitrator on Thor
27/06/14   cos       CR686719: Cleaning up AEQPH4 GL1 control info at call exit
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
10/06/14   jj        CR 668965 merge ASDIV from DI.3.0 to BO 2.0
25/06/14   cs        Remove FEATURE_ASYMMETRIC_GERAN_BANDS
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
08/05/14   sp        CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
22/04/14   jj        CR  647387 set inactive mode in trm in dual sim asdiv
23/03/14   sjw       CR637079:new interface for BPLMN failures
17/03/14   jk        CR631867:Enable aeqPhase2a only during EGPRS mode
16/04/14   smd       CR650539:Added sending MCS_CXM_STX_SET_POWER_IND back.
15/04/14   ka        CR611185: Ensure information from the MPH_START_GPRS_IDLE_MODE_REQ is used in RACH
11/04/14   pg        CR645875 Fix Bolt multisim compilation errors
09/04/14   pg        CR643846 RFLM/FED architecture support for multi-SIM
07/04/14   cs        CR629542 Updated CGPS TimeTag to use USTMR latch for Torino
21/01/14   am        CR590285 Limit txlev update as per the Power Class for each band
23/03/14   sjw       CR637079:new interface for BPLMN failures
17/03/14   jk        CR631867:Enable aeqPhase2a only during EGPRS mode
13/03/14   jj        CR  629747 DSDS: register inactive mode  before GL1 goes inactive
19/02/14   ap        CR618768 In SGLTE+G SS Retain TRM lock for TRAFFIC
14/02/14   mc        CR.572523 : COEX : Ensure all DL bands are registered with CXM on transition to FIND_BCCH
06/02/14   smd       CR548408: Remove sending MCS_CXM_STX messages.
03/02/14   cs        CR598751 Use clock enable check outside deterministic loop for TT
21/01/14   am        CR590285 Limit txlev update as per the Power Class for each band
20/01/14   ab        CR582408: DSDA - Update Rx Power in IDLE and Do not Update Power when RX OFF
19/12/13   jk        CR503359 - GL1 changes to support Rx diversity
14/01/14   jj        CR600114 tx- diversity in dime SGLTE
03/12/13   ab        CR582408: DSDA - Update Rx Power in IDLE and Do not Update Power when RX OFF
22/12/13   sp        CR588107: Correct F3 of logging SNRs
20/12/13   js        CR592583 : Removing compilation errors : MPSS.2.0.1 CRM for T+G (W removed)
04/09/13   dg        CR537752 Clear update_pending FLAG inside function l1_change_state
                     if the next state is FIND_BCCH
01/12/13   sk        CR583361 Correting the multi sim sysmode check
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
01/11/13   ab        CR.568623 : COEX : DSDA - update Rx Tx power for COEX with a faster convergence
05/12/13   cs        CR584551 Use correct gas_id for GPS Timetagging in SS mode
28/11/13   cja       CR521061 Correct merge error in CR455527 which brike sub 2 in G+G
22/11/13   cja       CR579775 On Dime use MCVS for settinig clocks for TCH AEq and speed-up
22/11/13   cja       CR455527 Remove featurisation from files in API directory
07/10/13   ab        CR 554820 DSDA GL1 - Update Tx power as per slot transmission for RF COEX
13/11/13   ap        CR562878 FR 17482: Page Blocking Management in Multi-SIM Single Radio Devices
08/11/13   cja       CR507514 Keep RACH power reduction until new Tx Lev scheduled
04/10/13   nm        CR552803 tighten power scan threshold from -107dBm to -102dBm for background power scan only
03/10/13   ws        CR499846 - Bug fix for RACH PWR reduction feature
23/05/13   pg        CR460555: Enable 3GPP Rel10 RACH Power Reduction feature
23/09/13   cja       CR548633 Set mcpm tx on flag, even if Tx already on.
09/09/13   ab        CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
05/09/13   nm        CR538507 Remove Null of handlers when calling gl1_msg_reset
15/08/13   sp        CR529865:  Ensure correct transfer sub is sending the release confirmation, Discard CR515386 changes
18/06/13   npt       CR497521 - Added VSTMR changes
15/08/13   sk        CR524039 GBTA bringup changes
05/08/13   cja       CR497487 New RF IRAT API to add RF ID in G2T.
01/08/13   sk        CR521009 Ensure the global flag rr_send_stop_gsm_mode_req is data spaced
29/07/13   sk        CR519663 Partial QBTA bringup changes
7/16/13    zc        CR470197 Replace memcpy with memscpy
01/07/13   sp        CR508123: Set GL1 MCPM State Correctly, Use of gl1_mcpm_state_type enum
19/06/13   sk        CR502217 Ensure memory allocated for third SUB frequency list
24/06/13   cs        Major Triton TSTS Syncup
16/05/13   ab        CR487396 - Support EGPRS IR Reduced Window in GFW
05/06/13   cs        More TSTS Updates
21/05/13   cs        TSTS Updates
25/04/13   pjr       CR475805 removed gl1_msg_set_multi_sim_mode from l1_change_state
24/04/12   pg        CR456742: re-enable use of MCPM GSM clock mutex
07/03/13   pjr       CR 460494 - reset gl1_msg_multi_sim_mode to SYS_MODEM_DEVICE_MODE_NONE
                     when both gsm clients become inactive
01/03/13   ws        CR 458470 - Fix referencing of tx_lev resolves erratic txlev
                     power in dedicated mode
18/02/13   cja       CR453465 Do not set MCPM AEq setting as part of data start request.
21/12/12   cja       CR434823 Add MCPM request for find_bcch to dedicated state change
09/10/12   cgc       CR393032 add l1_idle_if_gps_notify() set timetag_pending_req
14/12/12   cja       CR426248 Wait for HW Tx Off before MCPM request for voice stop.
11/02/13   ws        CR451284 - Don't overwrite current state with prev_state in
                     l1_change_state() - Stops responding to STOP_MODE_REQ
05/11/12   cja       CR414098 For Dime only need W clks if W neighbours
04/10/12   ky        Use active gas id in compare_signal_strength function
19/06/12   jj        CR367081 Ignoring RR messages once received MPH_STOP_GSM_MODE_REQ
                     until next state change
31/05/12   ab        CR 415136 RX and TX Time Power averaging update using msg router intf.
02/08/12   cgc       CR380234 Increase q size for MAX_PENDING_GSM_TIMETAG_MSGS
10/07/12   pg        CR373325 Check GSM clock before executing GPS timetag
31/05/12   ab        CR 415136 RX and TX Time Power averaging update using msg router intf.
31/05/12   ab        CR 366880 RX and TX Time Power averaging Feature for DSDA as part of SAR Req.
24/04/12   cgc       CR353517 Add gps request flag, Mutex protect and add gl1_drx_task_active
28/03/12   cgc       CR339393 Fix compile error on missing #endif
14/03/12   pg        Add support for QuRT
12/03/12   cgc       CR339393 Rework speedup to allow speedup on DTM->DED whilst on slot 2
05/03/12   ip        CR338937 Add Find BCCH to state change RAND and call MCPM
29/02/12   cja       CR339905 Allow concurrent G2W and channel assignment speed-up
14/02/12   cja       CR334913 Increase SW speed for G2W measurements in dedicated
30/01/12   cgc       CR332648 Increase CPU speed on channel assignment transition
07/12/11   cja       CR323534 When change state to inactive, request MCPM idle
15/11/11   cja       CR318115 Turn off MCPM W nbr during TCH/TRANS if no W
08/11/11   cja       CR318276 For panic reset request MCPM GERAN STOP
09/09/11   cja/ws    CR301686. Use DPC for MCPM update state
24/08/11   kb        CR302696 - Added Initialization function to initialize utils data values
13/07/11   cja       CR 296288 Remove GPS time tagging BU workaround
22/06/11   cja       Remove Nikel BU workarounds
02/06/11   cgc       CR289995 Blast api changed, add blast_has_preempt_trace
24/05/11   cja       Nikel BU remove GPS time tag call
30/03/11   cja       Initial Nikel IRAT
18/03/11   ws        Added Initial MCPM updates
03/12/10   cgc       Featurise previous change with FEATURE_GSM_GPRS_QDSP6
01/12/10   cgc       GPS mutex changes for moving to DPC task
30/11/10   ab        Support EGPRS WS REDUCTION during allocation of dynamic memory in start
                     gsm mode request when FEATURE_GSM_EGPRS_IR_OPT is defined.
14/10/10   pg        Featurise GL1 MUTEX debug and fix compiler errors
06/10/10   ap        Correction to CR: 235190 - 3GPP: Enhanced control of maximum
                      output power in a common BCCH cell
13/08/10   nf        Added MSG logging to mutex logging
14/06/10   ap        CR: 235190 - 3GPP: Enhanced control of maximum output power in
                      a common BCCH cell
11/03/10   pg        Add GL1 MUTEX debug capabilities
21/08/09   cs        CR154851 Fix the RX Sweep for newer targets to properly cleardown
08/09/09   cs        Fix Klocwork bounds checking in sort
03/07/09   sjw       Support Extended BCCH parameter for l1_compute_tc()
26/06/09   sjw       Remove use of RR public store from L1 (for GCF NV flag)
06/06/09   ab        Feature FEATURE_GSM_EGPRS_IR_OPT for LCU
29/05/09   ps        Zero Initialise memory relocation to Heap for selected data structure
31/03/09   cs        CR177251 Fix timetagging to avoid GPS database corruption
30/03/09   ws        Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                     to replace FEATURE_GSM_GPRS_MUSTANG
09/03/09   og        InterRAT reconnect failure - CR171844.
19/11/08   cs        CR162657 The cell info in the GPS timetag can be incorrect
09/10/08   cs        Allow the RX Sweep acquisition to be restarted
28/08/08   cs        Implement the Fast Rx Sweep functionality for FTM
28/07/98   hv        Added KxMutex support
28/04/08   cs        Always qualify RR TA data with valid flag even for NON_SYNC case
28/03/08   cs        Lint fix for DEBUG_PSCAN
20/03/08   agv       Fixed the LCU PMIC Band setup for a Channel Change.
14/02/08   nf        Added feature enabling other Tech L1's to find if GSM is in a TX state
17/01/08   og        Make sure the power scan meas results are sorted correctly
                     when the arfcn list length is one.
14/12/07   ws        merged LCU Bringup and demo changes from 6260 branch
02/10/07   og        Clean up of unused code.
19/07/07   og        UOOS changes, reduce number of measured samples.CR119147.
21/05/07   dv        Update gl1_calc_ho_timing_advance()
13/03/07   cs        Fix the CGPS support to compile without option defined
02/03/07   cs        Add support for new CGPS timetag
03/29/06   gfr       Skip timetag in first frame after waking up.
23/01/06   og        Fix qualifier type mismatch, removes remainder of Qlint
                     high level warnings.
11/04/05   gfr       Added missing #include "gl1_hw.h"
10/02/05   gfr       Fixed sleep timetag featurization
08/22/05   gfr       Changed prototype for gl1_set_band_group
08/22/05   sv        Return correct no of frequencies scanned when L1 frequency
                     list contains only 1 element.
08/19/05   gfr       Added gl1_set_band_group
06/13/05   yh        Implementing qsort.
05/25/05   gw        Suppress some Lint errors.
04/21/05   gfr       Print out all SNRs instead of just avg and min/max.
04/21/05   sv        Fix a MSG_ERR.
04/11/05   gfr       Added support for sleep timetag
03/16/05   pp        Added function L1_map_rxlev_to_dBm_x16()
02/25/05   pp        Added support for GSM to GSM Blind Handovers
                     Fix to not perform SYNCHRONISED and PSEUDO_SYNCHRONISED
                     handovers if BSIC is not known
02/02/05   gfr       Buffer up to 2 timetag notifications at a time
10/01/05   gfr       No need to buffer multiple pending timetag requests
12/23/04   dp        Fix to not cap power level if Anite GCF flag is set
08/26/04   sv        Lint cleanup
08/14/04   dp        Fixed Lint errors
05/26/04   sv        Lint Cleanup
05/14/04   gw        Print error message if IMSI is not valid.
03/26/04   gfr       Time tagging support.
02/25/04   gq        Fixed some lint errors.
01/28/04   gw        Changes to support reselection optimizations.
01/09/04   gw        Added functions to convert SI masks received from (G)RR
                     into TC masks for BCCH and extended BCCH.
11/05/03   gfr       Support for quad-band.
09/22/03   tb        Added function gl1_get_current_ded_chan_type().
08/29/03   gw        Added function l1_snr_msg().
07/11/03   gw        Added checking for divide by zeros.
06/19/03   gw        Added debug code in l1_change_state to help catch cases
                     where SCE is not stopped correctly when changing modes.
05/12/03   ATM       Moved Frame Number management entirely inside GTMRS
05/02/03   gw        Merged in power transfer changes from branch.
04/02/03   gw        Changes to reflect modifications in monscan module.
02/25/03   dp        Changed L1_get_ARFCNS interface for half rate
11/15/02   dp        Added gl1_get_band()
11/12/02   ATM       Enabled vocoder blanking code
11/08/02   ATM       Added central handover routines
10/16/02   dp        Changed L1_get_arfcns to handle negative offsets
10/01/02   ATM       Added central get and set FN routines
10/02/02   dp        Added code in L1_signal_strengths() to filter out
                     adjacent frequencies which are weaker than the carrier
                     by 13dB
09/18/02   dp        Passing in frequency redef parameters to L1_get_ARFCNS
                     to guarantee a seamless transition between the before and
                     after channels in the case of the start time lying in the
                     middle of a block
09/06/02   gr        Added commands for converting GPRS AGC bitfields into
                     dBx16 values.
08/12/02   gw        Changed references to MAXNO_ARFCNS to RR_MAX_PSCAN_FREQ_SCAN.
                     Added function l1_get_agc_ptr().  Removed function
                     L1_condition_rxlev.
07/29/02   ATM       Added "unslaming" logic to l1_change_state( INACTIVE )
06/27/02   gw        Added code to calculate the number of measurements in
                     serving cell running average to L1_calculate_paging_data.
06/10/02   dp        Changed l1_get_ARFCNS interface to pass in the before
                     and after channel parameters.  It now decides if it
                     should use the before or after freq parameters based
                     on if the start time has elapsed or not
05/06/02   JC        Added initialization o temp_agc array so that
                     ISR_SCAN_CNF doesn't trigger an SCH decode.
05/02/02   JC        Additional cleanup for logging.
05/02/02   JC        Additional cleanup to enhance pscan logging results.
05/02/02   JC        Changed >= check to > check in sort algorithm
                     to clean up power scan results and also added
                     initialization to the temp_ structures.
04/30/02   gw        Added function l1_compute_tc.
04/30/02   ATM       Added call to log power scan results.
03/29/02   JC        Added paging group to paging_data_T for logging.
03/20/02   dp        Added support for power control in steps of 2dB.
                     Split up L1_get_sig_strengths into separate functions
                     for tx and rx.
02/15/02   JC        Added L1_map_dBm_x16_to_rxlev().
02/01/02   JC        Cleaned up L1_normalise_signal_strength for AGC
                     support.
01/29/02   thh       The function L1_get_sig_strengths() now correctly returns
                     Tx-lev.  Previously, it was always returning 0.
                     Updated copyright date.
01/24/02   ATM       Added l1_change_state to allow central routine
                     for logging state changes
09/20/01   jc        Added L1_get_ARFCNS, L1_get_sig_strengths
08/31/01   JC        Cleanup of unnecessary compile switches.
                     Also, relocated L1_calculate_ARFCN here.
05/29/01   JC        Initial version.

===========================================================================*/

/*=========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "gl1_msgi.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "gtmrs.h"
#include "gtmrs_g.h"     /* for GSTMR_GET_FN_GERAN( gas_id) & GSTMR_SET_FN() */
#include "l1i.h"
#include "l1_isr.h"
#include "l1_log.h"    /* For logging state and FN */
#include "l1_task.h"
#include "l1_utils.h"
#include "l1_sc.h"
#include "l1_drx.h"
#include "geran_msgs.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_task.h"
#include "gpl1_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"
#include "geran_nv.h"
#include "l1_smscb.h"
#include "gts_api.h"

#include "gl1_hw.h"
#include "gl1_hwi.h"
#include "gl1_hw_g.h"
#include "gl1_hw_sleep_ctl.h"
#include "vtmrs_g.h"
#include <stringl/stringl.h>


#include "rex.h"

/* PMIC signaling needed for Solo.
**
*/

#include "geran_eng_mode_info.h"
#include <stdlib.h>   /* For qsort */


#include "mcpm_api.h"
#include "gl1_hw_clk_ctl.h"
#include "gprs_mem.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#include "gl1_arbitrator_tables.h"
#include "gpl1_gprs_isr.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_GSM_WTR_HOP
#include "gl1_msg_wtr_hop.h"
#endif
#ifdef FEATURE_WLAN_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#endif

#include "fs_public.h"
#include "fs_lib.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#include "gpl1_gprs_utils.h"
#include "l1_ds.h"

#if defined (FEATURE_QSH_EVENT_METRIC )|| defined (FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
#include "l1_log_qsh.h"
#endif /*FEATURE_QSH_EVENT_METRIC || FEATURE_QSH_EVENT_NOTIFY_TO_QSH */

#ifdef FEATURE_GL1_ASDIV_ACQ
#include "gl1_asdiv.h"
#endif

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */
#ifdef GL1_GMSK_MOD_DET
extern uint16 gl1_hw_is_gdet_threshold_enabled(void);
extern boolean gl1_hw_is_gdet_debug_enabled(void);
#endif
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/*
 ******************************************************************************
 * Function: Critical section Entry and Exit for time tagging
 *
 * Description: Macro for critical time tagging for GPS
 *
 * macro provided by GPS team and re-worked for GERAN use.
 ******************************************************************************
*/
#define CSC_RETRY_COUNT_MAX       10

#ifdef FEATURE_DATA_PDU_DEBUG
const uint8 cs_to_length[] =
{
   GL1_DEFS_CS1_BYTES,
   GL1_DEFS_CS2_BYTES,
   GL1_DEFS_CS3_BYTES,
   GL1_DEFS_CS4_BYTES,
   GL1_DEFS_ACCESS_BYTES,
   GL1_DEFS_EXT_ACCESS_BYTES,
   GL1_DEFS_MCS1_BYTES,
   GL1_DEFS_MCS2_BYTES,
   GL1_DEFS_MCS3_BYTES,
   GL1_DEFS_MCS4_BYTES,
   GL1_DEFS_MCS5_BYTES,
   GL1_DEFS_MCS6_BYTES,
   GL1_DEFS_MCS7_BYTES,
   GL1_DEFS_MCS8_BYTES,
   GL1_DEFS_MCS9_BYTES
};
#endif /* FEATURE_DATA_PDU_DEBUG */

/*Flag indicating the state change*/
boolean gl1_flag_state_change[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

/*
** CgC place a do while loop around the tagging functions
**     raise priority to prevent ints on this thread
*/
#define GNSS_DETERMINISTIC_TIMING_ENTER                   \
 {                                                        \
   uint32 q_retry_count = 0;                              \
   uint32 q_csc_old;                                      \
   uint32 q_csc;                                          \
   boolean b_interrupted = TRUE;                          \
   REX_DISABLE_PREMPTION();                               \
   q_csc_old = qurt_trace_get_marker(); /* fetch CSC */  \
   do                                                     \
   {

#define GNSS_DETERMINISTIC_TIMING_EXIT              \
   q_csc = qurt_trace_get_marker();                \
   /* if we were not interrupted */                 \
   if( q_csc_old == q_csc )                         \
   {   b_interrupted = FALSE; }                     \
   q_csc_old = q_csc;          /* shift value */    \
   /* make sure context not switched out, redo operations if interrupted */     \
   } while( b_interrupted && (++q_retry_count<CSC_RETRY_COUNT_MAX));            \
   REX_ENABLE_PREMPTION();                          \
   if( q_retry_count >= CSC_RETRY_COUNT_MAX )       \
   { MSG_GERAN_ERROR_0_G( "QURT CSC max retries"); }   \
   MSG_GERAN_MED_1_G( "Retry Count = %d", q_retry_count); \
   if ( b_interrupted )                             \
   {                                                \
      sclk_valid =FALSE;                                 \
   }                                                \
 }

/* -111 dBm and below maps to RXLEV=0 */
#define dBm_x16_VALUE_FOR_RXLEV_0  ((dBx16_T)(-111*16))

#define dBm_x16_VALUE_FOR_RXLEV_63 ((dBx16_T)(-48*16))
#define MAX_RF_POWER_INDEX 15

#define RXLEV_INVALID  0xFF
#define RXQUAL_INVALID 0xFF
#define SNR_INVALID    0xFF
gpl1_serving_cell_meas_t gpl1_serving_cell_meas[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE) };
extern boolean meas_report_valid[NUM_GERAN_DATA_SPACES];

/* Variable to track GTS timer counter */
boolean gl1_gts_timer_running[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE) };
static rex_timer_type   gl1_gts_timer_counter[NUM_GERAN_DATA_SPACES];
#ifdef GL1_GTS_DEBUG
static uint64 gl1_gts_timer_started[NUM_GERAN_DATA_SPACES] =   { INITIAL_VALUE( 0 ) };
static uint64 gl1_gts_timer_stopped[NUM_GERAN_DATA_SPACES]  =  { INITIAL_VALUE( 0 ) };
#endif

/* Functions to send Timing update to GPS module */
static void gl1_gts_timer_start(uint32 gts_timer_timeout, gas_id_t gas_id );
static void gl1_gts_time_update_cb( gas_id_t gas_id );
static void gl1_gts_send_timing_info( gas_id_t gas_id );
static boolean gl1_gts_chk_state_to_send(gas_id_t gas_id);
#ifdef GL1_GTS_DEBUG
static void gl1_gts_debug_enable(gts_GsmTimeInfoType *gts_msg, gas_id_t gas_id);
#endif
static gts_gl1_state_type gl1_gts_set_l1_state(gas_id_t gas_id);

/* Column2 is in Q16  
   1st Column below is the Linear value for 10*Log(Column2*10)
   This table cannot be used to estimate -ve dBm values 
*/
static const int32  simple_linear_log_base_10_conv_table_q16[][2] =
{
  /*   Linear 32U16           Log  32S16         Log Decimal */
  { (int32) 0x1F074848,  (int32) 0x0003E666 },    /* 3.9000 */
  { (int32) 0x1BA77535,  (int32) 0x0003D99A },    /* 3.8500 */
  { (int32) 0x18A592CD,  (int32) 0x0003CCCD },    /* 3.8000 */
  { (int32) 0x15F769CB,  (int32) 0x0003C000 },    /* 3.7500 */
  { (int32) 0x1393DF51,  (int32) 0x0003B333 },    /* 3.7000 */
  { (int32) 0x1172D5FF,  (int32) 0x0003A666 },    /* 3.6500 */
  { (int32) 0x0F8D125B,  (int32) 0x0003999A },    /* 3.6000 */
  { (int32) 0x0DDC2246,  (int32) 0x00038CCD },    /* 3.5500 */
  { (int32) 0x0C5A4715,  (int32) 0x00038000 },    /* 3.5000 */
  { (int32) 0x0B026208,  (int32) 0x00037333 },    /* 3.4500 */
  { (int32) 0x09CFE2ED,  (int32) 0x00036666 },    /* 3.4000 */
  { (int32) 0x08BEB89C,  (int32) 0x0003599A },    /* 3.3500 */
  { (int32) 0x07CB4327,  (int32) 0x00034CCD },    /* 3.3000 */
  { (int32) 0x06F24787,  (int32) 0x00034000 },    /* 3.2500 */
  { (int32) 0x0630E4A8,  (int32) 0x00033333 },    /* 3.2000 */
  { (int32) 0x0584899D,  (int32) 0x00032666 },    /* 3.1500 */
  { (int32) 0x04EAECE8,  (int32) 0x0003199A },    /* 3.1000 */
  { (int32) 0x046204B9,  (int32) 0x00030CCD },    /* 3.0500 */
  { (int32) 0x03E80000,  (int32) 0x00030000 },    /* 3.0000 */
  { (int32) 0x037B403D,  (int32) 0x0002F333 },    /* 2.9500 */
  { (int32) 0x031A5407,  (int32) 0x0002E666 },    /* 2.9000 */
  { (int32) 0x02C3F21F,  (int32) 0x0002D99A },    /* 2.8500 */
  { (int32) 0x0276F515,  (int32) 0x0002CCCD },    /* 2.8000 */
  { (int32) 0x02325761,  (int32) 0x0002C000 },    /* 2.7500 */
  { (int32) 0x01F52FEF,  (int32) 0x0002B333 },    /* 2.7000 */
  { (int32) 0x01BEAF00,  (int32) 0x0002A666 },    /* 2.6500 */
  { (int32) 0x018E1B70,  (int32) 0x0002999A },    /* 2.6000 */
  { (int32) 0x0162D03A,  (int32) 0x00028CCD },    /* 2.5500 */
  { (int32) 0x013C3A4F,  (int32) 0x00028000 },    /* 2.5000 */
  { (int32) 0x0119D69A,  (int32) 0x00027333 },    /* 2.4500 */
  { (int32) 0x00FB304B,  (int32) 0x00026666 },    /* 2.4000 */
  { (int32) 0x00DFDF43,  (int32) 0x0002599A },    /* 2.3500 */
  { (int32) 0x00C786B7,  (int32) 0x00024CCD },    /* 2.3000 */
  { (int32) 0x00B1D3F4,  (int32) 0x00024000 },    /* 2.2500 */
  { (int32) 0x009E7D44,  (int32) 0x00023333 },    /* 2.2000 */
  { (int32) 0x008D40F6,  (int32) 0x00022666 },    /* 2.1500 */
  { (int32) 0x007DE47E,  (int32) 0x0002199A },    /* 2.1000 */
  { (int32) 0x007033AC,  (int32) 0x00020CCD },    /* 2.0500 */
  { (int32) 0x00640000,  (int32) 0x00020000 },    /* 2.0000 */
  { (int32) 0x00592006,  (int32) 0x0001F333 },    /* 1.9500 */
  { (int32) 0x004F6ECE,  (int32) 0x0001E666 },    /* 1.9000 */
  { (int32) 0x0046CB69,  (int32) 0x0001D99A },    /* 1.8500 */
  { (int32) 0x003F1882,  (int32) 0x0001CCCD },    /* 1.8000 */
  { (int32) 0x00383BF0,  (int32) 0x0001C000 },    /* 1.7500 */
  { (int32) 0x00321E65,  (int32) 0x0001B333 },    /* 1.7000 */
  { (int32) 0x002CAB1A,  (int32) 0x0001A666 },    /* 1.6500 */
  { (int32) 0x0027CF8B,  (int32) 0x0001999A },    /* 1.6000 */
  { (int32) 0x00237B39,  (int32) 0x00018CCD },    /* 1.5500 */
  { (int32) 0x001F9F6E,  (int32) 0x00018000 },    /* 1.5000 */
  { (int32) 0x001C2F0F,  (int32) 0x00017333 },    /* 1.4500 */
  { (int32) 0x00191E6E,  (int32) 0x00016666 },    /* 1.4000 */
  { (int32) 0x00166320,  (int32) 0x0001599A },    /* 1.3500 */
  { (int32) 0x0013F3DF,  (int32) 0x00014CCD },    /* 1.3000 */
  { (int32) 0x0011C865,  (int32) 0x00014000 },    /* 1.2500 */
  { (int32) 0x000FD954,  (int32) 0x00013333 },    /* 1.2000 */
  { (int32) 0x000E2019,  (int32) 0x00012666 },    /* 1.1500 */
  { (int32) 0x000C96D9,  (int32) 0x0001199A },    /* 1.1000 */
  { (int32) 0x000B385E,  (int32) 0x00010CCD },    /* 1.0500 */
  { (int32) 0x000A0000,  (int32) 0x00010000 },    /* 1.0000 */
  { (int32) 0x0008E99A,  (int32) 0x0000F333 },    /* 0.9500 */
  { (int32) 0x0007F17B,  (int32) 0x0000E666 },    /* 0.9000 */
  { (int32) 0x00071457,  (int32) 0x0000D99A },    /* 0.8500 */
  { (int32) 0x00064F40,  (int32) 0x0000CCCD },    /* 0.8000 */
  { (int32) 0x00059F98,  (int32) 0x0000C000 },    /* 0.7500 */
  { (int32) 0x0005030A,  (int32) 0x0000B333 },    /* 0.7000 */
  { (int32) 0x00047783,  (int32) 0x0000A666 },    /* 0.6500 */
  { (int32) 0x0003FB28,  (int32) 0x0000999A },    /* 0.6000 */
  { (int32) 0x00038C53,  (int32) 0x00008CCD },    /* 0.5500 */
  { (int32) 0x0003298B,  (int32) 0x00008000 },    /* 0.5000 */
  { (int32) 0x0002D182,  (int32) 0x00007333 },    /* 0.4500 */
  { (int32) 0x0002830B,  (int32) 0x00006666 },    /* 0.4000 */
  { (int32) 0x00023D1D,  (int32) 0x0000599A },    /* 0.3500 */
  { (int32) 0x0001FECA,  (int32) 0x00004CCD },    /* 0.3000 */
  { (int32) 0x0001C73D,  (int32) 0x00004000 },    /* 0.2500 */
  { (int32) 0x000195BC,  (int32) 0x00003333 },    /* 0.2000 */
  { (int32) 0x0001699C,  (int32) 0x00002666 },    /* 0.1500 */
  { (int32) 0x00014249,  (int32) 0x0000199A },    /* 0.1000 */
  { (int32) 0x00011F3D,  (int32) 0x00000CCD },    /* 0.0500 */
  { (int32) 0x00010000,  (int32) 0x00000000 },    /* 0.0000 */
};

static const uint32 simple_linear_log_base_10_conv_table_q16_size =
  sizeof(simple_linear_log_base_10_conv_table_q16) / (2 * sizeof(simple_linear_log_base_10_conv_table_q16[0][0]));

static int32  gl1_linear_log_conversion_table_binary_search
(
  /*! Input value */
  int32  const  value
);

/* Array for determining the block of the RAC in BCCH_SDCCH_COMB */

const uint8   rach_comb[7] = { 0x18,0xe0,0xff,0xff,0x0f,0x30,0x00};

const uint8   rev_byte[16] = { 0,8,4,0xc,2,0xA,6,0xE,1,9,5,0xD,3,0xB,7,0xF };

extern dedicated_data_T l1_dedicated_data[];

/* NBIN table for working out how many bits to represent N */
const byte NBIN [65] = {
   1, 2, 4, 4, 8, 8, 8, 8, 16, 16, 16, 16, 16, 16, 16, 16,
   32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
   64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 128};


 /* RNTABLE for controlling the hopping sequence. taken from
    GSM 05.02 section 6.2.3  */
const  byte  RNTABLE [ 114 ] = {
   48,  98,  63,   1,  36,  95,  78, 102,  94,  73,
   0,   64,  25,  81,  76,  59, 124,  23, 104, 100,
   101, 47, 118,  85,  18,  56,  96,  86,  54,   2,
   80,  34, 127,  13,   6,  89,  57, 103,  12,  74,
   55, 111,  75,  38, 109,  71, 112,  29,  11,  88,
   87,  19,   3,  68, 110,  26,  33,  31,   8,  45,
   82,  58,  40, 107,  32,   5, 106,  92,  62,  67,
   77, 108, 122,  37,  60,  66, 121,  42,  51, 126,
   117,114,   4,  90,  43,  52,  53, 113, 120,  72,
   16,  49,   7,  79, 119,  61,  22,  84,   9,  97,
   91,  15,  21,  24,  46,  39,  93, 105,  65,  70,
   125, 99,  17, 123
};

/* The drivers layer reports Tx power into the arbitrator using RF Power index. This table maps RF Power Index to dBm */
const uint16 power_index_to_dBm[SYS_BAND_CELL_850+1][MAX_RF_POWER_INDEX+1] = {
/*    RF Power Index        0  1  2   3   4   5   6   7   8   9  10  11  12  13  14  15 */
   /* SYS_BAND_EGSM_900 */ {5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35},
   /* SYS_BAND_PGSM_900 */ {5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35},
   /* SYS_BAND_PCS_1900 */ {0, 2, 4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30},
   /* SYS_BAND_PCS_1800 */ {0, 2, 4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30},
   /* SYS_BAND_CELL_850 */ {5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35}
};

#ifdef FEATURE_GTA_DURING_PS_RACH

#define NO_RACH_FRAMES_51_MF_CCCH_COMB 27

const uint8 block_start_fn[21] = {0/*F*/, 1/*S*/, 2, 6, 10, 11, 12,
                                   16, 20, 21, 22, 26, 30,
                                   31, 32, 36, 40, 41, 42,
                                   46, 50/*Idle*/};

const uint8 rach_FN_ccch_comb[NO_RACH_FRAMES_51_MF_CCCH_COMB] = {4, 5,  /*(4, 5)*/
                                                                 14, 15, 16, 17, 18, 19, 20,/*(14, 36)*/
                                                                 21, 22, 23, 24, 25, 26, 27, 28, 29,
                                                                 30, 31, 32, 33, 34, 35, 36,
                                                                 45, 46}; /*45, 46)*/

#endif /*FEATURE_GTA_DURING_PS_RACH*/

/* RF power class per band */
extern byte   gl1_rf_power_class_per_band[NUM_GERAN_DATA_SPACES][5];
extern uint8  gl1_max_pcl_pwr_class[5][5][2];

extern boolean L1_is_FN_after_start_time           ( uint32 fn, gas_id_t gas_id );
extern boolean L1_is_FN_after_freq_redef_start_time( uint32 fn, gas_id_t gas_id );
extern void L1_sync_to_current ( l1_sc_bs_T *current_cell, gas_id_t gas_id );
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
extern void gl1_disable_ccch_in_transfer(gas_id_t gas_id);
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

/* Timetag message saved for processing in gl1_timetag_gps_tick, we need
   to buffer at least two since it is possible to get an early camping
   and stop camping request in the same frame from RR. */
#define MAX_PENDING_GSM_TIMETAG_MSGS (4)

/* Where to derive the current GSM time from responding */
typedef enum
{
   TIMETAG_SOURCE_GSTMR = 1,
   TIMETAG_SOURCE_SLEEPCLK
} gl1_timetag_source_type;

static boolean gl1_timetag_cgps_tag_and_send
(
   cgps_GsmCellTimingStructType *msg,
   gl1_timetag_source_type source,
   gas_id_t gas_id
);

/*
** CgC DTM speed state variables
*/
static boolean gl1_data_is_on[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static uint16  gl1_data_state[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };

typedef struct
{
  /* The L1 version of the FTM results cb */
  boolean init_once;
  boolean in_cell;

  cgps_GsmCellTimingStructType timetag_msg[MAX_PENDING_GSM_TIMETAG_MSGS];
  boolean timetag_pending_req;
  int timetag_pending_msg;
  uint32 bplmn_fail_count;
}l1_utils_data_t;

static l1_utils_data_t  l1_utils_data[NUM_GERAN_DATA_SPACES];

Frequency_List_T *l1_Frequency_List_Store[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( NULL ) };
#ifdef GL1_GMSK_MOD_DET
uint16 *decStat[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( NULL ) };
#endif
#ifdef  FEATURE_GSM_GPRS_TIME_PWR_AVG

/* In Q8 format - filter co-eff */
#define ALPHA_FILCOEFF_PWR_AVG ( 0.93 * ( 1 << 8 ) )
#define ALPHA_FILCOEFF_PWR_AVG_COEX ( 0.7 * ( 1 << 8 ) )

/* We are doing time based averaging and not number of samples.
  Thus over 21 frames with 8 slots = Divisor Average = 21x8=96ms
*/
#define TIME_AVG_8_FRAME (8 * 8)

// conversions from pcl to dbm to linear = 10 exp(dbm/10)
static const int16 convert_dbm_to_linear_900[] =
  {7943,7943,7943,5012,3162,1995,1259,794,501,316,200,
   126,79,50,32,20,13,8,5,3,3,3,3,3,3,3,3,3,3,3,3,3};

static const int16 convert_dbm_to_linear_1800[] =
  {1000,631,398,251,158,100,63,40,25,16,10,6,4,3,2,
   1,1,1,1,1,1,1,1,1,1,1,1,1,1,3981,2512,1585};

static const int16 convert_dbm_to_linear_1900[] =
  {1000,631,398,251,158,100,63,40,25,16,10,6,4,3,2,
   1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1995,1585};

extern dBx16_T gl1_msg_algs_linear_to_dBx16( uint32 x );
#ifdef GL1_GMSK_MOD_DET
extern uint16 gl1_hw_is_gdet_enabled(void);
#endif

/* TX annd RX power average globals */
gpl1_tx_power_t gpl1_tx_power[NUM_GERAN_DATA_SPACES];
gpl1_rx_power_t gpl1_rx_power[NUM_GERAN_DATA_SPACES];

#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_WLAN_COEX_SW_CXM
gpl1_snr_t gpl1_snr[NUM_GERAN_DATA_SPACES];
#endif

#define GL1_GPRS_GBTA_SUPPORT "/nv/item_files/modem/geran/gbta_support"
uint32 gl1_gprs_gbta_support = 1;

#define RACH_PWR_RED_LB_P5_MINUS_10DB           10
#define RACH_PWR_RED_LB_P5_MINUS_10DB_PWR_IDX   9
#define RACH_PWR_RED_HB_P2_MINUS_10DB           4
#define RACH_PWR_RED_HB_P2_MINUS_10DB_PWR_IDX   11

#define GL1_RACH_PWR_RED_MIN_RX_LEV     -720  // -45dBm * 16

typedef struct
{
   boolean active;
   int16   pwr_idx;
   boolean first_rach;
} gl1_rach_pwr_reduction_type;

#define _GL1_RACH_PWR_REDUCTION {FALSE, RACH_PWR_RED_HB_P2_MINUS_10DB_PWR_IDX, FALSE}
static gl1_rach_pwr_reduction_type gl1_rach_pwr_reduction[NUM_GERAN_DATA_SPACES] =
  { INITIAL_VALUE(_GL1_RACH_PWR_REDUCTION) };
#undef __GL1_RACH_PWR_REDUCTION


typedef enum
{
   PWR_STAT_BAND_IDX_900,
   PWR_STAT_BAND_IDX_1800,
   PWR_STAT_BAND_IDX_1900,
   PWR_STAT_BAND_IDX_MAX
} pwr_stat_band_idx_t;

/* linear_pwr = 10^(dbm/10)
 * dbm = 10 * log10(linear_pwr)
 *
 * dbm (linear_pwr)
 */

static const ds3g_tx_power_bin_e_type cvt_pcl_to_pwr_stat_idx[PWR_STAT_BAND_IDX_MAX][32] =
{
   /* 900 Band */
   {
      /* 39 (7943)      , 39 (7943)         , 39 (7943)         , 37 (5012)         , */
      DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5,

      /* 35 (3162)      , 33 (1995)         , 31 (1259)         , 29 (794)          , */
      DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5,

      /* 27 (501)       , 25 (316)          , 23 (200)          , 21 (126)          , */
      DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5,

      /* 19 (79)        , 17 (50)           , 15 (32)           , 13 (20)           , */
      DS3G_TX_POWER_BIN_4, DS3G_TX_POWER_BIN_4, DS3G_TX_POWER_BIN_4, DS3G_TX_POWER_BIN_3,

      /* 11 (13)        , 9  (8)            , 7  (5)            , 5  (3)            , */
      DS3G_TX_POWER_BIN_3, DS3G_TX_POWER_BIN_3, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2,

      /* 5  (3)         , 5  (3)            , 5  (3)            , 5  (3)           , */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2,

      /* 5  (3)         , 5  (3)            , 5  (3)            , 5  (3)            , */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2,

      /* 5  (3)         , 5  (3)            , 5  (3)            , 5  (3)              */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2
   },

   /* 1800 Band */
   {
      /* 30 (1000)      , 28 (631)          , 26 (398)          , 24 (251)          , */
      DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5,

      /* 22 (158)       , 20 (100)          , 18 (63)           , 16 (40)           , */
      DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_4, DS3G_TX_POWER_BIN_4,

      /* 14 (25)        , 12 (16)           , 10 (10)           , 8   (6)           , */
      DS3G_TX_POWER_BIN_3, DS3G_TX_POWER_BIN_3, DS3G_TX_POWER_BIN_3, DS3G_TX_POWER_BIN_3,

      /* 6  (4)         , 4  (3)            , 2  (2)            , 0   (1)           , */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2,

      /* 0  (1)         , 0  (1)            , 0  (1)            , 0   (1)           , */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2,

      /* 0  (1)         , 0  (1)            , 0  (1)            , 0   (1)           , */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2,

      /* 0  (1)         , 0  (1)            , 0  (1)            , 0   (1)           , */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2,

      /* 0  (1)         , 36 (3981)         , 34 (2512)         , 32  (1585)          */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5
   },

   {
      /* 1900 Band */
      /* 30 (1000)      , 28 (631)          , 26 (398)          , 24 (251)          , */
      DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5,

      /* 22 (158)       , 20 (100)          , 18 (63)           , 16 (40)           , */
      DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_4, DS3G_TX_POWER_BIN_4,

      /* 14 (25)        , 12 (16)           , 10 (10)           , 8  (6)            , */
      DS3G_TX_POWER_BIN_3, DS3G_TX_POWER_BIN_3, DS3G_TX_POWER_BIN_3, DS3G_TX_POWER_BIN_3,

      /* 6  (4)         , 4  (3)            , 2  (2)            , 0  (1)            , */
      DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2, DS3G_TX_POWER_BIN_2,

      /* R  (0)         , R  (0)            , R  (0)            , R  (0)            , */
      DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1,

      /* R  (0)         , R  (0)            , R  (0)            , R  (0)            , */
      DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1,

      /* R  (0)         , R  (0)            , R  (0)            , R  (0)            , */
      DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1,

      /* R  (0)         , R  (0)            , 33 (1995)         , 32 (1585)           */
      DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_1, DS3G_TX_POWER_BIN_5, DS3G_TX_POWER_BIN_5
   }
};

/* indicates whether the last command given to MCPM was to turn on MCVS 
   settings for bcch_list or to turn them off */
static boolean l1_utils_find_bcch_mcvs_on[NUM_GERAN_DATA_SPACES] = {FALSE};


static void gpl1_reset_modem_power_stats_tx(gas_id_t gas_id);
static boolean l1_state_change_bcch_to_idle(gas_id_t gas_id);

/*===========================================================================

FUNCTION init_l1_utils_data

DESCRIPTION
             Initializes utils data values.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/

void init_l1_utils_data( gas_id_t gas_id )
{
  memset ( &l1_utils_data[gas_id], 0 , sizeof ( l1_utils_data_t ) );
  l1_utils_data[gas_id].init_once = TRUE;
  l1_utils_data[gas_id].in_cell = FALSE;
}


/*===========================================================================

                           LOCAL FUNCTIONS

===========================================================================*/


/*===========================================================================

FUNCTION  gl1_get_FN

DESCRIPTION
  This function retrieves the L1 Frame Number.

RETURN VALUE
  dword - the L1 Frame Number

SIDE EFFECTS
  None.

===========================================================================*/
uint32 gl1_get_FN ( gas_id_t gas_id )
{
  return GSTMR_GET_FN_GERAN( gas_id);
}

/*===========================================================================
FUNCTION  l1_state_change_bcch_to_idle

DESCRIPTION
  This function returns TRUE if current L1 state is L1_IDLE_MODE and previous
  state is L1_FIND_BCCH with the state change flag being true.

RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None
===========================================================================*/
static boolean l1_state_change_bcch_to_idle(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean ret_val = FALSE;
  byte prev_state = l1_tskisr_blk->l1_prev_state;
  byte curr_state = l1_tskisr_blk->l1_state;

  if ((gl1_flag_state_change[gas_id] == TRUE) &&
        (((prev_state == L1_FIND_BCCH) &&
          ((curr_state == L1_IDLE_MODE) || (curr_state == L1_GPRS_IDLE_MODE)))
        #if defined (FEATURE_DUAL_SIM) || defined (FEATURE_TRIPLE_SIM)
        || ((prev_state == L1_MULTI_SIM_FIND_BCCH) && (curr_state == L1_MULTI_SIM_IDLE))
        #endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
     ))
  {
    ret_val = TRUE;
  }
  return ret_val;
}

/*===========================================================================

FUNCTION  l1_change_state

DESCRIPTION
  This function should be called when L1 changes state.
  Only log this if state is different than before.

RETURN VALUE
  None

SIDE EFFECTS
  L1 State will be logged
  Timing will be unslammed if slammed and transitioning to INACTIVE

===========================================================================*/
void l1_change_state ( const byte newState, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */
#ifdef FEATURE_GSM_COEX_SW_CXM
  ARFCN_T       arfcn = { INVALID_ARFCN, SYS_BAND_NONE };
#endif /* FEATURE_GSM_COEX_SW_CXM */

  byte StateNow;

  StateNow = l1_tskisr_blk->l1_state;

#ifdef FEATURE_GSM_QSC_TX_DIV
  if(gl1_trm_asdiv_debug_get_test_mode(gas_id))
  {
    gl1_trm_asdiv_debug_test_mode_init_sm(gas_id);
  }
#endif
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
/*Reset recovery data during transition to other state except transfer*/ 
   if (L1_TRANSFER_MODE != newState )
   {
     gpl1_gprs_recovery_reset(gas_id);
   }
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */

  if (L1_INACTIVE_MODE == newState )
  {
    rr_send_stop_gsm_mode_req[gas_id] = FALSE;
  }

  /*If the next state is not idle and transfer mode then clear the update_pending flag to avoid updating the ccch/paging
    group with the data of old cell*/
  if ( 
            ( L1_TRANSFER_MODE != newState )
         && ( L1_IDLE_MODE != newState )
 #if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
         && ( L1_MULTI_SIM_IDLE != newState ) 
#endif
     )
  {
    gprs_l1_idle_data->update_pending = FALSE;
  }



  if( newState != StateNow )
  {
#ifdef FEATURE_GERAN_TIMELINE_ERROR_RECOVERY     
	gl1_hw_reset_burst_recovery_counters(gas_id);
	gl1_hw_reset_burst_success_counters(gas_id);
#endif
    if ((StateNow == L1_TRANSFER_MODE) && (newState == L1_MULTI_SIM_IDLE))
    {
       l1_tskisr_blk->transfer_to_idle_fn = gl1_get_FN( gas_id );
    }

    if (newState != L1_MULTI_SIM_IDLE)
    {
       l1_tskisr_blk->transfer_to_idle_fn = GL1_DEFS_INVALID_FN;
    }
      
    /* Debug code to check SCE aborts */
    if (l1_sci_is_op_in_progress(gas_id))
    {
      MSG_GERAN_ERROR_2_G("Change state %d->%d with SCE active", l1_tskisr_blk->l1_state, newState);
    }

    MSG_GERAN_HIGH_3_G( "Change state %d->%d client id %d", StateNow, newState, l1_tskisr_blk->client_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
	gl1_qsh_notify_state_transition(gas_id,newState);
#endif

  /*Cancel any pending switches when state changes*/
#ifdef FEATURE_GSM_QSC_TX_DIV
    if ( read_pending_asdiv_switch_request (gas_id))
    {
       trm_switch_complete_input_type local_input_type = gl1_switch_complete_input[gas_id];
      if ( local_input_type.client <= TRM_LAST_CLIENT )

      
      {
        MSG_GERAN_HIGH_0_G("Cancel pending Switch on State Change");
        trm_ant_switch_div_switch_cancel (local_input_type );
        clear_pending_asdiv_switch_request(gas_id);
        if(trm_ant_switch_div_get_config(TRM_ANT_SWITCH_CONFIG_CURRENT) == TRM_ANT_SWITCH_DIV_CONFIG_IN_TRANSITION)
          {
            MSG_GERAN_ERROR_0_G("SWITCH Cancellation Failed");
          }
      }
      else
      {
        MSG_GERAN_ERROR_1_G("Invalaid asdiv client %d",gl1_switch_complete_input[gas_id].client);
      }
    }
#endif
    if( StateNow ==  L1_FIND_BCCH
#if defined  ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               ||( StateNow ==L1_MULTI_SIM_FIND_BCCH)
#endif /*FEATURE_DUAL_SIM*/
      )
     {
             if((newState != L1_FIND_BCCH)
#if defined  ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               ||( newState !=L1_MULTI_SIM_FIND_BCCH)
#endif /*FEATURE_DUAL_SIM*/
             )
             {
               l1_find_bcch_mcpm_change( FALSE, gas_id );
             }
     }
#ifdef FEATURE_GL1_ASDIV_ACQ
        /*For AsDiv Acq State Reset*/
        switch (newState) 
        {
          case L1_FIND_BCCH:
          case L1_NULL_MODE:
          case L1_MULTI_SIM_FIND_BCCH:
          case L1_MULTI_SIM_NULL:
            /* Reset the global var */
            gl1_asdiv_reset_antenna_to_use();
            break;
         default:
            /*If Any other mode except NULL or Acquisition, then clear AsDiv Acq Data*/
            gl1_asdiv_acq_reset(gas_id);
            break;
        }
#endif /*FEATURE_GL1_ASDIV_ACQ*/

    switch (newState)
    {
#ifdef FEATURE_GSM_DTM
    case L1_DTM_MODE:
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
      gl1_hw_reset_typ1_asdiv_switch_algorithm(gas_id);
#endif /*FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/

      switch (StateNow)
      {
      case L1_DEDICATED_MODE:
        MSG_GERAN_HIGH_2_G( "MCPM change required; DED to DTM state=0x%X isOn=%d", gl1_data_state[gas_id], gl1_data_is_on[gas_id]);
                  gl1_clkdata_speed(TRUE, L1_DED_DATA_DTM, L1_DED_DATA_NULL,L1_DTM,gas_id);
#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
        if (!gl1_hw_feature_disabled_aeq_8psk(gas_id) )
        {
          gl1_hw_mcpm_gprs_aeq_update(TRUE,gas_id);
        }
#endif
#ifdef FEATURE_AEQ_PHASE4
        gl1_reset_aeq4_ctl(gas_id);
#endif /*FEATURE_AEQ_PHASE4 */

#ifdef FEATURE_GSM_RX_DIVERSITY
                  gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_DEDICATED_STATE,
                                                      GL1_RXDIV_L1_DTM_STATE,
                                                      gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
        break;

      case L1_TRANSFER_MODE:
                  gl1_clkdata_speed(TRUE, L1_DED_DATA_DTM, L1_DED_DATA_TRANS,L1_DTM,gas_id);  /* state change only */
        MSG_GERAN_HIGH_2_G( "MCPM change required; TRANS to DTM state=0x%X isOn=%d", gl1_data_state[gas_id], gl1_data_is_on[gas_id]);
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_START_REQ, GL1_MCPM_GERAN_DTM,gas_id);

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
        gl1_disable_ccch_in_transfer(gas_id);
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_TRANSFER_STATE,
          GL1_RXDIV_L1_DTM_STATE,
          gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

        break;

      default:
        break;
      }
      break;
#endif
    case L1_TRANSFER_MODE:
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
             if (StateNow !=L1_DTM_MODE)
             {
                  gl1_hw_reset_typ1_asdiv_switch_algorithm(gas_id);
       }
#endif /*FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/
#ifdef FEATURE_GSM_QSC_TX_DIV
      gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXTX ,gas_id);
      gl1_trm_asdiv_reset_traffic_state_vars(gas_id);
#endif /* FEATURE_GSM_QSC_TX_DIV */
      switch (StateNow)
      {
      case L1_RANDOM_ACCESS_MODE:
      case L1_PACKET_ACCESS_MODE:
        MSG_GERAN_HIGH_0_G( "MCPM change required; RAND to TRANS");
                  gl1_clkdata_speed(TRUE, L1_DED_DATA_TRANS,L1_DED_DATA_NULL,L1_DATA,gas_id);
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_STOP_REQ, GL1_MCPM_GERAN_DATA,gas_id);
#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
        if (!gl1_hw_feature_disabled_aeq_8psk(gas_id) )
        {
          gl1_hw_mcpm_gprs_aeq_update(TRUE,gas_id);
        }
#endif
#ifdef FEATURE_AEQ_PHASE4
        gl1_reset_aeq4_ctl(gas_id);
#endif /*FEATURE_AEQ_PHASE4 */
#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_RANDOM_ACCESS_STATE,
                                            GL1_RXDIV_L1_TRANSFER_STATE,
                                            gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

        break;

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
      case L1_MULTI_SIM_IDLE:
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      case L1_IDLE_MODE:
      case L1_GPRS_IDLE_MODE:
        MSG_GERAN_HIGH_0_G( "MCPM change required; IDLE to TRANS");
                  gl1_clkdata_speed(TRUE, L1_DED_DATA_TRANS,L1_DED_DATA_NULL,L1_DATA,gas_id);
#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
        if (!gl1_hw_feature_disabled_aeq_8psk(gas_id) )
        {
          gl1_hw_mcpm_gprs_aeq_update(TRUE,gas_id);
        }
#endif
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        gpl1_set_tx_pwr_avg_active(gas_id);
        gpl1_set_rx_pwr_avg_active(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_GSM_RX_DIVERSITY
                  gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_IDLE_STATE,
                                                      GL1_RXDIV_L1_TRANSFER_STATE,
                                                      gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

        break;

#ifdef FEATURE_GSM_DTM
      case L1_DTM_MODE:
        MSG_GERAN_HIGH_2_G( "MCPM change required; DTM to TRANS state=0x%X isOn=%d", gl1_data_state[gas_id], gl1_data_is_on[gas_id]);
                  gl1_clkdata_speed(TRUE, L1_DED_DATA_TRANS,L1_DED_DATA_DTM,L1_DATA,gas_id);  /* state change only */
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_STOP_REQ, GL1_MCPM_GERAN_DATA,gas_id);
        gpl1_set_dtm_assig_status(FALSE, gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_DTM_STATE,
                                            GL1_RXDIV_L1_TRANSFER_STATE,
                                            gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

        /* reset RACH power reduction to deactive */
        gl1_set_rach_pwr_reduction(FALSE,gas_id);
        break;
#endif

      default:
        MSG_HIGH( "MCPM change required for TRANS", 0, 0, 0);
                  gl1_clkdata_speed(TRUE, L1_DED_DATA_TRANS,L1_DED_DATA_NULL,L1_DATA,gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_DEFAULT_STATE,
                                            GL1_RXDIV_L1_TRANSFER_STATE,
                                            gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
        break;
      }
      break;

    case L1_DEDICATED_MODE:
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
      gl1_hw_reset_typ1_asdiv_switch_algorithm(gas_id);
#endif /*FEATURE_GSM_TX_DIV_ANTNA_SWITCHING*/
#ifdef FEATURE_GSM_QSC_TX_DIV
      gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXTX ,gas_id);
      gl1_trm_asdiv_reset_traffic_state_vars(gas_id);
#endif /* FEATURE_GSM_QSC_TX_DIV */

#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
      gpl1_set_tx_pwr_avg_active(gas_id);
      gpl1_set_rx_pwr_avg_active(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

      /*Clear inter-rat HO triggered flag on entering dedicated mode*/
      l1_tskisr_blk->x2g_inter_rat_ho_triggered = FALSE;

      switch (StateNow)
      {
      case L1_RANDOM_ACCESS_MODE:
      case L1_PACKET_ACCESS_MODE:
        MSG_GERAN_HIGH_0_G( "MCPM change RAND to DED");
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_START_REQ, GL1_MCPM_GERAN_VOICE,gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_RANDOM_ACCESS_STATE,
                                            GL1_RXDIV_L1_DEDICATED_STATE,
                                            gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
        break;

#ifdef FEATURE_GSM_DTM
      case L1_DTM_MODE:
        gpl1_set_dtm_assig_status(FALSE, gas_id);
        if(gl1_msg_get_cs_timeslot(gas_id)==GL1_DEFS_TN_2)
        {
          /*
          ** CgC hang on to the DTM speed whilst still on slot 2, with state change
          */
                    gl1_clkdata_speed(TRUE, L1_DED_SPEEDUP_DTM_HANG, L1_DED_DATA_DTM,0,gas_id);
        }
        else
        {
                    gl1_clkdata_speed(FALSE, L1_DED_DATA_DTM, L1_DED_DATA_NULL,L1_DTM,gas_id);
        }
        MSG_GERAN_HIGH_2_G( "MCPM change required; DTM to DED state=0x%X isOn=%d", gl1_data_state[gas_id], gl1_data_is_on[gas_id]);
#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_DTM_STATE,
                                            GL1_RXDIV_L1_DEDICATED_STATE,
                                            gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
        break;
#endif

      default:
        MSG_GERAN_HIGH_0_G( "MCPM change required for DED");
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_START_REQ, GL1_MCPM_GERAN_VOICE,gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_DEFAULT_STATE,
                                            GL1_RXDIV_L1_DEDICATED_STATE,
                                            gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
        break;
      }
      break;

    case L1_RANDOM_ACCESS_MODE:
    case L1_PACKET_ACCESS_MODE:
      switch (StateNow)
      {

#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
      case L1_MULTI_SIM_IDLE:
      case L1_MULTI_SIM_DECOUPLED:
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      case L1_IDLE_MODE:
      case L1_GPRS_IDLE_MODE:
        MSG_GERAN_HIGH_0_G( "MCPM change required; IDLE to RAND");
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_START_REQ, GL1_MCPM_GERAN_RAND,gas_id);
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        gpl1_set_tx_pwr_avg_active(gas_id);
        gpl1_set_rx_pwr_avg_active(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_GSM_QSC_TX_DIV
        gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXTX ,gas_id);
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_IDLE_STATE,
                                            GL1_RXDIV_L1_RANDOM_ACCESS_STATE,
                                            gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY */

        break;

      case L1_FIND_BCCH:
#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
      case L1_MULTI_SIM_FIND_BCCH:
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

        MSG_GERAN_HIGH_0_G( "MCPM change required; FIND BCCH to RAND");
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_START_REQ, GL1_MCPM_GERAN_RAND,gas_id);
        break;

      default:
        break;
      }
      break;

    case L1_FIND_SCH:
    case L1_IDLE_MODE:
    case L1_GPRS_IDLE_MODE:
    case L1_FIND_BCCH:
    case L1_NULL_MODE:
    case L1_INACTIVE_MODE:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      /* DUAL_SIM STATE(S) */
    case L1_MULTI_SIM_NULL:
    case L1_MULTI_SIM_IDLE:
    case L1_MULTI_SIM_FIND_BCCH:
    case L1_MULTI_SIM_INACTIVE:
    case L1_MULTI_SIM_DECOUPLED:
      /* need to cater for the GTA case where could be in transfer then reading page
      don't want to change mcpm in this situation */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GSM_QSC_TX_DIV
      if (( newState == L1_NULL_MODE)||( newState == L1_INACTIVE_MODE)
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
        ||( newState == L1_MULTI_SIM_NULL)||( newState == L1_MULTI_SIM_INACTIVE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
        )
      {
        gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_INACTIVE ,gas_id);
      }


#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      else if( newState == L1_MULTI_SIM_FIND_BCCH)
      {
        gl1_trm_state_t curr_state = grm_get_trm_status(l1_tskisr_blk->client_id, gas_id);
        if( (curr_state == GL1_TRM_GRANTED) || (curr_state == GL1_TRM_RETAINED_FOR_ACCESS))
        {
          gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXACQ ,gas_id);
		  
#ifdef FEATURE_GL1_ASDIV_ACQ
        if (gl1_asdiv_acq_is_mon_active(gas_id))
        
        {
         /*On Entering This State, If scenario is for ReScan then Switch*/
          gl1_trm_ant_switch_div_set_config(gas_id);

          /*If anything except ReScan then Reset State*/
          if (!gl1_asdiv_acq_check_rescan(gas_id))
          {
            gl1_asdiv_acq_reset(gas_id);
          }
        }
#endif /*FEATURE_GL1_ASDIV_ACQ*/

      
        }
        else
        {
          gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_INACTIVE ,gas_id);
        }
      }
#endif
      else if( newState == L1_FIND_BCCH)
      {
        gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXACQ ,gas_id);

#ifdef FEATURE_GL1_ASDIV_ACQ
        if (gl1_asdiv_acq_is_mon_active(gas_id))
        
        {
         /*On Entering This State, If scenario is for ReScan then Switch*/
          gl1_trm_ant_switch_div_set_config(gas_id);

          /*If anything except ReScan then Reset State*/
          if (!gl1_asdiv_acq_check_rescan(gas_id))
          {
            gl1_asdiv_acq_reset(gas_id);
          }
        }
#endif /*FEATURE_GL1_ASDIV_ACQ*/

      }
      else
      {
        gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RX_ONLY ,gas_id);
      }
#endif /* FEATURE_GSM_QSC_TX_DIV */

             if((newState == L1_FIND_BCCH)
#if defined  ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               ||( newState ==L1_MULTI_SIM_FIND_BCCH)
#endif /**/
             )
             {
               l1_find_bcch_mcpm_change( TRUE, gas_id );
             }

      switch (StateNow)
      {
      case L1_RANDOM_ACCESS_MODE:
      case L1_PACKET_ACCESS_MODE:
        MSG_GERAN_HIGH_0_G( "MCPM change required; RAND to IDLE");
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_STOP_REQ, GL1_MCPM_GERAN_IDLE,gas_id);
        if((newState == L1_FIND_BCCH) ||(newState == L1_NULL_MODE)|| (newState == L1_INACTIVE_MODE))
        {
                    gl1_hw_turn_tx_off(TRUE, gas_id);
        }
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        gpl1_reset_tx_pwr_avg(gas_id);
        gpl1_reset_rx_pwr_avg(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */
#ifdef FEATURE_WLAN_COEX_SW_CXM
        gpl1_reset_snr_avg(gas_id);
#endif
        /* reset RACH power reduction to deactive */
        gl1_set_rach_pwr_reduction(FALSE,gas_id);
        gpl1_set_rx_pwr_avg_dbm10_coex((((int32) l1_get_serving_RLA_C(gas_id) * 10 ) >> 4),gas_id);
#ifdef FEATURE_AEQ_PHASE4
        gl1_reset_aeq4_ctl(gas_id);
#endif /*FEATURE_AEQ_PHASE4 */

#ifdef FEATURE_GSM_RX_DIVERSITY
	   gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_RANDOM_ACCESS_STATE, GL1_RXDIV_L1_IDLE_STATE, gas_id);
#endif /* FEATURE_GSM_RX_DIVERSITY */

        break;

#ifdef FEATURE_GSM_DTM
      case L1_DTM_MODE:
        gpl1_set_dtm_assig_status(FALSE, gas_id);
        MSG_GERAN_HIGH_0_G( "MCPM change required; DTM to IDLE");
                  gl1_clkdata_speed(FALSE, L1_DED_DATA_DTM, L1_DED_DATA_CLR,L1_DTM,gas_id);
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_STOP_REQ, GL1_MCPM_GERAN_IDLE,gas_id);
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        gpl1_reset_tx_pwr_avg(gas_id);
        gpl1_reset_rx_pwr_avg(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */
#ifdef FEATURE_WLAN_COEX_SW_CXM
        gpl1_reset_snr_avg(gas_id);
#endif
        /* reset RACH power reduction to deactive */
        gl1_set_rach_pwr_reduction(FALSE,gas_id);
        gpl1_set_rx_pwr_avg_dbm10_coex((((int32) l1_get_serving_RLA_C(gas_id) * 10 ) >> 4),gas_id);

#ifdef FEATURE_GPRS_GBTA
        SET_GBTA_STATUS(GRM_GBTA_NONE);
        grm_notify_event(GBTA_EV_NONE,TRUE,gas_id);
#endif /* FEATURE_GPRS_GBTA */
#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_DTM_STATE,
          GL1_RXDIV_L1_IDLE_STATE,
          gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
        break;
#endif

      case L1_DEDICATED_MODE:
       /*Remove this mcvs request as now moving from ded to idle/bcch*/
        gl1_hw_mcpm_amr_aeq_update(FALSE, gas_id);
        MSG_GERAN_HIGH_0_G( "MCPM change required; DED to IDLE");
                  gl1_clkdata_speed(FALSE, L1_DED_DATA_NULL, L1_DED_DATA_CLR,0,gas_id);     /* ensure all the state bits are cleared */
        gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_VOICE_STOP_REQ, GL1_MCPM_GERAN_IDLE,gas_id);
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        gpl1_reset_tx_pwr_avg(gas_id);
        gpl1_reset_rx_pwr_avg(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */
#ifdef FEATURE_WLAN_COEX_SW_CXM
        gpl1_reset_snr_avg(gas_id);
#endif
        /* reset RACH power reduction to deactive */
        gl1_set_rach_pwr_reduction(FALSE,gas_id);
        gpl1_set_rx_pwr_avg_dbm10_coex((((int32) l1_get_serving_RLA_C(gas_id) * 10 ) >> 4),gas_id);
#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_DEDICATED_STATE,
                                            GL1_RXDIV_L1_IDLE_STATE,
                                            gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#ifdef FEATURE_AEQ_PHASE4
        gl1_reset_aeq4_ctl(gas_id);
#endif /*FEATURE_AEQ_PHASE4 */
        break;

      case L1_TRANSFER_MODE:
        MSG_GERAN_HIGH_0_G( "MCPM change required; TRANS to IDLE");
                  gl1_clkdata_speed(FALSE, L1_DED_DATA_TRANS, L1_DED_DATA_CLR,L1_DATA,gas_id);
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
        gpl1_reset_tx_pwr_avg(gas_id);
        gpl1_reset_rx_pwr_avg(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */
#ifdef FEATURE_WLAN_COEX_SW_CXM
        gpl1_reset_snr_avg(gas_id);
#endif
        /* reset RACH power reduction to deactive */
        gl1_set_rach_pwr_reduction(FALSE,gas_id);
        gpl1_set_rx_pwr_avg_dbm10_coex((((int32) l1_get_serving_RLA_C(gas_id) * 10 ) >> 4),gas_id);
#ifdef FEATURE_GPRS_GBTA
        if (! IS_GBTA_ACTIVE())
        {
          SET_GBTA_STATUS(GRM_GBTA_NONE);
        }
        grm_notify_event(GBTA_EV_NONE,TRUE,gas_id);
#endif /* FEATURE_GPRS_GBTA */
#ifdef FEATURE_GSM_RX_DIVERSITY
        gl1_hw_rxdiv_handle_l1_change_state(GL1_RXDIV_L1_TRANSFER_STATE,
                                            GL1_RXDIV_L1_IDLE_STATE,
                                            gas_id);
#endif /*FEATURE_GSM_RX_DIVERSITY*/
        break;

      default:
#ifdef FEATURE_GPRS_GBTA
        grm_notify_event(GBTA_EV_NONE,TRUE,gas_id);
#endif /* FEATURE_GPRS_GBTA */
        break;
      }

      break;

    case L1_RESET_MODE:
      MSG_GERAN_HIGH_0_G( "MCPM change required; RESET");
      gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_STOP_REQ, GL1_MCPM_GERAN_STOP,gas_id);
      gl1_clkdata_state_init(gas_id);       /* just incase ensure all the state bits are cleared */
#ifdef FEATURE_GSM_GPRS_TIME_PWR_AVG
      gpl1_reset_tx_pwr_avg(gas_id);
      gpl1_reset_rx_pwr_avg(gas_id);
#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */
#ifdef FEATURE_WLAN_COEX_SW_CXM
      gpl1_reset_snr_avg(gas_id);
#endif

#ifdef FEATURE_GPRS_GBTA
      /* Panic  - Should this gas set transfer gas  gbta_state.*/
      //SET_GBTA_STATUS(GRM_GBTA_NONE);
      grm_notify_event(GBTA_EV_NONE,TRUE,gas_id);
#endif /* FEATURE_GPRS_GBTA */

      /* reset RACH power reduction to deactive */
      gl1_set_rach_pwr_reduction(FALSE,gas_id);
      gpl1_set_rx_pwr_avg_dbm10_coex((((int32) l1_get_serving_RLA_C(gas_id) * 10 ) >> 4),gas_id);
      break;


    default:
      MSG_GERAN_HIGH_2_G( "### GL1 Unhandled state ### %d->%d", StateNow, newState);
#ifdef FEATURE_GPRS_GBTA
      grm_notify_event(GBTA_EV_NONE,TRUE,gas_id);
#endif /* FEATURE_GPRS_GBTA */
      break;
    } /* newstate */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    l1_tskisr_blk->l1_prev_state = StateNow;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

    l1_tskisr_blk->l1_state = newState;

    /*Set the state change flag to TRUE*/
    gl1_flag_state_change[gas_id] = TRUE;
   /* If Gl1 State changes, check if GTS data can be sent */
    if(newState != StateNow)
    {
      GL1_ISR_LOCK(gas_id);

      /* Stop periodic Timer if it is already running */
      if( gl1_gts_timer_running[gas_id] )
      {
        gl1_gts_timer_deactivate(gas_id);
      }

      /* If new state is DED/DTM/Tranfer mode then start the timer */
      if(gl1_gts_chk_state_to_send(gas_id))
      {
        gl1_gts_timer_start(GL1_GTS_UPDATE_TIMEOUT, gas_id);
      }

      GL1_ISR_UNLOCK(gas_id);
    }
#ifdef FEATURE_GSM_WTR_HOP
    /*
    * Reset WTR Hop state machine to avoid leaving it in a bad state
    */
    gl1_msg_wtr_hop_rude_reset(gas_id);
#endif

    /*If we are moving in or out of idle state, check if there are any FT handlers present
    at msg layer */
    if ( ( newState == L1_IDLE_MODE )
      || ( StateNow == L1_IDLE_MODE  )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      || ( newState == L1_MULTI_SIM_IDLE )
      || ( StateNow == L1_MULTI_SIM_IDLE )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      )
    {
      gl1_msg_log_ft_handlers(gas_id);
    }

#ifdef FEATURE_GSM_COEX_SW_CXM
    /* The arbitrator needs to be notified of transitions to FIND_BCCH and INACTIVE states */
    switch(l1_tskisr_blk->l1_state)
    {
    case L1_FIND_BCCH:
#ifdef FEATURE_DUAL_SIM
    case L1_MULTI_SIM_FIND_BCCH:
#endif /* FEATURE_DUAL_SIM */
      garb_intf_notify_find_bcch(gas_id);
      break;

    case L1_NULL_MODE:
    case L1_INACTIVE_MODE:
    case L1_RESET_MODE:
#ifdef FEATURE_DUAL_SIM
    case L1_MULTI_SIM_NULL:
    case L1_MULTI_SIM_INACTIVE:
#endif /* FEATURE_DUAL_SIM */
#ifndef FEATURE_THOR_MODEM
      garb_intf_notify_exit(gas_id);
#endif
      break;

    default:
      break;
    }

    /* Nullify find_bcch params , when state changes*/
    /* check is performed as during find_bcch , GL1 transitions to NULL abnd back to FIND_BCCH
       so as not to reset the info required for COEX */
    if ((l1_tskisr_blk->l1_state != L1_FIND_BCCH) &&
        (l1_tskisr_blk->command  != L1_ACQUIRE_DURING_RESELECTION_COMMAND) &&
        (l1_tskisr_blk->command  != L1_ACQUIRE_COMMAND)
      #ifdef FEATURE_DUAL_SIM
        && (l1_tskisr_blk->l1_state != L1_MULTI_SIM_FIND_BCCH)
      #endif   /* FEATURE_DUAL_SIM */
        )
    {
      garb_intf_notify_find_bcch_update_arfcn(arfcn,gas_id);
    }
#endif /* FEATURE_GSM_COEX_SW_CXM */

    /* Update rpush_freq_err_in_ppm value during BCCH to IDLE state transition */
    if (l1_state_change_bcch_to_idle(gas_id) == TRUE)
    {
      /* Force update TCXO */
      gl1_msg_bcch_to_idle_init(gas_id);
    }

    l1_log_state(gas_id);
  }
}

/*===========================================================================

FUNCTION gl1_dtm_speedup_notify

DESCRIPTION
  Notified when slot 2 is slammed to slot 0
  So the DTM->DED speedup can be removed.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_dtm_speedup_notify(gas_id_t gas_id)
{
   gl1_clkdata_speed(FALSE, L1_DED_SPEEDUP_DTM_HANG, L1_DED_DATA_NULL,0,gas_id);
}

/*===========================================================================

FUNCTION gl1_dtm_speedup_init

DESCRIPTION
  Initialise the dtm speed state variables

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_clkdata_state_init(gas_id_t gas_id)
{
   gl1_data_is_on[gas_id]  = FALSE;
   gl1_data_state[gas_id]  = 0;
}

/*===========================================================================

FUNCTION gl1_set_clkdata_speed

DESCRIPTION
  This function sets the clock speed to data mode state.
  All states for MCPM_GERAN_DATA_START_REQ are now passed thru this function.
  and states are recorded in state bit fields.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_set_clkdata_speed(boolean dataSpeedReq, uint16 speedState, uint16 prevSpeedState, gas_id_t gas_id)
{
   gl1_data_state[gas_id] &= ~prevSpeedState;     /* reset any prev state change   */

   if(dataSpeedReq)
   {
      gl1_data_state[gas_id] |= speedState;       /* state requesting the speedup */

      if (gl1_data_state[gas_id] && (gl1_data_is_on[gas_id] == FALSE))
      {
         MSG_GERAN_HIGH_3_G( "MCPM change Speed START, req=0x%X prev=0x%X actual=0x%X", speedState, prevSpeedState, gl1_data_state[gas_id]);
         gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_DATA_START_REQ, GL1_MCPM_GERAN_DATA,gas_id);
         gl1_data_is_on[gas_id] = TRUE;
      }
   }
   else
   {
      /* Clear reason for speed request */
      gl1_data_state[gas_id] &= ~speedState;

      if (!gl1_data_state[gas_id] && (gl1_data_is_on[gas_id] == TRUE))
      {
          gl1_mcpm_state_type gl1_state;

          if(L1_DED_DATA_CLR==prevSpeedState)
          {
             gl1_state = GL1_MCPM_GERAN_IDLE;
          }
          else
          {
             gl1_state =  GL1_MCPM_GERAN_VOICE;
          }
          /*
          ** only turn off when all requesting states have been removed
          */
          gl1_data_is_on[gas_id] = FALSE;
          MSG_GERAN_HIGH_3_G( "MCPM change Speed STOP, req=0x%X prev=0x%X actual=0x%X", speedState, prevSpeedState, gl1_data_state[gas_id]);
          gl1_hw_mcpm_state_update_dpc(MCPM_GERAN_DATA_STOP_REQ, gl1_state,gas_id);
      }
   }
}
/*===========================================================================

FUNCTION gl1_clkdata_speed

DESCRIPTION
  This function changes the SW clock speed in dedicated mode
  All states for MCPM_GERAN_DATA_START_REQ are now passed thru this function.
  and states are recorded in state bit fields.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_clkdata_speed(boolean dataSpeedReq, uint16 speedState, uint16 prevSpeedState,uint32 scenario, gas_id_t gas_id )
{
   if ((gl1_mcvs_clock_set(dataSpeedReq, speedState,scenario, gas_id)) == FALSE)
   {
      gl1_set_clkdata_speed(dataSpeedReq, speedState, prevSpeedState, gas_id);
   }
}
/*===========================================================================

FUNCTION  compare_signal_strength_id

DESCRIPTION
  These functions are to be used by qsort as a compare function. They are
  exclusively used to sort an array with the indexes to have the L1_Frequency_List ARFCN/
  power/agc in descending order.

DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
static int compare_signal_strength_id1(const void * a, const void * b)
{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[GERAN_DATA_SPACE_INDEX_1];

  /* Indexes of both elements to be compared */
  uint16 *index1_ptr = (uint16 *)a;
  uint16 *index2_ptr = (uint16 *)b;

  return(l1_Frequency_List->power_dBm_x16[*index2_ptr] - l1_Frequency_List->power_dBm_x16[*index1_ptr]);
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
static int compare_signal_strength_id2(const void * a, const void * b)
{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[GERAN_DATA_SPACE_INDEX_2];

  /* Indexes of both elements to be compared */
  uint16 *index1_ptr = (uint16 *)a;
  uint16 *index2_ptr = (uint16 *)b;

  return(l1_Frequency_List->power_dBm_x16[*index2_ptr] - l1_Frequency_List->power_dBm_x16[*index1_ptr]);
}

#if defined ( FEATURE_TRIPLE_SIM )
static int compare_signal_strength_id3(const void * a, const void * b)
{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[GERAN_DATA_SPACE_INDEX_3];

  /* Indexes of both elements to be compared */
  uint16 *index1_ptr = (uint16 *)a;
  uint16 *index2_ptr = (uint16 *)b;

  return(l1_Frequency_List->power_dBm_x16[*index2_ptr] - l1_Frequency_List->power_dBm_x16[*index1_ptr]);
}
#endif /* FEATURE_TRIPLE_SIM */
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION  L1_SORT_SIGNAL_STRENGTHS

DESCRIPTION
  This function sorts the ARFCNs in the L1_Frequency_List in descending
  order of signal strength.

DEPENDENCIES
  None

RETURN VALUE
  uint16 - number of ARFCN's that satisfy the threshold criteria.

SIDE EFFECTS
  None

===========================================================================*/
uint16 L1_sort_signal_strengths( gas_id_t gas_id )
{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];
   /* Locals */
   dBx16_T  pwr_threshold;
   uint16          i;
#ifdef GL1_GMSK_MOD_DET
   uint16 dec_threshold = gl1_hw_is_gdet_threshold_enabled();
   boolean gdet_enabld  = gl1_hw_is_gdet_enabled();
#endif

   /* determine the threshold that was used for this scan */
   pwr_threshold = L1_pscan_get_threshold(gas_id);

   /* Bounds check to keep KW happy */
   if ( l1_Frequency_List->length >
          ( sizeof( l1_Frequency_List->ARFCN ) / sizeof( l1_Frequency_List->ARFCN[0] ) ) )
   {
     MSG_GERAN_ERROR_2_G( "Too many freq list elements requested %d clamp to max %d",
                l1_Frequency_List->length,
                ( sizeof( l1_Frequency_List->ARFCN ) / sizeof( l1_Frequency_List->ARFCN[0] ) ));

     l1_Frequency_List->length =
       ( sizeof( l1_Frequency_List->ARFCN ) / sizeof( l1_Frequency_List->ARFCN[0] ) );
   }

   if (l1_Frequency_List->length > 1)
   {
     /* The sort threshold is stored in the power scan global data */
      /* Filter out zeros and very low power values */
      /* Also filter out adjacent frequencies which are wekaer than the center
         frequency (carrier) by 13dB.   This eliminates the ghost cells in
         power scan which increase acquisition time
      */
      for (i =0  ; i < l1_Frequency_List->length  ; i++)
      {
         if ( i > 0 )
         {
            if (  ( l1_Frequency_List->ARFCN[i].num - l1_Frequency_List->ARFCN[i - 1].num == 1 ) &&
                  ( l1_Frequency_List->ARFCN[i].band == l1_Frequency_List->ARFCN[i - 1].band ) &&
                  ( l1_Frequency_List->power_dBm_x16[i-1] - l1_Frequency_List->power_dBm_x16[i]  < SYS_PARAM_ADJACENT_CELL_PWR_RX_LVL_DELTA_dB_x16 ) )
            {

#ifdef DEBUG_PSCAN
               #error code not present
#endif
               l1_Frequency_List->power_dBm_x16[i-1] = pwr_threshold;
            }
         }

         if ( i < l1_Frequency_List->length - 1 )
         {
            if (  ( l1_Frequency_List->ARFCN[i + 1].num - l1_Frequency_List->ARFCN[i].num == 1 ) &&
                  ( l1_Frequency_List->ARFCN[i + 1].band == l1_Frequency_List->ARFCN[i].band ) &&
                  ( l1_Frequency_List->power_dBm_x16[i + 1] - l1_Frequency_List->power_dBm_x16[i]  < SYS_PARAM_ADJACENT_CELL_PWR_RX_LVL_DELTA_dB_x16 ) )
            {

#ifdef DEBUG_PSCAN
               #error code not present
#endif
               l1_Frequency_List->power_dBm_x16[i+1] = pwr_threshold;
            }
         }

 #ifdef GL1_GMSK_MOD_DET
       /*pruning based on des stat avg*/
		 if(dec_threshold >= l1_Frequency_List->decstat[i])
        {
         if(gl1_hw_is_gdet_debug_enabled())
         {
          MSG_GERAN_LOW_3_G("signal is  GMSK ARFCN:%d  decstat threshold %d decstat avg:%d",l1_Frequency_List->ARFCN[i].num,dec_threshold,l1_Frequency_List->decstat[i]);
         }
        }
	   else
	   {
	   if( gdet_enabld == TRUE)
	   {
	    l1_Frequency_List->power_dBm_x16[i] = pwr_threshold;
        if(gl1_hw_is_gdet_debug_enabled())
        {
		  MSG_GERAN_LOW_3_G("signal is not GMSK ARFCN:%d  decstat threshold %d decstat avg:%d",l1_Frequency_List->ARFCN[i].num,dec_threshold,l1_Frequency_List->decstat[i]);
        }
	   }
	  }
#endif
         if ( l1_Frequency_List->power_dBm_x16[i] < pwr_threshold )
         {
            l1_Frequency_List->power_dBm_x16[i] = pwr_threshold;
         }

         /* Before sorting the array of indexes, make sure they contain the
          * correct values, starting from 0 until the length -1 */
         l1_Frequency_List->sorted_array_indexes[i] = i;
      }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#if defined ( FEATURE_TRIPLE_SIM )
      if ( GERAN_ACCESS_STRATUM_ID_3 == gas_id )
      {
        /* sort results by power using a function specific to the secondary data space */
        qsort( l1_Frequency_List->sorted_array_indexes, l1_Frequency_List->length, sizeof(uint16), compare_signal_strength_id3 );
      }
      else
#endif /* FEATURE_TRIPLE_SIM */
      if (GERAN_ACCESS_STRATUM_ID_2 == gas_id)
      {
        /* sort results by power using a function specific to the secondary data space */
        qsort(l1_Frequency_List->sorted_array_indexes, l1_Frequency_List->length, sizeof(uint16), compare_signal_strength_id2);
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
        /* sort results by power using a function specific to the primary data space */
        qsort(l1_Frequency_List->sorted_array_indexes, l1_Frequency_List->length, sizeof(uint16), compare_signal_strength_id1);
      }

      /* Find first occurrence of a frequency whose power is
        * the lower threshold and set the number threshold*/
      for(i = 0; i < l1_Frequency_List->length; i++)
      {
          if(l1_Frequency_List->power_dBm_x16[l1_Frequency_List->sorted_array_indexes[i]] == pwr_threshold)
         {
            break;
         }
      }

      l1_Frequency_List->num_above_sort_threshold_criteria = i;

   }
   else if (l1_Frequency_List->length == 1)
   {
      /* First of all, ensure the sorted array index is filled
       * correctly.
       */
      l1_Frequency_List->sorted_array_indexes[0] = 0;

      /* Now use the power comparison to set the num above sort
       * threshold criteria variable.
       */
      if( l1_Frequency_List->power_dBm_x16[0] > pwr_threshold )
      {
        l1_Frequency_List->num_above_sort_threshold_criteria = 1;
      }
      else
      {
        l1_Frequency_List->num_above_sort_threshold_criteria = 0;
      }
   }

#if defined (FEATURE_GSM_SIMULATE_GL1_RF) || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
   #error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */
   l1_log_power_scan(gas_id);

   return l1_Frequency_List->num_above_sort_threshold_criteria;

}


/*===========================================================================

FUNCTION  L1_MAP_DBM_X16_TO_RXLEV

DESCRIPTION
  This function translates a dBx16_T to RXLEV per GSM05.08, 8.1.4.

DEPENDENCIES
  None

RETURN VALUE
  RXLEV - 0..63

SIDE EFFECTS
  None

===========================================================================*/
uint8 L1_map_dBm_x16_to_rxlev( dBx16_T pwr_lvl_x16 )
{
  uint8 RXLEV;

  if (pwr_lvl_x16 <= dBm_x16_VALUE_FOR_RXLEV_0) {
    pwr_lvl_x16 = dBm_x16_VALUE_FOR_RXLEV_0;     /* floor    */
  } else if (pwr_lvl_x16 > dBm_x16_VALUE_FOR_RXLEV_63) {
    pwr_lvl_x16 = dBm_x16_VALUE_FOR_RXLEV_63;    /* saturate */
  }

  RXLEV = (uint8)(((dBx16_T)(pwr_lvl_x16 - dBm_x16_VALUE_FOR_RXLEV_0))/16);

  return RXLEV;
}

/*===========================================================================

FUNCTION  L1_MAP_RXLEV_TO_DBM_X16

DESCRIPTION
  This function translates a RXLEV to dBx16_T per GSM05.08, 8.1.4.

DEPENDENCIES
  None

RETURN VALUE
  dBx16_T - rx_power

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T L1_map_rxlev_to_dBm_x16( uint8 RXLEV )
{
  dBx16_T pwr_lvl_x16;

  pwr_lvl_x16 = ((dBx16_T)RXLEV * 16) + dBm_x16_VALUE_FOR_RXLEV_0;

  return pwr_lvl_x16;
}

/*===========================================================================

FUNCTION  L1_NORMALISE_SIGNAL_STRENGTHS

DESCRIPTION
  This function averages the accumulated Rx power measurement and updates
  the appropriate AGC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_normalise_signal_strengths( uint16         length,
                                    uint8          no_of_measurement[],
                                    dBx16_T        pwr_dBm_x16[],
                                    sys_algo_agc_T agc[]
#ifdef GL1_GMSK_MOD_DET
                                    ,uint16          decstat[]
#endif                                               
                                   )
{
  uint16   i;
  dBx16_T  pwr_dBm_x16_average;
#ifdef GL1_GMSK_MOD_DET
  uint16 decstat_avg;
#endif

  if ( length > RR_MAX_PSCAN_FREQ_SCAN )
  {
    length = RR_MAX_PSCAN_FREQ_SCAN ;

  }

  for ( i=0; i< length; i++)
  {
    if ( no_of_measurement[i] > 1 )
    {
      pwr_dBm_x16_average = pwr_dBm_x16[i]/no_of_measurement[i];
      agc[i].pwr_dBm_x16  = pwr_dBm_x16[i] = pwr_dBm_x16_average;
#ifdef GL1_GMSK_MOD_DET	  
	  decstat_avg = decstat[i]/no_of_measurement[i];
	  decstat[i] =  decstat_avg;
      if(gl1_hw_is_gdet_debug_enabled())
      {
	    MSG_GERAN_HIGH_4("decstat :%d  no of measurements  %d desstat avg:%d arfcn %d",decstat[i],no_of_measurement[i],decstat_avg,agc[i].arfcn.num);
      }
#endif 
	  
    }

    no_of_measurement[i] = 0;
  }
}

/*===========================================================================

FUNCTION  L1_CALCULATE_PAGING_DATA

DESCRIPTION
  This function does the paging parameter calculations as in GSM 05.02
  section 6.5.2

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_calculate_paging_data( gas_id_t gas_id )
{
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   /* Locals */
   word n; /* the N of the above spec */
   word pg; /* the Paging Group */
   word i,imsi;
   layer1_sys_info_T *cmpd_dta;
   paging_data_T     *pgng_dta;
   uint8              avg_len;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   cmpd_dta = &(l1_idle_data->campedon_cell_data);
   pgng_dta = &(l1_idle_data->paging_data);

   if (cmpd_dta->BS_PA_MFRMS == 0)
   {
      cmpd_dta->BS_PA_MFRMS = 2;
      MSG_GERAN_ERROR_0_G("BS_PA_MFRMS was 0.  Setting to 2.");
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if (gl1_msg_get_multi_sim_mode())
   {
     grm_set_drx_cycle( cmpd_dta->BS_PA_MFRMS, l1_tskisr_blk->client_id, gas_id );
   }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   if (cmpd_dta->BS_CC_CHANS == 0)
   {
      cmpd_dta->BS_CC_CHANS = 1;
      MSG_GERAN_ERROR_0_G("BS_CC_CHANS was 0.  Setting to 1.");
   }

   if (l1_idle_data->imsi_data.imsi_valid == TRUE)
   {
      if (cmpd_dta->BS_CCCH_SDCCH_COMB == FALSE)
      {
         n = (9 - cmpd_dta->BS_AG_BLKS_RES);
      }
      else
      {
         n = (3 - cmpd_dta->BS_AG_BLKS_RES);
      }
      n  *= cmpd_dta->BS_PA_MFRMS;
      i = (word)(n*cmpd_dta->BS_CC_CHANS);
      imsi = (word)(l1_idle_data->imsi_data.imsi_mod_1000.msb * 256);
      imsi += l1_idle_data->imsi_data.imsi_mod_1000.lsb;
      pg = imsi % i;
      pgng_dta->paging_group = (uint8)pg;
      pgng_dta->ccch_group   = (uint8)(pg/n);
      pg = pg%n;
      i = n / cmpd_dta->BS_PA_MFRMS;
      pgng_dta->paging_multiframe = (uint8)(pg / i);
      pgng_dta->paging_block_index = (uint8)( ( pg % i) +
                                              cmpd_dta->BS_AG_BLKS_RES);
      pgng_dta->paging_data_valid = TRUE;

      if (cmpd_dta->BS_CCCH_SDCCH_COMB == FALSE)
      {
         if ((pgng_dta->paging_block_index == 7) ||
             (pgng_dta->paging_block_index == 8))
         {
            pgng_dta->ext_paging_multiframe  = (uint8) ((pgng_dta->paging_multiframe + 1)
                                                       % cmpd_dta->BS_PA_MFRMS);
            pgng_dta->ext_paging_block_index = (uint8) ((pgng_dta->paging_block_index + 2
                                                       + cmpd_dta->BS_AG_BLKS_RES)
                                                       % 9);
         }
         else
         {
            pgng_dta->ext_paging_multiframe  = pgng_dta->paging_multiframe;
            pgng_dta->ext_paging_block_index = (uint8) (pgng_dta->paging_block_index + 2);
         }
      }
      else
      {
         if (cmpd_dta->BS_AG_BLKS_RES == 2)
         {
            pgng_dta->ext_paging_multiframe  = (uint8) ((pgng_dta->paging_multiframe + 2)
                                                       % cmpd_dta->BS_PA_MFRMS);
            pgng_dta->ext_paging_block_index = 2;
         }
         else
         {
            if (pgng_dta->paging_block_index == 0)
            {
               pgng_dta->ext_paging_multiframe  = pgng_dta->paging_multiframe;
            }
            else
            {
               pgng_dta->ext_paging_multiframe  = (uint8) ((pgng_dta->paging_multiframe + 1)
                                                          % cmpd_dta->BS_PA_MFRMS);
            }
            pgng_dta->ext_paging_block_index = (uint8) ((pgng_dta->paging_block_index + 2
                                                       + cmpd_dta->BS_AG_BLKS_RES)
                                                       % 3);
         }
      }
   }
   else
   {
      pgng_dta->paging_data_valid  = FALSE;
      pgng_dta->paging_group       = 0;
      pgng_dta->ccch_group         = 0;
      pgng_dta->paging_multiframe  = 0;
      pgng_dta->paging_block_index = cmpd_dta->BS_AG_BLKS_RES;
      pgng_dta->ext_paging_multiframe  = 0;
      pgng_dta->ext_paging_block_index = 0;

      MSG_GERAN_ERROR_0_G("IMSI not valid.");
   }

   /* Calculate the frequency of serving cell power measurement
      averaging */
   pgng_dta->pwr_av_freq = (uint8)((5*cmpd_dta->BS_PA_MFRMS)/2);
   if ( pgng_dta->pwr_av_freq < 11 )
   {
      pgng_dta->pwr_av_freq = 11;
   }

   /* Work out how many measurements to include in
    * serving cell power measurement running average.
    *
    * See 5.08 section 6.6.1.
    */
   avg_len = 21 / cmpd_dta->BS_PA_MFRMS;
            /* 5s / (time between own paging blocks) */
   if (avg_len < 5)
   {
      avg_len = 5;
   }

   l1_running_avg_set_avg_len( &l1_idle_data->campedon_cell_pwr_meas_avg, avg_len, gas_id );
}



/*===========================================================================

FUNCTION  L1_CALCULATE_ARFCN

DESCRIPTION
  This function calculates the next frequency when frequency hopping. The
  algorithm is given in GSM 05.02 section 6.2.3.

DEPENDENCIES
  pointer to channel data

RETURN VALUE
  next frequency ( from mobile frequency allocation list )

SIDE EFFECTS
  None

===========================================================================*/
ARFCN_T L1_calculate_ARFCN(
         int16 offset,
         channel_information_T *channel_data, gas_id_t gas_id)

{
  /* Locals */

  ARFCN_T next_ARFCN;
  uint32 N; /* number of mobile allocated frequencies */
  uint32 M; /* intermediate integer defined */
  uint32 Mprime;
  uint32 MAI; /* index to the mobile allocated frequency list */
  uint32 T1R; /* reduced T1 */
  uint32 Tprime;
  uint32 S;
  uint32 i;
  uint32 twotoNBIN;
  uint32 T3;
  uint32 fn;
  byte   fn_mod_102;
  byte   fn_mod_104;


  /* offset frame not used for hopping calculation */
  fn         = GSTMR_GET_FN_GERAN( gas_id);

  if(offset < 0)
  {
     fn = SUB_FN(fn, (uint16)(-offset));
  }
  else
  {
     fn = ADD_FN (fn, (uint16)offset);
  }

  fn_mod_102 = fn%102;
  fn_mod_104 = fn%104;

  N = channel_data->frequency_list.no_of_items;

  /* If the Number of frequencies in hopping list is 0 or greater than maximum */
  if ( ( N == 0 ) ||
       ( N >= ( sizeof( NBIN ) / sizeof ( NBIN[0] ) ) )
     )
  {
     MSG_GERAN_ERROR_0_G("Invalid parameters to L1_calculate_ARFCN().");
     return NULL_ARFCN;
  }

  if (channel_data->hopping_flag)
  {
    geran_eng_mode_data_write(ENG_MODE_HOPPING_FREQUENCY_LIST,&(channel_data->frequency_list),TRUE, gas_id);

    if ( channel_data->HSN == 0)
    {
      /* Cyclic hopping */
      MAI = (fn + channel_data->MAIO) % N;
    }
    else
    {
      /* Pseudo-random hopping */

      T1R = (fn / 1326) % 64;

      /* here, T3 = FN mod 51 */

      T3 = fn_mod_102 % 51;
      i = (channel_data->HSN ^ T1R ) + T3;
      M = RNTABLE [ i ];
      M += (fn_mod_104 % 26);

      twotoNBIN = NBIN[N];

      if (twotoNBIN == 0)
      {
         MSG_GERAN_ERROR_0_G("Invalid parameters to L1_calculate_ARFCN().");
         return NULL_ARFCN;
      }

      Mprime = M % twotoNBIN;
      Tprime = T3 % twotoNBIN;

      if ( Mprime < N)
      {
        S = Mprime;
      }
      else
      {
        S = (Mprime + Tprime ) % N ;
      }
      MAI = ( S + channel_data->MAIO ) % N;

    }
  }
  else
  {
    /* No hopping */
    MAI = 0;
    geran_eng_mode_data_write(ENG_MODE_DED_OR_PDCH_ARFCN, &(channel_data->frequency_list.channel[0]), TRUE, gas_id);
  }

  {
    eng_mode_hopping_status_t eng_mode_hopping = {0};

    eng_mode_hopping.hopping_status = channel_data->hopping_flag;

    geran_eng_mode_data_write(ENG_MODE_HOPPING_STATUS, (void *)&eng_mode_hopping, TRUE, gas_id);
  }

  next_ARFCN = channel_data->frequency_list.channel[MAI];

  return ( next_ARFCN );
}


/*===========================================================================

FUNCTION L1_get_ARFCNS

DESCRIPTION
  This function is called to retrieve the next 4 ARFCNs to use.



DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void L1_get_ARFCNS (
                      int16 start,
                      uint16 increment,
                      uint16 num_arfcns,
                      ARFCN_T arfcns[],
                      channel_information_T *chnl1_info_ptr,
                      channel_information_T *chnl2_info_ptr,
                      channel_information_T *freq_redef_chnl_info_ptr,gas_id_t gas_id
                   )
{
  uint16  i;
  int16  offset;
  uint32 fn;

  for (i=0, offset=start; i<num_arfcns; i++, offset+=increment)  /*lint !e734 Loss of precision (assignment) (16 bits to 15 bits)*/
  {
     /* offset frame nos used for hopping calculation */
     fn = GSTMR_GET_FN_GERAN( gas_id);
     if ( offset < 0 )
     {
        fn = SUB_FN(fn, (uint16)(-offset));
     }
     else
     {
        fn = ADD_FN (fn, (uint16)offset);
     }

     /*
        If frequency redefinition is in progress and this
        particular FN, is after the freq redef params, apply
        the freq redef channel info
     */
     if ( L1_is_FN_after_freq_redef_start_time ( fn , gas_id) )
     {
       arfcns[i] = L1_calculate_ARFCN(offset, freq_redef_chnl_info_ptr, gas_id);
     }
     /*
        If we have passed the start time, use the after parameters
        else use the before.  This is to handle cases when the start
        time lies across the middle of a block
     */
     else if ( L1_is_FN_after_start_time ( fn, gas_id ) )
     {
       arfcns[i] = L1_calculate_ARFCN(offset, chnl2_info_ptr, gas_id);
     }
     else
     {
       arfcns[i] = L1_calculate_ARFCN(offset, chnl1_info_ptr, gas_id);
     }
  }
}

/*===========================================================================

FUNCTION L1_get_txlev

DESCRIPTION
  This function is called to retrieve the next 4 tx power indices starting
  at the specified offset

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void L1_get_txlev (
                    uint8 block_start,
                    uint8 burst_spacing,
                    int16 block_txlev[],
                    gas_id_t gas_id)
{
    L1_retrieve_txlev(block_start,burst_spacing,block_txlev,gas_id);


    /* Write the TxLev */
    geran_eng_mode_data_write(ENG_MODE_TX_PWR_DED, (int16 *)&block_txlev[0], TRUE, gas_id);

}

/*===========================================================================

FUNCTION  L1_convert_power_idx_to_dBm

DESCRIPTION
  Converts power index to dB

===========================================================================*/
int16 L1_convert_power_idx_to_dBm( sys_band_T band, uint16 pidx )
{
  if( ( band < INVALID_BAND) && ( pidx < MAX_RF_POWER_INDEX+1) )
  {
    return (power_index_to_dBm[band][pidx]);
  }
  return 0;
}

/*===========================================================================

FUNCTION  L1_convert_tx_pwr_from_dBm_to_pcl

DESCRIPTION
  Converts TX power from dBm to power control level (PCL)
  Note: This function should only be used for TX power conversion as it 
  doesnt take negative dBm power into consideration

===========================================================================*/

uint8 L1_convert_tx_pwr_from_dBm_to_pcl( sys_band_T band, uint16 dBm, gas_id_t gas_id)
{
  uint16 const *ptr;
  uint8 pwr_indx = 0;
  uint8 i = 0;

  static const uint16 txlev_to_dBm_PGSM900_EGSM900_850[MAX_TXLEV+1] = 
                                             {39, 39, 39, 37, 35, 33, 31, 29, 27, 25, 
                                              23, 21, 19, 17, 15, 13, 11,  9,  7,  5,  
                                              0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
  
  static const uint16 txlev_to_dBm_PCS1900_DCS1800[MAX_TXLEV+1] = 
                                             {30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 
                                              10,  8,  6,  4,  2,  0,  0,  0,  0,  0,  
                                              0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };


  switch(band)
  {
    case SYS_BAND_CELL_850:
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
    {
      ptr = txlev_to_dBm_PGSM900_EGSM900_850;
    }
    break;

    case SYS_BAND_PCS_1900:
    case SYS_BAND_DCS_1800:
    {
      ptr = txlev_to_dBm_PCS1900_DCS1800;
    }
    break;
    
    default:
    {
      MSG_GERAN_ERROR_0_G("L1_convert_tx_pwr_from_dBm_to_pcl - INVALID BAND");
      return 0;
    }
  }


  while(i <= MAX_TXLEV)
  {
    if(ptr[i] <= dBm)
    {
      break;
    }
    i++;
  }

  if( i!= (MAX_TXLEV+1) )
  {
    pwr_indx = i;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("L1_convert_tx_pwr_from_dBm_to_pcl - INVALID input power in dBM %d", dBm);
  }
  return pwr_indx;
  
  
}
/*===========================================================================
FUNCTION  L1_CONVERT_WORD

DESCRIPTION
  This function converts a word into an ascii string
         ( decimal representation )

DEPENDENCIES
  **char    destination pointer
  *word     character running total
   word   word to convert to ascii decimal

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_convert_word(char **dest_str, uint16 *char_count, uint16   input)
{
   char   pon;
   boolean  leading_zero = TRUE;

   /* add a space */
   **dest_str = ' ';
   (*dest_str)++;
   (*char_count)++;
   if ((pon = (char)(input/10000)) > 0)
   {
   /* convert to ascii */
      pon += 48;
   /* save */
      **dest_str = pon;
      (*dest_str)++;
      (*char_count)++;
      leading_zero = FALSE;
   }
   if (((pon = (char)((input/1000)%10)) > 0)||
      (leading_zero == FALSE))
   {
   /* convert to ascii */
      pon += 48;
   /* save */
      **dest_str = pon;
      (*dest_str)++;
      (*char_count)++;
      leading_zero = FALSE;
   }
   if (((pon = (char)((input/100)%10)) > 0)||
      (leading_zero == FALSE))
   {
   /* convert to ascii */
      pon += 48;
   /* save */
      **dest_str = pon;
      (*dest_str)++;
      (*char_count)++;
      leading_zero = FALSE;
   }
   if (((pon = (char)((input/10)%10)) > 0)||
      (leading_zero == FALSE))
   {
   /* convert to ascii */
      pon += 48;
   /* save */
      **dest_str = pon;
      (*dest_str)++;
      (*char_count)++;
   }
   pon = (char)(input %10);

   /* convert to ascii */
   pon += 48;
   /* save */
   **dest_str = pon;
   (*dest_str)++;
   (*char_count)++;
}

/*===========================================================================

FUNCTION  l1_compute_tc

DESCRIPTION
  Calculate the value of TC based on the supplied frame number fn.

  TC = (fn div 51) mod 8.  See 5.02 section 6.3.4.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint8 l1_compute_tc( uint32 fn, boolean extbcch )
{
   uint8 tc;

   fn /= 51;
   tc  = fn % 8;

   /* for GRR's benefit, tag this TC value with an indicator */
   /* that the read was performed on Ext-BCCH if appropriate */
   if (extbcch)
   {
     tc |= EXTENDED_BCCH_TC_MASK;
   }

   return tc;
}

/*===========================================================================

FUNCTION  l1_get_agc_ptr

DESCRIPTION
  Searches through l1_Frequency_List and returns a pointer to the AGC
  struct associated with the given ARFCN.

DEPENDENCIES
  l1_Frequency_list must have been populated by doing a power scan.

RETURN VALUE
  Pointer to agc struct if ARFCN found, NULL otherwise.

SIDE EFFECTS
  None

===========================================================================*/
sys_algo_agc_T *l1_get_agc_ptr( ARFCN_T ARFCN, gas_id_t gas_id )
{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];

   uint32  i=0;

   while ( !ARFCNS_EQUAL(l1_Frequency_List->ARFCN[i], ARFCN) &&
           (i < l1_Frequency_List->length) &&
           ( i < ( ( sizeof( l1_Frequency_List->ARFCN ) / sizeof( l1_Frequency_List->ARFCN[0] ) ) - 1 ) )
         )
   {
      i++;
   }

   if (ARFCNS_EQUAL(l1_Frequency_List->ARFCN[i], ARFCN))
   {
      return &l1_Frequency_List->agc[i];
   }
   else
   {
      //MSG_ERROR("ARFCN not found.", 0,0,0);
      return NULL;
   }
}

/*===========================================================================
FUNCTION  l1_compute_P0

DESCRIPTION
  Calculate the value of P0 based on the supplied bit field.

  P0 = in * 2

DEPENDENCIES
  None

RETURN VALUE
  Pr value in dBx16

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T l1_compute_P0 (uint8 P0_bits)
{
  return ((P0_bits * 2) << 4);
}


/*===========================================================================
FUNCTION  l1_compute_Pb

DESCRIPTION
  Calculate the value of Pb based on the supplied bit field.

  Pb = in * 2

DEPENDENCIES
  None

RETURN VALUE
  Pr value in dBx16

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T l1_compute_Pb (uint8 Pb_bits)
{
  return ((Pb_bits * 2) << 4);
}


/*===========================================================================
FUNCTION  l1_compute_Pr

DESCRIPTION
  Calculate the value of Pr based on the supplied bit field.
  00 = 0-2 dB  = 1 dB
  01 = 4-6 dB  = 5 dB
  10 = 8-10 dB = 9 dB
  11 = no power control

DEPENDENCIES
  Pr value in dBx16

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
dBx16_T l1_compute_Pr (uint8 Pr_bits)
{
  switch (Pr_bits)
  {
    case 0: return (1 << 4);
    case 1: return (5 << 4);
    case 2: return (9 << 4);
    case 3:
    default:
      return 0;
  }
}

/*===========================================================================
FUNCTION  gl1_store_handover_details()
===========================================================================*/
void gl1_store_handover_details( mph_handover_req_T *msg_ptr )
{
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[msg_ptr->gas_id];

    /* Used for blind acquisition */
    l1_dedicated_data_ptr->ho_details.handover_type
                      = msg_ptr->handover_type;
    l1_dedicated_data_ptr->ho_details.BCCH_ARFCN
                      = msg_ptr->BCCH_ARFCN;
    l1_dedicated_data_ptr->ho_details.BSIC
        = (byte)( msg_ptr->BSIC.PLMN_colour_code<<3 )
        +   msg_ptr->BSIC.BS_colour_code;

    /* Used for TA and QBIT_LAG calculation */
    l1_dedicated_data_ptr->ho_details.handover_synchronisation
                      = msg_ptr->handover_synchronisation;
    l1_dedicated_data_ptr->ho_details.timing_advance_valid
                      = msg_ptr->timing_advance_valid;
    if( msg_ptr->timing_advance_valid )
    {
        l1_dedicated_data_ptr->ho_details.timing_advance
                          = msg_ptr->timing_advance;
    }
    else
    {
        l1_dedicated_data_ptr->ho_details.timing_advance = 0;
    }
    l1_dedicated_data_ptr->ho_details.real_time_difference_valid
                      = msg_ptr->real_time_difference_valid;
    if( msg_ptr->real_time_difference_valid )
    {
        l1_dedicated_data_ptr->ho_details.real_time_difference
                          = msg_ptr->real_time_difference;
    }
    else
    {
        l1_dedicated_data_ptr->ho_details.real_time_difference = 0;
    }
    l1_dedicated_data_ptr->ho_details.NCI = msg_ptr->NCI;

    /* Used for vocoder blanking */
    l1_dedicated_data_ptr->ho_details.old_channel_mode
                      = msg_ptr->old_channel_mode;
    l1_dedicated_data_ptr->ho_details.new_channel_mode
                      = msg_ptr->new_channel_mode;
}


/*===========================================================================
FUNCTION  gl1_calc_ho_timing_advance()
===========================================================================*/
boolean gl1_calc_ho_timing_advance (   l1_sc_bs_T *target_cell_details, byte *calculated_timing_advance, gas_id_t gas_id )
{
    boolean handover_is_possible = TRUE;
    byte    ho_ta = 0;
    int32   real_qbit_lag;   /* WORKING VARIABLE */
    int16   otd;
    int16   current_cells_qbit_offset;
    int16   handover_ta;
    dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

    current_cells_qbit_offset = (int16)( (8 - l1_get_current_timeslot(gas_id)) * 625 );

    otd = ( target_cell_details->qbit_lag - current_cells_qbit_offset );

    /* It is possible, theoretically, to obtain a otd value of -9999(if ncell is
    reported at -4999 and we are on TS0). Therfore need to account for this.
    We are going to use otd to calculate TA which is always positive and further
    down this function this value is cast to a word - therefore it needs to be +ve.
    */
    while ( otd < 0 )
    {
      otd += 5000;
    }


    /* Determine type of handover andtake action accordingly. */
    switch ( l1_dedicated_data_ptr->ho_details.handover_synchronisation )
    {
    /* ==================== */
    case NON_SYNCHRONISED:
    /* ==================== */
    {   /* For non-synchronized handover, BSIC must be known and time
         ** synchronization is needed.
         */
        if ( l1_dedicated_data_ptr->ho_details.timing_advance_valid )
        {
          ho_ta = l1_dedicated_data_ptr->ho_details.timing_advance;
        }
        else
        {
          ho_ta = 0;
        }
        break;
    }
    /* ==================== */
    case SYNCHRONISED:
    /* ==================== */
    {
        /* See 05.10 section A.1.13 */
        if ( l1_dedicated_data_ptr->ho_details.timing_advance_valid )
        {
            ho_ta = l1_dedicated_data_ptr->ho_details.timing_advance;
        }
        else
        {   /* TA not known we have to calculate it */

           real_qbit_lag = (word) otd;

           if ( real_qbit_lag > 4999 )
           {
              real_qbit_lag -= 5000;
              handover_ta = (int16) ((real_qbit_lag/2 + l1_dedicated_data_ptr->timing_advance_in_use)%256);

           }
           /* This computation is a re-arrangement of the formula given in 45.010
            * of OTD = RTD +t1 - t0.
            */
           else if ( real_qbit_lag > 1250 )
           {
              handover_ta = (int16) ((l1_dedicated_data_ptr->timing_advance_in_use
                              - (5000 - real_qbit_lag)/2)%256);
           }
           else
           {
              handover_ta = (int16) ((real_qbit_lag/2 + l1_dedicated_data_ptr->timing_advance_in_use)%256);
           }

           /* handover_ta is signed, so check for negative is needed */
           if ( handover_ta > 229  || handover_ta < 0  )
           {
              /* This is a negative timing advance - GSM 05.10 section A.3.3 */
              handover_ta = 0;
           }
           else if ( handover_ta >  63 )
           {
              /* Timing advance is too big - for NCI see GSM 04.08 table 10.55*/
              if ( l1_dedicated_data_ptr->ho_details.NCI == 0 )
              {
                 /* Ignore calculated value - use default */
                 handover_ta = 63;
              }
              else
              {
                 /* Abort handover */
                 handover_is_possible = FALSE;
              }
           }

           ho_ta = (byte) handover_ta;
        }
        break;
    }
    /* ==================== */
    case PRE_SYNCHRONISED:
    /* ==================== */
    {
        /* If the timing advance is not valid, use the default of 1 as specified
        ** by the standards. */
        if ( l1_dedicated_data_ptr->ho_details.timing_advance_valid )
        {
            ho_ta = l1_dedicated_data_ptr->ho_details.timing_advance;
        }
        else
        {
            ho_ta = 1;
        }

        /* correct qbit lag for timeslot number */
        real_qbit_lag = l1_dedicated_data_ptr->target_cell_details.qbit_lag
                        + l1_get_current_timeslot(gas_id)*625;

        if ( l1_dedicated_data_ptr->target_cell_details.qbit_lag < 0 )
        {
            real_qbit_lag += 5000;
        }
        if ( real_qbit_lag > 4999 )
        {
            real_qbit_lag -= 5000;
        }
        break;
    }
    /* ==================== */
    case PSEUDO_SYNCHRONISED:
    /* ==================== */
    {
        /* If the timing advance is not valid, use the default of 1 as specified
        ** by the standards.
        */
        if ( l1_dedicated_data_ptr->ho_details.timing_advance_valid )
        {
            ho_ta = l1_dedicated_data_ptr->ho_details.timing_advance;
        }
        else
        {   /* TA unknown - need to calculate */

            /* correct for timeslot number */
            real_qbit_lag = l1_dedicated_data_ptr->target_cell_details.qbit_lag
                            + l1_get_current_timeslot(gas_id)*625;

            if ( l1_dedicated_data_ptr->target_cell_details.qbit_lag < 0 )
            {
                real_qbit_lag += 5000;
            }
            if ( real_qbit_lag > 4999 )
            {
                real_qbit_lag -= 5000;
                ho_ta = (byte)((real_qbit_lag/2
                         + l1_dedicated_data_ptr->timing_advance_in_use)%256);
            }
            else if ( real_qbit_lag > 2500 )
            {   /* What is this computation ?? */
                ho_ta = (byte)((l1_dedicated_data_ptr->timing_advance_in_use
                         - (5000 - real_qbit_lag)/2)%256);
            }
            else
            {
                ho_ta = (byte)((real_qbit_lag/2
                         + l1_dedicated_data_ptr->timing_advance_in_use)%256);
            }

            /* Use the RTD provided by the BTS for pseudo-synch handovers */
            if ( l1_dedicated_data_ptr->ho_details.real_time_difference_valid )
            {
                ho_ta -= l1_dedicated_data_ptr->ho_details.real_time_difference;
            }
            /* ho_ta is unsigned, so check for negative is not needed */
            if ( ho_ta > 229 /* || ho_ta < 0 */ )
            {   /* This is a negative time advance - GSM 05.10 section A.3.3 */
                ho_ta = 0;
            }
            else if ( ho_ta >  63 )
            {   /* Time advance too big - for NCI see GSM 04.08 table 10.55*/
                if ( l1_dedicated_data_ptr->ho_details.NCI == 0 )
                {   /* Ignore calculated value - use default */
                    ho_ta = 63;
                }
                else
                {  /* Abort handover */
                    handover_is_possible = FALSE;
                }
            }
        }
        break;
    }
    /* ==================== */
    default:
    /* ==================== */
        MSG_GERAN_ERROR_0_G("Bad Handover Type ");
        handover_is_possible = FALSE;
    } /* END SWITCH */

    *calculated_timing_advance = ho_ta;
    return handover_is_possible;

}

/*===========================================================================

FUNCTION  L1_PRINT_HEX

DESCRIPTION
  This function prints a line of data in hex

DEPENDENCIES
  *byte    source pointer
   byte     count - no of bytes to print

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef GL1_DEBUG
#error code not present
#endif

/*===========================================================================

FUNCTION  gl1_get_band

DESCRIPTION
  Determines what band the specified ARFCN is in

DEPENDENCIES
===========================================================================*/
sys_band_T gl1_get_band( ARFCN_T arfcn )
{
  return arfcn.band;
}

/*===========================================================================

FUNCTION  l1_snr_msg

DESCRIPTION
  Display "F3" debug message containing SNR information in dB.

===========================================================================*/
void l1_snr_msg( dBx16_T *snrs, uint32 num, gas_id_t gas_id )
{
   dBx16_T snrs_msg[4];
   uint32 i;

   for (i = 0; i < 4; i++)
   {
      snrs_msg[i] = ( snrs[i] / 16 );
   }

   MSG_GERAN_MED_5_G( "SNRs (dB, %d bursts): %d %d %d %d",
                      num, snrs_msg[0], snrs_msg[1],
                      snrs_msg[2], snrs_msg[3] );
}


/*===========================================================================

FUNCTION  gl1_get_current_ded_chan_type

DESCRIPTION
  Determines what the current dedicated channel type is.

DEPENDENCIES
===========================================================================*/
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
channel_mode_T gl1_get_current_ded_chan_type( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  return ( l1_dedicated_data_ptr->channel_specification.channel_mode_1 );
  /* provide channel type for use by CSD stack:
   *  one of: SPEECH_V1, SPEECH_V2, SPEECH_V3, SIGNALLING_ONLY,
   *          DATA_9600, DATA_14400.
   */
}
#else
channel_mode_T gl1_get_current_ded_chan_type(void)
{
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  return ( l1_dedicated_data_ptr->channel_specification.channel_mode_1 );
  /* provide channel type for use by CSD stack:
   *  one of: SPEECH_V1, SPEECH_V2, SPEECH_V3, SIGNALLING_ONLY,
   *          DATA_9600, DATA_14400.
   */
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION  gl1_compute_BCCH_mask

DESCRIPTION
  Convert SI mask received from RR into mask to use for selecting TC values.
  The mask from RR is defined in rr_l1.h.  The mask for selecting TC values
  works like this:  if (mask & (1<<TC) != 0) then this block is received.

DEPENDENCIES
  None

===========================================================================*/
uint32 gl1_compute_BCCH_mask( rr_l1_si_type_T si_mask )
{
   uint32 tc_mask = 0;

   if ( si_mask == RR_L1_SI_TYPE_ALL_NORM )
   {
      tc_mask = SERV_BCCH_MASK_ALL;
   }
   else
   {
      /*lint --e{655} bit-wise op on enum */
      if ( (si_mask & RR_L1_SI_TYPE_1) != 0 )    /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_1;
      }

      if ( (si_mask & RR_L1_SI_TYPE_2) != 0 )    /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_2;
      }

      if ( (si_mask & RR_L1_SI_TYPE_2_bis) != 0 )  /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_2BIS;
      }

      if ( (si_mask & RR_L1_SI_TYPE_2_ter) != 0 )  /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_2TER;
      }

      if ( (si_mask & RR_L1_SI_TYPE_2_quater_NORM) != 0 )  /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_2QUATER;
      }

      if ( (si_mask & RR_L1_SI_TYPE_3) != 0 )   /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_3;
      }

      if ( (si_mask & RR_L1_SI_TYPE_4) != 0 )  /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_4;
      }

      if ( (si_mask & RR_L1_SI_TYPE_9) != 0 )  /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_9;
      }

      if ( (si_mask & RR_L1_SI_TYPE_13_NORM) != 0 )   /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_13;
      }

      if ( (si_mask & RR_L1_SI_TYPE_13_alt_NORM) != 0 )   /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_13ALT;
      }

      if ( (si_mask & RR_L1_SI_TYPE_15_NORM) != 0 )    /*lint !e641 */
      {
         tc_mask |= SERV_BCCH_MASK_15;
      }
   }

   return tc_mask;
}

/*===========================================================================

FUNCTION  gl1_compute_ext_BCCH_mask

DESCRIPTION
  Convert SI mask received from RR into mask to use for selecting TC values.
  The mask from RR is defined in rr_l1.h.  The mask for selecting TC values
  works like this:  if (mask & (1<<TC) != 0) then this block is received.

DEPENDENCIES
  None

===========================================================================*/
uint32 gl1_compute_ext_BCCH_mask( rr_l1_si_type_T si_mask )
{
   uint32 tc_mask = 0;

   if ( si_mask == RR_L1_SI_TYPE_ALL_EXT )
   {
      tc_mask = SERV_EXT_BCCH_MASK_ALL;
   }
   else
   {
      /*lint --e{655} bit-wise op on enum */
      if ( (si_mask & RR_L1_SI_TYPE_2_quater_EXT) != 0 ) /*lint !e641 */
      {
         tc_mask |= SERV_EXT_BCCH_MASK_2QUATER;
      }

      if ( (si_mask & RR_L1_SI_TYPE_7) != 0 )   /*lint !e641 */
      {
         tc_mask |= SERV_EXT_BCCH_MASK_7;
      }

      if ( (si_mask & RR_L1_SI_TYPE_8) != 0 )  /*lint !e641 */
      {
         tc_mask |= SERV_EXT_BCCH_MASK_8;
      }

      if ( (si_mask & RR_L1_SI_TYPE_13_EXT) != 0 ) /*lint !e641 */
      {
         tc_mask |= SERV_EXT_BCCH_MASK_13;
      }

      if ( (si_mask & RR_L1_SI_TYPE_13_alt_EXT) != 0 ) /*lint !e641 */
      {
         tc_mask |= SERV_EXT_BCCH_MASK_13ALT;
      }

      if ( (si_mask & RR_L1_SI_TYPE_15_EXT) != 0 )   /*lint !e641 */
      {
         tc_mask |= SERV_EXT_BCCH_MASK_15;
      }

      if ( (si_mask & RR_L1_SI_TYPE_16) != 0 )   /*lint !e641 */
      {
         tc_mask |= SERV_EXT_BCCH_MASK_16;
      }

      if ( (si_mask & RR_L1_SI_TYPE_17) != 0 )   /*lint !e641 */
      {
         tc_mask |= SERV_EXT_BCCH_MASK_17;
      }
   }

   return tc_mask;
}

boolean gl1_cell_valid_state_check(const byte gl1_state)
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  if (gl1_state >= (byte)L1_MULTI_SIM_NULL)
  {
    /* before moving to RACH, GL1 moves into L1_MULTI_SIM_DECOUPLED state from idle state */
    return((boolean)( (gl1_state == (byte)L1_MULTI_SIM_IDLE) || (gl1_state == (byte)L1_MULTI_SIM_DECOUPLED) ));
  }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  return((boolean)(gl1_state >= (byte)L1_IDLE_MODE));
}

/*===========================================================================

FUNCTION  gl1_timetag_c/gps

DESCRIPTION
  This function schedules a timetag message to be sent at the next tick.

===========================================================================*/
void gl1_timetag_cgps( byte l1_state, cgps_CellUpdateEnumType update_type, gas_id_t gas_id )
{
  cgps_GsmCellTimingStructType *msg;

  boolean                quick_return      = FALSE;
  l1_utils_data_t       *l1_utils_data_ptr = &l1_utils_data[gas_id];

  /* Check that if we are sending a leave notification, we actually
     sent an enter one at some point.  A failed WTOG handover would
     result in this for example, as would downlink signalling failure
     followed by switching to WCDMA (one for STOP_CAMPING and then one
     for the STOP_GSM request). */
  switch ( update_type )
  {
    case CGPS_TIMETAG_PRE_CELL_CHANGE:
    {
      if (!l1_utils_data_ptr->in_cell)
      {
        MSG_GERAN_MED_1_G( "Skipping time-tag pre-cell-change.",
                 0);

        quick_return = TRUE;
      }
      else
      {
        l1_utils_data_ptr->in_cell = FALSE;
      }
    }
    break;

    case CGPS_TIMETAG_POST_CELL_CHANGE:
    {
      l1_utils_data_ptr->in_cell = TRUE;
    }
    break;

    case CGPS_TIMETAG_REQUEST:
    {
      /* Flag a pending request so the ISR will process it next tick */
      l1_utils_data_ptr->timetag_pending_req = TRUE;
      quick_return = TRUE;
    }
    break;

    default:
    {
      MSG_GERAN_ERROR_1_G( "Unsupported time-tag update type %d", update_type);
      quick_return = TRUE;
    }
    break;
  }

  if ( !quick_return )
  {
    /* Lock interrupts since timetag_pending can be modified by the ISR */
    GL1_ISR_SAVE_LOCK(gas_id);

    /* Check that nothing strange is going on */
    if (l1_utils_data_ptr->timetag_pending_msg >= MAX_PENDING_GSM_TIMETAG_MSGS)
    {
      MSG_GERAN_ERROR_1_G( "Too many pending timetag notifications (type=%d)!",
                 update_type);

      GL1_ISR_SAVE_UNLOCK(gas_id);

      return;
    }

    /* Get the next message */
    msg = &l1_utils_data_ptr->timetag_msg[l1_utils_data_ptr->timetag_pending_msg++];

    /* Fill in the l1_state */
    msg->l1_state    = l1_state;

    /* Determine if we are camped on a valid cell */
    msg->cell_valid  = gl1_cell_valid_state_check(l1_state);

    /* Fill in the Update type */
    msg->update_type = update_type;

    /*
     * The filled in data at this point is later checked for
     * validity when we perform the timetagging and if the
     * cell info has changed we will then mark the cell as
     * invalid prior to sending to GPS.
     * This avoids the problem of GL1 marking the cell as invalid
     * when we send as we have moved on to FIND_BCCH state.
     * This causes GPS problems as they never fill-in there
     * valid cell database correctly.
     */
    if ( msg->cell_valid )
    {
      msg->cell.cell_info.arfcn = gl1_get_serving_cell_arfcn(gas_id);
      msg->cell.cell_info.bsic  = gl1_get_serving_cell_BSIC(gas_id);
    }

    /*
     * For Pre-Post cell change use the pending_msg count for TT
     * trigger and clear the pending_req as otherwise double TT
     * sends can occur with the same GL1 data
     */
    l1_utils_data_ptr->timetag_pending_req = FALSE;

    GL1_ISR_SAVE_UNLOCK(gas_id);
  }

  return;
}

/*===========================================================================

FUNCTION  gl1_timetag_c/gps_tag_and_send

DESCRIPTION
  This function sets the time in the timetag msg and sends it to CGPS/LSM.
  Should be called in ISR mode at the start of the ISR to ensure that
  the FN does not rollover during processing, or during a DRX sleep
  cycle if the sleep clock source is being used.

===========================================================================*/
static
boolean gl1_timetag_cgps_tag_and_send( cgps_GsmCellTimingStructType *msg,
                                       gl1_timetag_source_type       source,
                                       gas_id_t gas_id )
{
   sys_modem_as_id_e_type subs_id = geran_map_gas_id_to_nas_id( gas_id );
   gsmtr_fn_qs_type fn_qs;

   /*CgC removed GL1_ISR_LOCK(gas_id); for GPS mutex changes */

   /* Fill in rest of the message if the cell is valid */
   if ( msg->cell_valid )
   {
      /* Do not change the order in which times are latched. First latch the
         network time and then the GPS time. This is needed to consistent timing.
         lsm_gl1_get_gps_timer function may take a few extra cycles if we are doing a
         post-cell-change update. If network time is latched after GPS, it would
         not be a predictable delay always.
      */

      /*
       * Cache the FN and qs before deterministic latch loop
       * to avoid using the sleep FN if G Sleep occurs during latch
       * and to allow detection of a frame miss
       */
      /* For time-tag running in Asynch Int or sleep_state is ASLEEP don't perform direct GFN read, as it may
         already have been updated beyond current real FN*/
      if( (source == TIMETAG_SOURCE_GSTMR) && (gl1_hw_get_sleep_state(gas_id) != GL1_HW_SLEEP_ASLEEP) 
        && !gstmr_get_asynch_int_executing(gas_id))
      {
        /* active timetag - use vstmr FN/QS */

        fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);
        msg->cell.cell_time.fn = fn_qs.fn;
        msg->cell.cell_time.q_symbol_count = fn_qs.qs;
        msg->sleep_timetag = FALSE;
      }
      else
      {
        /* sleep timetag - use sclk to derive FN/QS */
        (void) gl1_hw_calc_gsm_time_from_sclk(&msg->cell.cell_time.fn,
          &msg->cell.cell_time.q_symbol_count,
          gas_id);
        msg->sleep_timetag = TRUE;
      }
      msg->cell.cell_time.time_slot      = l1_get_current_timeslot( gas_id );

      /* Get the GPS Hardware timer value */
      cgps_GetRefInfo( &msg->ref_info );

      switch ( msg->update_type )
      {
        case CGPS_TIMETAG_PRE_CELL_CHANGE:
        case CGPS_TIMETAG_POST_CELL_CHANGE:
        case CGPS_TIMETAG_REQUEST:
        {
          /*
           * For a delayed tag update ensure these are still consistent
           * as if this data refers to the old cell and the tag info
           * is updated with info from the new cell then the timetag
           * info is very different from what is should be during an
           * intercell HO leading to a loss of GPS lock.
           */
          ARFCN_T local_serving_cell_arfcn = gl1_get_serving_cell_arfcn(gas_id);

          if ( !ARFCNS_EQUAL( msg->cell.cell_info.arfcn, local_serving_cell_arfcn ) ||
               ( msg->cell.cell_info.bsic != gl1_get_serving_cell_BSIC(gas_id) ) )
          {
            msg->cell_valid = FALSE;
          }
        }
        break;

        /* Should never get here but just incase mark as invalid */
        default:
          msg->cell_valid = FALSE;
        break;
      }  /* switch ( msg->update_type ) */

      MSG_GERAN_MED_5_G( "G_TimeTag: FN: %d, QS: %d, TS: %d, SleepTT: %d, Sclk: 0x%x",
         msg->cell.cell_time.fn,
         msg->cell.cell_time.q_symbol_count,
         msg->cell.cell_time.time_slot,
         msg->sleep_timetag,
         (uint32) timetick_get_safe() );

   }     /* if ( msg->cell_valid ) */

   MSG_GERAN_LOW_3_G( "Sending TimeTag to CGPS (update_type=%d arfcn=%d state=%d)",
            msg->update_type, msg->cell.cell_info.arfcn.num, msg->l1_state );

   /*
    * Send the message to CGPS and uses the return from above ideally to
    * indicate delayed request
    */
   msg->subs_id = subs_id;

   return cgps_GsmCellInfoUpdate( msg );
}

/*===========================================================================

FUNCTION  gl1_timetag_gps_tick

DESCRIPTION
  This function processes any scheduled timetag messages and sends them.

===========================================================================*/
void gl1_timetag_gps_tick( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_utils_data_t *l1_utils_data_ptr = &l1_utils_data[gas_id];
  uint16 i;

  /* Send any pending pre or post cell changes */
  for ( i = 0; i < l1_utils_data_ptr->timetag_pending_msg; i++ )
  {
    (void)gl1_timetag_cgps_tag_and_send( &l1_utils_data_ptr->timetag_msg[i], TIMETAG_SOURCE_GSTMR, gas_id );
  }
  l1_utils_data_ptr->timetag_pending_msg = 0;

  /* Process any pending requests if the L1 state is stable */
  if ( l1_utils_data_ptr->timetag_pending_req && ( l1_tskisr_blk->main_command == L1_NULL_COMMAND ) )
  {
    cgps_GsmCellTimingStructType msg = { 0 };

    /*
     * Fill in the cell_valid and general cell info at this point
     * when we actually tag and send the data we compare with the current
     * camped cell details at that point. If this doesn't match we mark
     * the cell as invalid.
     */
    msg.l1_state   = l1_tskisr_blk->l1_state;
    msg.cell_valid = gl1_cell_valid_state_check(l1_tskisr_blk->l1_state);

    if ( msg.cell_valid )
    {
      msg.cell.cell_info.arfcn = gl1_get_serving_cell_arfcn(gas_id);
      msg.cell.cell_info.bsic  = gl1_get_serving_cell_BSIC(gas_id);
    }

    /* Tag and send the pending request */
    msg.update_type = CGPS_TIMETAG_REQUEST;

    if ( gl1_timetag_cgps_tag_and_send( &msg, TIMETAG_SOURCE_GSTMR,gas_id ) )
    {
      l1_utils_data_ptr->timetag_pending_req = FALSE;
    }
  }
}

/*===========================================================================

FUNCTION  gl1_timetag_gps_asleep

DESCRIPTION
  This function sends a timetag request response using timing from the
  sleep clock

===========================================================================*/
void gl1_timetag_cgps_asleep( byte l1_state, gas_id_t gas_id )
{
   l1_utils_data_t *l1_utils_data_ptr;
   cgps_GsmCellTimingStructType msg = { 0 };

   msg.update_type = CGPS_TIMETAG_REQUEST;
   l1_utils_data_ptr = &l1_utils_data[gas_id];
   /*
    * Fill in the cell_valid and general cell info at this point
    * when we actually tag and send the data we compare with the current
    * camped cell details at that point. If this doesn't match we mark
    * the cell as invalid.
    */
   msg.l1_state   = l1_state;
   msg.cell_valid = gl1_cell_valid_state_check(l1_state);

   if ( msg.cell_valid )
   {
     msg.cell.cell_info.arfcn = gl1_get_serving_cell_arfcn(gas_id);
     msg.cell.cell_info.bsic  = gl1_get_serving_cell_BSIC(gas_id);
   }

   /* Tag and send the pending request, if it fails, just mark as
      pending to try again when we wake up. */
   if ( !gl1_timetag_cgps_tag_and_send( &msg, TIMETAG_SOURCE_SLEEPCLK,gas_id ) )
   {
     l1_utils_data_ptr->timetag_pending_req = TRUE;
   }
}

/*===========================================================================

FUNCTION  gl1_post_cgps_time_tag_msg

DESCRIPTION
  This function posts the internal GL1_CGPS_MSG_TIMETAG_REQUEST message to
  the GSM Layer 1 task queue. The GAS ID parameter is for Dual SIM use

===========================================================================*/
static void gl1_post_cgps_time_tag_msg(boolean allow_sleep_tag, gas_id_t gas_id)
{
  gl1_cgps_timetag_type z_Msg, *p_Msg = &z_Msg;
  uint32      msg_len;

  memset( &z_Msg, 0, sizeof(z_Msg) );

  /* Construct message */
  p_Msg->message_header.message_set = MS_CGPS_GL1;
  p_Msg->message_header.message_id  = GL1_CGPS_MSG_TIMETAG_REQUEST;

  msg_len = ( sizeof(gl1_cgps_timetag_type) - sizeof(IMH_T) );

  PUT_IMH_LEN( msg_len, p_Msg );

  /* Fill in the payload */
  p_Msg->allow_sleep_timetag = allow_sleep_tag;

  p_Msg->gas_id = gas_id;

  MSG_GERAN_MED_1_G( "GSM Time-Tag REQUEST with allow_sleep %d",
                     allow_sleep_tag );

  /* Send message to GSM/GPRS L1 */
  if (GS_SUCCESS != gs_send_message(l1_queues[gas_id], p_Msg, TRUE))
  {
    MSG_GERAN_ERROR_0_G( "Unable to send CGPS Time-Tag request to GL1" );
  }

  return;
}

/*===========================================================================

FUNCTION  gl1_TimeTagRequest

DESCRIPTION
  This function allows the CGPS task to request GL1 to perform a sleep timetag
  which can be forced to trigger a sleep tag request for rude wakeup.

===========================================================================*/
void gl1_TimeTagRequest(boolean allow_sleep_timetag)
{
  gl1_post_cgps_time_tag_msg(allow_sleep_timetag, GERAN_ACCESS_STRATUM_ID_1);
}

void gl1_ds_TimeTagRequest(boolean allow_sleep_timetag, sys_modem_as_id_e_type as_id)
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  uint8 gas_id = (uint8)geran_map_nas_id_to_gas_id(as_id);

  gl1_post_cgps_time_tag_msg(allow_sleep_timetag, gas_id);
#else
  MSG_GERAN_ERROR_0_G( "gl1_ds_TimeTagRequest() called but FEATURE_DUAL_SIM not def" );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
}

/*===========================================================================
FUNCTION gl1_hw_gts_timer_init

DESCRIPTION
  creates/defines one sec timer for updating GPS module with G timing info 
===========================================================================*/
void gl1_gts_timer_init(gas_id_t gas_id )
{
    /* Create a timer for sending periodic timining info to GPS  */
    memset(&(gl1_gts_timer_counter[gas_id]), 0, sizeof(rex_timer_type));
    rex_def_timer_ex( &gl1_gts_timer_counter[gas_id],
                    (rex_timer_cb_type)gl1_gts_time_update_cb,
                    (gas_id_t)gas_id );
    gl1_gts_timer_running[gas_id] = FALSE;
}

/*===========================================================================
FUNCTION gl1_hw_gts_timer_deactivate

DESCRIPTION
  Deactivates the GTS one sec timer
===========================================================================*/
void gl1_gts_timer_deactivate( gas_id_t gas_id)
{
  if(gl1_gts_timer_running[gas_id] == TRUE)
 {
  /* Disable the timer */
    (void)rex_clr_timer( &gl1_gts_timer_counter[gas_id] );
  gl1_gts_timer_running[gas_id] = FALSE;
#ifdef GL1_GTS_DEBUG
    gl1_gts_timer_stopped[gas_id] = timetick_get_safe();
    MSG_GERAN_HIGH_1_G("GTS Timer stopped at %d",gl1_gts_timer_stopped[gas_id]);
#endif /* GL1_GTS_DEBUG */
 }
}

/*===========================================================================
FUNCTION gl1_hw_gts_timer_start

DESCRIPTION
  Starts a timer for the given duration for a specific GAS ID
  To be called after ISR lock has been done.
===========================================================================*/
static void gl1_gts_timer_start(uint32 gts_timer_timeout, gas_id_t gas_id )
{
  (void)rex_set_timer( &gl1_gts_timer_counter[gas_id],
                        gts_timer_timeout );
  gl1_gts_timer_running[gas_id] = TRUE;
#ifdef GL1_GTS_DEBUG
  gl1_gts_timer_started[gas_id] = timetick_get_safe();
  MSG_GERAN_HIGH_2_G(" GTS Timer started at %d, count for %d ms",gl1_gts_timer_started[gas_id], gts_timer_timeout);
#endif /* GL1_GTS_DEBUG */
}

/*===========================================================================
FUNCTION gl1_gts_time_update_cb

DESCRIPTION
This function called on Timer expiry. 
It shall call gl1_gts_send_timing_info() to sends GL1 timetag info to 
GPS module

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_gts_time_update_cb( gas_id_t gas_id )
{
  GL1_ISR_LOCK(gas_id);
  /* CR2006922 To avoid race condition while state change.
     If state is DED/DTM/Tranfer mode then only send timing info and start the timer */
  if(gl1_gts_chk_state_to_send(gas_id))
  {
  /* Fill GTS info and send to GPS */
  MSG_GERAN_LOW_0_G("GTS_DBG: Sending GTS from timer");
  gl1_gts_send_timing_info(gas_id);

  /* Start the GTS Timer now */
  gl1_gts_timer_start(GL1_GTS_UPDATE_TIMEOUT, gas_id);
  }
  GL1_ISR_UNLOCK(gas_id);   
}

/*===========================================================================
FUNCTION gl1_gts_send_gts_after_wakeup

DESCRIPTION
Send Gl1 timining information to GPS module in Idle mode

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_gts_send_gts_after_wakeup( gas_id_t gas_id )
{  

  MSG_GERAN_MED_0_G("GTS_DBG: Sending GTS after wkup");

  GL1_ISR_LOCK(gas_id);

  gl1_gts_send_timing_info(gas_id);  

  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================
FUNCTION gl1_gts_send_timing_info

DESCRIPTION
Send Gl1 timining information to GPS module

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gl1_gts_send_timing_info( gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gts_GsmTimeInfoType gts_msg = { 0 };  
  sys_modem_as_id_e_type subs_id = geran_map_gas_id_to_nas_id( gas_id );
  gsmtr_fn_qs_type fn_qs;
  gts_QtimeType gts_qtime_info = { 0 };
  int16 drift_qs;
  drift_qs = vstmr_get_drift(gas_id);

  if(
    (drift_qs > DEF_QS_GPS_TOLERANCE) ||
    (drift_qs < -DEF_QS_GPS_TOLERANCE) )
  {
    /* Large Drift */
    MSG_GERAN_MED_0_G( "GTS_DBG: Cannot send GTS due to TS alloc change");
    return;
  }

  /* Short Drift will be between -16 and +16 (or 0), so typecasting to int8 is fine */
  gts_msg.b_DriftQSym  = (int8) drift_qs;

  /* Fill in general cell info, Gl1 state, Frame number, Time slot and qs info */
  gts_msg.u_SubInfo = subs_id;

  gts_msg.u_GL1State   =  (uint8) gl1_gts_set_l1_state( gas_id );    
  gts_msg.z_Arfcn      =  gl1_get_serving_cell_arfcn( gas_id );
  gts_msg.u_BSIC       =  gl1_get_serving_cell_BSIC( gas_id );

  /* Read the current Frame and Qsym count values */
  fn_qs = gstmr_rd_hw_fn_qs_geran(gas_id);

  gts_msg.q_QSymCnt = fn_qs.qs;
  gts_msg.q_FrameNum = fn_qs.fn;
  gts_msg.u_TimeSlot = l1_get_current_timeslot( gas_id );

  /* Get current Qtime */
  gts_GetQtime( &gts_qtime_info );

  /* QTime - Same as 64 bit USTMR Time */
  gts_msg.z_QtimeInfo.b_QtimeValid = gts_qtime_info.b_QtimeValid;
  gts_msg.z_QtimeInfo.t_Qtime = gts_qtime_info.t_Qtime;
  gts_msg.z_QtimeInfo.f_QtimeBias = gts_qtime_info.f_QtimeBias;

  MSG_GERAN_LOW_5_G( "GTS_DBG_1: FN: %d, QS: %d, TS: %d, l1_state: %d, Sclk: 0x%x",
    gts_msg.q_FrameNum,
    gts_msg.q_QSymCnt,
    gts_msg.u_TimeSlot,
    l1_tskisr_blk->l1_state,
    (uint32) timetick_get_safe());

  gts_GsmTimeUpdate( &gts_msg );

#ifdef GL1_GTS_DEBUG
  /* Debug GTS data */
  gl1_gts_debug_enable(&gts_msg, gas_id);
#endif /* GL1_GTS_DEBUG */     
}

#ifdef GL1_GTS_DEBUG
/*===========================================================================
FUNCTION gl1_gts_debug_enable

DESCRIPTION
This function is to print GTS Timetag detail sent to GPS module

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_gts_debug_enable(gts_GsmTimeInfoType *gts_msg, gas_id_t gas_id)
{
  static uint64 time_sclk_last[NUM_GERAN_DATA_SPACES];
  uint64 time_sclk_cur = timetick_get_safe();
  static gsmtr_fn_qs_type fn_qs_last[NUM_GERAN_DATA_SPACES];
  int32 time_diff_fn_qs_ms;
  int32 time_diff_sclk_ms;
  int32 diff;
  boolean large_diff;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  time_diff_fn_qs_ms = (SUB_FN(gts_msg->q_FrameNum, fn_qs_last[gas_id].fn)*5000  
   + 
  (gts_msg->q_QSymCnt - fn_qs_last[gas_id].qs)) * 120 / (26*5000);

  time_diff_sclk_ms = (( time_sclk_cur - time_sclk_last[gas_id])* 1000) >> 15 ;

  if( time_diff_fn_qs_ms > time_diff_sclk_ms)
  {
     diff = time_diff_fn_qs_ms - time_diff_sclk_ms;
  }
  else
  {
    diff = time_diff_sclk_ms - time_diff_fn_qs_ms;
  }

  if( (diff >= 3) /* && (diff <= 12) */ )
  {
    large_diff = TRUE;

    MSG_GERAN_HIGH_4_G( "GTS_DBG-2: fnqs_diff: %d, sclk_diff: %d, diff: %d, large_diff: 1, LARGE_DIFF, l1_state: %d",
     time_diff_fn_qs_ms, time_diff_sclk_ms, diff, l1_tskisr_blk->l1_state);
  }
  else
  {
    large_diff = FALSE;

    MSG_GERAN_HIGH_4_G( "GTS_DBG-2: fnqs_diff: %d, sclk_diff: %d, diff: %d, large_diff: 0, small_diff, l1_state: %d",
     time_diff_fn_qs_ms, time_diff_sclk_ms, diff, l1_tskisr_blk->l1_state);
  }

  MSG_GERAN_HIGH_4_G( "GTS_DBG-3: l1_state: %d, sclk_prev: 0x%x, sclk_now: 0x%x, fn_prev: %d",
   gts_msg->u_GL1State, (uint32) time_sclk_last[gas_id], (uint32) time_sclk_cur, fn_qs_last[gas_id].fn);

  time_sclk_last[gas_id] = time_sclk_cur;
  fn_qs_last[gas_id].fn = gts_msg->q_FrameNum;
  fn_qs_last[gas_id].qs = gts_msg->q_QSymCnt;
}
#endif /* GL1_GTS_DEBUG */

/*===========================================================================
FUNCTION gl1_gts_set_l1_state

DESCRIPTION
Set Gl1 state information 

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static gts_gl1_state_type gl1_gts_set_l1_state(gas_id_t gas_id)
{
   gts_gl1_state_type gts_gl1_state;
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   switch(l1_tskisr_blk->l1_state)
   {
     case L1_IDLE_MODE:
       gts_gl1_state        = GTS_GL1_STATE_IDLE;
       break;
     case L1_GPRS_IDLE_MODE:
       gts_gl1_state        = GTS_GL1_STATE_GPRS_IDLE;
       break;
     case L1_MULTI_SIM_IDLE:
       gts_gl1_state        = GTS_GL1_STATE_MULTI_SIM_IDLE;       
       break;
     case L1_DEDICATED_MODE:
       gts_gl1_state        = GTS_GL1_STATE_DEDICATED;
       break;
     case L1_TRANSFER_MODE:
       gts_gl1_state        = GTS_GL1_STATE_TRANSFER;
       break;
     case L1_DTM_MODE:      
       gts_gl1_state        = GTS_GL1_STATE_DTM;
       break;
     default:
       gts_gl1_state        = GTS_GL1_STATE_OTHER;
       break;
   }    
   return gts_gl1_state;
}

/*===========================================================================
FUNCTION gl1_gts_chk_state_to_send

DESCRIPTION
Check Gl1 state to send GTS information

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static boolean gl1_gts_chk_state_to_send(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  boolean run_gts_timer;

  switch(l1_tskisr_blk->l1_state)
  {
    case L1_DEDICATED_MODE:
    case L1_TRANSFER_MODE:
    case L1_DTM_MODE:
      run_gts_timer = TRUE;
      break;

    default:
      run_gts_timer = FALSE;
      break;
  }

  return run_gts_timer;
}

/*===========================================================================

FUNCTION  gl1_set_band_group

DESCRIPTION
  Informs GL1 of which band group we will be operating in.

===========================================================================*/
void gl1_set_band_group (sys_band_mask_type band_mask, gas_id_t gas_id)
{
   /* Nothing to do but tell NPL1 for now */
   gl1_msg_set_band_group(band_mask);
}

/*===========================================================================

FUNCTION      l1_frequency_list_mem_init

DESCRIPTION   Ensures that global memory for l1_Frequency_List is initialised

RETURN VALUE
  None

SIDE EFFECTS
  Stores allocated pointers in the data space array in this file

===========================================================================*/
void l1_frequency_list_mem_init(gas_id_t gas_id)
{
  int index;
  Frequency_List_T *local_ptr=NULL;

  /* determine which data space is active */
  /* validate the data space index */
   if ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ||
       (gas_id == GERAN_ACCESS_STRATUM_ID_2)
#ifdef FEATURE_TRIPLE_SIM
       ||(gas_id == GERAN_ACCESS_STRATUM_ID_3)
#endif /* FEATURE_TRIPLE_SIM */
   )
   {
    if (l1_Frequency_List_Store[gas_id] == NULL)
    {
      local_ptr = (Frequency_List_T *)GPRS_MEM_CALLOC(sizeof(Frequency_List_T));
      l1_Frequency_List_Store[gas_id] = local_ptr;
      if (local_ptr == NULL)
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_0_G("l1_frequency_list_mem_init() failure");
      }
    }
  }
  else
  {
    MSG_GERAN_ERROR_0_G("Unable to determine active GL1 data space");
  }
  /* initialise the pointers within the structure */
  if (local_ptr != NULL)
  {
    for (index = 0; index < RR_MAX_PSCAN_FREQ_SCAN; index++)
    {
      local_ptr->agc_ptrs[index] = &(local_ptr->agc[index]);
    }
  }

  return;
}

/*===========================================================================

FUNCTION     l1_frequency_list_mem_free

DESCRIPTION  Deallocates dynamic memory used for L1's frequency list storage

RETURN VALUE
  None

SIDE EFFECTS
  Intentionally has no effect if FEATURE_EOOS is defined, because this feature
  requires GL1 to maintain its power scan results even whilst it is offline.

===========================================================================*/
void l1_frequency_list_mem_free(gas_id_t gas_id)
{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];
  int index = (int )gas_id ;

  /* validate the data space index */
  /* determine which data space is active */
  /* validate the data space index */
  if ( gas_id == GERAN_ACCESS_STRATUM_ID_1
#ifdef FEATURE_DUAL_SIM
       || gas_id == GERAN_ACCESS_STRATUM_ID_2
#endif /* FEATURE_DUAL_SIM */
#ifdef FEATURE_TRIPLE_SIM
       || gas_id == GERAN_ACCESS_STRATUM_ID_3
#endif /* FEATURE_TRIPLE_SIM */
      )
  {
    /* dynamic data needs to be freed and the data space updated */
    if (l1_Frequency_List_Store[index] != NULL)
    {
      if (l1_Frequency_List == l1_Frequency_List_Store[index])
      {
        l1_Frequency_List = NULL;
      }
      GPRS_MEM_FREE(l1_Frequency_List_Store[index]);
      l1_Frequency_List_Store[index] = NULL;
    }
  }

  return;
}


/*===========================================================================

FUNCTION      gpl1_get_burst_num_from_FN_in_IDLE

DESCRIPTION   gets the burst_num from the frame number required for GL1 ARB in IDLE mode.

RETURN VALUE
  burst_num

// direction = 0 uplink
// direction = 1 downlink

SIDE EFFECTS

===========================================================================*/
uint8 gpl1_get_burst_num_from_FN_in_IDLE(uint32 FN,boolean direction,gas_id_t gas_id)
{
  idle_data_T               *idle_data_ptr;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint8 burst_num = 0; // burst is 1st one
  uint32 FNMOD = 0;

  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

  if (direction)
  {  // downlink
     FNMOD = FN % 51;
     if ( (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB  == FALSE) ||
          ((idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == TRUE) &&
           (FNMOD < 30 ))
        )
     {
        FNMOD = FNMOD % 10;
        switch (FNMOD)
        {
          case 3:
          case 7:
             burst_num  = 1;
             break;

          case 4:
          case 8:
             burst_num  = 2;
             break;

          case 5:
          case 9:
             burst_num  = 3;
             break;

          default:
             burst_num  = 0;
             break;
        }
     }  // BS_CCCH_SDCCH_COMB = TRUE
     else
     {
       burst_num = 0;  // for F,S more than FNMOD 30
     }
   }

   return burst_num;
}

/*===========================================================================

FUNCTION      gpl1_get_burst_num_from_FN

DESCRIPTION   gets the burst_num from the frame number required for GL1 ARB

RETURN VALUE
  burst_num

// direction = 0 uplink
// direction = 1 downlink

SIDE EFFECTS

===========================================================================*/
uint8 gpl1_get_burst_num_from_FN(uint32 FN,boolean direction,gas_id_t gas_id)
{
  idle_data_T               *idle_data_ptr;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  byte l1_state;
  uint8 burst_num = 0; // burst is 1st one
  uint32 FNMOD = 0;
  channel_type_T      physical_channel_type;

  idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  l1_state =  l1_tskisr_blk->l1_state;

  if (direction)
  {  // downlink
    switch (l1_state)
    {
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
      case L1_MULTI_SIM_IDLE :
#endif
      case L1_IDLE_MODE:
      case L1_GPRS_IDLE_MODE:
        FNMOD = FN % 51;
        if ( (idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB  == FALSE) ||
              ((idle_data_ptr->campedon_cell_data.BS_CCCH_SDCCH_COMB == TRUE) &&
               (FNMOD < 30 ))
           )
        {
          FNMOD = FNMOD % 10;
          switch (FNMOD)
          {
            case 3:
            case 7:
               burst_num  = 1;
               break;

            case 4:
            case 8:
               burst_num  = 2;
               break;

            case 5:
            case 9:
               burst_num  = 3;
               break;

            default:
               burst_num  = 0;
               break;
          }
        }  // BS_CCCH_SDCCH_COMB = TRUE
        else
        {
          burst_num = 0;  // for F,S more than FNMOD 30
        }
        break;

      case L1_DEDICATED_MODE :
        physical_channel_type = l1_dedicated_data_ptr->channel_specification.channel_info_1_before.channel_type;
        if (physical_channel_type == SDCCH_4 ||
            physical_channel_type == SDCCH_8  )
        {
          FNMOD = FN % 51;
          if (physical_channel_type == SDCCH_8) // 8 SDCCH/8
          {
            if ( FNMOD  < 48 )
            {
              burst_num = FNMOD % 4;
            }
            else
            {
              burst_num = 0;
            }
          }
          else   // SDCCH_4 begins from FNMOD % 51 = 30  CCCH+BCCH+ 4 SDCCH/4
          {
            FNMOD = FNMOD % 10;
            switch (FNMOD)
            {
              case 1:
              case 5:
                 burst_num  = 1;
                 break;

              case 2:
              case 6:
                 burst_num  = 2;
                 break;

              case 3:
              case 7:
                 burst_num  = 3;
                 break;

              default:
                 burst_num  = 0;
                 break;
            }
          }
          break;
        }
      // continue
      //case L1_DEDICATED_MODE :  continue for TCH which has similar strcutue to 52 frame MOD 13.
      case L1_TRANSFER_MODE :
        FNMOD = FN % 13;
        switch (FNMOD)
        {
          case 1:
          case 5:
          case 9:
             burst_num  = 1;
             break;

          case 2:
          case 6:
          case 10:
             burst_num  = 2;
             break;

          case 3:
          case 7:
          case 11:
             burst_num  = 3;
             break;

          default:
             burst_num  = 0;
             break;
        }
        break;

      default:
        break;
    }
  }
  else  // Uplink Direction
  {
    switch (l1_state)
    {
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
      case L1_MULTI_SIM_IDLE :
#endif
      case L1_IDLE_MODE:
      case L1_GPRS_IDLE_MODE:

        /* Idle is only RACH irrespective of BS_CCCH_SDCCH_COMB */
        burst_num = 0;
        break;

      case L1_DEDICATED_MODE :
        physical_channel_type = l1_dedicated_data_ptr->channel_specification.channel_info_1_before.channel_type;
        if (physical_channel_type == SDCCH_4 ||
            physical_channel_type == SDCCH_8  )
        {
          FNMOD = FN % 51;
          if (physical_channel_type == SDCCH_8) // 8 SDCCH/8
          {
            if ( FNMOD < 12)
            {
              burst_num = FNMOD % 4;
            }
            else if ( FNMOD > 14)
            {
              FNMOD = FNMOD - 15;
              burst_num = FNMOD % 4;
            }
            else
            {
              burst_num = 0;
            }
          }
          else // BCCH+CCCH+ 4 SDCCH/4
          {
            FNMOD = FN % 51;
            if (FNMOD < 14)
            {
              FNMOD = FNMOD % 14;
              switch (FNMOD)
              {
                case 1:
                case 7:
                case 11:
                   burst_num  = 1;
                   break;

                case 2:
                case 8:
                case 12:
                   burst_num  = 2;
                   break;

                case 3:
                case 9:
                case 13:
                   burst_num  = 3;
                   break;

                default:
                   burst_num  = 0;
                   break;
              }
            }  // if (FNMOD < 14)
            else  // FNMOD > 36
            {
              FNMOD = FNMOD % 14;
              switch (FNMOD)
              {
                case 10:
                case 0:
                case 6:
                   burst_num  = 1;
                   break;

                case 11:
                case 1:
                case 7:
                   burst_num  = 2;
                   break;

                case 12:
                case 2:
                case 8:
                   burst_num  = 3;
                   break;

                default:
                   burst_num  = 0;
                   break;
              }
            }  // FNMOD > 36

          }
          break;
        }
      // continue
      //case L1_DEDICATED_MODE :  continue for TCH which has similar strcutue to 52 frame MOD 13.
      case L1_TRANSFER_MODE :
          FNMOD = FN % 13;
          switch (FNMOD)
          {
            case 1:
            case 5:
            case 9:
               burst_num  = 1;
               break;

            case 2:
            case 6:
            case 10:
               burst_num  = 2;
               break;

            case 3:
            case 7:
            case 11:
               burst_num  = 3;
               break;

            default:
               burst_num  = 0;
               break;
          }
          break;

      default:
        break;
    } // l1_state
  }

  return burst_num;
}


#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION      gl1_gbta_read_nv_gbta_support

DESCRIPTION   Read the NV from EFS

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void gl1_gbta_read_nv_gbta_support(gas_id_t gas_id)
{
   if (geran_efs_read_primary(GERAN_EFS_GBTA_SUPPORT,
               &gl1_gprs_gbta_support,
               sizeof(gl1_gprs_gbta_support)) < 0)
   {
     gl1_gprs_gbta_support = 1;
     MSG_GERAN_MED_1_G("GBTA support default %d",gl1_gprs_gbta_support);
   }

   MSG_GERAN_MED_1_G("GBTA support %d",gl1_gprs_gbta_support);
} // gl1_gbta_read_nv_gbta_support


boolean gl1_gbta_get_nv_gbta_support(gas_id_t gas_id)
{
  return gl1_gprs_gbta_support;
}
#endif /* FEATURE_GPRS_GBTA */


#ifdef  FEATURE_GSM_GPRS_TIME_PWR_AVG
/*===========================================================================

FUNCTION  gpl1_reset_tx_pwr_avg

DESCRIPTION
  Resets TX pwr average params when IDLE /INACTIVE

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void gpl1_reset_tx_pwr_avg( gas_id_t gas_id )
{

#ifdef FEATURE_QSH_EVENT_METRIC
  if (gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10 > 0)
  {
    (void)gpl1_calc_avg_tx_pwr(gas_id);
    gl1_qsh_write_tx_metric_on_reset(gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10, gas_id);
  }
#endif /*FEATURE_QSH_EVENT_METRIC*/
  gpl1_tx_power[gas_id].tx_pwr_accum_Q16  = 0;
  gpl1_tx_power[gas_id].accum_count       = 0;
  gpl1_tx_power[gas_id].avg_tx_pwr        = 0;
  gpl1_tx_power[gas_id].avg_tx_pwr_slot   = 0;
  gpl1_tx_power[gas_id].valid             = FALSE;
  gpl1_tx_power[gas_id].avg_tx_pwr_dbm_10   = 0;
  gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10 = 0;
} /* gpl1_reset_tx_pwr_avg */

/*===========================================================================

FUNCTION  gpl1_reset_rx_pwr_avg

DESCRIPTION
  Resets RX pwr average params when IDLE /INACTIVE

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void gpl1_reset_rx_pwr_avg( gas_id_t gas_id )
{
  gpl1_rx_power[gas_id].rx_pwr_accum      = 0;
  gpl1_rx_power[gas_id].accum_count       = 0;
  gpl1_rx_power[gas_id].avg_rx_pwr        = 0;
  gpl1_rx_power[gas_id].valid             = FALSE;
  gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10 = 0;
  gpl1_rx_power[gas_id].avg_rx_pwr_coex   = 0;
  // - Do not reset - as updated and used in reporting for ccch power.
  //gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10_coex = GL1_CXM_UNKNOWN_POWER;
} /* gpl1_reset_rx_pwr_avg */

#ifdef FEATURE_WLAN_COEX_SW_CXM
/*===========================================================================

FUNCTION  gpl1_enable_snr_avg

DESCRIPTION
  Enables SNR averaging and re-start SNR accumulation

===========================================================================*/
void gpl1_enable_snr_avg(gas_id_t gas_id)
{
  GL1_ISR_LOCK(gas_id);
  gpl1_reset_snr_avg(gas_id);
  gpl1_snr[gas_id].valid = TRUE;
  GL1_ISR_UNLOCK(gas_id);
}

/*===========================================================================

FUNCTION  gpl1_stop_snr_avg

DESCRIPTION
  Stops accumulating the SNR

===========================================================================*/
void gpl1_stop_snr_avg( gas_id_t gas_id )
{
  gpl1_snr[gas_id].valid = FALSE;
}

/*===========================================================================

FUNCTION  gpl1_reset_snr_avg

DESCRIPTION
  Re-set SNR averaging

===========================================================================*/
void gpl1_reset_snr_avg( gas_id_t gas_id )
{
  gpl1_snr[gas_id].accum_count    = 0;
  gpl1_snr[gas_id].valid          = FALSE;
  gpl1_snr[gas_id].avg_snr        = 0;
}

/*===========================================================================

FUNCTION  gpl1_update_snr_avg

DESCRIPTION
  Does the SNR averaging based on the filter: y(t) = alpha*x(t) + (1-alpha) *y(t-1)
  Seprate averaging for different alpha (WLAN will give only two alphas, one for WLAN ON and one for WLAN OFF)
  For the first time y(t-1) is 0
  filtering coefficient alpha is given by COEX
  Average SNR is calculated in dB

===========================================================================*/
void gpl1_update_snr_avg( uint16 snr, gas_id_t gas_id )
{
  if( gpl1_snr[gas_id].valid )
  {
    uint32 alpha;
    uint32 x1,x2;

    gpl1_snr[gas_id].accum_count++;

    alpha = garb_cxm_coex_metrics_alpha(gas_id);

    /* alpha*x(t) */
    x2 = (uint32) (( snr ) * alpha);

    /*  (1-alpha) *y(t-1) */
    x1 = (uint32)(( 256 - alpha) * gpl1_snr[gas_id].avg_snr );

    x1 = (uint32)(x1 + x2);

    x1 >>= 8;  /* Now in Q8  , DIV by 256 */

    gpl1_snr[gas_id].avg_snr = (uint32)(x1);
  }
}

/*===========================================================================

FUNCTION  gpl1_read_avg_snr

DESCRIPTION

===========================================================================*/
boolean gpl1_read_avg_snr( uint32 *filt_metric, uint32 *metric_cnt, gas_id_t gas_id )
{
  boolean avg_snr_valid = FALSE;

  GL1_ISR_LOCK(gas_id);

  if( gpl1_snr[gas_id].accum_count > 0 )
  {
    avg_snr_valid =  TRUE;

    if ( gpl1_snr[gas_id].avg_snr > 323 )
    {
      *filt_metric = (gl1_msg_SNR_to_dBx16( gpl1_snr[gas_id].avg_snr ) / 16);
    }
    else
    {
     *filt_metric = 0;
    }

    *metric_cnt = gpl1_snr[gas_id].accum_count;

    /* re-set the accumulation */
    gpl1_snr[gas_id].accum_count = 0;
  }
  GL1_ISR_UNLOCK(gas_id);

  return avg_snr_valid;
}
#endif

/*===========================================================================

FUNCTION  gpl1_set_tx_pwr_avg_active

DESCRIPTION
  Enable TX pwr average when RACHING/DEDICATED/TRANSFER/DTM

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void gpl1_set_tx_pwr_avg_active(gas_id_t gas_id)
{
  idle_data_T               *idle_data_ptr;
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint8 tx_lev_index;

  MSG_GERAN_MED_1_G("TX_PWR_AVG Active=%d",gl1_get_FN(gas_id));
  gpl1_reset_tx_pwr_avg(gas_id);
  gpl1_tx_power[gas_id].valid = TRUE;

  idle_data_ptr     = l1_tskisr_blk->current_params.L1Data.pIdle_data;
  /* Power Reported to MCS at the time of RACH only , later it is updated as per band/txlev */
  if (idle_data_ptr)
  {
     tx_lev_index = gl1_max_pcl_pwr_class[idle_data_ptr->campedon_cell_ARFCN.band]
                                          [gl1_rf_power_class_per_band[gas_id][idle_data_ptr->campedon_cell_ARFCN.band]][1];

     gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10 = txlev_to_dBm[idle_data_ptr->campedon_cell_ARFCN.band][tx_lev_index ] * 10;  // dbm10
  }
  else
  {
     gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10 = 0;
  }
} /* gpl1_set_tx_pwr_avg_active */

/*===========================================================================

FUNCTION  gpl1_get_tx_pwr_avg_active_state

DESCRIPTION
  Return TX power avg active state.
RETURN VALUE
  gpl1_tx_power[gas_id].valid

SIDE EFFECTS
  None.

===========================================================================*/
boolean gpl1_get_tx_pwr_avg_active_state(gas_id_t gas_id)
{
  return gpl1_tx_power[gas_id].valid;
}

/*===========================================================================

FUNCTION  gpl1_get_tx_pwr_avg_dbm10

DESCRIPTION
  Return TX power avg in dbm10.
RETURN VALUE
  gpl1_tx_power[gas_id].avg_tx_pwr_dbm_10

SIDE EFFECTS
  None.

===========================================================================*/
int32 gpl1_get_tx_pwr_avg_dbm10(gas_id_t gas_id)
{
  return gpl1_tx_power[gas_id].avg_tx_pwr_dbm_10;
}

/*===========================================================================

FUNCTION  avg_tx_slot_pwr_db_10

DESCRIPTION
  Return TX power avg in dbm10 for actual slot tx.
RETURN VALUE
  gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10

SIDE EFFECTS
  None.

===========================================================================*/
int32 gpl1_get_avg_tx_slot_pwr_db_10(gas_id_t gas_id)
{
  return gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10;
}

/*===========================================================================

FUNCTION  gpl1_set_rx_pwr_avg_active

DESCRIPTION
  Enable RX pwr average when RACHING/DEDICATED/TRANSFER/DTM

RETURN VALUE


SIDE EFFECTS
  None.

===========================================================================*/
void gpl1_set_rx_pwr_avg_active(gas_id_t gas_id)
{
  gpl1_reset_rx_pwr_avg(gas_id);
  gpl1_rx_power[gas_id].valid  = TRUE;
} /* gpl1_set_rx_pwr_avg_active */


/*===========================================================================

FUNCTION  gpl1_get_rx_pwr_avg_active_state

DESCRIPTION
  Return RX power avg active state.
RETURN VALUE
  gpl1_rx_power[gas_id].valid

SIDE EFFECTS
  None.

===========================================================================*/
boolean gpl1_get_rx_pwr_avg_active_state(gas_id_t gas_id)
{
  return gpl1_rx_power[gas_id].valid;
}

/*===========================================================================

FUNCTION  gpl1_get_rx_pwr_avg_dbm10

DESCRIPTION
  Return RX power avg in dbm10.
RETURN VALUE
  gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10

SIDE EFFECTS
  None.

===========================================================================*/
int32 gpl1_get_rx_pwr_avg_dbm10(gas_id_t gas_id)
{
  return gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10;
}


/*===========================================================================
FUNCTION  gpl1_set_rx_pwr_avg_dbm10_coex

DESCRIPTION
  Set the rx power during select specfic bcch request, such that
  GL1 First power report to MCS can have correct power reported when
  it enters idle and obtains First Freq id and is able to report power.

RETURN VALUE
  gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10_coex

SIDE EFFECTS
  None.

===========================================================================*/
void gpl1_set_rx_pwr_avg_dbm10_coex(int32 rx_pwr_dbm_10_coex ,gas_id_t gas_id)
{
  if (rx_pwr_dbm_10_coex != GL1_CXM_UNKNOWN_POWER)
  {
    gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10_coex = rx_pwr_dbm_10_coex;
  }
}

/*===========================================================================
FUNCTION  avg_rx_slot_pwr_db_10_coex

DESCRIPTION
  Return RX power avg in dbm10 for  rx.
RETURN VALUE
  gpl1_rx_power[gas_id].avg_rx_slot_pwr_db_10_coex

SIDE EFFECTS
  None.

===========================================================================*/
int32 gpl1_get_rx_pwr_avg_dbm10_coex(gas_id_t gas_id)
{
   l1_serving_cell_meas_T *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];
   volatile ISRTIM_CMD_BLK*  l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   /* Rx power is not calculated in Idle */
   if( (  (  l1_tskisr_blk->l1_state  == L1_IDLE_MODE)
          || (l1_tskisr_blk->l1_state == L1_GPRS_IDLE_MODE)
#if defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM )
          || (l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    ) &&
    (l1_serving_cell_meas_ptr->no_of_measurements != 0))
   {
     return ((((l1_serving_cell_meas_ptr->rx_power / l1_serving_cell_meas_ptr->no_of_measurements)) * 10 ) >> 4);
   }
   else
   {
     return gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10_coex;
   }
}

/*===========================================================================

FUNCTION  gpl1_update_tx_pwr_avg

DESCRIPTION
  Perform power average in linear domain.
  PCL index is converted to corresponding dBm value to a linear value using a LUT.
  for a particular GSM band

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/

void  gpl1_update_tx_pwr_avg( int16*                power_lvl_idx,
                              sys_band_T            band,
                              gl1_defs_coding_type  cs,
                              gas_id_t              gas_id
                            )
{
  /* CR1036802:coex_pwr_avg is true only when we average the power received from RF hence functionality remains same */
  pwr_stat_band_idx_t pwr_stat_band_idx;
  ds3g_tx_power_bin_e_type gpl1_tx_power_bin_idx;

  if(gpl1_tx_power[gas_id].valid)
  {
    switch (band)
    {
      /* GSM 850/900 band, max power is 0, min power is 19 */
      case SYS_BAND_CELL_850:
      case SYS_BAND_PGSM_900:
      case SYS_BAND_EGSM_900:
        if((cs >= GL1_DEFS_MCS5_CODING) && (cs <= GL1_DEFS_MCS9_CODING))
        {
           if (*power_lvl_idx < 8)
           {
              *power_lvl_idx = 8;
           }
        }
        /* Limit txlev as per Power Class for each band */
        if( *power_lvl_idx < gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1] )
        {
          *power_lvl_idx = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
        }

       
        pwr_stat_band_idx = PWR_STAT_BAND_IDX_900;
        break;

      /* DCS band, max power is 29, min power is 15 */
      case SYS_BAND_DCS_1800:
        if((cs >= GL1_DEFS_MCS5_CODING) && (cs <= GL1_DEFS_MCS9_CODING))
        {
           if ((*power_lvl_idx < 2) || (*power_lvl_idx > 28))
           {
              *power_lvl_idx = 2;
           }
        }

        /* Limit the PCL as per Power Class for each band */
        if ( gl1_rf_power_class_per_band[gas_id][band] == 0 ) /* class 1 */
        {
          if (*power_lvl_idx > 28 ) /* greater than 30 dBm */
          {
            *power_lvl_idx = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
          }
        }
        else if ( gl1_rf_power_class_per_band[gas_id][band] == 1 ) /* class 2 */
        {
          if ( (*power_lvl_idx > 28) && (*power_lvl_idx < 3)  ) /* greater than 24 dBm */
          {
            *power_lvl_idx = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
          }
        }

       
        pwr_stat_band_idx = PWR_STAT_BAND_IDX_1800;
        break;

      /* PCS band, max power is 30, min power is 15 */
      case SYS_BAND_PCS_1900:
        if((cs >= GL1_DEFS_MCS5_CODING) && (cs <= GL1_DEFS_MCS9_CODING))
        {
           if ((*power_lvl_idx < 2) || (*power_lvl_idx > 28))
           {
              *power_lvl_idx = 2;
           }
        }

        /* Limit the PCL as per Power Class for each band */
        if ( gl1_rf_power_class_per_band[gas_id][band] == 0 ) /* class 1 */
        {
          if (*power_lvl_idx > 29 ) /* greater than 30 dBm */
          {
            *power_lvl_idx = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
          }
        }
        else if ( gl1_rf_power_class_per_band[gas_id][band] == 1 ) /* class 2 */
        {
          if ( (*power_lvl_idx > 29) && (*power_lvl_idx < 3 ) ) /* greater than 24 dBm */
          {
            *power_lvl_idx = gl1_max_pcl_pwr_class[band][gl1_rf_power_class_per_band[gas_id][band]][1];
          }
        }

               pwr_stat_band_idx = PWR_STAT_BAND_IDX_1900;
        break;

      case INVALID_BAND:
      default:

        MSG_GERAN_ERROR_1_G(" Incorrect band type %d",band);

        pwr_stat_band_idx = PWR_STAT_BAND_IDX_MAX; /* means not found */
        break;
    } //switch (band)

    if( pwr_stat_band_idx != PWR_STAT_BAND_IDX_MAX )
    {
      gpl1_tx_power_bin_idx = cvt_pcl_to_pwr_stat_idx[pwr_stat_band_idx][*power_lvl_idx];
      gpl1_tx_power[gas_id].pwr_stats_tx_in_ts[gpl1_tx_power_bin_idx]++;
    } /* if( pwr_stat_band_idx != PWR_STAT_BAND_IDX_MAX ) */
  } //if(gpl1_tx_power[gas_id].valid)
} /* gpl1_update_tx_pwr_avg */


/* Returns linear value in Q16 */

/**
 * @brief Converts dBm in Q16 Format to Linear Scale 
 * 
 * @param dBm_Q16 
 * 
 * @return int32 
 */
int32  gl1_linear_log_conversion_table_binary_search
(
  /*! Input value */
  int32  const  dBm_Q16
)
{
  int32   idx1;
  int32   lo   = 0;
  int32   hi   = simple_linear_log_base_10_conv_table_q16_size - 1;
  boolean done = FALSE;

  /*Binary Search for best suitable value from Log Table*/
  do
  {
    idx1 = (lo + hi) / 2;
    if( dBm_Q16 <= (simple_linear_log_base_10_conv_table_q16[idx1][1]*10) )
    {
      if ((idx1 == ((int32)simple_linear_log_base_10_conv_table_q16_size - 1)) ||
          ( dBm_Q16 > (simple_linear_log_base_10_conv_table_q16[idx1 + 1][1]*10) ) )
      {
        done = TRUE;
      }
      lo = idx1 + 1;
    }
    else
    {
      hi = idx1 - 1;
    }

  } while(!done && lo <= hi);

  if( dBm_Q16 > (simple_linear_log_base_10_conv_table_q16[0][1]*10) )
  {
    idx1 = 0;
  }
  /* Ensure closest value is picked */
  else if ((idx1 > 0) &&
           (idx1 < ((int32)simple_linear_log_base_10_conv_table_q16_size - 1)))
  {
    /* Check closeness of value to higher index since above algorithm always
     * picks the higher value when converting
     */
    if( ( dBm_Q16 - (simple_linear_log_base_10_conv_table_q16[idx1 + 1][1]*10) ) <
        ( (simple_linear_log_base_10_conv_table_q16[idx1][1]*10) - dBm_Q16 )
      )
    {
      /* higher index/lower value is closer */
      idx1++;
    }
  }

  /* Return corresponding log/linear value*/
  return (simple_linear_log_base_10_conv_table_q16[idx1][0]);

} /* gl1_simple_linear_log_conversion_table_binary_search() */

/**
 * @brief Updates Tx Pwr Avg to be reported to MCS 
 * 
 * 
 * @param tx_pwr_dBm 
 * @param gas_id 
 */
void  gpl1_update_coex_tx_pwr_avg( int32       tx_pwr_dBm,
                                   gas_id_t    gas_id
                                 )
{
  if( gpl1_tx_power[gas_id].valid )
  {
    /* convert to Q16 before passing to the binary search function */
    int32  tx_pwr_linear_Q16 = gl1_linear_log_conversion_table_binary_search( (tx_pwr_dBm<<16) );

    MSG_GERAN_DEBUG_OPT_3_G(" tx pwr dBm %d Linq16 %d lin %d ",tx_pwr_dBm, tx_pwr_linear_Q16, (tx_pwr_linear_Q16 >> 16) );

    gpl1_tx_power[gas_id].tx_pwr_accum_Q16 += tx_pwr_linear_Q16;
    gpl1_tx_power[gas_id].accum_count++;
  }
  else
  {
    MSG_GERAN_ERROR_0_G(" gpl1_tx_power invalid " );
  }

} /* gpl1_update_coex_tx_pwr_avg */

/*===========================================================================

FUNCTION  gpl1_update_rx_pwr_avg

DESCRIPTION

  Addition of time average power in dBm16 as LUT would bve large.

RETURN VALUE
 none

SIDE EFFECTS
  None.

===========================================================================*/
void gpl1_update_rx_pwr_avg( int32 rssi,gas_id_t gas_id)
{
   if(gpl1_rx_power[gas_id].valid)
   {
     gpl1_rx_power[gas_id].rx_pwr_accum += rssi;
     gpl1_rx_power[gas_id].accum_count++;
   }
} /* gpl1_add_rx_pwr */


/*===========================================================================

FUNCTION  gpl1_calc_avg_tx_pwr

DESCRIPTION

  Average is computed in Linear using a single pole IIR FILTER

  Y(n) = alpha * y ( n-1 ) + ( 1- alpha) * x(n).

  alpha is forgetting factor.
  y(n-1) - previous power avg
  y(n)   - current power avg

  Averaging is done in Q8 format.
  We are doing time based averaging and not number of samples.
  Thus over 26 frames with 8 slots = Divisor Average = 26x8
 
EXCEPTIONS
  For Tx Pwr Report to MCS, No IIR FIlter is employed
 
RETURN VALUE
  pwr in db10

SIDE EFFECTS
  None.

===========================================================================*/
int32 gpl1_calc_avg_tx_pwr(gas_id_t gas_id)
{
  int64 avg_tx_pwr_slot=0;
  int64 avg_tx_pwr = 0;
  int32 y1=0,y2=0;

  if (gpl1_tx_power[gas_id].accum_count)
  {
     /* new calculated avg tx power */
     avg_tx_pwr_slot = gpl1_tx_power[gas_id].tx_pwr_accum_Q16/gpl1_tx_power[gas_id].accum_count;

     avg_tx_pwr_slot >>= 16;

     /*MCS Needs Tx Power Averaged over Total Slots in 8 Frames Duration , for which IIR Filter isnt needed */
     avg_tx_pwr      = gpl1_tx_power[gas_id].tx_pwr_accum_Q16 / TIME_AVG_8_FRAME;

     avg_tx_pwr >>= 16;
  }

  if (gpl1_tx_power[gas_id].accum_count == 0)
  {
    MSG_GERAN_MED_1_G(" TX_PWR_AVG accum_count=0 avg_tx_pwr_dbm_10 %d",gpl1_tx_power[gas_id].avg_tx_pwr_dbm_10);
    return gpl1_tx_power[gas_id].avg_tx_pwr_dbm_10;
  }
  
  /* apply filter y(n) = alpha * y(n-1) + ( 1-alpha) * x(n)  , alhpa 0.93*/
  /* y1 = alpha * y(n-1)
  ** y2 = ( 1-alpha) * x(n)
  */

  y1 = (int32)((int32)ALPHA_FILCOEFF_PWR_AVG_COEX * gpl1_tx_power[gas_id].avg_tx_pwr_slot);
  y2 = (int32)(((int32)(1<<8)  - (int32)ALPHA_FILCOEFF_PWR_AVG_COEX) * avg_tx_pwr_slot);

  /* add in Q8 */
  y1 += y2;

  /* Perform rounding */
  y1 +=  ( 1 << 7);  /* Add 0.5 in Q8 */
  y1 >>= 8;  /* Now in Q0 */

  gpl1_tx_power[gas_id].avg_tx_pwr = avg_tx_pwr;
  /* make y(n) as y(n-1) */
  gpl1_tx_power[gas_id].avg_tx_pwr_slot = y1;

  /* convert the power in linear to dbm 10 */
  gpl1_tx_power[gas_id].avg_tx_pwr_dbm_10     = (int32)(gl1_msg_algs_linear_to_dBx16(avg_tx_pwr) * 10) >> 4 ;
  gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10 = (int32)(gl1_msg_algs_linear_to_dBx16(y1) * 10) >> 4 ;

  /* init for next averaging */
  gpl1_tx_power[gas_id].accum_count  = 0;
  gpl1_tx_power[gas_id].tx_pwr_accum_Q16 = 0;

  MSG_GERAN_MED_3_G("TX_PWR_AVG avg_tx_pwr_lin %d avg_tx_pwr_dbm_10 %d avg_tx_slot_pwr_db_10 %d",
                    avg_tx_pwr,
                    gpl1_tx_power[gas_id].avg_tx_pwr_dbm_10,
                    gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10);

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
  gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_accum += gpl1_tx_power[gas_id].avg_tx_slot_pwr_db_10;
  gl1_hw_asdiv_mtpl_store[gas_id].number_of_tx_pwr_avg++;
#endif

  return gpl1_tx_power[gas_id].avg_tx_pwr_dbm_10;
} /* gpl1_calc_avg_tx_pwr */


/*===========================================================================

FUNCTION  gpl1_calc_avg_rx_pwr

DESCRIPTION

  Average is computed in dbm16 as range quite high for LUT

  Averaging is done in db16 format.

RETURN VALUE
  pwr in db10

SIDE EFFECTS
  None.

===========================================================================*/
int32 gpl1_calc_avg_rx_pwr(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_serving_cell_meas_T   *l1_serving_cell_meas_ptr =&l1_serving_cell_meas[gas_id];

  int32 avg_rx_pwr = 0;
  int32 x1=0,x2;
  int32 y1=0,y2;

  if (  (l1_tskisr_blk->l1_state == L1_DEDICATED_MODE )    ||
        (l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE  ) ||
        (l1_tskisr_blk->l1_state == L1_PACKET_ACCESS_MODE  )
#ifdef FEATURE_GSM_DTM
        || (l1_tskisr_blk->l1_state == L1_DTM_MODE)
#endif /*  FEATURE_GSM_DTM */
     )
  {
    if ( 0 != l1_serving_cell_meas_ptr->no_of_measurements )
    {
      avg_rx_pwr = (l1_serving_cell_meas_ptr->rx_power / l1_serving_cell_meas_ptr->no_of_measurements);
    }
  }
  else // in L1_TRANSFER_MODE
  {
    if ( 0 != gpl1_rx_power[gas_id].accum_count)
    {
       avg_rx_pwr = (gpl1_rx_power[gas_id].rx_pwr_accum / gpl1_rx_power[gas_id].accum_count);
    }
  }

  /* Do not perform averaging when avg rx power is zero */
  if(avg_rx_pwr == 0)
  {
    MSG_GERAN_MED_1_G("RX_PWR_AVG flt_avg_rx_pwr::Not performing avg %d",l1_serving_cell_meas_ptr->no_of_measurements);
    return gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10;
  }
  /* dont start with zero avg power - which means very high power avg_rx_pwr - non zero */
  if (gpl1_rx_power[gas_id].avg_rx_pwr == 0)
  {
    gpl1_rx_power[gas_id].avg_rx_pwr_coex   = avg_rx_pwr;
    MSG_GERAN_MED_1_G("RX_PWR_AVG flt_avg_rx_pwr first value avg_rx_pwr %d",avg_rx_pwr);
  }

   /* apply filter y(n) = alpha * y(n-1) + ( 1-alpha) * x(n)  , alhpa 0.93*/
   /* x1 = alpha * y(n-1)
   ** x2 = ( 1-alpha) * x(n)
   */
   x1 = (int32)((int32)ALPHA_FILCOEFF_PWR_AVG * gpl1_rx_power[gas_id].avg_rx_pwr);
   x2 = (int32)(((int32)(1<<8)  - (int32)ALPHA_FILCOEFF_PWR_AVG) * avg_rx_pwr);

   y1 = (int32)((int32)ALPHA_FILCOEFF_PWR_AVG_COEX * gpl1_rx_power[gas_id].avg_rx_pwr_coex);
   y2 = (int32)(((int32)(1<<8)  - (int32)ALPHA_FILCOEFF_PWR_AVG_COEX) * avg_rx_pwr);

   /* add in Q8 */
   x1 += x2;

   /* add in Q8 */
   y1 += y2;

   /* Rounding in Q8 */
   x1 += ( 1 << 7);

   /* Rounding in Q8 */
   y1 += ( 1 << 7);

   /* Filter was in Q8 , hence make it back to Q0 */
   x1 >>= 8;  /* Now in Q0 */

   /* Filter was in Q8 , hence make it back to Q0 */
   y1 >>= 8;  /* Now in Q0 */

   /* make x(n) as x(n-1) */
   gpl1_rx_power[gas_id].avg_rx_pwr = x1;
   /* make y(n) as y(n-1) */
   gpl1_rx_power[gas_id].avg_rx_pwr_coex = y1;

  /* convert the power in dbm16 to dbm 10 */
  gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10      = (int32)(x1 * 10) >> 4;
  gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10_coex = (int32)(y1 * 10) >> 4;

  MSG_GERAN_MED_3_G("RX_PWR_AVG flt_avg_rx_pwr %d avg_rx_pwr_dbm_10 %d avg_rx_pwr_coex %d",y1,gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10_coex,avg_rx_pwr);

  /* init for next averaging */
  gpl1_rx_power[gas_id].accum_count  = 0;
  gpl1_rx_power[gas_id].rx_pwr_accum = 0;

  return gpl1_rx_power[gas_id].avg_rx_pwr_dbm_10;
} /* gpl1_calc_avg_rx_pwr */

/*===========================================================================

FUNCTION  gl1_cxm_stx_set_avg_power

DESCRIPTION

   Wrapper Function to send MCS_CXM_STX_SET_POWER to CXM

RETURN VALUE
  pwr in db10

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_cxm_stx_set_avg_power
(
  cxm_tech_type       tech_id,
  /* Time averaged Tx power in dBm*10 */
  int32               tx_power,
  /* Time averaged Rx power in dBm*10 */
  int32               rx_power
)
{
  cxm_stx_set_pwr_ind_s set_avg_power;
  errno_enum_type  msgr_error;

  memset(&set_avg_power, 0, sizeof(cxm_stx_set_pwr_ind_s));

  msgr_init_hdr((msgr_hdr_struct_type*)  &set_avg_power,MSGR_GERAN_GL1,MCS_CXM_STX_SET_POWER_IND);

  set_avg_power.tech_id  = tech_id;
  set_avg_power.rx_pwr   = rx_power;
  set_avg_power.tx_pwr   = tx_power;
  set_avg_power.valid_pwrs = STX_RX_TX_VALID_BOTH;

  msgr_error = msgr_send( (msgr_hdr_struct_type *) &set_avg_power,
                           sizeof(cxm_stx_set_pwr_ind_s)
                         );
  ASSERT(msgr_error == E_SUCCESS);

} /* gl1_cxm_stx_set_avg_power */

#endif /* FEATURE_GSM_GPRS_TIME_PWR_AVG */

#ifdef FEATURE_GSM_GPRS_TX_INFO
/*===========================================================================

FUNCTION  gl1_get_current_tx_info

DESCRIPTION
  This function will fill in the passed pointer with the current TX arfcn and
  Band.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_get_current_tx_info (l1_store_tx_params_T *params_ptr)
{
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    params_ptr->tx_active = FALSE;

    /*
      A Packet Polled response is not covered by this functionality, L1 would
      in a TX state but in Idle Mode. This functionality is intended as low
      impact on loading and aims to give the other technology L1 a course
      guide as to GSM L1s current TX state.
     */
    switch (l1_tskisr_blk->l1_state)
    {
        case L1_RANDOM_ACCESS_MODE :
        case L1_PACKET_ACCESS_MODE :
        case L1_DEDICATED_MODE :
        case L1_TRANSFER_MODE :
        case L1_DTM_MODE :
        {
            ARFCN_T arfcn_ptr;
            gl1_hw_get_current_tx_arfcn(&arfcn_ptr,gas_id);
            params_ptr->arfcn.num  = arfcn_ptr.num;
            params_ptr->arfcn.band = arfcn_ptr.band;
            params_ptr->tx_active  = TRUE;
        }
        break;

        default: break;
    }

}
#endif


/*===========================================================================

FUNCTION  gpl1_config_modem_power_stats_tx

DESCRIPTION
Enables/disables capturing of tx power stats

RETURN VALUE
None

SIDE EFFECTS
None

===========================================================================*/
void gpl1_config_modem_power_stats_tx(boolean en_pwr_stats, sys_modem_as_id_e_type as_id)
{
   gas_id_t gas_id = geran_map_nas_id_to_gas_id( as_id );

   if(en_pwr_stats)
   {
      gpl1_reset_modem_power_stats_tx(gas_id);
   }
}

/*===========================================================================

FUNCTION  gpl1_get_modem_power_stats_tx

DESCRIPTION
Fetches the tx power stats and clears old stats

RETURN VALUE
Tx power stats in ms

SIDE EFFECTS
None

===========================================================================*/
void gpl1_get_modem_power_stats_tx(uint32 tx_power_bin[DS3G_TX_POWER_BIN_MAX], sys_modem_as_id_e_type as_id)
{
   ds3g_tx_power_bin_e_type  pwr_idx;
   gas_id_t gas_id = geran_map_nas_id_to_gas_id( as_id );

   for(pwr_idx = DS3G_TX_POWER_BIN_1; pwr_idx < DS3G_TX_POWER_BIN_MAX; pwr_idx++)
   {
      /* cvt from ts to ms (1 ts = 0.577ms) */
      tx_power_bin[pwr_idx] = (uint32) ( (double) gpl1_tx_power[gas_id].pwr_stats_tx_in_ts[pwr_idx] * 0.577);
   }

   gpl1_reset_modem_power_stats_tx(gas_id);
}

/*===========================================================================

FUNCTION  gpl1_reset_modem_power_stats_tx

DESCRIPTION
Clears and resets power stats

RETURN VALUE
None

SIDE EFFECTS
None

===========================================================================*/
static void gpl1_reset_modem_power_stats_tx(gas_id_t gas_id)
{
   /* initialize pwr stats */
   memset(&gpl1_tx_power[gas_id].pwr_stats_tx_in_ts, 0, sizeof(gpl1_tx_power[gas_id].pwr_stats_tx_in_ts) );
}

#ifdef FEATURE_GL1_LOG_MTX_WAIT
#endif /* FEATURE_GL1_LOG_MTX_WAIT */
/*===========================================================================

FUNCTION  calculate_txpwr_max_cch

DESCRIPTION
 Optional LB_MS_TXPWR_MAX_CCH parameter has been added in SI 13 rest octets
 The mobile station's maximum output power is based on parameters received in
 system information messages on (P)BCCH while in the (packet) idle mode.
 However, the network has difficulties setting the correct maximum output power
 when packet resources are allocated on a different frequency band than the
 common channels (P)BCCH. Especially because of the different mapping of power
 control levels on different frequency bands, the network cannot set the same,
 or even close to the same dBm value for the maximum output power on each
 frequency band.
RETURN VALUE
 max tx power

SIDE EFFECTS
  None.

===========================================================================*/
uint8 calculate_txpwr_max_cch (ARFCN_T arfcn
                               , boolean rach_pwr_red
                               , gas_id_t gas_id )
{
  sys_band_T band;
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  layer1_sys_info_T *camped_cell_data_ptr = &(l1_idle_data->campedon_cell_data);
  uint8 pmax;
  MSG_GERAN_HIGH_1_G("RACH power reduction active = %d",rach_pwr_red);
  gl1_set_rach_pwr_reduction(FALSE,gas_id);
  band = arfcn.band;

  switch (band)
  {
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_CELL_850:
     if(camped_cell_data_ptr->LB_MS_TXPWR_MAX_CCH_valid == FALSE)
     {
       pmax = camped_cell_data_ptr->MS_TXPWR_MAX_CCH;
     }
     else
     {
       pmax = camped_cell_data_ptr->LB_MS_TXPWR_MAX_CCH;
     }
     if(rach_pwr_red)
     {
       if(pmax < RACH_PWR_RED_LB_P5_MINUS_10DB)
       {
         gl1_set_rach_pwr_reduction(TRUE,gas_id);
         gl1_set_rach_pwr_reduction_pwr_idx(RACH_PWR_RED_LB_P5_MINUS_10DB_PWR_IDX, gas_id);
         MSG_GERAN_HIGH_1_G("applying RACH power reduction for LB: pmax = %d",pmax);
       }
     }
    break;

    case SYS_BAND_DCS_1800:
    case SYS_BAND_PCS_1900:
     pmax = camped_cell_data_ptr->MS_TXPWR_MAX_CCH;
     if(rach_pwr_red)
     {
       if(pmax < RACH_PWR_RED_HB_P2_MINUS_10DB)
       {
         gl1_set_rach_pwr_reduction(TRUE,gas_id);
         gl1_set_rach_pwr_reduction_pwr_idx(RACH_PWR_RED_HB_P2_MINUS_10DB_PWR_IDX, gas_id);
         MSG_GERAN_HIGH_1_G("applying RACH power reduction for HB: pmax = %d",pmax);
       }
     }
    break;

    default:
      MSG_GERAN_ERROR_0_G("Unsupported Band Class");
      pmax = camped_cell_data_ptr->MS_TXPWR_MAX_CCH;
  }

  return pmax;

}

/*===========================================================================

FUNCTION  calculate_init_txpwr_max_dedicated

DESCRIPTION
Initial Tx powerl level for dedicated channel

RETURN VALUE
 max tx power

SIDE EFFECTS
  None.

===========================================================================*/
uint8 calculate_init_txpwr_max_dedicated ( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  layer1_sys_info_T *camped_cell_data_ptr = &(l1_idle_data->campedon_cell_data);
  uint8 pmax;
  ARFCN_T  serving_BCCH = gl1_get_serving_cell_arfcn(gas_id);;

  switch (serving_BCCH.band)
  {
    case SYS_BAND_EGSM_900:
    case SYS_BAND_PGSM_900:
    case SYS_BAND_CELL_850:
     if(camped_cell_data_ptr->LB_MS_TXPWR_MAX_CCH_valid == FALSE)
     {
       pmax = camped_cell_data_ptr->MS_TXPWR_MAX_CCH;
     }
     else
     {
       pmax = camped_cell_data_ptr->LB_MS_TXPWR_MAX_CCH;
     }
    break;

    default:
      pmax = camped_cell_data_ptr->MS_TXPWR_MAX_CCH;
  }

  return pmax;

}


/*===========================================================================

FUNCTION  l1_idle_if_gps_notify

DESCRIPTION
  Initiate a Post Cell change notification to GPS, even when GPS is not active.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void  l1_idle_if_gps_notify(gas_id_t gas_id)
{
   gl1_timetag_cgps(L1_IDLE_MODE, CGPS_TIMETAG_POST_CELL_CHANGE,gas_id);

}
/*===========================================================================

FUNCTION  gl1_get_rach_pwr_reduction

DE SCRIPTION
  Get current state of RACH power reduction.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_get_rach_pwr_reduction(gas_id_t gas_id)
{
   return gl1_rach_pwr_reduction[gas_id].active;
}

/*===========================================================================

FUNCTION  gl1_set_rach_pwr_reduction

DESCRIPTION
  Set current state of RACH power reduction.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_set_rach_pwr_reduction(boolean set,gas_id_t gas_id)
{
   gl1_rach_pwr_reduction[gas_id].active = set;
}
/*===========================================================================

FUNCTION  gl1_set_rach_pwr_reduction_power

DESCRIPTION
  Set current value of RACH power reduction.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_set_rach_pwr_reduction_pwr_idx(int16 pwr_idx, gas_id_t gas_id)
{
   gl1_rach_pwr_reduction[gas_id].pwr_idx = pwr_idx;
}
/*===========================================================================

FUNCTION  gl1_get_rach_pwr_reduction_pcl

DESCRIPTION
  Get current value of RACH power reduction PCL.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
int16 gl1_get_rach_pwr_reduction_pwr_idx(gas_id_t gas_id)
{
   return gl1_rach_pwr_reduction[gas_id].pwr_idx;
}
/*===========================================================================

FUNCTION  gl1_set_rach_pwr_reduction_first_rach

DESCRIPTION
  Set current value of RACH power reduction.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_set_rach_pwr_reduction_first_rach(boolean first_rach, gas_id_t gas_id)
{
   gl1_rach_pwr_reduction[gas_id].first_rach = first_rach;

}
/*===========================================================================

FUNCTION  gl1_get_rach_pwr_reduction_first_rach

DESCRIPTION
  Get current value of RACH power reduction first RACH.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_get_rach_pwr_reduction_first_rach(gas_id_t gas_id)
{
   return gl1_rach_pwr_reduction[gas_id].first_rach;
}

/*===========================================================================

FUNCTION  gl1_activate_rach_pwr_reduction

DESCRIPTION
  Potentially activates RACH power reductin, depending on the parameters passed in
  PH_RANDOM_ACCESS_REQ.

RETURN VALUE
  boolean
  0 - inactive
  1 - active

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_activate_rach_pwr_reduction(boolean  pwr_reduction_valid,
                                     uint8    pwr_reduction_value,
                                     boolean  emergency_call,
                                     gas_id_t gas_id)
{
    gl1_rach_pwr_reduction[gas_id].active = FALSE;

    if (pwr_reduction_valid)
    {
      if(l1_get_serving_RLA_C(gas_id) > GL1_RACH_PWR_RED_MIN_RX_LEV)
      {
        if(pwr_reduction_value)
        {
                if(!emergency_call)
                {
                   /* if pwr_red is valid, serving_RLA_C is > -45dBm, pwr_red_value is 1 and
              this is not an emergency call, set pwr_reduction as active */
                   gl1_rach_pwr_reduction[gas_id].active = TRUE;
                   gl1_set_rach_pwr_reduction_first_rach(TRUE,gas_id);
                }
        }
        else
        {
            /* if pwr_red is valid serving_RLA_C is > -45dBm, and pwr_red_value is 0,
         set pwr_reduction as active*/
            gl1_rach_pwr_reduction[gas_id].active = TRUE;
            gl1_set_rach_pwr_reduction_first_rach(TRUE,gas_id);
        }
      }
    }
}
/*===========================================================================

FUNCTION  gl1_check_power_reduction_update

DESCRIPTION
  Checks if a power update should be immediately applied (power reduction disabled)
  or if power reduction ahould continue

RETURN VALUE
  boolean
  0 - inactive
  1 - active
SIDE EFFECTS
  None.

===========================================================================*/
byte gl1_check_pwr_reduction_update (ARFCN_T arfcn, byte txlev, byte current_power_level_in_use, gas_id_t gas_id)
{
    sys_band_T band;
    layer1_sys_info_T *camped_cell_data_ptr = &(l1_idle_data_store[gas_id].campedon_cell_data);
    uint8 pmax;
    byte ms_power_level_in_use = current_power_level_in_use;
    byte new_ms_power_level_in_use = current_power_level_in_use;

    band = arfcn.band;

    switch (band)
    {
      case SYS_BAND_EGSM_900:
      case SYS_BAND_PGSM_900:
      case SYS_BAND_CELL_850:
       if(camped_cell_data_ptr->LB_MS_TXPWR_MAX_CCH_valid == FALSE)
       {
         pmax = camped_cell_data_ptr->MS_TXPWR_MAX_CCH;
       }
       else
       {
         pmax = camped_cell_data_ptr->LB_MS_TXPWR_MAX_CCH;
       }
       new_ms_power_level_in_use = RACH_PWR_RED_LB_P5_MINUS_10DB;

      break;

      case SYS_BAND_DCS_1800:
      case SYS_BAND_PCS_1900:
        pmax = camped_cell_data_ptr->MS_TXPWR_MAX_CCH;
        new_ms_power_level_in_use = RACH_PWR_RED_HB_P2_MINUS_10DB;
      break;

      default:
        MSG_GERAN_ERROR_0_G("Unsupported Band Class");
        pmax = camped_cell_data_ptr->MS_TXPWR_MAX_CCH;
    }
    /* if RACH power reduction is active and new txlev has not changed from MS_TXPWR_MAX_CCH or
       LB_MS_TXPWR_MAX_CCH (whichever is applicable) continue to apply power reduction
       ELSE
       new Tx level should be applied so deactivate power reduction */
    if (gl1_get_rach_pwr_reduction(gas_id))
    {
      /* update power level in use to match RACH power reduction before disabling RACH power reduction */
      ms_power_level_in_use = new_ms_power_level_in_use;
      if (txlev != pmax)
      {
        gl1_set_rach_pwr_reduction(FALSE,gas_id);
      }
    }
    return ms_power_level_in_use;
}


/*===========================================================================

FUNCTION  gl1_tech_id_to_gas_id

DESCRIPTION
  CxM tech ID to GAS ID mapping function

RETURN VALUE
  GAS ID

SIDE EFFECTS
  None.

===========================================================================*/
gas_id_t gl1_tech_id_to_gas_id( cxm_tech_type tech_id )
{
  gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;

  switch ( tech_id )
  {
    case CXM_TECH_GSM1:
      gas_id = GERAN_ACCESS_STRATUM_ID_1;
      break;
#if defined ( FEATURE_DUAL_SIM )
    case CXM_TECH_GSM2:
      gas_id = GERAN_ACCESS_STRATUM_ID_2;
      break;
#endif /* FEATURE_DUAL_SIM_SIM */

#if defined ( FEATURE_TRIPLE_SIM )
    case CXM_TECH_GSM3:
      gas_id = GERAN_ACCESS_STRATUM_ID_3;
      break;
#endif /* FEATURE_TRIPLE_SIM */

    default:
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_1_G( "Invalid tech_id %d", tech_id);
      break;
   }

  return ( gas_id );
}

/*===========================================================================

FUNCTION  gl1_rf_mode_type_to_cxm_tech_id

DESCRIPTION
  RF mode type to CxM tech ID mapping function

RETURN VALUE
  GAS ID

SIDE EFFECTS
  None.

===========================================================================*/
cxm_tech_type gl1_rf_mode_type_to_cxm_tech_id( rfcom_mode_enum_type rf_mode_type )
{
  cxm_tech_type tech_id = CXM_TECH_DFLT_INVLD;

  switch(rf_mode_type)
  {
     case RFM_IMT_MODE:      /*!< WCDMA @todo depricate this enum @deprecated  */
        tech_id = CXM_TECH_WCDMA;
        break;
     case RFM_EGSM_MODE:     /*!< GSM @todo depricate this enum @deprecated  */
        tech_id = CXM_TECH_GSM1;
        break;
     case RFM_1X_MODE:       /*!< 1X Mode */
        tech_id = CXM_TECH_ONEX;
        break;
//   case RFM_GPS_MODE,      /*!< GPS Mode @todo depricate this enum @deprecated  */
     case RFM_1XEVDO_MODE:   /*!< HDR Rev 0, A, and B modes */
        tech_id = CXM_TECH_HDR;
        break;
//   case RFM_SVDO_MODE,     /*!< Mode to support simultaneous 1x and HDR transmit on a single device. */
//   case RFM_GNSS_MODE,     /*!< Global navigation systems mode. */
     case RFM_LTE_MODE:      /*!< UMTS Long Term Evolution mode. */
     case RFM_LTE_FTM_MODE:  /*!< UMTS Long Term Evolution FTM Mode */
        tech_id = CXM_TECH_LTE;
        break;
     case RFM_TDSCDMA_MODE:  /*!< TDSCDA mode */
     case RFM_TDSCDMA_MODE_FTM: /*!< TDSCDMA FTM NSmode */
        tech_id = CXM_TECH_TDSCDMA;
        break;
     case RFM_EGSM_MODE_2:   /*!< Second GSM mode for DSDA case for RF-MCPM interface */
        tech_id = CXM_TECH_GSM2;
        break;
     case RFM_EGSM_MODE_3:   /*!< Third GSM mode for TSTS/TSDA case for RF-MCPM interface */
        tech_id = CXM_TECH_GSM3;
        break;
//   case RFM_WLAN_MODE,     /*!< WLAN Mode */
//   case RFM_NUM_MODES,     /*!< Total number of supported Modes */
//   case RFM_INVALID_MODE = RFM_NUM_MODES, /*!< Invalid RFM Mode  */
//   case RFM_ALL_MODES = 0x7FFFFFFF /*!< Wildcard to indicate all Modes  */
     default:
        break;
  }

  return tech_id;

}
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)

/*===========================================================================

FUNCTION  grm_process_urgent_preemption

DESCRIPTION
  This function is called in response to UNLOCK_IMMEDIATELY events
  when the GERAN client is in a connected mode and any pre-emption
  implies that a client of even higher priority needs the transceiver

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void grm_process_urgent_preemption(trm_client_enum_t holding_client)
{
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  grm_client_enum_t grm_client;
  trm_reason_enum_t conn_reason;
  gas_id_t          gas_id;

  grm_client  = grm_map_trm_client_id_to_grm_client_id(holding_client);
  gas_id      = gl1_map_client_id_to_gas_id(grm_client);

  /* retrieve the priority of the current connection */
  conn_reason = grm_get_connection_priority(GRM_CS_CONNECTION, gas_id);
  if (TRM_ACCESS == conn_reason)
  {
    /* tell GRR about this pre-emption */
    MSG_GERAN_HIGH_2_G("Pre-emption of client %d whilst using reason %d", (int)holding_client, (int)conn_reason);
    L1_send_MPH_PREEMPTION_IND(RR_L1_CS_PREEMPTION, gas_id);
  }
#else
  NOTUSED(holding_client);
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

  return;
}

/*===========================================================================

FUNCTION  grm_cs_traffic_abort_callback

DESCRIPTION
  This function handles an abort callback in a
  CS connected state with TRM_TRAFFIC priority
  Note the expectation is that this callback will not get called

DEPENDENCIES
  Implements TRM "advanced" unlock callback mechanism

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_cs_traffic_abort_callback( trm_unlock_callback_data *unlock_data_p )
{
  if ( unlock_data_p )
  {
    grm_client_enum_t unlock_client = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);
    gas_id_t gas_id = check_gas_id(gl1_map_client_id_to_gas_id( unlock_client ));

    /* urgent unlock requests are passed up to GRR */
    if (unlock_data_p->event == TRM_UNLOCK_IMMEDIATELY)
    {
      grm_process_urgent_preemption(unlock_data_p->unlock_client);
    }
    else if (unlock_data_p->event != TRM_UNLOCK_CANCELLED)
    {
      MSG_GERAN_HIGH_2_G( "Ignoring unlock event %d for Client %d",
                        unlock_data_p->event,
                        unlock_data_p->winning_client_info.client_id );
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_cs_access_abort_callback

DESCRIPTION
  This function handles an abort callback in a
  CS connected state with TRM_ACCESS priority

DEPENDENCIES
  Implements TRM "advanced" unlock callback mechanism

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_cs_access_abort_callback( trm_unlock_callback_data *unlock_data_p )
{
  if ( unlock_data_p )
  {
    grm_client_enum_t unlock_client = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);
    gas_id_t gas_id = check_gas_id(gl1_map_client_id_to_gas_id( unlock_client ));

    /* urgent unlock requests are passed up to GRR */
    if (unlock_data_p->event == TRM_UNLOCK_IMMEDIATELY)
    {
      grm_process_urgent_preemption(unlock_data_p->unlock_client);
    }
    else if (unlock_data_p->event != TRM_UNLOCK_CANCELLED)
    {
      MSG_GERAN_HIGH_2_G( "Ignoring unlock event %d for Client %d",
                        unlock_data_p->event,
                        unlock_data_p->winning_client_info.client_id );
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_ignore_unlock_callback

DESCRIPTION
  This function always ignores an unlock callback

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_ignore_unlock_callback( trm_unlock_callback_data *unlock_data_p )
{
  if ( unlock_data_p )
  {
    grm_client_enum_t unlock_client = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);
    gas_id_t gas_id = check_gas_id(gl1_map_client_id_to_gas_id( unlock_client ));

    if (unlock_data_p->event != TRM_UNLOCK_CANCELLED)
    {
      MSG_GERAN_HIGH_2_G( "Ignoring unlock event %d for Client %d",
                        unlock_data_p->event,
                        unlock_data_p->winning_client_info.client_id );
    }
  }

  return;
}

#ifdef FEATURE_GTA_DURING_PS_RACH

/*===========================================================================

FUNCTION     gta_handle_abort_RACH_callback

DESCRIPTION  L1 task handler for GTA abort callback which has been triggered from
             G2X calling the GTA unlock callback when we are in RACH for PS state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gta_handle_abort_RACH_callback(grm_client_enum_t       winning_client_id,
                                          trm_reason_enum_t       winning_reason,
                                          trm_unlock_event_enum_t event,
                                          uint32                  unlock_by_sclk,
                                          gas_id_t                gas_id)
{
  uint32  frame_duration;
  uint32 suspension_FN;
  uint32 suspension_end_FN;
  uint32 rach_delay;
  idle_data_T *l1_idle_data = &l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  gas_id = check_gas_id(gas_id);

  unlock_by_sclk = unlock_by_sclk - timetick_get_safe();
  frame_duration = MSECS_TO_FRAMES(timetick_cvt_from_sclk(unlock_by_sclk, T_MSEC));
  suspension_FN = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), frame_duration );

  GL1_ISR_LOCK(gas_id);

  /*reset the suspension FN to invalid before calculating the new GTA suspension FN*/
  l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;

  /*need to suspend few frames before*/
  suspension_FN = SUB_FN(suspension_FN, SUSPENSION_FRAME_DELAY);

  /*we we allow GTA gap only if we have enough time to suspend*/
  (void)gpl1_block_bound_51_multiframe(suspension_FN, &l1_idle_data->random_access_data.suspension_FN_GTA, gas_id);

  //We need to suspend 1 frame before the block start frame
  l1_idle_data->random_access_data.suspension_FN_GTA = SUB_FN(l1_idle_data->random_access_data.suspension_FN_GTA, 1);

  MSG_GERAN_HIGH_3_G("GTA_PS_RACH :Handle GTA abort CB, unlock_by_sclk %d, frame_duration %d suspension_FN_GTA %d",
                      unlock_by_sclk, frame_duration, l1_idle_data->random_access_data.suspension_FN_GTA);

  if(IS_FRAME_NUM_LATER(SUB_FN(l1_idle_data->random_access_data.suspension_FN_GTA, REDUCE_FRAMES_TASK_TO_ISR_LATENCY), GSTMR_GET_FN_GERAN(gas_id)))
  {
    suspension_end_FN = ADD_FN(l1_idle_data->random_access_data.suspension_FN_GTA, G2X_PAGE_DUR_FRAMES);

    /*check if there is a pending single block assignment or DL TBF ass,  it collides with TA gap, then dont allow gap*/
    if( (l1_transfer_data[gas_id].starting_time != L1_GPRS_INVALID_FN ) && (l1_tskisr_blk->main_command == L1_TRANSFER_COMMAND)
        && (l1_transfer_data[gas_id].current_alloc_type == L1_SINGLE_BLOCK_ALLOC || l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC))

    {
      MSG_GERAN_HIGH_3_G("GTA_PS_RACH :Dont Allow GTA during PS RACH : suspension_FN_GTA %d curr aloc %d Single blk st fn %d",
                          l1_idle_data->random_access_data.suspension_FN_GTA,
                          l1_transfer_data[gas_id].current_alloc_type,
                          l1_transfer_data[gas_id].starting_time);

      l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
    }
    else if((l1_transfer_data[gas_id].starting_time != L1_GPRS_INVALID_FN ) && (SINGLE_BLOCK_RECV(gas_id) == TRUE))
    {
        MSG_GERAN_HIGH_3_G("GTA_PS_RACH :Dont Allow GTA during PS RACH : suspension_FN_GTA %d curr aloc %d SINGLE BLK st fn %d",
                            l1_idle_data->random_access_data.suspension_FN_GTA,
                            l1_transfer_data[gas_id].current_alloc_type,
                            l1_transfer_data[gas_id].starting_time);

        l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
    }
    else
    {
      MSG_GERAN_HIGH_2_G("GTA_PS_RACH :Allow GTA during PS RACH : Suspension FN %d curr fn %d",
                          l1_idle_data->random_access_data.suspension_FN_GTA,GSTMR_GET_FN_GERAN(gas_id) );

      /*check if there is any RACH pending which can collide with suspension gap. If yes, move it after gap*/
      if(gl1_msg_get_rach_state(gas_id) == RACH_STATE_WAITING_FOR_BLOCK_START)
      {
        uint32 new_rach_delay =0;
        boolean is_rach_after_gap = FALSE;

        //Check what is the RACH delay configured at msg layer*/
        rach_delay = gl1_msg_get_rach_delay(gas_id);

        /*if collision*/
        if(TRUE == gl1_check_if_rach_delay_collides_with_gap(rach_delay,
                                                              l1_idle_data->random_access_data.suspension_FN_GTA,
                                                              5,/*margin in the beginning*/
                                                              G2X_PAGE_DUR_FRAMES,
                                                              &new_rach_delay,
                                                              &is_rach_after_gap,
                                                              gas_id))
        {
          l1_idle_data->random_access_data.updated_ra_delay = new_rach_delay;
        }
        /*if rach is after gap*/
        else if (is_rach_after_gap == TRUE)
        {
          l1_idle_data->random_access_data.updated_ra_delay = rach_delay;

        }
        /*rach is before the gap*/
        else
        {
        MSG_GERAN_HIGH_1_G("GTA_PS_RACH :MK Rach before the gap updated_ra_delay %d",
                          l1_idle_data->random_access_data.updated_ra_delay);
          //l1_idle_data->random_access_data.updated_ra_delay = RACH_DELAY_INVALID;
        }
      }
      else
      {
          MSG_GERAN_HIGH_1_G("GTA_PS_RACH :MK Not at block start updated_ra_delay %d",
                            l1_idle_data->random_access_data.updated_ra_delay);
          //l1_idle_data->random_access_data.updated_ra_delay = RACH_DELAY_INVALID;
      }
    }
  }
  else
  {
    MSG_GERAN_HIGH_2_G("GTA_PS_RACH :Dont Allow GTA : Suspension FN too close to handle %d curr fn %d",
                        l1_idle_data->random_access_data.suspension_FN_GTA,GSTMR_GET_FN_GERAN(gas_id) );

    l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
  }

  GL1_ISR_UNLOCK(gas_id);

}

#endif /*FEATURE_GTA_DURING_PS_RACH*/


/*===========================================================================

FUNCTION  grm_handle_ps_unlock_request

DESCRIPTION
  This function contains the common handling of PS unlock callbacks

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/


void grm_handle_ps_unlock_request( trm_unlock_callback_data *unlock_data_p, grm_client_enum_t client_id, gas_id_t gas_id)
{
  if ( unlock_data_p )
{
#ifdef FEATURE_GTA_DURING_PS_RACH
  grm_client_enum_t winning_client_id;
  trm_reason_enum_t winning_reason;
  idle_data_T *l1_idle_data;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

    winning_reason = unlock_data_p->winning_client_info.reason;
    winning_client_id = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->winning_client_info.client_id);

    l1_idle_data = &l1_idle_data_store[gas_id];
    l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    /* open a GTA gap  for othertech's Page reason & we are doing RACH for PS if not blocked by higher layers*/
    if((gl1_is_geran_tuneaway_allowed(gas_id)) &&
       (GERAN_TA_SUPPORTED == geran_nv_g2x_ta_supported(winning_client_id)) &&
       (winning_reason == TRM_DEMOD_PAGE)&&
       (unlock_data_p->event == TRM_UNLOCK_BY) &&
       (l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE) &&
       (l1_idle_data->random_access_data.rach_for_ps_service == TRUE) &&
       (l1_idle_data->non_updated_lai_in_nmo1 == FALSE)
      )
    {
       gl1_grm_rach_ps_gta_abort_callback_t  abort_cb_msg;
       gl1_grm_rach_ps_gta_abort_callback_t  *abort_cb_msg_p;
       abort_cb_msg_p = &abort_cb_msg;

       abort_cb_msg_p->winning_client_id     = winning_client_id;
       abort_cb_msg_p->event                 = unlock_data_p->event;
       abort_cb_msg_p->unlock_in_sclk        = unlock_data_p->unlock_by_sclk;
       abort_cb_msg_p->gas_id                = gas_id;
       abort_cb_msg_p->winning_client_reason = winning_reason;

       /* set the inter-task header */
       abort_cb_msg_p->header.message_set = MS_L1_L1;
       abort_cb_msg_p->header.message_id  = L1_GTA_DUR_RACH_PS_CALLBACK;
       PUT_IMH_LEN( sizeof( gl1_grm_rach_ps_gta_abort_callback_t ) - sizeof( IMH_T ), abort_cb_msg_p );

       MSG_GERAN_HIGH_0_G( "GTA_PS_RACH :unlock for OtherTech Page reason");

       /*send msg to task to process it from task's context*/
       (void)L1_isr_send_message(GS_QUEUE_LAYER_1, abort_cb_msg_p, FALSE, gas_id);

    }
    else if ((unlock_data_p->event == TRM_UNLOCK_CANCELLED) &&
             (l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE) &&
             (l1_idle_data->random_access_data.rach_for_ps_service == TRUE))
    {
      l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
      if((l1_idle_data->random_access_data.updated_ra_delay != RACH_DELAY_INVALID)&&
              (!gl1_msgi_check_ft_handler_registered(gas_id,GL1_MSG_FT_HDLR_UL)))
      {
           /*GTA gap is cancelled. So configure RACH.when substate is L1_ACCESS_GTA_FW_LOAD_WAIT
             all conditions will be checked and RACH will be scheduled*/
            l1_tskisr_blk->sub_state = L1_ACCESS_GTA_FW_LOAD_WAIT;
      }
      MSG_GERAN_HIGH_0_G( "GTA_PS_RACH :TRM_UNLOCK_CANCELLED , make suspension FN invalid");
    }
    else if ((unlock_data_p->event == TRM_UNLOCK_IMMEDIATELY) &&
             (l1_tskisr_blk->l1_state == L1_RANDOM_ACCESS_MODE) &&
             (l1_idle_data->random_access_data.rach_for_ps_service == TRUE))
    {
      l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
      L1_send_MPH_L1_DS_ABORT_IND(gas_id, RR_DS_ABORT_TRANSFER, 200);
      MSG_GERAN_HIGH_0_G( "GTA_PS_RACH :TRM_UNLOCK_IMMEDIATELY: PS ABORT");
    }
    else
    {

      MSG_GERAN_HIGH_5_G( "GTA_PS_RACH : Ignoring unlock event %d for Client %d l1_state %d trm_reason %d TA allowed by upper layers %d",
                           unlock_data_p->event,
                           winning_client_id,
                           l1_tskisr_blk->l1_state,
                           winning_reason,
                           gl1_is_geran_tuneaway_allowed(gas_id));
    }
#else
    if (unlock_data_p->event != TRM_UNLOCK_CANCELLED)
    {
      MSG_GERAN_HIGH_4_G( "Client %d Ignoring unlock event %d for Client %d",
                        client_id,
                        unlock_data_p->event,
                        unlock_data_p->winning_client_info.client_id);
    }
#endif
  }
}

/*===========================================================================

FUNCTION  grm_ps_access_abort_callback

DESCRIPTION
  This function handles an abort callback when entering
  PS state via TRM_ACCESS

DEPENDENCIES
  GL1 will honour the unlock callback for unlock immediate  in PS Access
  Honouring unlock by is TBD

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_ps_access_abort_callback( trm_unlock_callback_data *unlock_data_p )
{
  if ( unlock_data_p )
  {
    grm_client_enum_t unlock_client = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);
    gas_id_t gas_id = check_gas_id(gl1_map_client_id_to_gas_id( unlock_client ));
    volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    if ( (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE) &&
         (l1_tskisr_blk->sub_state != L1_TRANSFER_INIT))
    {
      /* In transfer mode let the TBF unlock callback handle this */
      grm_gta_preemption_callback(unlock_data_p);
    }
    else
    {
      MSG_GERAN_HIGH_0_G( "Received PS access abort callback");
      grm_handle_ps_unlock_request( unlock_data_p, unlock_client, gas_id );
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_ps_traffic_abort_callback

DESCRIPTION
  This function handles an abort callback when entering
  PS state via TRAFFIC

DEPENDENCIES
  This callback should never be called by TRM

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_ps_traffic_abort_callback( trm_unlock_callback_data *unlock_data_p )
{
  if ( unlock_data_p )
  {
    grm_client_enum_t unlock_client = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);
    gas_id_t gas_id = check_gas_id(gl1_map_client_id_to_gas_id( unlock_client ));

    if (unlock_data_p->event != TRM_UNLOCK_CANCELLED)
    {
      MSG_GERAN_ERROR_2_G( "Ignoring unlock event %d for Client %d",
                        unlock_data_p->event,
                      unlock_data_p->winning_client_info.client_id );
    }
  }

  return;
}

/*===========================================================================

FUNCTION  grm_ps_bg_traffic_abort_callback

DESCRIPTION
  This function handles an abort callback when entering
  PS state via TRAFFIC

DEPENDENCIES
  GL1 will honour the unlock callback for unlock immediate  in PS Access
  Honouring unlock by is TBD

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void grm_ps_bg_traffic_abort_callback( trm_unlock_callback_data *unlock_data_p )
{
  if ( unlock_data_p )
  {
    grm_client_enum_t unlock_client = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);
    gas_id_t gas_id = check_gas_id(gl1_map_client_id_to_gas_id( unlock_client ));
    volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    if ( l1_tskisr_blk->l1_state == L1_TRANSFER_MODE )
    {
      /* In transfer mode let the TBF unlock callback handle this */
      grm_gta_preemption_callback(unlock_data_p);
    }
    else
    {
      unlock_client = grm_map_trm_client_id_to_grm_client_id(unlock_data_p->unlock_client);
      gas_id = gl1_map_client_id_to_gas_id( unlock_client );
      gas_id = check_gas_id(gas_id);

      MSG_GERAN_HIGH_0_G( "Received PS background traffic abort callback");

      grm_handle_ps_unlock_request( unlock_data_p, unlock_client, gas_id );
    }
  }
  return;
}
#endif  /* (FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM) */

/*===========================================================================

FUNCTION  gl1_get_current_state

DESCRIPTION
  This function returns TRUE if GL1 state is DTM

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean gl1_is_current_state_dtm(gas_id_t gas_id)
{
  boolean gl1_in_dtm_mode = FALSE;
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  if(l1_tskisr_blk->l1_state == L1_DTM_MODE)
  {
    gl1_in_dtm_mode = TRUE;
  }
  return gl1_in_dtm_mode;
}

/*===========================================================================

FUNCTION  gl1_get_current_sub_state

DESCRIPTION
  This function returns current L1 sub state

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
byte gl1_get_current_sub_state(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  return l1_tskisr_blk->sub_state;
}
/*===========================================================================

FUNCTION  gl1_retrieve_bplmn_fail_count

DESCRIPTION
  This function retrieves the BPLMN failure counter for the specified GAS ID

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None

===========================================================================*/
uint32 gl1_retrieve_bplmn_fail_count(const boolean reset_count, const gas_id_t gas_id)
{
  uint32 fail_count = 0;

  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    fail_count = l1_utils_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].bplmn_fail_count;
    if (reset_count)
    {
      l1_utils_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].bplmn_fail_count = 0;
    }
  }

  return(fail_count);
}
/*===========================================================================

FUNCTION  gl1_increment_bplmn_fail_count

DESCRIPTION
  This function increments the BPLMN failure counter for the specified GAS ID

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_increment_bplmn_fail_count(const gas_id_t gas_id)
{
  if (gas_id <= GERAN_ACCESS_STRATUM_ID_MAX)
  {
    l1_utils_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)].bplmn_fail_count++;
  }

  return;
}

/*===========================================================================

FUNCTION  gl1_handle_mph_start_gprs_idle_mode_req

DESCRIPTION
  This function performs load of L1 with the configuration received from
  GRR. Please use this function as the common point for handling of
  MPH_START_GPRS_IDLE_MODE_REQ.

  inactive_cnf_send is used in the call from gpl1_dual_sim_idle_if.c (
  function gpl1_gprs_handle_multi_sim_idle) where we need to send the CNF
  and release the TRM if we are holding it.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_handle_mph_start_gprs_idle_mode_req (mph_start_gprs_idle_mode_req_t *message_ptr)
{
  idle_data_T *l1_idle_data;
  gprs_l1_idle_data_t *gprs_l1_idle_data;
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk;
  gprs_pl1_serving_cell_meas_T *gprs_serv_cell_meas_ptr;
  gas_id_t gas_id;
  inter_task_BCCH_allocation_T  ba_buffer;

  /*
   * initialise data pointers
   */
  gas_id = check_gas_id(message_ptr->gas_id);
  l1_idle_data = &l1_idle_data_store[gas_id];
  gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[gas_id];

  /* Load up the data from the message */
  if (message_ptr->ccch_or_pccch == PCCCH_52) /* Store GPRS 52 mfrm data */
  {
    MSG_GERAN_ERROR_0_G("MPH_START_GPRS_IDLE_REQ(52) unsupported on dual sim");
  }
  else if (message_ptr->ccch_or_pccch == CCCH_51) /* store 51 mfrm data into exisiting GSM L1 data structs */
  {
    MSG_GERAN_MED_0_G("MPH_START_GPRS_IDLE_REQ(51)");
    gl1_drx_set_sleep_inhibit(FALSE, gas_id);
    l1_sc_start(L1SCModeIdle, gas_id);

    /* Enable c value calculations on ccch during
     * circuit switched idle/access modes
     */
    gprs_serv_cell_meas_ptr->cal_c_value_51_idle = TRUE;

    gprs_l1_idle_data->ccch_or_pccch = message_ptr->ccch_or_pccch;
    l1_idle_data->campedon_cell_data =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_sys_info;
    l1_idle_data->campedon_cell_data.BS_PA_MFRMS = l1_override_bs_pa_mfrms(l1_idle_data->campedon_cell_data.BS_PA_MFRMS,gas_id);

    l1_idle_data->layer1_extra_gprs_sys_info.access_burst_type =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.access_burst_type;
    gprs_l1_idle_data->gprs_campedon_cell_data.power_control.alpha =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.alpha;
    gprs_l1_idle_data->gprs_campedon_cell_data.power_control.t_avg_w =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.t_avg_w;
    gprs_l1_idle_data->gprs_campedon_cell_data.power_control.t_avg_t =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.t_avg_t;
    gprs_l1_idle_data->gprs_campedon_cell_data.power_control.pc_meas_chan =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.pc_meas_chan;
    gprs_l1_idle_data->gprs_campedon_cell_data.power_control.n_avg_i =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.power_control.n_avg_i;

    /* copy over GSM/GPRS51 ARFCN data */
    gprs_l1_idle_data->gprs_campedon_cell_arfcn =
        l1_idle_data->campedon_cell_ARFCN;
    l1_idle_data->non_drx_timer =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.non_drx_timer;

    l1_idle_data->drx_timer_max =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.drx_timer_max;

    l1_idle_data->imsi_data.imsi_mod_1000 =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.imsi_mod1000;
    l1_idle_data->imsi_data.imsi_valid = TRUE;
    l1_idle_data->layer1_extra_gprs_sys_info.ext_utbf_nodata =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.ext_utbf_nodata;


#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
    l1_idle_data->layer1_extra_gprs_sys_info.read_pch_in_transfer =
       message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.read_pch_in_transfer;

    MSG_GERAN_HIGH_2_G(" ext ul tbf no data = %d read_pch_in_transfer %d",
             l1_idle_data->layer1_extra_gprs_sys_info.ext_utbf_nodata,l1_idle_data->layer1_extra_gprs_sys_info.read_pch_in_transfer);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

    /* Config the egprs quality measurements module using the bep period
     * broadcast on the BCCH.
     */
    gpl1_egprs_scell_qual_meas_broadcast_config(
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.bep_period,
        gas_id);

    if (message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.decode_bcch
        == FALSE)
    {
      /* stop decoding SI */
      l1_idle_data->si_mask = RR_L1_SI_TYPE_NONE;
      l1_idle_data->BCCH_mask = SERV_BCCH_MASK_NONE;
      l1_idle_data->ext_BCCH_mask = SERV_EXT_BCCH_MASK_NONE;
    }

    l1_sc_balist_set_serving_cell(l1_idle_data->campedon_cell_ARFCN, gas_id);

    /* Tell SCE that GRR will schedule ncell BCCH decodes. */
    l1_sc_set_bcch_autonomy( FALSE, gas_id);

    gprs_serv_cell_meas_ptr->alpha =
        gprs_l1_idle_data->gprs_campedon_cell_data.power_control.alpha;
    gprs_serv_cell_meas_ptr->pmax =
        l1_idle_data->campedon_cell_data.MS_TXPWR_MAX_CCH;

    if (l1_idle_data->campedon_cell_data.LB_MS_TXPWR_MAX_CCH_valid)
    {
      gprs_serv_cell_meas_ptr->lb_pmax =
          l1_idle_data->campedon_cell_data.LB_MS_TXPWR_MAX_CCH;
      gprs_serv_cell_meas_ptr->gprs_LB_MS_TXPWR_MAX_CCH_valid = TRUE;

    }
    else
    {
      gprs_serv_cell_meas_ptr->gprs_LB_MS_TXPWR_MAX_CCH_valid = FALSE;
    }

    L1_remove_oob_ba(&ba_buffer,
        &message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_sys_info.BA_list);
    l1_sc_set_ba_list(&ba_buffer, 0xFF, gas_id);

    /* Set multiband reporting parameter */
    l1_sc_balist_set_multiband_reporting(
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_sys_info.multiband_reporting,
        gas_id);

    l1_sc_set_paging_multiframes(l1_idle_data->campedon_cell_data.BS_PA_MFRMS,
        gas_id);


    /* Determine whether to read all paging blocks based on the current paging mode */
    l1_idle_data->paging_data.paging_mode =
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.paging_mode;

    if (l1_idle_data->paging_data.paging_mode == NORMAL_PAGING)
    {
      l1_idle_data->paging_data.all_paging_blocks = FALSE;
    }
    else
    {
      l1_idle_data->paging_data.all_paging_blocks = TRUE;
    }

    L1_calculate_paging_data(gas_id);

    if (l1_smscb_is_active(gas_id))
    {
      ARFCN_T cbch_arfcn = l1_smscb_get_arfcn(gas_id);
      ARFCN_T serving_cell_arfcn = gl1_get_serving_cell_arfcn(gas_id);
      if (((cbch_arfcn.num != serving_cell_arfcn.num)
          && (cbch_arfcn.band != serving_cell_arfcn.band))
          || (l1_smscb_get_bsic(gas_id) != gl1_get_serving_cell_BSIC(gas_id)))
      {
        l1_smscb_init(gas_id);
      }
    }

    l1_smscb_set_cb_supported(
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_sys_info.cbch_channel_information_valid,
        gas_id);

    l1_smscb_set_chan_type(
        message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_sys_info.cbch_channel_information.channel_type,
        gas_id);
    l1_smscb_set_arfcn(gl1_get_serving_cell_arfcn(gas_id), gas_id);

    l1_smscb_set_bsic(gl1_get_serving_cell_BSIC(gas_id), gas_id);


    if ( gl1_msg_is_sbd_enabled( gas_id ) )
    {
      l1_xmsi_copy_params(
        &message_ptr->idle_mode_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.xmsi,
        gas_id );
    }

    GL1_ISR_SAVE_LOCK(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if ( l1_tskisr_blk->main_command != L1_MULTI_SIM_IDLE_COMMAND )
#endif
    {
       l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
    }
    l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    /* Clear out any overrid reason */
    gpl1_set_invert_sys_info_trm_priority(FALSE,gas_id);
#endif
    GL1_ISR_SAVE_UNLOCK(gas_id);

    /* wake up layer 1 isr */
    gl1_drx_require_next_tick(gas_id);
  }
}

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================

FUNCTION  gl1_refresh_nv_items

DESCRIPTION
  This function re-reads carrier specific EFS / NV items.

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
void gl1_refresh_nv_items(gas_id_t gas_id)
{
  /* Currently none of the EFS/NV items are carrier specific.
   * No need to refresh anything */
  MSG_GERAN_HIGH_0_G("NV Refresh done in GL1");
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

void gl1_print_control_block( uint8 *msg, gas_id_t gas_id )
{
  /*Control block max_size=23 octets*/
   uint32  i;
   uint32 *rb_data_p;
   uint32  rb_data[6] = { 0 };

   rb_data_p = rb_data;

   if ( !msg )
   {
     return;
   }

   /* Order the Octets for printing. */
   for ( i = 0; i < ARR_SIZE( rb_data ); i++ )
   {
     *rb_data_p  = ( *msg++ << 24 );
     *rb_data_p |= ( *msg++ << 16 );
     *rb_data_p |= ( *msg++ << 8  );
     *rb_data_p |= ( *msg++ );
     rb_data_p++;
   }
    MSG_GERAN_DEBUG_OPT_3_G( "Radio Block = %08x %08x %08x",
                       rb_data[0], rb_data[1], rb_data[2] );

    MSG_GERAN_DEBUG_OPT_4_G( " %08x %08x %08x FN:%d",
                     rb_data[3], rb_data[4], rb_data[5], gl1_get_FN( gas_id ) );
}

#ifdef FEATURE_DATA_PDU_DEBUG
void gl1_print_data_block(uint8 *msg, uint8 cs, const gas_id_t gas_id)
{
  /* data block max size=75 octets */
  uint8  blk_size, byte_cnt;
  uint32 u1, u2, u3;
  int8   i;

  if (geran_nv_data_pdu_debug_enabled(GERAN_DATA_PDU_DEBUG_GL1) == FALSE)
  {
    return;
  }

  if (!msg)
  {
    return;
  }

  blk_size = cs_to_length[cs];
  byte_cnt = 0;
  
  MSG_GERAN_HIGH_3_G( "GL1 DL: Coding Scheme = %d, Block Size = %d, FN = %d", cs, blk_size, gl1_get_FN(gas_id));

  while (byte_cnt < blk_size)
  {
    u1 = u2 = u3 = 0UL;

    for (i = 3; (i >= 0) && (byte_cnt < blk_size); i--)
    {
      u1 |= (uint32)(msg[byte_cnt++]) << (i * 8);
    }

    for (i = 3; (i >= 0) && (byte_cnt < blk_size); i--)
    {
      u2 |= (uint32)(msg[byte_cnt++]) << (i * 8);
    }

    for (i = 3; (i >= 0) && (byte_cnt < blk_size); i--)
    {
      u3 |= (uint32)(msg[byte_cnt++]) << (i * 8);
    }

    MSG_GERAN_HIGH_3_G("GL1 DL BLK: %08x %08x %08x", u1, u2, u3);
  }
}
#endif /* FEATURE_DATA_PDU_DEBUG */

/*===========================================================================

FUNCTION  gpl1_select_band_to_req_trm

DESCRIPTION
 GRR Passes the Bands that are required for Acqusition.
 These bands are used to populate the grm_valid_gsm_sys_bands
 array

RETURN VALUE
 None

SIDE EFFECTS
 This function is predominately for SGLTE+G DSDA Asymmetric  band support
 Calling this function in other sys mode e.g DRDSDS is harmless and can only
 make for more efficient use of band coex
===========================================================================*/
void gpl1_select_band_to_req_trm( byte band_bitmap, grm_client_enum_t client_id,gas_id_t gas_id )
{
  /*Pscan: Only for PRX*/
  grm_info_t  *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( grm_info_p )
  {
    uint16 i;

    /* Clear any band information */
    for ( i = 0; i < MAX_BAND_INFO_GRM; i++ )
    {
      grm_valid_gsm_sys_bands[gas_id][i] = SYS_BAND_CLASS_NONE;
    }

    i = 0;

    /*check the bands requested by RR in pscan request */
    if ( ( band_bitmap & RR_L1_BAND_BITMAP_EGSM ) ||
         ( band_bitmap & RR_L1_BAND_BITMAP_PGSM ) )
    {
      grm_valid_gsm_sys_bands[gas_id][i++] = SYS_BAND_GSM_EGSM_900;

      MSG_GERAN_HIGH_0_G( "SYS_BAND_GSM_EGSM_900" );
    }

    if ( band_bitmap & RR_L1_BAND_BITMAP_DCS_1800 )
    {
      grm_valid_gsm_sys_bands[gas_id][i++] = SYS_BAND_GSM_DCS_1800;

      MSG_GERAN_HIGH_0_G( "SYS_BAND_GSM_DCS_1800" );
    }

    if ( band_bitmap & RR_L1_BAND_BITMAP_PCS_1900 )
    {
      grm_valid_gsm_sys_bands[gas_id][i++] = SYS_BAND_GSM_PCS_1900;

      MSG_GERAN_HIGH_0_G( "SYS_BAND_GSM_PCS_1900" );
    }

    if ( band_bitmap & RR_L1_BAND_BITMAP_GSM_850 )
    {
      grm_valid_gsm_sys_bands[gas_id][i++] = SYS_BAND_GSM_850;

      MSG_GERAN_HIGH_0_G( "SYS_BAND_GSM_850" );
    }

    grm_valid_gsm_sys_bands_cnt[gas_id] = i;

    /* Copy over all band info to request info */
    for ( i = 0; i < MAX_BAND_INFO_GRM; i++ )
    {
      grm_info_p->grm_freq_info.bands[i].band = grm_valid_gsm_sys_bands[gas_id][i];
    }

    grm_info_p->grm_freq_info.num_bands = grm_valid_gsm_sys_bands_cnt[gas_id];

    MSG_GERAN_HIGH_2_G( "gpl1_select_band_to_req_trm num_valid_bands:%d client %d",
                        grm_info_p->grm_freq_info.num_bands, client_id );
  }
  else
  {
    MSG_GERAN_ERROR_0_G( "gpl1_select_band_to_req_trm grm_info_p NULL" );
  }
}

/*===========================================================================

FUNCTION  gl1_read_lte_prune_cells_below_threshold

DESCRIPTION
  Reads efs item (if present) to set LTE cell rsrp threshold.
  Sets global variables used to flag LTE prune feature.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gl1_read_lte_prune_cells_below_threshold(gas_id_t gas_id)
{
  uint32 gl1_lte_cell_rsrp_threshold = 0;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /* default minimum RSRP to LTE_PRUNE_LOW_RSRP_LEVEL */
  l1_sc_wcdma_data_ptr->rr_params.gl1_lte_cell_rsrp_threshold = LTE_PRUNE_LOW_RSRP_LEVEL;

  if (geran_efs_read_primary(GERAN_EFS_LTE_CELL_RSRP_TH,
              &gl1_lte_cell_rsrp_threshold,
              sizeof(gl1_lte_cell_rsrp_threshold)) < 0)
  {
    MSG_GERAN_HIGH_1("gl1_lte_cell_rsrp_threshold not set (default) %i", l1_sc_wcdma_data_ptr->rr_params.gl1_lte_cell_rsrp_threshold);
  }
  else
  {
    l1_sc_wcdma_data_ptr->rr_params.gl1_lte_cell_rsrp_threshold += gl1_lte_cell_rsrp_threshold;
    MSG_GERAN_HIGH_1("gl1_lte_cell_rsrp_threshold set %i", l1_sc_wcdma_data_ptr->rr_params.gl1_lte_cell_rsrp_threshold);
  }
}

/*===========================================================================

FUNCTION  l1_override_bs_pa_mfrms

DESCRIPTION
  Reads efs item (NV=65770) to override BS Pa mfrms to 2.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
byte l1_override_bs_pa_mfrms(byte BS_PA_MFRMS, gas_id_t gas_id)
{

  if (gl1_is_drx_debug_enabled(gas_id))
  {
     if (BS_PA_MFRMS == 4 ||
         BS_PA_MFRMS == 6 ||
         BS_PA_MFRMS == 8)
     {
        MSG_GERAN_HIGH_1_G("EFS_DBG_ENABLED, bs_pa_mfrms updated from %d to 2", BS_PA_MFRMS);
        return (2);
     }
  }
  else
  {
     MSG_GERAN_HIGH_0_G("EFS_DBG_DISABLED, bs_pa_mfrms not updated ");
  }
  return BS_PA_MFRMS;
}
#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION  gl1_is_client_for_bplmn

DESCRIPTION
 This function takes a client and checks if it is being used for bplmn

RETURN VALUE
  TRUE - client is used for BPLMN

SIDE EFFECTS
  None.

===========================================================================*/
boolean gl1_is_client_for_bplmn( grm_client_enum_t   grm_client)
{
  if ( (grm_client == GRM_GPRS1) || (grm_client == GRM_GPRS2 ))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION  gl1_convert_gas_id_to_bplmn_client

DESCRIPTION
 This function takes the generic gas id  and returns the equivalent
 bplmn client id

RETURN VALUE
  Associated BPLMN client

SIDE EFFECTS
  None.

===========================================================================*/
grm_client_enum_t gl1_convert_gas_id_to_bplmn_client( gas_id_t  gas_id)
{
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    return GRM_GPRS1;
  }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    return GRM_GPRS2;
  }
  else
  {
    MSG_ERROR("ERROR : Unexpected gas Id %d", gas_id,0,0);
    return GRM_GPRS1;
  }
}

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
/*===========================================================================

FUNCTION  convert_sub_reason_to_els_activity

DESCRIPTION
  TRM has defined a list of ELS Activities.
  Map the current sub reason used for trm request to new ELS Activity
 
DEPENDENCIES
  trm_els_activity_enum_t

RETURN VALUE
  ELS activity (trm_els_activity_enum_t)


SIDE EFFECTS

===========================================================================*/
trm_els_activity_enum_t convert_sub_reason_to_els_activity(sys_proc_type_e_type sub_reason, gas_id_t gas_id)
{

  trm_els_activity_enum_t activity;

  switch (sub_reason)
  {
    case SYS_PROC_TYPE_PAGE:
      activity = TRM_ELS_PAGE;
      break;
    case SYS_PROC_TYPE_EXTENDED_PAGE:
      activity = TRM_ELS_EXTEND_PAGE;
      break;
    case SYS_PROC_TYPE_SYSTEM_INFO_READING:
    case SYS_PROC_TYPE_INTER_FCELL_MEASUREMENT:
    case SYS_PROC_TYPE_IRAT_MEASUREMENT:
      activity = TRM_ELS_MEASUREMENT;
      break;
    case SYS_PROC_TYPE_IRAT_CELL_RESELECTION:
    case SYS_PROC_TYPE_CCO:
      activity = TRM_ELS_IRAT;
      break;
    case SYS_PROC_TYPE_ACQUISITION:
      if ( l1_tsk_buffer[gas_id].l1_state == L1_MULTI_SIM_IDLE )
      {
        activity = TRM_ELS_MEASUREMENT;
      }else
      { 
        activity = TRM_ELS_OOS;
      }
      break;
    case SYS_PROC_TYPE_IMS_SMS: //cbch
    case SYS_PROC_TYPE_NONE:
    case SYS_PROC_TYPE_DIVERSITY_CHAIN:
    case SYS_PROC_TYPE_BPLMN_SEARCH:
    case SYS_PROC_TYPE_PS_REGISTRATION:
      activity =    TRM_ELS_OTHER;
      break;
    default:
      MSG_GERAN_ERROR_1_G("Unexpected sub_reason %d", sub_reason );
      return TRM_ELS_OTHER;
      break;
  }
  MSG_GERAN_HIGH_2_G("sub_reason %d els activity %d", sub_reason, activity );
  return activity;
}
#endif /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */
#endif /* defined ( FEATURE_DUAL_SIM ) */
#ifdef FEATURE_GTA_DURING_PS_RACH

/*===========================================================================

FUNCTION  gpl1_block_bound_51_multiframe

DESCRIPTION
  calculates the CCCH block boundary for 51 MF (PLEASE SEE TEH NOTE BELOW)*
  ** NOTE that It considers the FCCH/SCH frame as a block containing 1 frame

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void gpl1_block_bound_51_multiframe(uint32 start_fn, uint32 *suspension_FN_ptr, gas_id_t gas_id)
{
  uint8 i = 0, offset = 0;
  uint32 fn_mod_51 = start_fn % MULTIFRAME_51;
  uint32 multiframe_start_fn = (start_fn / MULTIFRAME_51) * MULTIFRAME_51;

  if(fn_mod_51 == 50)
  {
    offset = 50;
  }
  else
  {
    for(i = 0; i < 20; i++)
    {
      if((fn_mod_51 >= block_start_fn[i]) && (fn_mod_51 < block_start_fn[i + 1]))
      {
        offset = block_start_fn[i];
        break;
      }
    }
  }

  *suspension_FN_ptr = multiframe_start_fn + offset;

  MSG_GERAN_HIGH_4_G("GTA_PS_RACH :before block bound %d mod51 %d, after block bound %d mod51 %d",start_fn, fn_mod_51,
                     *suspension_FN_ptr,(*suspension_FN_ptr) % MULTIFRAME_51 );
}

/*===========================================================================

    FUNCTION  gl1_check_if_rach_delay_collides_with_gap

    DESCRIPTION
      checkf is rach delay collides with the suspesnion gap. If yes, it calculates the updated delay which is moved after the gap

    RETURN VALUE
      updated delay or the same delay as input if no collision

    SIDE EFFECTS
      None.

    ===========================================================================*/
boolean gl1_check_if_rach_delay_collides_with_gap  (uint32 delay,
                                                               uint32 suspensionFN,
                                                               uint32 margin,
                                                               uint32 duration,
                                                               uint32* new_rach_delay,
                                                               boolean* is_rach_after_gap,
                                                               gas_id_t gas_id)
{
  idle_data_T               *l1_idle_data;
  uint32 rach_FN = 0;
  uint32 suspension_end_FN = 0;
  boolean collision = FALSE;

  l1_idle_data = &l1_idle_data_store[gas_id];

  rach_FN = gl1_get_rach_fn_OTA(delay,l1_idle_data->campedon_cell_data.BS_CCCH_SDCCH_COMB, gas_id );

  suspension_end_FN = ADD_FN(suspensionFN, duration);

  MSG_GERAN_MED_3_G("GTA_PS_RACH : gl1_get_rach_delay_if_collides_with_gap rach_delay = %d rach_FN %d suspension FN %d",
                     delay, rach_FN, suspensionFN);

  /*if rach fn is after gap*/
  if(IS_FRAME_NUM_LATER (rach_FN, suspension_end_FN))
  {
    *is_rach_after_gap = TRUE;
    MSG_GERAN_MED_0_G("GTA_PS_RACH : RACH is after gap");
  }
  /*Check if RACH FN is colliding with the GTA gap, move rach fn after the gap*/
  else if(CHECK_IF_FN_COLLIDES_WITH_GAP(rach_FN, SUB_FN(suspensionFN, margin), suspension_end_FN ))
  {
    /*move RACH 4 frames after the gap*/
    rach_FN = ADD_FN(suspension_end_FN, FRAMES_TO_MOVE_RACH);
    //delay = SUB_FN(rach_FN, GSTMR_GET_FN_GERAN(gas_id));

    *new_rach_delay = delay + FRAMES_TO_MOVE_RACH + duration;
    collision = TRUE;
    MSG_GERAN_MED_3_G("GTA_PS_RACH : RACH colliding with TA gap , Update RACH Delay %d rach fN %d, new_rach_delay %d", delay, rach_FN, *new_rach_delay);
  }
  return collision;
}

/*===========================================================================

    FUNCTION  gl1_get_rach_fn_OTA

    DESCRIPTION
      calculate teh RACH tx FN based on the ccch_comb or non comb channel configuration

    RETURN VALUE
      calculated rach FN

    SIDE EFFECTS
      None.

    ===========================================================================*/
uint32 gl1_get_rach_fn_OTA(uint32 delay, boolean ccch_sdcch_comb, gas_id_t gas_id )
{
  uint32 rach_fn = 0;
  uint8 index = 0;
  uint8 no_of_mf = delay / NO_RACH_FRAMES_51_MF_CCCH_COMB;

  index = delay % NO_RACH_FRAMES_51_MF_CCCH_COMB;

  if(ccch_sdcch_comb == TRUE)
  {
    rach_fn = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), ADD_FN((no_of_mf * MULTIFRAME_51 ), rach_FN_ccch_comb[index]));
  }
  else
  {
    rach_fn = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), delay);
  }

  MSG_GERAN_MED_4_G("GTA_PS_RACH : gl1_get_rach_fn_OTA delay %d, ccch_comb %d, rach_FN %d curr FN %d",
                     delay,ccch_sdcch_comb,rach_fn, GSTMR_GET_FN_GERAN(gas_id));
  return rach_fn;
}
#endif /*FEATURE_GTA_DURING_PS_RACH*/

/*===========================================================================

FUNCTION  l1_find_bcch_mcpm_change
DESCRIPTION
  This function requeste MCPM change at start/end of find_bcch so that
  clock speed is increased during find bcch for RF processing load.

===========================================================================*/
void l1_find_bcch_mcpm_change(boolean desired_find_bcch_start_state, gas_id_t gas_id)
{
   /* Ensure higher clock speed for find_bcch, required by RF driver */
   if(desired_find_bcch_start_state != l1_utils_find_bcch_mcvs_on[gas_id])
   {
     MSG_GERAN_HIGH_1_G("MCPM change for find_bcch %d", desired_find_bcch_start_state);
     l1_utils_find_bcch_mcvs_on[gas_id] = desired_find_bcch_start_state;
     gl1_clkdata_speed(desired_find_bcch_start_state, L1_FIND_BCCH_SPEEDUP, L1_DED_DATA_NULL,L1_BCCH, gas_id);
   }
   else
   {
     MSG_GERAN_HIGH_0_G("No change in requested find_bcch MCPM state");
   }
}

/*===========================================================================
FUNCTION  gl1_get_serving_cell_meas

DESCRIPTION
  This function provides the serving cell measurements
  
 Arguments
   as id
   
 Return value:   ( void *) &gpl1_serving_cell_meas_t

===========================================================================*/
void* gl1_get_serving_cell_meas ( sys_modem_as_id_e_type as_id )
{
  gas_id_t gas_id = geran_map_nas_id_to_gas_id( as_id );
  l1_serving_cell_meas_T*  l1_serving_cell_meas_ptr = &l1_serving_cell_meas[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  dBx16_T   rx_power_average = 0;
  uint8 rx_lev  = RXLEV_INVALID;
  boolean gl1_was_dtx_used;
  void *gpl1_serving_cell_meas_ptr = NULL;
  
  GL1_ISR_SAVE_LOCK(gas_id);

   if ( 0 != l1_serving_cell_meas_ptr->no_of_measurements )
  {
    rx_power_average   =  (dBx16_T)( l1_serving_cell_meas_ptr->rx_power / l1_serving_cell_meas_ptr->no_of_measurements );
    rx_lev = L1_map_dBm_x16_to_rxlev( rx_power_average );
  }

  switch( l1_tsk_buffer[gas_id].l1_state )
  {
   case L1_IDLE_MODE:
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )   	
   case L1_MULTI_SIM_IDLE:
   case L1_MULTI_SIM_DECOUPLED:
#endif   	
    gpl1_serving_cell_meas[gas_id].l1_state =  l1_tsk_buffer[gas_id].l1_state;
    gpl1_serving_cell_meas[gas_id].rxlev    =  rx_lev;
    gpl1_serving_cell_meas[gas_id].rxqual   =  RXQUAL_INVALID ;
    gpl1_serving_cell_meas[gas_id].snr      =  ( l1_serving_cell_meas_ptr->snr/16 ) ;
    break;

  case L1_RANDOM_ACCESS_MODE:
    gpl1_serving_cell_meas[gas_id].l1_state =  l1_tsk_buffer[gas_id].l1_state;
    gpl1_serving_cell_meas[gas_id].rxlev    =  rx_lev;
    gpl1_serving_cell_meas[gas_id].rxqual   =  RXQUAL_INVALID;
    gpl1_serving_cell_meas[gas_id].snr      =  SNR_INVALID;
    gpl1_serving_cell_meas[gas_id].stale_meas   =  FALSE;
    break;

  case L1_DEDICATED_MODE:
  case L1_DTM_MODE:
    gl1_was_dtx_used = gl1_get_dtx_used_this_period(gas_id);
    gpl1_serving_cell_meas[gas_id].l1_state =  l1_tsk_buffer[gas_id].l1_state;
    gpl1_serving_cell_meas[gas_id].rxlev    =  rx_lev;
    gpl1_serving_cell_meas[gas_id].rxqual   =  gl1_was_dtx_used? gl1_msg_get_rxqual_sub( gas_id):gl1_msg_get_rxqual_full((boolean*)&meas_report_valid,gas_id) ;
    gpl1_serving_cell_meas[gas_id].snr      =  SNR_INVALID;
    gpl1_serving_cell_meas[gas_id].stale_meas   =  FALSE;	
    break;
	
  case L1_TRANSFER_MODE:
    gpl1_serving_cell_meas[gas_id].l1_state =  l1_tsk_buffer[gas_id].l1_state;
    gpl1_serving_cell_meas[gas_id].rxlev    =  rx_lev;
    gpl1_serving_cell_meas[gas_id].snr      =  SNR_INVALID;
    gpl1_serving_cell_meas[gas_id].stale_meas   =  FALSE;	
    break;	
	
   default:
     gpl1_serving_cell_meas[gas_id].l1_state =  l1_tsk_buffer[gas_id].l1_state;
     gpl1_serving_cell_meas[gas_id].rxlev    =  RXLEV_INVALID;
     gpl1_serving_cell_meas[gas_id].rxqual   =  RXQUAL_INVALID;
     gpl1_serving_cell_meas[gas_id].snr      =  SNR_INVALID;
     gpl1_serving_cell_meas[gas_id].stale_meas   =  FALSE;	 
     break;
  }

  GL1_ISR_SAVE_UNLOCK(gas_id);

  gpl1_serving_cell_meas_ptr = (void *)&gpl1_serving_cell_meas[gas_id];

  return ( gpl1_serving_cell_meas_ptr );
}

#if defined (FEATURE_GSM_SIMULATE_GL1_RF)  || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
#error code not present
#endif /* FEATURE_GSM_SIMULATE_GL1_RF */
/* EOF */

