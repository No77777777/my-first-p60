/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              L 1   B C C H  M O D E   T A S K  P R O C E S S I N G

GENERAL DESCRIPTION
   This module contains the procedures to handle events while the L1
   task is in the FIND_BCCH mode.  Events can originate from other
   layers or from the L1 ISR.

EXTERNALIZED FUNCTIONS
  L1_handle_find_bcch
    Procedure to handle external events while the L1 task is in the
    FIND_BCCH mode.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_bcch_if.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ---------------------------------------------------------- 
07/02/18   hd       CR2181437 Report the BCCH as found when BSIC doesn't match after SCH decode
21/04/17   br        CR1041084 FR 36219: GDET – GMSK Modulation Detector
03/04/17   br        CR2027088 Band based ASDIV support during ACQ
02/06/17   sk        CR1116408 Do not order RESCAN as a part of ASDIV probe if GL1 is indicated to abort or deactivate stack.
03/11/15   ss        CR916273 FR 29852: AsDiv in Acquisition
17/07/15   smd       CR874148 Clear variables for GTA_PS_RACH before RACH at single sim mode.
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
20/06/14   jj        CR656872 fix critical gl1 kw error  
29/07/14   ap        CR695234: Changes for GL1's New Rach interface
10/07/14   cgc       CR631007 FR21768 enhancement force G2X measurement MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ
24/07/13   ss        CR515896  Set wcdma_list_update_inprogress to TRUE in msg_handler routine 
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
23/05/13   pg        CR460555: Enable 3GPP Rel10 RACH Power Reduction feature
21/06/13   cs        Major Triton TSTS Syncup
10/06/13   cs        TSTS Updates
01/05/13   pg        CR478903: read all normal and extended BCCH blocks during 
                     MPH_SELECT_SPECIFIC_BCCH_REQ / MPH_GPRS_RESELECTION_REQ  
17/08/12   ip        CR384421 Disable GSM sleep during CCO activity
03/05/11   cja       Nikel - only send G2L init when L neighbours
17/12/10   og        Changed G2L INIT and DEINIT.
27/01/11   dmb       The following RR<->L1 messages are removed (not used):
                     MPH_ADD_CHANNEL_ASSIGN_REQ/CNF,MPH_STOP_CAMPING_REQ,MPH_STOP_READ_SYS_INFO_REQ
17/12/10   og        Changed G2L INIT and DEINIT.
30/11/10   pg        CR 266126 - Don't process ISR_SCH_CNF if L1_frequency_list ptr
                     is NULL
17/12/10   og        Changed G2L INIT and DEINIT.
22/04/09   ws        CR 235527 - Don't process ISR_SCAN_CNF if L1_frequency_list ptr
                     is NULL
16/09/09   ps        Resolve Klocwork errors
08/09/09   cs        Fix Klocwork bounds checking in sort
29/05/09   ps        Zero Initialise memory relocation to Heap for selected data structure
23/04/09   nt        Fix to prevent L1-RR lock up, when L1 performs double SCH
                     acquisition
09/10/08   cs        Allow the RX Sweep acquisition to be restarted
02/10/08   cs        Lint fixes
28/08/08   cs        Implement the Fast Rx Sweep functionality for FTM
28/07/98   hv        Added KxMutex support
28/03/08   cs        Lint fix for DEBUG_PSCAN
19/02/08   og        Handle the case of power scan request containing a zero list.
24/01/08   dv        CR124527:Added L1 part of FEATURE_GPRS_FAST_RACH
19/07/07   og        UOOS changes, reduce number of measured samples.CR119147.
16/05/07   ws        Added FEATURE_GSM_L1_CONTROL_SET_BAND CR 114613
02/03/07   cs        Add support for new CGPS timetag
19/01/07   cs        Fix Pscan F3 logging info
04/05/06   rm        Fixing CR92108, relocated sending L1_POWER_SCAN_CNF to l1_isr_find_bcch().
03/05/06   rm        CR92108, Replaced L1_NULL_MODE state change with L1_ENTER_NULL_COMMAND
                     when processing ISR_SCAN_CNF. The state change was moved to ISR level.
12/22/05   rm        Changes to MPH_GPRS_RESELECTION_REQ functionality in FIND_BCCH mode.
12/22/05   rm        Changes to MPH_GPRS_RESELECTION_REQ functionality in FIND_BCCH mode.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
08/17/05   gfr       Move NPL1 RACH function calls to ISR context
06/07/05   av        Added Power scan results in a F3 message.
06/16/05   sv        Fixed lint error due to header file include.
06/13/05   yh        Implementing qsort.
03/11/05   sv        Mainlined FEATURE_GSM_CB
10/01/04   bm        Enhanced Parallel BCCH decode support
08/13/04   ws        Corrected Lint error of preveious EGPRS change
08/03/04   sv        Changed interface to send "RESELECTION_CNF" instead of "SPECIFIC_BCCH_CNF"
                     during full acquisition on a GPRS reselection.
08/10/04   ws        Fixed bug related to EGPRS to correct data to tx in RACH
07/29/04   ws        Changed EGPRS feature switch to FEATURE_GSM_EGPRS
07/21/04   bk        Merged in changes to RACH to support EGPRS
06/12/04   gfr       Control slew rate limiting in PL1.
06/11/04   dp        Changed CCH_UNI_DWN interface to take BSIC
06/09/04   gw        Set up l1_tskisr_blk->next_params.L1Data.pIdle_data
                     correctly when processing PH_RANDOM_ACCESS_REQ.
