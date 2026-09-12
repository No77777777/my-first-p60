/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            L 1   A C C E S S   M O D E   T A S K  P R O C E S S I N G

GENERAL DESCRIPTIONG
   This module contains the procedures to handle events while the L1
   task is in the RANDOM ACCESS mode for GPRS handling only.
   Events can originate from other layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_mac_l1_random_access_mode
    Procedure to handle mac l1 events while the L1 task is in the
    RANDOM ACCESS mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_prach_if.c#1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
15/06/18   ng         CR2261488 Remove 'l1_rcvd_non_drx_ind' related handling, it is no more needed
25/05/17   hd         CR2051434 Make the variable l1_rcvd_non_drx_ind dual spaced
01/08/16   og         CR1038345. UE does not get RAU accept after SRVCC.
16/03/16   sn         CR990119 RACH guard timer updation w.r.t to TBF starting time.
08/09/17   sn         CR1061730 FR43082: TA during PS Procedures- GL1 changes
20/07/17   rc         CR2079663 Correcting starting time in error recovery data in RACH mode.
16/03/17   mn         CR2020307 FR40846:Packet transfer Error Recovery mechanism.
18/5/16    sm        CR1017901 Added GL1 idle mode QSH events 
11/05/15   pjr        CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
30/03/15   nm         CR765891 Allowing TA for TDS during PS RACH
16/02/15   nm         CR789023 start SCE in SINGAL_BLOCK CONFIG as well for correct FN lag adjustment for ncells
09/12/14   br         CR767733: GL1 to update the rach guard timer, on  receiving  MAC_L1_DL_SINGLE_BLOCK Assignment
10/03/13   hd         CR628121 ISR lock before handling MAC_L1_SINGLE_BLOCK_CONFIG 
18/10/14   nm         CR738687 populate the transfer_data[gas_id].ul_st & dl_st on getting DL_TBF and DYNAMIC TBF cfg
10/09/14   br         CR706453 : In RACH mode, send pending idle cnf before changing main_command to TRANSFER_COMMAND
21/07/14   am         CR690871 Restart rach gaurd timer on reception on assignment
29/07/14   ap         CR695234: Changes for GL1's New Rach interface
30/05/14   ws         CR657674 Data + MMS Phase 1
08/05/14   br         CR661054 In DSDA mode send  MAC_L1 release cnf to transfer sub  after checking gas id
15/04/14   ka         CR611185: Ensure information from the MPH_START_GPRS_IDLE_MODE_REQ is used in RACH
04/04/13   ap         CR640781: stuck in access mode due to incorrect transfer config bit map mapping
08/11/13  mko         CR565394 if Imm assignment msg contains valid start time, then 
                      reset ul_st time before calling gp1_gprs_get_starting_time in RACH mode
27/09/13   sp         CR551580: Add NV logging of outgoing L1 messages
07/16/13   zc    	  CR470197 Replace memcpy with memscpy
21/06/13   cs         Major Triton TSTS Syncup
08/04/13   jk         CR462854:In idle mode do not delay sending of L1_MAC_TBF_REL_CONFIRM
08/11/12   ab         CR 418358 Increase RACH GUARD Timer based on starttime
15/02/13   ws         CR 453155 - Correct missing gas id in gprs_mac_l1_acc_func()
20/09/12   ws         CR 401931 - removed references to assign_search_params()
17/08/12   ip         R384421 Disable GSM sleep during CCO activity
25/04/12   pa         CR355881: Removed warning.
29/03/12   cja        CR316437 Inhibit sleep on X2G RAT change until it is complete
28/03/12   pa         CR 346062 Workaround for G2T Abort.
03/05/11   cja        Nikel - only send G2L init when L neighbours
14/01/11   ab         CR 258308   : P0 write buffer overflow in func
                      gpl1_gprs_calculate_paging_data()
17/12/10   og         Changed G2L INIT and DEINIT.
14/12/10   og         Extra Featurisation for G2L.
29/11/10   og         Adding G2L support.
07/07/09   ab         CR 183589 Incorrect handling of RLC mode change request
                      during extend UL TBF operation
11/03/09   og         CR173804 - Ignore downlink mac mode for Rel 6.
20/10/08   og         Single block start time corner case. CR161001.
28/07/98   hv         Added KxMutex support
15/05/08   nf         CR117393 fix for NC2 non-drx handling
22/04/08   ap         CR#144832 Ensure L1 drops back to IDLE when the single block start
                      time elapses on 52 multiframe.
17/01/07   og         Remove pending gprs quality reports when an uplink release
                      message is received. Resolves CR133824.
21/11/07   og         Ensure L1 drops back to IDLE when the single block start
                      time elapses. Resolves CR115529.
06/06/07   ws         CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                      is called when we reselect to PBCCH cell stopping NC2 reporting
26/01/07   og         Implement UL ctrl msg deletion in L1.
23/10/06   og         Set the tbf establishment type when issuing a single block
                      main command while in the access isr.
06/04/06   og         Remove the merge of change 302030. Need to allow 1 tdma frame
                      for L1 Task to L1 isr context switch.
17/03/06   og         Take downlink assignment mac mode into account. This is
                      required by release 4 ETSI spec.
16/03/06   og         Merge change 302030 from the gpl1 saber 5x branch. This removes
                      wrong expiry of immediate assignment single block scenarios.
15/09/05   ws         Added FEATURE_GSM_DTM around mac mode defs in MAC->L1 MSGs
29/08/05   sv         Use default WCDMA parameters on reselection.
22/08/05   og         Add the setting of mac mode to NPL1.
17/08/05   og         Removal of dummy ctrl block dual buffer.
02/06/05   og         Removal of header-header includes.
17/05/05   og         Remove the forcing of pkt paging mode to normal.
20/04/05   og         Addition of filtered bep params to the msg metrics log packet.
11/3/05    og         Changes to egprs dl tfi and IR handling.
25/01/05   ws         Change handling of power control for single ul block handler
                      on PBCCH cells. Don't use gamma if alpha_present is FALSE
17/01/05   ws         Lint updates
13/1/05    og         MS timebase adjustements confined to radio block gap.
01/12/04   ws         Update Alpha in SINGLE_UL handler if alpha is present
30/11/04   og         Enable interference measurements when cuta is ON.
24/11/04   og         Merge fixes from the saber 4x branch.
2/11/04    og         Addition of egprs srb test mode capability.
21/10/04   ws         Fixed compile issues with FEATURE_GSM_EGPRS undefined
19/10/04   og         Added single block bep period2 handling.
15/10/04   og         Removed Fixed TBF allocation related source code.
14/10/04   kf         Moved the setting of the bitmap in MAC_L1_SINGLE_BLOCK_CONFIG.
12/10/04   ws         Added Multiblock allocation for EGPRS
05/10/04   dlh        Added handling for PBCCH_DESCRIPTION_IND message.
17/05/04   ws         Added setting of power control for single block allocation
08/09/04   ws         Changed min. reaction time for DYNAMIC alloc's when camped on
                      PBCCH cell for no starting time
30/08/04   ws         Added handling for num_rb_alloc in single block alloc handler
26/05/04   sv         Changed "l1_isrtsk_blk" structure parameters for better
                      code readability.
23/06/04   og         Egprs Qual measurements handling.
28/04/04   rm         Replaced ta and rrbp codes with new function calls to
                      RRBP and PTCCH modules
23/04/04   ws         Corrected Lint Error's and Warnings
24/03/04   ws         Modified ph_data_req hadler to allow only 1 PACCH message in buffer
23/03/04   ws         Added tbf_freq_cnf_t handling to MAC_L1_FREQ_CONFIG handler
18/03/04   ws         Added support for nc2 non drx time
11/03/04   ws         Ensure ta param pointer is null to stop cont. TA params
                      swapping to UL
23/02/04   ws         Added cancelling starting time when starting time elapsed
                      in single block allocation
19/02/04   ws         Removed return to idle when starting time has expired in single
                      block assignment. Prach/rach can continue from RACH state.
14/01/04   ws         Adding missing l1_sc_stop() when releasing back to idle
26/11/03   ws         Lint cleanup
24/11/03   ws         Added starting_time_valid flag to cases where starting time
                      has expired
06/11/03   gfr        Support for quad-band.
14/10/03   npr        Removed pmax setup in MAC_L1_DYNAMIC_TBF_CONFIG as this is
                      done on start idle as a function of gsm/gprs Max CCH Pwr
07/10/03   dlh        Removed the rrbp_flag as no longer needed.
06/10/03   ws         Corrected handling of RRBP counters if DL RRBP message is received
26/09/03   dlh        Modified changes to the acquire psi handling.
12/09/03   pjr        Lint tidy
03/09/03   dlh        Added rrbp_52_idle to PAACH_RRBP signal and moved rrbp counters
29/03/03   dlh        Removed Calc_gap variable and tidied F3.
28/08/03   kf         Modified Reselection Req.
20/08/03   pjr        If no power control values are contained in TBF_CONFIG signals
                      added clearing of gamma_tn and alpha values to force PMAX tx power
12/08/03   ws         Added clearing of pending dl and pending dynamic tbf for new ass's
28/07/03   ws         Modified Calculation for elapsed starting time for DL single block
24/07/03   pjr        Modified single block config to only ever send one quality
                      report, so over write quality_report_time.
22/07/03   ws         Added release_pending to Abnormal ul_release
21/07/03   dlh        Tidy up page mode F3.
17/07/03   ws         Added check in DL_TBF_CONFIG and DYNAMIC_TBF_CONFIG to ensure
                      we are in RACH_MODE before configuring ul tbf
14/07/03   gw         Updated comments.
10/0703    bk         Changed the prototype to gpl1_gprs_setup_start_idle. Protected
                      certain sections of the code with INTLOCKs
10/07/03   pjr        Modified starting time check in MAC_PH_DATA_REQ
10/07/03   ws         Modified structure of DL_TBF_CONFIG and DYNAMIC_TBF_CONFIG
                      to ensure min starting time is actioned correctly
08/07/03   gw         GRR/L1 SCE cleanup.
04/07/03   ws         Modified handling of SAME_AS_BEFORE page mode if no page mode
                      has been ordered via MAC
25/06/03   pjr        Added starting time check to MAC_PH_DATA_REQ when part of
                      SINGLE block config.
25/06/03   gw         Use gpl1_gprs_setup_start_idle() to go to Idle Mode.
23/06/03   gfr        Compiler warning cleanup.
19/06/03   pjr        Modified QUALITY report reuest to save rrbp frame number
19/06/03   gw         Changes for starting/stopping SCE going in/out of
                      52 Idle Mode.
19/06/03   gw         Changes to starting and stopping of SCE.
18/06/03   pjr        Added call to function gpl1_gprs_calc_rf_pwr() to update
                      uplink power control whenever the power control parameters
                      are updated.
17/06/03   ws         Changed handling of pending starting time for release
10/06/03   pjr        Reduced the single block config start time block to 3 frames.
                      Added INTLOCK to MAC_L1_RELEASE handler.
                      Modified MPH_GPRS_L1PARAM_UPDATE_REQ handler.
