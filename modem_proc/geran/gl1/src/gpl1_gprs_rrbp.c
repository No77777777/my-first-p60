/*===========================================================================

                  R R B P   M E S S A G E   H A N D L I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle receive, transmission of
   RRBP messages.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2004-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_rrbp.c#2 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
16/05/18   rc       CR2242798 To set rrbp_expired_in_gta when rrbp entry deleted during TA
20/02/18   shm      Change Request 2192580 : KW P1 errors on TA.3.0 fix
09/10/17   sn       CR2119040 Deriving TBF ST time from RRBP correctly
19/09/17   rc       CR2110654 To set rrbp_expired_in_gta when rrbp entry deleted during suspension
09/09/16   sn       CR1062272 TRM RESERVE AT during IDLE to transfer
24/05/16   sn       CR1016717:Removing Excess F3 logging for packet transfer
02/03/16   sn       CR981034 Handling Packet TBF release of DL while wait for PTSR re-configuration
17/03/15   ab       GL1 COEX : CR804948handling of multiple RRBP during conflict resolution
25/02/15   nm       CR796233 Delete RRBP if its TN is falling outside the TX window of multislot class
26/12/14   sp       CR775363: Block G2T TA if Release pending when EPDAN with FAI is set
12/05/14   sk       CR767085 - For allowing G2T TA when clashing with EPDAN with CR & FAI set to 1
03/10/14   aga      CR732455 Changes required to tune away framework for TH 1.0 single sim build 
23/08/14   sp       CR692209 Use Hyperframe macro at conditional checks.
21/02/14   nk       CR611257 Handling for rrbp msg_type PCA_UL_FAI_MSG while adjusting starting time
20/06/14   pjr      DATA+MMS feature - USF utilisation calculation
30/05/14   ws       CR657674 Data + MMS Phase 1
24/04/14   sp       CR645327 - Crash at boundary checks on PDTCH Timeslots.
15/05/14   mko      CR604883 Using correct Macro to get Max Tx slots for MSC10 in gpl1_gprs_clean_rrbp_txd_info()
27/09/13   dg       CR550007: Clear RRBP entry for PDAN during concurrent transfer when there is a
          Tra/Trb violation in DTM mode
27/09/13   sp        CR551580: Add NV logging of outgoing L1 messages
15/08/13   sk       CR524039 GBTA bringup changes
07/16/13   zc     CR470197 Replace memcpy with memscpy
29/07/13   sk       CR519663 Partial QBTA bringup changes
21/06/13   cs       Major Triton TSTS Syncup
19/06/12   ab       CR 431446 - Incorrect highest RRBN FN calculation-leads to incorrect TBF startime .
05/03/13   ab       CR 457539 - RRBP cancellation when corresponding DL TS no longer exists after TBF reconfig
20/05/13   am       CR476324 TBF release and RRBP received in GTA gap
13/03/13   ab       CR462234 L1 should not cancel rrbp by checking incorrect DL TBF Ptr
                    when this is no DL TBF
15/02/13   ws       CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
19/06/12   ab       CR 370547 :GL1 not sending REL_CNF when releasing UL TBF
                    with existing UL+DL and UL being released.
27/04/12   ab       CR356700 -L1 to obey back to back TBF release,to come out of release state.
24/05/12   ab       CR 324793:MS sending REL_CNF immediately without sending RRBP
                    for uplink (PTR startime comes before RRBP ). RRBP ignored when TS changes
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
25/07/11   ab       CR#: 325121 - Remove Prev CR 296957. Do not perform lowest ts check in DTM
                    as PS slot can be on lower TS.
25/07/11   ab       CR#: 296957 - add rrbp lowest ts check under DTM to avoid cancell rrbp.
27/04/11   cgc      CR285147 Add NULL check when rrbp clash detected.
22/03/11   ky       CR277664:While checking the rrbp clashes for scheduling rx, if DL rrbp
                    message needs to be sent in the current frame then check the timeslot on
                    which this rrbp is received if that timeslot is not assinged in the current
                    dl allocation then dont apply any tra trb rules because anyhow this rrbp is
                    deleted during the scheduling of tx
29/01/11   ky       CR272959:In UL+DL TBF, Adjusting the pending dl start time such
                    that PCA_UL_FAI rrbp message goes out before gl1 sends connect
                    ind to gmac
01/11/10   cja      CR262318 Cancel RRBP if in timeslot lower than PDCH allocation
06/09/10   ab       ENH Add TEST MODE B Reconfig and add asymetric TS support.
30/06/10   ap       CR: 235190 - 3GPP: Enhanced control of maximum output power in
                     a common BCCH cell
25/06/10   pg       CR244125: Add NULL ptr check for dl_tbf_ptr
09/03/10   ab       CR#: 228650-chk downlink rrbp ts with assigned dl ts alloc
06/05/09   ap       Fix Compile Issue
24/11/08   og       FEATURE_GSM_GPRS_MSC33 GPL1 changes.
04/09/08   nt       CR146552:- Always allow PDAN RRBP if we are in UL/DL
17/0108    cs       Lint fixes
16/03/07   og       Protect PCCF rrbp message type from being deleted in case
                    a tn clash occurs due to MSC restrictions.
12/02/07   cs       Fix build warning
26/01/07   og       Implement UL ctrl msg deletion in L1.
28/11/06   dv       Lint cleanup
14/11/06   cs       CR102226 Fix the Uplink Power Logging to use unslammed slot
01/09/06   og       Account for all dl RRBPS when resolving timeslot clashes.
                    Resolves CR99977.
23/08/06   og       Ensure rrbp_ts is within valid timeslot range. Addresses one
                    of the issues raised in CR99977.
31/07/06   av       CR85810 NB AB TSC, and TA fix.
10/07/06   og       Use DTM multislot parameters in DTM mode. Resolves
                    CR94827.
07/07/06   ws       Added GL1_UNIT_TEST around rex_wait() calls
30/06/06   og       Changes to handle high polling rate.
30/06/06   og       Fix to Tta/Ttb violation checks when polled timeslot falls
                    behind the highest receive timeslot.
18/05/06   ws       If we have deleted our last dl rrbp and we have a full
                    or partial Release pending then action it CR93602
10/09/06   ws       Setup 51 idle start time correctly for idle RRBP handler
26/01/06   og       Neighbour cell acquisition changes to handle extended dynamic.
23/11/05   og       Update tx call back routines to provide more information.
10/11/05   og       Change the F3 tx timeslot to be absolute.
06/09/05   og       Correctly handle Tra/Trb clashes.
05/09/05   og       Multislot class 12 support.
12/08/05   ws       Updated for initial DTM release
08/08/05   og       Restructuring PL1 before addition of DTM.
06/07/05   av       CR68673 - PCA was not sent in response to a polled PDA. PL1 was going to sleep.
09/06/05   ws       Adjust Rx allocation if an RRBP clash is detected and RRBP type
                    is PDAN_MSG_WITH_FAI (CR 65723)
02/06/05   og       Removal of header-header includes.
06/05/05   og       Ensure num of messages is correctly handled when replacing
                    rrbp messages on the same index.
26/04/05   og       Update rrbp clash detection to include multislot cases.
25/04/05   nt       In gpl1_gprs_transfer_add_rrbp_msg() added code to delete clashing
                    PCA_UL_FAI_MSG RRBP if there is a PDAN_MSG_WITH_FAI.
18/03/05   og       Fix to PDAN vs TBF suspension clash handling.
11/03/05   ws       Don't setup rrbp params in rrbp_txd_buff if we cannot TX
                    RRBP due to no Tav or incorrect Coding scheme
16/02/05   og       Merge change 166041 from saber 4x branch, fix for CR57346.
06/12/04   ws       Updates for TEST mode B for GPRS
19/11/04   og       Fix to prevent expired 51 PCA from entering RRBP queue.
21/10/04   dlh      Corrected text formatting.
18/10/04   dlh      Added code for reselection delay in 52 idle.
06/10/04   dlh      Removed Frame slamming.
17/08/04   ws       Cleaned up EGPRS code and linted
03/08/04   ws       Added gpl1_gprs_ptcch.h to include and corrected typo
03/08/04   ws       Fixed merge error in previous checkin
03/08/04   rm       Replaced num_dl_ab_msgs with num_ab_dl_msgs
02/08/04   rm       Removed macro definitions
23/06/04   og       Egprs Qual measurements handling.
13/05/04   rm       Added more function descriptions
28/04/04   rm       Initial version

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"

#include "rex.h"
#include "ms.h"
#include "memory.h" /* for memcpy() */
#include "geran_tasks.h"
#include "gmacl1.h"
#include "l1_drx.h"
#include "l1_task.h"
#include "task.h"
#include "l1_utils.h" /* gl1_get_FN() */
#include "gmacsigl1.h"
#include "gmacsigl1_g.h"
#include "grlcl1.h" /* l1 <-> rlc api */

#include <stringl/stringl.h>


#include "gpl1_gprs_log.h"


#include "geran_eng_mode_info.h"
#include "geran_dual_sim_g.h"

#include "gpl1_gprs_task.h"
#include "l1_isr.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_ptcch.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_utils.h"
#include "gtmrs_g.h"

#include "gl1_msg_g.h"

typedef struct
{
gl1_msg_tx_pkt_cb_type      gpl1_gprs_rrbp_idle_tx_callback;
gl1_msg_tx_pkt_cb_type      gpl1_gprs_rrbp_prach_tx_callback;
boolean                     gpl1_gprs_rrbp_idle_tx_callback_setup;
boolean                     gpl1_gprs_rrbp_prach_tx_callback_setup;

gpl1_gprs_rrbp_info_T       gpl1_gprs_rrbp;
gl1_defs_tx_pkt_data_type   pkt_data;

#ifdef FEATURE_G2X_TUNEAWAY
boolean                     rrbp_expired_in_gta;
#endif

/* Due to the interface design I need to add a Fn for pending Polled */
/* Assignments. The RRBP's starting time is written over and now the */
/* sleep and DRX code cause the PCA to be missed due to early sleep. */
uint32 gpl1_idle_pca_start_frame_number;
gas_id_t gas_id;
}gpl1_gprs_rrbp_data_t;

static gpl1_gprs_rrbp_data_t  gpl1_gprs_rrbp_data[NUM_GERAN_DATA_SPACES];


gpl1_gprs_rrbp_info_T l1_get_gpl1_gprs_rrbp_info ( gas_id_t gas_id )
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp ;
}

uint32 l1_get_gpl1_idle_pca_start_frame_number ( gas_id_t gas_id )
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return gpl1_gprs_rrbp_data_ptr->gpl1_idle_pca_start_frame_number ;
}
void l1_init_gpl1_gprs_rrbp_data( gas_id_t gas_id )
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 i;
  for(i = 0; i < SIZE_OF_RRBP_BUFF; i++)
  {
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].msg_valid  = FALSE ;
  }
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs = 0;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs = 0;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs = 0;

  for(i = 0; i < SIZE_OF_RRBP_BUFF; i++)
  {
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].msg_valid = FALSE;
  }
}
/* Local function declarations */
static boolean  gpl1_gprs_calc_rrbp_tn_clash( uint8   rrbp_tx_tn,
                                              uint8*  tra_trb_cancel_ts_alloc_ptr,
                                              uint8*  l1_cancel_rx_alloc_ptr, gas_id_t gas_id );

/* This function returns the RRBP single block type (HI vs LO) appropriate the the RRBP
 * message type. This is passed down to the message layer to indicate the priority of
 * the RRBP Tx.
 */
static gl1_defs_fixed_block_type gpl1_gprs_rrbp_msg_to_rrbp_hi_lo(rrbp_msg_t rrbp_msg_type);

/* get the lowest TS of ul assignment, return invalid if no ul tbf present*/
static uint8 get_lowest_uplink_timeslot(gas_id_t gas_id);

/* Added for AB update; we need to store */
/* the tsc so a RRBP does not overwrite. */
extern uint8   gl1_active_pdch_tsc[];

extern dedicated_data_T l1_dedicated_data[];

/* Time slot mask , TS is mapped in reversed order */
extern const uint8 tn_mask[];

boolean tta_ttb_clash_flag;

/*===========================================================================

FUNCTION  gpl1_gprs_idle_add_rrbp_msg

DESCRIPTION
In idle mode, any received request to send an RRBP is stored for scheduling.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_idle_add_rrbp_msg(mac_l1_pacch_rrbp_t *message_ptr)
{
  uint32              FN_num;
  uint8               rrbp_index;

  gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
  gprs_l1_idle_data_t *gprs_l1_idle_data = gl1_ms_switch_gprs_idle_data(gas_id);
  idle_data_T         *l1_idle_data=&l1_idle_data_store[gas_id];
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  rrbp_index=0;

  MSG_GERAN_HIGH_1_G("RRBP l1_idle_data->starting_time(%d)", l1_idle_data->starting_time);

#ifndef GL1_UNIT_TEST
  /* Not required for Host test */

  /* wake up layer 1 isr */
  gl1_drx_require_next_tick(gas_id);

  (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);

  (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );

