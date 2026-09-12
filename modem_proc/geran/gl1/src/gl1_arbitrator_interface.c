/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                    G L 1  A R B I T R A T O R  I N T E R F A C E

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_arbitrator_interface.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
26/12/17   sn        CR2160741: Gl1 Coex timeline updation at Wakeup Cancel of Idle Sub
23/06/17   mn        CR2055160 Fix for regression caused by CR2009497(PDTCH last burst over PCH first burst)
11/07/17   sn        CR2137112 Correct USTMR_REF_START calculation of IDLE SUB first burst after wakeup 
31/07/17   mn        CR2077649 Fix for PDTCH last burst is yeilding over PCH first burst by using last appended USTMR start time
06/11/15   ab        CR927639 : CXM In X2G QDTA use desense_band_id as freq id to avoid X blanking.06/11/15   ab        CR935174 : Perform G IDLE Freq after all RX done to avoid X desense the G PCH RX.11/11/15   smd       CR929829 Check the correct flag for BPLMN active
06/11/15   ab        CR935174 : Perform G IDLE Freq after all RX done to avoid X desense the G PCH RX.
13/10/15   shr       CR916684 GSM power monitors should use Tier_10 priority to allow TX power backoff from X tech
18/09/15   cws       CR907895 Change unnecessary F3 to LOW 
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
25/08/15   ab        CR896178 DR-DSDS:X+G COEX monitor restriction of 2 per frame should not be enforced
                     for non-DR compatible PSCAN
24/08/15   mc        CR874662 : CXM Freq ID support for X2G startup and X2G preload
29/08/15   ab        CR893900 : GL1 COEX : Band Registration not done during IDLE+BPLMN stage, may lead to Monitor Blanking.
15/07/15   mc        CR861999 Failure to abort TCH mode when G+G COEX manager denies permission to FACCH Tx
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
09/07/15   cah       CR811835 :  FR 26893: Band-by-Band OOS and BPLMN Search - GL1
30/06/15   ab        CR869795 : SR-DSDS : Perform GBTA with Transfer SUB FCCH/SCH using COEX.
17/06/15   mc        CR855818 : COEX:MCS_CXM_WWAN_TECH_STATE_IND registrations for X2G IRAT need to indicate CXM_LNK_TYPE_IRAT
26/05/15   ab        CR841593 : GL1 COEX - G+G DSDS - RRBP_HI is YIELED when PCH RX collides with 3 TX slots
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
27/05/15   mc        CR.840262 : COEX handling for Dual-Multimode DR-DSDS
14/05/15   mc        CR.821519 : TH 2.0: WLAN COEX timeline reporting errors for FACCH and SDCCH
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
17/03/15   ab        GL1 COEX : CR804948 Handling of multiple RRBP during conflict resolution
07/04/15   shr       CR808932: COEX support for DTM
31/03/15   pa        CR815621:GRM restructuring for TH 2.0.
30/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
17/02/15   cws       CR796593 Set set_all_chan_type_hi to TRUE for Tx with high tier
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
27/02/15   am        CR646584 WLAN coex
25/02/14   cah       CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good serving cell conditions
23/02/15   mc        CR759541 Enable COEX managment for X2G IRAT activities in DR-DSDA
18/12/14   cgc       CR766576 FR23109 Adaptive Power monitor during CS in DSDA G + TDD ( TDS/LTE TDD)
23/01/15   npt       CR697236 Mainline FEATURE_GERAN_VSTMR
23/12/14   shr       CR747635   :Unification of micro priority tiers for GERAN
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
10/12/14   ws        CR768376 Q6 compile warning fixes
03/12/14   npt       CR759467 : Fix compiler warnings
26/09/14   mc        CR719955 : COEX : X+G[IDLE] DSDA - IDLE monitor not protected COEX with TIER_10 assignment
17/09/14   mc        CR718772: G[TCH+G[TCH] DSDA : VBATT Tx power backoff not seen on some frames
26/08/14   pa        CR715814 IFS measured client id is not updated from GL1 before passing to TRM
08/05/14   ws        CR 661576 GL1 Support for sending MCS_CXM_BAND_AVOID_PWR_IND
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
28/07/14   pjr       CR700772 Set monitor tier to ACTIVITY_TIER_10 in XPG mode when G in PS
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
03/07/14   og        CR652720. L2G DSDA bring-up updates.
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
18/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
21/05/14   ssh       CR668643 : gl1_hw_qta_gap_active() should return FALSE for multimode sub
12/05/14   mc        CR662627 : Remove usage of enhanced_fcch
16/04/14   og        CR650341. Raise the priority of the combined acquisition in FIND BCCH mode.
09/04/14   npt       CR.645866 : Use VSTMR API's instead of USTMR register reads for GARB calculations
21/03/14   mc        CR.588853 : COEX : GL1 changes for Enhanced FCCH algorithm
19/03/14   mc        CR.606503 : COEX : De-register Freq IDs from MCS when GAS de-registers from GL1 arbitrator
13/03/14   mc        CR.602368 : COEX : Support for Frequency Hopping over large ARFCN lists
05/03/14   mc        CR.583282 : Limit the number of PSCAN monitors per frame to that which can be COEX protected in X+G DSDA
27/01/14   pjr       CR.579214 : COEX : Prioritise PDTCH over TCH during 2 phase access
14/02/14   mc        CR.572523 : COEX : Ensure all DL bands are registered with CXM on transition to FIND_BCCH
14/01/14   og        CR589868. T+G DSDA support.
12/02/14   am        CR611265 RACH timing update to GARB
20/01/14   ab        CR582408: DSDA - Update Rx Power in IDLE and Do not Update Power when RX OFF
14/01/14   ws        CR599957 Fix compile warnings in garb_intf_arbitrate_acq() and
                     garb_intf_arbitrate_async_rx()
03/12/13   ab        CR582408: DSDA - Update Rx Power in IDLE and Do not Update Power when RX OFF
23/12/13   sp        CR546780: Backout CR/491138
26/12/13   dv        CR593657:Correct priority for Ncell BCCH in QTA gap
03/12/13   sk        CR584247 Ensure correct sys mode passed to GL1 Arbitrator
01/12/13   sk        CR583361 Correcting the multisim sys mode check
02/10/13   pjr       CR552448 DDR Frequency plan implementation - l1_arbitration modification
09/11/13   mc        CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
09/11/13   mc        CR556742 G+G COEX : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
07/10/13   ab        CR 554820 DSDA GL1 - Update Tx power as per slot transmission for RF COEX
07/11/13   js        CR 563464 : NCELL acquisition are being prioritized over SACCH TXs
11/10/13   pjr       CR.555300 : Changes to correctly register freqid during monitors  and acq
15/11/13   ap        CR575548 FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS
05/11/13   js        CR.555528 : COEX : CXM Priority and Freq ID for Async Rx
11/14/13   ab        CR 554820 DSDA GL1 - Update Tx power as per slot transmission for RF COEX
04/10/13   ab        CR553692: DSDA Register Freq with CXM during Single block Packet Access
10/10/13   ab        CR 554808 DSDA Geran is booking all the 4 tx slots with tier 1 when W is in connected mode
01/10/13   sk        CR552259 CR sync from 1.0/1.1
23/09/13   sk        2.0 syncup from 1.1
23/08/13   sk        CR533467 Partial QBTA bringup on 2.0 in DSDS
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
14/06/13   cs        TSTS Updates
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GSM_COEX_SW_CXM

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "gl1_arbitrator_types.h"
#include "gl1_arbitrator_interface.h"
#include "gl1_arbitrator_utils.h"
#include "gl1_arbitrator_tables.h"
#include "gl1_arbitrator_detect.h"
#include "gl1_arbitrator_cxm.h"
#include "gl1_hw_arbitration.h"
#include "l1_sc_int.h"
#include "l1_sc.h"
#include "l1_utils.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1_fm.h"

#include "msm.h"
#include <stringl/stringl.h>
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
#include "gpl1_grm_intf.h"
#include "l1_task.h"
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

#include "vstmr_geran.h"
#include "vtmrs_g.h"


/*===========================================================================

                            LOCAL DEFINES

===========================================================================*/
garb_arbitrated_gas_t arbitrated_gas[NUM_GERAN_DATA_SPACES];

extern transfer_data_T   l1_transfer_data[NUM_GERAN_DATA_SPACES];
/*===========================================================================

                     PROTOTYPES FOR LOCAL FUNCTIONS

===========================================================================*/
#ifdef FEATURE_GSM_COEX
static void    garb_intf_arbitrate_rx(gas_id_t gas_id);
static void    garb_intf_arbitrate_tx(gas_id_t gas_id);
static void    garb_intf_arbitrate_monitors(gas_id_t gas_id);
static void    garb_intf_arbitrate_acq(gas_id_t gas_id);
static void    garb_intf_arbitrate_sch(gas_id_t gas_id);
static void    garb_intf_arbitrate_async_rx(gas_id_t gas_id);
static void    garb_intf_arbitrate_x2g(gas_id_t gas_id);
#endif /* FEATURE_GSM_COEX*/
static void    garb_intf_notify(gl1_hw_arbitrated_event_t event, gas_id_t gas_id);
static boolean garb_intf_request_block_tx(gl1_msg_chan_type channel_type, gas_id_t gas_id);
static uint8   garb_intf_get_arfcns_to_register(frequency_list_T *frequency_list, gas_id_t gas_id);

/*===========================================================================

FUNCTION      garb_intf_register

DESCRIPTION   This function is called by a GAS to register for arbitration

RETURN VALUE  None

===========================================================================*/
void garb_intf_register(gl1_hw_arbitration_t* gl1_hw_arbitration, sys_modem_device_mode_e_type mode, boolean irat, gas_id_t gas_id)
{
   MSG_GERAN_HIGH_0_G("GARB : REGISTER");

   /* Initialise arbitrated GAS data */
   arbitrated_gas[gas_id].gas_id           = gas_id;
   arbitrated_gas[gas_id].arbitrated_frame = gl1_hw_arbitration;
   arbitrated_gas[gas_id].state            = GARB_NULL;
   arbitrated_gas[gas_id].multi_sim_mode   = garb_get_multi_rat_mode(gas_id);
   arbitrated_gas[gas_id].irat             = irat;

#ifdef FEATURE_GSM_COEX
   arbitrated_gas[gas_id].read_t_index  = 0;
   arbitrated_gas[gas_id].write_t_index = 0;
#endif

   gl1_hw_arbitration->notify_event = garb_intf_notify;

   /* Initialise CXM data and register the priority table callback with the CXM */
   arbitrated_gas[gas_id].cxm_time_ind.indication = CXM_LNK_DRCTN_INVLD;

   memset(&arbitrated_gas[gas_id].ul_arfcn_table, 0, sizeof(garb_arfcn_table_t));
   memset(&arbitrated_gas[gas_id].dl_arfcn_table, 0, sizeof(garb_arfcn_table_t));

   memset(&arbitrated_gas[gas_id].cxm_activity_table, 0, sizeof(cxm_activity_table_s[MAX_ACTIVITY_TIERS]));
   memset(&arbitrated_gas[gas_id].cxm_activity_table, 0, sizeof(cxm_activity_table_s[MAX_ACTIVITY_TIERS]));

   /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all bands */
   garb_cxm_arfcn_indication(NULL, 0, CXM_LNK_DRCTN_INVLD, TRUE, gas_id);

   /* Register the priority table callback with CXM */
   garb_cxm_register_priority_table_callback(gas_id);
}

/*===========================================================================

FUNCTION      garb_intf_unregister

DESCRIPTION   This function is called by an arbitrated GAS to unregister from
              arbitration.

RETURN VALUE  None

===========================================================================*/
void garb_intf_unregister(gas_id_t gas_id)
{
   MSG_GERAN_HIGH_1_G("GARB : FN=%d UNREGISTER", GSTMR_GET_FN_GERAN(gas_id));

   arbitrated_gas[gas_id].gas_id = GERAN_ACCESS_STRATUM_ID_UNDEFINED;
   arbitrated_gas[gas_id].state  = GARB_NULL;

   memset(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], 0, sizeof(garb_frame_t) );
   memset(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_NEXT_FRAME], 0, sizeof(garb_frame_t) );

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
   memset(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIRD_FRAME],  0, sizeof(garb_frame_t) );
   memset(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_FOURTH_FRAME], 0, sizeof(garb_frame_t) );
#endif /* #ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING */

   /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to deregister all Freq IDs */
   garb_cxm_arfcn_indication(NULL, 0, CXM_LNK_DRCTN_INVLD, FALSE, gas_id);

   garb_cxm_deregister_priority_table_callback(gas_id);

#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif
}

