
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           SCE: L1 DEDICATED MODE

GENERAL DESCRIPTION
   This module contains SCE code for L1 Dedicated mode, SDCCH and TCH
   measurements.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_dedi.c#3 $
$DateTime: 2021/06/28 08:40:41 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
30/01/20   ktg       CR2589703 Change to handle continuous fcch retries on ncell when rssi fluctuations are seen
06/02/20   nv       CR2617011 FR 54762: Framework for mitigating concurrency of transient scenarios across Q6 subsystems
03/01/18   ktg       CR2374184 fading cells reconf scheduling in dedicated mode only for SCH
06/12/18   ktg       CR2360036 fixing delayed sch reconf scheduling in dedicated mode for non fading cells
20/08/18   sk        CR2181447 Opt for fading cells reconf scheduling in dedicated mode
24/10/18   ktg       CR2336672: NCELL getting reported in Measurement Report despite getting failed multiple times in call connecting phase
26/05/17   mk        CR2020690 Wait for 2 complete SACCH periods before scheduling NCell FCCH and SCH after channel transition in dedicated mode
31/03/17   mk        CR2025712: SENS algorithm need to be extended for SDCCH mode also
06/07/17   mk        CR2071218 FR44121 -Synchronous Ncell Quality Monitor - schedule SCH based on poor gdet cnt
14/02/17   mk        CR2004610 : FR 40251 (SeNS) - Skip reporting NCELL if SCH reconfirmation failed with good GDET score
03/01/16   mn        CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
13/10/16   mk        CR1071720: Optimization for 3rd SCH reconf to consider it as failed if RSSI is more than -90dBm and SNR is less than 6 dB
06/10/16   sk        CR1005973 Do not report Ncell that in top 10 and if SCH is not re-confirmed in the last 30 seconds
12/02/15   am        CR771762 BCCH Decode Failures at Good RSSI when the NCell is drifted by 1.5KHz
02/12/14   ak        CR748234 Problems with legacy SCE Opt 16/17/20
29/10/14   jj        CR725942 Do Force ncell SCH in first attempt 
06/10/14   dg        CR727901 Do Force Acq. on Max re-tries only if SCH lies before the 
                     start of acq window
24/07/14   og        CR684296. Sanity check the gas id in dedicated isr and SCE.
30/05/14   ws        CR657674 Data + MMS Phase 1
24/04/13   og        CR652267. Sanity check the cell ptr in next_cell_tch( ).
16/08/13   cgc       CR530370 add SDCCH support for G2L measurements in dedicated mode.
08/05/13   cgc       CR453758 add FEATURE_CM_G2L search functionality FR-3009
20/05/13   cs        TSTS Updates
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
16/11/11   cja       CR318919 For DTM allow bigger NCell window + if SCH fail clear BSIC known
29/03/11   cja       CR281213 Add try_no to SCH fail F3 message.
30/06/10   cja       CR243878. Correct scheduling of SCH if not 52 frames after FCCH detection
17/04/09   nt        CR179739 - DTM/CS uses different acq window sizes
18/05/09   ap        CR174981 For engineeing mode. Reset timing advance when coming out of
                     transfer,dedicated or dtm modes.
08/05/09   nt        CR181055 - Changed the F3 when SCH fails in TCH. Removed timing error.
16/04/09   nt        CR174448 - Fix logging for FCCH/SCH failures
09/04/09   og        Seperate the dedicated and transfer SCH scheduling decisions
                     CR174438.
02/10/08   cs        Lint fixes
17/09/08   nt        Re-Introduce the CR88026
25/07/08   dv        CR143071: Fix to ensure abort of SCE when leaving dedicated.
22/04/08   og        NCELL SCH changes CR 88026.
11/04/08   og        NCELL SCH changes to support a wider range of offsets. CR 88026.
20/07/07   nt        CR107476 sync up the wcdma scheduling and the start of the
                     SACCH periods via addition of flag wcdma_start_of_sacch_indication.
24/04/07   dv        Fix for CR117210:Un-necessary scheduling of ncell FB while ncell
                     SB active.
23/04/07   dv        Fix for CR117081:Early ncell invalidation
28/02/07   dv        Fix for CR94002:unnecessary scheduling of FB searches
17/03/06   og        Fix to compilation issue in previous checkin.
16/03/06   og        Shrink the allowed FB/SB acquisition window size by 2 timeslots
                     to the TDMA frame tick. Resolves CR 88025.
26/01/06   og        Neighbour cell acquisition changes to handle extended dynamic.
10/18/05   sv        Fix for mdsp errors\halts due to conflicts between WCDMA RX and SDCCH
                     RX scheduling.
09/06/05   sv        Schedule 3G measurements in SDCCH only when there is enough gap between
                     SDCCH\SACCH Rx\Tx.
01/09/05   nt        Enabled the SCH scheduling in gpl1_dtm_gprs_schedule_sch_if_allowed
                     after adding code to calculate TX TN which prevents the RX conflicts
31/08/05   nt        Added the function gpl1_dtm_gprs_schedule_sch_if_allowed to make
                     sure SCH can be received without interruption to the TX. At the
                     moment all SCH scheduling is disabled for DTM.
08/12/05   ws        Added initial FEATURE_GSM_DTM support
03/14/05   pp        Revamped ncell scheduling algorithm on sdcch
01/26/05   sv        Add support for Frame Manager.
08/03/04   dp        Fixed bug in computing deltafnlost
07/15/04   dp        Fixed bug in scheduling ncell tone detects
06/21/04   gsc       Updated to use new API for l1_sci_check_available_frames.
04/22/04   dnn/hg    Added missing featurization around some GtoW code.
03/31/04   bm/jc     Fix for gsm_ncell_handler trying to register when wcdma_ded_srch
                     is in progress.
03/19/04   dp        Changed priority of TCH ncell scheduling so that the
                     first SCH decode after tone gets top priority
03/18/04   gsc       Some More Lint clean up.
02/25/04   gw        Lint clean up.
01/30/04   dp        Merged fix for Ncell SCH scheduling bug in half rate
11/05/03   gfr       Support for quad-band.
09/24/03   tb        Removed use of fcch_decode_failures in schedule algorithm.
09/12/03   tb        Added use of fcch_decode_failures to schedule algorithm.
08/28/03   rr        Surround cell optimizations
08/28/03   gw        Changes to support new result type returned by l1_sc_read_sb
                     functions.
08/24/03   dp        Revamped ncell scheduling algorithm on TCH to fix
                     Samsung call drops.   l1_sc_balist_reset_counters is now
                     invoked when we enter SDCCH or TCH (we need to reset ncell
                     states during handovers )
08/07/01   tb        Changes for wcdma searching on SDCCH.
08/01/03   tb        Changes to wcdma search scheduling.
07/14/03   gw        Removed ASSERTs.
06/12/03   gw        Changed L1_SCDRV_DURATION_FCB to L1_SC_DURATION_FCB.
06/06/03   tb        Changes to support dedicated GtoW measurements.
05/02/03   dp        Changes to preserve Ncell BSIC across state transitions
05/02/03   gw        Merged in power transfer changes from branch.
04/08/03   gw        Added call to l1_sc_monscan_update() in l1_sc_enter_dedi()
                     to make sure that old serving cell is picked up if it has
                     just been added to the BA list.
04/04/03   gw        Added call to l1_sc_balist_reset_counters() in
                     l1_sc_enter_dedi().  This will be removed again soon.
04/02/03   gw        Changes to reflect modifications in monscan module.
01/10/03   gw        Changed some SCE function calls to reflect renaming of some
                     BA list functions done as part of SCE clean up.  Changed
                     references to l1_sc_bs_T elements rank_rx_power and
                     prev_rank_rx_power to rx_power and prev_rx_power respectively.
11/13/02   dp        Bumped up priority of sch_unknown_cell in Ncell
                     scheduling
10/16/02   dp        Temp s/w workaround for mdsp errors due to ZIF tune time
10/01/02   ATM       Added central get and set FN routines
09/06/02   plm       Added extern for l1_sc_adjust_lag_tch() to eliminate compiler
                     warning.
08/21/02   dp        Added debug msg for SC failures
07/26/02   dp        Added fix to decode SCH at next available opportunity
                     once a SCH failure is detected and also when the rx lvl
                     diff from the previous measurement drops by more than 10
                     dB. Also fixed a Ncell scheduling bug
07/15/02   ATM       Added FCCH/SCH decode packets
07/12/02   ATM       Added logging for NCELL ACQ.
06/15/02   dp        Fix to share Ncell info between dedicated modes.  Cleaned
                     up SCH wrap handling
05/09/02   dp        Fix for handover failures when SCH wraps around
05/06/02   dp        Moved invocation of l1_sci_tch_tick to ISR to prevent
                     race condition with the task
04/24/02   dp        Reinforced abort mechanism on SDCCH
04/21/02   dp        Surround cell optimizations to reacquire after cell is lost.
                     Introduced power threshold to exclude cells below the noise
                     floor from surround cell acquisition
04/02/02   dp        Fixed bug in SDCCH SC scheduling
03/29/02   dp        Replaced TEST_CONTINUOUS_TONES with DEBUG_CONTINUOUS_TONES
03/26/02   dp        Moved next_cell_sdcch to ISR to prevent message queue
                     overflow.  Made changes to surround cell time tracking to
                     handle corner cases
03/25/02   gw        Changed when/how next_cell_sdcch is called. Now similar to
                     original CCWW code.  Was necessary to prevent call back
                     messages from ISR to task overflowing the message queue.
02/20/02   dp        Fixed some bugs in SC scheduling
02/07/02   gw        Improved starting and stoppng of SCE.
12/21/01   gw        Changes to calculation of fn_lag for SB reception to
                     support DRX scheduling.
12/18/01   dp        Changes to support surround cell on SDCCH
10/25/01   dp        Revamped the code to support separate tone detection and
                     SCH decodes on the TCH.  Deleted unused functions
09/25/01   mk        General SCE cleanup. Using OLD_SCE for non-supported
                     code.
09/16/01   JC        Added some QCT headers and cleaned up include files.
08/20/01   mk        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "l1_isr.h"
#include "l1_sc.h"
#include "l1_sc_drv.h"
#include "msg.h"
#include "l1_log.h"
#include "l1_utils.h"   /* for ADD_FN() and SUB_FN() macros */
#include "l1i.h"
#include "mcfg_nv_api.h"
#include "l1_fm.h"
#ifdef FEATURE_GSM_DTM
#include "gpl1_gprs_log.h"
#include "gpl1_gprs_serving_cell.h"
#endif
#include "geran_eng_mode_info.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "geran_dual_sim_g.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
/*
This is in terms of the tick which is incremented
every SACCH multiframe(104 frames)
*/

#define TEN_SECONDS  21

/* (4000 / 4.615 / 13) */
#define FOUR_SECONDS  64

#define MAX_SCH_SEPARATION 3
#define MAX_TRIES 3

#define MAX_NCELLS_TO_DECODE 10

/*
 * How many idle slots do we have to wait before an SB comes along.
 * Indexed by the neighbours frame number % 51.
 */
static const byte next_chance_to_get_sb[] =
{
   2,0,7,5,3,1,10, 8,6,4,
   2,0,7,5,3,1,10, 8,6,4,
   2,0,9,7,5,3, 1, 8,6,4,
   2,0,9,7,5,3, 1, 8,6,4,
   2,0,9,7,5,3, 1,10,8,6,4
};

static uint8 tch_idle_frame[NUM_GERAN_DATA_SPACES] =
               { INITIAL_VALUE( 25 ) };