20/05/03   ws         Modifications to use correct starting time calculation
15/05/03   ws         Implemented toggling for ul dummy data block
15/05/03   pjr        Changed quality report handling to increment the number of
                      quality reports to send.
14/05/03   pjr        Modified gprs_serv_cell_meas.quality_report_time_index check from
                      5 to (MAX_NUM_QUAL_RPRTS - 1)
02/05/03   ws         Added STOP and START PSI REQ
30/04/03   pjr        Modified MAC_PH_DATA_REQ to allow for case when pending PRR info
                      changed before starting time expires.
04/03/03   pjr        Removed setting of l1_tskisr_blk->main_command to L1_IDLE_COMMAND
                      from MPH_GPRS_L1PARAM_UPDATE_REQ signal handler.
24/04/03   ws         preset pending_alloc_type to L1_NULL_ALLOC
17/04/03   DLH        Added fix for page mode SAME_AS_BEFORE case
02/04/03   gw         Removed references to obsolete BA list struct.
04/03/03   pjr        Added L1_ABORT_PRACH functionality
28/02/03   pjr        Modified MPH_START_GPRS_IDLE_MODE_REQ handler and
                      MAC_L1_RELEASE handler.
26/02/03   npr        Changed instances of INVALID to GPL1_INVALID
14/02/03   pjr        Changes to 52 mode starting times. Change to MAC_L1_RELEASE.
13/02/03   ws         Updates for DL power control
05/02/02   ws         Modified min reaction time with TEMP_DELAY_OS for scheduling
                      change
12/19/02   pjr        In MAC_L1_DL_TBF_CONFIG and MAC_L1_DYNAMIC_TBF_CONFIG Added
                      storing of power control parameters in serving cell structure.
12/13/02   ws         Added  MPH_START_GPRS_IDLE_REQ to return back to 51 MFRM idle mode
12/02/02   ws         Added handling of MPH_START_MM_NON_DRX_IND and MPH_STOP_MM_NON_DRX_IND
11/29/02   pjr        Added check for current FN being to close to start time in
                      MAC_L1_SINGLE_BLOCK_CONFIG.
11/25/02   pjr        Added feature switch, FEATURE_GSM_GPRS_PCCCH, around PRACH
                      functionality.
11/21/02   ws         Added reseting of RLC read buffer when moving into transfer mode
11/20/02   DLH        Added MPH_START_MM_NON_DRX_IND and MPH_STOP_MM_NON_DRX_IND
11/12/02   DLH        Added code to copy nc_non_drx_period and nco values to sb_tbf struct
11/11/02   ws         Set dl tbf pointer to null when only ul is established
08/11/02   pjr        Made quality reporting time an array to handle pending
                      ack/nacks.
04/11/02   pjr        Merge Neighbour cell measurements under feature switch
01/11/02   pjr        Added serving cell measurements under feature switch
10/21/02    he        Replaced FN by gl1_get_FN()
16/10/02    pjr       Changed start time for no supplied start time to FN+4
11/10/02    DLH       Removed MAC_L1_PAGE_MODE handler
10/10/02    DLH       Added extra param to mac access function.
10/09/02    ws        Added sb_ul_dl_type for sigle ul block alloc
09/18/02    ws        Moved setting of main_command to end of MAC_L1_DL_TBF_CONFIG and
                      MAC_L1_FREQ_CONFIG, MAC_L1_DYNAMIC_TBF_CONFIG, MAC_L1_SINGLE_BLOCK
18/09/02    DLH       Added mph_gprs_l1param_update_req_t handler.
18/09/02    ws        Added adjustment of rrbp fn if it were received from immed.
                      assignment
17/09/02    ws        Added abort functionality to allow release to be supported
12/09/02    pjr       Removed function gpl1_gprs_pending_prach_req()
09/05/02    pjr       Modified single block config and added clearing of
                      l1_transfer_data.l1_called_from_sngl_blk flag in oth TBF
                      configs
09/03/02    pjr       Added two-phase access requirements to SINGLE BLOCK config.
08/29/02    DLH       Added MAC_L1_PAGE_MODE handler
19/08/02    pjr       Corrected more Lint error's and Warning's
8/08/02     ws        Corrected Lint error's and Warning's
05/08/02    ws        Changed FEATURE_GPRS to FEATURE_GSM_GPRS_L1
07/25/02    pjr       Added halting of PRACH TX when MAC_L1_FREQ_CONFIG signal
                      processed.
07/24/02    pjr       Added hanndling of MAC_L1_RELEASE.
                      Added setting of prach sub_state.
07/19/02    pjr       Removed gl1_msg_store_ta from gpl1_gprs_pending_prach_req()
18/07/02    ws        Fixed ARM compile error with global_ta_param structure
15/07/02    ws        Added gpl1_gprs_chk_blk_bndry() to adjust RRBP fn it does not lie
                      on the firsat frame of a radio block.
15/07/02    ws        Changed handling of timing advance for MAC_L1_DYNAMIC_TBF_CONFIG
                      and MAC_L1_DOWNLINK_TBF_CONFIG
09/07/02    pjr       Added function gpl1_gprs_pending_prach_req()
02/07/02    ws        Added support for tbf_est_type to allow MAC_PH_CONNECT_IND to
                      be configured correctly
27/06/02    ws        added support of gpl1_gprs_get_starting_time()
11/06/02    pjr       Added function gpl1_gprs_chk_blk_bndry() to check starting
                      time falls on a block boundary. Changed CONF signal handling
                      to use this function.
28/05/02    ws        Added mac mode setting in MAC_L1_DOWNLINK_TBF_CONFIG
27/05/02    ws        Added Global TA pointer for concurrent TBF work
24/05/02    pjr       Added debug message to MAC_PH_RANDOM_ACCESS message
                      handling, and general tidy.
09/05/02    pjr       Added handling of single block tbf allocation
21/12/01    pjr       Added MAC_PH_RANDOM_ACCESS message handling
01/11/01    ws        created
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "customer.h"


#include "gpl1_gprs_task.h"
#include "gpl1_gprs_if.h"
#include "l1_isr.h"
#include "gpl1_gprs_isr.h"
#include "comdef.h"
#include "gmacl1.h"
#include "gl1_msg_g.h"
#include "l1_task.h"
#include "ms.h"
#include "sys_stru.h"
#include "memory.h" // for memcpy()
#include "rr_public_store.h"
#include "msg.h"
#include "gpl1_gprs_rrbp.h"
#include "gpl1_gprs_ptcch.h"
#include "l1_drx.h"

#include <stringl/stringl.h>

#ifdef  FEATURE_GSM_TO_LTE
#include "l1i.h"
#endif

#ifdef PL1_FW_SIM

#include "pl1_test_harness.h"

#endif


#include "l1_sc.h"
#include "l1_sc_int.h"


#include "l1_utils.h" /* for gl1_get_FN() */
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#include "gpl1_gprs_utils.h"
#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#include "gpl1_gprs_error_recovery.h"
#endif/* FEATURE_GERAN_PTM_ERR_RECOVERY */


extern void gpl1_gprs_setup_RESELECTION_GPRS( IMH_T *msg_header );


#ifdef FEATURE_GSM_TDS
extern boolean l1_sc_irat_performing_acq_meas(gas_id_t gas_id);
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
/*  Prototype of local function */
void sngl_blck_move_to_boundary(uint32 *st_ptr);

extern void  gsm_l1_set_ecio_rssi_levels( uint16   qsearch,
                                          uint16   ecode,
                                          gas_id_t gas_id
                                        );

extern uint32 l1_max_rach_guard_time[NUM_GERAN_DATA_SPACES];