#endif

  MSG_GERAN_MED_1_G("PL1: MAC_L1_PACCH_RRBP Rcvd (abs_fn = %d)",message_ptr->absolute_fn);
  l1_transfer_data[gas_id].polling_at_tbf_start_time = FALSE;

  /* Check if the rrbp was received from the immed assignment */
  if (message_ptr->rrbp_msg == PCA_51_MSG)
  {
    const uint32  fn = gl1_get_FN( message_ptr->gas_id );

    /* convert starting_time mod42432 FN to absolute FN */
    boolean  st_elapsed = gpl1_gprs_adjust_51_starting_time( &message_ptr->absolute_fn,gas_id );

    /* adjust RRBP fn to align to start of next block */
    gpl1_gprs_chk_blk_bndry( &message_ptr->absolute_fn );

    /* if the starting time for the rrbp has elapsed or we haven't enough time to process it */
    if( ( FALSE != st_elapsed )
     || ( IS_FRAME_NUM_LATER(ADD_FN( fn, 4 ), message_ptr->absolute_fn )))
    {
      /* report starting time elapsed.. */

      MSG_GERAN_HIGH_3_G( "RRBP from IA polling st elapsed!! %d  rrbp_fn %d [%d]",
                st_elapsed,
                message_ptr->absolute_fn,
                fn );

      /* Don't load the RRBP msg on the queuing array */
      return;
    }

  }
  else
  {
    /* adjust RRBP fn to align to start of next block */
    gpl1_gprs_chk_blk_bndry(&message_ptr->absolute_fn);
  }

  l1_idle_data->starting_time = message_ptr->absolute_fn;
  l1_idle_data->starting_time_valid = TRUE;
  gprs_l1_idle_data->start_time_valid = FALSE;

  /* Now work out where to put new RRBP message in the buffer
  * assuming buffer is not full up, note there may be a situation
  * where we will ignore the message as it clashes with a RRBP that
  * we want to send that is already in the buffer
  */
  FN_num = message_ptr->absolute_fn;

  rrbp_index = gpl1_gprs_get_matching_rrbp(FN_num, FN_num,
    message_ptr->ts_of_dl_msg, gas_id
    );

  MSG_GERAN_MED_3_G("Add idle rrbp, abolute_fn=%d, msg_type=%d, last_msg_type=%d ",
    message_ptr->absolute_fn, message_ptr->rrbp_msg, gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);

  if (rrbp_index < SIZE_OF_RRBP_BUFF)
  {
    /* we have a clash so work out which message to send
    * on this RRBP
    */

    /* alternate sending of PCA_MSG and PDAN_MSG( both types) */
    if ( ( (message_ptr->rrbp_msg == PCA_DL_MSG)     ||
      (message_ptr->rrbp_msg == PCA_UL_MSG)
      )                                           &&
      ( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PDAN_MSG)          &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PDAN_MSG_WITH_FAI) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PDAN_MSG_WITH_CRD) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PRR_MSG)
      )
      )
    {
      /* clash is between PCA and PCA so ignore incomming RRBP msg */
      MSG_GERAN_MED_2_G("PL1: idle RRBP ignored - PCAs clashing %d, %d",message_ptr->rrbp_msg,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
      return;
    }

    /* alternate sending of PCA_MSG and PDAN_MSG */
    if ( (message_ptr->rrbp_msg == PDAN_MSG)  &&
      ( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PCA_DL_MSG) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PCA_UL_MSG) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PRR_MSG)
      )
      )
    {
      /* clash is between PDAN and PDAN so ignore incomming RRBP msg */
      MSG_GERAN_MED_2_G("PL1: idle RRBP ignored - PDANs clashing %d, %d",message_ptr->rrbp_msg,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
      return;
    }

    if ( (message_ptr->rrbp_msg != PDAN_MSG_WITH_FAI)  &&
      (message_ptr->rrbp_msg != PDAN_MSG_WITH_CRD)  &&
      (message_ptr->rrbp_msg != PRR_MSG)
      )
    {
      /* error in incoming rrbp message */
      MSG_GERAN_MED_2_G("PL1: idle RRBP ignored - invalid msg %d, %d",message_ptr->rrbp_msg,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
      return;
    }

  }
  else if (rrbp_index == GPL1_FULL_RRBP_BUFF)
  {  /* There are valid msgs but no FN clashes  */
    /* report some exception here and break
    * out of the loop since more than  5 RRBP msgs
    * are outstanding -  MAC error???         */
    MSG_GERAN_ERROR_0_G("PL1: IDLE RRBP error!");
    return;
  }
  else if (rrbp_index == SIZE_OF_RRBP_BUFF)
  {  /* The position either contains no RRBP
    * or the RRBP FN has passed, therefore fill it up
    */
    rrbp_index = gpl1_gprs_check_for_rrbp_msg(FALSE,gas_id);
  }

  if (rrbp_index < SIZE_OF_RRBP_BUFF)
  {
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid        = TRUE;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].received_in_idle = TRUE;
    /* Need to save the PCA Idle start frame as PL1 writes over the */
    /* start time when the PDA is processed, and the PCA is missed. */
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num        =
    gpl1_gprs_rrbp_data_ptr->gpl1_idle_pca_start_frame_number                   = message_ptr->absolute_fn;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_burst_type  = message_ptr->msg_burst_type;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].timeslot         = message_ptr->ts_of_dl_msg;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type    = message_ptr->rrbp_msg;

    memscpy(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg,
        sizeof(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg),
        message_ptr->pacch_ab_data,
        sizeof(message_ptr->pacch_ab_data) );

    /* increment ul and dl msg count */
    if((message_ptr->rrbp_msg == PCA_UL_MSG) ||
      (message_ptr->rrbp_msg == PRR_MSG)    ||
      (message_ptr->rrbp_msg == PCA_51_MSG) ||
      (message_ptr->rrbp_msg == PCA_UL_FAI_MSG))
      gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs++;
    else
      gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs++;

    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs++;
  }

  MSG_GERAN_MED_2_G("Add idle rrbp %d, %d",rrbp_index, gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_idle_tx_callback_setup);

  MSG_GERAN_MED_3_G("Add idle rrbp [%d] [%d] [%d]", gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs, gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs, gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs);


  /*  Setup and Configure RRBP callbacks  */
  if (!gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_idle_tx_callback_setup)
  {
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_idle_tx_callback.scheduled_callback = (gl1_msg_scheduled_data_callback) gpl1_gprs_idle_rrbp_tx_callback;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_idle_tx_callback.discarded_callback = (gl1_msg_discarded_data_callback) gpl1_gprs_idle_rrbp_discard_callback;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_idle_tx_callback.usf_utilisation_callback = (gl1_msg_usf_utilisation_callback) gpl1_gprs_tx_dyn_rrbp_usf_util_cb;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_idle_tx_callback_setup = TRUE;
  }

 if((l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED)&& 
                         (l1_transfer_data[gas_id].starting_time_valid)&&
                         (l1_transfer_data[gas_id].starting_time == FN_num))
{
    l1_transfer_data[gas_id].polling_at_tbf_start_time = TRUE;
  
    MSG_GERAN_HIGH_1_G("DL TBF:RRBP %d", l1_idle_data->starting_time);
    
}
  /*  Configure for PACCHU transmission */
  gl1_msg_cfg_ul_pacch_pdtch(gl1_active_pdch_tsc[gas_id],TS0,l1_idle_data->campedon_cell_BSIC,&gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_idle_tx_callback,gas_id);

  return;
}

/*===========================================================================

FUNCTION  gpl1_gprs_prach_add_rrbp_msg

DESCRIPTION
In PRACH mode, any received request to send an RRBP is stored for scheduling.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_prach_add_rrbp_msg(mac_l1_pacch_rrbp_t *message_ptr)
{
  uint8 rrbp_index;

  gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];

  MSG_GERAN_MED_0_G("PL1: MAC_L1_PACCH_RRBP Rcvd");

  /*  To be consistant with RRBP operation in Transfer mode, upto
  four pending RRBP will be maintained. If RRBP requests received
  on different blocks indicate the same frame only one uplink frame
  will be sent.   */
  /*  Search array to determine if a free space exists. If not raise an
  fatal error */
  /*  Set local flag to determine if buffer full error has occurred.  */

  /* Check if the rrbp was received from the immed assignment */
  if (message_ptr->rrbp_msg == PCA_51_MSG)
  {
    const uint32  fn = gl1_get_FN( gas_id );

    /* convert starting_time mod42432 FN to absolute FN */
    boolean st_elapsed = gpl1_gprs_adjust_51_starting_time( &message_ptr->absolute_fn,gas_id );

    /* adjust RRBP fn to align to start of next block */
    gpl1_gprs_chk_blk_bndry( &message_ptr->absolute_fn );

    /* if the starting time for the rrbp has elapsed or we haven't enough time to process it */
    if( ( FALSE != st_elapsed )
      ||( IS_FRAME_NUM_LATER( ADD_FN( fn, 4 ),message_ptr->absolute_fn )) )
    {
      /* report starting time elapsed.. */

      MSG_GERAN_HIGH_3_G( "RRBP from IA polling st elapsed!! %d  rrbp_fn %d [%d]",
                st_elapsed,
                message_ptr->absolute_fn,
                fn );

      /* Don't load the RRBP msg on the queuing array */
      return;
    }

  }else
  {
    /* adjust RRBP fn to align to start of next block */
    gpl1_gprs_chk_blk_bndry(&message_ptr->absolute_fn);
  }

  rrbp_index = gpl1_gprs_check_for_rrbp_msg(FALSE,gas_id);

  if (rrbp_index == SIZE_OF_RRBP_BUFF)
  {
    /*  RRBP buffer full error has occurred */
    MSG_GERAN_ERROR_0_G("PL1: Transfer RRBP Full error!");
    return;
  }

  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid       = TRUE;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num       = message_ptr->absolute_fn;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_burst_type = message_ptr->msg_burst_type;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].timeslot        = message_ptr->ts_of_dl_msg;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type   = message_ptr->rrbp_msg;

  memscpy(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg,
          sizeof(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg),
          message_ptr->pacch_ab_data,sizeof(message_ptr->pacch_ab_data) );

  MSG_GERAN_MED_1_G("PL1: rrbp_fn =%d ",message_ptr->absolute_fn);

  /* increment transfer rrbp counters as this rrbp will be actioned in transfer */
  /* increment ul and dl msg count */
  if ( (message_ptr->rrbp_msg == PCA_UL_MSG) ||
    (message_ptr->rrbp_msg == PRR_MSG)    ||
    (message_ptr->rrbp_msg == PCA_51_MSG) ||
    (message_ptr->rrbp_msg == PCA_UL_FAI_MSG)
    )
  {
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs++;
  }
  else
  {
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs++;
  }

  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs++;

  /*  Setup and Configure RRBP callbacks  */
  if (!gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_prach_tx_callback_setup)
  {
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_prach_tx_callback.scheduled_callback = gpl1_gprs_prach_rrbp_tx_callback;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_prach_tx_callback.discarded_callback = gpl1_gprs_prach_rrbp_discard_callback;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_idle_tx_callback.usf_utilisation_callback = gpl1_gprs_tx_dyn_rrbp_usf_util_cb;
    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_prach_tx_callback_setup = TRUE;
//    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_prach_tx_callback.gas_id = gas_id;
  }

  MSG_GERAN_MED_2_G("Add prach rrbp %d %d",rrbp_index, gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_prach_tx_callback_setup);

  MSG_GERAN_MED_3_G("Add prach rrbp [%d] [%d] [%d]", gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs, gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs, gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs);

  /*  Configure for PACCHU transmission */
  gl1_msg_cfg_ul_pacch_pdtch(gl1_active_pdch_tsc[gas_id],TS0,l1_idle_data->campedon_cell_BSIC,&gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp_prach_tx_callback,gas_id);

  return;
}



