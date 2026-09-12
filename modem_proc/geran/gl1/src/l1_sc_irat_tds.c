
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

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_irat_tds.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
06/12/17   km        CR973889 Idle Timeline Opt - Sleep between PCH and IRAT
29/10/17   km        CR2133917 GL1 support for L+L; FR37926 non-DDS sub IRAT optimization
06/03/2016 ng        CR1022465 Clear drx_ref once IRAT activity gets scheduled
26/05/16   sk        CR1020757 Add DTF events for G2X IRAT scenarios
13/08/15   ws        CR526143 Klocwork P1 fix to address Buffer overflow in buffer of 
                     type ttdsirat_freq_list_type
30/12/15   js        CR956837 Set DRX reference only when IRAT activity is reserved
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
17/08/15   cja       CR890396 In G2T dedicated abort do cleanup before setting state to NULL
14/08/14   ws        CR889273 Fix Static declarations of variables which are not Dual data spaced
23/07/15   aga       CR878462 Avoid seting wcdma_srch_in_progress flag for TD-reconf during the page bursts
23/07/15   aga       CR876916 Avoid booking with L1FM IRAT activity for T and W if GFW is still loaded
15/07/15   cja       CR871031 Remove forcing gas id 1 for T2G/G2T (required for W+W).
15/07/15   aga/zf    CR865337: NULL2T IRAT support.
03/07/15   cja       CR866307 G2T and G2L in QTA gap
28/05/15   jk        CR845444:Remove THOR featurization while populating g2t_enter_ptr
27/05/15   zf        CR844122: Start TDS ID search if ID search frame is current or in the past
15/05/15   sp        CR838865: Fix compile errors on TA20 build.
23/03/15   jj        CR 811970 Mutex dead lock l1_sci_tick_wcdma 
                        L1_send_LTE_CPHY_IRAT_MEAS_G2L_DEINIT_REQ caused by merge  of CR  765859 
24/04/15   cja       CR800657 QTA gap for G2W IRAT
16/04/15   cja       CR823510 When abort in SCHEDULE_TDS state do not call rf meas exit
15/04/15   zf        CR823212 Don't do multiple RF enter/exit without pairing ones
14/04/15   aga       CR799530 Reserve W/TD/L IRAT ID search with TRM in advance
09/04/15   cgc       CR819956 Thor 2.x RF API change, G2W use MSGR
30/03/15   cja       CR800650 Tidy-up SC IRAT state machine
                     Initial version - files moved from l1_sc_irat.
===========================================================================*/


#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif

#ifdef FEATURE_GSM_TDS

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "l1_sc_irat.h"

#include "l1_sc.h"
#include "l1i.h"
#include "l1_utils.h"
#include "l1_drx.h"
#include "l1_fm.h"


#include "gpl1_grm_intf.h"

#include "tdsiratif.h"
#include "tds_l1_api.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT*/

#include "gl1_hw_g.h"

#include "gpl1_gprs_task.h"
#include "gl1_msg_wtr_hop.h"

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

/************************** function prototypes ***************************************/

static void g2t_irat_enter_rf(tdsirat_meas_rf_param_type *g2t_enter_ptr, uint32 uarfcn , gas_id_t gas_id );
static boolean gl1_sc_irat_schedule_tds_meas_drx(void ( *abort_callback )( void*,gas_id_t ), gas_id_t gas_id);
static boolean gl1_sc_irat_can_tds_meas_go(void ( *abort_callback )( void*,gas_id_t ), gas_id_t gas_id);

/*===========================================================================

FUNCTION l1_sc_tds_report

DESCRIPTION


DEPENDENCIES
  none

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_tds_report(gas_id_t gas_id)
{
  uint16 idx;
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  if(l1_sc_get_tds_irat_active(gas_id))
  {
    rr_l1_irat_measurements_u  no_irat_measurements;

    no_irat_measurements.utran_tdd.uarfcn        = 0;
    no_irat_measurements.utran_tdd.no_of_entries = 0;

    /* If this report is being sent then cell search is stopped due to
       QSEARCH_I not being met, meaning all RR timers are stopped.
       Need to ensure that all cell IDs are started again and reported
       */
    l1_sc_restart_cell_id_srch(gas_id);

    for (idx=0; idx < l1_sc_wcdma_data_ptr->rr_params.num_freqs; idx++)
    {
      if(RR_L1_IRAT_RAT_UTRAN_TDD == l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].rat)
      {
        no_irat_measurements.utran_tdd.uarfcn  = l1_sc_wcdma_data_ptr->rr_params.freq_list[idx].arfcn.UARFCN;
        L1_send_MPH_SURROUND_IRAT_MEAS_IND( RR_L1_IRAT_RAT_UTRAN_TDD, &no_irat_measurements, gas_id );
      }
    }
  }
}
/*===========================================================================

FUNCTION l1_sc_g2t_wait_state

DESCRIPTION
  This function handles the irat search wait state for G2T

===========================================================================*/
void l1_sc_g2t_wait_state(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id)
{
  uint16 freq_idx;
  
  
  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_RECONFIRM)
  {
    freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset;
  }
  else
  {
    l1_sc_set_gtow_srch_type(GTOW_SRCH_TYPE_ID, gas_id);
    freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;
  }
  
  
  MSG_GERAN_HIGH_3_G("Going to schedule G2T: srch_type %d, idx %d arfcn %d",
                   l1_sc_wcdma_data_ptr->wcdma_srch.srch_type,
                   freq_idx,
                   l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN);
  
  /* Book activity for GAP + 1 frame as GAP starts from next to next frame*/
  /* FN-2      FN-1            FN ....     FN+14     FN+15
  Current   sendAcq/start   GAPstart    GAPend    Call back from FM if activity doesn't complete */
  
  if(gl1_sc_irat_schedule_tds_meas_drx(l1_sci_wcdma_aborted,gas_id))
  {
    l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_UTRAN_TDD;
    
    MSG_GERAN_HIGH_0_G("Changing wcdms_srch.srch_state to GTOW_SRCH_STATE_SCHEDULE_TDS");
    
    /* Turn the SM and schedule the RX. Startup will be sent in next tick */
    l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_SCHEDULE_TDS;
    l1_sc_globals_ptr->wcdma_srch_in_progress = TRUE;
    L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT_TDS,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
    
    #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
    if(l1_sc_g2x_timeline_opt_enable[gas_id])
    {
      gl1_drx_require_next_tick(gas_id);
    }
    #endif
  }
}

