/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1   I D L E  M O D E   T A S K  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events while the L1
   task is in the IDLE mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_idle_mode
    Procedure to handle external events while the L1 task is in the
    IDLE mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_idle_if.c#2 $
$DateTime: 2019/12/30 04:21:22 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
20/01/17   akm      CR1109976 Optimization in G2X BPLMN search
17/11/17   ng        CR2145414 De-Init X RAT on receiving Early camping request after Ded->Idle transition 
22/01/18   br        CR2174586 GL1 to handle MPH_DSC_THRESHOLD_IND to get epd pref correctly
09/02/16   mk        CR2002074 Add LB_MS_TXPWR_MAX_CCH for Tx power level initialization
26/09/16   ksb       CR059213 Send a pre-cell time tage in foreground powersacan
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
03/01/16   mn        CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
18/5/16    sm        CR1017901 Added GL1 idle mode QSH events 
17/05/16   dg        CR1014646 Don't turn MCPM ON for G2X neighbors on receiving list search request if G2X is already active
26/04/16   hd        CR998694 Send MPH_INTERRAT_LIST_SRCH_TIME_IND with gap to next CCCH as soon as the REQ is also received
03/01/16   hd        CR982929 Do a rude wakeup when empty SI mask is received while asleep.
22/01/16   db        CR957526 Fix for TRM reservation without usage for BCCH block reception
05/11/15   js        CR935064 Check for reservation for BCCH and set drx next tick
04/11/15   ap        CR934830 GL1 incorrectly responds with L1_DECODE_BCCH_LIST_CNF during PSCAN suspension
17/10/15   js        CR924016 Set next tick required when receive SYS info, when GL1 is awake
29/09/15   dg        CR914960 Don't turn MCPM off for G2X neighbor on receiving list search request if G2X is already active
25/09/15   mko       CR912000 Don't do rude wakeups when SI mask is zero in SYS_INFO_REQ 
14/09/15   am        CR907561 Ensure GL1 does not wake-up when BCCH mask is cleared
09/09/15   cjl       CR904792 Reset SIB procedure priority when an empty BCCH mask is received
07/08/15   am        CR899527 Ensure GL1 wakeup and reserve a high priority task
28/08/15   cja       CR892470 G2X to cope with overlapping init/deinit/init 
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
21/07/15   cjl       CR876903 Use system procedure types for TRM subreasons 
17/07/15   smd       CR874148 Clear variables for GTA_PS_RACH before RACH at single sim mode.
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
26/05/15   am        CR840857 Ensure GL1 is awake to schedule SYS INFO REQ
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
08/04/15   pjr       CR818535 FR25951 Changes to support Single Voice Session 
31/03/15   cja       CR800650 Optimise irat state machine
30/03/15   ap        CR811753 FR22272 - TRM Unification - GL1 changes to support 
                                        SysInfo priority inversion during early camping
05/12/14   sp        CR767083: Override paging multiframe to 2, for PDT tests
18/02/14   br        CR796260 Handling the race condition between  RACH req and IDLE update request 
11/02/15   js        CR775188 GL1 changes to receive EPD preference
29/12/14   am        CR774723 Avoid unnecessary wake-up on SYS_INFO_REQ
25/11/14   br        CR760802 Clear TRM reservation for BCCH decode if upper layer is cleared SI mask.
10/12/14   dg        CR765859 Do not go to sleep if MCPM is not updated for G2X MEAS STOP activity
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
03/12/14   npt       CR759467 Fix compiler warnings
03/12/14   cgc       CR715394 G2L code Idle/Dedicated rework FEATURE_THOR_G2L_REWORK
03/07/13   jk        CR499895 Trigger force/rude wakeup before sending neighbor update commands to MCPM
30/10/14   cs        CR701894 Enhanced Page Detection Support
28/10/14   cgc       CR745548 GL1 make gl1_drx_require_and_wait_for_next_tick() conditional on awake
07/10/14   pa        CR733749:Handling of MPH_DSC_THRESHOLD_IND in the RACH state. 
18/09/14   am        CR725978 Enable rach_ready_to_send for GPLT
02/09/14   ap        CR717210 Single Sim GL1 is stuck in RACH mode
20/06/14   jj        CR656872 fix critical gl1 kw error  
21/08/14   dv        CR711417 Ensure DR mode is enabled when camping on cell 
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
19/06/14   sp        CR681217 Turn Tx On after Idle abort when processing RACH Request even for Single Sim mode.
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
03/04/14   nm        CR642857 Stop SCE on getting MPH_IMMED_ASSIGN_REQ in idle mode
10/03/14   am        CR624678 TX-ON should happen in full frame
24/01/14   cah       CR598552: enable sleep for DSDS GSTMR+VSTMR
18/11/13   pjr       CR563429 Fixed vocoder handover on sub1 while sub 2 was in a call
23/10/13   pa        CR565614:Copied 128-bit ciphering key to internal data structure.
24/07/13   ss        CR515896  Set wcdma_list_update_inprogress to TRUE in msg_handler routine 
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
23/05/13   pg        CR460555: Enable 3GPP Rel10 RACH Power Reduciton feature 
07/16/13   zc        CR470197 Replace memcpy with memscpy
24/07/13   jk        CR464982:Changes to support GEA5_4 ciphering
26/06/13  cs         Major Triton TSTS Syncup
02/02/13   ss        CR446305. Ensure GSM L1 does not go to sleep while processing the
                     WCDMA UPDATE LIST REQ in idle mode.
10/04/13   pg        CR469228 Avoid schedule two tx band events
09/04/13   pjr       CR470217 Ported next_params.L1Data.pDedi_data asignments from DIME
06/04/13   sv        CR462075: Corrected the logic when ignoring GRR commands in GTW PLMN cases.
14/03/13   cja       CR458005 Turn on W clock for G2W BPLMN
10/02/13   ip        CR448394
02/02/13   og        CR446305. Ensure GSM L1 does not go to sleep while processing the
                     WCDMA UPDATE LIST REQ in idle mode.
20/02/13   cja       CR 455032 Changes for build without TD feature
13/12/12   xz        CR430244 : Variant of CR396846.
09/10/12   cgc       CR393032 add l1_idle_if_gps_notify(), removed early camping notify
06/12/12   cja       CR383165 Remove compiler warnings
28/11/12   ws        CR425531 - Fix compile error for LTE featured off in CR 396846
29/10/12   ss        CR396846 : Process Read sys info request only if next BCCH read does not coincides with active G2X measurement 
                     scheduled in frame manager
17/08/12   ip        CR384421 Disable GSM sleep during CCO activity
26/07/12   jj        CR376793 L1 receives MPH_DECODE_BCCH_LIST_REQ with invalid ARFCN in RR message
25/06/12   og        CR372287. RACH failure due to idle mode to access race condition.
08/06/12   pa        GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
25/05/12   pg        Add support for RF Task08/06/12   pa        CR367120: Send MPH_READ_SYS_INFO_FAILURE_IND to RR during G2T idle meas.
28/03/12   cja       CR316437 Inhibit sleep on X2G RAT change until it is complete
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
08/02/12   ab        CR: 181344 - 3GPP Spec Update: Receive filter reset upon change of frequency
06/09/11   cja       CR304637. Only wait for frame tick if request leads to FW enable/disable
31/08/11   cja       CR304401. Wait for frame tick so FW not deactivated when suspended.
27/08/11   og        Completing the fix for CR303398.
26/08/11   og        CR303398. Run the require next tick before l1_sc_stop( ) and
                     deactivate.
19/07/11   ip       CR293147 Int Lock the change in state of tx status.
02/06/11   og        CR290078. Ensure stopping SCE and deactivating are done atomically.
03/05/11   cja       Nikel - only send G2L init when L neighbours
29/03/11   og        CR280195. Ensure LTE L1 G2L is deinitialised when suspending.
28/01/11   nf        CR271199 - used correct cast for MPH_IDLE_UPDATE_REQ
27/01/11   dmb       The following RR<->L1 messages are removed (not used):
                     MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
17/12/10   og        Changed G2L INIT and DEINIT.
19/10/10   ip        CR257869 Complete mode switch in ISR context
22/04/09   ws        CR 235527 - Don't process ISR_SCAN_CNF if L1_frequency_list ptr
                     is NULL
08/02/10   cja       XO Update
16/09/09   ps        Resolve Klocwork errors
16/06/09   ap        Fix Lint/compile warnings
09/06/09   nf        Added FEATURE_GSM_PCH_SINGLE_BURST_DEC
29/05/09   ps        Zero Initialise memory relocation to Heap for selected data structure
30/03/09   ws        Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                     to replace FEATURE_GSM_GPRS_MUSTANG
30/01/09   nf        CR168086 - No Sleep while W-BPLMN active
01/10/08   cs        CR 159782 Fix for GAN Inter Cell HO failures, need to add synch code
28/07/08   hv        Added KxMutex support
25/07/08   ap        Added FEATURE_GAN: Changes to support GAN->GSM HO.
20/03/08   agv       Fixed the LCU PMIC Band setup for a Channel Change.
19/02/08   og        Handle the case of power scan request containing a zero list.
24/01/08   dv        CR124527:Added L1 part of FEATURE_GPRS_FAST_RACH
21/11/07   og        Ensure L1 drops back to IDLE when the single block start
                     time elapses. Resolves CR115529.
05/07/07   cs        CR 120089 Reset the cipher flag in IMMED_ASSIGN for TC 26.8.1.2.3.2
02/03/07   cs        Add support for new CGPS timetag
06/12/06   ws        CR 107482 seed Rand based on frame number when starting early camping.
17/08/06   og        Basic featured-off EDTM code drop.
31/05/06   og        Merge CHANGE 331391 from gsm l1 saber 4x branch. Resolves CR 92215.
01/31/06   ip        CR86620 added RACH intialisation if RACH type chnages
12/14/05   sv        Support the changed prototype for MPH_DECODE_BCCH_LIST_CNF.
11/17/05   gw        Add calls to gl1_drx_require_next_tick() in various places
                     to improve L1 response time.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
10/06/05   sv        Backout previous change.
10/06/05   og        Merge SCE fix of CR78327 ( CHANGE 247820 ) from raven branch.
08/17/05   gfr       Move NPL1 RACH function calls to ISR context
06/13/05   yh        Implementing qsort.
02/06/05   og        Removal of header-header includes.
20/04/05   og        Addition of filtered bep params to the msg metrics log packet.
03/11/05   sv        Mainlined FEATURE_GSM_CB
02/11/04   bm        Support for Background WCDMA PLMN Scan
01/26/05   sv        Add support for Frame Manager.
10/01/04   bm        Enhanced Parallel BCCH decode support
09/12/04   og        Fix to tx power control by tracking c value on ccch.
08/10/04   ws        EGPRS - Fixed RACH data contents bug
07/29/04   ws        Changed EGPRS feature switch to FEATURE_GSM_EGPRS
07/21/04   bk        Merged in changes to RACH to support EGPRS
06/12/04   gfr       Control slew rate limiting in PL1.
07/06/04   gfr       Fix ARFCN check for initializing CBCH.
06/11/04   dp        Changes to L1_setup_RESELECTION to use BSIC and not BCC.
                     Also set pAgc_data to point to AGC struct of cell we're
                     reselecting to.
