
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        INTERRAT SURROUND CELL

GENERAL DESCRIPTION
   This module contains routines specific to performing TDS interRAT
   measurements while in GSM.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2002-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_irat_wcdma.c#4 $
$DateTime: 2020/02/10 05:49:45 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
17/02/16   zf        CR2008580: Reset tick_wcdma after aborting W ID search in TBF
11/03/15   dp/ng     CR981929 Fix W cell drift calculations when position wraps around 307200 cx8 (38400 * 8)
19/12/17   km        CR1021898 Allow G2W reconfirm to schedule before going to sleep in "PCH Decoded" async interrupt
06/12/17   km        CR973889 Idle Timeline Opt - Sleep between PCH and IRAT
25/07/17   ng        CR2081244 FR37926: G2W/G2T reconf search in QTA periodicity should be 10 sec; but stick to forced search scheduling if forced meas ongoing
13/07/17   ng        CR2075022 Reset G2W ID search scheduling cycle only for a particular freq when reconf attempt on cell belonging to that freq fails
29/10/17   km        CR2133917 GL1 support for L+L; FR37926 non-DDS sub IRAT optimization
06/03/2016 ng        CR1022465 Clear drx_ref once IRAT activity gets scheduled
26/05/16   sk        CR1020757 Add DTF events for G2X IRAT scenarios
12/04/16   zf        CR1002584: Set DRX reference for W reconf when activity is reserved 
22/01/16   dg        CR963952 Don't send surround irat meas ind with uarfcn as 0 to higher layers
30/12/15   js        CR956837 Set DRX reference only when IRAT activity is reserved
10/09/15   dg        CR905478 Send SURROUND_MEAS_IND to GRR if energy of W cell is less than threshold during G2W ded mode reconf search
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
07/08/14   dp        CR886529 Only send abort_cb when idle id handler is done
23/07/15   aga       CR876916 Avoid booking with L1FM IRAT activity for T and W if GFW is still loaded
06/07/15   zf        CR865792: Start ID search when current frame is equal or later than ID search FN
06/07/15   cws       CR864008 Set wcdma_srch_in_progress flag when srch_state moves to SCHEDULE_WCDMA
16/06/15   dg        CR854712 Ensure feature FEATURE_GSM_SEARCH_ALL_WCELL runs during every iteration of G2W ID search
26/06/15   zf        CR862712: For idle mode G2W search, wait until RF exit done before releasing TRM lock
11/06/15   og        CR849397. Wrong frequency index used to schedule the WCDMA reconfirm.
20/05/15   aga       CR840984 NULL2X IRAT changes for G2W IRAT
19/05/15   cja       CR839842 For G2T do not changes search type during search.
28/04/15   jj        CR829554 mutex dead lock in ded_srch_reconf_cb
05/05/15   dp        CR830787 ensure srch handlers complete before SCE cleans up
21/04/15   cja       CR826384 For reconf do not use idle monitors in progress flag
03/02/15   cjl       CR790433 Create separate TRM activity for WCDMA reconfirm when needed
14/04/15   aga       CR799530 Reserve W/TD/L IRAT ID search with TRM in advance
14/04/15   cgc       CR794901 added L1_send_MPH_SURROUND_WCDMA_MEAS_IND for common xfer/dedicated results
09/04/15   dp        CR819956 Thor 2.x RF API change, G2W use MSGR
30/03/15   cja       CR800650 Tidy-up SC IRAT state machine
                      Initial version - files moved from l1_sc_irat.
===========================================================================*/


#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "l1_sc_irat.h"
#include "l1_sc.h"
#include "l1i.h"
#include "l1_utils.h"
#include "l1_drx.h"
#include "l1_fm.h"

#include "wl1x2wsrchapi.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT*/

#include "gl1_hw_g.h"
#include "gpl1_gprs_task.h"


#include "gl1_msgrif_init.h"

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#include "l1_log_qsh.h"
#endif

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/



/************************** external function definitions ***************************************/




/************************** global data ***************************************/

#define CX8_PER_WFRAME 307200

/************************** function prototypes ***************************************/

static void  l1_sc_update_stopping_cell_id_srch(uint16 freq_idx, gas_id_t gas_id);

static void  l1_sc_process_wcdma_id_srch_results( void*  freq_index, gas_id_t gas_id );

static void  l1_sc_process_wcdma_id_srch_tbf_results( void*  freq_index, gas_id_t gas_id );

static void  update_rr_params_from_id_in_tbf( const t_stored_meas  stored_meas[MAX_WCDMA_FREQS],
                                              t_rr_params*         rr_params_ptr,
                                              gas_id_t             gas_id
                                            );

static boolean gl1_sc_irat_schedule_wcdma_meas_drx(gas_id_t gas_id);
static boolean gl1_sc_irat_can_wcdma_meas_go(gas_id_t gas_id);


/*===========================================================================

FUNCTION l1_sc_g2w_id_wait_state

DESCRIPTION
  This function handles the irat search wait state for G2W ID search 

===========================================================================*/
void l1_sc_g2w_id_wait_state(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id)
{
   uint16 freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
 
   MSG_GERAN_HIGH_2_G("Going to schedule g2w id search %d arfcn %d", freq_idx, l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);

   if(gl1_sc_irat_schedule_wcdma_meas_drx(gas_id))
   {

      l1_fm_extend_required_frames(FM_PRI_WCDMA_IDSEARCH, 3 * MAX_ID_SEARCH_FRAMES, gas_id);
      l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_ID, gas_id);
      l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_WCDMA;
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_SCHEDULE_WCDMA;
      l1_sc_globals_ptr->wcdma_srch_in_progress = TRUE;
      L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);

      gl1_drx_require_next_tick(gas_id);
   }
}

/*===========================================================================

FUNCTION l1_sc_g2w_reconf_wait_state

DESCRIPTION
  This function handles the irat search wait state for G2W reconfirm 

===========================================================================*/
void l1_sc_g2w_reconf_wait_state(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id)
{
  uint32              current_fn            = GSTMR_GET_FN_GERAN(gas_id);
  uint16              freq_idx              = l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset;

   MSG_GERAN_HIGH_2_G("Going to schedule g2w reconf %d arfcn %d", freq_idx, l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);

   /* If we have finished page decode */
   if(!l1_sc_is_idle_rx_in_progress(gas_id))
   {
     uint32 start_fn = l1_sc_wcdma_data_ptr->scheduled_wcdma_reconf_fn;
     uint32 frames_to_start = SUB_FN (start_fn,current_fn);

     /* If the time to perform wcdma reconf has arrived then try to schedule it */
     if ( frames_to_start == 0 )
     {
       /* if booked frames are still available to use */
       if ( l1_sc_frames_available (FM_PRI_WCDMA_RECONF, MAX_WCDMA_RECONF_FRAMES, l1_wcdma_abort_scheduled_reconfirm, 0, gas_id) )
       {
        /* Schedule the W reconf activity */
         l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_SCHEDULE_WCDMA;
         l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_WCDMA;
         l1_sc_globals_ptr->wcdma_srch_in_progress = TRUE;
         L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
         /* Make sure we don't go to sleep */
         gl1_drx_require_next_tick(gas_id);
       }
       else
       {
         MSG_GERAN_HIGH_1_G("G2W scheduled reconf blocked at FN=%d",current_fn);
       }
     }

     /* If the booking has expired, was missed, is not present or is invalid then try to rebook activity otherwise keep waiting */
     else if (frames_to_start > PCH_TO_WCDMA_RECONF_GAP)
     {
       /* Check that frames are available while maintaining gap between W reconf and demod page */
         l1_sc_wcdma_data_ptr->scheduled_wcdma_reconf_fn = ADD_FN(current_fn, PCH_TO_WCDMA_RECONF_GAP);
         /* Schedule the WCDMA reconfirm FM activity */
         l1_fm_set_required_frame_number( FM_PRI_WCDMA_RECONF, 
                                          l1_sc_wcdma_data_ptr->scheduled_wcdma_reconf_fn,
                                          MAX_WCDMA_RECONF_FRAMES, gas_id);
         MSG_GERAN_HIGH_1_G("G2W scheduled reconf scheduled for FN=%d",
                            l1_sc_wcdma_data_ptr->scheduled_wcdma_reconf_fn);

        if(!gl1_msg_get_multi_sim_mode())
        {
          /* Enable DRX Sleep until our scheduled activity */
          gl1_drx_require_tick(l1_sc_wcdma_data_ptr->scheduled_wcdma_reconf_fn,gas_id);
        }
     }
   }
}
/*===========================================================================

FUNCTION l1_sc_g2w_schedule

DESCRIPTION
  This function handles the irat search wait state for G2T 

===========================================================================*/
void l1_sc_g2w_schedule(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id)
{
   uint16 freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
   boolean activity_can_run = TRUE;

   if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type ==  GTOW_SRCH_TYPE_RECONFIRM)
   {
      uint16  i;
      uint16  num_cells;

      freq_idx  = l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset;
      num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;

      /* optimised for value 32 */
      for( i=0; i < MAX_WCDMA_CELLS_PER_FREQ ; i+=4 )
      {
         l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i]   = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].wcdma_cell;
         l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+1] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i+1].wcdma_cell;
         l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+2] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i+2].wcdma_cell;
         l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[i+3] = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i+3].wcdma_cell;
      }

      if( num_cells > MAX_WCDMA_CELLS_PER_FREQ )
      {
         num_cells = MAX_WCDMA_CELLS_PER_FREQ;
      }

      if (gl1_msg_wcdma_reconfirm (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
                                   num_cells,
                                   &l1_sc_wcdma_data_ptr->gl1_msg_wcdma_cells_buffer[0],
                                   &l1_sc_wcdma_data_ptr->recon_rpt,
                                   GL1_DEFS_TN_0,
                                   freq_idx,
                                   gas_id))
      {
         /*FR37926 non-DDS sub IRAT optimization, Reconfirm search period should be made 10 sec (reconf cycle * 2)
           But if Forced search is active, then should use normal periodicity, as intention is to do search every 2.5 secs */
         if (l1_sc_g2x_check_for_extend_srch_period(gas_id) && (l1_sc_wcdma_data_ptr->rr_params.ForcedMeas == FORCE_MEAS_NULL))
         
         {
            l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + (l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle << 1);
         }
         else
         {
            l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick + l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_cycle;
         }

         MSG_GERAN_HIGH_2_G("G2W scheduled reconf in progress for UARFCN %d, num_cells %d",
                            l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
                            num_cells);
      }
      else
      {
         activity_can_run = FALSE;
      }
   }
   else
   {
       l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_ID, gas_id);
       freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;