/*===========================================================================

FUNCTION l1_sc_g2t_scehdule

DESCRIPTION
  This function handles the irat search wait state for G2T

===========================================================================*/
void l1_sc_g2t_schedule(l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr, l1_sc_globals_T *l1_sc_globals_ptr, gas_id_t gas_id)
{
   uint16 freq_idx;
   int32  startup_time = GL1_EXTRA_G2T_STARTUP_MARGIN;
   int32  rf_setup_time = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

   /* Gap start position is at next frame tick */
   /* Add Offset added by g2t_startup(mdsp_ftsm ) */
   l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs = startup_time + mdsp_ftsm(gas_id);

   if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type != GTOW_SRCH_TYPE_RECONFIRM)
   {
      tdsirat_acq_list_type       acq_list;

      // for acquisition, GL1 needs frequency.
      freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_id_freq_offset;

      /*TDS will need the whole Gap in TCH for 1 cell RECONF - send 4 anyway*/
      acq_list.num_of_freq    = 1;
      acq_list.freq[0].uarfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;

      // use stored value from previous acq
      acq_list.freq[0].ts0_position = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.ts0_position;

      acq_list.freq[0].last_lna_state = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_lna_state;

      // GL1 is doing id search again. if previous acq is failed or success, then mark it non so that TL1
      // can perform acquisition again
      if ((l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_acq_state == TDSIRAT_ACQ_FAILURE) ||
          (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_acq_state == TDSIRAT_ACQ_SUCCESS) ||
          (l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_acq_state == TDSIRAT_MEAS_DONE))
      {
         l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_acq_state = TDSIRAT_ACQ_NONE;
      }
      acq_list.freq[0].last_acq_state = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_acq_state;

      /* Gap Length = 15 G Frames - start delay (GL1_EXTRA_G2T_STARTUP_MARGIN) -
                      Rf setup time (rf_setup_time)- Cleanup processing time (GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS)*/
      l1_sc_wcdma_data_ptr->gap_info.gap_length_qs = (MAX_TDS_SEARCH_FRAMES * QS_PER_FRAME) - rf_setup_time - GL1_EXTRA_LTE_STARTUP_MARGIN
                                       - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS ;

      gl1_hw_calc_gap_values( &l1_sc_wcdma_data_ptr->gap_info,gas_id );

      MSG_GERAN_HIGH_3_G("G2T: gap start %dqs gap length %dqs current qs value %dqs",
                   l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs,
                   l1_sc_wcdma_data_ptr->gap_info.gap_length_qs,
                   gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
	  
	  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_ID); 
 	  #endif

      g2t_irat_enter_rf( &g2t_enter_rf_param, acq_list.freq[0].uarfcn, gas_id);

      /* Send a req message to LTE L1 */
      L1_send_TDS_IRAT_ACQ_REQ( &acq_list, gas_id);

      /*Just to ensure that we don't send a RECONF req AND a ID req within the same paging
       *block setup a var here that can be shared with the cell ID search.
       *Reconf. Searches are triggred in the 1st RX of a paging read
       */
      l1_sc_wcdma_data_ptr->reconf_req_fn = GSTMR_GET_FN_GERAN( gas_id);
   }
   else  /* Reconfirm search */
   {
      tdsirat_freq_list_type meas_list;
      uint16                 num_cells;
      uint16                 num_jdscells;
      uint8                  i = 0;

      freq_idx = l1_sc_wcdma_data_ptr->wcdma_srch.curr_reconfirm_freq_offset;
      num_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].num_cells;
      num_jdscells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells;
      
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

      MSG_GERAN_HIGH_3_G("srch_in_progress %d set to TRUE, active rat %d, freq_list rat %d ",
                             l1_sc_globals_ptr->wcdma_srch_in_progress,
                             l1_sc_globals_ptr->active_irat,
                             l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].rat);

      l1_sc_globals_ptr->wcdma_srch_in_progress = TRUE;
      L1_LOG_VAR(l1_sc_globals_ptr->wcdma_srch_in_progress,L1_FNAME_L1_SC_IRAT,L1_VAR_W_SRCH_IN_PROGRESS,gas_id);
      
      /**
       * Scenario: MEAS is scheduled to trigger on paging block tick,
       * before actually passing message to TL1, it may be possible
       * that RR sends update WCDMA_LIST which causes number of cells
       * 0. In this case, don't trigger MEAS and initialise state.
       */
      if((num_cells  == 0)|| (num_jdscells == 0))
      {
        MSG_GERAN_HIGH_3_G("G2T:re-conf  abort: ARFCN %d, num_cells %d  num_jdscells %d",
                    l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN, num_cells, num_jdscells);

        l1_sc_wcdma_data_ptr->wcdma_srch.abort = TRUE;
        return;
      }

      if( num_cells > TDSIRAT_MAX_TDS_CELLS_PER_FREQ )
      {
        num_cells = TDSIRAT_MAX_TDS_CELLS_PER_FREQ;
      }

      if( num_jdscells > TDSIRAT_MAX_TDS_CELLS_PER_FREQ )
      {
        num_jdscells = TDSIRAT_MAX_TDS_CELLS_PER_FREQ;
      }

      MSG_GERAN_HIGH_4_G("G2T:re-conf. search: ARFCN %d num_cells %d, num_jdscells %d, idx %d",
               l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN,  num_cells, num_jdscells, freq_idx);

      /*TDS will need the whole Gap in TCH for 1 cell RECONF - send 4 anyway*/
      meas_list.num_of_freq          = 1;
      meas_list.freq[0].uarfcn       = l1_sc_wcdma_data_ptr->rr_params.freq_list[freq_idx].arfcn.UARFCN;
      meas_list.freq[0].num_of_cells = num_cells;

      meas_list.freq[0].lna_state = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_lna_state;
      meas_list.freq[0].acq_state = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.last_acq_state;
      meas_list.freq[0].ts0_position = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.ts0_position;
      meas_list.freq[0].measuredFreq = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.measuredFreq;
      meas_list.freq[0].num_of_jds_cells = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.num_of_jds_cells;
      meas_list.freq[0].rssi = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].tdd_freq_params.rssi;

      for( i=0;i < num_cells;i++ )
      {
         meas_list.freq[0].cell[i].cpid = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].tds_cell.cell_parameter_id;
         meas_list.freq[0].cell[i].cell_position = l1_sc_wcdma_data_ptr->stored_meas[freq_idx].cell_params[i].tds_cell.cell_position;
      }

      /* Gap Length = 15 G Frames - start delay (GL1_EXTRA_G2T_STARTUP_MARGIN) -
                      Rf setup time (rf_setup_time)- Cleanup processing time (GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS)*/
      l1_sc_wcdma_data_ptr->gap_info.gap_length_qs = (MAX_TDS_SEARCH_FRAMES * QS_PER_FRAME) - rf_setup_time - GL1_EXTRA_LTE_STARTUP_MARGIN
                                         - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS ;

      gl1_hw_calc_gap_values( &l1_sc_wcdma_data_ptr->gap_info,gas_id );

      MSG_GERAN_HIGH_4_G("G2T: gap start %dqs gap length %dqs rf_setup_time %dqs,current qs value %dqs ",
                     l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs,
                     l1_sc_wcdma_data_ptr->gap_info.gap_length_qs,
                     rf_setup_time,
                     gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));
	  
	  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
      gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_RECONFIRM); 
	  #endif

      g2t_irat_enter_rf( &g2t_enter_rf_param, meas_list.freq[0].uarfcn, gas_id);

      /* Reconfirm attempt: Send a req message to LTE L1 */
      L1_send_TDS_IRAT_MEAS_REQ( &meas_list, gas_id);
   }

   /* Zero value refers to the next frame tick */
   gl1_hw_g2t_startup(startup_time, gas_id);

   /* Clean up offset = 15 G Frames (reduced by 1 immediately on same frame tick) - cleanup offset (mdsp_ftsm())
                        - rf setup time. No need to subtract cleanup processing time as it is allocated to TFW for cleanup
                 GFW should start looking for locks after cleanup processing time*/
   /* FN         ....             FN+13              FN+14
      GAPstart                                       GAPend
      Cleanupoffset = 14G -x      G-x(write mdsp)         */
   gl1_hw_g2t_cleanup( (((MAX_TDS_SEARCH_FRAMES) * QS_PER_FRAME) -
                               rf_setup_time  - mdsp_ftsm(gas_id)),gas_id);

   // state should be changed here after sending request
   l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_ACTIVE;
   l1_sc_globals_ptr->active_irat = RR_L1_IRAT_RAT_UTRAN_TDD;

   /*Clear drx_ref for this IRAT activity (G2T search- FM_PRI_TDS_ACQ) now */
   l1_sc_wcdma_data_ptr->drx_ref =0;

}
/*===========================================================================
FUNCTION l1_sc_wcdma_ded_ready_tds

DESCRIPTION
  Cut the TDS code into a new function call as its so huge !!!
  Function called when L1 has an open idle frame for a tds measurement.


DEPENDENCIES
  none

RETURN VALUE
  returns TRUE, if a tds measurement will use the idle frame.

SIDE EFFECTS
  None

===========================================================================*/
boolean l1_sc_wcdma_ded_ready_tds(boolean tds_perform_acq,
                                         uint16 f_off,
                                         uint16  ded_meas_count,
                                         gas_id_t gas_id)
{
    boolean  irat_tds_scheduled = FALSE;
    uint32   ccch_gap = 0;
    l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
    l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);