06/04/04   sv        Assign appropriate enum value to "L1_confirm_ID".
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
05/24/04   gw        Never tell SCE to do BCCH decodes autonomously.
04/05/04   dp        Fix to handle a null decode bcch list and just return
                     a decode bcch list cnf to RR
04/02/04   gw        Removed some lint errors.
03/26/04   gfr       Time tagging support.
03/05/04   gfr       Compile fixes for background HPLMN searching.
02/25/04   gw        Lint clean up.
02/19/04   gfr       Compile fixes for no background HPLMN searching.
01/13/04   gw        Updated Cell Broadcast code to support Quadband.
01/09/04   gw        Merged in support for new SI reading API from branch.
11/05/03   gfr       Support for quad-band.
10/09/03   gsc       Added support for GSM Cell Broadcast under FEATURE_GSM_CB
09/09/03   kf        Set up paging parameters for reorg in L1_setup_EARLY_CAMPING.
07/11/03   gw        Sped up response to MPH_RESELECTION_REQ.
07/08/03   gw        GRR/L1 SCE cleanup. Added calls to l1_sc_set_bcch_autonomy() to support
                     GRR scheduling ncell BCCH decodes.
06/19/03   gw        Added l1_sc_stop() to MPH_DECODE_BCCH_LIST_REQ.
06/10/03   sw        Added support for SI13 on Extended BCCH.
06/06/03   tb        Support for dedicated mode gtow searching.
04/29/03   dp        Changes to preserve Ncell BSIC across transitions
05/02/03   gw        Merged in power transfer changes from branch.
04/28/03   tb        Support for MPH_UE_MODE_CHANGE_REQ from RR.
04/04/03   gw        Moved some calls to l1_sc_stop() to support transferring
                     ncell info between modes.
04/02/03   gw        Changes to reflect modifications in monscan module.
02/25/03   gw        Added call to gl1_drx_require_TX() when processing
                     PH_RANDOM_ACCESS_REQ.  This causes the Tx chain to turn on
                     right away rather than waiting until we leave Idle Mode.
02/21/03   gw        Added call to L1_setup_DEACTIVATE() to process
                     MPH_STOP_GSM_MODE_REQ.  This function contains support for
                     deep sleep.
01/10/03   gw        Changed some SCE function calls to reflect renaming of some
                     BA list functions done as part of SCE clean up.
01/07/03   ws        Reset non drx timer parameter if EARLY CAMPING is required
12/05/02   gw        Added support for decoding SI 2quater on extended BCCH.
12/02/02   ws        Removed MSG_ERROR from GRR message handler for GPRS
11/21/02   mk        Deleted unsupported code under FEATURE_GSM_ALTERNATE_PLMN_SEARCH.
11/18/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/15/02   dp        Added support for multiband reporting
11/14/02   ws/gw     Added gw code to support non drx timer
11/03/02   tb        Changes to support INTERRAT_GTOW
10/08/02   kf        Added PL1 GPRS measurement code hooks under switch
                     FEATURE_GPRS_MEAS.
10/07/02   dp        Support for fully parallel decodes
10/08/02   ATM       FEATURE_MULTIMODE_GSM is mainlined - removed flag per jchao
10/02/02   dp        Fix to stop SCE when we receive MPH_STOP_CAMPING_REQ and
                     issue a rude wakeup.  Handle MPH_DECODE_BCCH_LIST_REQ in
                     idle mode
08/29/02   JC        Modified usage of gl1_msg_cfg_ul_rach() and gl1_msg_tx_rach
                     to support multi-band operation.
08/28/02   mk/dp     Change to ignore a change page mode req to EXTENDED_PAGING
                     since its handled internally by L1
08/22/02   dp        Fix to default to a non-combined structure if we receive
                     MPH_EARLY_CAMPING_REQ with paging_mode=PAGING_REORG
08/13/02   dp        Support to handle STOP_EARLY_CAMPING_REQ
08/12/02   gw        Changed RR-L1 i/f for pscan as part of cell selection /
                     re-selection optimizations.
06/24/02   dp        Changed L1_setup_START_IDLE to default to NORMAL_PAGING
                     as part of the cell selection/reselection optimizations.
                     L1 also doesn't automatically switch to PAGING_REORG when
                     it receives MPH_IDLE_UPDATE_REQ from RR
05/30/02   pjr/mk    Added PL1 GPRS code hooks under switch FEATURE_GSM_GPRS_L1.
05/20/02   mk        Unified StartIdle and Reselection PL1 I/f.
05/17/02   mk        Unified PowerScan and Acquisition PL1 I/f.
05/15/02   mk        Added INTLOCK()/INTFREE() around L1_FIND_BCCH_COMMAND.
05/10/02   JC        Added support for new gl1_cfg_power_measure API.
05/10/02   tb        Added FEATURE_MULTIMODE_GSM support.
04/29/02   gw        Added error messages to indicate when messages to L1 are
                     ignored.
04/17/02   JC        For MPH_SELECT_SPECIFIC_BCCH_REQ primitive command an
                     Acq only instead of (powerscan+ Acq).
04/16/02   mk        Fixed bug, added call to l1_sc_stop() during cell
                     reselection.
03/27/02   JC        Modifications due to new power measure message layer
                     API changes.
03/22/02   gw        Added fix to stop ISR sleeping when it should be switching
                     to Random Access Mode. Only works when sleep controller is
                     not being used.
03/20/02   dp        TXLEV to power index changes
03/06/02   dp        Support to frequency hop access bursts
03/05/02   JC        Removed MID_TIMER_EXPIRY support, since L1 doesn't use
                     gs_timers anymore.
02/12/02   thh       Remove HW/frame delay from ra_delay.  This reponsibility
                     belongs in the lower layer(s).
                     Updated copyright date.
02/07/02   gw        Added code to stop SCE when leaving Idle Mode.
02/05/02   JC        Added support for AGC.
01/24/02   ATM       Adding Calls to log paging_data.
12/18/01   dp        Set abort callback to NULL for RACH callback
10/16/01   gw        Mainlined feature OLD_SCE.
09/28/01   jc        Add support for RACH using message layer code.
09/25/01   mk        Non-supported SCE code featurized using OLD_SCE.
08/31/01   JC        Removed obsoleted compile switches and added std QCT hdrs
07/26/01   JC        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
/* Alphabetical Order */
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1_drx.h"
#include "l1_ds.h"
#include "l1_ds_v.h"
#include "l1_ds_v_g.h"
#include "l1_log.h"                   /* For logging */
#include "l1_sc_int.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "ms.h"
#include "ms_timer.h"
#include "sys_stru.h"
#include "gl1_msg_g.h"
#include "gl1_hw_sleep_ctl.h"
#include "msg.h"
#include "l1_smscb.h"
#include "cbl1if.h"
#include "l1_sc.h"
#include <stringl/stringl.h>

#include "gpl1_gprs_task.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "l1_isr.h"
#include "mcfg_nv_api.h"

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"

#ifdef  FEATURE_GSM_TO_LTE
#include "l1i.h"
#endif

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

typedef struct
{

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
boolean background_wcdma_plmn_scan_active ;
#endif /* FEATURE_GTOW_BACKGROUND_PLMN_SEARCH */
  boolean rach_ready_to_send;
}l1_idle_if_data_t;

extern void L1_suspend_background_HPLMN_search( gas_id_t gas_id );

extern boolean re_calculate_initial_rach[NUM_GERAN_DATA_SPACES];
extern dedicated_data_T l1_dedicated_data[];


static l1_idle_if_data_t  l1_idle_if_data[NUM_GERAN_DATA_SPACES] = {INITIAL_VALUE_S(0)};


void l1_set_rach_ready_to_send( boolean ready_to_send, gas_id_t gas_id )
{
  l1_idle_if_data[gas_id].rach_ready_to_send = ready_to_send;
}

boolean l1_get_background_wcdma_plmn_scan_active( gas_id_t gas_id )
{
  l1_idle_if_data_t *l1_idle_if_data_ptr = &l1_idle_if_data[gas_id];

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
  return l1_idle_if_data_ptr->background_wcdma_plmn_scan_active;
#else
  return FALSE;
#endif /* FEATURE_GTOW_BACKGROUND_PLMN_SEARCH */
}

void l1_set_background_wcdma_plmn_scan_active(boolean background_wcdma_plmn_scan_active, gas_id_t gas_id)
{
  l1_idle_if_data_t *l1_idle_if_data_ptr = &l1_idle_if_data[gas_id];
#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
  l1_idle_if_data_ptr->background_wcdma_plmn_scan_active = background_wcdma_plmn_scan_active;
#endif /* FEATURE_GTOW_BACKGROUND_PLMN_SEARCH */

  return;
}



/*===========================================================================

FUNCTION  L1_IS_READ_SYS_INFO_REQ_POSSIBLE

DESCRIPTION
  This function checks if the next read sys info activity coincides with the current G2W or G2L activity in progress

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  Ideally , a BCCH read must be given higher priority but when the G2X measurement are already scheduled, abort mechanism may not complete before the BCCH read. Hence this approach is used. 

===========================================================================*/

