/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              SCE: L1 IDLE and FINDBCCH

GENERAL DESCRIPTION
   This module contains surrounding cell code for idle mode.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_idle.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
01/02/18   km        CR2097153 Do not increment paging_cycle_tick when GBTA gap is opened for power monitors
02/03/16   mn        CR970666 Correcting TC value in PH_DATA_IND for logging purpose
16/11/15   dg        CR936934 Wait for FN tick for timer calculation when received MPH_SPECIFIC_IRAT_FREQ_UPDATE_REQ for G2X forced search
05/10/15   sp        CR898457: Ignore next tick req for SCH/FCCH state configurations in next_cell_bcch
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
30/06/15   ab        CR869795 : SR-DSDS : Perform GBTA with Transfer SUB FCCH/SCH using COEX.
30/06/15   ab        CR851337 : DR-DSDS. Design to allow SR(GBTA) to DR Transition.
26/03/15   br        CR804095 GL1 is not scheduling G2W reconfirm search ,while other sub is in transfer ,If 2G Neighbor cell count is zero
21/04/15   cja       CR826384 For reconf do not use idle monitors in progress flag
30/03/15   cja       CR800650 Optimise irat state machine
24/02/15   ak        CR792387 MPH_GPRS_SURROUND_UPDATE_REQ fixes
23/02/15   mko       CR783222 Backing out CR638933/624044/763966 changes 
12/02/15   am        CR771762 BCCH Decode Failures at Good RSSI when the NCell is drifted by 1.5KHz
10/12/14   ggu       CR785000 Fix the incomplete code merge of CR763966
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
11/12/14   ak        CR739561 SCE opt top2/top6 mode, disable pwr msr of blacklisted cells
10/12/14   ggu       CR763966 Only suspend the TRM release when make sure the IRAT activity will happen at next frame
27/11/14   ak        CR748234 Problems with legacy SCE Opt 16/17/20
22/10/14   sk        CR735697 Ensure GL1 doesnt toggle between SI3/SI4 on TRM denial, and does explicit TRM release before making reservation in non-G only mode 
11/11/14   npt       CR751383 Avoid mutex dead lock. When the Idle ISR runs we shouldn’t post the W rcv to task instead do it inline in the ISR itself 
25/09/14   dg        CR729470 Schedule monitors on 7 NCELLS per PCH in case of SS mode
03/10/14   aga       CR732455 Changes required to tune away framework for TH 1.0 single sim build 
03/10/14   dp        CR733253 When DR-DSDS is enabled, allow idle G2X reconfirmation
                              searches on sub1 when sub2 is in transfer mode.
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
02/07/14   mc        CR687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
09/05/14   dv        CR655393 enable G2W TA for G2W ID search on MM sub when Gsub in xfer
02/04/14   cja       CR641428 Correct serving cell qbit lag for ccch_group when add to ba list
04/09/14   sai       CR634580: DSDA: Ensure SC wcdma ticks are triggered on idle sub when other sub is in data call.
17/12/13   ab        CR598397 DSDA:Protect G Monitors getting desence when W in connected mode.
16/12/13   dv        CR569122 High missed page rate due to W ID search
05/12/13   am        CR581771 Ensure frequency correction to re-selection cell is always applied
04/12/13   ap        CR584084 Addressing WCDMA reconfirmation search whilst GERAN in Transfer
01/12/13   sk        CR583361 Backout the CR549390
27/11/13   aga       CR549390 G2W_TA IRAT Enable W ID search and reconf in case the other G sub is in data call
05/10/13   js        CR461049 Cell Reselection Failure when 3 KHz frequency drift is applied on N cell
15/08/13   sk        CR524039 GBTA bringup changes
25/06/13   cs        Major Triton TSTS Syncup
05/03/13   ab        CR 457982-when in GTA Gap , do not allow W Reconfirm for the Idle Sub.
26/03/13   ws        CR466973 - DSDS BU, moved prototype externs to header file
13/02/13   og        CR452363. Ensure the WCDMA ID search scheduling routines are gas id specific.
11/02/13   og        CR451559. More changes to distinguish the correct gas id.
07/11/12   ss/cgc    CR404843: DSDS version check cell ptr l1_sc_balist_check_cell() is within pool address range
26/11/12   zf        CR417719: Optimization of Ncell acquisition
02/10/12   zf        CR309242: Change number of power monitor per PCH to 5 for DSDS.
                     For SS build, it is still 7 to comply with GCF requirement.
03/09/12   zf        CR341843: Opt16: Re-use SCH timing for re-acquiring Ncell.
03/09/12   zf/npt    CR339131: blacklist Ncell which fails FCB receive - Opt17
16/08/12   zf        CR390228: Featurize Opt 20.
28/06/12   zf        CR318881: Preserve serving cell BSIC.
12/01/12   cja       CR328305 Fix exception due to receiving_cell being Null
10-03-16   tjw       Renamed UNUSED to __UNUSED_ARG to avoid name clash.
16/09/09   ps        Resolve Klocwork errors
03/05/07   dv        Fix for CR118002:Surround Cell List not updated with ncell BSIC
05/25/05   sv        Fix to correctly schedule Ncell ACQ which was causing Ncell
                     BCCHs to fail.
12/30/04   bm        Fix for L1 Idle abort, in a race condition resulted in
                     SCE STOP, STOP, STRAT, STOP resulting in stuck in IDLE
10/20/04   sv        Optimization of Ncell SCE awake time
05/26/04   sv        Lint Cleanup.
05/12/04   gw        Send SURROUND_UPDATE_FAILURE_IND if BSIC changes while SU
                     is pending.  If cell enters top 6 after being out for more
                     than 30s then set bsic as unknown.
02/25/04   gw        Lint clean up.
11/05/03   gfr       Support for quad-band.
09/03/03   gw        Changed bcch_clear_mask[] so that SI3/SI4 and SI7/SI8  are
                     handled correctly.  Made next_cell_bcch() available outside
                     this module since it needs to be called from GPL1 too.
08/28/03   gw        Changes to support new result type returned by l1_sc_read_sb
                     functions.
07/14/03   gw        Removed ASSERTs.
07/14/03   gw        Added check for SCE being active in next_cell_bcch().
07/14/03   kf        added cell->pbcch == TRUE check to pbcch_done
10/07/03   kf        changed pbcch_done_idle
10/07/03   kf        added field to L1_send_MPH_SURROUND_UPDATE_FAILURE_IND
07/11/03   gfr       Lint cleanup.
07/11/03   gw        Changes to speed up reselection.
07/10/03   kf        added field to L1_send_MPH_SURROUND_UPDATE_FAILURE_IND
07/09/03   gw        Changes to conditions for sending MPH_SURROUND_FAILURE_IND.
07/08/03   gw        GRR/L1 SCE cleanup. Changes to ncell BCCH decoding.
06/23/03   gfr       Compiler warning cleanup.
06/12/03   gw        Changed number of retries on ncell acq from 1 to 0.
                     Removed some unneeded declarations.
06/03/03   gw        Added l1_sc_idle_post_sort_process() to detect ncells
                     re-entering top six and take appropriate action (e.g.
                     make ncell ready for immediate BCCH decode).
29/05/03   kf        ncell pbcch read changes
05/12/03   ATM       Moved Frame Number management entirely inside GTMRS
05/02/03   gw        Merged in power transfer changes from branch.
05/01/03   ag        Updates to ncell pbcch reading
04/28/03   gw        Changes for preserving power measure info.
04/16/03   sw        Added initial changes for WCDMA neighbour cell search
                     during packet idle on PCCCH
04/09/03   gw        Modified mechanism for adding serving cell to BA list.
04/08/03   gw        Added call to l1_sc_monscan_update() in l1_sc_enter_idle()
                     to make sure that old serving cell is picked up if it has
                     just been added to the BA list.  Also modified
                     l1_sc_calc_running_avg_length() to make sure that running
                     averages have at least 5 samples.