#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
      if( (l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found == TRUE) &&( nv_for_search_all_w_cell ) )
      {
         l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan --;
      }
#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */

      l1_fm_extend_required_frames(FM_PRI_WCDMA_IDSEARCH, 3*MAX_ID_SEARCH_FRAMES, gas_id);

      gl1_msg_wcdma_identify( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,
                              &l1_sc_wcdma_data_ptr->id_rpt,
                              gas_id
                             );

       gl1_set_ncell_block_start(TRUE,gas_id);
   }

   if (activity_can_run)
   {
      l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ACTIVE;
      l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_WCDMA;
      l1_sc_globals_ptr->wcdma_srch_in_progress = TRUE;
      L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);

      /*Clear drx_ref for this IRAT activity (G2W reconf and ID search- FM_PRI_WCDMA_IDSEARCH) now*/
      l1_sc_wcdma_data_ptr->drx_ref =0;
   }
}

/*===========================================================================

FUNCTION l1_sc_process_wcdma_id_srch_results

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent wcdma id search.

  1- Duplicate cells (scrambling codes) are removed from the id report.

  2- Remove any cells in the stored measurements structure that are not
     in the id report.

  3- Update the stored measurement structure with the results.

  The id report contains:

  Scrambling code (0..511, or all codes
  cell diversity
  PN Position (19 bit value, or PN_POSITION_UNKNOWN)

  RSCP     Received signal strength (in dBm)
  EcNo     Energy value returned from firmware
  RSSI

  The rr_report structure is updated and MPH_SURROUND_WCDMA_IDLE_MEAS_IND is
  sent to RR.

  This function handles both WCDMA and LTE cells.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_process_wcdma_id_srch_results( void*  freq_index, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  rpt_idx;
  uint16  meas_idx;
  uint32  freq_idx;

  uint16 num_rpt_cells;

#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
  uint16 rr_listed_wcdma_cell_found = 0;
  int16  Qualmin_EcIo = 0;
  /*Variable indicating good wcdma cell found which is not there in RR list*/
  boolean rr_found_good_wcdma_cell = FALSE;
#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */

  gl1_wcdma_cell_meas_rpt  stripped_id_rpt = {0};

  boolean cell_found = FALSE;
  boolean scr_already_in_strip = FALSE;

  /* Holds the list of cells to be deleted */
  struct stored_meas_delete_list_tag
  {
    uint16 scr_code_to_delete[MAX_WCDMA_CELLS_PER_UARFCN];
    uint16 list_length;

  }stored_meas_delete_list={{0},0};

  /* Get the freq and the number of cells from the report */
  freq_idx      = (uint32) (void *) freq_index;
  num_rpt_cells = l1_sc_wcdma_data_ptr->id_rpt.num_cell_measurements;

  #ifdef FEATURE_GSM_SEARCH_ALL_WCELL
 if(nv_for_search_all_w_cell)
 {
 switch( l1_sc_wcdma_data_ptr->rr_params.wcdma_EcIo_threshold_code)
      {

       case 0 :  Qualmin_EcIo = (-20*2); /* -20 dB */
                 break;

       case 1 :  Qualmin_EcIo = (-6*2);  /* -6 dB */
                 break;

       case 2 :  Qualmin_EcIo = (-18*2);  /* -18 dB */
                 break;

       case 3 :  Qualmin_EcIo = (-8*2);   /* -8 dB */
                 break;

       case 4:  Qualmin_EcIo = (-16*2);  /* -16 dB */
                 break;

       case 5:  Qualmin_EcIo = (-10*2);  /* -10 dB */
                 break;

       case 6 :  Qualmin_EcIo = (-14*2); /* -14 dB */
                 break;
       case 7 :  Qualmin_EcIo = (-12*2); /* -12 dB */
                 break;

      default:   Qualmin_EcIo = (-12*2); /* -12 dB */
                 break;

      }
 }