boolean l1_is_READ_SYS_INFO_REQ_possible(rr_l1_si_type_T  si_mask,gas_id_t gas_id)
{
  
   uint32                    BCCH_mask;
   uint32                    ext_BCCH_mask;
   boolean                   return_value;


   uint32 gap_to_next_activity = 0;
   uint32 gap_to_next_bcch     = 51;  /* The BCCH read is not in the current and next TC we surely know that the bcch_read is atleast 51 frames away*/
   uint32 gap_to_next_ext_bcch = 51;
   uint32 multiframe_div_51;
   uint32 local_FN;
   
   int32 num_frames_for_g2w;
   int32 num_frames_for_g2l;
   int32 num_frames_for_g2t;


   GL1_ISR_SAVE_LOCK(gas_id);

   local_FN          = gl1_get_FN(gas_id);
   multiframe_div_51 = local_FN / 51;

   /* Check if the G2X acivity is on*/
   if (l1_sc_check_irat_active(gas_id))
   {
      
      BCCH_mask      = gl1_compute_BCCH_mask( si_mask );
      ext_BCCH_mask  = gl1_compute_ext_BCCH_mask( si_mask );

      
      //MSG_MED(" MPH_READ_SYS_INFO_REQ si_mask 0x%x  BCCH_mask=0x%x, ext_BCCH_mask=0x%x", si_mask ,BCCH_mask, ext_BCCH_mask );



       /* Work out gap to next normal BCCH block to receive
          * if any.
          */
         if ( BCCH_mask != SERV_BCCH_MASK_NONE )
         {
            
            uint32 TC;
            uint32 TC_plus_1;
            uint32 gap_to_next_block_0;
            
            uint32 BCCH_unsafe_mask = 0; 
            uint32 BCCH_mask_common = 0;

            /* Work out gap to next block received by adding
             * gap to next block 0 to 51 * number of mframes
             * until next TC value.
             */
            
            TC        = multiframe_div_51 % 8;
            TC_plus_1 = (TC+1)%8;

            /* Extra checks are only required if the bcch read is in the current TC or the next TC*/
            BCCH_unsafe_mask = ((1uL<<TC) | (1uL<<TC_plus_1)); 

            /*store information wheather the bcch read is in the current or the next TC*/
            BCCH_mask_common = BCCH_unsafe_mask & BCCH_mask;

            /* The BCCH read is not in the current and next TC we surely know that the bcch_read is atleast 51 frames away*/
            if ( BCCH_mask_common == 0 )
            {
               gap_to_next_bcch = 51; 
            }
            else
            {
               /* The BCCH read is in current TC*/
               if ( BCCH_mask_common  & 1uL<<TC )
               {
                  /* if The BCCH read is in current TC we can use that only if we are  block 0 of the multiframe*/
                  if (frame_counters[gas_id].FNmod51 == 0)
                  {
                     gap_to_next_bcch = 0;
            }
                  else   /*we know that BCCH is not the current TC*/
            {
                     gap_to_next_bcch = 51;
                  }
               }
               /* The BCCH read is in current TC + 1*/
               if ( BCCH_mask_common  & ( 1uL<<TC_plus_1 ) )
               {
                   /* we know the BCCH read is in the block 0 of the next multiframe*/
                  gap_to_next_block_0 = 51 - frame_counters[gas_id].FNmod51;
                  gap_to_next_bcch = MIN( gap_to_next_bcch ,gap_to_next_block_0 );
                  
               }
               }

            }


         /* Work out gap to next extended BCCH block to receive
          * if any.
          */
         if ( ext_BCCH_mask != SERV_EXT_BCCH_MASK_NONE)
         {
            
            uint32 TC;
            uint32 TC_plus_1;
            uint32 gap_to_next_block_1;

            uint32 BCCH_unsafe_mask = 0; 
            uint32 BCCH_mask_common = 0;

            /* Work out gap to next block received by adding
             * gap to next block 0 to 51 * number of mframes
             * until next TC value.
             */
            
            TC            = multiframe_div_51 % 8;
            TC_plus_1 = (TC+1)%8;

            /* Extra checks are only required if the bcch read is in the current TC or the next TC*/
            BCCH_unsafe_mask = ((1uL<<TC) | (1uL<<TC_plus_1)); 

            /*store information wheather the bcch read is in the current or the next TC*/
            BCCH_mask_common = BCCH_unsafe_mask & BCCH_mask;

            /* The BCCH read is not in the current and next TC we surely know that the bcch_read is atleast 51 frames away*/
            if ( BCCH_mask_common == 0 )
            {
               gap_to_next_bcch = 51; 
            }
            else
            {
               /* The BCCH read is in current TC*/
               if ( BCCH_mask_common  & 1uL<<TC )
               {
                  /* if The BCCH read is in current TC, we can use that only if we have not passed the 4th block of the multiframe*/
                  if ( frame_counters[gas_id].FNmod51 <= 4 )
                  {
                     gap_to_next_ext_bcch = 4 - frame_counters[gas_id].FNmod51;   /* BCCH read is in block 4 of the current TC*/
            }
                  else   /*we know that BCCH is not the current TC*/
            {
                     gap_to_next_ext_bcch = 51;
                  }
               }
               /* The BCCH read is in current TC + 1*/
               if ( BCCH_mask_common  & ( 1uL<<TC_plus_1 ) )
               {
                   /* we know the BCCH read is in the block 4 of the next multiframe*/
                  gap_to_next_block_1 = 55 - frame_counters[gas_id].FNmod51;
                  gap_to_next_ext_bcch = MIN( gap_to_next_ext_bcch ,gap_to_next_block_1 );
                  
               }
            }

         }

         /* Take the minimum of the gaps */
         if (gap_to_next_bcch <= gap_to_next_ext_bcch)
         {
            gap_to_next_activity = gap_to_next_bcch;
         }
         else
         {
            gap_to_next_activity = gap_to_next_ext_bcch;
         }
        
         num_frames_for_g2w =  ( (int32) l1_fm_get_required_frames_to_complete(FM_PRI_WCDMA_IDSEARCH,gas_id) );
#ifdef FEATURE_GSM_TO_LTE
         num_frames_for_g2l =  ( (int32) l1_fm_get_required_frames_to_complete(FM_PRI_LTE_IDSEARCH,gas_id) );
#else
         num_frames_for_g2l = 0;
#endif
#ifdef FEATURE_GSM_TDS
         num_frames_for_g2t =  ( (int32) l1_fm_get_required_frames_to_complete(FM_PRI_TDS_ACQ,gas_id) );
#else
         num_frames_for_g2t = 0;
#endif
         if ( gap_to_next_activity <   num_frames_for_g2w  )
         {
            MSG_HIGH(" MPH_READ_SYS_INFO_REQ during G2W search: si_mask 0x%x  BCCH_mask=0x%x, ext_BCCH_mask=0x%x", si_mask ,BCCH_mask, ext_BCCH_mask );
            return_value = FALSE;
         }
         else if (gap_to_next_activity < num_frames_for_g2l )
         {
            MSG_HIGH(" MPH_READ_SYS_INFO_REQ during G2L search: si_mask 0x%x  BCCH_mask=0x%x, ext_BCCH_mask=0x%x", si_mask ,BCCH_mask, ext_BCCH_mask );
            return_value = FALSE;
         }
         else if(gap_to_next_activity < num_frames_for_g2t)
        {
           MSG_HIGH(" MPH_READ_SYS_INFO_REQ during G2T search: si_mask 0x%x  BCCH_mask=0x%x, ext_BCCH_mask=0x%x", si_mask ,BCCH_mask, ext_BCCH_mask );
           return_value = FALSE;
         }
         else
         {
            return_value = TRUE;
         }

   }
   else
   {
     //MSG_HIGH(" MPH_READ_SYS_INFO_REQ No G2X activity on: si_mask 0x%x  BCCH_mask=0x%x, ext_BCCH_mask=0x%x", si_mask ,BCCH_mask, ext_BCCH_mask );
     return_value = TRUE;
   }

   GL1_ISR_SAVE_UNLOCK(gas_id);
   return return_value;

}