04/04/03   gw        Added code to preserve ncell power info when entering Idle
                     Mode.  Also added code to add serving cell to BA list when
                     leaving Idle Mode.
04/02/03   gw        Changes to support modifications in monscan module.
02/28/03   kf        Added support for ncell pbcch reading
02/17/03   ag        Update to FEATURE_GPRS_TBF_SUSPEND
02/13/03   tb        Changed call to l1_sc_wcdma_leave_idle
02/07/03   ag        Updates to FEATURE_GPRS_TBF_SUSPEND
02/03/03   ws        Merged NCELL BCCH changes from branch
01/20/03   ag        Correct problem with clearing BCCH mask introduced during
                     previous change.
01/16/03   ag        Add BCCH_MASK_4_13NORM
01/10/03   gw        Changed some SCE function calls to reflect renaming of some
                     BA list functions done as part of SCE clean up.
12/03/02   mk        General GPRS featurization cleanup.
11/27/02   ws        Update latest GPRS baseline
11/26/02   he        Removed findbcch structure.
11/25/02   mk        Updated GPRS baseline.
11/22/02   ag        General improvements to NCELL BCCH reading under switch
                     FEATURE_GPRS_TBF_SUSPEND
11/21/02   mk        Deleted unsupported code under FEATURE_GSM_ALTERNATE_PLMN_SEARCH.
11/15/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/12/02   ag        Added PL1 GPRS TBF suspension for NCELL BCCH reading code
                     hooks under switch FEATURE_GPRS_TBF_SUSPEND.
11/03/02   tb        Added support for INTERRAT_GTOW
10/08/02   kf        Added PL1 GPRS measurement code hooks under switch
                     FEATURE_GPRS_MEAS.
09/24/02   dp        Workaround for mdsp SBI errors when Ncell SCH offset lies
                     near FTSM.  Problem is due to large ZIF tune time.  Will
                     be pulled out when ZIF tune time is fixed
09/06/02   plm       Featurized forward function and variable declarations to
                     eliminate compiler warnings.
08/15/02   dp        Added support for SI7 | SI8
08/14/02   gw        Removed negation of timing error in sb_done_xxx functions.
08/12/02   gw        Featurized code for alternate PLMN searching.  Modified
                     ncell BCCH decode functions to return failure indication
                     to RR if cell is not valid or decode fails all retries.
07/29/02   dp        Put back SCH and BCCH decode rates to 30 secs and 5 mins
                     as specified in the specs
07/02/02   gw        Removed function calc_pch_per_period as no longer needed.
                     Modified function l1_sc_set_paging_multiframes.
06/27/02   dp        Changed bcch_mask_clear to decode (SI7|SI8) but not
                     both
06/27/02   gw        Changes to support running averages of ncell power
                     measurements in Idle Mode.
05/21/02   gw        Updated pscan to use latest message layer functions.
03/22/02   JC        Added support for new NCELL RXLEV reporting, also added
                     QCT function headers w/ comments on select fcns. Replaced
                     GSM_DEBUG w/ MSG_ macros for ATM.
02/07/02   gw        Improved starting and stopping of SCE.
12/21/01   gw        Changes to support DRX scheduling.
12/18/01   dp        Made changes to handle SC aborts when switching modes
10/16/01   gw        Mainlined FEATURE_FINDBCCH and OLD_SCE.  Made changes to
                     support message layer interface for monitoring and pscan.
                     Changed CCWW types to QCT types.
10/10/01   gw        Added support for message layer Ncell reception.
                     Mainlined feature MON_TMP.
09/25/01   mk/gw     General SCE cleanup. Mainlined MONITOR_DEBUG, added
                     PScan/Ncell support using FINDBCCH and MULTIMON.
09/13/01   JC        Cleanup of included header files.
08/20/01   mk/gw     Added Ncell support, featurized via MONITOR_DEBUG.
08/20/01   mk        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "gl1_sys_params.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1_isr.h"
#include "l1_sc.h"
#include "l1i.h"
#include "l1_utils.h"
#include "gpl1_gprs_isr.h"
#include "msg.h"
#include "l1_task.h"
#include "l1_drx.h"

#include "geran_dual_sim.h"
#include "gpl1_dual_sim.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
#ifdef FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17
#define MAX_TRIES_FCB   2
#else
#define MAX_TRIES_FCB   1
#endif
#define MAX_TRIES_SB    2
#define MAX_TRIES_BCCH  1

/* For acquisition top six are split into two groups
 * Top cells    - rank < TOP_CELLS_THRESH
 * Bottom cells - rank >= TOP_CELLS_THRESH
 *
 * A tick is 51 frames
 */
#define TOP_CELLS_THRESH                3

#define MIN_NUM_CELLS                   6
#define MAX_BLACKLISTED_CELLS           3
#define TICKS_PER_FCB_BLACKLIST_CELLS   510  /*  2 min */

#define TICKS_PER_FCB_TOP_CELLS         42   /* 10 sec */
#define TICKS_PER_FCB_BOTTOM_CELLS      127  /* 30 sec */
#define TICKS_PER_SB                    120  /* 28 sec (keep < 30) */
#define SB_PER_BCCH                     10   /*  5 min */
#define TICKS_PER_FCB_DIFF              (TICKS_PER_FCB_BOTTOM_CELLS - TICKS_PER_FCB_TOP_CELLS)

#ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS
/* With BS_PA_MFRMS of 4, we need 31.02s to do power measurements 5 times for 32 cells.
 * With BS_PA_MFRMS of 5, we need 38.77s to do power measurements 5 times for 32 cells.
 * So, we are proposing a value of 31.02s to do power measurements (details below).
 *
 * We can do 5 power monitors with the PCH. So, to do power monitors for 32 Ncells, 5 times, 
 * we need to monitor 32x5 => 160 monitors. On each power measure command, since we can do
 * 5 monitors, so it means we need to do 32 commands (160/5 => 32).
 *
 * Now, with BS_PA_MFRMS of 4, as PCH (and power monitor) is every 4 ticks (1 tick is 51 frames).
 * So, to run 32 commands, we need (4x32 + 4) ticks to complete the measurement.
 * (Extra 4 is added as we may start measurement just when PCH finished).
 * This gives us 132 ticks to do 5 round of measurements for 32 NCells (31.02s).
 *
 * Now, with BS_PA_MFRMS of 5, as PCH (and power monitor) is every 5 ticks (1 tick is 51 frames).
 * So, to run 32 commands, we need (5x32 + 5) ticks to complete the measurement.
 * (Extra 5 is added as we may start measurement just when PCH finished).
 * This gives us 165 ticks to do 5 round of measurements for 32 NCells (38.775s).
 */
#define TICKS_REQD_FOR_PWR_MSR       132 /* 132 ticks is 132x235ms => 31.02s */
#endif /* #ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS */

/*
 * The number of frames to delay any TRM release for a SUB that
 * has a pending idle surround cell activity message posted
 * to avoid an unnecessary release of firmware and the lock
 * as the SC activity action is delayed
 */
#define PWR_MEAS_IDLE_TRM_SUSPENSION_FRAME_DELAY 2

typedef struct
{

struct
{
   uint16 paging_multiframes;
} idle;
} l1_sc_idle_data_t;


/*
static const uint16 bcch_mask_clear[ 16 ] =
{
   ( uint16 )~0x0001, ( uint16 )~0x0002, ( uint16 )~0x0044, ( uint16 )~0x0088, ( uint16 )~0x0010, ( uint16 )~0x0020, ( uint16 )~0x0044, ( uint16 )~0x0088,
   ( uint16 )~0x0000, ( uint16 )~0x0000, ( uint16 )~0x0000, ( uint16 )~0x0800, ( uint16 )~0x0000, ( uint16 )~0x0000, ( uint16 )~0x0000, ( uint16 )~0x8000
};
*/