#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */
  if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
  {
    if ((l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN != l1_sc_wcdma_data_ptr->id_rpt.UARFCN) &&
      (num_rpt_cells != 0))
  {
    /* wcdma cell list must have changed, don't want to store this
       data because it may include cells no longer on list */
    MSG_GERAN_HIGH_1_G("Unknown WCDMA meas rpt: ch:%d!", l1_sc_wcdma_data_ptr->id_rpt.UARFCN);
    return;
  }
  }
  /* For LTE cells, the above check is done before calling this
   * function.
   */

  if (num_rpt_cells > MAX_WCDMA_CELLS_PER_UARFCN)
  {
    MSG_GERAN_ERROR_1_G("Too many WCDMA cells in rpt:%d!", num_rpt_cells);
    num_rpt_cells = MAX_WCDMA_CELLS_PER_UARFCN;
  }


  if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
  {


  /* Print out the cells direct from the report */
  MSG_GERAN_HIGH_3_G("%d PILOTS ID'd on UARFCN %d (nv_for_search_all_w_cell %d)", num_rpt_cells
                                                   ,l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN
                                                   ,nv_for_search_all_w_cell);

 /**********************************************************/
 /* Go through the report and strip out cells that are the */
 /* same so that there is only one entry for each PSC that */
 /* is the strongest entry.                                */
 /**********************************************************/

  MSG_GERAN_HIGH_0_G("--------- Sorted Cells ---------");

  stripped_id_rpt.UARFCN = l1_sc_wcdma_data_ptr->id_rpt.UARFCN;

  for (rpt_idx=0; rpt_idx<num_rpt_cells; rpt_idx++)
  {
    uint16  strip_idx;

    scr_already_in_strip = FALSE;

    /* See if this PSC already is in the temporary stripped list */
    for (strip_idx=0;strip_idx<stripped_id_rpt.num_cell_measurements; strip_idx++)
    {
      if ( stripped_id_rpt.meas[strip_idx].cell.scr_code ==
           l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx].cell.scr_code )
      {

        scr_already_in_strip = TRUE;
        /******************************************************************/
        /* There is already an entry for this SCR - it can be overwritten */
        /* if the EcNo of this entry is better                            */
        /******************************************************************/
        if ( stripped_id_rpt.meas[strip_idx].EcNo <
             l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx].EcNo )
        {
          stripped_id_rpt.meas[strip_idx] = l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];
        }

        break;
      }

    }  /* for (strip_idx = 0;strip_idx < stripped_id_rpt.num_cell_measurements; strip_idx++) */

    if (!scr_already_in_strip)
    {
      /*Load this entry into the stripped list*/
      stripped_id_rpt.meas[stripped_id_rpt.num_cell_measurements] = l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];
      stripped_id_rpt.num_cell_measurements++;
    }

  } /* for (rpt_idx=0; rpt_idx < num_rpt_cells; rpt_idx++) */

  /* Copy the temporary stripped list back over the ID report */
  l1_sc_wcdma_data_ptr->id_rpt = stripped_id_rpt;

  l1_sc_forced_meas_event( freq_idx, FORCE_MEAS_COMPLETED, gas_id);

  }  /* Is this a WCDMA UARFCN */

 /***********************************************************/
 /* Remove cell entries in the stored meas database if they */
 /* are not contained within this ID report                 */
 /***********************************************************/
 if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat )
 {
  for ( meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++ )
  {

    cell_found = FALSE;

    for (rpt_idx=0; rpt_idx<l1_sc_wcdma_data_ptr->id_rpt.num_cell_measurements; rpt_idx++)
    {
        if ( l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.scr_code ==
           l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx].cell.scr_code )
      {
        cell_found = TRUE;
        break;
      }
    }

    if ((!cell_found) && (stored_meas_delete_list.list_length < MAX_WCDMA_CELLS_PER_UARFCN))
    {
      /* Mark for deletion - don't delete now as we are using this database */
      stored_meas_delete_list.scr_code_to_delete[stored_meas_delete_list.list_length] =

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.scr_code;

      stored_meas_delete_list.list_length++;
    }

    }   /* for ( meas_idx=0; meas_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++ ) */
  } /* if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat ) */


  /* Now perform the delete of any marked stored meas entries */
  while( stored_meas_delete_list.list_length )
  {
    remove_stored_meas_cell_entry(
      (uint32)l1_sc_wcdma_data_ptr->id_rpt.UARFCN,
      stored_meas_delete_list.scr_code_to_delete[--stored_meas_delete_list.list_length],
      (uint8) RR_L1_IRAT_RAT_WCDMA,
      gas_id
      );
  }

 /**********************************************************/
 /* Loop through all the entries returned in the ID report */
 /* and store the data values in the stored meas database  */
 /**********************************************************/
  for (rpt_idx=0; rpt_idx<l1_sc_wcdma_data_ptr->id_rpt.num_cell_measurements; rpt_idx++)
  {
    gl1_wcdma_cell_meas_type*  id_rpt_entry_ptr = &l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx];

    MSG_GERAN_HIGH_3_G("  %d) SCR_CODE: %d ENG: %d dB",
      rpt_idx+1,
      id_rpt_entry_ptr->cell.scr_code,
      l1_sc_wcdma_data_ptr->id_rpt.meas[rpt_idx].EcNo/2);

    /**************************************************************/
    /* See if there is already a matching entry for this PSC      */
    /* in the stored_meas database. If there is update the values */
    /* otherwise add the entry.                                   */
    /**************************************************************/
    if ( get_stored_meas_cell_entry( l1_sc_wcdma_data_ptr->id_rpt.UARFCN,
                                     id_rpt_entry_ptr->cell.scr_code,
                                     (uint8) RR_L1_IRAT_RAT_WCDMA,
                                     NULL,
                                     NULL,
                                     gas_id)
       )
    {

      if ( id_rpt_entry_ptr->EcNo > MIN_PWR_TO_BE_VALID_CELL )
      {
        /***********************************************************/
        /* Update all the relevent parameters for the entry in the */
        /* stored meas database. These will get reported to RR in  */
        /* the next measurement report.                            */
        /***********************************************************/
        update_stored_meas_cell_entry(l1_sc_wcdma_data_ptr->id_rpt.UARFCN, id_rpt_entry_ptr, (uint8) RR_L1_IRAT_RAT_WCDMA, TRUE, gas_id);

      }
      else
      {
        /*************************************************************/
        /* Remove the entry from the stored meas database the cell's */
        /* power is too low to use.                                  */
        /*************************************************************/
        remove_stored_meas_cell_entry((uint32)l1_sc_wcdma_data_ptr->id_rpt.UARFCN, id_rpt_entry_ptr->cell.scr_code,
                                      (uint8) RR_L1_IRAT_RAT_WCDMA, gas_id);
      }
    }
    else
    {
      /*********************************************************/
      /* Check if this entry can be allowed to be added to the */
      /* stored_meas database.                                 */
      /*********************************************************/
#ifdef FEATURE_GSM_SEARCH_ALL_WCELL

      MSG_GERAN_HIGH_2_G("wcdma_cell_nf %d expa_wcdma_cs %d",l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found,
      l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan);

      if(( l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found == TRUE )&&( nv_for_search_all_w_cell ) )
      {

      if(id_rpt_entry_ptr->EcNo > Qualmin_EcIo )
      {
      /* found an intra freequency cell during all scrambling code  search */
      add_stored_meas_cell_entry( l1_sc_wcdma_data_ptr->id_rpt.UARFCN, id_rpt_entry_ptr,(uint8) RR_L1_IRAT_RAT_WCDMA, TRUE, gas_id );
      l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found = FALSE ;

      /*Set the below flag to TRUE*/
      rr_found_good_wcdma_cell = TRUE;
      }

      }
      else
#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */
      {
     if ( id_rpt_entry_ptr->EcNo > MIN_PWR_TO_BE_VALID_CELL )
      {

        /* Fill in the diversity value as defaulting to ON */
        id_rpt_entry_ptr->cell.diversity = DIVERSITY_ON;

        if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells == 0)
        {
          /* RR passed in frequency only, so all scr_codes are acceptable */
          add_stored_meas_cell_entry( l1_sc_wcdma_data_ptr->id_rpt.UARFCN, id_rpt_entry_ptr,(uint8) RR_L1_IRAT_RAT_WCDMA, TRUE, gas_id );
#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
        if (( id_rpt_entry_ptr->EcNo > Qualmin_EcIo )&&( nv_for_search_all_w_cell ) )
        {
          rr_listed_wcdma_cell_found++;
        }
#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */
        }
        else
        {
          uint16 cell_idx;

        for (cell_idx=0;
             cell_idx<l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;
               cell_idx++
             )
        {

            if( ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_idx].wcdma_cell.scr_code ==
                 id_rpt_entry_ptr->cell.scr_code
                )
              &&( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA )
              )
          {
            /* use the diversity value from RR list */
            id_rpt_entry_ptr->cell.diversity =
                l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_idx].wcdma_cell.diversity;

            /* Found cell in our current list from RR, so OK to add */
              add_stored_meas_cell_entry( l1_sc_wcdma_data_ptr->id_rpt.UARFCN, id_rpt_entry_ptr, (uint8) RR_L1_IRAT_RAT_WCDMA, TRUE, gas_id );
#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
        if ( (id_rpt_entry_ptr->EcNo > Qualmin_EcIo ) &&( nv_for_search_all_w_cell ) )
          {
            rr_listed_wcdma_cell_found++;
          }
#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */
            break;
          }
        }
        }

      }
      else
      {
        MSG_GERAN_HIGH_0_G("Cell power too low to be a valid cell!");
      }
    }
    } /* End of else the cell is not found in the stored measurements */
  }   /* for( rpt_idx = 0; rpt_idx < l1_sc_wcdma_data_ptr->id_rpt.num_cell_measurements; rpt_idx++ ) */
#ifdef FEATURE_GSM_SEARCH_ALL_WCELL
     if (nv_for_search_all_w_cell )
     {
        if((rr_listed_wcdma_cell_found == 0 )&&( l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found == FALSE)&&(l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan < 0))
        {
        /* no rr listed cell found*/
         l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan = MAX_EXPD_WCDMA_CELLS_SEARCH;
         l1_sc_wcdma_data_ptr->rr_listed_wcdma_cell_not_found = TRUE ;

        }

        /*If already good cell is found, clear flag number_of_expanded_wcdma_cell_scan to ensure that feature
            FEATURE_GSM_SEARCH_ALL_WCELL works during next G2W id search*/
        if(rr_found_good_wcdma_cell == TRUE)
        {
          l1_sc_wcdma_data_ptr->number_of_expanded_wcdma_cell_scan = -1;
        }

     }
#endif /*FEATURE_GSM_SEARCH_ALL_WCELL */
  l1_sc_update_stopping_cell_id_srch((uint16)freq_idx, gas_id);

  /* If no cells found on UARFCN then report to RR */
  if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells == 0)
  {
    MSG_GERAN_HIGH_1_G("No cell found on UARFCN=%d",l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
   
    L1_send_not_found_MPH_SURROUND_IRAT_MEAS_IND(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat, 
                                                 l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN, 
                                                 gas_id);
  }

#ifdef FEATURE_INCREASED_PERIOD_FOR_CELL_ID_CELL_RECONF_SRCHS
  l1_sc_send_cell_id_srch_to_rr((void *) freq_idx, gas_id);
#endif
    /* start the Femto search timer here */
    l1_isr_femto_timer_start();

} /* l1_sc_process_wcdma_id_srch_results */

/*===========================================================================

FUNCTION l1_sc_update_stopping_cell_id_srch

DESCRIPTION
  This function will decide whether or not to stop Cell Id searches after
  the id srch results are processed.

  If all the cells in the stored list are found then the id srchs are
  stopped.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_update_stopping_cell_id_srch(uint16 freq_idx, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  /*This is the place to stop cell id searches
  When we identified all the requested cells by RR
  */
  uint8 index_i, index_j;

  /*If we have not found cells on this UARFCN then don't stop searching!*/
  if(  (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells!=0)
     &&( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN )
     &&( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat          == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat )
    )
  {
    /* We have the cell list from RR, Now find out if we have identified
     * all the cells requested by RR
     */
    for(index_i=0; index_i<l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells; index_i++)
    {
        for(index_j=0; index_j<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; index_j++)
        {

          if( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat &&
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat == RR_L1_IRAT_RAT_WCDMA )
          {
            if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[index_i].wcdma_cell.scr_code
                == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[index_j].wcdma_cell.scr_code)
            {
              break;
            }
          }
#ifdef FEATURE_GSM_TDS
          else if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat &&
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat == RR_L1_IRAT_RAT_UTRAN_TDD)
          {
            if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[index_i].tds_cell.cell_parameter_id
                == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[index_j].tds_cell.cell_parameter_id)
            {
              break;
            }
          }
