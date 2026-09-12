/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    L 1   D U A L  T R A N S F E R   M O D E   T A S K  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events while the L1
   task is in the L1 DTM mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_transfer_mode
    Procedure to handle external events while the L1 task is in the
    L1 transfer mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2005-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_dtm_if.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
11/12/17   mk        CR2152940: Revert CR 2001095 and related CR changes
03/05/18   rv        CR2233364 : Add feature flags for FR38904 changes - GERAN
11/08/17   rc        CR2084090  Holding the POWER_CONTROL processing for a block if CHANNEL_MODE_MODIFY_REQ is not handled yet in DTM mode.
23/01/18   rv        CR2128926 In case of G2X handover in DTM mode, stop the vocoder properly
14/12/17   hp        CR2158828 Release Tx chain when state changes from DTM_CHANNEL_REL to MULTI_SIM_FIND_BCCH
27/01/17   cws       CR1115987 Delay the turn tx off when transitioning from DTM to find BCCH in multisim
08/08/16   snjv      CR1043681 Release TRM/RF/FW when exiting DTM to MSim NULL Mode
22/08/17   nm        CR2096887 Reset tuneaway and ncell data when leaving transfer
30/05/17   rv        CR2031381: L1 fix to address issues with CR2001095 causing GCF failures
23/09/16   sn        CR1083503: Delay MAC_l1_RELEASE by one frame during DTM_INIT substate transition 
23/09/16   mn        CR1069317: Delay MAC_l1_RELEASE by one frame during DTM_INIT substate 
04/05/16   hd        CR1004583: Delay the vocoder stop and start till the point just before VFR 
                     is aligned to optimize the mute duration during the time of G2G handover
23/11/15   smd       CR942807 Copy the whole message structure of MPH_STOP_GSM_REQ to gl1_dtm_if_data
21/09/15   jk        CR911165: Set sub_sub_state to L1_DED_INIT while handling MPH_CHANNEL_MODE_MODIFY_REQ when aborting CS channel in DTM mode
21/07/15   cjl       CR876897 Fix null pointer dereference
17/06/15   cjl       CR853555 Use dynamic connected mode reasons for CS/PS traffic
30/03/15   am        CR813763 Backout CR460963
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr       CR818535 FR25951 Changes to support Single Voice Session 
05/12/14   sp        CR767083: Override paging multiframe to 2, for PDT tests
09/02/15   og        CR641070. Mutex protect the handling of the stop gsm mode req.
12/09/14   mko       CR460963: Dont reset SI mask while coming out from Transfer if we are camping back on to same ARFCN
28/05/14   ap        CR671971: Change SGLTE mode on reception of UE_MODE_CHANGE_REQ
18/08/14   br        CR707757 Add mutex lock in DTM state while handling Handover Req , to Prevent concurrent task update
14/08/14   sp        CR663691: Send any pending RelCnf to MAC when moving out of DTM to other States (except transfer)
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
30/05/14   ws        CR657674 Data + MMS Phase 1
05/05/14   cs        CR655385 Handle NAS-CB messages in DTM mode so SKIP_CNF sent
28/04/14   ws        CR641591 - EDTM Don't connect Pkt CS assignemt if reconfigured before DTM_CHANNEL_RELEAS_REQ
                     is received
06/05/14   npt       CR649118 Do not apply large negative time adjustment
07/09/13   br        CR537706: In GL1 , gpl1_gprs_handle_dtm_mode() does not handle the msg MPH_UE_MODE_CHANGE_REQ 
03/12/13   js        CR583478 l1_tskisr_blk remain uninitialized.
18/11/13   pjr       CR563429 Fixed vocoder handover on sub1 while sub 2 was in a call
12/11/13   mko       CR570221 Race condition so stop request is ignored after MAC channel release
11/26/13   sp        CR566318: Update DTM event after waiting a frame when Channel Mode Modify Cnf is being checked.
26/09/13   hd        CR550418: Clear TBF release pending flags when abort is triggered due to channel 
                     assignment or handover 
04/11/13   cja       CR570221 Race condition so stop request is ignored after MAC channel release
04/09/13   ws        CR538258 Fixed casting issue for MAC_L1_DL_TBF_CONFIG reception in DTM mode
20/08/13   mko       CR466596:gmactransfer.c:6302 GMAC AB Rel, Expected UL_DL (0x02) in PH_CONNECT_IND, received: 0x1
07/16/13   zc    	 CR470197 Replace memcpy with memscpy
15/03/13   ws        CR 463978 - Fix compile errors for QTF environment
28/10/11   mc        CR 313642 For dual SIM, ensure that the correct next state and
                     sub-state are selected during DTM channel release.
21/09/12   cja       CR400640 When abort DTM adjust gstmr when slam fro TS2 to TS0
17/08/12   ip        CR384421 Disable GSM sleep during CCO activity
19/03/11   rc        CR:344697 Featurized code under FEATURE_MODEM_NO_VOICE_SUPPORT to support no voice builds
27/10/11   ws        CR 310452 Remove FEATURE_INTER_RAT_HO_OPT from disabling vocoder
                     during DTM channel release.
15/04/11   ap        CR281977. Race condition between Channe Mode modify and
                     MAC_L1_DYNAMIC_TBF CONFIG in DTM Mode.
14/01/11   ab        CR 258308   : P0 write buffer overflow in func
                     gpl1_gprs_calculate_paging_data()
09-10-28   tjw       Clean up medium Lint errors
01/19/09   rc        Moved INVALID_ARFCN and ARFCNS_EQUAL in to gprsdef.h
15/01/09   nt        CR167218 - changes to restore old rot and pdm values after HO fails
28/07/98   hv        Added KxMutex support
18/06/08   dv        CR147809 - Handle DTM Blind HO case
10/04/08   ap        CR 113202 Handle MAC_L1_PDCH_REL_TS in DTM
27/03/08   nf        Added NV Item handling to R-SACCH and R-FACCH
17/01/08   cs        Lint fix
21/06/07   og        Ensure the afc information ise set correctly when doing G2G
                     non-blind handover. CR122525 (CR120843).
26/03/07   og        Add support for the MPH_STOP_NC_MEASUREMENTS_REQ message.
02/03/07   cs        Add support for new CGPS timetag
26/01/07   og        Implement UL ctrl msg deletion in L1.
23/01/07   og        Fix to compilation problems.
23/01/07   og        Added handling of the RR->L1 MPH_DTM_L1PARAM_UPDATE_REQ message.
30/10/06   og        DTM enhanced release code drop.
28/06/06   og        Change to dedicated mode before calling control function, resolves
                     CR88467.
09/05/06   og        Warn about PDH_REL message not being handled in DTM.
05/04/06   nt        CR90366 - Added code to handle STOP_GSM_MODE_REQ under DTM.
06/03/06   nt        CR89132 - Added backup of main command in handling of MAC_L1_RELEASE
22/02/06   nt        Put in handling of the MPH_RECONNECT_CHANNEL_REQ whilst in DTM
22/02/06   ws        Use packet transfer handler for MAC_PH_DATA_REQ. CR88106.
06/02/05   nt        Added code to handle simultaneous CMM and L1_REL. This uses the
                     new internal message MAC_L1_RELEASE_AND_RR_CMM_COMBINED to get
                     handled properly. L1 will now only signla to RR and MAC upon
                     completion of both of the proceses.
13/12/05   nt        Changed order of isr event setting to after task has completed its
                     message processing for MAC_L1_DYNAMIC_TBF_CONFIG,
                     MAC_L1_DL_TBF_CONFIG and MAC_L1_TS_RECONFIG. Problems seen at IOT
                     with isr running to reconfigure without pending data setup.
                     Also changed the calling of gl1_complete_channel_handover to
                     after the l1_tskisr_blk->sub_state gets set.
25/11/05   nt        Added handling of ASSIGNMENT and HANDOVER commands in DTM
02/11/05   nt        Added code to handle the DTM_ASSIGNMENT with DTM mode
28/10/05   nt        Added fix for UL+DL rel to release handler code. Also added
                     extra release ind handler when releasing but not returning to
                     dedicated.
                     Added fix for handling a dtm_channel_release above a mac_l1_rel
02/10/05   gfr       Use new gl1_msg_ul_rach API
19/09/05   nt        Added handling of the TBF assignment and reconfig messages
                     MAC_L1_TS_RECONFIG, MAC_L1_DYNAMIC_TBF_CONFIG, MAC_L1_DL_TBF_CONFIG
                     and releasing MAC_L1_RELEASE.
                     Also added handling of MPH_CHANNEL_MODE_MODIFY_REQ.
15/09/05   ws        Added dummy function to remove warning when FEATURE_GSM_DTM
                     is not defined
01/09/05   ws        Linted and code cleanup for FEATURE_GSM_DTM
24/08/05   ws        Changed prototype for MPH_DTM_CHANNEL_RELEASE_IND to
                     include cs_released and ps_released flags
24/08/05   ws        Added Missing code for L1-> L2 Messages
12/08/05   ws        Updated for initial DTM release
20/07/05   nt        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"


#ifdef FEATURE_GSM_DTM

#include "comdef.h"
#include "geran_tasks.h"
#include "gl1_msg_g.h"

#include "l1_isr.h"
#include "l1_sc_int.h"
#include "l1_task.h"
#include "task.h"
#include "l1_utils.h"
#include <stringl/stringl.h>

#include "gpl1_gprs_log.h"
#include "gpl1_gprs_task.h"
#include "gpl1_gprs_if.h"
#include "gpl1_gprs_isr.h"
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_utils.h"
#include "gprsdef.h"
#include "gl1_voice_server.h"
#include "gpl1_gprs_if_g.h"


#include "ms.h"
#include "sys_stru.h"
#include "msg.h" //for f3 message

#include "l1_sc.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

