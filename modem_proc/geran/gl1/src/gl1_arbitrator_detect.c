 /*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                    G L 1  A R B I T R A T O R  D E T E C T

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_arbitrator_detect.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
26/12/17   sn        CR2160741: Gl1 Coex timeline updation at Wakeup Cancel of Idle Sub
23/06/17   mn        CR2055160 Fix for regression caused by CR2009497(PDTCH last burst over PCH first burst)
20/07/15   shr       CR866607 Observing yield/proceed on both subs when in collision during data transfer on one sub
26/05/15   ab        CR841593 : GL1 COEX - G+G DSDS - RRBP_HI is YIELED when PCH RX collides with 3 TX slots
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
15/05/15   sp       CR838865: Fix compile errors on TA20 build.
14/05/15   mc        CR.821519 : TH 2.0: WLAN COEX timeline reporting errors for FACCH and SDCCH
06/05/15   mc        CR833173 : Bad monitor placement no-zones seen in G[PSCAN]+G[IDLE] causing PSCAN failures
17/03/15   ab        GL1 COEX : CR804948 Handling of multiple RRBP during conflict resolution
07/04/15   shr       CR808932: COEX support for DTM
13/03/15   mc        CR773788 : COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
03/12/14   npt       CR759467 - Fix compiler warnings
29/10/14   ab        CR745429 - Featurization correction for PCHXFER for SS / CR merges.
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
23/08/14   mc        CR707745 : [BOLT2.0][SG+G]: Incorrect CXM priority assignment policy in use for SGLTE+G
22/08/14   sp        CR687227: Return burst number for Idle activity based on FM reserved time.
04/08/14   mc        CR703283 Insane no-zone calculation triggers gfw_burst_scheduler.cc:801
                     Assertion (0 <= burstRxTxInfo.lastRx && burstRxTxInfo.lastRx <= TS7) failed
02/07/14   mc        CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
27/01/14   pjr       CR.579214 : COEX : Prioritise PDTCH over TCH during 2 phase access
23/01/14   js        CR584210: DSDA : GERAN VBATT support for PS allowing CS+PS VBATT detection.
14/01/14   og        CR589868. T+G DSDA support.
12/02/14   am        CR611265 RACH timing update to GARB
24/01/14   ab        CR 602373 - DSDA DSDA RRBP handling as GFW supports enhance TX scheduling
13/12/13   mc        CR.583042 : COEX : DSDA CS + PS - PS call drops in RxTx overlap scenario
05/12/13   mc        CR.583009 : COEX : Fix KW buffer read overflow error in monitor no-zone
10/12/13   ws        CR 587586 - Remove DUAL_SIM featurisation in API
03/12/13   sk        CR584247 Ensure correct sys mode passed to GL1 Arbitrator
29/11/13   js        CR.574402 : COEX : VBATT : Accurate prediction of N+1 frame Tx power for CS (TCH and SDCCH)
09/11/13   mc        CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
09/11/13   mc        CR.556742 : COEX : G+G : Enhancements for monitor positioning and improved SDCCH/FACCH conflict detection
15/11/13   ap        CR575548 FR 15265: DSDA L/TDS/G+G with Opportunistic SGLTE/SGTDS
10/10/13   ab        CR 554808 DSDA Geran is booking all the 4 tx slots with tier 1 when W is in connected mode
01/10/13   sk        CR552259 CR sync from 1.0/1.1
06/09/13   ab        CR 538322 DSDx GL1 ARB should account for 10 slots when doing FCCH in Pkt Transfer.
23/09/13   sk        2.0 syncup from 1.1
15/08/13   sk        CR524039 GBTA bringup changes
29/07/13   sk        CR519663 Partial QBTA bringup changes
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
25/03/13   mc        CR.464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_GSM_COEX

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "gl1_arbitrator_types.h"
#include "gl1_arbitrator_detect.h"
#include "gl1_arbitrator_utils.h"
#include "gl1_arbitrator_tables.h"
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_if_g.h"
#include "gmacsigl1.h"
#include "gmacsigl1_g.h"
#include "gpl1_gprs_rrbp.h"
#include "l1_isr.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_ptcch.h"
#include "l1_fm.h"
#include "l1_utils.h"

#include "gtmrs.h"
#include "gtmrs_g.h"

#include "gl1_msg_g.h"
#include "gpl1_gprs_utils.h"
#include "gpl1_gprs_serving_cell.h"



/*===========================================================================

                            LOCAL DEFINES

===========================================================================*/
extern garb_arbitrated_gas_t arbitrated_gas[NUM_GERAN_DATA_SPACES];
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
extern void gpl1_get_cell_information(int16 *qbit_lag,ARFCN_T *arfcn,gpl1_g2x_ta_priority_t pri,gas_id_t gas_id);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

/*===========================================================================

                     PROTOTYPES FOR LOCAL FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION      garb_detect_and_resolve_rx_conflicts

DESCRIPTION   Detects any conflict between Tx activities on this GAS and
              activities on the peer GAS. Performs arbitration if a conflict
              is detected.

              In the case of DSDA a conflict could be an RxTx desense - RxRx
              conflicts are not a concern in DSDA.

              This function will set the CXM priority tier for the specified
              activity. If there is a conflict then the CXM priority tier
              will be selected based on conflict resolution, otherwise the
              selected CXM priority will be a default based on channel type.

RETURN VALUE  None

===========================================================================*/
void garb_detect_and_resolve_rx_conflicts(garb_frame_t* frame, uint8 ts, gas_id_t gas_id)
{
   uint32   this_s_time  = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].s_time;
   uint32   this_e_time  = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].e_time;
   uint32   this_channel = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].channel_type;
   gas_id_t peer_gas_id  = garb_peer_gas_id(gas_id);
   uint32   i            = 0;
   uint8    peer_conflict_read_t_index;

   /* There will only be conflicts to resolve if another GAS is registered */
   if( !garb_is_g_plus_g() ) { return; }

   /* Read timeline entries for peer */
   peer_conflict_read_t_index = arbitrated_gas[peer_gas_id].read_t_index;

   /* Iterate through the timeline of the peer GAS and look for conflicts */
   /* Iterate only 2 Frame Timeline Events */
   for(i=0; 
            ( ( i < arbitrated_gas[peer_gas_id].timeline_num_entries[peer_conflict_read_t_index]) && 
              (arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].frame_ref_count < GARB_TIMELINE_MAX_FRAMES_CONFLICTING));
            i++)
   {
      uint32  peer_s_time  = arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].s_time;
      uint32  peer_e_time  = arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].e_time;
      uint32  peer_channel = arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].channel_type;
      boolean peer_rxtx    = arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].rxtx;

      if( !USTMR_IS_CONFLICT( this_s_time, this_e_time, peer_s_time, peer_e_time) ) 
	  	{
         MSG_GERAN_DEBUG_OPT_4_G("GARB_OPT:Rx No_ustmr_confict this_s_time =%d this_e_time=%d peer_channel=%d peer_e_time=%d" ,this_s_time,this_e_time,peer_s_time,peer_e_time);
		 continue;
		}

      if( (peer_rxtx==GARB_RX) && IS_VALID_CHANNEL(peer_channel) && IS_VALID_CHANNEL(this_channel) && (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDA))
      {
         /*******************************
         *
         * DSDS RxRx Conflict (GBTA)
         * TSTS RxRx Conflict (GBTA)
         *
         ********************************/
         cxm_activity_type peer_cxm_tier = ACTIVITY_TIER_20;

         (*garb_resolve_conflict_lut[this_channel][peer_channel]) (ts, TRUE, i, &peer_cxm_tier, peer_conflict_read_t_index,gas_id);

         MSG_GERAN_HIGH_3_G("GARB : CONFLICT : RxRx : [This Channel=%d][Peer Channel=%d](%d)", this_channel, peer_channel, ts);
      }
      if (peer_rxtx == GARB_TX && IS_VALID_CHANNEL(peer_channel) && IS_VALID_CHANNEL(this_channel))
      {
         /*******************************
         *
         * DSDA RxTx Conflict (De-sense)
         * DSDS RxTx Conflict (GBTA)
         * TSTS RxTx Conflict (GBTA)
         *
         ********************************/
         cxm_activity_type peer_cxm_tier = ACTIVITY_TIER_20;

         (*garb_resolve_conflict_lut[this_channel][peer_channel]) (ts, TRUE, i, &peer_cxm_tier, peer_conflict_read_t_index,gas_id); 

         MSG_GERAN_HIGH_3_G("GARB : CONFLICT : RxTx : [This Channel=%d][Peer Channel=%d](%d)", this_channel, peer_channel, ts);
      }
   }
}