const uint16 bcch_mask_clear[ 16 ] =
{
   0x0001, 0x0002, 0x00cc, 0x00cc, 0x0010, 0x0020, 0x00cc, 0x00cc,
   0x0100, 0x0200, 0x0400, 0x8800, 0x1000, 0x2000, 0x4000, 0x8800
};

static void wcdma_done_idle( void* unused, gas_id_t gas_id );
static void fcb_done_idle( cell_T* cell, gas_id_t gas_id );
static void sb_done_idle( cell_T* cell, gas_id_t gas_id );
static void bcch_done_idle( cell_T* cell, gas_id_t gas_id );
static void paging_block( void* unused, gas_id_t gas_id );
       void next_cell_bcch( gas_id_t gas_id );
static uint8 l1_sc_calc_running_avg_length( uint8 cell_count, uint8 paging_multiframes );
static void receive_failed_fcb( cell_T* cell, gas_id_t gas_id );
static void l1_sc_idle_updt_running_avg_len( gas_id_t gas_id );

static l1_sc_idle_data_t  l1_sc_idle_data[NUM_GERAN_DATA_SPACES];

/*===========================================================================

                            IMPLEMENTATION

===========================================================================*/

void l1_sc_enter_idle( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();

   l1_sc_globals_ptr->max_bsic_age = 216*60; /* ca 60 secs */
   l1_sc_globals_ptr->sorted_cell_max_count = SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL;

   /* Update ra_len for the BA list, and store paging_multiframes for surround_tick increment */
   l1_sc_idle_updt_running_avg_len(gas_id);

   /* Preserve power measurements for cells in old BA list. */
   l1_sc_balist_transfer_power_info(gas_id);
   l1_sc_bcch_reset_high_priority_flag(gas_id); 

   /* Update monscan since BA list may have changed. */
   l1_sc_monscan_update(gas_id);

   /* Reset BSIC info. */
   l1_sc_balist_reset_counters(gas_id);

   /* Sort the BA list so that we can send
    * measurements reports with cells in
    * them.
    */
   l1_sc_balist_sort_restrict_ba_len( FALSE, gas_id);

   l1_sc_wcdma_enter_idle(gas_id);
#if defined(FEATURE_GPRS_GBTA) || defined (FEATURE_G2X_TUNEAWAY)
   SET_MISSED_PAGE_STATUS(FALSE,gas_id);
#endif
}

void l1_sc_leave_idle( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   ASSERT_TASK();
   
   l1_sc_bcch_reset_high_priority_flag(gas_id);   

   if ( l1_sc_globals_ptr->monscan_in_progress )
   {
      l1_sc_monscan_abort(gas_id);
   }

   if (!l1_sc_wcdma_leave_idle(gas_id))
   {
      if (( l1_sc_globals_ptr->receive_in_progress )&&(!l1_sc_globals_ptr->wcdma_srch_in_progress) )
      {
         l1_sc_abort_receive(gas_id);
      }
   }


   if (!l1_sc_globals_ptr->autonomous_bcch)
   {
      gpl1_gprs_update_bcch_stop( TRUE, gas_id );
   }

   /* Save the serving cell info in case
    * we are re-selecting and it is in
    * the new BA list.
    */
   l1_sc_idle_add_serving_cell_to_balist(gas_id);
#if defined(FEATURE_GPRS_GBTA) || defined (FEATURE_G2X_TUNEAWAY)
   SET_MISSED_PAGE_STATUS(FALSE,gas_id);
#endif
}

void l1_sc_set_paging_multiframes( uint16 paging_multiframes, gas_id_t gas_id )
{
   l1_sc_idle_data_t *l1_sc_idle_data_ptr = &l1_sc_idle_data[gas_id];

   uint8 ra_len;

   ASSERT_TASK();

   l1_sc_idle_data_ptr->idle.paging_multiframes = paging_multiframes;
   ra_len = l1_sc_calc_running_avg_length( l1_sc_balist_get_num_cells( gas_id),
                                           (uint8)l1_sc_idle_data_ptr->idle.paging_multiframes );
   l1_sc_balist_set_ra_length( ra_len, gas_id );
   l1_sc_monscan_set_req_passes( ra_len, gas_id );
   l1_sc_wcdma_set_paging_multiframes(paging_multiframes,gas_id);
}

/*===========================================================================

FUNCTION L1_SC_CALC_RUNNING_AVG_LENGTH

DESCRIPTION
  Works out how many measurements should be included in the running average
  for each neighbor cell.  This will usually be five except when BS_PA_MFRMS
  is small and the BA list is short.

  See GSM 05.05 6.6.1.

DEPENDENCIES
  cell_count         - the length of the BA list.
  paging_multiframes - BS_PA_MFRMS.

RETURN VALUE
  Number of measurements to include in each ncell running average.

SIDE EFFECTS
  None
===========================================================================*/
static uint8 l1_sc_calc_running_avg_length( uint8 cell_count, uint8 paging_multiframes )
{
   /* number of paging blocks in about 5 seconds */
   static const uint8 pch_per_5_sec[] = { 0, 0, 11, 7, 5, 4, 3, 3, 3, 2 };

   uint8 ra_len;
   uint8 pch_per_period;

   if ( (cell_count > 0) && (paging_multiframes <= 9) )
   {
      pch_per_period = (uint8)(( 5u * cell_count + 6 ) / 7);
      if ( pch_per_period < pch_per_5_sec[ paging_multiframes ] )
      {
         pch_per_period = pch_per_5_sec[ paging_multiframes ];
      }

      if (cell_count > 7)
      {
      /* Running average length =
       *    (total monitors per period) / (number of cells).
       */
         ra_len = (uint8)((pch_per_period * 7u) / cell_count);
      }
      else
      {
         /* Because we only ever do one measurement per
          * cell per paging block we are going to violate
          * the spec for cell_count <= 5 and
          * paging_multiframes <= 5.  Make sure we take
          * too long rather than having too few measurements.
          */
         if (pch_per_period < 5)
         {
            ra_len = 5;
         }
         else
         {
            ra_len = pch_per_period;
         }
      }

   }
   else
   {
      ra_len = 0;
   }

   return ra_len;
}

