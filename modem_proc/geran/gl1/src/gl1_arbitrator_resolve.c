/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                   G L 1  A R B I T R A T O R  R E S O L V E

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1_arbitrator_resolve.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
23/06/17   mn        CR2055160 Fix for regression caused by CR2009497(PDTCH last burst over PCH first burst)
21/12/15   sk	     CR938638 Reducing no of F3s
15/07/15   nm        CR852442 Fix the merge error from CR833968 where setting of ignore_xcch flag got deleted
14/07/15  shr        CR850946 Last burst of PDTCH getting yielded on collision with 1st burst of PCH of idle sub
26/05/15   ab        CR833968 : GL1 COEX - RRBP_HI gets incorrectly de-priortised over Paging CCCH .
03/03/15   nm        CR801161 Fix GBTA_DEBUG compilation error
27/02/15   ab        CR750511: GSM Timeline reporting for WLAN COEX
10/02/15   cws       CR789401 Revert G high priority to tier_10 except for data + MMS
03/12/14   npt       CR.759467 : Fix compiler warnings
01/12/14   mc        CR.725853 : DSDA G[DDS]+G[MMS] : MCS using floored values for T10, T15, T20 hence T5 adaptation is required
10/11/14   pjr       CR.752497 : Increase value of USF Utilization counter from 8 to 16
23/08/14   mc        CR.707745 : [BOLT2.0][SG+G]: Incorrect CXM priority assignment policy in use for SGLTE+G
15/07/14   pjr       CR694682 Increase priority for PDTCH TX when more than 8 consecutive blocks are blanked
30/05/14   ws        CR657674 Data + MMS Phase 1
03/03/14   pjr       CR.625399 : Merge missing changes from CR579214
27/01/14   pjr       CR.579214 : COEX : Prioritise PDTCH over TCH during 2 phase access
09/11/13   mc        CR.572746 : COEX : DSDA G+G VBATT power back-offs for TxTx collisions are not correctly applied
04/09/13   sk        CR537595 Adding an explicit F3 to differentiate xCCH failure due to activity conflict
15/08/13   sk        CR524039 GBTA bringup changes
30/07/13   dv        CR520564 : Deal with new priority level introduced by TDS
29/07/13   sk        CR519663 Partial QBTA bringup changes
05/04/13   mc        CR.470155 : COEX Support - GERAN arbitrator framework - PS integration
13/03/13   mc        CR.459487 : COEX Support - GERAN arbitrator framework
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#ifdef FEATURE_DUAL_SIM
#if defined(FEATURE_GSM_COEX)

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#include "gl1_arbitrator_types.h"
#include "gl1_arbitrator_resolve.h"
#include "gl1_arbitrator_utils.h"
#include "gl1_arbitrator_tables.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1_utils.h"
#include "l1_task.h"

/*===========================================================================

                            LOCAL DEFINES

===========================================================================*/
extern garb_arbitrated_gas_t arbitrated_gas[NUM_GERAN_DATA_SPACES];

#ifdef FEATURE_GPRS_GBTA
boolean ignore_xcch =  FALSE;
#endif /* FEATURE_GPRS_GBTA */
/*===========================================================================

                     PROTOTYPES FOR LOCAL FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION      garb_resolve_none

DESCRIPTION   Resolver for NULL conflict

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_none(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)

{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;
   *conflict_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_hi_prio

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_hi_prio(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)

{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;
   *conflict_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_lo_prio

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_lo_prio(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

  *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;
  *conflict_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_hi_prio_dual_data

DESCRIPTION   This function is used resolve conflict in Data + MMS 

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_hi_prio_dual_data(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA;
   *conflict_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO_DUAL_DATA;
}

/*===========================================================================

FUNCTION      garb_resolve_lo_prio_dual_data

DESCRIPTION   This function is used resolve conflict in Data + MMS 

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_lo_prio_dual_data(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

  *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO_DUAL_DATA;
  *conflict_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA;
}

/*===========================================================================
FUNCTION      garb_resolve_tch_pdtch

DESCRIPTION   should resolve high priority for tch except when
              transfer is in L1_SINGLE_BLOCK_ALLOC

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_tch_pdtch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   /* Always T_20 allowing Macro Prio to win in TCH Vs PDTCH */
   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   /*does not get used in TX RX , for TX TX TX_TCH_TIER_T20 > TX_PDTCH_TIER_20*/
   *conflict_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_pdtch_tch