extern boolean l1_sc_adjust_lag_tch( l1_sc_bs_T* bs, sword timing_error, gas_id_t gas_id );

extern boolean  l1_sc_balist_check_cell(  gas_id_t  gas_id,
                                          cell_T*   cell_ptr
                                       );

void l1_sc_set_tch_idle_frame( uint8 idle_frame, gas_id_t gas_id );

void fcb_done_tch( cell_T* cell, gas_id_t gas_id );

/**********************************/

static void sb_done_sdcch( cell_T* cell, gas_id_t gas_id );
static void next_cell_sdcch( void *unused, gas_id_t gas_id );

static void next_cell_tch( gas_id_t gas_id );
static void sb_done_tch( cell_T* cell, gas_id_t gas_id );

#ifdef FEATURE_GSM_DTM
static void next_cell_dtm( gas_id_t gas_id );
static void gpl1_dtm_gprs_schedule_sch_if_allowed ( cell_T *cell, gas_id_t gas_id );
#endif

static boolean gpl1_schedule_fading_cell (gas_id_t gas_id);
extern dedicated_data_T l1_dedicated_data[];

void l1_sc_enter_dedi( gas_id_t gas_id )
{
   ASSERT_TASK();

   /* Update monscan since serving cell may have been added
    * to BA list.
    */
   l1_sc_monscan_update(gas_id);

   l1_sc_wcdma_enter_dedi(gas_id);
}

void l1_sc_leave_dedi( gas_id_t gas_id )
{
   uint8 dummy_value=0xFF;
   ASSERT_TASK();
   l1_sc_wcdma_leave_dedi(gas_id);
   geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&(dummy_value),FALSE, gas_id);
}

void l1_sc_good_sb_dedi( cell_T* cell, byte sb_data[4], dword start_fn, sword timing_error, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   dword bs_fn;
   byte  bsic;

   if (!cell || !sb_data)
   {
      MSG_GERAN_ERROR_2_G("Rcvd NULL cell %d or sb_data %d pointers",
        cell, sb_data);
      return;
   }

   /*lint -esym(715, timing_error) not referenced */

   bs_fn = l1_sc_decode_sb( sb_data, &bsic );
   if ( !cell->bs.bsic_known )
   {
      cell->bs.bsic_known = TRUE;
      cell->bs.bsic = bsic;
      /* TAP POINT FOR NCELL_ACQ_SUCCESS */
      l1_log_ncell_acq( L1_LOG_NCELL_ACQ_SUCCESS,
                        cell->rank,
                        cell->bs.arfcn,
                        cell->bs.rx_power, gas_id );
   }
   else if ( cell->bs.bsic != bsic )
   {
      cell->bs.bsic = bsic;
      /* TODO is it done in RR? */
   }

   /*
   Once decoded don't decode for another 10 secs
   20 = (10000/(4.615*104)) - surround_tick is incremented every
   104 frames(4 SACCH multiframes))
   Account for the fact that there could be a max of 11 idle frames
   after the tick is activated before a decode is scheduled
   */
   cell->next_tick = l1_sc_globals_ptr->surround_tick + TEN_SECONDS - MAX_SCH_SEPARATION;


   cell->bs.sb_data[ 0 ] = sb_data[ 0 ];
   cell->bs.sb_data[ 1 ] = sb_data[ 1 ];
   cell->bs.sb_data[ 2 ] = sb_data[ 2 ];
   cell->bs.sb_data[ 3 ] = sb_data[ 3 ];

   /*
      Refine the SCH offset from the timing error returned in the
      SCH metrics
   */
   cell->bs.fn_lag = SUB_FN( start_fn, bs_fn );

   cell->last_bsic_fn = start_fn;
   cell->try_no = 0;
   cell->in_fade = FALSE;
   cell->cell_stale_sch = FALSE;

#ifdef FEATURE_GSM_SYNQ_SUPPORT
   cell->gdet_based_sch_reconfirm = FALSE;
   cell->bad_gdet_pwr_mon_cntr = 0;
#endif
   /*
      Set this to prevent the same cell from being accessed again till the
      next rank period
   */
   cell->bs.prev_rx_power = cell->bs.rx_power;
}

void l1_sc_bad_sb_dedi( cell_T* cell,gas_id_t gas_id  )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   if (!cell)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cell pointer");
      return;
   }

   ++cell->try_no;

   if ( cell->try_no >= MAX_TRIES )
   {

         /* If BSIC known and failures observed => reconfirmation failed
            otherwise, the initial acq failed due to SCH decode failures
         */
         if ( cell->bs.bsic_known )
         {
            /* TAP POINT FOR NCELL_RECONFIRMATION_LOST */
            l1_log_ncell_acq( L1_LOG_NCELL_ACQ_RECONF_LOST,
                              cell->rank,
                              cell->bs.arfcn,
                              cell->bs.rx_power, gas_id );
            cell->fn_when_lost = -32 * MAX_TCH_TONE_DETECT_ATTEMPTS * MULTIFRAME_26;
         }
         else
         {
            /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason SCH) */
            l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_SCH,
                              cell->rank,
                              cell->bs.arfcn,
                              cell->bs.rx_power, gas_id );

            /* Save fn when cell was lost.  Used in scheduling */
            cell->fn_when_lost = (int32)l1_sc_get_fn(gas_id);
         }
         cell->bs.bsic_known = FALSE;
         cell->bs.tone_detected = FALSE;
         cell->bs.bsic = 0;

         cell->try_no = 0;
         cell->in_fade = FALSE;
         cell->cell_stale_sch = FALSE;

         /* TODO delete power measurements cause cell is lost */
   }
   else
   {
      /* If SCH fails, decode at next available opportunity*/
      cell->next_tick = l1_sc_globals_ptr->surround_tick;
      cell->in_fade = TRUE;
   }
}


/***
 *
 *  Module:
 *  -------
 *  Dedicated SDCCH measurements.
 *
 ***/

//#define MODULE_NUMBER 10085

void l1_sc_enter_dedi_sdcch( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();

   l1_sc_globals_ptr->max_bsic_age = 216*20; /* ca 20 secs */

   /* Reset ncell structs (BSIC is preserved ) */
   l1_sc_balist_reset_counters(gas_id);

   l1_sc_wcdma_enter_dedi(gas_id);
}

void l1_sc_leave_dedi_sdcch( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();

   if ( l1_sc_globals_ptr->monscan_in_progress )
   {
      l1_sc_monscan_abort(gas_id);
   }

   if ( l1_sc_globals_ptr->receive_in_progress )
   {
      l1_sc_abort_receive(gas_id);
   }

   if ( l1_sc_wcdma_srch_active (gas_id) )          /* G2L changed */
   {
      l1_sc_wcdma_abort_ded_search(gas_id);
   }
}

void l1_sc_set_ba_list_dedi_sdcch( inter_task_BCCH_allocation_T* ba_list, byte ncc_permitted, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();

   if ( l1_sc_balist_prepare_new( ba_list, ncc_permitted, gas_id ) )
   {
      if ( l1_sc_globals_ptr->receive_in_progress &&
           !l1_sc_globals_ptr->receiving_cell->in_new_ba_list )
      {
         /* maybe a bit early */
         l1_sc_abort_receive(gas_id);
      }
      l1_sc_monscan_update(gas_id);
   }
}



void l1_sc_report_period_done_sdcch( gas_id_t gas_id )
{
   ASSERT_TASK();

   l1_sci_inc_surround_tick(1, gas_id);
}


static void fcb_sb_done_sdcch( cell_T* cell, gas_id_t gas_id )
{
    byte sb_data[4];
    dword start_fn;
    int16 timing_error;

    if (!cell)
    {
       MSG_GERAN_ERROR_0_G("Rcvd NULL cell pointer");
       return;
    }

    ASSERT_TASK();

    if ( l1_sc_read_fcbsb( sb_data, &start_fn, &timing_error, gas_id ) )
    {
        l1_sc_good_sb_dedi( cell, sb_data, start_fn, timing_error, gas_id );
        cell->bs.qbit_lag = timing_error;
        cell->bs.fn_when_read = gl1_get_FN(gas_id);
        cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
    }
    else
    {
        l1_sc_bad_sb_dedi( cell, gas_id );
    }
}


static void sb_done_sdcch( cell_T* cell, gas_id_t gas_id )
{
   byte               sb_data[4];
   dword              start_fn;
   int16              timing_error;
   l1_sc_rcv_result_T result;

   if (!cell)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cell pointer");
      return;
   }

   ASSERT_TASK();

   result = l1_sc_read_sb( sb_data, &start_fn, &timing_error, gas_id );

   if ( result == L1_SC_DECODE_SUCCESS )
   {

#ifdef ZIF_TUNE_TIME_FIXED
      #error code not present
#else
      /* +2 because receive is started two frame before it happens.
       * Eventually this should be moved inside l1_sc_good_sb_dedi
       * once sb and fcb frame number handling are made consistent.
       */
       /* This is a temp workaround for the mdsp errors sue to the
          large ZIF tune time
       */
      l1_sc_good_sb_dedi( cell, sb_data, ADD_FN( start_fn, 2), timing_error, gas_id );
#endif

      l1_sc_adjust_lag( &cell->bs, timing_error );
      cell->bs.fn_when_read = gl1_get_FN(gas_id);
      cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
      MSG_GERAN_MED_3_G("SCH err=%d qbitlag=%d fnlag=%d",timing_error,cell->bs.qbit_lag,cell->bs.fn_lag);
   }
   else
   {
#ifdef FEATURE_GSM_SENS_SUPPORT
      if(gl1_hw_get_sens_fr_ctrl() && (result == L1_SC_DEC_FAIL_WITH_GOOD_GDET))
      {
        cell->reassert = TRUE;
      }
#endif
      l1_sc_bad_sb_dedi( cell, gas_id );
      MSG_GERAN_MED_2_G("SCH FAIL qbitlag=%d fnlag=%d",cell->bs.qbit_lag,cell->bs.fn_lag);
   }
}
static boolean gpl1_schedule_fading_cell (gas_id_t gas_id)
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  dedicated_data_T  *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  uint8 num_cells;
  cell_T* cell;
  uint8    n;
  uint8 good_cell_count = 0;
  if ( l1_dedicated_data_ptr->schedule_fading_cells == FALSE )
  {
    MSG_GERAN_LOW_0_G("Serving cell Rxqual is good, do not schedule fading cells");
    return FALSE;
  }
  num_cells = ( l1_sc_globals_ptr->sorted_cell_count > MAX_NCELLS_TO_DECODE ) ?
               MAX_NCELLS_TO_DECODE:
               l1_sc_globals_ptr->sorted_cell_count;  
  for ( n = 0; n < num_cells ; ++n )
  {
     cell = l1_sc_globals_ptr->sorted_cells[ n ];
     if ( cell->bs.bsic_known && l1_sc_rx_power_acceptable(cell->bs.rx_power) )
     {
       if ( ++good_cell_count > 2 )
       {
         MSG_GERAN_LOW_0_G("Three or more ncells with acceptable power level, do not schedule fading cells");
         return FALSE;
       }
     }
  }	   
  return TRUE;

}