void next_cell_bcch( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint16 n;
   ASSERT_TASK();

   if (l1_sc_globals_ptr->active)
   {
      /* Do FCCH/SCH acquisition only for top-2 or top-6 cells */
      for ( n = 0; n < l1_sc_globals_ptr->sorted_cell_count && n < l1_sc_globals_ptr->sorted_cell_max_count && !l1_sc_globals_ptr->receive_in_progress; ++n )
      {
         cell_T* cell = l1_sc_globals_ptr->sorted_cells[ n ];
         if ( cell->in_new_ba_list ) /* no receives on cells to be removed from BA list */
         {
            if ( (cell->next_tick <= l1_sc_globals_ptr->surround_tick) && (cell->bs.rx_power >= SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16) )
            {
               /* time to do "FCCH+SCH combined acquisition" or "SCH reconf" */

#ifdef FEATURE_GSM_POWER_OPTION16
               if ( ( !cell->bs.bsic_known ) || ( cell->bs.bsic_known && cell->fcb_try_no > 0 ) )
#else /* FEATURE_GSM_POWER_OPTION16 */
               if ( !cell->bs.bsic_known )
#endif /* FEATURE_GSM_POWER_OPTION16 */
               {
                  /* Do FCCH+SCH combined acquisition */

#ifdef FEATURE_GSM_POWER_OPTION16
                  if ( cell->bs.bsic_known )
                  {
                      MSG_GERAN_MED_2_G("Starting Ncell FCB receive on ARFCN %d with known bsic retry. Rank %d", cell->bs.arfcn.num, cell->rank);
                  }
                  else
#endif /* FEATURE_GSM_POWER_OPTION16 */
                  {
                      MSG_GERAN_MED_1_G("Starting Ncell FCB receive on ARFCN %d", cell->bs.arfcn.num);
                  }

                  l1_sc_receive_fcb( cell, fcb_done_idle, gas_id );
               }
               else
               {
                  /* Do SCH reconf */

                  cell->try_no = 0;
                  MSG_GERAN_MED_1_G("Starting Ncell SB receive on ARFCN %d", cell->bs.arfcn.num);
                  l1_sc_receive_sb( cell, sb_done_idle, gas_id );
               }
            }
            else if ( cell->sb_count_to_bcch == BCCH_DO_NOW && cell->bs.bsic_known )
            {
               /* Do BCCH read */

               if( cell->bcch_mask == 0 )
               {
                  cell->sb_count_to_bcch = BCCH_DONT_DO;
                  MSG_GERAN_ERROR_1_G("Ncell %d bcch_mask=0", cell->bs.arfcn.num);
               }
               else
               {
                  {
                     cell->try_no = 0;
                     MSG_GERAN_MED_1_G("Starting Ncell BCCH receive on ARFCN %d", cell->bs.arfcn.num);
                     l1_sc_receive_bcch( cell, bcch_done_idle, gas_id );
                  }

                  /* If we get here from the task context, then we require the next frame
                     where the ISR will deduce the number of frames to Ncell (P)BCCH offset
                  */
				  if ((!L1_IN_IRQ_MODE()) && (!mdsp_asynch_dsp_int_enabled(gas_id)))
                  {
                      gl1_drx_require_next_tick(gas_id);
                  }

               }
            }
         }  /* if in new balist */
      } /* for */

 
#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
      /* Do BCCH read for remaining 3-6 ncells (if requested by RR) */
      for ( /* n = 0 */; n < (l1_sc_globals_ptr->sorted_cell_count_rpt_to_rr) && (n < SCE_MAX_SORTED_CELLS_IDLE_MODE_WEAK_SCELL) && (!l1_sc_globals_ptr->receive_in_progress); ++n )
      {
         cell_T* cell = l1_sc_globals_ptr->sorted_cells[ n ];
         if ( cell->in_new_ba_list ) /* no receives on cells to be removed from BA list */
         {           
            if ( cell->sb_count_to_bcch == BCCH_DO_NOW && cell->bs.bsic_known )
            {
               /* Do BCCH read */

               if( cell->bcch_mask == 0 )
               {
                  cell->sb_count_to_bcch = BCCH_DONT_DO;
                  MSG_GERAN_ERROR_1_G("Ncell %d bcch_mask=0", cell->bs.arfcn.num);
               }
               else
               {
                  {
                     cell->try_no = 0;
                     MSG_GERAN_MED_1_G("Starting Ncell BCCH receive on ARFCN %d", cell->bs.arfcn.num);
                     l1_sc_receive_bcch( cell, bcch_done_idle, gas_id );
                  }

                  /* If we get here from the task context, then we require the next frame
                     where the ISR will deduce the number of frames to Ncell (P)BCCH offset
                  */
                  if ((!L1_IN_IRQ_MODE()) && (!mdsp_asynch_dsp_int_enabled(gas_id)))
                  {
                      gl1_drx_require_next_tick(gas_id);
                  }
               }
            }
         }  /* if in new balist */
      } /* for */
#endif /* #ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST */

   } /* if active */
}

/* This function is a blend of the original */
/* fcb_done_idle and sb_done_idle.          */
static void fcb_done_idle( cell_T* cell, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    uint8      sb_data[4];
    uint32     start_fn;
    int16     timing_error;

    ASSERT_TASK();


   if ( l1_sc_read_fcbsb( sb_data, &start_fn, &timing_error, gas_id ) )
   {
      uint8  bsic;
      uint32 bs_fn;

      bs_fn = l1_sc_decode_sb( sb_data, &bsic );

      if ( !cell->bs.bsic_known )
      {
         /* new bsic (bsic decoded for first time) */
         cell->bs.bsic_known = TRUE;
         cell->bs.bsic       = bsic;
         cell->next_tick     = l1_sc_globals_ptr->surround_tick + TICKS_PER_SB;
            /* If GRR is controlling BCCH/PBCCH decodes then don't
             * schedule them from L1
             */
            if (!l1_sc_globals_ptr->autonomous_bcch)
            {
               cell->sb_count_to_bcch = BCCH_DONT_DO;
            }
            else
            {
               cell->sb_count_to_bcch = BCCH_DO_AFTER_MEAS_RPT; /* don't receive BCCh sys infos unless RR knows this cell */
            }
      }
      else if ( cell->bs.bsic != bsic )
      {
         /* different bsic than stored */
         cell->bs.bsic   = bsic;
         cell->next_tick = l1_sc_globals_ptr->surround_tick + TICKS_PER_SB;
            /* If GRR is controlling BCCH/PBCCH decodes then don't
             * schedule them from L1
             */
            if (!l1_sc_globals_ptr->autonomous_bcch)
            {
               cell->sb_count_to_bcch = BCCH_DONT_DO;
            }
            else
            {
               cell->sb_count_to_bcch = BCCH_DO_AFTER_MEAS_RPT; /* don't receive BCCh sys infos unless RR knows this cell */
            }
      }
      else if ( cell->sb_count_to_bcch > 0 )
      {
         /* same bsic, and need to wait before reading BCCH */
         --cell->sb_count_to_bcch;
         cell->next_tick += TICKS_PER_SB;
      }

      cell->bs.sb_data[ 0 ] = sb_data[ 0 ];
      cell->bs.sb_data[ 1 ] = sb_data[ 1 ];
      cell->bs.sb_data[ 2 ] = sb_data[ 2 ];
      cell->bs.sb_data[ 3 ] = sb_data[ 3 ];
      cell->bs.fn_lag       = SUB_FN( start_fn, bs_fn );
      cell->bs.qbit_lag     = 0;
      l1_sc_adjust_lag( &cell->bs, timing_error );

      cell->last_bsic_fn = start_fn;

      cell->fcb_try_no   = 0;
      cell->bs.fn_when_read = gl1_get_FN(gas_id);
      cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
      MSG_GERAN_MED_3_G("Success FCB receive on ARFCN %d. Rank %d GAS %d", cell->bs.arfcn.num, cell->rank, gas_id);

      next_cell_bcch(gas_id);
   }
   else if ( cell->fcb_try_no < MAX_TRIES_FCB-1 )
   {
      ++cell->fcb_try_no;
      receive_failed_fcb( cell, gas_id );
   }
   else
   {
#ifdef FEATURE_GSM_POWER_OPTION16
       cell->bs.bsic_known = FALSE;
#endif /* FEATURE_GSM_POWER_OPTION16 */
#ifdef FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17
       if ( ( l1sc_get_ba_cell_count(gas_id) < MIN_NUM_CELLS ) ||
            ( l1sc_get_ba_blacklisted_cell_count(gas_id) >= MAX_BLACKLISTED_CELLS ) )
#endif /* FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17 */
       {
           receive_failed_fcb( cell, gas_id );
       }
#ifdef FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17
       else
       {
           /* Blacklist the failing Ncell for 2 mins */
           cell->next_tick    = l1_sc_globals_ptr->surround_tick + TICKS_PER_FCB_BLACKLIST_CELLS;
#ifdef FEATURE_GSM_SCE_DISABLE_PWR_MSR_ON_BLACKLIST_NCELLS
           if(TICKS_PER_FCB_BLACKLIST_CELLS > TICKS_REQD_FOR_PWR_MSR)
           {
             /* disabled only when dur blacklisting is more than pwr msr duration */
             cell->pwr_msr_disabled_till_tick = cell->next_tick - TICKS_REQD_FOR_PWR_MSR;
           }
           else
           {
             cell->pwr_msr_disabled_till_tick = l1_sc_globals_ptr->surround_tick;
           }
#endif
           cell->in_blacklist = TRUE;
           cell->fcb_try_no   = 0;
           MSG_GERAN_MED_3_G("Failed FCB receive on ARFCN %d in blacklist. Rank %d GAS %d",
                   cell->bs.arfcn.num, cell->rank, gas_id);
           MSG_GERAN_MED_5_G("SCE: Blacklisting: FN: %d, s-tick: %d, BL-end: %d, PwrDis: %d, arfcn: %d",
            GSTMR_GET_FN_GERAN(gas_id),
            l1_sc_globals_ptr->surround_tick,
            cell->next_tick,
            cell->pwr_msr_disabled_till_tick,
            cell->bs.arfcn.num);

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
           /* Remove blacklisted Ncell from sorted BA list immediately
            * (Do not include serving cell in idle mode)
            */
           l1_sc_update_sorted_ba_list(FALSE, gas_id);
#endif

           next_cell_bcch(gas_id);
       }
#endif /* FEATURE_GSM_NCELL_LOWER_ACQ_FREQ_OPTION17 */
   }
}