/*===========================================================================

FUNCTION  L1_HANDLE_IDLE_MODE

DESCRIPTION
  This function handles the L1 Task in idle mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_idle_mode(IMH_T *msg_header, gas_id_t arg_gas_id)

{
  /* Locals */
  inter_task_BCCH_allocation_T  ba_buffer;
  gas_id_t gas_id;
 
  if (msg_header->message_set == MS_RR_L1)
  {
#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
    if (l1_get_background_wcdma_plmn_scan_active(arg_gas_id))
    {  
      /* If background WCDMA PLMN Search is active, then do not
       * process any other message from RR except the three below
       */
      if ((msg_header->message_id != (byte)MPH_INTERRAT_LIST_SRCH_TIME_REQ) &&
          (msg_header->message_id != (byte)MPH_SUSPEND_GSM_MODE_REQ) &&
          (msg_header->message_id != (byte)MPH_STOP_GSM_MODE_REQ)
         )
    {
        gas_id = arg_gas_id;  /* MSG_GERAN_ERROR_1_G needs gas_id */ 
        MSG_GERAN_ERROR_1_G("unexpected RR message=%d when doing WCDMA PLMN scan", msg_header->message_id);
        return;
      }
    }
#endif

    /* function L1_find_bcch_mode or  puts the L1 ISR into idle mode.
    This function interprets all message in this mode.
    when the ISR sends a ISR_SCAN_CNF message to the L1 task. */

    switch (msg_header->message_id )
    {
        /* Enable Sleep indication */
      case MPH_ENABLE_SLEEP_IND:
        {
          L1_handle_ENABLE_SLEEP_IND ( msg_header );
          break;
        }

        /*******************************************************
        *                                                      *
        *                MPH_POWER_SCAN_REQ                    *
        *                                                      *
        ********************************************************/
      case MPH_POWER_SCAN_REQ:
        {
          mph_power_scan_req_T *message_ptr;
          idle_data_T               *l1_idle_data;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

          message_ptr = (mph_power_scan_req_T *)msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          l1_idle_data = &l1_idle_data_store[gas_id];
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];

          if( FALSE == L1_setup_PSCAN( msg_header ) )
          {
            if ( message_ptr->search_mode == RR_SEARCH_BACKGROUND )
            {
              MSG_GERAN_MED_0_G("Power scan initiated in background");
              if ( NO_HPLMN_SEARCH == l1_idle_data->background_HPLMN_search_activity &&
                   l1_tskisr_blk->current_params.L1_confirm_ID != L1_NULL_CNF)
              {
                MSG_ERROR("Unexpected L1_confirm_ID set to %d! Resetting", l1_tskisr_blk->current_params.L1_confirm_ID, 0, 0);
                 /* reset to a NULL_CNF so that we dont send the BCCH_LIST_CNF twice */
                l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
                l1_tskisr_blk->next_params.L1_confirm_ID = L1_NULL_CNF;
              }
              /* Set up for background power scan
              * l1_isr will kick off background pscan based on
              * l1_idle_data.background_HPLMN_search_activity
              */
              l1_idle_data->background_HPLMN_search_activity = POWER_SCAN;
              l1_tskisr_blk->pscan_state = L1_SCAN_END;			  
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
              gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_BPLMN_SRCH_START);
#endif
            }
            else
            {
              MSG_GERAN_MED_0_G("Power scan initiated in foreground");

              l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
              /* Foreground power scan  - abort idle mode and kick off pscan */

              /* Tell the SCE to stop its idle mode procedures */
              l1_sc_stop(gas_id);
			  
			  /* Tell CGPS we are leaving this cell */
  			    gl1_timetag_cgps(l1_tskisr_blk->l1_state, CGPS_TIMETAG_PRE_CELL_CHANGE,gas_id);

    			/* Wait for real next frame tick to send pending TT update from DPC task */
    			gl1_wait_for_dpc( gas_id );

              GL1_ISR_LOCK(gas_id);
              /* Command the  the ISR to start measuring */
              L1_send_ISRTIM_SCAN_REQ(L1_FIND_BCCH_COMMAND,gas_id);
              GL1_ISR_UNLOCK(gas_id);
            }

            /* Rude wakeup from SLEEP, so we respond faster */
            gl1_drx_require_next_tick(gas_id);

          }
          else
          {
            /* Something is not right about the power scan request.
            * Send back a confirmation to avoid any upper layer
            * timeouts.
            */
            L1_send_MPH_POWER_SCAN_CNF( FALSE,gas_id ); /* suspending boolean */
          }

          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_CHANGE_PAGE_MODE_REQ              *
        *                                                      *
        ********************************************************/
      case MPH_CHANGE_PAGE_MODE_REQ:

        {
          byte paging_mode;
          mph_change_page_mode_req_T *message_ptr;
          idle_data_T *l1_idle_data;

          message_ptr = ( mph_change_page_mode_req_T *)msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);
          l1_idle_data = &l1_idle_data_store[gas_id];
          paging_mode = message_ptr->paging_mode & 3;

          GL1_ISR_LOCK(gas_id);

          /* L1 handles extended paging internally! */
          if (paging_mode != EXTENDED_PAGING)
          {
            l1_idle_data->paging_data.paging_mode = paging_mode;
          }

          switch ( paging_mode )
          {
          case NORMAL_PAGING:
            l1_idle_data->paging_data.all_paging_blocks = FALSE;
            break;

            /* L1 handles extended paging internally! */
          case EXTENDED_PAGING:
            break;

          case PAGING_REORGANIZATION:
            l1_idle_data->paging_data.all_paging_blocks = TRUE;
            break;

          case SAME_AS_BEFORE:
            break;

          default:
            MSG_GERAN_ERROR_1_G("Invalid paging_mode %d.", paging_mode);
            break;
          }

          GL1_ISR_UNLOCK(gas_id);

          (void)l1_log_paging_data( gas_id);

          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_IDLE_UPDATE_REQ                   *
        *                                                      *
        ********************************************************/
      case MPH_IDLE_UPDATE_REQ:

        {
          mph_idle_update_req_T *message_ptr;

          idle_data_T               *l1_idle_data;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
          gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;

          message_ptr = ( mph_idle_update_req_T *) msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          /* Enter idle mode, so allow sleep */
          gl1_drx_set_sleep_inhibit(FALSE, gas_id);

          l1_idle_data = &l1_idle_data_store[gas_id];
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[gas_id];
		  if(l1_tskisr_blk->main_command == L1_ACCESS_COMMAND)
	      {
            MSG_GERAN_MED_0_G("Main command is L1_ACCESS_COMMAND, so not hanlding MPH_IDLE_UPDATE_REQ");
		  }
	   	  else
		  {

          /* Load up the data  from the message */
          GL1_ISR_SAVE_LOCK(gas_id);

          l1_idle_data->campedon_cell_data = message_ptr->layer1_sys_info;
          l1_idle_data->campedon_cell_data.BS_PA_MFRMS = l1_override_bs_pa_mfrms(l1_idle_data->campedon_cell_data.BS_PA_MFRMS, gas_id);
          l1_idle_data->imsi_data.imsi_valid = message_ptr->imsi_valid;
          l1_idle_data->imsi_data.imsi_mod_1000 = message_ptr->imsi_mod_1000;

          L1_calculate_paging_data(gas_id);

          if ( l1_smscb_is_active(gas_id) )
          {
            ARFCN_T cbch_arfcn = l1_smscb_get_arfcn(gas_id);
            ARFCN_T serving_cell_arfcn = gl1_get_serving_cell_arfcn(gas_id);
            if (!ARFCNS_EQUAL(cbch_arfcn, serving_cell_arfcn) ||
              l1_smscb_get_bsic(gas_id) != gl1_get_serving_cell_BSIC(gas_id))
            {
              l1_smscb_init(gas_id);

            }
          }

          l1_smscb_set_cb_supported (
            message_ptr->layer1_sys_info.cbch_channel_information_valid,gas_id
            );

          l1_smscb_set_chan_type(
            message_ptr->layer1_sys_info.cbch_channel_information.channel_type,gas_id
            );

          l1_smscb_set_arfcn( gl1_get_serving_cell_arfcn(gas_id),gas_id);

          l1_smscb_set_bsic( gl1_get_serving_cell_BSIC(gas_id),gas_id );

          l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

          GL1_ISR_SAVE_UNLOCK(gas_id);

          /* AGV REM Make sure the GPRS Sliding Avg is initialised properly. */
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

          l1_sc_set_bcch_autonomy( FALSE,gas_id );

          /*copy BA list to SC engine*/
          L1_remove_oob_ba(&ba_buffer,&message_ptr->layer1_sys_info.BA_list);
          l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);

          /* Set multiband reporting parameter */
          l1_sc_balist_set_multiband_reporting( message_ptr->layer1_sys_info.multiband_reporting,gas_id );

          /*tell surrounding cell engine gap between paging blocks*/
          l1_sc_set_paging_multiframes(l1_idle_data->campedon_cell_data.BS_PA_MFRMS,gas_id);

          if ( gl1_msg_is_sbd_enabled( gas_id ) )
          {
            l1_xmsi_copy_params(&message_ptr->xmsi,gas_id);
          }

          l1_idle_if_gps_notify(gas_id);

           (void)l1_log_paging_data(gas_id);
		  }
          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_EARLY_CAMPING_REQ                 *
        *                                                      *
        ********************************************************/
      case MPH_EARLY_CAMPING_REQ:
        {
          mph_early_camping_req_T* message_ptr = ( mph_early_camping_req_T* )msg_header;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

          gas_id = check_gas_id(message_ptr->gas_id);
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          L1_setup_EARLY_CAMPING ( msg_header );

          /* Change the ISR mode */
          l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
          break;
        }

        /********************************************************
        *                                                      *
        *            MPH_DECODE_BCCH_LIST_REQ
        *                                                      *
        ********************************************************/
      case MPH_DECODE_BCCH_LIST_REQ:
        {
          mph_decode_bcch_list_req_T  *message_ptr;
          idle_data_T               *l1_idle_data;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
          BCCH_List_T *l1_BCCH_List_ptr;

          /* Align pointers */
          message_ptr = ( mph_decode_bcch_list_req_T *)msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);

          l1_idle_data = &l1_idle_data_store[gas_id];
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          l1_BCCH_List_ptr = &l1_BCCH_List[gas_id];

          /* If RR sends a null list, just send back a cnf and do nothing */
          if ( message_ptr->frequency_list.num_frequencies == 0 )
          {
            MSG_GERAN_ERROR_0_G("RR->L1 BCCH decode list empty");
            /* Send CNF(ERROR) back to RR */
            L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
          }
          else
          {

            L1_setup_DECODE_BCCH_LIST( msg_header );

            if ( message_ptr->search_mode == RR_SEARCH_BACKGROUND )
            {
              MSG_GERAN_MED_0_G("Decode BCCH list initiated in background");
              GL1_ISR_LOCK(gas_id);
              l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;
              /* Set up for background power scan */
              /* l1_isr will kick off background pscan based on
              l1_idle_data.background_HPLMN_search_activity
              */
              l1_idle_data->background_HPLMN_search_activity = ACQ_BCCH_LIST;
              GL1_ISR_UNLOCK(gas_id);

            }
            else
            {
              MSG_GERAN_MED_0_G("Decode BCCH list initiated in foreground");
              /* Tell the SCE to stop its idle mode procedures */
              l1_sc_stop(gas_id);

              l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
              /* Foreground list decodes */
              /* Abort idle mode and transition to FIND_BCCH */
              GL1_ISR_LOCK(gas_id);
              l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
              l1_tskisr_blk->command = L1_ACQUIRE_BCCH_LIST_COMMAND;
              L1_send_ISRTIM_ACQ_LIST_REQ( l1_BCCH_List_ptr->current_sch_index,gas_id );
              GL1_ISR_UNLOCK(gas_id);
            }
          }

          /* Rude wakeup from SLEEP, so we respond faster */
          gl1_drx_require_next_tick(gas_id);

          break;
        }

        /********************************************************
        *                                                      *
        *        MPH_DECODE_BCCH_LIST_ABORT_REQ                *
        *                                                      *
        ****************************************************** */

      case MPH_DECODE_BCCH_LIST_ABORT_REQ:
        {
          mph_decode_bcch_list_abort_req_T* message_ptr = ( mph_decode_bcch_list_abort_req_T* )msg_header;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
          gas_id = check_gas_id(message_ptr->gas_id);

          GL1_ISR_LOCK(gas_id);
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          /*
          Indicate that a decode bcch list cnf is to be sent.
          Only sent when aborted by a MPH_DECODE_BCCH_LIST_ABORT_REQ
          */

          l1_tskisr_blk->next_params.L1_confirm_ID = L1_DECODE_BCCH_LIST_CNF;

          L1_transfer_new_data(gas_id);
          L1_suspend_background_HPLMN_search(gas_id);
          GL1_ISR_UNLOCK(gas_id);

          break;
        }

        /********************************************************
        *                                                      *
        *        MPH_POWER_SCAN_ABORT_REQ                      *
        *                                                      *
        ****************************************************** */
        // temp until rr_l1.h updated
      case MPH_POWER_SCAN_ABORT_REQ:
        {
          mph_power_scan_abort_req_T* message_ptr = ( mph_power_scan_abort_req_T* )msg_header;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

          gas_id = check_gas_id(message_ptr->gas_id);
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          GL1_ISR_LOCK(gas_id);
          /*
          Indicate that a power scan cnf is to be sent.
          Only sent when aborted by a MPH_POWER_SCAN_ABORT_REQ
          */

          l1_tskisr_blk->next_params.L1_confirm_ID = L1_POWER_SCAN_CNF;

          L1_transfer_new_data(gas_id);
          L1_suspend_background_HPLMN_search(gas_id);
          GL1_ISR_UNLOCK(gas_id);
          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_READ_SYS_INFO_REQ                 *
        *                                                      *
        ********************************************************/

      case MPH_READ_SYS_INFO_REQ:
        {
          mph_read_sys_info_req_T* mph_read_sys_info_header_ptr = (mph_read_sys_info_req_T*)msg_header;
          mph_read_sys_info_req_T* message_ptr = ( mph_read_sys_info_req_T* )msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);


          if ( l1_is_READ_SYS_INFO_REQ_possible(mph_read_sys_info_header_ptr->si_mask,gas_id) )
          {
            L1_process_READ_SYS_INFO_REQ( (mph_read_sys_info_req_T *)msg_header ); /*lint !e740 */
            break;
          }
          else
          {
            MSG_HIGH("TDS: Received MPH_READ_SYS_INFO_REQ during the G2X measurement", 0, 0, 0);
            L1_send_MPH_READ_SYS_INFO_FAILURE_IND(FALSE, FALSE,gas_id);
            break;
          }
        }

        /********************************************************
        *                                                      *
        *                MPH_SELECT_SPECIFIC_BCCH_REQ          *
        *                                                      *
        ********************************************************/

      case MPH_SELECT_SPECIFIC_BCCH_REQ:
        {
          mph_select_specific_bcch_req_T *message_ptr;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

          /* align pointers */
          message_ptr = (mph_select_specific_bcch_req_T *)msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);

          l1_tskisr_blk = &l1_tsk_buffer[gas_id];

          /* Tell the SCE to stop */
          l1_sc_stop(gas_id);

          GL1_ISR_LOCK(gas_id);

          l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
          L1_send_ISRTIM_ACQ_REQ(message_ptr->specific_channel_no, gas_id);
          GL1_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_COEX_SW_CXM
          garb_intf_notify_find_bcch_update_arfcn(message_ptr->specific_channel_no,gas_id);
          garb_intf_notify_find_bcch_update_rx_power(message_ptr->specific_channel_no,
              (int32)(l1_tskisr_blk->next_params.L1Data.pAgc_data->pwr_dBm_x16),gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

          /* Rude wakeup from SLEEP, so we respond faster */
          gl1_drx_require_next_tick(gas_id);

          break;
        }

      case MPH_CHANNEL_ASSIGN_REQ:
        {

          byte temp;

          mph_channel_assign_req_T  *message_ptr;
          idle_data_T               *l1_idle_data;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
          dedicated_data_T *l1_dedicated_data_ptr;

          message_ptr = (mph_channel_assign_req_T *)msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          l1_idle_data = &l1_idle_data_store[gas_id];
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

          /* Now fill in the new dedicated data */
            memscpy ( &l1_dedicated_data_ptr->channel_specification,
                      sizeof(l1_dedicated_data_ptr->channel_specification),
                      &message_ptr->channel_spec,
                      sizeof(channel_spec_T) );

          /* Load up transmit power */
          temp = l1_dedicated_data_ptr->channel_specification.power_level;
          //            gl1_msg_store_tx_lev(temp);
          l1_dedicated_data_ptr->ms_power_level_in_use = temp;
          l1_dedicated_data_ptr->ms_power_level_to_use = temp;
          l1_dedicated_data_ptr->ms_power_level_to_report = temp;
          l1_dedicated_data_ptr->ordered_ms_power_level = temp;

          /* Transfer the AGC information from IDLE to DEDICATED */
          l1_dedicated_data_ptr->c0_agc = l1_idle_data->campedon_cell_agc;

          l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_CHANNEL_ASSIGN_CNF;

          l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;

          /* Tell the SCE to stop */
          l1_sc_stop(gas_id);

          /* Restart L1 in dedicated mode */
          GL1_ISR_LOCK(gas_id);
          l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND;
          GL1_ISR_UNLOCK(gas_id);

          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_IGNORE_WCDMA_NEIGHBORS_REQ        *
        *                                                      *
        ********************************************************/

      case MPH_IGNORE_WCDMA_NEIGHBORS_REQ:
        {
          mph_ignore_wcdma_neighbors_req_T* message_ptr;
          message_ptr = ( mph_ignore_wcdma_neighbors_req_T* )msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);
          gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);
          l1_sc_wcdma_ignore_neighbors( message_ptr );
          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_WCDMA_CELL_UPDATE_LIST_REQ        *
        *                                                      *
        ********************************************************/

      case MPH_WCDMA_CELL_UPDATE_LIST_REQ:
        {
          mph_wcdma_cell_update_list_req_T* message_ptr;
          message_ptr = ( mph_wcdma_cell_update_list_req_T* )msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);
          l1_sc_set_wcdma_list_update_inprogress( TRUE , gas_id );
          gl1_drx_task_active( gas_id );
          gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);
          l1_sc_wcdma_update_list( message_ptr, gas_id );
          gl1_drx_task_inactive( arg_gas_id );
          l1_sc_set_wcdma_list_update_inprogress( FALSE , gas_id );
          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ     *
        *                                                      *
        ********************************************************/

        case MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ:
          {
              mph_specific_irat_freq_update_req_T* msg;
              msg = ( mph_specific_irat_freq_update_req_T* )msg_header;  
 
              l1_sc_specific_irat_freq_update(msg, msg->gas_id);
              break;
          }
        

        /********************************************************
        *                                                      *
        *                MPH_UE_MODE_CHANGE_REQ                *
        *                                                      *
        ********************************************************/

      case MPH_UE_MODE_CHANGE_REQ:
        {
          mph_ue_mode_change_req_T  *message_ptr;
          message_ptr = ( mph_ue_mode_change_req_T *) msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);

          /* If this occurs during going to sleep or we are asleep
          it can execute when mDSP is asleep and no NPA request
          and no GSM clocks, delay until isr */
          /* wake up layer 1 isr */
          gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);

          l1_sc_wcdma_set_mode(message_ptr->gsm_only,gas_id);
          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_STOP_GSM_MODE_REQ                 *
        *                                                      *
        ********************************************************/

      case MPH_STOP_GSM_MODE_REQ:
        {
          mph_stop_gsm_mode_req_T *message_ptr = (mph_stop_gsm_mode_req_T*)msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          /* Rude wakeup from SLEEP, so we respond faster */
          /* wake up layer 1 isr */
          gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);

          GL1_ISR_LOCK(gas_id);

          /* Tell the SCE to stop its idle mode procedures */
          l1_sc_stop(gas_id);

          /* Stop L1 ISR */
          L1_setup_DEACTIVATE( msg_header );

          GL1_ISR_UNLOCK(gas_id);

          break;
        }