/*===========================================================================

FUNCTION      garb_intf_notify_gstmr_tick

DESCRIPTION   This function is called by GL1 prior to ISR processing. This
              takes a USTMR reference for the on air start time of the
              frame.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_gstmr_tick(gas_id_t gas_id)
{
#ifdef FEATURE_GARB_QTF
   uint32 offset = arbitrated_gas[gas_id].qtf_ustmr_offset;
#else
   uint32 offset = 0;
#endif /* FEATURE_GARB_QTF */
   uint32  ota_fn_start_ustmr;

   uint32 ustmr_start;
   uint32 ustmr_end ; 
   
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   uint32 ustmr_ota_sw_fn = gstmr_get_ota_ustmr_start_time_sw_fn( 1, gas_id );
   
   uint32 fn = GSTMR_GET_FN_GERAN(gas_id);

   MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT:Ota_ustmr %d,fn %d",ustmr_ota_sw_fn,fn);
   
   ustmr_start = USTMR_ADD(ustmr_ota_sw_fn, offset);
   ustmr_end   = USTMR_ADD(ustmr_start, USTMR_PER_FRAME );

   
   arbitrated_gas[gas_id].multi_sim_mode = garb_get_multi_rat_mode(gas_id);

   memset( &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], 0, sizeof(garb_frame_t) );

   arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn            = fn;
   arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].FnMod26       = fn % 26;

   if((arbitrated_gas[gas_id].state == GARB_IDLE) && 
      (OTHER_GSM_CLIENT_IN_TRANSFER(gas_id)) && 
      (l1_tskisr_blk->l1_isr_called_from_sleep_warmup_isr == TRUE))
     {
    
      uint8 read_index = arbitrated_gas[gas_id].read_t_index;
      boolean fn_found = FALSE;
  
      /* Iterate through the timeline of the own GAS and look for OTA Fn value.Since OTA FN starting time is updated before going to sleep 
      update that starting time with OTA starting time */
      for(int i=0; ( i < arbitrated_gas[gas_id].timeline_num_entries[read_index]) ; i++)
      {
        if(arbitrated_gas[gas_id].timeline[read_index][i].fn == (ADD_FN(fn,1))) //Gl1 will wake up 1 frame earlier to The OTA FN hence 1 FN is added to Current FN
         { 
             l1_fm_priority_T next_activity = l1_fm_get_next_activity(&arbitrated_gas[gas_id].params_idle.next_activity, gas_id);
             ota_fn_start_ustmr = arbitrated_gas[gas_id].timeline[read_index][i].s_time;
             fn_found = TRUE;
              /*S_TIME is derived from UstmrRefStart by subtracting RF SETUP time and some other gaurd offsets.So, UstmrRefStart should be later than S_TIME.
               If not then Gl1 has OTA FN-1 USTMR time at the wakeup. In that case add USTMR_PER_FRAME to the Wake up USTMR time to get correct UstmrRefStart*/
             if(next_activity == FM_PRI_CCCH)
             {
                if(!(USTMR_IS_LATER(ustmr_ota_sw_fn,ota_fn_start_ustmr)))
                {
                    ota_fn_start_ustmr = USTMR_ADD(ustmr_ota_sw_fn, USTMR_PER_FRAME );
                }
                else
                {
                    ota_fn_start_ustmr= ustmr_ota_sw_fn;
                }
             }

             MSG_GERAN_HIGH_3_G("GARB_OPT:ustmr_start_wakeup %d,ustmr_start_actual %d next_act = %d",ustmr_ota_sw_fn,ota_fn_start_ustmr,next_activity);
         }
      }

      if(fn_found ==TRUE)
      {
           arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart = ota_fn_start_ustmr;
           arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefEnd   = USTMR_ADD(ota_fn_start_ustmr, USTMR_PER_FRAME );
      }
      else
      {
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart = ustmr_start;
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefEnd   = ustmr_end;
    
      }
    }
   else
    {
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart = ustmr_start;
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefEnd   = ustmr_end;
    }

   MSG_GERAN_DEBUG_OPT_3_G("GARB_OPT: FN=%d : This frame has an on air start time of  USTMR=%d ustmr_end=%d", fn, arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart,arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefEnd);
    
#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_intf_coex_update_active_policy( gas_id );
#endif /* FEATURE_WLAN_COEX_SW_CXM */

}

/*===========================================================================

FUNCTION      garb_intf_notify

DESCRIPTION   This function is called from an arbitrated GAS to notify that
              activities are ready for arbitration.

RETURN VALUE  None

===========================================================================*/
static void garb_intf_notify(gl1_hw_arbitrated_event_t event, gas_id_t gas_id)
{
#ifdef FEATURE_GSM_COEX
   switch(event)
   {
   case ARBITRATE_RX:
      garb_intf_arbitrate_rx(gas_id);
      break;
   case ARBITRATE_TX:
      garb_intf_arbitrate_tx(gas_id);
      break;
   case ARBITRATE_MONITORS:
      garb_intf_arbitrate_monitors(gas_id);
      break;
   case ARBITRATE_ACQ:
      garb_intf_arbitrate_acq(gas_id);
      break;
   case ARBITRATE_SCH:
      garb_intf_arbitrate_sch(gas_id);
      break;
   case ARBITRATE_ASYNCRX:
      garb_intf_arbitrate_async_rx(gas_id);
      break;
   case ARBITRATE_X2G:
      garb_intf_arbitrate_x2g(gas_id);
      break;
   default:
      MSG_GERAN_ERROR_1_G("GARB : Unknown event %d", event);
      break;
   }
#endif /* FEATURE_GSM_COEX */
}

#ifdef FEATURE_GSM_COEX
/*===========================================================================

FUNCTION      garb_intf_arbitrate_rx

DESCRIPTION   This function is called to arbitrate RX slots programmed up
              in this frame.

RETURN VALUE  None

===========================================================================*/
static void garb_intf_arbitrate_rx(gas_id_t gas_id)
{
   int i;

   gl1_hw_arbitrated_rx_t* gmsg_frame = &arbitrated_gas[gas_id].arbitrated_frame->rx;
   garb_rx_frame_t* garb_frame = &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx;

   garb_frame->configured   = TRUE;
   garb_frame->cxm_freqid   = garb_cxm_get_freqid(gmsg_frame->arfcn, GARB_RX, gas_id);
   garb_frame->search_width = USTMR_FROM_QS( (gmsg_frame->search_width*4) / 2);

   /* Iterate through the slots and calculate the start and end time of any that are configured */
   for(i=0;i<GL1_DEFS_SLOTS_IN_FRAME;i++)
   {
      gl1_hw_arbitrated_rx_slot_t* gmsg_slot = &gmsg_frame->slots[i];
      garb_slot_t* garb_slot = &garb_frame->slots[i];

      if(gmsg_slot->configured==FALSE) continue;

      garb_slot->configured    = TRUE;
      garb_slot->arfcn         = gmsg_frame->arfcn;
      garb_slot->channel_type  = garb_get_channel_type(gmsg_slot->channel_type, GARB_RX, gas_id);
      garb_slot->s_time        = garb_get_slot_s_time(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], GARB_RX, i, gas_id);
      garb_slot->e_time        = garb_get_slot_e_time(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], GARB_RX, i, gas_id);
      garb_slot->burst_num     = gmsg_slot->burst_num;
      garb_slot->cxm_tier      = garb_get_cxm_priority_tier(GARB_RX, garb_slot->channel_type, i, gas_id);;

      garb_detect_and_resolve_rx_conflicts(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], i, gas_id);

     *gmsg_slot->desense_id    = garb_frame->cxm_freqid;
     *gmsg_slot->priority      = arbitrated_gas[gas_id].cxm_activity_table[garb_slot->cxm_tier].priority;

      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.num_slots++;
   }

   MSG_GERAN_DEBUG_OPT_1_G("GARB_OPT: ARBITRATE RX : %d slots", garb_frame->num_slots);

}

/*===========================================================================

FUNCTION      garb_intf_arbitrate_tx

DESCRIPTION   This function is called to arbitrate TX slots programmed up
              in this frame.

RETURN VALUE  None

===========================================================================*/
static void garb_intf_arbitrate_tx(gas_id_t gas_id)
{
   int i;

   gl1_hw_arbitrated_tx_t* gmsg_frame = &arbitrated_gas[gas_id].arbitrated_frame->tx;
   garb_tx_frame_t* garb_frame = &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx;
   boolean          set_all_chan_type_hi = FALSE;
   uint8            max_ts_high_cxm_tier = 0;

   garb_frame->configured     = TRUE;
   garb_frame->cxm_freqid     = garb_cxm_get_freqid(gmsg_frame->arfcn, GARB_TX, gas_id);
   garb_frame->timing_advance = USTMR_FROM_QS(gmsg_frame->timing_advance*4);
   garb_frame->power          = L1_convert_power_idx_to_dBm(gmsg_frame->arfcn.band, gmsg_frame->power);

   /* Iterate through the slots and calculate the start and end time of any that are configured */
   for(i=0;i<GL1_DEFS_SLOTS_IN_FRAME;i++)
   {
      gl1_hw_arbitrated_tx_slot_t* gmsg_slot = &gmsg_frame->slots[i];
      garb_slot_t* garb_slot = &garb_frame->slots[i];

      if(gmsg_slot->configured==FALSE) continue;

      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.num_slots++;
      garb_slot->configured    = TRUE;
      garb_slot->arfcn         = gmsg_frame->arfcn;
      garb_slot->channel_type  = garb_get_channel_type(gmsg_slot->channel_type, GARB_TX, gas_id);
      garb_slot->s_time        = garb_get_slot_s_time(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], GARB_TX, i, gas_id);
      garb_slot->e_time        = garb_get_slot_e_time(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], GARB_TX, i, gas_id);
      garb_slot->burst_num     = gmsg_slot->burst_num;
      garb_slot->cxm_tier      = garb_get_cxm_priority_tier(GARB_TX, garb_slot->channel_type, i, gas_id);
      garb_slot->priority      = gmsg_slot->pdu_prio;

      MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT :  ARBITRATE TX : garb_slot->pri %d , gmsg_slot->pri %d", garb_slot->priority,*gmsg_slot->priority);

      garb_detect_and_resolve_tx_conflicts(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], i, gas_id);

      if (  ((arbitrated_gas[gas_id].multi_sim_mode == GARB_MODE_GPG_DSDA) ||
             (arbitrated_gas[gas_id].multi_sim_mode == GARB_MODE_GPG_DSDS))  &&
             ( garb_slot->channel_type == GARB_CHAN_ULDL_PDTCH  ||
              garb_slot->channel_type == GARB_CHAN_UL_RRBP_HI ||
              garb_slot->channel_type == GARB_CHAN_UL_RRBP_LO)
         )
      {
        if (garb_slot->cxm_tier == ACTIVITY_TIER_GPG_HI_PRIO || garb_slot->cxm_tier == ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA)
        {
          set_all_chan_type_hi = TRUE;
          max_ts_high_cxm_tier = i;
        }
      }

      /* X + G case set cxm prio appropriately */
      /* *********First 2 slots always get CXM_HIGH , there is no conflict resolution **********/
      if ( (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDS) &&
           (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDA)
#ifdef FEATURE_TRIPLE_SIM
           &&(arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_TSTS)
#endif
         )
      {
        if (arbitrated_gas[gas_id].params_ps.num_ul_slots  > 2)  /* upto 2 slots- allow min 2 tx slots */
        {
          if( ((garb_frame->num_slots == 3 ||
               garb_frame->num_slots == 4)) &&   /* make cxm prio unchange for 1,2 , set high  for 3,4  as per channel type*/
              ((garb_slot->channel_type == GARB_CHAN_UL_RRBP_HI) ||
               (garb_slot->channel_type == GARB_CHAN_UL_RRBP_LO) || 
               (garb_slot->priority == GPDU_HIGH_PRIORITY) ) )
          {
            garb_slot->cxm_tier =  ACTIVITY_TIER_GPG_HI_PRIO;
            set_all_chan_type_hi = TRUE;  // xpg dsda
            max_ts_high_cxm_tier      = i;
          }
        }
      }

      *gmsg_slot->desense_id    = garb_frame->cxm_freqid;
      *gmsg_slot->priority      = arbitrated_gas[gas_id].cxm_activity_table[garb_slot->cxm_tier].priority;
      *gmsg_slot->backoff_vbatt = garb_slot->backoff_vbatt;
   }

   if (set_all_chan_type_hi)
   {
     for(i=0;i<GL1_DEFS_SLOTS_IN_FRAME;i++)  // for cases LHLH -> HHHH
     {
       gl1_hw_arbitrated_tx_slot_t* gmsg_slot = &gmsg_frame->slots[i];
       garb_slot_t* garb_slot = &garb_frame->slots[i];

       if(gmsg_slot->configured==FALSE) continue;

       /* FW cannot tune 2 times and hence only until actual T_10 make the cxm prio high */
       if( (garb_slot->cxm_tier == ACTIVITY_TIER_GPG_HI_PRIO ||
            garb_slot->cxm_tier == ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA) &&
            
           ( (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDS) &&
             (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDA) &&
             (max_ts_high_cxm_tier == i) )
         )
       {
         break;
       }
       else
       {
         if( (arbitrated_gas[GERAN_ACCESS_STRATUM_ID_1].state == GARB_PS) && (arbitrated_gas[GERAN_ACCESS_STRATUM_ID_2].state == GARB_PS) )
         {
           garb_slot->cxm_tier       = ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA;
         }
         else
         {
           garb_slot->cxm_tier       = ACTIVITY_TIER_GPG_HI_PRIO;
         }
         *gmsg_slot->priority       = arbitrated_gas[gas_id].cxm_activity_table[garb_slot->cxm_tier].priority;
         /* This allows this_frame to have contigous timeline entries */
         garb_frame->slots[i].channel_type = GARB_CHAN_UL_RRBP_HI;
         MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT: ARBITRATE TX More than one TX SLOT ts %d tier(T_20 4, T10 2) %d",i,garb_slot->cxm_tier);
       }
     }
   }

   MSG_GERAN_DEBUG_OPT_1_G("GARB_OPT : ARBITRATE TX : %d slots", garb_frame->num_slots);
}


