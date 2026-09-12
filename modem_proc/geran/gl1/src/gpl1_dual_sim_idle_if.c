/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

   L 1  M U L T I  S I M   I D L E  M O D E  T A S K  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events while the L1
   task is in the IDLE mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_idle_mode
    Procedure to handle external events while the L1 task is in the
    IDLE mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2009-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dual_sim_idle_if.c#2 $
$DateTime: 2019/12/30 04:21:22 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
20/01/17   akm      CR1109976 Optimization in G2X BPLMN search
10/05/17   sk        CR2024440 Add an explicit intlock for DivRx meas buffer update
02/01/18   km        CR2013722 Update DSC threshold in SR mode
26/09/16   ksb       CR059213 Send a pre-cell time tage in foreground powersacan 
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
18/5/16    sm        CR1017901 Added GL1 idle mode QSH events 
17/05/16   dg        CR1014646 Don't turn MCPM ON for G2X neighbors on receiving list search request if G2X is already active
26/04/16   hd        CR998694 Send MPH_INTERRAT_LIST_SRCH_TIME_IND with gap to next CCCH as soon as the REQ is also received
27/02/16   sk        CR981473 Correction of handling DS ABORT INDICATION message, gpl1_gprs_handle_ds_abort_ind() called
10/09/15   sk        CR899847 GERAN changes to trigger QSH events and handler for debugging
29/09/15   dg        CR914960 Don't turn MCPM off for G2X neighbors on receiving list search request if G2X is already active
28/08/15   cja       CR892470 G2X to cope with overlapping init/deinit/init 
05/08/15   hd        CR884936 Don't delay for next frame when GL1 handles the MPH_UE_CHANGE_MODE_REQ in idle mode when L1 is awake
23/07/15   cah       CR872243 LWG + G DR-DSDS : LTE Conn with G2G mobility failing, FC -> DTA Measurements 
06/07/15   nk        CR866473 handle DS ABORT INDICATION message in Idle mode.
13/07/15   ab        CR832798 FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
12/07/15   nm        CR861458 Fix the interworking issues between PCHXFER, DRDSDS, RX/TX split
25/05/15   sjv       CR843437 Make sure to add PD bands to TRM reserve_at , if next PCH block includes PM on denied bands
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
10/04/15   cah       CR819666 Problem in the logic for adding the denied bands to trm_reserve_at and trm_request
31/03/15   pa        CR815621:GRM restructuring for TH 2.0. 
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
30/03/15   ap        CR811753 FR22272 - TRM Unification - GL1 changes to support 
                                        SysInfo priority inversion during early camping
25/03/15   ap       CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
05/12/14   sp        CR767083: Override paging multiframe to 2, for PDT tests
25/02/14   cah       CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good serving cell conditions
18/02/14   br        CR796260 Handling the race condition between  RACH req and IDLE update request 
19/02/15   nk        CR790888 L1 to send RACH confirmation with correct state when received PH_RANDOM_ACCESS_REQ in IDLE mode
11/02/15   js        CR775188 GL1 changes to receive EPD preference
30/01/15   pa        CR703985:GSM RxD RxLEV report should be based on maximum of PRx and DRx average RSSIs.
10/12/14   dg        CR765859 Do not go to sleep if MCPM is not updated for G2X MEAS STOP activity
15/12/14   ggu       CR769740: To prevent FM retaining the TRM when abort the RA
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   ws        CR768376 Q6 compile warning fixes
28/10/14   cgc       CR745548 GL1 make gl1_drx_require_and_wait_for_next_tick() conditional on awake
10/11/14   ap        CR753835 Handling race conditions that occur due to receiving RA abort followed by GRR msg
04/11/14   ap        CR745457 Allow GL1 to abort Idle and move to Decoupled State when TRM is held with RETAINED_FOR_ACCESS
22/10/14   cah       CR737592: optimisation for persistent denial based on Scell RSSI and SnR
17/10/14   cs        CR735918 Assign supported bands for initial ACQ based on UE support
08/10/14   pa        CR732823: Hybrid Tuneaway Fallback: Avg RSSI is an avergae of last 3 PCH blocks. 
07/10/14   pa        CR733749:Handling of MPH_DSC_THRESHOLD_IND in the RACH state. 
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build 
03/10/14   ap        CR730903 Handling race conditions of receiving RA req following RA abort and select specific BCCH req in decoupled state
24/09/14   cah       CR729408: Updates to reduce number of TRM API calls in Gl1. 
11/09/14   am        CR677361 Addressing KW GL1 P1 errors
09/09/14   pa      	 CR719024: Implementation of Hybrid Tuneaway Fallback Algorithm  
20/06/14   jj        CR656872 fix critical gl1 kw error  
28/05/14   ap        CR671971: Change SGLTE mode on reception of UE_MODE_CHANGE_REQ
29/08/14   cah       CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
21/08/14   dv        CR711417 Ensure DR mode is enabled when camping on cell 
07/08/14   dv        CR706211 allow DRDSDS fallback mechanism to be controlled via BOOL
31/07/14   cos       CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
16/06/14   br        CR680551 Dont check fo trm status in gpl1_client_in_tuneaway_gap, if gbta is active
30/06/14   sp        CR687317: On TRM denial for RACH, use call_in_task_trm_release to task.
20/07/14   zc	     CR691195 DRDSDS release device if currently granted when receiving a rach request 
14/07/14   cs        CR693967 Get DR-DSDS mode during startup
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
01/07/14   cs        CR688003 Runtime enable for DR-DSDS
07/08/14   ss        CR689428 Ensure TRM is released after RACH abort  
07/07/14   smd       CR690073: Fixed a compile error when SGLTE_DUAL_SIM feature is not defined.
02/07/14   aga       CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution 
30/05/14   ws        CR657674 Data + MMS Phase 1
20/05/14   smd       CR662851: Release and request RF device again for RACH in SGLTE+G
29/05/14   miaof     CR669986 GL1 resets TRM request state to NULL when changing main command to L1_ACCESS_COMMAND_WAIT_FOR_TRM
29/05/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
21/05/14   ssh       CR668643: gl1_hw_qta_gap_active() should return FALSE for multimode sub
08/05/14   sp        CR655478:Handle Unsupported Bands on WTRs while RANE during Powerscan.
10/05/14   rc        CR662550: Back out changes made in CR655548
06/05/14   smd       CR655548: Release and request RF device again for RACH in SGLTE+G
15/04/14   ka        CR611185: Ensure information from the MPH_START_GPRS_IDLE_MODE_REQ is used in RACH
09/04/14   ap        CR640266 L1 is unable to sleep because X2G reselect in progress is always indicated
18/03/14   ap        CR629844 SGLTE BPLMN support when G1 in PS call in G+G - PS Hopping
18/03/14   ap        CR614949 FR 19574: Performing LTE/TDS measurements while GERAN is in connected mode using the second WTR
24/01/14  cah       CR598552: enable sleep for DSDS GSTMR+VSTMR
06/01/13   sk        CR594544 Do not release TRM from task when on L1_ACCESS_COMMAND_WAIT_FOR_TRM
11/12/13   sk        CR588079 Ensure Transfer doesnt start when TRM release pending in the task
13/12/13   ap        CR588766: FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS amendment
12/02/13   pjr       CR574393 Added break between MPH_EARLY_CAMPING_REQ and MPH_DECODE_BCCH_LIST_REQ 
28/11/13   cgc       CR581186 Desense SYS_PARAM_MIN_ACCEPTABLE_RX_LVL in SGLTE mode                     
27/11/13   mko       CR505036 Turn on W clock for G2W BPLMN in multisim mode
26/11/13   sk        CR581905 Ensure TRM doesnt get released immediately after grant for RACH
19/11/13   ssh/ip    CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   nm        CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
12/11/13   pjr       CR574393 Added break between MPH_EARLY_CAMPING_REQ and MPH_DECODE_BCCH_LIST_REQ 
                              cases in function gpl1_gprs_handle_dual_sim_idle