static void receive_failed_fcb( cell_T* cell, gas_id_t gas_id )
{
    l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
    if (cell->rank < TOP_CELLS_THRESH)
    {
        cell->next_tick = l1_sc_globals_ptr->surround_tick + TICKS_PER_FCB_TOP_CELLS;
    }
    else
    {
        cell->next_tick = l1_sc_globals_ptr->surround_tick + TICKS_PER_FCB_BOTTOM_CELLS;
    }
    MSG_GERAN_MED_3_G("Failed Ncell FCB receive on ARFCN %d Rank %d. Starting FCB ACQ again. GAS %d",
            cell->bs.arfcn.num, cell->rank, gas_id);
    next_cell_bcch(gas_id);
}


static void sb_done_idle( cell_T* cell, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint8              sb_data[4];
   uint32             start_fn;
   int16              timing_error;
   l1_sc_rcv_result_T result;

   ASSERT_TASK();

   result = l1_sc_read_sb( sb_data, &start_fn, &timing_error, gas_id );

   if ( result == L1_SC_DECODE_SUCCESS )
   {
      uint8  bsic;
      uint32 bs_fn;

      bs_fn = l1_sc_decode_sb( sb_data, &bsic );
      if ( !cell->bs.bsic_known )
      {
         /* new bsic (bsic decoded for first time) */
         cell->bs.bsic_known = TRUE;
         cell->bs.bsic = bsic;
         /* If GRR is controlling BCCH/PBCCH decodes then don't
          * schedule them from L1
          */
         if (!l1_sc_globals_ptr->autonomous_bcch)
         {
            cell->sb_count_to_bcch = BCCH_DONT_DO;
         }
         else
         {
            cell->sb_count_to_bcch = BCCH_DO_AFTER_MEAS_RPT; /* don't receive BCCh sys infos unless RR knows this cell */
         }
      }
      else if ( cell->bs.bsic != bsic )
      {
         /* different bsic */
         cell->bs.bsic = bsic;

         /* If GRR is controlling BCCH/PBCCH decodes then don't
          * schedule them from L1
          */
         if (!l1_sc_globals_ptr->autonomous_bcch)
         {
            if (cell->sb_count_to_bcch == BCCH_DO_NOW)
            {
               L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, FALSE, gas_id );
               MSG_GERAN_MED_1_G("Ncell %d BSIC changed. SU cancelled.", cell->bs.arfcn.num);
            }
            else
            {
               MSG_GERAN_MED_1_G("Ncell %d BSIC changed.", cell->bs.arfcn.num);
            }
            cell->sb_count_to_bcch = BCCH_DONT_DO;
         }
         else
         {
            cell->sb_count_to_bcch = BCCH_DO_AFTER_MEAS_RPT; /* don't receive BCCh sys infos unless RR knows this cell */
            MSG_GERAN_MED_1_G("Ncell %d BSIC changed.", cell->bs.arfcn.num);
         }
      }
      else if ( cell->sb_count_to_bcch > 0 )
      {
         /* same bsic */
         --cell->sb_count_to_bcch;
      }

      cell->bs.sb_data[ 0 ] = sb_data[ 0 ];
      cell->bs.sb_data[ 1 ] = sb_data[ 1 ];
      cell->bs.sb_data[ 2 ] = sb_data[ 2 ];
      cell->bs.sb_data[ 3 ] = sb_data[ 3 ];

      /* -1 because we start rx one frame before it happens. */
#ifdef ZIF_TUNE_TIME_FIXED
      #error code not present
#else
      /*
         ZIF_WORKAROUND
         Account for the fact that we wake up 2 frames before the
         SCH burst.  Workaround for mdsp SBI errors due to ZIF tune
         time.  Will be pulled out after ZIF tune time is fixed
      */
      cell->bs.fn_lag = SUB_FN( start_fn, SUB_FN(bs_fn,2) );
#endif

      l1_sc_adjust_lag( &cell->bs, timing_error );

      cell->last_bsic_fn = start_fn;
      cell->next_tick += TICKS_PER_SB;
      /* In some cases the SB receive may have been delayed by a long time.      */
      /* In such cases we need to increment next_tick by more than TICKS_PER_SB. */
      if ( cell->next_tick <= l1_sc_globals_ptr->surround_tick )
      {
         cell->next_tick = l1_sc_globals_ptr->surround_tick + TICKS_PER_SB;
      }

      cell->bs.fn_when_read = gl1_get_FN(gas_id);
      cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);

#ifdef DEBUG_SCE_FCB_MSG
      MSG_GERAN_MED_2_G("Success SB receive on ARFCN %d. Rank %d", cell->bs.arfcn.num, cell->rank);
#endif

      next_cell_bcch(gas_id);
   }
   else if ( (result == L1_SC_BLOCKED) && (cell->bs.bsic_known) )
   {
      /* Pretend that the SB was received OK, but don't
       * update any info. since we didn't get anything new.
       */
      if ( cell->sb_count_to_bcch > 0 )
      {
         --cell->sb_count_to_bcch;
      }

      cell->next_tick += TICKS_PER_SB;
      /* In some cases the SB receive may have been delayed by a long time.      */
      /* In such cases we need to increment next_tick by more than TICKS_PER_SB. */
      if ( cell->next_tick <= l1_sc_globals_ptr->surround_tick )
      {
         cell->next_tick = l1_sc_globals_ptr->surround_tick + TICKS_PER_SB;
      }
#ifdef DEBUG_SCE_FCB_MSG
      MSG_GERAN_MED_2_G("Blocked SB receive on ARFCN %d. Rank %d", cell->bs.arfcn.num, cell->rank);
#endif

      next_cell_bcch(gas_id);
   }
   else if ( cell->try_no < MAX_TRIES_SB-1 )
   {
#ifdef FEATURE_GPRS_GBTA
     if (result == L1_SC_SCHED_FAIL )
     {
       /* should not increment retry count as SCH not attempted */
       MSG_GERAN_HIGH_0_G("GL1 GBTA sb_done_idle L1_SC_SCHED_FAIL YIELD");
     }
     else
     {
       ++cell->try_no;
     }
#else /* FEATURE_GPRS_GBTA*/
     ++cell->try_no;
#endif

     if ( cell->bs.bsic_known )
     {
        MSG_GERAN_MED_2_G("Starting Ncell SB receive on ARFCN %d retry. GAS %d", cell->bs.arfcn.num, gas_id);
        l1_sc_receive_sb( cell, sb_done_idle, gas_id );
     }
     else
     {
        MSG_GERAN_MED_2_G("Starting Ncell FCB receive on ARFCN %d failed SB receive. GAS %d", cell->bs.arfcn.num, gas_id);
        cell->fcb_try_no = 0;
        l1_sc_receive_fcb( cell, fcb_done_idle, gas_id );
     }
   }
   else if ( cell->bs.bsic_known )
   {
#ifndef FEATURE_GSM_POWER_OPTION16
      cell->bs.bsic_known = FALSE;
#endif /* FEATURE_GSM_POWER_OPTION16 */
      cell->try_no = 0;
      cell->fcb_try_no = 0;

      /* If there is a pending BCCH read then
       * tell GRR we're abandoning it.
       */
      if (cell->sb_count_to_bcch == BCCH_DO_NOW)
      {
         L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, FALSE, gas_id );
         cell->sb_count_to_bcch = BCCH_DONT_DO;
         MSG_GERAN_MED_1_G("Abandoning SU on %d.", cell->bs.arfcn.num);
      }

      MSG_GERAN_MED_2_G("Starting Ncell FCB receive on ARFCN %d with known bsic. GAS %d", cell->bs.arfcn.num, gas_id);
      l1_sc_receive_fcb( cell, fcb_done_idle, gas_id );
   }
   else
   {
      receive_failed_fcb( cell, gas_id );
   }
}