/*===========================================================================

FUNCTION  gpl1_gprs_transfer_add_rrbp_msg

DESCRIPTION
In transfer mode, any received request to send an RRBP is stored for scheduling.

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_transfer_add_rrbp_msg(mac_l1_pacch_rrbp_t *message_ptr)
{
  uint32              FN_num;
  uint8               rrbp_index;
  uint32              oldest_rrbp_fn;

  gas_id_t gas_id = check_gas_id(message_ptr->gas_id);
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8               oldest_rrbp = 0;

  MSG_GERAN_LOW_1_G(" rrbp msg [%d]",message_ptr->rrbp_msg);

  if(message_ptr->absolute_fn < (gl1_get_FN( gas_id ) + 2) )
  {
    /* ignore this RRBP as we don't have time to send it */
    MSG_GERAN_HIGH_2_G("RRBP_FN ERR [%d] [%d]",message_ptr->absolute_fn,gl1_get_FN( gas_id ));

    /* If the RRBP message type is PDAN with CRD inform MAC that we can't send it */
    if (message_ptr->rrbp_msg == PDAN_MSG_WITH_CRD)
    {
      /* Inform mac to retry sending PDAN with CRD */
      gpl1_gprs_send_l1_mac_rrbp_failure_ind(TRUE, gas_id);
    }

    return;
  }


  /* adjust RRBP fn to align to start of next block */
  gpl1_gprs_chk_blk_bndry(&message_ptr->absolute_fn);

  /* Now work out where to put new RRBP message in the buffer
  * assuming buffer is not full up, note there may be a situation
  * where we will ignore the message as it clashes with a RRBP that
  * we want to send that is already in the buffer
  */
  FN_num = message_ptr->absolute_fn;

  rrbp_index = gpl1_gprs_get_matching_rrbp(FN_num, FN_num,
    message_ptr->ts_of_dl_msg,gas_id
    );
  if (rrbp_index < SIZE_OF_RRBP_BUFF)
  {
    /* we have a clash so work out which message to send
    * on this RRBP
    */

    /* alternate sending of PCA_MSG and PDAN_MSG( both types) */
    if ( ( (message_ptr->rrbp_msg == PCA_DL_MSG)     ||
      (message_ptr->rrbp_msg == PCA_UL_MSG)
      )                                           &&
      ( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PDAN_MSG)          &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PDAN_MSG_WITH_FAI) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PDAN_MSG_WITH_CRD) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type != PRR_MSG)
      )
      )
    {
      /* clash is between PCA and PCA so ignore incomming RRBP msg */
      MSG_GERAN_MED_2_G("PL1: transfer RRBP ignored - PCAs clashing %d, %d",message_ptr->rrbp_msg,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
      return;
    }

    /* alternate sending of PCA_MSG and PDAN_MSG */
    if ( (message_ptr->rrbp_msg == PDAN_MSG)       &&
      ( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type  != PCA_DL_MSG)     &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type  != PCA_UL_MSG)     &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type  != PCA_DL_FAI_MSG) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type  != PRR_MSG)
      )
      )
    {
      /* clash is between PDAN and PDAN so ignore incomming RRBP msg */
      MSG_GERAN_MED_2_G("PL1: transfer RRBP ignored - PDANs clashing %d, %d",message_ptr->rrbp_msg,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
      return;
    }

    if ( (message_ptr->rrbp_msg != PDAN_MSG_WITH_FAI)  &&
      (message_ptr->rrbp_msg != PDAN_MSG_WITH_CRD)  &&
      (message_ptr->rrbp_msg != PCA_DL_FAI_MSG)     &&
      (message_ptr->rrbp_msg != PRR_MSG)
      )
    {
      /* error in incoming rrbp message */
      MSG_GERAN_MED_2_G("PL1: transfer RRBP ignored - invalid msg %d, %d",message_ptr->rrbp_msg,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
      return;
    }

  }
  else if (rrbp_index == GPL1_FULL_RRBP_BUFF)
  {  /* There are valid msgs but no FN clashes  */
    /* We now have more than 5 RRBP messages in the buffer */
    MSG_GERAN_ERROR_0_G("PL1: Transfer RRBP error!");

    /* need to check if the rrbp received is PDAN with FAI or CRD or
    * in the case of downlink unack mode pca with FAI as these
    * must be sent !!!!
    */
    if( (message_ptr->rrbp_msg == PDAN_MSG_WITH_FAI) ||
      (message_ptr->rrbp_msg == PDAN_MSG_WITH_CRD) ||
      (message_ptr->rrbp_msg == PCA_DL_FAI_MSG)
      )
    {
      /* now find the oldest RRBP position and overwrite it */
      for(rrbp_index=0;rrbp_index< SIZE_OF_RRBP_BUFF;rrbp_index++)
      {
        /* initialise oldest rrbp fn */
        oldest_rrbp_fn = 0;
        if(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num > oldest_rrbp_fn)  /*?? Roya ?? oldest!!! */
          {
          oldest_rrbp = rrbp_index;
          oldest_rrbp_fn = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num;
        }
      }
      /* Restore position to store RRBP message in */
      rrbp_index= oldest_rrbp;
    }
    else
    {
      MSG_GERAN_MED_1_G("rrbp_add_transfer, message_ptr->rrbp_msg=%d",message_ptr->rrbp_msg);
      return;
    }
  }
  else if (rrbp_index == SIZE_OF_RRBP_BUFF)
  {/*
    *The position either contains no RRBP
    * or the RRBP FN has passed, therefore fill it up
    */
    rrbp_index = gpl1_gprs_check_for_rrbp_msg(FALSE,gas_id);
  }

  if( rrbp_index < SIZE_OF_RRBP_BUFF )
  {
    /* Setup a pointer to the location we want to write to */
    rrbp_params_T* const params_ptr = &( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[ rrbp_index ] );

    if( FALSE != params_ptr->msg_valid )
    {
      /* If the location we want to over-write
       * contains a valid rrbp msg then make
       * sure the num msgs counters are
       * decremented before over-writing.
       */
      switch( params_ptr->rrbp_msg_type )
      {
      case  PCA_UL_MSG:
      case  PCA_UL_FAI_MSG:
      case  PRR_MSG:

        if( 0 != gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs )
        {
          gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs--;
        }
        break;

      case  PCA_DL_MSG:
      case  PCA_DL_FAI_MSG:
      case  PDAN_MSG_WITH_FAI:

        if( 0 != gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ab_dl_msgs )
        {
          gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ab_dl_msgs--;
        }

      default: /*lint !e616 : no break statement for avoiding repitition */
        if( 0 != gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs )
        {
          gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs--;
        }
        break;
      }

      if( 0 != gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs )
      {
        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs--;
      }

      MSG_GERAN_MED_3_G(" Replaced rrbp msg %u with %u on index %u ",
              params_ptr->rrbp_msg_type,
              message_ptr->rrbp_msg,
              rrbp_index );
    }
    else
    {
      params_ptr->msg_valid = TRUE;
    }

    /* Copy the rrbp msg content */
    params_ptr->received_in_idle = FALSE;
    params_ptr->frame_num        = message_ptr->absolute_fn;
    params_ptr->rrbp_burst_type  = message_ptr->msg_burst_type;
    params_ptr->timeslot         = message_ptr->ts_of_dl_msg;
    params_ptr->rrbp_msg_type    = message_ptr->rrbp_msg;

    /* Copy the MAC control data */
    memscpy( params_ptr->rrbp_msg,
            sizeof(params_ptr->rrbp_msg),
            message_ptr->pacch_ab_data,
            sizeof( message_ptr->pacch_ab_data ) );

    /* Increment the relevant msg counters */
    switch( message_ptr->rrbp_msg )
    {
    case  PCA_UL_MSG:
    case  PCA_UL_FAI_MSG:
    case  PRR_MSG:
      gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs++;
      break;

    default:
      gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs++;
      break;
    }

    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs++;

     MSG_GERAN_MED_5_G("PL1: MAC_L1_PACCH_RRBP Rcvd fn = %d  tn %d [%d] [%d] [%d]"
                        ,message_ptr->absolute_fn
                        ,message_ptr->ts_of_dl_msg
                        ,rrbp_index
                        ,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs
                        ,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs);

  }
  return;
}


/*===========================================================================

FUNCTION  gpl1_gprs_single_tx_rrbp

DESCRIPTION
  In single access mode, the stored RRBP message is scheduled for transmission.

DEPENDENCIES

RETURN VALUE
  Transmitted rrbp_msg_type

SIDE EFFECTS
  None

===========================================================================*/
rrbp_msg_t  gpl1_gprs_single_tx_rrbp(uint8 rrbp_to_tx,
  frequency_information_T *freq_info_ptr, gas_id_t gas_id)
{
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  gl1_defs_coding_type               cs = GL1_DEFS_CS1_CODING;
  gl1_msg_ul_pacch_pdtch_signal_type signal_info;
  uint8 slammed_slot = 0;

  /*  See if current frame number is four before required over the air
  frame number for RRBP (Four allows for the two unusable frames) */

  /*  Correct frame to program RRBP transmission. */
  /*  Determine type of transmission  */

  if (rrbp_to_tx < SIZE_OF_RRBP_BUFF)
  {

    /* temp place holder for TX PWR to write Eng Mode Info */
    int16 tmp_tx_pwr[8] = {0};

    cs = gpl1_gprs_set_rrbp_coding_scheme(rrbp_to_tx,gas_id);

    slammed_slot = gpl1_gprs_ts_conv(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot,gas_id);

    signal_info.tn = (gl1_defs_tn_type)slammed_slot;

    gpl1_gprs_calculate_arfcns(l1_get_gprs_arfcns(gas_id), freq_info_ptr, NORM,gas_id);
    /*  Power control value */
    signal_info.TXLEV = gprs_serv_cell_meas_ptr->pmax;

    /*msg_data[0] = gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg; */
    gpl1_gprs_rrbp_data_ptr->pkt_data.gprs.data = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg;

    gl1_msg_tx_pacch_pdtch_fixed(
       &gpl1_gprs_rrbp_data_ptr->pkt_data,
       &signal_info,
       l1_get_gprs_arfcns(gas_id),
       cs,
       gpl1_gprs_rrbp_msg_to_rrbp_hi_lo(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg_type),
       gas_id
       );

    gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg_type;

    gpl1_gprs_delete_rrbp_entry(rrbp_to_tx,gas_id);

    /* Report using original TS allocation and not slammed */
    gpl1_log_gprs_power_control( GL1_MSG_UL_PACCH_PDTCH,
                                 (gl1_defs_tn_type) gpl1_gprs_ts_conv_unslam( signal_info.tn ,gas_id),
                                 signal_info.TXLEV,
                                 cs, gas_id );

    tmp_tx_pwr[0] = signal_info.TXLEV;
    geran_eng_mode_data_write(ENG_MODE_TX_PWR, tmp_tx_pwr, TRUE, gas_id);

    MSG_GERAN_MED_3_G("Single TX RRBP %d type=%d idx=%d", gl1_get_FN( gas_id ), gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type, rrbp_to_tx);
  }


  return (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
}

/*===========================================================================

FUNCTION  gpl1_gprs_transfer_tx_rrbp

DESCRIPTION
  In transfer mode, the stored RRBP message is scheduled for transmission.

DEPENDENCIES

RETURN VALUE
  Transmitted rrbp_msg_type

SIDE EFFECTS
  None

===========================================================================*/
rrbp_msg_t  gpl1_gprs_transfer_tx_rrbp(uint8 rrbp_to_tx, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

  gl1_defs_coding_type               cs = GL1_DEFS_CS1_CODING;
  gl1_msg_ul_pacch_pdtch_signal_type signal_info;
  ARFCN_T                             hop_arfcns[4];
  uint8 slammed_slot = 0;
  transfer_data_T *transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;
  frequency_information_T *freq_info_ptr = transfer_data_ptr->frequency_info_ptr;
  uint16  ts_alloc_dl;

  /*  See if current frame number is four before required over the air
  frame number for RRBP (Four allows for the two unusable frames) */

  /*  Correct frame to program RRBP transmission. */
  /*  Determine type of transmission  */

  if (rrbp_to_tx < SIZE_OF_RRBP_BUFF)
  {

    if (transfer_data_ptr->l1_test_mode == L1_TEST_MODE_B)
    {

      /* Now clear out the rx ctrl and tx rrbp status */
      gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status = 0;

      /* mark RRBP's which are TX's on UL timeslots
       * The loop back data needs deleting
       */
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot == gpl1_gprs_tm_b_store[gas_id].ul_tn[0])
      {
        gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status = 0x1;
      }

      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot == gpl1_gprs_tm_b_store[gas_id].ul_tn[1])
      {
        gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status |= 0x2;
      }

#ifdef FEATURE_GSM_GPRS_MSC33
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot == gpl1_gprs_tm_b_store[gas_id].ul_tn[2])
      {
        gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status = 0x4;

        MSG_GERAN_LOW_2_G(" tx rrbp tn_2 %d ctrl_s %d",gpl1_gprs_tm_b_store[gas_id].ul_tn[2],
                                             gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status);

      }
#endif /* FEATURE_GSM_GPRS_MSC33 */

      MSG_GERAN_LOW_3_G(" tx rrbp tn_0 %d tn_1 %d ctrl_s %d",gpl1_gprs_tm_b_store[gas_id].ul_tn[0],
                                                    gpl1_gprs_tm_b_store[gas_id].ul_tn[1],
                                                    gpl1_gprs_tm_b_store[gas_id].tx_rrbp_status);
    }

    cs = gpl1_gprs_set_rrbp_coding_scheme(rrbp_to_tx,gas_id);

    slammed_slot = gpl1_gprs_ts_conv(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot,gas_id);

    signal_info.tn = (gl1_defs_tn_type)slammed_slot;

    /* Show the Tn to the mDSP, and the OTA Tn. */
    MSG_GERAN_MED_3_G("RRBP:slamTn=%d,OTA_Tn=%d,FN=%d",
              signal_info.tn,
              gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot,
              GSTMR_GET_FN_GERAN( gas_id));

    /* make sure the rrbp ts is compatible with the assigned active timeslot
    ** configuration. Ignore rrbp otherwise and inform mac as per the
    ** conditions below
    */
    switch (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg_type)
    {
      case PCA_DL_FAI_MSG :
      case PDAN_MSG :
      case PDAN_MSG_WITH_FAI :
      case PDAN_MSG_WITH_CRD :
          /* sanity check dl_tbf_ptr before use */
          if( transfer_data_ptr->dl_tbf_ptr != NULL )
          {
            ts_alloc_dl = (uint16)transfer_data_ptr->dl_tbf_ptr->ts_allocation;
          }
          else
          {
            /* these cases imply that we still have a DL TBF, so must be an error */
            MSG_GERAN_ERROR_0_G(" NULL dl_tbf_ptr ");
            /* set TS allocation as none */
            ts_alloc_dl = 0;
          }
          break;

      case PCCF_MSG :
      case PMR_MSG  :
          /* MAC can replace RRPB PDAN message with UL control message like Pkt PSI Status
             ** only when DL TBF Exists. It uses above causes as RRBP message type.
          */
          if ( l1_transfer_data[gas_id].current_alloc_type == L1_DOWNLINK_ALLOC )
          {
            /* sanity check dl_tbf_ptr before use */
            if( transfer_data_ptr->dl_tbf_ptr != NULL )
            {
              ts_alloc_dl = (uint16)transfer_data_ptr->dl_tbf_ptr->ts_allocation;
            }
            else
            {
              /* these cases imply that we still have a DL TBF, so must be an error */
              MSG_GERAN_ERROR_1_G(" NULL dl_tbf_ptr alloc_type %d ",l1_transfer_data[gas_id].current_alloc_type);
              /* set TS allocation as none */
              ts_alloc_dl = 0;
            }
          }
          else if ( l1_transfer_data[gas_id].current_alloc_type == L1_DYNAMIC_DOWN_ALLOC )
          {
            /* sanity check dl_tbf_ptr before use */
            if( transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr != NULL )
            {
              ts_alloc_dl = (uint16)transfer_data_ptr->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation;
            }
            else
            {
              /* these cases imply that we still have a DL TBF, so must be an error */
              MSG_GERAN_ERROR_1_G("NULL dl_tbf_ptr alloc_type %d ",l1_transfer_data[gas_id].current_alloc_type);
              /* set TS allocation as none */
              ts_alloc_dl = 0;
            }
          }
          else
          {
            MSG_GERAN_ERROR_1_G("UL TBF Exists.MAC should use USF.current_alloc_type %d",l1_transfer_data[gas_id].current_alloc_type);
            /* no comparision for other rrbp types */
            ts_alloc_dl = 0xFFFF;
          }
          break;

      default :
          /* no comparision for other rrbp types */
          ts_alloc_dl = 0xFFFF;
          break;
    }

    /* For DTM , dont do ts check  */
    if( (ts_alloc_dl & (uint16)tn_mask[gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot])  &&
#ifdef FEATURE_GSM_DTM
        ( (l1_tskisr_blk->l1_state == L1_DTM_MODE ) ||
          (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot >= l1_get_current_timeslot(gas_id)) )
#else
        (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot >= l1_get_current_timeslot(gas_id))
#endif
      )
    {
      signal_info.TXLEV = (int16)gprs_serv_cell_meas_ptr->pch[gpl1_gprs_rrbp_data_ptr->
                                                               gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot];


#ifdef FEATURE_GSM_DTM
        if(l1_tskisr_blk->l1_state == L1_DTM_MODE)
        {
          L1_get_ARFCNS ( 1, 1, 4, hop_arfcns,
                           &l1_dedicated_data_ptr->channel_specification.channel_info_1_before,
                           &l1_dedicated_data_ptr->channel_specification.channel_info_1_after,
                           &l1_dedicated_data_ptr->new_channel_info, gas_id );
        }else
        {
          gpl1_gprs_calculate_arfcns(hop_arfcns, freq_info_ptr, NORM,gas_id);
        }
#else
        gpl1_gprs_calculate_arfcns(hop_arfcns, freq_info_ptr, NORM,gas_id);
#endif
      /* msg_data[0] = gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg;*/
      gpl1_gprs_rrbp_data_ptr->pkt_data.gprs.data = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg;

      /* If we are transmitting normal bursts for RRBP ensure
      * we have a valid TAV
      */
      if( ((cs == GL1_DEFS_CS1_CODING) && (gpl1_gprs_timing_advance_valid(gas_id) ) ) ||
        (cs == GL1_DEFS_ACCESS_CODING) ||
        (cs == GL1_DEFS_EXT_ACCESS_CODING) )

      {

        /* temp place holder for TX PWR to write Eng Mode Info */
        int16 tmp_tx_pwr[8] = {0};

        switch( frame_counters[gas_id].FNmod13 )
        {
           case  7:
             /* Mark this timeslot as being used for
             * rrbp transmission so we can avoid it
             * while doing interf meas.
             */
           gpl1_gprs_scell_disable_int_meas( (gl1_defs_tn_type) gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot, gas_id );
             break;
           default:
             break;
        }

         gl1_msg_tx_pacch_pdtch_fixed(&gpl1_gprs_rrbp_data_ptr->pkt_data,
          &signal_info,
          hop_arfcns,
          cs,
          gpl1_gprs_rrbp_msg_to_rrbp_hi_lo(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg_type),
          gas_id
          );

         if ((slammed_slot >= GL1_DEFS_SLOTS_IN_FRAME ||
             l1_get_current_timeslot(gas_id) >= GL1_DEFS_SLOTS_IN_FRAME))
         {
#ifdef FEATURE_QSH_MDUMP
            QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
            ERR_GERAN_FATAL_2_G("RRBP Tx timeslot not in range -slammed:%d, curr_ts:%d",slammed_slot,l1_get_current_timeslot(gas_id));
         }

        /* Report using original TS allocation and not slammed */
        gpl1_log_gprs_power_control( GL1_MSG_UL_PACCH_PDTCH,
                                     (gl1_defs_tn_type) gpl1_gprs_ts_conv_unslam( signal_info.tn,gas_id ),
                                     signal_info.TXLEV,
                                     cs, gas_id );

        tmp_tx_pwr[0] = signal_info.TXLEV;
        geran_eng_mode_data_write(ENG_MODE_TX_PWR, tmp_tx_pwr, TRUE, gas_id);

        /* Store rrbp msg pointer for detecting successful transmission of RRBP */
        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.rrbp_txd_buff[gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.idx].rrbp_txd_buff
                                                     = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg;

        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.rrbp_txd_buff[gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.idx].timeslot = signal_info.tn;
        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.rrbp_txd_buff[gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.idx++].cs = cs;

        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_msgs_txd++;

        /* allow for wrap arround of buffer */
        if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.idx >= MAX_NUM_UL_PACCH_MSGS)
        {
          gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.idx =0;
        }

        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg_type;

        MSG_GERAN_MED_4_G("Transfer TX RRBP timeslot %d [%d] %d type=%d ", ADD_MOD( slammed_slot, l1_get_current_timeslot(gas_id), GL1_DEFS_SLOTS_IN_FRAME ),
                            gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].frame_num,
                     gl1_get_FN( gas_id ), gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
      }
      else
      {
        MSG_GERAN_ERROR_0_G("PL1: rrbp NOT tx as no tav");
      }
    }
    else
    {
      MSG_GERAN_ERROR_3_G("PL1: rrbp NOT tx time slot not compatible ts_alloc_dl=%0x tx_tn=%d tx_tn_mask=%d",
                 ts_alloc_dl,
                 gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot,
                 tn_mask[gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].timeslot]);

      /* If the RRBP message type is PDAN with CRD inform MAC that we can't send it */
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg_type == PDAN_MSG_WITH_CRD )
      {
        /* Inform mac to retry sending PDAN with CRD */
        gpl1_gprs_send_l1_mac_rrbp_failure_ind(TRUE, gas_id);
        MSG_GERAN_HIGH_0_G("PL1: inform mac failure to send pdan with chan desc");
      }

      /* Should we stop T3192 , becuase if we get PDA with CTRL ACK=1 , MAC will ignore it ????? */

      /* If the RRBP message type is PDAN with FBI=1 ACK timer T3192 should be stopped.*/
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg_type == PDAN_MSG_WITH_FAI )
      {
        grlc_dl_stop_t3192(gas_id);
        MSG_GERAN_HIGH_0_G("PL1: stop T2192 as final pdan with fbi not sent");
      }

      /* If the RRBP message type is PCA for UNACK timer T3192 should be stopped.*/
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_msg_type == PCA_DL_FAI_MSG )
      {
        grlc_dl_stop_t3192(gas_id);
        MSG_GERAN_HIGH_0_G("PL1: stop T2192 as final pca not sent");
      }

      /* Do not update the gpl1_gprs_rrbp.last_msg_type */
    }

    gpl1_gprs_delete_rrbp_entry(rrbp_to_tx,gas_id);
  }

  return (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.last_msg_type);
}