DESCRIPTION   should resolve low priority for pdtch except
              when transfer is in L1_SINGLE_BLOCK_ALLOC

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_pdtch_tch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);

   if (l1_transfer_data[gas_id].current_alloc_type == L1_SINGLE_BLOCK_ALLOC)
   {
      *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;
      MSG_GERAN_HIGH_0_G("PDTCH conflict with TCH during L1_SINGLE_BLOCK_ALLOC");
   }
   else
   {
      /* It is PDTCH TX , high priority and peer is Voice Rx , Priotise PDTCH TX */
      if (  !is_rx && conflict_is_rx &&
            (arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].priority == GPDU_HIGH_PRIORITY ||
            l1_get_number_consecutive_blanked_blocks(gas_id) >= GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD))
      {
         *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;
         if (l1_get_number_consecutive_blanked_blocks(gas_id) >= GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD)
         {
           MSG_GERAN_HIGH_0_G(" PDTCH_TX blanked consecutively more than 16 times");
         }
         else
         {
           MSG_GERAN_HIGH_0_G(" PDTCH_TX_CTRL conflict with TCH RX ");
         }
      }
      /* It is PDTCH TX , HIGH priority and peer is TCH Tx , Priotise PDTCH TX.*/
      else
      if (  !is_rx && !conflict_is_rx &&
            (arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].priority == GPDU_HIGH_PRIORITY ||
            l1_get_number_consecutive_blanked_blocks(gas_id) >= GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD))  
      {
         *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;
         if (l1_get_number_consecutive_blanked_blocks(gas_id) >= GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD)
         {
            MSG_GERAN_HIGH_0_G("PDTCH_TX blanked consecutively more than 16 times");
         }
         else
         {
            MSG_GERAN_HIGH_0_G(" PDTCH_TX_CTRL conflict with TCH TX");
         }
      }
      else
      {
         *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;
      }
   }

   *conflict_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

}

/*===========================================================================

FUNCTION      garb_resolve_pdtch_pdtch

DESCRIPTION   should resolve low priority for pdtch except
              when transfer is in L1_SINGLE_BLOCK_ALLOC

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_pdtch_pdtch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);


   if (l1_transfer_data[gas_id].current_alloc_type == L1_SINGLE_BLOCK_ALLOC)
   {
      *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA;
      MSG_GERAN_HIGH_0_G("PDTCH conflict with TCH during L1_SINGLE_BLOCK_ALLOC");
   }
   else
   {
      /* It is PDTCH TX , high priority and peer is data Rx , Priotise PDTCH TX */
      if (  !is_rx && conflict_is_rx &&
            (arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].priority == GPDU_HIGH_PRIORITY ||
            l1_get_number_consecutive_blanked_blocks(gas_id) >= GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD))
      {
         *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA;
         if (l1_get_number_consecutive_blanked_blocks(gas_id) >= GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD)
         {
            MSG_GERAN_HIGH_0_G(" PDTCH_TX blanked consecutively more than 16 times");
         }
         else
         {
            MSG_GERAN_HIGH_0_G(" PDTCH_TX_CTRL conflict with PDTCH RX ");
         }
      }
      /* It is PDTCH TX , HIGH priority and peer is TCH Tx , Priotise PDTCH TX.*/
      else
      if (  !is_rx && !conflict_is_rx &&
            (arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].priority == GPDU_HIGH_PRIORITY ||
            l1_get_number_consecutive_blanked_blocks(gas_id) >= GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD))  
      {
         *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO_DUAL_DATA;
         if (l1_get_number_consecutive_blanked_blocks(gas_id) >= GARB_MAX_NUM_CONT_BLANKED_UL_PDTCH_THRESHOLD)
         {
            MSG_GERAN_HIGH_0_G(" PDTCH_TX blanked consecutively more than 16 times");
         }
         else
         {
            MSG_GERAN_HIGH_0_G(" PDTCH_TX_CTRL conflict with PDTCH TX");
         }
      }
      else
      {
         *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO_DUAL_DATA;
      }
   }

   *conflict_priority_tier =  ACTIVITY_TIER_GPG_LO_PRIO_DUAL_DATA;
}
/*===========================================================================

FUNCTION      garb_resolve_pdtch_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_pdtch_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

  MSG_GERAN_DEBUG_OPT_5_G("GARB_OPT :conflict_index=%d conflict_read_t_index=%d peer_burst_num=%d burst_num=%d FN=%d",conflict_index,conflict_read_t_index,peer_burst_num,burst_num,gl1_get_FN(gas_id));

#ifdef FEATURE_GPRS_GBTA
   /* Lower the prio for CCCH  as we have 3 more burst to decode*/
   if ( gl1_msg_get_multi_sim_standby_mode() )
    {
      if ( (burst_num == 3) && (peer_burst_num == 0) && (*cxm_priority_tier == ACTIVITY_TIER_GPG_LO_PRIO))
      {
        MSG_GERAN_HIGH_3_G("GARB : GBTA Prio upgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
        *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

      }
    }
#endif  /* FEATURE_GPRS_GBTA*/

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;

}