static void next_cell_sdcch(void *unused, gas_id_t gas_id)
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

   cell_T* cell;
   cell_T* cell_above_pwr_threshold;
   word    n;
   uint8   num_cells;
   uint32  deltafnlost = 0;
   uint32  max_deltafnlost_top3 = 0;
   uint32  min_deltafnlost_bottom3 = 0x7fffffff;
   uint32  max_deltafnlost_bottom3 = 0;
   uint32  min_deltafnlost_outOftop6 = 0x7fffffff;
   uint32  max_deltafnlost_outOftop6 = 0;

   cell_T* strong_sch_unknown_cell_top3 = NULL;
   cell_T* strong_sch_unknown_cell_bottom3 = NULL;
   cell_T* sch_unknown_cell_outOftop6 = NULL;
   cell_T* known_confirmed_cell = NULL;
   cell_T* known_unconfirmed_cell = NULL;
   cell_T* known_fading_cell = NULL;
   boolean schd_fading_cell = gpl1_schedule_fading_cell(gas_id);
#ifdef FEATURE_GSM_SYNQ_SUPPORT
   cell_T* power_mon_gdet_rogue_cell = NULL;
#endif
   ASSERT_TASK();


   /* Wait for min 150 frames in new channel before start scheduling NCELL acq */
   if(FALSE == l1_sc_globals_ptr->start_ncell_rep_n_acq_dedi)
   {
       return;
   }

   /*lint -esym(715, unused) not referenced */

   /* Clip the number of cells to decode to  MAX_NCELLS_TO_DECODE (10) */
   num_cells = ( l1_sc_globals_ptr->sorted_cell_count > MAX_NCELLS_TO_DECODE ) ?
               MAX_NCELLS_TO_DECODE:
               l1_sc_globals_ptr->sorted_cell_count;

   /* Clear out FCCH/SCH detects for weaker cells or cells
      outside the top MAX_NCELLS_TO_DECODE
   */
   for ( n = 0; n < l1_sc_globals_ptr->sorted_cell_count ; ++n )
   {
      cell = l1_sc_globals_ptr->sorted_cells[ n ];

      if ( n>=MAX_NCELLS_TO_DECODE || !l1_sc_rx_power_acceptable(cell->bs.rx_power) )
      {
         if ( cell->try_no > 0 )
         {
            cell->try_no = 0;
            l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                              cell->rank,
                              cell->bs.arfcn,
                              cell->bs.rx_power, gas_id );
            /*Give SCH 1 chance before stopping any more decode attempts */
            cell->bs.bsic_known = FALSE;
         }
         /*If we haven't detected SCH on ncell we don't want to start*/
         if (cell->bs.bsic_known == FALSE)
         {
            cell->bs.tone_detected = FALSE;
         }
      }
   }

   /* Loop through sorted cells (clipped to  MAX_NCELLS_TO_DECODE ) */
   /*
      Categorize them based on whether the BSIC is known (SCH decoded ),
      or BSIC unknown (haven't decoded the fcch and sch yet - combined acq
      to be scheduled)
   */
   for ( n = 0; n < num_cells ; ++n )
   {
      cell = l1_sc_globals_ptr->sorted_cells[ n ];

      if( ( cell->bs.bsic_known == TRUE )&& (( cell->next_tick + ( 3*TEN_SECONDS ) ) < l1_sc_globals_ptr->surround_tick ))
      {
        MSG_GERAN_HIGH_0_G("The cell is back in top 10 after more than 30 seconds, reconfirm SCH at next available opportunity");
        cell->next_tick = l1_sc_globals_ptr->surround_tick;
        cell->in_fade = TRUE;
        cell->cell_stale_sch = TRUE;
      }	  

      /* Cells below the noise floor are excluded from the scheduling algorithm */
      if ( ( l1_sc_rx_power_acceptable( cell->bs.rx_power ) ) ||  ( ( schd_fading_cell == FALSE ) && ( cell->bs.bsic_known )) )
      {
         cell_above_pwr_threshold = cell;
      }
      else
      {
         cell_above_pwr_threshold = NULL;
      }

      {
         if ( cell->bs.bsic_known )
         {

            /* If cell is below absolute threshold or if the current rx lvl
               has dropped by more than 10dB from the last known level,
               start SCH decodes immediately.  This is to help us drop the
               cell faster
            */
            if ( ( cell_above_pwr_threshold == NULL )||
                 ( !l1_sc_rx_power_drop_acceptable (cell->bs.rx_power - cell->bs.prev_rx_power ) ) )
            {
               MSG_GERAN_HIGH_2_G("Cell %d drop=%d Trig SCH",cell->bs.arfcn.num, cell->bs.rx_power - cell->bs.prev_rx_power);
               /* Decode at next available opportunity */
               cell->next_tick = l1_sc_globals_ptr->surround_tick;
               cell_above_pwr_threshold = cell;
               cell->in_fade = TRUE;
            }

            /* BSIC known but cell not confirmed within last 10 secs*/
            if ( cell->next_tick <= l1_sc_globals_ptr->surround_tick )
            {
               if ( cell->in_fade )
               {
                  if ( known_fading_cell == NULL )
                  {
                     known_fading_cell = cell_above_pwr_threshold;
                  }
               }
               else
               {
                  if ( known_unconfirmed_cell == NULL )
                  {
                     known_unconfirmed_cell = cell_above_pwr_threshold;
                  }
               }
            }
#ifdef FEATURE_GSM_SYNQ_SUPPORT
/*
  Schedule a SCH reconfirm for rogue NCELL detected by synq algo
*/
            else if( cell->gdet_based_sch_reconfirm )
            {
               if ( power_mon_gdet_rogue_cell == NULL )
               {
                    power_mon_gdet_rogue_cell = cell_above_pwr_threshold;
               }
            }
#endif
            else
            {
               /* BSIC known and cell confirmed within the last 10 secs */
               if ( known_confirmed_cell == NULL )
               {
                  known_confirmed_cell = cell_above_pwr_threshold;

               }
            }

         }
         else if ( cell_above_pwr_threshold != NULL )
         {
            uint32 sc_FN = l1_sc_get_fn(gas_id);

            /* Start a combined acquisition only if there are enough frames */
            if ( l1_fm_get_available_frames(FM_PRI_NCELL_ACQ, sc_FN, gas_id) > L1_SC_DURATION_FCB )
            {
               if ( cell->fn_when_lost > 0 )
               {
                  deltafnlost = SUB_FN (sc_FN, (uint32)cell->fn_when_lost);
               }
               else
               {
                  deltafnlost = ADD_FN (sc_FN, (uint32)(-cell->fn_when_lost));
               }

               /* Find and schedule the unknown cell from each group that
                  hasn't been searched in the longest time.  The groups are
                  the top 3, bottom 3, and out of the top 6.  Also find the
                  most recently searched unknown cell from the bottom 2 groups.
                  Later we will choose when to cancel the higher power groups in
                  favor of the lower power groups based on this information.
               */

               if ( cell->rank < 3 )
               {
                  if ( deltafnlost > max_deltafnlost_top3 )
                  {
                     strong_sch_unknown_cell_top3 =  cell_above_pwr_threshold;
                     max_deltafnlost_top3 = deltafnlost;
                  }

               }
               else if ( cell->rank < 6 )
               {
                  if ( deltafnlost > max_deltafnlost_bottom3 )
                  {
                     strong_sch_unknown_cell_bottom3 =  cell_above_pwr_threshold;
                     max_deltafnlost_bottom3 = deltafnlost;
                  }
                  if ( deltafnlost < min_deltafnlost_bottom3 )
                  {
                     min_deltafnlost_bottom3 = deltafnlost;
                  }

               }
               else
               {
                  if ( deltafnlost > max_deltafnlost_outOftop6 )
                  {
                     sch_unknown_cell_outOftop6 =  cell_above_pwr_threshold;
                     max_deltafnlost_outOftop6 = deltafnlost;
                  }
                  if ( deltafnlost < min_deltafnlost_outOftop6 )
                  {
                     min_deltafnlost_outOftop6 = deltafnlost;
                  }

               }
            }
         }
      } //if in ba_list
   }//for

   /* Cancel any scheduled top 3 sch unknown search if all cells from the top3 have been searched
      since the last bottom 3 search.  Keep in mind the default values if there are no unknown
      cells in a given group when editing this section.  If combined acq in progress this func won't
      be called and there won't be multiple cells scheduled so this code doesn't change anything.
   */
   if ( strong_sch_unknown_cell_bottom3 != NULL && min_deltafnlost_bottom3 > max_deltafnlost_top3 )
   {
      strong_sch_unknown_cell_top3 = NULL;
   }

   /* Cancel any scheduled top 3 and bottom 3 sch unknown search if a cell out of the top 6
      needs to be searched.  The desired algorithm when all cells are unknown is:
              ABC D ABC E ABC F ABC G, repeat with H instead of G, etc
      where cell powers from strongest to weakest are A, B, etc.  If combined acq in progress this
      func won't be called and there won't be multiple cells scheduled so this code doesn't change
      anything.
   */
   if ( sch_unknown_cell_outOftop6 != NULL
        && min_deltafnlost_outOftop6 > max_deltafnlost_bottom3
        && min_deltafnlost_outOftop6 > max_deltafnlost_top3
        && min_deltafnlost_bottom3 > max_deltafnlost_top3 )
   {
      strong_sch_unknown_cell_top3 = NULL;
      strong_sch_unknown_cell_bottom3 = NULL;
   }

   /* A known fading cell is given first priority since its just a one
      frame SCH and we want to drop/reconfirm him asap
   */
   if ( known_fading_cell )
   {
      l1_sc_receive_sb( known_fading_cell, sb_done_sdcch, gas_id );
   }
#ifdef FEATURE_GSM_SYNQ_SUPPORT
   else if( power_mon_gdet_rogue_cell )
   {
      l1_sc_receive_sb( power_mon_gdet_rogue_cell, sb_done_sdcch, gas_id );
      MSG_GERAN_MED_1_G("SyNQ_FR: Scheduling rogue cell, cnt %d", power_mon_gdet_rogue_cell->bad_gdet_pwr_mon_cntr);
      power_mon_gdet_rogue_cell->gdet_based_sch_reconfirm = FALSE;
      power_mon_gdet_rogue_cell->bad_gdet_pwr_mon_cntr = 0;
   }
#endif
   else if ((l1_sc_wcdma_ded_pending(gas_id)) &&
            (l1_fm_get_ccch_gap(gas_id) >= L1_SC_DURATION_WCDMA_DED_SRCH))
   {
      (void) l1_sc_wcdma_ded_ready(FALSE, gas_id);
   }
   /*
      SCH unknown cells get high priority irrespective of whether they are
      in the top 6 or not.
   */
   else if ( strong_sch_unknown_cell_top3 )
   {
      /* TAP POINT FOR NCELL_ACQ_START */
      l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                        strong_sch_unknown_cell_top3->rank,
                        strong_sch_unknown_cell_top3->bs.arfcn,
                        strong_sch_unknown_cell_top3->bs.rx_power, gas_id );

      l1_sc_receive_fcb( strong_sch_unknown_cell_top3,fcb_sb_done_sdcch, gas_id );
   }
   else if ( strong_sch_unknown_cell_bottom3 )
   {
      /* TAP POINT FOR NCELL_ACQ_START */
      l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                        strong_sch_unknown_cell_bottom3->rank,
                        strong_sch_unknown_cell_bottom3->bs.arfcn,
                        strong_sch_unknown_cell_bottom3->bs.rx_power, gas_id );

      l1_sc_receive_fcb( strong_sch_unknown_cell_bottom3,fcb_sb_done_sdcch, gas_id );
   }
   else if ( sch_unknown_cell_outOftop6 )
   {
      /* TAP POINT FOR NCELL_ACQ_START */
      l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                        sch_unknown_cell_outOftop6->rank,
                        sch_unknown_cell_outOftop6->bs.arfcn,
                        sch_unknown_cell_outOftop6->bs.rx_power, gas_id );
      l1_sc_receive_fcb( sch_unknown_cell_outOftop6, fcb_sb_done_sdcch, gas_id );
   }
   else if ( known_unconfirmed_cell )
   {
      l1_sc_receive_sb( known_unconfirmed_cell, sb_done_sdcch, gas_id );
   }
   else if ( known_confirmed_cell )
   {
      //l1_sc_receive_sb( known_confirmed_cell, sb_done_sdcch );
   }
}