/*===========================================================================

FUNCTION      garb_intf_arbitrate_monitors

DESCRIPTION   This function is called to arbitrate monitors programmed up
              in this frame.

RETURN VALUE  None

===========================================================================*/
static void garb_intf_arbitrate_monitors(gas_id_t gas_id)
{
   gl1_hw_arbitrated_monitors_t* gmsg_monitors = &arbitrated_gas[gas_id].arbitrated_frame->monitors;
   cxm_activity_type tier = ACTIVITY_TIER_20;
   uint32             i;
   uint32            nozone;
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
   boolean skip_required = FALSE;
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   
#endif /* FEATURE_DUAL_SIM / FEATURE_TRIPLE_SIM */

#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
   uint32            fn   = ADD_FN(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn, 1);
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS */

   /* Initialise COEX params to default values */
   gmsg_monitors->max_per_frame = 7;
   gmsg_monitors->nozone_count  = 0;

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
   if (gl1_drdsds_enabled(gas_id))
   {
      switch (arbitrated_gas[gas_id].state)
      {
         case GARB_IDLE:
         case GARB_2P_ACCESS:
         case GARB_PS:
         case GARB_CS_PS:
             /*make sure skip feature is not active for BPLMN power scan which uses TRM_ACQUISITION*/             
             
             if((  gpl1_get_multi_sim_bplmn_state(gas_id) == L1_MULTI_SIM_BGRND_HPLMN_RUNNING ) &&
                GRM_ACQUISITION_REASON(grm_gl1_get_current_trm_reason(gl1_convert_gas_id_to_bplmn_client(gas_id), gas_id))
                )
             {
               skip_required = FALSE;
               MSG_GERAN_HIGH_0_G("dg_ch: GARB : ARBITRATE MONS Skip flag FALSE BPLMN Pscan Active!!");
             }
             else
             {
               skip_required = TRUE;
             }

            break;
         default:
            break;
      }
   }
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/

   /* Frequency ID and Priority */
   for(i=0; i < gmsg_monitors->count; i++)
   {
      gmsg_monitors->monitor[i].skip = FALSE;
      switch(arbitrated_gas[gas_id].multi_sim_mode)
      {
      /************************
      * G PLUS G DSDS
      ************************/
      case GARB_MODE_GPG_DSDS:
         tier = ACTIVITY_TIER_20;
         break;
      /************************
      * G PLUS G DSDA
      ************************/
      case GARB_MODE_GPG_DSDA:
         /* For G+G DSDA monitors are re-positioned to avoid Tx on other RAT - do this on the final iteration */
         if( (i == (gmsg_monitors->count-1) ) && garb_detect_and_resolve_monitor_conflicts(gas_id))
         {
            for(nozone=0;nozone<MAX_MONITOR_NOZONES;nozone++)
            {
               if(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone].configured==FALSE) { continue; }

               gmsg_monitors->nozone_start[nozone] = QS_FROM_USTMR(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone].s_time);
               gmsg_monitors->nozone_end[nozone]   = QS_FROM_USTMR(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone].e_time);
               gmsg_monitors->nozone_count++;
            }
         }
         tier = ACTIVITY_TIER_20;
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
         gmsg_monitors->monitor[i].skip = (skip_required && !grm_is_band_registered(gmsg_monitors->monitor[i].arfcn.band, l1_tskisr_blk->client_id, gas_id));
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/
         break;
      /************************
      * X PLUS G DSDS
      ************************/
      case GARB_MODE_XPG_DSDS:
         {
            tier = ACTIVITY_TIER_10;
         }
         break;
      /************************
      * X PLUS G DSDA
      * C PLUS G DSDA
      * W PLUS G DSDA
      * T PLUS G DSDA
      * L PLUS G DSDA
      ************************/
      case GARB_MODE_XPG_DSDA:
      case GARB_MODE_CPG_DSDA:
      case GARB_MODE_WPG_DSDA:
      case GARB_MODE_TPG_DSDA:
      case GARB_MODE_LPG_DSDA:
         switch(arbitrated_gas[gas_id].state)
         {
#ifdef FEATURE_GSM_COEX_REDUCED_MEAS
         case GARB_FIND_BCCH:
            /* Number of monitors must be restricted in PSCAN if we are a victim in X+G DSDA */
            if ( (arbitrated_gas[gas_id].gas_is_coex_victim) &&
                 (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE) )
            {
              gmsg_monitors->max_per_frame = 2;
            }
            tier = ACTIVITY_TIER_10;
            break;
         case GARB_CS:
         case GARB_CS_PS:

#ifdef FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED
            /* 3 Mon scheduled when Peer Tech is TDD */
            if (l1_sc_monscan_adaptive_active(gas_id))
            {
               tier = ACTIVITY_TIER_25;
            }
            else
#endif /* FEATURE_GSM_COEX_ADAPTIVE_MON_TDD_SCHED */
            {
               tier = garb_intf_is_monitor_coex_protected(fn % 104, gas_id) ? ACTIVITY_TIER_10 : ACTIVITY_TIER_20;
            }
            break;
         case GARB_CS_SDCCH:
            {
               tier = garb_intf_is_monitor_coex_protected(fn % 102, gas_id) ? ACTIVITY_TIER_10 : ACTIVITY_TIER_20;
            }
            break;
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS */
         case GARB_PS:
            {
#ifdef FEATURE_GSM_COEX_REDUCED_MEAS_GPRS
               tier = garb_intf_is_monitor_coex_protected(fn % 104, gas_id) ? ACTIVITY_TIER_10 : ACTIVITY_TIER_20;
#else
               tier = ACTIVITY_TIER_10;
#endif /* FEATURE_GSM_COEX_REDUCED_MEAS_GPRS */
            }
            break;
         case GARB_NULL:/* X2G IRAT */
#ifdef FEATURE_MCS_WWCOEX_UNIFIED_PRIORITY
            tier = ACTIVITY_TIER_5;
#else
            tier = ACTIVITY_TIER_15;
#endif /*FEATURE_MCS_WWCOEX_UNIFIED_PRIORITY */

            if( (arbitrated_gas[gas_id].cxm_peer_tech & 0x1 ) != 0 )
            {
              tier = ACTIVITY_TIER_25;
            }
            break;
         default:
            /* For DSDA X+G assign all monitors TIER_10 protection */
            tier = ACTIVITY_TIER_10;
            break;
         }

         /* TIER_25 is used for COEX unprotected slots in T+G DSDA */
         if( (arbitrated_gas[gas_id].multi_sim_mode==GARB_MODE_TPG_DSDA) && (tier==ACTIVITY_TIER_20) )
         {
            tier = ACTIVITY_TIER_25;
         }
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)		 
         gmsg_monitors->monitor[i].skip = (skip_required && 
                                           !grm_is_band_registered(gmsg_monitors->monitor[i].arfcn.band, l1_tskisr_blk->client_id, gas_id));
#endif /*FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM*/        
         break;
#ifdef FEATURE_TRIPLE_SIM
      /************************
      * TSTS
      ************************/
      case GARB_MODE_TSTS:
         tier = gl1_hw_qta_gap_active(gas_id) ? ACTIVITY_TIER_10 : ACTIVITY_TIER_20;
         break;
#endif /* FEATURE_TRIPLE_SIM */
      /************************
      * DKDC (Don't Know, Don't Care)
      ************************/
      case GARB_MODE_INVALID:
         tier = ACTIVITY_TIER_20;
         break;
      }

      if(tier==ACTIVITY_TIER_10) {arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.num_protected_slots++;}

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.configured          = TRUE;
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.mon[i].configured   = TRUE;
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.mon[i].cxm_tier     = tier;
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.mon[i].channel_type = GARB_CHAN_UNKNOWN;
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.mon[i].cxm_freqid   = garb_cxm_get_freqid(gmsg_monitors->monitor[i].arfcn, GARB_RX, gas_id);
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.mon[i].arfcn        = gmsg_monitors->monitor[i].arfcn;
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.mon[i].s_time       = garb_get_mon_s_time(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME],gas_id);
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.mon[i].e_time       = garb_get_mon_s_time(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME],gas_id);
      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.mon[i].link_type    = CXM_LNK_TYPE_POWER_MONITOR;

      arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].mons.num_mons            = arbitrated_gas[gas_id].arbitrated_frame->monitors.count;
#endif /* FEATURE_GSM_GPRS_TXSHARING_TIMELINE_REPORTING */

      gmsg_monitors->monitor[i].priority   = arbitrated_gas[gas_id].cxm_activity_table[tier].priority;
      gmsg_monitors->monitor[i].desense_id = garb_cxm_get_freqid(gmsg_monitors->monitor[i].arfcn, GARB_RX, gas_id);
   }

   MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT: ARBITRATE MONS count=%d multi_rat_mode=%d", arbitrated_gas[gas_id].arbitrated_frame->monitors.count, arbitrated_gas[gas_id].multi_sim_mode);
}

/*===========================================================================

FUNCTION      garb_intf_arbitrate_acq

DESCRIPTION   This function is called to arbitrate ACQ programmed up
              in this frame.

RETURN VALUE  None

===========================================================================*/
static void garb_intf_arbitrate_acq(gas_id_t gas_id)
{
   gl1_hw_arbitrated_acq_t*  gmsg_acq      = &arbitrated_gas[gas_id].arbitrated_frame->acq;
   cxm_activity_type         tier          = ACTIVITY_TIER_20;
#ifdef FEATURE_GSM_COEX_ENHANCED_FCCH
   boolean                   enhanced_fcch = arbitrated_gas[gas_id].gas_is_coex_victim;
#else /* FEATURE_GSM_COEX_ENHANCED_FCCH */
   boolean                   enhanced_fcch = FALSE;
#endif /* FEATURE_GSM_COEX_ENHANCED_FCCH */

   /* Frequency ID and Priority */
   switch(arbitrated_gas[gas_id].multi_sim_mode)
   {
   /************************
   * G PLUS G DSDA:
   ************************/
   case GARB_MODE_GPG_DSDA:
#ifndef FEATURE_GSM_COEX_ENHANCED_FCCH
      if( ((arbitrated_gas[peer_gas_id].state!=GARB_CS) || (arbitrated_gas[peer_gas_id].state!=GARB_CS_PS)) && (arbitrated_gas[peer_gas_id].state!=GARB_CS_SDCCH) )
      {
         tier = ACTIVITY_TIER_10;
      }
      else
#endif /* FEATURE_GSM_COEX_ENHANCED_FCCH */
      {
         tier = ACTIVITY_TIER_20;
      }
      break;
   /************************
   * X PLUS G DSDA:
   * C PLUS G DSDA:
   * W PLUS G DSDA:
   * T PLUS G DSDA:
   * L PLUS G DSDA:
   ***********************/
   case GARB_MODE_XPG_DSDA:
   case GARB_MODE_CPG_DSDA:
   case GARB_MODE_WPG_DSDA:
   case GARB_MODE_TPG_DSDA:
   case GARB_MODE_LPG_DSDA:
      tier = ACTIVITY_TIER_20;
      break;
   /************************
   * G PLUS G DSDS
   * X PLUS G DSDS
   * TSTS
   ************************/
   case GARB_MODE_GPG_DSDS:
   case GARB_MODE_XPG_DSDS:
#ifdef FEATURE_TRIPLE_SIM
   case GARB_MODE_TSTS:
#endif /* FEATURE_TRIPLE_SIM */
      switch(arbitrated_gas[gas_id].state)
      {
      case GARB_IDLE:
         tier = gl1_hw_qta_gap_active(gas_id) ? ACTIVITY_TIER_10 : ACTIVITY_TIER_20;
         break;
         
        /* Applicable to GARB_MODE_GPG_DSDS , dont care for GARB_MODE_XPG_DSDS as it perform a GTA */
        /* GARB_MODE_GPG_DSDS -> Allow GBTA - PCH vs Transfer SUB FCCH in Idle Frame */
        /* GFW does not perform conflict check for FCCH and it should always be high priority ,
           such that PCH burst can be YILED when conflict with FCCH */
      case GARB_PS :
         tier = ACTIVITY_TIER_5;
         break;
         
      default:
         tier = ACTIVITY_TIER_10;
         break;
      }
      break;
   default:
    break;
   }

   gmsg_acq->enhanced_fcch_detection = enhanced_fcch;
   gmsg_acq->priority                = arbitrated_gas[gas_id].cxm_activity_table[tier].priority;
   gmsg_acq->desense_id              = garb_cxm_get_freqid(gmsg_acq->arfcn, GARB_RX, gas_id);
}