/*===========================================================================

FUNCTION  L1_handle_mac_l1_random_access_mode

DESCRIPTION
  This function handles the L1 Task in random access mode for
  signals received from MAC for GPRS operation only.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/


void gpl1_gprs_handle_random_access_mode(IMH_T *msg_header)
{
gas_id_t gas_id;
inter_task_BCCH_allocation_T  ba_buffer;

  if (msg_header->message_set == MS_MAC_L1)
  {

    switch ( msg_header->message_id )
    {
    /*******************************************************
    *                                                      *
    *                MAC_L1_FREQ_CONFIG                    *
    *                                                      *
    ********************************************************/

      case  MAC_L1_FREQ_CONFIG:
      {
        mac_l1_freq_config_t  *message_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        boolean error;
        tbf_freq_cnf_t tbf_type;
        boolean move_to_transfer=FALSE;

        /*lint -e740 */
        message_ptr = ( mac_l1_freq_config_t *) msg_header;

        /*lint +e740 */
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        MSG_GERAN_MED_1_G("PL1: MAC_L1_FREQ_CONFIG Rcvd - bmap %d", l1_transfer_data[gas_id].config_bitmap);
        /*  check for un supported frequencies in ma list that are not in the
            supported bands*/
        error = gpl1_gprs_check_frequency_params(
                                                  &(message_ptr->frequency_information.frequency_list),
                                                  message_ptr->frequency_information.hopping_flag,
                                                  gas_id
                                                );

        /* read out tbf type from msg */
        tbf_type = (tbf_freq_cnf_t)message_ptr->tbf_freq_cnf;

        /* send signal to mac to confirm or fail frequency params */
        L1_send_l1_mac_freq_ass_conf(error,tbf_type, gas_id);

        /* if no error then continue */
        if(error != TRUE)
        {
         l1_transfer_data[gas_id].frequency_info_1 = message_ptr->frequency_information;

          /*  If part of a single block allocation and an allocation is already
              in progress but another freq conf is received before the starting
              time has elapsed, then clear single block config and ph data req
              flags as they will be received again. */
          if (FREQ_CONFIG_RECV(gas_id) && SINGLE_BLOCK_RECV(gas_id) && UPLINK_DATA_VALID(gas_id))
          {
            /*  Clear single block config and uplink data flags  */
            CLEAR_SINGLE_BLOCK_RECV(gas_id);
            CLEAR_UPLINK_DATA_VALID(gas_id);

          }

          /* Mark MAC_L1_FREQ_CONFIG as received */
          SET_FREQ_CONFIG_RECV(gas_id);

          if(SINGLE_BLOCK_ALLOC_COMP(gas_id))
          {
            /* If we are allocated 2 blocks then check if we have
             * received an UL dummy block
             */
            if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 2)
            {

              if(MULTI_BLOCK_ALLOC_COMP(gas_id))
              {
                /*  All signals have been received to fully describe a SINGLE BLOCK
                 * UL allocation then so move to L1_TRANSFER state and service it
                 */
                move_to_transfer= TRUE;
              }

            }else if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 1)
            {
              /*  All signals have been received to fully describe a SINGLE BLOCK
               *  UL allocation, so move to L1_TRANSFER state and service it
               */
              move_to_transfer = TRUE;
            }

            if(move_to_transfer)
            {
              /* If all signals have been received to fully
               * describe a Single block allocation then
               * move to L1_TRANSFER state and service it
               */
              GL1_ISR_SAVE_LOCK(gas_id);
              l1_transfer_data[gas_id].current_alloc_type = L1_SINGLE_BLOCK_ALLOC;
              l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
              l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
              l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
              RESET_BITMAP(gas_id);
              l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
              l1_tskisr_blk->command = L1_NULL_COMMAND;
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
              gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

              GL1_ISR_SAVE_UNLOCK(gas_id);
            }
          }
          else if (DYNAMIC_TBF_ALLOC_COMP(gas_id))
          {
            /* Prepare the SCE to start in Transfer mode.
             * It won't start until l1_sci_go
             * is called from the ISR.
             */
            l1_sc_start( L1SCModeGprsTrans, gas_id );

            /* If all signals have been received to fully
             * describe a Dynamic UL allocation then
             * move to L1_TRANSFER state and service it
             */
            GL1_ISR_SAVE_LOCK(gas_id);
            l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_ALLOC;
            l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
            RESET_BITMAP(gas_id);

            l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
            l1_tskisr_blk->command = L1_NULL_COMMAND;
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
            gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

            GL1_ISR_SAVE_UNLOCK(gas_id);
          }
          else if (DOWNLINK_TBF_ALLOC_COMP(gas_id))
          {
            /* Prepare the SCE to start in Transfer mode.
             * It won't start until l1_sci_go
             * is called from the ISR.
             */
            l1_sc_start( L1SCModeGprsTrans, gas_id );

            /* If all signals have been received to fully
             * describe a Downlink allocation then
             * move to L1_TRANSFER state and service it
             */
            GL1_ISR_SAVE_LOCK(gas_id);

            l1_transfer_data[gas_id].current_alloc_type = L1_DOWNLINK_ALLOC;
            l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
            RESET_BITMAP(gas_id);

            l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];// BK HACK
            l1_tskisr_blk->command = L1_NULL_COMMAND;
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
            gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

            GL1_ISR_SAVE_UNLOCK(gas_id);
          }

        } /* End of if error */

      break;
      } /* End of case */

    /*******************************************************
    *                                                      *
    *                MAC_L1_FIXED_TBF_CONFIG               *
    *                                                      *
    ********************************************************/


      case MAC_L1_FIXED_TBF_CONFIG:
      {
        mac_l1_fixed_tbf_config_t  *message_ptr;
        gas_id_t gas_id;
        message_ptr = ( mac_l1_fixed_tbf_config_t *) msg_header;
        gas_id = check_gas_id(message_ptr->gas_id);
        /* Not supported, flag an error */
        MSG_GERAN_ERROR_1_G("Fixed allocation not supported (gas_id=%d)", gas_id);
        break;

      }  /* End of Case */

    /*******************************************************
    *                                                      *
    *                MAC_L1_DYNAMIC_TBF_CONFIG             *
    *                                                      *
    ********************************************************/

      case MAC_L1_DYNAMIC_TBF_CONFIG:
      {
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
        uint8  ul_ts_alloc = 0;
        uint8 i;
        mac_l1_dynamic_tbf_config_t  *message_ptr;
        boolean                      st_elapsed;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_dynamic_tbf_config_t *) msg_header;
        /*lint +e740  */
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

        MSG_GERAN_MED_0_G("PL1: MAC_L1_DYNAMIC_TBF_CONFIG Rcvd");



        /* Setup mode to determine whether this TBF is EGPRS or GPRS */
       l1_transfer_data[gas_id].egprs_or_gprs_tbf = message_ptr->dynamic_tbf.tbf_mode;

        /* Setup test mode for EGPRS
         * either TEST_MODE_OFF, TEST_MODE_B or TEST_MODE_SRB
         */
       l1_transfer_data[gas_id].dynamic_tbf_1.test_mode = message_ptr->dynamic_tbf.test_mode;

        if( FALSE != message_ptr->dynamic_tbf.bep_period2.present )
        {
           l1_transfer_data[gas_id].dynamic_tbf_1.bep_period2 =
                message_ptr->dynamic_tbf.bep_period2.bep_period2;
        }
        else
        {
           l1_transfer_data[gas_id].dynamic_tbf_1.bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
        }

#ifdef FEATURE_GSM_DTM
        /* Copy in the mac mode */
       l1_transfer_data[gas_id].dynamic_tbf_1.mac_mode = message_ptr->dynamic_tbf.mac_mode;
#else
       l1_transfer_data[gas_id].dynamic_tbf_1.mac_mode = MAC_L1_MAC_MODE_DYNAMIC;
