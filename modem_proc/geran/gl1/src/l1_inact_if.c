/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       L 1   I N A C T I V E    M O D E   T A S K    P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events while the L1
   task is in the INACTIVE mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_inactive_mode
    Procedure to handle external events while the L1 task is in the
    INACTIVE mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_inact_if.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
17/05/19   nsa       CR2454628 during CSFB, do RnN only for enabled bands in RFC  
09/04/18   sk        CR2217923 To limit the CR2212372 fix to Blind HO scenarios
13/02/17   ks        CR2005881 : KW P1 errors on AT.2.3 fix
27/03/17   sk        CR2019715 Release Tx chain when exchanged by GRR from IRAT client, for X2G CCO/resel procedures
28/09/17   asr       CR2118220: Use corret RR api to fill RF power class in FTM mode
17/04/17   nm        CR2032010 make rf power class variable dual data spaced
11/09/16   mn        CR1087630 Initialize power class for Band supported based on gas_id
09/09/16   br        CR907911 Call init_l1_multi_sim() during stack activation to initialise multi sim data.
15/12/17   ap        CR1103567 FR36133: Extended LTE Signaling (ELS) to manage tune-away gaps in DSDS 
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
03/06/16   hd        CR1021365 Send MCPM_GERAN_DATA_STOP_REQ while exiting the FTM mode
18/5/16    sm        CR1017901 Added GL1 idle mode QSH events 
05/02/16   mko       CR972145:Extension of 938347 fix, Change wtog_bplmn_task_substate to WTOG_BPLMN_STARTING when gl1 gets resume req.
                     It makes gl1 to wait for 1frame after RF/FW load before giving the SLEEP REQ.
21/11/15   zf        CR941844: Handle x2g_ho_in_progress in Start GSM mode
17/11/15   sjw       CR938544 Ensure the DSDS handler for BPLMN subtracts the X2G frame overhead.
09/11/15   og        CR929332. Don't do immediate logging of power monitors when
                     suspending GSM L1.
20/10/15   smd       CR927648 GL1 goes to the SS code path for wtog bplmn if modem is SS. 
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
16/09/15   cjl       CR908527 improve system procedures used for TRM access
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
19/08/15   hd        CR892269 Release the Tx client at the start of X2G CCO procedure
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons 
21/07/15   ap        CR876593 GERAN should not  retain the transmit chain  after X to G Cell Change Order
21/07/15   cjl       CR876741 Replace TRM subreasons with system procedure types
30/06/15   ws        CR852519 - Resolve GL1 enum conversion compile warnings
15/06/15   cjl       CR854431 Use correct TRM reason after IRAT reselection
30/04/15   ap        CR811057 FR22272 - TRM Unification  - Logging Changes
23/04/15   cjl       CR822231 Cross WTR IRAT reselection changes to GL1 exchange mechanism
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
16/03/15   pg        CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP  
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
09/12/14   nk        CR767805 Update MultiSim Client IDs when started in GSM MODE BPLMN
24/11/14   sjv       CR748431 Update DR mode flags after Multi_sim_mode has been set on receiving start_gsm_mode
06/11/14   nk        CR751005 Update MMode when handling START_GSM_MODE_BPLMN_REQ
03/11/14   nm        CR749542 reset trm client ids for all subs when doing startmode on any sub except when pch xfer enabled
06/11/14   cs        CR744751 Correctly send firmware valid imsi during RESUME_REQ
27/10/14   sk        CR745031 Ensure client ID's initialised for all the subs when start mode req received
29/10/14   cah       CR747188- incorrect bands reported to TRM/ DR mode not enabled correctly.
29/10/14   ab        CR745429 - Featurization correction for PCHXFER for SS / CR merges.
28/10/14   ss        CR746380 Set TRM extention flag outside of grm intialisation function.
17/10/14   cs        CR735918 Assign supported bands for initial ACQ based on UE support
03/10/14   dv        CR734079 Initalise TRM bands for G2X SS mode as well.
30/09/14   dv        CR726273 Initalise TRM bands for X2G SS mode as well.
22/09/14   ap        CR720862: FR 20912: SGLTE+G DSDA with WTR asymmetric GSM support
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
01/09/14   cja       CR715411 Add support of MPH_DEEP_SLEEP_IND
21/08/14   dv        CR711417 Ensure DR mode is enabled when camping on cell
08/08/14   ka        CR706866 FR18739: DR-DSDS RxD Idle, part 2 - new TRM clients
06/08/14   ka        CR699964 Implement Idle RXD support in GL1
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
04/08/14   pa      CR701897: In WTOG BPLMN Resume, check RF device before initialising mult-sim FW and RF.
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
24/07/14   ka        CR692214 TRM APS feature changes required with new TRM api's
09/07/14   am        CR691142 Update new multisim mode when all subs are inactive
21/07/14   cja       CR696687 Complete fis for CR690067 (lost in merge/promotion)
21/07/14   cs        CR697245 New GRM API support for WTR Hopping
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
07/07/14   cja       CR690067 Make W2G BPLMN suspend/resume for dual SIM more like single SIM
23/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/04/14   pjr       CR600612 Dedicated WTR Hop initial code drop
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
29/04/14   pjr       CR600612 Dedicated WTR Hop initial code drop
13/05/14   cja       CR655926 Use worst case time for hw init in W2G BPLMN calculations as FN set during init.
25/04/14   dg        CR654695 Back-out CR649702 to avoid regression
04/04/14   sai       CR638388: Delay TBF re-establish in DSDS if G2W resel ongoing on other sub.
04/04/14   pg        CR643846 RFLM/FED architecture support for multi-SIM
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
26/03/14   ka        CR630601: FR 18493: adaptive page skipping in 1xSRLTE+G DSDS (GERAN L1 changes)
13/03/14   dv        CR621050: G2W HO call drop:Only release TRM lock after RF api calls made.
09/03/14   smd       CR628651: Put both LOCK and UNLOCK under MULTI_SIM feature
06/03/14   smd       CR627298: Fixed the race condition when GS1 and GS2 processing START_GSM_REQ at the same time.
13/02/14   ka        CR614268: Add debug for BPLMN search not resuming
30/01/14   mc        CR609473 SGLTE COEX : GAS needs to register with COEX management
24/01/14   cah       CR598552: enable sleep for DSDS GSTMR+VSTMR
11/26/13   zc        CR564251: change hard-coded grant to sub1 in TRM init
08/01/13   js        CR597012: Removal of GERAN compilation errors for T+G build
07/01/14   cgc       CR516981 propagation from 2.x, add l1_sc_wcdma_set_mode_from_init()
14/12/13   sjw       CR589502: Install different unlock callback for change of acq priority
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
13/11/13   js        CR564251:Remove hard coded sub in gl1_hw_trm_init
05/12/13   sjw       CR583608: Use TRM lock acquired using inter-RAT exchange
28/11/13   cja       CR521061 Remove unused RF API and mainline FEATURE_GSM_RFA_TASK.
10/10/13   cgc       CR556101 added L1_G2X_terminate()
05/10/13   gk        CR534923:Make sure DRX has been reset before gl1 init
01/10/13   sk        CR552259 CR sync from 1.0/1.1
14/08/13   fan       CR528515 -  Race condition in START_GSM_MODE handler does not get chance to reset l1_drx before the next GSTMR INT is fired
16/09/13   cs        CR503042 Updated XO RGS handling for Triton
17/08/13   ss        CR530647 Initialize arbitrator while processing MPH_START_GSM_MODE_FOR_BPLMN_REQ
26/08/13   gk        CR484897:W2G handover using exchange of TRM lock
21/08/13   npt       CR531633 - Remove FEATURE_DIME_MODEM dependencies
15/08/13   sk        CR524039 GBTA bringup changes
05/08/13   sk        CR523406 Ensure GL1 registers with arbitrator always on SUB activation
08/05/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
29/07/13   sk        CR519663 Partial QBTA bringup changes
25/06/13   cs        Major Triton TSTS Syncup
16/05/13   ab        CR487396 - Support EGPRS IR Reduced Window in GFW
23/05/13   ss        CR490280 Clear IRAT SC list while terminating GSM
23/05/13   ss        CR490459 Execute L1_resume_req independent of dual sim mode
23/05/12   sjw       CR471047 Explicit reject of CB messages in inactive mode, to decommit memory
15/05/13   cja       CR487801 When suspend and GSM stop at the same time call inact task
10/06/13   cs        Further TSTS Updates
06/06/13   cs        TSTS Updates
24/04/13   sjw       CR304803: ensure container for START_GSM_MODE_FOR_BPLMN is initialised
05/04/13   mc        CR470155 : COEX Support - GERAN arbitrator framework - PS integration
02/04/13   ss        CR470243 Inact task while deactivating from GSM mode even in dual sim mode
25/03/13   mc        CR464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc        CR459487 : COEX Support - GERAN arbitrator framework
20/02/13   cgc       CR445920 add mdsp_wakeup() for single sim mode
10/02/13   ip        CR448394 use start mode request multisim setting
07/02/13   ap        CR449348 DSDS W/G+G IRAT MO Voice: HO from W2G requires attention
02/02/13   ip        CR444436 GFW APP voting mechanism for two clients
23/01/13   ss        CR444575: Set mdsp_awake_flag to TRUE/False while enable/disabling GFW app
25/02/13   og        CR414084. Ensure rxlm and txlm buffers are deallocated when
                     the suspend confirmation is received.