/*
** >800 lines of TDS code follows
*/
    if (l1_sc_wcdma_data_ptr->tds_irat_active)
    {
      /**
       * CR 350696: In JDS. TL1 does measurement based on frequency
       * but in MEAS_REQ, TL1 requires TDD cell information received
       * in ACQ_RSP.
       */
          uint16                                num_reconf_cells = l1_sc_wcdma_data_ptr->stored_meas[f_off].num_cells;
      tdsirat_acq_list_type                 acq_list;
      tdsirat_freq_list_type                meas_list;
      int32 rf_setup_time = 0;
      int32 gap_end_position = 0;
      uint8 lowest_rx_tn  = 0;
      uint8 highest_txrx_tn = 3;

      /** CR 350696 : Safe check on number of cells */
      if( num_reconf_cells > TDSIRAT_MAX_TDS_CELLS_PER_FREQ)
      {
          num_reconf_cells = TDSIRAT_MAX_TDS_CELLS_PER_FREQ;
      }
      /* Alternate - do 1 Cell ID then 1 Reconf.
      * If no reconfs then just try cell Ids.
      * If no cell Ids skip.
      */

      //debug
      MSG_GERAN_MED_5_G("G2T: GFN %d, reconf_req_fn %d l1_sc_wcdma_data_ptr->toggle_id_reconf %x num_reconf_cells %d ded_meas_count %d",
                         GSTMR_GET_FN_GERAN(gas_id),l1_sc_wcdma_data_ptr->reconf_req_fn,l1_sc_wcdma_data_ptr->toggle_id_reconf,num_reconf_cells,ded_meas_count);


      /*CR359646*/
      if(l1_sc_wcdma_data_ptr->tds_irat_active)
      {
        /* Safe check. This should not happen*/
        if((num_reconf_cells == 0) && (!tds_perform_acq))
        {
          MSG_GERAN_ERROR_0_G("G2T: Force GL1 to do ACQ");
          tds_perform_acq = TRUE;
        }
      }

      /* Release shared resource mutex
      * gap start point varies dependant on
      * which mode we're in
      */
      switch (l1_sc_globals_ptr->mode)
      {
       case L1SCModeDediSDCCh:

         /* CR 342226 */
         /* Calculate GAP value available for doing measurement  */
         ccch_gap = l1_fm_get_ccch_gap(gas_id);

         MSG_GERAN_HIGH_3_G("TDS SDCCH: FNmod102 FN %d GAP %d",frame_counters[gas_id].FNmod102,ccch_gap,0 );


         if(ccch_gap >= L1_SC_DURATION_WCDMA_DED_SRCH)
         {

           /* CR 371123*/
           {
             /* the gap_to_ccch activity is calculated from the previous frame
             therefore reduce gap by 1*/
             ccch_gap--;
             /* Allocate max 15 G Frames*/
             ccch_gap = (ccch_gap > MAX_TDS_SEARCH_FRAMES) ? MAX_TDS_SEARCH_FRAMES : ccch_gap;
           }
           /*
           Perfrorm acq when:
           1. all cells are not acquired ded_meas_count != 0.
           This won't be true in TDD as TL1 only detetcts 4 strongest cells irrespective of number of tdd cells
           provided by RR. Therefore l1_sc_wcdma_data_ptr->toggle_id_reconf is used to perform measurement

           2.  l1_sc_wcdma_data_ptr->toggle_id_reconf is TRUE in the begining so perform acq
           3. If no cells are detected (num_reconf_cells ==0), perfrom acq.
           */

           /*CR359646*/
           if(tds_perform_acq)
           {
             acq_list.num_of_freq               = 1;
             acq_list.freq[0].uarfcn            = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN;

             MSG_GERAN_HIGH_3_G("G2T: Acq on UARFCN %d, num_cells %d, f_off %d",
                   acq_list.freq[0].uarfcn,l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells, f_off );

             acq_list.freq[0].ts0_position   = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.ts0_position;
             acq_list.freq[0].last_lna_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_lna_state;

             if ((l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_ACQ_FAILURE) ||
                 (l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_ACQ_SUCCESS) ||
                 (l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_MEAS_DONE))
             {
               l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state = TDSIRAT_ACQ_NONE;
             }
             acq_list.freq[0].last_acq_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state;

             if( ded_meas_count > TDSIRAT_MAX_TDS_CELLS_PER_FREQ)
             {
                 ded_meas_count = TDSIRAT_MAX_TDS_CELLS_PER_FREQ;
             }
             /*
             * When Gl1 writes to mdsp,
             * then mdsp_ftm() is added to the
             * offset provided to the GFW. Since TL1 is passing gap info to
             * TFW transparently, therefore to make GFW and TFW in sync,
             * mdsp_ftsm() is added to the gap
             * start position.
             */
             /* Remaining time in current frame will be added in gl1_hw_calc_gap_values when GL1 latches TDSTMR*/
             l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs = (mdsp_ftsm(gas_id) + GL1_EXTRA_G2T_STARTUP_MARGIN);


             /* Have to reduce the gap length to allow for tune time of following Rx*/
             rf_setup_time = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

             /* work out gap end
              * CCCH gap frames - rf_setup_time
              */

             gap_end_position = (ccch_gap*QS_PER_FRAME)- rf_setup_time - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS;

             /*
              * Gap Length:  Gap end position
              *             - gap start position
              *             - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS (Time to process cleanup command by GFW)
              */

             l1_sc_wcdma_data_ptr->gap_info.gap_length_qs = (gap_end_position
                                         - l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs);

             MSG_GERAN_HIGH_3_G("G2T: l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs %d l1_sc_wcdma_data_ptr->gap_info.gap_length_qs %d rf_setup_time %d",
                      l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs, l1_sc_wcdma_data_ptr->gap_info.gap_length_qs, rf_setup_time);

             gl1_hw_calc_gap_values( &l1_sc_wcdma_data_ptr->gap_info, gas_id );

			 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
			 gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_DED_SDCCH_ID); 
			 #endif

             MSG_GERAN_HIGH_3_G("G2T: gap start %dqs gap length %dqs current qs value %dqs",
                      l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs,
                      l1_sc_wcdma_data_ptr->gap_info.gap_length_qs,
                      gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)));

             /* startup calc only cares about frame granularity */
             gl1_hw_g2t_startup(GL1_EXTRA_G2T_STARTUP_MARGIN, gas_id);

             g2t_irat_enter_rf( &g2t_enter_rf_param, acq_list.freq[0].uarfcn, gas_id);

             /* Reconfirm attempt: Send a req message to LTE L1 */
             L1_send_TDS_IRAT_ACQ_REQ( &acq_list, gas_id);

             /*
             * Clean up command offset :
             * Number of Idle Frames
             * - RF setup time
             * - mdsp_ftsm() as while writing cleanup command it is added to the offset. If mdsp_ftsm() is not subtracted from
             *   here, then cleanup command offset will be in the Rx slot and clean up command execution will be close to the Rx
             */
             gl1_hw_g2t_cleanup( gap_end_position  - mdsp_ftsm(gas_id), gas_id);

#if (defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG))
             l1_sc_wcdma_data_ptr->gap_info_debug.clean_pos_qs = gap_end_position;
#endif


             l1_sc_wcdma_data_ptr->toggle_id_reconf &= (~(1 << f_off));

             irat_tds_scheduled = TRUE;
             // state should be chnaged here
             // scenario...GL1 sends ACQ req and waiting for rsp..
             // on next idle frame GL1 calls this fun, since acq rsp hasn't arrived and toggle is set..
             // therefore GL1 will not send MEAS REQ..(num_reconf_cells ==0)
             // implies that GL1 will not send ACQ REQ / MEA REQ but state will be corrupted...
           }
           else
           {
             l1_sc_wcdma_data_ptr->toggle_id_reconf |= ( 1 << f_off);

             MSG_GERAN_HIGH_3_G("G2T: num_reconf_cells %d", num_reconf_cells, 0, 0);

             /* TDD it may be possible that RSCP field is not valid in the ACQ_RSP(RSCP is valid only if acq state is MAES_SUCCESS)
             In that case num_reconf_cells will be zero. To fix this, removed check on num_reconf_cells*/
            /*if( 0 != num_reconf_cells )*/
            {
               uint16 i;

               /*TDS will need the whole Gap in TCH for 1 cell RECONF - send 4 anyway*/
               meas_list.num_of_freq               = 1;
               meas_list.freq[0].uarfcn       = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN;


               MSG_GERAN_HIGH_3_G("G2T: Meas on UARFCN %d, num_cells %d, f_off %d",
                   acq_list.freq[0].uarfcn,l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells, f_off );

               meas_list.freq[0].num_of_cells = num_reconf_cells;
               meas_list.freq[0].lna_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_lna_state;
               meas_list.freq[0].acq_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state;
               meas_list.freq[0].ts0_position = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.ts0_position;
               meas_list.freq[0].measuredFreq = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.measuredFreq;
               meas_list.freq[0].num_of_jds_cells = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.num_of_jds_cells;
               meas_list.freq[0].rssi = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.rssi;
               for( i=0;i < num_reconf_cells;i++ )
               {
                 meas_list.freq[0].cell[i].cpid = l1_sc_wcdma_data_ptr->stored_meas[f_off].cell_params[i].tds_cell.cell_parameter_id;
                 meas_list.freq[0].cell[i].cell_position = l1_sc_wcdma_data_ptr->stored_meas[f_off].cell_params[i].tds_cell.cell_position;
               }

               /*
               * When Gl1 writes to mdsp,
               * then mdsp_ftm() is added to the
               * offset provided to the GFW. Since TL1 is passing gap info to
               * TFW transparently, therefore to make GFW and TFW in sync,
               * mdsp_ftsm() is added to the gap
               * start position.
               */
               /* Remaining time in current frame will be added in gl1_hw_calc_gap_values when GL1 latches TDSTMR*/
               l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs = (mdsp_ftsm(gas_id) + GL1_EXTRA_G2T_STARTUP_MARGIN);

               /* Have to reduce the gap length to allow for tune time of following Rx*/
               rf_setup_time = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

               /* work out gap end
                * CCCH gap Frames - rf_setup_time
                */

               gap_end_position = (ccch_gap*QS_PER_FRAME)- rf_setup_time - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS;

               /*
                * Gap Length:  Gap end position
                *             - gap start position
                *             - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS (Time to process cleanup command by GFW)
                */

               l1_sc_wcdma_data_ptr->gap_info.gap_length_qs = (gap_end_position
                                          - l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs);

               gl1_hw_calc_gap_values( &l1_sc_wcdma_data_ptr->gap_info, gas_id );

			   #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
			   gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_DED_SDCCH_RECONFIRM); 
			   #endif

               MSG_GERAN_HIGH_3_G("G2T: gap start %dqs gap length %dqs rf_setup_time %dqs",
                        l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs,
                        l1_sc_wcdma_data_ptr->gap_info.gap_length_qs,
                        rf_setup_time);

               /* startup calc only cares about frame granularity */
               gl1_hw_g2t_startup(GL1_EXTRA_G2T_STARTUP_MARGIN, gas_id);
               g2t_irat_enter_rf( &g2t_enter_rf_param, meas_list.freq[0].uarfcn, gas_id);

               /* Reconfirm attempt: Send a req message to LTE L1 */
               L1_send_TDS_IRAT_MEAS_REQ(&meas_list, gas_id);

               /*
               * Clean up command offset :
               * Number of Idle Frames
               * - RF setup time
               * - mdsp_ftsm() as while writing cleanup command it is added to the offset. If mdsp_ftsm() is not subtracted from
               *   here, then cleanup command offset will be in the Rx slot and clean up command execution will be close to the Rx
               */
               gl1_hw_g2t_cleanup( gap_end_position  - mdsp_ftsm(gas_id), gas_id);

               irat_tds_scheduled = TRUE;

#if (defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG))
             l1_sc_wcdma_data_ptr->gap_info_debug.clean_pos_qs = gap_end_position;
#endif

             }
           }

         }
         else
         {
           MSG_GERAN_HIGH_3_G("G2T: sdcch error - missed Idle frame: FN %d ccch_gap %d",
                   frame_counters[gas_id].FNmod26,ccch_gap,0 );
         }

         break;

       case L1SCModeDediDTM:
       case L1SCModeDediTCh:

         MSG_GERAN_HIGH_3_G("G2T: Ded frame FN %d idle FN %d",
                 frame_counters[gas_id].FNmod26,l1_sc_get_tch_idle_frame(gas_id),0 );

         // find out the highest remaining slot for DTM case
         if(l1_sc_globals_ptr->mode == L1SCModeDediDTM)
         {
            gpl1_gprs_calc_pkt_txfr_gap(&lowest_rx_tn, &highest_txrx_tn, gas_id);
            highest_txrx_tn  = (highest_txrx_tn > 3 ? 3 : highest_txrx_tn);
         }

         if( ((l1_sc_get_tch_idle_frame(gas_id)-1) - frame_counters[gas_id].FNmod26) >0)
         {
           /*
           Perfrorm acq when:
           1. all cells are not acquired ded_meas_count != 0.
           This won't be true in TDD as TL1 only detetcts 4 strongest cells irrespective of number of tdd cells
           provided by RR. Therefore l1_sc_wcdma_data_ptr->toggle_id_reconf is used to perform measurement

           2.  l1_sc_wcdma_data_ptr->toggle_id_reconf is TRUE in the begining so perform acq
           3. If no cells are detected (num_reconf_cells ==0), perfrom acq.
           */
           /*CR359646*/
           if(tds_perform_acq)
           {
             uint16 gap_start_pos;

             acq_list.num_of_freq               = 1;
             acq_list.freq[0].uarfcn            = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN;


             MSG_GERAN_HIGH_3_G("G2T: Acq on UARFCN %d, num_cells %d, f_off %d",
                   acq_list.freq[0].uarfcn,l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells, f_off );

             // use stored value from previous acq
             acq_list.freq[0].ts0_position = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.ts0_position;

             acq_list.freq[0].last_lna_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_lna_state;

             // GL1 is doing id search again. if previous acq is failed or success, then mark it non so that TL1
             // can perform acquisition again
             if ((l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_ACQ_FAILURE) ||
                 (l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_ACQ_SUCCESS) ||
                 (l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_MEAS_DONE))
             {
               l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state = TDSIRAT_ACQ_NONE;
             }
             acq_list.freq[0].last_acq_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state;

             if( ded_meas_count > MAX_WCDMA_CELLS_PER_FREQ )
             {
               ded_meas_count = MAX_WCDMA_CELLS_PER_FREQ;
             }

             /* In TCH this calcualtion is fixed - 1st work out how far to last TX burst before Idle
              * relative to the current frame tick(qs=0)
              */
             gap_start_pos = ( (((l1_sc_get_tch_idle_frame(gas_id)-2) - frame_counters[gas_id].FNmod26 )*QS_PER_FRAME)
                                              +((highest_txrx_tn + 1)*QS_PER_SLOT)  + GL1_EXTRA_G2T_STARTUP_MARGIN );
             /*
             * Add remaining qs ((QS_PER_FRAME - rd_qsymbol_count)) of
             * current frame to the gap start pos. When Gl1 writes to mdsp,
             * then mdsp_ftm() is added to the
             * offset provided to the GFW. Since TL1 is passing gap info to
             * TFW transparently, therefore to make GFW and TFW in sync,
             * mdsp_ftsm() is added to the gap
             * start position.
             */
             /* Remaining time in current frame will be added in gl1_hw_calc_gap_values when GL1 latches TDSTMR*/
             l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs = gap_start_pos + (mdsp_ftsm(gas_id));


             /* Have to reduce the gap length to allow for tune time of following Rx*/
             rf_setup_time = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

             /* work out gap end
              * Idle Frame + No. of slots to lowest timeslot - rf_setup_time + FTSM
              */

             gap_end_position = ((l1_sc_get_tch_idle_frame(gas_id) - frame_counters[gas_id].FNmod26 )*QS_PER_FRAME)
                                  - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS - rf_setup_time + mdsp_ftsm(gas_id);

             /*
              * Gap Length:  Gap end position
              *             - gap start position
              *             - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS (Time to process cleanup command by GFW)
              */

             l1_sc_wcdma_data_ptr->gap_info.gap_length_qs = (gap_end_position
                                         - l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs);

             MSG_GERAN_HIGH_3_G("G2T: l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs %d gap_start_pos %d l1_sc_wcdma_data_ptr->gap_info.gap_length_qs %d",
                      l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs, gap_start_pos, l1_sc_wcdma_data_ptr->gap_info.gap_length_qs);

             gl1_hw_calc_gap_values( &l1_sc_wcdma_data_ptr->gap_info, gas_id );

			 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
			 gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_DED_DTM_TCH_ID); 
			 #endif

             MSG_GERAN_HIGH_3_G("G2T: gap start %dqs gap length %dqs rf_setup_time %dqs",
                      l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs,
                      l1_sc_wcdma_data_ptr->gap_info.gap_length_qs,
                      rf_setup_time);

             /* startup calc only cares about frame granularity */
             gl1_hw_g2t_startup(gap_start_pos, gas_id);

             g2t_irat_enter_rf( &g2t_enter_rf_param, acq_list.freq[0].uarfcn, gas_id);

             /* Reconfirm attempt: Send a req message to LTE L1 */
             L1_send_TDS_IRAT_ACQ_REQ( &acq_list, gas_id);

             /*
             * Clean up command offset :
             * Number of frames from current frame to idle frame
             * - RF setup time
             * - mdsp_ftsm() as while writing cleanup command it is added to the offset. If mdsp_ftsm() is not subtracted from
             *   here, then cleanup command offset will be in the Rx slot and clean up command execution will be close to the Rx
             */
             gl1_hw_g2t_cleanup(gap_end_position - mdsp_ftsm(gas_id), gas_id);

#if (defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG))
             l1_sc_wcdma_data_ptr->gap_info_debug.clean_pos_qs = gap_end_position;
#endif


             l1_sc_wcdma_data_ptr->toggle_id_reconf &= (~(1 << f_off));

             irat_tds_scheduled = TRUE;
             // state should be chnaged here
             // scenario...GL1 sends ACQ req and waiting for rsp..
             // on next idle frame GL1 calls this fun, since acq rsp hasn't arrived and toggle is set..
             // therefore GL1 will not send MEAS REQ..(num_reconf_cells ==0)
             // implies that GL1 will not send ACQ REQ / MEA REQ but state will be corrupted...
           }
           else
           {
             l1_sc_wcdma_data_ptr->toggle_id_reconf |= ( 1 << f_off);

             MSG_GERAN_HIGH_3_G("G2T: num_reconf_cells %d", num_reconf_cells, 0, 0);

             /* TDD it may be possible that RSCP field is not valid in the ACQ_RSP(RSCP is valid only if acq state is MAES_SUCCESS)
             In that case num_reconf_cells will be zero. To fix this, removed check on num_reconf_cells*/
            /*if( 0 != num_reconf_cells )*/
            {
               uint16 i, gap_start_pos_reconf;

               /*TDS will need the whole Gap in TCH for 1 cell RECONF - send 4 anyway*/
               meas_list.num_of_freq               = 1;
               meas_list.freq[0].uarfcn       = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN;


               MSG_GERAN_HIGH_3_G("G2T: Meas on UARFCN %d, num_cells %d, f_off %d",
                   acq_list.freq[0].uarfcn,l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells, f_off );

               //meas_list.freq[0].num_of_cells = 4;
               meas_list.freq[0].num_of_cells = num_reconf_cells;

               meas_list.freq[0].lna_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_lna_state;
               meas_list.freq[0].acq_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state;
               meas_list.freq[0].ts0_position = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.ts0_position;
               meas_list.freq[0].measuredFreq = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.measuredFreq;
               meas_list.freq[0].num_of_jds_cells = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.num_of_jds_cells;
               meas_list.freq[0].rssi = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.rssi;

               for( i=0;i < num_reconf_cells;i++ )
               {
                 meas_list.freq[0].cell[i].cpid = l1_sc_wcdma_data_ptr->stored_meas[f_off].cell_params[i].tds_cell.cell_parameter_id;
                 meas_list.freq[0].cell[i].cell_position = l1_sc_wcdma_data_ptr->stored_meas[f_off].cell_params[i].tds_cell.cell_position;
               }

               /* In TCH this calcualtion is fixed - 1st work out how far to last TX burst before Idle
                * relative to the current frame tick(qs=0)
                */
               gap_start_pos_reconf = ( (((l1_sc_get_tch_idle_frame(gas_id)-2) - frame_counters[gas_id].FNmod26 )*QS_PER_FRAME)
                                              +((highest_txrx_tn + 1)*QS_PER_SLOT)  + GL1_EXTRA_G2T_STARTUP_MARGIN);
               /*
               * Add remaining qs ((QS_PER_FRAME - rd_qsymbol_count)) of
               * current frame to the gap start pos. When Gl1 writes to mdsp,
               * then mdsp_ftm() is added to the
               * offset provided to the GFW. Since TL1 is passing gap info to
               * TFW transparently, therefore to make GFW and TFW in sync,
               * mdsp_ftsm()is added to the gap
               * start position.
               */
               /* Remaining time in current frame will be added in gl1_hw_calc_gap_values when GL1 latches TDSTMR*/
               l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs = gap_start_pos_reconf + (mdsp_ftsm(gas_id));


               /* Have to reduce the gap length to allow for tune time of following Rx*/
               rf_setup_time = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

               /* work out gap end
                * Idle Frame + No. of slots to lowest timeslot - rf_setup_time + FTSM
                */

               gap_end_position = ((l1_sc_get_tch_idle_frame(gas_id) - frame_counters[gas_id].FNmod26 )*QS_PER_FRAME)
                                    - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS - rf_setup_time + mdsp_ftsm(gas_id);

               /*
                * Gap Length:  Gap end position
                *             - gap start position
                *             - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS (Time to process cleanup command by GFW)
                */

               l1_sc_wcdma_data_ptr->gap_info.gap_length_qs = (gap_end_position
                                         - l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs);

               gl1_hw_calc_gap_values( &l1_sc_wcdma_data_ptr->gap_info, gas_id );

			   #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
			   gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_DED_DTM_TCH_RECONFIRM); 
			   #endif

               MSG_GERAN_HIGH_3_G("G2T: gap start %dqs gap length %dqs rf_setup_time %dqs",
                        l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs,
                        l1_sc_wcdma_data_ptr->gap_info.gap_length_qs,
                       rf_setup_time);

               /* startup calc only cares about frame granularity */
               gl1_hw_g2t_startup(gap_start_pos_reconf, gas_id);
               g2t_irat_enter_rf( &g2t_enter_rf_param, meas_list.freq[0].uarfcn, gas_id);

               /* Reconfirm attempt: Send a req message to LTE L1 */
               L1_send_TDS_IRAT_MEAS_REQ(&meas_list, gas_id);

               /*
               * Clean up command offset :
               * Number of frames from current frame to idle frame
               * - RF setup time
               * - mdsp_ftsm() as while writing cleanup command it is added to the offset. If mdsp_ftsm() is not subtracted from
               *   here, then cleanup command offset will be in the Rx slot and clean up command execution will be close to the Rx
               */
               gl1_hw_g2t_cleanup(gap_end_position  - mdsp_ftsm(gas_id), gas_id);

               irat_tds_scheduled = TRUE;

#if (defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG))
             l1_sc_wcdma_data_ptr->gap_info_debug.clean_pos_qs = gap_end_position;
#endif

             }
           }

         }
         else
         {
           MSG_GERAN_HIGH_3_G("G2T: ded error - missed Idle frame: FN %d idle FN %d",
                   frame_counters[gas_id].FNmod26,l1_sc_get_tch_idle_frame(gas_id),0 );
         }

        break;
        case L1SCModeGprsTrans:
          {

            /*CR347049*/

            /**
             * Calculate highest tx/rx slot and lowest rx slot
             */
            gpl1_gprs_calc_pkt_txfr_gap(&lowest_rx_tn, &highest_txrx_tn, gas_id);

            /**
             * Allocate 12 or more G slots to T
             */
            highest_txrx_tn  = (highest_txrx_tn > 3 ? 3 : highest_txrx_tn);

            MSG_GERAN_HIGH_3_G("G2T: Xfer FNmod26 %d highest_tx_tn %d",frame_counters[gas_id].FNmod26,highest_txrx_tn,0 );

            if( ( ((25-1) - frame_counters[gas_id].FNmod26) >0 )
                &&( gl1_msg_pdch_block_active(gas_id)  )
              )
            {
             /*
             Perfrorm acq when:
             1. all cells are not acquired ded_meas_count != 0.
             This won't be true in TDD as TL1 only detetcts 4 strongest cells irrespective of number of tdd cells
             provided by RR. Therefore l1_sc_wcdma_data_ptr->toggle_id_reconf is used to perform measurement

             2.  l1_sc_wcdma_data_ptr->toggle_id_reconf is TRUE in the begining so perform acq
             3. If no cells are detected (num_reconf_cells ==0), perfrom acq.
             */
            // if ((num_reconf_cells ==0) || ( (l1_sc_wcdma_data_ptr->toggle_id_reconf & (1 << f_off)) && (0 != ded_meas_count)))
             /*CR359646*/
             if(tds_perform_acq)
             {
               uint16 gap_start_pos_xfer;

               acq_list.num_of_freq     = 1;
               acq_list.freq[0].uarfcn  = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN;

               MSG_GERAN_HIGH_3_G("G2T: Acq on UARFCN %d, num_cells %d, f_off %d",
                                    acq_list.freq[0].uarfcn,l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells, f_off );

               acq_list.freq[0].ts0_position = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.ts0_position;
               acq_list.freq[0].last_lna_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_lna_state;

               // GL1 is doing id search again. if previous acq is failed or success, then mark it non so that TL1
               // can perform acquisition again
               if ((l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_ACQ_FAILURE) ||
                   (l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_ACQ_SUCCESS) ||
                   (l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state == TDSIRAT_MEAS_DONE))
               {
                 l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state = TDSIRAT_ACQ_NONE;
               }
               acq_list.freq[0].last_acq_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state;

               if( ded_meas_count > MAX_WCDMA_CELLS_PER_FREQ )
               {
                 ded_meas_count = MAX_WCDMA_CELLS_PER_FREQ;
               }

               /* work out gap start: No. of slots remaining to pre-idle frame +
                * No. of slots to highest tx timeslot
                */

               gap_start_pos_xfer =  (((25-2) - frame_counters[gas_id].FNmod26 )*QS_PER_FRAME)
                                                +(( highest_txrx_tn + 1 )*QS_PER_SLOT)
                                                + GL1_EXTRA_G2T_STARTUP_MARGIN;

               /*
               * Add remaining qs ((QS_PER_FRAME - rd_qsymbol_count)) of
               * current frame to the gap start pos. When Gl1 writes to mdsp,
               * then mdsp_ftm() is added to the
               * offset provided to the GFW. Since TL1 is passing gap info to
               * TFW transparently, therefore to make GFW and TFW in sync,
               * mdsp_ftsm()is added to the gap
               * start position.
               */
               /* Remaining time in current frame will be added in gl1_hw_calc_gap_values when GL1 latches TDSTMR*/
               l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs = gap_start_pos_xfer + (mdsp_ftsm(gas_id));

               /* Have to reduce the gap length to allow for tune time of following Rx*/
               rf_setup_time = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

               /* work out gap end
                * Idle Frame + No. of slots to lowest timeslot - rf_setup_time + FTSM
                */

               gap_end_position = ((25 - frame_counters[gas_id].FNmod26 )*QS_PER_FRAME)+(( LOWEST_DL_SLOT(lowest_rx_tn) )*QS_PER_SLOT)
                                   - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS - rf_setup_time + mdsp_ftsm(gas_id);

               /*
                * Gap Length:  Gap end position
                *             - gap start position
                *             - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS (Time to process cleanup command by GFW)
                */

               l1_sc_wcdma_data_ptr->gap_info.gap_length_qs = (gap_end_position
                                             - l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs);

               MSG_GERAN_HIGH_3_G("G2T: l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs %d gap_start_pos %d l1_sc_wcdma_data_ptr->gap_info.gap_length_qs %d",
                                  l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs, gap_start_pos_xfer, l1_sc_wcdma_data_ptr->gap_info.gap_length_qs);

               gl1_hw_calc_gap_values( &l1_sc_wcdma_data_ptr->gap_info, gas_id );

			   #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
			   gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_GPRSTRANS_ID); 
               #endif

               MSG_GERAN_HIGH_3_G("G2T: gap start %dqs gap length %dqs rf_setup_time %dqs",
                                         l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs,
                                         l1_sc_wcdma_data_ptr->gap_info.gap_length_qs,
                                         rf_setup_time);



               gl1_hw_g2t_startup(gap_start_pos_xfer, gas_id);
               g2t_irat_enter_rf( &g2t_enter_rf_param, acq_list.freq[0].uarfcn, gas_id);


               /* Reconfirm attempt: Send a req message to LTE L1 */
               L1_send_TDS_IRAT_ACQ_REQ( &acq_list, gas_id);

               /*
               * Clean up command offset :
               * Number of frames from current frame to idle frame
               * + Till RX Slot in frame after idle frame
               * - RF setup time
               * - mdsp_ftsm() as while writing cleanup command it is added to the offset. If mdsp_ftsm() is not subtracted from
               *   here, then cleanup command offset will be in the Rx slot and clean up command execution will be close to the Rx
               */
               /* Need to check this!!!! */
               gl1_hw_g2t_cleanup( gap_end_position  - mdsp_ftsm(gas_id), gas_id);

#if (defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG))
               l1_sc_wcdma_data_ptr->gap_info_debug.clean_pos_qs = gap_end_position;
#endif


               l1_sc_wcdma_data_ptr->toggle_id_reconf &= (~(1 << f_off));

               irat_tds_scheduled = TRUE;
               // state should be chnaged here
               // scenario...GL1 sends ACQ req and waiting for rsp..
               // on next idle frame GL1 calls this fun, since acq rsp hasn't arrived and toggle is set..
               // therefore GL1 will not send MEAS REQ..(num_reconf_cells ==0)
               // implies that GL1 will not send ACQ REQ / MEA REQ but state will be corrupted...
             }
             else
             {
               l1_sc_wcdma_data_ptr->toggle_id_reconf |= ( 1 << f_off);

               MSG_GERAN_HIGH_3_G("G2T: num_reconf_cells %d", num_reconf_cells, 0, 0);

               /* TDD it may be possible that RSCP field is not valid in the ACQ_RSP(RSCP is valid only if acq state is MAES_SUCCESS)
               In that case num_reconf_cells will be zero. To fix this, removed check on num_reconf_cells*/
              {
                  uint16 i, gap_start_pos_xfer_reconf;

                 /*TDS will need the whole Gap in TCH for 1 cell RECONF - send 4 anyway*/
                 meas_list.num_of_freq    = 1;
                 meas_list.freq[0].uarfcn = l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].arfcn.UARFCN;


                 MSG_GERAN_HIGH_3_G("G2T: Meas on UARFCN %d, num_cells %d, f_off %d",
                     meas_list.freq[0].uarfcn,l1_sc_wcdma_data_ptr->rr_params.freq_list[f_off].num_cells, f_off );

                 meas_list.freq[0].num_of_cells = num_reconf_cells;
                 meas_list.freq[0].lna_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_lna_state;
                 meas_list.freq[0].acq_state = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.last_acq_state;
                 meas_list.freq[0].ts0_position = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.ts0_position;
                 meas_list.freq[0].measuredFreq = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.measuredFreq;
                 meas_list.freq[0].num_of_jds_cells = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.num_of_jds_cells;
                 meas_list.freq[0].rssi = l1_sc_wcdma_data_ptr->stored_meas[f_off].tdd_freq_params.rssi;

                 for( i=0;i < num_reconf_cells;i++ )
                 {
                   meas_list.freq[0].cell[i].cpid = l1_sc_wcdma_data_ptr->stored_meas[f_off].cell_params[i].tds_cell.cell_parameter_id;
                   meas_list.freq[0].cell[i].cell_position = l1_sc_wcdma_data_ptr->stored_meas[f_off].cell_params[i].tds_cell.cell_position;
                 }

                 /* work out gap start: No. of slots remaining to pre-idle frame +
                  * No. of slots to highest tx timeslot
                  */
                 /**
                  * CR347573: Correct gap start position.
                  */
                 gap_start_pos_xfer_reconf = (((25-2) - frame_counters[gas_id].FNmod26 )*QS_PER_FRAME)
                                                +(( highest_txrx_tn + 1 )*QS_PER_SLOT)
                                                + GL1_EXTRA_G2T_STARTUP_MARGIN;

                 /*
                 * Add remaining qs ((QS_PER_FRAME - rd_qsymbol_count)) of
                 * current frame to the gap start pos. When Gl1 writes to mdsp,
                 * then mdsp_ftm() is added to the
                 * offset provided to the GFW. Since TL1 is passing gap info to
                 * TFW transparently, therefore to make GFW and TFW in sync,
                 * mdsp_ftsm()is added to the gap
                 * start position.
                 */
                 /* Remaining time in current frame will be added in gl1_hw_calc_gap_values when GL1 latches TDSTMR*/
                 l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs = gap_start_pos_xfer_reconf + (mdsp_ftsm(gas_id));

                 /* Have to reduce the gap length to allow for tune time of following Rx*/
                 rf_setup_time = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

                 /* work out gap end
                  * Idle Frame + No. of slots to lowest timeslot - rf_setup_time + FTSM
                  */

                 gap_end_position = ((25 - frame_counters[gas_id].FNmod26 )*QS_PER_FRAME)+(( LOWEST_DL_SLOT(lowest_rx_tn) )*QS_PER_SLOT)
                                   - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS - rf_setup_time + mdsp_ftsm(gas_id);

                 /*
                  * Gap Length:  Gap end position
                  *             - gap start position
                  *             - GL1_HW_G2T_CLEANUP_CMD_PROCESS_QS (Time to process cleanup command by GFW)
                  */
                 l1_sc_wcdma_data_ptr->gap_info.gap_length_qs = (gap_end_position
                                             - l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs);

                 gl1_hw_calc_gap_values( &l1_sc_wcdma_data_ptr->gap_info, gas_id );

				 #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
				 gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_START_GPRSTRANS_RECONFIRM); 
				 #endif

                 MSG_GERAN_HIGH_3_G("G2T: gap start %dqs gap length %dqs rf_setup_time %dqs",
                          l1_sc_wcdma_data_ptr->gap_info.gap_start_pos_qs,
                          l1_sc_wcdma_data_ptr->gap_info.gap_length_qs,
                          rf_setup_time);


                 gl1_hw_g2t_startup(gap_start_pos_xfer_reconf, gas_id);
                 g2t_irat_enter_rf( &g2t_enter_rf_param, meas_list.freq[0].uarfcn, gas_id);

                 /* Reconfirm attempt: Send a req message to LTE L1 */
                 L1_send_TDS_IRAT_MEAS_REQ(&meas_list, gas_id);

                 /*
                 * Clean up command offset :
                 * Number of frames from current frame to idle frame
                 * + Till RX Slot in frame after idle frame
                 * - RF setup time
                 * - mdsp_ftsm() as while writing cleanup command it is added to the offset. If mdsp_ftsm() is not subtracted from
                 *   here, then cleanup command offset will be in the Rx slot and clean up command execution will be close to the Rx
                 */
                 gl1_hw_g2t_cleanup(gap_end_position - mdsp_ftsm(gas_id), gas_id);

                 irat_tds_scheduled = TRUE;

#if (defined(FEATURE_GSM_TDS) && defined(FEATURE_GSM_TDS_DEBUG))
                 l1_sc_wcdma_data_ptr->gap_info_debug.clean_pos_qs = gap_end_position;
#endif

               }
             }
            }
            else
            {
                 MSG_GERAN_HIGH_1_G("G2T: pkt txfr error - missed Idle frame: FN %d ",
                          frame_counters[gas_id].FNmod26);
            }
           break;
          }

       default:
             MSG_GERAN_HIGH_0_G("G2T:No configuration for this mode");
         break;
      }
  }/*l1_sc_wcdma_data_ptr->tds_irat_active*/
  /* huge amounts of TDS > 800 lines */

  return irat_tds_scheduled;

} /* l1_sc_wcdma_ded_ready_tds */