#endif

       l1_transfer_data[gas_id].dynamic_tbf_2.mac_mode = L1_MAC_MODE_INVALID;

       /* Make sure these are reset */
       l1_transfer_data[gas_id].dynamic_tbf_1.ul_tfi_st = L1_GPRS_INVALID_FN;
       l1_transfer_data[gas_id].dynamic_tbf_2.ul_tfi_st = L1_GPRS_INVALID_FN;

           /* Prepare the SCE to start in Transfer mode.
            * It won't start until l1_sci_go
            * is called from the ISR context.
            * Ensure we haven't already started SC.
            */
        if (l1_tskisr_blk->main_command != L1_TRANSFER_COMMAND)
        {
          l1_sc_start( L1SCModeGprsTrans, gas_id );
        }

       l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;

        if (message_ptr->dynamic_tbf.power_control_params.present)
        {
          uint8 index;
          power_control_gamma_T *pwr_cntrl_ptr;

          /*  Update Alpha value  */
          gprs_serv_cell_meas_ptr->alpha = message_ptr->dynamic_tbf.power_control_params.alpha;

          /*  Extract gamma_tn values  */
          pwr_cntrl_ptr = &(message_ptr->dynamic_tbf.power_control_params.gamma_TN0);
          index = 0;

          do
          {
            if (pwr_cntrl_ptr->present)
            {
              gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
            }
            else
            {
              gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;

            }

            /*  Increment pointer  */
            pwr_cntrl_ptr++;
            index++;

          }while (index < 8);

        }
        else
        {
          uint8 index;
          /*  If no power control parameters, use PMAX power (GSM5.05 10.2.1) */
          /*  Easiest way is to set gamma_tn to 0, and alpha to 0 */
          for(index=0;index<8;index++)
          {
            gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
          }
          gprs_serv_cell_meas_ptr->alpha = 0;

        }

        /*  Update uplink power based on the modified power control parameters. */
        gpl1_gprs_calc_rf_pwr( gas_id );

        /* setup downlink power control parameter's */
        if (message_ptr->dynamic_tbf.dl_ctrl_param.params_valid)
        {
         l1_transfer_data[gas_id].dl_power_control.params_valid = TRUE;
         l1_transfer_data[gas_id].dl_power_control.po = message_ptr->dynamic_tbf.dl_ctrl_param.po;
        }
        else
        {
         l1_transfer_data[gas_id].dl_power_control.params_valid = FALSE;
        }

       l1_transfer_data[gas_id].dynamic_tbf_1.usf_granularity = message_ptr->dynamic_tbf.usf_granularity;

        gpl1_gprs_add_to_active_ul_ta_params(message_ptr->dynamic_tbf.timing_advance_params,FALSE,0, gas_id);

        /* Map MAC description of USF values and timeslots into L1_transfer_data */

        /*
          * Mapping element 0 = usf value for TN0
          *         element 7 = usf value for TN7
          *
          * USF value range 0 to 7
          * USF value = 0xff TN not supported
          */

       l1_transfer_data[gas_id].dynamic_tbf_1.nts = 0;

        /* ensure the Downlink TBF pointer is null */
       l1_transfer_data[gas_id].dl_tbf_ptr=0;

        for(i=0;i< MAX_TN;i++)
        {
          if (message_ptr->dynamic_tbf.usf[i].present)
          {
              /* For debug purposes */
              ul_ts_alloc |= ( 0x80 >> i );


           l1_transfer_data[gas_id].dynamic_tbf_1.usf_value[i] = message_ptr->dynamic_tbf.usf[i].usf_value;
           l1_transfer_data[gas_id].dynamic_tbf_1.nts++;
          }
          else
            /* All this does is set's element to invalid i.e. TN not allocated i.e. 0xff*/
           l1_transfer_data[gas_id].dynamic_tbf_1.usf_value[i] = GPL1_INVALID;

        }

        /*  Reset ul_desc pointer so any previous READS discarded. */
        memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
       l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  = &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];


        GL1_ISR_SAVE_LOCK(gas_id);
        /* check we are still in rach mode and have not transitioned
          * to transfer mode via a previous assignment
        */

        if (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
        {
          MSG_GERAN_MED_0_G("DYNAMIC_TBF_CONFIG rcvd in transfer");
          GL1_ISR_SAVE_UNLOCK(gas_id);
          return;
        }

        l1_transfer_data[gas_id].ul_st = L1_GPRS_INVALID_FN;

        /* starting time not always present in dynamic allocation so check */
        if(message_ptr->dynamic_tbf.tbf_start_time_valid)
        {
          /*  Determine if starting time present and if it needs adjusting  */
          st_elapsed = gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].dynamic_tbf_1.starting_time,
                                                   message_ptr->dynamic_tbf.starting_time,CCCH_51, gas_id);

          l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].dynamic_tbf_1.starting_time;

          /* check if the starting time was received from a immed. Ass */
          {
            /* if the starting time has elapsed or we haven't enough time to process it */
            if ( (st_elapsed) || ( ((gl1_get_FN( gas_id )+ TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME) >=l1_transfer_data[gas_id].starting_time))
            {
              /* action the starting time in 8 frames */
              MSG_GERAN_MED_1_G("st elapsed [%d]",gl1_get_FN( gas_id ));
             l1_transfer_data[gas_id].starting_time_valid = TRUE;
             l1_transfer_data[gas_id].starting_time = ((gl1_get_FN( gas_id ) + TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME);
              gpl1_gprs_chk_blk_bndry(&l1_transfer_data[gas_id].starting_time);
              l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].starting_time;
            }
          }

          /* Update the rach guard time */
          if ( SUB_FN(l1_transfer_data[gas_id].starting_time,gl1_get_FN(gas_id)) >= L1_MAX_RANDOM_ACCESS_MODE_FRAMES)
          {
            l1_max_rach_guard_time[gas_id] = SUB_FN(l1_transfer_data[gas_id].starting_time,gl1_get_FN(gas_id)) +
                                            L1_RACH_ADDITIONAL_GUARD_MARGIN;
            MSG_HIGH("GL1 start time greater than 30s , RACH guard time updated g_time %d",l1_max_rach_guard_time[gas_id],0,0);
          }
        }
        else
        {
          message_ptr->dynamic_tbf.starting_time = (gl1_get_FN( gas_id ));

          message_ptr->dynamic_tbf.starting_time =
                        ((message_ptr->dynamic_tbf.starting_time + TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME);

          message_ptr->dynamic_tbf.tbf_start_time_valid = TRUE;
          /*  Determine if starting time present and if it needs adjusting  */
          (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].dynamic_tbf_1.starting_time,
                                      message_ptr->dynamic_tbf.starting_time,CCCH_51, gas_id);

          l1_transfer_data[gas_id].ul_st = l1_transfer_data[gas_id].dynamic_tbf_1.starting_time;
        }


        /* Mark mac_l1_dynamic_tbf_config received */
        SET_DYNAMIC_TBF_RECV(gas_id);

        if (DYNAMIC_TBF_ALLOC_COMP(gas_id))
        {
          /* If all signals have been received to fully
            * describe a DYNAMIC UL allocation then
            * move to L1_TRANSFER state and service it
            */
          /* setup tbf establish type  */
          l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;
          l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_ALLOC;
          l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
          RESET_BITMAP(gas_id);

          l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];// BK HACK
          l1_tskisr_blk->command = L1_NULL_COMMAND;
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
          gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

          L1_restart_frames_in_random_access_mode(gas_id);
        }
        GL1_ISR_SAVE_UNLOCK(gas_id);

        MSG_GERAN_MED_1_G(" ul_ts_alloc  0x%X ", ul_ts_alloc);

        break;

      }/*lint !e550 ul_ts_alloc only used for debug*/

   /*******************************************************
   *                                                      *
   *                MAC_L1_SINGLE BLOCK_CONFIG            *
   *                                                      *
   ********************************************************/
      case MAC_L1_SINGLE_BLOCK_CONFIG:
      {
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
        boolean starting_time_elapsed;
        uint32   temp_starting_time;
        uint16   tn=0;
        boolean  move_to_transfer = FALSE;

        /*  Set up for single block config  */
        mac_l1_single_block_config_t *message_ptr;

        /*lint -e740 ccww legacy code */
        message_ptr = (mac_l1_single_block_config_t *) msg_header;
        /*lint +e740 */
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];


        MSG_GERAN_MED_2_G("PL1: MAC_L1_SINGLE_BLOCK_CONFIG Rcvd ts%d bmap %d",message_ptr->sb_tbf.ts_number,l1_transfer_data[gas_id].config_bitmap);


        /* We don't know if we are GPRS or EGPRS here, since for 2 phase access we can be assigned
         * a Multiblock allocation for EGPRS  but get assigned a GPRS UL TBF
         *
         * therefore setup mode to GPRS */

       l1_transfer_data[gas_id].egprs_or_gprs_tbf = TBF_MODE_GPRS;


        if(message_ptr->sb_tbf.tbf_mode == TBF_MODE_EGPRS)
        {
          /* Setup number of blocks to transmit for multiblock allocation */
         l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc = message_ptr->sb_tbf.num_rb_alloc;

          MSG_GERAN_HIGH_2_G("PL1: EGPRS Multiblock alloc blks %d [%d]",l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc,gl1_get_FN( gas_id ));
        }else
        {
          /* setup single block allocation for GPRS */
         l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc = 1;
        }

        /* Copy the bep period 2, required by the egprs bep quality
         * measurements module.
         */
        if ( FALSE != message_ptr->sb_tbf.bep_period2.present )
        {
         l1_transfer_data[gas_id].single_block_tbf_1.bep_period2 =
              message_ptr->sb_tbf.bep_period2.bep_period2;
        } else
        {
         l1_transfer_data[gas_id].single_block_tbf_1.bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
        }

        if((!l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc) || (l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc > 2))
        {
          MSG_GERAN_ERROR_1_G("num rb block incorrect %d",l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc);
          return;
        }


       l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;

        /*  Reset ul_desc pointer so any previous READS discarded. */
        memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
       l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  = &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];

        /* ensure the Downlink TBF pointer is null */
       l1_transfer_data[gas_id].dl_tbf_ptr=0;

        /*  Extract the information from the message  */
        /*  Access type, single phase or two phase. This determines which state
            PL1 returns to after transferring the single block, access state for
            two phase access, idle state for everything else. */
       l1_transfer_data[gas_id].single_block_tbf_1.two_phase_access =
                                           message_ptr->sb_tbf.two_phase_access;

        /* setup single block type to uplink as this is a single ul block */
       l1_transfer_data[gas_id].single_block_tbf_1.sb_ul_dl_type = SINGLE_UL_BLOCK;


        /*  timeslot number */
       l1_transfer_data[gas_id].single_block_tbf_1.ts_number =
                                                message_ptr->sb_tbf.ts_number;

        /*  If present, alpha value */
       l1_transfer_data[gas_id].single_block_tbf_1.alpha_present =
                                              message_ptr->sb_tbf.alpha_present;
        if (l1_transfer_data[gas_id].single_block_tbf_1.alpha_present)
        {
         l1_transfer_data[gas_id].single_block_tbf_1.alpha = message_ptr->sb_tbf.alpha;

          /*  Update Alpha value  */
          gprs_serv_cell_meas_ptr->alpha =l1_transfer_data[gas_id].single_block_tbf_1.alpha;
        }


        /*  Downlink power struct */
       l1_transfer_data[gas_id].single_block_tbf_1.downlink_pwr =
                                              message_ptr->sb_tbf.downlink_pwr;


        MSG_GERAN_HIGH_3_G("alpha pres %d alpha %d gamma %d",message_ptr->sb_tbf.alpha_present,message_ptr->sb_tbf.alpha,message_ptr->sb_tbf.gamma_tn);
        {
          MSG_GERAN_HIGH_1_G("Update gamma %d",message_ptr->sb_tbf.gamma_tn);
          /*  Gamma_tn  */
         l1_transfer_data[gas_id].single_block_tbf_1.gamma_tn =
                                                  message_ptr->sb_tbf.gamma_tn;

          /* Update Gamma values only for the single block timeslot, set to zero
           * for all other cases
           */
          for(tn=0;tn<8;tn++)
          {
            if(tn ==l1_transfer_data[gas_id].single_block_tbf_1.ts_number)
            {
              gprs_serv_cell_meas_ptr->gamma_tn[tn] =l1_transfer_data[gas_id].single_block_tbf_1.gamma_tn;
            }else
            {
              gprs_serv_cell_meas_ptr->gamma_tn[tn] = 0;
            }
          }

         /*  Update uplink power based on the modified power control parameters. */
         gpl1_gprs_calc_rf_pwr( gas_id );
        }

        /* setup downlink power control parameter's */
       l1_transfer_data[gas_id].dl_power_control = message_ptr->sb_tbf.downlink_pwr;

        GL1_ISR_SAVE_LOCK(gas_id);

        /*  Starting time, also perform some checks */

        /*  Now check if the starting time is the first frame of a block, if
            not move to next block boundary. Function will return modified
            starting time, If starting time has elapsed function will return
            TRUE. */
        starting_time_elapsed = gpl1_gprs_get_starting_time(&temp_starting_time,
                                                            message_ptr->sb_tbf.starting_time,
                                                            CCCH_51, gas_id);

        if (!starting_time_elapsed)
        {
          if (gpl1_gprs_adjust_mod_num(gl1_get_FN( gas_id ), 3, FRAMES_IN_HYPERFRAME) >= (uint32)temp_starting_time)
          {
            starting_time_elapsed = TRUE;

          }
        }

       l1_transfer_data[gas_id].single_block_tbf_1.timing_advance_params =
                                      message_ptr->sb_tbf.timing_advance_params;

        /* copy nc_non_drx_timer params into local PL1 struct.  Used for both 51 and 52 idle mode */
       l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nc_non_drx_period = message_ptr->sb_tbf.nc_non_drx_period;
       l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nco               = message_ptr->sb_tbf.nco;
       l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.meas_report       = message_ptr->sb_tbf.meas_report;
       l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nc_non_drx_valid  = FALSE;

        if(l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.meas_report)
        {
         if(l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nco == 2)
         {
          l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nc_non_drx_valid = TRUE;

           MSG_GERAN_LOW_0_G("NC_NON_DRX_PERIOD ACTIVE");
         }
        }

        /*  Check if starting time has elapsed, if so, inform MAC, stop process
            and return to IDLE  */
        if (starting_time_elapsed)
        {
          MSG_GERAN_MED_1_G("PL1: SNGLE BLCK ST elapsed fn %d",gl1_get_FN( gas_id ));

          /* clear out the PACCH FIFO as we have not sent it */
          gpl1_gprs_reset_pacch_fifo(gas_id);

          /* Reset meas_flag here because the measurement report will not now be transmitted */
          l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.meas_report       = FALSE;
          l1_transfer_data[gas_id].single_block_tbf_1.nc_non_drx_params.nc_non_drx_valid  = FALSE;

          /* Stay in rach as MAC will retry once we send the SB FAilure */
          //gpl1_gprs_setup_start_idle( (void *)0);
          RESET_BITMAP(gas_id);

          l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

          /* Do not send the MAC 51 SB failure here, wait till access mode
           * has stopped first.
           */
          l1_access_isr_set_mac_sb_failure( gas_id);

          /* reset access states for next (p)rach request */
          l1_transfer_data[gas_id].current_alloc_type = L1_NULL_ALLOC;
          l1_transfer_data[gas_id].starting_time_valid = FALSE;
        }
        else
        {
          /*  If this is a packet resource request then send a Quality Report
              to mac two blocks prior to sending PRR  */
          if (message_ptr->sb_tbf.two_phase_access)
          {
            /*  Measurement Report required, should be sent to Mac no earlier than
                two blocks prior to the starting time and no later than block before
                  the starting time.  */
            MSG_GERAN_MED_1_G("PL1: MAC_L1_QUALITY_RPT Rcvd %d ",gl1_get_FN( gas_id ));

            /*  Set up quality reporting time to be starting time */
            if (gpl1_gprs_adjust_mod_num((uint32)(l1_transfer_data[gas_id].starting_time), -(int32)(gl1_get_FN( gas_id )),
                                                            FRAMES_IN_HYPERFRAME) < (uint32)8)
            {
              /* Increment the number of quality reports to send */
              gprs_serv_cell_meas_ptr->quality_report_count++;

              /*  Indicate associated rrbp_fn in response */
              gprs_serv_cell_meas_ptr->qrt =l1_transfer_data[gas_id].starting_time;

              /*  Sent quality report immediately */
              gpl1_gprs_build_quality_report(gas_id);

            }
            else
            {
              /*  Set up quality reporting time to be within two blocks of
                  starting time Only ever send one so over write if new request */
              gprs_serv_cell_meas_ptr->quality_report_time[0] =l1_transfer_data[gas_id].starting_time;

            }


          } /* End of two phase access switch */

            /*  If single block config has been previously received but not sent,
                (waiting for starting time) and another single block config is
                received, then clear Freq config and uplink data valid flags
                as new ones will be sent */
          if (FREQ_CONFIG_RECV(gas_id) && SINGLE_BLOCK_RECV(gas_id) && UPLINK_DATA_VALID(gas_id))
          {
            /*  Clear freq config and uplink data flags  */
           CLEAR_FREQ_CONFIG_RECV(gas_id);
           CLEAR_UPLINK_DATA_VALID(gas_id);

          }

          /* Mark mac_l1_single_block_config received */
          SET_SINGLE_BLOCK_RECV(gas_id);

          /* Update the rach guard time */
          if ( SUB_FN(l1_transfer_data[gas_id].starting_time,gl1_get_FN(gas_id)) >= L1_MAX_RANDOM_ACCESS_MODE_FRAMES)
          {
             l1_max_rach_guard_time[gas_id] = SUB_FN(l1_transfer_data[gas_id].starting_time,gl1_get_FN(gas_id)) +
                                             L1_RACH_ADDITIONAL_GUARD_MARGIN;
             MSG_HIGH("GL1 start time greater than 30s , RACH guard time updated g_time %d",l1_max_rach_guard_time[gas_id],0,0);
          }
        } /*  End of starting time elapsed else */



        if (SINGLE_BLOCK_ALLOC_COMP(gas_id))
        {

          /* If we are allocated 2 blocks then check if we have
           * received an UL dummy block
           */
          if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 2)
          {

            if(MULTI_BLOCK_ALLOC_COMP(gas_id))
            {
              /*  All signals have been received to fully describe a SINGLE BLOCK
               * UL allocation then so move to L1_TRANSFER state and service it
               */
              move_to_transfer= TRUE;
            }

          }else if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 1)
          {
            /*  All signals have been received to fully describe a SINGLE BLOCK
             *  UL allocation then so move to L1_TRANSFER state and service it
             */
            move_to_transfer = TRUE;
          }

          if(move_to_transfer)
          {

            /* setup tbf establish type  */
            l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;

            l1_transfer_data[gas_id].current_alloc_type = L1_SINGLE_BLOCK_ALLOC;
            l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
            l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
            l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;
            RESET_BITMAP(gas_id);
            l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
            l1_tskisr_blk->command = L1_NULL_COMMAND;
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
            gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/
            l1_sc_start( L1SCModeGprsTrans, gas_id );
            L1_restart_frames_in_random_access_mode(gas_id);
          }
        }