#endif
       }

      if(index_j == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells)
      {
          break;
      }

    } /* for(index_i=0; index_i<l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells; index_i++) */

    if(index_i == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells)
    {
        /* All the cells in the frequency list have been measured, so don't do anymore cell id srchs */

        l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx] = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;

        MSG_GERAN_HIGH_2_G("Stop Cell ID Srch on UARFCN=%d RAT %d",
                  l1_sc_wcdma_data_ptr->stop_cell_id_srch[freq_idx],l1_sc_wcdma_data_ptr->stored_meas[freq_idx].rat);
    }
  } /* if the arfcns and RATs match and number of cells is not zero */
}


/*===========================================================================

FUNCTION l1_sc_g2w_get_ecio_logarithmic_average

DESCRIPTION
  Wrapper for the various alternative functions, keep the code readable !
  
DEPENDENCIES
  none

RETURN VALUE
  result of average

SIDE EFFECTS
  None

===========================================================================*/

int16 l1_sc_g2w_get_ecio_logarithmic_average(int16  current_ecio, int16 previous_ecio)
{

int16  avg_result = 
#ifdef FEATURE_GSM_TO_WCDMA_MSGR
              gl1_g2w_get_ecio_logarithmic_average(
#else //FEATURE_G2W_DRIVER_IN_WL1
              wl1_x2w_srch_ecio_logarithmic_average(
#endif
                                                  current_ecio,
                                                  previous_ecio );
    return  avg_result;                                             
}

/*===========================================================================

FUNCTION l1_sc_process_wcdma_recon_srch_results

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent wcdma reconfirmation search.

  1- The results are read from the recon_rpt structure.

  2- If reported EcNo is below the minimum for the cell to be valid then
     cell id search is restarted.

  3- If the RSCP has fluctuated compared to the last measurement then cell
     id search is restarted.

  4- All cells are marked for deletion except the strongest 8.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void l1_sc_process_wcdma_recon_srch_results( void*  freq_index, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 rpt_idx, meas_idx, freq_idx;

  uint16 tmp_scr_code;
  uint16 num_rpt_cells;

  int64 cx8_drift;

  uint16 new_idx = 0;
  uint16 min_idx = 0;
  uint16 num_valid = 0;
  int16 min_avg = 0;


  freq_idx = (uint16) (uint32) (void *) freq_index;

  if( RR_L1_IRAT_RAT_WCDMA == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat )
  {
    if (l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN != l1_sc_wcdma_data_ptr->recon_rpt.UARFCN)
  {
    /* wcdma cell list must have changed, so just ignore this result */
    MSG_GERAN_HIGH_1_G("Unknown WCDMA meas rpt: ch:%d!", l1_sc_wcdma_data_ptr->recon_rpt.UARFCN);

    return;
  }


  num_rpt_cells = l1_sc_wcdma_data_ptr->recon_rpt.num_cell_measurements;

  if (num_rpt_cells > MAX_WCDMA_CELLS_PER_UARFCN)
  {
    num_rpt_cells = MAX_WCDMA_CELLS_PER_UARFCN;

      MSG_GERAN_ERROR_1_G("Too many WCDMA cells in rpt:%d!", num_rpt_cells);
  }

  /* First step: Add report data to our stored_meas struct */
  for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
  {
    boolean found = FALSE;

      tmp_scr_code = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.scr_code;

    for (rpt_idx=0; rpt_idx<num_rpt_cells; rpt_idx++)
    {
      if (l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].cell.scr_code == tmp_scr_code)
      {
          found = TRUE;

          /* Update SYSCLK Slew for W measurement drift */
          if (rpt_idx == 0)
          {
            /* calculate drift under assumption forward maximum is 307200/2 cx8 and backwards maximum is -307200/2 cx8 */

            cx8_drift = (int64)l1_sc_wcdma_data_ptr->recon_rpt.meas[0].cell.PN_pos - l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.PN_pos;
            
            if(cx8_drift > CX8_PER_WFRAME/2)
            {                       
              cx8_drift -= CX8_PER_WFRAME;
            }
            else if(cx8_drift < -(CX8_PER_WFRAME/2))
            {
              cx8_drift += CX8_PER_WFRAME;
            }

            MSG_GERAN_MED_3_G("W slew adjust %u -> %u = %ld cx8 drift", 
                              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.PN_pos,
                              l1_sc_wcdma_data_ptr->recon_rpt.meas[0].cell.PN_pos,
                              cx8_drift);

            gl1_sleepctl_error_feedback(SCLK_WCDMA, cx8_drift, gas_id);
          }

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.PN_pos =
                  l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].cell.PN_pos;

          if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != 0)
          {
            /* Look for RSCP fluctuations, if so then start cell id searches again! */
            if( l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg > MIN_EcNo_TO_BE_VALID_CELL )
            {
              /* This cell won't be deleted */
              int16 RSCP_diff;

              if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last < l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].RSCP)
              {
                    RSCP_diff = l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].RSCP - l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last;
              }
              else
              {
                    RSCP_diff = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last - l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].RSCP;
              }

              if(RSCP_diff >MAX_RSCP_FLUCTUATION)
              {
                  l1_sc_restart_specific_cell_id_srch(freq_idx, gas_id);

                  MSG_GERAN_HIGH_1_G("RSCP fluctuation is greater than %d db",MAX_RSCP_FLUCTUATION);
              }

            }

          /* we have a previous measurement, so we can calc avg
           * check each parameter independently.
           */
          if( 0 != l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_last )
          {
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg =
                l1_sc_g2w_get_ecio_logarithmic_average(  l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].EcNo,
                                                         l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_last);
          }
          else
          {
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_last = l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].EcNo;

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg =
                          l1_sc_g2w_get_ecio_logarithmic_average( l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].EcNo,
                                                                  l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].EcNo );

          }
          l1_sc_g2x_filter_treatment(NULL, l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].RSSI, freq_idx, meas_idx, RR_L1_IRAT_RAT_WCDMA, gas_id);


          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg =
              ((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg/2) +
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg);

          if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg <= MIN_PWR_TO_BE_VALID_CELL )
          {
            /* Need to stop making searches on cells that no longer
             * are relevant (due to extremely low power), otherwise we
             * never stop searching. Mark for deletion by setting found
             * to FALSE */
             MSG_GERAN_HIGH_2_G("Stop reconfirming wcdma cell:low EcNo UARFCN=%d, scr_code=%d, Restart ID search ",
                                                   l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cached_UARFCN,
                                                   l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx].wcdma_cell.scr_code );

             l1_sc_restart_specific_cell_id_srch(freq_idx, gas_id);

             found = FALSE;

          } /* if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg <= MIN_PWR_TO_BE_VALID_CELL ) */

        }  /* if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != 0) */

        /* Fix for CR10421. This ensures that, if un-initialised, the average values are set
         * when first used in order to make downstream calculations accurate.
         */
          if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last == 0)
          {
             /* AVG is in dB units and not raw FW units */
             l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg =
                    l1_sc_g2w_get_ecio_logarithmic_average( l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].EcNo,
                                                            l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].EcNo );

             l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg =
              ((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_avg/2) +
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSSI_avg);
          }

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last =
                  l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].RSCP;

          l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.EcNo_last =
                  l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].EcNo;

          l1_sc_g2x_filter_treatment(NULL, l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].RSSI, freq_idx, meas_idx, RR_L1_IRAT_RAT_WCDMA, gas_id);

        break;

        } /* End of if (l1_sc_wcdma_data_ptr->recon_rpt.meas[rpt_idx].cell.scr_code == tmp_scr_code) */

      } /*  for( rpt_idx=0; rpt_idx < num_rpt_cells; rpt_idx++ ) */

    if (!found)
    {
      /* No current measurement, so mark for deletion */
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = 0;

      /* set this deleted cell RSCP average to zero, so its not included
         in the strongest 8 filter that follows */
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = 0;
  }

    } /* for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++) */


  /* Second step: Of the cells with valid averages, mark for deletion all
     but the strongest 8 */
  for (meas_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
  {
      int16 curr_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg;

    if (curr_avg != 0)
    {
      if (num_valid < 8)
      {
        num_valid++;

        if (curr_avg < min_avg)
        {
          min_avg = curr_avg;
          min_idx = meas_idx;
        }
      }
      else
      {
        if (curr_avg < min_avg)
        {
          /* mark this cell for deletion (set RSCP_list = 0) */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last = 0;
          /* remove this cell from further consideration */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_avg = 0;
        }
        else
        {
          uint16 tmp_idx;

          /* mark the min cell for deletion (set RSCP_list = 0) */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_last = 0;

          /* remove min cell from further consideration */
            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[min_idx].wcdma_cell.RSCP_avg = 0;

          /* go find new min cell */
          min_avg = 0;

          for (tmp_idx = 0; tmp_idx <= meas_idx; tmp_idx++)
          {
              int16 tmp_avg = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[tmp_idx].wcdma_cell.RSCP_avg;

            if ((tmp_avg != 0) && (tmp_avg < min_avg))
            {
              min_avg = tmp_avg;
              min_idx = tmp_idx;
            }
          }
        }

      }
      }    /* if (curr_avg != 0) */
    }      /* for( meas_idx=0; meas_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++ ) */

  /* Third step: Delete any items in our stored_meas struct marked for deletion */
  for (meas_idx=0, new_idx=0; meas_idx<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; meas_idx++)
  {
      if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx].wcdma_cell.RSCP_last != 0)
    {
      if (new_idx != meas_idx)
      {
        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx] =
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[meas_idx];

        l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx] =
                l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[meas_idx];
      }
      new_idx++;
    }
  }

  l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells = new_idx;

  if (new_idx != meas_idx)
  {
    /* set these 2 params to zero (to cleanup, not necessary) */
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[new_idx].wcdma_cell.RSCP_last = 0;
      l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[new_idx].wcdma_cell.scr_code = 0;

  }

  /* If no cells found on UARFCN then report to RR */
  if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells == 0)
  {
    MSG_GERAN_HIGH_1_G("No cell found on UARFCN=%d",l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
   
    L1_send_not_found_MPH_SURROUND_IRAT_MEAS_IND(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat, 
                                                 l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN, 
                                                 gas_id);
  }

  /* Generate report for RR */
  gsm_l1_update_rr_report( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                             &l1_sc_wcdma_data_ptr->rr_ignore_list,
                             &l1_sc_wcdma_data_ptr->rr_params,
                             &l1_sc_wcdma_data_ptr->rr_report,
                             gas_id
                           );

  l1_sc_forced_meas_event( freq_idx, FORCE_MEAS_COMPLETED, gas_id);

  /* Send report to RR */
  l1_sc_wcdma_send_rr_report(NULL, gas_id);

  /* generate log */
  l1_sc_wcdma_log_known_list(gas_id);

  }
  else
  {
    MSG_GERAN_ERROR_0_G(" Cannot process WCDMA reconf result of an LTE frequency ");
  }
}  /* l1_sc_process_wcdma_recon_srch_results */