#ifdef FEATURE_G2T_SRCH_LOGGING

/* Logs the responses sent to G by Tds for the ACQ resquest during G2T*/
void l1_g2t_irat_log_acq_resp( const tdsirat_freq_list_type*  acq_rsp_ptr, gas_id_t gas_id )
{

    G2T_IRAT_ACQ_RESP_LOG_PACKET_type   *acq_log_ptr;
    uint8 i,j;
    uint8 log_packets_num_of_freq, log_packets_num_of_cells;

    static uint32 previous_fn = 0, current_fn = 0 ;  /* static_dual_spaced_ignore */
    static uint16 previous_qs = 0, current_qs = 0;   /* static_dual_spaced_ignore */

    log_packets_num_of_freq = acq_rsp_ptr->num_of_freq;
    acq_log_ptr = (G2T_IRAT_ACQ_RESP_LOG_PACKET_type *)log_alloc(G2T_IRAT_ACQ_RESP_LOG_PACKET, G2T_IRAT_ACQ_RESP_LOG_PACKET_LEN(log_packets_num_of_freq) );
    if (  acq_log_ptr != NULL)
    {
        acq_log_ptr->version = 1;
        current_fn = GSTMR_GET_FN_GERAN(gas_id);
        current_qs = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id)  );

        acq_log_ptr->num_of_freq =  acq_rsp_ptr->num_of_freq;
        
        for (i = 0 ; i < log_packets_num_of_freq ; i++) {
             acq_log_ptr->freq[i].uarfcn = acq_rsp_ptr->freq[i].uarfcn;
             acq_log_ptr->freq[i].lna_state = acq_rsp_ptr->freq[i].lna_state;
             acq_log_ptr->freq[i].measuredFreq = acq_rsp_ptr->freq[i].measuredFreq;
             acq_log_ptr->freq[i].num_of_cells = acq_rsp_ptr->freq[i].num_of_cells;
             acq_log_ptr->freq[i].ts0_position = acq_rsp_ptr->freq[i].ts0_position;
             acq_log_ptr->freq[i].acq_state = acq_rsp_ptr->freq[i].acq_state;

             log_packets_num_of_cells = acq_rsp_ptr->freq[i].num_of_cells ;
             for (j = 0 ; j< log_packets_num_of_cells ; j++) {

                  acq_log_ptr->freq[i].cell[j].cpid = acq_rsp_ptr->freq[i].cell[j].cpid;
                  acq_log_ptr->freq[i].cell[j].rscp = acq_rsp_ptr->freq[i].cell[j].rscp;
                  acq_log_ptr->freq[i].cell[j].cell_position = acq_rsp_ptr->freq[i].cell[j].cell_position;

            }

        }

       acq_log_ptr->time_since_last_measurement_ms     =     (((( current_fn - previous_fn) * QS_PER_FRAME ) + (current_qs - previous_qs) ) * 0.000923 );

       log_commit(acq_log_ptr);
       previous_fn = current_fn;
       previous_qs = current_qs;
    }

}