static void bcch_done_idle( cell_T* cell, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint8  *bcch_data;
   dBx16_T rx_power;
   int16   timing_error;
   uint8   tc;
   uint16  temp_bcch_mask;
   uint8   num_bursts;

   ASSERT_TASK();

   /* Don't process cell if its not set to DO_NOW. This prevents us sending
    * INDs to (G)RR after they have sent STOP.
    */
   if (cell->sb_count_to_bcch == BCCH_DO_NOW)
   {
      if ( l1_sc_read_bcch( &bcch_data, &rx_power, &timing_error, &tc, &num_bursts, gas_id ) )
      {
         l1_sc_adjust_lag( &cell->bs, -timing_error );

         L1_send_sysinfo_PH_DATA_IND(TRUE,BCCH,FALSE,cell->bs.arfcn,tc,bcch_data,23, gas_id);
         
         /* Bounds check to resolve Klocwork ABR errors */
         if (tc >= (sizeof(bcch_mask_clear)/sizeof(bcch_mask_clear[0])))
         {
           MSG_GERAN_ERROR_1_G( "Too many cells %d clamp to max ", tc);
           tc = (sizeof(bcch_mask_clear)/sizeof(bcch_mask_clear[0])) - 1;
         }

         /* if L1 is allowed to, clear the SIs for this TC value out of the mask */
         cell->bcch_mask &= ~(bcch_mask_clear[ tc ] & cell->bcch_clear_mask);

         MSG_GERAN_LOW_1_G("CALLBCK_CLEAR_MASK %d",cell->bcch_mask);

         if ( cell->bcch_mask == 0 )
         {
            if ( !l1_sc_globals_ptr->autonomous_bcch )
            {
               cell->sb_count_to_bcch = BCCH_DONT_DO;
            }
            else
            {
               /* receive new BCCh sys infos 3 and 4 in 5 minutes */
               cell->sb_count_to_bcch = SB_PER_BCCH;
            }

            cell->bcch_mask        = BCCH_MASK_3;
            cell->bcch_clear_mask |= BCCH_MASK_34;
            next_cell_bcch(gas_id);
         }
         else
         {
            cell->try_no = 0;
            MSG_GERAN_MED_1_G("Starting Ncell BCCH receive on ARFCN %d", cell->bs.arfcn.num);
            l1_sc_receive_bcch( cell, bcch_done_idle, gas_id );
         }

         cell->bs.fn_when_read  = gl1_get_FN(gas_id);
         cell->bs.scell_freq_err = gl1_hw_get_xo_acc_freq_err(gas_id);
      }
      else
      {
         /* Bounds check to resolve Klocwork ABR errors */
         if (tc >= (sizeof(bcch_mask_clear)/sizeof(bcch_mask_clear[0])))
         {
           MSG_GERAN_ERROR_1_G( "Too many cells %d clamp to max ", tc);
           tc = (sizeof(bcch_mask_clear)/sizeof(bcch_mask_clear[0])) - 1;
         }

#ifdef FEATURE_GPRS_GBTA_DEBUG
         MSG_GERAN_MED_3_G("bcch_done_idle FAIL bcch_mask %d mask_clear tc %d clear_mask %d",
                             cell->bcch_mask,bcch_mask_clear[ tc ],cell->bcch_clear_mask);
#endif /* FEATURE_GPRS_GBTA_DEBUG */

         /* See if this BCCH block would have cleared the mask.
          * If the mask would have been cleared then stop the receive
          * and send a failure indication.
          */
         temp_bcch_mask = cell->bcch_mask & ~(bcch_mask_clear[ tc ] & cell->bcch_clear_mask);

         if (temp_bcch_mask == 0)
         {
#if MAX_TRIES_BCCH > 1
            if ( cell->try_no < MAX_TRIES_BCCH-1 )
            {
#ifdef FEATURE_GPRS_GBTA
      // to do code. Not to increment the cell->try_no if unable to schedule.
      //but I guess this code is featured out hence not needed.
#endif /* FEATURE_GPRS_GBTA*/
               ++cell->try_no;
               MSG_GERAN_MED_1_G("Starting Ncell BCCH receive on ARFCN %d", cell->bs.arfcn.num);
               l1_sc_receive_bcch( cell, bcch_done_idle, gas_id );
            }
            else
#endif
            {
               if ( !l1_sc_globals_ptr->autonomous_bcch )
               {
                  cell->sb_count_to_bcch = BCCH_DONT_DO;
               }
               else
               {
                  /* retry BCCh after next SB */
                  cell->sb_count_to_bcch = 1;
               }

               /* Always reset to SI3 */
               cell->bcch_mask        = BCCH_MASK_3;
               cell->bcch_clear_mask |= BCCH_MASK_34;

               /* tell RR we didn't get BCCH for this cell */
               L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, FALSE, gas_id);

               next_cell_bcch(gas_id);
            }
         }
      }
   }
   else
   {
      cell->bcch_mask        = BCCH_MASK_3;
      cell->bcch_clear_mask |= BCCH_MASK_34;
      next_cell_bcch(gas_id);
   }
}

extern boolean l1_sc_balist_check_cell( gas_id_t gas_id, cell_T*  cell_ptr);


void l1_sc_read_ext_bcch( ARFCN_T arfcn, gas_id_t gas_id )
{
   cell_T* cell = l1_sc_balist_get_cell_by_arfcn( arfcn, gas_id );

   ASSERT_TASK();

   if ( (cell == NULL) || !(cell->bs.bsic_known) )
   {
      MSG_GERAN_ERROR_1_G("Cannot read EXT BCCH for cell %d",arfcn.num);
      L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( arfcn, FALSE, gas_id );
   }
   else
   {
      l1_sc_balist_check_cell( gas_id, cell);

      cell->bcch_mask       |= BCCH_MASK_EXT;
      cell->bcch_clear_mask |= BCCH_MASK_EXT;
      /* get BCCh asap */
      cell->sb_count_to_bcch = BCCH_DO_NOW;
   }
}

void l1_sc_set_ba_list_idle( inter_task_BCCH_allocation_T* ba_list, uint8 ncc_permitted, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_idle_data_t *l1_sc_idle_data_ptr = &l1_sc_idle_data[gas_id];

   uint8 ra_len;

   ASSERT_TASK();

#ifdef FEATURE_GSM_SCE_REDUCED_NCELL_LIST
   if(ba_list->no_of_entries >= SCE_REDUCED_NCELL_LIST_BALIST_MIN_SZ)
   {
      /* ba list >= 4, so use optimised top 2 mode */
      l1_sc_set_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_BALIST_SZ_EN, gas_id);
   }
   else
   {
      /* ba list small, so use normal top 6 mode */
      l1_sc_clear_bmask_reduced_ncell_list(SCE_REDUCED_NCELL_LIST_BALIST_SZ_EN, gas_id);
   }