06/04/04   sv        Assign appropriate enum value to "L1_confirm_ID".
06/03/04   gw        Send correct command to ISR when initiating list decode.
06/02/04   gw        Allow L1 to handle MPH_DECODE_BCCH_LIST_REQ after getting
                     MPH_SELECT_SPECIFIC_BCCH_REQ.
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters
                     for better code readability.
05/24/05   gw        Handle MPH_DECODE_BCCH_LIST_REQ.
03/31/04   gw        Added processing of messages from CB task.
03/26/04   gfr       Time tagging support.
02/25/04   gw        Lint clean up.
02/17/04   gfr       Pass drivers the acq index during list search.
02/04/04   dp        Added support for background HPLMN searching
01/09/04   gw        Merged in support for new SI reading API from branch.
11/05/03   gfr       Support for quad-band.
08/08/03   sw        Added support for SI13 on Extended BCCH.
28/07/03   kf        transfer of bsic information for pbcch sync.
16/07/03   kf        modified ncell pbcch reading.
07/11/03   gfr       Lint cleanup.
06/23/03   gfr       Compiler warning cleanup.
10/06/03   ws        Added PBCCH signal handler.
06/06/03   tb        Support for dedicated mode gtow searching.
05/02/03   gw        Merged in power transfer changes from branch.
04/28/03   tb        Support for MPH_UE_MODE_CHANGE_REQ from RR.
04/04/03   gw        Added calls to l1_sc_init() to make sure that ncell info is
                     cleared on certain mode transitions.
03/18/03   gw        Added condition to make task ignore
                     ISR_DECODE_BCCH_LIST_BCCH_CNF if it is already processing
                     MPH_SELECT_SPECIFIC_BCCH_REQ.
02/21/03   gw        Added call to L1_setup_DEACTIVATE() to process
                     MPH_STOP_GSM_MODE_REQ.  This function contains support for
                     deep sleep.
01/14/03   tb        Added handling of GTOW msgs from RR.
12/05/02   gw        Added support for decoding SI 2quater on extended BCCH.
11/25/02   mk        Updated GPRS, added support for FEATURE_GSM_GPRS_PCCCH.
11/13/02   mk        Updated GPRS code.
10/07/02   dp        Added support for fully parallel decodes
10/01/02   ATM       Added central get and set FN routines
09/17/02   plm       Deleted declared but unused local variable power_index from
                     function L1_handle_find_bcch() to eliminate compiler warning.
09/10/02   dp        Added support to send back MPH_DECODE_BCCH_LIST_CNF when
                     we get a MPH_DECODE_BCCH_LIST_ABORT_REQ
09/03/02   dp        Added support to handle PH_RANDOM_ACCESS_REQ in FIND_BCCH.
                     Fixed compiler warnings
08/13/02   dp        Added support for parallel acq/BCCH decodes
08/12/02   gw        Changed RR-L1 i/f for pscan as part of cell selection /
                     re-selection optimizations.
07/23/02   ATM       Moved L1_handle_null_mode() into l1_null_if.c.
07/10/02   dp        Fix to handle MPH_FIND_BCCH_REQ in FIND_BCCH state
06/27/02   dp        Added support for early camping
07/16/02   jc        Add code to check for FIND_BCCH while in FIND_BCCH state.
05/30/02   pjr/mk    Added PL1 GPRS code hooks under switch FEATURE_GSM_GPRS_L1.
05/23/02   mk        Disabled 'next' Acquisition if L1_specific_bcch flag set
05/20/02   mk        Unified StartIdle PL1 I/f.
05/17/02   mk        Unified PowerScan and Acquisition PL1 I/f.
05/15/02   mk        Added INTLOCK()/INTFREE() around L1_FIND_BCCH_COMMAND.
05/10/02   JC        Added support for new gl1_msg_cfg_power_measure API.
05/10/02   tb        Added FEATURE_MULTIMODE_GSM support.
05/06/02   JC        Added num_satisfy_threshold_criteria variable for
                     future use.
04/29/02   gw        Added error messages to indicate when messages to L1 are
                     ignored.
04/17/02   JC        For MPH_SELECT_SPECIFIC_BCCH_REQ primitive command an
                     Acq only instead of (powerscan+ Acq).
03/27/02   JC        Modifications due to new power measure message layer
                     API changes.
03/05/02   JC        Added call to L1_map_dBm_x16_to_rxlev() for RXLEV rpt.
                     Also, removed legacy L1_start_idle_reporting_timer/
                     L1_stop_idle_reporting_timer functions. Added references
                     to SYS_PARAM_MAX_FRAMES_FOR_FCCH_DETECTION, etc..
                     Removed legacy support for separate FCCH detection/
                     SCH decoding.
02/27/02   gw        Added support for MPH_SELECT_SPECIFIC_BCCH_REQ when in
                     FindBCCH mode.
02/25/02   gw        Fix from 01/28/02 was undone.  Put it back
02/12/02   thh       Seed the randomizer with the current FN the first time
                     we go to Idle.
                     Updated copyright date.