/* Logs the responses sent to G by Tds for the power measure resquest during G2T*/
void l1_g2t_irat_log_meas_resp(const tdsirat_freq_list_type*  meas_rsp_ptr, gas_id_t gas_id)
{

    G2T_IRAT_MEAS_RESP_LOG_PACKET_type   *meas_log_ptr;
    uint8 i,j;
    uint8 log_packets_num_of_freq, log_packets_num_of_cells;

    static uint32 previous_fn = 0, current_fn = 0 ;  /* static_dual_spaced_ignore */
    static uint16 previous_qs = 0, current_qs = 0;   /* static_dual_spaced_ignore */

    log_packets_num_of_freq = meas_rsp_ptr->num_of_freq;
    meas_log_ptr = (G2T_IRAT_MEAS_RESP_LOG_PACKET_type *)log_alloc(G2T_IRAT_MEAS_RESP_LOG_PACKET, G2T_IRAT_MEAS_RESP_LOG_PACKET_LEN(log_packets_num_of_freq) );
    if (  meas_log_ptr != NULL)
    {
       meas_log_ptr->version = 1;

      current_fn = GSTMR_GET_FN_GERAN(gas_id);
      current_qs = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id) );

      meas_log_ptr->num_of_freq = meas_rsp_ptr->num_of_freq;
        for (i = 0 ; i < log_packets_num_of_freq ; i++) {
             meas_log_ptr->freq[i].uarfcn = meas_rsp_ptr->freq[i].uarfcn;
             meas_log_ptr->freq[i].lna_state = meas_rsp_ptr->freq[i].lna_state;
             meas_log_ptr->freq[i].measuredFreq = meas_rsp_ptr->freq[i].measuredFreq;
             meas_log_ptr->freq[i].num_of_cells = meas_rsp_ptr->freq[i].num_of_cells;
             meas_log_ptr->freq[i].ts0_position = meas_rsp_ptr->freq[i].ts0_position;
             meas_log_ptr->freq[i].acq_state = meas_rsp_ptr->freq[i].acq_state;

             log_packets_num_of_cells = meas_rsp_ptr->freq[i].num_of_cells ;
             for (j = 0 ; j< log_packets_num_of_cells ; j++) {

                  meas_log_ptr->freq[i].cell[j].cpid = meas_rsp_ptr->freq[i].cell[j].cpid;
                  meas_log_ptr->freq[i].cell[j].rscp = meas_rsp_ptr->freq[i].cell[j].rscp;
                  meas_log_ptr->freq[i].cell[j].cell_position = meas_rsp_ptr->freq[i].cell[j].cell_position;
             }

          }
       meas_log_ptr->time_since_last_measurement_ms     =    (( (( current_fn - previous_fn) * QS_PER_FRAME ) + (current_qs - previous_qs) ) * 0.000923 );

       log_commit(meas_log_ptr);

       previous_fn = current_fn;
       previous_qs = current_qs;
    }
}