/*===========================================================================

FUNCTION  gpl1_gprs_calc_rrbp_tn_clash

DESCRIPTION
  The timeslot of scheduled RRBP is checked against the available transmitted
window.

  This function assumes no uplink timeslots are present yet. It also compares
  against all the receive timeslots made up of the downlink tfi timeslots and
  usf monitor timeslots.

  This function assumes it is run in packet transfer mode.

DEPENDENCIES

RETURN VALUE
  Boolean defining yes / no

SIDE EFFECTS
  None

===========================================================================*/
static boolean  gpl1_gprs_calc_rrbp_tn_clash( uint8   rrbp_tx_tn,
                                              uint8*  tra_trb_cancel_ts_alloc_ptr,
                                              uint8*  l1_cancel_rx_alloc_ptr, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  /* Work out the current lowest and highest absolute timeslots
   * Note: Not using the timeslot conversion routines and adding
   * directly for efficiency purposes.
   */
  const uint8  lowest_rx_tn  = gprs_serv_cell_meas_ptr->lowest_tn.tn + l1_get_current_timeslot(gas_id);
  const uint8  highest_rx_tn = gprs_serv_cell_meas_ptr->highest_tn + l1_get_current_timeslot(gas_id);

  /* Adjust rrbp_tn to corresponding Rx slot */
  const uint8  rrbp_equiv_rx_tn = ADD_MOD( rrbp_tx_tn, 3, GL1_DEFS_SLOTS_IN_FRAME );

  /* Initialise the clash boolean to FALSE */
  boolean  clash_found = FALSE;

  /* These variables are set according to multislot class and the
   * number of allocated uplink timeslots.
   *
   * Power monitors are to be performed between the highest transmit
   * and lowest receive in the next tdma frame, apply Tta, Trb.
   * This is assumed initially.
   */
  uint8  ttb_tta = PL1_MULTISLOT_Ttb;
  uint8  trb_tra = PL1_MULTISLOT_Tra;

  tta_ttb_clash_flag = FALSE;

#ifdef FEATURE_GSM_DTM

  if( L1_DTM_MODE == l1_tskisr_blk->l1_state )
  {
    ttb_tta = PL1_DTM_MULTISLOT_Ttb;
    trb_tra = PL1_DTM_MULTISLOT_Tra;
  }

#endif

  /* Check the uplink TBF mac mode parameter and the number of uplink timeslots
   * allocated.
   */
  switch(l1_transfer_data[gas_id].current_alloc_type )
  {
  case  L1_DYNAMIC_ALLOC:
  case  L1_DYNAMIC_DOWN_ALLOC:

    /* Make sure the pointer has a valid address
     * before attempting to de-reference it.
     */
    if( NULL !=l1_transfer_data[gas_id].dynamic_tbf_ptr )
    {
      if(  ( MAC_L1_MAC_MODE_EXTENDED_DYNAMIC ==l1_transfer_data[gas_id].mac_mode )

#ifdef  FEATURE_GSM_GPRS_MSC33
         &&( PL1_CLASS12_Tra == trb_tra )
#endif
         /* No changes for MSC33/34 since Tta only applies
          * in 4 UL TS case on MSC12.
          */
         &&( 4 ==l1_transfer_data[gas_id].dynamic_tbf_ptr->nts )
        )
      {
        /* Power monitors are to be performed between the highest receive
         * and lowest transmit, apply Tta, Trb.
         */
#ifdef FEATURE_GSM_DTM

        if( L1_DTM_MODE == l1_tskisr_blk->l1_state )
        {
          ttb_tta = PL1_DTM_MULTISLOT_Tta;

          trb_tra = PL1_DTM_MULTISLOT_Trb;
        }
        else
        {
          ttb_tta = PL1_MULTISLOT_Tta;

          trb_tra = PL1_MULTISLOT_Trb;
        }
#else
        ttb_tta = PL1_MULTISLOT_Tta;

        trb_tra = PL1_MULTISLOT_Trb;
#endif

      }
    }
    else
    {
        MSG_GERAN_ERROR_0_G(" NULL dynamic_tbf_ptr ");
    }

    break;

  default:
    break;
  } /* End of switch statement on current_alloc_type */

  /* Initialise the cancel receive alloc to 0x0 */
  *l1_cancel_rx_alloc_ptr = 0x0;

  /* Check for Tra/Trb violation.
   * Use the equal sign because the number of timeslots in between is needed
   * and not simply the mathematical difference.
   */
  if( SUB_MOD( lowest_rx_tn, rrbp_equiv_rx_tn, GL1_DEFS_SLOTS_IN_FRAME ) <= trb_tra )
  {
    *l1_cancel_rx_alloc_ptr = ( 0x01 << rrbp_equiv_rx_tn );

    *l1_cancel_rx_alloc_ptr |=
        ( 0x01 << ( ADD_MOD( rrbp_equiv_rx_tn, 1, GL1_DEFS_SLOTS_IN_FRAME ) ) );

    if( 2 == trb_tra )
    {
        *l1_cancel_rx_alloc_ptr |=
        ( 0x01 << ( ADD_MOD( rrbp_equiv_rx_tn, 2, GL1_DEFS_SLOTS_IN_FRAME ) ) );
    }

    MSG_GERAN_ERROR_3_G( "Tra/Trb violation lowest ts %d highest ts %d rrbp ts %d",
              lowest_rx_tn,
              highest_rx_tn,
              rrbp_tx_tn );

    clash_found = TRUE;

    /* Inform client of Trb/Tra clash */
    *tra_trb_cancel_ts_alloc_ptr = *l1_cancel_rx_alloc_ptr;
  }
  else
  {
    /* Set the flag to FALSE */
    *tra_trb_cancel_ts_alloc_ptr = 0x0;
  }

  /* Check for Tta/Ttb violation.
   * Use the equal sign because the number of timeslots in between is needed
   * and not simply the mathematical difference.
   */
  /* Inlcude the case where the transmission falls in the next
   * TDMA frame boundary. "||( )" statement.
   */
  if( ( SUB_MOD( rrbp_equiv_rx_tn, highest_rx_tn, GL1_DEFS_SLOTS_IN_FRAME ) <= ttb_tta )
    ||( (rrbp_tx_tn + 3) < (highest_rx_tn - ( (highest_rx_tn >> 3) << 3 ) ) )
    )
  {
    *l1_cancel_rx_alloc_ptr = ( 0x01 << rrbp_equiv_rx_tn );

    *l1_cancel_rx_alloc_ptr |=
        ( 0x01 << ( SUB_MOD( rrbp_equiv_rx_tn, 1, GL1_DEFS_SLOTS_IN_FRAME ) ) );

    if( (rrbp_tx_tn + 3) < (highest_rx_tn - ( (highest_rx_tn >> 3) << 3 ) ) )
    {
        *l1_cancel_rx_alloc_ptr |=
            ( 0x01 << ( ADD_MOD( rrbp_equiv_rx_tn, 1, GL1_DEFS_SLOTS_IN_FRAME ) ) );
    }

    if( 2 == ttb_tta )
    {
        *l1_cancel_rx_alloc_ptr |=
            ( 0x01 << ( SUB_MOD( rrbp_equiv_rx_tn, 2, GL1_DEFS_SLOTS_IN_FRAME ) ) );
    }

    MSG_GERAN_ERROR_3_G( " Tta/Ttb violation lowest ts %d highest ts %d rrbp ts %d ",
              lowest_rx_tn,
              highest_rx_tn,
              rrbp_tx_tn );

    tta_ttb_clash_flag = TRUE;

    clash_found = TRUE;
  }

  /* Return the clash decision to client */
  return  clash_found;
}


/*===========================================================================

FUNCTION  GPL1_GPRS_PRACH_RRBP_START_OF_BLOCK

DESCRIPTION
  This function determines whether a particular frame number is the correct
  frame number to progam a RRBP transmission on a block boundary.

DEPENDENCIES
  Frame number mod52

RETURN VALUE
    Boolean defining yes / no

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_prach_rrbp_start_of_block(uint8 frame_no)
{
  boolean return_value = FALSE;

  /*  Determine if the current frame is a block programming frame.  */
  frame_no %= 13;

  switch (frame_no)
  {
  /*  Changed to four bursts before the required RRBP to allow for commanding
  NPL1 with not sending PRACH's in the two proceeding frames  */
  case  2:
  case  6:
  case  11:
    return_value = TRUE;
    break;

  default:

    break;

  } /*  End of switch */

  /*  Returns true if requested frame lies within an ordered block    */
  return (return_value);

} /*  End of function gpl1_gprs_prach_rrbp_start_of_block  */