/***
 *
 *  Module:
 *  -------
 *  Dedicated TCH measurements.
 *
 ***/

//#define MODULE_NUMBER 10086


void l1_sc_enter_dedi_tch( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();

   l1_sc_globals_ptr->max_bsic_age = 216*20; /* ca 20 secs */
   l1_sc_globals_ptr->send_tch_tick = TRUE;

   /* Reset ncell structs (BSIC is preserved ) */
   l1_sc_balist_reset_counters(gas_id);

   l1_sc_wcdma_enter_dedi(gas_id);
}

void l1_sc_leave_dedi_tch( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();


      l1_sc_monscan_abort(gas_id);
      l1_sc_abort_receive(gas_id);
      l1_sc_wcdma_abort_ded_search(gas_id);

   l1_sc_globals_ptr->send_tch_tick = FALSE;
}

void l1_sc_set_ba_list_dedi_tch( inter_task_BCCH_allocation_T* ba_list, byte ncc_permitted, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();

   if ( l1_sc_balist_prepare_new( ba_list, ncc_permitted, gas_id ) )
   {
      if ( l1_sc_globals_ptr->receive_in_progress &&
           !l1_sc_globals_ptr->receiving_cell->in_new_ba_list )
      {
         /* maybe a bit early */
         l1_sc_abort_receive(gas_id);

         /* no receive pending so we must enable the tick */
         l1_sc_globals_ptr->send_tch_tick = TRUE;
      }
      l1_sc_monscan_update(gas_id);
   }
}

void l1_sc_report_period_done_tch(  gas_id_t gas_id )
{
   ASSERT_TASK();

   l1_sci_inc_surround_tick(1, gas_id);
}

static void sb_done_tch( cell_T* cell,gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   byte               sb_data[4];
   dword              start_fn;
   int16              timing_error;
   l1_sc_rcv_result_T result;

   if (!cell)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL cell pointer");
      return;
   }

   ASSERT_TASK();

   result = l1_sc_read_sb_tch( sb_data, &start_fn, &timing_error, gas_id );

   result = l1_sc_decide_about_sb_tch(cell->try_no, gas_id);

   if ( result == L1_SC_DECODE_SUCCESS )
   {
#ifdef FEATURE_GSM_SENS_SUPPORT      
      cell->reassert = FALSE;
#endif     
      l1_sc_good_sb_dedi( cell, sb_data, start_fn, timing_error, gas_id );

      /* If the SCH wrapped around, decode the SCH in the new position
         immediately to sync up the fn lag.   This fix is for handover
         failures when the SCH wraps around.  Can be optimized
      */
      if ( l1_sc_adjust_lag_tch( &cell->bs, timing_error, gas_id ) )
      {
         MSG_GERAN_HIGH_0_G("SCH wrapped around ");
         cell->next_tick = l1_sc_globals_ptr->surround_tick;
      }

      cell->bs.fn_when_read = gl1_get_FN(gas_id);
      cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
      MSG_GERAN_MED_3_G("SCH err=%d qbitlag=%d fnlag=%d",timing_error,cell->bs.qbit_lag,cell->bs.fn_lag);
   }
   else
   {
#ifdef FEATURE_GSM_SENS_SUPPORT
      if(gl1_hw_get_sens_fr_ctrl() && (result == L1_SC_DEC_FAIL_WITH_GOOD_GDET))
      {
        cell->reassert = TRUE;
      }
#endif
      l1_sc_bad_sb_dedi( cell, gas_id );
      MSG_GERAN_MED_3_G("SCH FAIL qbitlag=%d fnlag=%d try no %d",cell->bs.qbit_lag,cell->bs.fn_lag,cell->try_no);
   }
}

void l1_sc_tch_tick( void* unused, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();

   /*lint -esym(715, unused) not referenced */

   gas_id = check_gas_id(gas_id);

   /* if tick arrives just after disabling it, throw it away */
   if ( l1_sc_globals_ptr->send_tch_tick )
   {
      l1_sc_globals_ptr->send_tch_tick = FALSE;

#ifdef FEATURE_GSM_DTM
      if ( l1_sc_globals_ptr->mode == L1SCModeDediDTM )
      {
        next_cell_dtm(gas_id);
      }
      else
#endif
      {
        next_cell_tch(gas_id);
      }


      /*This flag indicates to l1_sc_wcdma that this is the start of a SACCH period.
      The WCDMA searches can then be sync'ed to the start of this period. This flag
      is FALSE here as this is called every TCH idle frame, and the next_cell_tch
      function has been called prior to this and allowed the possibility of the
      measurements to get scheduled. This should not be moved above this function*/
      l1_sc_globals_ptr->wcdma_start_of_sacch_indication = FALSE;
   }
}

void l1_sc_set_tch_idle_frame( uint8 idle_frame, gas_id_t gas_id)
{
   tch_idle_frame[gas_id] = idle_frame;
}

uint8 l1_sc_get_tch_idle_frame( gas_id_t gas_id )
{
   return tch_idle_frame[gas_id];
}

static void next_cell_tch(gas_id_t gas_id)
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   dword   pre_idle_frame_no_mod_26;
   dword   next_pre_idle_fn;
   cell_T* cell;
   cell_T* cell_above_pwr_threshold;
   word    fn_mod_51;
   word    n;
   uint8   num_cells;
   int32   deltafnlost = 0;
   int32   max_deltafnlost_top3 = 0;
   int32   min_deltafnlost_bottom3 = 0x7fffffff;
   int32   max_deltafnlost_bottom3 = 0;
   int32   min_deltafnlost_outOftop6 = 0x7fffffff;
   int32   max_deltafnlost_outOftop6 = 0;
   boolean tone_in_progress = FALSE;
   uint32  fn_lag;

   cell_T* strong_tone_unknown_cell_top3 = NULL;
   cell_T* strong_tone_unknown_cell_bottom3 = NULL;
   cell_T* strong_sch_unknown_cell = NULL;
   cell_T* tone_unknown_cell_outOftop6 = NULL;
   cell_T* sch_unknown_cell = NULL;
   cell_T* known_confirmed_cell = NULL;
   cell_T* known_unconfirmed_cell = NULL;
   cell_T* known_fading_cell = NULL;
#ifdef FEATURE_GSM_SYNQ_SUPPORT
   cell_T* power_mon_gdet_rogue_cell = NULL;