extern void gl1_setup_new_channel ( gas_id_t gas_id );
gl1_dtm_if_data_t  gl1_dtm_if_data[NUM_GERAN_DATA_SPACES];
extern dedicated_data_T l1_dedicated_data[];

/*===========================================================================

FUNCTION  gpl1_dtm_invalidate_mac_l1_release_from_message_store

DESCRIPTION
  This function simply invalidates the message held in the rr_mac message store.
  This is required if there was a stored message that no longer needs servicing.
  An example would be if a MAC_L1_RELEASE which was fully releasing back to
  dedicated simply becomes a partial release as a new assignment is received.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_dtm_invalidate_mac_l1_release_from_message_store( gas_id_t gas_id )
{
  gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[gas_id];
  /* Delete the message after checking it is a MAC_L1_RELEASE */
  if ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid &&
       ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set == MS_MAC_L1) &&
       ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MAC_L1_RELEASE)
     )
  {

    MSG_GERAN_HIGH_0_G("Invalidate MAC_L1_RELEASE message ");

    gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
  }
  else
    /* If message was combined with CMM remove the release only component*/
  if ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid &&
       ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set == MS_L1_L1) &&
       ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MAC_L1_RELEASE_AND_RR_CMM_COMBINED)
     )

  {
    MSG_GERAN_HIGH_0_G("Change MAC_L1_RELEASE_AND_RR_CMM_COMBINED to MPH_CHANNEL_MODE_MODIFY_REQ message ");
    gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_RR_L1;
    gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = (int) MPH_CHANNEL_MODE_MODIFY_REQ;
  }
  else
  {
    MSG_GERAN_HIGH_3_G("No MAC_L1_RELEASE message in buffer Curr SET=%d ID=%d ",
      gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set,
      gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id,
      0);
  }

}

/*===========================================================================

FUNCTION  gpl1_dtm_set_current_rr_mac_l1_msg_id

DESCRIPTION
  This function is an access function that simply sets the static
  current_rr_mac_l1_msg_id by passing in the IMH_T. It is used by the
  ded_if function

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_dtm_set_current_rr_mac_l1_msg_id( IMH_T message, gas_id_t gas_id )
{
  gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[gas_id];

  gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
  gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = message.message_set;
  gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = message.message_id;

}

#define  GL1_PKT_XFER_MAX_WAIT_FRAMES  (4)
/*===========================================================================

FUNCTION  gpl1_gprs_handle_dtm_mode

DESCRIPTION
  This function handles the L1 Task in Transfer mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gpl1_gprs_handle_dtm_mode(IMH_T *msg_header, gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk= NULL;
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

  if (msg_header->message_set == MS_MAC_L1)
  {
    switch (msg_header->message_id)
    {
    /********************************************************
    * MAC_L1 messages requiring handling by the ded_if file *
    * and having no immediate consequence therfore needing  *
    * no events setting for the DTM FSM                     *
    ********************************************************/
    case MAC_L1_FREQ_CONFIG:
    case MAC_L1_DUMMY_CTRL_BLOCK:
    case MAC_L1_DTM_CHANNEL_ASSIGN_REQ:
      {

        L1_handle_dedicated_mode(msg_header);
        break;

      }

    /****************************************************************
    * MAC_L1 messages requiring handling by the transfer_if file    *
    * and which require event setting causing ISR state transisiton *
    ****************************************************************/
    case MAC_L1_DYNAMIC_TBF_CONFIG:
    case MAC_L1_DL_TBF_CONFIG:
    case MAC_L1_TS_RECONFIG:
      {
        if( msg_header->message_id == MAC_L1_DYNAMIC_TBF_CONFIG )
        {
          mac_l1_dynamic_tbf_config_t* msg  = ( mac_l1_dynamic_tbf_config_t* )msg_header;
          gas_id = msg->gas_id;
        }else
        if( msg_header->message_id == MAC_L1_DL_TBF_CONFIG )
        {
           mac_l1_dl_tbf_config_t* msg = ( mac_l1_dl_tbf_config_t* ) msg_header;
           gas_id = msg->gas_id;
        }else
        {
           mac_l1_ts_reconfig_t* msg =  ( mac_l1_ts_reconfig_t* )msg_header;
           gas_id = msg->gas_id;
        }

        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        /***********************************************************************/
        /* Set the event to be sent to the DTM state machine, this may depend  */
        /* upon the current setting of the dtm_command_event. To prevent any   */
        /* undesirable behaviour due to race conditions the current setting is */
        /* checked, to make sure that another more important proceedure isn't  */
        /* cancelled/interrupted.                                              */
        /***********************************************************************/
        if ( (l1_get_dtm_command_event(gas_id) == L1_DTM_STOP) ||
             (get_dtm_isr_state(gas_id) == L1_DTM_CS_PS_ABORT) )
        {
          MSG_GERAN_HIGH_0_G("Unservicable MAC reconfig - RR is aborting DTM");
        }
        else
        if ( l1_get_dtm_command_event(gas_id) == L1_DTM_DEDICATED_RECONFIG )
        {
          gpl1_gprs_handle_transfer_mode(msg_header);

          /* A channel_mode_modify command is about to be processed */
          l1_set_dtm_command_event(L1_DTM_RECONFIG_BOTH_CS_PS,gas_id);
        }else
          if ((l1_dedicated_data_ptr->L1_confirm_msg == MPH_CHANNEL_MODE_MODIFY_CNF) &&
              (l1_tskisr_blk->sub_state    == L1_DTM_INIT) &&
              (l1_tskisr_blk->current_params.L1Data.pDtm_data->l1_dtm_init_mode == L1_DTM_CS_ONLY))
        /***********************************************************************/
        /* This condition is there to guard against the error where a          */
        /* channel mode modify confirmation was not being returned. This       */
        /* condition arises if Layer1 receives a channel mode modify request   */
        /* during DTM. L1 aborts the CS Channel configures the new channel     */
        /* and in the next frame is about to change channel and send CMM CNF.  */
        /* Before the frame executes L1 receives a MAC_L1_DYNAMIC_TBF_CONFIG.  */
        /* This changes the L1 sub states and prevents the CS channel change   */
        /* and CMM CNF message.                                                */
        /***********************************************************************/
        {
          MSG_GERAN_MED_1_G("delay a frame fn%d",GSTMR_GET_FN_GERAN( gas_id));
          (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
          (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
          MSG_GERAN_MED_1_G("fn %d",GSTMR_GET_FN_GERAN( gas_id));
          gpl1_gprs_handle_transfer_mode(msg_header);
          
#ifdef  FEATURE_GSM_EDTM
          if( L1_DTM_ENHANCED_RELEASE != l1_get_dtm_command_event(gas_id) )
#endif   
          {
           l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG,gas_id);
           MSG_GERAN_MED_2_G("Updating DTM Event to %d at fn %d",l1_get_dtm_command_event(gas_id), GSTMR_GET_FN_GERAN( gas_id));    
          }

        }
        else
        {
          gpl1_gprs_handle_transfer_mode(msg_header);
          
#ifdef  FEATURE_GSM_EDTM
          if( L1_DTM_ENHANCED_RELEASE != l1_get_dtm_command_event(gas_id) )
#endif   
          {
           l1_set_dtm_command_event(L1_DTM_TRANSFER_RECONFIG,gas_id);
          }
        }

        break;
      }

    /*************************************************************
    * MAC_L1 messages requiring handling by the transfer_if file *
    * and having no immediate consequence therfore needing no    *
    * events setting for the DTM FSM                             *
    **************************************************************/
    case MAC_L1_STOP_RLC_TX:
    case MAC_L1_CONTENTION_RES:
    case MAC_L1_PACCH_RRBP:
    case MAC_PH_DATA_REQ:
      {
        gpl1_gprs_handle_transfer_mode(msg_header);
        break;
      }

      /*******************************************************
      *                                                       *
      *                MAC_L1_PDCH_REL_TS                     *
      *                                                       *
      ********************************************************/
    case MAC_L1_CS_RECONFIG:
    case MAC_L1_POWER_CONTROL:
    case MAC_L1_PDCH_REL_TS:
      {
        uint8  wait_for_isr_count = 0;
        mac_l1_release_t  *message_ptr;
        gl1_dtm_if_data_t *gl1_dtm_if_data_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        gl1_dtm_if_data_ptr = &gl1_dtm_if_data[gas_id];

         GL1_ISR_SAVE_LOCK(gas_id);

         while((l1_tskisr_blk->main_command == L1_DEDICATED_STOP)&&
                 (gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid)&&
               (gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MPH_CHANNEL_MODE_MODIFY_REQ)&&
               (  wait_for_isr_count <= GL1_PKT_XFER_MAX_WAIT_FRAMES )
             )
        {
          GL1_ISR_SAVE_UNLOCK(gas_id);
          (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
          (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );

          MSG_GERAN_HIGH_1_G(" wait for isr to get FN, count %d ", wait_for_isr_count );

          wait_for_isr_count++;

          GL1_ISR_SAVE_LOCK(gas_id);
        }
        GL1_ISR_SAVE_UNLOCK(gas_id);
        gpl1_gprs_handle_transfer_mode(msg_header);

        break;
      }
      /*******************************************************
      *                                                       *
      *                MAC_L1_RELEASE                         *
      *                                                       *
      ********************************************************/
    case MAC_L1_RELEASE:
      {
        mac_l1_release_t  *message_ptr;
        gl1_dtm_if_data_t *gl1_dtm_if_data_ptr;
        boolean leave_dtm_state = FALSE;
        boolean leave_gprs_transfer = FALSE;
        uint16 backup_main_command;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

        message_ptr = (mac_l1_release_t *)msg_header;
        l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
        gl1_dtm_if_data_ptr = &gl1_dtm_if_data[message_ptr->gas_id];

        GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
        /*DTM_INIT needs one frame tick. Delay release by 1 frame to do graceful release*/
        if(l1_tskisr_blk->sub_state == L1_DTM_INIT)
        {
          MSG_GERAN_HIGH_0_G("Delay one frame");
          GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);
          (void)rex_clr_sigs(l1_task_tcb_read(gas_id), GPL1_WAIT_FOR_ISR_TO_GET_FN);
          (void)rex_wait( GPL1_WAIT_FOR_ISR_TO_GET_FN );
          GL1_ISR_SAVE_LOCK(message_ptr->gas_id);
        } 
        
        /*
        The l1_isrtsk_blk->main_command should never have been set up as
        L1_TRANSFER_COMMAND in DTM, but just as a precaution test for this and
        reset if TRUE. The reason is that this is widley refered to in the
        releasing code within gpl1_gprs_handle_tbf_release();
        */
        if( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
        {
          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
        }

        /*
        We need to be able to determine whether this release will cause a move out
        of the DTM state. This must be done prior to the gpl1_gprs_handle_tbf_release()
        function call as this may change essential parameters that give clues about the
        final expected outcome.
        */
        if ( ((message_ptr->release_tbf == DL_TBF_NORMAL) ||
              (message_ptr->release_tbf == DL_TBF_ABNORMAL)) &&
             ((l1_transfer_data[gas_id].pending_alloc_type == L1_DOWNLINK_ALLOC) ||
              (l1_transfer_data[gas_id].pending_alloc_type == L1_NULL_ALLOC))
           )
        {
          leave_dtm_state = TRUE;
        }
        else
        if ( ((message_ptr->release_tbf == UL_TBF_NORMAL) ||
              (message_ptr->release_tbf == UL_TBF_ABNORMAL)) &&
             ((l1_transfer_data[gas_id].pending_alloc_type == L1_DYNAMIC_ALLOC) ||
              (l1_transfer_data[gas_id].pending_alloc_type == L1_NULL_ALLOC))
           )
        {
          leave_dtm_state = TRUE;
        }
        else
        if ( (message_ptr->release_tbf == UL_AND_DL_TBF_NORMAL) ||
             (message_ptr->release_tbf == UL_AND_DL_TBF_ABNORMAL))
        {
          leave_dtm_state = TRUE;
        }

        /*
        To make sure that any event being handled isn't lost
        backit up to be restored after the function call
        */
        backup_main_command = l1_tskisr_blk->main_command;

        /*
        Call the transfer releasing function to release the apropriate TBF
        and inform the ISR to swap over to the new tbf parameters
         */
        leave_gprs_transfer = gpl1_gprs_handle_tbf_release( message_ptr->release_tbf, message_ptr->gas_id );


        l1_tskisr_blk->main_command = backup_main_command;


        /*
        Check to see if DTM mode can be stopped from here.
        Check there is no pending allocation and no RRBP's to transmit
        */
        if ( leave_gprs_transfer && leave_dtm_state )
        {
          /* The task can stop the TBF immediately */
          l1_set_dtm_command_event(L1_DTM_TRANSFER_STOP,message_ptr->gas_id);

          /* clear out the PACCH UL FIFO if we have any msgs remaining from transfer */
          gpl1_gprs_reset_pacch_fifo(gas_id);

          /* Ensure we clear all rrbp's before returning to dedicated */
          gpl1_gprs_reset_rrbp_enteries(message_ptr->gas_id);
        }
        else
        /* If only leave_gprs_transfer is true then this means that a TBF can been
        released now but there is a pending TBF so stay in this DTM state but send
        a release to MAC */
        if ( leave_gprs_transfer && !leave_dtm_state )
        {

          MSG_GERAN_HIGH_0_G("rel confirm gpl1_dtm_if");

          /*  Indicate to MAC release procedure is complete */
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending, message_ptr->gas_id);

          /* reset relelase type and rel confirm status*/
          l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
          l1_transfer_data[gas_id].confirm_pending = SENT;

        }

        /*
        Decide if the message should be saved. It should only be saved if
        terminating the DTM state and returning to dedicated. This will be for
        FULL releases only
        */
        if ( leave_dtm_state )
        {
          if ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid == FALSE )
          {
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_MAC_L1;
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = msg_header->message_id;
          }
          else
          if ( (gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set == MS_RR_L1) &&
               (gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MPH_CHANNEL_MODE_MODIFY_REQ) )
          {
              /* Overwrite the currently stored release message */
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_L1_L1;
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = MAC_L1_RELEASE_AND_RR_CMM_COMBINED;

            MSG_GERAN_HIGH_3_G("combining MAC_L1_RELEASE message in DTM for CMM set:%d id:%d",
              gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set,
              gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id,
              0);

          }
          else
          {
            MSG_GERAN_ERROR_3_G("Cannot handle MAC_L1_RELEASE set:0x%x id:0x%x in progress",
              gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set,
              gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id,
              0 );
          }
        }

        GL1_ISR_SAVE_UNLOCK(message_ptr->gas_id);

        break;
      }

      /*******************************************************
      *                                                      *
      *                MAC_L1_QUALITY_RPT                    *
      *                                                      *
      ********************************************************/
    case MAC_L1_QUALITY_RPT:
      {
        mac_l1_quality_rpt_t  *message_ptr;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;

        message_ptr = (mac_l1_quality_rpt_t *)msg_header;
        gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[message_ptr->gas_id];

        MSG_GERAN_LOW_2_G("PL1: MAC_L1_QUALITY_RPT Rcvd %d %d ",
                                                 gl1_get_FN(message_ptr->gas_id),
                                                 message_ptr->rrbp_value);

        if (message_ptr->rrbp_value != L1_GPRS_INVALID_FN)
        {
          /*  Set up quality reporting time to be starting time */
          if ((uint32)gpl1_gprs_adjust_mod_num
                                       (
                                         message_ptr->rrbp_value, - (int32) gl1_get_FN(message_ptr->gas_id),
                                         FRAMES_IN_HYPERFRAME) < (uint32) 8
                                       )
          {
            /* Increment the number of quality reports to send */
            gprs_serv_cell_meas_ptr->quality_report_count++;

            /*  Indicate associated rrbp_fn in response */
            gprs_serv_cell_meas_ptr->qrt = message_ptr->rrbp_value;

            /*  Sent quality report immediately */
            gpl1_gprs_build_quality_report(message_ptr->gas_id);

          } else
          {
            gprs_serv_cell_meas_ptr->quality_report_time
                      [gprs_serv_cell_meas_ptr->quality_report_time_index++] =
                                                                 message_ptr->rrbp_value;

            if (gprs_serv_cell_meas_ptr->quality_report_time_index > (MAX_NUM_QUAL_RPRTS - 1))
            {
              gprs_serv_cell_meas_ptr->quality_report_time_index = 0;
            }
          }

        } else
        {
          /* Increment the number of quality reports to send */
          gprs_serv_cell_meas_ptr->quality_report_count++;

          /*  Indicate associated rrbp_fn in response */
          gprs_serv_cell_meas_ptr->qrt = message_ptr->rrbp_value;

          /*  Sent quality report immediately */
          gpl1_gprs_build_quality_report(message_ptr->gas_id);

        }
        break;
      }