27/11/13   mko       CR505036 Turn on W clock for G2W BPLMN in multisim mode
23/10/13   am        CR523325 Do not use fm suspend_trm_release for TA gap
17/10/13   mko       CRCR556411:If firmware_load_status is pending while handling the PH_RANDOM_ACCESS_REQ then put it to TRM denial for RACH.
10/10/13   js        CR520476 Always set the power scan frequency parameters to 0, when we receive power scan request
24/07/13   ss        CR515896 Set wcdma_list_update_inprogress to TRUE in msg_handler routine 
05/10/13   gk        CR516242:Reset the number of frequencies completed to 0, after receiving the power scan request
05/10/13   gk        CR542963:Removed Reset of release pending at gprs idle mode req.
16/08/13   gk        CR530309:In case of back-to-back PLMN search, no need to wait for 3 frames for the FW to be setup properly, if the PLMN state is NULL
05/10/13   sk        CR554393 Ensure RACH doesnt get processed inside the QTA gap
03/10/13   sk        CR553207 Set DRX off when idle is aborted for RACH and ensure trm reserve for Access is not made when going to sleep
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
17/09/13   sp        CR542963: Removed Reset of release pending at gprs idle mode req.
23/05/13   pg        CR460555: Enable 3GPP Rel10 RACH Power Reduciton feature  
12/09/13   js        CR512394:Send IDLE_MODE_CNF in case of receiving IDLE_MODE_REQ, despite sub being in IDLE
09/09/13   ab        CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
02/09/13   nm        propagation(from Nikel) - CR487632 Do not release trm unconditionaly on RA abort
02/09/13   nm        propagation(from Nikel) - CR511322 Reset trm request state if RACH is aborted
02/09/13   nm        propagation(from Nikel) of CR465537  GL1/GRR – Introduce MPH_RA_ABORT_REQ and MPH_RA_ABORT_CNF for DSDS
                     to resolve state transition in RR/L1.
28/08/13   sk        CR535310 Ensure RACH doesnt get processed inside the GBTA gap
08/08/13   dv        CR519879 Revert CR513969
07/08/13   gk        CR509196 :Abort the previous power scan, in case of reception of back-to-back power scan request
16/07/13   dv        CR513969 Prevent Changing from Idle to xfer in QTA gap
21/06/13   cs        Major Triton TSTS Syncup
12/06/13   ap        CR499133 Handle Rach Abort Req in DSDS
07/06/13   cs        TSTS Updates
29/05/13   am        CR488891 Ensure SCE is stopped on randomm access
03/05/13   sjw       CR444512: avoid RACH in QTA gap kept open with a delayed TRM release
29/04/13   ws        CR481359 enable GTA support
06/04/13   sv        CR462075: Corrected the logic when ignoring GRR commands in GTW PLMN cases.
05/09/13   ws        CR445852: use correct casting for MPH_IDLE_UPDATE_REQ - Fixes TX issues on sub2
25/03/13   mc        CR464496 COEX Support - GERAN arbitrator SW CXM integration
12/02/13   ip        CR450696
10/02/13   ip        CR448394
29/01/13   sjw       CR445852: use correct casting for MPH_IDLE_UPDATE_REQ
19/01/13   mc        CR440588
18/05/12   mc        DSDS CR359187:To avoid further collisions with QTA gaps, Set L1_DECOUPLE_MULTI_SIM_IDLE_COMMAND when
                     denying RACH due to QTA in progress.