#ifdef FEATURE_GTA_DURING_PS_RACH
        {
          idle_data_T *l1_idle_data = &l1_idle_data_store[gas_id];
          l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
        }
#endif /*FEATURE_GTA_DURING_PS_RACH*/
        GL1_ISR_SAVE_UNLOCK(gas_id);

        break;
      }

   /********************************************************
   *                                                       *
   *                MAC_L1_DL_SINGLE_BLOCK                 *
   *                                                       *
   ********************************************************/
    case MAC_L1_DL_SINGLE_BLOCK:
    {
      mac_l1_dl_single_block_t *message_ptr;
      boolean starting_time_elapsed;

      /*lint -e740 ccww legacy code */
      message_ptr = ( mac_l1_dl_single_block_t *) msg_header;
      /*lint +e740 */
      gas_id = check_gas_id(message_ptr->gas_id);
      MSG_GERAN_HIGH_1_G("PL1: MAC_L1_DL_SINGLE_BLOCK Rcvd - bmap %d",l1_transfer_data[gas_id].config_bitmap);



      /* We can only be a GPRS TBF here so setup mode to GPRS */
      l1_transfer_data[gas_id].egprs_or_gprs_tbf = TBF_MODE_GPRS;

      l1_transfer_data[gas_id].single_block_tbf_1.two_phase_access = 0;
      l1_transfer_data[gas_id].single_block_tbf_1.sb_ul_dl_type = SINGLE_DL_BLOCK;
      l1_transfer_data[gas_id].single_block_tbf_1.ts_number = message_ptr->tn;

      /*  Reset ul_desc pointer so any previous READS discarded. */
      memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
      l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  = &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];

      /*  Starting time, also perform some checks */

      /*  Now check if the starting time is the first frame of a block, if
            not move to next block boundary. Function will return modified
            starting time, If starting time has elapsed function will return
            TRUE.
       */
      starting_time_elapsed = gpl1_gprs_get_starting_time(NULL,
                                                           message_ptr->tbf_starting_time,
                                                           CCCH_51, gas_id);

      MSG_GERAN_HIGH_2_G("st %d fn %d",l1_transfer_data[gas_id].starting_time,gl1_get_FN( gas_id ));

      /* make sure we have enough time to action the stating time */
      if(((l1_transfer_data[gas_id].starting_time - (gl1_get_FN( gas_id )) < 4)) && (!starting_time_elapsed))
      {
        starting_time_elapsed = TRUE;
      }

      /*  Check if starting time has elapsed, if so, inform MAC, stop process
          and return to IDLE  */
      if (starting_time_elapsed)
      {
        /*  Inform MAC  */
        l1_mac_sig_t            message_buffer,*message;

        RESET_BITMAP(gas_id);

        MSG_GERAN_MED_1_G("PL1: SNGLE BLCK ST elapsed %d",l1_transfer_data[gas_id].config_bitmap);

        /* Align the pointers */
        message = &message_buffer;

        /* Set up the message header */
        message->task_id = MS_MAC_L1;
        message->sig_id = L1_MAC_51_SB_FAILURE;
        message->msg.sb_51_failure.gas_id = check_gas_id(message_ptr->gas_id);

        /* Send the message, NULL used for MAC_PH_DATA_IND */
		L1_send_gprs_gmac_message(message, NULL,gl1_get_FN(gas_id), gas_id);

      }

      /*  If single block config has been previously received but not sent,
            (waiting for starting time) and another single block config is
            received, then clear Freq config and uplink data valid flags
            as new ones will be sent */
      if (FREQ_CONFIG_RECV(gas_id) && SINGLE_BLOCK_RECV(gas_id) )
      {
        /*  Clear freq config flags  */
        CLEAR_FREQ_CONFIG_RECV(gas_id);
        MSG_GERAN_MED_2_G("clr f_cfg %d %d",gl1_get_FN( gas_id ),l1_transfer_data[gas_id].config_bitmap);

      }

      /* Mark mac_l1_single_block_config received if the startiing time has not elapsed*/
      if(!starting_time_elapsed)
      {
        SET_SINGLE_BLOCK_RECV(gas_id);
        if ( SUB_FN(l1_transfer_data[gas_id].starting_time,gl1_get_FN(gas_id)) >= L1_MAX_RANDOM_ACCESS_MODE_FRAMES)
          {
             l1_max_rach_guard_time[gas_id] = SUB_FN(l1_transfer_data[gas_id].starting_time,gl1_get_FN(gas_id)) +
                                             L1_RACH_ADDITIONAL_GUARD_MARGIN;
             MSG_GERAN_HIGH_1_G("GL1 start time greater than 30s , RACH guard time updated g_time %d",l1_max_rach_guard_time[gas_id]);
          }
      }


      if (SINGLE_DL_BLOCK_ALLOC_COMP(gas_id))
      {
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        /*  All signals have been received to fully describe a SINGLE BLOCK
            UL allocation then so move to L1_TRANSFER state and service it */
        MSG_GERAN_MED_1_G("alloc comp %d",gl1_get_FN( gas_id ));

        /* setup tbf establish type  */
        l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

        l1_transfer_data[gas_id].current_alloc_type = L1_SINGLE_BLOCK_ALLOC;
        l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
        l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
        l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;

        RESET_BITMAP(gas_id);

        l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
        l1_tskisr_blk->command = L1_NULL_COMMAND;
        l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
        gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT,l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/
        L1_restart_frames_in_random_access_mode(gas_id);
      }
#ifdef FEATURE_GTA_DURING_PS_RACH
      {
        idle_data_T *l1_idle_data = &l1_idle_data_store[gas_id];
        l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
      }
#endif /*FEATURE_GTA_DURING_PS_RACH*/
      break;
    }

   /*******************************************************
   *                                                      *
   *                MAC_L1_DL_TBF_CONFIG                  *
   *                                                      *
   ********************************************************/
      case MAC_L1_DL_TBF_CONFIG:
      {

        mac_l1_dl_tbf_config_t  *message_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
        boolean                 st_elapsed;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_dl_tbf_config_t *) msg_header;
        /*lint +e740 */
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];
        gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];

        MSG_GERAN_MED_0_G("MAC_L1_DL_TBF_CONFIG ");
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
        if(l1_tskisr_blk->main_command == L1_WAIT_FOR_MDSP_TO_COMPLETE)
		{
            L1_send_MPH_START_IDLE_MODE_CNF(gas_id);
		}