22/02/13   cja       CR443834 Extend mdsp_awake to indicate FW enable/disable
14/02/13   pjr       CR452272 added support to set multisim mode at activation time
17/08/12   ip        CR384421 Disable GSM sleep during CCO activity
20/06/12   cja       Fix compiler warning
14/03/12   pg        Add support for local HWIO macros (msm.h deprecated)
21/12/11   cja       CR316437 Inhibit sleep on RAT change until it is complete
07/12/11   cgc       CR 323173 Add service for ISR_SUSPEND_CNF
09/09/11   cja       CR303806  Ensure G2X disable on suspend and inactive
26/08/11   og        Port over CR286561.
30/03/11   cja       Add initial changes for Nikel IRAT
12/01/11   ws        Removed GP clk switching for NikeL
22/02/11   ip        CR267278 GP clock vote handled by RF drivers
09/02/11   og        CR273622. Decrease the available BPLMN search time to take
                     into account initialization delays.
23/06/11   sjw       DSDS - CR293304 clear going_active flags when in single standby
28/04/11   pg        Fix compiler warnings
28/02/11   ip        Fix compilation error
22/02/11   ip/cs     CR267278 GP clock vote handled by RF drivers
26/01/11   pg        Added FEATURE_GSM_GP_CLK_DISABLE for sourcing GSM clock
                     from MSM in all modes i.e. GP_CLK always disabled
10/01/11   ap        DSDS CR270377 SYNC SIM is disabling L1 ISR while ASYNC SIM is still
                     active when consecutive start/stop GSM mode req
30/11/10   ab        Support EGPRS WS REDUCTION during allocation of dynamic memory in start
                     gsm mode request when FEATURE_GSM_EGPRS_IR_OPT is defined.
22/11/10   ky        Panic reset mechanism is changed. For all errors gl1 first
                     sees an nv item set or not. if this nv item is set then does
                     error fatal immediately other wise gl1 sends signal to grr for
                     recovery
08/11/10   dv        CR262337 - RRC abort timer times out for larger values
                     of drx_interval
28/07/10   ws        Added missing msm.h from previous checkin
24/07/10   pg        CR248443: Avoid forcing tripler on if SWC is not enabled
05/07/10   rc        Enables GP clock if its off in Q6
25/06/10   ap        CR244291 - Prevent frequency list memory freeing with FEATURE_EOOS
22/04/10   ab        CR: 234113 - For FEATURE_GSM_EGPRS_IR_OPT - 32 byte alignment for
                     heap address as part of requirement for DMA by FW
23/03/10   cs        Fix Lint indent warning
22/03/10   ip        Do not check for (!clk_regime_is_on(CLK_RGM_GSM_M))
19/03/10   ip        CR 230249 Ensure tripler is on !!
10/03/10   pg        CR229343 Change BPLMN sub_state during RESUME msg handling
02/03/10   ip        Modification to resolve compiler issue
25/02/10   ip        CR227540 Add NULL pointer check for Mem Heap alloc/dealloc
17/12/09   cs        Fix previous integrated update for surious FEATURE define
21/08/09   cs        CR154851 Fix the RX Sweep for newer targets to properly cleardown
06/06/09   ab        Feature FEATURE_GSM_EGPRS_IR_OPT for LCU
29/05/09   ps        Zero Initialise memory relocation to Heap for selected data structure
18/05/09   ws        Featured off rfm_turn_on_ldo() for POSEIDON
30/03/09   ws        Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                     to replace FEATURE_GSM_GPRS_MUSTANG
04/12/08   og        Mustang G2W IRAT integration.
21/10/08   ws        Added missing FEATURE_GSM_GPRS_LCU to rf_turn_on_rf_ldo()
09/10/08   cs        Allow the RX Sweep acquisition to be restarted
23/09/08   cs        CR149660 Add in LPM changes so we can support UMTS targets
28/08/08   cs        Implement the Fast Rx Sweep functionality for FTM
17/04/08   cs        CR141866 New LPM mode for 7500 that doesn't break GPS
05/08/08   ws        Configure RF LDO for LCU
05/11/07   og        Add NV item 1027 support to EGPRS BEP.
12/10/07   cs        CR129559 Add support for asserting sleep at task startup
17/09/07   agv       CR127820 - iRAT W2G handover AGC setting.
06/02/07   cs        Fix all sig definitions in l1_os.h
05/16/06   pp        Support for unified sleep task.
03/01/05   ws        Initialise main_command to NULL_COMMAND CR 88269
12/12/05   gw        Added support for low power mode.
11/15/05   sv        Lint cleanup.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
11/07/05   sv        Initialize BPLMN state when activating GSM.
29/09/05   og        Merge to fixes of CR74437 and CR70332 from the mainline branch
                     Fixes inter RAT TC and 42.4.8.4.5 respectively.
06/09/05   gw        Moved call to L1_disable_ftm_mode() after call to
                     L1isr_send_MPH_STOP_GSM_MODE_CNF
05/02/05   gw        Move calls to sleep_enter_mode()/sleep_leave_mode()
                     to task context.
04/25/05   bm        Disable FTM mode after sending STOP_GSM_MODE_CNF
04/21/05   sv        Add support for L1 "Panic Reset"
04/20/05   og        Addition of filtered bep params to the msg metrics log packet.
03/22/05   gw        Make sure that L1 is voting against TCXO shutdown during
                     initialization.
02/11/04   bm        Support for Background WCDMA PLMN Scan
01/28/05   yh        Added a signal for mDSP download timeout
09/12/04   og        Fix to tx power control by tracking c value on ccch.
09/24/04   bm        Support for split up the L1 deactivation into TASK and ISR
08/23/04   dp        Added support for "fast camping" for FTM BER measurements
08/03/04   gw        Ensure that L1 starts up with MM non-DRX mode disabled.
06/23/04   gw        Handle case where MPH_STOP_GSM_MODE_REQ comes right after
                     MPH_START_GSM_MODE_REQ.
06/11/04   bm        Remove setting state to NULL, and remove sending
                     START_GSM_MODE_CNF. This is moved to l1_isr in INACT mode.
