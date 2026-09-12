
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              SCE: L1 IDLE and FINDBCCH

GENERAL DESCRIPTION
   This module contains surrounding cell code for GPRS idle mode.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/gpl1_gprs_sc_idle_meas.c#1 $

when       who       what, where, why
--------   -----  ----------------------------------------------------------
17/02/15   ak     CR792387 MPH_GPRS_SURROUND_UPDATE_REQ fixes
14/01/15   nk     CR779625 Do not start SC tick after BCCH stop if any activity is ONGOING
12/01/15   sjv    CR756154 Enhance Band Registration for Activities 
12/12/14   pa     CR763859:Handling of RxD clients in GL1 TRM structures.
05/12/14   fm     CR766852 Schedule the next G2X TA action, when the scheduled one is cancelled
19/11/14   aga    CR755567 Restart SCE after BCCH stopped 
03/10/14   aga    CR732455 Changes required to tune away framework for TH 1.0 single sim build 
12/09/14   nm     CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
29/08/14   cah      CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
19/08/14   cah    CR711290 gpl1_gprs_update_bcch_stop using old api for sending band release to TRM.
31/07/14   cos    CR697355 Bolt 2.0 GL1 updates required for enabling RxD cal with the new DR-DSDS TRM apis
04/07/14   cs     CR688003 GRM functions moved to gpl1_grm_intf.c
02/07/14   aga    CR675214: G2T TA feature and GTA/G2W TA re-worked optimized solution 
30/05/14   ws     CR657674 Data + MMS Phase 1
22/05/14   cah    Bolt 2.0: FEATURE_GERAN_DRDSDS: Ncell & irat measurement scheduling changes.
10-03-16   tjw    Renamed UNUSED to __UNUSED_ARG to avoid name clash.
13/02/09   ap     Lint fixes for high Warning
28/07/98   hv     Added KxMutex support
06/06/07   ws     CR 118908 - Don't reset nc_mode in l1_sc_init() since this
                  is called when we reselect to PBCCH cell stopping NC2 reporting
28/11/06   dv     Lint cleanup - var next_bsic removed as redundant
29/09/05   og     Merge to fixes of CR74437 and CR70332 from the mainline branch
                  Fixes inter RAT TC and 42.4.8.4.5 respectively.
02/06/05   og     Removal of header-header includes.
30/12/04   bm     Fix for L1 Idle abort, in a race condition resulted in
                  SCE STOP, STOP, STRAT, STOP resulting in stuck in IDLE
18/08/04   kf     changed gpl1_gprs_update_bcch() to handle si 7&8 aquire
28/07/04   ws     Removed Inter-RAT Feature switches
02/06/04   kf     changed gpl1_gprs_sc_idle_post_sort_process as per cr 44600.
12/05/04   gw     Send SURROUND_UPDATE_FAILURE_IND if BSIC changes while SU
                  is pending.  If cell enters top 6 after being out for more
                  than 30s then set bsic as unknown.
23/04/04   ws     Corrected Lint Error's and Warnings
09/03/04   ws     Cleared trans.bcch_in_progress when stopping bcch receives
02/25/04   gw     Changes to support GSM L1 lint clean up.
26/11/03   ws     Lint cleanup
06/11/03   gfr    Support for quad-band.
20/10/03   bk     Moved MSG_HIGH into else statement to prevent NULL pointer access
04/09/03   gw     Changes to update BCCH/PBCCH functions to call GSM function
                  next_cell_bcch() when in 51 Idle rather than
                  gpl1_gprs_next_cell_idle().
04/09/03   kf     Changed the number of ncells measured per page period
                  from 11 to 6.
09/02/03   sw     Allow WCDMA Reconfirmation when in Page Reorganise if
                  LIST_SEARCH_IN_PAGING_BLOCK is defined.
08/28/03   gw     Changes to support new result type returned by l1_sc_read_sb
                  functions.
08/08/03   kf     changed msg_errror to err_fatal.
06/08/03   kf     lint changes.
14/07/03   gw     Removed ASSERTs.
14/07/03   gw     Added check for SCE being active in next_cell function.
14/07/03   kf     added cell->pbcch == TRUE check to pbcch_done
11/07/03   gw     Changes to speed up reselection.
11/07/03   sw     Moved WCDMA ID search trigger with zero length BA list
                  to l1_sc_pwr.c