#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
        /********************************************************
        *                                                      *
        *                MPH_SUSPEND_GSM_MODE_REQ              *
        *                                                      *
        ********************************************************/

      case MPH_SUSPEND_GSM_MODE_REQ:
        {
          mph_suspend_gsm_mode_req_T *message_ptr = (mph_suspend_gsm_mode_req_T *)msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          /* Tell the SCE to stop its idle mode procedures */
          l1_sc_stop(gas_id);

          /* Stop L1 ISR */
          L1_setup_SUSPEND(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_G2W_BPLMN_SRCH_SUSPEND);
#endif
          break;
        }

        /********************************************************
        *                                                      *
        *                MPH_INTERRAT_LIST_SRCH_TIME_REQ       *
        *                                                      *
        ********************************************************/

      case MPH_INTERRAT_LIST_SRCH_TIME_REQ:
        {
          mph_interrat_list_srch_time_req_T* message_ptr;
          l1_idle_if_data_t *l1_idle_if_data_ptr;

          message_ptr = ( mph_interrat_list_srch_time_req_T* )msg_header; /*lint !e740 */
          gas_id = check_gas_id(message_ptr->gas_id);


          /* If MPH_INTERRAT_LIST_SRCH_TIME_REQ is received while we are alseep, 
           * need to do force/rude wakeup before neighbor update commands are sent 
           * to MCPM. 
           */
          gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);

          /* Indicate ISR to stop/start scheduling Ncell activity,
          * and keep reporting TIME till next Paging Block to RR
          */
          GL1_ISR_LOCK(gas_id);
          gl1_irat_mutex_lock( gas_id);
          l1_idle_if_data_ptr = &l1_idle_if_data[gas_id];
          if(message_ptr->command == RR_L1_LIST_SRCH_TIME_REQ_START)
          {

             gl1_drx_task_active(gas_id);
             l1_idle_if_data_ptr->background_wcdma_plmn_scan_active = TRUE;

             MSG_GERAN_HIGH_0_G("MPH_INTERRAT_LIST_SRCH_TIME_REQ - start");

             if ((message_ptr->rat == RR_L1_IRAT_RAT_WCDMA)&& (!l1_sc_get_wcdma_irat_mcpm_active(gas_id)) )
             {
               gl1_hw_mcpm_neighbor_update(MCPM_G2W_MEAS_START,L1_IDLE_G2W, gas_id);
             }
             else
             {
#ifdef  FEATURE_GSM_TO_LTE
                 if((message_ptr->rat == RR_L1_IRAT_RAT_LTE)&& (!l1_sc_get_lte_irat_mcpm_active(gas_id)))
                  {
                     gl1_hw_mcpm_neighbor_update(MCPM_G2L_MEAS_START,L1_IDLE_G2L, gas_id);
                  }
                  else
#endif
                 {
#ifdef FEATURE_GSM_TDS
                     if ((message_ptr->rat == RR_L1_IRAT_RAT_UTRAN_TDD)&& (!l1_sc_get_tds_irat_mcpm_active(gas_id)))
                     {
                        gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_START,L1_IDLE_G2T, gas_id);
                     }
#endif
                 }
             }
          }
          else
          {
               l1_idle_if_data_ptr->background_wcdma_plmn_scan_active = FALSE;

               MSG_GERAN_HIGH_0_G("MPH_INTERRAT_LIST_SRCH_TIME_REQ - stop");

               if ((message_ptr->rat == RR_L1_IRAT_RAT_WCDMA)&& (!l1_sc_get_wcdma_irat_mcpm_active(gas_id)) )
               {
                  gl1_hw_mcpm_neighbor_update(MCPM_G2W_MEAS_STOP,L1_IDLE_G2W, gas_id);
               }

#ifdef  FEATURE_GSM_TO_LTE
               else if((message_ptr->rat == RR_L1_IRAT_RAT_LTE)&& (!l1_sc_get_lte_irat_mcpm_active(gas_id)))
                  {
                     gl1_hw_mcpm_neighbor_update(MCPM_G2L_MEAS_STOP,L1_IDLE_G2L, gas_id);
                  }
#endif

#ifdef FEATURE_GSM_TDS
               else if ((message_ptr->rat == RR_L1_IRAT_RAT_UTRAN_TDD)&& (!l1_sc_get_tds_irat_mcpm_active(gas_id)))
                     {
                        gl1_hw_mcpm_neighbor_update(MCPM_G2TDS_MEAS_STOP,L1_IDLE_G2T, gas_id);
                     }
#endif
               else /*Decrement active task count*/
               {
                 gl1_drx_task_inactive(gas_id);
               }
          }
          gl1_irat_mutex_unlock( gas_id);
          GL1_ISR_UNLOCK(gas_id);

          break;
        }
#endif

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM  /* @@OG: EDTM design */

        /*******************************************************
        *                                                      *
        *                MPH_IMMED_ASSIGN_REQ                  *
        *                                                      *
        ********************************************************/

      case MPH_IMMED_ASSIGN_REQ:
        {
          mph_immed_assign_req_T  *message_ptr;
          byte                    temp;
          idle_data_T               *l1_idle_data;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
          dedicated_data_T *l1_dedicated_data_ptr;

          message_ptr = ( mph_immed_assign_req_T *)msg_header; /*lint !e740 unusual pointer cast */
          gas_id = check_gas_id(message_ptr->gas_id);

          l1_idle_data=&l1_idle_data_store[gas_id];
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

          /* give the L1 ISR some previous history */
          l1_dedicated_data_ptr->previous_chnl_type = SDCCH;
          /* Load up the data  from the message */
          l1_dedicated_data_ptr->channel_specification.DTX_indicator = message_ptr->DTX_indicator;

          /* Reset the cipher flag to avoid non-SDCCH setup using previous channel setting */
          l1_dedicated_data_ptr->channel_specification.cipher_flag = FALSE;

          temp = message_ptr->timing_advance;
          l1_dedicated_data_ptr->timing_advance_in_use = temp;
          l1_dedicated_data_ptr->timing_advance_to_use = temp;
          l1_dedicated_data_ptr->ordered_timing_advance = temp;
          gl1_msg_store_ta( temp,gas_id );


          l1_dedicated_data_ptr->channel_specification.starting_time_valid = message_ptr->starting_time_valid;

          temp = calculate_init_txpwr_max_dedicated(gas_id);

          l1_dedicated_data_ptr->ms_power_level_in_use = temp;
          l1_dedicated_data_ptr->ms_power_level_to_use = temp;
          l1_dedicated_data_ptr->ordered_ms_power_level = temp;

          /* Transfer the AGC information from IDLE to DEDICATED */
          l1_dedicated_data_ptr->c0_agc = l1_idle_data->campedon_cell_agc;
          sys_algo_agc_enable_slew_rate_limiting(&l1_dedicated_data_ptr->c0_agc);

          l1_dedicated_data_ptr->channel_specification.starting_time[0] = message_ptr->starting_time[0];
          l1_dedicated_data_ptr->channel_specification.starting_time[1] = message_ptr->starting_time[1];
          l1_dedicated_data_ptr->starting_time = (message_ptr->starting_time[0]*256)+
            (message_ptr->starting_time[1]%256);
          l1_dedicated_data_ptr->freq_redef_imminent = FALSE;
          l1_dedicated_data_ptr->channel_specification.before_parameters_valid =
            message_ptr->before_parameters_valid;
          l1_dedicated_data_ptr->channel_specification.channel_info_1_before = message_ptr->channel_info_before;
          l1_dedicated_data_ptr->channel_specification.channel_info_1_after = message_ptr->channel_info_after;
          /* Set default Value */
          l1_dedicated_data_ptr->channel_specification.channel_mode_1 = SIGNALLING_ONLY;
          l1_dedicated_data_ptr->channel_specification.channel_mode_2 = SIGNALLING_ONLY;

          l1_dedicated_data_ptr->L1_confirm_msg = (uint16)MPH_IMMED_ASSIGN_CNF;

         /* Tell the SCE to stop its idle mode procedures */
          l1_sc_stop(gas_id);


          /* Change the ISR mode */
          GL1_ISR_LOCK(gas_id);

          l1_tskisr_blk->next_params.L1Data.pDedi_data = l1_dedicated_data_ptr;

          l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND;

          GL1_ISR_UNLOCK(gas_id);

          /* Reset the l1 tx data buffers */
          l1_dcch_s0_buffer[gas_id].data_valid = FALSE;
          l1_sacch_s0_buffer[gas_id].data_valid = FALSE;

#ifdef DEBUG
          if (message_ptr->before_parameters_valid) {
            temp = message_ptr->channel_info_before.channel_type;
            if ( temp == SDCCH_4 ) {
              qsprintf(debug_string,
                "beforechnl=SDCCH/4(%d)\n",
                message_ptr->channel_info_before.subchannel );
            }

            else if (  temp == SDCCH_8 ) {
              qsprintf(debug_string,
                "beforechnl=SDCCH/8(%d)\n",
                message_ptr->channel_info_before.subchannel );
            }

            gs_fast_log_progress( GS_UNIQUE_EXCEPTION_NUMBER,
              GS_LOG_TYPE_TASK_PROGRESS,
              debug_string);
          }
          temp = message_ptr->channel_info_after.channel_type;

          if ( temp == SDCCH_4 ) {
            qsprintf(debug_string,
              "afterchnl=SDCCH/4(%d)\n",
              message_ptr->channel_info_after.subchannel );
          } else if (  temp == SDCCH_8 ) {
            qsprintf(debug_string,
              "afterchnl=SDCCH/8(%d)\n",
              message_ptr->channel_info_after.subchannel );
          }

          gs_fast_log_progress( GS_UNIQUE_EXCEPTION_NUMBER,
            GS_LOG_TYPE_TASK_PROGRESS,
            debug_string);
#endif


          /* inform surround cell engine */
          switch ( message_ptr->channel_info_after.channel_type ) {
      case SDCCH_8:
      case SDCCH_4:
        l1_sc_start( L1SCModeDediSDCCh, gas_id );
        break;
      default:
        l1_sc_start( L1SCModeDediTCh, gas_id );
        break;
          }

          L1_remove_oob_ba(&ba_buffer,&l1_idle_data->campedon_cell_data.BA_list);
          l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);
          break;
        }