/*===========================================================================

FUNCTION      garb_detect_and_resolve_tx_conflicts

DESCRIPTION   Detects any conflict between Tx activities on this GAS and
              activities on the peer GAS. Performs arbitration if a conflict
              is detected.

              In the case of DSDA a conflict could be an RxTx desense or TxTx
              VBATT droop or SAR violation.

              This function will set the CXM priority tier for the specified
              activity. If there is a conflict then the CXM priority tier
              will be selected based on conflict resolution, otherwise the
              selected CXM priority will be a default based on channel type.

RETURN VALUE  None

===========================================================================*/
void garb_detect_and_resolve_tx_conflicts(garb_frame_t* frame, uint8 ts, gas_id_t gas_id)
{
   uint32   this_s_time  = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].s_time;
   uint32   this_e_time  = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].e_time;
   uint32   this_channel = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].channel_type;
   gas_id_t this_gas_id  = gas_id;
   gas_id_t peer_gas_id  = garb_peer_gas_id(gas_id);
   uint32   i            = 0;
   uint8    peer_conflict_read_t_index;

   /* There will only be conflicts to resolve if another GAS is registered */
   if( !garb_is_g_plus_g() ) { return; }

   /* Read timeline entries for peer */
   peer_conflict_read_t_index = arbitrated_gas[peer_gas_id].read_t_index;

   /* Iterate through the timeline of the peer GAS and look for conflicts */
   for(i=0; 
           ( ( i < arbitrated_gas[peer_gas_id].timeline_num_entries[peer_conflict_read_t_index]) && 
             (arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].frame_ref_count < GARB_TIMELINE_MAX_FRAMES_CONFLICTING));
           i++)

   {
      uint32  peer_s_time   = arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].s_time;
      uint32  peer_e_time   = arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].e_time;
      uint32  peer_channel  = arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].channel_type;
      boolean peer_rxtx     = arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].rxtx;

      if( !USTMR_IS_CONFLICT( this_s_time, this_e_time, peer_s_time, peer_e_time) ) 
	  	{
	  	MSG_GERAN_DEBUG_OPT_4_G("GARB_OPT:Tx No_ustmr_confict this_s_time =%d this_e_time=%d peer_channel=%d peer_e_time=%d" ,this_s_time,this_e_time,peer_s_time,peer_e_time);
	  	continue;
		}

      if( (peer_rxtx==GARB_TX) && IS_VALID_CHANNEL(peer_channel) && IS_VALID_CHANNEL(this_channel) && (arbitrated_gas[gas_id].multi_sim_mode==GARB_MODE_GPG_DSDA) )
      {
         /*******************************
         *
         * DSDA TxTx Conflict (VBATT)
         *
         *******************************/
         cxm_activity_type peer_cxm_tier = ACTIVITY_TIER_20;

         double TxP_SUB1_backoff_dB;
         double TxP_SUB2_backoff_dB;

         uint8  pri_SUB1 = 0;
         uint8  pri_SUB2 = 0;

         cxm_activity_type this_cxm_tier = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

         (*garb_resolve_conflict_lut[this_channel][peer_channel]) (ts, FALSE, i, &peer_cxm_tier, peer_conflict_read_t_index,gas_id);

         pri_SUB1 = arbitrated_gas[this_gas_id].cxm_activity_table[this_cxm_tier].priority;
         pri_SUB2 = arbitrated_gas[peer_gas_id].cxm_activity_table[peer_cxm_tier].priority;

         if(garb_get_vbatt_tx_power_backoffs(
            arbitrated_gas[this_gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.power,
            arbitrated_gas[peer_gas_id].timeline[peer_conflict_read_t_index][i].power,
            pri_SUB1,
            pri_SUB2,
            &TxP_SUB1_backoff_dB,
            &TxP_SUB2_backoff_dB,
            gas_id ) )
         {
            arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].backoff_vbatt = (int16)(TxP_SUB1_backoff_dB*100); /* dBm100 */
         }

         MSG_GERAN_HIGH_3_G("GARB : CONFLICT : TxTx : [This Channel=%d][Peer Channel=%d](%d)", this_channel, peer_channel, ts);
      }
      else if(IS_VALID_CHANNEL(peer_channel) && IS_VALID_CHANNEL(this_channel))
      {
         /*******************************
         *
         * DSDA TxRx Conflict (De-sense)
         * DSDS TxRx Conflict (GBTA)
         * DSDS TxTx Conflict (Not Used)
         * TSTS TxRx Conflict (GBTA)
         * TSTS TxTx Conflict (Not Used)
         *
         *******************************/

         cxm_activity_type peer_cxm_tier = ACTIVITY_TIER_20;

         (*garb_resolve_conflict_lut[this_channel][peer_channel]) (ts, FALSE, i, &peer_cxm_tier, peer_conflict_read_t_index,gas_id);

         MSG_GERAN_HIGH_3_G("GARB : CONFLICT : TxRx : [This Channel=%d][Peer Channel=%d](%d)", this_channel, peer_channel, ts);
      }
   }

}

/*===========================================================================

FUNCTION      garb_detect_and_resolve_monitor_conflicts

DESCRIPTION   Detects any conflict between monitor activities on this GAS
              and activities on the peer GAS. Performs monitor arbitration
              if a conflict is detected.

RETURN VALUE  TRUE if no-zones were configured, FALSE otherwise

===========================================================================*/
boolean garb_detect_and_resolve_monitor_conflicts(gas_id_t gas_id)
{
   uint32   frame_s_time = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart;
   uint32   frame_e_time = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefEnd;
   uint32   s_tx_time    = 0;
   uint32   e_tx_time    = 0;
   uint32   nozone_count = 0;
   uint32   i            = 0;
   uint8    peer_conflict_read_t_index;
   gas_id_t peer_gas_id  = garb_peer_gas_id(gas_id);

   /* The no-zones for monitor positioning on this SUB are configured based on the position of the Tx's on the peer SUB */
   for(i=0;i<GARB_TIMELINE_MAX_FRAMES_CONFLICTING;i++)
   {
      /* Read timeline entries for peer */
      peer_conflict_read_t_index = arbitrated_gas[garb_peer_gas_id(gas_id)].read_t_index;
            
      /* If there are no Tx'x configured on this timeline entry then skip to the next iteration */
      if(arbitrated_gas[peer_gas_id].timeline_tx[peer_conflict_read_t_index][i].configured==FALSE) {continue;}

      /* Initialise the start and end time of the Tx */
      s_tx_time = arbitrated_gas[peer_gas_id].timeline_tx[peer_conflict_read_t_index][i].s_time;
      e_tx_time = arbitrated_gas[peer_gas_id].timeline_tx[peer_conflict_read_t_index][i].e_time;

      /* First check if the Tx overlaps with the current frame */
      if( !USTMR_IS_CONFLICT(frame_s_time, frame_e_time, s_tx_time, e_tx_time) ) {continue;}

      /* If the Tx started before the current frame then set the no-zone start time to zero */
      if(USTMR_IS_LATER(frame_s_time, s_tx_time) )
      {
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].configured= TRUE;
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].s_time = 0;
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].e_time = USTMR_SUB(e_tx_time, frame_s_time);
      }
      else
      /* If the Tx ends after the current frame then set the end time to QS_PER_FRAME */
      if(USTMR_IS_LATER(e_tx_time, frame_e_time))
      {
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].configured = TRUE;
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].s_time = USTMR_SUB(s_tx_time, frame_s_time);
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].e_time = USTMR_PER_FRAME;
      }
      /* If the Tx starts and ends within the current frame the set the no-zone to match the Tx */
      else
      {
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].configured = TRUE;
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].s_time = USTMR_SUB(s_tx_time, frame_s_time);
         arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].monitor_nozone[nozone_count].e_time = USTMR_SUB(e_tx_time, frame_s_time);
      }

      nozone_count++;
   }

   return (nozone_count != 0);
}