/* callback functions */
/*===========================================================================

FUNCTION ID_REPORT_CB

DESCRIPTION
  This callback function is used by the search driver to pass results
  of a ID search back to L1.

  GERAN - LTE reselection

  The equivalent of this is needed for LTE measurements results.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void id_report_cb (gl1_wcdma_cell_meas_rpt *rpt, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint32 freq_idx;

  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_ID_REPORT); 
  #endif

  if (rpt != &l1_sc_wcdma_data_ptr->id_rpt)
  {
    MSG_GERAN_ERROR_1_G("G2W:id_report_cb Invalid rpt pointer 0x%p", rpt);
    return;
  }

  freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
  #else  
  l1_fm_clear_required_frames(FM_PRI_WCDMA_IDSEARCH, gas_id);

  l1_sc_set_gtow_srch_state_null (gas_id);
  #endif

  if (l1_sc_wcdma_data_ptr->wcdma_srch.done)
  {
    l1_scdrv_call_in_task( l1_sc_wcdma_data_ptr->wcdma_srch.done,
                           NULL, gas_id
                         );
  }
  else
  {
    MSG_GERAN_HIGH_0_G( "G2W:id_report_cb no done function!" );
  }
  
  /*If WCDMA aborted the gap, try to re-schedule same frequency at next opportunity*/
  if(rpt->w_aborted_gap)
  {
    MSG_GERAN_HIGH_1_G("Next avail G2X ID search should be freq idx %d", l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset);

    l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick;

    /* subtract 1 modulo num-frequencies */
    l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset = (l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset + 
                                                            l1_sc_wcdma_data_ptr->rr_params.num_freqs - 1) %
                                                            l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  }
  else
  {
    l1_scdrv_call_in_task( l1_sc_process_wcdma_id_srch_results,(void *)freq_idx, gas_id);
  }
}


/*===========================================================================

FUNCTION RECONFIRM_REPORT_CB

DESCRIPTION
  This callback function is used by the search driver to pass results
  of a RECONFIRM search back to L1.

  GERAN - LTE reselection

  The equivalent of this is needed for LTE reconfirmation results.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void reconfirm_report_cb (gl1_wcdma_cell_meas_rpt *rpt, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32 freq_idx;

  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_RECONFIRM_REPORT); 
  #endif

  if (rpt != &l1_sc_wcdma_data_ptr->recon_rpt)
  {
    MSG_GERAN_ERROR_1_G("Invalid reconfirm rpt pointer 0x%p ", rpt);

    return;
  }

  freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset;

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
  #else
  l1_sc_set_gtow_srch_state_null (gas_id);
  #endif

  /*If WCDMA aborted the gap, try to re-schedule same frequency at next opportunity*/
  if(rpt->w_aborted_gap)
  {

    MSG_GERAN_HIGH_1_G("Next avail G2X RECONF search should be freq idx %d", l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset);

    l1_sc_wcdma_data_ptr->wcdma_srch.reconfirm_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick;

    /* subtract 1 modulo num-frequencies */
    l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset = (l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset +
                                                                   l1_sc_wcdma_data_ptr->rr_params.num_freqs - 1) %
                                                                   l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  }
  else
  {
  l1_scdrv_call_in_task( l1_sc_process_wcdma_recon_srch_results,
                         (void *)freq_idx, gas_id
                       );
   }
}

/*===========================================================================

FUNCTION ID_ABORT_CB

DESCRIPTION
  This callback function is used by the search driver to let L1 know
  that the ID search has been aborted.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void id_abort_cb (gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_ID_ABORT); 
  #endif

  MSG_GERAN_HIGH_3_G("WCDMA id search aborted! rx_ip %d w_id_ip %d w_tries %d",
                     l1_sc_globals_ptr->receive_in_progress,
                     l1_sc_globals_ptr->wcdma_srch_in_progress,
                     l1_sc_wcdma_data_ptr->wcdma_tries);

  l1_fm_clear_required_frames(FM_PRI_WCDMA_IDSEARCH, gas_id);

  if ( (l1_sc_wcdma_data_ptr->drx_ref != 0) &&
     (IS_FRAME_NUM_LATER(l1_sc_wcdma_data_ptr->id_search_fn,gl1_get_FN( gas_id ))) )
  {
    /* remove old frame */
    gl1_drx_release_fn_tick( l1_sc_wcdma_data_ptr->id_search_fn, gas_id );
    l1_sc_wcdma_data_ptr->drx_ref = 0;
  }

  if ((l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_RAN_OUT_OF_FRAMES) && (l1_sc_wcdma_data_ptr->wcdma_srch.state == GtowStateIdle))
  {
     if (++l1_sc_wcdma_data_ptr->wcdma_tries == WCDMA_MAX_TRIES)
     {
        #ifdef FEATURE_GSM_TO_WCDMA_MSGR
        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
        #else
        l1_sc_set_gtow_srch_state_null (gas_id);
        #endif

        MSG_GERAN_HIGH_1_G("Abandoning WCDMA ID search after %d tries", WCDMA_MAX_TRIES);
     }
     else
     {
        l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_WAIT;

        MSG_GERAN_MED_3_G("WCDMA search IdState=Wait rip=%i idip=%i try:%d",
                l1_sc_globals_ptr->receive_in_progress,
                l1_sc_globals_ptr->wcdma_srch_in_progress, l1_sc_wcdma_data_ptr->wcdma_tries );

        l1_sc_globals_ptr->wcdma_srch_in_progress = FALSE;
     }
  }
  else
  {
     #ifdef FEATURE_GSM_TO_WCDMA_MSGR
     l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
     #else
     l1_sc_set_gtow_srch_state_null (gas_id);
     #endif
  }

}


/*===========================================================================

FUNCTION RECONFIRM_ABORT_CB

DESCRIPTION
  This callback function is used by the search driver to let L1 know
  that the RECONFIRM search has been aborted.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void reconfirm_abort_cb (gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_RECONFIRM_ABORT); 
  #endif
  
  MSG_GERAN_HIGH_2_G("WCDMA reconfirm search aborted! reconf_mode %d srch_state %d",l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode,l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);
  
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
  #else
  l1_sc_set_gtow_srch_state_null (gas_id);
  #endif

  l1_scdrv_call_in_task( l1_sc_wcdma_send_rr_report,
                         (char*)0, gas_id
                       );
}


/*===========================================================================

FUNCTION id_in_tbf_abort_cb

DESCRIPTION
  This callback function is used by the search driver to let L1 know
  that the ID search has been aborted.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void id_in_tbf_abort_cb (gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_ID_TBF_ABORT); 
  #endif
  
  MSG_GERAN_HIGH_0_G("WCDMA id search aborted!");

  trans[gas_id].tick_wcdma = FALSE;

  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
  #else
  l1_fm_clear_required_frames(FM_PRI_WCDMA_IDSEARCH, gas_id);

  l1_sc_set_gtow_srch_state_null (gas_id);
  #endif

  /* If no longer in transfer mode, then the abort is due to the transition
  back to idle mode so setup the IDLE callbacks here. */
  if (l1_sc_wcdma_data_ptr->wcdma_srch.xfer_mode == FALSE)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.send_report = TRUE;

    gl1_msg_cfg_wcdma_meas( id_report_cb,
                            id_abort_cb,
                            reconfirm_report_cb,
                            reconfirm_abort_cb,
                            #ifdef FEATURE_GSM_TO_WCDMA_MSGR
                            srch_handler_done_cb,
                            #endif
                            gas_id
                          );
  }
}