02/07/02   gw        Changes to support cleaner starting and stopping of SCE.
01/14/02   JC        Added AGC to power scan. Changed all areas where RX is
                     concerned.
01/28/02   ATM       Adding Calls to log paging_data.
01/28/02   gw        Replaced uncommented call to L1_send_ISRTIM_FIRST_FCB_REQ
                     with call to L1_send_ISRTIM_ACQ_REQ.
08/31/01   JC        Removed unnecessary compile switches & general cleanup.
04/02/01   JC        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#include "comdef.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "l1_task.h"
#include "l1_utils.h"
#include "ms.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "sys_stru.h"
#include "l1_log.h"
#include "l1_isr.h"
#include "msg.h"
#include "l1i.h"
#include "l1_smscb.h"
#include "cbl1if.h"

#include "gpl1_gprs_task.h"

#include "geran_dual_sim.h"
#include "geran_dual_sim_g.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */


#ifdef FEATURE_GL1_ASDIV_ACQ
#include "gl1_asdiv.h"
#endif

/*===========================================================================

                     PROTOTYPES FOR EXTERNAL FUNCTIONS

===========================================================================*/
extern void gpl1_gprs_setup_DECODE_PBCCH( void );

/*===========================================================================
 *                       EXTERNAL DATA
===========================================================================*/
extern boolean last_acq_was_list;