#endif

   if ( l1_sc_balist_prepare_new( ba_list, ncc_permitted, gas_id ) )
   {
      if ( l1_sc_globals_ptr->receive_in_progress )
      {
         if (l1_sc_globals_ptr->receiving_cell )
         {
           if (!l1_sc_globals_ptr->receiving_cell->in_new_ba_list )
           {
              l1_sc_abort_receive(gas_id);
           }
         }
         else
         {
            l1_sc_abort_receive(gas_id);
         }
      }
      ra_len = l1_sc_calc_running_avg_length( l1_sc_balist_get_num_cells( gas_id),
                                              (uint8)l1_sc_idle_data_ptr->idle.paging_multiframes );
      l1_sc_monscan_update(gas_id);
      l1_sc_monscan_set_req_passes( ra_len, gas_id );
      l1_sc_balist_set_ra_length( ra_len, gas_id );
   }
}

void l1_sc_report_period_done_idle( gas_id_t gas_id )
{
   ASSERT_TASK();
   paging_block( NULL, gas_id );
}

static void paging_block( void* unused, gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   l1_sc_idle_data_t *l1_sc_idle_data_ptr = &l1_sc_idle_data[gas_id];

   NOTUSED( unused );


   l1_sc_wcdma_receive(wcdma_done_idle, gas_id);


   if ( !l1_sc_globals_ptr->receive_in_progress )
   {
      MSG_GERAN_MED_3_G("Paging block: next_cell_bcch: FN: %d, s-tick: %d, mfrms: %d",
        GSTMR_GET_FN_GERAN(gas_id),
        l1_sc_globals_ptr->surround_tick,
        l1_sc_idle_data_ptr->idle.paging_multiframes);

      next_cell_bcch(gas_id);
   }

}


/* wcdma_done_idle         */
static void wcdma_done_idle( void* unused, gas_id_t gas_id )
{
   /* called in task context */
   NOTUSED( unused );

   MSG_GERAN_LOW_0_G("wcdma_done_idle");

   next_cell_bcch(gas_id);
}

void l1_wcdma_schedule_idle( void* unused, gas_id_t gas_id )
{
   void ( *fcptr )( void*, gas_id_t gas_id ) = NULL;

   /* called in task context */
   NOTUSED( unused );

   MSG_GERAN_LOW_0_G("l1_wcdma_schedule_idle");

   l1_sc_wcdma_receive(fcptr, gas_id);

}



/*===========================================================================

FUNCTION L1_SCI_PAGING_BLOCK_TICK

DESCRIPTION
  Called whenever a PCH block is incoming during IDLE mode. This function
  kicks off the ncell measurements for the current paging block.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
/* Number of monitors per paging period.  */
#define L1_SC_NCELL_NUM_MONITORS_PER_PCH_SS               7
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#define L1_SC_NCELL_NUM_MONITORS_PER_PCH               5
#else
#define L1_SC_NCELL_NUM_MONITORS_PER_PCH            7
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

void l1_sci_paging_block_tick( gas_id_t gas_id )
{
  boolean skip_frame = FALSE;
  #if defined(FEATURE_GPRS_GBTA) || defined (FEATURE_DUAL_SIM_GERAN_TUNEAWAY)
  l1_sc_wcdma_data_t *l1_sc_wcdma_data_ptr = &l1_sc_wcdma_data[gas_id];
  #endif /* defined(FEATURE_GPRS_GBTA) || defined (FEATURE_DUAL_SIM_GERAN_TUNEAWAY)*/

  MSG_GERAN_HIGH_2_G("l1_sci_paging_block_tick: gl1_msg_get_multi_sim_sys_mode:%d geran_get_sys_multi_sim_pref:%d ",
                      gl1_msg_get_multi_sim_sys_mode(),
                      geran_get_sys_multi_sim_pref()
                     );

  #if defined(FEATURE_GPRS_GBTA) || defined (FEATURE_G2X_TUNEAWAY)

  MSG_GERAN_HIGH_2_G("OTHER_GSM_CLIENT_IN_TRANSFER %d, drdsds %d",OTHER_GSM_CLIENT_IN_TRANSFER(gas_id), gl1_drdsds_enabled(gas_id));
  /* as we are not suspending in Transfer */
  /* same as checking GBTA Active/Allowed */
  if(gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY ||
     (gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY &&
      geran_get_sys_multi_sim_pref() == SYS_MODEM_DS_PREF_DUAL_STANDBY))
  {
    if( ! (grm_gbta_info[gas_id].idle_event == GBTA_EV_IDLE_PWR_MON )   || gl1_drdsds_enabled(gas_id))
    {
      if (l1_sci_wcdma_paging_block_tick(gas_id))
      {
       /* setup monitors and skip the first frame */
       skip_frame = TRUE;
      }
    }
  }
  else
  #endif /*  FEATURE_GPRS_GBTA */
  {
    if (l1_sci_wcdma_paging_block_tick(gas_id))
    {
      /* setup monitors and skip the first frame */
      skip_frame = TRUE;
    }
  }

  /* surround_tick is in units of 51 multi frames.
   * In Idle mode, each paging block is every bs_pa_mfrms (n=2 to 9).
   * surround_tick should be incremented irrespective of TRM getting denied / PCH failure.
   */
  l1_sci_inc_surround_tick(l1_sc_idle_data[gas_id].idle.paging_multiframes, gas_id);

  if(gl1_msg_get_multi_sim_standby_mode() == SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
  {
    /*7 Monitors per paging period.in case of SS and 5 otherwise	*/
    l1_sci_monscan_do_idle(L1_SC_NCELL_NUM_MONITORS_PER_PCH_SS,
                           SYS_PARAM_NUM_MEASUREMENTS_PER_FRAME_NCELL_PWR_MON,
                           gas_id );
  }
  else
  {
    l1_sci_monscan_do_idle(L1_SC_NCELL_NUM_MONITORS_PER_PCH,
                           SYS_PARAM_NUM_MEASUREMENTS_PER_FRAME_NCELL_PWR_MON,
                           gas_id );
  }

  if ((l1_get_idle_monitors_in_progress(gas_id) == 0) && (!skip_frame)) 
  {
     /* no GSM surrounding cells nor WCDMA reconf so need to schedule WCDMA id search here */
     l1_wcdma_schedule_idle( NULL, gas_id );
  }
}

#ifdef FEATURE_GPRS_GBTA
/*===========================================================================

FUNCTION  l1_sci_paging_block_gbta_tick

DESCRIPTION
  This function is called to tick the wcdma paging block tick when
  GBTA is active. Dont want to call l1_sci_paging_block_tick as this
  function is called in the idle frame of transfer (52)

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sci_paging_block_gbta_tick( gas_id_t gas_id )
{

   if (l1_sci_wcdma_paging_block_tick(gas_id))
   {
      /* no GSM surrounding cells so need to schedule WCDMA id search here */
      l1_wcdma_schedule_idle( NULL, gas_id );

   }

}
/*===========================================================================

FUNCTION gl1_allow_sci_inc_surround_tick_gbta

DESCRIPTION
  Increment surround_tick.  It is a wrapper for l1_sci_inc_surround_tick

  Surround_tick is in units of 51 multi frames.
  In Idle mode, each paging block is every bs_pa_mfrms (n=2 to 9).
  In Dedicated mode, it should be incremented every 51 multi frame.

  Note that Surround_tick should be incremented irrespective of TRM getting denied / PCH failure.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_allow_sci_inc_surround_tick_gbta( gas_id_t gas_id )
{
  l1_sci_inc_surround_tick(l1_sc_idle_data[gas_id].idle.paging_multiframes, gas_id);
}

#endif /* FEATURE_GPRS_GBTA */

