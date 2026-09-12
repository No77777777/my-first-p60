/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                G L 1  F R A M E   L A Y E R

GENERAL DESCRIPTION
   This file together with gl1_hw_gsm.c and gl1_hw_gprs.c contains the
   procedures for the frame layer of non portable layer 1.   The procedures
   used herein are intended for sole use by the message layer functions and
   not intended for use by the portable layer 1 software.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_hw_clk_ctl.c#7 $
$DateTime: 2021/12/01 22:37:06 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
30/11/21   sal      CR3069703 Add mutex protection for mcpm_gsm_param read/write.
12/08/21   sal      CR3010819 Ensure ongoing MCPM state update in DPC is MCPM_INIT before setting main command to ENTER_NULL_COMMAND .
04/02/20   nv		CR2615257 Reverting the hack made to handle L1_TIME_LINE_DELAY scenario in HA as part of FR:51554
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
05/11/15   sp       CR933554 TCH Loopbacks clock bump to 384MHz
27/04/17   sk       CR2039341 Reverting the changes of CR2023003
03/03/17   sk       CR2023003 Vote for higher DDR clock for voice call in DRX only mode
22/09/16   akm      CR1067259 CPU clock boost for G2W IRAT in QTA gap
31/08/16   akm      CR1026866 Check all the indexes of the buffer for mcpm ongoing activity
25/08/16   dg       CR1056221 Send MCVS req of 384 MHZ while initiating any G2X IRAT search in dedicated mode
30/06/16   hd       CR1035752 Send MCPM_GERAN_DATA_STOP_REQ only when it was not sent before
30/06/16   mko      CR1008652: Don't consider MCPM INIT request as invalid request type in GL1 code (Applicable only for TABASCO)
17/06/16   ng       CR1030473 Send MCVS request of 384 MHZ at the time of initiating G2X search when GL1 is in Transfer mode (modify fix of CR 1021833 & 1025293)
13/06/16   ng       CR1027483 Setting the MCVS clock data in correct order, essential to add this fix with CR1025293 & CR1021833
10/06/16   ng       CR1025293 Extend fix of CR 1021833: Send MCVS request of 384 MHZ at the time of initiating G2X search with HMSC ON
31/05/16   dg       CR1021833 Send MCVS request of 384 MHZ at the time of initiating G2L search with HMSC ON
30/03/16   dg       CR990072 Call MCPM API for BIMC boost before starting G2L search in packet transfer mode with AEQ ON
25/01/16   sp       CR958352 Dont request for MCVS Change if already using the same configuration.
11/01/16   sk       CR960443 MCVS changes for Jacala
07/12/15   hd       CR947930 Do not place 230.4 MHz MCPM request again when switching off 288 MHz MCPM request
10/12/15   hd       CR949845 Redefine the macro MCPM_REQ_STATE so that it doesn't have dependency on MCPM
03/12/15   dp       CR929648 reduce power usage when waiting for TRM during find_bcch
30/11/15   cja/hd   CR944880 Check if G2X MCPM stop request queued up in DPC task after start request
25/11/15   cja      CR942064 Add mcvs speed bump for G2L and G2T in dedicated mode 
01/09/15   pg       CR898692 increase EGPRS+AEQ/RxD clock bump to 499.2Mhz
05/08/15   cws      CR881986 Use only gsm1/gsm2 client for trm asdiv request
23/07/15   cjl      CR878351 fix compiler warning
21/07/15   cws      CR873163 fix asdiv/pchxfer compile warnings
14/07/15   cws      CR845909 Switch antenna when AGCH decode fails
11/06/15   pg       CR850142 Featurise dedicated parallel SCH clock bump to avoid unnecessary MCPM/NPA calls
14/04/15   hd       CR821910: Call G2T and G2W init once MCPM is configured.
07/05/15   fan/akh  CR834237 FR19329 Full G wake-up cancellation + code cleanup
05/07/15   zc       CR826075 Band Specific ASDIV Enable
01/05/15   cws      CR794322 Correct the switching logic to be consistent with CR766904
27/04/15   jj       CR 828973 increase clock speed at find bcch mode
20/04/15   ip       CR811868 Dual Multisim mode code changes + debug overhaul
31/03/15   pa       CR815621:GRM restructuring for TH 2.0.
10/12/14   dg       CR765859 Do not go to sleep if MCPM is not updated for G2X MEAS STOP activity
15/12/14   cs       CR754853 Add support for Parallel SCH detection handling
10/12/14   ws       CR768376 Q6 compile warning fixes
26/11/14   jj       CR 760174 fix  compiler warning
01/10/14   zc     CR707083: FR 22623: ASDiv (+tuner) for TDD-LTE/TDS/G+G DR-DSDS w/ TDS/G CSFB
13/11/14   npt      CR746180 Remove KW errors
09/09/14   cgc      CR721718 add l1_sc_send_irat_g2l_init() for FEATURE_GSM_TO_LTE changes to fix compile errors for non-lte build
04/09/14   am       CR682760 Addressing KW GL1 P1 errors
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
31/07/14   cos      CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
30/07/14   ssh      CR701245: Avoid running an adjoining activity after rude wake up if the FW/RF is being initialized in that frame
15/07/14   dg       CR694542 GL1 to send INIT_REQ to LTE only when it has received LIST REQ
                    from RR with valid LTE neighbors
09/07/14   dg       CR682537 Send INIT_REQ to ML1 only when MCPM is configured for G2L Meas
18/06/14   cos      CR682031 - GL1 control algorithm and changes for supporting AEQ PHASE 4
02/06/14   jj       CR 688565 re-name function get_gsm_client_from_gas_id to
                    gl1_map_gas_id_to_client_id  under asdiv flag