/*===========================================================================

FUNCTION  L1_HANDLE_FIND_BCCH

DESCRIPTION
  This function handles the L1 Task in find bcch mode.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_handle_find_bcch(IMH_T *msg_header)

{
  gas_id_t gas_id;
   /* Locals */


   /* function l1_handle_idle_mode starts off this mode by initiating
   a power measurement scan of surrounding cells. this terminates
   when the ISR sends a ISR_SCAN_CNF message to the L1 task. */

   if (msg_header->message_set == MS_L1_L1)
   {
      uint16 num_satisfy_threshold_criteria = 0;
      uint16 i;

      switch (msg_header->message_id)
      {

        /********************************************************
         *                                                      *
         *                ISR_SCAN_CNF                          *
         *                                                      *
         ********************************************************/

         case ISR_SCAN_CNF:
         {
           isrtim_scan_cnf_T *message_ptr;
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
           Frequency_List_T *l1_Frequency_List;

           message_ptr = (isrtim_scan_cnf_T *)msg_header;
           gas_id = check_gas_id(message_ptr->gas_id);
           l1_tskisr_blk = &l1_tsk_buffer[gas_id];

           l1_Frequency_List = l1_Frequency_List_Store[gas_id];

          /* Check L1_Frequency_List pointer is valid, since MPH_STOP_MODE_REQ
            * may have deallocated dynamic memory causing a potential page fault
            */
            if(l1_Frequency_List == NULL)
            {
              MSG_GERAN_HIGH_0_G("l1_Frequency_List NULL!");
              break;
            }

            {
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

#ifdef DEBUG_PSCAN /* Disabled this F3 printout since it is adding 30 ms to our camp on time */
#error code not present
#endif
                l1_Frequency_List->results->meas[i].arfcn = l1_Frequency_List->ARFCN[l1_Frequency_List->sorted_array_indexes[i]];
              }

              l1_Frequency_List->results->num_frequencies = num_satisfy_threshold_criteria;

              /* ... transition to Null Mode ... */
              GL1_ISR_SAVE_LOCK(gas_id);

#ifdef FEATURE_GL1_ASDIV_ACQ              
              if (gl1_asdiv_acq_enabled(gas_id) &&                         /* Asdiv in Acquisition is enabled */
                  !gl1_asdiv_acq_check_rescan(gas_id) &&                   /* A Rescan for AsDiv in Acquisition is not attempted */
                  ((num_satisfy_threshold_criteria != 0) ||                /* If Some Valid Cells are found or Freq. length is within Limits*/
                  (l1_Frequency_List->length <= GL1_ASDIV_ACQ_MAX_FREQ_LIST_LEN))&&
                  ( l1_tskisr_blk->main_command == L1_NULL_COMMAND)) 
              {
                /* change sub state to decide AsDiv for Acquisition*/
                l1_tskisr_blk->sub_state = L1_ASDIV_ACQ_SWITCH;
                gl1_asdiv_acq_update_params( GL1_ASDIV_ACQ_EV_PSCAN_END,
                                             l1_Frequency_List->results->meas[0].arfcn,
                                             num_satisfy_threshold_criteria,
                                             l1_Frequency_List->length,
                                             gas_id);

                if(0 == num_satisfy_threshold_criteria)
                {
                    L1_send_ISRTIM_SCAN_REQ( L1_FIND_BCCH_COMMAND, gas_id );
                }                                
              }
              else
              {
                if (gl1_asdiv_acq_check_rescan(gas_id)) 
                {
                  if(num_satisfy_threshold_criteria ==0)
                  {
                  gl1_trm_ant_switch_div_set_mode(TRM_CLIENT_MODE_RXACQ ,gas_id);
				  /*On Entering This State, If scenario is for ReScan then Switch*/
                  gl1_trm_ant_switch_div_set_config(gas_id);
                  }
                  gl1_asdiv_acq_reset(gas_id);
                }
#endif /*FEATURE_GL1_ASDIV_ACQ*/

              if ( l1_tskisr_blk->main_command != L1_DEACTIVATE_COMMAND)
              {
                /* ... transition to Null Mode ... */
                l1_tskisr_blk->main_command = L1_ENTER_NULL_COMMAND;
              }

#ifdef FEATURE_GL1_ASDIV_ACQ              
        }
#endif /*FEATURE_GL1_ASDIV_ACQ*/


              l1_tskisr_blk->next_params.L1_confirm_ID = L1_POWER_SCAN_CNF;

              GL1_ISR_SAVE_UNLOCK(gas_id);
            }
         }
         break;


        /********************************************************
         *                                                      *
         *                ISR_SCH_CNF                           *
         *                                                      *
         ********************************************************/

        /* The L1 ISR has found a SCH on the channel or not as indicated
         in  the ISRTIM_FIRST_FCB_CNF message to the L1 task. */

         case ISR_SCH_CNF:
         {
            l1_sch_cnf_T *msg_ptr;
            uint8         BSIC;
            idle_data_T               *l1_idle_data;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
            Frequency_List_T *l1_Frequency_List;

            msg_ptr = (l1_sch_cnf_T *)msg_header; /*lint !e740 */
            gas_id = check_gas_id(msg_ptr->gas_id);

            l1_Frequency_List = l1_Frequency_List_Store[gas_id];

            l1_idle_data = &l1_idle_data_store[gas_id];
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            /* Check L1_Frequency_List pointer is valid, since MPH_STOP_MODE_REQ
            * may have deallocated dynamic memory causing a potential page fault
            */
            if(l1_Frequency_List == NULL)
            {
              MSG_GERAN_HIGH_0_G("l1_Frequency_List NULL!");
              break;
            }


            {
              BSIC = get_BSIC_from_sch_data( msg_ptr->sch_block );
              MSG_GERAN_HIGH_3_G("ISR_SCH_CNF Req with BSIC %d, decoded-%d, Success=%d",l1_tskisr_blk->next_params.bsic_expected,BSIC,msg_ptr->success);
              if (msg_ptr->success && 
                  l1_tskisr_blk->sub_state != L1_ACQUIRING_DURING_RESELECTION &&
                  l1_tskisr_blk->x2g_resel_in_progress == FALSE)
              {
                if (l1_tskisr_blk->next_params.bsic_expected != GSM_INVALID_BSIC && 
                    BSIC != l1_tskisr_blk->next_params.bsic_expected)
                {
                  MSG_GERAN_ERROR_3_G("SCH decoded with different BSIC -- decoded:%2x, requested=%2x, substate=%d",
                                       BSIC,l1_tskisr_blk->next_params.bsic_expected,
                                       l1_tskisr_blk->sub_state);

                  msg_ptr->success = FALSE;
                }
              }
               if (msg_ptr->success)
               {
                  /* A FCH/SCH combination has been found on the given ARFCN */
                  /* Inform upper layers */
                  if (l1_tskisr_blk->sub_state == L1_ACQUIRING_DURING_RESELECTION)
                  {
                       L1_send_MPH_RESELECTION_CNF( TRUE, gas_id );
                  }
                  else
                  {
                       L1_send_MPH_SELECT_SPECIFIC_BCCH_CNF( TRUE,
                                                             msg_ptr->sch_block, gas_id );
                  }
                  /* Start decoding the CCH only */
                  l1_idle_data->campedon_cell_BSIC = BSIC;

                  l1_idle_data->campedon_cell_ARFCN = l1_Frequency_List->ARFCN[l1_Frequency_List->length];
                  l1_idle_data->campedon_cell_agc   = l1_Frequency_List->agc[l1_Frequency_List->length];
                  sys_algo_agc_enable_slew_rate_limiting(&l1_idle_data->campedon_cell_agc);

                  /* Setup default BCCH decoding mask in case we go
                   * to Idle Mode.
                   */
                  l1_idle_data->si_mask        = RR_L1_SI_TYPE_ALL_NORM | RR_L1_SI_TYPE_2_quater_EXT | RR_L1_SI_TYPE_13_EXT;
                  l1_idle_data->BCCH_mask      = gl1_compute_BCCH_mask( l1_idle_data->si_mask );
                  l1_idle_data->ext_BCCH_mask  = gl1_compute_ext_BCCH_mask( l1_idle_data->si_mask );

                  L1_send_ISRTIM_CCH_UNI_DWN_REQ( BSIC,
                                                  l1_Frequency_List->ARFCN[l1_Frequency_List->length],
                                                  l1_idle_data->ext_BCCH_mask,
                                                  &l1_idle_data->campedon_cell_agc, gas_id );
               }
               else
               {
                  if (l1_tskisr_blk->sub_state == L1_ACQUIRING_DURING_RESELECTION)
                  {
                     L1_send_MPH_RESELECTION_CNF( FALSE, gas_id );
                  }
                  else
                  {
                    /* If the command is not L1_NULL_COMMAND then L1 is re-attempting
                     the acquisition for another try. Only send the final failure confirmation
                     upon completion of all attempts, or RR may request something that cannot
                     be serviced.*/
                    if (l1_tskisr_blk->command == L1_NULL_COMMAND)
                    {
                      L1_send_MPH_SELECT_SPECIFIC_BCCH_CNF( FALSE, NULL, gas_id );
                    }
                    else
                    {
                      MSG_GERAN_HIGH_0_G("Acquisition failed awaiting retry");
                    }
                  }
               }
            }

            /* (Re)-Initialize the SCE */
            l1_sc_init(gas_id);

            break;
         }  /* End of case ISR_SCH_CNF: */

         default:
            MSG_GERAN_ERROR_1("L1->L1(BCCH) message %d ignored.", msg_header->message_id);
            break;
      } /* end of switch (msg_header->message_id) */
   } /* end of    if (msg_header->message_set == MS_L1_L1)  */

   else if (msg_header->message_set == MS_RR_L1)
   {
      switch (msg_header->message_id)
      {
        /* Enable Sleep indication */
        case MPH_ENABLE_SLEEP_IND:
        {
          L1_handle_ENABLE_SLEEP_IND ( msg_header );
          break;
        }

        /********************************************************
         *                                                      *
         *                MPH_STOP_GSM_MODE_REQ                 *
         *                                                      *
         ********************************************************/

         case MPH_STOP_GSM_MODE_REQ:

            /* Stop L1 ISR */
            L1_setup_DEACTIVATE( msg_header );
            break;


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

            l1_sc_wcdma_set_mode(message_ptr->gsm_only,gas_id);

            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_POWER_SCAN_REQ                    *
         *                                                      *
         ********************************************************/

         case MPH_POWER_SCAN_REQ:
         {
            mph_power_scan_req_T *message_ptr;
            message_ptr = (mph_power_scan_req_T *)msg_header;
            gas_id = check_gas_id(message_ptr->gas_id);

            if( FALSE == L1_setup_PSCAN( msg_header ) )
            {
              /* Command the  the ISR to start measuring */
              GL1_ISR_LOCK(gas_id);
              L1_send_ISRTIM_SCAN_REQ( L1_FIND_BCCH_COMMAND, gas_id );
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
            uint8 bsic_requested;

            sys_band_mask_type             band_mask;

            /* align pointers */
            message_ptr = (mph_select_specific_bcch_req_T *)msg_header; /*lint !e740 */
            gas_id = check_gas_id(message_ptr->gas_id);
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            bsic_requested = (message_ptr->bsic.PLMN_colour_code << 3) | message_ptr->bsic.BS_colour_code;

            /* If we are about to camp on a 1800 band cell ensure we are running at
             * low clock speed, otherwise set some other band to clock at high speed
             */
            if(message_ptr->specific_channel_no.band == SYS_BAND_DCS_1800)
            {
              band_mask = SYS_BAND_MASK_GSM_DCS_1800;
            }else
            {
              band_mask = SYS_BAND_MASK_GSM_850;
            }

            gl1_set_band_group(band_mask, gas_id);

            GL1_ISR_LOCK(gas_id);
            l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
            L1_send_ISRTIM_ACQ_REQ(message_ptr->specific_channel_no, gas_id);
            l1_tskisr_blk->next_params.bsic_expected = bsic_requested;            
            MSG_GERAN_MED_2_G("SpecificBCCH Req with ARFCN:%d, BSIC %2x",message_ptr->specific_channel_no.num, bsic_requested);
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
         ******************************************************/

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
         *        MPH_DECODE_BCCH_LIST_ABORT_REQ                *
         *                                                      *
         ****************************************************** */

         case MPH_DECODE_BCCH_LIST_ABORT_REQ:
         {
            /* Change the ISR mode */
            mph_decode_bcch_list_abort_req_T* message_ptr = ( mph_decode_bcch_list_abort_req_T* )msg_header;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

            gas_id = check_gas_id(message_ptr->gas_id);
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            GL1_ISR_LOCK(gas_id);
            l1_tskisr_blk->main_command = L1_ENTER_NULL_COMMAND;
            /*
               Indicate that a decode bcch list cnf is to be sent.
               Only sent when aborted by a MPH_DECODE_BCCH_LIST_ABORT_REQ
             */

            l1_tskisr_blk->next_params.L1_confirm_ID = L1_DECODE_BCCH_LIST_CNF;
            GL1_ISR_UNLOCK(gas_id);

            break;
         }


        /********************************************************
         *                                                      *
         *                MPH_EARLY_CAMPING                    *
         *                                                      *
         ******************************************************/

         case MPH_EARLY_CAMPING_REQ:
         {
            mph_early_camping_req_T* message_ptr = ( mph_early_camping_req_T* )msg_header;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

            gas_id = check_gas_id(message_ptr->gas_id);
            l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            L1_setup_EARLY_CAMPING( msg_header );

            /* Change the ISR mode */
            l1_tskisr_blk->main_command = L1_IDLE_COMMAND;

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
            l1_sc_wcdma_ignore_neighbors( msg );
            break;
         }

        /********************************************************
         *                                                      *
         *                MPH_WCDMA_CELL_UPDATE_LIST_REQ        *
         *                                                      *
         ********************************************************/

         case MPH_WCDMA_CELL_UPDATE_LIST_REQ:
         {
            mph_wcdma_cell_update_list_req_T* msg;
            msg = ( mph_wcdma_cell_update_list_req_T* )msg_header; /*lint !e740 */
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
         *                                                      *
         *                MPH_READ_SYS_INFO_REQ                 *
         *                                                      *
         ********************************************************/

         case MPH_READ_SYS_INFO_REQ:

         {
           mph_read_sys_info_req_T* message_ptr = ( mph_read_sys_info_req_T* )msg_header;
           idle_data_T               *l1_idle_data;
           volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

           gas_id = check_gas_id(message_ptr->gas_id);
           l1_idle_data=&l1_idle_data_store[gas_id];
           l1_tskisr_blk = &l1_tsk_buffer[gas_id];

            /* While L1 is in FindBCCH mode it will ignore the
             * mask RR sends for the normal BCCH and get all
             * normal BCCH blocks.  It will use the mask RR
             * provides on the extended BCCH.  Also, L1 will
             * remember the mask and apply it in Idle Mode.
             */
            L1_process_READ_SYS_INFO_REQ( (mph_read_sys_info_req_T *)msg_header ); /*lint !e740 */

            l1_tskisr_blk->current_params.extended_BCCH_mask = l1_idle_data->ext_BCCH_mask;
            break;
         }

         /********************************************************
          *                                                      *
         *            MPH_DECODE_BCCH_LIST_REQ                  *
         *                                                      *
         ********************************************************/
         case MPH_DECODE_BCCH_LIST_REQ:
         {
            mph_decode_bcch_list_req_T* message_ptr = ( mph_decode_bcch_list_req_T* )msg_header;
            volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
            BCCH_List_T *l1_BCCH_List_ptr;
            gas_id_t gas_id = check_gas_id(message_ptr->gas_id);

            l1_tskisr_blk = &l1_tsk_buffer[gas_id];
            l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];

            /* Only process in L1_GETTING_BCCH sub state to avoid
             * this list decode interrupting a previous one that's
             * still in progress.
             */
            if ( (l1_tskisr_blk->sub_state == L1_GETTING_BCCH) ||
                 ( (l1_tskisr_blk->sub_state     == L1_ACQUIRING) &&
                   (l1_tskisr_blk->sub_sub_state == L1_ACQ_END)   &&
                   (last_acq_was_list            == FALSE)
                 )
               )
            {
               mph_decode_bcch_list_req_T  *message_ptr;

               /* Align pointers */
               message_ptr = ( mph_decode_bcch_list_req_T *)msg_header; /*lint !e740 */
               gas_id = check_gas_id(message_ptr->gas_id);

               /* If RR sends a null list, just send back a cnf and do nothing */
               if ( message_ptr->frequency_list.num_frequencies == 0 )
               {
                  MSG_GERAN_ERROR_0_G("RR->L1 BCCH decode list empty");

                  /* Send CNF(ERROR) back to RR */
                  L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE, gas_id);
               }
               else
               {
                  L1_setup_DECODE_BCCH_LIST( msg_header );

                  /* Don't handle background search in this state. */
                  MSG_GERAN_MED_0_G("Decode BCCH list initiated in foreground");

                  GL1_ISR_LOCK( gas_id);
                  l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
                  l1_tskisr_blk->command      = L1_ACQUIRE_BCCH_LIST_COMMAND;
                  L1_send_ISRTIM_ACQ_LIST_REQ( l1_BCCH_List_ptr->current_sch_index,message_ptr->gas_id );
                  GL1_ISR_UNLOCK( gas_id);
               }
            }
            else
            {
               MSG_GERAN_ERROR_1_G("RR->L1(BCCH) message %d ignored.", msg_header->message_id);
            }
            break;
         }



         case MPH_START_GPRS_IDLE_MODE_REQ:
         case MPH_GPRS_L1PARAM_UPDATE_REQ:
         {
            gpl1_gprs_handle_grr_idle_mode(msg_header);

            break;
         }

         case MPH_GPRS_RESELECTION_REQ:
         {
            gpl1_gprs_handle_reselection_bcch_mode((mph_gprs_reselection_req_t *)msg_header);

            break;
         }

         default:
            MSG_GERAN_ERROR_1("RR->L1(BCCH) message %d ignored.", msg_header->message_id);
             break;

      } /* End of switch */
   } /* End of    else if (msg_header->message_set == MS_RR_L1) */
   else if (msg_header->message_set == MS_L2_L1)
   {
     /********************************************************
      *                                                      *
      *                PH_RANDOM_ACCESS_REQ                  *
      *                                                      *
      ********************************************************/
      if (msg_header->message_id == PH_RANDOM_ACCESS_REQ)
      {
         ph_random_access_req_T   *message_ptr;
         idle_data_T               *l1_idle_data;
         volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
         gas_id_t gas_id;

         message_ptr = ( ph_random_access_req_T *) msg_header; /*lint !e740 */
         gas_id = check_gas_id(message_ptr->gas_id);
         l1_idle_data=&l1_idle_data_store[gas_id];
         l1_tskisr_blk = &l1_tsk_buffer[gas_id];

         /*Check to see whether we need to calculate RACH delay(PS HO enhancement)*/
         if (message_ptr->l2_channel_type == L2_RACH )
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
         /* Map txlev to power index before passing to msg layer */
         l1_idle_data->random_access_data.txlev =
           l1_idle_data->campedon_cell_data.MS_TXPWR_MAX_CCH;

         /* Extract RACH content to transmit on access burst */
         l1_idle_data->random_access_data.ra_content =
           message_ptr->random_access_content;

         l1_idle_data->random_access_data.pwr_reduction_valid = message_ptr->pwr_reduction_valid;
         l1_idle_data->random_access_data.pwr_reduction_value = message_ptr->pwr_reduction_value;
         l1_idle_data->random_access_data.emergency_call = message_ptr->emergency_call;
         /* Check if this is an EGPRS packet channel request */
         if (message_ptr->egprs_ra_content_present)
         {
           /* Extract RACH content and TSCH to transmit on 11 bit access burst */
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

         l1_tskisr_blk->next_params.L1Data.pIdle_data = l1_idle_data;

         l1_tskisr_blk->main_command = L1_ACCESS_COMMAND;
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
      else if (msg_header->message_id == CB_SKIP_REQ) /*lint !e641 */
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
      MSG_GERAN_ERROR_0("L1(BCCH) - Unknown message set.");
   }
}

/*===========================================================================

FUNCTION  L1_SELECT_NEXT_ARFCN

DESCRIPTION
  This function selects the next ARFCN in the Frequency List.

DEPENDENCIES
  ARFCN index

RETURN VALUE
  success

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_select_next_ARFCN( word *ARFCN_index, gas_id_t gas_id  )

{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];
   /* increment the pointer and determine if the ARFCN is useable */
   (*ARFCN_index)++;

   /* Bounds check to keep KW happy */
   if ( *ARFCN_index >=
          ( sizeof( l1_Frequency_List->agc ) / sizeof( l1_Frequency_List->agc[0] ) ) )
   {
     MSG_GERAN_ERROR_2_G( "Too many freq list agc elements requested %d max %d",
                *ARFCN_index,
                ( sizeof( l1_Frequency_List->agc ) / sizeof( l1_Frequency_List->agc[0] ) ));

     return(FALSE);
   }
   else if ((*ARFCN_index < l1_Frequency_List->length) &&
            (l1_Frequency_List->agc[*ARFCN_index].pwr_dBm_x16 > SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16))
   {
      /* All is well */
      return (TRUE);
   }
   else
   {
      return(FALSE);
   }
}

/*===========================================================================

FUNCTION  L1_send_ISRTIM_SCAN_REQ

DESCRIPTION
  This function selects the ISRTIM_SCAN_REQ to the ISR by loading the
  common memory block 'l1_isrtsk_blk'

DEPENDENCIES
  command - the next main state required by the ISR

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISRTIM_SCAN_REQ( byte command, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];

  /* Load the Block */
  l1_tskisr_blk->lockout = TRUE;

  l1_tskisr_blk->next_params.L1Data.pScan_list = l1_Frequency_List;
  l1_tskisr_blk->command = L1_SCAN_COMMAND;
  l1_tskisr_blk->main_command = command;
  l1_tskisr_blk->lockout = FALSE;
}