14/02/13   pjr       CR452272 added support to set multisim mode at activation time
09/10/12   cgc       CR393032 add l1_idle_if_gps_notify()
19/12/11   ap        DSDS CR326356 ACCESS REQ is further pre-empted causing race condition
20/11/11   ap        DSDS CR322448 ACCESS REQ is pre-empted causing race condition
30/06/11   nt        apply CR257869 Complete mode switch in ISR context to DSDS
06/04/11   nt        DSDS CR279823 - Change to ensure no race between ISR and task for access
16/02/11   sjw       DSDS - removal of FEATURE_MULTI_SIM_WCDMA_TUNEAWAY
09/02/11   nt        CR274736 DSDS Change to fix wakeup issue on SCMM target
31/01/11   nt        CR273642 DSDS Always Wakeup GSM L1 upon PH_RANDOM_ACCESS_REQ
27/01/11   dmb       The following RR<->L1 messages are removed (not used):
                     MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
11/01/11   nt        DSDS CR269112 - Fix to manage sleep for DSDS scenarios
15/10/10   ap        DSDS Unable to leave idle mode as stuck in idle abort
09/07/10   nt        Move ASYNC to SYNC sim swap to ISR context
30/06/10   sjw       Introduction of GRM framework
09/06/10   nt        CR241996 - SWAP async to SYNC only after lock acquisition
08/05/10   nt        CR237159 - DS decouple IDLE mode upon rach retry
13/04/10   nt        CR233867: Reset the BPLMN state when receiving a powerscan/bcch_decode
23/03/10   nt        Changes to prevent BPLMN mechanism from getting into the wrong state
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1_drx.h"
#include "l1_ds.h"
#include "l1_log.h"                   /* For logging */
#include "l1_sc_int.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "ms.h"
#include "ms_timer.h"
#include "sys_stru.h"
#include "msg.h"
#include "l1_smscb.h"
#include "cbl1if.h"
#include "l1_sc.h"

#include "gpl1_gprs_task.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "l1_isr.h"

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"

#include "gpl1_dual_sim.h"
#include "geran_dual_sim_g.h"

#include "gpl1_grm_intf.h"
#include "gl1_drdsds.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif 

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
/*=======================EXTRENAL FUNCTIONS=========================*/

extern void L1_suspend_background_HPLMN_search( gas_id_t gas_id );