03/22/04   gfr       Support for the ISR watchdog.
02/25/04   gw        Lint clean up.
06/23/03   gfr       Compiler warning cleanup.
06/06/03   tb        Support for dedicated mode gtow searching.
04/28/03   tb        Handling of new gsm_only field in MPH_START_GSM_MODE_REQ
03/25/03   jc/dl     Added call to gl1_msg_init () when going active.
03/11/03   gw        Added call to gl1_drx_reset() when going active.
10/08/02   ATM       FEATURE_MULTIMODE_GSM is mainlined - removed flag per jchao
10/04/02   gw        Added code to tell sleep task when GSM becomes active.
                     Required to support sleep in dualmode.
06/02/02   jc        Moved sbi_set_mode(), renamed l1_tch.
04/23/02   JC        Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"
#include "geran_tasks.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "memory.h"
#include "ms.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "l1.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "l1_drx.h"
#include "l1_sc.h"

#include "l1_os.h"

#include "ftm.h"

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"

#include "gprs_mem.h"
#include "gpl1_dual_sim.h"
#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "rfm.h"

#include "gpl1_grm_intf.h"

#include "mcs_hwio.h"
#include "fws.h"

#include "gtmrs.h"
#include "gtmrs_g.h"

#include "rr_multiband.h"
#include "rr_multiband_g.h"
#include "mcfg_nv_api.h"

#include "l1_sc.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif 


extern void l1_set_pscan_validate_results(boolean l1_pscan_validate_results, gas_id_t gas_id);
extern gprs_pl1_serving_cell_meas_T gprs_serv_cell_meas[];

extern void L1_initialise_L1_data( gas_id_t gas_id );

void L1_enable_ftm_mode( gas_id_t gas_id );
void L1_disable_ftm_mode( gas_id_t gas_id );
boolean L1_ftm_mode_enabled( void );

extern void l1_call_panic_reset_in_task( l1_panic_reset_type_T error_code, gas_id_t gas_id );

typedef struct
{
  boolean l1_in_ftm_mode; /* only used by FEATURE_FTM_GSM_BER */
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
  uint32 drx_interval;
#endif /* FEATURE_WTOG_BACKGROUND_PLMN_SEARCH */
}l1_inact_if_data_t;

static l1_inact_if_data_t  l1_inact_if_data[NUM_GERAN_DATA_SPACES];

static void l1_enter_lpm( gas_id_t gas_id );

static void l1_xmsi_use_cached_params( gas_id_t gas_id );

/* RF power class per band for egsm,pgsm,1900,1800,850 */
byte gl1_rf_power_class_per_band[NUM_GERAN_DATA_SPACES][5];

/*===========================================================================
FUNCTION  l1_init_band_power_class

DESCRIPTION
  Obtains the RF power class from RR for each band supported.
  It does not considers what multiband MS supports.
  However its usage is always under teh right supported band.
  It assumes RR isgiving right Power class

DEPENDENCIES
  None

RETURN VALUE
  Void

SIDE EFFECTS
  None

===========================================================================*/
static void l1_init_band_power_class(gas_id_t gas_id)
{
  if (L1_ftm_mode_enabled())
  {    
    gl1_rf_power_class_per_band[gas_id][BAND_EGSM]    = rr_get_rf_power_class(BAND_EGSM, gas_id);;
    gl1_rf_power_class_per_band[gas_id][BAND_PGSM]    = rr_get_rf_power_class(BAND_PGSM, gas_id);
    gl1_rf_power_class_per_band[gas_id][BAND_PCS]     = rr_get_rf_power_class(BAND_PCS, gas_id);
    gl1_rf_power_class_per_band[gas_id][BAND_DCS]     = rr_get_rf_power_class(BAND_DCS, gas_id);
    gl1_rf_power_class_per_band[gas_id][BAND_GSM_850] = rr_get_rf_power_class(BAND_GSM_850, gas_id);
  }
  else
  {
    gl1_rf_power_class_per_band[gas_id][BAND_EGSM]    = rr_gapi_get_band_rf_power_class(BAND_EGSM, gas_id);
    gl1_rf_power_class_per_band[gas_id][BAND_PGSM]    = rr_gapi_get_band_rf_power_class(BAND_PGSM, gas_id);
    gl1_rf_power_class_per_band[gas_id][BAND_PCS]     = rr_gapi_get_band_rf_power_class(BAND_PCS, gas_id);
    gl1_rf_power_class_per_band[gas_id][BAND_DCS]     = rr_gapi_get_band_rf_power_class(BAND_DCS, gas_id);
    gl1_rf_power_class_per_band[gas_id][BAND_GSM_850] = rr_gapi_get_band_rf_power_class(BAND_GSM_850, gas_id);

     MSG_GERAN_HIGH_5_G("Init power class :  %d[EGSM] , %d[PGSM,  %d[PCS],  %d[DCS],  %d[850]", 
                         gl1_rf_power_class_per_band[gas_id][BAND_EGSM], 
                         gl1_rf_power_class_per_band[gas_id][BAND_PGSM],
                         gl1_rf_power_class_per_band[gas_id][BAND_PCS] ,
                         gl1_rf_power_class_per_band[gas_id][BAND_DCS],
                         gl1_rf_power_class_per_band[gas_id][BAND_GSM_850]);
  }
} /* l1_init_band_power_class */

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
uint32 l1_get_drx_interval( gas_id_t gas_id )
{
  l1_inact_if_data_t *l1_inact_if_data_ptr = &l1_inact_if_data[gas_id];
  return l1_inact_if_data_ptr->drx_interval;
}

void l1_decrement_drx_interval( gas_id_t gas_id )
{
  l1_inact_if_data_t *l1_inact_if_data_ptr = &l1_inact_if_data[gas_id];

  if( l1_inact_if_data_ptr->drx_interval > 0 )
  {
   l1_inact_if_data_ptr->drx_interval--;
  }
}

void l1_subtract_drx_interval( uint32 sub, gas_id_t gas_id )
{
  l1_inact_if_data_t *l1_inact_if_data_ptr = &l1_inact_if_data[gas_id];

  if ( l1_inact_if_data_ptr->drx_interval >= sub )
  {
    l1_inact_if_data_ptr->drx_interval -= sub;
  }
  else
  {
    l1_inact_if_data_ptr->drx_interval = 0;
  }
}

void l1_set_drx_interval( uint32 val,gas_id_t gas_id )
{
  l1_inact_if_data_t *l1_inact_if_data_ptr = &l1_inact_if_data[gas_id];

  l1_inact_if_data_ptr->drx_interval = val;
}

/*===========================================================================

FUNCTION  l1_initialise_drx_interval

DESCRIPTION
  This function initialises the DRX interval counter for X2G BPLMN by reducing
  the original timer value received from GRR by a fixed TDMA frame overhead

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define X2G_BPLMN_FN_OVERHEAD 6 /* 2 frames for HW Init + 4 frames for time from abort to sending the suspend ind */
static void l1_initialise_drx_interval(const uint32 timer_val, gas_id_t gas_id)
{
  l1_inact_if_data_t *l1_inact_if_data_ptr = &l1_inact_if_data[gas_id];

  /* convert the timer duration (milliseconds) into frame ticks */
  l1_inact_if_data_ptr->drx_interval = (uint32)(MSECS_TO_FRAMES(timer_val));

  /* subtract additional overhead needed by GL1 where possible */
  if (l1_inact_if_data_ptr->drx_interval > X2G_BPLMN_FN_OVERHEAD)
  {
    MSG_GERAN_HIGH_2_G("No of frames for X2G BPLMN search: %d reduced to %d",
                       (int)(l1_inact_if_data_ptr->drx_interval),
                       (int)(l1_inact_if_data_ptr->drx_interval - X2G_BPLMN_FN_OVERHEAD));

    l1_inact_if_data_ptr->drx_interval -= X2G_BPLMN_FN_OVERHEAD;
  }
  else
  {
    MSG_GERAN_ERROR_1_G("No of frames for X2G BPLMN search: %d reduced to 0",
                        (int)(l1_inact_if_data_ptr->drx_interval));

    l1_inact_if_data_ptr->drx_interval = 0;
  }

  return;
}
#endif /* FEATURE_WTOG_BACKGROUND_PLMN_SEARCH */