/*===========================================================================

FUNCTION  L1_SEND_ISRTIM_ACQ_REQ

DESCRIPTION
  This function selects the ISRTIM_SCH_REQ to the ISR by loading the
  common memory block 'l1_isrtsk_blk'

DEPENDENCIES
  ARFCN_index

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISRTIM_ACQ_REQ( ARFCN_T ARFCN, gas_id_t gas_id )
{
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];
   sys_algo_agc_T *agc_ptr;

   /* Load the Block */
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_tskisr_blk->lockout     = TRUE;

   agc_ptr = l1_get_agc_ptr ( ARFCN, gas_id );

   if ( agc_ptr )
   {
        l1_Frequency_List->agc[l1_Frequency_List->length] = *agc_ptr;
   }
   else
   {
     /* Use an unused item in freq list */
     agc_ptr = &l1_Frequency_List->agc[l1_Frequency_List->length];
     sys_algo_agc_init(agc_ptr, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );
     sys_algo_agc_disable_slew_rate_limiting(agc_ptr);
   }

   l1_tskisr_blk->next_params.L1Data.pAgc_data = agc_ptr;

   l1_Frequency_List->ARFCN[l1_Frequency_List->length] = ARFCN;

   l1_tskisr_blk->next_params.L1Data.pAgc_data = agc_ptr;
   l1_tskisr_blk->next_params.frames_for_FCBdetect = SYS_PARAM_MAX_FRAMES_FOR_FCCH_DETECTION;
   l1_tskisr_blk->next_params.frames_for_SCHdecode = SYS_PARAM_MAX_FRAMES_FOR_SCH_DECODING;
   l1_tskisr_blk->next_params.max_SCH_attempts = SYS_PARAM_MAX_ATTEMPTS_FOR_SCH_DECODING;
   l1_tskisr_blk->next_params.ARFCN = ARFCN;
   l1_tskisr_blk->command        = L1_ACQUIRE_COMMAND;
   l1_tskisr_blk->next_params.bsic_expected = GSM_INVALID_BSIC;
   l1_tskisr_blk->lockout        = FALSE;
}