10/07/03   kf     changed gpl1_gprs_sc_receive_pbcch
10/07/03   kf     added field to L1_send_MPH_SURROUND_UPDATE_FAILURE_IND
09/07/03   gw     Added sending of MPH_SURROUND_FAILURE_IND.
08/07/03   gw     GRR/L1 SCE cleanup. Changes to ncell BCCH decoding.
06/06/03   kf     changes for reduced number of rxlev measurements per paging block
29/05/03   kf     ncell pbcch read changse
14/05/03   kf     changed order of sch/bcch reading
02/05/03   gw     Merged in power transfer changes from branch.
01/05/03   ag     Updates to ncell pbcch reading.
28/04/03   gw     Changes for preserving ncell power measure info.
16/04/03   sw     Added initial changes for WCDMA neighbour cell search
                  during packet idle on PCCCH
04/09/03   gw     Added support for transferring ncell power measurement info
                  between L1 modes.
04/04/03   gw     Added code to explicitly reset ncell info when entering and
                  leaving GPRS Idle Mode.  Later this will be replaced with
                  code to support transferring ncell info between modes.
02/04/03   gw     Changes to reflect modifications in monscan module.
21/03/03   kf     changed num_meas from 3 to 1
20/03/03   pjr    Changed l1_sc_mon_scan_abort to l1_sc_mon_scan_abort_gprs
                  in gpl1_gprs_sc_leave_idle.
12/03/03   pjr    Removed call to gpl1_gprs_next_cell_idle() from
                  gpl1_gprs_sci_paging_block_tick, NORMAL PAGING and modified
                  num_pwr_meas_to_schedule in PAGING REORG.
28/02/03   kf     Added support for ncell pbcch reading
24/02/03   ws     Added FEATURE_GPRS_TBF_SUSPEND around gpl1_gprs_do_bcch_52
05/02/03   ws     Merged SC changes from branch
16/01/03   AG     Add BCCH_MASK_4_13NORM
10/01/03   gw     Changed some SCE function calls to reflect renaming of some
                  BA list functions done as part of SCE clean up.
08/10/02   kf     Initial Version.
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"


#include "l1_isr.h"
#include "l1_sc.h"
#include "l1i.h"
#include "l1_utils.h"
#include "msg.h" // F3 Messages
#include "gl1_sys_params.h"
#include "l1_sc_int.h"
#include "gpl1_gprs_isr.h" // Needed to disable LIST_SEARCH_IN_PAGING_BLOCK
#include "err.h"

#include "gpl1_dual_sim.h"
#include "gpl1_grm_intf.h"
#include "gpl1_gprs_utils.h"
#include "l1_task.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
extern void next_cell_bcch( gas_id_t gas_id );
extern const uint16 bcch_mask_clear[];

/* For acquisition top six are split into two groups
 * Top cells    - rank < TOP_CELLS_THRESH
 * Bottom cells - rank >= TOP_CELLS_THRESH
 *
 * A tick is 52 frames
 */
#define TOP_CELLS_THRESH_GPRS               3
#define TICKS_PER_FCB_TOP_CELLS_GPRS       40 /* 10 sec */
#define TICKS_PER_FCB_BOTTOM_CELLS_GPRS   120 /* 30 sec */
#define TICKS_PER_SB_GPRS                 120 /* 30 sec */
#define TICKS_PER_FCB_DIFF_GPRS           (TICKS_PER_FCB_BOTTOM_CELLS_GPRS - TICKS_PER_FCB_TOP_CELLS_GPRS)
#define MAX_TRIES_SB_GPRS    2