#endif

        /* Setup mode to determine whether this TBF is EGPRS or GPRS */
       l1_transfer_data[gas_id].egprs_or_gprs_tbf = message_ptr->tbf_mode;

        /* (1) Used for passing MCS 1- 9 crc failed payload data to RLC ONLY in UNACK mode
         * for RLC ACK mode  MCS 1- 9 crc failed payload data is thrown away
         *
         * (2) Used to Setup Incremental redundancy for Downlink TBF
         * RLC Unack mode = IR OFF
         * RLC ACK mode  = IR ON
         */
       l1_transfer_data[gas_id].dl_tbf_1.rlc_mode = message_ptr->rlc_mode;

       l1_transfer_data[gas_id].dl_tbf_1.link_qual_meas_mode =
            message_ptr->link_qual_meas_mode;

        if( FALSE != message_ptr->bep_period2.present )
        {
           l1_transfer_data[gas_id].dl_tbf_1.bep_period2 =
                message_ptr->bep_period2.bep_period2;
        }
        else
        {
           l1_transfer_data[gas_id].dl_tbf_1.bep_period2 = GPRS_PL1_BEP_PERIOD2_INVALID;
        }

        /* Make sure an uplink assignment message is not
         * being queued before writing the parameter for
         * the two structures.
         */
        if( !(  ( UPLINK_TBF_ESTABLISHED ==l1_transfer_data[gas_id].tbf_est_type )
              &&( L1_TRANSFER_COMMAND == l1_tskisr_blk->main_command )
             )
          )
        {
            /* Set the mac mode to invalid in the two
             * available dynamic tbf structures.
             * Ignore the mac mode included in the downlink
             * assignment.
             */
           l1_transfer_data[gas_id].dynamic_tbf_1.mac_mode = L1_MAC_MODE_INVALID;
           l1_transfer_data[gas_id].dynamic_tbf_2.mac_mode = L1_MAC_MODE_INVALID;
        }

        /* Prepare the SCE to start in Transfer mode.
            * It won't start until l1_sci_go
            * is called from the ISR context.
            * Ensure we haven't already started SC.
            */
        if (l1_tskisr_blk->main_command != L1_TRANSFER_COMMAND)
        {
          l1_sc_start( L1SCModeGprsTrans, gas_id );
        }

       l1_transfer_data[gas_id].l1_called_from_sngl_blk = FALSE;

        /*  Reset ul_desc pointer so any previous READS discarded. */
        memset(&l1_transfer_data[gas_id].rlc_ul_status,0,sizeof(rlc_ul_status_T) );
       l1_transfer_data[gas_id].rlc_ul_status.ul_desc_ptr  = &l1_transfer_data[gas_id].rlc_ul_status.ul_desc[0];

        /* Set the mac mode to invalid in the 2nd
         * available dynamic tbf structure.
         * Ignore the mac mode included in the downlink
         * assignment (Rel 6).
         */
       l1_transfer_data[gas_id].dl_tbf_1.mac_mode = L1_MAC_MODE_INVALID;
       l1_transfer_data[gas_id].dl_tbf_2.mac_mode = L1_MAC_MODE_INVALID;

        /* copy power control parameters */
        /*  Load serving cell parameters from message */
        if (message_ptr->power_control.present)
        {
          uint8 index;
          power_control_gamma_T *pwr_cntrl_ptr;

          /*  Update Alpha value  */
          gprs_serv_cell_meas_ptr->alpha = message_ptr->power_control.alpha;

          /*  Extract gamma_tn values  */
          pwr_cntrl_ptr = &(message_ptr->power_control.gamma_TN0);
          index = 0;

          do
          {
            if (pwr_cntrl_ptr->present)
            {
              gprs_serv_cell_meas_ptr->gamma_tn[index] = pwr_cntrl_ptr->gamma;
            }
            else
            {
              gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;

            }

            /*  Increment pointer  */
            pwr_cntrl_ptr++;
            index++;

          }while (index < 8);

        }
        else
        {
          uint8 index;
          /*  If no power control parameters, use PMAX power (GSM5.05 10.2.1) */
          /*  Easiest way is to set gamma_tn to 0, and alpha to 0 */
          for(index=0;index<8;index++)
          {
            gprs_serv_cell_meas_ptr->gamma_tn[index] = 0;
          }
          gprs_serv_cell_meas_ptr->alpha = 0;

        }

        /*  Update uplink power based on the modified power control parameters. */
        gpl1_gprs_calc_rf_pwr( gas_id );

         /* setup downlink power control parameter's */
       l1_transfer_data[gas_id].dl_power_control = message_ptr->downlink_pwr;

        /* Not checking present flag as assuming DL timeslot alloc
          * always present.
          *
          * Change order of timeslot description
          */
       l1_transfer_data[gas_id].dl_tbf_1.nts = gpl1_gprs_reorder_tn(
                                                            &message_ptr->ts_allocation,
                                                            &l1_transfer_data[gas_id].dl_tbf_1.ts_allocation
                                                          );

        gpl1_gprs_add_to_active_dl_ta_params(message_ptr->ta_params,gas_id);

        if (message_ptr->dl_tfi_present)
        {
         l1_transfer_data[gas_id].dl_tbf_1.dl_tfi_present = TRUE;
         l1_transfer_data[gas_id].dl_tbf_1.dl_tfi = message_ptr->dl_tfi;
        }
        else
        {
         l1_transfer_data[gas_id].dl_tbf_1.dl_tfi_present = FALSE; // should already have tfi from UL TBF
        }

        GL1_ISR_SAVE_LOCK(gas_id);

        /* check we are still in rach mode and have not transitioned
         * to transfer mode via a previous assignment
         */

        if (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE)
        {
          MSG_GERAN_MED_0_G("DL_TBF_CONFIG rcvd in transfer");
          GL1_ISR_SAVE_UNLOCK(gas_id);
          return;
        }

        l1_transfer_data[gas_id].dl_st = L1_GPRS_INVALID_FN;

        /* check to see if a start time has been specified */
        if (message_ptr->tbf_start_time_valid)
        {
          /*  Determine if starting time present and if it needs adjusting  */
          st_elapsed = gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].dl_tbf_1.tbf_start_time,
                                                   message_ptr->tbf_start_time,
                                                   CCCH_51, gas_id);

          l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].dl_tbf_1.tbf_start_time;

          /* check if the starting time was received from a immed. Ass */
          {
            /* if the starting time has elapsed or we haven't enough time to process it */
            if ( (st_elapsed) || ( ((gl1_get_FN( gas_id )+ TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME) >=l1_transfer_data[gas_id].starting_time))
            {
              /* action the starting time in 5 frames */
              MSG_GERAN_MED_1_G("st elapsed [%d]",gl1_get_FN( gas_id ));
              l1_transfer_data[gas_id].starting_time_valid = TRUE;
              l1_transfer_data[gas_id].starting_time = ((gl1_get_FN( gas_id ) + TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME);
              gpl1_gprs_chk_blk_bndry(&l1_transfer_data[gas_id].starting_time);
              l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].starting_time;
            }
          }
          if ( SUB_FN(l1_transfer_data[gas_id].starting_time,gl1_get_FN(gas_id)) >= L1_MAX_RANDOM_ACCESS_MODE_FRAMES)
          {
            l1_max_rach_guard_time[gas_id] = SUB_FN(l1_transfer_data[gas_id].starting_time,gl1_get_FN(gas_id)) +
                                            L1_RACH_ADDITIONAL_GUARD_MARGIN;
            MSG_HIGH("GL1 start time greater than 30s , RACH guard time updated g_time %d",l1_max_rach_guard_time[gas_id],0,0);
          }

        }
        else
        {
          message_ptr->tbf_start_time = (gl1_get_FN( gas_id ));

          /* check if the starting time was received from a immed. Ass */
          {
                  message_ptr->tbf_start_time = ((message_ptr->tbf_start_time + TEMP_DELAY_OS + 1)% FRAMES_IN_HYPERFRAME);
          }
          message_ptr->tbf_start_time_valid = TRUE;
          /*  Determine if starting time present and if it needs adjusting  */
          (void)gpl1_gprs_get_starting_time(&l1_transfer_data[gas_id].dl_tbf_1.tbf_start_time,
                                      message_ptr->tbf_start_time,
                                      CCCH_51, gas_id);

          l1_transfer_data[gas_id].dl_st = l1_transfer_data[gas_id].dl_tbf_1.tbf_start_time;
        }

        /* Mark mac_l1_dl_tbf_config received */
        SET_DOWNLINK_TBF_RECV(gas_id);

        /* Check if transition to TRANSFER mode is possible */

        if (DOWNLINK_TBF_ALLOC_COMP(gas_id))
        {
          /* If all signals have been received to fully
           * describe a DOWNLINK allocation then
           * move to L1_TRANSFER state and service it
           */
          /* setup tbf establish type  */
          l1_transfer_data[gas_id].tbf_est_type = DOWNLINK_TBF_ESTABLISHED;

          l1_transfer_data[gas_id].current_alloc_type = L1_DOWNLINK_ALLOC;
          l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;

          RESET_BITMAP(gas_id);

          l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
          l1_tskisr_blk->command = L1_NULL_COMMAND;
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
          gpl1_gprs_recovery_register_event(GPL1_DL_CONNECT_EVENT,l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/ 
        }
#ifdef FEATURE_GTA_DURING_PS_RACH
        {
          idle_data_T *l1_idle_data = &l1_idle_data_store[gas_id];
          l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
        }
#endif /*FEATURE_GTA_DURING_PS_RACH*/

      GL1_ISR_SAVE_UNLOCK(gas_id);
      break;
      }
   /*******************************************************
   *                                                      *
   *                MAC_L1_RELEASE                        *
   *                                                      *
   ********************************************************/
      case MAC_L1_RELEASE:
      {
        mac_l1_release_t *message_ptr;
        prach_params_T  *l1_prach_params_ptr;
        gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_release_t *) msg_header;
        /*lint +e740 */
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_prach_params_ptr = gl1_ms_switch_prach_params_idle_if_data(gas_id);
        gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
        MSG_GERAN_HIGH_0_G("MAC_L1_REL");

        GL1_ISR_SAVE_LOCK(gas_id);

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
        gpl1_gprs_register_release_recovery_event(message_ptr->release_tbf,gas_id);