#ifdef  FEATURE_GSM_EDTM

    case  MAC_L1_EDTM_CS_REL_TBF_ALLOC:
    {
      const mac_l1_edtm_cs_rel_tbf_alloc_t* message_ptr =
        (mac_l1_edtm_cs_rel_tbf_alloc_t*) msg_header;

      /* Copy message contents over and set the message validity
       * boolean to TRUE.
       */
      gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[message_ptr->gas_id];

      gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc = *message_ptr;

      MSG_GERAN_MED_1_G(" MAC_L1_EDTM_CS_REL_TBF_ALLOC flg tbf_type %u ",message_ptr->tbf_type );

      /*When Packet CS release message is received then L1 need to discard the previous pending assignments */
      if (message_ptr->tbf_type != NO_TBF_PRESENT)
      {
        transfer_data_T*  transfer_data_ptr;
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        /* Setup a pointer to the current transfer data parameters */
        transfer_data_ptr = l1_tskisr_blk->current_params.L1Data.pTransfer_data;

        if (transfer_data_ptr->starting_time_valid == TRUE)
        {
          if(transfer_data_ptr->dl_st != L1_GPRS_INVALID_FN)
          {
            gas_id = message_ptr->gas_id;
            gpl1_gprs_delete_dl_rrbp_entries(gas_id);
            transfer_data_ptr->dl_st = L1_GPRS_INVALID_FN;
          }

          if (transfer_data_ptr->ul_st != L1_GPRS_INVALID_FN)
          {
            transfer_data_ptr->ul_st = L1_GPRS_INVALID_FN;
          }
          transfer_data_ptr->starting_time_elapsed = TRUE;
          transfer_data_ptr->starting_time_valid = FALSE;
        }
      }
    }
    break;

#endif

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

    case  MAC_L1_DELETE_UL_CTRL_MSG:
    {
      const mac_l1_delete_ul_ctrl_msg_t* message_ptr =
        (mac_l1_delete_ul_ctrl_msg_t*) msg_header;

      GL1_ISR_LOCK(message_ptr->gas_id);

      MSG_GERAN_MED_2_G(" MAC_L1_DELETE_UL_CTRL_MSG rcvd FN %u type %u ", GSTMR_GET_FN_GERAN( message_ptr->gas_id ),message_ptr->message_type);

      gpl1_queue_mac_l1_delete_ul_ctrl_msg( message_ptr->message_type, gas_id );

      GL1_ISR_UNLOCK(message_ptr->gas_id);
    }
    break;