12/06/14   ssh      CR671029: On G+G wake up collision defer wake up by 1 frame
10/06/14   jj       CR 668965 merge ASDIV from DI.3.0 to BO 2.0
10/06/14    jj      CR 668965 merge ASDIV from DI.3.0 to BO 2.0
09/06/15   cja      CR674692 Make MCPM DPC queue failure a fatal error
23/05/14   pg       CR669538 Increase Q6 clk speed when AEQ is enabled for voice
20/05/14   ssh      CR667235 Whilst reprogramming sleep on WUP cancel always check if all conditions met
12/05/14   zc       CR660275: Do not set asdiv wakeup config unless granted TRM
01/05/14   zc       CR655687: SM ASDIV PCH Failure routine using deprecated TRM function
17/04/14   jj       CR 650872 send switch complete based on  call back from rf in pch crc fail
16/04/14   ws       CR 645067 - Increase Q6 clk speed when AEQ2A is enabled in EGPRS mode
11/04/14   jj       CR 647256  client ID correction in trm message to adapt for multisim
01/04/14   xz       CR634903 Clear maintain_tds_ref_count timely if it is not forewakeup
19/03/14   ws       CR635101  Resolve P1 Critical Klocwork errors in GL1 code space
11/03/14   jk       CR628002:Request MCVS clock frequency change though it is same as the previous request
18/02/14   am       CR614496 GL1 to not call MCPM for ciphering clock if awake
30/01/14   cgc      CR608970 Compiler Optimisation issue ensure gl1_hw_mcpm_config_g2t_stop handler is deregistered.
29/11/13   ssh      CR583144: On Rude wakeup make sure pending mcpm sleep req is done before initiating a mcpm wake up req.
12/12/13   cja      CR585605 Only turn tx on if not already on (so do not get 2 Tx config
01/12/13   sk       CR583361 Correcting the multisim sys mode check
02/10/13   pjr      CR552448 DDR Frequency plan implementation - l1_arbitration modification
28/11/13   cja      CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
26/11/13   cja      CR581785 Add GL1 MCVS manager so correct MCVS frequency selected
22/11/13   cja      CR579775 On Dime use MCVS for settinig clocks for TCH AEq and speed-up
22/11/13   cja      CR455527 Remove featurisation from files in API directory
17/10/13   cja      CR562798 Clear maintain ref count when shutdown (after IRAT cnf)
21/01/13   jj       CR 443405  modified implementation of dynamic tx-div disable
15/03/13   jj       CR 463546 tx -div- improvements
21/01/13   jj       CR 443331 GSM Standalone ASDiv Function Fail After Several Switch
19/12/12   jj       CR 433242 HTC DS/DA Requirement: L1-TRM Interface -  tx-diversity
26/11/12   jj       CR 424199 Adding Scell measurement for tx-diversity when BA list is empty
23/09/13   cja      CR548633 Set mcpm tx on flag, even if Tx already on.
03/09/13   cja      CR538140 Add mcpm_tx_on flag for race condition in sending Tx Config
10/09/13   cs       CR532298 Integrate all wakeup-mgr changes for Multi-SIM
10/07/13   sai      CR509527: Ensure wakeup cancel enabled after MCPM sleep req completed.
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
07/08/13   pa     CR521830: Bolt: Support for MCPM GSM Non EV Data setting for DL
09/08/13   zf       CR527223: Send wrong STOP REQ to MCPM for sub2.
08/08/13   cja      CR519510 Use DPC to call MCPM, send Tx Config only when MCPM ready
01/07/13   sp       CR508123: Set GL1 MCPM State Correctly, Use of gl1_mcpm_state_type enum
18/06/13   cs       Major TSTS Triton Syncup
04/06/13   cs       Added third MCPM client support for TSTS
24/05/13   kb/npt   CR459207 - Maintain TDSCDMA frame reference count through
                    GSM sleep when there are TDSCDMA neighbours
17/05/13   cs       TSTS Updates
28/03/13   pg       CR46732/4474817 Do not disable G2W clocks whilst OLS is pending
18/03/13   cja      CR464426 Relinquish W/T nbrs after handover complete so W/T STMR stays on
18/02/13   cja      CR453465 Do not set MCPM AEq setting as part of data start request.
07/12/12   jv       CR438196: Using RF Task API gl1_hw_rf_idle_frame_processing() for performing idle frame processing.
14/12/12   cja      CR426248 Wait for HW Tx Off before MCPM request for voice stop.
07/12/12   cja      CR429173 Remove Dime compiler warnings
06/12/12   npt      CR428630 - Maintain WCDMA frame reference count while in sleep
22/11/12   cja      CR414098 For Dime only need W clks if W neighbours
14/11/12   npt      CR420954 - Added changes for power collapse
23/10/12   npt      CR413635 - Port sleep code for DIME modem
01/05/12   ws       CR355391 - Deprecate arm.h, armasm.h and hw.h
26/03/12   pg       Make FEATURE_MCPM and FEATURE_DYNAMIC_CLOCK_VOLTAGE_SCALING mutually exclusive
14/03/12   pg       Add support for local HWIO macros (msm.h deprecated)
14/03/12   pg       Add support for QuRT
14/03/12   pg       Add support for DAL interrupt controller (tramp deprecated)
21/02/12   cja      CR338055 Recovery for Nikel RxFE Stuck
13/12/11   cja      CR325153 Use MCPM_GERAN_PARMS_UPDATE_REQ
05/12/11   cja      CR323200 Run MCPM start req via dpc, wait for it to complete
17/11/11   cja      CR318115 Turn off MCPM W nbr during TCH/TRANS if no W
17/11/11   cja      CR319954 Do not sleep unless GSM MCPM is idle
07/11/11   cja      CR311154 Add request MCPM AEq on for AMR
18/10/11   jj       CR310766 Support of GSM thermal Read in idle Frame
20/10/11   cja      CR313577 Ensure WSTMR on until W requests it for G2W handover
12/10/11   ws       CR312154 - Featurise remaining clkrgm API calls uder FEATURE_MCPM
03/10/11   cja      CR310347 Clear voice active before requesting MCPM voice stop as it runs in dpc
09/09/11  cja/og    CR301686. Use DPC for MCPM update state
05/09/11   cja      CR305056 Increase clock speeds for non-signalling mode (FTM)
16/08/11   cja      CR302396 Always request MCPM W neighbour so WSTMR on for sleep
11/07/11   cja      CR295252. Correct request to MCPM for dynamic AEq.
20/06/11   cja      Use hw_tx_on as flag to stop MCPM voice.
31/05/11   ws       Fixed Compile warning in gl1_hw_get_mcpm_state()
24/05/11   cja      Add get gl1_mcpm_state
26/04/11   cja      Correct Nikel compiler warnings
15/04/11   ws       CR282280 - Move IRAT specific NPA calls to DPC
14/03/11   nf       CR281205 Reworked 9K clock voting
20/04/11   cja      Nikel cipher clock gating
18/04/11   cja      Fix compiler warnings
30/03/11   cja      Initial IRAT for Nikel
21/03/11   cja      Initial changes for Nikel
18/03/11   ws       Added Initial MCPM updates
12/01/11   ws       Added FEATURE_MCPM to avoid compile errors on MSM 8960
14/06/11   npt      CR 278528 - GSM latency node cancellation
15/04/11   ws       282280 - Move IRAT specific NPA calls to DPC
05/04/11   cs       Fix previous checkin with enable param
01/04/11   cs       CR282064 Remove the edge clock switch from task for non 9K
10/03/11   ws       CR276952 Added EBI bus request for EGPRS Data mover access
28/03/11   nf       CR281205 Reworked 9K clock voting
21/03/11   pg       CR279773: Move AGC_ENA reg setting into ISR context
22/02/11   ip/cs    CR267278 GP clock vote handled by RF drivers
15/02/11   ws       CR 275370 - Disable clk switching,Request 340 MIPS for duration
                    of GERAN RAT
21/01/11   ws       CR 263081 - Implement NPA Latency node of 1mSec
08/12/10   ws       CR 267264 - Merge NPA clients for CPU resource to 1 client to avoid
                    mutiple npa_cancel_request() which can take 1-2 mSecs to return
07/01/11   pg       Added FEATURE_GSM_GP_CLK_DISABLE for sourcing GSM clock
                    from MSM in all modes i.e. GP_CLK always disabled
04/01/11   nt       DSDS further omissions to WCDMA clock switching on C+G builds
21/12/10   nt       DSDS for C+G builds ensure that no WCDMA clocks are enabled
15/12/10   nt       DSDS - force on the tripler clock always in DSDS
02/12/10   nf       9K changed the way clk delays and clk switching is handled
18/11/10   nf       9K added handling for clock speed based on efs value
08/11/10   nf       9K:switch clock changes to task, remove FEE and added WCDMA RFX0 config
05/11/10   pg       CR263096: Revert to using W client for GSM_RX clock resource
                    for CM (reverting partial changes from CR253315).
07/10/10   ky       254466:Removed fw unused shared variables from the sw code
20/09/10   og       Added LTE clock regime client support.
15/09/10   pg       CR253315: Only enable/disable clock resources for GSM client on Genesis.
                    All W client clock resource changes will be handled by WL1.
13/09/10   cja      CR254232 Do not select WCDMA ChipXn for W2G, controlled by W.
29/07/10   cja      CR248652 Trek - only set MDSP register if MDSP is enabled and awake
02/08/10   tjw      Removed unused clk.h (CMI phase 1 re-clean up)
                    Mop up lint errors (no functional change)
28/07/10   ws       Added missing msm.h from previous checkin
24/07/10   pg       CR248443: Avoid forcing tripler on if SWC is not enabled
27/07/10   pm       Featurised TREK changes(CR244447) using FEATURE_GSM_DCDC_VOLTAGE_CONTROL
27/07/10   dv       CR242019: Correct Fix for this CR
23/07/10   ip       CR240253 Cancelled the dedicated MIPS for Edge
23/07/10   dv       CR242019 - Only maintain Frame Reference Count when not in GSM only mode
20/07/10   cs       KW Fixes
12/07/10   ws       CR 246146 Replace POSEIDON feature with XO feature for CLKRGM_RESOURCE_TCXO_PDM
                    resource since GENESIS uses TCXO and therefore the PDM clock resource
08/07/10   cja      CR244447. Add setting AGC_ENA for Trek
05/07/10   rc       Added support for GEA register access if GP clock is off in Q6
21/06/10   ip       CR241947 Fix race issue when cyphering enabled and GLLC
                    access a register just after clks are switched off.
01/07/10   nf       Enabling support for 9K RF Power Collapse
15/06/10   cja      Add WCDMA_Chip Xn to GSM mode when resetting W clocks (to match setting)
08/06/10   cs       CR241881 Only allocate NPA data structures once
02/06/10   cja      CR239657 Do not config all WCDMA mode clocks for W measurements
07/05/10   ip       CR235204 DCVS settings added for Poseidon 2
07/05/10   nf       9K changes for Clock Gating
29/04/10   cs       CR236825 Improve NPA to avoid problem with panic reset
22/04/10   ip       CR234360 Added int locks around GLLC function call for clocks
09/04/10   ip       CR221022 Added extra FEE state to block an estimation if required
07/04/10   ip       CR233138 Do not need to control TCXO_PDM clock resource
05/04/10   ip       CR233184 - mDSP clock scaling does not work, added workaround
25/03/10   cs       Fix debug clock info for poseidon
24/03/10   ip       CR231770 - Added GLLC client control for GSAC
24/03/10   ip       CR231696 - Changed mDSP default clock speed to 72MHz
24/03/10   ip       CR231770 remove the turning off of GP clock
24/03/10   ws       Fix compile errors For 9K for CR 229308
23/03/10   cs       CR230952 Deregister NPA idle client in EDGE mode
22/03/10   ip       CR229308 - GSAC clock being called without GP clock on
18/03/10   ip       CR230381 AGC and TX_COND clock OFF for POSEIDON
17/03/10   cs       Fix HCI for previous checkin
11/03/10   cs       CR229926 NPA robustness improvements
09/03/10   tjw      Remove unused declaration. Change arg name to avoid clash
                    with gl1_hw_clk_ctl.h
04/03/10   ip       CR228168 Added DEVMAN code changes for mDSP speed
02/03/10   ip       CR228169, Additional clock gating code changes for Poseidon
23/12/09   ws       Added volatile type for accessing pMdspHostIfBaseStore
03/11/09   ky       Disabling MODEM_MODE_SEL register write for GPLT VP build
26/01/10   ip       CR224128 Added SCMM clock gating changes
09/11/09   ip       Added RF calls under sleep control
30/11/09   rc       For Pos-2 G2W Configured WCDMA clocks & overrode SSRV offline clock to GSM
04/11/09   ip       Added first draft of NPA for cpu clock resource
29/10/09   ws       Removed clk_regime_config_wcdma_clocks() as no longer required
14/07/09   ip       Remove the calls for FEE resource, control is in sclk
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
26/03/09   ws       Merged clkregie API cleanup from Taxis modem
20/03/09   cs       Fix Lint errors
16/02/09   ip       CR 172435, turn on/off GSM_RXF_SAMP clock during sleep protocol
13/02/09   ip       CR 164895, Enable WSTMR resource on mode switch
13/02/09   ip       CR 171818, Added featurisation of TXC clock
10/02/09   ip       CR 141780, Added enabling and disabling of GSM CLKs on Dora
10/02/09   cs       Remove iRAT hard coded aclk values for Mustang uses new API
09/02/09   ip       CR 171625, Do not turn off the TXC clock for LCU in Dedicated
02/02/09   cs       New api for using SSRV0/1
30/01/09   ip       CR 170758, Coded FEE clock switching for LCU
30/01/09   ip       CR 169036, Coded SSRV0 clock switching for LCU
30/01/09   ip       CR 170015, Reset GCF 42.4.2.3.6
30/01/09   ip       CR 170625, Reset from W2G with CLKRGM49
30/01/09   ip       CR 167135 Added API call for sleep debug code
25/01/09   cs       Updates from Mustang iRAT work
05/01/09   cs       Fix the extern definition of gl1_hw_tx_on
19/12/08   ip       CR167592 Added additional LCU clock switching
01/12/08   cs       Add new split gl1_hw_clk_ctl.c from gl1_hw.c

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif


#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_clk_ctl.h"
#include "gl1_hw_clk_ctl_g.h"
#include "l1_sc_drv.h"
#include "gl1_hw_gsac.h"

#include "gtmrs.h"
#include "l1i.h"

#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "rfm.h"

#include "ftm.h"

#include "msg.h"            /* Diagnostics MSGs */
#include "err.h"            /* ERR_FATAL call   */

#include "rex.h"

#include "gl1_mutex.h"

#include "gfw_sw_intf.h"
#include "mcs_hwio.h"

#include "l1_drx.h"
#include "gl1_msgi.h"

#include "mcpm_api.h"
#include "gtmrs_g.h"

#include "gl1_msg_g.h"

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#include "gpl1_dual_sim.h"
#include "gl1_hw_sleep_ctl.h"
#endif /*( FEATURE_DUAL_SIM ) |( FEATURE_TRIPLE_SIM )*/
#include "geran_dual_sim_g.h"

/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/

/* MCVS Speedup Clock Speeds */
#define MCVS_Q6_CLK_KHZ_TCH_AEQ     230400
#define MCVS_Q6_CLK_KHZ_DATA        288000
#define MCVS_Q6_CLK_KHZ_TCH_AEQ_PH4 288000

#ifdef FEATURE_GERAN_JACALA_MODEM
  #define MCVS_Q6_CLK_KHZ_PSCAN       326000
  #define MCVS_Q6_CLK_KHZ_FIND_BCCH   326000
#else 
  #define MCVS_Q6_CLK_KHZ_PSCAN       384000
  #define MCVS_Q6_CLK_KHZ_FIND_BCCH   384000
#endif
/* Thor: vote for 480MHz should result in Q6 setting of 499.2MHz (SVS - same as 384MHz, so no power penalty)
   Tabasco: vote for 480MHz should result in Q6 setting of 480MHz (SVS_PLUS - voltage step from SVS, as 384MHz is top of SVS)
   Atlas: vote for 480MHz should result in Q6 setting of 499.2MHz (SVS - voltage step frmo LOW_SVS, as 403.2MHz is top of LOW_SVS)
*/
#define MCVS_Q6_CLK_KHZ_EGPRS       480000
#define MCVS_Q6_CLK_KHZ_PAR_ACQ     384000
#define MCVS_Q6_CLK_KHZ_DED_G2X     384000  /*For G2X in dedicated, always give req for 384 instead of 288*/

#define NUM_MCPM_GERAN_EVENTS       11

#ifdef FEATURE_SLPC_TRIPLE_SIM
void gl1_hw_mcpm_config_g2t_stop (sys_modem_as_id_e_type as_id);
#endif

/*===========================================================================

                     LOCAL STORAGE

===========================================================================*/

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

gl1_mcpm_state_type gl1_mcpm_state[NUM_GERAN_DATA_SPACES] =
                             { INITIAL_VALUE( GL1_MCPM_GERAN_STOP ) };

boolean gl1_mcpm_idle[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( TRUE ) };

boolean gl1_mcpm_tx_on[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( FALSE ) };



static uint16  gl1_mcvs_state[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };
static uint32  gl1_mcvs_khz [NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };

gl1_mcpm_update_gsm_dpc_param_type mcpm_gsm_param[NUM_GERAN_DATA_SPACES][MCPM_DPC_INDEX_MAX + 1];
static uint32  mcpm_dpc_index[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };
static mcpm_request_parms_type gl1_hw_g2w_stop_request_params[NUM_GERAN_DATA_SPACES];
#ifdef FEATURE_SLPC_TRIPLE_SIM
static mcpm_request_parms_type gl1_hw_g2t_stop_request_params[NUM_GERAN_DATA_SPACES];
#endif /* FEATURE_SLPC_TRIPLE_SIM */

static mcpm_request_type MCPM_REQ_STATE (mcpm_request_type req_in, gas_id_t gas_id);
/*===========================================================================

                     FUNCTION PROTOTYPES

===========================================================================*/
boolean  dynamic_tx_div_enable_disable  ;
/* ----------------------------------------------------------------------- */
/* Flag indicating if the tx chain clock is enabled or not                 */
/* ----------------------------------------------------------------------- */

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING

void gl1_hw_switch_antenna_tx_div (gas_id_t gas_id );

#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */
/*===========================================================================

                                FUNCTIONS

===========================================================================*/
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern boolean  gl1_hw_rfa_rf_gsm_set_antenna_req_cmd( boolean antenna_pos ,gas_id_t gas_id);
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */


/*===========================================================================

FUNCTION gl1_hw_mcpm_config_g2w_stop

DESCRIPTION
  GSM frame tick handler for calling MCPM config for G2W STOP. This would be
  registered as a FT handler if G2W_STOP was called in the first wakeup frame.

===========================================================================*/
void gl1_hw_mcpm_config_g2w_stop ( sys_modem_as_id_e_type as_id )
{
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);

   if (!gl1_drx_first_awake_frame(gas_id))
   {
      MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_hw_g2w_stop_request_params[gas_id]);
#ifdef FEATURE_SLPC_TRIPLE_SIM
      gl1_hw_sleep_maintain_ref_count (WCDMA,FALSE,gas_id);
#else
      gl1_hw_sleep_maintain_ref_count (FALSE,gas_id);
#endif /* FEATURE_SLPC_TRIPLE_SIM */

      gstmr_deregister_handler_geran(gl1_hw_mcpm_config_g2w_stop,geran_map_gas_id_to_nas_id(gas_id));
      MSG_GERAN_MED_1_G("ISR:gl1_hw_mcpm_config_g2w_stop ISR handler deregistered 0x%08x", gl1_hw_mcpm_config_g2w_stop);

#ifdef FEATURE_SLPC_TRIPLE_SIM
      /*
      ** added to ensure handler is deregistered, llvm optimisation was providing wrong address
      ** to the deregister function. g2t and g2w are mutually exclusive so only 1 should be registered.
      */
      if (gstmr_is_handler_registered_geran(gl1_hw_mcpm_config_g2t_stop, as_id))
      {
         gstmr_deregister_handler_geran(gl1_hw_mcpm_config_g2t_stop, as_id);
      }
#endif
   }
}