/*===========================================================================

FUNCTION  L1_handle_inactive_mode

DESCRIPTION
  This function handles the L1 Task in inactive mode.    Here the L1 sits and
  waits for RR to tell it to 'go active'.  Primary implementation here is that
  in a multimode implementation, several L1's are running but only one can be
  active at a given time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_inactive_mode(IMH_T  *msg_header)
{
  gas_id_t gas_id;

  /* Check for any messages from RR */
  if (msg_header->message_set == MS_RR_L1)
  {
    switch  (msg_header->message_id )
    {
      /* Enable Sleep indication */
      case MPH_ENABLE_SLEEP_IND:
        {
          L1_handle_ENABLE_SLEEP_IND ( msg_header );
          break;
        }
      case MPH_START_GSM_MODE_REQ:
        L1_handle_MPH_START_GSM_MODE_REQ( msg_header );
        break;

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
      case MPH_START_GSM_MODE_FOR_BPLMN_REQ:
        {
          mph_start_gsm_mode_for_bplmn_req_T *message_ptr = (mph_start_gsm_mode_for_bplmn_req_T *)msg_header; /*lint !e740 */

          gas_id = check_gas_id(message_ptr->gas_id);
          MSG_GERAN_HIGH_1_G("MPH_START_GSM_MODE_FOR_BPLMN_REQ with timeout %dms", (int)(message_ptr->timer_val));
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_W2G_BPLMN_SRCH_START);
#endif

          l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_SUSPENDED,gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
          if ( IS_MULTI_SIM_MODE( message_ptr->multi_sim_mode ) )
          {
            volatile ISRTIM_CMD_BLK  *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            mph_start_gsm_mode_req_T  message_container;

            /* initialise the container */
            memset(&message_container, 0, sizeof(mph_start_gsm_mode_req_T));

            message_container.gsm_only = TRUE;
            message_container.multi_sim_mode = message_ptr->multi_sim_mode;
            message_container.gas_id = gas_id;

            /* then handle it like a normal MPH_START_GSM_MODE_REQ */
            L1_handle_MPH_START_GSM_MODE_REQ( (IMH_T *)(&message_container) );

            /* Now tell the L1 ISR to switch states */
            l1_tskisr_blk->main_command = L1_WTOG_BPLMN_COMMAND;

            /* Initialize the BPLMN search state when activating GSM */
            l1_tskisr_blk->current_params.L1_confirm_ID = l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;

            /* must initialise the global DRX interval counter here */
            l1_initialise_drx_interval(message_ptr->timer_val, gas_id);
          }
          else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
          {
            L1_handle_MPH_START_GSM_MODE_FOR_BPLMN_REQ( (IMH_T *)message_ptr );
          }
        }
        break;
#endif /* FEATURE_WTOG_BACKGROUND_PLMN_SEARCH */

      case MPH_STOP_GSM_MODE_REQ:
        {
          mph_stop_gsm_mode_req_T *message_ptr = (mph_stop_gsm_mode_req_T *)msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);

          L1_setup_DEACTIVATE( msg_header );
          MSG_GERAN_HIGH_0_G("MPH_STOP_MODE_REQ in INACT.");
        }
        break;

      case MPH_RAT_CHANGE_IND:
        {
          mph_rat_change_ind_T *message_ptr = (mph_rat_change_ind_T *)msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);

          gl1_hw_inact_deep_sleep(TRUE, gas_id);
          l1_tsk_buffer[gas_id].acq_for_blind_ho = FALSE;
          MSG_GERAN_HIGH_0_G("MPH_RAT_CHANGE_IND in INACT");
        }
        break;

      case MPH_DEEP_SLEEP_IND:
        {
          mph_deep_sleep_ind_T *message_ptr = (mph_deep_sleep_ind_T *)msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          gl1_hw_inact_deep_sleep(FALSE, gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_OOS);
#endif
          MSG_GERAN_HIGH_0_G("MPH_DEEP_SLEEP_IND in INACT");
        }
        break;

      case MPH_SET_PRIORITY_IND:
        {
          /* no specific action required */;
        }
        break;

#ifdef FEATURE_MODEM_CONFIG_REFRESH
      case MPH_NV_REFRESH_REQ:
        {
          L1_handle_MPH_NV_REFRESH_REQ( msg_header );
          break;
        }
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

      default:
        MSG_GERAN_ERROR_1("RR->L1(INACT) message %d ignored.", msg_header->message_id);
        break;
    }
  }
  else if (msg_header->message_set == MS_L1_L1)
  {
    switch  (msg_header->message_id )
    {
      case ISR_DEACT_CNF:
        {
          isrtim_deact_cnf_T *message_ptr;
          /* align pointers */
          message_ptr = (isrtim_deact_cnf_T *)msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);

          /* Force G2X shutdown */
          l1_sc_irat_check_active(0, 0, TRUE, 0, FALSE, gas_id);

          gl1_hw_inact_task( message_ptr->deep_sleep, FALSE, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

          if ( gl1_msg_get_multi_sim_mode() )
          {
             /* Do this AFTER inact_task called so that RF api's called*/
             l1_swap_multi_sim_sync_state_or_deactivate(gas_id);
          }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

          if (message_ptr->deep_sleep)
          {
#ifndef FEATURE_UNIFIED_SLEEP_TASK
            sleep_enter_mode( SLEEP_MODE_GSM_DEEP_SLEEP );
#endif
          }
          else
          {
#ifndef FEATURE_UNIFIED_SLEEP_TASK
            sleep_leave_mode( SLEEP_MODE_GSM );
#endif
          }

          L1isr_send_MPH_STOP_GSM_MODE_CNF (gas_id);

          /* Disable FTM mode after sending the STOP_MODE_CNF to FTM */
          L1_disable_ftm_mode (gas_id );

        }
        break;

      case L1_ENTER_LPM_REQ:
        {
          l1_enter_lpm_req_T *message_ptr = ( l1_enter_lpm_req_T* )msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);
          l1_enter_lpm(gas_id);
        }
        break;



        /*
         ** CgC Service transition from L1_WTOG_BPLMN_MODE -> L1_SUSPEND
         ** when MPH_STOP_MODE_REQ is also received at same time.
         */
      case ISR_SUSPEND_CNF:
        {
          isrtim_suspend_cnf_T* message_ptr = ( isrtim_suspend_cnf_T* )msg_header;
          gas_id_t gas_id = check_gas_id(message_ptr->gas_id);

          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

          gl1_hw_inact_task(FALSE, TRUE, gas_id);
          L1_send_MPH_SUSPEND_GSM_MODE_IND(gas_id );
          l1_set_bplmn_suspend_in_progress(FALSE,gas_id);

          if(( l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE ) && (l1_get_gsm_stop_req_in_queue(gas_id)))
          {
              l1_tskisr_blk->main_command = L1_NULL_COMMAND;
              l1_tskisr_blk->command   = L1_NULL_COMMAND;
              l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_SUSPENDED,gas_id);
              l1_set_wtog_bplmn_task_state(NO_HPLMN_SEARCH,gas_id);
              l1_set_gsm_stop_req_in_queue(FALSE,gas_id);
              l1_change_state(L1_INACTIVE_MODE,gas_id);
              L1isr_send_MPH_STOP_GSM_MODE_CNF (gas_id);
              MSG_HIGH("###WTOG queued stop req actioned FN=%d",gl1_get_FN(gas_id),0,0);
          }
        }
        break;

      default:
        MSG_GERAN_ERROR_1("GERAN: Inactive mode L1 message id=%d ignored, FN=%d", msg_header->message_id);
        break;
    }
  }
  else if (msg_header->message_set == MS_CB_L1)
  {
    if (msg_header->message_id == CB_SCHEDULING_REQ)  /*lint !e641 */
    {
      cb_scheduling_req_type *message_ptr;
      message_ptr = (cb_scheduling_req_type *) msg_header; /*lint !e740 */

      gas_id = geran_map_nas_id_to_gas_id(message_ptr->as_id);

      /* reject the message explicitly so that CB can decommit the memory */
      L1_send_CB_SCHEDULING_CNF(message_ptr->scheduling_descriptor.scheduling_period_fn_list,
                                gas_id);

    }
    else if (msg_header->message_id == CB_SKIP_REQ)  /*lint !e641 */
    {
      cb_skip_req_type *message_ptr;
      message_ptr = (cb_skip_req_type *) msg_header; /*lint !e740 */

      gas_id = geran_map_nas_id_to_gas_id(message_ptr->as_id);

      /* reject the message explicitly so that CB can decommit the memory */
      L1_send_CB_SKIP_CNF(message_ptr->scheduling_descriptor.scheduling_period_fn_list,
                          gas_id);
    }
  }

  return;
}