#endif
#endif


    default:
      MSG_GERAN_ERROR_0_G("DTM MODE Bad message received");
      break;

    } /* End of switch */

  }
  else
  if (msg_header->message_set == MS_RR_L1)
  {
    switch ( msg_header->message_id )
    {

    /* Enable Sleep indication */
  case MPH_ENABLE_SLEEP_IND:
    {
    L1_handle_ENABLE_SLEEP_IND ( msg_header );
        break;
      }

    /********************************************************
    *                                                         *
    *                MPH_HANDOVER_REQ                         *
    *                                                         *
    ***********************************************************/
    case MPH_HANDOVER_REQ:
      {
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
		  gl1_dtm_if_data_t *gl1_dtm_if_data_ptr; 
          mph_handover_req_T* message_ptr = ( mph_handover_req_T* )msg_header;
          l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
          gl1_dtm_if_data_ptr = &gl1_dtm_if_data[message_ptr->gas_id];
          GL1_ISR_LOCK(message_ptr->gas_id);
          (void)L1_handle_dedicated_mode(msg_header);
          if(l1_tskisr_blk->l1_state != L1_DEDICATED_MODE)
          {

           gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
           gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_RR_L1;
           gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = (uint16)MPH_HANDOVER_REQ;

           l1_set_dtm_command_event(L1_DTM_STOP,message_ptr->gas_id);
          }
          GL1_ISR_UNLOCK(message_ptr->gas_id);
          break;
      }
#ifdef FEATURE_SGLTE    
               /********************************************************
               *                                                       *
               *                MPH_UE_MODE_CHANGE_REQ                 *
               *                                                       *
               ********************************************************/
            case MPH_UE_MODE_CHANGE_REQ:
              {
                mph_ue_mode_change_req_T  *message_ptr = (mph_ue_mode_change_req_T *)msg_header; /*lint !e740 unusual pointer cast */
                //   gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[message_ptr->gas_id];
                /* Set SGLTE mode - only expected on gas_id 0*/
                gl1_hw_set_sglte_mode(message_ptr->sglte_mode_active, message_ptr->gas_id);
                
                l1_sc_wcdma_set_mode(message_ptr->gsm_only, message_ptr->gas_id);
                break;
              }
#endif
    /********************************************************
    *                                                         *
    *                MPH_CHANNEL_ASSIGN_REQ                   *
    *                                                         *
    ***********************************************************/
    case MPH_CHANNEL_ASSIGN_REQ:
      {
        mph_channel_assign_req_T* message_ptr = ( mph_channel_assign_req_T* )msg_header;
        gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[message_ptr->gas_id];

        (void)L1_handle_dedicated_mode(msg_header);

        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_RR_L1;
        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = (uint16)MPH_CHANNEL_ASSIGN_REQ;

        l1_set_dtm_command_event(L1_DTM_STOP,message_ptr->gas_id);



        break;
      }
    /********************************************************
    *                                                         *
    *                MPH_RECONNECT_CHANNEL_REQ                *
    *                                                         *
    ***********************************************************/
    case MPH_RECONNECT_CHANNEL_REQ:
      {
        mph_reconnect_channel_req_T* message_ptr = ( mph_reconnect_channel_req_T* )msg_header;
        gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[message_ptr->gas_id];
        (void)L1_handle_dedicated_mode(msg_header);

        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_RR_L1;
        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = (uint16)MPH_RECONNECT_CHANNEL_REQ;

        l1_set_dtm_command_event(L1_DTM_STOP,message_ptr->gas_id);

        break;
      }
    /********************************************************
    *                                                         *
    *                MPH_CHANNEL_MODE_MODIFY_REQ              *
    *                                                         *
    ***********************************************************/
    case MPH_CHANNEL_MODE_MODIFY_REQ:
      {

        mph_channel_mode_modify_req_T  *message_ptr;
        gl1_dtm_if_data_t *gl1_dtm_if_data_ptr;

        message_ptr = (mph_channel_mode_modify_req_T *)msg_header; /*lint !e740 */
        gl1_dtm_if_data_ptr = &gl1_dtm_if_data[message_ptr->gas_id];

        if ( ( message_ptr->dedicated_channel_type == FULL_RATE_TRAFFIC )
          || ( message_ptr->dedicated_channel_type == HALF_RATE_TRAFFIC )
          )
        {
          /* Stop the SCE */
          l1_sc_stop(message_ptr->gas_id);

          /* Now fill in the new dedicated data */
          l1_dedicated_data_ptr->new_channel_specification.channel_mode_1 =
            message_ptr->channel_mode;

#ifdef FEATURE_GSM_AMR
          /* Now fill in the multirate configuration data */
          memscpy ( &l1_dedicated_data_ptr->new_multirate_configuration,
                   sizeof(l1_dedicated_data_ptr->new_multirate_configuration),
                   &message_ptr->multirate_config,
                   sizeof(multirate_config_T) );
#endif
#ifdef FEATURE_GSM_TRANSFER_DED_SERVING_CELL_INFO
          /* Previous cell applies only to inter-cell handovers */
          l1_dedicated_data_ptr->previous_cell_details.bsic_known = FALSE;
#endif

          /****************************************************************************/
          /* If there is a stored up MAC_L1_RELEASE then overwrite this with the      */
          /* MAC_L1_RELEASE_AND_RR_CMM_COMBINED. This way they can both get processed */
          /* simultaneously. Don't do this though if the DTM event is                 */
          /* L1_DTM_TRANSFER_STOP as this must go to the ISR to be processed to end   */
          /* DTM. In this one case we must reject the CMM.                            */
          /****************************************************************************/
          if ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid )
          {
            if ( (gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set == MS_MAC_L1) &&
                 (gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MAC_L1_RELEASE)
                  &&
                 (l1_get_dtm_command_event(message_ptr->gas_id) != L1_DTM_TRANSFER_STOP)
               )
            {
              /* Overwrite the currently stored release message */
              gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
              gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_L1_L1;
              gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = MAC_L1_RELEASE_AND_RR_CMM_COMBINED;

              MSG_GERAN_HIGH_3_G("combining MAC_L1_RELEASE message in DTM for CMM set:%d id:%d",
                gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set,
                gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id,
                0);

              if ( l1_get_dtm_command_event(message_ptr->gas_id) == L1_DTM_TRANSFER_RECONFIG )
              {
                /* Use this event if a MAC_L1_RELEASE has clashed with the
                MPH_CHANNEL_MODE_MODIFY_REQ*/
                l1_set_dtm_command_event(L1_DTM_RECONFIG_BOTH_CS_PS,message_ptr->gas_id);
              }
              else
              {
                l1_set_dtm_command_event(L1_DTM_DEDICATED_RECONFIG,message_ptr->gas_id);
              }

            }
            else
            {
              MSG_GERAN_ERROR_3_G("Can't handle MPH_CHANNEL_MODE_MODIFY_REQ set:0x%x id:0x%x",
                gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set,
                gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id,
                0 );

              /* Reject the MPH_CHANNEL_MODE_MODIFY_REQ */
              L1_send_MPH_CHANNEL_MODE_MODIFY_CNF(OTHER_ERROR,message_ptr->gas_id);

            }

          }

          /*******************************************************/
          /* Set up the correct dtm event, and save the message. */
          /*******************************************************/
          if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid == FALSE )
          {
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_RR_L1;
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = (uint16)MPH_CHANNEL_MODE_MODIFY_REQ;

            if ( l1_get_dtm_command_event(message_ptr->gas_id) == L1_DTM_TRANSFER_RECONFIG )
            {
              /* Use this event if a MAC_L1_RELEASE has clashed with the
              MPH_CHANNEL_MODE_MODIFY_REQ*/
              l1_set_dtm_command_event(L1_DTM_RECONFIG_BOTH_CS_PS,message_ptr->gas_id);
            }
            else
            {
              l1_set_dtm_command_event(L1_DTM_DEDICATED_RECONFIG,message_ptr->gas_id);
            }

          }
        }
        else
        {
          L1_send_MPH_CHANNEL_MODE_MODIFY_CNF(OTHER_ERROR,message_ptr->gas_id);
        }

        break;
      }
    /********************************************************
    *                                                         *
    *                Dedicated RR messages                    *
    *                                                         *
    ***********************************************************/
    case MPH_DEDICATED_UPDATE_REQ:
    case MPH_FREQUENCY_REDEFINITION_REQ:
    case MPH_SET_CIPHERING_MODE_REQ:
    case MPH_DEDICATED_WCDMA_CELL_UPDATE_LIST_REQ:
    case MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ:
    case MPH_STOP_NC_MEASUREMENTS_REQ:
      {
       (void)L1_handle_dedicated_mode(msg_header);
        break;
      }
    /**********************************************************
    *                                                         *
    *                MPH_DTM_CHANNEL_RELEASE_REQ              *
    *                                                         *
    ***********************************************************/
    case MPH_DTM_CHANNEL_RELEASE_REQ:
      {

        mph_dtm_channel_release_req_t  *message_ptr;
        idle_data_T               *l1_idle_data;
        gl1_dtm_if_data_t *gl1_dtm_if_data_ptr;
        gas_id_t gas_id;

        message_ptr = (mph_dtm_channel_release_req_t *)msg_header;
        gas_id = message_ptr->gas_id;
        l1_idle_data = &l1_idle_data_store[message_ptr->gas_id];
        gl1_dtm_if_data_ptr = &gl1_dtm_if_data[message_ptr->gas_id];

        MSG_GERAN_HIGH_1_G("DTM Task: Received dtm channel release req from L1 PSR:%d",message_ptr->ps_release);

#ifdef FEATURE_GSM_TRANSFER_DED_SERVING_CELL_INFO
        /* Previous cell applies only to inter-cell handovers */
        l1_dedicated_data_ptr->previous_cell_details.bsic_known = FALSE;
#endif

        /* Copy over any data */
        if (message_ptr->valid_bcch_carrier)
        {
          l1_idle_data->campedon_cell_ARFCN = message_ptr->serving_bcch_carrier;
          l1_idle_data->campedon_cell_BSIC = message_ptr->BSIC.BS_colour_code;
          l1_idle_data->campedon_cell_BSIC +=
                                          (byte)(message_ptr->BSIC.PLMN_colour_code * 8);

          /* Setup default BCCH decoding mask in case we go to Idle Mode */
          l1_idle_data->si_mask        = RR_L1_SI_TYPE_ALL_NORM;
          l1_idle_data->BCCH_mask      = gl1_compute_BCCH_mask(l1_idle_data->si_mask);
          l1_idle_data->ext_BCCH_mask  = gl1_compute_ext_BCCH_mask(l1_idle_data->si_mask);
        }
        else
        {
          l1_idle_data->campedon_cell_ARFCN.num = INVALID_ARFCN;

          /* Tell CGPS we are leaving this cell */
          gl1_timetag_cgps(L1_NULL_MODE, CGPS_TIMETAG_PRE_CELL_CHANGE,gas_id);
        }

        /* Store the message for post processing after the abort */
        if ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid )
        {
          MSG_GERAN_HIGH_3_G("Overwriting MAC_RR message in DTM for DCRR set:%d id:%d",
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set,
            gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id,
            0 );
        }

        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = TRUE;
        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set = MS_RR_L1;
        gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id = msg_header->message_id;

        l1_sc_stop(message_ptr->gas_id);