/*===========================================================================

FUNCTION      garb_reset_timeline

DESCRIPTION   This function clears the timeline of the supplied GAS.

RETURN VALUE  None

===========================================================================*/
void garb_reset_timeline(uint8 write_t_index, gas_id_t gas_id)
{
   MSG_GERAN_DEBUG_OPT_1_G("GARB_OPT:garb_reset_timeline write_t_index=%d",write_t_index);
   arbitrated_gas[gas_id].timeline_num_entries[write_t_index] = 0;
   arbitrated_gas[gas_id].timeline_rx[write_t_index][GARB_TIMELINE_THIS_FRAME].configured = FALSE;
   arbitrated_gas[gas_id].timeline_rx[write_t_index][GARB_TIMELINE_NEXT_FRAME].configured = FALSE;

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
   arbitrated_gas[gas_id].timeline_rx[write_t_index][GARB_TIMELINE_THIRD_FRAME].configured  = FALSE;
   arbitrated_gas[gas_id].timeline_rx[write_t_index][GARB_TIMELINE_FOURTH_FRAME].configured = FALSE;
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

   arbitrated_gas[gas_id].timeline_tx[write_t_index][GARB_TIMELINE_THIS_FRAME].configured = FALSE;
   arbitrated_gas[gas_id].timeline_tx[write_t_index][GARB_TIMELINE_NEXT_FRAME].configured = FALSE;

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
   arbitrated_gas[gas_id].timeline_tx[write_t_index][GARB_TIMELINE_THIRD_FRAME].configured  = FALSE;
   arbitrated_gas[gas_id].timeline_tx[write_t_index][GARB_TIMELINE_FOURTH_FRAME].configured = FALSE;
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

}

/*===========================================================================

FUNCTION      garb_is_slot_contiguous_to_timeline_entry

DESCRIPTION   This function determines if the specified event and timeline
              entry are contiguos and identical.

              This is used when placing a new event onto the timeline to
              determine if the event requires a new timeline entry or can
              be amalgamated with the existing timeline entry by extending
              the end time of the existing entry.

RETURN VALUE  TRUE is the specified event is contiguous and identical
              to the specified timeline entry.

===========================================================================*/
static boolean garb_is_event_contiguous_to_timeline_entry(garb_slot_t* event, garb_rxtx_t rxtx, garb_activity_t* activity, gas_id_t gas_id)
{
   boolean rc = FALSE;

   if(USTMR_IS_CONFLICT(event->s_time, event->e_time, activity->s_time, activity->e_time))
   {
      rc = ( ( activity->rxtx         == rxtx                ) &&
             ( activity->arfcn.num    == event->arfcn.num    ) &&
             ( activity->arfcn.band   == event->arfcn.band   ) &&
             ( activity->channel_type == event->channel_type ) );
   }

   return rc;
}

/*===========================================================================

FUNCTION      garb_append_event_to_timeline

DESCRIPTION   This function appends the specified events to the timeline
              of the supplied GAS.

RETURN VALUE  None

===========================================================================*/
static void garb_append_events_to_timeline(garb_frame_t* frame, garb_rxtx_t rxtx, garb_rx_mon_type_t rx_type,uint8 index, uint8 write_t_index,gas_id_t gas_id)
{
   uint8 count = arbitrated_gas[gas_id].timeline_num_entries[write_t_index], start_index = count, i;
   garb_slot_t* events;
   uint32 fn = frame->fn;
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
   gpl1_g2x_ta_priority_t pri_ongoing;
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

  MSG_GERAN_DEBUG_OPT_4_G("GARB_OPT:garb_append_events_to_timeline rxtx=%d,rx_type=%d,index=%d ,write_t_index =%d",rxtx,rx_type,index,write_t_index);
 
  /* Obtain the Events from the timeslots configured */
   if (rxtx == GARB_RX) 
   {
     if (rx_type == TYPE_RX)
     {
       events = &frame->rx.slots[0];
     } 
#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
     else //TYPE_RX_MON
     {
       events = &frame->mons.mon[0];
     }
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */
   }
   else // (rxtx == GARB_TX) 
   {
     events = &frame->tx.slots[0];
   }

   for(i=0;i<GL1_DEFS_SLOTS_IN_FRAME;i++)
   {
      if(events[i].configured==FALSE) {continue;}

      if( (count==0) || !garb_is_event_contiguous_to_timeline_entry(&events[i], rxtx, &arbitrated_gas[gas_id].timeline[write_t_index][count-1], gas_id) )
      {
         /* This event is not contiguous with the previous timeline entry - append a new entry to the timeline */
         arbitrated_gas[gas_id].timeline[write_t_index][count].rxtx         = rxtx;
         arbitrated_gas[gas_id].timeline[write_t_index][count].fn           = ADD_FN(fn,1);  // It is the over the AIR FN
         arbitrated_gas[gas_id].timeline[write_t_index][count].arfcn        = events[i].arfcn;
         arbitrated_gas[gas_id].timeline[write_t_index][count].link_type    = events[i].link_type;
         arbitrated_gas[gas_id].timeline[write_t_index][count].channel_type = events[i].channel_type;
         arbitrated_gas[gas_id].timeline[write_t_index][count].burst_num    = events[i].burst_num;
         arbitrated_gas[gas_id].timeline[write_t_index][count].s_time       = events[i].s_time;
         arbitrated_gas[gas_id].timeline[write_t_index][count].e_time       = events[i].e_time;
         arbitrated_gas[gas_id].timeline[write_t_index][count].cxm_tier     = events[i].cxm_tier;
         arbitrated_gas[gas_id].timeline[write_t_index][count].power        = (rxtx==GARB_RX) ? 0 : frame->tx.power;
         arbitrated_gas[gas_id].timeline[write_t_index][count].frame_ref_count = index;
         MSG_GERAN_DEBUG_OPT_5_G("GARB_OPT:apnd_timline1 :burst_num=%d channel_type=%d fn=%d write_t_index =%d cxm_tier = %d",
		 	                arbitrated_gas[gas_id].timeline[write_t_index][count].burst_num,
		 	                arbitrated_gas[gas_id].timeline[write_t_index][count].channel_type,
		 	                arbitrated_gas[gas_id].timeline[write_t_index][count].fn,
		 	                write_t_index,
		 	                arbitrated_gas[gas_id].timeline[write_t_index][count].cxm_tier);
		 	  MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT:apnd_timline2:s_time=%d e_time=%d ",
		 	                arbitrated_gas[gas_id].timeline[write_t_index][count].s_time,
		 	                arbitrated_gas[gas_id].timeline[write_t_index][count].e_time
		 	                );
         count++;
      }
      else
      {
         /* This Rx slot is contiguous with the most recent timeline entry - extend the duration of the existing entry */
         arbitrated_gas[gas_id].timeline[write_t_index][count-1].e_time     = events[i].e_time;
		 MSG_GERAN_DEBUG_OPT_1_G("GARB_OPT:Rx_slot is contiguous e_time=%d" , arbitrated_gas[gas_id].timeline[write_t_index][count-1].e_time);
      }
   }

   if( (rxtx==GARB_RX) && (count != arbitrated_gas[gas_id].timeline_num_entries[write_t_index]) && (rx_type == TYPE_RX) )
   {
      arbitrated_gas[gas_id].timeline_rx[write_t_index][index].configured = TRUE;
      arbitrated_gas[gas_id].timeline_rx[write_t_index][index].s_time     = arbitrated_gas[gas_id].timeline[write_t_index][start_index].s_time;
      arbitrated_gas[gas_id].timeline_rx[write_t_index][index].e_time     = arbitrated_gas[gas_id].timeline[write_t_index][count-1].e_time;
	  MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT:rxtx=%d write_t_index=%d" ,rxtx,write_t_index);
   }
   else
   if( (rxtx==GARB_TX) && (count != arbitrated_gas[gas_id].timeline_num_entries[write_t_index]) )
   {
      arbitrated_gas[gas_id].timeline_tx[write_t_index][index].configured = TRUE;
      arbitrated_gas[gas_id].timeline_tx[write_t_index][index].s_time     = arbitrated_gas[gas_id].timeline[write_t_index][start_index].s_time;
      arbitrated_gas[gas_id].timeline_tx[write_t_index][index].e_time     = arbitrated_gas[gas_id].timeline[write_t_index][count-1].e_time;
	  MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT:rxtx=%d write_t_index=%d" ,rxtx,write_t_index);
   }

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
   if( (arbitrated_gas[gas_id].multi_sim_mode == GARB_MODE_GPG_DSDA) &&
       (arbitrated_gas[gas_id].state == GARB_PS)  &&
       (rx_type  == TYPE_RX)                      && //perform only once per call of this func.
       (gpl1_g2x_ta_is_activty_running(&pri_ongoing,gas_id)))
    {
      /* G2X_TA_PRI_SERV_BCCH - Is now ALWAYS a GBTA Type Activity  -> ASYNC_ACTIVITY_GBTA*/
      if ((pri_ongoing == G2X_TA_PRI_SERV_PCH) || (pri_ongoing == G2X_TA_PRI_SERV_BCCH))
      {
        /* get  qbit lag */
        uint32                 s_ccch_time,e_ccch_time,alpha;
        uint32                 offset;
        ARFCN_T                ccch_arfcn;
        int16                  qbit_lag;
        
        gpl1_get_cell_information(&qbit_lag,&ccch_arfcn,pri_ongoing,gas_id);

        /* alpha  always non zero becuase we dont know if ccch_arfcn and pdtch have same  arfcn */
        alpha   = USTMR_FROM_QS(gl1_hw_get_rf_gsm_tx_setup_time(gas_id));
        
        /* Calculate start time */
        s_ccch_time  = USTMR_ADD(frame->UstmrRefStart, (qbit_lag)*USTMR_PER_QS);
        offset  = USTMR_SLOT_GUARD_PERIOD + alpha + USTMR_DEFAULT_SEARCH_WIDTH;
        s_ccch_time  = USTMR_SUB(s_ccch_time, offset);

        /* Calculate end time */
        e_ccch_time  = USTMR_ADD(frame->UstmrRefStart, (qbit_lag+QS_PER_SLOT)*USTMR_PER_QS);

        offset  = USTMR_SLOT_GUARD_PERIOD + USTMR_DEFAULT_SEARCH_WIDTH +

        ((gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY) ? USTMR_FROM_QS(GL1_FW_EXTENDED_GUARD_PERIOD * 4) : 0);

        e_ccch_time  = USTMR_ADD(e_ccch_time, offset);
   
        /* count already incremented */
        arbitrated_gas[gas_id].timeline[write_t_index][count].rxtx            = GARB_RX;
        arbitrated_gas[gas_id].timeline[write_t_index][count].fn              = ADD_FN(fn,1);  // It is the over the AIR FN
        arbitrated_gas[gas_id].timeline[write_t_index][count].arfcn           = ccch_arfcn;
        arbitrated_gas[gas_id].timeline[write_t_index][count].channel_type    = GARB_CHAN_DL_CCCH;
        arbitrated_gas[gas_id].timeline[write_t_index][count].link_type       = CXM_LNK_TYPE_UNUSED;
        arbitrated_gas[gas_id].timeline[write_t_index][count].burst_num       = gpl1_get_burst_num_from_FN_in_IDLE(ADD_FN(fn,1),TRUE,gas_id);
        arbitrated_gas[gas_id].timeline[write_t_index][count].s_time          = s_ccch_time;
        arbitrated_gas[gas_id].timeline[write_t_index][count].e_time          = e_ccch_time;
        arbitrated_gas[gas_id].timeline[write_t_index][count].cxm_tier        = ACTIVITY_TIER_10;
        arbitrated_gas[gas_id].timeline[write_t_index][count].power           = 0;
        arbitrated_gas[gas_id].timeline[write_t_index][count].frame_ref_count = index;

        count ++;
      }
    }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */

   arbitrated_gas[gas_id].timeline_num_entries[write_t_index] = count;
   MSG_GERAN_DEBUG_OPT_1_G("GARB_OPT:timeline_num_entries=%d" , arbitrated_gas[gas_id].timeline_num_entries[write_t_index]);
}