/*===========================================================================

FUNCTION  gpl1_gprs_send_L1_MAC_RRBP_FAILURE_IND

DESCRIPTION
  Creates L1_MAC_RRBP_FAILURE_IND message and sends to the GS_QUEUE_MAC
  queue.

  The function informs MAC that a PDAN with CRD to be sent on the RRBP can not
  be transmitted

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_send_l1_mac_rrbp_failure_ind(boolean pdan_crd_flag, gas_id_t gas_id)
{

  l1_mac_sig_t   message_buffer,*message;

  message = &message_buffer;

  /* Set up the message header */
  message->task_id = MS_MAC_L1;
  message->sig_id =  L1_MAC_RRBP_FAILED_IND;
  message->msg.rrbp_failed_ind.rrbp_contained_chan_des = pdan_crd_flag;
  message->msg.rrbp_failed_ind.gas_id = gas_id;
  /* Send the message */
  L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);
}


/*===========================================================================

FUNCTION  gpl1_gprs_idle_rrbp_tx_callback

DESCRIPTION
  This function is called by Non Portable Layer 1 when a scheduled RRBP
  is sucessfully transmitted during idle mode.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_idle_rrbp_tx_callback( gl1_msg_tx_scheduled_data_type  msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
                                      uint8                           num_msg_txd,
                                      gas_id_t                        gas_id)
{
  MSG_GERAN_HIGH_3_G("sched cb  num txd %d [%d] msgs_txd[0] = %d",num_msg_txd, gl1_get_FN( gas_id ), *(msgs_txd[0].msg_data));
}


/*===========================================================================

FUNCTION   gpl1_gprs_idle_rrbp_discard_callback

DESCRIPTION
  This function is called by Non Portable Layer 1 during (idle mode) when a
  scheduled PACCH or PDTCH is discarded due to higher priority of another
  PDCH being scheduled or the message buffer in the mDSP is full.


  Passes back the pointer to the discarded message.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_idle_rrbp_discard_callback( gl1_msg_tx_discarded_data_type  msgs_discarded[ GL1_DEFS_MAX_UL_MSGS ],
                                           uint8                           num_msg_txd)
{
  if(num_msg_txd)
  {
  }
  MSG_GERAN_ERROR_1(" PDTCH DISCARDED CALLBACK msgs_discarded[0]=%d",*(msgs_discarded[0].msg_data));
}


/*===========================================================================

FUNCTION  GPL1_GPRS_PRACH_RRBP_TX_CALLBACK

DESCRIPTION
  This function is the callback function called during packet access mode
  when the RRBP has been successfully sent.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_prach_rrbp_tx_callback( gl1_msg_tx_scheduled_data_type  msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
                                       uint8                            num_msg_txd,
                                       gas_id_t gas_id )
{
  prach_params_T  *l1_prach_params_ptr = gl1_ms_switch_prach_params_idle_if_data(gas_id);
  NOTUSED(msgs_txd);

  /* Validate by checking num_msg_txd which should be 1 */
  if (num_msg_txd)
  {
    /*  Set abort frame number  */
    if (l1_prach_params_ptr != NULL && l1_prach_params_ptr->rrbp_pending == TRUE)
    {
      l1_prach_params_ptr->abort_fn = gl1_get_FN( gas_id );
    }
  }

} /*  End of function gpl1_gprs_prach_rrbp_tx_callback */


/*===========================================================================

FUNCTION  GPL1_GPRS_PRACH_RRBP_DISCARD_CALLBACK

DESCRIPTION
  This function is the callback function called during packet access
  mode when the RRBP has not been sent but discarded by NPL1.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_prach_rrbp_discard_callback( gl1_msg_tx_discarded_data_type  msgs_discarded[ GL1_DEFS_MAX_UL_MSGS ],
                                            uint8                           num_msg_txd,
                                            gas_id_t                        gas_id)
{
  /* vars assigned to themselves to keep lint happy */
  NOTUSED(msgs_discarded);
  NOTUSED(num_msg_txd);

  /*  Debug Message */
  MSG_GERAN_MED_1_G("RRBP DISCARD CALLBACK - RRBP FN %u", gl1_get_FN( gas_id ));

} /*  End of function gpl1_gprs_prach_rrbp_discard_callback  */


/*===========================================================================

FUNCTION  gpl1_gprs_transfer_find_rrbp

DESCRIPTION
  Search through rrbp stored entries with frame numbers  earlier or equal to
  fn_max and calculates the starting time.

DEPENDENCIES
  None

RETURN VALUE
  Boolean defining yes / no

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_transfer_find_rrbp(uint32 fn_max, uint32 *starting_time,gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  boolean    starting_time_present = FALSE;
  uint8      rrbp_index;
  rrbp_msg_t rrbp_msg_type;

  rrbp_index = gpl1_gprs_get_matching_rrbp(GPL1_INVALID_FN, fn_max, GPL1_INVALID_TN, gas_id);

  if (rrbp_index >= SIZE_OF_RRBP_BUFF)
  {
    return(starting_time_present);
  }

  rrbp_msg_type = gpl1_gprs_get_rrbp_msg_type(rrbp_index, gas_id);

  if ( (rrbp_msg_type == PCA_UL_FAI_MSG)    ||
    (rrbp_msg_type == PCA_DL_FAI_MSG)    ||
    (rrbp_msg_type == PDAN_MSG_WITH_FAI) ||
    (rrbp_msg_type == PDAN_MSG_WITH_CRD)
    )
  {
    /* Save the RRBP so move the suspend block to after RRBP frame */
    starting_time_present = TRUE;
    *starting_time  = (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num + 2) % FRAMES_IN_HYPERFRAME;
    MSG_GERAN_LOW_1_G("move susp [%d]",gl1_get_FN( gas_id ));
  }
  else
  {
    gpl1_gprs_delete_rrbp_entry(rrbp_index,gas_id);
  }

  return(starting_time_present);
}

/*===========================================================================

FUNCTION  gpl1_gprs_transfer_find_rrbp_type

DESCRIPTION
  Search through RRBP enteries for valid PCA or PDAN with FAI or PDAN
  with CRD RRBP and sets starting time

DEPENDENCIES
  None

RETURN VALUE
  Boolean defining yes / no

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_transfer_find_rrbp_type(uint32 *starting_time, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  boolean    starting_time_present = FALSE;
  uint8      rrbp_index = 0;
  uint32  temp_st_time1 ,temp_st_time2;
  temp_st_time1 = temp_st_time2 = GPL1_INVALID_FN;

  /* scan the rrbp list */
  while (rrbp_index < SIZE_OF_RRBP_BUFF)
  {
    if(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid)
    {
      /* Now check to see if we have to send the RRBP or delete it */
      switch (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type)
      {
      case PCA_UL_MSG:
      case PCA_DL_MSG:
      case PCA_UL_FAI_MSG:
      case PCA_DL_FAI_MSG:
      case PDAN_MSG_WITH_FAI:
      case PDAN_MSG_WITH_CRD:
        {
          /* Save the RRBP so move the suspend block to after RRBP frame */
          starting_time_present = TRUE;
          temp_st_time2 = ADD_FN(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num, 4);
          if((GPL1_INVALID_FN == temp_st_time1) || (IS_FRAME_NUM_LATER(temp_st_time2,temp_st_time1)))
          {
            temp_st_time1  = temp_st_time2;
          }
          MSG_GERAN_LOW_0_G("move susp");
          break;
        }
      default:
        break;
        /* ignore other rrbp msg types */
      }
    }

    rrbp_index++;
  }

  if(starting_time_present)
  {
    *starting_time  = temp_st_time1;
  }
  return(starting_time_present);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_matching_rrbp

DESCRIPTION
  Search through RRBP entries for matching Fn's and TN's

DEPENDENCIES
  None

RETURN VALUE
  returns index value to RRBP buffer

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_matching_rrbp(uint32 fn_min, uint32 fn_max, uint8 tn_num, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 rrbp_index ;
  uint8 valid_msg_found = 0;


  for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
  {
    if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == TRUE)
    {
      if ( ( ( tn_num == GPL1_INVALID_TN ) || ( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].timeslot == tn_num ) )                           &&
        ( ( (fn_min == fn_max)          && (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num == fn_min) )                            ||
        ( (fn_min == GPL1_INVALID_FN) && (gpl1_gprs_is_frame_num_later(fn_max,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num)) ) ||
        ( (fn_max == GPL1_INVALID_FN) && (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num >= fn_min) )                            ||
        ( (fn_min == fn_max)          && (fn_max == GPL1_INVALID_FN) ) ) )
      {
        return (rrbp_index);
      }
      else
      {
        valid_msg_found++;
      }
    }
  }

  if (valid_msg_found == SIZE_OF_RRBP_BUFF)
  {
    rrbp_index = GPL1_FULL_RRBP_BUFF;
  }

  return(rrbp_index);
}

/*===========================================================================

FUNCTION  gpl1_gprs_calc_num_ab_dl_rrbp

DESCRIPTION
  Search through RRBP entries for DL RRBP'S and increment ab_dl_rrbp counter
  if any PCA DL RRBP's of PDAN's with FAI set have been found

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_calc_num_ab_dl_rrbp(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint16 rrbp_index;

  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ab_dl_msgs = 0;

  for(rrbp_index=0;rrbp_index< SIZE_OF_RRBP_BUFF;rrbp_index++)
  {
    if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == TRUE)
    {
      if( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_DL_MSG) ||
        (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_DL_FAI_MSG) ||
        (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PDAN_MSG_WITH_FAI) )
      {
        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ab_dl_msgs++;
      }
    }
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_matching_rrbp_msg_type

DESCRIPTION
  Finds a valid matching RRBP message type from a list of RRBP entries

DEPENDENCIES
  None

RETURN VALUE
  returns index value to RRBP buffer

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_matching_rrbp_msg_type(rrbp_msg_t rrbp_msg_type, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 rrbp_index;

  for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
  {
    if ( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == TRUE) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == rrbp_msg_type) )
    {
      return (rrbp_index);
    }
  }
  return(rrbp_index);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_mathching_rrbp_index

DESCRIPTION
  Finds a valid matching RRBP message type from a list of RRBP entries

DEPENDENCIES
  None

RETURN VALUE
  returns index value to RRBP buffer

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_mathching_rrbp_index(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 rrbp_index;

  for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
  {
    if ( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == TRUE) &&
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_UL_MSG || 
	  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_UL_FAI_MSG || 
	  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_DL_MSG) )
    {
      break;
    }
  }
  return(rrbp_index);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_num_rrbp_msg

DESCRIPTION
  Returns total number rrbp messages remaining to be scheduled for transmission

DEPENDENCIES
  None

RETURN VALUE
  uint8 num_rrbp_msgs

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_num_rrbp_msg(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_dl_num_rrbp_msg

DESCRIPTION
  Returns only number rrbp messages for DL tbf remaining to be scheduled
  for transmission

DEPENDENCIES
  None

RETURN VALUE
  uint8 num_dl_msgs

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_dl_num_rrbp_msg(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_ul_num_rrbp_msg

DESCRIPTION
  Returns only number rrbp messages UL tbf remaining to be scheduled
  for transmission

DEPENDENCIES
  None

RETURN VALUE
  uint8 num_ul_msgs

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_ul_num_rrbp_msg(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_dl_ab_num_rrbp_msg

DESCRIPTION
  Returns only number rrbp messages abnormal DL tbf remaining to be scheduled
  for transmission

DEPENDENCIES
  None

RETURN VALUE
  uint8 num_ab_dl_msgs

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_dl_ab_num_rrbp_msg(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ab_dl_msgs);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_num_rrbp_msg_txd

DESCRIPTION
  Returns number of unacknowledged transmitted RRBP messages

DEPENDENCIES
  None

RETURN VALUE
  uint8 gpl1_gprs_rrbp.num_msgs_txd

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_get_num_rrbp_msg_txd(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_msgs_txd);
}

/*===========================================================================

FUNCTION  gpl1_gprs_reset_num_rrbp_msg_txd

DESCRIPTION
  Resets the number of transmitted rrbp msgs to zero.
  Currently used as part of the initialisation routine when first
  entering packet transfer mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_reset_num_rrbp_msg_txd( gas_id_t gas_id )
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_msgs_txd = 0;
}


/*===========================================================================

FUNCTION  gpl1_gprs_get_rrbp_frame

DESCRIPTION
Returns frame number of a given RRBP entry

DEPENDENCIES
  None

RETURN VALUE
  uint32 frame_number

SIDE EFFECTS
  None

===========================================================================*/
uint32 gpl1_gprs_get_rrbp_frame(uint8 rrbp_index, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_rrbp_msg_type

DESCRIPTION
Get RRBP message type for a given entry

DEPENDENCIES
  None

RETURN VALUE
  returns RRBP message type

SIDE EFFECTS
  None

===========================================================================*/
rrbp_msg_t gpl1_gprs_get_rrbp_msg_type(uint8 rrbp_index, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  return(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_lowest_rrbp_msg_FN

DESCRIPTION
Get FN, RRBP message type and (slammed) timeslot for the next RRBP
rrbp_tn  = slammed_tn  - is slammed TN

Finds the lowest RRBN FN to be be build and then populates the rrbp ts info.


DEPENDENCIES
  None

RETURN VALUE
  returns RRBP Frame Number

SIDE EFFECTS
  None

===========================================================================*/
uint32 gpl1_gprs_get_lowest_rrbp_msg_FN(rrbp_msg_t *rrbp_msg_type, pcr_format_t* rrbp_burst_type, uint8 *rrbp_ts, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 rrbp_index;
  uint8 slammed_tn = 0;
  uint32 rrbp_frame_num = GPL1_INVALID_FN,lowest_rrbp_frame_num = GPL1_INVALID_FN;
  boolean first_rrbp_entry = FALSE;

  if(gpl1_gprs_get_num_rrbp_msg(gas_id))
  {
    for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
    {
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid)
      {
        if (first_rrbp_entry == FALSE )
        {
          lowest_rrbp_frame_num =  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num;
          rrbp_frame_num = lowest_rrbp_frame_num;
          first_rrbp_entry = TRUE;
        }
        else
        {
          rrbp_frame_num =  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num;
        }
        //if ( rrbp_frame_num < lowest_rrbp_frame_num)
        if (! IS_FRAME_NUM_LATER(rrbp_frame_num,lowest_rrbp_frame_num))
        {
           lowest_rrbp_frame_num = rrbp_frame_num;
        }
      }
    }

    /* return the lowest RRBP Fn entry */
    for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
    {
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid)
      {
        if (lowest_rrbp_frame_num == gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num )
        {
           /* slammed_tn  - is slammed TN */
          slammed_tn = gpl1_gprs_ts_conv(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].timeslot,gas_id);
          rrbp_ts[slammed_tn] = slammed_tn;
          rrbp_msg_type[slammed_tn]   = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type;
          rrbp_burst_type[slammed_tn] = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_burst_type;
   
          rrbp_frame_num = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num; // same as lowest_rrbp_frame_num
        }
      }
    }
  }
  return rrbp_frame_num;
}