#endif /*FEATURE_G2T_SRCH_LOGGING*/

/*===========================================================================

FUNCTION l1_sc_tds_process_acq_results

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent TDS id search. It uses as a template
  l1_sc_process_wcdma_id_srch_results().

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

  This function handles both WCDMA, LTE and TDS cells.

DEPENDENCIES
  none

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void  l1_sc_tds_process_acq_results( const tdsirat_freq_list_type*  acq_rsp_ptr, gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

#ifdef  FEATURE_GSM_TDS_DSDA
  if( FALSE != acq_rsp_ptr->freq[0].ignore )
  {
    if(0 == acq_rsp_ptr->freq[0].num_of_cells)
    {
      MSG_GERAN_ERROR_0_G(" De-sense! acq result ignored");

      return;
    }
    else
    {
      MSG_GERAN_ERROR_1_G(" De-sense! acq result not ignored num cells %d ", acq_rsp_ptr->freq[0].num_of_cells );
    }
  }
#endif

  /* Process response from TDD when id_state is active.
    In a case where FM has already aborted the request because allocated frames have elapsed,
    FM l1_sci_wcdma_aborted clean the wcdma_srch.id_state */
  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED)
  {
  	if(l1_sc_globals_ptr->mode == L1SCModeDediSDCCh)
  		{
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_SDCCH_ID); 
#endif
  		}
	else if((l1_sc_globals_ptr->mode == L1SCModeDediDTM) || (l1_sc_globals_ptr->mode == L1SCModeDediTCh))
  		{
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_DTM_TCH_ID); 
#endif
  		}
	else if(l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
  		{
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_GPRSTRANS_ID); 
#endif
  		}
	
    l1_sc_ded_tds_acq_results(acq_rsp_ptr, gas_id);
  }
  else
  {
    #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
	gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_IDLE_ID); 
	#endif
	
    l1_sc_idle_tds_acq_results(acq_rsp_ptr, gas_id);
  }

  #ifdef FEATURE_G2T_SRCH_LOGGING
     l1_g2t_irat_log_acq_resp(acq_rsp_ptr, gas_id);
  #endif /*FEATURE_G2T_SRCH_LOGGING*/

}