/*===========================================================================

FUNCTION      garb_intf_arbitrate_sch

DESCRIPTION   This function is called to arbitrate SCH programmed up
              in this frame.

RETURN VALUE  None

===========================================================================*/
static void garb_intf_arbitrate_sch(gas_id_t gas_id)
{
   gl1_hw_arbitrated_sch_t*  gmsg_sch = &arbitrated_gas[gas_id].arbitrated_frame->sch;
   cxm_activity_type         tier     = ACTIVITY_TIER_20;

   /* Frequency ID and Priority */
   switch(arbitrated_gas[gas_id].multi_sim_mode)
   {
   /************************
   * G PLUS G DSDA
   ************************/
   case GARB_MODE_GPG_DSDA:
      tier = ACTIVITY_TIER_15;
      break;
   /************************
   * X PLUS G DSDA
   * C PLUS G DSDA
   * W PLUS G DSDA
   * T PLUS G DSDA
   * L PLUS G DSDA
   ************************/
   case GARB_MODE_XPG_DSDA:
   case GARB_MODE_CPG_DSDA:
   case GARB_MODE_WPG_DSDA:
   case GARB_MODE_TPG_DSDA:
   case GARB_MODE_LPG_DSDA:
#ifdef FEATURE_MCS_WWCOEX_UNIFIED_PRIORITY
      tier = (arbitrated_gas[gas_id].state==GARB_NULL) ? ACTIVITY_TIER_5  /* IRAT */ : ACTIVITY_TIER_10; /* G SUB */
#else
      tier = (arbitrated_gas[gas_id].state==GARB_NULL) ? ACTIVITY_TIER_15 /* IRAT */ : ACTIVITY_TIER_10; /* G SUB */
#endif /* FEATURE_MCS_WWCOEX_UNIFIED_PRIORITY */
      break;
   /************************
   * G PLUS G DSDS
   ************************/
   case GARB_MODE_GPG_DSDS:
      if(arbitrated_gas[gas_id].state==GARB_IDLE)
      {
         tier = (arbitrated_gas[garb_peer_gas_id(gas_id)].state==GARB_PS) ? ACTIVITY_TIER_10 : ACTIVITY_TIER_20;
      }
      /* GARB_MODE_GPG_DSDS -> Allow GBTA - PCH vs Transfer Sub SCH in Idle Frame  */
      /* Transfer SUB SCH is high priority as per legacy and hence should be priortised ,
         such that PCH burst can be YILED when conflict with SCH */
      else if  (arbitrated_gas[gas_id].state == GARB_PS)
      {
         tier = ACTIVITY_TIER_5;
      }
      break;
   /************************
   * X PLUS G DSDS
   * TSTS
   ************************/
   case GARB_MODE_XPG_DSDS:
#ifdef FEATURE_TRIPLE_SIM
   case GARB_MODE_TSTS:
#endif /* FEATURE_TRIPLE_SIM */
      if(arbitrated_gas[gas_id].state==GARB_IDLE)
      {
         tier = gl1_hw_qta_gap_active(gas_id) ? ACTIVITY_TIER_10 : ACTIVITY_TIER_20;
      }
      break;
   default:
      break;
   }

   gmsg_sch->priority   = arbitrated_gas[gas_id].cxm_activity_table[tier].priority;
   gmsg_sch->desense_id = garb_cxm_get_freqid(gmsg_sch->arfcn, GARB_RX, gas_id);
}

/*===========================================================================

FUNCTION      garb_intf_arbitrate_async_rx

DESCRIPTION   This function is called to arbitrate SCH programmed up
              in this frame.

RETURN VALUE  None

===========================================================================*/
static void garb_intf_arbitrate_async_rx(gas_id_t gas_id)
{
   gl1_hw_arbitrated_asyncrx_t* gmsg_async_rx = &arbitrated_gas[gas_id].arbitrated_frame->async_rx;
   cxm_activity_type            tier          = ACTIVITY_TIER_20;

   /* Frequency ID and Priority */
   switch(arbitrated_gas[gas_id].multi_sim_mode)
   {
      /************************
      * G PLUS G DSDA:
      ************************/
      case GARB_MODE_GPG_DSDA:
         tier = ACTIVITY_TIER_15;
         break;
      /************************
      * X PLUS G DSDA:
      * C PLUS G DSDA:
      * W PLUS G DSDA:
      * T PLUS G DSDA:
      * L PLUS G DSDA:
      ***********************/
      case GARB_MODE_XPG_DSDA:
      case GARB_MODE_CPG_DSDA:
      case GARB_MODE_WPG_DSDA:
      case GARB_MODE_TPG_DSDA:
      case GARB_MODE_LPG_DSDA:
#ifdef FEATURE_MCS_WWCOEX_UNIFIED_PRIORITY
         tier = (arbitrated_gas[gas_id].state==GARB_NULL) ? ACTIVITY_TIER_5  /* IRAT */ : ACTIVITY_TIER_10; /* G SUB */
#else
         tier = (arbitrated_gas[gas_id].state==GARB_NULL) ? ACTIVITY_TIER_15 /* IRAT */ : ACTIVITY_TIER_10; /* G SUB */
#endif /* FEATURE_MCS_WWCOEX_UNIFIED_PRIORITY */
         break;
      /************************
      * G PLUS G DSDS
      * X PLUS G DSDS
      * TSTS
      ************************/
      case GARB_MODE_GPG_DSDS:
      case GARB_MODE_XPG_DSDS:
#ifdef FEATURE_TRIPLE_SIM
      case GARB_MODE_TSTS:
#endif /* FEATURE_TRIPLE_SIM */
         switch(arbitrated_gas[gas_id].state)
         {
         case GARB_IDLE:
            if(gl1_hw_qta_gap_active(gas_id))
            {
               tier = ACTIVITY_TIER_10;
            }
            else
            {
               tier = (arbitrated_gas[garb_peer_gas_id(gas_id)].state==GARB_PS) ? ACTIVITY_TIER_10 : ACTIVITY_TIER_20;
            }
            break;
         default:
            tier = ACTIVITY_TIER_10;
            break;
         }
         break;
   default:
         break;
   }

   gmsg_async_rx->priority   = arbitrated_gas[gas_id].cxm_activity_table[tier].priority;
   gmsg_async_rx->desense_id = garb_cxm_get_freqid(gmsg_async_rx->arfcn, GARB_RX, gas_id);
}

#endif /* FEATURE_GSM_COEX */

/*===========================================================================

FUNCTION      garb_intf_arbitrate_x2g

DESCRIPTION   This function is called to arbitrate an X2G gap. Used for QTA
              only.

              Desense ID:

              This is the Freq ID that GFW uses to register with FW CXM on 
              X2G startup. In X2G QTA cases the actual commands (SYNC RX etc)
              are sometimes sent too late to be able to satisfy the FX CXM
              registration period

              By providing the Freq ID GFW can register at the point of X2G
              startup (the entire frame is registers).

              In cases in which we have a QTA gap with PCH + interband
              monitors CXM_UNKNOWN_FREQUENCY will be used.

              Priority (activity) :

              This is the CXM priority GFW will use to register periods of
              activity during the gap (e.g. a CCCH burst). This priority
              needs to be high enough to ensure the source RAT gets a YIELD
              on the TA device for the duration of the gap.

              In non 3WCXM scenarios GFW uses this priority for the entire
              gap.

              Priority (non activity) :

              This is the CXM priority GFW will use to register periods of
              inactivity between bursts. This priority has to be high enough
              to ensure the source RAT gets a YIELD on the TA device for the
              duration of the gap (in non QBTA scenarios) but low enough to
              ensure that no concurrent Tx will get a YIELD.

RETURN VALUE  None

===========================================================================*/
static void garb_intf_arbitrate_x2g(gas_id_t gas_id)
{
   gl1_hw_arbitrated_x2g_t* gmsg_x2g   = &arbitrated_gas[gas_id].arbitrated_frame->x2g;
   uint32                   desense_id = CXM_UNKNOWN_FREQID;
   ARFCN_T                  arfcn      = gmsg_x2g->arfcn;

   /* It is only possible to specify a single Freq ID in the X2G startup */
   if( (gmsg_x2g->interband_monitors == FALSE) && (arfcn.band < INVALID_BAND) )
   {
      desense_id = arbitrated_gas[gas_id].dl_band_table[arfcn.band];
   }
   else if (gl1_hw_qta_gap_active(gas_id) &&
            (garb_intf_gas_is_xpg_coex_victim(gas_id)) &&
            (gmsg_x2g->interband_monitors == TRUE) &&
            (arbitrated_gas[gas_id].desense_band < INVALID_BAND )
           )
   {
     /* use the desence band for the first PCH slot */
     /* This allows X tech to perform TX backoff */
     desense_id   = arbitrated_gas[gas_id].dl_band_table[arbitrated_gas[gas_id].desense_band];
     MSG_GERAN_MED_2("QxTA serving_band %d desense band %d", arfcn.band,arbitrated_gas[gas_id].desense_band);
   }

   gmsg_x2g->desense_id            = desense_id;
   gmsg_x2g->activity_priority     = arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_10].priority;
   gmsg_x2g->no_activity_priority  = arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_20].priority;
}

/*===========================================================================

FUNCTION      garb_intf_notify_gstmr_post_tick

DESCRIPTION   The timeline is re-built here : we maintain a timeline of two
              frames duration for each SUB.

              The N+0 frame ("this frame") is built based on the FW commands
              that have just been intercepted on this frame tick. This is an
              actualrepresentation of the frame that will go out over the air
              in 4.615ms + 576us (FTSM) time.

              The N+1 frame ("next frame") is predicted based on GL1 state
              and other parameters.

              This function also triggers any pending frequency, timeline
              or power reports to be sent to MCS.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_gstmr_post_tick(byte L1_prev_state, byte L1_state, gas_id_t gas_id)
{
#ifdef  FEATURE_GSM_COEX
    uint8 write_t_index = arbitrated_gas[gas_id].write_t_index;
#ifdef  FEATURE_GSM_DTM
   /* DTM exit scenarios require change of GARB states. As the notifications to the arbitrator
   are issued before the L1 main state changes, this had to be done here. This code checks if previous state was DTM and given what the current L1 maine state is,
   the arbitrator state is changed accordingly*/
   if(L1_prev_state == L1_DTM_MODE)
   {
      if(L1_state == L1_TRANSFER_MODE)
      {
         arbitrated_gas[gas_id].state = GARB_PS;
      }
      else if(L1_state == L1_DEDICATED_MODE)
      {
         arbitrated_gas[gas_id].state = GARB_CS;
      }
   }
#endif /*FEATURE_GSM_DTM*/

   garb_reset_timeline(write_t_index,gas_id);

   garb_append_frame_to_timeline(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], GARB_TIMELINE_THIS_FRAME, write_t_index,gas_id);

   garb_get_frame(ADD_FN(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn, 1), GARB_TIMELINE_NEXT_FRAME, gas_id);

   garb_append_frame_to_timeline(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_NEXT_FRAME], GARB_TIMELINE_NEXT_FRAME, write_t_index,gas_id);

   /* If there is a pending timeline notification then send it to CXM */
   garb_cxm_activity_timeline_indication(write_t_index,gas_id);

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
   garb_get_frame(ADD_FN(arbitrated_gas[gas_id].frames[GARB_TIMELINE_NEXT_FRAME].fn, 1), GARB_TIMELINE_THIRD_FRAME, gas_id);

   garb_append_frame_to_timeline(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIRD_FRAME], GARB_TIMELINE_THIRD_FRAME, write_t_index,gas_id);

   garb_get_frame(ADD_FN(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIRD_FRAME].fn, 1), GARB_TIMELINE_FOURTH_FRAME, gas_id);

   garb_append_frame_to_timeline(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_FOURTH_FRAME], GARB_TIMELINE_FOURTH_FRAME, write_t_index,gas_id);
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

  /* Update the timleline write index */
  arbitrated_gas[gas_id].read_t_index = arbitrated_gas[gas_id].write_t_index;

  MSG_GERAN_DEBUG_OPT_1_G("GARB_OPT:read_t_index =%d",arbitrated_gas[gas_id].read_t_index);

  /* Toggle Timeline buffer */
  arbitrated_gas[gas_id].write_t_index =  arbitrated_gas[gas_id].write_t_index ? 0 : 1;

  MSG_GERAN_DEBUG_OPT_1_G("GARB_OPT:write_t_index =%d",arbitrated_gas[gas_id].write_t_index);
  
#endif /* FEATURE_GSM_COEX */

   if(frame_counters[gas_id].FNmod8 == FNMOD8_FOR_CXM_REPORT)
   {
      if( (arbitrated_gas[gas_id].cxm_freq_ind.frequency_list.no_of_items > MAX_CXM_SCELL_ARFCNS) &&
        ( (arbitrated_gas[gas_id].state == GARB_CS) ||
          (arbitrated_gas[gas_id].state == GARB_CS_SDCCH) ||
          (arbitrated_gas[gas_id].state == GARB_PS) ||
          (arbitrated_gas[gas_id].state == GARB_CS_PS) ) )
      {
         ARFCN_T arfcns[8];

         arbitrated_gas[gas_id].cxm_freq_ind.get_arfcns(2, 1, 8, &arfcns[0], gas_id);

         garb_cxm_arfcn_indication(&arfcns[0], 8 , CXM_LNK_DRCTN_UL_AND_DL, TRUE, gas_id);
      }
      else
      if((arbitrated_gas[gas_id].state != GARB_IDLE) &&
         (arbitrated_gas[gas_id].state != GARB_FIND_BCCH)) /* In IDLE it is called after page decode */
      {
         garb_cxm_power_indication(gas_id);
      }
   }

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
   /* If there is a pending timeline notification then send it to CXM */
   garb_cxm_set_slot_activity_timeline(arbitrated_gas[gas_id].read_t_index,gas_id);
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