/*===========================================================================

FUNCTION      garb_append_frame_to_timeline

DESCRIPTION   This function appends the supplied frame to the timeline
              of the supplied GAS.

RETURN VALUE  None

===========================================================================*/
void garb_append_frame_to_timeline(garb_frame_t* frame, uint8 index, uint8 write_t_index, gas_id_t gas_id)
{
   garb_append_events_to_timeline(frame, GARB_RX, TYPE_RX,index, write_t_index,gas_id);

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
   garb_append_events_to_timeline(frame, GARB_RX, TYPE_RX_MON,index, write_t_index,gas_id);
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

   garb_append_events_to_timeline(frame,  GARB_TX, TYPE_RX_INVALID,index, write_t_index,gas_id);
}

/*===========================================================================

FUNCTION      garb_get_frame

DESCRIPTION   Builds the anticipated frame for the specified FN and GAS.

RETURN VALUE  None

===========================================================================*/
boolean garb_get_frame(uint32 fn, uint8 frame_index, gas_id_t gas_id)
{
   boolean rc = FALSE;
   garb_frame_t* frame = &arbitrated_gas[gas_id].frames[frame_index];

   uint32 i;

   uint8   TN = 0;

   ASSERT( frame_index < GARB_TIMELINE_MAX_FRAMES );

   memset(frame, 0, sizeof(garb_frame_t) );

   frame->fn            = fn;
   frame->UstmrRefStart = USTMR_ADD(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart, (SUB_FN(fn, GSTMR_GET_FN_GERAN(gas_id))*USTMR_PER_FRAME) );
   frame->UstmrRefEnd   = USTMR_ADD(frame->UstmrRefStart, USTMR_PER_FRAME);

   MSG_GERAN_DEBUG_OPT_3_G("GARB_OPT:garb_get_frame:fn =%d frame->UstmrRefStar=%d this_frame_ustart = %d",
      frame->fn, frame->UstmrRefStart ,arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart);
   
   switch(arbitrated_gas[gas_id].state)
   {
   /*************
    * IDLE
   *************/
   case GARB_IDLE:
      {
         l1_fm_priority_T next_activity = l1_fm_get_next_activity(&arbitrated_gas[gas_id].params_idle.next_activity, gas_id);
         
         frame->fn            = arbitrated_gas[gas_id].params_idle.next_activity.fn;
         frame->UstmrRefStart = USTMR_ADD(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart, (SUB_FN(frame->fn, GSTMR_GET_FN_GERAN(gas_id))*USTMR_PER_FRAME) );

          MSG_GERAN_DEBUG_OPT_3_G("GARB_OPT:garb_get_frame:fn =%d frame->UstmrRefStar=%d  this_frame_ustart = %d",
      frame->fn, frame->UstmrRefStart ,arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart);
         
         if( next_activity == FM_PRI_CCCH)
         {
            frame->rx.configured            = TRUE;
            frame->rx.search_width          = USTMR_DEFAULT_SEARCH_WIDTH;
            frame->rx.num_slots             = 1;

            /* Initialise the slot parameters */
            frame->rx.slots[0].configured   = TRUE;
            frame->rx.slots[0].channel_type = GARB_CHAN_DL_CCCH;
            frame->rx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(frame->fn, 1), TRUE, gas_id);
            frame->rx.slots[0].s_time       = garb_get_slot_s_time(frame, GARB_RX, 0, gas_id);
            frame->rx.slots[0].e_time       = garb_get_slot_e_time(frame, GARB_RX, 0, gas_id);
            frame->rx.slots[0].cxm_tier     = MAX_ACTIVITY_TIERS;

            rc = TRUE;
         }
         else
         if( (next_activity == FM_PRI_NCELL_ACQ) || (next_activity == FM_PRI_NCELL_SCH) || (next_activity == FM_PRI_NCELL_BCCH) )
         {
            /* Initialise the frame parameters for NCELL activity */
            frame->rx.configured            = TRUE;
            frame->rx.num_slots             = 1;
            frame->rx.slots[0].configured   = TRUE;
            frame->rx.slots[0].channel_type = GARB_CHAN_NCELL_LO;
            frame->rx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
            frame->rx.slots[0].s_time       = frame->UstmrRefStart;
            frame->rx.slots[0].e_time       = USTMR_ADD(frame->rx.slots[0].s_time, USTMR_PER_FRAME);
            frame->rx.slots[0].cxm_tier     = MAX_ACTIVITY_TIERS;

            rc = TRUE;
         }
         break;
      }
   /*************
   * ACCESS
   ***************/
   case GARB_ACCESS:
      {
         if( garb_is_rach_frame( fn, gas_id ) )
         {
            /* Initialise the frame parameters and the Tx slot */
            frame->tx.configured            = TRUE;
            frame->tx.num_slots             = 1;
            frame->tx.power                 = txlev_to_dBm[arbitrated_gas[gas_id].params_access.arfcn.band][arbitrated_gas[gas_id].params_access.txlev];
            frame->tx.slots[0].configured   = TRUE;
            frame->tx.slots[0].channel_type = GARB_CHAN_UL_RACH;
            frame->tx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), FALSE, gas_id);
            frame->tx.slots[0].s_time       = garb_get_slot_s_time(frame, GARB_TX, 0, gas_id);
            frame->tx.slots[0].e_time       = garb_get_slot_e_time(frame, GARB_TX, 0, gas_id);
            frame->tx.slots[0].cxm_tier     = MAX_ACTIVITY_TIERS;

            rc = TRUE;
         }
         else if( gl1_msg_ccch_is_rx_on_next_frame( gas_id ) ) /* Ask the CCCH message layer module */
         {
            /* Initialise the frame parameters and the Rx slot */
            frame->rx.configured            = TRUE;
            frame->rx.num_slots             = 1;
            frame->rx.slots[0].configured   = TRUE;
            frame->rx.slots[0].channel_type = GARB_CHAN_DL_CCCH;
            frame->rx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
            frame->rx.slots[0].s_time       = garb_get_slot_s_time(frame, GARB_RX, 0, gas_id);
            frame->rx.slots[0].e_time       = garb_get_slot_s_time(frame, GARB_RX, 0, gas_id);
            frame->rx.slots[0].cxm_tier     = MAX_ACTIVITY_TIERS;

            rc = TRUE;
         }
         break;
      }
   /*************
   * CS (SDCCH)
   **************/
   case GARB_CS_SDCCH:
      {
#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
         uint32 FnMod102 = fn % 102;
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */
         /* Timeline built for SDCCH is derived from the sub-channel and SDCCH4 / SDCCH8 look-up tables */
         if(!garb_is_sdcch_saach_frame(fn, frame_index, gas_id)) { break; }

         if( arbitrated_gas[gas_id].params_sdcch.channels[frame_index].dl_channel == GARB_CHAN_ULDL_SDCCH)
         {
            /* Initialise the frame parameters and the Rx slot */
            frame->rx.configured            = TRUE;
            frame->rx.search_width          = USTMR_TCH_SEARCH_WIDTH;
            frame->rx.slots[0].configured   = TRUE;
            frame->rx.slots[0].channel_type = arbitrated_gas[gas_id].params_sdcch.channels[frame_index].dl_channel;
            frame->rx.slots[0].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
            frame->rx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
            frame->rx.slots[0].s_time       = garb_get_slot_s_time(frame, GARB_RX, 0, gas_id);
            frame->rx.slots[0].e_time       = garb_get_slot_e_time(frame, GARB_RX, 0, gas_id);
            frame->rx.slots[0].cxm_tier     = garb_get_cxm_priority_tier(GARB_RX, frame->rx.slots[0].channel_type, 0, gas_id);
            frame->rx.num_slots             = 1;
         }
         else
         if( arbitrated_gas[gas_id].params_sdcch.channels[frame_index].ul_channel == GARB_CHAN_ULDL_SDCCH)
         {
            /* Initialise the frame parameters and the Tx slot */
            frame->tx.configured            = TRUE;
            frame->tx.timing_advance        = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.timing_advance;
            frame->tx.power                 = L1_convert_power_idx_to_dBm(arbitrated_gas[gas_id].params_sdcch.frequency_list.channel[0].band, L1_get_txlev_for_FN(fn % 104,gas_id));
            frame->tx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), FALSE, gas_id);
            frame->tx.slots[0].configured   = TRUE;
            frame->tx.slots[0].channel_type = arbitrated_gas[gas_id].params_sdcch.channels[frame_index].ul_channel;
            frame->tx.slots[0].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
            frame->tx.slots[0].s_time       = garb_get_slot_s_time(frame, GARB_TX, 0, gas_id);
            frame->tx.slots[0].e_time       = garb_get_slot_e_time(frame, GARB_TX, 0, gas_id);
            frame->tx.slots[0].cxm_tier     = garb_get_cxm_priority_tier(GARB_TX, frame->tx.slots[0].channel_type, 0, gas_id);
            frame->tx.num_slots             = 1;
         }

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
         if( (arbitrated_gas[gas_id].params_monscan.ba_list_size!=0) &&  (FnMod102 < 100) && (arbitrated_gas[gas_id].params_sdcch.num_ncell_arfcn) )
         {
            /* Initialise the frame parameters for MONSCAN */
            frame->mons.configured          = TRUE;
            frame->mons.mon[0].configured   = TRUE;
            frame->mons.mon[0].arfcn        = garb_get_monscan_arfcn(fn, gas_id);
            frame->mons.mon[0].channel_type = GARB_CHAN_UNKNOWN;
            frame->mons.mon[0].link_type    = CXM_LNK_TYPE_POWER_MONITOR;
            frame->mons.mon[0].s_time       = garb_get_mon_s_time(frame,gas_id);
            frame->mons.mon[0].e_time       = garb_get_mon_e_time(frame,gas_id);
            frame->mons.mon[0].cxm_tier     = ACTIVITY_TIER_20;
            frame->mons.num_mons            = 1;
         
            rc = TRUE;
         }
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

         break;
      }
   /*************
   *CS /CS_PS
   **************/
   case GARB_CS_PS:
   case GARB_CS:
      {
         uint32 FnMod26 = fn % 26;
         /* Timeline built for CS is derived from the position of the SACCH and IDLE frame + FACCH and RATSCCH state */
         TN = arbitrated_gas[gas_id].params_cs.TN;
         (void)garb_is_tch_frame(fn, frame_index, L1_get_dtx_state(gas_id), gas_id);

         if(arbitrated_gas[gas_id].params_cs.channels[frame_index].dl_channel != GARB_CHAN_UNKNOWN)
         {
            /* Initialise the frame parameters and the Rx slot */
            frame->rx.configured             = TRUE;
            frame->rx.search_width           = USTMR_TCH_SEARCH_WIDTH;
            frame->rx.slots[TN].configured   = TRUE;
            frame->rx.slots[TN].channel_type = arbitrated_gas[gas_id].params_cs.channels[frame_index].dl_channel;
            frame->rx.slots[TN].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
            frame->rx.slots[TN].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
            frame->rx.slots[TN].s_time       = garb_get_slot_s_time(frame, GARB_RX, TN, gas_id);
            frame->rx.slots[TN].e_time       = garb_get_slot_e_time(frame, GARB_RX, TN, gas_id);
            frame->rx.slots[TN].cxm_tier     = garb_get_cxm_priority_tier(GARB_RX, frame->rx.slots[TN].channel_type, TN, gas_id);
            frame->rx.num_slots              = 1;

            rc = TRUE;
         }

         if(arbitrated_gas[gas_id].params_cs.channels[frame_index].ul_channel != GARB_CHAN_UNKNOWN)
         {
            /* Initialise the frame parameters and the Tx slot */
            frame->tx.configured             = TRUE;
            frame->tx.timing_advance         = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.timing_advance;
            frame->tx.power                  = txlev_to_dBm[arbitrated_gas[gas_id].params_cs.frequency_list.channel[0].band][L1_get_txlev_for_FN(fn % 104,gas_id)];
            frame->tx.slots[TN].configured   = TRUE;
            frame->tx.slots[TN].channel_type = arbitrated_gas[gas_id].params_cs.channels[frame_index].ul_channel;
            frame->tx.slots[TN].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
            frame->tx.slots[TN].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), FALSE, gas_id);
            frame->tx.slots[TN].s_time       = garb_get_slot_s_time(frame, GARB_TX, TN, gas_id);
            frame->tx.slots[TN].e_time       = garb_get_slot_e_time(frame, GARB_TX, TN, gas_id);
            frame->tx.slots[TN].cxm_tier     = garb_get_cxm_priority_tier(GARB_TX, frame->tx.slots[TN].channel_type, TN, gas_id);
            frame->tx.num_slots              = 1;

            rc = TRUE;
         }

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
         if( (arbitrated_gas[gas_id].params_monscan.ba_list_size!=0) 
              && (FnMod26 != arbitrated_gas[gas_id].params_cs.idle_frame) 
              && (FnMod26 != arbitrated_gas[gas_id].params_cs.idle_frame-1)
              && (arbitrated_gas[gas_id].params_cs.num_ncell_arfcn))
         {
             /* Initialise the frame parameters for MONSCAN */
             frame->mons.configured          = TRUE;
             frame->mons.mon[0].configured   = TRUE;
             frame->mons.mon[0].arfcn        = garb_get_monscan_arfcn(fn, gas_id);
             frame->mons.mon[0].channel_type = GARB_CHAN_UNKNOWN;
             frame->mons.mon[0].link_type    = CXM_LNK_TYPE_POWER_MONITOR;
             frame->mons.mon[0].s_time       = garb_get_mon_s_time(frame,gas_id);
             frame->mons.mon[0].e_time       = garb_get_mon_e_time(frame,gas_id);
             frame->mons.mon[0].cxm_tier     = ACTIVITY_TIER_20;
             frame->mons.num_mons            = 1;

             rc = TRUE;
         }
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

         if(FnMod26 == arbitrated_gas[gas_id].params_cs.idle_frame)
         {
            /* Initialise the frame parameters for NCELL activity */
            frame->rx.configured            = TRUE;
            frame->rx.num_slots             = 1;
            frame->rx.slots[TN].configured   = TRUE;
            frame->rx.slots[TN].channel_type = GARB_CHAN_NCELL_HI;
            frame->rx.slots[TN].arfcn        = gl1_get_serving_cell_arfcn(gas_id); // band id should mitigate .
            frame->rx.slots[TN].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
            frame->rx.slots[TN].s_time       = frame->UstmrRefStart;
            frame->rx.slots[TN].e_time       = USTMR_ADD(frame->rx.slots[TN].s_time, USTMR_PER_FRAME);
            frame->rx.slots[TN].cxm_tier     = ACTIVITY_TIER_10;

            rc = TRUE;
         }

         if(arbitrated_gas[gas_id].state == GARB_CS)
         {
            break;
         }
      }
   /*************
   * PS
   **************/
   case GARB_PS:
     {
         /* Timeline built for PS is derived from the UL and DL slots and RRBP, PACCH, PTCCH and IDLE frames */
         uint32 FnMod52 = fn % 52;
#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
         uint32 FnMod13 = fn % 13;
         garb_params_ps_rrbp_t  prev_blk_rrbp_params;
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */
         boolean dirn; // downlink
         uint8 ptcch_tn,slammed_tn;
         uint8 count = 0;
         boolean set_all_chan_type_hi = FALSE;
         gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
         uint8  max_ts_high_cxm_tier = 0;

         if( (FnMod52!=24) || (FnMod52!=50) ) /* FnMod51 - 24 and 50 are IDLE frames */
         {
            if( garb_is_rrbp_block_boundary(fn, gas_id) )
            {
#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
              if ( frame_index == GARB_TIMELINE_FOURTH_FRAME )
              {
                prev_blk_rrbp_params = arbitrated_gas[gas_id].params_ps.rrbp_params; 
              }
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */
              arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn  = garb_get_next_rrbp_fn(gas_id);
            }

            if(((FnMod52==11) || (FnMod52==37)) && (arbitrated_gas[gas_id].state != GARB_CS_PS))  // PTCCH FRAME
            {
               /* channel Type PTCCH */
               /* need to know if there is UL or DL or nothing to be scheduled */
               if ( gpl1_check_if_ptcch_scheduled(fn,&ptcch_tn,&dirn,gas_id) )
               {
                  MSG_GERAN_MED_2_G("GARB PTCCH -dirn(1-ul, 0-dl) %d FnMod52=%d", dirn,FnMod52);

                  if (dirn) // uplink
                  {
                     slammed_tn  = gpl1_gprs_ts_conv(ptcch_tn, gas_id);

                     /* Initialise the frame parameters and the first Tx slot */
                     frame->tx.configured                     = TRUE;
                     frame->tx.timing_advance                 = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.timing_advance;
                     frame->tx.power                          = L1_convert_power_idx_to_dBm(gprs_serv_cell_meas_ptr->ul_power_band,gprs_serv_cell_meas_ptr->pch[ptcch_tn]);
                     frame->tx.num_slots                      = 1;
                     frame->tx.slots[slammed_tn].configured   = TRUE;
                     frame->tx.slots[slammed_tn].channel_type = GARB_CHAN_ULDL_PTCCH;
                     frame->tx.slots[slammed_tn].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
                     frame->tx.slots[slammed_tn].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), FALSE, gas_id);
                     frame->tx.slots[slammed_tn].s_time       = garb_get_slot_s_time(frame, GARB_TX, slammed_tn, gas_id);
                     frame->tx.slots[slammed_tn].e_time       = garb_get_slot_e_time(frame, GARB_TX, slammed_tn, gas_id);
                     frame->tx.slots[slammed_tn].cxm_tier     = garb_get_cxm_priority_tier(GARB_TX, frame->tx.slots[slammed_tn].channel_type, slammed_tn, gas_id);
                  }
                  else
                  {
                     slammed_tn  = gpl1_gprs_ts_conv(ptcch_tn, gas_id);
                     frame->rx.configured                     = TRUE;
                     frame->rx.search_width                   = USTMR_GPRS_SEARCH_WIDTH;
                     frame->rx.num_slots                      = 1;
                     frame->rx.slots[slammed_tn].configured   = TRUE;
                     frame->rx.slots[slammed_tn].channel_type = GARB_CHAN_ULDL_PTCCH;
                     frame->rx.slots[slammed_tn].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
                     frame->rx.slots[slammed_tn].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
                     frame->rx.slots[slammed_tn].s_time       = garb_get_slot_s_time(frame, GARB_RX, slammed_tn, gas_id);
                     frame->rx.slots[slammed_tn].e_time       = garb_get_slot_e_time(frame, GARB_RX, slammed_tn, gas_id);
                     frame->rx.slots[slammed_tn].cxm_tier     = garb_get_cxm_priority_tier(GARB_RX, frame->rx.slots[slammed_tn].channel_type, slammed_tn, gas_id);
                  }
                  rc = TRUE;
               }
            }
            else
            {
               if(arbitrated_gas[gas_id].params_ps.num_dl_slots!=0)
               {
                  /* Initialise the frame parameters and the first Rx slots */
                  frame->rx.configured            = TRUE;
                  frame->rx.search_width          = USTMR_GPRS_SEARCH_WIDTH;
                  frame->rx.num_slots             = arbitrated_gas[gas_id].params_ps.num_dl_slots;

                  /* iterate through each slot and check if its active*/
                  for(i=0;i<GL1_DEFS_SLOTS_IN_FRAME;i++)
                  {
                     if(arbitrated_gas[gas_id].params_ps.ps_dl_alloc[i] == FALSE)
                       {continue;}
                     else
                     {
                        /* Initialise the slot parameters */
                        frame->rx.slots[i].configured   = TRUE;
                        frame->rx.slots[i].channel_type = GARB_CHAN_ULDL_PDTCH;
                        frame->rx.slots[i].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
                        frame->rx.slots[i].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
                        frame->rx.slots[i].s_time       = garb_get_slot_s_time(frame, GARB_RX, i, gas_id);
                        frame->rx.slots[i].e_time       = garb_get_slot_e_time(frame, GARB_RX, i, gas_id);
                        frame->rx.slots[i].cxm_tier     = garb_get_cxm_priority_tier(GARB_RX, frame->rx.slots[i].channel_type, i, gas_id);
                     }
                  }
               }

               if(arbitrated_gas[gas_id].params_ps.num_ul_slots!=0)
               {
                   /* This loop is **NOW** based on slammed  TN */
                   for (i=0;i<GL1_DEFS_SLOTS_IN_FRAME;i++)
                   {
                     if ( (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn != GL1_DEFS_INVALID_FN)                &&
                          (IS_FRAME_NUM_LATER(fn,  SUB_FN(arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn, 2) ) ) &&
                          (!IS_FRAME_NUM_LATER(fn, ADD_FN(arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn, 2) ) ) &&
                          (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_ts[i] != 0xFF))   // i is slammed tn for rrbp.
                     {
                       slammed_tn = i;
                     }
                     else
                     {
                       if (arbitrated_gas[gas_id].params_ps.ps_ul_alloc[i] == TRUE) 
                       {
                         slammed_tn = i;
                       }                      
                       else
                       { 
                         continue;
                       }
                     }

                     {
                        /* Initialise the slot parameters */
                        frame->tx.num_slots++;
                        frame->tx.configured            = TRUE;
                        frame->tx.slots[i].configured   = TRUE;
                        frame->tx.timing_advance        = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.timing_advance;
                        frame->tx.power                 = L1_convert_power_idx_to_dBm
                                                            (gprs_serv_cell_meas_ptr->ul_power_band,
                                                             gprs_serv_cell_meas_ptr->pch[gpl1_gprs_ts_conv_unslam(i,gas_id)]);
                        frame->tx.slots[i].channel_type = garb_get_tx_channel_type(fn, i, gas_id); // i is slammed tn
                        frame->tx.slots[i].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
                        frame->tx.slots[i].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), FALSE, gas_id);

                        if(frame->tx.slots[i].channel_type == GARB_CHAN_UL_RRBP_HI)
                        {
                          set_all_chan_type_hi  = TRUE;
                          max_ts_high_cxm_tier  = i;
                        }
                        count ++;

                        frame->tx.slots[i].s_time       = garb_get_slot_s_time(frame, GARB_TX, i, gas_id);
                        frame->tx.slots[i].e_time       = garb_get_slot_e_time(frame, GARB_TX, i, gas_id);
                        frame->tx.slots[i].cxm_tier     = garb_get_cxm_priority_tier(GARB_TX, frame->tx.slots[i].channel_type, i, gas_id);
                     }
                  }

                  if (set_all_chan_type_hi)
                  {
                    for (i=0;i< MAX_TN;i++)  // SLAMMED TN
                    {
                      if(frame->tx.slots[i].configured)
                      {
                        /* FW cannot tune 2 times and hence only until actual rrbp make the cxm prio high */
                        if( (frame->tx.slots[i].channel_type == GARB_CHAN_UL_RRBP_HI) && 
                            ( (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDS) &&
                              (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDA) &&
                              (max_ts_high_cxm_tier == i) )
                          )
                        {
                          /* do not program the remaining timeslots as HIGH as it would lead to excessive blanking on other sub*/
                          break;
                        }
                        else
                        {
                          MSG_GERAN_MED_2_G("GARB : CHANNEL TYPE %d forced to RRBP_HI for ts %d",frame->tx.slots[i].channel_type,i);
                          frame->tx.slots[i].channel_type = GARB_CHAN_UL_RRBP_HI;
                          frame->tx.slots[i].cxm_tier     = garb_get_cxm_priority_tier(GARB_TX, frame->tx.slots[i].channel_type, i, gas_id);
                        }
                      }
                    }  // end  of for 
                  } // end of set_all_chan_type_hi
               }
               else
               {
                  if( (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn != GPL1_INVALID_FN) &&
                      ( IS_FRAME_NUM_LATER(fn, SUB_FN(arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn, 2) ) )&& /* RRBP for UL=0 */
                      (!IS_FRAME_NUM_LATER(fn, ADD_FN(arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_fn, 2) ) ) )
                  {
                     for (i=0;i< MAX_TN;i++) // i is slammed ts
                     {
                       if (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_ts[i] != 0xFF)  
                       {
                         slammed_tn = arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_ts[i];
                         frame->tx.configured            = TRUE;
                         frame->tx.timing_advance        = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.timing_advance;
                         frame->tx.power                 = L1_convert_power_idx_to_dBm
                                                            (gprs_serv_cell_meas_ptr->ul_power_band,
                                                             gprs_serv_cell_meas_ptr->pch[gpl1_gprs_ts_conv_unslam(slammed_tn,gas_id)]);
                         frame->tx.num_slots++;
                         frame->tx.slots[slammed_tn].configured   = TRUE;
                         frame->tx.slots[slammed_tn].channel_type = ( (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_msg[slammed_tn]==PDAN_MSG ) || 
                                                                      (arbitrated_gas[gas_id].params_ps.rrbp_params.rrbp_msg[slammed_tn]==PMR_MSG) ) ? 
                                                                       GARB_CHAN_UL_RRBP_LO : GARB_CHAN_UL_RRBP_HI;

                         if(frame->tx.slots[i].channel_type == GARB_CHAN_UL_RRBP_HI)
                         {
                           set_all_chan_type_hi  = TRUE;
                           max_ts_high_cxm_tier  = i;
                         }

                         frame->tx.slots[slammed_tn].arfcn        = gpl1_get_serving_cell_next_arfcn(fn,gas_id);
                         frame->tx.slots[slammed_tn].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), FALSE, gas_id);
                         frame->tx.slots[slammed_tn].s_time       = garb_get_slot_s_time(frame, GARB_TX, slammed_tn, gas_id);
                         frame->tx.slots[slammed_tn].e_time       = garb_get_slot_e_time(frame, GARB_TX, slammed_tn, gas_id);
                         frame->tx.slots[slammed_tn].cxm_tier     = garb_get_cxm_priority_tier(GARB_TX, frame->tx.slots[slammed_tn].channel_type, slammed_tn, gas_id);

                         MSG_GERAN_HIGH_3_G("GARB : RRBP - no UL slots configured - generating Tx slot for RRBP chan type %d slammed_tn %d s_time %d",
                                            frame->tx.slots[slammed_tn].channel_type,slammed_tn,frame->tx.slots[slammed_tn].s_time);
                       }
                     }
                     
                     if (set_all_chan_type_hi)
                     {
                       for (i=0;i< MAX_TN;i++)  // SLAMMED TN
                       {
                         if(frame->tx.slots[i].configured)
                         {
                           /* FW cannot tune 2 times and hence only until actual rrbp make the cxm prio high */
                           if( (frame->tx.slots[i].channel_type == GARB_CHAN_UL_RRBP_HI) && 
                               ( (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDS) &&
                                 (arbitrated_gas[gas_id].multi_sim_mode!=GARB_MODE_GPG_DSDA) &&
                                 (max_ts_high_cxm_tier == i) )
                             )
                           {
                             /* do not program the remaining timeslots as HIGH as it would lead to excessive blanking on other sub*/
                             break;
                           }
                           else
                           {
                             frame->tx.slots[i].channel_type = GARB_CHAN_UL_RRBP_HI;
                             frame->tx.slots[i].cxm_tier     = garb_get_cxm_priority_tier(GARB_TX, frame->tx.slots[i].channel_type, i, gas_id);
                           }
                         }
                       }  // end  of for 
                     } // end of set_all_chan_type_hi
                  }
               }