/*===========================================================================

FUNCTION  L1_SEND_ISRTIM_ACQ_LIST_REQ

DESCRIPTION
  This function selects the ISRTIM_SCH_REQ to the ISR by loading the
  common memory block 'l1_isrtsk_blk'

DEPENDENCIES
  ARFCN_index

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISRTIM_ACQ_LIST_REQ( uint16 ARFCN_index, gas_id_t gas_id )

{
   BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];

   /* Load the Block */
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_tskisr_blk->lockout = TRUE;

   if ( l1_get_agc_ptr ( l1_BCCH_List_ptr->arfcn[ ARFCN_index ], gas_id ) )
   {
     l1_tskisr_blk->next_params.L1Data.pAgc_data = l1_BCCH_List_ptr->agc_ptrs[ARFCN_index];
   }
   else
   {
      /*
      * If BCCH decode list freq is not in power scanlist,
      * use agc struct with default values
      */
      l1_tskisr_blk->next_params.L1Data.pAgc_data = &l1_BCCH_List_ptr->agc[ARFCN_index];
   }

   l1_tskisr_blk->next_params.frames_for_FCBdetect = SYS_PARAM_MAX_FRAMES_FOR_FCCH_DETECTION;
   l1_tskisr_blk->next_params.frames_for_SCHdecode = SYS_PARAM_MAX_FRAMES_FOR_SCH_DECODING;
   l1_tskisr_blk->next_params.max_SCH_attempts = SYS_PARAM_MAX_ATTEMPTS_FOR_SCH_DECODING;
   l1_tskisr_blk->next_params.ARFCN_index       = ARFCN_index;
   l1_tskisr_blk->next_params.ARFCN = l1_BCCH_List_ptr->arfcn[ARFCN_index];

   /*
      This line was taken out of L1_send_ISRTIM_ACQ_LIST_REQ
      so that we could have a common function for background
      HPLMN searching.  Its invoked by itself after this function is called
   */

   l1_tskisr_blk->lockout        = FALSE;
}