/*===========================================================================

FUNCTION  gpl1_gprs_check_for_rrbp_msg

DESCRIPTION
  Returns the first RRBP entry for a valid_msg

DEPENDENCIES
  None

RETURN VALUE
  Return index to RRBP buffer

SIDE EFFECTS
  None

===========================================================================*/
uint8 gpl1_gprs_check_for_rrbp_msg(boolean valid_msg, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 rrbp_index;

  for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
  {
    if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == valid_msg)
    {
      return (rrbp_index);
    }
  }
  return(rrbp_index);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_ul_rrbp_msg_fn

DESCRIPTION
  Returns the highest FN of PCA UL  message

DEPENDENCIES
  None

RETURN VALUE
  Returns the FN of PCA  message

SIDE EFFECTS
  None

===========================================================================*/
uint32 gpl1_gprs_get_ul_rrbp_msg_fn(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 rrbp_index;
  uint32 rrbp_fn = L1_GPRS_INVALID_FN;
  uint32 temp_rrbp_fn = L1_GPRS_INVALID_FN;

  for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
  {
    if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == TRUE)
    {
      if((gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_UL_FAI_MSG) ||
         (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_UL_MSG ))
      {
        temp_rrbp_fn = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num;

        /* to get highest rrbp FN */
        if ( rrbp_fn != L1_GPRS_INVALID_FN )
        {
          if ( gpl1_gprs_is_frame_num_later(temp_rrbp_fn,rrbp_fn))
          {
             rrbp_fn = temp_rrbp_fn;
          }
        }
        else
        {
          rrbp_fn = temp_rrbp_fn;
        }

        continue;
      }
    }
  }

  return (rrbp_fn);
}

/*===========================================================================

FUNCTION  gpl1_gprs_get_dl_ab_rrbp_msg_fn

DESCRIPTION
  Returns the highest FN of PCA DL/PDAN FAI  message

DEPENDENCIES
  None

RETURN VALUE
  Returns the FN of  rrbp message

SIDE EFFECTS
  None

===========================================================================*/
uint32 gpl1_gprs_get_dl_ab_rrbp_msg_fn(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 rrbp_index;
  uint32 rrbp_fn = L1_GPRS_INVALID_FN;
  uint32 temp_rrbp_fn = L1_GPRS_INVALID_FN;

  for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
  {
    if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == TRUE)
    {
      if((gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_DL_MSG) ||
         (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PCA_DL_FAI_MSG)||
         gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PDAN_MSG_WITH_FAI)
      {
        temp_rrbp_fn = gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num;

        /* to get highest rrbp FN */
        if ( rrbp_fn != L1_GPRS_INVALID_FN )
        {
          if ( gpl1_gprs_is_frame_num_later(temp_rrbp_fn,rrbp_fn))
          {
             rrbp_fn = temp_rrbp_fn;
          }
        }
        else
        {
          rrbp_fn = temp_rrbp_fn;
        }

        continue;
      }
    }
  }

  return (rrbp_fn);
}

/*===========================================================================

FUNCTION  gpl1_gprs_check_for_rrbp_duplicates

DESCRIPTION
  Check for any duplicated RRBP enteries.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_check_for_rrbp_duplicates(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 i,j,index;

  /*  Check if any of the RRBP stored for transmission are duplicated
  and break the multislot class */

  for (i=0; i<SIZE_OF_RRBP_BUFF; i++)
  {
    for (j=1;j<SIZE_OF_RRBP_BUFF;j++)
    {
      index = (i+j)%SIZE_OF_RRBP_BUFF;

      switch (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].rrbp_msg_type)
      {
      case PDAN_MSG:
      case PDAN_MSG_WITH_FAI:
      case PDAN_MSG_WITH_CRD:
        {
          /*  Only applicable to PDAN */
          if ((gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].msg_valid) && (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[index].msg_valid))
          {
            /*  Only applicable for valid messages  */
            if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].frame_num == gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[index].frame_num)
            {
              if( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].timeslot == gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[index].timeslot )
              {
                /* Initialise the violation boolean top FALSE */
                uint8  tra_trb_cancel_ts_alloc = 0x0;
                uint8  l1_cancel_rx_alloc      = 0x0;

                if( gpl1_gprs_calc_rrbp_tn_clash( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].timeslot,
                                                  &( tra_trb_cancel_ts_alloc ),
                                                  &( l1_cancel_rx_alloc ), gas_id )
                  )
                {
                  /*  Clash found remove rrbp */
                  gpl1_gprs_delete_rrbp_entry(i,gas_id);
                }
              }
            }
          }
        }

        break;

      default:
        break;
      }
    }
  }
  return;
}

/*===========================================================================

FUNCTION  gpl1_gprs_set_rrbp_coding_scheme

DESCRIPTION
Set the relevent coding schmeme for a given rrbp message

DEPENDENCIES
  None

RETURN VALUE
  uint8 Coding Scheme value

SIDE EFFECTS
  None

===========================================================================*/
gl1_defs_coding_type gpl1_gprs_set_rrbp_coding_scheme(uint8 rrbp_to_tx, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  gl1_defs_coding_type  coding_scheme = GL1_DEFS_CS1_CODING;

  if (rrbp_to_tx >= SIZE_OF_RRBP_BUFF)
    return(coding_scheme);


  switch(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_to_tx].rrbp_burst_type)
  {
  case NB:
    MSG_GERAN_MED_0_G("rrbp_burst_type = NB");
    break;

  case AB_8_BIT:
    coding_scheme = GL1_DEFS_ACCESS_CODING;
    MSG_GERAN_MED_0_G("rrbp_burst_type = AB_8");
    break;

  case AB_11_BIT:
    coding_scheme = GL1_DEFS_EXT_ACCESS_CODING;
    MSG_GERAN_MED_0_G("rrbp_burst_type = AB_11");
    break;

  default:
    /* report some exception here */
    MSG_GERAN_ERROR_1_G("bad cs %d",coding_scheme);
    break;
  }

  return(coding_scheme);
}

/*===========================================================================

FUNCTION  gpl1_gprs_delete_rrbp_entry

DESCRIPTION
  Delete RRBP entry from the stored RRBP array and updates the parameters

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_delete_rrbp_entry(uint8 rrbp_idx, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];

  /* delete the RRBP */
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].msg_valid = FALSE;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].frame_num = GPL1_INVALID_FN;

  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs--;


  /* decrement ul or dl message count depending on rrbp msg type */
  if ( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].rrbp_msg_type == PCA_UL_MSG)     ||
    (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].rrbp_msg_type == PCA_UL_FAI_MSG) ||
    (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].rrbp_msg_type == PRR_MSG)        ||
    (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].rrbp_msg_type == PCA_51_MSG)
    )
  {
    if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs > 0 )
    {
      gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs--;
    }
    else
    {
      MSG_GERAN_ERROR_0_G("Incorrect setting of UL RRBP");
    }
  }
  else
  {
    if(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs > 0 )
    {
      if ( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ab_dl_msgs &&
        ((gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].rrbp_msg_type == PCA_DL_MSG)      ||
        (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].rrbp_msg_type == PCA_DL_FAI_MSG)  ||
        (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].rrbp_msg_type == PDAN_MSG_WITH_FAI))
        )
      {
        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ab_dl_msgs--;

        MSG_GERAN_MED_1_G("ab dl count = %d",gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs);
      }

      gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs--;

    }
    else
    {
      MSG_GERAN_ERROR_0_G("Incorrect setting of DL RRBP");
    }
  }

   MSG_GERAN_MED_5_G( "delete rrbp entry %d num_rrbp_msgs=%d type [%d] num dl [%d] num ul [%d]"
           ,rrbp_idx
           ,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs
           ,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_idx].rrbp_msg_type
           ,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs
           ,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs );

  return;
}

/*===========================================================================

FUNCTION  gpl1_gprs_delete_dl_rrbp_entries

DESCRIPTION
  Deleting RRBP entries related to DL transfer

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_delete_dl_rrbp_entries(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 i;

  for(i=0;i<SIZE_OF_RRBP_BUFF;i++)
  {
    if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].msg_valid)
    {
      if( (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].rrbp_msg_type == PDAN_MSG_WITH_FAI) ||
        (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].rrbp_msg_type == PCA_DL_FAI_MSG)    ||
        (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].rrbp_msg_type == PDAN_MSG)          ||
        (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].rrbp_msg_type == PDAN_MSG_WITH_CRD) )
      {
        MSG_GERAN_HIGH_2_G("DL_RRBP_MSG deleted %d [%d]",gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].rrbp_msg_type,gl1_get_FN( gas_id ));

        gpl1_gprs_delete_rrbp_entry(i,gas_id);

        if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[i].rrbp_msg_type == PDAN_MSG_WITH_CRD)
        {
          /* Inform mac to retry sending PDAN with CRD */
          gpl1_gprs_send_l1_mac_rrbp_failure_ind(TRUE, gas_id);
        }
      }
    }
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_reset_rrbp_enteries

DESCRIPTION
  Initialise RRBP array and parameters

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_reset_rrbp_enteries(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];

  memset(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params,0, SIZE_OF_RRBP_BUFF*sizeof(rrbp_params_T) );
  memset(&gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info,0,sizeof(rrbp_txd_T));

  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_rrbp_msgs    = 0;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ul_msgs = 0;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs = 0;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_ab_dl_msgs = 0;
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_msgs_txd    = 0;

  return;
}