/*===========================================================================

FUNCTION l1_sc_tds_process_meas_results

DESCRIPTION
  This function is called to from the L1 task to process the
  results of the most recent TDS reconfirmation search.
  This function is based on l1_sc_process_wcdma_recon_srch_results()

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
void  l1_sc_tds_process_meas_results( const tdsirat_freq_list_type*  meas_rsp_ptr, gas_id_t gas_id  )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T	 *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

#ifdef  FEATURE_GSM_TDS_DSDA
  if( FALSE != meas_rsp_ptr->freq[0].ignore )
  {
    MSG_GERAN_ERROR_0_G(" De-sense! meas result ignored");

    return;
  }
#endif

  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
      if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED)
      {
		  if(l1_sc_globals_ptr->mode == L1SCModeDediSDCCh)
			  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_SDCCH_RECONFIRM); 
#endif
			  }
		  else if((l1_sc_globals_ptr->mode == L1SCModeDediDTM) || (l1_sc_globals_ptr->mode == L1SCModeDediTCh))
			  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_DTM_TCH_RECONFIRM); 
#endif
			  }
		  else if(l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
			  {
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_GPRSTRANS_RECONFIRM); 
#endif
			  }
		 
         l1_sc_ded_tds_meas_results(meas_rsp_ptr, gas_id);
      }
      else
      {
         #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
	     gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_IDLE_RECONFIRM); 
	     #endif
		 
         l1_sc_idle_tds_meas_results(meas_rsp_ptr, gas_id);
      }

  }

#ifdef FEATURE_G2T_SRCH_LOGGING
  l1_g2t_irat_log_meas_resp(meas_rsp_ptr, gas_id);
#endif /*FEATURE_G2T_SRCH_LOGGING*/

}

/*===========================================================================

FUNCTION l1_sc_tds_process_abort

DESCRIPTION

===========================================================================*/
void l1_sc_tds_process_abort ( gas_id_t gas_id )
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T    *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);

  MSG_GERAN_HIGH_3_G("G2T: l1_sc_tds_process_abort srch_state %d, srch_type %d, rx in progress %d",
           l1_sc_wcdma_data_ptr->wcdma_srch.srch_state, l1_sc_wcdma_data_ptr->wcdma_srch.srch_type, l1_sc_globals_ptr->receive_in_progress);

  gl1_irat_mutex_lock( gas_id);
  if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_state != GTOW_SRCH_STATE_NULL)
  {
     l1_sc_wcdma_data_ptr->wcdma_srch.srch_state = GTOW_SRCH_STATE_CLEANUP;

     if (l1_sc_wcdma_data_ptr->wcdma_srch.srch_type == GTOW_SRCH_TYPE_DEDICATED)
     {
        #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
		gl1_qsh_event_notify(gas_id,GL1_QSH_EVENT_G2T_IRAT_SEARCH_END_DED_ABORT); 
        #endif

        MSG_GERAN_HIGH_2_G("G2T Abort ded state, pending startup %d, cleanup %d",mdsp_decrement_startup_time_offset_flag[gas_id], mdsp_decrement_cleanup_time_offset_flag[gas_id]);

        /* If g2x startup not sent yet then cancel startup/cleanup, otherwise issue cleanup on next frame tick with time 0*/
        if (mdsp_decrement_startup_time_offset_flag[gas_id])
        {
           mdsp_decrement_startup_time_offset_flag[gas_id] = FALSE;
           mdsp_decrement_cleanup_time_offset_flag[gas_id] = FALSE;
        }

        l1_g2t_gap_end(gas_id);
     }
  }

  /* Ensure that flag to confirm ABORT_CNF has been received is reset */
  l1_sc_wcdma_data_ptr->l1_g2t_abort_cmd_sent = FALSE;
  gl1_irat_mutex_unlock( gas_id);
}

/*===========================================================================

FUNCTION l1_g2t_gap_end

DESCRIPTION
       Called at the end of a G2T gap either triggers g2x cleanup
       or complets gap (rf meas exit, go to NULL)

===========================================================================*/
void l1_g2t_gap_end(gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

   /* If g2x cleanup needs to be sent - force it now */
   if (mdsp_decrement_cleanup_time_offset_flag[gas_id] == TRUE)
   {
      l1_sc_wcdma_data_ptr->l1_tds_issue_cleanup = TRUE;
   }
   else
   /* If g2x cleanup done, gap complete */
   {
      g2t_irat_exit_rf( &g2t_enter_rf_param, gas_id);
      l1_sc_set_gtow_srch_state_null (gas_id);
   }
   l1_sc_stop_wcdma_receive(gas_id);
}

/*===========================================================================

FUNCTION g2t_irat_enter_rf

DESCRIPTION


DEPENDENCIES
  none.

RETURN VALUE


SIDE EFFECTS
  None
===========================================================================*/