#endif  /* FEATURE_GSM_EDTM */
#endif  /* FEATURE_GSM_DTM */



      case MPH_ABORT_RA_REQ:
        {

          mph_abort_ra_req_T  *message_ptr;
          message_ptr = ( mph_abort_ra_req_T *)msg_header; /*lint !e740 unusual pointer cast */
          gas_id = check_gas_id(message_ptr->gas_id);

          /*
          * Stop any pending RA bursts and revoke READY_TO_SEND
          */
          GL1_ISR_LOCK(gas_id);
          l1_idle_if_data[gas_id].rach_ready_to_send = FALSE;
          gl1_msg_abort_rach(gas_id);
          re_calculate_initial_rach[gas_id] = TRUE;
          L1_send_MPH_ABORT_RA_CNF(gas_id, GL1_IN_IDLE_MODE);
          GL1_ISR_UNLOCK(gas_id);

          break;
        }

      case MPH_DSC_THRESHOLD_IND:
        L1_handle_MPH_DSC_THRESHOLD_IND( msg_header );
        break;
        /********************************************************
        *                                                      *
        *                Added extra messages for GPRS         *
        *                                                      *
        ********************************************************/
      case MPH_NC_MEASUREMENTS_REQ:
      case MPH_STOP_NC_MEASUREMENTS_REQ:
      case MPH_GPRS_L1PARAM_UPDATE_REQ:
      case MPH_GPRS_SURROUND_UPDATE_REQ:
      case MPH_STOP_SURROUND_UPDATE_REQ:
      case MPH_GPRS_RESELECTION_REQ:

      default:
        {
          gpl1_gprs_handle_grr_idle_mode(msg_header);
#ifdef DEBUG
          gs_fast_log_progress( GS_UNIQUE_EXCEPTION_NUMBER,
            GS_LOG_TYPE_TASK_PROGRESS,
            "L1 unexpected message in Idle)\n");
#endif

          break;
        }


    } /* end of switch */
  } /* end of if */

  /********************************************************
  *                                                      *
  *                PH_RANDOM_ACCESS_REQ                  *
  *                                                      *
  ********************************************************/
  else if (msg_header->message_set == MS_L2_L1)
  {
    if (msg_header->message_id == PH_RANDOM_ACCESS_REQ)
    {
      ph_random_access_req_T   *message_ptr;
      idle_data_T               *l1_idle_data;
      volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

      message_ptr = ( ph_random_access_req_T *) msg_header; /*lint !e740 */
      gas_id = check_gas_id(message_ptr->gas_id);

      l1_idle_data = &l1_idle_data_store[gas_id];
      l1_tskisr_blk = &l1_tsk_buffer[gas_id];

      GL1_ISR_LOCK(gas_id);

      /* check whether READY_TO_SEND has been flagged to GL2 */
      /* because GRR may have cancelled this RACH since then */
      if (FALSE == l1_idle_if_data[gas_id].rach_ready_to_send)
      {
        GL1_ISR_UNLOCK(gas_id);
        MSG_GERAN_ERROR_0_G("RACH READY_TO_SEND has been revoked");
        return;
      }

      /*Check to see whether we need to calculate RACH delay(PS HO enhancement)*/
      if (message_ptr->l2_channel_type == L2_RACH)
      {

        /*
        * Add 1 to the value returned as the return value corresponds to
        * the number of RACH slots NOT used, so the delay to the first is
        * the return value plus 1
        */

        l1_idle_data->random_access_data.ra_delay =
          L1_calculate_RA_first_delay(l1_idle_data->campedon_cell_data.TX_integer);
      }
      else
      {
        l1_idle_data->random_access_data.ra_delay = 0;
        MSG_GERAN_MED_2_G("Fast RACH requested - zero delay used FN=%d, chan_type = %d",
          GSTMR_GET_FN_GERAN( gas_id),message_ptr->l2_channel_type);
      }
      re_calculate_initial_rach[gas_id] = FALSE;

      /* Map txlev to power index before passing to msg layer */
      l1_idle_data->random_access_data.txlev =
        l1_idle_data->campedon_cell_data.MS_TXPWR_MAX_CCH;

      /* Extract RACH content to transmit on access burst */
      l1_idle_data->random_access_data.ra_content =
        message_ptr->random_access_content;

         /* Extract RACH power reduction info */
         l1_idle_data->random_access_data.pwr_reduction_valid = message_ptr->pwr_reduction_valid;
         l1_idle_data->random_access_data.pwr_reduction_value = message_ptr->pwr_reduction_value;
         l1_idle_data->random_access_data.emergency_call = message_ptr->emergency_call;
      /* Check if this is an EGPRS packet channel request */
      if (message_ptr->egprs_ra_content_present)
      {
        /* Extract RACH content and TSC to transmit on 11 bit access burst */
        l1_idle_data->random_access_data.egprs_ra = TRUE;
        l1_idle_data->random_access_data.egprs_tsc = message_ptr->egprs_tsc;
        l1_idle_data->random_access_data.ra_content =
          message_ptr->egprs_ra_content;
      }
      else
      {
        l1_idle_data->random_access_data.egprs_ra = FALSE;
      }

#ifdef FEATURE_GTA_DURING_PS_RACH
      /* clear variables for GTA_PS_RACH. They are not needed in SS RACH */
      l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
      l1_idle_data->random_access_data.rach_for_ps_service = FALSE;
      l1_idle_data->random_access_data.updated_ra_delay = RACH_DELAY_INVALID;
#endif /*FEATURE_GTA_DURING_PS_RACH*/

      /* Tell the SCE to stop its idle mode procedures */
      l1_sc_stop(gas_id);

      l1_tskisr_blk->main_command = L1_ACCESS_COMMAND;

      GL1_ISR_UNLOCK(gas_id);

      /* Tell the DRX manager that we need the next
      * frame tick and also that we need the Tx
      * chain on (indefinitely).
      */
      gl1_drx_require_next_tick(gas_id);

      /* If rude wake-up does not happen then wait for next frame tick
       * so that TX_ON happen in full frame 
       */
      if( gl1_drx_is_gsm_asleep( gas_id ) )
      {
#ifndef GL1_UNIT_TEST
        (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
        (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
#endif
      }
    }
    /********************************************************
    *                                                      *
    *                PH_RANDOM_ACCESS_IND                  *
    *                                                      *
    ********************************************************/
    else if(PH_RANDOM_ACCESS_IND == msg_header->message_id)
    {
       ph_random_access_ind_T *message_ptr = (ph_random_access_ind_T *)msg_header;
       gas_id = check_gas_id(message_ptr->gas_id);
       if (message_ptr->rach_for_ps_traffic)
       {
          MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for PS");
       } else
       {
          MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for CS");
       }

       GL1_ISR_LOCK(gas_id);
       /* mark the interface as READY_TO_SEND */
       l1_idle_if_data[gas_id].rach_ready_to_send = TRUE;
       GL1_ISR_UNLOCK(gas_id);

       L1_send_PH_READY_TO_SEND_IND(L2_RACH, gas_id);
    }
    else
    {
      gas_id = arg_gas_id;  /* MSG_GERAN_ERROR_1_G needs gas_id */ 
      MSG_GERAN_ERROR_1_G("L2->L1(IDLE) message %d ignored.", msg_header->message_id);
    }
  }
  else if (msg_header->message_set == MS_L1_L1)
  {
    uint16 num_satisfy_threshold_criteria=0;
    uint16   i;

    switch (msg_header->message_id )
    {
      /********************************************************
      *                                                      *
      *                ISR_SCAN_CNF                          *
      *                                                      *
      ********************************************************/

    case ISR_SCAN_CNF:
      {
        isrtim_scan_cnf_T *message_ptr = ( isrtim_scan_cnf_T* )msg_header;
          Frequency_List_T *l1_Frequency_List;

          gas_id = check_gas_id(message_ptr->gas_id);
          l1_Frequency_List = l1_Frequency_List_Store[gas_id];

        /* Check L1_Frequency_List pointer is valid, since MPH_STOP_MODE_REQ
        * may have deallocated dynamic memory causing a potential page fault
        */
        if(l1_Frequency_List == NULL)
        {
          MSG_GERAN_HIGH_0_G("l1_Frequency_List NULL!");
          break;
        }

        /* A set of measurements exist in the l1_Frequency_List. */

        /* Calculate the signal strength averages for the list ... */
        L1_normalise_signal_strengths(l1_Frequency_List->length,
          l1_Frequency_List->no_of_measurement,
          l1_Frequency_List->power_dBm_x16,
          l1_Frequency_List->agc
#ifdef GL1_GMSK_MOD_DET
          ,l1_Frequency_List->decstat
#endif
		  );
        /* ... sort it ... */
          num_satisfy_threshold_criteria = L1_sort_signal_strengths(gas_id);

        /* Bounds check to resolve Klocwork ABR errors */
        if ( num_satisfy_threshold_criteria >
          ( sizeof( l1_Frequency_List->results->meas ) / sizeof( l1_Frequency_List->results->meas[0] ) ) )
        {
          MSG_GERAN_ERROR_2_G( "Too many freq list elements requested %d clamp to max %d",
            num_satisfy_threshold_criteria,
            ( sizeof( l1_Frequency_List->results->meas ) / sizeof( l1_Frequency_List->results->meas[0] ) ));

          num_satisfy_threshold_criteria =
            ( sizeof( l1_Frequency_List->results->meas ) / sizeof( l1_Frequency_List->results->meas[0] ) );
        }

        /* ... copy the results into RR's buffer ... */
        for (i=0; i<num_satisfy_threshold_criteria; i++)
        {
          l1_Frequency_List->results->meas[i].RXLEV_average =
            L1_map_dBm_x16_to_rxlev(l1_Frequency_List->power_dBm_x16[l1_Frequency_List->sorted_array_indexes[i]]);

          l1_Frequency_List->results->meas[i].arfcn = l1_Frequency_List->ARFCN[l1_Frequency_List->sorted_array_indexes[i]];
        }

        l1_Frequency_List->results->num_frequencies = num_satisfy_threshold_criteria;

        //INTLOCK();
        //L1_suspend_background_HPLMN_search();
        //l1_idle_data.background_HPLMN_search_activity = NO_HPLMN_SEARCH;
        //INTFREE();

        /* ... and tell RR we're done. */
          L1_send_MPH_POWER_SCAN_CNF(FALSE,gas_id);

      }
      break;

    case  L1_ISR_SINGLE_BLK_FAILURE_CNF:
      {
        t_l1_isr_single_blk_failure_cnf *message_ptr = (t_l1_isr_single_blk_failure_cnf *)msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          gpl1_gprs_send_mac_51_sb_failure( (void *) NULL, gas_id );

        /* No need to worry about freeing/rewinding this memory
        * because it is built in the gs_read_messages( )
        * function.
        */

        MSG_GERAN_MED_1_G(" L1_ISR_SINGLE_BLK_FAILURE_CNF rcvd fn %d",
                          GSTMR_GET_FN_GERAN( gas_id )
                         );

      }
      break;

    default:
        gas_id = arg_gas_id;  /* MSG_GERAN_ERROR_1_G needs gas_id */ 
      MSG_GERAN_ERROR_1_G("RR->L1(IDLE) message %d ignored.", msg_header->message_id);
      break;
    }
  }

  /*******************************************************
  *                                                      *
  *                   GPRS MAC-L1 messages               *
  *                                                      *
  ********************************************************/
  else if (msg_header->message_set == MS_MAC_L1)
  {
    gpl1_gprs_handle_mac_idle_mode(msg_header);
  }
  /*******************************************************
  *                                                      *
  *                   GSM CB-L1 messages                 *
  *                                                      *
  ********************************************************/
  else if ( msg_header->message_set == MS_CB_L1)
  {
    if (msg_header->message_id == CB_SCHEDULING_REQ) /*lint !e641 */
    {
      cb_scheduling_req_type *message_ptr;
      message_ptr = (cb_scheduling_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_scheduling_info
        ( message_ptr->channel_ind,
        message_ptr->scheduling_mode,
        message_ptr->scheduling_descriptor.scheduling_period_fn_list,
        message_ptr->scheduling_descriptor.scheduling_period_length,
        geran_map_nas_id_to_gas_id(message_ptr->as_id)
        );
    }
    else if (msg_header->message_id == CB_SKIP_REQ)  /*lint !e641 */
    {
      cb_skip_req_type *message_ptr;
      message_ptr = (cb_skip_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_skip_info
        ( message_ptr->channel_ind,
        message_ptr->scheduling_descriptor.scheduling_period_fn_list,
        message_ptr->scheduling_descriptor.scheduling_period_length,
        message_ptr->skip_ind,
        geran_map_nas_id_to_gas_id(message_ptr->as_id)
        );
    }
    else if (msg_header->message_id == CB_DEACTIVATE_REQ )  /*lint !e641 */
    {
      cb_deactivate_req_type *message_ptr;
      message_ptr = (cb_deactivate_req_type *) msg_header; /*lint !e740 */

      l1_smscb_process_deactivate_info( message_ptr->channel_ind, geran_map_nas_id_to_gas_id(message_ptr->as_id) );
    }
  }
  else
  {
    gas_id = arg_gas_id;  /* MSG_GERAN_ERROR_1_G needs gas_id */ 
    MSG_GERAN_ERROR_0_G("L1(IDLE) - Unknown message set.");
  }

}

/*===========================================================================

FUNCTION  L1_REMOVE_OOB_BA

DESCRIPTION
  This function copies the BCCH ARFCNs from baptr2 to baptr1 except for
  out of band ARFCNs

DEPENDENCIES
  pntr to output BA list, pntr to input BA list

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  L1_remove_oob_ba(inter_task_BCCH_allocation_T *baptr1,
                            inter_task_BCCH_allocation_T *baptr2 )

{
   byte input_size;
   byte output_size;
   ARFCN_T BA_ARFCN;
   word  i;

   input_size = baptr2->no_of_entries;
   output_size = 0;

   for ( i=0 ; i < input_size; i++)
   {
      BA_ARFCN = baptr2->BCCH_ARFCN[i];
      baptr1->BCCH_ARFCN[output_size] = BA_ARFCN;
      output_size++;
   }
   baptr1->no_of_entries = output_size;
}

/*===========================================================================

FUNCTION  L1_setup_START_IDLE

DESCRIPTION
  Process an MPH_START_IDLE_REQ message from (G)RR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_setup_START_IDLE( IMH_T *msg_header )
{
   mph_start_idle_req_T          *message_ptr;
   inter_task_BCCH_allocation_T  ba_buffer;
   idle_data_T               *l1_idle_data;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
   gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
   gas_id_t                      gas_id;

   message_ptr = (mph_start_idle_req_T *)msg_header; /*lint !e740 */
   gas_id = check_gas_id(message_ptr->gas_id);

   /* Enter idle mode, so allow sleep */
   gl1_drx_set_sleep_inhibit(FALSE, gas_id);

   /* align pointers */
   l1_idle_data = &l1_idle_data_store[gas_id];
   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[gas_id];

   /* Reset the flag which allows c value calculations
    * to run on ccch during cs idle.
    */
   gprs_serv_cell_meas_ptr->cal_c_value_51_idle = FALSE;

   /* Load up the data  from the message */
   l1_idle_data->campedon_cell_data      = message_ptr->layer1_sys_info;
   l1_idle_data->imsi_data.imsi_valid    = message_ptr->imsi_valid;
   l1_idle_data->imsi_data.imsi_mod_1000 = message_ptr->imsi_mod_1000;

   if (message_ptr->decode_bcch == FALSE)
   {
      /* stop decoding SI */
      l1_idle_data->si_mask       = RR_L1_SI_TYPE_NONE;
      l1_idle_data->BCCH_mask     = SERV_BCCH_MASK_NONE;
      l1_idle_data->ext_BCCH_mask = SERV_EXT_BCCH_MASK_NONE;
   }
   else
   {
      /* keep doing whatever we were doing in
       * the previous mode
       */
   }

   if ( l1_smscb_is_active(gas_id) )
   {
      ARFCN_T cbch_arfcn = l1_smscb_get_arfcn(gas_id);
      ARFCN_T serving_cell_arfcn = gl1_get_serving_cell_arfcn(gas_id);
      if (!ARFCNS_EQUAL(cbch_arfcn, serving_cell_arfcn) ||
          l1_smscb_get_bsic(gas_id) != gl1_get_serving_cell_BSIC(gas_id) )
      {
         l1_smscb_init(gas_id);

      }
   }

   l1_smscb_set_cb_supported (
      message_ptr->layer1_sys_info.cbch_channel_information_valid,gas_id
      );

   l1_smscb_set_chan_type(
      message_ptr->layer1_sys_info.cbch_channel_information.channel_type,gas_id
      );
   l1_smscb_set_arfcn( gl1_get_serving_cell_arfcn(gas_id),gas_id);

   l1_smscb_set_bsic( gl1_get_serving_cell_BSIC(gas_id),gas_id );


   /* Decode BCCH only if RR tells us to
      Part of the cell resel optimizations and a departure from earlier
      releases
   */
    /*set state of SC engine*/
   l1_sc_start(L1SCModeIdle, gas_id);

   /* Tell SCE to get ncell BCCHs every five minutes. */
   l1_sc_set_bcch_autonomy( FALSE,gas_id );

   l1_sc_balist_set_serving_cell( l1_idle_data->campedon_cell_ARFCN,gas_id );

   /*copy BA list to SC engine*/
   L1_remove_oob_ba(&ba_buffer,&message_ptr->layer1_sys_info.BA_list);
   l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);

   /* Set multiband reporting parameter */
   l1_sc_balist_set_multiband_reporting( message_ptr->layer1_sys_info.multiband_reporting,gas_id );

   /*tell surrounding cell engine gap between paging blocks*/
   l1_sc_set_paging_multiframes(l1_idle_data->campedon_cell_data.BS_PA_MFRMS,gas_id);

   L1_calculate_paging_data(gas_id);

   /* We no longer start off in PAGING_REORG we receive the START_IDLE_REQ.
      Part of the cell resel optimizations and a departure from earlier
      releases
   */
   l1_idle_data->paging_data.paging_mode       = NORMAL_PAGING;
   l1_idle_data->paging_data.all_paging_blocks = FALSE;

   /* make sure that the non drx timer is disabled */
   l1_idle_data->drx_timer_max = 0;

   if ( gl1_msg_is_sbd_enabled( gas_id ) )
   {
     l1_xmsi_copy_params(&message_ptr->xmsi,gas_id);
   }

   l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;

   l1_idle_if_gps_notify(gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   /* Clear pre-idle mode indications */
   l1_tskisr_blk->x2g_resel_in_progress = FALSE;
   l1_tskisr_blk->x2g_cco_in_progress = FALSE;

   gpl1_set_invert_sys_info_trm_priority(FALSE,gas_id);
#endif
}


/*===========================================================================

FUNCTION  L1_setup_EARLY_CAMPING

DESCRIPTION
  Process an MPH_EARLY_CAMPING_REQ message from (G)RR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_setup_EARLY_CAMPING( IMH_T *msg_header )
{

   mph_early_camping_req_T                *message_ptr;
   idle_data_T                            *l1_idle_data;
   volatile ISRTIM_CMD_BLK                *l1_tskisr_blk;
   gprs_pl1_serving_cell_meas_T           *gprs_serv_cell_meas_ptr;
   l1_sc_wcdma_data_t                     *l1_sc_wcdma_data_ptr;
   gas_id_t                               gas_id;

   /* align pointers */
   message_ptr = (mph_early_camping_req_T *)msg_header; /*lint !e740 */
   gas_id = check_gas_id(message_ptr->gas_id);
   
   l1_idle_data = &l1_idle_data_store[gas_id];
   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
   gprs_serv_cell_meas_ptr = &gprs_serv_cell_meas[gas_id];
   
   /* If Re camping after moving out of Dedicated mode, need to deactivate X RATs
      will be initialized on receiving new neighbor list from RR */
   if( l1_sc_wcdma_data_ptr->wcdma_srch.prev_state == GtowStateDedicated )
   {
     l1_sc_wcdma_data_ptr->wcdma_srch.prev_state = GtowStateNull;
	 
     /* Force G2X shutdown */
     l1_sc_irat_check_active(0, 0, TRUE, 0, FALSE, gas_id);
   }


   /* Reset the flag which allows c value calculations
    * to run on ccch during cs idle.
    */
   gprs_serv_cell_meas_ptr->cal_c_value_51_idle = FALSE;

   l1_idle_data->paging_data.paging_mode = message_ptr->paging_mode;

   /* Set state of SC engine */
   l1_sc_start(L1SCModeIdle, gas_id);

   /* Tell SCE that GRR will schedule ncell BCCH decodes. */
   l1_sc_set_bcch_autonomy( FALSE,gas_id );

   l1_sc_balist_set_serving_cell( l1_idle_data->campedon_cell_ARFCN,gas_id );

   /* Early camping was introduced so that we don't miss pages during a
      reslection when RR is trying to decode the SIs to figure out the
      paging parameters.   RR tells us what mode we should start off in.
      If it knows the paging parameters, we start off in NORMAL_PAGING,
      otherwise in paging reorg.  In either case we continue decoding
      whichever SIs we were decoding in the previous mode.
   */
   if ( message_ptr->paging_mode == NORMAL_PAGING )
   {
      /* Load up the data  from the message */
      l1_idle_data->campedon_cell_data.BS_PA_MFRMS        = message_ptr->BS_PA_MFRMS;
      l1_idle_data->campedon_cell_data.BS_AG_BLKS_RES     = message_ptr->BS_AG_BLKS_RES;
      l1_idle_data->campedon_cell_data.BS_CC_CHANS        = message_ptr->BS_CC_CHANS;
      l1_idle_data->campedon_cell_data.BS_CCCH_SDCCH_COMB = message_ptr->BS_CCCH_SDCCH_COMB;
      l1_idle_data->imsi_data.imsi_valid                  = message_ptr->imsi_valid;
      l1_idle_data->imsi_data.imsi_mod_1000               = message_ptr->imsi_mod_1000;

      l1_idle_data->paging_data.all_paging_blocks         = FALSE;

      l1_idle_data->campedon_cell_data.BS_PA_MFRMS = l1_override_bs_pa_mfrms(message_ptr->BS_PA_MFRMS, gas_id);
      /*tell surrounding cell engine gap between paging blocks*/
      l1_sc_set_paging_multiframes(l1_idle_data->campedon_cell_data.BS_PA_MFRMS,gas_id);

      L1_calculate_paging_data(gas_id);

      (void)l1_log_paging_data( gas_id);
   }
   else
   {
      /* Don't actually have enough info to calculate paging
       * data, but have to setup some parameters anyway.
       */
      l1_idle_data->paging_data.paging_data_valid = FALSE;

      /* Paging reorg mode */
      l1_idle_data->paging_data.all_paging_blocks = TRUE;

      /* Start off on timeslot 0 since we don't know
       * which one we're really meant to be on.
       */
      l1_idle_data->paging_data.ccch_group = 0;

      /*
         Assume its non-combined so that we play it safe and read all
         paging blocks.  At this stage we don't know what the control
         channel structure is.
      */
      l1_idle_data->campedon_cell_data.BS_CCCH_SDCCH_COMB = FALSE;
      l1_idle_data->imsi_data.imsi_valid                  = message_ptr->imsi_valid;
      l1_idle_data->imsi_data.imsi_mod_1000               = message_ptr->imsi_mod_1000;
   }

   l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;

   /* Reset non drx timer parameters since cell reselection may cause non
    * drx timer to continue on target cell which is incorrect
    */
   l1_idle_data->drx_timer_max = 0;
   l1_idle_data->non_drx_timer = 0;

   /*
    **  moved GPS gl1_timetag_gps post cell notify to start idle locations
   */

   if ( gl1_msg_is_sbd_enabled( gas_id ) )
   {
     l1_xmsi_copy_params(&message_ptr->xmsi,gas_id);
   }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   //Enable DR mode by defaul when camping on to new cell
   MSG_GERAN_HIGH_0_G("Enabling DR mode");
   L1_enable_dual_receive(l1_tskisr_blk->client_id, gas_id);
#endif

}

/*===========================================================================

FUNCTION  L1_process_READ_SYS_INFO_REQ

DESCRIPTION
  Process an MPH_READ_SYS_INFO_REQ message from (G)RR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  May cause L1 to start decoding the BCCH and/or extended BCCH.

===========================================================================*/
void L1_process_READ_SYS_INFO_REQ( mph_read_sys_info_req_T *message_ptr )
{
   uint32 prev_BCCH_mask;
   uint32 prev_ext_BCCH_mask;
   boolean wakeup = FALSE;
   gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];

   prev_BCCH_mask = l1_idle_data->BCCH_mask;
   prev_ext_BCCH_mask =  l1_idle_data->ext_BCCH_mask;

   if (message_ptr->replace_prev_mask)
   {
      l1_idle_data->si_mask = message_ptr->si_mask;
   }
   else
   {
      l1_idle_data->si_mask |= message_ptr->si_mask; /*lint !e655 bit-wise operation uses enums */
   }

   l1_idle_data->BCCH_mask      = gl1_compute_BCCH_mask( l1_idle_data->si_mask );
   l1_idle_data->ext_BCCH_mask  = gl1_compute_ext_BCCH_mask( l1_idle_data->si_mask );

   MSG_GERAN_MED_2_G("MPH_READ_SYS_INFO_REQ 0x%x, %d",
           message_ptr->si_mask,
           message_ptr->replace_prev_mask);

   MSG_GERAN_MED_2_G("BCCH_mask=0x%x, ext_BCCH_mask=0x%x",
           l1_idle_data->BCCH_mask,
           l1_idle_data->ext_BCCH_mask);

   /* Reset SIB procedure priority when an empty BCCH mask is received */
   if (l1_idle_data->si_mask == RR_L1_SI_TYPE_NONE)
   {
     gpl1_set_invert_sys_info_trm_priority(FALSE, gas_id);
   }

   if(gl1_hw_get_sleep_state(gas_id) == GL1_HW_SLEEP_ASLEEP)
   {
     if (l1_idle_data->si_mask != RR_L1_SI_TYPE_NONE)
     {
       uint32 this_FN, qsym, gap_to_bcch, bcch_fn;

       gl1_get_gsm_time_tag( gas_id, &this_FN, &qsym );
       if ( qsym > 0 )
       {
         this_FN = ADD_FN(this_FN,1);
       }
       /* find gap to the next BCCH */
       gap_to_bcch = L1_gap_to_next_bcch( this_FN, l1_idle_data->BCCH_mask, l1_idle_data->ext_BCCH_mask, gas_id );
       if ( gap_to_bcch != GL1_DEFS_INVALID_FN )
       {
         bcch_fn = ADD_FN( this_FN, gap_to_bcch );
         /* if BCCH frame is after wake-up frame then no need to wake-up now
          * Also wakeup if BCCH RX is in conflict with any low priority
          * reserved task
          */
         if (   (IS_FRAME_NUM_LATER( (gl1_get_FN(gas_id) + 1), bcch_fn ))
             || l1_fm_task_in_conflict( FM_PRI_CCCH, bcch_fn, gas_id)
             || (   (l1_tsk_buffer[gas_id].reservation_for_bcch)
                 && (l1_get_fm_trm_priority(gas_id) == FM_PRI_CCCH)
                 && (IS_FRAME_NUM_LATER(bcch_fn, gl1_get_FN(gas_id)))
                )
            )
         {
           wakeup = TRUE;
         }
         else
         {
           MSG_GERAN_HIGH_0_G("No need to rude wake-up");
         }
       }
       /* Code should not hit here, as  L1_gap_to_next_bcch() will return GL1_DEFS_INVALID_FN only if the SI mask is NONE */
       else 
       {
         MSG_GERAN_ERROR_0_G("Invaid gap_to_bcch for non zero SI mask");
       }
     }
     /* When empty BCCH mask is received do rude wakeup */
     else if( l1_tsk_buffer[gas_id].reservation_for_bcch )
     {
       wakeup = TRUE;
     }

   }
   else
   { 
     /*already awake state check if next tick reqiured based on SI mask*/
     if(l1_tsk_buffer[gas_id].reservation_for_bcch)
     {
       wakeup = TRUE;
     }
     else
     {
       MSG_GERAN_HIGH_0_G("next drx tick is not needed");
     }
   }

   if ( wakeup )
   {
     gl1_drx_require_next_tick(gas_id);
   }
}


