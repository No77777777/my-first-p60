/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            L 1   A C C E S S   M O D E   T A S K  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events while the L1
   task is in the RANDOM ACCESS mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_random_access_mode
    Procedure to handle external events while the L1 task is in the
    RANDOM ACCESS mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_rach_if.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
29/01/18   cws       CR2006110 Install tx handler if access req is received in access init state after GTA in access mode
09/02/16   mk        CR2002074 Add LB_MS_TXPWR_MAX_CCH for Tx power level initialization
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
29/03/16   mn        CR995137 Access tune away support: Allowing other tech's paging at GPRS access
26/10/16   sp        CR1081225: Ignore unlock event in PS RACH state if MM Non drx state is false.
03/03/15   sk        CR983614 Missing handling of MPH_DECODE_BCCH_LIST_ABORT_REQ incorporated in L1_handle_random_access_mode()
27/02/16   sk        CR981473 Correction of handling DS ABORT INDICATION message, gpl1_gprs_handle_ds_abort_ind() called
02/09/15   nm        CR897078 Change the reason from Access to TRAFFIC if RACH for CS is received
17/08/15   nm        CR886907 Reset GTA suspension FN if CS RACH req received while already in PS access
06/07/15   nk        CR866473 handle DS ABORT INDICATION message in NULL mode and RACH mode.
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
04/05/15   mko       CR832042 Don't update the RA delay for single sim mode as TA during RACH is only applicable for multi-sim
28/04/15   mko       CR828316 Enable G to TD tuneaway during Rach process by default
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
08/04/15   pjr       CR818535 FR25951 Changes to support Single Voice Session 
30/03/15   mc        CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
30/03/15   nm        CR765891 Allowing TA for TDS during PS RACH
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
07/10/14   pa        CR733749:Handling of MPH_DSC_THRESHOLD_IND in the RACH state.
28/05/14   ap        CR671971: Change SGLTE mode on reception of UE_MODE_CHANGE_REQ
22/05/14   nk        CR667961 Make ab_tsc crash code FTM_MODE proof
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
01/05/14   nk        CR656822 Debug prints/crash to catch when ab_tsc is greater than 2
12/02/14   am        CR611265 RACH timing update to GARB
24/01/14   cah       CR598552: enable sleep for DSDS GSTMR+VSTMR
04/09/13   nk        CR537601 RACH Failure occured
18/11/13   pjr       CR563429 Fixed vocoder handover on sub1 while sub 2 was in a call
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
25/06/13   cs        Major Triton TSTS Syncup
21/05/13   cs        TSTS Updates
09/04/13   pjr       CR470217 Ported next_params.L1Data.pDedi_data asignments from DIME
17/08/12   ip        CR384421 Disable GSM sleep during CCO activity
19/03/11   rc        CR:344697 Featurized code under FEATURE_MODEM_NO_VOICE_SUPPORT to support no voice builds
13/03/12   ws        CR335057 - fixed bug in last checkin
13/03/12   ws        CR335057 eSACCH buffer flush and restore after inter-cell handovers
19/08/10   cs        CR251983 Add check for zero tx_integer and avoid mod operation
06/08/10   og        CR249828. Lock the mutex when tasking the message layer to
                     transmit and abort rach.
14/06/10   ap        CR: 235190 - 3GPP: Enhanced control of maximum output power in
                     a common BCCH cell
31/07/09   cs        CR190648 Calculate the random RACH params correctly
30/03/09   ws        Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                     to replace FEATURE_GSM_GPRS_MUSTANG
28/07/08   hv        Added KxMutex support
20/03/08   agv       Fixed the LCU PMIC Band setup for a Channel Change.
19/02/08   og        Handle the case of power scan request containing a zero list.
24/01/08   dv        CR124527:Added L1 part of FEATURE_GPRS_FAST_RACH
06/11/07   sjw       CR115620 Removed use of FEATURE_INTER_RAT_HO_OPT around
                     gl1_msg_vocoder_released(), required for Worldphone targets
05/07/07   cs        CR 120089 Reset the cipher flag in IMMED_ASSIGN for TC 26.8.1.2.3.2
06/06/07   ws        CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                     is called when we reselect to PBCCH cell stopping NC2 reporting
17/07/06   ws        Ensure we always setup EGPRS ra content if present
14/07/06   dv        Fix for CR96966. Had to take care not to break CR86620
01/31/06   ip        CR86620 added RACH intialisation if RACH type chnages
12/14/05   sv        Support the changed prototype for MPH_DECODE_BCCH_LIST_CNF.
05/05/05   nt        Added handling of MPH_STOP_NC_MEASUREMENTS_REQ from GRR.
04/12/05   dp        Fix to reset vocode flags on transition to dedicated mode
03/11/05   sv        Mainlined FEATURE_GSM_CB
01/07/05   dp        Moved sending MPH_IMMED_ASSIGN_CNF to ISR toprevent race
                     condition with RR