/*===========================================================================

FUNCTION  L1_handle_reset_mode

DESCRIPTION
  This function handles the L1 Task in Reset mode. We have reached here because
  either the L1 or RR has detected a failure in the lower layers and so we are
  trying to reset our software\hardware to a good state without doing a hard reset
  or power down. Here the L1 sits and waits for RR to tell it to 'go active'.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_handle_reset_mode(IMH_T  *msg_header)
{

  /* Check for any messages from RR */
   if (msg_header->message_set == MS_RR_L1)
   {
      switch  (msg_header->message_id )
      {
         case MPH_START_GSM_MODE_REQ:
            L1_handle_MPH_START_GSM_MODE_REQ( msg_header );
            break;

         default:
            MSG_GERAN_ERROR_1("RR->L1(RESET) message %d ignored.", msg_header->message_id);
            break;
      }
   }
}
/*===========================================================================

FUNCTION  L1_handle_ENABLE_SLEEP_IND

DESCRIPTION
  This function handles the ENABLE_SLEEP_IND from RR.
  This will allow DRX manager to go to sleep after being prevented
  or held off due to CCO request.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_handle_ENABLE_SLEEP_IND( IMH_T  *msg_header )
{
  gas_id_t gas_id;
   mph_enable_sleep_ind_T *message_ptr = (mph_enable_sleep_ind_T *)msg_header; /*lint !e740 */
   gas_id = check_gas_id(message_ptr->gas_id);

   MSG_GERAN_HIGH_0_G("ENABLE SLEEP IND given from RR");
   gl1_drx_set_sleep_inhibit_for_cco_req ( FALSE, gas_id );
}

/*===========================================================================

FUNCTION  L1_handle_MPH_START_GSM_MODE_REQ

DESCRIPTION
  This function handles the MPH_START_GSM_MODE_REQ from RR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_handle_MPH_START_GSM_MODE_REQ( IMH_T  *msg_header )
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   int      as_idx;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   mph_start_gsm_mode_req_T *message_ptr = (mph_start_gsm_mode_req_T *)msg_header; /*lint !e740 */
   gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* initialise the global frequency list storage */
   l1_frequency_list_mem_init(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
 /* When two start_gsm_mode requests (1 for each sim) are close together
  * there is a possibility that the ISR can interrupt  the task, handling start
  * 2nd gsm mode request while sim state is still unallocated i.e not sync or async.
  * This causes ISR not to revert the task pointers to 2nd sim. This is prevented
  * using following  ISR lock.
  */

  GL1_ISR_SAVE_LOCK(gas_id);
#endif

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  /* Clear the g2x resel in progress flag */
  l1_tsk_buffer[gas_id].g2x_resel_in_progress = FALSE;
#endif
 /* Always reset the iRAT handover Ind used for AGC. */
   l1_interrat_ho_update(FALSE, gas_id);

   l1_tsk_buffer[gas_id].acq_for_blind_ho = FALSE;

   /* Enable L1 FTM mode if set in FTM.  Its remains set till
    * its reset when L1 gets deactivated
    */
   if ( ftm_get_mode() == FTM_MODE )
   {
      L1_enable_ftm_mode(gas_id);
   }
   else
   {
      L1_disable_ftm_mode(gas_id);
   }

 #ifndef FEATURE_UNIFIED_SLEEP_TASK
   /* Tell sleep task to be in GSM mode */
   sleep_enter_mode( SLEEP_MODE_GSM );
 #endif

   /* Set inhibit sleep flag*/
   gl1_drx_set_sleep_inhibit(message_ptr->x2g_resel_in_progress, gas_id);
 
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
   if( message_ptr->x2g_resel_in_progress)
   {
     gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_IRAT_RESEL);
   }
   if (message_ptr->x2g_cco_in_progress)
   {
     gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_IRAT_CCO);
   }