#endif

   boolean wcdma_srch_scheduled = FALSE;
   boolean wcdma_srch_pending = FALSE;
   boolean schd_fading_cell   = gpl1_schedule_fading_cell(gas_id);   

   ASSERT_TASK();

   /* Wait for min 150 frames in new channel before start scheduling NCELL acq */
   if(FALSE == l1_sc_globals_ptr->start_ncell_rep_n_acq_dedi)
   {
       l1_sc_wcdma_ded_tick(wcdma_srch_pending, wcdma_srch_scheduled, gas_id);
       l1_sc_globals_ptr->send_tch_tick = TRUE;
       return;
   }
   
   /* calculate frame number of next pre idle frame */
   /*
      tch_idle_frame is passed in from the l1_tch and depends on timeslot/subchannel
   */
   pre_idle_frame_no_mod_26 = tch_idle_frame[gas_id] - 1;

   /*add 26+FRAMES_PER_HYPERFRAME so subtract of bs.fn_lag won't generate negative value   */

   next_pre_idle_fn = l1_sc_get_fn(gas_id) + 26 + FRAMES_PER_HYPERFRAME;
   next_pre_idle_fn -= ( next_pre_idle_fn - pre_idle_frame_no_mod_26 ) MODULO 26;


   /* Clip the number of cells to decode to  MAX_NCELLS_TO_DECODE (10) */
   num_cells = ( l1_sc_globals_ptr->sorted_cell_count > MAX_NCELLS_TO_DECODE ) ?
               MAX_NCELLS_TO_DECODE:
               l1_sc_globals_ptr->sorted_cell_count;

   /* Check for a tone detect in progress and clear out tone detects for weaker cells or cells
      outside the top MAX_NCELLS_TO_DECODE
   */
   for ( n = 0; n < l1_sc_globals_ptr->sorted_cell_count ; ++n )
   {
       cell = l1_sc_globals_ptr->sorted_cells[ n ];

       if( FALSE == l1_sc_balist_check_cell( gas_id, cell ) )
       {
         MSG_GERAN_HIGH_1_G("cell ptr 0x%p out of bounds", cell );
         continue;
       }

	   
	   if(l1_sc_globals_ptr->last_top3_fcch_attempted_cell && ((l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.tone_detected) || (l1_sc_globals_ptr->last_top3_fcch_attempted_cell->try_no == 0)))
	   {
				l1_sc_globals_ptr->last_top3_fcch_attempted_cell = NULL;
	   }

       if ( n>=MAX_NCELLS_TO_DECODE || !l1_sc_rx_power_acceptable(cell->bs.rx_power) )
       {
            if ( cell->try_no > 0 && (!l1_sc_globals_ptr->last_top3_fcch_attempted_cell || 
            (l1_sc_globals_ptr->last_top3_fcch_attempted_cell->rank > l1_sc_globals_ptr->sorted_cell_count) ||
            (l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.arfcn.num != cell->bs.arfcn.num)))
           {
             cell->try_no = 0;
               l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                                    cell->rank,
                                    cell->bs.arfcn,
                                    cell->bs.rx_power, gas_id );
               /*Give SCH 1 chance before stopping any more decode attempts */
               cell->bs.bsic_known = FALSE;
           }
           /*If we haven't detected SCH on ncell we don't want to start*/
           if (cell->bs.bsic_known == FALSE)
           {
              cell->bs.tone_detected = FALSE;
           }
       }
       else if ( cell->try_no > 0 && !tone_in_progress && cell->bs.bsic_known==FALSE && !cell->bs.tone_detected )
       {
           tone_in_progress = TRUE;
           if ( cell->rank < 3 ) {
               strong_tone_unknown_cell_top3 = cell;
           }
           else if ( cell->rank < 6 ) {
               strong_tone_unknown_cell_bottom3 = cell;
           }
           else {
               tone_unknown_cell_outOftop6 = cell;
           }
       }
   }

   if (l1_sc_wcdma_ded_pending(gas_id))
   {
     wcdma_srch_pending = TRUE;
   }

   /* Loop through sorted cells (clipped to  MAX_NCELLS_TO_DECODE ) */
   /*
      Categorize them based on whether the BSIC is known (SCH decoded ),
      tone detected (fcch decoded but sch not yet decoded ) or tone
      unknown ( haven't decoded the fcch yet
   */
   for ( n = 0; n < num_cells ; ++n )
   {
      cell = l1_sc_globals_ptr->sorted_cells[ n ];

      if( ( cell->bs.bsic_known == TRUE )&& (( cell->next_tick + ( 3*TEN_SECONDS ) ) < l1_sc_globals_ptr->surround_tick ))
      {
        MSG_GERAN_HIGH_0_G("The cell is back in top 10 after more than 30 seconds, reconfirm SCH at next available opportunity");
        /* Decode at next available opportunity */
        cell->next_tick = l1_sc_globals_ptr->surround_tick;
        cell->in_fade = TRUE;
        cell->cell_stale_sch = TRUE;
      }

      /* If Ncell rx power < -112 dBm don't do a Ncell decode */
      if ( (l1_sc_rx_power_acceptable(cell->bs.rx_power) ) || ( ( schd_fading_cell == FALSE ) && ( cell->bs.bsic_known )))
      {
          cell_above_pwr_threshold = cell;
      }
      else
      {
          cell_above_pwr_threshold = NULL;
      }

      {
         if ( cell->bs.bsic_known )
         {
             /* If cell is below absolute threshold or if the current rx lvl
                has dropped by more than 10dB from the last known level,
                start SCH decodes immediately.  This is to help us drop the
                cell faster
             */
             if ( ( cell_above_pwr_threshold == NULL )||
                  ( !l1_sc_rx_power_drop_acceptable (cell->bs.rx_power - cell->bs.prev_rx_power ) ) )
             {
                MSG_GERAN_HIGH_2_G("Cell %d drop=%d Trig SCH",cell->bs.arfcn.num, cell->bs.rx_power - cell->bs.prev_rx_power);
                /* Decode at next available opportunity */
                cell->next_tick = l1_sc_globals_ptr->surround_tick;
                cell_above_pwr_threshold = cell;
                cell->in_fade = TRUE;
             }
             /*
             Compute serving cell's frame position on the surrounding
             cell's 51 multiframe using the FN lag between the two cells
             */

            /* If the SCH wrapped around, compensate for it in the
               scheduling
            */
            if ( cell->bs.sch_wrap_direction == 1)
            {
               fn_lag = ADD_FN ( cell->bs.fn_lag, 51);
            }
            else if ( cell->bs.sch_wrap_direction == -1)
            {
               fn_lag = SUB_FN ( cell->bs.fn_lag, 51);
            }
            else
            {
               fn_lag = cell->bs.fn_lag;
            }

            if ( cell->bs.qbit_lag < 0 )
            {
               fn_mod_51 = ( next_pre_idle_fn - fn_lag ) MODULO 51;
            }
            else
            {
                fn_mod_51 = ( (next_pre_idle_fn - fn_lag) + 1 ) MODULO 51;
            }

            if ( next_chance_to_get_sb[ fn_mod_51 ] == 0 )
            {
                 MSG_GERAN_HIGH_1_G("SB scheduling fn_mod_51 %d",fn_mod_51);
                /* BSIC known but cell not confirmed within last 10 secs*/
                if (cell->next_tick <= l1_sc_globals_ptr->surround_tick)
                {
                    if ( cell->in_fade )
                    {
                       if (known_fading_cell == NULL)
                       {
                           known_fading_cell = cell_above_pwr_threshold;
                       }
                    }
                    else
                    {
                       if (known_unconfirmed_cell == NULL)
                       {
                          known_unconfirmed_cell =  cell_above_pwr_threshold;
                       }
                    }

                }
#ifdef FEATURE_GSM_SYNQ_SUPPORT
/*
  Schedule a SCH reconfirm for rogue NCELL detected by synq algo
*/
                else if(cell->gdet_based_sch_reconfirm)
                {
                    if(power_mon_gdet_rogue_cell == NULL)
                    {
                        power_mon_gdet_rogue_cell = cell_above_pwr_threshold;
                    }
                }
#endif
                else
                {
                    /* BSIC known and cell confirmed within the last 10 secs */
                    if ( known_confirmed_cell == NULL)
                    {
                        known_confirmed_cell =  cell_above_pwr_threshold;

                    }
                }
            }
         }
         else if (cell->bs.tone_detected)
         {
            uint32 sc_FN = l1_sc_get_fn(gas_id);
             /*
             Check if SCH is expected in the idle frame of this 26 multiframe*/
            if (( SUB_FN ( cell->last_tone_fn + cell->next_sb_expected , sc_FN)) < MULTIFRAME_26)
            {
                if (cell->rank < 6)
                {
                    /* FCCH known, SCH unknown - cell in best 6 */
                    if (strong_sch_unknown_cell == NULL)
                    {
                        strong_sch_unknown_cell =  cell_above_pwr_threshold;
                    }

                }
                else
                {
                    /* FCCH known, SCH unknown - cell not in best 6 */
                    if (sch_unknown_cell == NULL)
                    {
                        sch_unknown_cell =  cell_above_pwr_threshold;
                    }

                }

                /*
                Predict when the next SCH might be received
                in case the SCH is not successfully decoded
                Moved this here from good_sb and bad_sb because
                the cell is not guaranteed a SCH decode attempt
                could be pre-empted by a higher priority cell.  In that
                case, next_sb_expected would not be updated if it was in
                those functions.
                */
                if (cell->idle_frames_to_next_sb == 9)
                {
                    cell->next_sb_expected += 2*MULTIFRAME_26;
                    cell->idle_frames_to_next_sb = 11;
                }
                else
                {
                    cell->next_sb_expected += 9*MULTIFRAME_26;
                    if (cell->idle_frames_to_next_sb == 11)
                     {
                        cell->idle_frames_to_next_sb = 20;
                     }
                     else
                     {
                        cell->idle_frames_to_next_sb = 9;
                     }
                }

             }
         }
         else if (cell_above_pwr_threshold != NULL && !tone_in_progress)
         {
                uint32 sc_FN = l1_sc_get_fn(gas_id);

                if ( cell->fn_when_lost > 0 )
                {
                   deltafnlost = (int32)SUB_FN( sc_FN, (uint32)cell->fn_when_lost );
                }
                else
                {
                   deltafnlost = (int32)ADD_FN( sc_FN, (uint32)(-cell->fn_when_lost) );
                }

                /* Find and schedule the unknown cell from each group that
                   hasn't been searched in the longest time.  The groups are
                   the top 3, bottom 3, and out of the top 6.  Also find the
                   most recently searched unknown cell from the bottom 2 groups.
                   Later we will choose when to cancel the higher power groups in
                   favor of the lower power groups based on this information.
                */

                if (cell->rank < 3)
                {
                   if ( deltafnlost > max_deltafnlost_top3 )
                   {
                      strong_tone_unknown_cell_top3 =  cell_above_pwr_threshold;
                      max_deltafnlost_top3 = deltafnlost;
                   }

                }
                else if (cell->rank < 6)
                {
                   if ( deltafnlost > max_deltafnlost_bottom3 )
                   {
                      strong_tone_unknown_cell_bottom3 =  cell_above_pwr_threshold;
                      max_deltafnlost_bottom3 = deltafnlost;
                   }
                   if ( deltafnlost < min_deltafnlost_bottom3 )
                   {
                      min_deltafnlost_bottom3 = deltafnlost;
                   }

                }
                else
                {
                    if ( deltafnlost > max_deltafnlost_outOftop6 )
                    {
                       tone_unknown_cell_outOftop6 =  cell_above_pwr_threshold;
                       max_deltafnlost_outOftop6 = deltafnlost;
                    }
                    if ( deltafnlost < min_deltafnlost_outOftop6 )
                    {
                       min_deltafnlost_outOftop6 = deltafnlost;
                    }

                }
         }
      } //if in ba_list
   }//for

   /* Cancel any scheduled top 3 tone unknown search if all cells from the top3 have been searched
      since the last bottom 3 search.  Keep in mind the default values if there are no unknown
      cells in a given group when editing this section.  If tone in progress there won't be multiple
      cells scheduled so this code doesn't change anything.
   */
   if ( strong_tone_unknown_cell_bottom3 != NULL && min_deltafnlost_bottom3 > max_deltafnlost_top3 )
   {
      strong_tone_unknown_cell_top3 = NULL;
   }

   /* Cancel any scheduled top 3 and bottom 3 tone unknown search if a cell out of the top 6
      needs to be searched.  The desired algorithm when all cells are unknown is:
              ABC D ABC E ABC F ABC G, repeat with H instead of G, etc
      where cell powers from strongest to weakest are A, B, etc.  If tone in progress there won't
      be multiple cells scheduled so this code doesn't change anything.
   */
   if ( tone_unknown_cell_outOftop6 != NULL
         && min_deltafnlost_outOftop6 > max_deltafnlost_bottom3
         && min_deltafnlost_outOftop6 > max_deltafnlost_top3
         && min_deltafnlost_bottom3 > max_deltafnlost_top3      )
   {
      strong_tone_unknown_cell_top3 = NULL;
      strong_tone_unknown_cell_bottom3 = NULL;
   }

   /*
      SCH unknown cells get high priority irrespective of whether they are
      in the top 6 or not.  Once you don't decode the SCH after the tone at
      the first available opportunity (2 idle frames) we fall out of sync
   */
   if ( strong_sch_unknown_cell )
   {
      l1_sc_receive_sb_tch( strong_sch_unknown_cell, sb_done_tch, gas_id );
   }
   else if ( sch_unknown_cell )
   {
      l1_sc_receive_sb_tch( sch_unknown_cell, sb_done_tch, gas_id );
   }
   /* A known fading cell is given first priority since its just a one
      frame SCH and we want to drop/reconfirm him asap
   */
   else if ( known_fading_cell )
   {
      l1_sc_receive_sb_tch( known_fading_cell, sb_done_tch, gas_id );
   }
#ifdef FEATURE_GSM_SYNQ_SUPPORT
   else if ( power_mon_gdet_rogue_cell )
   {
      l1_sc_receive_sb_tch( power_mon_gdet_rogue_cell, sb_done_tch, gas_id );
      MSG_GERAN_MED_1_G("SyNQ_FR: Scheduling rogue cell, cnt %d", power_mon_gdet_rogue_cell->bad_gdet_pwr_mon_cntr);
      power_mon_gdet_rogue_cell->gdet_based_sch_reconfirm = FALSE;
      power_mon_gdet_rogue_cell->bad_gdet_pwr_mon_cntr = 0;
   }