08/10/04   ws        EGPRS - Fixed RACH data contents bug
07/29/04   ws        Changed EGPRS feature switch to FEATURE_GSM_EGPRS
07/21/04   bk        Merged in changes to RACH to support EGPRS
06/09/04   gfr       Remove obselete calls to gl1_msg_store_tx_lev() and enable
                     slew rate limiting for dedicated channel AGC.
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
04/05/04   dp        Fix to handle a null decode bcch list and just return
                     a decode bcch list cnf to RR.  Fix for L1 getting stuck
                     in "Access abort"
03/31/04   gw        Added handling of messages from CB task.
02/25/04   gw        Lint clean up.
02/02/04   kf        Added fiels to L1_send_MPH_READ_SYS_INFO_FAILURE_IND.
01/09/04   gw        Merged in support for new SI reading API from branch.
11/05/03   gfr       Support for quad-band.
10/09/03   kf        Added handling of MPH_WCDMA_CELL_UPDATE_LIST_REQ.
06/19/03   pjr       Added MPH_DECODE_BCCH_LIST_REQ handler.
06/06/03   tb        Support for dedicated mode gtow searching.
04/28/03   tb        Support for MPH_UE_MODE_CHANGE_REQ from RR.
02/21/03   gw        Added call to L1_setup_DEACTIVATE() to process
                     MPH_STOP_GSM_MODE_REQ.  This function contains support for
                     deep sleep.
12/13/02   ws        Fix to allow GPRS RR->L1 messages to be handled correctly.
                     Also added prototype for gpl1_gprs_handle_random_access_mode()
10/15/02   dp        Fix to set the ordered_timing_advance to that specified
                     in the MPH_IMMED_ASSIGN_REQ.  Defaults to earlier
                     value otherwise
10/08/02   ATM       FEATURE_MULTIMODE_GSM is mainlined - removed flag per jchao
08/29/02   JC        Changed usage of gl1_msg_tx_rach() for multi-band
                     operation.
08/23/02   plm       Removed declaration of local variable new_serving_cell_info
                     in the function L1_handle_random_access_mode() which was
                     declared but not used to eliminate compiler warning.
                     Added extern void L1_setup_EARLY_CAMPING
                     ( IMH_T *msg_header ); to eliminate compiler warning.
08/12/02   gw        Changed RR-L1 i/f for pscan as part of cell selection /
                     re-selection optimizations.
06/27/02   dp        Added support for early camping.  Added support for
                     handling SCH decodes during reselection
05/30/02   pjr/mk    Added PL1 GPRS code hooks under switch FEATURE_GSM_GPRS_L1.
05/20/02   mk        Unified StartIdle and Reselection PL1 I/f.
05/17/02   mk        Unified PowerScan and Acquisition PL1 I/f.
05/15/02   mk        Added INTLOCK()/INTFREE() around L1_FIND_BCCH_COMMAND.
05/10/02   tb        Added FEATURE_MULTIMODE_GSM support.
04/29/02   gw        Added error messages to indicate when messages to L1 are
                     ignored.
04/17/02   JC        For MPH_SELECT_SPECIFIC_BCCH_REQ primitive command an
                     Acq only instead of (powerscan+ Acq).
03/27/02   JC        Modifications due to new power measure message layer
                     API changes.
03/20/02   dp        Cleaned up some redundant TA and power level variables
03/06/02   dp        Support to frequency hop access bursts
02/22/02   JC        Removed legacy call to L1_start_idle_reporting_timer.
02/27/02   gw        Changed order of SCE intializations when transitioning to
                     Idle mode.
02/12/02   thh       Remove HW/frame delay from ra_delay.  This reponsibility
                     belongs in the lower layer(s).
                     Vslick-beautify the whole module.
                     Updated copyright date.
02/07/02   gw        Improved starting and stopping of SCE.
02/05/02   JC        Added support for AGC.
01/24/02   ATM       Adding Calls to log paging_data.
09/20/01   jc        Added message layer support for ta, tx_lev.
08/31/01   JC        Cleanup of compile switch and added std QCT fcn template.
07/26/01   JC        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"
/* Alphabetical Order */
#include <stdlib.h>            /* rand(), RAND_MAX */

#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1_sc_int.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "ms.h"
#include "sys_stru.h"
#include "l1_log.h"
#include "geran_msgs.h"
#include "l1_smscb.h"
#include "cbl1if.h"
#include "secapi.h"
#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "geran_dual_sim_g.h"
/* For garb_intf_notify_access_update */
#include "gl1_arbitrator_interface.h"
#include "gpl1_gprs_task.h"