#endif
   /* Set inhibit sleep flag for cco requests to pretect CFN number mismatch on WCDMA */
   /* This flag is cleared when we receive MPH_ENABLE_SLEEP_IND */
   gl1_drx_set_sleep_inhibit_for_cco_req (message_ptr->x2g_cco_in_progress, gas_id);

  /* Initialize DRX manager */
   gl1_drx_reset(gas_id);

   /* Initialise the L1 data variables */
   L1_initialise_L1_data(gas_id);
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   init_l1_multi_sim(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   /* initialize the power class for band supported */
   l1_init_band_power_class(gas_id);

#if !defined ( FEATURE_DUAL_SIM ) && !defined ( FEATURE_TRIPLE_SIM )
   /* Only perform this operation if this is the first time through i.e. non-dual sim or if
   dual sim is about to become active then flag will be FALSE still */
   /* Reset the message layer handlers */
   gl1_msg_init (gas_id);
#endif /* !FEATURE_DUAL_SIM & !FEATURE_TRIPLE_SIM */

   /* Preset main command in case ISR interrupts this function */
   l1_tskisr_blk->main_command = L1_NULL_COMMAND;

   /* Now initialize the hardware.  The RF has a dependency on NV */
   /* being started this is why we are initializing after receipt */
   /* of the TASK_START_SIG */

   /* QDSP6 IRAT integration - Ensure this is set prior to calling gl1_hw_init( ) */
   gl1_hw_set_gsm_only_mode( message_ptr->gsm_only, gas_id );

#ifdef FEATURE_SGLTE
   /* Set SGLTE mode - Ensure this is set prior to calling gl1_hw_init( ) */
   gl1_hw_set_sglte_mode( message_ptr->sglte_mode_active, gas_id );
#endif

   /* Set the client IDs for each sub */
   L1_set_multi_sim_client_ids( gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /*
   If this is a start up for a DUAL sim then the following must be ascertained
      1. What is the priority of this SIM - notify TRM
      2. Is there already a SIM active, and if so this must therefore be the ASYNC SIM
   */
  if ( IS_MULTI_SIM_MODE( message_ptr->multi_sim_mode ) )
  {
    uint8 i, num_inactive_subs = 0;

    GL1_MULTI_SIM_MODE_LOCK();

    for ( i = GERAN_DATA_SPACE_INDEX_1; i < NUM_GERAN_DATA_SPACES; i++ )
    {
      if( l1_tsk_buffer[i].stop_gsm_mode_cnf_sent )
      {
        num_inactive_subs++;
      }
    }

    /* check the global flag for Dual SIM */
    if ( gl1_msg_get_multi_sim_mode() && ( num_inactive_subs != NUM_GERAN_DATA_SPACES ) )
    {
      if( gl1_msg_get_multi_sim_sys_mode() != message_ptr->multi_sim_mode )
      {
        MSG_GERAN_HIGH_3_G("Multi-SIM mode update discarded: new mode %d, previous mode %d, num_inactive_subs %d",message_ptr->multi_sim_mode,gl1_msg_get_multi_sim_sys_mode(),num_inactive_subs);
      }

      /* Set a flag to indicate the transition from Inactive to Active state.
       This is required to combat a race condition */
      l1_tskisr_blk->going_active = TRUE;

      /* ensure that everything is awake to process the new setup */
      gl1_drx_require_next_tick(gas_id);
    }
    else
    {
      MSG_GERAN_HIGH_3_G("Multi-SIM now active (primary client=%d) new mode %d, previous mode %d",(int)(l1_tskisr_blk->client_id), message_ptr->multi_sim_mode, gl1_msg_get_multi_sim_sys_mode());

      /* Multi SIM's not yet active  */
      gl1_msg_set_multi_sim_sys_mode(message_ptr->multi_sim_mode);

      /*set device configuration for DR once multi sim is active*/

    }
    GL1_MULTI_SIM_MODE_UNLOCK();
   }
   else
   {
     /* Multi SIM not yet */
     gl1_msg_set_multi_sim_sys_mode(message_ptr->multi_sim_mode);

     for ( as_idx = GERAN_DATA_SPACE_INDEX_1; as_idx < NUM_GERAN_DATA_SPACES; as_idx++ )
     {
       l1_tsk_buffer[as_idx].going_active = FALSE;
     }

   }
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

   /* initialise GRM info for the client that is starting up */
   grm_initialise( l1_tskisr_blk->client_id, gas_id );

   /* at start up initialise for all the supported gsm sys bands */
   grm_set_valid_acquisition_bands( l1_tskisr_blk->client_id, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* Indicate to TRM APS reasons */
   grm_register_aps_info(l1_tskisr_blk->client_id, gas_id);

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
   /* Register client for ELS */
   grm_els_state_registration(l1_tskisr_blk->client_id, gas_id);
#endif  /* FEATURE_CRAT_MSIM_TEST_FRAMEWORK */

  l1_tsk_buffer[gas_id].stop_gsm_mode_cnf_sent = FALSE;

  /* Interrogate TRM to discover if device supports DRDS */
  grm_set_device_configuration( gas_id );

  /* Check for status at startup */
  grm_set_dr_enabled_status(l1_tskisr_blk->client_id, gas_id);

  GL1_ISR_SAVE_UNLOCK(gas_id);

/* with PCH in transfer feature, there are new trm clients which needs to do grm init*/
#ifdef FEATURE_DUAL_SIM
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    grm_initialise(GRM_GPRS1, gas_id);
  }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    grm_initialise(GRM_GPRS2, gas_id);
  }
#endif /* FEATURE_DUAL_SIM */
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
  if (gas_id == GERAN_ACCESS_STRATUM_ID_1)
  {
    grm_initialise(GRM_GSM1_SECONDARY, gas_id);
  }
  else if (gas_id == GERAN_ACCESS_STRATUM_ID_2)
  {
    grm_initialise(GRM_GSM2_SECONDARY, gas_id);
  }
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */

  L1_enable_dual_receive(l1_tskisr_blk->client_id, gas_id);

  /* check whether an RF chain has already been granted to this stack */
  /* using the Multimode TRM Exchange mechanism in the protocol layer */
  if (geran_is_multi_mode_gas_id(gas_id))
  {
    trm_get_info_return_type get_grant;

    grm_client_enum_t        this_client = gl1_map_gas_id_to_client_id(gas_id);

    if ((l1_tskisr_blk->client_id == this_client) &&
        (GRM_NO_CLIENT            != this_client))
    {
      if (TRM_DENIAL != grm_get_granted( this_client, &get_grant, gas_id ))
      {
        /* lock has been acquired on behalf of this client already */
        /* but we cannot use it with new rx/tx mapping so release it and get a suitable chain */
        trm_resource_enum_t     requested_chain_type;
        trm_duration_t          duration;
        trm_reason_enum_t       reason;
        sys_proc_type_e_type    sub_reason;
        grm_tag_t               tag;

        grm_info_t  *grm_info_p = grm_get_info_ptr_from_client(this_client, gas_id);
        uint32 i;

        /* If X2G HO in progress, using TRM_TRAFFIC reason. */
        if ( message_ptr->x2g_ho_in_progress )
        {
          reason = TRM_TRAFFIC;
        }
        else
        {
          reason = get_grant.info.client_info.reason;
        }

        requested_chain_type = gl1_ms_get_trm_resource (gas_id, reason);

        /* Not sure whether to expect the IRAT Client to pass a dynamic connected mode reason or not so check both */
        /* If lock exchanged with TRAFFIC reason for x2G CCO/resel, ensure Tx chain is released explicitly here */
        if (  ( TRM_TRAFFIC == reason ) && ( message_ptr->x2g_resel_in_progress == FALSE ) && ( message_ptr->x2g_cco_in_progress == FALSE ) )
        {
          duration = GRM_ACCESS_DURATION_SCLKS;
          tag = GRM_RAN_ACCESS_RETRY;
          sub_reason = gl1_get_specific_sys_proc (GERAN_CONN_PROCEDURE, gas_id);
        }
        else
        {
          duration = timetick_cvt_to_sclk( RESELECTION_DURATION_MS, T_MSEC );
          sub_reason = SYS_PROC_TYPE_IRAT_CELL_RESELECTION;
          reason = TRM_CHANNEL_MAINTENANCE;
          tag = GRM_RANE_RESELECTION;

          /* Release the Tx chain as it is not required for CCO */
          grm_release_tx_chain(grm_map_rx_to_tx_client_id(this_client, gas_id), gas_id);

        }
 
        
        for (i = 0; i < MAX_BAND_INFO_GRM; i++ )
        {
          grm_info_p->grm_freq_info.bands[i].band = grm_valid_gsm_sys_bands[gas_id][i];
        }
        
        grm_info_p->grm_freq_info.num_bands = grm_valid_gsm_sys_bands_cnt[gas_id];

        MSG_GERAN_HIGH_3_G("swap IRAT client chain for GSM chain type=%d reason=%d subreason=%d",requested_chain_type,reason,sub_reason);

        grm_force_request_and_notify ( this_client,
            requested_chain_type,
            duration,
            reason,
            sub_reason,
            tag);
      }
    }
    else
    {
      /* something strange in the global data setup */
      MSG_GERAN_ERROR_2_G("Client ID mismatched %d vs %d",
                          (int)(l1_tskisr_blk->client_id),
                          (int)this_client);
    }
  }

   if ( gl1_msg_get_multi_sim_mode() )
   {
     gl1_hw_multi_sim_init(l1_tdma_1_ISR,
            l1_task_tcb_read(gas_id),
            gsm_l1_wait,
            L1_WAIT_HW_SIG,
            L1_TIMEOUT_HW_SIG,
            gas_id);
   }
   else
   {
      gl1_hw_init(l1_tdma_1_ISR,
            l1_task_tcb_read(gas_id),
            gsm_l1_wait,
            L1_WAIT_HW_SIG,
            L1_TIMEOUT_HW_SIG
            ,GL1_HW_INIT_ACTIVE_RAT
            ,gas_id);
   }

#ifdef FEATURE_GPRS_GBTA
   gl1_gbta_read_nv_gbta_support(gas_id);
#endif /* FEATURE_GPRS_GBTA */

   /* Initialize DRX manager - must occur after gl1_hw_init() */
   gl1_drx_reset(gas_id);

   /* let wcdma surround engine know the mode,  */
   /* must be done after gl1_hw_init() */
   l1_sc_wcdma_set_mode_from_init(message_ptr->gsm_only, gas_id);
#else
   gl1_hw_init(l1_tdma_1_ISR,
               l1_task_tcb_read(gas_id),
               gsm_l1_wait,
               L1_WAIT_HW_SIG,
               L1_TIMEOUT_HW_SIG
               ,GL1_HW_INIT_ACTIVE_RAT
               ,gas_id
               );


  /* Initialize DRX manager - must occur after gl1_hw_init() */
   gl1_drx_reset(gas_id);

   /* let wcdma surround engine know the mode,  */
   /* must be done after gl1_hw_init() */
   l1_sc_wcdma_set_mode_from_init(message_ptr->gsm_only, gas_id);

   /* Activate the ISR watchdog */
   l1_isr_watchdog_activate(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   /* Now tell the L1 ISR to switch states */
   l1_tskisr_blk->main_command = L1_ACTIVATE_COMMAND;

   /* Initialize the BPLMN search state when activating GSM */
   l1_tskisr_blk->current_params.L1_confirm_ID = l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* Store the X2G reselection flag for use until we reach idle mode */
   l1_tskisr_blk->x2g_resel_in_progress = message_ptr->x2g_resel_in_progress;

   /* Store the X2G CCO flag for use until we reach idle mode */
   l1_tskisr_blk->x2g_cco_in_progress = message_ptr->x2g_cco_in_progress;
#endif
   /* On the first power scan after activation, validate
      that the RF hw is working */
   l1_set_pscan_validate_results(TRUE,gas_id);

   /* Make sure MM non-DRX mode is not active. */
   l1_idle_data->mm_non_drx_mode = FALSE;

   l1_idle_data->non_updated_lai_in_nmo1 = FALSE;
   /* Make sure the GPRS Sliding Avg is initialised properly. */
   gprs_serv_cell_meas_ptr->state = SCM_INIT;

   /* Do the initialisation for the serve cell structures */
   (void)gpl1_gprs_serving_cell_meas(0,gas_id);

   /* Reset the flag which allows c value calculations
    * to run on ccch during cs idle.
    */
   gprs_serv_cell_meas_ptr->cal_c_value_51_idle = FALSE;

   /* Convey nv item information to the egprs quality measurements
    * module.
    */
   gpl1_egprs_scell_qual_meas_nv_item_config(mcfg_gcf_nv_get_status(),gas_id );

}

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
/*===========================================================================

FUNCTION  L1_handle_MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ

DESCRIPTION
  This function handles the MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ from RR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_handle_MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ( IMH_T  *msg_header )
{
   mph_resume_gsm_mode_for_bplmn_req_T *message_ptr =
    (mph_resume_gsm_mode_for_bplmn_req_T *) msg_header;
   gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_W2G_BPLMN_SRCH_RESUME);
#endif

#ifndef FEATURE_UNIFIED_SLEEP_TASK
   /* Tell sleep task to be in GSM mode */
   sleep_enter_mode( SLEEP_MODE_GSM );
#endif

   MSG_GERAN_HIGH_1_G("MPH_RESUME_GSM_MODE_FOR_BPLMN_REQ with timeout %dms", (int)(message_ptr->timer_val));

   /* Reset the message layer handlers */
   gl1_msg_init (gas_id);

   /* Preset main command in case ISR interrupts this function */
   l1_tskisr_blk->main_command = L1_NULL_COMMAND;

   /* Now initialize the hardware.  The RF has a dependency on NV */
   /* being started this is why we are initializing after receipt */
   /* of the TASK_START_SIG                                       */

   /* Request is being handled by GL1 Task. Check whether SUB has valid
      RF device or not */

   /* at start up initialise for all the supported gsm sys bands */
   grm_set_valid_acquisition_bands( l1_tskisr_blk->client_id, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if ( gl1_msg_get_multi_sim_mode() )
   {
    gl1_hw_multi_sim_init(l1_tdma_1_ISR,
                                l1_task_tcb_read( gas_id ),
                                gsm_l1_wait,
                                L1_WAIT_HW_SIG,
                                L1_TIMEOUT_HW_SIG,
                                gas_id);
   }
   else
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
   {
    gl1_hw_init(l1_tdma_1_ISR,
          l1_task_tcb_read(gas_id),
          gsm_l1_wait,
          L1_WAIT_HW_SIG,
          L1_TIMEOUT_HW_SIG
          ,GL1_HW_INIT_RX_ONLY
          ,gas_id
          );
   }

   /* Initialize DRX manager - must occur after gl1_hw_init() */
   gl1_drx_reset(gas_id);

   /* initialise the global DRX interval counter */
   l1_initialise_drx_interval(message_ptr->timer_val, gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if ( TRUE == gl1_msg_get_multi_sim_mode() )
   {
     /* Now tell the L1 ISR to switch states */
     /* Request for TRM grant */
     l1_tskisr_blk->main_command = L1_WTOG_BPLMN_TRM_REQUEST_COMMAND;
     l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_TRM_REQUEST,gas_id);
   }
   else
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */
   {
     /* This is for Single SIM mode */
     /* Now tell the L1 ISR to switch states */
     l1_tskisr_blk->main_command = L1_RESUME_COMMAND;

     /* Change sub_state to STARTING in case we receive an MPH_STOP_GSM_MODE_REQ before L1 ISR runs.
        The next L1 ISR run will set the sub_state to ACTIVE state and could happen here
        anyway (if L1 ISR interrupted this function at this point in state L1_RESUME_COMMAND) */
     l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_STARTING,gas_id);
   }

   /* Activate the ISR watchdog */
   l1_isr_watchdog_activate(gas_id);

   /* On the first power scan after activation, validate
    that the RF hw is working */
   l1_set_pscan_validate_results(TRUE,gas_id);
}