#endif
   else if (l1_sc_wcdma_ded_ready(FALSE,gas_id))
   {
     wcdma_srch_scheduled = TRUE;
   }
   else if (known_unconfirmed_cell)
   {
      l1_sc_receive_sb_tch( known_unconfirmed_cell, sb_done_tch, gas_id );
   }
   /* No FCCH decoded, cell in top 3 */
   else if( ( strong_tone_unknown_cell_top3 ) &&  !(strong_tone_unknown_cell_top3->bs.tone_detected) )
   {
       /*
       Search 22 idle frames for the FCCH
       22 = 2 attempts * 11 since we are guaranteed to get a FCCH
       within 11 idle frames
       */
       if (strong_tone_unknown_cell_top3->try_no <= MAX_TCH_TONE_DETECT_ATTEMPTS)
       {
           /* Trigger NCELL_ACQ_START only on first tone detect attempt */
	      if(l1_sc_globals_ptr->last_top3_fcch_attempted_cell && 
            ((l1_sc_globals_ptr->last_top3_fcch_attempted_cell->rank != L1_SC_NO_RANK) && (l1_sc_globals_ptr->last_top3_fcch_attempted_cell->rank < l1_sc_globals_ptr->sorted_cell_count)) &&
            (strong_tone_unknown_cell_top3->bs.arfcn.num != l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.arfcn.num) &&
            /*Dont give up on previous attempted cell went bad by just 1dBm*/
            ((l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.rx_power <= (strong_tone_unknown_cell_top3->bs.rx_power)) && (l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.rx_power >= (strong_tone_unknown_cell_top3->bs.rx_power - 16))))
         {
           if (l1_sc_globals_ptr->last_top3_fcch_attempted_cell->try_no < MAX_TCH_TONE_DETECT_ATTEMPTS)
           {
             l1_sc_globals_ptr->last_top3_fcch_attempted_cell->try_no++;
             l1_sc_receive_fcb_tch( l1_sc_globals_ptr->last_top3_fcch_attempted_cell, fcb_done_tch, gas_id );
             MSG_GERAN_HIGH_5_G("Ncell schedule Fcch:stored fcch cell arfcn %d tryno. %d power %d dBm Current cell arfcn %d power %d dBm",
             l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.arfcn.num,l1_sc_globals_ptr->last_top3_fcch_attempted_cell->try_no,l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.rx_power/16,
             strong_tone_unknown_cell_top3->bs.arfcn.num,strong_tone_unknown_cell_top3->bs.rx_power/16);
           }
           else
           {
             l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                              l1_sc_globals_ptr->last_top3_fcch_attempted_cell->rank,
                              l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.arfcn,
                              l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.rx_power, gas_id );
             
             l1_sc_globals_ptr->last_top3_fcch_attempted_cell->try_no = 0;
            
             /* Save fn when cell was lost.  Used in scheduling */
             l1_sc_globals_ptr->last_top3_fcch_attempted_cell->fn_when_lost = (int32)l1_sc_get_fn(gas_id);
             MSG_GERAN_MED_1_G("Ncell Tone detect failed: ARFCN = %d",l1_sc_globals_ptr->last_top3_fcch_attempted_cell->bs.arfcn.num);
             l1_sc_globals_ptr->last_top3_fcch_attempted_cell = NULL;
           }
         }
         else
         {
           if ( 0 == strong_tone_unknown_cell_top3->try_no )
           {
              /* TAP POINT FOR NCELL_ACQ_START */
              l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                                strong_tone_unknown_cell_top3->rank,
                                strong_tone_unknown_cell_top3->bs.arfcn,
                                strong_tone_unknown_cell_top3->bs.rx_power, gas_id );

              MSG_GERAN_HIGH_2_G("Ncell Tone Start Cell: %d, Rank %d",strong_tone_unknown_cell_top3->bs.arfcn.num,strong_tone_unknown_cell_top3->rank);
           }
           strong_tone_unknown_cell_top3->try_no++;
		   l1_sc_globals_ptr->last_top3_fcch_attempted_cell = strong_tone_unknown_cell_top3;
           l1_sc_receive_fcb_tch( strong_tone_unknown_cell_top3, fcb_done_tch, gas_id );
		}
       }
       else
       {
           /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
           l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                             strong_tone_unknown_cell_top3->rank,
                             strong_tone_unknown_cell_top3->bs.arfcn,
                             strong_tone_unknown_cell_top3->bs.rx_power, gas_id );

           strong_tone_unknown_cell_top3->try_no = 0;
		   l1_sc_globals_ptr->last_top3_fcch_attempted_cell = NULL;

           /* Save fn when cell was lost.  Used in scheduling */
           strong_tone_unknown_cell_top3->fn_when_lost = (int32)l1_sc_get_fn(gas_id);
           MSG_GERAN_MED_1_G("Ncell Tone detect failed: ARFCN = %d",strong_tone_unknown_cell_top3->bs.arfcn.num);
        }
   }
   /* No FCCH decoded, cell in bottom 3 */
   else if( ( strong_tone_unknown_cell_bottom3 ) && !(strong_tone_unknown_cell_bottom3->bs.tone_detected) )
   {
        l1_sc_globals_ptr->last_top3_fcch_attempted_cell = NULL;
       if (strong_tone_unknown_cell_bottom3->try_no <= MAX_TCH_TONE_DETECT_ATTEMPTS)
       {
           /* Trigger NCELL_ACQ_START only on first tone detect attempt */
           if ( 0 == strong_tone_unknown_cell_bottom3->try_no )
           {
              /* TAP POINT FOR NCELL_ACQ_START */
              l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                                strong_tone_unknown_cell_bottom3->rank,
                                strong_tone_unknown_cell_bottom3->bs.arfcn,
                                strong_tone_unknown_cell_bottom3->bs.rx_power, gas_id );

              MSG_GERAN_HIGH_2_G("Ncell Tone Start Cell: %d, Rank %d",strong_tone_unknown_cell_bottom3->bs.arfcn.num,strong_tone_unknown_cell_bottom3->rank);
           }


           strong_tone_unknown_cell_bottom3->try_no++;
           l1_sc_receive_fcb_tch( strong_tone_unknown_cell_bottom3, fcb_done_tch, gas_id );
       }
       else
       {
           /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
           l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                             strong_tone_unknown_cell_bottom3->rank,
                             strong_tone_unknown_cell_bottom3->bs.arfcn,
                             strong_tone_unknown_cell_bottom3->bs.rx_power, gas_id );

           strong_tone_unknown_cell_bottom3->try_no = 0;

           /* Save fn when cell was lost.  Used in scheduling */
           strong_tone_unknown_cell_bottom3->fn_when_lost = (int32)l1_sc_get_fn(gas_id);
           MSG_GERAN_MED_1_G("Ncell Tone detect failed: ARFCN = %d",strong_tone_unknown_cell_bottom3->bs.arfcn.num);
        }


   }

   /* FCCH not decoded, cell out of top 6 */
   else if( ( tone_unknown_cell_outOftop6 ) && !(tone_unknown_cell_outOftop6->bs.tone_detected) )
   {
       /*
         Search 22 idle frames for the FCCH
        22 = 2 attempts * 11 since we are guaranteed to get a FCCH
        within 11 idle frames
       */
       if (tone_unknown_cell_outOftop6->try_no <= MAX_TCH_TONE_DETECT_ATTEMPTS)
       {
           /* Trigger NCELL_ACQ_START only on first tone detect attempt */
           if ( 0 == tone_unknown_cell_outOftop6->try_no )
           {
              /* TAP POINT FOR NCELL_ACQ_START */
              l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                                tone_unknown_cell_outOftop6->rank,
                                tone_unknown_cell_outOftop6->bs.arfcn,
                                tone_unknown_cell_outOftop6->bs.rx_power, gas_id );

           }
           tone_unknown_cell_outOftop6->try_no++;
           l1_sc_receive_fcb_tch( tone_unknown_cell_outOftop6, fcb_done_tch, gas_id );
       }
       else
       {
           /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
           l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                             tone_unknown_cell_outOftop6->rank,
                             tone_unknown_cell_outOftop6->bs.arfcn,
                             tone_unknown_cell_outOftop6->bs.rx_power, gas_id );
           tone_unknown_cell_outOftop6->try_no = 0;

           /* Save fn when cell was lost.  Used in scheduling */
           tone_unknown_cell_outOftop6->fn_when_lost = (int32)l1_sc_get_fn(gas_id);

           MSG_GERAN_MED_1_G("Ncell Tone detect failed: ARFCN = %d",tone_unknown_cell_outOftop6->bs.arfcn.num);
       }
   }
   else if (known_confirmed_cell)
   {
      //l1_sc_receive_sb_tch( known_confirmed_cell, sb_done_tch );
   }

   l1_sc_wcdma_ded_tick(wcdma_srch_pending, wcdma_srch_scheduled, gas_id);
   l1_sc_globals_ptr->send_tch_tick = TRUE;
}

/*
** For G2L changes , l1_sc_wcdma_srch_active() func an check both wcdma srch flags.
*/
void l1_sci_tick_dedi_sdcch( uint32 available_frames, uint8 stopping, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

   ASSERT_INT();

   l1_fm_tick(available_frames, gas_id);

   if ( !l1_sc_globals_ptr->receive_in_progress && !stopping
        && !l1_sc_wcdma_srch_active(gas_id)
      )
   {
      next_cell_sdcch( NULL, gas_id );
   }

   l1_sc_wcdma_sdcch_tick(gas_id);

   l1_sci_tick_receive(gas_id);
}

void l1_sci_tick_dedi_tch( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_INT();

   gas_id = check_gas_id(gas_id);


   if ( l1_sc_globals_ptr->send_tch_tick )
   {
      /* Moving this to ISR to prevent race condition - David
         Symptoms are "Too many rx msgs in ncell_fcch_rx_state_machine
      */
      l1_sc_tch_tick ( NULL, gas_id );
   }
   /* l1_sci_check_available_frames not needed on TCh */
}
/**********************************************/

void fcb_done_tch( cell_T* cell, gas_id_t gas_id )
{

    dword     start_fn;
    int16     timing_error;

    if (!cell)
    {
       MSG_GERAN_ERROR_0_G("Rcvd NULL cell pointer");
       return;
    }

    ASSERT_TASK();


   if ( l1_sc_read_fcb(&start_fn, &timing_error, gas_id ) )
   {
      if ( !cell->bs.tone_detected )
      {
         cell->bs.tone_detected = TRUE;

         //Reset try number for SCH decodes
         cell->try_no = 0;

      }

      /*
      FN lag is the frame number difference between the serving and surrounding cells.
      Cannot determine it unless the SCH is decoded.  Set to 0 for now
      */

      cell->bs.fn_lag = 0;
      cell->bs.qbit_lag     = timing_error;

      //Tacks on new qbit_lag to any existing qbit_lag in the cell
      //l1_sc_adjust_lag( &cell->bs, timing_error );

      /*
      Store the last occurence of the tone to predict when to expect
      the SCH relative to the tone
      */
      cell->last_tone_fn = start_fn;

      /*
      Serving cell idle always lines up with the SC SCH 2 multiframes
      after the SC FCCH lines up with the serving cell's idle
      */
      cell->next_sb_expected = 2 * MULTIFRAME_26;
      cell->idle_frames_to_next_sb = 2;
      cell->first_ncell_sch_attempt = TRUE;
      cell->bs.fn_when_read = gl1_get_FN(gas_id);
      cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
   }

/* This will repeatedly do a tone detect every idle frame - No SCH decodes */
#ifdef DEBUG_CONTINUOUS_TONES
   cell->bs.tone_detected = FALSE;
   cell->try_no = 0;
#endif

}