/*===========================================================================

FUNCTION      garb_resolve_ccch_pdtch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_pdtch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;

   /* In a three way conflict CCCH can conflict with a TX in the curent Frame and an RX in the next Frame , the priority assignment can cause CCCH to 
        lose against the TX but win against the RX, causing the CCCH to win against the TX on macro priority eventually. Thus for first conflict if it has been
        downgraded ( e.g against 4th PDTCH Burst), its CXM prio should not be raised .
   */
   MSG_GERAN_DEBUG_OPT_5_G("GARB_OPT:ccch_pdtch :conflict_index=%d conflict_read_t_index=%d  peer_burst_num=%d burst_num=%d FN=%d",conflict_index,conflict_read_t_index,peer_burst_num,burst_num,gl1_get_FN(gas_id));

   if (*cxm_priority_tier == ACTIVITY_TIER_GPG_LO_PRIO)
   {
     MSG_GERAN_HIGH_3_G("GARB :  Prio downgraded due to prev conflict ccch_pdtch FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
   }
   else
   {
     *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

#ifdef FEATURE_GPRS_GBTA
     /* Lower the prio for CCCH  as we have 3 more burst to decode*/
     if (gl1_msg_get_multi_sim_standby_mode() )
     {
        if ( (burst_num == 0) && (peer_burst_num == 3) && (*cxm_priority_tier == ACTIVITY_TIER_GPG_HI_PRIO))
        {
          MSG_GERAN_HIGH_3_G("GARB : GBTA Prio downgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
          *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;
#ifdef FEATURE_GPRS_GBTA_DEBUG
          MSG_GERAN_MED_2_G("GARB : GBTA  thisburst_num  %d peer_burst_num %d",burst_num,peer_burst_num);
#endif /* FEATURE_GPRS_GBTA_DEBUG */
        }
      }
#endif  /* FEATURE_GPRS_GBTA*/
    }
    *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;

}

/*===========================================================================

FUNCTION      garb_resolve_ccch_rrbph

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_rrbph(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

#ifdef FEATURE_GPRS_GBTA
   ignore_xcch =  TRUE;
#endif  /* FEATURE_GPRS_GBTA*/

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_rrbph_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbph_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

#ifdef FEATURE_GPRS_GBTA
   ignore_xcch =  TRUE;
#endif /* FEATURE_GPRS_GBTA */

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;

}
/*===========================================================================

FUNCTION      garb_resolve_ccch_rrbpl

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_rrbpl(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;

   /* CCCH can conflict with TX for curent Frame and next Frame RX , thus for first conflict it has been downgraded ( 4th Burst)
   , its CXM prio should not be raised .
   */  
   if (*cxm_priority_tier == ACTIVITY_TIER_GPG_LO_PRIO)
   {
     MSG_GERAN_HIGH_3_G("GARB :  Prio downgraded due to prev conflict ccch_rrbpl FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
   }
   else
   {
     *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

#ifdef FEATURE_GPRS_GBTA
     /* Lower the prio for CCCH  as we have 3 more burst to decode*/
     if (gl1_msg_get_multi_sim_standby_mode())
     {
       if ( (burst_num == 0) && (peer_burst_num == 3) && (*cxm_priority_tier == ACTIVITY_TIER_GPG_HI_PRIO))
       {
         MSG_GERAN_HIGH_3_G("GARB : GBTA Prio downgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
         *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;
#ifdef FEATURE_GPRS_GBTA_DEBUG
         MSG_GERAN_MED_2_G("GARB : GBTA  thisburst_num  %d peer_burst_num %d",burst_num,peer_burst_num);
#endif /* FEATURE_GPRS_GBTA_DEBUG */
       }
     }
#endif  /* FEATURE_GPRS_GBTA*/
   }
*conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;

}



/*===========================================================================

FUNCTION      garb_resolve_rrbpl_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbpl_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;
   
   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

#ifdef FEATURE_GPRS_GBTA
     /* Lower the prio for CCCH  as we have 3 more burst to decode*/
   if ( arbitrated_gas[gas_id].multi_sim_mode == GARB_MODE_GPG_DSDS )
    {
      if ( (burst_num == 3) && (peer_burst_num == 0) && (*cxm_priority_tier == ACTIVITY_TIER_GPG_LO_PRIO))
      {
        MSG_GERAN_HIGH_3_G("GARB : GBTA Prio upgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
        *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;
#ifdef FEATURE_GPRS_GBTA_DEBUG
        MSG_GERAN_MED_2_G("GARB : GBTA  thisburst_num  %d peer_burst_num %d",burst_num,peer_burst_num);
#endif /* FEATURE_GPRS_GBTA_DEBUG */
      }
    }
#endif  /* FEATURE_GPRS_GBTA*/

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;

}

/*===========================================================================

FUNCTION      garb_resolve_ccch_ptcch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_ptcch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   /* CCCH vs DL PTCCH - CCCH has high prio */
   if ( conflict_is_rx )
   {
     MSG_GERAN_HIGH_3_G("GARB : GBTA Prio upgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
     MSG_GERAN_HIGH_2_G("GARB : GBTA  thisburst_num  %d peer_burst_num %d",burst_num,peer_burst_num);
     *cxm_priority_tier  = ACTIVITY_TIER_GPG_HI_PRIO;
   }
   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_ptcch_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ptcch_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
#ifdef FEATURE_GPRS_GBTA_DEBUG
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;
#endif /* FEATURE_GPRS_GBTA_DEBUG */

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   /* CCCH vs DL PTCCH - CCCH  has high prio */
   if ( is_rx )
   {
     MSG_GERAN_HIGH_3_G("GARB : GBTA Prio downgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
#ifdef FEATURE_GPRS_GBTA_DEBUG
     MSG_GERAN_MED_2_G("GARB : GBTA  thisburst_num  %d peer_burst_num %d",burst_num,peer_burst_num);
#endif /* FEATURE_GPRS_GBTA_DEBUG */
     *cxm_priority_tier  = ACTIVITY_TIER_GPG_LO_PRIO;
   }
   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;

}

/*===========================================================================

FUNCTION      garb_resolve_ptcch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ptcch_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
#ifdef FEATURE_GPRS_GBTA_DEBUG
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;
#endif /* FEATURE_GPRS_GBTA_DEBUG */

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   /* DL PTCCH  vs SCH- PTCCH D  has low prio */
   if ( is_rx )
   {
     MSG_GERAN_HIGH_3_G("GARB : GBTA Prio upgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);

#ifdef FEATURE_GPRS_GBTA_DEBUG
     MSG_GERAN_MED_2_G("GARB : GBTA  thisburst_num  %d peer_burst_num %d",burst_num,peer_burst_num);
#endif /* FEATURE_GPRS_GBTA_DEBUG */
     *cxm_priority_tier  = ACTIVITY_TIER_GPG_LO_PRIO;
   }

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}


/*===========================================================================

FUNCTION      garb_resolve_ptcch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ptcch_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
#ifdef FEATURE_GPRS_GBTA_DEBUG
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;
#endif /* FEATURE_GPRS_GBTA_DEBUG */

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   /* DL PTCCH  vs SCH- PTCCH D  has low prio */
   if ( is_rx )
   {
      MSG_GERAN_HIGH_3_G("GARB : GBTA Prio upgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
#ifdef FEATURE_GPRS_GBTA_DEBUG
      MSG_GERAN_MED_2_G("GARB : GBTA  thisburst_num  %d peer_burst_num %d",burst_num,peer_burst_num);
#endif /* FEATURE_GPRS_GBTA_DEBUG */
      *cxm_priority_tier  = ACTIVITY_TIER_GPG_LO_PRIO;
   }

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_pdtch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_pdtch_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_pdtch_ncelll

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_pdtch_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}
/*===========================================================================

FUNCTION      garb_resolve_ccch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ccch_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_rrbph_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbph_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_rrbph_ncelll

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbph_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_rrbpl_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbpl_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_rrbpl_ncelll

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_rrbpl_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_ncellh_ccch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncellh_ccch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_ncellh_ncelll

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncellh_ncelll(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_ncelll_ptcch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_ptcch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;
   boolean conflict_is_rx = (arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].rxtx==GARB_RX);
#ifdef FEATURE_GPRS_GBTA_DEBUG
   uint8 peer_burst_num = arbitrated_gas[garb_peer_gas_id(gas_id)].timeline[conflict_read_t_index][conflict_index].burst_num;
   uint8 burst_num = (is_rx) ? arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].burst_num : arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].burst_num;
#endif /* FEATURE_GPRS_GBTA_DEBUG */

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   /* SCH vs DL PTCCH - CCCH has high prio */
   if ( conflict_is_rx )
   {
     MSG_GERAN_HIGH_3_G("GARB : GBTA Prio upgrade FN %d cxm_prio(before) %d rxtx %d",gl1_get_FN(gas_id),*cxm_priority_tier,conflict_is_rx);
#ifdef FEATURE_GPRS_GBTA_DEBUG
     MSG_GERAN_MED_2_G("GARB : GBTA  thisburst_num  %d peer_burst_num %d",burst_num,peer_burst_num);
#endif /* FEATURE_GPRS_GBTA_DEBUG */
     *cxm_priority_tier  = ACTIVITY_TIER_GPG_HI_PRIO;
   }
   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;

}

/*===========================================================================

FUNCTION      garb_resolve_ncelll_paach

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_paach(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_ptcch_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_rrbpl(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_ncelll_ncellh

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_ncellh(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_ncelll_pdtch

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_pdtch(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_HI_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}

/*===========================================================================

FUNCTION      garb_resolve_ncelll_rrbph

DESCRIPTION

PARAMS

RETURN VALUE  None

===========================================================================*/
void garb_resolve_ncelll_rrbph(uint8 ts, boolean is_rx, uint8 conflict_index, cxm_activity_type* conflict_priority_tier, uint8 conflict_read_t_index, gas_id_t gas_id)
{
   cxm_activity_type* cxm_priority_tier = (is_rx) ? &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].rx.slots[ts].cxm_tier : &arbitrated_gas[gas_id].frames[GARB_TIMELINE_THIS_FRAME].tx.slots[ts].cxm_tier;

   *cxm_priority_tier = ACTIVITY_TIER_GPG_LO_PRIO;

   *conflict_priority_tier = (*cxm_priority_tier==ACTIVITY_TIER_GPG_HI_PRIO) ? ACTIVITY_TIER_GPG_LO_PRIO : ACTIVITY_TIER_GPG_HI_PRIO;
}
#endif /* defined(FEATURE_GSM_COEX)  */
#endif /* FEATURE_DUAL_SIM */