#ifdef FEATURE_SLPC_TRIPLE_SIM
/*===========================================================================

FUNCTION gl1_hw_mcpm_config_g2t_stop

DESCRIPTION
  GSM frame tick handler for calling MCPM config for G2T STOP. This would be
  registered as a FT handler if G2T_STOP was called in the first wakeup frame.

===========================================================================*/
void gl1_hw_mcpm_config_g2t_stop (sys_modem_as_id_e_type as_id)
{
   gas_id_t gas_id = geran_map_nas_id_to_gas_id(as_id);
   if (!gl1_drx_first_awake_frame(gas_id))
   {
      MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_hw_g2t_stop_request_params[gas_id]);
      gl1_hw_sleep_maintain_ref_count (TDS, FALSE, gas_id);

      gstmr_deregister_handler_geran(gl1_hw_mcpm_config_g2t_stop, geran_map_gas_id_to_nas_id(gas_id));
      MSG_GERAN_MED_1_G("ISR:gl1_hw_mcpm_config_g2t_stop ISR handler deregistered 0x%08x", gl1_hw_mcpm_config_g2t_stop);

      /*
      ** added to ensure handler is deregistered, llvm optimisation was providing wrong address
      ** to the deregister function. g2t and g2w are mutually exclusive so only 1 should be registered.
      */
      if (gstmr_is_handler_registered_geran(gl1_hw_mcpm_config_g2w_stop, as_id))
      {
         gstmr_deregister_handler_geran(gl1_hw_mcpm_config_g2w_stop, as_id);
      }
   }
}
#endif /* FEATURE_SLPC_TRIPLE_SIM */

/*===========================================================================
FUNCTION gl1_hw_gllc_encrypt_enable

DESCRIPTION
  This function enables the tripler and encrypt clock for GLLC.
  Also stops GL1 from going to sleep as that would disable tripler.

RETURN
  None
===========================================================================*/
void gl1_hw_gllc_encrypt_enable( void )
{
  //
  //
  //
  // Temporary solution to get the build working
  // gas_id should be an argument of gl1_hw_gllc_encrypt_enable
  //
  //
  //
   gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;

   GDRV_ISR_LOCK(gas_id);

   /* ensure that we stay awake(and therfore the clks are on.
    * this prevents a race consitions whereby GLLC enables clocks
    * but we go to sleep and disable them eventually casuing a crash
    * due to a register access with no clk enabled
    */
   gl1_drx_require_next_tick(gas_id);

   /* Stops GL1 from going into sleep asynchronously to GLLC */
   gl1_drx_task_active(gas_id);

   GDRV_ISR_UNLOCK(gas_id);

}

/*===========================================================================
FUNCTION gl1_hw_gllc_encrypt_disable

DESCRIPTION
  This function disables the tripler and encrypt clock for GLLC.
  Also allows GL1 to sleep which will disable tripler.

RETURN
  None
===========================================================================*/
void gl1_hw_gllc_encrypt_disable( void )
{
  //
  //
  //
  // Temporary solution to get the build working
  // gas_id should be an argument of gl1_hw_gllc_encrypt_enable
  //
  //
  //
   gas_id_t gas_id = GERAN_ACCESS_STRATUM_ID_1;
   GDRV_ISR_LOCK(gas_id);

   /* Allow GL1 to sleep asynchronously to GLLC */
   gl1_drx_task_inactive(gas_id);

   GDRV_ISR_UNLOCK(gas_id);

}