#endif

        switch (message_ptr->release_tbf)
        {
          case  UL_TBF_NORMAL:
          {

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
            #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/


            /*  Check if there is an UL RRBP pending  */
            if (gpl1_gprs_get_ul_num_rrbp_msg(gas_id))
            {
              l1_prach_params_ptr->rrbp_pending = TRUE;
              MSG_GERAN_MED_1_G("prach-rrbp release pending [%d]",gl1_get_FN( gas_id ));

            }else
            {
              MSG_GERAN_MED_1_G("prach normal release[%d]",gl1_get_FN( gas_id ));

              /* set release type for release confirm which is sent we
              * get back to IDLE mode
              */
              l1_prach_params_ptr->rrbp_pending = FALSE;

              /* Ensure that the release confirm will be sent */
             l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
             l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL;
            }

            break;
          }

          case  UL_TBF_ABNORMAL:
          {
            /* Setup a pointer to the serving cell meas
             * structure.
             */
            gprs_pl1_serving_cell_meas_T* scell_meas_ptr = gprs_serv_cell_meas_ptr;

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

            /*  As abnormal there will be no RRBP outstanding */
            MSG_GERAN_MED_1_G("prach abnormal release[%d]",gl1_get_FN( gas_id ));

            /* set release type for release confirm which is sent we
            * get back to IDLE mode
            */
            l1_prach_params_ptr->rrbp_pending = FALSE;

            /* Ensure that the release confirm will be sent */
            l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
            l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_UL;

            /* Remove any pending GPRS quality reports
             * MAX_NUM_QUAL_RPRTS has a value 24.
             */
            scell_meas_ptr->quality_report_time[0] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[1] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[2] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[3] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[4] = L1_GPRS_INVALID_FN;

            scell_meas_ptr->quality_report_time[5] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[6] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[7] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[8] = L1_GPRS_INVALID_FN;

            scell_meas_ptr->quality_report_time[9]  = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[10] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[11] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[12] = L1_GPRS_INVALID_FN;

            scell_meas_ptr->quality_report_time[13] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[14] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[15] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[16] = L1_GPRS_INVALID_FN;

            scell_meas_ptr->quality_report_time[17] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[18] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[19] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[20] = L1_GPRS_INVALID_FN;

            scell_meas_ptr->quality_report_time[21] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[22] = L1_GPRS_INVALID_FN;
            scell_meas_ptr->quality_report_time[23] = L1_GPRS_INVALID_FN;

            /* Reset the counter and the index to help prevent
             * already signalled quality reports from being generated
             * by the L1 task.
             */
            scell_meas_ptr->quality_report_time_index = 0;
            scell_meas_ptr->quality_report_count      = 0;

            /* Mask off the quality report time bit for the same reason
             * as above.
             */
            scell_meas_ptr->task_event &= (~QUALITY_REPORT_TIME);

            break;
          }

          case  DL_TBF_NORMAL:
          {

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
             #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

            /*  Check if there is an DL RRBP pending  */
            if( gpl1_gprs_get_dl_num_rrbp_msg(gas_id) )
            {
              l1_prach_params_ptr->rrbp_pending = TRUE;
              MSG_GERAN_MED_1_G("prach-rrbp release pending [%d]",gl1_get_FN( gas_id ));

            }else
            {
              MSG_GERAN_MED_1_G("prach normal release[%d]",gl1_get_FN( gas_id ));

              /* set release type for release confirm which is sent we
              * get back to IDLE mode
              */
              l1_prach_params_ptr->rrbp_pending = FALSE;

              /* Ensure that the release confirm will be sent */
             l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
             l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_DL;
            }

            break;
          }

        case  DL_TBF_ABNORMAL:
          {
            /*  As abnormal there will be no RRBP outstanding */
            MSG_GERAN_MED_1_G("prach abnormal release[%d]",gl1_get_FN( gas_id ));

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
             #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

            /* set release type for release confirm which is sent we
            * get back to IDLE mode
            */
            l1_prach_params_ptr->rrbp_pending = FALSE;

            /* Ensure that the release confirm will be sent */
           l1_transfer_data[gas_id].confirm_pending = SEND_IN_ISR;
           l1_transfer_data[gas_id].release_pending = L1_FULL_RELEASE_DL;

            break;
          }

        default:
          {
            MSG_GERAN_ERROR_1_G("Release tbf type bad! %d",message_ptr->release_tbf);

            break;
          }


        } /* End of switch */

        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        if((l1_tskisr_blk->l1_state == L1_IDLE_MODE) &&
           (l1_transfer_data[gas_id].release_pending != L1_NO_RELEASE))
        {
          gpl1_gprs_send_l1_mac_tbf_rel_confirm(l1_transfer_data[gas_id].release_pending,gas_id);
          l1_transfer_data[gas_id].confirm_pending = SENT;
          l1_transfer_data[gas_id].release_pending = L1_NO_RELEASE;
        }

        /*  Set starting time to invalid as release has been received */
        l1_transfer_data[gas_id].starting_time = L1_GPRS_INVALID_FN;

        /* stop the SCE as we are about to return to idle */
        l1_sc_stop(gas_id);

        GL1_ISR_SAVE_UNLOCK(gas_id);

        /*  Return to IDLE when release received  */
        gpl1_gprs_setup_start_idle((void *)0, gas_id);

        break;
      }

   /*******************************************************
   *                                                      *
   *                MAC_PH_DATA_REQ                       *
   *                                                      *
   ********************************************************/
    case MAC_PH_DATA_REQ:
    {
      mac_ph_data_req_t  *message_ptr;
      volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
      mac_ul_ctrl_buffer_T *mac_data_ptr;
      boolean move_to_transfer = FALSE;

      /*lint -e740 ccww legacy code */
      message_ptr = ( mac_ph_data_req_t *) msg_header;
      /*lint +e740 ccww legacy code */
      gas_id = check_gas_id(message_ptr->gas_id);
      MSG_GERAN_MED_1_G("PL1: MAC_PH_DATA_REQ Rcvd - bitmap %d",l1_transfer_data[gas_id].config_bitmap);

      l1_tskisr_blk = &l1_tsk_buffer[gas_id];

      mac_data_ptr =l1_transfer_data[gas_id].mac_ul_ctrl_buff_ptr;

#if 0
      /* We should only have 1 PACCH msg in the buffer, overwrite if needed */
      if(mac_data_ptr->pending_msgs + mac_data_ptr->sched_msgs)
      {
        /* Decrement the message pointer to overwrite the current PACH and not generate two  */

        if( --(mac_data_ptr->pacch_data_in_ptr) < mac_data_ptr->pacch_data)
        {
          mac_data_ptr->pacch_data_in_ptr = &(mac_data_ptr->pacch_data[MAX_NUM_UL_PACCH_MSGS -1]);
        }

        /* Decrement the number of messages */
        mac_data_ptr->pending_msgs--;

      }
#endif

      /* check there is space to copy new message into L1 buffer */
      if(( mac_data_ptr->pending_msgs + mac_data_ptr->sched_msgs) < MAX_NUM_UL_PACCH_MSGS )
      {
        /* Copy PACCH message into L1 buffer */
	        				memscpy(
                 mac_data_ptr->pacch_data_in_ptr->control_block,
	             sizeof(mac_data_ptr->pacch_data_in_ptr->control_block),
                 message_ptr->dl_ctrl_block,
                 SIZE_OF_CONTROL_RADIO_BLOCK
              );

#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM
        /* Set the valid boolean */
        mac_data_ptr->pacch_data_in_ptr->valid = TRUE;
#endif
#endif

        /*
         * Increment in pointer for next access.
         * Check if end of buffer has been reached,
         * and set to first element if it has
         */
        if( ++(mac_data_ptr->pacch_data_in_ptr) > &(mac_data_ptr->pacch_data[MAX_NUM_UL_PACCH_MSGS -1]) )
        {
          mac_data_ptr->pacch_data_in_ptr = mac_data_ptr->pacch_data;
        }

        /* Data now available in L1 buffer for transmission */
        mac_data_ptr->data_valid = TRUE;

        /* Increment the number of messages */
        mac_data_ptr->pending_msgs++;

        /* PACCH UL CONTROL available */
       l1_transfer_data[gas_id].mac_ul_ctrl_available = TRUE;

        /*  If part of a single block allocation and an allocation is already
            in progress but another freq conf is received before the starting
            time has elapsed, then clear single block config and freq config
            flags as they will be received again. */
        if (FREQ_CONFIG_RECV(gas_id) && SINGLE_BLOCK_RECV(gas_id) && UPLINK_DATA_VALID(gas_id))
        {
          /* Validate uplink data reception. if we are configured to transmit
           * 2 blocks then reception of more than 1 MAC_PH_DATA_REQ is allowed!!
           * Otherwise clear out Frequency and single block reception status
           */

          if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 1)
          {

            /*  Clear freq config and uplink data flags  */
            CLEAR_FREQ_CONFIG_RECV(gas_id);
            CLEAR_SINGLE_BLOCK_RECV(gas_id);
          }

        }

        /*  Data received set the SET_UPLINK_DATA_VALID bit. This flag is
            currently only used when checking for single block config. */
        SET_UPLINK_DATA_VALID(gas_id);

      }
      else
      {
        MSG_GERAN_ERROR_0_G("PL1: ERROR MAC UL PACCH BUFFER OVERFLOW");
      }

      if (SINGLE_BLOCK_ALLOC_COMP(gas_id))
      {
        /* If we are allocated 2 blocks then check if we have
         * received an UL dummy block
         */
        if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 2)
        {

          if(MULTI_BLOCK_ALLOC_COMP(gas_id))
          {
            /*  All signals have been received to fully describe a SINGLE BLOCK
             * UL allocation then so move to L1_TRANSFER state and service it
             */
            move_to_transfer= TRUE;
          }

        }else if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 1)
        {
          /*  All signals have been received to fully describe a SINGLE BLOCK
           *  UL allocation, so move to L1_TRANSFER state and service it
           */
          move_to_transfer = TRUE;
        }

        if(move_to_transfer)
        {
          /*  All signals have been received to fully describe a SINGLE BLOCK
              UL allocation then so move to L1_TRANSFER state and service it */

          /*  AS the MAC_PH_DATA_REQ signal will be the last one received in this
              sequence, check that the starting time has not elapsed. */
          if ((((l1_transfer_data[gas_id].starting_time - gl1_get_FN( gas_id ) + FRAMES_IN_HYPERFRAME) % FRAMES_IN_HYPERFRAME) >= 4) &&
                    (((l1_transfer_data[gas_id].starting_time - gl1_get_FN( gas_id ) + FRAMES_IN_HYPERFRAME) % FRAMES_IN_HYPERFRAME) < (FRAMES_IN_HYPERFRAME >> 1)))
          {
            /* Make sure the tbf establishment type is set, ensures
             * boundary case uplink release request from MAC is serviced
             * correctly.
             */
           l1_transfer_data[gas_id].tbf_est_type = UPLINK_TBF_ESTABLISHED;
           l1_transfer_data[gas_id].current_alloc_type = L1_SINGLE_BLOCK_ALLOC;
           l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
           l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
           l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;

            l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
            l1_tskisr_blk->command = L1_NULL_COMMAND;
            l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
            gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

          }
          else
          {
            MSG_GERAN_MED_1_G("PL1: SNGLE BLCK ST elapsed fn %d",gl1_get_FN( gas_id ));

            /* clear out the PACCH FIFO as we have noy sent it */
            gpl1_gprs_reset_pacch_fifo(gas_id);

            /* Stay in idle as MAC will retry once we send the SB Failure */
            //gpl1_gprs_setup_start_idle( (void *)0);
                l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

            /* Do not send the MAC 51 SB failure here, wait till access mode
             * has stopped first.
             */
            l1_access_isr_set_mac_sb_failure(gas_id );

            /* reset access states for next (p)rach request */
            l1_transfer_data[gas_id].current_alloc_type = L1_NULL_ALLOC;
            l1_transfer_data[gas_id].starting_time_valid = FALSE;

          }/*  Not enough time to process, starting time has elapsed */
          RESET_BITMAP(gas_id);
        }/*  Not enough time to process, starting time has elapsed */

      }
      break;
    }

   /*******************************************************
   *                                                      *
   *                MAC_L1_DUMMY_CTRL_BLOCK               *
   *                                                      *
   ********************************************************/
      case MAC_L1_DUMMY_CTRL_BLOCK:
      {

        mac_l1_dummy_ctrl_block_t  *message_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        boolean move_to_transfer = FALSE;

        /*lint -e740 ccww legacy code */
        message_ptr = ( mac_l1_dummy_ctrl_block_t *) msg_header;
        /*lint +e740 */
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];


        MSG_GERAN_MED_0_G("MAC_L1_DUMMY_CONTROL ");

        memscpy( l1_transfer_data[gas_id].dummy_control_block,
                 sizeof(l1_transfer_data[gas_id].dummy_control_block),
                 message_ptr->dummy_ctrl_block,
                 sizeof(message_ptr->dummy_ctrl_block) );

        /* Mark mac_l1_dummy_ctrl_block received */
        SET_DUMMY_CTRL_RECV(gas_id);

        if( DYNAMIC_TBF_ALLOC_COMP(gas_id) )
        {
          /* Prepare the SCE to start in Transfer mode.
           * It won't start until l1_sci_go
           * is called from the ISR.
           */
          l1_sc_start( L1SCModeGprsTrans, gas_id );

          /* If all signals have been received to fully
           * describe a DYNAMIC UL allocation then
           * move to L1_TRANSFER state and service it
           */
          GL1_ISR_SAVE_LOCK(gas_id);
          l1_transfer_data[gas_id].current_alloc_type = L1_DYNAMIC_ALLOC;
          l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
          l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
          l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;

          RESET_BITMAP(gas_id);

          l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
          l1_tskisr_blk->command = L1_NULL_COMMAND;
          l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
          gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

          GL1_ISR_SAVE_UNLOCK(gas_id);

        }else if (SINGLE_BLOCK_ALLOC_COMP(gas_id))
        {

          /* If we are allocated 2 blocks then check if we have
           * received an UL dummy block
           */
          if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 2)
          {

            if(MULTI_BLOCK_ALLOC_COMP(gas_id))
            {
              /*  All signals have been received to fully describe a SINGLE BLOCK
               * UL allocation then so move to L1_TRANSFER state and service it
               */
              move_to_transfer= TRUE;
            }

          }else if(l1_transfer_data[gas_id].single_block_tbf_1.num_rb_alloc == 1)
          {
            /*  All signals have been received to fully describe a SINGLE BLOCK
             *  UL allocation, so move to L1_TRANSFER state and service it
             */
            move_to_transfer = TRUE;
          }

          if(move_to_transfer)
          {
            /*  All signals have been received to fully describe a SINGLE BLOCK
                UL allocation then so move to L1_TRANSFER state and service it */

            /*  AS the MAC_PH_DATA_REQ signal will be the last one received in this
                sequence, check that the starting time has not elapsed. */
            if ((((l1_transfer_data[gas_id].starting_time - gl1_get_FN( gas_id ) + FRAMES_IN_HYPERFRAME) % FRAMES_IN_HYPERFRAME) >= 4) &&
                      (((l1_transfer_data[gas_id].starting_time - gl1_get_FN( gas_id ) + FRAMES_IN_HYPERFRAME) % FRAMES_IN_HYPERFRAME) < (FRAMES_IN_HYPERFRAME >> 1)))
            {
             l1_transfer_data[gas_id].current_alloc_type = L1_SINGLE_BLOCK_ALLOC;
             l1_transfer_data[gas_id].pending_alloc_type = L1_NULL_ALLOC;
             l1_transfer_data[gas_id].pending_dynamic_tbf_ptr = NULL;
             l1_transfer_data[gas_id].pending_dl_tbf_ptr = NULL;

              l1_tskisr_blk->next_params.L1Data.pTransfer_data = &l1_transfer_data[gas_id];
              l1_tskisr_blk->command = L1_NULL_COMMAND;
              l1_tskisr_blk->main_command = L1_TRANSFER_COMMAND;
#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY
              gpl1_gprs_recovery_register_event(GPL1_UL_CONNECT_EVENT, l1_transfer_data[gas_id].starting_time, gas_id);
#endif /*FEATURE_GERAN_PTM_ERR_RECOVERY*/

            }
            else
            {
              MSG_GERAN_MED_1_G("PL1: SNGLE BLCK ST elapsed fn %d",gl1_get_FN( gas_id ));

              /* clear out the PACCH FIFO as we have noy sent it */
              gpl1_gprs_reset_pacch_fifo(gas_id);

              /* Stay in idle as MAC will retry once we send the SB Failure */
              //gpl1_gprs_setup_start_idle( (void *)0);
              RESET_BITMAP(gas_id);

              /* reset access states for next (p)rach request */

              /* Drop back to IDLE or 52 IDLE, MAC will not release L1 in this case */
              l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

              /* Do not send the MAC 51 SB failure here, wait till access mode
               * has stopped first.
               */
              l1_access_isr_set_mac_sb_failure(gas_id );

              l1_transfer_data[gas_id].current_alloc_type = L1_NULL_ALLOC;
              l1_transfer_data[gas_id].starting_time_valid = FALSE;

            }/*  Not enough time to process, starting time has elapsed */
          } /* end of move_to_transfer */
        }

      break;
      } /* end of case */




    /********************************************************
    *                                                      *
    *                MAC_L1_PACCH_RRBP                     *
    *                                                      *
    ********************************************************/
      case MAC_L1_PACCH_RRBP:
        gpl1_gprs_prach_add_rrbp_msg((mac_l1_pacch_rrbp_t *)msg_header);
            break;

    default:
      break; // do nothing

    } /* end of  switch */

  } /* end of if MS_MAC_L1 */

  else if(msg_header->message_set == MS_RR_L1)
  {
    switch ( msg_header->message_id )
    {
      /********************************************************
      *                                                      *
      *                MPH_START_GPRS_IDLE_REQ               *
      *                                                      *
      ********************************************************/

    /* Enable Sleep indication */
    case MPH_ENABLE_SLEEP_IND:
    {
      L1_handle_ENABLE_SLEEP_IND ( msg_header );
      break;
    }

      case MPH_START_GPRS_IDLE_MODE_REQ:
      {
         mph_start_gprs_idle_mode_req_t *message_ptr;
         message_ptr = ( mph_start_gprs_idle_mode_req_t *) msg_header;

        gl1_handle_mph_start_gprs_idle_mode_req(message_ptr);

        gas_id = check_gas_id(message_ptr->gas_id);
        gpl1_gprs_setup_start_idle((void *)0, gas_id);
        break;
      }
      /********************************************************
       *                                                      *
       *                MPH_START_MM_NON_DRX_IND:             *
       *                                                      *
       ********************************************************/
      case MPH_START_MM_NON_DRX_IND:
      {
        idle_data_T               *l1_idle_data;
        mph_start_mm_non_drx_ind_t* message_ptr = ( mph_start_mm_non_drx_ind_t* )msg_header;

        gas_id = check_gas_id(message_ptr->gas_id);
        l1_idle_data=&l1_idle_data_store[gas_id];
        MSG_GERAN_HIGH_1_G("MPH_START_MM_NON_DRX_IND %d",message_ptr->non_updated_lai_in_nmo1);
        /* Enable the MM non-drx mode period */
        l1_idle_data->mm_non_drx_mode = TRUE;
        l1_idle_data->non_updated_lai_in_nmo1 = message_ptr->non_updated_lai_in_nmo1;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_NON_DRX_START_IND);
#endif

        break;
      }

      /********************************************************
       *                                                      *
       *                MPH_STOP_MM_NON_DRX_IND:              *
       *                                                      *
       ********************************************************/
      case MPH_STOP_MM_NON_DRX_IND:
      {
        idle_data_T               *l1_idle_data;

        mph_stop_mm_non_drx_ind_t* message_ptr = ( mph_stop_mm_non_drx_ind_t* )msg_header;

        gas_id = check_gas_id(message_ptr->gas_id);
        l1_idle_data=&l1_idle_data_store[gas_id];
        MSG_GERAN_MED_0_G("MPH_STOP_MM_NON_DRX_IND");
        /* Disable the MM non-drx mode period */
        l1_idle_data->mm_non_drx_mode = FALSE;
        l1_idle_data->non_updated_lai_in_nmo1 = FALSE;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
        gl1_qsh_event_notify(gas_id, GL1_QSH_EVENT_NON_DRX_STOP_IND);
#endif

        break;
      }

      /********************************************************
       *                                                      *
       *                MPH_NC_MEASUREMENT_REQ                *
       *                                                      *
       ********************************************************/
      case MPH_NC_MEASUREMENTS_REQ:
      {
         mph_nc_measurements_req_t *message_ptr;
         gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;
        /*lint -e740*/
         message_ptr = ( mph_nc_measurements_req_t *) msg_header;
        /*lint +e740*/
         gas_id = check_gas_id(message_ptr->gas_id);
         gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
         gpl1_gprs_meas_mode_ptr->nc_mode=TRUE;

         /* Load up the data from the message */
         /* copy BA list to SC engine*/
         L1_remove_oob_ba(&ba_buffer,&message_ptr->balist);
         l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);

         MSG_GERAN_HIGH_0_G("Warning: NC_MEAS_REQ in PRACH.");
         break;
      }

      /********************************************************
       *                                                      *
       *                MPH_STOP_NC_MEASUREMENT_REQ
       *                                                      *
       ********************************************************/
      case MPH_STOP_NC_MEASUREMENTS_REQ:
      {
        mph_stop_nc_measurements_req_t *message_ptr;
        gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;

        /*lint -e740*/
        message_ptr = ( mph_stop_nc_measurements_req_t *) msg_header;
        /*lint +e740*/
        gas_id = check_gas_id(message_ptr->gas_id);
        /* Load up the data from the message */
        /* copy BA list to SC engine*/
        gpl1_gprs_meas_mode_ptr = gl1_ms_switch_meas_mode_data(gas_id);
        gpl1_gprs_meas_mode_ptr->nc_mode=FALSE;

        L1_remove_oob_ba(&ba_buffer,&message_ptr->balist);
        l1_sc_set_ba_list(&ba_buffer, 0xFF,gas_id);

        MSG_GERAN_HIGH_0_G("Warning: STOP_NC_MEAS_REQ in PRACH.");
        break;
      }

      case MPH_GPRS_RESELECTION_REQ:
      {
        mph_gprs_reselection_req_t *message_ptr;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        /*lint -e740*/
        message_ptr = ( mph_gprs_reselection_req_t *) msg_header;
        /*lint +e740*/
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        MSG_GERAN_LOW_0_G("MPH_GPRS_RESELECTION_REQ");

        {
          gpl1_gprs_setup_RESELECTION_GPRS( msg_header );
          if (message_ptr->sync_flag)
          {
            GL1_ISR_SAVE_LOCK(gas_id);
            l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
            L1_send_ISRTIM_ACQ_REQ(message_ptr->gprs_surround_data.bcch_params.bcch_arfcn, gas_id);
            GL1_ISR_SAVE_UNLOCK(gas_id);

          }
          else
          {
            GL1_ISR_SAVE_LOCK(gas_id);
            l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
            l1_tskisr_blk->command = L1_DECODE_BCCH_COMMAND;
            GL1_ISR_SAVE_UNLOCK(gas_id);
          }

        }


    /* Use default parameters for WCDMA scheduling when reselecting to a new cell
     * default params => 15 (never schedule measurements)
     *                => 7  (Ec\No threshold for UTRAN reselection, -12dB)
     * These parameters will be later updated by RR through the
     * MPH_WCDMA_CELL_UPDATE_LIST_REQ after reading the SI2quater info
     */
        /*gsm_l1_set_ecio_rssi_levels(15,7,gas_id);*/
        l1_sc_gsm_l1_set_ecio_rssi_levels(15,15,7,gas_id);

        break;
      }


      /********************************************************
       *                                                      *
       *                MPH_GPRS_L1_PARAM_UPDATE_REQ          *
       *                                                      *
       ********************************************************/
      case MPH_GPRS_L1PARAM_UPDATE_REQ:
      {
        MSG_GERAN_MED_0("*** MPH_GPRS_L1PARAM_UPDATE_REQ ***");

        /*  Load the parameters into the pending store  */
        gpl1_gprs_L1_params_pending(msg_header);

        break;
      } /* end case MPH_GPRS_L1PARAM_UPDATE_REQ */

      default:
      {
        MSG_GERAN_ERROR_1("INVALID MS_RR_L1 message %d", msg_header->message_id);
      }
      break;

    } /* end switch */
  } /* end if MS_RR_L1 */
}

void sngl_blck_move_to_boundary(uint32 *st_ptr)
{

  switch  ((*st_ptr)%13)
  {
    case 0:
    case 4:
    case 8:
      /*  If already at start do not modify */

      break;

    case 12:
      /*  If a PTCCH / IDLE slot do not modify, will be take care of later */

      break;

    case 1:
    case 5:
    case 9:
      /*  Move to next boundary by incrementing the appropriate number of
          frames, may move it to PTCCH / IDLE slot, if so this will be adjusted
          later.  */
      (*st_ptr) += 3;

      break;

    case 2:
    case 6:
    case 10:
      /*  Move to next boundary by incrementing the appropriate number of
          frames, may move it to PTCCH / IDLE slot, if so this will be adjusted
          later.  */
      (*st_ptr) += 2;

      break;

    case 3:
    case 7:
    case 11:
      /*  Move to next boundary by incrementing the appropriate number of
          frames, may move it to PTCCH / IDLE slot, if so this will be adjusted
          later.  */
      (*st_ptr)++;

      break;

    default:
      break;

  }

  /* check if starting time clashes with a PTCCH or IDLE frame */
  if(CHECK_FOR_PTCCH_IDLE(*st_ptr))
  {
    (*st_ptr)++;
  }

}