static void g2t_irat_enter_rf(tdsirat_meas_rf_param_type *g2t_enter_ptr, uint32 uarfcn, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  rfm_meas_common_param_type*   g2x_common_ptr;
  trm_set_irat_band_device_info target_device;
  sys_band_class_e_type         bandin;

  g2x_common_ptr = gl1_get_irat_rf_structure_ptr(gas_id);    

  /*
  ** use common enter function, same for all g techs to set content of gl1_irat_rf_common_structure
  */
  if (l1_sc_x2g_ta_irat_gap(gas_id))
  {
     if ( gl1_hw_qta_gap_active(gas_id))
     {
        /* Use QTA RF structure, source RAT parameters already complete */
        MSG_GERAN_HIGH_1_G("G2T in QTA gap, src tech %d",g2x_common_ptr->header.source_tech);
        g2x_common_ptr->script_type    = RFM_MEAS_PROGRAM_STARTUP;  /* Anything except NULL2X */
        g2x_common_ptr->scenario_type  = RFM_MEAS_SCENARIO_IRAT;
     }
     else
     {
        rfm_meas_common_init_params( g2x_common_ptr);
        MSG_GERAN_HIGH_0_G("G2T NULL2X gap");      
        g2x_common_ptr->source_param.gsm_params.sub_id = gas_id;
        g2x_common_ptr->script_type = RFM_MEAS_PROGRAM_NULL2X;
        g2x_common_ptr->scenario_type  = RFM_MEAS_SCENARIO_IRAT;
     }
  }
  else
  {
     g2x_set_common_rf_enter(g2x_common_ptr, gas_id);
  }

  bandin = tdsl1_api_get_tds_band( uarfcn );
  target_device = grm_get_irat_rf_dev_from_grm_info( bandin,
                                                     l1_tskisr_blk->client_id,
                                                     gas_id );

  /*
  ** Target specific params
  */
  g2t_enter_ptr->src_rat        = TDSIRAT_RAT_GSM;
  g2t_enter_ptr->startup_rf_idx = G2X_STARTUP_BURST_IDX ;
  g2t_enter_ptr->cleanup_rf_idx = G2X_CLEANUP_BURST_IDX ;
  g2t_enter_ptr->p_comm_rf_params = g2x_common_ptr;  /* the common rf pointer */
  g2t_enter_ptr->target_device  = target_device.primary_device;
  g2t_enter_ptr->uarfcn         = uarfcn;

  g2t_enter_ptr->rxlm_rx_buffer = gl1_hw_get_tds_rxlm_buf0_idx();

  MSG_GERAN_MED_0_G("G2T RF meas enter");
  tdsirat_rf_enter(g2t_enter_ptr);        /* call the external rf enter */
}

/*===========================================================================

FUNCTION g2t_irat_exit_rf

DESCRIPTION


DEPENDENCIES
  none.

RETURN VALUE


SIDE EFFECTS
  None
===========================================================================*/

void g2t_irat_exit_rf(tdsirat_meas_rf_param_type *g2t_enter_ptr, gas_id_t gas_id )
{
    MSG_GERAN_MED_0_G("G2T RF meas exit");
    tdsirat_rf_exit(g2t_enter_ptr);        /* call the external rf exit */
}

/*===========================================================================

FUNCTION g2t_populate_startup_params

DESCRIPTION


DEPENDENCIES
  none.

RETURN VALUE


SIDE EFFECTS
  None
===========================================================================*/
void  g2t_populate_startup_params(tdsirat_startup_param_type  *ptr_rf_startup, gas_id_t gas_id)
{
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];

  gsm_l1_tds_gap_info_type *gap_info_ptr= &l1_sc_wcdma_data_ptr->gap_info;

  ptr_rf_startup->gap_start_subfn         = gap_info_ptr->gap_start_subfn;
  ptr_rf_startup->gap_start_pos           = gap_info_ptr->gap_start_pos;

  ptr_rf_startup->gap_end_subfn           = gap_info_ptr->gap_end_subfn;
  ptr_rf_startup->gap_end_pos             = gap_info_ptr->gap_end_pos;
  /*
  ** copy in the previously filled in g2t_enter_rf_param struct
  */
  ptr_rf_startup->rf_param                = g2t_enter_rf_param;


  MSG_GERAN_MED_1_G("TDSCDMA_IRAT_STARTUP_REQ enter TDS State %d",l1_sc_get_tds_state(gas_id));

  /* to be moved ?? */
//  tds_state_local = l1_sc_get_tds_state(gas_id);
//  l1_sc_set_tds_state(++tds_state_local,gas_id);

} /* g2t_populate_startup_params */

/*===========================================================================

FUNCTION gl1_sc_irat_schedule_tds_meas_drx

DESCRIPTION

Schedule using L1 Frame Manager TD ID search activity then call DRX Manager

DEPENDENCIES
  none.

RETURN VALUE
  TRUE if TD ID can go be scheduled now

SIDE EFFECTS
  None
===========================================================================*/

static boolean gl1_sc_irat_schedule_tds_meas_drx(void ( *abort_callback )( void*,gas_id_t ), gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK *l1_tskisr_blk  = &l1_tsk_buffer[gas_id];
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  uint32 current_FN = gl1_get_FN( gas_id );

#ifdef FEATURE_IRAT_RESERVE_SLEEP
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
   /* No need to request/reserve if paging is still ongoing*/
  if(FM_STATE_USING_FRAMES == l1_fm_get_fm_table_state(gas_id,FM_PRI_CCCH) ||
     (l1_tskisr_blk->firmware_loaded
      #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
      && !l1_sc_g2x_timeline_opt_enable[gas_id]
      #endif
     ))
#else
  if( FM_STATE_USING_FRAMES == l1_fm_get_fm_table_state(gas_id,FM_PRI_CCCH))
#endif
  {
    MSG_GERAN_HIGH_1_G("TDS_IDSEARCH Paging still ongoing at FN: %d", current_FN);
    return FALSE;
  }
#endif /* FEATURE_IRAT_RESERVE_SLEEP */

 /* If possible attempt to do it ASAP */
  if(!gl1_hw_qta_gap_active(gas_id) && 
     !l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent &&
     ADD_FN(l1_sc_wcdma_data_ptr->id_search_fn, 1) != current_FN /* Avoid reserving for current frame +1 if we got just got denied*/
     #ifdef FEATURE_GSM_G2X_TIMELINE_REDUCTION
     && !l1_sc_g2x_timeline_opt_enable[gas_id]
     #endif
    )
  {
    /* Book activity for GAP + 1 frame as GAP starts from next to next frame*/
             /* FN-2      FN-1            FN ....     FN+14     FN+15
                Current   sendAcq/start   GAPstart    GAPend    Call back from FM if activity doesn't complete */

    l1_sc_wcdma_data_ptr->id_search_fn = current_FN;
    l1_fm_set_required_frame_number(FM_PRI_TDS_ACQ, l1_sc_wcdma_data_ptr->id_search_fn , MAX_TDS_SEARCH_FRAMES + 1, gas_id);
    if(!gl1_sc_irat_can_tds_meas_go(abort_callback,gas_id))
    {
      l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = FALSE;
      MSG_GERAN_HIGH_1_G("TDS_IDSEARCH attempt failed at FN: %d", l1_sc_wcdma_data_ptr->id_search_fn);

    }
    else
    {
      return TRUE;
    }
  }

  if(!l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent)
  {
    l1_sc_wcdma_data_ptr->id_search_fn = ADD_FN(current_FN, TDS_IDSEARCH_ADVANCE_NOTICE);

    if(!gl1_msg_get_multi_sim_mode())
    {
#ifdef FEATURE_IRAT_RESERVE_SLEEP
      /* Give DRX manager the reservation FN */
      l1_sc_wcdma_data_ptr->drx_ref = gl1_drx_require_tick( l1_sc_wcdma_data_ptr->id_search_fn, gas_id ); /* Wake-up early?*/
#endif /* FEATURE_IRAT_RESERVE_SLEEP */
    }
    /* Reserve ID search with L1 FM */
    l1_fm_set_required_frame_number(FM_PRI_TDS_ACQ, l1_sc_wcdma_data_ptr->id_search_fn , MAX_TDS_SEARCH_FRAMES + 1, gas_id);
    l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = TRUE;

    MSG_GERAN_HIGH_1_G("TDS_IDSEARCH reserved at FN: %d", l1_sc_wcdma_data_ptr->id_search_fn);
    return FALSE;
  }
  else if(IS_FRAME_NUM_LATER_OR_EQUAL(current_FN, l1_sc_wcdma_data_ptr->id_search_fn))
  {
      l1_sc_wcdma_data_ptr->l1_irat_reserve_at_sent = FALSE;
      l1_sc_wcdma_data_ptr->drx_ref = 0;
      return gl1_sc_irat_can_tds_meas_go(abort_callback, gas_id);
  }

  MSG_GERAN_HIGH_2_G("TDS_IDSEARCH reserved at FN: %d current FN:%d" , l1_sc_wcdma_data_ptr->id_search_fn, current_FN);

  return FALSE;

}

/*===========================================================================

FUNCTION gl1_sc_irat_can_tds_meas_go

DESCRIPTION

Check using L1 Frame manager if TD ID search activity can go ahead

DEPENDENCIES
  none.

RETURN VALUE
  TRUE if TD ID can go be scheduled now

SIDE EFFECTS
  None
===========================================================================*/

static boolean gl1_sc_irat_can_tds_meas_go(void ( *abort_callback )( void*,gas_id_t ), gas_id_t gas_id)
{
  l1_sc_wcdma_data_t  *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  l1_sc_globals_T     *l1_sc_globals_ptr    = gl1_ms_switch_l1_sc_globals_store(gas_id);



  if ((l1_sc_wcdma_data_ptr->tds_irat_active) && (l1_sc_frames_available(FM_PRI_TDS_ACQ, MAX_TDS_SEARCH_FRAMES+1, abort_callback, 0, gas_id)))
  {
    MSG_GERAN_HIGH_0_G("TDS_IDSEARCH can go ahead");
    return TRUE;
  }
  else
  {
    l1_sc_globals_ptr->wcdma_srch_in_progress = FALSE;
    l1_fm_clear_required_frames( FM_PRI_TDS_ACQ, gas_id );
    MSG_GERAN_HIGH_1_G("TDS_IDSEARCH TRM denied l1_sc_wcdma_data_ptr->tds_irat_active:%d", l1_sc_wcdma_data_ptr->tds_irat_active);
    return FALSE;
  }

}

#endif /* FEATURE_GSM_TDS*/

/* EOF */