/*===========================================================================

FUNCTION  gpl1_gprs_reset_rrbp_msgs_txd

DESCRIPTION
Reset rrbp_msg_txd

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_reset_rrbp_msgs_txd(gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_msgs_txd    = 0;
  return;
}

/*===========================================================================

FUNCTION  gpl1_gprs_clean_rrbp_enteries

DESCRIPTION
  Tidying up RRBP array and remove old messages and check for current FN
  matched rrbp message

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  gpl1_gprs_clean_rrbp_enteries( uint32   current_fn,
                                     uint8    num_rx_pdch,
                                     uint8*   tra_trb_cancel_ts_alloc_ptr,
                                     uint8*   l1_cancel_ts_alloc_ptr, gas_id_t gas_id )
{
  /* Initialise local variables */
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  const uint32  fn_num     = ADD_FN( current_fn, NPL1_FN_OFFSET );
  uint8         rrbp_count = 0;
  uint32        timeslot   = GL1_DEFS_SLOTS_IN_FRAME;

  /* Remove expired rrbp messages */
  gpl1_gprs_delete_oldest_frame_num( current_fn, gas_id );

  /*check & delete any RRBP whose TN is falling beyond the TX window of our multi slot class*/
  gpl1_gprs_delete_out_of_tx_window_rrbp(current_fn,gas_id);

  /* Reset the tra/trb timeslot allocation */
  *tra_trb_cancel_ts_alloc_ptr = 0x0;

  while( 0 != timeslot-- )
  {
    /* Get the rrbp message destined for the next frame
     * and timeslot.
     */
    uint8  rrbp_index = gpl1_gprs_get_matching_rrbp( fn_num,
                                                     fn_num,
                                                     (uint8) timeslot, gas_id );
    if( rrbp_index < SIZE_OF_RRBP_BUFF )
    {
      /* Setup a pointer to the contents of this message */
      const rrbp_params_T*  params_ptr = &( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[ rrbp_index ] );

      /* This boolean is filled by the clash detection function so that
       * appropriate action is taken in the next two radio block periods.
       */
      uint8  tra_trb_cancel_ts_alloc = 0x0;
      uint8  l1_cancel_rx_alloc      = 0x0;
      uint8  pl1_multislot_sum       = PL1_MULTISLOT_SUM;

#ifdef FEATURE_GSM_DTM

      if( L1_DTM_MODE == l1_tskisr_blk->l1_state )
      {
          pl1_multislot_sum = PL1_DTM_MULTISLOT_SUM;
      }

#endif

      rrbp_count++;

      if( ( FALSE != gpl1_gprs_calc_rrbp_tn_clash( params_ptr->timeslot,
                                                   &( tra_trb_cancel_ts_alloc ),
                                                   &( l1_cancel_rx_alloc ), gas_id )
          )
        ||( ( rrbp_count + num_rx_pdch ) > pl1_multislot_sum )
        )
      {
         /*********************************************************
          ***
          *** Check if there is a  clash of RRBP and Rx Tn's
          *** (1) If RRBP is PDAN and it clashes, delete PDAN (04.60 sec 8.1.2.2)
          *** (2) If RRBP is PCA and it clashes, remove clashed Tn.
          ***
          *********************************************************/
         if(params_ptr->rrbp_msg_type == PCA_DL_FAI_MSG ||
            params_ptr->rrbp_msg_type == PDAN_MSG ||
            params_ptr->rrbp_msg_type == PDAN_MSG_WITH_FAI ||
            params_ptr->rrbp_msg_type == PDAN_MSG_WITH_CRD)
         {
            /* As above rrbp_msg_type is for DL TBF , DL TBF pointer should be valid and chcked for NULL */
            if( NULL == l1_tskisr_blk->current_params.L1Data.pTransfer_data->dynamic_tbf_ptr->dl_tbf_ptr)
            {
              *l1_cancel_ts_alloc_ptr      = 0;
              *tra_trb_cancel_ts_alloc_ptr = 0;

              MSG_GERAN_ERROR_0_G(" NULL dl_tbf_ptr ");

              return;
            }

            if(!(l1_tskisr_blk->current_params.L1Data.pTransfer_data->dynamic_tbf_ptr->dl_tbf_ptr->ts_allocation
                & tn_mask[params_ptr->timeslot]))
            {
                /* There is a tn clash in the rrbp but this rrbp wont be transmitted because that dl timeslot is no longer present
                in the dl allocation. Deletion of this rrbp happens when tx command is populated*/
                *l1_cancel_ts_alloc_ptr      = 0;
                *tra_trb_cancel_ts_alloc_ptr = 0;
                return;
             }
         }

         /* Check the rrbp message type */
         switch( params_ptr->rrbp_msg_type )
         {
             case  PCA_UL_MSG:
             case  PCA_DL_MSG:
             case  PCA_UL_FAI_MSG:
             case  PRR_MSG:
             case  PCA_51_MSG:
             case  PCA_DL_FAI_MSG:
             case  PDAN_MSG_WITH_FAI:
             case  PCCF_MSG:
             {
               /* If certain receive timeslots need to be ignored due to
                * Tra Trb violation then explicitly indicate that inorder
                * to take into account in 2 consecutive block periods.
                */
               *tra_trb_cancel_ts_alloc_ptr |= tra_trb_cancel_ts_alloc;

               /* Add in the receive timeslots to be cancelled
                * These include both the Tra/Trb violations in
                * addition to the Tta/Ttb violations.
                */
               *l1_cancel_ts_alloc_ptr |= l1_cancel_rx_alloc;
             }
             break;

             case  PDAN_MSG:
             case  PDAN_MSG_WITH_CRD:
             {
               transfer_data_T *transfer_data_ptr =  (transfer_data_T *)
                   l1_tskisr_blk->current_params.L1Data.pTransfer_data;

               /* If we are in concurrency and the PDAN timeslot is shared between
                * the uplink TBF and the downlink TBF then MS must respond to it.
                */
               if(  ( L1_DYNAMIC_DOWN_ALLOC == transfer_data_ptr->current_alloc_type )
                  &&( MAC_L1_MAC_MODE_EXTENDED_DYNAMIC == transfer_data_ptr->mac_mode )
                  &&( GPL1_INVALID != transfer_data_ptr->dynamic_tbf_ptr->usf_value[ params_ptr->timeslot ] )
                 )
               {
                 *tra_trb_cancel_ts_alloc_ptr |= tra_trb_cancel_ts_alloc;

                 *l1_cancel_ts_alloc_ptr |= l1_cancel_rx_alloc;
               }
               else
               /* If we are in concurrent UL/DL to comply with the spec 44.060 8.1.2.2 we need
               to ensure that the PDAN is sent as this takes precedence over the UL radio blocks
               */
               if(  ( L1_DYNAMIC_DOWN_ALLOC == transfer_data_ptr->current_alloc_type )
                  &&( MAC_L1_MAC_MODE_DYNAMIC == transfer_data_ptr->mac_mode )
                 )
               {
#ifdef FEATURE_GSM_DTM
                 /*Check if TTA/TTB clash is observed during DTM. It means that RRBP TS is coming
           adjacent to RX of CS */
             if(( L1_DTM_MODE == l1_tskisr_blk->l1_state ) && (tta_ttb_clash_flag == TRUE))
             {
                 gpl1_gprs_delete_rrbp_entry(rrbp_index,gas_id);
           MSG_HIGH( "RRBP PDAN message deleted bcz of TTA/TTB violation in DTM mode fn %d",current_fn,0,0 );
         }
                 else
                 {
                   *tra_trb_cancel_ts_alloc_ptr |= tra_trb_cancel_ts_alloc;
                   *l1_cancel_ts_alloc_ptr |= l1_cancel_rx_alloc;
                 }
#else
                 *tra_trb_cancel_ts_alloc_ptr |= tra_trb_cancel_ts_alloc;

                 *l1_cancel_ts_alloc_ptr |= l1_cancel_rx_alloc;
#endif
               }
               else
               {
                 switch( transfer_data_ptr->release_pending )
                 {
                 case  L1_FULL_RELEASE_DL:
                 case  L1_PARTIAL_RELEASE_DL:
                 {
                   if( 1 == gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs )
                   {
                     /****
                      **** Make sure we action the DL release which is waiting for
                      **** The last DL RRBP to transmitted, which now has been
                      **** deleted
                      ****/

                     /* Ensure we only set the  pca fn once!! */
                     if( FALSE == transfer_data_ptr->wait_for_pca_valid )
                     {
                       /* wait for any new assignments on the PACCH
                        * last block to be processed is block when PCA is sent
                        */
                       transfer_data_ptr->wait_for_pca_fn    = ADD_FN( current_fn, 10 );
                       transfer_data_ptr->wait_for_pca_valid = TRUE;

                       MSG_GERAN_MED_3_G( "last pdan removed %u, release fn[%u]",
                                current_fn,
                                transfer_data_ptr->wait_for_pca_fn,
                                0 );
                     } /* End of if( ) wait pca valid is FALSE */
                   } /* End of if( ) this is the last dl rrbp
                      * and dl release is pending
                      */
                 }
                   break;

                 default:
                   break;
                 }

                 /* Delete this entry from the RRBP queue */
                 gpl1_gprs_delete_rrbp_entry( rrbp_index,gas_id );

               } /* End of else statement */
             }
             break;

             default:
             {
               gpl1_gprs_delete_rrbp_entry( rrbp_index,gas_id );

             } /* End of default statement */
             break;

         } /* End of switch statement */
      }    /* End of if there is a timeslot clash or number of pdch exceeds limit. */
    }      /* End of if rrbp index is less than invalid */
  }        /* End of while timeslot != 0 */
  tta_ttb_clash_flag = FALSE;        /*Clear tta_ttb_clash_flag at end of function*/
}

/*===========================================================================

FUNCTION  gpl1_gprs_clean_rrbp_txd_info

DESCRIPTION
  Remove acknowledged transmitted rrbp messages from buffer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_clean_rrbp_txd_info( uint8                           num_msg_txd,
                                    gl1_msg_tx_scheduled_data_type  msgs_txd[ GL1_DEFS_MAX_UL_MSGS ],
                                    gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8 i, sched_msgs;

  for (i=0;i<num_msg_txd;i++)
  {
    sched_msgs = 0;

    while (sched_msgs < MAX_NUM_UL_RRBP_MSGS)
    {
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.rrbp_txd_buff[sched_msgs].rrbp_txd_buff == msgs_txd[i].msg_data)
      {
        MSG_GERAN_HIGH_3_G(" RRBP txd cs %d tn %d [%d]",
          gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.rrbp_txd_buff[sched_msgs].cs,
          gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.rrbp_txd_buff[sched_msgs].timeslot,
          gl1_get_FN( gas_id ) );

        /* clear out this entry as it's been reported */
        memset(&gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.rrbp_txd_buff[sched_msgs],0,sizeof(rrbp_txd_param_T));
        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.rrbp_txd_buff[sched_msgs].rrbp_txd_buff = NULL;
        gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.idx = SUB_MOD(gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.txd_info.idx,1,MAX_NUM_UL_RRBP_MSGS);
      }
      sched_msgs++;
    }
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_rrbp_to_abort_tbf_suspension

DESCRIPTION
Determines whether there are remaining RRBP messages to be transmitted in
  order aborting TBF suspension.

DEPENDENCIES
  None

RETURN VALUE
  Boolean abort_tbf_suspension yes/no

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_gprs_rrbp_to_abort_tbf_suspension( boolean tdscdma_client, uint32 fn_max,gas_id_t gas_id )
{
    gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
    boolean  abort_tbf_suspension = FALSE;

    /* Search the rrbp queue for non-pdan rrbp
     * msgs which cause the tbf suspension to
     * be cancelled.
     */
    uint32  i = SIZE_OF_RRBP_BUFF;

    const rrbp_params_T*  rrbp_param_ptr =  /* Initialise ptr to last element */
        &( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[ (SIZE_OF_RRBP_BUFF - 1) ] );

    while( i-- > 0 )
    {
      if( ( FALSE != rrbp_param_ptr->msg_valid )
        && !gpl1_gprs_find_rrbp_allows_TA(tdscdma_client,rrbp_param_ptr->rrbp_msg_type,gas_id))
      {
        if( rrbp_param_ptr->frame_num <= fn_max )
        {
          /* Cancel the TBF suspension */
          abort_tbf_suspension = TRUE;

          MSG_GERAN_MED_3_G( "rrbp type %d tbf susp aborted [%d]",
                   rrbp_param_ptr->rrbp_msg_type,
                   gl1_get_FN( gas_id  ),
                   0 );

          /* Leave while, no need to look in
           * the rest of the rrbp queue.
           */
          break;
        } /* End of if rrbp blk period falls within tbf susp period */
      }   /* End of if rrbp is valid and is not pdan */

      /* Move to the next element in the queue */
      --rrbp_param_ptr;

    } /* End of while loop */

    /* Search the rrbp queue for pdan rrbp
     * msgs which need to deleted.
     * Do this only if the previous search
     * did not cause the tbf suspension to
     * be cancelled.
     */
    if( FALSE == abort_tbf_suspension )
    {
      /* Initialise while loop counter */
      i = SIZE_OF_RRBP_BUFF;

      /* Initialise ptr to last element */
      rrbp_param_ptr = &( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[ (SIZE_OF_RRBP_BUFF - 1) ] );

      while( i-- > 0 )
      {
        if( ( FALSE != rrbp_param_ptr->msg_valid )
          && gpl1_gprs_find_rrbp_allows_TA(tdscdma_client,rrbp_param_ptr->rrbp_msg_type,gas_id))
        {
          if( rrbp_param_ptr->frame_num <= fn_max )
          {
            MSG_GERAN_MED_3_G( "PDAN RRBP vs. SUSPENSION [%u]",
                     gl1_get_FN( gas_id  ),
                     0,
                     0 );

            gpl1_gprs_delete_rrbp_entry( (uint8) i,gas_id );
            gpl1_set_expired_rrbp_msg_in_gta( TRUE, gas_id );

            if ( rrbp_param_ptr->rrbp_msg_type == PDAN_MSG_WITH_CRD )
            {
              /* Inform mac to retry sending PDAN with CRD */
              gpl1_gprs_send_l1_mac_rrbp_failure_ind(TRUE, gas_id);
            }
          
            /* Do not stop T3192 , becuase if we get PDA with CTRL ACK=1 , MAC will ignore it */
            if ( (rrbp_param_ptr->rrbp_msg_type == PDAN_MSG_WITH_FAI) || (PCA_DL_FAI_MSG == rrbp_param_ptr->rrbp_msg_type) )
            {
              grlc_dl_start_t3192(gas_id);
              MSG_GERAN_HIGH_2_G("RRBP %d is not sent : T3192_status %d",rrbp_param_ptr->rrbp_msg_type, grlc_dl_t3192_status(gas_id));
            }  

          } /* End of if rrbp blk period falls within tbf susp period */
        }   /* End of if rrbp is valid and is not pdan */

        /* Move to the next element in the queue */
        --rrbp_param_ptr;

      } /* End of while loop */
    }   /* End of if tbf suspension is still going ahead */

    /* Return boolean to client */
    return  abort_tbf_suspension;
}

/*===========================================================================

FUNCTION  gpl1_gprs_delete_oldest_frame_num

DESCRIPTION
  Deleting rrbp entries with oldest frame number older than the current frame
  number

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_delete_oldest_frame_num(uint32 current_fn, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8  rrbp_index;

  for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
  {
    if (
      (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == TRUE) &&
      (gpl1_gprs_is_frame_num_later(current_fn,gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num))
      )
    {
      MSG_GERAN_ERROR_3_G( "RRBP type %u Deleted FN passed! [%d] %d",
                 gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type,
                 current_fn,
                 gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num );

      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PDAN_MSG_WITH_CRD)
      {
        /* Inform mac to retry sending PDAN with CRD */
        gpl1_gprs_send_l1_mac_rrbp_failure_ind(TRUE, gas_id);
      }

      /* Do not stop T3192 , becuase if we get PDA with CTRL ACK=1 , MAC will ignore it */
      if (gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type == PDAN_MSG_WITH_FAI )
      {
        MSG_GERAN_HIGH_1_G("PL1: final pdan with fbi not sent : T3192_status %d",grlc_dl_t3192_status(gas_id));
      }

      gpl1_gprs_delete_rrbp_entry(rrbp_index,gas_id);

#ifdef FEATURE_G2X_TUNEAWAY
      /* release is pending in GTA gap and rrbp has expired */
      if ( gpl1_gprs_awaiting_release(gas_id) )
      {
        MSG_GERAN_HIGH_0_G("RRBP expired in GTA");
        gpl1_gprs_rrbp_data_ptr->rrbp_expired_in_gta = TRUE;
      }
#endif /* FEATURE_G2X_TUNEAWAY */
    }
  }
}