/*===========================================================================

FUNCTION  L1_SEND_ISRTIM_DECODE_BCCH_LIST_REQ

DESCRIPTION
  This function selects the ISRTIM_SCH_REQ to the ISR by loading the
  common memory block 'l1_isrtsk_blk'

DEPENDENCIES
  ARFCN_index

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISRTIM_DECODE_BCCH_LIST_REQ( gas_id_t gas_id )

{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Load the Block */
   l1_tskisr_blk->lockout        = TRUE;
   l1_tskisr_blk->command        = L1_DECODE_BCCH_LIST_COMMAND;
   l1_tskisr_blk->lockout        = FALSE;
}


/*===========================================================================

FUNCTION  L1_SEND_ISRTIM_CCH_UNI_DWN_REQ

DESCRIPTION
  This function the command to read the BCCH to the ISR
  i.e. read the BCCH, but not BCCH_ext

DEPENDENCIES

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_send_ISRTIM_CCH_UNI_DWN_REQ( word           BSIC,
                                     ARFCN_T        ARFCN,
                                     uint32         ext_BCCH_mask,
                                     sys_algo_agc_T *agc_ptr,  gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   /* Load the Block */
   l1_tskisr_blk->lockout = TRUE;

   l1_tskisr_blk->next_params.L1Data.pAgc_data   = agc_ptr;
   l1_tskisr_blk->next_params.BSIC               = BSIC;
   l1_tskisr_blk->next_params.ARFCN              = ARFCN;
   l1_tskisr_blk->next_params.extended_BCCH_mask = ext_BCCH_mask;

   /* we have already acquired; so we are aligned with
      the serving cell for BCCH decode */
   l1_tskisr_blk->next_params.QB_lag             = 0;

   l1_tskisr_blk->command = L1_DECODE_BCCH_COMMAND;

   l1_tskisr_blk->lockout = FALSE;
}