extern boolean re_calculate_initial_rach[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION  gpl1_gprs_handle_multi_sim_idle

DESCRIPTION
  This function handles the L1 Task in idle mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_multi_sim_idle(IMH_T *msg_header, gas_id_t gas_id)
{
   /* Locals */
   inter_task_BCCH_allocation_T  ba_buffer;

   if (msg_header->message_set == MS_RR_L1)
   {
#ifdef FEATURE_GTOW_BACKGROUND_PLMN_SEARCH
      if(l1_get_background_wcdma_plmn_scan_active(gas_id))
      {
        /* If background WCDMA PLMN Search is active, then do not
         * process any other message from RR except the three below
         */
        if ((msg_header->message_id != (byte) MPH_INTERRAT_LIST_SRCH_TIME_REQ) &&
            (msg_header->message_id != (byte) MPH_SUSPEND_GSM_MODE_REQ) &&
            (msg_header->message_id != (byte) MPH_STOP_GSM_MODE_REQ)
           )
        {
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
              gas_id_t gas_id;
              int suspend_cntr = 0;

              message_ptr = (mph_power_scan_req_T *)msg_header; /*lint !e740 unusual pointer cast */
              gas_id = message_ptr->gas_id;
              l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];
              l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];

              if( ( NO_HPLMN_SEARCH != l1_idle_data->background_HPLMN_search_activity )
                   && ( message_ptr->search_mode == RR_SEARCH_BACKGROUND )
                )
              {
                /* Incase of back-to-back PLMN search, without the first one completing fully,
                * We need to abort the previous search.
                */
                GL1_ISR_LOCK(gas_id);
                l1_tskisr_blk->current_params.L1_confirm_ID = L1_NULL_CNF;
                l1_multi_sim_bplmn_abort( 0, gas_id );
                GL1_ISR_UNLOCK(gas_id);

               /* Note that, we need to wait for 3 frames, so that the FW is set to proper state
                * and then only, can the next power scan proceed. This 3 frame wait only holds
                * till PLMN state is not set to NULL.
                */
                while ( (L1_MULTI_SIM_BGRND_HPLMN_NULL != gl1_ms_background_hplmn_state(gas_id)) && (suspend_cntr < (SUSPENSION_FRAME_DELAY+2) ) )
                {
                  (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
                  (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
                  suspend_cntr++;
                }
                l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH ;
                if ( L1_MULTI_SIM_BGRND_HPLMN_NULL != gl1_ms_background_hplmn_state(gas_id))
                {
                  MSG_ERROR("Background plmn state not NULL", 0, 0, 0);
                }
              }

            if( FALSE == L1_setup_PSCAN( msg_header ) )
            {
              if ( message_ptr->search_mode == RR_SEARCH_BACKGROUND )
              {
                 MSG_GERAN_MED_0_G("Power scan initiated in background");
                 /* Set up for background power scan
                  * l1_isr will kick off background pscan based on
                  * l1_idle_data.background_HPLMN_search_activity
                  */
                 GL1_ISR_LOCK(gas_id);
                 l1_idle_data->background_HPLMN_search_activity = POWER_SCAN;
                 l1_tskisr_blk->pscan_state = L1_SCAN_END;
                 GL1_ISR_UNLOCK(gas_id);			 
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
                 gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_BPLMN_SRCH_START);
#endif
              }
              else
              {
                 MSG_GERAN_MED_0_G("Power scan initiated in foreground");

                 L1_suspend_background_HPLMN_search(gas_id);
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
              L1_send_MPH_POWER_SCAN_CNF( FALSE, gas_id ); /* suspending boolean */
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
            gas_id_t gas_id;
            mph_change_page_mode_req_T *message_ptr;
            idle_data_T               *l1_idle_data;

            message_ptr = ( mph_change_page_mode_req_T *)msg_header; /*lint !e740 */
            gas_id = message_ptr->gas_id;
            l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];
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

            (void)l1_log_paging_data(gas_id);

            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_IDLE_UPDATE_REQ                   *
         *                                                      *
         ********************************************************/
         case MPH_IDLE_UPDATE_REQ:

         {
            gas_id_t gas_id;
            mph_idle_update_req_T     *message_ptr;
            idle_data_T               *l1_idle_data;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

            message_ptr = ( mph_idle_update_req_T *) msg_header; /*lint !e740 */

            gas_id = message_ptr->gas_id;
            l1_idle_data=&l1_idle_data_store[gas_id];
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];
			if(l1_tskisr_blk->main_command == L1_DECOUPLE_MULTI_SIM_COMMAND)
	    	{
               MSG_GERAN_MED_0_G("Main command is L1_DECOUPLE_MULTI_SIM_COMMAND, so not hanlding MPH_IDLE_UPDATE_REQ");
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
               message_ptr->layer1_sys_info.cbch_channel_information_valid, gas_id
               );

            l1_smscb_set_chan_type(
               message_ptr->layer1_sys_info.cbch_channel_information.channel_type, gas_id
               );

            l1_smscb_set_arfcn( gl1_get_serving_cell_arfcn(message_ptr->gas_id), gas_id);

            l1_smscb_set_bsic( gl1_get_serving_cell_BSIC(message_ptr->gas_id), gas_id );

            l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

            GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

            l1_sc_set_bcch_autonomy( FALSE, gas_id );

            /*copy BA list to SC engine*/
            L1_remove_oob_ba(&ba_buffer,&message_ptr->layer1_sys_info.BA_list);
            l1_sc_set_ba_list(&ba_buffer, 0xFF,message_ptr->gas_id);

            /* Set multiband reporting parameter */
            l1_sc_balist_set_multiband_reporting( message_ptr->layer1_sys_info.multiband_reporting, gas_id );

            /*tell surrounding cell engine gap between paging blocks*/
            l1_sc_set_paging_multiframes(l1_idle_data->campedon_cell_data.BS_PA_MFRMS, gas_id);

            (void)l1_log_paging_data( gas_id );

             gpl1_set_invert_sys_info_trm_priority(FALSE,gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
           /* Clear out any overrid reason */
           l1_clear_global_override_trm_reason(gas_id);
#endif
           l1_idle_if_gps_notify(gas_id);
           }

            break;
         }
        /********************************************************
        *                                                      *
        *                MPH_START_GPRS_IDLE_REQ               *
        *                                                      *
        ********************************************************/

        case MPH_START_GPRS_IDLE_MODE_REQ:
          {
            gas_id_t gas_id;
            mph_start_gprs_idle_mode_req_t *message_ptr;
            gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
            uint8 i;

            message_ptr = (mph_start_gprs_idle_mode_req_t *)msg_header;
            gas_id = check_gas_id(message_ptr->gas_id);

            gl1_handle_mph_start_gprs_idle_mode_req(message_ptr);
            gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

            /*  Initialise the tx power array to maximum allowable power  */
            for (i=0; i<8; i++)
            {
              gprs_serv_cell_meas_ptr->pch[i] = gprs_serv_cell_meas_ptr->pmax;
            }

            gprs_serv_cell_meas_ptr->quality_report_time_index = 0;

            /*  Initialise the running average structure  */
            /*  Initialize serving cell measurement averaging filter */
            gprs_serv_cell_meas_ptr->rla_p_count_idle = GPRS_SC_RLA_P_COUNT_IDLE;

            l1_running_avg_init(&gprs_serv_cell_meas_ptr->rla_p_buffer, 
                                gpl1_gprs_get_serv_cell_meas_buf(PRX_MEAS_BUFFER, gas_id),
#if defined(FEATURE_GSM_RX_DIVERSITY_IDLE) || defined (FEATURE_GSM_RX_DIVERSITY_DATA)
                                gpl1_gprs_get_serv_cell_meas_buf(DIVRX_MEAS_BUFFER, gas_id),
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA || FEATURE_GSM_RX_DIVERSITY_DATA*/
                                (L1_GPRS_SERV_CELL_MEAS_BUF_LEN - 1),
                                gas_id);

            /*  Set for Idle 52 mode measurements */
            l1_running_avg_set_avg_len(&gprs_serv_cell_meas_ptr->rla_p_buffer, GPRS_SERV_CELL_IDLE_52_AVG_LEN,gas_id);

            l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;
            l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

            l1_idle_if_gps_notify(gas_id);
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
           /* Clear out any overrid reason */
           l1_clear_global_override_trm_reason(gas_id);   

#endif		
          }
          break;

        /********************************************************
         *                                                      *
         *                MPH_EARLY_CAMPING_REQ                 *
         *                                                      *
         ********************************************************/
         case MPH_EARLY_CAMPING_REQ:
         {
            mph_early_camping_req_T* message_ptr = ( mph_early_camping_req_T* )msg_header;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
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
            gas_id_t gas_id;
            mph_decode_bcch_list_req_T  *message_ptr;
            idle_data_T               *l1_idle_data;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
            BCCH_List_T *l1_BCCH_List_ptr;

            /* Align pointers */
            message_ptr = ( mph_decode_bcch_list_req_T *)msg_header; /*lint !e740 */
            gas_id = message_ptr->gas_id;
            l1_idle_data=&l1_idle_data_store[gas_id];
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];

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
                     l1_idle_data->background_HPLMN_search_activity
                  */
                   l1_tskisr_blk->command = L1_NULL_COMMAND;
                  l1_idle_data->background_HPLMN_search_activity = ACQ_BCCH_LIST;
                  GL1_ISR_UNLOCK(gas_id);

                }
                else
                {
                   MSG_GERAN_MED_0_G("Decode BCCH list initiated in foreground");
                   /* Tell the SCE to stop its idle mode procedures */
                   l1_sc_stop(gas_id);

                   L1_suspend_background_HPLMN_search(gas_id);
                   l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
                   /* Foreground list decodes */
                   /* Abort idle mode and transition to FIND_BCCH */
                   GL1_ISR_LOCK(gas_id);
                   l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
                   l1_tskisr_blk->command = L1_ACQUIRE_BCCH_LIST_COMMAND;
                   L1_send_ISRTIM_ACQ_LIST_REQ( l1_BCCH_List_ptr->current_sch_index, gas_id );
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
            gas_id_t gas_id;
            mph_decode_bcch_list_abort_req_T* message_ptr = ( mph_decode_bcch_list_abort_req_T* )msg_header;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
            gas_id = message_ptr->gas_id;

            l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            GL1_ISR_LOCK(gas_id);
            /*
               Indicate that a decode bcch list cnf is to be sent.
               Only sent when aborted by a MPH_DECODE_BCCH_LIST_ABORT_REQ
            */

            l1_tskisr_blk->next_params.L1_confirm_ID = L1_DECODE_BCCH_LIST_CNF;

            L1_transfer_new_data(gas_id );
            l1_multi_sim_bplmn_abort(0,gas_id );
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
            gas_id_t gas_id;
            mph_power_scan_abort_req_T* message_ptr = ( mph_power_scan_abort_req_T* )msg_header;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

            gas_id = message_ptr->gas_id;

            l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            GL1_ISR_LOCK(gas_id);
            /*
               Indicate that a power scan cnf is to be sent.
               Only sent when aborted by a MPH_POWER_SCAN_ABORT_REQ
            */

            l1_tskisr_blk->next_params.L1_confirm_ID = L1_POWER_SCAN_CNF;

            L1_transfer_new_data(gas_id );
            l1_multi_sim_bplmn_abort(0,gas_id );
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
            L1_process_READ_SYS_INFO_REQ( (mph_read_sys_info_req_T *)msg_header ); /*lint !e740 */
            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_SELECT_SPECIFIC_BCCH_REQ          *
         *                                                      *
         ********************************************************/

         case MPH_SELECT_SPECIFIC_BCCH_REQ:
         {
            gas_id_t gas_id;
            mph_select_specific_bcch_req_T *message_ptr;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

            /* align pointers */
            message_ptr = (mph_select_specific_bcch_req_T *)msg_header; /*lint !e740 */
            gas_id = message_ptr->gas_id;
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            /* Tell the SCE to stop */
            l1_sc_stop(gas_id);

            GL1_ISR_LOCK(gas_id);
            l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
            L1_send_ISRTIM_ACQ_REQ(message_ptr->specific_channel_no, gas_id );
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

         /********************************************************
         *                                                      *
         *                MPH_IGNORE_WCDMA_NEIGHBORS_REQ        *
         *                                                      *
         ********************************************************/

         case MPH_IGNORE_WCDMA_NEIGHBORS_REQ:
         {
            mph_ignore_wcdma_neighbors_req_T* msg;
            msg = ( mph_ignore_wcdma_neighbors_req_T* )msg_header; /*lint !e740 */
            l1_sc_wcdma_ignore_neighbors( msg);
            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_WCDMA_CELL_UPDATE_LIST_REQ        *
         *                                                      *
         ********************************************************/

         case MPH_WCDMA_CELL_UPDATE_LIST_REQ:
         {
            gas_id_t gas_id;
            mph_wcdma_cell_update_list_req_T* msg;
            msg = ( mph_wcdma_cell_update_list_req_T* )msg_header; /*lint !e740 */

            gas_id = check_gas_id(msg->gas_id);

            l1_sc_set_wcdma_list_update_inprogress( TRUE , gas_id );
            gl1_drx_task_active(gas_id);
            gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);
            l1_sc_wcdma_update_list( msg, gas_id );
            gl1_drx_task_inactive(gas_id);
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
            gas_id_t gas_id;
            mph_ue_mode_change_req_T  *message_ptr;
            message_ptr = ( mph_ue_mode_change_req_T *) msg_header; /*lint !e740 */

            gas_id = message_ptr->gas_id;

            /* If this occurs during going to sleep or we are asleep
                     it can execute when mDSP is asleep and no NPA request
                     and no GSM clocks, delay until isr */
            /* wake up layer 1 isr */
            gl1_drx_require_and_wait_for_next_tick_conditional(gas_id);
#ifdef FEATURE_SGLTE
            /* Set SGLTE mode - only expected on gas_id 0*/
            gl1_hw_set_sglte_mode(message_ptr->sglte_mode_active, message_ptr->gas_id);
#endif
            l1_sc_wcdma_set_mode(message_ptr->gsm_only, gas_id );
            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_STOP_GSM_MODE_REQ                 *
         *                                                      *
         ********************************************************/

         case MPH_STOP_GSM_MODE_REQ:
         {
            gas_id_t gas_id;
             mph_stop_gsm_mode_req_T *message_ptr;
              message_ptr = (mph_stop_gsm_mode_req_T *)msg_header; /*lint !e740 unusual pointer cast */

            gas_id = message_ptr->gas_id;

            /* Tell the SCE to stop its idle mode procedures */
            l1_sc_stop(gas_id);

            /* Stop L1 ISR */
            L1_setup_DEACTIVATE( msg_header );

            /* Rude wakeup from SLEEP, so we respond faster */
            gl1_drx_require_next_tick(gas_id);

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
            gas_id_t gas_id;

            mph_suspend_gsm_mode_req_T *message_ptr = (mph_suspend_gsm_mode_req_T *)msg_header;
            gas_id = message_ptr->gas_id;

            /* Tell the SCE to stop its idle mode procedures */
            l1_sc_stop( gas_id );

            /* Stop L1 ISR */
            L1_setup_SUSPEND( gas_id );
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
             gas_id_t gas_id;
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
            
            if(message_ptr->command == RR_L1_LIST_SRCH_TIME_REQ_START)
            {
               gl1_drx_task_active(gas_id);
               l1_set_background_wcdma_plmn_scan_active( TRUE,gas_id );
               MSG_GERAN_HIGH_0_G("MPH_INTERRAT_LIST_SRCH_TIME_REQ - start");
 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
               gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_G2W_BPLMN_SRCH_START);
#endif
 
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
              l1_set_background_wcdma_plmn_scan_active( FALSE,gas_id );
              MSG_GERAN_HIGH_0_G("MPH_INTERRAT_LIST_SRCH_TIME_REQ - stop");
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
              gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_G2W_BPLMN_SRCH_END);
#endif

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

         case MPH_ABORT_RA_REQ:
         {
           mph_abort_ra_req_T  *message_ptr;
           gas_id_t gas_id;
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
           message_ptr = ( mph_abort_ra_req_T *)msg_header; /*lint !e740 unusual pointer cast */
           gas_id = check_gas_id(message_ptr->gas_id);

           l1_tskisr_blk = &l1_tsk_buffer[gas_id];

             MSG_GERAN_HIGH_3_G("MPH_ABORT_RA_REQ - Move to IDLE mode, m_cmd %d trm_st %d gas_id %d",
                                l1_tskisr_blk->main_command,
                                grm_get_trm_status(l1_tskisr_blk->client_id, gas_id),
                                gas_id);

             GL1_ISR_LOCK(gas_id);
             /* 
              * Stop any pending RA bursts and revoke READY_TO_SEND
              */
             l1_tskisr_blk->rach_ready_to_send = FALSE;

               re_calculate_initial_rach[gas_id] = TRUE;
             if ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_DECOUPLED )
             {
               switch (l1_tskisr_blk->l1_prev_state)
               {
                  case L1_MULTI_SIM_FIND_BCCH:
                    L1_send_MPH_ABORT_RA_CNF(gas_id, GL1_IN_NULL_MODE);
                    break;
                  case L1_MULTI_SIM_IDLE:
                    l1_tskisr_blk->main_command = L1_IDLE_COMMAND;
                    L1_send_MPH_ABORT_RA_CNF(gas_id, GL1_IN_DECOUPLED_MODE);
                    break;
                  default:
                    MSG_GERAN_HIGH_1_G("Unknown state %d", l1_tskisr_blk->l1_prev_state);
                    break;
               }
             } else
             {
             /* As RR may not send START_IDLE_REQ if lock is never granted and asked to abort */
             l1_tskisr_blk->main_command = L1_MULTI_SIM_IDLE_COMMAND;
               /* Let RR know not to wait for IDLE_CNF */
               L1_send_MPH_ABORT_RA_CNF(gas_id, GL1_IN_IDLE_MODE);
             }

             /* CR769740: To prevent FM retaining the TRM when abort the RA */
             if(GL1_TRM_RETAINED_FOR_ACCESS == grm_get_trm_status(l1_tskisr_blk->client_id, gas_id))
             {
                grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);
             }
             
             GL1_ISR_UNLOCK(gas_id);
             gl1_drx_require_and_wait_for_next_tick(gas_id);
             GL1_ISR_LOCK(gas_id);
             GL1_ISR_UNLOCK(gas_id);

           break;
         }

        /********************************************************
         *                                                      *
         *                MPH_DSC_THRESHOLD_IND                 *
         *                                                      *
         ********************************************************/
        case MPH_DSC_THRESHOLD_IND:
          L1_handle_MPH_DSC_THRESHOLD_IND( msg_header );
        break;

        case MPH_L1_DS_ABORT_IND:
        {
         gpl1_gprs_handle_ds_abort_ind((mph_l1_ds_abort_ind_T *)msg_header);
         break;
        }

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


   else if (msg_header->message_set == MS_L2_L1)
   {
     /********************************************************
      *                                                      *
      *                PH_RANDOM_ACCESS_REQ                  *
      *                                                      *
      ********************************************************/
     if (msg_header->message_id == PH_RANDOM_ACCESS_REQ)
     {
       MSG_GERAN_HIGH_0_G("Sending RACH CNF from IDLE mode");
       L1_send_PH_RANDOM_ACCESS_CNF_with_result((gas_id_t)gas_id, GERAN_ACCESS_IN_IDLE_STATE);
     }
     /********************************************************
      *                                                      *
      *                PH_RANDOM_ACCESS_IND                  *
      *                                                      *
      ********************************************************/
     else if (msg_header->message_id == PH_RANDOM_ACCESS_IND)
     {
       ph_random_access_ind_T *message_ptr = (ph_random_access_ind_T *)msg_header;
       volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
       idle_data_T               *l1_idle_data;
       gas_id = check_gas_id(message_ptr->gas_id);
       if (message_ptr->rach_for_ps_traffic)
       {
         MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for PS");
       } else
       {
         MSG_GERAN_HIGH_0_G("Layer 2 requests permission to RACH for CS");
       }
       l1_tskisr_blk = &l1_tsk_buffer[gas_id];
       l1_idle_data = &l1_idle_data_store[gas_id];
       GL1_ISR_LOCK(message_ptr->gas_id);
        /* mark the interface as READY_TO_SEND */
        l1_tskisr_blk->rach_ready_to_send = TRUE;
#ifdef FEATURE_QTA
       if (l1_tskisr_blk->qta_in_progress)
       {
         l1_idle_data->sc_stop_pending = TRUE;
         MSG_HIGH("Currently in QTA gap, do not stop SCE now", 0, 0, 0);
       } else
#endif
       {
         /* Tell the SCE to stop its idle mode procedures */
         l1_sc_stop(gas_id);
       }

       /* Wake up here, so that IDLE mode can successfully abort and move to the DECOUPLED state */
       gl1_drx_require_next_tick(gas_id);

       if (grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) == GL1_TRM_RETAINED_FOR_ACCESS)
       {
         /* this is to prevent frame manager from retaining the lock */
         grm_set_trm_status(GL1_TRM_GRANTED, l1_tskisr_blk->client_id, gas_id);
       }

       l1_tskisr_blk->main_command = L1_DECOUPLE_MULTI_SIM_COMMAND;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
       gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_RACH);
#endif	

       GL1_ISR_UNLOCK(gas_id);
     } else
     {
       MSG_GERAN_ERROR_1("L2->L1(IDLE) message %d ignored.", msg_header->message_id);
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
            gas_id_t           gas_id;
            Frequency_List_T  *l1_Frequency_List;
            isrtim_scan_cnf_T *message_ptr = (isrtim_scan_cnf_T *)msg_header;

            gas_id = message_ptr->gas_id;

            l1_Frequency_List = l1_Frequency_List_Store[gas_id];

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

            /* ... copy the results into RR's buffer ... */
            for (i=0; i<num_satisfy_threshold_criteria; i++)
            {
               l1_Frequency_List->results->meas[i].RXLEV_average =
                  L1_map_dBm_x16_to_rxlev(l1_Frequency_List->power_dBm_x16[l1_Frequency_List->sorted_array_indexes[i]]);

               l1_Frequency_List->results->meas[i].arfcn = l1_Frequency_List->ARFCN[l1_Frequency_List->sorted_array_indexes[i]];
            }

            l1_Frequency_List->results->num_frequencies = num_satisfy_threshold_criteria;

            /* ... and tell RR we're done. */
            L1_send_MPH_POWER_SCAN_CNF(FALSE,gas_id);
         }
         break;

         case  L1_ISR_SINGLE_BLK_FAILURE_CNF:
         {
           gas_id_t                         gas_id;
           t_l1_isr_single_blk_failure_cnf *message_ptr = (t_l1_isr_single_blk_failure_cnf *)msg_header;

           gas_id = message_ptr->gas_id;
           gpl1_gprs_send_mac_51_sb_failure( (void *) NULL, gas_id );

           /* No need to worry about freeing/rewinding this memory
            * because it is built in the gs_read_messages( )
            * function.
            */
           MSG_GERAN_MED_1_G(" L1_ISR_SINGLE_BLK_FAILURE_CNF rcvd fn %d",
                   GSTMR_GET_FN_GERAN(gas_id ));
         }
         break;

         default:
            MSG_GERAN_ERROR_1("RR->L1(IDLE) message %d ignored.", msg_header->message_id);
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

         l1_smscb_process_deactivate_info( message_ptr->channel_ind,
                                           geran_map_nas_id_to_gas_id( message_ptr->as_id ) );
      }
   }
   else
   {
      MSG_GERAN_ERROR_1_G( "L1(IDLE) - Unknown message set:%d",
                           msg_header->message_set );
   }

}