#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
               if( (arbitrated_gas[gas_id].params_monscan.ba_list_size!=0)
                    && (FnMod13 != 10) && (FnMod13 != 11) 
                    && (arbitrated_gas[gas_id].params_ps.num_ncell_arfcn) 
                    && (arbitrated_gas[gas_id].state != GARB_CS_PS))
               {
                  /* Initialise the frame parameters for MONSCAN */
                  frame->mons.configured          = TRUE;
                  frame->mons.mon[0].configured   = TRUE;
                  frame->mons.mon[0].arfcn        = garb_get_monscan_arfcn(fn, gas_id);
                  frame->mons.mon[0].channel_type = GARB_CHAN_UNKNOWN;
                  frame->mons.mon[0].link_type    = CXM_LNK_TYPE_POWER_MONITOR;
                  frame->mons.mon[0].s_time       = garb_get_mon_s_time(frame,gas_id);
                  frame->mons.mon[0].e_time       = garb_get_mon_e_time(frame,gas_id);
                  frame->mons.mon[0].cxm_tier     = ACTIVITY_TIER_20;
                  frame->mons.num_mons            = 1;

                  rc = TRUE;
               }
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */

               rc = TRUE;
            }
            
#ifdef FEATURE_GSM_GPRS_TIMELINE_REPORTING
            /* Caters for for consecutive rrbp last rrbp burst */
            if ( garb_is_rrbp_block_boundary(fn, gas_id) && (frame_index == GARB_TIMELINE_FOURTH_FRAME ))
            {
              /* Put back the rrbp params as we have not pass the previous radio block */
              arbitrated_gas[gas_id].params_ps.rrbp_params = prev_blk_rrbp_params;
            }