/*===========================================================================

FUNCTION  L1_handle_MPH_START_GSM_MODE_FOR_BPLMN_REQ

DESCRIPTION
  This function handles the MPH_START_GSM_MODE_FOR_BPLMN_REQ from RR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_handle_MPH_START_GSM_MODE_FOR_BPLMN_REQ( IMH_T  *msg_header )
{
   /* align pointers */
   mph_start_gsm_mode_for_bplmn_req_T *message_ptr =
     (mph_start_gsm_mode_for_bplmn_req_T *)msg_header; /*lint !e740 */
   gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

   /* initialise the global frequency list storage */
   l1_frequency_list_mem_init(gas_id);

   /* Always reset the iRAT handover Ind used for AGC. */
   l1_interrat_ho_update(FALSE, gas_id);

 #ifndef FEATURE_UNIFIED_SLEEP_TASK
   /* Tell sleep task to be in GSM mode */
   sleep_enter_mode( SLEEP_MODE_GSM );
 #endif

   /* Reset the message layer handlers */
   gl1_msg_init (gas_id);

   if(gl1_msg_get_multi_sim_sys_mode() != message_ptr->multi_sim_mode )
   {
      MSG_GERAN_HIGH_2_G("Multi-SIM mode update: new mode %d, previous mode %d",message_ptr->multi_sim_mode,gl1_msg_get_multi_sim_sys_mode());
      gl1_msg_set_multi_sim_sys_mode(message_ptr->multi_sim_mode);
   }

   /* Set the client IDs for each sub */
   L1_set_multi_sim_client_ids( gas_id );

   /* Preset main command in case ISR interrupts this function */
   l1_tskisr_blk->main_command = L1_NULL_COMMAND;

   /* Now initialize the hardware.  The RF has a dependency on NV */
   /* being started this is why we are initializing after receipt */
   /* of the TASK_START_SIG                                       */

   /* at start up initialise for all the supported gsm sys bands */
   grm_set_valid_acquisition_bands( l1_tskisr_blk->client_id, gas_id );

   gl1_hw_init(l1_tdma_1_ISR,
               l1_task_tcb_read(gas_id),
               gsm_l1_wait,
               L1_WAIT_HW_SIG,
               L1_TIMEOUT_HW_SIG
               ,GL1_HW_INIT_RX_ONLY
               ,gas_id
               );

   /* Set inhibit sleep flag*/
   gl1_drx_set_sleep_inhibit(TRUE, gas_id);

   /* Initialize DRX manager - must occur after gl1_hw_init() */
   gl1_drx_reset(gas_id);

   /* Now tell the L1 ISR to switch states */
   l1_tskisr_blk->main_command = L1_WTOG_BPLMN_COMMAND;

   /* Initialize the BPLMN search state when activating GSM */
   l1_tskisr_blk->current_params.L1_confirm_ID = l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;

   /* initialise the global DRX interval counter */
   l1_initialise_drx_interval(message_ptr->timer_val, gas_id);

   /* Activate the ISR watchdog */
   l1_isr_watchdog_activate(gas_id);

   /* On the first power scan after activation, validate
      that the RF hw is working */
   l1_set_pscan_validate_results(TRUE,gas_id);
}