/*===========================================================================

FUNCTION gl1_hw_cipher_clock_required

DESCRIPTION
  Sets a gsac clock is required or not, for turning on/off clock

===========================================================================*/
void gl1_hw_cipher_clock_required(boolean cipher_clk_on,gas_id_t gas_id)
{
   /* As GL1 gets ciphering request in dedicated mode, EDGE-Core is enabled then
    * GSAC clock is not required. This is required only from LLC as EDGE-Core might
    * be disabled if GL1 is asleep
    */
#if 0
   if (cipher_clk_on)
   {
      gl1_hw_gsac_clock_is_required( GL1_HW_GSAC_CLOCK_MDSP_MASK,gas_id );
   }
   else
   {
      gl1_hw_gsac_clock_not_required( GL1_HW_GSAC_CLOCK_MDSP_MASK,gas_id );
   }
#endif
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_set_state

DESCRIPTION
  Sets status of mcpm state

===========================================================================*/
void gl1_hw_mcpm_set_state(gl1_mcpm_state_type mcpm_state,gas_id_t gas_id)
{
   gl1_mcpm_state[gas_id] = mcpm_state;
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_get_start_req

DESCRIPTION
  Returns status of mcpm start request

===========================================================================*/
gl1_mcpm_state_type gl1_hw_mcpm_get_state(gas_id_t gas_id)
{
   return (gl1_mcpm_state[gas_id]);
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_state_update

DESCRIPTION
  Sets a new GL1 state for MCPM and update MCPM

===========================================================================*/
void gl1_hw_mcpm_state_update(
    mcpm_request_type    req_state,
    gl1_mcpm_state_type  gl1_state,
    gas_id_t             gas_id)
{
  static boolean mcpm_voice_active[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(FALSE)};
  static boolean mcpm_data_active[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE(FALSE)};

   mcpm_request_parms_type gl1_mcpm_request_parms;

   gl1_mcpm_request_parms.update_info = MCPM_NO_UPDATE;

  switch(req_state)
   {
      case MCPM_GERAN_START_REQ:
         if((mcpm_data_active[gas_id] == FALSE) && (mcpm_voice_active[gas_id] == FALSE))
         {
            MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);
         }
         break;

      case MCPM_GERAN_DATA_START_REQ:

#ifdef FEATURE_GSM_MCPM_DL_DATA_RATE
         gl1_mcpm_request_parms.update_info = MCPM_DL_DATA_RATE_UPDATE;
         gl1_mcpm_request_parms.dl_datarate = MCPM_GSM_DL_NONEV;
#else
         gl1_mcpm_request_parms.update_info = MCPM_NO_UPDATE;
#endif /*FEATURE_GSM_MCPM_DL_DATA_RATE*/
         mcpm_data_active[gas_id] = TRUE;
         MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);
         break;

      case MCPM_GERAN_STOP_REQ:
      if(mcpm_data_active[gas_id] == TRUE)
         {
            mcpm_data_active[gas_id] = FALSE;
            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_DATA_STOP_REQ,gas_id), &gl1_mcpm_request_parms);
         }
      if(mcpm_voice_active[gas_id] == TRUE)
         {
            mcpm_voice_active[gas_id] = FALSE;
            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_VOICE_STOP_REQ,gas_id), &gl1_mcpm_request_parms);
         }
      if(gl1_hw_mcpm_get_state(gas_id) != GL1_MCPM_GERAN_STOP)
         {
            /* Relinquish T and W Nbrs */
            gl1_mcpm_request_parms.neighbor_meas = MCPM_G2TDS_MEAS_STOP;
            gl1_mcpm_request_parms.update_info = MCPM_NEIGHBOR_MEAS_UPDATE;
            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
            gl1_hw_sleep_maintain_ref_count (TDS, FALSE,gas_id);
            gl1_mcpm_request_parms.neighbor_meas = MCPM_G2W_MEAS_STOP;
            gl1_mcpm_request_parms.update_info = MCPM_NEIGHBOR_MEAS_UPDATE;
            MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);
            gl1_hw_sleep_maintain_ref_count (WCDMA, FALSE,gas_id);
         }
         break;

      case MCPM_GERAN_DATA_STOP_REQ:
         if(mcpm_data_active[gas_id] == TRUE)
         {
           if (mcpm_voice_active[gas_id] == FALSE)
           {
             /* Going to idle - request MCPM to disable AEq */
             gl1_mcpm_request_parms.receiver_config = 0;
             gl1_mcpm_request_parms.update_info = (MCPM_RECEIVER_CONFIG_UPDATE);
             MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);
           }
           else
           {
             MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);
           }
           mcpm_data_active[gas_id] = FALSE;
         }
         break;

      case MCPM_GERAN_VOICE_START_REQ:
         if (!mcpm_voice_active[gas_id])
         {
            mcpm_voice_active[gas_id] = TRUE;
            MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);
         }
         gl1_mcpm_tx_on[gas_id] = TRUE;
         break;

      case MCPM_GERAN_VOICE_STOP_REQ:
         if ((gl1_state == GL1_MCPM_GERAN_TX_OFF) && (mcpm_voice_active[gas_id]))
         {
            gl1_mcpm_tx_on[gas_id] = FALSE;
            mcpm_voice_active[gas_id] = FALSE;
            if (mcpm_data_active[gas_id] == FALSE)
            {
               /* Going to idle - request MCPM to disable AEq */
               gl1_mcpm_request_parms.receiver_config = 0;
               gl1_mcpm_request_parms.update_info = (MCPM_RECEIVER_CONFIG_UPDATE);
               MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);
            }
            else
            {
               MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);
            }
         }
         else
         {
             MSG_GERAN_MED_0_G("MCPM Change for voice stop not required");
         }
         break;

      default:
         MCPM_Config_Modem(MCPM_REQ_STATE(req_state,gas_id), &gl1_mcpm_request_parms);

         /* Clear the pending sleep req flag*/
         if(req_state == MCPM_GERAN_GO_TO_SLEEP_REQ)
         {
           mcpm_sleep_req_pending[gas_id] = FALSE;
         }
         break;
   }

   gl1_hw_mcpm_set_state(gl1_state, gas_id);
  if((mcpm_voice_active[gas_id] == FALSE) && (mcpm_data_active[gas_id] == FALSE))
   {
      gl1_mcpm_idle[gas_id] = TRUE;
   }
   else
   {
      gl1_mcpm_idle[gas_id] = FALSE;
   }

   MSG_GERAN_MED_3_G("MCPM Change %d complete gl1_mcpm_idle[%d]:%d",
                     (int16)req_state, gas_id, (int16)gl1_mcpm_idle[gas_id]);
}
/*===========================================================================

FUNCTION g1l_hw_is_mcpm_idle

DESCRIPTION
  Returns whether MCPM is idle for GSM (so voice and data not active)

===========================================================================*/
boolean g1l_hw_is_mcpm_idle (gas_id_t gas_id)
{
   return (gl1_mcpm_idle[gas_id]);
}

/*===========================================================================

FUNCTION g1l_hw_is_mcpm_tx_on

DESCRIPTION
  Returns whether MCPM is idle for GSM (so voice and data not active)

===========================================================================*/
boolean g1l_hw_is_mcpm_tx_on (gas_id_t gas_id)
{
   return (gl1_mcpm_tx_on[gas_id]);
}

/*===========================================================================
FUNCTION g1l_hw_set_mcpm_tx_on

DESCRIPTION
  Sets state of gl1_set_mcpm_tx_on

===========================================================================*/
void gl1_hw_set_mcpm_tx_on (boolean mcpm_tx_on_setting, gas_id_t gas_id)
{
    gl1_mcpm_tx_on[gas_id] = mcpm_tx_on_setting;
}

/*===========================================================================
FUNCTION gl1_hw_mcpm_tx_on_cb

DESCRIPTION
  Callback for MCPM when Tx block is on. Set gl1 flag to indicate Tx is on

===========================================================================*/
void gl1_hw_mcpm_tx_on_gas_cb (gas_id_t gas_id)
{
   gl1_hw_set_mcpm_tx_on(TRUE, gas_id);
   MSG_GERAN_MED_0_G("MCPM CB: Tx on");
}

/*===========================================================================
FUNCTION gl1_hw_mcpm_tx_on_gas1_cb

DESCRIPTION
  Callback for MCPM when Tx block is on for GAS ID 1
===========================================================================*/
void gl1_hw_mcpm_tx_on_gas1_cb (void)
{
   gl1_hw_mcpm_tx_on_gas_cb(GERAN_ACCESS_STRATUM_ID_1);
}