#if defined (FEATURE_GSM_BAND_AVOIDANCE)
   if( frame_counters[gas_id].FNmod416 == FNMOD416_FOR_CXM_REPORT )
   {
      if( (arbitrated_gas[gas_id].state == GARB_CS) || (arbitrated_gas[gas_id].state==GARB_CS_PS) || (arbitrated_gas[gas_id].state == GARB_CS_SDCCH) )
      {
         garb_cxm_band_avoid_pwr_ind(gas_id);
      }
   }
#endif
}

/*===========================================================================

FUNCTION      garb_register_bcch_list_arfcns()

DESCRIPTION   This function regsiters arfcns in bcch list for 
              Foreground/BPLMN including BPLMN in IDLE.
              
RETURN VALUE  None

===========================================================================*/
void garb_register_bcch_list_arfcns(boolean bplmn_acq_bcch_list, gas_id_t gas_id)
{
  uint8 max_freq_reg_num = MAX_CXM_FREQIDS;
  uint8 num_arfcn_to_register = 0;
  boolean band_reg = FALSE;
  uint16 i=0;
  uint8 no_of_items = 0;
  frequency_list_T *frequency_list = &arbitrated_gas[gas_id].params_find_bcch.frequency_list;

  /* first entry occupied by SCELL in IDLE */
  if ((arbitrated_gas[gas_id].state == GARB_IDLE) && (bplmn_acq_bcch_list == TRUE))
  {
    max_freq_reg_num = MAX_CXM_FREQIDS - 1 - 4; // -1 for scell, 4 for 4 bands
    frequency_list->channel[num_arfcn_to_register] = arbitrated_gas[gas_id].params_idle.frequency_list.channel[0];
    arbitrated_gas[gas_id].params_find_bcch.rx_power_dbm10[num_arfcn_to_register] = gpl1_get_rx_pwr_avg_dbm10_coex(gas_id);
    num_arfcn_to_register++;
    band_reg = TRUE;
  }

  /* specfic bcch / reselection_req case */
  if ( (arbitrated_gas[gas_id].state == GARB_FIND_BCCH) && 
       (arbitrated_gas[gas_id].params_find_bcch.length == 0) && 
       (arbitrated_gas[gas_id].params_find_bcch.arfcn.num != INVALID_ARFCN) )
  {
    frequency_list->channel[num_arfcn_to_register] = arbitrated_gas[gas_id].params_find_bcch.arfcn;
    num_arfcn_to_register++;
    no_of_items = num_arfcn_to_register;
  }
  else  /* foreground BPLMN/IDLE */
  /* Register upto MAX_CXM_FREQIDS */
  {
    for ( i = 0; ((i < arbitrated_gas[gas_id].params_find_bcch.length) && ( no_of_items < max_freq_reg_num ));i++)
    {
      /* add to Freq list to register ARFCN */
      if (arbitrated_gas[gas_id].params_find_bcch.bcch_decoded_list[i] == FALSE)
      {
        frequency_list->channel[num_arfcn_to_register] = arbitrated_gas[gas_id].params_find_bcch.arfcn_list[i];
        
        arbitrated_gas[gas_id].params_find_bcch.rx_power_dbm10[num_arfcn_to_register] = 
                  (((arbitrated_gas[gas_id].params_find_bcch.rx_power_instant_list[i] + GARB_FIND_BCCH_DESENSE_BIAS) * 10 ) >> 4);
        no_of_items++;
        num_arfcn_to_register++;
      }
    }
  }

  MSG_GERAN_LOW_3_G("GARB : FN=%d  REG ARFCS max_freq_reg_num %d num_arfcn_to_register %d ", 
                      GSTMR_GET_FN_GERAN(gas_id),max_freq_reg_num ,num_arfcn_to_register);
  
  frequency_list->no_of_items = num_arfcn_to_register;

  if (no_of_items)
  {
    /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all Freq IDs */
    garb_cxm_arfcn_indication(&frequency_list->channel[0], frequency_list->no_of_items, CXM_LNK_DRCTN_DL, band_reg, gas_id);
  }
}

/*===========================================================================

FUNCTION      garb_intf_notify_find_bcch

DESCRIPTION   This function notifies the arbitrator of a transition into
              the FIND BCCH state.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_find_bcch(gas_id_t gas_id)
{
   MSG_GERAN_MED_1_G("GARB : FN=%d  num_arfcn %d NOTIFY FIND BCCH", GSTMR_GET_FN_GERAN(gas_id));

   /* Initialise the state parameters for FIND BCCH */
   arbitrated_gas[gas_id].state = GARB_FIND_BCCH;

   if (arbitrated_gas[gas_id].params_find_bcch.arfcn.num != INVALID_ARFCN)
   {
     garb_register_bcch_list_arfcns(FALSE, gas_id);
   }
   else
   {
     /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register bandss */
     garb_cxm_arfcn_indication(&arbitrated_gas[gas_id].params_find_bcch.arfcn,0, CXM_LNK_DRCTN_DL, TRUE, gas_id);
   }

#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(GSTMR_GET_FN_GERAN(gas_id),GL1_DEFS_INVALID_FN,CXM_HIGH_PRIO_ACQ,gas_id);
#endif

#ifdef FEATURE_MODEM_DDR_MGMT
   if (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
   {
     trm_update_tech_state(TRM_GSM1,TRM_ACQUISITION, SYS_BAND_GSM_EGSM_900);
   }
#endif /* FEATURE_MODEM_DDR_MGMT */
}

/*===========================================================================

FUNCTION      garb_intf_notify_find_bcch_update_arfcn

DESCRIPTION   This function updates ARFCN params during specific selection bcch
              as part of early camping.
              
RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_find_bcch_update_arfcn(ARFCN_T arfcn, gas_id_t gas_id)
{
   MSG_GERAN_LOW_3_G("GARB : FN=%d NOTIFY FIND BCCH  UPDATE ARFCN %d band %d", GSTMR_GET_FN_GERAN(gas_id),arfcn.num,arfcn.band); 

   /* Initialise the state parameters for FIND BCCH */
   arbitrated_gas[gas_id].params_find_bcch.length = 0;
   arbitrated_gas[gas_id].params_find_bcch.freq_reg_threshold_counter = 0;
   arbitrated_gas[gas_id].params_find_bcch.arfcn                    = arfcn;
   arbitrated_gas[gas_id].params_find_bcch.rx_power_dbm10[0] = GL1_CXM_UNKNOWN_POWER;
}

/*===========================================================================

FUNCTION      garb_intf_notify_find_bcch_update_rx_power

DESCRIPTION   This function updates rx power during specific bcch read.
              
RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_find_bcch_update_rx_power(ARFCN_T arfcn, int32 rx_pwr,gas_id_t gas_id)
{
   MSG_GERAN_LOW_3_G("GARB : FN=%d NOTIFY FIND BCCH  UPDATE RXPOWER %d rx_pwr %d", GSTMR_GET_FN_GERAN(gas_id),arfcn.num,rx_pwr); 

   if (  (arbitrated_gas[gas_id].params_find_bcch.arfcn.num != INVALID_ARFCN)
       && (ARFCNS_EQUAL(arbitrated_gas[gas_id].params_find_bcch.arfcn ,arfcn)))
   {
     arbitrated_gas[gas_id].params_find_bcch.rx_power_dbm10[0]  = (((rx_pwr + GARB_FIND_BCCH_DESENSE_BIAS) * 10 ) >> 4);
   }
}


/*===========================================================================

FUNCTION      garb_intf_notify_find_bcch_setup_bcch_list()

DESCRIPTION   This function sets up the bcch LIST for COEX FREQ REG
              
RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_find_bcch_setup_bcch_list(uint16 bcch_len_list,ARFCN_T  *arfcn_list,
                                                dBx16_T *rx_power_list,boolean *bcch_decoded_list,
                                                boolean bplmn_acq_bcch_list,gas_id_t gas_id)
{
  MSG_GERAN_HIGH_3_G("GARB : FN=%d setup_bcch_list bcch_len_list %d bplmn_acq_bcch_list %d", 
                      GSTMR_GET_FN_GERAN(gas_id),bcch_len_list,bplmn_acq_bcch_list); 

  memset(&arbitrated_gas[gas_id].params_find_bcch,0,sizeof(garb_params_find_bcch_t));

  arbitrated_gas[gas_id].params_find_bcch.bplmn_bcch_acq_list = bplmn_acq_bcch_list;
  arbitrated_gas[gas_id].params_find_bcch.length = bcch_len_list;
  arbitrated_gas[gas_id].params_find_bcch.bcch_decoded_list = bcch_decoded_list;
  arbitrated_gas[gas_id].params_find_bcch.arfcn_list = arfcn_list;
  arbitrated_gas[gas_id].params_find_bcch.rx_power_instant_list = rx_power_list;
  arbitrated_gas[gas_id].params_find_bcch.freq_reg_threshold_counter = 0;

  /*key used for power reporting in FIND_BCCH_MODE */
  arbitrated_gas[gas_id].params_find_bcch.arfcn = arbitrated_gas[gas_id].params_find_bcch.arfcn_list[0];
}

/*===========================================================================

FUNCTION      garb_intf_notify_find_bcch_decode_done()

DESCRIPTION   This function fethches new set of freq id.
              
RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_find_bcch_decode_done(ARFCN_T arfcn_done,gas_id_t gas_id)
{
   arbitrated_gas[gas_id].params_find_bcch.freq_reg_threshold_counter++;
   
   MSG_GERAN_LOW_4_G("GARB : FN=%d garb_intf_notify_find_bcch_decode_done %d freq_reg_threshold_counter %d REGISTRATION_THRESH %d", 
                      GSTMR_GET_FN_GERAN(gas_id),arfcn_done.num,arbitrated_gas[gas_id].params_find_bcch.freq_reg_threshold_counter,GARB_FIND_BCCH_FREQID_REGISTRATION_THRESH);

   if (arbitrated_gas[gas_id].params_find_bcch.freq_reg_threshold_counter > GARB_FIND_BCCH_FREQID_REGISTRATION_THRESH)
   {
     garb_register_bcch_list_arfcns(arbitrated_gas[gas_id].params_find_bcch.bplmn_bcch_acq_list, gas_id);
     /* Reset the threshold counter for re-reg */
     arbitrated_gas[gas_id].params_find_bcch.freq_reg_threshold_counter = 0;
   }
}

/*===========================================================================

FUNCTION      garb_intf_notify_idle

DESCRIPTION   This function notifies the arbitrator of a transition into
              the IDLE state.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_idle(ARFCN_T arfcn, boolean ccch_sdcch_comb, paging_mode_T paging_mode, byte paging_mfrms, byte paging_mfrm, byte opb, gas_id_t gas_id)
{
   MSG_GERAN_HIGH_1_G("GARB : FN=%d NOTIFY IDLE", GSTMR_GET_FN_GERAN(gas_id));

   /* Initialise the state parameters for IDLE */
   arbitrated_gas[gas_id].state                       = GARB_IDLE;
   arbitrated_gas[gas_id].params_idle.ccch_sdcch_comb = ccch_sdcch_comb;
   arbitrated_gas[gas_id].params_idle.frequency_list.no_of_items  = 1;
   arbitrated_gas[gas_id].params_idle.frequency_list.channel[0]	  = arfcn;
   arbitrated_gas[gas_id].desense_band                            = arfcn.band;
   arbitrated_gas[gas_id].params_idle.interband                   = garb_intf_monscan_query_interband_cells(arfcn.band,gas_id);
   arbitrated_gas[gas_id].params_idle.idle_freq_id_update         = NO_FREQ_ID_UPDATE;
   arbitrated_gas[gas_id].params_idle.paging_mode     = paging_mode;
   arbitrated_gas[gas_id].params_idle.paging_period   = paging_mfrms*51;
   arbitrated_gas[gas_id].params_idle.paging_position = paging_mfrm*51 + garb_paging_frame[opb];

#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif

   /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all Freq IDs */
   garb_cxm_arfcn_indication(&arfcn, 1, CXM_LNK_DRCTN_DL, TRUE, gas_id);
}