#ifdef FEATURE_GSM_TO_WCDMA_MSGR
/*===========================================================================

FUNCTION RECONFIRM_ABORT_CB

DESCRIPTION
  This callback function is used by the search driver to let L1 know
  that it's installed search handler has completed execution.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void srch_handler_done_cb(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  MSG_GERAN_HIGH_1_G("SCE notified of G2W search handler complete in srch_state %d", l1_sc_wcdma_data_ptr->wcdma_srch.srch_state);

  if(l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_CLEANUP_WAIT)
  {
    if(g2w_is_wfw_active(gas_id))
    {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      /* this shouldn't happen, but if it does, debug why */
      ERR_FATAL("g2w drivers told SCE it was done with actions, yet driver state says it's still active", 0, 0, 0);
    }

    l1_sc_set_gtow_srch_state_null(gas_id);
  }
}
#endif /* FEATURE_GSM_TO_WCDMA_MSGR */


/*===========================================================================

FUNCTION update_rr_params_from_id_in_tbf

DESCRIPTION
  This function will update the rr_params from the ID search
  results during TBFsuspension

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void update_rr_params_from_id_in_tbf( const t_stored_meas  stored_meas[MAX_WCDMA_FREQS],
                                             t_rr_params*         rr_params_ptr,
                                             gas_id_t             gas_id
                                           )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  f_idx = 0;

  for (  f_idx = 0; f_idx < rr_params_ptr->num_freqs; ++f_idx     )
  {
      uint16  c_idx;

      rr_params_ptr->freq_list[f_idx].freq_only_data = TRUE;
      rr_params_ptr->freq_list[f_idx].num_cells      = l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells;

      if( RR_L1_IRAT_RAT_WCDMA == rr_params_ptr->freq_list[f_idx].rat )
      {
        for(  c_idx = 0;
            c_idx < l1_sc_wcdma_data_ptr->stored_meas[f_idx].num_cells;
          ++c_idx       )
        {
          rr_params_ptr->freq_list[f_idx].cell_list[c_idx].wcdma_cell.scr_code =
             l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].wcdma_cell.scr_code;

          rr_params_ptr->freq_list[f_idx].cell_list[c_idx].wcdma_cell.diversity =
            l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].wcdma_cell.diversity;

          /* PN_pos in transfer / dedciated is now in chipx8 */

          rr_params_ptr->freq_list[f_idx].cell_list[c_idx].wcdma_cell.PN_pos =
            l1_sc_wcdma_data_ptr->stored_meas[f_idx].cell_params[c_idx].wcdma_cell.PN_pos ;
        }
    }
  } /* for (f_idx ... */
} /* update_rr_params_from_id_in_tbf */

/*===========================================================================

FUNCTION l1_sc_process_wcdma_id_srch_tbf_results

DESCRIPTION
  This function will process the ID search results during TBF suspension

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  l1_sc_process_wcdma_id_srch_tbf_results( void *freq_index, gas_id_t gas_id )
{
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

    l1_sc_process_wcdma_id_srch_results( freq_index, gas_id );

    update_rr_params_from_id_in_tbf( &( l1_sc_wcdma_data_ptr->stored_meas[0] ),
                                     &l1_sc_wcdma_data_ptr->rr_params,
                                     gas_id
                                   );
}

/*===========================================================================

FUNCTION id_in_tbf_report_cb

DESCRIPTION
  This callback function is used by the search driver to pass results
  of a ID search back to L1.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void id_in_tbf_report_cb (gl1_wcdma_cell_meas_rpt *rpt, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32 freq_idx;

  if (rpt != &l1_sc_wcdma_data_ptr->id_rpt)
  {
    MSG_GERAN_ERROR_0_G("Invalid id rpt!");
    return;
  }

  freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_in_tbf_freq_offset;

  l1_fm_clear_required_frames(FM_PRI_WCDMA_IDSEARCH, gas_id);
  
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
  #else
  l1_sc_set_gtow_srch_state_null (gas_id);
  #endif

  if (l1_sc_wcdma_data_ptr->wcdma_srch.done)
  {
    l1_scdrv_call_in_task(l1_sc_wcdma_data_ptr->wcdma_srch.done, NULL, gas_id);
  }
  else
  {
    MSG_GERAN_HIGH_0_G( "no done function!" );
  }
  /*If WCDMA aborted the gap, try to re-schedule same frequency at next opportunity*/
  if(rpt->w_aborted_gap)
  {
    MSG_GERAN_HIGH_1_G("Next avail G2X ID in TBF search should be freq idx %d", freq_idx);
    l1_sc_wcdma_data_ptr->wcdma_srch.id_tick[freq_idx] = l1_sc_wcdma_data_ptr->wcdma_srch.page_cycle_tick;
  }
  else
  {
  l1_scdrv_call_in_task(l1_sc_process_wcdma_id_srch_tbf_results, (void *)freq_idx, gas_id);
  }
}