#ifdef FEATURE_GSM_EDTM
        if ( message_ptr->ps_release == FALSE)
        {
          GL1_ISR_SAVE_LOCK(gas_id);

          l1_set_dtm_command_event(L1_DTM_ENHANCED_RELEASE,gas_id);

          /* Add the handling of the MAC_L1_EDTM_CS_REL_TBF_ALLOC
           * message content here.
           */
          switch( gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.tbf_type )
          {
          case  NO_TBF_PRESENT:
            break;

          case  DL_TBF_PRESENT:
          {
            /* Create a downlink assignment message and pass it on
             * to the transfer message handler.
             */

            MSG_GERAN_MED_0_G(" Enh alloc DL TBF ");

            /* Fill the message using the stored info */
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.mac_mode             = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.mac_mode;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.ts_allocation        = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.ts_allocation;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.downlink_pwr         = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.downlink_pwr;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.tbf_start_time_valid = FALSE;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.tbf_start_time       = 0;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.ta_params            = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.ta_params;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.power_control        = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.power_control;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.dl_tfi_present       = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.dl_tfi_present;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.dl_tfi               = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.dl_tfi;

            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.bep_period2         = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.bep_period2;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.tbf_mode            = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.tbf_mode;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.link_qual_meas_mode = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.link_qual_meas_mode;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.rlc_mode            = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.rlc_mode;
            /* Msg id and set */
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.message_header.message_id  = MAC_L1_DL_TBF_CONFIG;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.message_header.message_set = MS_MAC_L1;
            gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config.gas_id = message_ptr->gas_id;
            /* Don't forget the extended timing advance information
             * present in the realloc message.
             * mac_l1_pkt_ext_ta_t             pkt_ext_ta;
             */

            gpl1_gprs_handle_transfer_mode( (IMH_T *) &gl1_dtm_if_data_ptr->mac_l1_dl_tbf_config ) ;
          }
            break;

          case  UL_TBF_PRESENT:
          {
            /* Create an uplink assignment message and pass it on
             * to the transfer message handler.
             */

            MSG_GERAN_MED_0_G(" Enh alloc UL TBF ");

            /* Fill the message using the stored info */
            gl1_dtm_if_data_ptr->mac_l1_dynamic_tbf_config.dynamic_tbf = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf;

            /* Msg id and set */
            gl1_dtm_if_data_ptr->mac_l1_dynamic_tbf_config.message_header.message_id  = MAC_L1_DYNAMIC_TBF_CONFIG;
            gl1_dtm_if_data_ptr->mac_l1_dynamic_tbf_config.message_header.message_set = MS_MAC_L1;
            gl1_dtm_if_data_ptr->mac_l1_dynamic_tbf_config.gas_id = message_ptr->gas_id;

            /* Don't forget the extended timing advance information
             * present in the realloc message.
             * mac_l1_pkt_ext_ta_t             pkt_ext_ta;
             */

            gpl1_gprs_handle_transfer_mode( (IMH_T *) &gl1_dtm_if_data_ptr->mac_l1_dynamic_tbf_config );
          }

            break;

          case  UL_DL_TBF_PRESENT:
          {
            /* Create a packet timeslot reconfigure message and pass it on
             * to the transfer message handler.
             */

            MSG_GERAN_MED_0_G(" Enh alloc UL & DL TBF ");

            /* Fill the message using the stored info */
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.uplink_tbf_type       = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.mac_mode;
#if 0
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.global_packet_ta      = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.timing_advance_params;
#endif
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.power_control_params  = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.power_control_params;

            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.starting_time_present = FALSE;
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.starting_time         = 0;

            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.dl_ctrl_param   = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.dl_ctrl_param;
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf_granularity = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf_granularity;

            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf[0] = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf[0];
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf[1] = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf[1];
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf[2] = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf[2];
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf[3] = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf[3];
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf[4] = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf[4];
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf[5] = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf[5];
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf[6] = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf[6];
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.usf[7] = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.ul_tbf.usf[7];

            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.dl_tfi_present   = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.dl_tfi_present;
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.dl_tfi           = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.dl_tfi;
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.dl_ts_allocation = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.ts_allocation;

            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.tbf_mode            = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.tbf_mode;
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.link_qual_meas_mode = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.link_qual_meas_mode;
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.rlc_mode            = gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.dl_tbf.rlc_mode;
            /* Msg id and set */
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.message_header.message_id  = MAC_L1_TS_RECONFIG;
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.message_header.message_set = MS_MAC_L1;
            gl1_dtm_if_data_ptr->mac_l1_ts_reconfig.gas_id = message_ptr->gas_id;
            /* Don't forget the extended timing advance information
             * present in the realloc message.
             * mac_l1_pkt_ext_ta_t             pkt_ext_ta;
             */

            gpl1_gprs_handle_transfer_mode( (IMH_T *) &gl1_dtm_if_data_ptr->mac_l1_ts_reconfig );
          }

            break;

          default:

            MSG_GERAN_ERROR_3_G( " Unknown enh alloc tbf type %u ",
                       gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.tbf_type,
                       0,
                       0 );
            break;
          }

          /* Reset the stored info */
          gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.tbf_type = NO_TBF_PRESENT;

          GL1_ISR_SAVE_UNLOCK(gas_id);

        }
        else
        {
#endif
        l1_set_dtm_command_event(L1_DTM_STOP,message_ptr->gas_id);

#ifdef  FEATURE_GSM_EDTM
        }
#endif

#ifdef FEATURE_GTA_GBTA_IN_2PHASE_ACCESS
        GL1_ISR_SAVE_LOCK(gas_id);
        gpl1_gprs_reset_tuneaway_and_ncell_data(gas_id);
        GL1_ISR_SAVE_UNLOCK(gas_id);
#endif /* FEATURE_GTA_GBTA_IN_2PHASE_ACCESS*/

        break;
      }
    /**********************************************************
    *                                                         *
    *                  MPH_STOP_GSM_MODE_REQ                  *
    *                                                         *
    ***********************************************************/
    case MPH_STOP_GSM_MODE_REQ:
      {
        gas_id_t gas_id;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        mph_stop_gsm_mode_req_T *stop_gsm_mode_req = (mph_stop_gsm_mode_req_T*)msg_header;
        gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[stop_gsm_mode_req->gas_id];
		l1_tskisr_blk = &l1_tsk_buffer[stop_gsm_mode_req->gas_id];

        GL1_ISR_LOCK(stop_gsm_mode_req->gas_id);

        gas_id = stop_gsm_mode_req->gas_id;
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        /* Store stop_gsm_req and process it later */
        gl1_dtm_if_data_ptr->mph_stop_gsm_mode_req = (*stop_gsm_mode_req);

        gl1_save_current_cell_data( FALSE,stop_gsm_mode_req->gas_id );
		

		
        /* Save the message for processing later */
        gpl1_dtm_set_current_rr_mac_l1_msg_id(*msg_header,stop_gsm_mode_req->gas_id);

        /* Stop SCE */
        l1_sc_stop(gas_id);


        if (l1_tskisr_blk->l1_state == L1_DTM_MODE)
        {
          l1_set_dtm_command_event(L1_DTM_STOP,stop_gsm_mode_req->gas_id);

          GL1_ISR_UNLOCK(stop_gsm_mode_req->gas_id);
        }
        else
        {
           l1_tskisr_blk->sub_state = L1_DED_SUSPEND;

          GL1_ISR_UNLOCK(stop_gsm_mode_req->gas_id);

           L1_setup_DEACTIVATE( msg_header );
           l1_set_dtm_command_event(L1_DTM_NULL_COMMAND,stop_gsm_mode_req->gas_id);
        }
        break;
      }

#ifdef  FEATURE_GSM_EDTM

    case  MPH_DTM_L1PARAM_UPDATE_REQ:
      {
        const mph_dtm_l1param_update_req_t*  msg_ptr =
          (const mph_dtm_l1param_update_req_t*)  msg_header;
        idle_data_T               *l1_idle_data=&l1_idle_data_store[msg_ptr->gas_id];

        MSG_GERAN_MED_1_G("MPH_DTM_L1PARAM_UPDATE_REQ",
          0);
          /* Config the egprs quality measurements module using the bep period
           * broadcast on the BCCH.
           */
          gpl1_egprs_scell_qual_meas_broadcast_config
          ( msg_ptr->l1_system_info_params.ccch_idle_mode_params.layer1_extra_gprs_sys_info.bep_period, msg_ptr->gas_id );
          l1_idle_data->campedon_cell_data = msg_ptr->l1_system_info_params.ccch_idle_mode_params.layer1_sys_info; /* Struct to struct copy */

          l1_idle_data->campedon_cell_data.BS_PA_MFRMS = l1_override_bs_pa_mfrms(l1_idle_data->campedon_cell_data.BS_PA_MFRMS, gas_id);
      } /* End of case MPH_DTM_L1PARAM_UPDATE_REQ */
      break;