/*===========================================================================

FUNCTION      garb_intf_notify_idle_update_freq_list

DESCRIPTION   This function prepares the list for ARFCNS with SCELL and upto TOP 6 NCELL
              for which L1 performs the ACQ/BCCH.

              First Part  : Check if registration is needed with CXM.
                            ( First Registration or More than  6 NCELL)
 
              Second Part : If there is new BA list or First Registration
                               Register Freq with MCS  
                            else 
                               only update Power of existing freq id.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_idle_update_freq_list(uint8    sorted_cell_count,
                                                     uint8     sorted_cell_max_count,
                                                     cell_T**  sorted_cells,
                                                     gas_id_t  gas_id)
{
   uint8 num ,i;
   boolean freq_id_reg_needed = FALSE;

   boolean sorted_cell_found_in_arfcn_table[SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL];
   memset(&sorted_cell_found_in_arfcn_table[0], FALSE, ARR_SIZE(sorted_cell_found_in_arfcn_table));

   /* Check if NCELL are present */
   if ( (arbitrated_gas[gas_id].state == GARB_IDLE) && sorted_cell_count)
   {
     /* PART 1 */

     /* First time registration, as only serving_cell is registered.*/
     if (arbitrated_gas[gas_id].params_idle.frequency_list.no_of_items == 1)
     {
        freq_id_reg_needed = TRUE;
     }
     /* (no_of_items > 1 ) , check if from sorted cell list arfcns are registered with mcs */
     /* takes care of update due to new  BA list or sorted cell list due to cell blacklisting */
     /* if there is new cell added , it will be absent in the  dl_arfcn_table list */
     else 
     {
        for ( num = 0; (
                         (num < sorted_cell_count) && 
                         (num < sorted_cell_max_count) &&
                         (num < (BA_MAX_LEN + 1)) &&
                         (num < SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL));
              ++num )
        {
           for ( i = 1; i< arbitrated_gas[gas_id].params_idle.frequency_list.no_of_items;i++)
           {
             if(sorted_cells[ num ])
             {
               if ( sorted_cells[ num ]->bs.arfcn.num == (uint16)arbitrated_gas[gas_id].dl_arfcn_table.arfcns[i].arfcn)
               {
                 sorted_cell_found_in_arfcn_table [num] = TRUE;
                 break;
               }
             }
           }
        }

        /* Check if any new cell added */
        /* ARFCN absent in regsistered cxm table entry , Need to register with CXM */
        for ( num = 0; (
                         (num < sorted_cell_count) &&
                         (num < sorted_cell_max_count) &&
                         (num < SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL));
              ++num )
        {
          if (sorted_cell_found_in_arfcn_table [num] == FALSE)
          {
             freq_id_reg_needed = TRUE;
             break;
          }
        }
     } 

     /* PART 2 */

     if (freq_id_reg_needed)
     {
        frequency_list_T frequency_list;
        dBx16_T    *rx_power_dbm10 = &arbitrated_gas[gas_id].params_idle.rx_power_dbm10[0];

        frequency_list.no_of_items = 0;

        for ( num = 0; ((num < sorted_cell_count) &&
                        (num < sorted_cell_max_count) &&
                        (num < (BA_MAX_LEN + 1)) &&
                        (num < (SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL)));
              ++num )
        {
          if(sorted_cells[ num ])
          {
             frequency_list.channel[num + 1] = sorted_cells[ num ]->bs.arfcn;
             rx_power_dbm10[num + 1]       = ((sorted_cells[ num ]->bs.rx_power * 10) >> 4); // dbm10
             frequency_list.no_of_items++;
          }
        }

        /* For Serving Cell */
        if (frequency_list.no_of_items)
        {
          /* SCELL Freq on 1st slot */
          frequency_list.channel[0] =   gl1_get_serving_cell_arfcn(gas_id);
          frequency_list.no_of_items++;
        
          /*Service cell rx power */
          rx_power_dbm10[0] = gpl1_get_rx_pwr_avg_dbm10_coex(gas_id);

          arbitrated_gas[gas_id].params_idle.frequency_list = frequency_list;
          arbitrated_gas[gas_id].params_idle.idle_freq_id_update = FREQ_ID_UPDATE;
        }
     }
     else // Just  power update as all arfcn present , but they may not be in same order after sorting
     {
       arbitrated_gas[gas_id].params_idle.idle_freq_id_update = NO_FREQ_ID_UPDATE;
       for ( i = 1; i< arbitrated_gas[gas_id].params_idle.frequency_list.no_of_items;i++)
       {
         for ( num = 0; ((num < sorted_cell_count) &&
                         (num < sorted_cell_max_count) &&
                         (num < (BA_MAX_LEN + 1)) &&
                         (num < (SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL+1)));
               ++num )
         {
           if(sorted_cells[ num ])
           {
             if (sorted_cells[ num ]->bs.arfcn.num == (uint16)arbitrated_gas[gas_id].dl_arfcn_table.arfcns[i].arfcn)
             {
                arbitrated_gas[gas_id].params_idle.rx_power_dbm10[i] = ((sorted_cells[ num ]->bs.rx_power * 10) >> 4);
                break;
             }
           }
         }
       }
       arbitrated_gas[gas_id].params_idle.rx_power_dbm10[0] = gpl1_get_rx_pwr_avg_dbm10_coex(gas_id);
       garb_cxm_power_indication(gas_id);
     }
   }
}/* garb_intf_notify_idle_update_freq_list */

/*===========================================================================

FUNCTION      garb_intf_notify_idle_update_reg_arfcns

DESCRIPTION   This function updates  the arbitrator with SCELL and upto TOP 6 NCELL
              for which L1 performs the ACQ/BCCH.by registrring Freq with MCS.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_idle_update_reg_arfcns(boolean leaving_idle,gas_id_t gas_id )
{
  if (leaving_idle)
  {
    arbitrated_gas[gas_id].params_idle.idle_freq_id_update = NO_FREQ_ID_UPDATE;
  }

  if(arbitrated_gas[gas_id].params_idle.idle_freq_id_update == FREQ_ID_UPDATE)
  {
    /* Update if interband cell added to BA list */
    arbitrated_gas[gas_id].params_idle.interband     = 
                  garb_intf_monscan_query_interband_cells(
                      arbitrated_gas[gas_id].params_idle.frequency_list.channel[0].band,
                      gas_id);

    /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all Freq IDs */
    garb_cxm_arfcn_indication(&arbitrated_gas[gas_id].params_idle.frequency_list.channel[0],
                               arbitrated_gas[gas_id].params_idle.frequency_list.no_of_items,
                               CXM_LNK_DRCTN_DL, 
                               TRUE,
                               gas_id);

    arbitrated_gas[gas_id].params_idle.idle_freq_id_update = NO_FREQ_ID_UPDATE;
  }
} /* garb_intf_notify_idle_update_reg_arfcns */

/*===========================================================================

FUNCTION      garb_intf_notify_exit

DESCRIPTION   This function notifies the arbitrator of an exit from the
              current state.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_exit( gas_id_t gas_id)
{
   /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to deregister all Freq IDs */
   garb_cxm_arfcn_indication(NULL, 0, CXM_LNK_DRCTN_INVLD, FALSE, gas_id);

   arbitrated_gas[gas_id].state = GARB_NULL;

#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif

   MSG_GERAN_HIGH_1_G("GARB : FN=%d NOTIFY EXIT", GSTMR_GET_FN_GERAN(gas_id));
}

/*===========================================================================

FUNCTION      garb_intf_notify_access

DESCRIPTION   This function notifies the arbitrator of a transition into
              the RACH state.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_access(ARFCN_T arfcn, uint8 delay, int16 txlev, gas_id_t gas_id)
{
   MSG_GERAN_HIGH_2_G("GARB <- GL1: FN=%d NOTIFY ACCESS delay=%d", GSTMR_GET_FN_GERAN(gas_id), delay);

   /* Initialise the state parameters for ACCESS */
   arbitrated_gas[gas_id].state                 = GARB_ACCESS;
   arbitrated_gas[gas_id].params_access.arfcn   = arfcn;
   arbitrated_gas[gas_id].params_access.txlev   = txlev;
   arbitrated_gas[gas_id].params_access.rach_fn = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), delay);
   arbitrated_gas[gas_id].params_access.rach_delay = delay;

#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(GSTMR_GET_FN_GERAN(gas_id),GL1_DEFS_INVALID_FN,CXM_HIGH_PRIO_RACH,gas_id);
#endif

   /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all Freq IDs */
   garb_cxm_arfcn_indication(&arfcn, 1, CXM_LNK_DRCTN_UL_AND_DL, TRUE, gas_id);
}

/*===========================================================================

FUNCTION      garb_intf_notify_access_update

DESCRIPTION   This function updates the RACH parameters

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_access_update(ARFCN_T arfcn, uint8 delay, int16 txlev, gas_id_t gas_id)
{
   MSG_GERAN_HIGH_2_G("GARB <- GL1: FN=%d NOTIFY ACCESS UPDATE delay=%d", GSTMR_GET_FN_GERAN(gas_id), delay);

   /* Initialise the state parameters for ACCESS */
   arbitrated_gas[gas_id].state                 = GARB_ACCESS;
   arbitrated_gas[gas_id].params_access.arfcn   = arfcn;
   arbitrated_gas[gas_id].params_access.txlev   = txlev;
   arbitrated_gas[gas_id].params_access.rach_fn = ADD_FN(GSTMR_GET_FN_GERAN(gas_id), delay);
   arbitrated_gas[gas_id].params_access.rach_delay = delay;
}

/*===========================================================================

FUNCTION      garb_intf_notify_twophase_access

DESCRIPTION   This function notifies the arbitrator in ACCESS state freq information.


RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_twophase_access(frequency_list_T frequency_list, gas_id_t gas_id)
{
   MSG_GERAN_HIGH_2_G("GARB <- GL1: FN=%d NOTIFY TWO PHASE ACCESS no_of_items=%d", GSTMR_GET_FN_GERAN(gas_id), frequency_list.no_of_items);
   arbitrated_gas[gas_id].state = GARB_2P_ACCESS;
   arbitrated_gas[gas_id].params_two_phase.frequency_list  = frequency_list;


  /*
   * The CXM supports a maximum of 24 registered ARFCNs at any one time. Note
   * that seperate registrations are required for both UL and DL. The allocation
   * of 24 x UL or DL ARFCS is segmented as follows:
   *
   * 10 x UL ARFCN (SCELL)
   * 10 x DL ARFCN (SCELL)
   *  4 x DL Bands (NCELL)
   *
   * If there are 10 or less ARFCNs in the frequency list then we can do a one-shot
   * registration with CXM which will be good for the lifetime of the assignment.
   *
   * GARB_TODO : If however there are more than 10 ARFCNs in the frequency list
   * then we are required to periodically update CXM with a new list of registered.
   * ARFCNs.
   *
   */
   if(frequency_list.no_of_items > MAX_CXM_SCELL_ARFCNS )
   {
      MSG_GERAN_HIGH_2_G("GARB : FN=%d Too many ARFCNs (%d) in two assignment list for one-shot registration with CXM", GSTMR_GET_FN_GERAN(gas_id), frequency_list.no_of_items);
      return;
   }

   /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all Freq IDs */
   garb_cxm_arfcn_indication(&frequency_list.channel[0], frequency_list.no_of_items, CXM_LNK_DRCTN_UL_AND_DL, TRUE, gas_id);

#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif
}

/*===========================================================================

FUNCTION      garb_intf_get_arfcns_to_register

DESCRIPTION   Obtains num of arfncs to register with CXM.
              Bound checks when  no_of_items > MAX_CXM_SCELL_ARFCNS.
              Prevents double registration during block registration boundary  			  

RETURN VALUE  num_arfcn_to_register

===========================================================================*/
uint8  garb_intf_get_arfcns_to_register(frequency_list_T   *frequency_list, gas_id_t gas_id)
{
  byte num_arfcn_to_register = frequency_list->no_of_items;
  
  if( (frequency_list->no_of_items > MAX_CXM_SCELL_ARFCNS ) && (frame_counters[gas_id].FNmod8 != FNMOD8_FOR_CXM_REPORT) )
  {
     num_arfcn_to_register = MAX_CXM_SCELL_ARFCNS - frame_counters[gas_id].FNmod8 + 1;
  
     MSG_GERAN_HIGH_2_G("GARB : Too many ARFCNs (%d )in for single-shot CXM registration arfcn_registered %d",
        arbitrated_gas[gas_id].cxm_freq_ind.frequency_list.no_of_items,num_arfcn_to_register);
  }
  else if( (frequency_list->no_of_items > MAX_CXM_SCELL_ARFCNS ) && (frame_counters[gas_id].FNmod8 == FNMOD8_FOR_CXM_REPORT) )
  {
     /* Do nothing */
     /* in garb_intf_notify_gstmr_post_tick -> frequency registration will be performed */
     num_arfcn_to_register = 0;
     MSG_GERAN_HIGH_1_G("GARB : Too many ARFCNs (%d )in for single-shot CXM registration , register in post tick",
        arbitrated_gas[gas_id].cxm_freq_ind.frequency_list.no_of_items);
  }
  /* Dont need this else if as any one of above will be TRUE for > MAX_CXM_SCELL_ARFCNS 
  else
  if (frequency_list->no_of_items > MAX_CXM_SCELL_ARFCNS )
  {  
     num_arfcn_to_register = MAX_CXM_SCELL_ARFCNS;
  }*/
  else  // actual , .. number less than MAX_CXM_SCELL_ARFCNS
  {  
    num_arfcn_to_register = frequency_list->no_of_items; 
  }

  return num_arfcn_to_register;
}