/*===========================================================================

FUNCTION L1_SC_PROCESS_WCDMA_DED_ID_SRCH

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent stage1/stage2 search.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void  l1_sc_process_wcdma_ded_id_srch( void* freq_index, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16  cell_idx;
  uint16 tmp_scr_code;
  boolean cell_found;

  uint16  freq_idx = (uint16) (uint32) (void *) freq_index;
  gl1_concman_config_params_type gl1_concman_config_params;
  cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

  /* Frequency list may have changed if RR has sent L1 a new list
   * while this search was in progress. We may still be able to
   * use this data, if the cell is still on our list.
   */
  if(  ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN != l1_sc_wcdma_data_ptr->saved_ded_rpt.UARFCN )
     ||( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat          == RR_L1_IRAT_RAT_LTE )
    )
  {
  /* Find the new index */
    for(freq_idx=0; freq_idx<l1_sc_wcdma_data_ptr->rr_params.num_freqs; freq_idx++)
    {
      if( ( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN == l1_sc_wcdma_data_ptr->saved_ded_rpt.UARFCN )
        &&( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat          == RR_L1_IRAT_RAT_WCDMA )
        )
      {
        break;
      }
    }
  }

  gl1_concman_config_params.type = CMGR_TRANSIENT_END;
  gl1_concman_config_params.scenario = CMGR_GERAN_PTM_G2W_GAP;
  gl1_concman_config_params.update_scenario = 0;
  gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_IRAT_MEASUREMENT);
  gl1_concman_config_params.start_timestamp = 0;
  gl1_concman_config_params.duration_msec = 0;
  gl1_concman_config_params.flow_ctrl_time_msec = 0;
  gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);
  
  if (freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs)
  {
    cell_found = FALSE;

    l1_sc_forced_meas_event( freq_idx, FORCE_MEAS_COMPLETED, gas_id);

    /* find the cell in the rr_params struct */
    for (cell_idx=0;
         cell_idx<l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells;
         cell_idx++
       )
    {
      tmp_scr_code = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_idx].wcdma_cell.scr_code;

      if (l1_sc_wcdma_data_ptr->saved_ded_rpt.scr_code == tmp_scr_code)
      {
        /* save off the PN position for use by driver on next search */
        l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[cell_idx].wcdma_cell.PN_pos =
                l1_sc_wcdma_data_ptr->saved_ded_rpt.PN_pos;

        /* Now save off the current data into the stored_meas database */
        {
          uint16 search_idx;

          boolean found = FALSE;
          boolean add_to_db = FALSE;

          /* See if the newly found scr is in the database */
          for(  search_idx = 0;
                search_idx < l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
              ++search_idx
             )
          {
            if(l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[search_idx].wcdma_cell.scr_code
                 == tmp_scr_code )
            {
              found = TRUE;

              break;
            }
          }

          /* if not found add it to the database                */
          /* but only if it really is a good cell ie good power */
          if(  ( !found )
             &&( l1_sc_wcdma_data_ptr->saved_ded_rpt.EcNo > MIN_PWR_TO_BE_VALID_CELL )
            )
          {
            if (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells < MAX_WCDMA_CELLS_PER_UARFCN)
            {
              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[search_idx].wcdma_cell.scr_code
                = tmp_scr_code;

              l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells++;

              add_to_db = TRUE;
            }
          }
          else if (!found) /* not found in local db */
          {
            MSG_GERAN_MED_2_G("Cell(%d) found but low ecno %d",tmp_scr_code,l1_sc_wcdma_data_ptr->saved_ded_rpt.EcNo);
          }

          /* update the db or add to it*/
          if (found || add_to_db)
          {
             cell_found = TRUE;

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[search_idx].wcdma_cell.PN_pos =
                                                       l1_sc_wcdma_data_ptr->saved_ded_rpt.PN_pos;

            /*
             * Save the "last" RSCP and EcNo figures so that a running average
             * can be performed in the non-DRX period if we return to packet i
             * idle after this measurement
             */

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[search_idx].wcdma_cell.RSCP_last =
                                                       l1_sc_wcdma_data_ptr->saved_ded_rpt.RSCP;

            l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_energy[search_idx].wcdma_cell.EcNo_last =
                                                       l1_sc_wcdma_data_ptr->saved_ded_rpt.EcNo;

          }
          /* now keep only the top 8 cells */
          update_stored_measurements_for_top_8(freq_idx, gas_id);
        }

        break;

      }  /* if (l1_sc_wcdma_data_ptr->saved_ded_rpt.scr_code == tmp_scr_code) */
    } /* end for */

    /* If no cells found on UARFCN then report to RR */
    if ((cell_found == FALSE) && (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state == GTOW_SRCH_STATE_NULL))
    {
       MSG_GERAN_MED_1_G("No cell found on UARFCN=%d",l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
       
       L1_send_not_found_MPH_SURROUND_IRAT_MEAS_IND(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat, 
                                                    l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN, 
                                                    gas_id);
    }

    {
        /*This is the place to stop cell id searches
        When we identified all the requested cells by RR
        */
        uint8 index_i, index_j;

        if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells!=0)
        {
          /*We have the cell list from RR, Now find out if we have identified
           all the cells requested by RR
          */
          for(index_i=0; index_i<l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells; index_i++)
          {
              for(index_j=0; index_j<l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells; index_j++)
              {
                if(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].cell_list[index_i].wcdma_cell.scr_code
                      == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[index_j].wcdma_cell.scr_code)
                  break;
              }

              if(index_j == l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells)
              {
                  break;
              }
          }

          if(index_i == l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells)
          {
              /* We have found all the cells in stored list, so, don't do anymore cell id srchs */

           /* @@OG:
            * This check is not needed for LTE cells in LTE equivalent of this function.
            * This is because RR informs L1 about LTE frequencies and not cells nad hence
            * we have nothing to compare against.
            */
            l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[freq_idx] = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;

            MSG_GERAN_HIGH_1_G("Stop Cell ID Srch on UARFCN=%d", l1_sc_wcdma_data_ptr->stop_ded_cell_id_srch[freq_idx]);
          }

        }  /* if( l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells != 0 ) */
    }

    /* check to see if RR passed this cell down to L1 */
    if (cell_idx >= l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].num_cells)
    {
      MSG_GERAN_HIGH_2_G("CELL %d UARFCN %d not on list",l1_sc_wcdma_data_ptr->saved_ded_rpt.scr_code,
              l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
    }

  } /* if( freq_idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs ) */
  else
  {
  /* RR updated the frequency list while the search was in progress and as a result
   * the results of this id search can be discarded because the frequency no longer
   * exists.
   */
    MSG_GERAN_HIGH_2_G("CELL %d UARFCN %d not on list anymore",l1_sc_wcdma_data_ptr->saved_ded_rpt.scr_code,
              l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
  }
} /* l1_sc_process_wcdma_ded_id_srch */

/*===========================================================================

FUNCTION DED_SRCH_ID_CB

DESCRIPTION
  This callback function is used by the search driver to pass results
  of a Dedicated mode search back to L1.  This should get called at the end
  of every stage 2 search completion.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ded_srch_id_cb (gl1_wcdma_cell_ded_rpt *rpt, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32 freq_idx;

  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_DED_ID); 
  #endif

  if (rpt != &l1_sc_wcdma_data_ptr->ded_srch_rpt)
  {
    MSG_GERAN_ERROR_0_G("Invalid ded_srch rpt!");
    return;
  }

  freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;

  /* copy ded_srch_rpt to save_ded_rpt for task to use */
  l1_sc_wcdma_data_ptr->saved_ded_rpt = l1_sc_wcdma_data_ptr->ded_srch_rpt;

  MSG_GERAN_MED_2_G("WCDMA measurement successful on %d srchs=%d", rpt->UARFCN,l1_sc_wcdma_data_ptr->requested_ded_id_srchs);

  /*Decrement the requested_ded_id_srchs, this gives the number of CB
   *pending for dedicated id search
   */
  if (l1_sc_wcdma_data_ptr->requested_ded_id_srchs > 0) {
      l1_sc_wcdma_data_ptr->requested_ded_id_srchs--;
  }


  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  /* With message feature processing is run in non-interrupt context already.
     Also this function is called in a loop for each cell. Need to make sure
     that saved_ded_rpt doesn't get overwritten before the task context can run
     and process that cell */
  l1_sc_process_wcdma_ded_id_srch((void *)freq_idx, gas_id);

  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
  #else
  /* Do not set wcdma_srch_in_progress = FALSE until all ID searches and
   * reconfirmations in Dedicated mode have completed.
   * If an abort is issued, we need the flag to be TRUE so that the abort
   * operation is really performed.
   */
  if ((l1_sc_wcdma_data_ptr->reconf_cells.num_cells == 0) && (l1_sc_wcdma_data_ptr->requested_ded_id_srchs == 0))
  {
     l1_sc_set_gtow_srch_state_null (gas_id);
  }
  /* @@OG:
   * Mutex lock shared data in l1_sc_process_wcdma_ded_id_srch( ).
   */
  l1_scdrv_call_in_task(l1_sc_process_wcdma_ded_id_srch, (void *)freq_idx, gas_id);
  #endif

  if (l1_sc_wcdma_data_ptr->wcdma_srch.state != GtowStateDedicated)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled = FALSE;
  }

}


/*===========================================================================

FUNCTION DED_SRCH_ABORT_CB

DESCRIPTION
  This callback function is used by the search driver to let L1 know
  that the Dedicated mode search has been aborted.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ded_srch_abort_cb( boolean w_aborted_gap, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  
  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_DED_ABORT); 
  #endif
  
  MSG_GERAN_HIGH_0_G("WCDMA dedicated search aborted!");

  l1_sc_set_gtow_srch_state_null (gas_id);
  
  /*If WCDMA aborted the gap, try to re-schedule same frequency at next opportunity*/
  if(w_aborted_gap)
  {
    MSG_GERAN_HIGH_1_G("Next avail G2X ID search should be freq idx %d", l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset);

    /* subtract 1 modulo num-frequencies */
    l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset = (l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset + 
                                                            l1_sc_wcdma_data_ptr->rr_params.num_freqs - 1) %
                                                            l1_sc_wcdma_data_ptr->rr_params.num_freqs;
  }

  if (l1_sc_wcdma_data_ptr->wcdma_srch.state != GtowStateDedicated)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled = FALSE;
  }
}


/*===========================================================================

FUNCTION l1_sc_process_wcdma_ded_recon_srch

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent wcdma dedicated mode reconfirmation search.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  Any shared data with ISR needs to be mutex locked.

===========================================================================*/
static void l1_sc_process_wcdma_ded_recon_srch(uint16 freq_index, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  uint16 idx;
  uint16 num_cells;

  uint16 num_good_cells=0;

  static wcdma_dedicated_meas_T    best_cells[MAX_NUMBER_OF_DED_BEST_WCDMA_CELLS];

  gl1_wcdma_cell_ded_reconf_rpt*  rpt = &l1_sc_wcdma_data_ptr->saved_ded_recon_rpt;
  gl1_concman_config_params_type gl1_concman_config_params;
  cmgr_client_enum_t client = ((gas_id == GERAN_ACCESS_STRATUM_ID_1) ? CMGR_GSM1 : CMGR_GSM2);

  gl1_concman_config_params.type = CMGR_TRANSIENT_END;
  gl1_concman_config_params.scenario = CMGR_GERAN_PTM_G2W_GAP;
  gl1_concman_config_params.update_scenario = 0;
  gl1_concman_config_params.priority = cmgr_get_priority(client,CMGR_IRAT_MEASUREMENT);
  gl1_concman_config_params.start_timestamp = 0;
  gl1_concman_config_params.duration_msec = 0;
  gl1_concman_config_params.flow_ctrl_time_msec = 0;
  gl1_concman_scenario_config_request(gl1_concman_config_params, gas_id);

  /* add/update the new reconfirmations into the db of cells measured */
  
  update_stored_measurements_from_ded_reconf( rpt,
                                              freq_index,
                                              gas_id
                                            );

  /* get the number of cells - that go to RR*/
  num_cells = (rpt->num_cell_measurements > MAX_NUMBER_OF_DED_BEST_WCDMA_CELLS) ?
                    MAX_NUMBER_OF_DED_BEST_WCDMA_CELLS :  rpt->num_cell_measurements;


  /* reformat for RR - note unsorted - not in any order */
  for( idx=0; idx < num_cells; idx++ )
  {
      if(rpt->meas[idx].EcNo <= MIN_PWR_TO_BE_VALID_CELL)
      {
          MSG_GERAN_HIGH_2_G("G2W: scr_code:%d EcNo:%d is too low",rpt->meas[idx].scr_code, rpt->meas[idx].EcNo);
      }
      else
      {
          best_cells[num_good_cells].UARFCN            = rpt->UARFCN;
          best_cells[num_good_cells].Scrambling_Code   = rpt->meas[idx].scr_code;
          best_cells[num_good_cells].RSCP              = rpt->meas[idx].RSCP;
          best_cells[num_good_cells].RSSI              = rpt->meas[idx].rssi;
          best_cells[num_good_cells].Ec_Nox2           = rpt->meas[idx].EcNo;
          best_cells[num_good_cells].PN_Offset         = (uint16) (rpt->meas[idx].PN_pos >> 3); /* report as chips not chipx8 */

          num_good_cells++;
      }
  }

  num_cells = num_good_cells;

  l1_sc_forced_meas_event( freq_index, FORCE_MEAS_COMPLETED, gas_id);

  /*If no cell found, still send SURROUND_IRAT_MEAS_IND to GRR*/
  if(num_cells == 0)
  {
    L1_send_not_found_MPH_SURROUND_IRAT_MEAS_IND(l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].rat, 
                                                 l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_index].arfcn.UARFCN, 
                                                 gas_id);
  }
  else
  {
  /* now report the top 8 of these reconfirmed cells to RR */
    L1_send_MPH_SURROUND_WCDMA_MEAS_IND( num_cells, best_cells, gas_id );
  }

} /* l1_sc_process_wcdma_ded_recon_srch */