#endif /* FEATURE_GSM_EDTM */

    /*******************************************************
    *                                                      *
    *                MPH_SET_PRIORITY_IND                  *
    *                                                      *
    ********************************************************/
    case MPH_SET_PRIORITY_IND:
      {
        mph_set_priority_ind_T* message_ptr = (mph_set_priority_ind_T* )msg_header;

        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        if (message_ptr->procedure_type == GERAN_CONN_PROCEDURE)
        {
          grm_check_connected_reason(
          l1_tskisr_blk->client_id,
          GRM_CS_CONNECTION,
          gas_id);
        }
        break;
      }

    default:
      MSG_GERAN_ERROR_1_G("Invalid MS_RR_L1 message id %d",msg_header->message_id);

      break;
    } /* End of switch  */
  }
  else if (msg_header->message_set == MS_L2_L1)
  {


    switch (msg_header->message_id)
    {
      /* Request to send data on the LAPDm channel */

      /********************************************************
       *                                                      *
       *                PH_DATA_REQ                           *
       *                                                      *
       ********************************************************/

      case PH_DATA_REQ:
      {
            ph_data_req_T  *message_ptr;
            l1_dcch_buffer_T  *dcch_buffer_ptr;
            l1_sacch_buffer_T *sacch_buffer_ptr;

            message_ptr = (ph_data_req_T *)msg_header; /*lint !e740 */

            MSG_GERAN_HIGH_1_G("Rcvd PH_DATA_REQ chan_type %d",message_ptr->l2_channel_type);

            /* Check which channel the message is to be sent on */
            if (message_ptr->l2_channel_type == DCCH)
            {
                dcch_buffer_ptr = &l1_dcch_s0_buffer[gas_id];

                /* Check if the buffer is full */
                if (dcch_buffer_ptr->data_valid == TRUE)
                {
                    /* Buffer is full */
#ifdef HL_DEBUG
                    #error code not present
#endif
                }

                else
                {
                    memscpy(dcch_buffer_ptr->dcch_data,
						    sizeof(dcch_buffer_ptr->dcch_data),
                            message_ptr->layer2_frame,
                            DCCH_MESSAGE_UNIT);

                    dcch_buffer_ptr->data_valid = TRUE;
                }
            }

            else
            {
                /* It's a SACH message */
                /* Set the message pointer */
                sacch_buffer_ptr = &l1_sacch_s0_buffer[gas_id];

                /* Check if the buffer is full */
                if (sacch_buffer_ptr->data_valid)
                {
                    /* Buffer is full */
#ifdef HL_DEBUG
                    #error code not present
#endif
                }

                else
                {
                    memscpy(sacch_buffer_ptr->sacch_data,
						    sizeof(sacch_buffer_ptr->sacch_data),
                            message_ptr->layer2_frame,
                            SACCH_MESSAGE_UNIT);

                    sacch_buffer_ptr->data_valid = TRUE;
                }
                if(l1_dedicated_data_ptr->rsacch_ctrl.nv_active == TRUE)
                {
                    /*copy repetition info flag*/
                    l1_dedicated_data_ptr->rsacch_ctrl.l2_repetition_info = message_ptr->l2_repetition_info;
                    MSG_GERAN_HIGH_2_G("R-SACCH: UL PH_DATA_REQ (DTM) Rep %d, BufV %d",message_ptr->l2_repetition_info,l1_dedicated_data_ptr->rsacch_ctrl.buffer_valid);
                }
            }
            break;
        }
        /*******************************************************
         *                                                      *
         *                PH_EMPTY_FRAME_REQ                    *
         *                                                      *
         ********************************************************/

    case PH_EMPTY_FRAME_REQ:
        {
            /* Request to send blank data on the LAPDm channel */

            ph_empty_frame_req_T  *message_ptr;
            l1_dcch_buffer_T  *dcch_buffer_ptr;
            l1_sacch_buffer_T *sacch_buffer_ptr;

            message_ptr = (ph_empty_frame_req_T *)msg_header; /*lint !e740 */

            /* Check which channel the message is to be sent on */
            if ( message_ptr->l2_channel_type == DCCH)
            {
                dcch_buffer_ptr = &l1_dcch_s0_buffer[gas_id];

                /* Check if the buffer is full */
                if (dcch_buffer_ptr->data_valid)
                {
                    /* Buffer is full */
#ifdef HL_DEBUG
                    #error code not present
#endif
                }
                else
                {
                    memscpy(dcch_buffer_ptr->dcch_data,
						    sizeof(dcch_buffer_ptr->dcch_data),
                            fill_frame,
                            DCCH_MESSAGE_UNIT);

                    dcch_buffer_ptr->data_valid = TRUE;
                }
            }

            else
            {
                /* It's a SACH message */
                /* Set the message pointer */
                sacch_buffer_ptr = &l1_sacch_s0_buffer[gas_id];

                /* Check if the buffer is full */
                if (sacch_buffer_ptr->data_valid)
                {
                    /* Buffer is full */
#ifdef HL_DEBUG
                    #error code not present
#endif

                }

                else
                {
                    memscpy(sacch_buffer_ptr->sacch_data,
						    sizeof(sacch_buffer_ptr->sacch_data),
                            fill_frame,
                            SACCH_BUFFER_SIZE);

                    sacch_buffer_ptr->data_valid = TRUE;
                }
            }
            break;
        }
        /*******************************************************
         *                                                      *
         *                PH_RANDOM_ACCESS_REQ                  *
         *                                                      *
         ********************************************************/
    case PH_RANDOM_ACCESS_REQ:
        {
            /*
                Potential race condition - check when testing
                If the random access req comes in before the
                dedicated mode on current cell has been suspended
                and target cell details loaded, then target_cell_details
                will be incorrect.  In that case HANDOVER_IND will have to
                be sent after dedicated mode has been suspended
            */

            ph_random_access_req_T   *message_ptr;
            idle_data_T               *l1_idle_data;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
            gl1_msg_rach_cb_type cb;

            message_ptr = ( ph_random_access_req_T *) msg_header; /*lint !e740 */
            l1_idle_data=&l1_idle_data_store[message_ptr->gas_id];
            l1_tskisr_blk = &l1_tsk_buffer[message_ptr->gas_id];
            l1_dedicated_data_ptr->access_burst_content =
            message_ptr->random_access_content;

            /* No callbacks required for handover access bursts -
               just transmit RACHs.  No need to read anything on the
               AGCH
            */
            cb.data_cb    = NULL;
            cb.abort_cb   = NULL;

            /* Configure handover access bursts */
            gl1_msg_cfg_ul_rach ( NULL,l1_idle_data->campedon_cell_BSIC,
                                  GL1_MSG_RACH_HANDOVER, &cb,gas_id);

            /* Restart L1 in dedicated mode */
            GL1_ISR_LOCK(message_ptr->gas_id);
            l1_tskisr_blk->main_command = L1_DEDICATED_COMMAND;
            l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;
            GL1_ISR_UNLOCK(message_ptr->gas_id);
            break;
        }

        /********************************************************
         *                                                         *
         *                MPH_STOP_SACCH_MEASUREMENT_REPORT        *
         *                                                         *
         ***********************************************************/

    case PH_STOP_SACCH_MEASUREMENT_REPORT:
        {
          ph_stop_sacch_measurement_report_T* message_ptr = ( ph_stop_sacch_measurement_report_T* )msg_header;
            /* Stop sending SACCH measurement reports as soon as
               L2 tells us too.    This is to fix the issue with
               the network receiving a measurement report after it
               has commanded a release
            */
            l1_dedicated_data_ptr->stop_meas_rpt = TRUE;

            GL1_ISR_LOCK(message_ptr->gas_id);
            /* The change is propagated to the ISR */
            L1_transfer_new_data( message_ptr->gas_id);
            GL1_ISR_UNLOCK(message_ptr->gas_id);
            break;
        }

    default:
        MSG_GERAN_ERROR_1_G("L2->L1(DED) message %d ignored.", msg_header->message_id);
        break;
    } /* end of switch */

  }
  else
  if ( ( msg_header->message_set == MS_L1_L1 ) ||
       ( msg_header->message_set == MS_CB_L1 ) )
  {
    /* Handling if INTRA L1 and CB-L1 messaging done in l1_ded_if */
    L1_handle_dedicated_mode( msg_header);
  }
  else
  {
    MSG_GERAN_ERROR_1_G("Invalid MS_xx_L1 message from unknown source %d",
                                                            msg_header->message_set);
  } /* end of (if message_set ... */



} /* End of Function */