/*===========================================================================

FUNCTION  l1_sc_idle_add_serving_cell_to_balist

DESCRIPTION
  Takes the info from the Idle Mode serving cell and puts it in the
  serving cell field of the BA list.  If we re-enter Idle this can
  be used to add the serving cell to the BA list - useful for
  re-selection.

DEPENDENCIES
  Need to coordinate fields populated by this function with fields
  transferred by l1_sc_balist_add_serving_cell().

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_sc_idle_add_serving_cell_to_balist( gas_id_t gas_id )
{
   cell_T* cell;
   idle_data_T  *idle_data_ptr;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

   cell                = l1_sc_balist_get_serving_cell_ptr(gas_id);
   cell->bs.arfcn      = gl1_get_serving_cell_arfcn(gas_id);
   cell->bs.rx_power   = gl1_get_serving_cell_rx_power(gas_id);
#ifdef FEATURE_GSM_CELL_RESELECTION_OPTION20
   if( NULL != idle_data_ptr )
   {
   cell->bs.bsic_known = TRUE;
   cell->bs.bsic       = gl1_get_serving_cell_BSIC(gas_id);
     /* Adjust lag for ccch_group. If ccch_group is 0 then could have qbit_lag = 0, fn_lag = 0 */
     cell->bs.qbit_lag   = (5000 - ((idle_data_ptr->paging_data.ccch_group << 1 ) * QS_PER_SLOT)) ;
     cell->bs.fn_lag     = SUB_FN(0,1);
   }
   else
   {
      MSG_GERAN_ERROR_0(" NULL idle_data_ptr, dont store BA scell BSIC ");
      cell->bs.bsic_known = FALSE;
   cell->bs.qbit_lag   = 0;
   cell->bs.fn_lag     = 0;
   }
#endif /* FEATURE_GSM_CELL_RESELECTION_OPTION20 */

   l1_sc_balist_set_serving_cell_valid( TRUE, gas_id );

#ifdef DEBUG_SCE_MSG
   #error code not present
#endif
}


/***
 *
 *  Module:
 *  -------
 *  SCE code for BCCH reading
 *
 ***/

/*===========================================================================

FUNCTION L1_SC_IDLE_POST_SORT_PROCESS

DESCRIPTION
  Should be called each time the BA list is sorted in Idle Mode.  This
  function looks for cells entering or leaving top six and takes
  appropriate action. E.g. schedules cells re-entering top six for
  BCCH decode after next measurement report.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sc_idle_post_sort_process( gas_id_t gas_id )
{
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint32  i;
   cell_T *cell;

   /* Loop through sorted cells */
   for (i=0; i<l1_sc_globals_ptr->sorted_cell_count; i++)
   {
      cell = l1_sc_globals_ptr->sorted_cells[i];

      /* Is this cell in the top six?
       * Kind of redundant check.
       */
      if (cell->rank < 6)
      {
         /* Did it just enter top six or just
          * get strong enough to see?
          */
         if ( (cell->prev_rank >= 6) ||
              ( (cell->bs.prev_rx_power < SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16) &&
                (cell->bs.rx_power >= SYS_PARAM_MIN_ACCEPTABLE_RX_LVL_dBm_x16 ) ) )
         {
            /* Have we already decoded the BSIC on this cell? */
            if (cell->bs.bsic_known)
            {
               /* Check how long cell has been out of top 6. */
               if (cell->next_tick < l1_sc_globals_ptr->surround_tick)
               {
                  /* Need to make sure BCCH is redone soon. */
                  /* but not if GRR is controlling BCCH/PBCCH decodes. */
                  if (!l1_sc_globals_ptr->autonomous_bcch)
                  {
                     /* inform RR that we are cancelling surround update req */
                     if(cell->sb_count_to_bcch == BCCH_DO_NOW)
                     {
                        L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, FALSE , gas_id);
                     }

                     cell->sb_count_to_bcch = BCCH_DONT_DO;
                  }
                  else
                  {
                     cell->sb_count_to_bcch = BCCH_DO_AFTER_MEAS_RPT; /* don't receive BCCh sys infos unless RR knows this cell */
                  }
               }
               else
               {
                  /* Cell has been out of best 6 for a long time -
                   * mark for new acquisition.
                   * However need to be careful for the scenario where we have just
                   * found a ncell FB/SB and next_tick has been incremented by
                   * l1_sc_globals.surround_tick + TICKS_PER_SB which will also
                   * drop us into this else statement but for the wrong reason!!
                   */
                  if( (cell->next_tick - l1_sc_globals_ptr->surround_tick) > TICKS_PER_SB )
                  {
                     if (!l1_sc_globals_ptr->autonomous_bcch)
                     {
                        /* inform RR that we are cancelling surround update req */
                        if(cell->sb_count_to_bcch == BCCH_DO_NOW)
                        {
                           L1_send_MPH_SURROUND_UPDATE_FAILURE_IND( cell->bs.arfcn, FALSE , gas_id);
                        }
                     }

                     cell->sb_count_to_bcch = BCCH_DONT_DO;
                     cell->bs.bsic_known    = FALSE;
                  }
               }
            }
         }

         /* Check for cells without BSIC known entering/leaving top group. */
         if (!cell->bs.bsic_known)
         {
            if ( (cell->rank < TOP_CELLS_THRESH) && (cell->prev_rank >= TOP_CELLS_THRESH) )
            {
               /* Shorten time delay to next acq. attempt. */
               if (cell->next_tick > TICKS_PER_FCB_DIFF)
               {
                  cell->next_tick -= TICKS_PER_FCB_DIFF;
               }
               else
               {
                  cell->next_tick = 0;
               }
            }
            else if ( (cell->rank >= TOP_CELLS_THRESH) && (cell->prev_rank < TOP_CELLS_THRESH) )
            {
               /* Lengthen time delay to next acq. attempt. */
               cell->next_tick += TICKS_PER_FCB_DIFF;
            }
         }
      }
   }
}


void l1_sc_reselection_apply_freq_correction( ARFCN_T arfcn ,gas_id_t gas_id)
{
  cell_T*  cell_ptr = l1_sc_balist_get_cell_by_arfcn( arfcn ,gas_id);
  if( NULL != cell_ptr )
  {
    if( FALSE != cell_ptr->afc_info.valid )
    {
      gl1_msg_acq_afc_init( arfcn,gas_id );
      gl1_msg_reset_pdm( cell_ptr->afc_info.freq_error, arfcn,gas_id );
      gl1_msg_update_xo_rgs(TRUE, TRUE, gas_id);
    }
  }
}

/*===========================================================================

FUNCTION l1_sc_idle_updt_running_avg_len

DESCRIPTION
  Called to initialize ra_len and mfrms

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

static void l1_sc_idle_updt_running_avg_len( gas_id_t gas_id )
{
   l1_sc_idle_data_t *l1_sc_idle_data_ptr = &l1_sc_idle_data[gas_id];
   idle_data_T       *l1_idle_data_ptr    = &l1_idle_data_store[gas_id];
   uint8              ra_len;

   if(  (l1_idle_data_ptr->paging_data.paging_data_valid)
     && (l1_idle_data_ptr->paging_data.paging_mode == NORMAL_PAGING) )
   {
      /* camping data available */

      /* update paging_multiframes only if mfrms are available.
      * This is mainly needed when we start in GPRS mode and then go into idle mode
      */
      l1_sc_idle_data_ptr->idle.paging_multiframes = l1_idle_data_ptr->campedon_cell_data.BS_PA_MFRMS;

      ra_len = l1_sc_calc_running_avg_length( l1_sc_balist_get_num_cells( gas_id),
                                        (uint8)l1_sc_idle_data_ptr->idle.paging_multiframes );

      if(ra_len == 0)
      {
         /* ra_len not known/invalid, so use default value */
         ra_len = L1_SC_MAX_BUF_LEN_GSM;
      }
   }
   else
   {
      /* not camped / SI3 not read so BS_PA_MFRMS is not valid */
      ra_len = L1_SC_MAX_BUF_LEN_GSM;
   }
  
   l1_sc_balist_set_ra_length( ra_len, gas_id );
   l1_sc_monscan_set_req_passes( ra_len, gas_id );
}