#ifdef FEATURE_GSM_DTM
/*===========================================================================

FUNCTION  next_cell_dtm

DESCRIPTION
  This function decides which ncell to target for the reception of FCH/SCH
  it is a clone of the existing dedicated function with the addition of the
  check to see if the ncell sch can knock off the last TX burst

DEPENDENCIES
  none

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
static void next_cell_dtm(gas_id_t gas_id)
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   dword   pre_idle_frame_no_mod_26;
   dword   next_pre_idle_fn;
   cell_T* cell;
   cell_T* cell_above_pwr_threshold;
   word    fn_mod_51;
   word    n;
   uint8   num_cells;
   int32   deltafnlost = 0;
   int32   max_deltafnlost_top3 = 0;
   int32   min_deltafnlost_bottom3 = 0x7fffffff;
   int32   max_deltafnlost_bottom3 = 0;
   int32   min_deltafnlost_outOftop6 = 0x7fffffff;
   int32   max_deltafnlost_outOftop6 = 0;
   boolean tone_in_progress = FALSE;
   uint32  fn_lag;

   cell_T* strong_tone_unknown_cell_top3 = NULL;
   cell_T* strong_tone_unknown_cell_bottom3 = NULL;
   cell_T* strong_sch_unknown_cell = NULL;
   cell_T* tone_unknown_cell_outOftop6 = NULL;
   cell_T* sch_unknown_cell = NULL;
   cell_T* known_confirmed_cell = NULL;
   cell_T* known_unconfirmed_cell = NULL;
   cell_T* known_fading_cell = NULL;

   boolean wcdma_srch_scheduled = FALSE;
   boolean wcdma_srch_pending = FALSE;
   boolean schd_fading_cell = gpl1_schedule_fading_cell(gas_id);

   ASSERT_TASK();

   MSG_GERAN_HIGH_0_G("NEXT CELL DTM");

   /* calculate frame number of next pre idle frame */
   /*
      tch_idle_frame is passed in from the l1_tch and depends on timeslot/subchannel
   */
   pre_idle_frame_no_mod_26 = tch_idle_frame[gas_id] - 1;

   /*
   add 26+FRAMES_PER_HYPERFRAME so subtract of bs.fn_lag
   won't generate negative value
   */

   next_pre_idle_fn = l1_sc_get_fn(gas_id) + 26 + FRAMES_PER_HYPERFRAME;
   next_pre_idle_fn -= ( next_pre_idle_fn - pre_idle_frame_no_mod_26 ) MODULO 26;


   /* Clip the number of cells to decode to  MAX_NCELLS_TO_DECODE (10) */
   num_cells = ( l1_sc_globals_ptr->sorted_cell_count > MAX_NCELLS_TO_DECODE ) ?
               MAX_NCELLS_TO_DECODE:
               l1_sc_globals_ptr->sorted_cell_count;

   /*
   Check for a tone detect in progress and clear out tone detects for weaker cells or
   cells outside the top MAX_NCELLS_TO_DECODE
   */
   for ( n = 0; n < l1_sc_globals_ptr->sorted_cell_count ; ++n )
   {
       cell = l1_sc_globals_ptr->sorted_cells[ n ];

       if ( n>=MAX_NCELLS_TO_DECODE || !l1_sc_rx_power_acceptable(cell->bs.rx_power) )
       {
           if ( cell->try_no > 0 ) {
               cell->try_no = 0;
               l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                                    cell->rank,
                                    cell->bs.arfcn,
                                    cell->bs.rx_power, gas_id );

               /*Give SCH 1 chance before stopping any more decode attempts */
               cell->bs.bsic_known = FALSE;
           }
           /*If we haven't detected SCH on ncell we don't want to start*/
           if (cell->bs.bsic_known == FALSE)
           {
              cell->bs.tone_detected = FALSE;
           }
       }
       /* If we just use !cell->bs.bsic_known, we have the corner case of a cell
        * whose tone is detected but whose SCH has failed being rescheduled
        * again
        */
       else if ( cell->try_no > 0 && !tone_in_progress && cell->bs.bsic_known==FALSE && !cell->bs.tone_detected )
       {
           tone_in_progress = TRUE;
           if ( cell->rank < 3 ) {
               strong_tone_unknown_cell_top3 = cell;
           }
           else if ( cell->rank < 6 ) {
               strong_tone_unknown_cell_bottom3 = cell;
           }
           else {
               tone_unknown_cell_outOftop6 = cell;
           }
       }
   }

   if (l1_sc_wcdma_ded_pending(gas_id))
   {
     wcdma_srch_pending = TRUE;
   }

   /* Loop through sorted cells (clipped to  MAX_NCELLS_TO_DECODE ) */
   /*
      Categorize them based on whether the BSIC is known (SCH decoded ),
      tone detected (fcch decoded but sch not yet decoded ) or tone
      unknown ( haven't decoded the fcch yet
   */
   for ( n = 0; n < num_cells ; ++n )
   {
      cell = l1_sc_globals_ptr->sorted_cells[ n ];

      /* If Ncell rx power < -112 dBm don't do a Ncell decode */
      if ( ( l1_sc_rx_power_acceptable(cell->bs.rx_power) ) || ( ( schd_fading_cell == FALSE ) && ( cell->bs.bsic_known )) )
      {
          cell_above_pwr_threshold = cell;
      }
      else
      {
          cell_above_pwr_threshold = NULL;
      }

      {
         if ( cell->bs.bsic_known )
         {
             /* If cell is below absolute threshold or if the current rx lvl
                has dropped by more than 10dB from the last known level,
                start SCH decodes immediately.  This is to help us drop the
                cell faster
             */
             if ( ( cell_above_pwr_threshold == NULL )||
                  ( !l1_sc_rx_power_drop_acceptable ( cell->bs.rx_power - cell->bs.prev_rx_power ) )
                )
             {
                MSG_GERAN_HIGH_3_G("Cell %d drop=%d Trig SCH",
                  cell->bs.arfcn.num,
                  cell->bs.rx_power - cell->bs.prev_rx_power,
                  0  );

                /* Decode at next available opportunity */
                cell->next_tick = l1_sc_globals_ptr->surround_tick;
                cell_above_pwr_threshold = cell;
                cell->in_fade = TRUE;
             }
             /*
             Compute serving cell's frame position on the surrounding
             cell's 51 multiframe using the FN lag between the two cells
             */

            /* If the SCH wrapped around, compensate for it in the
               scheduling
            */
            if ( cell->bs.sch_wrap_direction == 1)
            {
               fn_lag = ADD_FN ( cell->bs.fn_lag, 51);
            }
            else if ( cell->bs.sch_wrap_direction == -1)
            {
               fn_lag = SUB_FN ( cell->bs.fn_lag, 51);
            }
            else
            {
               fn_lag = cell->bs.fn_lag;
            }

            if ( cell->bs.qbit_lag < 0 )
            {
               fn_mod_51 = ( next_pre_idle_fn - fn_lag ) MODULO 51;
            }
            else
            {
                fn_mod_51 = ( (next_pre_idle_fn - fn_lag) + 1 ) MODULO 51;
            }

            if ( next_chance_to_get_sb[ fn_mod_51 ] == 0 )
            {
                /* BSIC known but cell not confirmed within last 10 secs*/
                if (cell->next_tick <= l1_sc_globals_ptr->surround_tick)
                {
                    if ( cell->in_fade )
                    {
                       if (known_fading_cell == NULL)
                       {
                           known_fading_cell = cell_above_pwr_threshold;
                       }
                    }
                    else
                    {
                       if (known_unconfirmed_cell == NULL)
                       {
                          known_unconfirmed_cell =  cell_above_pwr_threshold;
                       }
                    }

                }
                else
                {
                    /* BSIC known and cell confirmed within the last 10 secs */
                    if ( known_confirmed_cell == NULL)
                    {
                        known_confirmed_cell =  cell_above_pwr_threshold;

                    }
                }
            }
         }
         else if (cell->bs.tone_detected)
         {
            uint32 sc_FN = l1_sc_get_fn(gas_id);
             /*
             Check if SCH is expected in the idle frame of this 26 multiframe*/
            if (( SUB_FN ( cell->last_tone_fn + cell->next_sb_expected , sc_FN)) <
                  MULTIFRAME_26
               )
            {
                if (cell->rank < 6)
                {
                    /* FCCH known, SCH unknown - cell in best 6 */
                    if (strong_sch_unknown_cell == NULL)
                    {
                        strong_sch_unknown_cell =  cell_above_pwr_threshold;
                    }

                }
                else
                {
                    /* FCCH known, SCH unknown - cell not in best 6 */
                    if (sch_unknown_cell == NULL)
                    {
                        sch_unknown_cell =  cell_above_pwr_threshold;
                    }

                }

                /*
                Predict when the next SCH might be received
                in case the SCH is not successfully decoded
                Moved this here from good_sb and bad_sb because
                the cell is not guaranteed a SCH decode attempt
                could be pre-empted by a higher priority cell.  In that
                case, next_sb_expected would not be updated if it was in
                those functions.
                */
                if (cell->idle_frames_to_next_sb == 9)
                {
                    cell->next_sb_expected += 2*MULTIFRAME_26;
                    cell->idle_frames_to_next_sb = 11;
                }
                else
                {
                    cell->next_sb_expected += 9*MULTIFRAME_26;
                    if (cell->idle_frames_to_next_sb == 11)
                    {
                       cell->idle_frames_to_next_sb = 20;
                    }
                    else
                    {
                        cell->idle_frames_to_next_sb = 9;
                    }
                }

             }
         }
         else if (cell_above_pwr_threshold != NULL && !tone_in_progress)
         {
                uint32 sc_FN = l1_sc_get_fn(gas_id);

                if ( cell->fn_when_lost > 0 )
                {
                   deltafnlost = (int32)SUB_FN( sc_FN, (uint32)cell->fn_when_lost );
                }
                else
                {
                   deltafnlost = (int32)ADD_FN( sc_FN, (uint32)(-cell->fn_when_lost) );
                }

                /* Find and schedule the unknown cell from each group that
                   hasn't been searched in the longest time.  The groups are
                   the top 3, bottom 3, and out of the top 6.  Also find the
                   most recently searched unknown cell from the bottom 2 groups.
                   Later we will choose when to cancel the higher power groups in
                   favor of the lower power groups based on this information.
                */

                if (cell->rank < 3)
                {
                   if ( deltafnlost > max_deltafnlost_top3 )
                   {
                      strong_tone_unknown_cell_top3 =  cell_above_pwr_threshold;
                      max_deltafnlost_top3 = deltafnlost;
                   }

                }
                else if (cell->rank < 6)
                {
                   if ( deltafnlost > max_deltafnlost_bottom3 )
                   {
                      strong_tone_unknown_cell_bottom3 =  cell_above_pwr_threshold;
                      max_deltafnlost_bottom3 = deltafnlost;
                   }
                   if ( deltafnlost < min_deltafnlost_bottom3 )
                   {
                      min_deltafnlost_bottom3 = deltafnlost;
                   }

                }
                else
                {
                    if ( deltafnlost > max_deltafnlost_outOftop6 )
                    {
                       tone_unknown_cell_outOftop6 =  cell_above_pwr_threshold;
                       max_deltafnlost_outOftop6 = deltafnlost;
                    }
                    if ( deltafnlost < min_deltafnlost_outOftop6 )
                    {
                       min_deltafnlost_outOftop6 = deltafnlost;
                    }

                }
         }
      } //if in ba_list
   }//for

   /* Cancel any scheduled top 3 tone unknown search if all cells from the top3 have been
   searched since the last bottom 3 search.  Keep in mind the default values if there are
   no unknown cells in a given group when editing this section.  If tone in progress
   there won't be multiple cells scheduled so this code doesn't change anything.
   */
   if ( strong_tone_unknown_cell_bottom3 != NULL &&
        min_deltafnlost_bottom3 > max_deltafnlost_top3
      )
   {
      strong_tone_unknown_cell_top3 = NULL;
   }

   /* Cancel any scheduled top 3 and bottom 3 tone unknown search if a cell out of the
   top 6 needs to be searched.  The desired algorithm when all cells are unknown is:
   ABC D ABC E ABC F ABC G, repeat with H instead of G, etc where cell powers from
   strongest to weakest are A, B, etc.  If tone in progress there won't be multiple
   cells scheduled so this code doesn't change anything.
   */
   if ( tone_unknown_cell_outOftop6 != NULL
         && min_deltafnlost_outOftop6 > max_deltafnlost_bottom3
         && min_deltafnlost_outOftop6 > max_deltafnlost_top3
         && min_deltafnlost_bottom3 > max_deltafnlost_top3      )
   {
      strong_tone_unknown_cell_top3 = NULL;
      strong_tone_unknown_cell_bottom3 = NULL;
   }

   /*
      SCH unknown cells get high priority irrespective of whether they are
      in the top 6 or not.  Once you don't decode the SCH after the tone at
      the first available opportunity (2 idle frames) we fall out of sync
   */
   if ( strong_sch_unknown_cell )
   {
     gpl1_dtm_gprs_schedule_sch_if_allowed( strong_sch_unknown_cell, gas_id );
   }
   else if ( sch_unknown_cell )
   {
     gpl1_dtm_gprs_schedule_sch_if_allowed( sch_unknown_cell, gas_id );
   }
   /* A known fading cell is given first priority since its just a one
      frame SCH and we want to drop/reconfirm him asap
   */
   else if ( known_fading_cell )
   {
     gpl1_dtm_gprs_schedule_sch_if_allowed( known_fading_cell, gas_id );
   }
   else if (l1_sc_wcdma_ded_ready(FALSE,gas_id))
   {
     wcdma_srch_scheduled = TRUE;
   }
   else if (known_unconfirmed_cell)
   {
     gpl1_dtm_gprs_schedule_sch_if_allowed( known_unconfirmed_cell, gas_id );
   }
   /* No FCCH decoded, cell in top 3 */
   else if ( strong_tone_unknown_cell_top3 )
   {
       /*
       Search 22 idle frames for the FCCH
       22 = 2 attempts * 11 since we are guaranteed to get a FCCH
       within 11 idle frames
       */
       if (strong_tone_unknown_cell_top3->try_no <= MAX_TCH_TONE_DETECT_ATTEMPTS)
       {
           /* Trigger NCELL_ACQ_START only on first tone detect attempt */
           if ( 0 == strong_tone_unknown_cell_top3->try_no )
           {
              /* TAP POINT FOR NCELL_ACQ_START */
              l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                                strong_tone_unknown_cell_top3->rank,
                                strong_tone_unknown_cell_top3->bs.arfcn,
                                strong_tone_unknown_cell_top3->bs.rx_power, gas_id );

              MSG_GERAN_HIGH_3_G("Ncell Tone Start Cell: %d, Rank %d",
                strong_tone_unknown_cell_top3->bs.arfcn.num,
                strong_tone_unknown_cell_top3->rank,
                0);
           }
           strong_tone_unknown_cell_top3->try_no++;
           l1_sc_receive_fcb_tch( strong_tone_unknown_cell_top3, fcb_done_tch, gas_id );
       }
       else
       {
           /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
           l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                             strong_tone_unknown_cell_top3->rank,
                             strong_tone_unknown_cell_top3->bs.arfcn,
                             strong_tone_unknown_cell_top3->bs.rx_power, gas_id );

           strong_tone_unknown_cell_top3->try_no = 0;

           /* Save fn when cell was lost.  Used in scheduling */
           strong_tone_unknown_cell_top3->fn_when_lost = (int32)l1_sc_get_fn(gas_id);
           MSG_GERAN_MED_3_G("Ncell Tone detect failed: ARFCN = %d",
             strong_tone_unknown_cell_top3->bs.arfcn.num,
             0,
             0);
        }
   }
   /* No FCCH decoded, cell in bottom 3 */
   else if ( strong_tone_unknown_cell_bottom3 )
   {
       if (strong_tone_unknown_cell_bottom3->try_no <= MAX_TCH_TONE_DETECT_ATTEMPTS)
       {
           /* Trigger NCELL_ACQ_START only on first tone detect attempt */
           if ( 0 == strong_tone_unknown_cell_bottom3->try_no )
           {
              /* TAP POINT FOR NCELL_ACQ_START */
              l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                                strong_tone_unknown_cell_bottom3->rank,
                                strong_tone_unknown_cell_bottom3->bs.arfcn,
                                strong_tone_unknown_cell_bottom3->bs.rx_power, gas_id );

              MSG_GERAN_HIGH_3_G("Ncell Tone Start Cell: %d, Rank %d",
                strong_tone_unknown_cell_bottom3->bs.arfcn.num,
                strong_tone_unknown_cell_bottom3->rank,
                0);
           }


           strong_tone_unknown_cell_bottom3->try_no++;
           l1_sc_receive_fcb_tch( strong_tone_unknown_cell_bottom3, fcb_done_tch, gas_id );
       }
       else
       {
           /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
           l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                             strong_tone_unknown_cell_bottom3->rank,
                             strong_tone_unknown_cell_bottom3->bs.arfcn,
                             strong_tone_unknown_cell_bottom3->bs.rx_power, gas_id );

           strong_tone_unknown_cell_bottom3->try_no = 0;

           /* Save fn when cell was lost.  Used in scheduling */
           strong_tone_unknown_cell_bottom3->fn_when_lost = (int32)l1_sc_get_fn(gas_id);
           MSG_GERAN_MED_3_G("Ncell Tone detect failed: ARFCN = %d",
             strong_tone_unknown_cell_bottom3->bs.arfcn.num,
             0,
             0);
        }


   }

   /* FCCH not decoded, cell out of top 6 */
   else if ( tone_unknown_cell_outOftop6 )
   {
       /*
         Search 22 idle frames for the FCCH
        22 = 2 attempts * 11 since we are guaranteed to get a FCCH
        within 11 idle frames
       */
       if (tone_unknown_cell_outOftop6->try_no <= MAX_TCH_TONE_DETECT_ATTEMPTS)
       {
           /* Trigger NCELL_ACQ_START only on first tone detect attempt */
           if ( 0 == tone_unknown_cell_outOftop6->try_no )
           {
              /* TAP POINT FOR NCELL_ACQ_START */
              l1_log_ncell_acq( L1_LOG_NCELL_ACQ_START,
                                tone_unknown_cell_outOftop6->rank,
                                tone_unknown_cell_outOftop6->bs.arfcn,
                                tone_unknown_cell_outOftop6->bs.rx_power, gas_id );

           }
           tone_unknown_cell_outOftop6->try_no++;
           l1_sc_receive_fcb_tch( tone_unknown_cell_outOftop6, fcb_done_tch, gas_id );
       }
       else
       {
           /* TAP POINT FOR NCELL_ACQ_FAILURE (failure reason FCCH) */
           l1_log_ncell_acq( L1_LOG_NCELL_ACQ_FAIL_FCCH,
                             tone_unknown_cell_outOftop6->rank,
                             tone_unknown_cell_outOftop6->bs.arfcn,
                             tone_unknown_cell_outOftop6->bs.rx_power, gas_id );
           tone_unknown_cell_outOftop6->try_no = 0;

           /* Save fn when cell was lost.  Used in scheduling */
           tone_unknown_cell_outOftop6->fn_when_lost = (int32)l1_sc_get_fn(gas_id);

           MSG_GERAN_MED_3_G("Ncell Tone detect failed: ARFCN = %d",
             tone_unknown_cell_outOftop6->bs.arfcn.num,
             0,
             0);
       }
   }
   else if (known_confirmed_cell)
   {
      //l1_sc_receive_sb_tch( known_confirmed_cell, sb_done_tch );
   }

   l1_sc_wcdma_ded_tick(wcdma_srch_pending, wcdma_srch_scheduled, gas_id);
   l1_sc_globals_ptr->send_tch_tick = TRUE;
}