#endif //FEATURE_WTOG_BACKGROUND_PLMN_SEARCH

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================

FUNCTION  L1_handle_MPH_NV_REFRESH_REQ

DESCRIPTION
  This function handles the MPH_NV_REFRESH_REQ from RR.
  This will refresh all network related EFS / NV items.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_handle_MPH_NV_REFRESH_REQ(IMH_T *msg_header)
{
  mph_nv_refresh_req_T *message_ptr = (mph_nv_refresh_req_T *)msg_header; /*lint !e740 */
  gas_id_t gas_id = check_gas_id(message_ptr->gas_id);

  MSG_GERAN_HIGH_0_G("MPH_NV_REFRESH_REQ rcvd from GRR");

  /* Re-read carrier specific EFS/NV items. */
  gl1_refresh_nv_items(gas_id);

  /* Send CNF to GRR */
  L1_send_MPH_NV_REFRESH_CNF(gas_id);
}
#endif /* FEATURE_MODEM_CONFIG_REFRESH */

/*===========================================================================

FUNCTION  l1_xmsi_use_cached_params

DESCRIPTION
  This function handles the refresh of the firmware shared
  IMSI information when valid data exists in the l1 idle data
  store for the requested G SUB.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_xmsi_use_cached_params( gas_id_t gas_id )
{
  idle_data_T *l1_idle_data_p = &l1_idle_data_store[gas_id];

  if ( !l1_idle_data_p->imsi_data.imsi_valid )
  {
    MSG_GERAN_ERROR_0_G( "GL1 stored imsi data is invalid" );

    l1_idle_data_p->imsi_data.updated = FALSE;
  }
  else
  {
    /* Set to force IMSI update to firmware during idle setup */
    l1_idle_data_p->imsi_data.updated = TRUE;
  }
}

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
void L1_resume_req( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
 #ifndef FEATURE_UNIFIED_SLEEP_TASK
   /* Tell sleep task to be in GSM mode */
   sleep_enter_mode( SLEEP_MODE_GSM );
 #endif

   /* Reset the message layer handlers */
   gl1_msg_init(gas_id);

   /* Now initialize the hardware.  The RF has a dependency on NV */
   /* being started this is why we are initializing after receipt */
   /* of the TASK_START_SIG                                       */

   /* at start up initialise for all the supported gsm sys bands */
   grm_set_valid_acquisition_bands( l1_tskisr_blk->client_id, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   if ( gl1_msg_get_multi_sim_mode() )
   {
     gl1_hw_multi_sim_init(l1_tdma_1_ISR,
            l1_task_tcb_read( gas_id ),
            gsm_l1_wait,
            L1_WAIT_HW_SIG,
            L1_TIMEOUT_HW_SIG,
            gas_id);
   }
   else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
   {
   gl1_hw_init(l1_tdma_1_ISR,
               l1_task_tcb_read(gas_id),
               gsm_l1_wait,
               L1_WAIT_HW_SIG,
               L1_TIMEOUT_HW_SIG,
               GL1_HW_INIT_ACTIVE_RAT,
               gas_id);
    }

   /* Initialize DRX manager - must occur after gl1_hw_init() */
   gl1_drx_reset(gas_id);

   /* Activate the ISR watchdog */
   l1_isr_watchdog_activate(gas_id);

   /* On the first power scan after activation, validate
      that the RF hw is working */
   l1_set_pscan_validate_results(TRUE,gas_id);

   /* Make sure MM non-DRX mode is not active. */
   l1_idle_data->mm_non_drx_mode = FALSE;
   l1_idle_data->non_updated_lai_in_nmo1 = FALSE;          
   /*
    * If valid IMSI data exists then update shared firmware data
    * as the information store may have been reset during other
    * activity handling. Needed for SBD/EPD handling to work correctly.
    */
   l1_xmsi_use_cached_params( gas_id );
}
#endif

/*===========================================================================

FUNCTION  L1_enable_ftm_mode

DESCRIPTION
   Sets L1 to FTM mode

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void L1_enable_ftm_mode( gas_id_t gas_id )
{
  l1_inact_if_data_t *l1_inact_if_data_ptr = &l1_inact_if_data[gas_id];

  l1_inact_if_data_ptr->l1_in_ftm_mode = TRUE;
}

/*===========================================================================

FUNCTION  L1_disable_ftm_mode

DESCRIPTION
   Disables FTM mode in L1

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
void L1_disable_ftm_mode( gas_id_t gas_id )
{
  l1_inact_if_data_t *l1_inact_if_data_ptr = &l1_inact_if_data[gas_id];

  l1_inact_if_data_ptr->l1_in_ftm_mode = FALSE;
}


/*===========================================================================

FUNCTION  L1_ftm_mode_enabled

DESCRIPTION
   Returns TRUE if Factory Test Mode is enabled on **any** stack

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_ftm_mode_enabled( void )
{
  int     as_idx;
  boolean ftm_active = FALSE;

  /* check all data spaces */
  for ( as_idx = GERAN_DATA_SPACE_INDEX_1; as_idx < NUM_GERAN_DATA_SPACES; as_idx++ )
  {
    if ( l1_inact_if_data[as_idx].l1_in_ftm_mode )
    {
      ftm_active = TRUE;

      break;
    }
  }

  return( ftm_active );
}
/*===========================================================================

FUNCTION  L1_ftm_enabled

DESCRIPTION
   Returns TRUE if Factory Test Mode is enabled on **any** stack

DEPENDENCIES
  None

RETURN VALUE

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_ftm_enabled( gas_id_t gas_id )
{
  l1_inact_if_data_t *l1_inact_if_data_ptr = &l1_inact_if_data[gas_id];

  return l1_inact_if_data_ptr->l1_in_ftm_mode;
}

/*===========================================================================

FUNCTION GL1_ENTER_LOW_POWER_MODE

DESCRIPTION
  Force L1 hardware into low power mode.
  This function will not have any affect unless L1 is inactive.

  Actual mechanism is to send L1_FORCE_LPM_REQ to the L1 task.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_enter_low_power_mode( void )
{
   /* Locals */
   l1_enter_lpm_req_T message_buffer, *message;
   int                message_length;

   /* Align the pointers */
   message                             = &message_buffer;

   /* Set up the message header */
   message->message_header.message_set = MS_L1_L1;
   message->message_header.message_id  = L1_ENTER_LPM_REQ;
   message_length                      = ( sizeof(l1_enter_lpm_req_T) - sizeof(IMH_T) ); /*lint !e778 expression = 0 */
   PUT_IMH_LEN( message_length, message )

   /* always use the primary GAS ID */
   message->gas_id = GERAN_ACCESS_STRATUM_ID_1;

   /* Send the message */
   (void)gs_send_message( l1_queues[GERAN_ACCESS_STRATUM_ID_1], message, TRUE );
}

/*===========================================================================

FUNCTION L1_ENTER_LPM

DESCRIPTION
  Takes the required action to put the L1 hardware into a low power state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_enter_lpm( gas_id_t gas_id )
{
   /* Activate and then deactivate GSM L1.
    * Use DEEP_SLEEP as inact reason.
    * This will put the L1 hardware in a
    * low power state.
    */

   gl1_hw_init( NULL,
                l1_task_tcb_read(gas_id),
                gsm_l1_wait,
                L1_WAIT_HW_SIG,
                L1_TIMEOUT_HW_SIG
                ,GL1_HW_INIT_INACTIVE
                ,gas_id
                );

   gl1_hw_inact( TRUE, gas_id );

#if !defined FEATURE_UNIFIED_SLEEP_TASK
   sleep_enter_mode( SLEEP_MODE_GSM_DEEP_SLEEP );
#endif

   MSG_GERAN_HIGH_0_G( "GL1 in LPM");
}

/* EOF */