/*===========================================================================

FUNCTION  L1_handle_MPH_DSC_THRESHOLD_IND

DESCRIPTION
  This function handles downlink signalling counter threshold indications from RR. When the DSC drops below a certain %
  of max DSC then a signal is sent to L1. If the count rises above a certain % of max DSC then a signal is also sent.
  The downlink signalling count is indication of our receiver performance and GL1 will use the indications from
  RR to either diable Dual receive and fallback to tuneaway or request the diversity chain for the PCH receive.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_MPH_DSC_THRESHOLD_IND( IMH_T *msg_header )
{
  gas_id_t                 gas_id;
  grm_info_t              *grm_info_p;
  l1_serving_cell_meas_T *l1_serving_cell_meas_ptr;
  mph_dsc_threshold_ind_T *message_ptr     = (mph_dsc_threshold_ind_T *)msg_header;
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = NULL;;

  gas_id     = check_gas_id(message_ptr->gas_id);
  l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_serving_cell_meas_ptr = &l1_serving_cell_meas[gas_id];

  /* In Idle state, GL1 sends MPH_BLK_QUALITY_IND with channel type CCCH.
     RR echoes back same channel type in MPH_DSC_THRESHOLD_IND. */
  if ( CCCH != message_ptr->l2_channel_type ) 
  {
    MSG_GERAN_ERROR_1_G( "Invalid MPH_DSC_THRESHOLD_IND from RR channel %d", 
                         message_ptr->l2_channel_type );

    return;
  }

  l1_set_epd_pref(message_ptr->epd_pref , gas_id);