/*===========================================================================

FUNCTION  gpl1_gprs_delete_out_of_tx_window_rrbp

DESCRIPTION
  This function checks if any RRBP TN is falling outside the TX window fo rour MS class.

  If any found, delete that RRBP message

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_gprs_delete_out_of_tx_window_rrbp(uint32 current_fn, gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  uint8  rrbp_index;
  uint32        timeslot   = 0;
  const uint32  fn_num     = ADD_FN( current_fn, NPL1_FN_OFFSET );
  uint8 lowest_rrbp_tn = GPL1_INVALID_TN;
  uint8 lowest_ul_tn = GPL1_INVALID_TN;
  uint8 lowest_tx_tn = GPL1_INVALID_TN; 

  /*get the lowest tn of uplink assignment if any*/
  lowest_ul_tn = get_lowest_uplink_timeslot(gas_id);
  
  for( timeslot = 0; timeslot < GL1_DEFS_SLOTS_IN_FRAME ; timeslot++)
  {
    rrbp_index = gpl1_gprs_get_matching_rrbp( fn_num,
                                              fn_num,
                                              (uint8) timeslot, gas_id );

    if( rrbp_index < SIZE_OF_RRBP_BUFF )
    {
      if(lowest_rrbp_tn == GPL1_INVALID_TN)
      {
        /*store the lowest rrbp tn for this frame*/
        lowest_rrbp_tn = timeslot;

        /*find the lowest of all*/
        lowest_tx_tn = (lowest_ul_tn != GPL1_INVALID_TN ) ? MIN(lowest_rrbp_tn, lowest_ul_tn) : lowest_rrbp_tn;
      }

      /*check if this RRBP tn is falling outside our TX window*/
      if((lowest_tx_tn != GPL1_INVALID_TN) && 
         (timeslot > (lowest_tx_tn + PL1_MULTISLOT_TX -1 )))
      {
        MSG_GERAN_ERROR_4_G( "RRBP Deleted due to MAX TX window violation! type %u [FN %d] [TN %d] lowest tx tn %d",
                              gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].rrbp_msg_type,
                              current_fn,
                   timeslot,
                   lowest_tx_tn);
      
        gpl1_gprs_delete_rrbp_entry(rrbp_index,gas_id);
      }
    }
  }
}

uint8 get_lowest_uplink_timeslot(gas_id_t gas_id)
{
  uint8 lowest_tx_tn = GPL1_INVALID_TN;
  uint8 timeslot;
  
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  transfer_data_T *transfer_data_ptr =  (transfer_data_T *)
      l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  if( (transfer_data_ptr != NULL) &&
      (transfer_data_ptr->dynamic_tbf_ptr != NULL))
  {
    for( timeslot = 0; timeslot < GL1_DEFS_SLOTS_IN_FRAME ; timeslot++)
    {
      /*find the lowest tx TN from the ul tbf assignment*/
      if(GPL1_INVALID != transfer_data_ptr->dynamic_tbf_ptr->usf_value[timeslot])
      {
        lowest_tx_tn = timeslot;
        break;
      }
    }
  }
  return lowest_tx_tn;
}


/*===========================================================================

FUNCTION  gpl1_gprs_find_rrbp_during_suspension

DESCRIPTION
  This function searches rrbp queue for non-pdan msgs.

  If any are found then the search is aborted and client
  is informed that tbf suspension should be cancelled.

  If non are found then it searches queue for pdan msgs
  and deletes any ones that clash with the planned tbf suspension.

DEPENDENCIES
  None

RETURN VALUE
  boolean  allowed_to_go.

SIDE EFFECTS
  None

===========================================================================*/
boolean  gpl1_gprs_find_rrbp_during_suspension( boolean tdscdma_client, uint32  start_fn,
                                                uint32  end_fn ,gas_id_t gas_id)
{
    gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
    boolean  allowed_to_go = TRUE;

    /* Search the rrbp queue for non-pdan rrbp
     * msgs which cause the tbf suspension to
     * be cancelled.
     */
    uint32  i = SIZE_OF_RRBP_BUFF;

    const rrbp_params_T*  rrbp_param_ptr =  /* Initialise ptr to last element */
        &( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[ (SIZE_OF_RRBP_BUFF - 1) ] );

    while( i-- > 0 )
    {
      if( ( FALSE != rrbp_param_ptr->msg_valid )
        && !gpl1_gprs_find_rrbp_allows_TA(tdscdma_client,rrbp_param_ptr->rrbp_msg_type,gas_id))
      {
        uint32  rrbp_start = rrbp_param_ptr->frame_num;
        uint32  rrbp_end   = ADD_FN( rrbp_start, 3 );

        if( (  ( rrbp_start >= start_fn )
             &&( rrbp_start <= end_fn )
            )
          ||(  ( rrbp_end >= start_fn )
             &&( rrbp_end <= end_fn )
            ) )
        {
          /* Cancel the TBF suspension */
          allowed_to_go = FALSE;

          MSG_GERAN_MED_3_G( "rrbp type %d cancels tbf susp [%d]",
                   rrbp_param_ptr->rrbp_msg_type,
                   gl1_get_FN( gas_id  ),
                   0 );

          /* Leave while, no need to look in
           * the rest of the rrbp queue.
           */
          break;
        } /* End of if rrbp blk period falls within tbf susp period */
      }   /* End of if rrbp is valid and is not pdan */

      /* Move to the next element in the queue */
      --rrbp_param_ptr;

    } /* End of while loop */

    /* Search the rrbp queue for pdan rrbp
     * msgs which need to deleted.
     * Do this only if the previous search
     * did not cause the tbf suspension to
     * be cancelled.
     */
    if( FALSE != allowed_to_go )
    {
      /* Initialise while loop counter */
      i = SIZE_OF_RRBP_BUFF;

      /* Initialise ptr to last element */
      rrbp_param_ptr = &( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[ (SIZE_OF_RRBP_BUFF - 1) ] );

      while( i-- > 0 )
      {
        if( ( FALSE != rrbp_param_ptr->msg_valid )
          && gpl1_gprs_find_rrbp_allows_TA(tdscdma_client,rrbp_param_ptr->rrbp_msg_type,gas_id))
        {
          uint32  rrbp_start = rrbp_param_ptr->frame_num;
          uint32  rrbp_end   = ADD_FN( rrbp_start, 3 );

          if( (  ( rrbp_start >= start_fn )
               &&( rrbp_start <= end_fn )
              )
            ||(  ( rrbp_end >= start_fn )
               &&( rrbp_end <= end_fn )
              ) )
          {
            MSG_GERAN_MED_3_G( "SUSPEND vs. PDAN RRBP [%u]",
                     gl1_get_FN( gas_id  ),
                     0,
                     0 );

            gpl1_gprs_delete_rrbp_entry( (uint8) i ,gas_id);
            gpl1_set_expired_rrbp_msg_in_gta( TRUE, gas_id );

            if ( rrbp_param_ptr->rrbp_msg_type == PDAN_MSG_WITH_CRD )
            {
              /* Inform mac to retry sending PDAN with CRD */
              gpl1_gprs_send_l1_mac_rrbp_failure_ind(TRUE, gas_id);
            }
          
            /* Do not stop T3192 , becuase if we get PDA with CTRL ACK=1 , MAC will ignore it */
            if ( (rrbp_param_ptr->rrbp_msg_type == PDAN_MSG_WITH_FAI) || (PCA_DL_FAI_MSG == rrbp_param_ptr->rrbp_msg_type) )
            {
              grlc_dl_start_t3192(gas_id);
              MSG_GERAN_HIGH_2_G("RRBP %d is not sent : T3192_status %d",rrbp_param_ptr->rrbp_msg_type, grlc_dl_t3192_status(gas_id));
            }

          } /* End of if rrbp blk period falls within tbf susp period */
        }   /* End of if rrbp is valid and is not pdan */

        /* Move to the next element in the queue */
        --rrbp_param_ptr;

      } /* End of while loop */
    }   /* End of if tbf suspension is still going ahead */

    /* Return boolean to client */
    return  allowed_to_go;
}


boolean  gpl1_gprs_find_rrbp_allows_TA ( boolean tdscdma_client, uint32  rrbp_msg_t, gas_id_t gas_id)
{
   boolean tuneaway_allowed = FALSE;
   switch(rrbp_msg_t)
   {
    case PDAN_MSG:
      tuneaway_allowed = TRUE;
      break;

    case PDAN_MSG_WITH_CRD:
	  if(tdscdma_client)
      {
		MSG_GERAN_MED_1_G("G2X_TA SEQ Prioritise G2T TA over RRBP %d",rrbp_msg_t);        
        tuneaway_allowed = TRUE;
	  }
	  break;
	  
    case PDAN_MSG_WITH_FAI:
      if(tdscdma_client)
      {
	    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        transfer_data_T*  transfer_data_ptr =  l1_tskisr_blk->current_params.L1Data.pTransfer_data;
        switch( transfer_data_ptr->release_pending )
        {
             case  L1_FULL_RELEASE_UL:
             case  L1_FULL_RELEASE_DL:
             case  L1_FULL_RELEASE_UL_DL:
               MSG_GERAN_HIGH_1_G( "G2X_Seq DePrio G2T TA rel_pending=%d", transfer_data_ptr->release_pending );
			   tuneaway_allowed = FALSE;
			 break;
			 
			 default:
                MSG_GERAN_MED_1_G("G2X_TA SEQ Prioritise G2T TA over RRBP %d",rrbp_msg_t);        
                tuneaway_allowed = TRUE;
			    break;
		 
		}
      }
      break;
    default:
      break;

  }
 return tuneaway_allowed;
}


#if defined ( FEATURE_GSM_DTM ) && defined ( FEATURE_GSM_EDTM )

/* This function searches for the supplied msg type and
 * deletes it from the rrbp msg queue.
 */
void  gpl1_rrbp_delete_msg( uint8  csn_msg_type ,gas_id_t gas_id)
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];
  /* Search the rrbp queue for non-pdan rrbp
   * msgs which cause the tbf suspension to
   * be cancelled.
   */
  uint32  i          = SIZE_OF_RRBP_BUFF;
  uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id );

  /* Get the current tdma frame number */
  const uint32  next_fn = ADD_FN( current_fn, 1 );

  rrbp_params_T*  rrbp_param_ptr =  /* Initialise ptr to last element */
      &( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[ (SIZE_OF_RRBP_BUFF - 1) ] );

  /* Check every possible queue index value */
  while( i-- != 0 )
  {
    if(  ( FALSE != rrbp_param_ptr->msg_valid )
       &&( csn_msg_type == ((rrbp_param_ptr->rrbp_msg[ 1 ] & 0xFC)>>2U) )
       /* Make sure we dont delete something that is already being handled
        * by msg layer cause this will cause issues with tx callback.
        */
       &&( SUB_FN( rrbp_param_ptr->frame_num, next_fn ) <= (26U) )
      )
    {
      switch(l1_transfer_data[gas_id].release_pending )
      {
      case  L1_FULL_RELEASE_DL:
      case  L1_PARTIAL_RELEASE_DL:
      {
        if( 1 == gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.num_dl_msgs )
        {
          /****
           **** Make sure we action the DL release which is waiting for
           **** The last DL RRBP to transmitted, which now has been
           **** deleted
           ****/

          /* Ensure we only set the  pca fn once!! */
          if( FALSE ==l1_transfer_data[gas_id].wait_for_pca_valid )
          {
            /* wait for any new assignments on the PACCH
             * last block to be processed is block when PCA is sent
             */
           l1_transfer_data[gas_id].wait_for_pca_fn    = ADD_FN( next_fn, 9 );
           l1_transfer_data[gas_id].wait_for_pca_valid = TRUE;

            MSG_GERAN_MED_3_G( "last pdan removed %u, release fn[%u]",
                     next_fn,
                    l1_transfer_data[gas_id].wait_for_pca_fn,
                     0 );
                     } /* End of if( ) wait pca valid is FALSE */
          } /* End of if( ) this is the last dl rrbp
                      * and dl release is pending
                      */
        }
        break;

        default:
        break;

      } /* End of switch statement */

     /* Print an F3 message */
     MSG_GERAN_HIGH_2_G(" Delete rrbp csn type %u FNmod13 %u ", csn_msg_type, frame_counters[gas_id].FNmod13);

     /* Use already coded function */
      gpl1_gprs_delete_rrbp_entry( i,gas_id );
    }

    /* Decrement pointer position */
    --rrbp_param_ptr;

  } /* End of while loop */

  rrbp_param_ptr = &( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[ 0 ] );

}

#endif /* FEATURE_GSM_DTM && FEATURE_GSM_EDTM */

/* This function returns the RRBP single block type (HI vs LO) appropriate the the RRBP
 * message type. This is passed down to the message layer to indicate the priority of
 * the RRBP Tx.
 */
static gl1_defs_fixed_block_type gpl1_gprs_rrbp_msg_to_rrbp_hi_lo(rrbp_msg_t rrbp_msg_type)
{
  return ((rrbp_msg_type==PDAN_MSG) ||  (rrbp_msg_type==PMR_MSG)) ? GL1_DEFS_FIXED_RRBP_LO : GL1_DEFS_FIXED_RRBP_HI;
}

#ifdef FEATURE_G2X_TUNEAWAY

/*===========================================================================

FUNCTION  gpl1_expired_rrbp_msg

DESCRIPTION
  This function finds if any of the RRBF FN has passed the input frame.
  It can be used to check the expired RRBP.

DEPENDENCIES
  None

RETURN VALUE
  boolean  TRUE if any of the RRBP FN has passed the input frame.

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_expired_rrbp_msg( uint32 current_fn , gas_id_t gas_id)
{
  uint8  rrbp_index;

  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];

  if( gpl1_gprs_get_num_rrbp_msg(gas_id) )
  {
    for (rrbp_index=0; rrbp_index<SIZE_OF_RRBP_BUFF; rrbp_index++)
    {
      if ( ( gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].msg_valid == TRUE ) &&
           ( gpl1_gprs_is_frame_num_later( current_fn, gpl1_gprs_rrbp_data_ptr->gpl1_gprs_rrbp.params[rrbp_index].frame_num ) )
         )
      {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/* This function returns the variable rrbp_expired_in_gta
 */
boolean gpl1_expired_rrbp_msg_in_gta( gas_id_t gas_id )
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];

  return (gpl1_gprs_rrbp_data_ptr->rrbp_expired_in_gta);
}

/* This function sets the variable rrbp_expired_in_gta
 */
void gpl1_set_expired_rrbp_msg_in_gta( boolean expired, gas_id_t gas_id )
{
  gpl1_gprs_rrbp_data_t *gpl1_gprs_rrbp_data_ptr = &gpl1_gprs_rrbp_data[gas_id];

  gpl1_gprs_rrbp_data_ptr->rrbp_expired_in_gta  = expired;
}

#endif /* FEATURE_G2X_TUNEAWAY */

/* EOF */