/*===========================================================================
FUNCTION gl1_hw_mcpm_tx_on_gas2_cb

DESCRIPTION
  Callback for MCPM when Tx block is on for GAS ID 2
===========================================================================*/
void gl1_hw_mcpm_tx_on_gas2_cb (void)
{
   gl1_hw_mcpm_tx_on_gas_cb(GERAN_ACCESS_STRATUM_ID_2);
}
/*===========================================================================
FUNCTION gl1_hw_mcpm_tx_on_gas3_cb

DESCRIPTION
  Callback for MCPM when Tx block is on for GAS ID 2
===========================================================================*/
void gl1_hw_mcpm_tx_on_gas3_cb (void)
{
   gl1_hw_mcpm_tx_on_gas_cb(GERAN_ACCESS_STRATUM_ID_3);
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_gts_apc

DESCRIPTION
  GTS MCPM update

===========================================================================*/
void gl1_hw_mcpm_gts_apc( gas_id_t gas_id )
{
  MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" gl1_hw_mcpm_gts_apc(): Before acq isr lock");

  GDRV_ISR_LOCK(gas_id);

  MSG_GERAN_HIGH_0_G(GL1_SLEEP_HDR" MCPM Change for Sleep");
  gl1_hw_mcpm_state_update(MCPM_GERAN_GO_TO_SLEEP_REQ, GL1_MCPM_GERAN_SLEEP,gas_id);

#ifdef FEATURE_MODEM_DDR_MGMT
  if (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
  {
    /* when trm = reason is TRM_NUM_REASONS , band value is invalid */
    trm_update_tech_state(TRM_GSM1,TRM_NUM_REASONS,SYS_BAND_GSM_EGSM_900);
  }
#endif /* FEATURE_MODEM_DDR_MGMT */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  gl1_hw_sleep_handle_deferred_wakeup_conflict(gas_id);
#endif

  GDRV_ISR_UNLOCK(gas_id);
}


/*===========================================================================

FUNCTION gl1_hw_mcpm_state_update_in_dpc_ongoing

DESCRIPTION
  Accessor function which returns TRUE if any MCPM DPC tasks are still ongoing

===========================================================================*/
void gl1_hw_mcpm_state_update_init(gas_id_t gas_id)
{
  memset(&mcpm_gsm_param[gas_id][0],
         0,
         sizeof(gl1_mcpm_update_gsm_dpc_param_type)*(MCPM_DPC_INDEX_MAX + 1));
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_state_update_in_dpc_ongoing

DESCRIPTION
  Accessor function which returns TRUE if any MCPM DPC tasks are still ongoing

===========================================================================*/
boolean gl1_hw_mcpm_state_update_in_dpc_ongoing(gas_id_t gas_id)
{
  uint8 i;

  for(i = 0; i <= MCPM_DPC_INDEX_MAX; i++)
  {
    if(mcpm_gsm_param[gas_id][i].ongoing)
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*===========================================================================

FUNCTION gl1_hw_check_ongoing_mcpm_update

DESCRIPTION
  Accessor function which returns TRUE if the latest MCPM request is same 
  as passed to the function.

===========================================================================*/
boolean gl1_hw_check_ongoing_mcpm_update(mcpm_request_type req_state, gas_id_t gas_id)
{
  gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr;
  mcpm_gsm_param_ptr=&mcpm_gsm_param[gas_id][mcpm_dpc_index[gas_id]];

  if(req_state == mcpm_gsm_param_ptr->mcpm_request)
  {
    return TRUE;
  }
  
  return FALSE;
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_state_update_apc

DESCRIPTION
  Sets a new GL1 state for MCPM and update MCPM

===========================================================================*/
static void gl1_hw_mcpm_state_update_apc( gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr)
{
  gl1_hw_mcpm_state_update(mcpm_gsm_param_ptr->mcpm_request,
                           mcpm_gsm_param_ptr->mcpm_param.gl1_state,
                           mcpm_gsm_param_ptr->gas_id);

  mcpm_gsm_param_ptr->ongoing = FALSE;
}
/*===========================================================================

FUNCTION gl1_hw_mcpm_state_update_dpc

DESCRIPTION
  Sets a new GL1 state for MCPM and update MCPM

===========================================================================*/
void gl1_hw_mcpm_state_update_dpc(mcpm_request_type req_state, gl1_mcpm_state_type gl1_state ,gas_id_t gas_id)
{
  gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr;
  GL1_DPC_QUEUE_LOCK(gas_id);
  mcpm_dpc_index[gas_id]++;
  if( mcpm_dpc_index[gas_id] > MCPM_DPC_INDEX_MAX )
  {
    mcpm_dpc_index[gas_id] = 0;
  }

  mcpm_gsm_param_ptr=&mcpm_gsm_param[gas_id][mcpm_dpc_index[gas_id]];

  mcpm_gsm_param_ptr->mcpm_request = req_state;
  mcpm_gsm_param_ptr->mcpm_param.gl1_state = gl1_state;
  mcpm_gsm_param_ptr->gas_id = gas_id;
  mcpm_gsm_param_ptr->ongoing = TRUE;
  GL1_DPC_QUEUE_UNLOCK(gas_id);

  /* Set update info to aid debug */
  mcpm_gsm_param_ptr->update_info = MCPM_NO_UPDATE;

  if (!rexl4_queue_dpc((rex_apc_func_type)gl1_hw_mcpm_state_update_apc, (uint32)mcpm_gsm_param_ptr))
  {
     MSG_GERAN_FATAL_0_G("Failed to queue MCPM update DPC");
  }
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_if_stop_req_after_start

DESCRIPTION

    This function checks if there is any stop request is queued in the DPC task after a start request is processed completely 
    This function is called after every G2X MCPM start request to check if any STOP request is queued in the DPC task after that.

NOTES:


===========================================================================*/

static boolean gl1_hw_mcpm_if_stop_req_after_start(mcpm_nbr_meas_type start_req_type, gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr, gas_id_t gas_id)
{
  uint32                mcpm_ptr_index;
  uint32                index;
  mcpm_nbr_meas_type    stop_req_type = start_req_type + 1;

  mcpm_ptr_index = mcpm_gsm_param_ptr - &mcpm_gsm_param[gas_id][0];
  GL1_DPC_QUEUE_LOCK(gas_id);
  if (mcpm_ptr_index == mcpm_dpc_index[gas_id])
    {
      /* The index is not changed after a Start request has been processed, so there is no MCPM request has been queued 
         as possibility is very less that MCPM_DPC_INDEX_MAX number of MCPM request being piled up after the 
         start request is queued up and processed by MCPM and indexes become equal */
      GL1_DPC_QUEUE_UNLOCK(gas_id);
      return FALSE;
    }
  else if (mcpm_ptr_index < mcpm_dpc_index[gas_id])
    {
      /* If MCPM request is queued in DPC task and mcpm_dpc_index is not wrapped around MCPM_DPC_INDEX_MAX */
      for (index = mcpm_ptr_index + 1; index <= mcpm_dpc_index[gas_id]; index++)
        {
          if(mcpm_gsm_param[gas_id][index].mcpm_param.nbr_type == stop_req_type)
            {
              MSG_GERAN_HIGH_3_G("Stop_req queued in DPC task for start_req: %d, at index: %d, mcpm_dpc_index: %d",start_req_type, index, mcpm_dpc_index[gas_id]);
              GL1_DPC_QUEUE_UNLOCK(gas_id);
              return TRUE;
            }
        }
    }
  else if (mcpm_ptr_index > mcpm_dpc_index[gas_id])
    {
      /* If MCPM request is queued in DPC task and mcpm_dpc_index is wrapped around MCPM_DPC_INDEX_MAX */
      for (index = mcpm_ptr_index + 1 ; index <= MCPM_DPC_INDEX_MAX; index++)
        {
          /* Check if any MCPM request piled up between mcpm_ptr_index and MCPM_DPC_INDEX_MAX */
          if(mcpm_gsm_param[gas_id][index].mcpm_param.nbr_type == stop_req_type)
            {
              MSG_GERAN_HIGH_3_G("Stop_req queued in DPC task for start_req: %d, at index: %d, mcpm_dpc_index: %d",start_req_type, index, mcpm_dpc_index[gas_id]);
              GL1_DPC_QUEUE_UNLOCK(gas_id);
              return TRUE;
            }
        }
      for (index = 0; index <= mcpm_dpc_index[gas_id]; index++)
        {
          /* Check if any MCPM request piled up between 0 and mcpm_dpc_index */
          if(mcpm_gsm_param[gas_id][index].mcpm_param.nbr_type == stop_req_type)
            {
              MSG_GERAN_HIGH_3_G("Stop_req queued in DPC task for start_req: %d, at index: %d, mcpm_dpc_index: %d",start_req_type, index, mcpm_dpc_index[gas_id]);
              GL1_DPC_QUEUE_UNLOCK(gas_id);
              return TRUE;
            }
        }
    }
  GL1_DPC_QUEUE_UNLOCK(gas_id);
  return FALSE;
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_config_apc

DESCRIPTION
  Sets a new GL1 state for MCPM and update MCPM

===========================================================================*/
static void gl1_hw_mcpm_config_info_apc( gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr,gas_id_t gas_id)
{
  mcpm_request_parms_type gl1_mcpm_request_parms;

  memset(&gl1_mcpm_request_parms, NULL, sizeof(gl1_mcpm_request_parms));

  if (mcpm_gsm_param_ptr->update_info == MCPM_NEIGHBOR_MEAS_UPDATE)
  {
      MSG_GERAN_HIGH_2_G( "MCPM Change GSM Neighbour %d in state %d", (uint16)mcpm_gsm_param_ptr->mcpm_param.nbr_type, gl1_hw_mcpm_get_state(gas_id));

      gl1_mcpm_request_parms.update_info = (MCPM_NEIGHBOR_MEAS_UPDATE|MCPM_MCVS_SCALE_UPDATE);
      gl1_mcpm_request_parms.neighbor_meas = mcpm_gsm_param_ptr->mcpm_param.nbr_type;
      /*mcpm uses a uint32 pointer to read scenario info structure for each tech*/
      gl1_mcpm_request_parms.mcvs_request.mcvs_req_type = MCVS_FULL_REQUEST;
      gl1_mcpm_request_parms.mcvs_request.mcvs_update_info = MCPM_MCVS_SCENARIO_INFO_UPDATE;
      gl1_mcpm_request_parms.mcvs_request.scenario_info = (uint32*)&mcpm_gsm_param_ptr->mcpm_param.mcvs_param.scenario ;
#ifdef FEATURE_SLPC_TRIPLE_SIM
      switch ( mcpm_gsm_param_ptr->mcpm_param.nbr_type )
      {
       case MCPM_G2W_MEAS_START:
          /* If we have a pending MCPM_G2W_MEAS_STOP, abort it since we now have an MCPM_G2W_MEAS_START/STOP */
          if (gstmr_is_handler_registered_geran(gl1_hw_mcpm_config_g2w_stop,geran_map_gas_id_to_nas_id(gas_id)))
          {
             /* Deregister the MCPM_G2W_MEAS_STOP handler, if necessary */
             gstmr_deregister_handler_geran(gl1_hw_mcpm_config_g2w_stop,geran_map_gas_id_to_nas_id(gas_id));
             MSG_GERAN_MED_0_G("gl1_hw_mcpm_config_g2w_stop ISR handler deregistered");
          }
          MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
          /* Only call G2W init once MCPM config is done for WCDMA */
          l1_sc_send_irat_g2x_init(RR_L1_IRAT_RAT_WCDMA, gl1_hw_mcpm_if_stop_req_after_start(MCPM_G2W_MEAS_START, mcpm_gsm_param_ptr, gas_id), gas_id);
          gl1_hw_sleep_maintain_ref_count (WCDMA,TRUE,gas_id);
/*Decrement task inactive count*/
          gl1_drx_task_inactive(gas_id);
        break;

      case MCPM_G2W_MEAS_STOP:
         if(gl1_drx_first_awake_frame(gas_id))
         {
               gl1_hw_g2w_stop_request_params[gas_id] = gl1_mcpm_request_parms;
               gstmr_register_handler_geran(gl1_hw_mcpm_config_g2w_stop,geran_map_gas_id_to_nas_id(gas_id));
               MSG_GERAN_MED_0_G("gl1_hw_mcpm_config_g2w_stop registered as ISR handler");
         }
         else
         {
            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
               gl1_hw_sleep_maintain_ref_count (WCDMA,FALSE,gas_id);
         }
         /*Decrement task inactive count*/
         gl1_drx_task_inactive(gas_id);
        break;

         case MCPM_G2TDS_MEAS_START:
            /* If we have a pending MCPM_G2W_MEAS_STOP, abort it since we now have an MCPM_G2W_MEAS_START/STOP */
            if (gstmr_is_handler_registered_geran(gl1_hw_mcpm_config_g2t_stop, geran_map_gas_id_to_nas_id(gas_id)))
            {
               /* Deregister the MCPM_G2T_MEAS_STOP handler, if necessary */
               gstmr_deregister_handler_geran(gl1_hw_mcpm_config_g2t_stop, geran_map_gas_id_to_nas_id(gas_id));
               MSG_GERAN_MED_0_G("gl1_hw_mcpm_config_g2t_stop ISR handler deregistered");
            }
            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);

            /* Only call G2T init once MCPM config is done for TDSCDMA */
            l1_sc_send_irat_g2x_init(RR_L1_IRAT_RAT_UTRAN_TDD, gl1_hw_mcpm_if_stop_req_after_start(MCPM_G2TDS_MEAS_START, mcpm_gsm_param_ptr, gas_id), gas_id);
            gl1_hw_sleep_maintain_ref_count (TDS, TRUE, gas_id);
/*Decrement task inactive count*/
            gl1_drx_task_inactive(gas_id);
            break;

         case MCPM_G2TDS_MEAS_STOP:
            if(gl1_drx_first_awake_frame(gas_id) && (!gl1_hw_sleep_get_forcedwakeup(gas_id)))
            {
               gl1_hw_g2t_stop_request_params[gas_id] = gl1_mcpm_request_parms;
               gstmr_register_handler_geran(gl1_hw_mcpm_config_g2t_stop, geran_map_gas_id_to_nas_id(gas_id));
               MSG_GERAN_MED_0_G("gl1_hw_mcpm_config_g2t_stop registered as ISR handler");
            }
            else
            {
               MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
               gl1_hw_sleep_maintain_ref_count (TDS, FALSE, gas_id);
            }
            /*Decrement task inactive count*/
            gl1_drx_task_inactive(gas_id);
            break;

         case MCPM_G2L_MEAS_START:
		 	
            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
            l1_sc_send_irat_g2x_init(RR_L1_IRAT_RAT_LTE, gl1_hw_mcpm_if_stop_req_after_start(MCPM_G2L_MEAS_START, mcpm_gsm_param_ptr, gas_id), gas_id);
         /*Decrement task inactive count*/
            gl1_drx_task_inactive(gas_id);
            break;

         case MCPM_G2L_MEAS_STOP:

            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
            /*Decrement task inactive count*/
            gl1_drx_task_inactive(gas_id);

            break;

         default:

            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);

            break;
      }
#else
#if defined(FEATURE_INTERRAT_HANDOVER_GTOW) || defined(FEATURE_INTERRAT_CELL_RESELECTION_GTOW )
      /* If we have a pending MCPM_G2W_MEAS_STOP, abort it since we now have an MCPM_G2W_MEAS_START/STOP */
      if (gstmr_is_handler_registered_geran(gl1_hw_mcpm_config_g2w_stop,geran_map_gas_id_to_nas_id(gas_id)))
      {
         /* Deregister the MCPM_G2W_MEAS_STOP handler, if necessary */
         gstmr_deregister_handler_geran(gl1_hw_mcpm_config_g2w_stop,geran_map_gas_id_to_nas_id(gas_id));
         MSG_GERAN_MED_0_G("gl1_hw_mcpm_config_g2w_stop ISR handler deregistered");
      }

      if ( mcpm_gsm_param_ptr->mcpm_param.nbr_type == MCPM_G2W_MEAS_START )
      {
         MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
         /* Only call G2W init once MCPM config is done for WCDMA */
         l1_sc_send_irat_g2x_init(RR_L1_IRAT_RAT_WCDMA, gl1_hw_mcpm_if_stop_req_after_start(MCPM_G2W_MEAS_START, mcpm_gsm_param_ptr, gas_id), gas_id);
         gl1_hw_sleep_maintain_ref_count (TRUE,gas_id);
      }
      else if ( mcpm_gsm_param_ptr->mcpm_param.nbr_type == MCPM_G2W_MEAS_STOP )
      {
         if(gl1_drx_first_awake_frame(gas_id))
         {
               gl1_hw_g2w_stop_request_params[gas_id] = gl1_mcpm_request_parms;
               gstmr_register_handler_geran(gl1_hw_mcpm_config_g2w_stop,geran_map_gas_id_to_nas_id(gas_id));
               MSG_GERAN_MED_0_G("gl1_hw_mcpm_config_g2w_stop registered as ISR handler");
         }
         else
         {
            MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
            gl1_hw_sleep_maintain_ref_count (FALSE,gas_id);
         }
         /*Decrement task inactive count*/
         gl1_drx_task_inactive(gas_id);
      }
      else if ( mcpm_gsm_param_ptr->mcpm_param.nbr_type == MCPM_G2TDS_MEAS_START )
      {
         MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);

         /* Only call G2T init once MCPM config is done for TDSCDMA */
         l1_sc_send_irat_g2x_init(RR_L1_IRAT_RAT_UTRAN_TDD, gl1_hw_mcpm_if_stop_req_after_start(MCPM_G2TDS_MEAS_START, mcpm_gsm_param_ptr, gas_id), gas_id);
         gl1_hw_sleep_maintain_ref_count (TDS, TRUE, gas_id);
      }
      else if ( mcpm_gsm_param_ptr->mcpm_param.nbr_type == MCPM_G2L_MEAS_START )
      {
          MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
          l1_sc_send_irat_g2x_init(RR_L1_IRAT_RAT_LTE, gl1_hw_mcpm_if_stop_req_after_start(MCPM_G2L_MEAS_START, mcpm_gsm_param_ptr, gas_id), gas_id);
      }
      else
      {
          MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);

          if((mcpm_gsm_param_ptr->mcpm_param.nbr_type == MCPM_G2L_MEAS_STOP) || (mcpm_gsm_param_ptr->mcpm_param.nbr_type == MCPM_G2TDS_MEAS_STOP))
          {
            /*Decrement task inactive count*/
            gl1_drx_task_inactive(gas_id);
          }
      }
#endif
#endif /* FEATURE_SLPC_TRIPLE_SIM */
  }
  else
  {
    if (mcpm_gsm_param_ptr->update_info == MCPM_MCVS_SCALE_UPDATE)
    {
      gl1_mcpm_request_parms.update_info = MCPM_MCVS_SCALE_UPDATE;
      if (mcpm_gsm_param_ptr->mcpm_param.aeq_enable)
      {
        gl1_mcpm_request_parms.mcvs_request.mcvs_req_type = MCVS_FULL_REQUEST;
        gl1_mcpm_request_parms.mcvs_request.mcvs_update_info = (MCPM_MCVS_Q6_CLK_UPDATE|MCPM_MCVS_SCENARIO_INFO_UPDATE);
        gl1_mcpm_request_parms.mcvs_request.q6_clock_KHz = mcpm_gsm_param_ptr->mcvs_q6_speed;
		gl1_mcpm_request_parms.mcvs_request.scenario_info = (uint32*)&mcpm_gsm_param_ptr->mcpm_param.mcvs_param.scenario;
      }
      else
      {
        gl1_mcpm_request_parms.mcvs_request.mcvs_req_type = MCVS_RELEASE_REQUEST;
      }
      MSG_GERAN_HIGH_2_G("MCPM Change - MCVS request %d, for %d kHz", mcpm_gsm_param_ptr->mcpm_param.aeq_enable, mcpm_gsm_param_ptr->mcvs_q6_speed);
	  MCPM_Config_Modem(MCPM_REQ_STATE(MCPM_GERAN_PARMS_UPDATE_REQ,gas_id), &gl1_mcpm_request_parms);
    }
  }
}

/*===========================================================================

FUNCTION  gl1_hw_mcpm_config_info_apc_geran0
      Wrapper for sub 0 for gl1_hw_mcpm_config_apc

DESCRIPTION
  Sets a new GL1 state for MCPM and update MCPM

===========================================================================*/
static void gl1_hw_mcpm_config_info_apc_geran0( gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr)
{
  gl1_hw_mcpm_config_info_apc(mcpm_gsm_param_ptr,GERAN_ACCESS_STRATUM_ID_1);
}
#if defined ( FEATURE_DUAL_SIM )
/*===========================================================================

FUNCTION  gl1_hw_mcpm_config_info_apc_geran1
Wrapper for sub 1 for gl1_hw_mcpm_config_apc

DESCRIPTION
  Sets a new GL1 state for MCPM and update MCPM

===========================================================================*/
static void gl1_hw_mcpm_config_info_apc_geran1( gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr)
{
  gl1_hw_mcpm_config_info_apc(mcpm_gsm_param_ptr,GERAN_ACCESS_STRATUM_ID_2);
}
#endif /* FEATURE_DUAL_SIM */

#if defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  gl1_hw_mcpm_config_info_apc_geran2
Wrapper for sub 2 for gl1_hw_mcpm_config_apc

DESCRIPTION
  Sets a new GL1 state for MCPM and update MCPM

===========================================================================*/
static void gl1_hw_mcpm_config_info_apc_geran2(
              gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr )
{
  gl1_hw_mcpm_config_info_apc( mcpm_gsm_param_ptr,
                               GERAN_ACCESS_STRATUM_ID_3 );
}
#endif /* FEATURE_TRIPLE_SIM */

/*===========================================================================

FUNCTION gl1_hw_mcpm_config_dpc

DESCRIPTION
  Sets a new GL1 state for MCPM and update MCPM

===========================================================================*/
static void gl1_hw_mcpm_config_update_dpc(uint16 update_info, mcpm_nbr_meas_type req_nbr, boolean aeq_enable, uint32 mcvs_q6_speed, uint32 scenario, gas_id_t gas_id)
{
  gl1_mcpm_update_gsm_dpc_param_type *mcpm_gsm_param_ptr;
  GL1_DPC_QUEUE_LOCK(gas_id);
  mcpm_dpc_index[gas_id]++;
  if( mcpm_dpc_index[gas_id] > MCPM_DPC_INDEX_MAX )
  {
    mcpm_dpc_index[gas_id] = 0;
  }

  mcpm_gsm_param_ptr=&mcpm_gsm_param[gas_id][mcpm_dpc_index[gas_id]];

  mcpm_gsm_param_ptr->mcpm_request = MCPM_GERAN_PARMS_UPDATE_REQ;
  mcpm_gsm_param_ptr->update_info = update_info;
  
  mcpm_gsm_param_ptr->mcpm_param.mcvs_param.scenario.ScenarioId = scenario;
  mcpm_gsm_param_ptr->mcpm_param.mcvs_param.scenario.ActionTime= 0;
  
  if (update_info == MCPM_NEIGHBOR_MEAS_UPDATE)
  {
     mcpm_gsm_param_ptr->mcpm_param.nbr_type = req_nbr;
  }
  else
  {
     mcpm_gsm_param_ptr->mcpm_param.aeq_enable = aeq_enable;
  }
  mcpm_gsm_param_ptr->mcvs_q6_speed = mcvs_q6_speed;
  GL1_DPC_QUEUE_UNLOCK(gas_id);
  
  switch ( gas_id )
  {
    case GERAN_ACCESS_STRATUM_ID_1:
      if (!rexl4_queue_dpc((rex_apc_func_type)gl1_hw_mcpm_config_info_apc_geran0, (uint32)mcpm_gsm_param_ptr))
      {
        MSG_GERAN_FATAL_0_G( "Failed to queue MCPM config info DPC sub 0" );
      }
    break;
#if defined ( FEATURE_DUAL_SIM )
    case GERAN_ACCESS_STRATUM_ID_2:
      if (!rexl4_queue_dpc((rex_apc_func_type)gl1_hw_mcpm_config_info_apc_geran1, (uint32)mcpm_gsm_param_ptr))
      {
        MSG_GERAN_FATAL_0_G( "Failed to queue MCPM config info DPC sub 1" );
      }
    break;
#endif /* FEATURE_DUAL_SIM */

#if defined ( FEATURE_TRIPLE_SIM )
    case GERAN_ACCESS_STRATUM_ID_3:
      if (!rexl4_queue_dpc((rex_apc_func_type)gl1_hw_mcpm_config_info_apc_geran2, (uint32)mcpm_gsm_param_ptr))
      {
        MSG_GERAN_FATAL_0_G( "Failed to queue MCPM config info DPC sub 2" );
      }
    break;
#endif /* FEATURE_TRIPLE_SIM */

    default:
      MSG_GERAN_FATAL_0_G( "Invalid gas_id" );
    break;
  }
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_neighbor_update

DESCRIPTION
  Sets a new neighbour parameter for MCPM and updates MCPM

===========================================================================*/
void gl1_hw_mcpm_neighbor_update (mcpm_nbr_meas_type req_nbr, uint32 scenario, gas_id_t gas_id)
{
  uint32 mcpm_scenario = 0;
   
   /*Mcpm uses 32 bit int to extract scenario info as this: 
     lower 8 bits (7:0)	  - interal to mcpm
     1 bit	  (8)	  - Scenario status ON/OFF
     upper 23 bits(31:9)  - Scenario ID (range 0- 2^23)
     MCPM_MCVS_BOOST_SCENARIO_BIT_SHIFTS = 8
   */
   switch(req_nbr)
   {
     case MCPM_G2L_MEAS_START:
     case MCPM_G2W_MEAS_START:
     case MCPM_G2TDS_MEAS_START:
       mcpm_scenario = ((scenario<<1)|TRUE)<<MCPM_MCVS_BOOST_SCENARIO_BIT_SHIFTS;
     break;
      
     case MCPM_G2L_MEAS_STOP:
     case MCPM_G2W_MEAS_STOP:
     case MCPM_G2TDS_MEAS_STOP:
       mcpm_scenario = ((scenario<<1)|FALSE)<<MCPM_MCVS_BOOST_SCENARIO_BIT_SHIFTS;
     break;
      
     default:
       MSG_GERAN_HIGH_0_G("Invalid Requested Neighbor start/stop request");
     break;
   }
   MSG_GERAN_HIGH_3_G("FR47064:gl1_hw_mcpm_neighbor_update neighbor type %d, gl1 Scenario ID %d mcpm Scenario sent %d", req_nbr,scenario, mcpm_scenario);
   gl1_hw_mcpm_config_update_dpc (MCPM_NEIGHBOR_MEAS_UPDATE, req_nbr, TRUE, 0,mcpm_scenario, gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_amr_aeq_update

DESCRIPTION
  Sets an AEq parameter for MCPM and updates MCPM

===========================================================================*/
void gl1_hw_mcpm_amr_aeq_update (boolean aeq_on,gas_id_t gas_id)
{
    gl1_clkdata_speed(aeq_on, L1_DED_SPEEDUP_AEQ, L1_DED_DATA_NULL, L1_DED_AEQ, gas_id);
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_gprs_aeq_update

DESCRIPTION
  Sets an AEq parameter for MCPM and updates MCPM

===========================================================================*/
void gl1_hw_mcpm_gprs_aeq_update (boolean aeq_on,gas_id_t gas_id)
{
   /* In Dime no clock changes required for AEq in data mode */
   MSG_GERAN_LOW_1_G("GPRS AEq MCPM update (no change), %d", aeq_on);
}

/*===========================================================================

FUNCTION  gl1_egprs_mcpm_change
DESCRIPTION
  This function requeste MCPM change at start/end of EGPRS mode so that
  clock speed is increased during AEQ-2A algorithm running in GFW

===========================================================================*/
void g11_hw_egprs_mcpm_change( boolean egprs_start, gas_id_t gas_id )
{
   /* Ensure higher clock speed for power scan, required by RF driver */
   MSG_GERAN_HIGH_1_G( "MCPM change for EGPRS speedup %d", egprs_start);
   gl1_clkdata_speed(egprs_start, L1_EGPRS_SPEEDUP, L1_DED_DATA_NULL, L1_EGPRS, gas_id);
}

/*===========================================================================

FUNCTION gl1_mcvs_clear_req

DESCRIPTION
  To clear mcvs freq used on stop mode req.
===========================================================================*/

void gl1_mcvs_clear_req(gas_id_t gas_id)
{
   gl1_mcvs_khz[gas_id] = 0;
}
/*===========================================================================

FUNCTION gl1_mcvs_clock_set

DESCRIPTION
  If appropriate sets the MCVS clock data, and requests MCPM for clock speed update.
  Returns whether clock change was done using MCVS
===========================================================================*/
boolean gl1_mcvs_clock_set(boolean SpeedReq, uint16 SpeedState, uint32 Scenario, gas_id_t gas_id)
{
  uint32 mcvs_q6_clk_khz;
  boolean use_mcvs = FALSE;
  boolean mcvs_on;
  uint32 mcpm_scenario =0;

  switch (SpeedState)
  {

  case L1_EGPRS_SPEEDUP:
    use_mcvs = TRUE;
    mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_EGPRS;
    break;

  case L1_DED_SPEEDUP_CHAN_RECONFIG:
  case L1_DED_SPEEDUP_AEQ:
     use_mcvs = TRUE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_TCH_AEQ;
     break;

  case L1_DED_SPEEDUP_G2X:
     use_mcvs = TRUE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_DED_G2X;
     break;


  case L1_FIND_BCCH_SPEEDUP:
     use_mcvs = TRUE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_FIND_BCCH;
     break;
  case L1_PSCAN_SPEEDUP:
     use_mcvs = TRUE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_PSCAN;
     break;

  case L1_DED_SPEEDUP_DTM_HANG:
     use_mcvs = FALSE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_DATA;
     break;
  case L1_DED_SPEEDUP_LOOPBACK:
     use_mcvs = TRUE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_PAR_ACQ;
     break;

#ifdef FEATURE_AEQ_PHASE4
  case L1_DED_SPEEDUP_AEQ_PH4:
     use_mcvs = TRUE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_TCH_AEQ_PH4;
     break;
#endif  /*FEATURE_AEQ_PHASE4*/
#ifdef FEATURE_GSM_PARALLEL_SCH_DEDICATED
  case L1_PAR_ACQ_SPEEDUP:
  case L1_TIME_LINE_ERROR:
     use_mcvs = TRUE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_PAR_ACQ;
     break;
#endif /* FEATURE_GSM_PARALLEL_SCH_DEDICATED */
  default:
     use_mcvs = FALSE;
     mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_DATA;
     break;
   }


   if (use_mcvs)
   {
     /*Mcpm uses 32 bit int to extract scenario info as this: 
      lower 8 bits (7:0)	- interal to mcpm
      1 bit        (8)	        - Scenario status ON/OFF
      upper 23 bits(31:9)	- Scenario ID (range 0- 2^23)

      MCPM_MCVS_BOOST_SCENARIO_BIT_SHIFTS = 8
    */
    mcpm_scenario = ((Scenario<<1)|SpeedReq)<<MCPM_MCVS_BOOST_SCENARIO_BIT_SHIFTS;
    MSG_GERAN_HIGH_5_G("MCVS request %d, state %d current_state %d, Scenario ID %d Scenario sent %d", SpeedReq, SpeedState, gl1_mcvs_state[gas_id],Scenario,mcpm_scenario);
    if(SpeedReq)
    {
      gl1_mcvs_state[gas_id] |= SpeedState;
    }
    else
    {
       gl1_mcvs_state[gas_id] &= ~SpeedState;
    }

    if (gl1_mcvs_state[gas_id] == 0)
    {
       mcvs_on = FALSE;
       mcvs_q6_clk_khz = 0;
    }
    else
    {
       mcvs_on = TRUE;

       if (gl1_mcvs_state[gas_id] & L1_EGPRS_SPEEDUP)
       {
         mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_EGPRS;
       }
       else if (gl1_mcvs_state[gas_id] & L1_PSCAN_SPEEDUP)
       {
          mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_PSCAN;
       }
       else if(gl1_mcvs_state[gas_id] & L1_FIND_BCCH_SPEEDUP )
       {
         mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_FIND_BCCH;
       }
#ifdef FEATURE_GSM_PARALLEL_SCH_DEDICATED
       else if ( gl1_mcvs_state[gas_id] & L1_PAR_ACQ_SPEEDUP )
       {
          mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_PAR_ACQ;
       }
#endif /* FEATURE_GSM_PARALLEL_SCH_DEDICATED */
       else if (gl1_mcvs_state[gas_id] & L1_DED_SPEEDUP_LOOPBACK)
       {
          mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_PAR_ACQ;
       }
       else if (gl1_mcvs_state[gas_id] & L1_DED_SPEEDUP_G2X)
       {
          mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_DED_G2X;
       }
#ifdef FEATURE_AEQ_PHASE4
       else if (gl1_mcvs_state[gas_id] & L1_DED_SPEEDUP_AEQ_PH4)
       {
          mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_TCH_AEQ_PH4;
       }
#endif /*FEATURE_AEQ_PHASE4 */
       else
       {
          mcvs_q6_clk_khz = MCVS_Q6_CLK_KHZ_TCH_AEQ;
       }
    }

    if (mcvs_q6_clk_khz ==  gl1_mcvs_khz[gas_id])
    {
       MSG_GERAN_MED_2_G("No change to MCVS, state %d, %d kHz", gl1_mcvs_state[gas_id], mcvs_q6_clk_khz);
    }
    else
    {
        /* If The MCVS request is to 230.4 MHz and this is a switching off a higher speed request, no need to place the MCPM request again */
        if ((mcvs_q6_clk_khz == MCVS_Q6_CLK_KHZ_TCH_AEQ) && (SpeedReq == FALSE))
        {
          mcvs_on = FALSE;
        }

      /* Even if MCVS speed does not change, send request to MCPM so that mcpm gets update about scenario start/stop*/
        MSG_GERAN_HIGH_3_G("MCPM Change: MCVS dpc request %d, state %d, for %d kHz", mcvs_on, gl1_mcvs_state[gas_id], mcvs_q6_clk_khz);
      gl1_hw_mcpm_config_update_dpc(MCPM_MCVS_SCALE_UPDATE, NULL, mcvs_on, mcvs_q6_clk_khz, mcpm_scenario, gas_id );
        gl1_mcvs_khz[gas_id] = mcvs_q6_clk_khz;
     }
   }

  return (use_mcvs);
}

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING

/*===========================================================================

FUNCTION gl1_hw_switch_antenna_tx_div

DESCRIPTION
  queue switch_antenna_tx_div funcs to switch the antenna for tx diversity
NOTES:

These calls envoke on idle frame  from DPC task context.

===========================================================================*/

void gl1_hw_switch_antenna_tx_div (gas_id_t gas_id)

{
  int trm_retrn ;
  trm_ant_switch_set_input_type  ant_switch_set_input;
  trm_switch_complete_input_type trm_switch_complete ;
  grm_client_enum_t grm_client_id;

  gas_id = check_gas_id(gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
  grm_client_id = check_asdiv_grm_client(grm_client_id);
  ant_switch_set_input.client = trm_switch_complete.client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
#else
  ant_switch_set_input.client = trm_switch_complete.client = TRM_GSM1;
#endif /* ( FEATURE_DUAL_SIM ) ||  ( FEATURE_TRIPLE_SIM ) */

  MSG_GERAN_HIGH_3_G (" gl1_hw_switch_antenna_tx_div  ",0,0,0);

  if (antenna_to_use_for_tx [gas_id ] ==  2)
  {
    switch ( trm_ant_switch_div_get_config ( TRM_ANT_SWITCH_CONFIG_CURRENT))
    {
      case TRM_ANT_SWITCH_DIV_CONFIG_0 :
        MSG_GERAN_HIGH_3_G (" keep using antenna 1 no switch required  ",0,0,0);
        break;

      case TRM_ANT_SWITCH_DIV_CONFIG_1 :
        /* need to remove comments when rf changes are available */
        ant_switch_set_input.config = TRM_ANT_SWITCH_DIV_CONFIG_0 ;
        trm_retrn = trm_ant_switch_div_set_config(ant_switch_set_input);
        if (TRM_ANT_SWITCH_DIV_SET_SUCCESS == trm_retrn )
        {
          if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (FALSE,gas_id ))
          {
            MSG_GERAN_HIGH_3_G (" RF set antenna 1  sucessfully ",0,0,0);
            trm_switch_complete.config = TRM_ANT_SWITCH_DIV_CONFIG_0 ;
            trm_ant_switch_div_switch_complete (trm_switch_complete );
            MSG_GERAN_HIGH_3_G ("trm_ant_switch_div_set_config sucess ",0,0,0);
          }
          else
          {
            MSG_GERAN_HIGH_3_G (" RF set antenna 1 wrong ",0,0,0);
          }
        }
        else
        {
          MSG_GERAN_HIGH_3_G ("trm_ant_switch_div_set_config fail trm_rtn = %d ",trm_retrn,0,0);
        }
        break;

      default :
        MSG_GERAN_HIGH_3_G( " invalid tx div antenna config =%d",0,0,0);
        break;

    }
    MSG_GERAN_HIGH_3_G( " initial antenna antenna_to_use_for_tx [gas_id=%d ] =%d",gas_id,antenna_to_use_for_tx [gas_id ],0);
  }
  else if (antenna_to_use_for_tx [gas_id ] ==  1)
  {
    switch ( trm_ant_switch_div_get_config ( TRM_ANT_SWITCH_CONFIG_CURRENT))
    {
      case TRM_ANT_SWITCH_DIV_CONFIG_0 :
         ant_switch_set_input.config = TRM_ANT_SWITCH_DIV_CONFIG_1 ;
         trm_retrn = trm_ant_switch_div_set_config(ant_switch_set_input);
         if (TRM_ANT_SWITCH_DIV_SET_SUCCESS == trm_retrn )
         {
            if (gl1_hw_rfa_rf_gsm_set_antenna_req_cmd (TRUE , gas_id))
            {
              MSG_GERAN_HIGH_3_G (" RF set antenna 2  sucessfully ",0,0,0);
              trm_switch_complete.config = TRM_ANT_SWITCH_DIV_CONFIG_1 ;
              MSG_GERAN_HIGH_3_G ("trm_ant_switch_div_set_config sucess ",0,0,0);
              trm_ant_switch_div_switch_complete (trm_switch_complete );
            }
            else
            {
              MSG_GERAN_HIGH_3_G ("RF set antenna 2 wrong ",0,0,0);
            }
          }
          else
          {
            MSG_GERAN_HIGH_3_G ("trm_ant_switch_div_set_config fail rtn = %d ",trm_retrn,0,0);
          }
          break;

      case TRM_ANT_SWITCH_DIV_CONFIG_1 :
        MSG_GERAN_HIGH_3_G (" keep using antenna 2 no switch required  ",0,0,0);
        break;

      default :
        MSG_GERAN_HIGH_3_G( " invalid tx div antenna config =%d",0,0,0);
        break;

    }
  }
}



void gl1_hw_switch_antenna_tx_div_ccch_fail (boolean is_pch, gas_id_t gas_id)
{
  trm_ant_switch_set_input_type  ant_switch_set_input;
  trm_switch_complete_input_type trm_switch_complete ;
  grm_client_enum_t grm_client_id;

  gas_id = check_gas_id(gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  grm_client_id = gl1_map_gas_id_to_client_id(gas_id);
  grm_client_id = check_asdiv_grm_client(grm_client_id);
  ant_switch_set_input.client = trm_switch_complete.client = grm_map_grm_client_id_to_trm_client_id(grm_client_id);
#else
  ant_switch_set_input.client = trm_switch_complete.client = TRM_GSM1;
#endif /* ( FEATURE_DUAL_SIM ) ||  ( FEATURE_TRIPLE_SIM ) */

  if (gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE1) || gl1_trm_asdiv_enable(gas_id, GL1_TRM_ASDIV_TYPE2))
  {
    MSG_GERAN_HIGH_1_G("gl1_hw_switch_antenna_tx_div_ccch_fail called, pch = %d ",is_pch);

    /*
    Set the new config on PCH decode fail
    gl1_trm_asd_set_mode_and_config installs a callback, that will set the antenna if the mode/config set is successful
    */
  if(is_pch)
  {
      switch ( trm_ant_switch_div_get_config ( TRM_ANT_SWITCH_CONFIG_CURRENT))
      {
        case TRM_ANT_SWITCH_DIV_CONFIG_0 :
          antenna_to_use_for_tx [gas_id ] = 2 ;
          type2_alg_antenna_to_use[gas_id] = 2;
          gl1_trm_asd_set_mode_and_config(TRM_CLIENT_MODE_SLEEP, TRM_ANT_SWITCH_DIV_CONFIG_1, gas_id);
          break;

        case TRM_ANT_SWITCH_DIV_CONFIG_1 :
          antenna_to_use_for_tx [gas_id ] = 1 ;
          type2_alg_antenna_to_use[gas_id] = 1;
          gl1_trm_asd_set_mode_and_config(TRM_CLIENT_MODE_SLEEP, TRM_ANT_SWITCH_DIV_CONFIG_0, gas_id);
          break;

        default :
          MSG_GERAN_HIGH_3_G( " invalid tx div antenna config =%d",0,0,0);
          break;
      }
  }
  else
  {
    gl1_trm_ant_switch_div_set_config(gas_id);
  }
  }
}

#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

/*===========================================================================

FUNCTION MCPM_REQ_STATE

DESCRIPTION
  This function returns the MCPM request ID based on the SUB ID. It replaces the old 
  macro MCPM_REQ_STATE to a function so that it doesn't have to depend on MCPM changes
  while maintaining table for MCPM request IDs

NOTES:

===========================================================================*/
static mcpm_request_type MCPM_REQ_STATE (mcpm_request_type req_in, gas_id_t gas_id)
{
  mcpm_request_type req_out = req_in;

  if(gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    return req_out;
  }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    switch(req_in)
    {
      case MCPM_GERAN_START_REQ:
        req_out = MCPM_GERAN1_START_REQ;
        break;

      case MCPM_GERAN_STOP_REQ:
        req_out = MCPM_GERAN1_STOP_REQ;
        break;

      case MCPM_GERAN_INIT_REQ:
        req_out = MCPM_GERAN1_INIT_REQ;
        break;

      case MCPM_GERAN_WAKE_UP_REQ:
        req_out = MCPM_GERAN1_WAKE_UP_REQ;
        break;
      
      case MCPM_GERAN_GO_TO_SLEEP_REQ:
        req_out = MCPM_GERAN1_GO_TO_SLEEP_REQ;
        break;
      
      case MCPM_GERAN_IDLE_REQ:
        req_out = MCPM_GERAN1_IDLE_REQ;
        break;

      case MCPM_GERAN_VOICE_START_REQ:
        req_out = MCPM_GERAN1_VOICE_START_REQ;
        break;
      
      case MCPM_GERAN_VOICE_STOP_REQ:
        req_out = MCPM_GERAN1_VOICE_STOP_REQ;
        break;

      case MCPM_GERAN_DATA_START_REQ:
        req_out = MCPM_GERAN1_DATA_START_REQ;
        break;
      
      case MCPM_GERAN_DATA_STOP_REQ:
        req_out = MCPM_GERAN1_DATA_STOP_REQ;
        break;

      case MCPM_GERAN_PARMS_UPDATE_REQ:
        req_out = MCPM_GERAN1_PARMS_UPDATE_REQ;
        break;

      default:
        MSG_GERAN_ERROR_0_G("Invalid mcpm_request_type");
    }
  }
#if defined ( FEATURE_TRIPLE_SIM )
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_3)
  {
    switch(req_in)
    {
      case MCPM_GERAN_START_REQ:
        req_out = MCPM_GERAN2_START_REQ;
        break;

      case MCPM_GERAN_STOP_REQ:
        req_out = MCPM_GERAN2_STOP_REQ;
        break;

      case MCPM_GERAN_INIT_REQ:
        req_out = MCPM_GERAN2_INIT_REQ;
        break;

      case MCPM_GERAN_WAKE_UP_REQ:
        req_out = MCPM_GERAN2_WAKE_UP_REQ;
        break;
      
      case MCPM_GERAN_GO_TO_SLEEP_REQ:
        req_out = MCPM_GERAN2_GO_TO_SLEEP_REQ;
        break;
      
      case MCPM_GERAN_IDLE_REQ:
        req_out = MCPM_GERAN2_IDLE_REQ;
        break;

      case MCPM_GERAN_VOICE_START_REQ:
        req_out = MCPM_GERAN2_VOICE_START_REQ;
        break;
      
      case MCPM_GERAN_VOICE_STOP_REQ:
        req_out = MCPM_GERAN2_VOICE_STOP_REQ;
        break;

      case MCPM_GERAN_DATA_START_REQ:
        req_out = MCPM_GERAN2_DATA_START_REQ;
        break;
      
      case MCPM_GERAN_DATA_STOP_REQ:
        req_out = MCPM_GERAN2_DATA_STOP_REQ;
        break;

      case MCPM_GERAN_PARMS_UPDATE_REQ:
        req_out = MCPM_GERAN2_PARMS_UPDATE_REQ;
        break;

      default:
        MSG_GERAN_ERROR_0_G("Invalid mcpm_request_type");
    }
  }
#endif
  else
  {
    MSG_GERAN_ERROR_0_G("Invalid gas_id");
  }
  return req_out;
}

/*===========================================================================

FUNCTION gl1_get_mcvs_speed

DESCRIPTION
Returns Current MCVS speed

NOTES:

===========================================================================*/

uint32 gl1_get_mcvs_speed(gas_id_t gas_id)
{
  MSG_GERAN_HIGH_1_G("MCVS speed %d" ,gl1_mcvs_khz[gas_id]);
  return (gl1_mcvs_khz[gas_id]);
}

/*===========================================================================

FUNCTION gl1_hw_mcpm_boost_request

DESCRIPTION
Boost clock speed

NOTES:

===========================================================================*/
void gl1_hw_mcpm_boost_request(uint32 scenario,uint32 boostRequest, uint32 boostTime, gas_id_t gas_id)
{
  mcpm_tech_type mcpm_tech = gl1_hw_map_gas_id_to_mcpm_tech( gas_id );
  uint32 mcpm_scenario = 0;
    mcpm_scenario = ((scenario<<1)|TRUE)<<MCPM_MCVS_BOOST_SCENARIO_BIT_SHIFTS;
  (void)MCPM_Service_Boost_Request(mcpm_tech,boostRequest|mcpm_scenario,NULL,NULL,boostTime);
  MSG_GERAN_HIGH_2_G("MCPM Boost Request %d for % us time", boostRequest, boostTime);
}

/* EOF */