/*===========================================================================

FUNCTION      garb_intf_notify_cs_assignment

DESCRIPTION   This function notifies the arbitrator of a CS assignment.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_cs_assignment(
   uint8              TN,
   frequency_list_T   frequency_list,
   uint8              subchannel,
   boolean            is_fr,
   boolean            is_amr,
   uint8              sacch_frame,
   uint8              idle_frame,
   garb_get_arfcns_t  get_arfcns,
   garb_request_tx_t* request_tx,
   uint8              L1_state,
   gas_id_t           gas_id )

{
   byte num_arfcn_to_register = 0;
   MSG_GERAN_HIGH_1_G("GARB <- GL1: FN=%d NOTIFY CS ASSIGNMENT",GSTMR_GET_FN_GERAN(gas_id));

   if(L1_state == L1_DTM_MODE)
   {
     arbitrated_gas[gas_id].state                     = GARB_CS_PS;
   }
   else
   {
     arbitrated_gas[gas_id].state                     = GARB_CS;
   }

   /* Initialise the state parameters for CS */
   arbitrated_gas[gas_id].params_cs.TN                = TN;
   arbitrated_gas[gas_id].params_cs.sacch_frame       = sacch_frame-1;
   arbitrated_gas[gas_id].params_cs.idle_frame        = idle_frame-1;
   arbitrated_gas[gas_id].params_cs.frequency_list    = frequency_list;
   arbitrated_gas[gas_id].params_cs.is_fr             = is_fr;
   arbitrated_gas[gas_id].params_cs.subchannel        = subchannel;
   arbitrated_gas[gas_id].cxm_time_ind.last_ind_fn    = GARB_CXM_IND_UPDATE_NOW;
   arbitrated_gas[gas_id].cxm_freq_ind.frequency_list = frequency_list;
   arbitrated_gas[gas_id].cxm_freq_ind.get_arfcns     = get_arfcns;

   memset(&arbitrated_gas[gas_id].params_cs.channels[0], 0, sizeof(garb_channels_t[GARB_TIMELINE_MAX_FRAMES]));

   *request_tx = garb_intf_request_block_tx;

  /*
   * The CXM supports a maximum of 24 registered ARFCNs at any one time. Note
   * that seperate registrations are required for both UL and DL. The allocation
   * of 24 x UL or DL ARFCS is segmented as follows:
   *
   * 10 x UL ARFCN (SCELL)
   * 10 x DL ARFCN (SCELL)
   *  4 x DL Bands (NCELL)
   *
   * If there are 9 or less ARFCNs in the frequency list then we can do a one-shot
   * registration with CXM which will be good for the lifetime of the assignment.
   *
   * GARB_TODO : If however there are more than 9 ARFCNs in the frequency list
   * then we are required to periodically update CXM with a new list of registered.
   * ARFCNs.
   *
   */
   num_arfcn_to_register = garb_intf_get_arfcns_to_register(&frequency_list,gas_id);
   if (num_arfcn_to_register)
   {
     if (frequency_list.no_of_items > MAX_CXM_SCELL_ARFCNS)
     {
       /* num_arfcn_to_register will be either MAX_CXM_SCELL_ARFCNS or less if its not registration boundary */
       get_arfcns( 2, 1, num_arfcn_to_register, &frequency_list.channel[0], gas_id);
     }
     
     /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all Freq IDs */
     garb_cxm_arfcn_indication(&frequency_list.channel[0], num_arfcn_to_register, CXM_LNK_DRCTN_UL_AND_DL, TRUE, gas_id);
   }

#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif

#ifdef FEATURE_MODEM_DDR_MGMT
   if (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
   {
     trm_update_tech_state( TRM_GSM1, TRM_TRAFFIC, gl1_map_gsm_band_to_sys_band( frequency_list.channel[0].band ) );
     MSG_GERAN_MED_2_G( "GARB : trm_update_tech_state band_cs:%d trm_band:%d",
                       frequency_list.channel[0].band,
                        gl1_map_gsm_band_to_sys_band( frequency_list.channel[0].band ) );
   }
#endif /* FEATURE_MODEM_DDR_MGMT */
}

/*===========================================================================

FUNCTION      garb_intf_notify_ps_assignment

DESCRIPTION   This function notifies the arbitrator of a PS assignment.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_ps_assignment(frequency_list_T frequency_list, uint8 num_ul_slots, uint8 num_dl_slots,
                                    garb_get_arfcns_t get_arfcns, boolean *ps_ul_alloc, boolean *ps_dl_alloc,
                                    uint8 L1_state, gas_id_t gas_id)
{
   byte num_arfcn_to_register = 0;
   MSG_GERAN_HIGH_3_G("GARB MSG <- GL1 : FN=%d NOTIFY PS ASSIGNMENT UL=%d DL=%d", GSTMR_GET_FN_GERAN(gas_id), num_ul_slots, num_dl_slots);

   memscpy(&arbitrated_gas[gas_id].params_ps.ps_ul_alloc, (sizeof(boolean)*GL1_DEFS_SLOTS_IN_FRAME), ps_ul_alloc, (sizeof(boolean)*GL1_DEFS_SLOTS_IN_FRAME));
   memscpy(&arbitrated_gas[gas_id].params_ps.ps_dl_alloc, (sizeof(boolean)*GL1_DEFS_SLOTS_IN_FRAME), ps_dl_alloc, (sizeof(boolean)*GL1_DEFS_SLOTS_IN_FRAME));

   if(L1_state == L1_DTM_MODE)
   {
      arbitrated_gas[gas_id].state                       = GARB_CS_PS;
   }
   else
   {
      arbitrated_gas[gas_id].state                       = GARB_PS;
   }

   /* Initialise the state parameters for PS */
   arbitrated_gas[gas_id].params_ps.frequency_list     = frequency_list;
   arbitrated_gas[gas_id].params_ps.num_ul_slots       = num_ul_slots;
   arbitrated_gas[gas_id].params_ps.num_dl_slots       = num_dl_slots;
   arbitrated_gas[gas_id].cxm_time_ind.last_ind_fn     = GARB_CXM_IND_UPDATE_NOW;
   arbitrated_gas[gas_id].cxm_freq_ind.frequency_list  = frequency_list;
   arbitrated_gas[gas_id].cxm_freq_ind.get_arfcns      = get_arfcns;
#ifdef FEATURE_GSM_COEX
   arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn = garb_get_next_rrbp_fn(gas_id);
#endif /* FEATURE_GSM_COEX */

  /*
   * The CXM supports a maximum of 24 registered ARFCNs at any one time. Note
   * that seperate registrations are required for both UL and DL. The allocation
   * of 24 x UL or DL ARFCS is segmented as follows:
   *
   * 10 x UL ARFCN (SCELL)
   * 10 x DL ARFCN (SCELL)
   *  4 x DL Bands (NCELL)
   *
   * If there are 10 or less ARFCNs in the frequency list then we can do a one-shot
   * registration with CXM which will be good for the lifetime of the assignment.
   *
   * GARB_TODO : If however there are more than 10 ARFCNs in the frequency list
   * then we are required to periodically update CXM with a new list of registered.
   * ARFCNs.
   *
   */
   if(frequency_list.no_of_items)
   {
     num_arfcn_to_register = garb_intf_get_arfcns_to_register(&frequency_list,gas_id);
     if (num_arfcn_to_register)
     {
       if (frequency_list.no_of_items > MAX_CXM_SCELL_ARFCNS)
       {
         /* num_arfcn_to_register will be either MAX_CXM_SCELL_ARFCNS or less if its not registration boundary */
         get_arfcns( 2, 1, num_arfcn_to_register, &frequency_list.channel[0], gas_id);
       }
     
       /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all Freq IDs */
       garb_cxm_arfcn_indication(&frequency_list.channel[0], num_arfcn_to_register, CXM_LNK_DRCTN_UL_AND_DL, TRUE, gas_id);
     }
   }
#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif


#ifdef FEATURE_MODEM_DDR_MGMT
   if (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
   {
     trm_update_tech_state(TRM_GSM1,TRM_BG_TRAFFIC,gl1_map_gsm_band_to_sys_band(frequency_list.channel[0].band));
     MSG_GERAN_MED_2_G("GARB : trm_update_tech_state band_ps %d , trm_band %d",
                       frequency_list.channel[0].band,
                       gl1_map_gsm_band_to_sys_band(frequency_list.channel[0].band));

   }
#endif /* FEATURE_MODEM_DDR_MGMT */
}

/*===========================================================================

FUNCTION      garb_intf_notify_monscan_ba_list

DESCRIPTION   This function notifies the arbitrator that a monitor scan has
              started on a new BA list.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_monscan_ba_list(cell_T** cell_pptr, uint8 num_arfcns, garb_is_coex_protected_mon_t* is_coex_protected_monitor, gas_id_t gas_id )
{
   MSG_GERAN_HIGH_2_G("GARB MSG <- GL1 : FN=%d NOTIFY MONSCAN BA LIST num_cells=%d", GSTMR_GET_FN_GERAN(gas_id), num_arfcns);

   /* Initialise the state parameters for MONSCAN */
   arbitrated_gas[gas_id].params_monscan.ba_list_pptr = cell_pptr;
   arbitrated_gas[gas_id].params_monscan.ba_list_size = num_arfcns;

#ifdef FEATURE_GSM_COEX
   *is_coex_protected_monitor = garb_intf_is_monitor_coex_protected;
#else
  *is_coex_protected_monitor = FALSE;
#endif
}

/*===========================================================================

FUNCTION      garb_intf_notify_sdcch_assignment

DESCRIPTION   This function notifies the arbitrator of an SDCCH assignment.

RETURN VALUE  None

===========================================================================*/
void garb_intf_notify_sdcch_assignment(frequency_list_T  frequency_list, boolean is_sdcch_8, uint8 subchannel, garb_get_arfcns_t get_arfcns, gas_id_t gas_id )
{
   byte num_arfcn_to_register = 0;
   MSG_GERAN_HIGH_3_G("GARB MSG <- GL1 : FN=%d NOTIFY SDCCH ASSIGNMENT is_sdcch_8=%d subchannel=%d", GSTMR_GET_FN_GERAN(gas_id), is_sdcch_8, subchannel);

   /* Initialise the state parameters for SDCCH */
   arbitrated_gas[gas_id].state                       = GARB_CS_SDCCH;
   arbitrated_gas[gas_id].params_sdcch.is_sdcch_8     = is_sdcch_8;;
   arbitrated_gas[gas_id].params_sdcch.subchannel     = subchannel;
   arbitrated_gas[gas_id].cxm_time_ind.last_ind_fn    = GARB_CXM_IND_UPDATE_NOW;
   arbitrated_gas[gas_id].cxm_freq_ind.frequency_list = frequency_list;
   arbitrated_gas[gas_id].cxm_freq_ind.get_arfcns     = get_arfcns;

   memset(&arbitrated_gas[gas_id].params_sdcch.channels[0], 0, sizeof(garb_channels_t[GARB_TIMELINE_MAX_FRAMES]));

  /*
   * The CXM supports a maximum of 24 registered ARFCNs at any one time. Note
   * that seperate registrations are required for both UL and DL. The allocation
   * of 24 x UL or DL ARFCS is segmented as follows:
   *
   * 10 x UL ARFCN (SCELL)
   * 10 x DL ARFCN (SCELL)
   *  4 x DL Bands (NCELL)
   *
   * If there are 10 or less ARFCNs in the frequency list then we can do a one-shot
   * registration with CXM which will be good for the lifetime of the assignment.
   *
   * GARB_TODO : If however there are more than 10 ARFCNs in the frequency list
   * then we are required to periodically update CXM with a new list of registered.
   * ARFCNs.
   *
   */
   num_arfcn_to_register = garb_intf_get_arfcns_to_register(&frequency_list,gas_id);
   if (num_arfcn_to_register)
   {
     if (frequency_list.no_of_items > MAX_CXM_SCELL_ARFCNS)
     {
       /* num_arfcn_to_register will be either MAX_CXM_SCELL_ARFCNS or less if its not registration boundary */
       get_arfcns( 2, 1, num_arfcn_to_register, &frequency_list.channel[0], gas_id);
     }
     
     /* Trigger the MCS_CXM_WWAN_TECH_STATE_IND here to register all Freq IDs */
     garb_cxm_arfcn_indication(&frequency_list.channel[0], num_arfcn_to_register, CXM_LNK_DRCTN_UL_AND_DL, TRUE, gas_id);
   }

#ifdef FEATURE_WLAN_COEX_SW_CXM
   garb_cxm_high_priority_event_ind(0,0,CXM_HIGH_PRIO_UNKNOWN,gas_id);
#endif
}