#endif /* FEATURE_GSM_GPRS_TIMELINE_REPORTING */
         }
         else if(arbitrated_gas[gas_id].state != GARB_CS_PS)
         {
            /* Initialise the frame parameters for NCELL activity */
            frame->rx.configured            = TRUE;
            frame->rx.num_slots             = 1;
            frame->rx.slots[0].configured   = TRUE;
            frame->rx.slots[0].channel_type = GARB_CHAN_NCELL_HI;
            frame->rx.slots[0].arfcn        = gl1_get_serving_cell_arfcn(gas_id); // band id should mitigate .
            frame->rx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
            frame->rx.slots[0].s_time       = USTMR_SUB(frame->rx.slots[0].s_time, 2*USTMR_PER_SLOT); /* offset by 2 slots as FCCH window opens early */
            frame->rx.slots[0].e_time       = USTMR_PER_FRAME-1;
            frame->rx.slots[0].s_time       = frame->UstmrRefStart;
            frame->rx.slots[0].e_time       = USTMR_ADD(frame->rx.slots[0].s_time, USTMR_PER_FRAME);

            frame->rx.slots[0].cxm_tier     = ACTIVITY_TIER_10;

            rc = TRUE;
         }
      break;
    }
   /*************
   * 2P_ACCESS
   **************/
   case GARB_2P_ACCESS:
      {
         /* Initialise the frame parameters and the Rx slot */
         frame->rx.configured            = TRUE;
         frame->rx.num_slots             = 1;
         frame->rx.slots[0].configured   = TRUE;
         frame->rx.slots[0].channel_type = GARB_CHAN_ULDL_PDTCH;
         frame->rx.slots[0].arfcn        = arbitrated_gas[gas_id].params_two_phase.frequency_list.channel[0];
         frame->rx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
         frame->rx.slots[0].s_time       = garb_get_slot_s_time(frame, GARB_RX, 0, gas_id);
         frame->rx.slots[0].e_time       = garb_get_slot_s_time(frame, GARB_RX, 0, gas_id);
         frame->rx.slots[0].cxm_tier     = ACTIVITY_TIER_10;

         rc = TRUE;
         break;
      }

   case GARB_NULL:
   default:
      break;
   }

   return rc;
}
/*===========================================================================

FUNCTION      garb_get_frame_at_wakeup_conflict

DESCRIPTION   Builds the anticipated frame for the specified FN and GAS at Wakeup Cancel.

RETURN VALUE  None

===========================================================================*/
void garb_get_frame_at_wakeup_conflict(uint32 fn, uint8 frame_index, gas_id_t gas_id)
{
      
      /*Calculate the new reserved FN timeline and update the timelines accordingly */
      
      garb_frame_t* frame = &arbitrated_gas[gas_id].frames[frame_index];
      l1_fm_priority_T next_activity = l1_fm_get_next_activity(&arbitrated_gas[gas_id].params_idle.next_activity, gas_id);
      uint32 i;
      uint8   TN = 0;

      /* USTMR REF Start is taken same as Last USTMR REF Start when Last VSTMR is run*/
      uint32 this_frame_fn = arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn;
   
      ASSERT( frame_index < GARB_TIMELINE_MAX_FRAMES );
   
      memset(frame, 0, sizeof(garb_frame_t) );
     
      frame->fn            = arbitrated_gas[gas_id].params_idle.next_activity.fn;

      /* Calculate USTMR Ref start w.r.to new reserved FN */

      frame->UstmrRefStart = USTMR_ADD(arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart, (SUB_FN(frame->fn, this_frame_fn)*USTMR_PER_FRAME) );
      frame->UstmrRefEnd   = USTMR_ADD(frame->UstmrRefStart, USTMR_PER_FRAME);
   
      MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT:garb_get_frame_at_wakeup_conflict:fn =%d frame->UstmrRefStar=%d",
                         frame->fn, frame->UstmrRefStart);

      MSG_GERAN_DEBUG_OPT_2_G("GARB_OPT:garb_get_frame_at_wakeup_conflict:this_frame_fn =%d  this_frame_ustart = %d",
                        arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].fn, arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].UstmrRefStart);
      
      if( next_activity == FM_PRI_CCCH)
       {
          frame->rx.configured            = TRUE;
          frame->rx.search_width          = USTMR_DEFAULT_SEARCH_WIDTH;
          frame->rx.num_slots             = 1;
   
          /* Initialise the slot parameters */
          frame->rx.slots[0].configured   = TRUE;
          frame->rx.slots[0].channel_type = GARB_CHAN_DL_CCCH;
          frame->rx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(frame->fn, 1), TRUE, gas_id);
          frame->rx.slots[0].s_time       = garb_get_slot_s_time(frame, GARB_RX, 0, gas_id);
          frame->rx.slots[0].e_time       = garb_get_slot_e_time(frame, GARB_RX, 0, gas_id);
          frame->rx.slots[0].cxm_tier     = MAX_ACTIVITY_TIERS;
 
        }
        else if( (next_activity == FM_PRI_NCELL_ACQ) || (next_activity == FM_PRI_NCELL_SCH) || (next_activity == FM_PRI_NCELL_BCCH) )
        {
            /* Initialise the frame parameters for NCELL activity */
            frame->rx.configured            = TRUE;
            frame->rx.num_slots             = 1;
            frame->rx.slots[0].configured   = TRUE;
            frame->rx.slots[0].channel_type = GARB_CHAN_NCELL_LO;
            frame->rx.slots[0].burst_num    = gpl1_get_burst_num_from_FN(ADD_FN(fn, 1), TRUE, gas_id);
            frame->rx.slots[0].s_time       = frame->UstmrRefStart;
            frame->rx.slots[0].e_time       = USTMR_ADD(frame->rx.slots[0].s_time, USTMR_PER_FRAME);
            frame->rx.slots[0].cxm_tier     = MAX_ACTIVITY_TIERS;

        }
}
      
#endif /* FEATURE_GSM_COEX */