void gpl1_gprs_update_bcch( ARFCN_T                             arfcn,
                            grr_l1_surround_cell_si_requested_t si_requested,
                            normal_extended_t                   si13_normal_or_ext, gas_id_t gas_id )
{
   cell_T* cell = l1_sc_balist_get_cell_by_arfcn( arfcn, gas_id );
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();

   if ( (cell == NULL) || !(cell->bs.bsic_known) || (cell->rank >= 6) )
   {
      /* failure case */
      if(cell == NULL)
      {
         MSG_GERAN_ERROR_1_G("Cannot read BCCH for cell arfcn %d as cell not found in ba list",arfcn.num);
      }
      else
      {
         /* rank not valid if bsic_known is 0 as cell is not present in sorted_cells list */
         if(!cell->bs.bsic_known)
         {
            MSG_GERAN_MED_1_G("Cannot read BCCH for cell arfcn: %d, bsic_known=0", arfcn.num);
         }
         else
         {
            MSG_GERAN_MED_3_G("Cannot read BCCH for cell arfcn: %d, bsic_known=%d, rank=%d",
               arfcn.num, cell->bs.bsic_known, cell->rank );
         }
      }
      L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( arfcn, FALSE , gas_id);
   }
   else
   {
      if (si_requested == GRR_L1_DECODE_SI3_AND_SI13)
      {
         if (si13_normal_or_ext == BCCH_NORMAL)
         {
            cell->bcch_mask |= BCCH_MASK_3_13NORM;
            cell->bcch_clear_mask |= BCCH_MASK_3_13NORM;
         }
         else
         {
            cell->bcch_mask |= BCCH_MASK_3_13EXT;
            cell->bcch_clear_mask |= BCCH_MASK_3_13EXT;
         }
      }
      else if ( si_requested == GRR_L1_DECODE_SI7_ONLY )
      {
           cell->bcch_mask       |= BCCH_MASK_7;
           cell->bcch_clear_mask |= BCCH_MASK_78;
      }
      else
      {
            cell->bcch_mask |= BCCH_MASK_3;
            cell->bcch_clear_mask |= BCCH_MASK_34;
      }



      /* get BCCh asap */
      cell->sb_count_to_bcch = BCCH_DO_NOW;

      MSG_GERAN_LOW_1_G("SURROUND ARFCN %d ",cell->bs.arfcn.num);

      MSG_GERAN_MED_3_G("SU arfcn=%d, req=%d, norm_ext=%d", cell->bs.arfcn.num, si_requested, si13_normal_or_ext);

      /* If we're in Idle mode call next cell function now
       * to avoid waiting until next paging block.
       */
      if ( !l1_sc_globals_ptr->receive_in_progress)
      {
         if( l1_sc_globals_ptr->mode==L1SCModeIdle )
         {
            next_cell_bcch(gas_id);
         }
      }
   }
}

/* Stop ncell BCCH and PBCCH decodes on all ncells. */
void gpl1_gprs_update_bcch_stop( boolean send_ind, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint32  i, next_suspension_FN, next_resume_FN;
   cell_T *cell;
   boolean ind;
   gpl1_g2x_ta_priority_t pri;
   uint8 sorted_cell_count_to_iterate;

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
   sorted_cell_count_to_iterate = l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr;
#else
   sorted_cell_count_to_iterate = l1_sc_globals_ptr->sorted_cell_count;
#endif

   /* Loop though sorted cells and set all BCCHs
    * to DONT_DO.
    */
   for (i=0; i < sorted_cell_count_to_iterate; i++)
   {
      cell = l1_sc_globals_ptr->sorted_cells[i];

      /* intlock / intfree to avoid ISR sending IND as well as task
       * that might confuse GRR
       */
      GL1_ISR_SAVE_LOCK(gas_id);
      ind = (cell->sb_count_to_bcch == BCCH_DO_NOW) ? TRUE : FALSE;
      cell->sb_count_to_bcch = BCCH_DONT_DO;

      /*Cancel NBCCH activity if planned/scheduled*/
      if( gpl1_g2x_ta_is_act_planned(G2X_TA_PRI_SERV_BCCH,gas_id) &&
         ARFCNS_EQUAL(cell->bs.arfcn, trans[gas_id].bcch_cell->bs.arfcn))
      {
        MSG_GERAN_MED_1_G("gpl1_gprs_update_bcch_stop :Cancel planned NBCCH activity ARFCN %d", 
                           cell->bs.arfcn.num);
        
        gpl1_g2x_ta_cancel_table_field(G2X_TA_PRI_SERV_BCCH,gas_id);

        /* BCCH update is stopped, so GL1 should schedule the next action */
        gpl1_g2x_ta_scheduler(gas_id);
        
        /* If SCE is stopped restart it, unless other tune-away activity is planned */
        if(TRUE == gpl1_get_stop_sc_tick_trans(gas_id))
        {
          if(TRUE == gpl1_g2x_ta_is_activty_running( &pri,gas_id))
          {
            MSG_GERAN_MED_1_G("Activity %d is ONGOING", pri);
          }
          else
          {
            if((gpl1_g2x_ta_next_planned_suspension(&next_suspension_FN,&next_resume_FN,gas_id)))
            {
              if(IS_FRAME_NUM_LATER(next_suspension_FN,ADD_FN(GSTMR_GET_FN_GERAN(gas_id),NPL1_FN_OFFSET + TEMP_DELAY_OS)))
              {
                gpl1_start_sc_tick_trans(gas_id);
              }
            }
            else
            {
              gpl1_start_sc_tick_trans(gas_id);
            }
          }
        }
      }

      GL1_ISR_SAVE_UNLOCK(gas_id);

      if ( send_ind && ind )
      {
         /* tell GRR we didn't get BCCH for this cell */
         L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, TRUE, gas_id );
      }

   }

}