#ifdef   FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  /* make sure we return from here when PCHXFER is enabled*/
  if(l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
  {
    return;
  }
#endif


  grm_info_p = grm_get_info_ptr_from_client(l1_tskisr_blk->client_id, gas_id);

  if ( !grm_info_p )
  {
      MSG_GERAN_ERROR_0_G( GL1_DRDS_DBG_HDR"grm_info_p is NULL. Can't process MPH_DSC_THRESHOLD_IND" );

      return;
  }

  grm_info_p->dsc_threshold = message_ptr->percent_of_max;
  grm_info_p->rssi = L1_map_rxlev_to_dBm_x16(message_ptr->RXLEV_average);
  
  GL1_ISR_SAVE_LOCK(gas_id);
  grm_info_p->Scell_SNR = l1_serving_cell_meas_ptr->snr;
  GL1_ISR_SAVE_UNLOCK(gas_id);

  MSG_GERAN_HIGH_4_G( GL1_DRDS_DBG_HDR"MPH_DSC_THRESHOLD_IND dsc %d rssi %d snr %d DR enabled %d",
                                     grm_info_p->dsc_threshold,
                                     grm_info_p->rssi,
                                     grm_info_p->Scell_SNR,
                                     grm_info_p->dr_enabled);

  /* Only valid if UE configured as DR-DSDS capable */
  if ( !grm_device_config_is_drdsds() )
  {
    return;
  }

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  /* Trigger Hybrid Tuneaway State machine*/
  gl1_drdsds_hybrid_tuneaway_trigger(DRDSDS_MPH_DSC_THRESHOLD_IND_EVENT, 
                                     NULL, 
                                     l1_tskisr_blk->client_id,
                                     gas_id); 

  /*update the good serving cell indicator*/
  grm_persistent_denial_monitoring(GRM_GOOD_SERVING_CELL_CHECK,
                                   l1_tskisr_blk->client_id,
                                   GRM_NULL_COMMAND,
                                   gas_id);

  /*if this indication is for PCH decode then tick the persistent denial algorithm*/
  (void)grm_persistent_denial_monitoring(GRM_DECREMENT_DENIED_REG_COUNT,
                                        l1_tskisr_blk->client_id, 
                                        GRM_NULL_COMMAND, 
                                        gas_id);


#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/   

}