extern void rach_callback( uint16 content, gas_id_t gas_id
#ifdef FEATURE_GSM_COEX_RACH_DELAY
    , boolean yield
#endif
);

boolean            re_calculate_initial_rach[NUM_GERAN_DATA_SPACES];
gl1_msg_rach_type  initial_rach_type[NUM_GERAN_DATA_SPACES];

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
extern dedicated_data_T l1_dedicated_data[];

/*===========================================================================

FUNCTION  L1_handle_random_access_mode

DESCRIPTION
  This function handles the L1 Task in random access mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_random_access_mode(IMH_T *msg_header)
{
  gas_id_t gas_id;
  /* Locals */
  inter_task_BCCH_allocation_T  ba_buffer;

  /* This function interprets all messages in this mode*/

  /********************************************************
   *                                                      *
   *                MPH_POWER_SCAN_REQ                    *
   *                                                      *
   ********************************************************/

  if (msg_header->message_set == MS_RR_L1) {
    switch ( msg_header->message_id ) {
      /* Enable Sleep indication */
      case MPH_ENABLE_SLEEP_IND:
      {
      L1_handle_ENABLE_SLEEP_IND ( msg_header );
          break;
        }

      case  MPH_POWER_SCAN_REQ:
        {
          mph_power_scan_req_T *message_ptr;
          message_ptr = (mph_power_scan_req_T *)msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);
           if( FALSE == L1_setup_PSCAN( msg_header ) )
           {
             GL1_ISR_LOCK(gas_id);
              /* Command the  the ISR to start measuring */
             L1_send_ISRTIM_SCAN_REQ(L1_FIND_BCCH_COMMAND,gas_id);
             GL1_ISR_UNLOCK(gas_id);
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
         *            MPH_SELECT_SPECIFIC_BCCH_REQ              *
         *                                                      *
         ********************************************************/
         case MPH_SELECT_SPECIFIC_BCCH_REQ:
         {
            mph_select_specific_bcch_req_T *message_ptr;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

            /* align pointers */
            message_ptr = (mph_select_specific_bcch_req_T *)msg_header;  /*lint !e740 unusual pointer cast */
            gas_id = check_gas_id(message_ptr->gas_id);
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            GL1_ISR_LOCK(gas_id);
            l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
            L1_send_ISRTIM_ACQ_REQ(message_ptr->specific_channel_no, gas_id);
            GL1_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_COEX_SW_CXM
            garb_intf_notify_find_bcch_update_arfcn(message_ptr->specific_channel_no,gas_id);
            garb_intf_notify_find_bcch_update_rx_power(message_ptr->specific_channel_no,
              (int32)(l1_tskisr_blk->next_params.L1Data.pAgc_data->pwr_dBm_x16),gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_START_IDLE_REQ                    *
         *                                                      *
         ********************************************************/


      case MPH_START_IDLE_REQ:
        {
          mph_start_idle_req_T* message_ptr = ( mph_start_idle_req_T* )msg_header;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

          gas_id = check_gas_id(message_ptr->gas_id);
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];

          L1_setup_START_IDLE( msg_header );

          /* Change the ISR mode */
          l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

          (void)l1_log_paging_data( gas_id);
          break;
        }


        /********************************************************
         *                                                      *
         *                MPH_EARLY_CAMPING                     *
         *                                                      *
         ********************************************************/

      case MPH_EARLY_CAMPING_REQ:
        {
          mph_early_camping_req_T* message_ptr = ( mph_early_camping_req_T* )msg_header;
          volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

          gas_id = check_gas_id(message_ptr->gas_id);
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          L1_setup_EARLY_CAMPING( msg_header );

          /* Change the ISR mode */
          l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

          (void)l1_log_paging_data( gas_id);

          break;
        }


        /********************************************************
         *                                                      *
         *                MPH_READ_SYS_INFO_REQ                 *
         *                                                      *
         ********************************************************/

      case MPH_READ_SYS_INFO_REQ:
        {
            mph_read_sys_info_req_T* message_ptr = ( mph_read_sys_info_req_T* )msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          L1_send_MPH_READ_SYS_INFO_FAILURE_IND(FALSE, FALSE, gas_id);
           break;
        }

        /********************************************************
         *                                                      *
         *            MPH_DECODE_BCCH_LIST_REQ                  *
         *                                                      *
         ********************************************************/
      case MPH_DECODE_BCCH_LIST_REQ:
        {
             mph_decode_bcch_list_req_T  *message_ptr;
             volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
             BCCH_List_T *l1_BCCH_List_ptr;

             /* Align pointers */
             message_ptr = ( mph_decode_bcch_list_req_T *)msg_header; /*lint !e740 unusual pointer cast */
             gas_id = check_gas_id(message_ptr->gas_id);
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

                GL1_ISR_LOCK(gas_id);
                l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
                l1_tskisr_blk->command = L1_ACQUIRE_BCCH_LIST_COMMAND;
                L1_send_ISRTIM_ACQ_LIST_REQ( l1_BCCH_List_ptr->current_sch_index,gas_id );
                GL1_ISR_UNLOCK(gas_id);
              }
            break;
        }

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
          l1_idle_data = &l1_idle_data_store[gas_id];
          l1_tskisr_blk = &l1_tsk_buffer[gas_id];
          l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];

          /* Clear out ERSACCH vars*/
          l1_dedicated_data_ptr->handover_info.gl1_inter_handover_in_progress = FALSE;
          l1_dedicated_data_ptr->handover_info.gl1_inter_handover_fail = FALSE;

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

/* HV: exclude as per AV: PMIC signaling needed for Solo. Mustang uses RTR6285 and diff PMIC */

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


        /*******************************************************
        *                                                      *
        *                MPH_ABORT_RA_REQ                      *
        *                                                      *
        ********************************************************/

      case MPH_ABORT_RA_REQ:
        {
           mph_abort_ra_req_T  *message_ptr;
           message_ptr = ( mph_abort_ra_req_T *) msg_header; /*lint !e740 unusual pointer cast */
           gas_id = check_gas_id(message_ptr->gas_id);

          /*
           * Stop any pending RA bursts
           */

          GL1_ISR_LOCK(gas_id);

          gl1_msg_abort_rach(gas_id);
          re_calculate_initial_rach[gas_id] = TRUE;
          L1_send_MPH_ABORT_RA_CNF(gas_id, GL1_IN_RACH_MODE);

#ifdef FEATURE_GTA_DURING_PS_RACH
          {
            idle_data_T *l1_idle_data;
            l1_idle_data = &l1_idle_data_store[gas_id];

            /*reset the rach for ps service flag and re populate it from the message ptr*/
            l1_idle_data->random_access_data.updated_ra_delay = RACH_DELAY_INVALID;
            MSG_GERAN_MED_0_G("GTA_PS_RACH : MPH_ABORT_RA_REQ mark updated delay invalid");
          }
#endif /*FEATURE_GTA_DURING_PS_RACH*/

          GL1_ISR_UNLOCK(gas_id);

          break;

        }

        /********************************************************
        *                                                       *
        *                MPH_UE_MODE_CHANGE_REQ                 *
        *                                                       *
        ********************************************************/

      case MPH_UE_MODE_CHANGE_REQ:
        {
           mph_ue_mode_change_req_T  *message_ptr;
           message_ptr = ( mph_ue_mode_change_req_T *) msg_header; /*lint !e740 unusual pointer cast */
           gas_id = check_gas_id(message_ptr->gas_id);
#ifdef FEATURE_SGLTE
           /* Set SGLTE mode */
           gl1_hw_set_sglte_mode(message_ptr->sglte_mode_active, gas_id);
#endif
           l1_sc_wcdma_set_mode(message_ptr->gsm_only,gas_id);
           break;
        }

        /********************************************************
        *                                                       *
        *                MPH_STOP_GSM_MODE_REQ                  *
        *                                                       *
        ********************************************************/

      case MPH_STOP_GSM_MODE_REQ:
        {
          /* Stop L1 ISR */
          L1_setup_DEACTIVATE( msg_header );
          break;
        }

         /********************************************************
         *                                                       *
         *                MPH_WCDMA_CELL_UPDATE_LIST_REQ         *
         *                                                       *
         ********************************************************/

      case MPH_WCDMA_CELL_UPDATE_LIST_REQ:
        {
          mph_wcdma_cell_update_list_req_T* msg;
          msg = ( mph_wcdma_cell_update_list_req_T* )msg_header; /*lint !e740 unusual pointer cast */
          l1_sc_set_wcdma_list_update_inprogress( TRUE , msg->gas_id );
          l1_sc_wcdma_update_list( msg, msg->gas_id );
          l1_sc_set_wcdma_list_update_inprogress( FALSE , msg->gas_id );

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
        *                                                       *
        *                MPH_STOP_NC_MEASUREMENT_REQ            *
        *                                                       *
        ********************************************************/
      case MPH_STOP_NC_MEASUREMENTS_REQ:
        {
          mph_stop_nc_measurements_req_t *message_ptr;
          gpl1_gprs_meas_mode_T *gpl1_gprs_meas_mode_ptr;

          message_ptr = ( mph_stop_nc_measurements_req_t *) msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);

          gpl1_gprs_meas_mode_ptr =
               gl1_ms_switch_meas_mode_data(gas_id);

          gpl1_gprs_meas_mode_ptr->nc_mode = FALSE;

          MSG_GERAN_LOW_0_G("MPH_STOP_NC_MEASUREMENTS_REQ");

          break;
        }

      /********************************************************
        *                                                     *
        *                MPH_DSC_THRESHOLD_IND                *
        *                                                     *
        ********************************************************/
      case MPH_DSC_THRESHOLD_IND:
        {
          /* RR should send MPH_DSC_THRESHOLD_IND with L2_RACH channel type.
             If not, some problem with L1 and RR, flash an error */
          mph_dsc_threshold_ind_T *message_ptr;
          message_ptr = (mph_dsc_threshold_ind_T *)msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);
          if (L2_RACH != message_ptr->l2_channel_type) 
          {
            MSG_GERAN_ERROR_1_G("Invalid MPH_DSC_THRESHOLD_IND from RR channel %d", 
                                message_ptr->l2_channel_type); 
          }
          break;
        }

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
      /********************************************************
      *                                                       *
      *           MPH_REDUCED_NCELL_MEAS_IND                  *
      *                                                       *
      ********************************************************/
      case MPH_REDUCED_NCELL_MEAS_IND:
        {
          mph_reduced_ncell_meas_ind_T *message_ptr = (mph_reduced_ncell_meas_ind_T *) msg_header;
          gas_id = check_gas_id(message_ptr->gas_id);  
          l1_sc_config_reduced_ncell_list_rr( message_ptr->reduced_ncell_list, gas_id );
          break;
        }
#endif
      case MPH_L1_DS_ABORT_IND:
      {
           gpl1_gprs_handle_ds_abort_ind((mph_l1_ds_abort_ind_T *)msg_header);
           break;
      }

      case MPH_DECODE_BCCH_LIST_ABORT_REQ:
      {
         mph_decode_bcch_list_abort_req_T* message_ptr;
         gas_id_t gas_id;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         message_ptr = ( mph_decode_bcch_list_abort_req_T *)msg_header; /*lint !e740 unusual pointer cast */
         gas_id = check_gas_id(message_ptr->gas_id);
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];

         GL1_ISR_LOCK(gas_id);
         /* move back to NULL state */
         l1_tskisr_blk->main_command = L1_ENTER_NULL_COMMAND;
         /* In case ISR has already executed FIND_BCCH command & changed state, order it to send confirm */
         if((l1_tskisr_blk->l1_state == L1_FIND_BCCH)||(l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH))         
         {
              /* Indicate that a decode bcch list cnf is to be sent.
              Only sent when aborted by a MPH_DECODE_BCCH_LIST_ABORT_REQ
              */              
              l1_tskisr_blk->next_params.L1_confirm_ID = L1_DECODE_BCCH_LIST_CNF;
         }
         /* If ISR has not still executed FIND_BCCH command & is in RACH state only, send confirm from here only */
         else
         {
              L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
         }
         GL1_ISR_UNLOCK(gas_id);
         break;
         
       }

      default:

#ifdef DEBUG
        qsprintf(debug_string,
                 "Unexpected message in RA Mode\n");
        gs_fast_log_progress( GS_UNIQUE_EXCEPTION_NUMBER,
                              GS_LOG_TYPE_TASK_PROGRESS,
                              debug_string);

#endif

        /* Handle GSM GPRS RR->L1 messages */
        gpl1_gprs_handle_random_access_mode(msg_header);

        break;
    }  /* End of switch */
  }
  else if (msg_header->message_set == MS_L1_L1) {
      /****************************************************************
           *                                                                *
           *                l1_isr_gstmr_steal_resp_T                                *
           *                                                               *
           *****************************************************************
           */
    }
  else if (msg_header->message_set == MS_L2_L1) {
      /********************************************************
       *                                                      *
       *                PH_RANDOM_ACCESS_REQ                  *
       *                                                      *
       ********************************************************/
     if (msg_header->message_id == PH_RANDOM_ACCESS_REQ) {
        ph_random_access_req_T   *message_ptr;
        word                      delay;
        ARFCN_T                   arfcn;
        int16                     txlev;
        uint16                    ra_content;
        gl1_msg_rach_cb_type      cb;
        idle_data_T               *l1_idle_data;
        volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
        trm_reason_enum_t         reason;
        sys_proc_type_e_type      sub_reason;
        grm_unlock_callback_t     unlock_callback;


        message_ptr = ( ph_random_access_req_T *) msg_header; /*lint !e740 unusual pointer cast */
        gas_id = check_gas_id(message_ptr->gas_id);
        l1_idle_data = &l1_idle_data_store[gas_id];
        l1_tskisr_blk = &l1_tsk_buffer[gas_id];

        /*
         * L1_calculate_RA_delay returns the number of RACH slots to be left
         * unused, so the delay to the next RA burst is this value + 1
         */

        if (re_calculate_initial_rach[gas_id])
        {
            /*Check to see whether we need to calculate RACH delay(PS HO enhancement)*/
            if (message_ptr->l2_channel_type == L2_RACH)
            {
               delay =
               L1_calculate_RA_first_delay(l1_idle_data->campedon_cell_data.TX_integer);
            }

            else
            {
               delay = 0;
               MSG_GERAN_MED_2_G("Fast RACH requested - zero delay used FN=%d, chan_type = %d",
                     GSTMR_GET_FN_GERAN( gas_id),message_ptr->l2_channel_type);
            }
            re_calculate_initial_rach[gas_id] = FALSE;
        }
        else
        {
          if (l1_tskisr_blk->sub_state == L1_ACCESS_INIT)
          {
             /*Check to see whether we need to calculate RACH delay(PS HO enhancement)*/
                if (message_ptr->l2_channel_type == L2_RACH)
                {
                  delay =
                    L1_calculate_RA_first_delay(l1_idle_data->campedon_cell_data.TX_integer);
                }

                else
                {
                  delay = 0;
                  MSG_GERAN_MED_2_G("Fast RACH requested - zero delay used FN=%d, chan_type = %d",
                     GSTMR_GET_FN_GERAN( gas_id),message_ptr->l2_channel_type);
                }
          }
          else
          {
             delay =
                L1_calculate_RA_delay(l1_idle_data->campedon_cell_data.TX_integer,
                                 l1_idle_data->campedon_cell_data.
                                 BS_CCCH_SDCCH_COMB);
          }
        }

        cb.data_cb  = rach_callback;
        cb.abort_cb = NULL;

        arfcn = l1_idle_data->campedon_cell_ARFCN;

        txlev = l1_idle_data->campedon_cell_data.MS_TXPWR_MAX_CCH;

        /* Extract RACH content to transmit on 8 bit access burst */
        ra_content = message_ptr->random_access_content;

        GL1_ISR_LOCK(gas_id);
#ifdef FEATURE_GTA_DURING_PS_RACH
        /*reset the rach for ps service flag and re populate it from the message ptr*/
        l1_idle_data->random_access_data.rach_for_ps_service = message_ptr->rach_for_ps_traffic;

        /*mark suspension fn as invalid if this is for CS */
        if(l1_idle_data->random_access_data.rach_for_ps_service == FALSE)
        {
          l1_idle_data->random_access_data.suspension_FN_GTA = L1_GPRS_INVALID_FN;
        }

        MSG_GERAN_MED_2_G("GTA_PS_RACH : rach_for_ps_service %d susp FN %d ", 
                           l1_idle_data->random_access_data.rach_for_ps_service, 
                           l1_idle_data->random_access_data.suspension_FN_GTA);

#endif /*FEATURE_GTA_DURING_PS_RACH*/

        /* determine the TRM parameters for this new RACH */
        if(message_ptr->rach_for_ps_traffic == FALSE)
        {
          /* retrieve the reason and sub-reason to use for this CS RACH */
          gpl1_determine_rach_reasons(GRM_CS_CONNECTION, &reason, &sub_reason, gas_id);
         
          if (reason == TRM_TRAFFIC)
          {
            unlock_callback = (grm_unlock_callback_t)&grm_cs_traffic_abort_callback;
          }
          else
          {
            unlock_callback = (grm_unlock_callback_t)&grm_cs_access_abort_callback;
          }
          
          grm_change_priority(l1_tskisr_blk->client_id, 
                              reason,
                              sub_reason, 
                              gas_id);
          grm_retain_lock( l1_tskisr_blk->client_id,
                             unlock_callback,
                             gas_id );
        }

        /* do we need to send a EGPRS Packet channel request ? */
        if(message_ptr->egprs_ra_content_present)
        {
           /* only call cfg function if RACH type different
            * from original configuration
            */
           if ( GL1_MSG_RACH_EXTENDED != initial_rach_type[gas_id] )
           {
             if ( ftm_get_mode() != FTM_MODE )
             {
               if(message_ptr->egprs_tsc > 2)
               {
#ifdef FEATURE_QSH_MDUMP
                 QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
                 ERR_GERAN_FATAL_1_G("RACH DATA:: egprs_tsc: %d", message_ptr->egprs_tsc);
               }
             }
             gl1_msg_cfg_ul_rach(message_ptr->egprs_tsc,
                                 l1_idle_data->campedon_cell_BSIC,
                                 GL1_MSG_RACH_EXTENDED, &cb,gas_id);
           }

           /* Extract RACH content to transmit on 11 bit access burst */
           ra_content = message_ptr->egprs_ra_content;
        }
        else
        {
           if ( GL1_MSG_RACH_NORMAL != initial_rach_type[gas_id] )
           {
               gl1_msg_cfg_ul_rach(NULL,l1_idle_data->campedon_cell_BSIC,
                                   GL1_MSG_RACH_NORMAL, &cb,gas_id);
           }
        }
        if (l1_tskisr_blk->sub_state == L1_ACCESS_RUNNING 
#ifdef FEATURE_GTA_DURING_PS_RACH
          ||
            (l1_tskisr_blk->sub_state == L1_ACCESS_GTA_SUSPENDING) ||
            (l1_tskisr_blk->sub_state == L1_ACCESS_GTA_TRM_WAIT) ||
            (l1_tskisr_blk->sub_state == L1_ACCESS_GTA_FW_LOAD_WAIT) ||
            (l1_tskisr_blk->sub_state == L1_ACCESS_INIT)
#endif /*FEATURE_GTA_DURING_PS_RACH*/
           )
        {
#ifdef FEATURE_GTA_DURING_PS_RACH

            l1_idle_data->campedon_cell_ARFCN = arfcn;
            l1_idle_data->random_access_data.txlev = txlev;
            l1_idle_data->random_access_data.ra_delay = delay;
            l1_idle_data->random_access_data.ra_content = ra_content;
            l1_idle_data->random_access_data.updated_ra_delay = delay;

          /*Check for multisim standby mode & not G only active sub and suspension FN to be valid*/
          if(CHECK_IF_GTA_ALLOWED_IN_PS_RACH() && l1_idle_data->non_updated_lai_in_nmo1 == FALSE &&
             l1_idle_data->random_access_data.suspension_FN_GTA != L1_GPRS_INVALID_FN)
          {
              uint32 new_rach_delay =0;
              boolean is_rach_after_gap = FALSE;

              /*if collision*/
              if(TRUE == gl1_check_if_rach_delay_collides_with_gap(delay,
                                                                    l1_idle_data->random_access_data.suspension_FN_GTA,
                                                                    5,/*margin in the beginning*/
                                                                    G2X_PAGE_DUR_FRAMES,
                                                                    &new_rach_delay,
                                                                    &is_rach_after_gap,
                                                                    gas_id))
              {
                l1_idle_data->random_access_data.updated_ra_delay = new_rach_delay;
              }
              /*if rach is after gap*/
              else if (is_rach_after_gap == TRUE)
              {
                l1_idle_data->random_access_data.updated_ra_delay = delay;
              }
              else
              {
                /*this means this RACH fn is before gap & will get processed before the gap itself, no need to store this delay*/
                //l1_idle_data->random_access_data.updated_ra_delay = RACH_DELAY_INVALID;

#ifdef FEATURE_GSM_COEX_SW_CXM
                /* new delay is calculated, update garb with this */
                garb_intf_notify_access_update(arfcn,delay,txlev,gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */
                gl1_msg_tx_rach ( arfcn,
                                  txlev,
                                  delay,
                                  ra_content,
                                  gas_id);

              }
              
              MSG_GERAN_HIGH_3_G("GTA_PS_RACH : RACH_REQ received in access state %d, delay %d updated delay %d", 
                                  l1_tskisr_blk->sub_state, delay, l1_idle_data->random_access_data.updated_ra_delay);
          }
          else
#endif /*FEATURE_GTA_DURING_PS_RACH*/
        
          {
          
#ifdef FEATURE_GSM_COEX_SW_CXM
            /* new delay is calculated, update garb with this */
            garb_intf_notify_access_update(arfcn,delay,txlev,gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */
            gl1_msg_tx_rach ( arfcn,
                              txlev,
                              delay,
                              ra_content,
                              gas_id);
          }
        }

        l1_tskisr_blk->main_command = L1_NULL_COMMAND;

        GL1_ISR_UNLOCK(gas_id);
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
       L1_send_PH_READY_TO_SEND_IND(L2_RACH, gas_id);
     }
    else
    {
        MSG_GERAN_ERROR_1("L2->L1(RACH) message %d ignored.", msg_header->message_id);
     }
  }
  /*******************************************************
  *                                                      *
  *                   GSM CB-L1 messages                 *
  *                                                      *
  ********************************************************/
  else if ( msg_header->message_set == MS_CB_L1)
  {
     if (msg_header->message_id == CB_SCHEDULING_REQ)  /*lint !e641 */
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
  else if ( (msg_header->message_set == MS_MAC_L1) || (msg_header->message_set == MS_RLC_L1) )
  {
     gpl1_gprs_handle_random_access_mode(msg_header);
  }
  else
  {
      MSG_GERAN_ERROR_0("L1(RACH) - Unknown message set.");
  }
}


/*******************************************************************************
 *
 *  Function name:  L1_calculate_RA_first_delay
 *  ------------------------------------
 *
 *  Description:
 *  ------------
 *  This function calculates the delay between the initiation of the immediate
 *  assignment procedure and the first channel request. See GSM 04.08 section 3.3.1
 *
 *  Parameters:
 *  -----------
 *  Tx integer
 *
 *  Returns:
 *  --------
 *  Delay ( in frames )
 *
 ******************************************************************************/
byte  L1_calculate_RA_first_delay( byte tx_integer )
{
  /* Locals */
  uint32 rand_integer;
  uint8 rand_array[sizeof(uint32)];
  uint32 rn;

  if (secapi_get_random(SECAPI_SECURE_RANDOM, rand_array, sizeof(uint32)) == E_SUCCESS)
  {
    rand_integer = (((uint32)rand_array[0] << 24) | ((uint32)rand_array[1] << 16) | ((uint32)rand_array[2] << 8) | (uint32)rand_array[3]);
  }
  else
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0("ERROR secapi_get_random() returned FAILURE");
  }

  /* set tx integer to MAX(tx_integer, 8) */
  if ( tx_integer < 8 ) {
    tx_integer = 8;
  }

  /* Get rand from 0 to tx_integer - 1 */
  rn = ( rand_integer % tx_integer );

  return((byte)rn);
}

/*******************************************************************************
 *
 *  Function name:  L1_calculate_RA_delay
 *  ------------------------------------
 *
 *  Description:
 *  ------------
 *  This function calculates the random delay (in frames) between
 *  bursts on the RACH. see GSM 04.08 section 3.3.1
 *
 *  Parameters:
 *  -----------
 *  Tx integer, BS_CCCH_SDCCH_COMB,
 *
 *  Returns:
 *  --------
 *  the delay between the last burst and the next
 *
 ******************************************************************************/
word L1_calculate_RA_delay( byte tx_integer, boolean BS_CCCH_SDCCH_COMB )
{
  /* Locals */
  uint32 rand_integer;
  uint8 rand_array[sizeof(uint32)];
  word   s;
  uint32 rn       = 0;

  if (secapi_get_random(SECAPI_SECURE_RANDOM, rand_array, sizeof(uint32)) == E_SUCCESS)
  {
    rand_integer = (((uint32)rand_array[0] << 24) | ((uint32)rand_array[1] << 16) | ((uint32)rand_array[2] << 8) | (uint32)rand_array[3]);
  }
  else
  {
#ifdef FEATURE_QSH_MDUMP
    QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
    ERR_GERAN_FATAL_0("ERROR secapi_get_random() returned FAILURE");
  }

  /* Avoid a div by zero error if invalid tx_integer of zero */
  if ( tx_integer )
  {
    /* Get rand from 0 to tx_integer - 1 */
    rn = ( rand_integer % tx_integer );
  }

  /* calculate the parameter S of section 3.3.1.2 */
  switch ( tx_integer ) {
    case 3:
    case 8:
    case 14:
    case 50:
      if (BS_CCCH_SDCCH_COMB == TRUE) {
        s = 41;
      } else {
        s = 55;
      }
      break;

    case 4:
    case 9:
    case 16:
      if (BS_CCCH_SDCCH_COMB == TRUE) {
        s = 52;
      } else {
        s = 76;
      }
      break;

    case 5:
    case 10:
    case 20:
      if (BS_CCCH_SDCCH_COMB == TRUE) {
        s = 58;
      } else {
        s = 109;
      }
      break;

    case 6:
    case 11:
    case 25:
      if (BS_CCCH_SDCCH_COMB == TRUE) {
        s = 86;
      } else {
        s = 163;
      }
      break;

    case 7:
    case 12:
    case 32:
      if (BS_CCCH_SDCCH_COMB == TRUE) {
        s = 115;
      } else {
        s = 217;
      }
      break;

    default:
      s = 0;

#ifdef DEBUG
      gs_fast_log_progress( GS_UNIQUE_EXCEPTION_NUMBER,
                            GS_LOG_TYPE_TASK_PROGRESS,
                            "Incorrect TxInteger\n");

#endif

      break;
  }

  return((word)(rn + s));
}