/*===========================================================================

FUNCTION DED_SRCH_RECONF_CB

DESCRIPTION
    This callback function will get called at the completion of every
    reconfirmation search that occurs in dedicated mode.  Reconfirmation
    searches occur after cell id searches (stage1/stage2) have completed.

    This function is called in isr context and saves stuff off so that
    processing can occur in task context.

    This function can be revisited to see if all processing can be done here
    in isr context rather than ship it up to task context.

DEPENDENCIES
  gl1_wcdmadrv_ded_mode_cfg_init is called to pass this function address to
  the driver.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void ded_srch_reconf_cb (gl1_wcdma_cell_ded_reconf_rpt *rpt, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint16 freq_idx;

  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2W_IRAT_SEARCH_END_DED_RECONFIRM); 
  #endif

  if (rpt != &l1_sc_wcdma_data_ptr->ded_recon_rpt)
  {
    MSG_GERAN_ERROR_0_G("Invalid ded reconf rpt!");
    return;
  }

  /* freq is the same as stage 2 id search */
  freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_ded_srch_freq_offset;

  /* copy entire ded_recon_rpt to saved_ded_recon_rpt for task to use */
  l1_sc_wcdma_data_ptr->saved_ded_recon_rpt = l1_sc_wcdma_data_ptr->ded_recon_rpt;

  MSG_GERAN_LOW_1_G("WCDMA reconfirmation successful %d!", rpt->UARFCN);

  l1_sc_process_wcdma_ded_recon_srch( freq_idx, gas_id);
  
  #ifdef FEATURE_GSM_TO_WCDMA_MSGR
  l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP_WAIT;
  #else
  /* indicate we are done with this sample ram capture */
  l1_sc_set_gtow_srch_state_null (gas_id);
  #endif

  if (l1_sc_wcdma_data_ptr->wcdma_srch.state != GtowStateDedicated)
  {
    l1_sc_wcdma_data_ptr->wcdma_srch.xfer_srchdrv_enabled = FALSE;
  }
}

/*===========================================================================

FUNCTION gl1_sc_irat_schedule_wcdma_meas_drx

DESCRIPTION

Schedule using L1 Frame Manager W ID search activity then call DRX Manager

DEPENDENCIES
  none.

RETURN VALUE
  TRUE if W ID can go be scheduled now

SIDE EFFECTS
  None
===========================================================================*/

static boolean gl1_sc_irat_schedule_wcdma_meas_drx(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32 current_FN = gl1_get_FN( gas_id );

#ifdef FEATURE_IRAT_RESERVE_SLEEP
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
  if(FM_STATE_USING_FRAMES == l1_fm_get_fm_table_state(gas_id,FM_PRI_CCCH) || 
     (l1_tskisr_blk->firmware_loaded
      #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
      && !l1_sc_g2x_timeline_opt_enable[gas_id]
      #endif
     ))
#else
  if(FM_STATE_USING_FRAMES == l1_fm_get_fm_table_state(gas_id,FM_PRI_CCCH))
#endif
  {
    MSG_GERAN_HIGH_1_G("WCDMA_IDSEARCH Paging still ongoing at FN: %d", current_FN);
    return FALSE;
  }
#endif /* FEATURE_IRAT_RESERVE_SLEEP */

  /* Conditions for performing W search right away with opportunistic trm request
     (as opposed to waiting and using reserve_at mechanism) */

  if(!gl1_hw_qta_gap_active(gas_id) &&
     !l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent &&
     ADD_FN(l1_sc_wcdma_data_ptr->id_search_fn, 1) != current_FN /* Avoid reserving for current frame +1 if we got just got denied*/
     #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
     && (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_ID_IN_TBF ||
         !l1_sc_g2x_timeline_opt_enable[gas_id])
     #endif
    )
  {
    l1_sc_wcdma_data_ptr->id_search_fn = current_FN;
    l1_fm_set_required_frame_number(FM_PRI_WCDMA_IDSEARCH, l1_sc_wcdma_data_ptr->id_search_fn , MAX_ID_SEARCH_FRAMES, gas_id);
    if(!gl1_sc_irat_can_wcdma_meas_go(gas_id))
    {
      l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = FALSE;
      MSG_GERAN_HIGH_1_G("WCDMA_IDSEARCH attempt failed at FN: %d", l1_sc_wcdma_data_ptr->id_search_fn);

    }
    else
    {
      return TRUE;
    }
  }

  if(!l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent)
  {
    l1_sc_wcdma_data_ptr->id_search_fn = ADD_FN(current_FN, WCDMA_IDSEARCH_ADVANCE_NOTICE);

    if(!gl1_msg_get_multi_sim_mode())
    {
#ifdef FEATURE_IRAT_RESERVE_SLEEP
      /* Give DRX manager the reservation FN */
      l1_sc_wcdma_data_ptr->drx_ref = gl1_drx_require_tick( l1_sc_wcdma_data_ptr->id_search_fn, gas_id );
#endif /* FEATURE_IRAT_RESERVE_SLEEP */
    }
    /* Reserve ID search with L1 FM */
    l1_fm_set_required_frame_number(FM_PRI_WCDMA_IDSEARCH, l1_sc_wcdma_data_ptr->id_search_fn , MAX_ID_SEARCH_FRAMES, gas_id);
    l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = TRUE;

    MSG_GERAN_HIGH_1_G("WCDMA_IDSEARCH reserved at FN: %d", l1_sc_wcdma_data_ptr->id_search_fn);
    return FALSE;
  }
  else if(IS_FRAME_NUM_LATER_OR_EQUAL(current_FN, l1_sc_wcdma_data_ptr->id_search_fn))
  {
      l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = FALSE;
      l1_sc_wcdma_data_ptr->drx_ref = 0;
      return gl1_sc_irat_can_wcdma_meas_go(gas_id);
  }

  MSG_GERAN_HIGH_2_G("WCDMA_IDSEARCH reserved at FN: %d current FN:%d" , l1_sc_wcdma_data_ptr->id_search_fn, current_FN);

  return FALSE;

}

/*===========================================================================

FUNCTION gl1_sc_irat_can_wcdma_meas_go

DESCRIPTION

Check using L1 Frame manager if W ID search activity can go ahead

DEPENDENCIES
  none.

RETURN VALUE
  TRUE if W ID can go be scheduled now

SIDE EFFECTS
  None
===========================================================================*/

static boolean gl1_sc_irat_can_wcdma_meas_go(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  if ((l1_sc_wcdma_data_ptr->wcdma_irat_active) && (l1_sc_frames_available(FM_PRI_WCDMA_IDSEARCH, MAX_ID_SEARCH_FRAMES, l1_sci_wcdma_aborted, 0, gas_id)))
  {
    MSG_GERAN_HIGH_0_G("WCDMA_IDSEARCH can go ahead");
    return TRUE;
  }
  else
  {

    l1_fm_clear_required_frames( FM_PRI_WCDMA_IDSEARCH, gas_id );
    MSG_GERAN_HIGH_1_G("WCDMA_IDSEARCH TRM denied l1_sc_wcdma_data_ptr->wcdma_irat_active:%d", l1_sc_wcdma_data_ptr->wcdma_irat_active);
    return FALSE;
  }

}
/* EOF */