/*===========================================================================

FUNCTION  gpl1_dtm_complete_abort_procedure

DESCRIPTION
  This function controls the abort process. It will firstly complare the abort
  status with that expected for the abort message. If TRUE then the apropriate
  function is called to complete the abort, setting up the new states etc.
  ISR context - no need for INTLOCK

DEPENDENCIES
  l1_tskisr_blk

RETURN VALUE
  boolean TRUE/FALSE depends on whether abort has been handled.

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_dtm_complete_abort_procedure( l1_dtm_mode_T abort_status, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[gas_id];
  boolean abort_processed = FALSE;
  transfer_data_T          *transfer_data_ptr =
                                     l1_tskisr_blk->current_params.L1Data.pTransfer_data;

  if ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid == FALSE )
  {
    MSG_GERAN_ERROR_0_G("No messages to handle in gpl1_dtm_complete_abort_procedure()");
    return abort_processed;
  }

  MSG_GERAN_MED_3_G("dtm_abort_procedure: abort_status=%d, msg=(%d,%d)", 
     abort_status, 
     gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set,
     gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id);

  /*********************************************************/
  /* Get the message that has kicked off the abort and the */
  /* check that the abort has completed by referenceing the*/
  /* abort status. Run the correct code to complete the    */
  /* abort and return                                      */
  /*********************************************************/
  switch ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_set )
  {

  case MS_L1_L1:
    {
      /**********************************************************
      *                                                         *
      *                MAC_L1_RELEASE_AND_RR_CMM_COMBINED       *
      *                                                         *
      * Handles simultaneous release for mac and channel mode   *
      * modify from RR.                                         *
      ***********************************************************/
      if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MAC_L1_RELEASE_AND_RR_CMM_COMBINED )
      {
        if ( abort_status == L1_DTM_CS_PS )
        {
          /* Now fill in the new dedicated data */
          l1_dedicated_data_ptr->channel_specification.channel_mode_1 =
          l1_dedicated_data_ptr->new_channel_specification.channel_mode_1;

      #ifdef FEATURE_GSM_AMR
          /* Now fill in the multirate configuration data */
          memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
                   sizeof(l1_dedicated_data_ptr->multirate_configuration),
                   &l1_dedicated_data_ptr->new_multirate_configuration,
                   sizeof(multirate_config_T) );
      #endif


          /* Save last tx power  & TA on current channel in case we need to
             reconnect
          */
          l1_dedicated_data_ptr->last_cell_tx_power =
          l1_dedicated_data_ptr->ms_power_level_in_use;

          l1_dedicated_data_ptr->last_cell_timing_advance =
          l1_dedicated_data_ptr->timing_advance_in_use;

          l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_CHANNEL_MODE_MODIFY_CNF;

          /* Restart L1 in dedicated mode */
          /* ISR context - no need for INTLOCK */
          l1_change_state( L1_DEDICATED_MODE, gas_id );
          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;
          l1_tskisr_blk->sub_state = L1_DED_INIT;

          /* change the mode of the SCE to the dedicated mode */
          l1_sc_start(L1SCModeDediTCh, gas_id);

          /* Run this so that the state transition takes effect immediately.
           * Otherwise we'd waste a frame till the ISR runs again
           */
          (void)L1_control_dedicated(gas_id);

          L1isr_send_MPH_DTM_CHANNEL_RELEASE_IND (NO_ERROR, TRUE, FALSE, gas_id);
          abort_processed = TRUE;
          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
        }
        else
        {
          MSG_GERAN_HIGH_1_G("Not ready to complete MAC_L1_RELEASE_AND_RR_CMM_COMBINED Status:%d",abort_status);
        }
      }
    }
    break;

  case MS_RR_L1:
    {
        /****************************************************
        *                                                   *
        *                MPH_STOP_GSM_MODE_REQ              *
        *                                                   *
        *****************************************************/
      if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MPH_STOP_GSM_MODE_REQ )
      {
        if ( abort_status == L1_DTM_CS_PS )
        {
          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
          l1_tskisr_blk->current_params.L1Data.pDtm_data->l1_dtm_init_mode = L1_DTM_NONE;

          if (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
          {
            MSG_GERAN_HIGH_1_G("Slam CS to rel 2 -> tn 0 [%d]",gl1_get_FN(gas_id));
            /* change timeslot so that the CS slot lies on relative TN 2 */
            L1_dtm_change_timeslot( 0,NULL,gas_id );

            /* Inform Message layer to Rx and Tx all CS chanels on TN 0 */
            gl1_msg_set_cs_timeslot(GL1_DEFS_TN_0,gas_id);

            /* Now change the mode back to the lowest TN Slam */
            l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_LOWEST;

          }

          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_tskisr_blk->command = L1_NULL_COMMAND;
          l1_change_state( L1_DEDICATED_MODE, gas_id );
          l1_tskisr_blk->sub_state = L1_DED_SUSPEND;

          L1_setup_DEACTIVATE( &gl1_dtm_if_data_ptr->mph_stop_gsm_mode_req.message_header );
#ifdef FEATURE_GSM_AUDIO_GAP_OPTIMIZATION
          l1_vs_stop_client(gas_id);
#endif
          abort_processed = TRUE;

        }

      }
      else
      /********************************************************
      *                                                         *
      *                MPH_DTM_CHANNEL_RELEASE_REQ              *
      *                                                         *
      ***********************************************************/
      if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MPH_DTM_CHANNEL_RELEASE_REQ )
      {
        idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
        /* The CS and PS abort must have be completed to fully process the abort */
        if ( abort_status == L1_DTM_CS_PS )
        {
          if (l1_idle_data->campedon_cell_ARFCN.num != INVALID_ARFCN )
          {
            /* Change the ISR mode */
            l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;
            l1_tskisr_blk->next_params.BSIC = l1_idle_data->campedon_cell_BSIC;
            l1_tskisr_blk->next_params.ARFCN = l1_idle_data->campedon_cell_ARFCN;
            l1_tskisr_blk->next_params.extended_BCCH_mask = SERV_EXT_BCCH_MASK_NONE;

            /* Transition to L1_FIND_BCCH state. We do not go back into the
             * start of find BCCH , but the part that reads the BCCH
             * ISR context - no need for INTLOCK
             */
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
            l1_tskisr_blk->command = L1_NULL_COMMAND;
            L1_transfer_new_data( gas_id );
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if ( gl1_msg_get_multi_sim_mode() )
            {
              l1_tskisr_blk->sub_state = L1_MULTI_SIM_RESELECTION;
              l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
              l1_tskisr_blk->multi_sim_reselection_state = L1_MULTI_SIM_RESELECTION_STOPPED;
              l1_change_state( L1_MULTI_SIM_FIND_BCCH, gas_id );
			  /* Release RF, FW and TRM when going to BCCH Mode */
              call_in_task_grm_release(l1_tskisr_blk->client_id);
            }
            else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            {
              l1_tskisr_blk->sub_state = L1_GETTING_BCCH;
              l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
              l1_change_state( L1_FIND_BCCH, gas_id );
            }
            L1isr_send_MPH_DTM_CHANNEL_RELEASE_IND (NO_ERROR, TRUE, TRUE, gas_id);
            
            gl1_hw_terminate_tch(gas_id);
            l1_vs_stop_client(gas_id);

            gl1_msg_disable_ciphering(gas_id);
          }
          else
          {
            /* Transition to appropriate state
             * ISR context - no need for INTLOCK
             */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            if ( gl1_msg_get_multi_sim_mode() )
            {
              l1_change_state( L1_MULTI_SIM_NULL, gas_id );
              /* Release RF, FW and TRM when going to NULL Mode */
              call_in_task_grm_release(l1_tskisr_blk->client_id);
            }
            else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            {
              l1_change_state( L1_NULL_MODE, gas_id );
            }
            l1_tskisr_blk->main_command = L1_NULL_COMMAND;
            L1_terminate_dedicated();
            L1isr_send_MPH_DTM_CHANNEL_RELEASE_IND (NO_ERROR, TRUE, TRUE, gas_id);

            gl1_hw_terminate_tch(gas_id);
            l1_vs_stop_client(gas_id);

            gl1_msg_disable_ciphering(gas_id);
          }

          if (l1_dedicated_data_ptr->l1_dtm_slam_mode == L1_CS_TN_SLAM_TN2)
          {

            MSG_GERAN_HIGH_1_G("Slam CS to rel 2 -> tn 0 [%d]",gl1_get_FN(gas_id));
            /* change timeslot so that the CS slot lies on relative TN 2 */
            L1_dtm_change_timeslot( 0,NULL, gas_id );

            /* Inform Message layer to Rx and Tx all CS chanels on TN 0 */
            gl1_msg_set_cs_timeslot(GL1_DEFS_TN_0, gas_id );

            /* Now change the mode back to the lowest TN Slam */
            l1_dedicated_data_ptr->l1_dtm_slam_mode = L1_CS_TN_SLAM_LOWEST;

          }

          l1_tskisr_blk->current_params.L1Data.pDtm_data->l1_dtm_init_mode = L1_DTM_NONE;

          abort_processed = TRUE;

          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
        }
#ifdef FEATURE_GSM_EDTM
        else
        if ( (get_dtm_isr_state(gas_id) == L1_DTM_CS_ABORT) &&
             (abort_status == L1_DTM_CS_ONLY)
           )
        {

          MSG_GERAN_HIGH_0_G("Preparing for Enhanced Release");
          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          L1_terminate_dedicated();
          L1isr_send_MPH_DTM_CHANNEL_RELEASE_IND (NO_ERROR, FALSE, TRUE, gas_id);

          gl1_hw_terminate_tch(gas_id);
          l1_vs_stop_client(gas_id);

          gl1_msg_disable_ciphering(gas_id);

          abort_processed = TRUE;

          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;

          l1_sc_start(L1SCModeGprsTrans, gas_id);
        }
#endif
        else
        {
          MSG_GERAN_HIGH_0_G("Not ready to complete MPH_DTM_CHANNEL_RELEASE_REQ");
        }
      }
      else
      /****************************************************
      *                                                   *
      *                MPH_CHANNEL_ASSIGN_REQ             *
      *                                                   *
      *****************************************************/
      if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MPH_CHANNEL_ASSIGN_REQ )
      {
        if ( abort_status == L1_DTM_CS_PS )
        {
          /* @@Warning: the state change must occur before
           * complete channel assignment is called.
           */
          l1_change_state( L1_DEDICATED_MODE, gas_id );

          gl1_complete_channel_assignment(gas_id);

          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_tskisr_blk->command = L1_NULL_COMMAND;

          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
          l1_tskisr_blk->current_params.L1Data.pDtm_data->l1_dtm_init_mode = L1_DTM_NONE;
          
          abort_processed = TRUE;

        }

      }
      else
      /****************************************************
      *                                                   *
      *                MPH_HANDOVER_REQ                   *
      *                                                   *
      *****************************************************/
      if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MPH_HANDOVER_REQ )
      {

        if ( abort_status == L1_DTM_CS_PS )
        {
          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
          l1_tskisr_blk->current_params.L1Data.pDtm_data->l1_dtm_init_mode = L1_DTM_NONE;

          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_tskisr_blk->command = L1_NULL_COMMAND;
          l1_change_state( L1_DEDICATED_MODE, gas_id );
          l1_tskisr_blk->sub_state = L1_DED_SUSPEND;

          /* @@OG: dont care about DTM for now
           * Do not apply afc of target cell.
           */
          gl1_setup_new_channel(gas_id);

          abort_processed = TRUE;

        }

      }
      else
      /****************************************************
      *                                                   *
      *                MPH_RECONNECT_CHANNEL_REQ          *
      *                                                   *
      *****************************************************/
      if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MPH_RECONNECT_CHANNEL_REQ )
      {

        if ( abort_status == L1_DTM_CS_PS )
        {
          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
          l1_tskisr_blk->current_params.L1Data.pDtm_data->l1_dtm_init_mode = L1_DTM_NONE;

          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_tskisr_blk->command = L1_NULL_COMMAND;
          l1_change_state( L1_DEDICATED_MODE, gas_id );
          l1_tskisr_blk->sub_state = L1_DED_SUSPEND;

          gl1_msg_reset_ho_tt_adjust(gas_id);
          gl1_complete_channel_reconnect(gas_id);

          abort_processed = TRUE;

        }

      }
      else
      /**********************************************************
      *                                                         *
      *                MPH_CHANNEL_MODE_MODIFY_REQ              *
      *                                                         *
      ***********************************************************/
      if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MPH_CHANNEL_MODE_MODIFY_REQ )
      {
        /* The CS and PS abort must have be completed to fully process the abort */
        if ( abort_status == L1_DTM_CS_PS || abort_status == L1_DTM_CS_ONLY )
        {
          /* Now fill in the new dedicated data */
          l1_dedicated_data_ptr->channel_specification.channel_mode_1 =
          l1_dedicated_data_ptr->new_channel_specification.channel_mode_1;

  #ifdef FEATURE_GSM_AMR
          /* Now fill in the multirate configuration data */
          memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
                   sizeof(l1_dedicated_data_ptr->multirate_configuration),
                   &l1_dedicated_data_ptr->new_multirate_configuration,
                   sizeof(multirate_config_T) );
  #endif

          /* Save last tx power  & TA on current channel in case we need to
             reconnect */
          l1_dedicated_data_ptr->last_cell_tx_power =
          l1_dedicated_data_ptr->ms_power_level_in_use;

          l1_dedicated_data_ptr->last_cell_timing_advance =
          l1_dedicated_data_ptr->timing_advance_in_use;

          l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_CHANNEL_MODE_MODIFY_CNF;

          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;
          l1_tskisr_blk->sub_state    = L1_DTM_INIT;
          l1_tskisr_blk->sub_sub_state    = L1_DED_INIT;

          l1_tskisr_blk->current_params.
            L1Data.pDtm_data->l1_dtm_init_mode = L1_DTM_CS_ONLY;

          abort_processed = TRUE;

          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;

        }
        else
        {
          MSG_GERAN_HIGH_0_G("Not ready to complete MPH_CHANNEL_MODE_MODIFY_REQ");
        }
      }
      else
      {
        MSG_GERAN_ERROR_0_G("ERROR:Unknown RR DTM abort reason !!");
      }
    }
    break;

  case MS_MAC_L1:
    {
      if ( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MAC_L1_RELEASE )
      {
        /************************************************************/
        /* The PS abort must be complete to fully process the abort */
        /* and return to the DEDICATED mode                         */
        /************************************************************/
        if ( abort_status == L1_DTM_PS_ONLY )
        {
          l1_change_state( L1_DEDICATED_MODE, gas_id );
          l1_tskisr_blk->sub_state = L1_DED_AFTER;
          l1_tskisr_blk->sub_sub_state = L1_DED_RUNNING;
          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          L1isr_send_MPH_DTM_CHANNEL_RELEASE_IND (NO_ERROR, TRUE, FALSE, gas_id);
          abort_processed = TRUE;

          /* change the mode of the SCE to the dedicated mode */
          l1_sc_start(L1SCModeDediTCh, gas_id);

          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
        }
        else
        {
          MSG_GERAN_HIGH_0_G("Not ready to complete MAC_L1_RELEASE");
        }
      }
      else
      /**********************************************************
      *                                                         *
      *                MAC_L1_DTM_CHANNEL_ASSIGN_REQ            *
      *                                                         *
      ***********************************************************/
      if( gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.message_id == (uint16)MAC_L1_DTM_CHANNEL_ASSIGN_REQ )
      {

        if ( abort_status == L1_DTM_CS_PS )
        {
          /********************************************************************/
          /* Complete the DTM_CHANNEL_ASSIGNMENT_REQ, copying over any new CS */
          /* allocation into the dedicated data structure.                    */
          /********************************************************************/
          byte temp;

          /* Now fill in the new dedicated data */
          memscpy ( &l1_dedicated_data_ptr->channel_specification,
                   sizeof(l1_dedicated_data_ptr->channel_specification),
                   &l1_dedicated_data_ptr->new_channel_specification,
                   sizeof(channel_spec_T) );

  #ifdef FEATURE_GSM_AMR
          /* Now fill in the multirate configuration data */
          memscpy ( &l1_dedicated_data_ptr->multirate_configuration,
                   sizeof(l1_dedicated_data_ptr->multirate_configuration),
                   &l1_dedicated_data_ptr->new_multirate_configuration,
                   sizeof(multirate_config_T) );
  #endif

          l1_dedicated_data_ptr->starting_time = l1_dedicated_data_ptr->new_starting_time;

          /* Load up transmit power */
          temp = l1_dedicated_data_ptr->channel_specification.power_level;
          l1_dedicated_data_ptr->ms_power_level_in_use = temp;
          l1_dedicated_data_ptr->ms_power_level_to_use = temp;
          l1_dedicated_data_ptr->ms_power_level_to_report = temp;
          l1_dedicated_data_ptr->ordered_ms_power_level = temp;

          l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_DTM_CHANNEL_ASSIGN_CNF;

          //Can we do without this ??
          //l1_tskisr_blk->command = L1_SET_HANDVR_CIPH_COMMAND;

          l1_tskisr_blk->main_command = L1_NULL_COMMAND;
          l1_tskisr_blk->command = L1_NULL_COMMAND;

          l1_dedicated_data_ptr->L1_confirm_msg = (byte)MPH_DTM_CHANNEL_ASSIGN_CNF;
          l1_tskisr_blk->next_params.L1Data.pDtm_data->l1_dtm_init_mode = L1_DTM_CS_PS;
          l1_tskisr_blk->sub_state = L1_DTM_INIT;
          L1_transfer_new_data( gas_id);
          abort_processed = TRUE;

          /* Clear out the message store */
          gl1_dtm_if_data_ptr->current_rr_mac_l1_msg_id.valid = FALSE;
          }
        else
        {
          MSG_GERAN_HIGH_0_G("Not ready to complete MAC_L1_DTM_CHANNEL_ASSIGN_REQ");
        }

      }
      else
      {
        MSG_GERAN_ERROR_0_G("ERROR:Unknown DTM abort reason !!");
      }
    }
    break;

  default: MSG_GERAN_ERROR_0_G("Unknown message in gpl1_dtm_complete_abort_procedure()");
  }

  if (abort_processed)
  {
    /*Aborting DTM procedure and moving out to Dedicated/FindBCCH - 
          we can send rel confirm and dont have to wait for PCA_FN*/
    if ((l1_tskisr_blk->l1_state != L1_TRANSFER_MODE) &&
        (transfer_data_ptr->wait_for_pca_valid == TRUE) &&
        (transfer_data_ptr->release_pending != L1_NO_RELEASE))
    {
       MSG_GERAN_HIGH_2_G("Sending rel cnf before wait_for_pca_fn:%d, GFN:%d",GSTMR_GET_FN_GERAN(gas_id), transfer_data_ptr->wait_for_pca_fn);
       
       /* Indicate to MAC release procedure is complete */
       gpl1_gprs_send_l1_mac_tbf_rel_confirm(transfer_data_ptr->release_pending, gas_id);
       
       /* reset relelase type and rel confirm status*/
       transfer_data_ptr->release_pending = L1_NO_RELEASE;
       transfer_data_ptr->confirm_pending = SENT;

       transfer_data_ptr->pca_sent      = TRUE;
       transfer_data_ptr->wait_for_pca_valid = FALSE;
       transfer_data_ptr->wait_for_pca_fn = L1_GPRS_INVALID_FN;
    }
  }

  return abort_processed;
}

#ifdef FEATURE_GSM_EDTM

dtm_tbf_type_t get_queued_mac_l1_edtm_cs_rel_tbf_alloc( gas_id_t gas_id )
{
  gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[gas_id];
  return gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.tbf_type;
} // get_queued_mac_l1_edtm_cs_rel_tbf_alloc

void set_queued_mac_l1_edtm_cs_rel_tbf_alloc(dtm_tbf_type_t tbf_type,gas_id_t gas_id)
{
  gl1_dtm_if_data_t *gl1_dtm_if_data_ptr = &gl1_dtm_if_data[gas_id];
  gl1_dtm_if_data_ptr->queued_mac_l1_edtm_cs_rel_tbf_alloc.tbf_type = tbf_type;
}
#endif
#else

/* Dummy function to avoid warning when FEATURE_GSM_DTM is not defined */
void gpl1_dtm_control_mode_dummy(void)
{
}

#endif /* FEATURE_GSM_DTM */