/*===========================================================================

FUNCTION  gpl1_dtm_gprs_schedule_sch_if_allowed

DESCRIPTION
  Checks for any clash of TX with the SCH about to be scheduled.

DEPENDENCIES
  new timeslot number

RETURN VALUE
  none

SIDE EFFECTS
  None

===========================================================================*/
static void gpl1_dtm_gprs_schedule_sch_if_allowed ( cell_T *cell, gas_id_t gas_id )
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
  gprs_pl1_serving_cell_meas_T  *gprs_serv_cell_meas_ptr=&gprs_serv_cell_meas[gas_id];
  int ncell_scell_ts_clash_result = 0;
  int highest_tn;
  
  /* Instantiate and initialise the lowest and highest timeslot
   * information structures.
   */
  gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, /* relative timeslot */
                                          FALSE          /* transmit boolean */
                                        };
  gl1_defs_tn_struct  highest_tn_info = { GL1_DEFS_TN_0, /* relative timeslot */
                                          FALSE          /* transmit boolean */
                                        };

  /* Get the values for the highest and the lowest timeslots being used */
  if (trans[gas_id].reconfig_two_close == TRUE)
  {
    /* use old parameters as new have yet to be appled to the pdtcch */
    highest_tn = gprs_serv_cell_meas_ptr->prev_highest_tn;

    trans[gas_id].reconfig_two_close = FALSE;
  }
  else
  {
    highest_tn = gprs_serv_cell_meas_ptr->highest_tn;
  }

  /* The highest TN must be the TX TN so is adjusted by having 3 slots added */
  if ( ( highest_tn - (int)lowest_tn_info.tn)  == 3   )
  {
    /* In Class 8, the highest TS a Tx can lie is on the second highest Rx TS */
    highest_tn += 2;
  }
  else
  {
    highest_tn += 3;
  }

  /* Set the highest timeslot info */
  highest_tn_info.tn = (gl1_defs_tn_type) highest_tn;

  /* For DTM mode if PS Tx in TS6 can spread into next frame, unless about to reconfigure PS */
  if ((highest_tn_info.tn > GL1_DEFS_TN_5) && (get_dtm_isr_state(gas_id) != L1_DTM_PS_RECONFIG))
  {
     lowest_tn_info.tn = GL1_DEFS_TN_1;
  }

  /*
  See if the SCH occurs in the acquisition window i.e. between the highest tn and
  the lowest tn
  */
  ncell_scell_ts_clash_result = gl1_msg_ncell_sch_pos_in_acq_window
                                  (
                                  cell->bs.qbit_lag,
                                  &(lowest_tn_info),
                                  &(highest_tn_info),
                                  gas_id
                                  );

  /* Check the clash result */
  /* @@OG: Force scheduling of the MAX_TRY attempt */

  if(( cell->first_ncell_sch_attempt)||(  ( ncell_scell_ts_clash_result == 0 )
  ||( ( ncell_scell_ts_clash_result == -1 ) &&
    ( MAX_TRIES == cell->try_no )
        &&(FALSE == mcfg_gcf_nv_get_status())
       )
    ))
  {
    /* SCH is in the window so schedule a receive */
    l1_sc_receive_sb_tch( cell, sb_done_tch, gas_id );
    cell->first_ncell_sch_attempt = FALSE;
  }
  else
  {
    /*-----------------------------------------------------------------------------------
         If the BSIC is unknown then do label this as a bad SCH, instead assume that the
         timing info is valid and don't reconfirm.  Reschedule it after 4 seconds
      ---------------------------------------------------------------------------------*/
    if ((cell->bs.bsic_known) && (cell->try_no < MAX_TRIES))
    {
      MSG_GERAN_MED_3_G( "SCH rescheduled later, lotn=%d hitn=%d ARFCN=%d",
               lowest_tn_info.tn,
               highest_tn,
               cell->bs.arfcn.num );

      MSG_GERAN_MED_3_G( "cell->bs.qbit_lag %d",
               cell->bs.qbit_lag,
               0,
               0 );

      cell->try_no++;
      cell->next_tick = l1_sc_globals_ptr->surround_tick + FOUR_SECONDS;
    }
    else
    {
      MSG_GERAN_MED_3_G( "MARK SCH BAD, lotn=%d hitn=%d ARFCN=%d",
               lowest_tn_info.tn,
               highest_tn,
               cell->bs.arfcn.num );

      gpl1_gprs_sc_bad_sb_trans( cell, gas_id );
    }
  }
}

#endif /* FEATURE_GSM_DTM */