/*===========================================================================

FUNCTION  l1_xmsi_copy_params

DESCRIPTION
  None

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_xmsi_copy_params (xmsi_params_T *xmsi_ptr, gas_id_t gas_id)
{
    idle_data_T *l1_idle_data=&l1_idle_data_store[gas_id];

    l1_idle_data->imsi_data.updated = FALSE;

    /*IMSI*/
    if ( xmsi_ptr->imsi_valid )
    {
        memscpy(&l1_idle_data->imsi_data.imsi,sizeof(l1_idle_data->imsi_data.imsi),xmsi_ptr->imsi,GL1_SIM_IMSI_SIZE);
        l1_idle_data->imsi_data.imsi_full_valid = TRUE;
        l1_idle_data->imsi_data.updated         = TRUE;
    }
    else
    {
        l1_idle_data->imsi_data.imsi_full_valid = FALSE;
    }

    /*TMSI*/
    if ( xmsi_ptr->tmsi_valid )
    {
        memscpy(&l1_idle_data->imsi_data.tmsi,sizeof(l1_idle_data->imsi_data.tmsi),xmsi_ptr->tmsi,GL1_SIM_TMSI_SIZE);
        l1_idle_data->imsi_data.tmsi_valid = TRUE;
        l1_idle_data->imsi_data.updated    = TRUE;
    }
    else
    {
        l1_idle_data->imsi_data.tmsi_valid = FALSE;
    }

    /*PTMSI*/
    if ( xmsi_ptr->ptmsi_valid )
    {
        memscpy(&l1_idle_data->imsi_data.ptmsi,sizeof(l1_idle_data->imsi_data.ptmsi),xmsi_ptr->ptmsi,GL1_SIM_PTMSI_SIZE);
        l1_idle_data->imsi_data.ptmsi_valid = TRUE;
        l1_idle_data->imsi_data.updated     = TRUE;
    }
    else
    {
        l1_idle_data->imsi_data.ptmsi_valid = FALSE;
    }

    /* If any of the cached params are updated then update EPD active */
    if ( l1_idle_data->imsi_data.updated )
    {
      l1_idle_data->imsi_data.epd_pref = xmsi_ptr->epd_pref;
      l1_set_epd_pref(xmsi_ptr->epd_pref,gas_id);
    }
}


/* EOF */