/*===========================================================================

FUNCTION  L1_enable_dual_receive

DESCRIPTION
  This function enables/disables DR.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_enable_dual_receive(grm_client_enum_t client_id, gas_id_t gas_id )
{
  grm_info_t *grm_info_p = grm_get_info_ptr_from_client(client_id, gas_id);

  if ( !grm_info_p )
  {
    MSG_GERAN_ERROR_0_G( "Null grm_info_p - did not enable DR mode" );

    return;
  }

  /* Only valid if UE configured as DR-DSDS capable */
  if ( grm_device_config_is_drdsds() )
  {
    grm_info_p->dr_enabled = grm_enable_disable_dual_receive( TRUE, client_id, gas_id );

    /* On early camping or start gsm mode, based upon dr enabled status
       set hybrid tuneaway state machine*/
    if ( grm_info_p->dr_enabled ) 
    {
      grm_info_p->rxd_priority = DRDSDS_RXD_LP;
    }
    else
    {
      grm_info_p->rxd_priority = DRDSDS_RXD_NULL;
    }

    gl1_drdsds_set_hybrid_tuneaway_state(client_id, gas_id); 

    MSG_GERAN_HIGH_2_G( "Setting DR enabled mode to:%d rxd_priority:%d",
                        grm_info_p->dr_enabled, grm_info_p->rxd_priority );
  }
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/* EOF */