#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_GSM_COEX
/*===========================================================================

FUNCTION      garb_intf_is_monitor_coex_protected

DESCRIPTION   This function is called to determine if the specified monitor
              will be protected from COEX interference.

              The determination is made according to a blanking pattern which
              maps TIER_10 and TIER_20 allocation across the 102 (SDCCH) or
              104 (TCH) multiframe.

              The blanking pattern is selected to meet the following
              criterea:

              No more than 3 x TIER_10 monitors per cell per 102(SDCCH)
              or 104(TCH) MF.

              Monitor reduction of 50% i.e. maximum monitor cadence of
              1:2 for TIER_10 vs TIER_20 allocation.

              For BA List Size > 16 :

              It is not possible to meet both the 3 x TIER 10 monitors per
              cell per MF and the 50% criterea here. In these scenarios we
              will be under-monitoring to the 3 monitors per cell per MF
              criterea.

              The blanking mask will give every other monitor position a
              TIER_10 allocation in a 1:2 cadence.

              For BA List Size <=16 :

              For TCH there are 96 monitor positions in the 104MF (IDLE and
              IDLE - 1 frames are excluded). The blanking pattern is defined
              across 32 monitor positions (3 x 32 = 96). The blanking pattern
              allows each ARFCN to have one TIER_10 allocation - which will
              result in 3 x protected meas per ARFCN per 104MF.

              For HR the 50 : 50 blanking pattern is aligned to protected
              the monitor in the TCH frame.

RETURN VALUE  TRUE if the monitor is COEX protected, FALSE otherwise.

===========================================================================*/
boolean garb_intf_is_monitor_coex_protected(uint8 FNMod_102_104, gas_id_t gas_id)
{
   boolean rc      = TRUE;
   uint8   monitor = 0;
   uint8   odd_or_even_protected = 0;

   /* If this GAS is not a COEX victim then all monitor positions have protection */
   if(!arbitrated_gas[gas_id].gas_is_coex_victim) {return rc;}

   /* The FNMod104 (TCH) needs to be converted into an index into the 32 frame */
   /* blanking pattern. First we need determine the position of the current */
   /* monitor within the 96 monitor positions in the 104MF (i.e. the position */
   /* within the current 104MF excluding the IDLE and IDLE-1 frames). This is */
   /* done by taking the number of elapsed 26MFs and multiplying by the number */
   /* of monitor positions within the 26MF (24) then adding on the number of frames */
   /* elapsed in the current 26MF. Finally we Mod32 the result to determing the */
   /* monitor position within the repeating 32 frame blanking pattern */
   if(arbitrated_gas[gas_id].state==GARB_CS)
   {
      monitor =  ( ( (FNMod_102_104 / 26 ) * COEX_MONSCAN_LENGTH_TCH_MF26 ) + (FNMod_102_104 % 26) ) % 33;

      odd_or_even_protected = (!arbitrated_gas[gas_id].params_cs.is_fr) && (arbitrated_gas[gas_id].params_cs.subchannel == 1);

      /* TIER 10 allocations for each cell in the BA list are placed from the start of the */
      /* blanking pattern at a 1:2 cadence - anything outside of this range is COEX unprotected */
      if( monitor >= (arbitrated_gas[gas_id].params_monscan.ba_list_size * COEX_MONSCAN_REDUCTION_FACTOR) )
      {
         rc = FALSE;
      }

      /* Enforce the 1:2 cadence for 0 >= monitor < (BA List Size * 2) */
      if((monitor % COEX_MONSCAN_REDUCTION_FACTOR)!=odd_or_even_protected)
      {
         rc = FALSE;
      }
   }
   else
   /* The FNMod102 (SDCCH) needs to be converted into an index into the 32 frame */
   /* blanking pattern. Here the index into the repeating 32 frame blanking */
   /* pattern is simply obtained directly from the number of elapsed frames in  */
   /* the current FNMod102 */
   if(arbitrated_gas[gas_id].state==GARB_CS_SDCCH)
   {
      monitor = FNMod_102_104 % 33;

      /* TIER 10 allocations for each cell in the BA list are placed from the start of the */
      /* blanking pattern at a 1:2 cadence - anything outside of this range is COEX unprotected */
      if( monitor >= (arbitrated_gas[gas_id].params_monscan.ba_list_size * COEX_MONSCAN_REDUCTION_FACTOR) )
      {
         rc = FALSE;
      }

      /* Enforce the 1:2 cadence for 0 >= monitor < (BA List Size * 2) */
      if((monitor % COEX_MONSCAN_REDUCTION_FACTOR)!=odd_or_even_protected)
      {
         rc = FALSE;
      }
   }
   /* In PS monitors are scheduled on FNMod13 0-10. This includes 3 serving cell */
   /* measurements and 8 NCELL measurements. The serving cell measurements are scheduled */
   /* on FNMod13 0, 5 and 9. */
   else
   if(arbitrated_gas[gas_id].state==GARB_PS)
   {
      uint8 FNMod13 = FNMod_102_104 % 13;
      /* Total num of protected monitors already scheduled this FNMod104 */
      monitor =  ( (FNMod_102_104 / 13 ) * COEX_MONSCAN_LENGTH_PS_MF13 );
      rc      =  FALSE;

      switch(FNMod13)
      {
      case 0:
         /* Protected Monitor (Serving) */
         rc = TRUE;
         break;
      case 1:
         /* Protected Monitor (BA list) */
         monitor += 1;
         rc = TRUE;
         break;
      case 3:
         /* Protected Monitor (BA list) */
         monitor += 2;
         rc = TRUE;
         break;
      case 5:
         /* Protected Monitor (Serving) */
         rc = TRUE;
         break;
      case 6:
         /* Protected Monitor (BA list) */
         monitor += 3;
         rc = TRUE;
         break;
      case 8:
         /* Protected Monitor (BA list) */
         monitor += 4;
         rc = TRUE;
         break;
      case 9:
         /* Protected Monitor (Serving) */
         rc = TRUE;
         break;
      default:
         break;
      }

      /* Measuring each ARFCN in the BA list twice per 104MF is deemed sufficient, remaining monitors will be unprotected */
      if(monitor >= (arbitrated_gas[gas_id].params_monscan.ba_list_size * 2) )
      {
         rc = FALSE;
      }
   }
   /* Monitor reduction only supported in TCH, SDCCH and PS */
   else
   {
      return rc;
   }

   return rc;
}

/*===========================================================================

FUNCTION      garb_intf_request_block_tx

DESCRIPTION   This function is used by the TCH state machines to check if
              a block Tx (e.g. FACCH or RATSCCH) can procede without
              encountering any COEX conflicts with high priority Rx's (e.g.
              PCH, SACCH etc).

RETURN VALUE  TRUE if the block Tx can proceed, FALSE otherwise

===========================================================================*/
static boolean garb_intf_request_block_tx(gl1_msg_chan_type channel_type, gas_id_t gas_id)
{
   boolean      rc = TRUE;

#ifdef FEATURE_GSM_COEX_TX_BLOCK_DELAY

   uint32       this_gas_fn       = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn;
   uint32       tx_block_duration = garb_get_tx_block_duration(channel_type, this_gas_fn, gas_id);
   uint32       this_tx_s_t       = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart;
   uint32       this_tx_e_t       = USTMR_ADD(this_tx_s_t, (tx_block_duration * USTMR_PER_FRAME));

   gas_id_t     peer_gas_id       = garb_peer_gas_id(gas_id);
   garb_state_t peer_gas_state    = arbitrated_gas[peer_gas_id].state;

   uint32       peer_activity_s_t = 0;
   uint32       peer_activity_e_t = 0;

   /* SS, DSDS and DR-DSDS modes do not required Tx block delay (this is for DSDA only) */
   if(gl1_msg_get_multi_sim_sys_mode()!=SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE) {return TRUE;}

   /* FACCH and RATSCCH are supported */
   switch(channel_type)
   {
   case GL1_MSG_FACCH:
   case GL1_MSG_FACCH_AFS:
   case GL1_MSG_FACCH_WFS:
   case GL1_MSG_FACCH_HS:
   case GL1_MSG_FACCH_AHS:
   case GL1_MSG_RATSCCH_FS:
   case GL1_MSG_RATSCCH_WFS:
   case GL1_MSG_RATSCCH_HS:
      break; /* Proceed with conflict detection */
   default:
      return TRUE;
   }

   switch(peer_gas_state)
   {
   case GARB_IDLE:
      // DR-DSDS and DSDA :  Delay block Tx for IDLE activities
      peer_activity_s_t = arbitrated_gas[peer_gas_id].params_idle.next_activity.ustmr_s_time;
      peer_activity_e_t = arbitrated_gas[peer_gas_id].params_idle.next_activity.ustmr_e_time;
      break;
   case GARB_CS:
      // TODO : DSDA G+G de-prioritised
      break;
   case GARB_CS_SDCCH:
      // TODO : DSDA G+G de-prioritised
      break;
   default:
      break;
   }

   if( USTMR_IS_CONFLICT(this_tx_s_t, this_tx_e_t, peer_activity_s_t, peer_activity_e_t) )
   {
      MSG_GERAN_HIGH_3_G("Block Tx @ustmr=%d delayed due to conflict with high priority Rx activity on GASID=0x%d @ustmr=%d", this_tx_s_t, peer_gas_id, peer_activity_s_t);
      rc = FALSE;
   }
#endif /* FEATURE_GSM_COEX_TX_BLOCK_DELAY */

   return rc;

}



/*===========================================================================

FUNCTION      garb_intf_gas_id_xpg_coex_agressor

DESCRIPTION   This function indicates if the GAS is a COEX agressor in XPG
              DSDA mode.

RETURN VALUE  TRUE if the GAS is a COEX agressor

===========================================================================*/
boolean garb_intf_gas_is_xpg_coex_agressor(gas_id_t gas_id)
{
   return ( (arbitrated_gas[gas_id].gas_is_coex_agressor) && (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDA) );
}

/*===========================================================================

FUNCTION      garb_intf_gas_id_xpg_coex_victim

DESCRIPTION   This function indicates if the GAS is a COEX victim in XPG
              DSDA mode.

RETURN VALUE  TRUE if the GAS is a COEX victim

===========================================================================*/
boolean garb_intf_gas_is_xpg_coex_victim(gas_id_t gas_id)
{
   return ( (arbitrated_gas[gas_id].gas_is_coex_victim) && (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDA) );
}
#endif /* FEATURE_GSM_COEX */
#endif /* FEATURE_DUAL_SIM */


#ifdef FEATURE_WLAN_COEX_SW_CXM
/*===========================================================================

FUNCTION      garb_intf_coex_update_active_policy

DESCRIPTION   This function updates CXM policies to GFW through MSGR when GFW
              is awake and CXM policy is getting updated for first time.
              CXM policy is bit-field type and updated to GL1 by MCS.

RETURN VALUE  NONE

===========================================================================*/
void garb_intf_coex_update_active_policy( gas_id_t gas_id )
{
  if( arbitrated_gas[gas_id].cxm_policy_updated &&
        gl1_hw_async_intf_cxm_policy_cmd( arbitrated_gas[gas_id].cxm_policy, gas_id )
    )
  {
    arbitrated_gas[gas_id].cxm_policy_updated = FALSE;
    MSG_GERAN_HIGH_0_G("cxm activie policy updated");
  }
}
#endif /* FEATURE_WLAN_COEX_SW_CXM */

/*===========================================================================

FUNCTION      garb_intf_update_timeline_at_wakeup_cancel

DESCRIPTION   This function updates CXM timlines at wakeup Cancel

RETURN VALUE  NONE

===========================================================================*/
void garb_intf_update_timeline_at_wakeup_cancel(gas_id_t gas_id )
{

   uint8 write_t_index = arbitrated_gas[gas_id].write_t_index;
   arbitrated_gas[gas_id].state = GARB_IDLE;
  
   garb_reset_timeline(write_t_index,gas_id);

   garb_append_frame_to_timeline(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME], GARB_TIMELINE_THIS_FRAME, write_t_index,gas_id);

   garb_get_frame_at_wakeup_conflict(ADD_FN(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn, 1), GARB_TIMELINE_NEXT_FRAME, gas_id);

   garb_append_frame_to_timeline(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_NEXT_FRAME], GARB_TIMELINE_NEXT_FRAME, write_t_index,gas_id);

   /* If there is a pending timeline notification then send it to CXM */
   garb_cxm_activity_timeline_indication(write_t_index,gas_id);

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
   garb_get_frame_at_wakeup_conflict(ADD_FN(arbitrated_gas[gas_id].frames[GARB_TIMELINE_NEXT_FRAME].fn, 1), GARB_TIMELINE_THIRD_FRAME, gas_id);

   garb_append_frame_to_timeline(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIRD_FRAME], GARB_TIMELINE_THIRD_FRAME, write_t_index,gas_id);

   garb_get_frame_at_wakeup_conflict(ADD_FN(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIRD_FRAME].fn, 1), GARB_TIMELINE_FOURTH_FRAME, gas_id);

   garb_append_frame_to_timeline(&arbitrated_gas[gas_id].frames[GARB_TIMELINE_FOURTH_FRAME], GARB_TIMELINE_FOURTH_FRAME, write_t_index,gas_id);
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

  /* Update the timleline write index */
  arbitrated_gas[gas_id].read_t_index = arbitrated_gas[gas_id].write_t_index;

  MSG_GERAN_HIGH_1_G("GARB_OPT:read_t_index =%d",arbitrated_gas[gas_id].read_t_index);

  /* Toggle Timeline buffer */
  arbitrated_gas[gas_id].write_t_index =  arbitrated_gas[gas_id].write_t_index ? 0 : 1;

  MSG_GERAN_DEBUG_OPT_1_G("GARB_OPT:write_t_index =%d",arbitrated_gas[gas_id].write_t_index);


}

#endif /* FEATURE_GSM_COEX_SW_CXM */