/*===========================================================================

FUNCTION  gpl1_gprs_handle_reselection_bcch_mode

DESCRIPTION
  This function handles reselection in FindBcch mode

DEPENDENCIES
  param1
  param2
  param3
  param4

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gpl1_gprs_handle_reselection_bcch_mode(mph_gprs_reselection_req_t *msg_ptr)
{
  gas_id_t gas_id = check_gas_id(msg_ptr->gas_id);
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;

  l1_tskisr_blk = &l1_tsk_buffer[msg_ptr->gas_id];

  MSG_GERAN_MED_0_G("FindBcch mode:51_RESELECTION_GPRS campedon_cell_ARFCN()");

  /* This flag tells the ISR that a reselection cnf is to be sent */
  GL1_ISR_SAVE_LOCK(gas_id);
  L1_send_ISRTIM_ACQ_REQ(msg_ptr->gprs_surround_data.bcch_params.bcch_arfcn, gas_id);
  l1_tskisr_blk->main_command = L1_FIND_BCCH_COMMAND;
  l1_tskisr_blk->command = L1_ACQUIRE_DURING_RESELECTION_COMMAND;
  GL1_ISR_SAVE_UNLOCK(gas_id);
#ifdef FEATURE_GSM_COEX_SW_CXM
  garb_intf_notify_find_bcch_update_arfcn(msg_ptr->gprs_surround_data.bcch_params.bcch_arfcn,gas_id);
  garb_intf_notify_find_bcch_update_rx_power(msg_ptr->gprs_surround_data.bcch_params.bcch_arfcn,
                                            (int32)(l1_tskisr_blk->next_params.L1Data.pAgc_data->pwr_dBm_x16),gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */
  
}
