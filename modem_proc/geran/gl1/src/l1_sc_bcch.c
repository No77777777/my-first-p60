
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                            SCE: BCCH RECEIVE and MEASUREMENTS

GENERAL DESCRIPTION
   This module contains driver functions for SCE BCCH reception and SC
   measurements.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_bcch.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who       what, where, why
--------   --------  ---------------------------------------------------------- 
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
18/01/16   zf        CR961805: Set first burst flag in x2g_startup cmd
01/02/18   sk       CR1101677 Issue X2G Cleanup if required when aborting NSCH/NBCCH inside QBTA
10/02/17   rv        CR2003369 NBCCH not prioritized over CCCH when collision is detected with the same SUB
07/10/16   gmr       CR1050459 Optimize the durations for NCELL Activities
12/10/16  mn         CR1074548 Duplicate entries in consecutive log packets 0X5A6C.
04/07/16   akm       CR1036812 Scheduling NCELL BCCH activity from post tick
21/06/16   km        CR1026966 Clearing Frame Manager before rescheduling Ncell BCCH
19/10/15   br        CR926522 GL1 to Pass the frequency offset to rotator to correct in GBTA.
13/05/16   sk        CR1013011 Clear DRX_REF variable of sb/fcb/bcch if the activity's start_FN is skipped/missed.
09/05/16   sm        CR1002861 Added QSH GL1 handler : PCH collision with Ncell activity and continuous Ncell activity failure  .  
02/11/15   sk        CR932290 Ensure NBCCH is not aborted when prioritized over CCCH
16/09/15   hd        CR905810 Check if drx_ref is already set before registering for NCELL activity
03/09/15   nm        CR899334 dont prioritize nbcch immediately if bcch fn elapsed
26/08/15   nm        CR890755 correct the next nbcch/sbcch fn calculation in transfer state
19/08/15   mc        CR874662 CXM Freq ID support for X2G startup and X2G preload
15/07/15   sjv       CR869294 Book in DRX manager for Msim Idle NCell activity only while reserving TRM 
23/07/15   js        CR835459 Avoid retrying NBCCH in the next frame when ISR overrun is deteected.   
12/06/15   js        CR850684 Change QBTA offset for X2G startup 
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
30/04/15   ip        CR799995 FR26290 Sleep when X tech is in connected state
30/04/15   am        CR830972 Use individual subs for storing afc
20/02/15   nm        CR798066 check ptm gas id before resetting trm status from nbcch abort callback
09/02/14   nm        CR793018 correct the hyperframe wrap around logic for bcch fn
12/02/15   am        CR771762 BCCH Decode Failures at Good RSSI when the NCell is drifted by 1.5KHz
05/02/15   npt       CR786002 Fix compilation errors when FEATURE_DUAL_SIM is enabled     
19/01/15   npt       CR783249 Fix compilation errors on Thor when FEATURE_DUAL_SIM is enabled    
12/01/15   sjv       CR756154 Enhance Band Registration for Activities 
12/12/14   sk        CR763710 Ensure NBCCH is read when consistently colliding with serving PCH or other sub's PCH
12/12/14   pa        CR763859:Handling of RxD clients in GL1 TRM structures.
05/12/14   pa        CR765062: Release BCCH band registration in Init and Waiting state. 
18/11/14   ab        CR758820 PCHXFER : Correct Featurization and Enable for FR21036 : Own in PCH in Transfer for SS.
04/11/14   sk        CR748804 Ensure NBCCH read doesnt get aborted when given priority over page
20/10/14   sk        CR735697 Ensure GL1 doesnt toggle between SI3/SI4 on TRM denial, and does explicit TRM release before making reservation in non-G only mode
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
29/08/14   cah       CR716092: Handling of TRM_AYSNC_DR_INFO_REQ and removal of DR check before band release
03/09/14   sp        CR715190 Function call to reset GBTA state from Active to Allowed/None
18/08/14   og        CR636938. Prevent NCELL BCCH from getting scheduled with
                     a -ve QS offset.
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
28/07/14   ssh       CR700687: Handle X2G start up from warm up ISR
22/07/14   cah       CR697586  trm_client.cpp:4539   Assertion  search_activity_q.cnt == 0 failed
10/07/14   js        CR593074- ROTA/QBTA Feature implementation
04/07/14   cs        CR688003 GRM functions moved to gpl1_grm_intf.c
30/05/14   ws        CR657674 Data + MMS Phase 1
17/06/14   cs/ch     CR651795 TRM Updated changes for DR-DSDS
29/05/14   cs/pa     CR660831 Use new trm_status definitions specific to GL1
22/05/14   cah       Bolt 2.0: FEATURE_GERAN_DRDSDS: Scell measurement changes
08/05/14   nk        CR660831: GSM Layer 1 compares wrong enum values for internal TRM_DENIAL status.Change TRM_DENIAL enum to TRM_NOT_GRANTED to aoid any confusion going forward.
08/04/14   cs        CR638933 Allow equal priority event to be booked when suspend is set
17/03/14   sp        CR633653:Reschedule NBCCH and NSCH activities if the resrved occassion is missed due to a deferred wakeup from W
03/03/14   am        CR630494 Don't release FW in the same frame as FW setup
12/03/14   ssh       CR627985 Skip first burst decode when waking up from sleep inside a QTA gap
25/02/14   am        CR615566 Activities should not clear their DRX entries based on reference
04/02/14   am        CR604532 Don't release FW in the same frame as FW setup
15/01/13   sp        CR600563: Correct the Regression of 592577 which resulted in no ncell acq in Single Sim mode.
19/11/13   ssh/ip    CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
15/11/13   nm        CR575532 Back-out changes for CR537690 & CR547955 - power optimization changes
04/10/13   nk        CR 549654 : Preventing scheduling of bcch state machine when frame manager has aborted that activity
18/10/13   sk        CR563250 Ensure NBCCH booked with QTA over head and issue CM startup only on the startup frame
11/10/13   sk        CR557161 Remove the duplicate trm release at the end of the GBTA gap
05/10/13   sk        CR554665 Backout the power opt temperorily(CR537690)
27/09/13   ab        CR550563 TR 1.1 - GBTA - Make Pseudo TRM_GRANT NULL after activity over.
09/09/13   ab        CR539370 TR 1.1 GBTA - Transfer Sub should not be given TRM grant in during GBTA Active gap.
29/08/13   gk        CR496693:The drx reference should be removed when the reservation FN has been reached
15/08/13   sk        CR524039 GBTA bringup changes
25/06/13   cs        Major Triton TSTS Syncup
22/05/13   ap        CR485912: Revised GFW state transitions
10/06/13   cs        TSTS Updates
16/05/13   og        CR453445. QTA Ncell BCCH.
15/04/13   am        CR473834 Avoid TRM release while FW loading is in progress
19/04/13   cja       CR448231 Correct CR398356 for trans mode NCell SCH outside gap
13/04/13   mc        CR471759 DSDS: QTA Support : Initial code changes & Triton Bring-Up
25/02/13   ap        CR 449179 DSDS Don't issue NCELL BCCH commands to the msg layer when QTA init fail
07/02/13   ap        CR449859 Always enter GFW state MEAS_ACTIVE for gonly sub else
                             enter GFW ACTIVE state
18/01/13   mc        CR437838
12/12/12   mc        CR431452 NCELL SCH and BCCH start FN corrections for QTA
10/05/11   nt        CR287266 Incorrect move to StateWaiting in DSDS
27/04/10   cs        CR236460 Latest AFC\XO handling updates
10-03-16   tjw       Renamed UNUSED to __UNUSED_ARG to avoid name clash.
08/02/10   cja       XO Update
23/03/09   cs        Updates for common XO interface
09/11/06   og        Fix to CR92174, handle IDLE NCELL SCH / BCCH decode at high frequency
                     error conditions.
04/05/05   nt        Added extra check that the bcch.mask has been set, to the funciton
                     next_bcch_fn to stop any possibility of the while loop getting stuck.
03/07/05   sv        Tick the DRX Manager while issuing an ncell abort.
02/11/05   sv        Fixed Idle Abort problem due to issuing multiple aborts.
01/26/05   sv        Add support for Frame Manager.
11/01/04   yhong     Changed NCell Abort API to take a parameter
10/20/04   sv        Optimization of Ncell SCE awake time
10/08/04   ws        Removed l1_sci_bcch_active as functionality moved to NPL1
09/22/04   gfr       Added function l1_sci_bcch_active to check if a BCCH is active.
08/26/04   sv        Lint cleanup
06/14/04   gfr       Log ncell BCCH metrics.
06/22/04   dp        Lint cleanup
06/21/04   gsc       Added new function setup_next_bcch_fn which setups up
                     the frame manager with the required SC BCCH frames in the
                     future. Pass in an extra argument to l1_sci_set_required_frames.
                     Also, call l1_sci_clear_required_frames when giving up.
06/02/04   gw        Keep L1 awake in RanOutOfFrames state.
05/26/04   sv        Lint Cleanup.
05/14/04   gw        Yet another refinement of the SCE aborting mechanism.
03/16/04   gw        Modified receive module design to be compatible with
                     background PLMN search.
02/25/04   gw        Lint clean up.
02/09/04   dp        Added support for background HPLMN searching
11/05/03   gfr       Support for quad-band.
02/10/03   kf        Bug fix in gpl1_gprs_sc_receive_pbcch_trans.
08/28/03   gw        Changed assignment in if() in l1_sci_abort_bcch() to test.
08/21/03   gw        Modified aborting mechanism to support aborting BCCH
                     without aborting other receives in progress at same time.
12/08/03   kf        Removed sch_timing_error.
07/14/03   gw        Removed ASSERTs.
14/07/03   kf        gprs ncell fh fn changes.
07/11/03   gfr       Lint cleanup.
07/11/03   gw        Added check for repeat_period==0 before dividing by it when
                     calcualting FN for ncell pbcch.
07/08/03   kf        resurrected psi1 repeat period.
07/08/03   gw        GRR/L1 SCE cleanup. Changes to support GRR scheduling of ncell BCCH decodes.
06/23/03   gfr       Compiler warning cleanup.
06/18/03   ag        Set state to NULL after GPRS Xfer BCCH read.
06/12/03   gw        Renamed BCCH_MAX_TRIES to L1_SC_BCCH_MAX_TRIES moved it to
                     l1_sc.h and changed value from 3 to 1.
06/11/03   gw        Fixed function gpl1_gprs_update_bcch_stop().
06/09/03   gw        Added mechanism to ensure that bcch doesn't require
                     multiple ticks (at the same time) from the DRX manager.
29/05/03   kf        ncell pbcch read changes
05/02/03   gw        Merged in power transfer changes from branch.
05/01/03   ag        Updates to ncell pbcch reading
04/30/03   gw        Corrected next_bcch_fn() so that it will always return a
                     future FN (never current FN).
04/28/03   gw/bk     Optimized function next_bcch_fn().
04/04/03   gw        Removed obsolete debug code.
02/28/03   kf        Added support for ncell pbcch reading
02/17/03   ag        Correction to qbit offset and NCELL BCCH frame number
                     under switch FEATURE_GPRS_TBF_SUSPEND
02/07/03   ag        Updates to FEATURE_GPRS_TBF_SUSPEND
01/16/03   ws        Modified Ncell BCCH scheduling
01/16/03   ag        Add BCCH_MASK_4_13NORM
12/18/02   ag        Adjustments to start_qbit under switch
                     FEATURE_GPRS_TBF_SUSPEND
12/03/02   mk        General GPRS featurization cleanup, added temporary feature
                     FEATURE_DEBUG_GPRS_OK.
11/26/02   mk        Fixed BCCH not being decoded bug in GSM caused by GPRS
                     featurization error in l1_sci_tick_bcch().
11/15/02   mk        Updated GPRS baseline with release 2.0 functionality.
11/12/02   ag        Added PL1 GPRS TBF suspension for NCELL BCCH reading code
                     hooks under switch FEATURE_GPRS_TBF_SUSPEND.
10/08/02   kf        Added PL1 GPRS measurement code hooks under switch
                     FEATURE_GPRS_MEAS.
10/01/02   ATM       Added central get and set FN routines
07/11/02   gw        Changed reference to power_dBm_x16 in metrics report to
                     pwr_dBm_x16.
07/03/02   dp        Reduced BCCH_MAX_TRIES to 3
07/02/02   dp        Fixed bug in passing ARFCN array to rx_ncell_bcch (was
                     previously passing in only 3 ARFCNs)
06/27/02   dp        gl1_msg_rx_ncell_bcch interface change for GPRS
05/21/02   gw        Changed type for power from uint8 to dBx16_T to eliminate
                     gl1_dBm_to_l1pwr.
05/09/02   JC        Fixed hardcode of AGC rx_power s.t. it inherits the cell
                     structure agc parameters.
03/28/02   JC        Replaced GSM_DEBUG macros with MSG_ macros.
03/05/02   JC        Cleanup of legacy header include.
02/14/02   gw        Changes to match new message layer interface for AGC.
02/07/02   gw        Improved mechanism for doing aborts.
12/21/01   gw        Changes to support DRX.  Modified mechanism for scheduling
                     neighbor cell BCCH reception.
12/18/01   dp        Changes to reflect ncell handler name change
10/31/01   dp        Incorporated changes due to new CCCH decode_rpt format
10/16/01   gw        Changed CCWW types to QCT types.  Removed conversion to
                     dBm from l1_sci_read_bcch().
10/10/01   gw        Modified Ncell BCCH reception to use message layer
                     functions.
09/28/01   jc        Featurized references to global variables that have been
                     removed.
09/25/01   mk/gw     General SCE cleanup. Mainlined MONITOR_DEBUG
08/31/01   JC        Placed COMMENT block around Ps_EQU reference & removed
                     references to obsolete compile switch and variables.
08/20/01   mk/gw     Added Ncell support, featurized via MONITOR_DEBUG.
08/20/01   mk        Initial version.  Minor Cleanup and Header Addition.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "comdef.h"
#include "l1_drx.h"
#include "gl1_msg_g.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_log.h"
#include "l1_sc.h"
#include "l1_utils.h"   /* for gl1_get_FN( ) */
#include "ms.h"
#include "msg.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "rr_l2.h"
#include "sys_stru.h"
#include "l1_fm.h"

#include "gpl1_gprs_if.h"
#include "gpl1_gprs_utils.h"
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "gmacl1.h"
#include "l1_task.h"
#include "l1_os.h"
#include "l1_sc_int.h"

#include "geran_dual_sim.h"
#include "gpl1_dual_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
/*
 * Extern declarations
 * ------------------------------
 */

extern boolean  bcch_mask_ext[NUM_GERAN_DATA_SPACES];
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

/*
 * Prototypes for local Functions
 * ------------------------------
 */
static void bcch_aborted( void* dummy, gas_id_t gas_id );
static void ncell_bcch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void ncell_bcch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );
static void ncell_bcch_abort_callback( gas_id_t gas_id );
static uint32 next_bcch_fn( uint32 serv_fn, gas_id_t gas_id );
static void setup_next_bcch_fn(uint32 serv_fn, boolean pbcch_flag, gas_id_t gas_id);
static void l1_sc_schedule_retry ( uint32 frame_no , gas_id_t gas_id );

//static void gpl1_gprs_ncell_pbcch_data_callback( gl1_msg_decode_rpt *rpt );
//static void gpl1_gprs_ncell_pbcch_metrics_callback( gl1_msg_metrics_rpt *rpt[] );
static void ncell_bcch_data_callback_gprs( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void ncell_bcch_metrics_callback_gprs( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );

static void gpl1_ncell_bcch_rx( gas_id_t gas_id );

typedef struct
{

/*
 * Static Variable Declarations
 * ----------------------------
 */
struct
{
   enum
   {
      StateNull,
      StateInit,
      StateWaiting,
      StateActive,
      StateRanOutOfFrames,
      StateAborting
   }                    state;

   ARFCN_T              arfcn;
   uint8                bsic;
   sys_algo_agc_T      *agc;
   uint32               fn_lag;
   uint32               start_fn;
   int16                start_qbit;

   uint8                tries;
   uint16               mask;
   boolean              success;

   uint8                returned_tc;

   boolean              data_done;      /* Need to make sure both callbacks have */
   boolean              metrics_done;   /* occured before we go to StateDone     */

   gl1_msg_rx_rpt       rx_rpt;
   gl1_msg_decode_rpt   decode_rpt;
   gl1_msg_metrics_rpt  metrics_rpts[4];

   uint8                drx_ref;

   /* @@OG : Testing potential fix to CR92174 */
   l1_ncell_afc_info_t*  afc_info_ptr;

#ifdef FEATURE_GPRS_GBTA
   uint8                scheduled_status;
#endif /* FEATURE_GPRS_GBTA */

   boolean               nbcch_use_high_priority;
   gl1_msg_chan_type     chan_type;  // can be PCH or BCCH
   async_ccch_type_t     async_ccch_type; // SYNC or ASYNC RX

} bcch;


boolean ncell_bcch_in_progress ;
}l1_sc_bcch_data_t;

static l1_sc_bcch_data_t  l1_sc_bcch_data[NUM_GERAN_DATA_SPACES];

ARFCN_T get_next_bcch_arfcn(gas_id_t gas_id)
{
  return l1_sc_bcch_data[gas_id].bcch.arfcn;
}

/*
 *     Function name :  l1_sc_bcch_init
 *     --------------------------------
 *
 *     Description :
 *     ---------------
 *     Init static data in this file.
 *
 */
void l1_sc_bcch_init( gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   l1_sc_bcch_data_ptr->bcch.state   = StateNull;
   l1_sc_bcch_data_ptr->bcch.drx_ref = 0;
}

/*
 *     Function name :  l1_sc_bcch_shutdown
 *     ------------------------------------
 *
 *     Description :
 *     ---------------
 *     Clean up static data in this file.
 *
 */
void l1_sc_bcch_shutdown( gas_id_t gas_id )
{
}

/*
 *     Function name :  l1_sci_start_bcch
 *     ----------------------------------
 *
 *     Description :
 *     ---------------
 *     Start the BCCh receive state machine.
 *
 *     NOTE: call this function with interrupts locked
 *
 *     Parameters :
 *     ---------------
 *     IN - bs   - The base station to receive the BCCh from.
 *     IN - mask - Bitmask indicating which of the BCCh blocks in a 408
 *                 frame period to receive. Bit 0 corresponds to the first
 *                 normal BCCh (TC==0) and so on, up to bit 8 which
 *                 corresponds to the first extended BCCh (TC==0) and so on.
 *
 */
void l1_sc_start_bcch( cell_T *cell, gas_id_t gas_id )
{
   int i;
   l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   ASSERT_TASK();

   if ( l1_sc_bcch_data_ptr->bcch.state != StateNull )
   {
      MSG_GERAN_ERROR_0_G("ncell BCCH already in progress");
   }

   /* Set up info needed for the reception/decoding */
   l1_sc_bcch_data_ptr->bcch.arfcn          = cell->bs.arfcn;
   l1_sc_bcch_data_ptr->bcch.bsic           = cell->bs.bsic;
   l1_sc_bcch_data_ptr->bcch.agc            = &(cell->bs.agc);
   l1_sc_bcch_data_ptr->bcch.start_qbit     = cell->bs.qbit_lag;
   l1_sc_bcch_data_ptr->bcch.fn_lag         = cell->bs.fn_lag;

   while( l1_sc_bcch_data_ptr->bcch.start_qbit < 0 )
   {
     l1_sc_bcch_data_ptr->bcch.start_qbit += 5000;
     l1_sc_bcch_data_ptr->bcch.fn_lag     = SUB_FN( l1_sc_bcch_data_ptr->bcch.fn_lag, 1 );
   }

   l1_sc_bcch_data_ptr->bcch.tries          = 0;
   l1_sc_bcch_data_ptr->bcch.mask           = cell->bcch_mask;
   l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = FALSE;

   /* Clear the flags so we know when both callbacks have been called */
   l1_sc_bcch_data_ptr->bcch.data_done      = FALSE;
   l1_sc_bcch_data_ptr->bcch.metrics_done   = FALSE;

   /* Set up stuff required by the gl1_msg layer             */
   /* Some of this doesn't really need to be done every time */
   l1_sc_bcch_data_ptr->bcch.rx_rpt.data = &l1_sc_bcch_data_ptr->bcch.decode_rpt;
   for (i=0; i<4; i++)
   {
      l1_sc_bcch_data_ptr->bcch.rx_rpt.metrics[i] = &l1_sc_bcch_data_ptr->bcch.metrics_rpts[i];
   }

   l1_sc_bcch_data_ptr->bcch.state          = StateInit;

   /* Setup the pointer to this ncells freq error correction info */
   l1_sc_bcch_data_ptr->bcch.afc_info_ptr = &( cell->afc_info );

}


/*
 *     Function name :  l1_sci_abort_bcch
 *     ----------------------------------
 *
 *     Description :
 *     ---------------
 *     Stop the BCCh state machine before it finished.
 *     Call from ISR context.
 */
void l1_sci_abort_bcch( gas_id_t gas_id )
{
   l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   ASSERT_INT();

   switch (l1_sc_bcch_data_ptr->bcch.state)
   {
      case StateInit:
         l1_sc_bcch_data_ptr->bcch.state = StateNull;

         if ( (l1_sc_bcch_data_ptr->bcch.drx_ref != 0) &&
              (l1_sc_bcch_data_ptr->bcch.start_fn != gl1_get_FN( gas_id )) )
         {
            /* remove old frame */
            gl1_drx_release_fn_tick( l1_sc_bcch_data_ptr->bcch.start_fn, gas_id );

            l1_sc_bcch_data_ptr->bcch.drx_ref = 0;
         }

         l1_sc_abort_receive_callback(gas_id);

         break;

      case StateWaiting:
         l1_sc_bcch_data_ptr->bcch.state = StateNull;

         if ( (l1_sc_bcch_data_ptr->bcch.drx_ref != 0) &&
              (l1_sc_bcch_data_ptr->bcch.start_fn != gl1_get_FN( gas_id )) )
         {
            /* remove old frame */
            gl1_drx_release_fn_tick( l1_sc_bcch_data_ptr->bcch.start_fn, gas_id );

            l1_sc_bcch_data_ptr->bcch.drx_ref = 0;
         }

         l1_sc_abort_receive_callback(gas_id);
         l1_fm_clear_required_frames(FM_PRI_NCELL_BCCH, gas_id);

         break;

      case StateActive:
         l1_sc_bcch_data_ptr->bcch.state = StateAborting;
         gl1_msg_abort_rx_ncell_bcch(DELAYED_ABORT,gas_id);
         break;

      case StateRanOutOfFrames:
         l1_sc_bcch_data_ptr->bcch.state = StateAborting;
         break;

      case StateAborting:
         /* do nothing */
         MSG_GERAN_ERROR_0_G("Multiple aborts.");
         break;

      case StateNull:
      default:
         l1_sc_bcch_data_ptr->bcch.state = StateNull;

         /* this should never happen */
         l1_sc_abort_receive_callback(gas_id);
         l1_fm_clear_required_frames(FM_PRI_NCELL_BCCH, gas_id);
         MSG_GERAN_ERROR_1_G("Unexpected state %d", l1_sc_bcch_data_ptr->bcch.state);

         break;
   }

   gl1_drx_require_next_tick(gas_id);
}

/*
 *     Function name :  bcch_aborted
 *     -----------------------------
 *
 *     Description :
 *     ---------------
 *     Callback to be called when a BCCh receive has to be aborted because
 *     layer1 needs frames that were previously available to the surround
 *     cell engine.
 *
 *     Parameters :
 *     ---------------
 *     IN - dummy - Not used.
 *
 */
static void bcch_aborted( void* dummy, gas_id_t gas_id )
{
   l1_sc_bcch_data_t *l1_sc_bcch_data_ptr;
   NOTUSED( dummy );
   ASSERT_INT();

   l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   if((l1_sc_bcch_data_ptr->bcch.state != StateAborting) )
   {
      /* Abort the message layer only if a Ncell BCCH is in progress */
      if(l1_sc_bcch_data_ptr->ncell_bcch_in_progress)
      {
         /* The message layer abort callback will schedule retries */
         l1_sc_bcch_data_ptr->bcch.state = StateRanOutOfFrames;
         gl1_msg_abort_rx_ncell_bcch(DELAYED_ABORT,gas_id);
      }
   }
   else
   {
      MSG_GERAN_MED_0_G("Ncell BCCH abort already in progress!");
   }

}

/*
 *     Function name :  l1_sci_tick_bcch
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Tick the BCCh receive state machine.
 *     After calling l1_sci_start_bcch() this function must be called every frame
 *     until l1_sci_tick_bcch() returns TRUE or the search is aborted via
 *     l1_sci_abort_bcch().
 *     The first call to l1_sci_tick_bcch() must be done in the frame following the
 *     frame where l1_sci_start_bcch() has been called.
 *
 *     Returns :
 *     -----------
 *     FALSE when l1_sci_tick_bcch() should be called again.
 *     TRUE when done.
 *
 */
void l1_sci_tick_bcch( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
   l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];

   uint32 current_FN = gl1_get_FN( gas_id );
   
   ASSERT_INT();

   switch ( l1_sc_bcch_data_ptr->bcch.state )
   {
      case StateInit:
      {
         setup_next_bcch_fn (current_FN, FALSE, gas_id );

         MSG_GERAN_MED_1_G("%d frames to Ncell BCCH receive",SUB_FN(l1_sc_bcch_data_ptr->bcch.start_fn,current_FN));
         l1_sc_bcch_data_ptr->bcch.state = StateWaiting;
      }
         /* fall through */

      case StateWaiting:
         if ( l1_sc_bcch_data_ptr->bcch.start_fn == gl1_get_FN( gas_id ) )
         {
            /* Our tick has arrived so drx_ref is no longer valid. */
            l1_sc_bcch_data_ptr->bcch.drx_ref = 0;

            if ( l1_fm_are_frames_available(FM_PRI_NCELL_BCCH, L1_SC_DURATION_NEW_BCCH, bcch_aborted, 0, gas_id))
            {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
              /* Make sure that FW/RF are initialised and if this is "NOT" first QTA gap after wake up from sleep*/
              if ( gl1_msg_get_multi_sim_mode() &&
                  ( l1_tskisr_blk->firmware_loaded == FALSE ) &&
                    ( gl1_check_qta_active_in_warmup(gas_id) == FALSE ) )
              {
                if ( grm_get_trm_status(l1_tskisr_blk->client_id, gas_id) != GL1_TRM_NOT_GRANTED )
               {
                 l1_tskisr_blk->firmware_loaded = TRUE;
#ifdef FEATURE_QTA
                 if ( l1_tskisr_blk->qta_in_progress )
                 {
                    uint32 offset;

                    MSG_GERAN_HIGH_1_G("FN = %d Init NCELL BCCH - QTA in progress",
                                       gl1_get_FN(gas_id));
#ifdef FEATURE_QBTA 
                    if(gl1_check_qbta_allowed( gas_id)) 
                    {                    
                      offset = l1_sc_bcch_data_ptr->bcch.start_qbit  ;
                    }
                    else
#endif                    
                    {
                      offset = GL1_FW_QBTA_CM_START_UP_OFFSET ;
                    }
                    if(!gl1_hw_qta_fw_rf_init(FALSE, offset ,FALSE,l1_sc_bcch_data_ptr->bcch.arfcn,FALSE, TRUE, gas_id))
                    {
                      l1_fm_clear_required_frames(FM_PRI_NCELL_BCCH, gas_id);
                                                 l1_tskisr_blk->firmware_loaded = FALSE;
                    }
                    else
                    {
                      /* Prevent FM from releasing in the same frame */
                      l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                     FM_PRI_NCELL_BCCH, gas_id );
                    }
                 }
                 else
#endif /* FEATURE_QTA*/
                 {
                    /* Issue the call to init the firmware and RF */
                    gl1_hw_multi_sim_fw_rf_init(TRUE,
                                                l1_task_tcb_read(gas_id),
                                                gsm_l1_wait,
                                                L1_WAIT_HW_SIG,
                                                L1_TIMEOUT_HW_SIG,
                                                GL1_HW_INIT_RX_ONLY,
                                                l1_tskisr_blk->client_id,
                                                FALSE,
                                                gas_id);

                   /* Prevent FM from releasing in the same frame */
                   l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                                  FM_PRI_NCELL_BCCH, gas_id );
                 }
               }
             }
               /* We would expect that the RF activity would have finished and firward loaded.
                          But there is possibility that ISR may have overrun or we may be very close to the frame boundary,
                          hence to avoid 2 sync RX issued to GFW we tick over and don't schedule commands in this frame.  */
               if (gl1_check_possible_isr_overrun(gas_id))
               {
                  /*Dont consider this as an BCCH attempt*/
                  if (l1_sc_bcch_data_ptr->bcch.tries)
                     l1_sc_bcch_data_ptr->bcch.tries--;

                  MSG_GERAN_MED_0_G( "Retry NCELL BCCH, current skipped, possible ISR overrun");
                  l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = FALSE;
                  l1_sc_schedule_retry( ADD_FN(current_FN , 2) , gas_id);
               }
               else
               {
                 if ( (!gl1_msg_get_multi_sim_mode()) ||
                      ( l1_tskisr_blk->firmware_loaded == TRUE))
                  {
                       gpl1_ncell_bcch_rx(gas_id);
                  }
                  else
                  {
                       MSG_GERAN_HIGH_0_G("Retry NCELL BCCH - firmware not initialised.");
                       l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = FALSE;
                       l1_sc_schedule_retry(current_FN, gas_id);
                  }
               }
#else
             /* Single SIM case */
             gpl1_ncell_bcch_rx(gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            }
            else
            {
                l1_sc_schedule_retry(current_FN,gas_id);
            }
         }
         else if (IS_FRAME_NUM_LATER(gl1_get_FN( gas_id ),l1_sc_bcch_data_ptr->bcch.start_fn ))
         {
           MSG_GERAN_HIGH_1_G("Retry NCELL BCCH - as reserved FN:%d is elapsed", l1_sc_bcch_data_ptr->bcch.start_fn);
           /* Our tick is skipped so drx_ref is no longer valid. */
           l1_sc_bcch_data_ptr->bcch.drx_ref = 0;
           l1_sc_schedule_retry(current_FN, gas_id);
         }
         break;

      case StateActive:
         /* We take care of this state in l1_sci_post_tick_bcch() */
         break;

      case StateRanOutOfFrames:
      case StateAborting:
               if( FALSE != l1_scell_afc_info_ptr->valid )
               {
                 /* Initialise the AFC algorithm back to scell value */
                 gl1_msg_acq_afc_init( l1_idle_data->campedon_cell_ARFCN ,gas_id);

                 gl1_msg_set_afc_info( l1_scell_afc_info_ptr->afc_freq,gas_id );

                 /* Reset the afc info for the serving cell */
                 MSG_GERAN_MED_2_G( "Restore afc_freq %dXO for scell %d",
                          l1_scell_afc_info_ptr->afc_freq,
                          l1_idle_data->campedon_cell_ARFCN.num);

                 l1_scell_afc_info_ptr->valid = FALSE;
               }
               else
               {
                 MSG_GERAN_MED_0_G( "No scell afc info to restore");
               }

         gl1_drx_require_next_tick(gas_id);
         break;

      case StateNull:
         /* Should never get here, but make lint happy */
         break;
   }
}

/*
 *     Function name :  gpl1_ncell_bcch_rx
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Programs up the reception of the block.
 *
 *     Returns :
 *     -----------
 *     none
 *
 */
static void gpl1_ncell_bcch_rx( gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
  l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];
   ARFCN_T arfcns[4];
   gl1_msg_rx_cb_type         rx_cb;
   uint16 i;

   /* Our tick has arrived so drx_ref is no longer valid. */
   l1_sc_bcch_data_ptr->bcch.drx_ref = 0;
   l1_sc_bcch_data_ptr->bcch.state = StateActive;

   /* Need this to accomodate GPRS interface which needs 4 ARFCNS */
   for (i=0; i<4; i++)
   {
     arfcns[i] = l1_sc_bcch_data_ptr->bcch.arfcn;
   }

   rx_cb.data_cb    = ncell_bcch_data_callback;
   rx_cb.metrics_cb = ncell_bcch_metrics_callback;
   rx_cb.log_cb     = NULL;
   rx_cb.abort_cb   = ncell_bcch_abort_callback;

   /* OG: Added to tune out any frequency errors */
   gl1_msg_get_afc_info( &(l1_scell_afc_info_ptr->afc_freq), gas_id );

   l1_scell_afc_info_ptr->valid = TRUE;

   MSG_GERAN_ERROR_1_G( "Saved scell phase %d value",
     l1_scell_afc_info_ptr->afc_freq );

   /* Initialise the AFC algorithm */
   gl1_msg_acq_afc_init( l1_sc_bcch_data_ptr->bcch.arfcn,gas_id );

   if( FALSE != l1_sc_bcch_data_ptr->bcch.afc_info_ptr->valid )
   {
     MSG_GERAN_MED_1_G( "set afc freq %d for sb decode",
       l1_sc_bcch_data_ptr->bcch.afc_info_ptr->afc_freq );

     gl1_msgi_reset_acq_afc( l1_sc_bcch_data_ptr->bcch.afc_info_ptr->afc_freq ,gas_id);
   }


   if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) )
   {
     /* For a PCH/BCCH setup the asynch int possible flag */
     l1_set_asynch_int_possible( gas_id );
   }

   gl1_msg_cfg_ncell_bcch( l1_sc_bcch_data_ptr->bcch.bsic & 0x07, TRUE, &rx_cb,gas_id );
   gl1_msg_rx_ncell_bcch ( arfcns, l1_sc_bcch_data_ptr->bcch.agc, (uint16)l1_sc_bcch_data_ptr->bcch.start_qbit, &l1_sc_bcch_data_ptr->bcch.rx_rpt,GL1_MSG_BCCH,ASYNC_ACTIVITY_SUSPENSION,0,gas_id );
   MSG_GERAN_LOW_2_G("start_qbit_gprs:%d SFN:%d",l1_sc_bcch_data_ptr->bcch.start_qbit,l1_sc_bcch_data_ptr->bcch.start_fn);
   MSG_GERAN_LOW_2_G("neigh_fn:%d l1_sc_bcch_data_ptr->bcch.fn_lag:%d",(l1_sc_bcch_data_ptr->bcch.start_fn - l1_sc_bcch_data_ptr->bcch.fn_lag),l1_sc_bcch_data_ptr->bcch.fn_lag);
   gl1_set_ncell_block_start(TRUE, gas_id);

   l1_sc_bcch_data_ptr->ncell_bcch_in_progress = TRUE;

   gl1_drx_require_next_tick(gas_id);

}

void l1_sci_post_tick_bcch( gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
  uint32 current_FN = gl1_get_FN( gas_id );

   ASSERT_INT();

   if ( l1_sc_bcch_data_ptr->bcch.state == StateActive)
   {
         gl1_drx_require_next_tick(gas_id);
   }
   else if( l1_sc_bcch_data_ptr->bcch.state == StateInit )
   {
         setup_next_bcch_fn (current_FN, FALSE, gas_id );

         MSG_GERAN_MED_1_G("%d frames to Ncell BCCH receive",SUB_FN(l1_sc_bcch_data_ptr->bcch.start_fn,current_FN));
         l1_sc_bcch_data_ptr->bcch.state = StateWaiting;   
   }
}



/*
 *     Function name :  l1_sc_read_bcch
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Read results of an BCCh receive.
 *     Must only be called after a call to l1_sci_tick_bcch() returned TRUE.
 *
 *     Parameters :
 *     ---------------
 *     OUT - bcch_data      - Receives pointer to data of the received BCCh.
 *                            Only set if this function returns TRUE.
 *                            Data is valid only till next BCCh search is
 *                            started.
 *     OUT - rx_power       - receive power level of the BCCh.
 *                            Only set if this function returns TRUE.
 *     OUT - timing_error   - The quarter bit timing error of the BCCh. Positive
 *                            means the burst has been received later than expected.
 *                            Only set if this function returns TRUE.
 *     OUT - tc             - The value of TC for the BCCh.
 *                            0..7 for normal BCCh, 8..15 for BCCh ext.
 *                            Always valid.
 *
 *     Returns :
 *     -----------
 *     TRUE when an FCB has been found.
 *     FALSE if no FCB has been found or the receive was aborted because
 *     no frames were available.
 *
 */
  boolean l1_sc_read_bcch( uint8** bcch_data, dBx16_T* rx_power, int16* timing_error, uint8* tc, uint8* num_bursts, gas_id_t gas_id )
  {
     int16   te  = 0;
     dBx16_T pwr = 0;
     int     i;
     l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
     uint8  burst_count = 0;  
     ASSERT_TASK();
  
     *tc     = l1_sc_bcch_data_ptr->bcch.returned_tc;
     if ( l1_sc_bcch_data_ptr->bcch.success )
     {
        *bcch_data    = l1_sc_bcch_data_ptr->bcch.decode_rpt.ccch.data;
  
        /* Have to average the metrics of the four bursts in the block */
        for (i=0; i<4; i++)
        {
           if (l1_sc_bcch_data_ptr->bcch.metrics_rpts[i].valid)
           {
             te  += l1_sc_bcch_data_ptr->bcch.metrics_rpts[i].timing_offset;
             pwr += l1_sc_bcch_data_ptr->bcch.metrics_rpts[i].pwr_dBm_x16;
             burst_count++;
           }
        }
  
        if (burst_count)
        {
          *timing_error = te/burst_count;
          *rx_power     = pwr/burst_count;
        }
        else
        {
          *timing_error = 0;
          *rx_power     = 0;
        }
     }
     else
     {
        for (i=0; i<4; i++)
        {
          if (l1_sc_bcch_data_ptr->bcch.metrics_rpts[i].valid)
              burst_count++;
        }
  
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG
        /* genuine crash when more burst are scheduled */
        if(burst_count > 1)
        {
          if ( l1_sc_bcch_data_ptr->bcch.chan_type == GL1_MSG_CCCH)
          {
            if(GL1_HW_EFS_DEBUG_ENABLED( GL1_EFS_DEBUG_XCCH_IN_XFER ))
            {
#ifdef FEATURE_QSH_MDUMP
              QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
              ERR_FATAL("xCCH failed (arfcn=%d fn=%d) , burst_count %d",
                         l1_sc_bcch_data_ptr->bcch.arfcn.num,
                         gl1_get_FN( gas_id ),
                         burst_count);
            }
          }
          else
          {
            if(GL1_HW_EFS_DEBUG_ENABLED( GL1_EFS_DEBUG_BXCCH_IN_XFER ))
            {
#ifdef FEATURE_QSH_MDUMP
              QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
               ERR_FATAL("BCCH failed (arfcn=%d fn=%d) , burst_count %d",
                          l1_sc_bcch_data_ptr->bcch.arfcn.num,
                          gl1_get_FN( gas_id ),
                          burst_count);
            }
          }
        }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG */
     }
  
     if (burst_count)
     {
       *timing_error = te/burst_count;
       *rx_power     = pwr/burst_count;
     }
     else
     {
       *timing_error = 0;
       *rx_power     = 0;
     }
  
#ifdef FEATURE_GPRS_GBTA_DEBUG
     MSG_GERAN_MED_1_G("scheduled_status %d ",l1_sc_bcch_data_ptr->bcch.scheduled_status );
#endif /* FEATURE_GPRS_GBTA */
  
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG
     if (burst_count)
         MSG_GERAN_MED_3_G("scheduled num_burst %d te %d rxp %d",burst_count,*timing_error, *rx_power);
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG*/
  
     *num_bursts = burst_count;
  
     return l1_sc_bcch_data_ptr->bcch.success;
  }


static void ncell_bcch_data_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
  l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
#ifdef FEATURE_GPRS_GBTA
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_GPRS_GBTA */

   ASSERT_INT();

   /* Log the metrics */
   l1_log_ncell_bcch_msg_metrics(&rpt->ccch, gas_id);
   l1_fm_clear_required_frames(FM_PRI_NCELL_BCCH, gas_id);

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif

   l1_sc_bcch_data_ptr->bcch.success = rpt->ccch.hdr.good_data ? TRUE : FALSE ;
   l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = FALSE;

   l1_sc_bcch_data_ptr->bcch.data_done = TRUE;
   if (l1_sc_bcch_data_ptr->bcch.metrics_done)
   {
      l1_sc_bcch_data_ptr->bcch.state = StateNull;
      l1_sci_receive_done(gas_id);
   }

   if ( rpt->ccch.hdr.good_data )
   {
      MSG_GERAN_MED_3_G("Ncell BCCH Decoded: ARFCN=%d TC=%d FN:%d", l1_sc_bcch_data_ptr->bcch.arfcn.num, l1_sc_bcch_data_ptr->bcch.returned_tc, gl1_get_FN( gas_id ));
   }
   else
   {
      MSG_GERAN_MED_3_G("Ncell BCCH Failed (ARFCN=%d, TC=%d, FN=%d)...", l1_sc_bcch_data_ptr->bcch.arfcn.num, l1_sc_bcch_data_ptr->bcch.returned_tc, gl1_get_FN( gas_id ));
   }

   l1_sc_bcch_data_ptr->ncell_bcch_in_progress = FALSE;

#ifdef FEATURE_GPRS_GBTA
   l1_sc_bcch_data_ptr->bcch.scheduled_status = rpt->ccch.hdr.scheduled_status;
   MSG_GERAN_MED_1_G("ncell_bcch_data_callback scheduled_status %d ",rpt->ccch.hdr.scheduled_status );
#endif /*   FEATURE_GPRS_GBTA */

#ifdef FEATURE_GPRS_GBTA
   /* After BCCH decode , reset back to GBTA_ALLOWED */
   if (IS_GBTA_ACTIVE())
   {
     gpl1_gbta_reset_active_state(gas_id);
     /* as we never got the lock but if it gets extended then usefull to know if that was gbta extension */
     grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);
   }
#endif /* FEATURE_GPRS_GBTA */

   if( FALSE != l1_scell_afc_info_ptr->valid )
   {
     /* Initialise the AFC algorithm back to scell value */
     gl1_msg_acq_afc_init( l1_idle_data->campedon_cell_ARFCN ,gas_id);

     gl1_msg_set_afc_info( l1_scell_afc_info_ptr->afc_freq,gas_id );

     /* Reset the afc info for the serving cell */
     MSG_GERAN_MED_2_G( "Restore afc_freq %dXO for scell %d",
              l1_scell_afc_info_ptr->afc_freq,
              l1_idle_data->campedon_cell_ARFCN.num);

     l1_scell_afc_info_ptr->valid = FALSE;
   }
   else
   {
     MSG_GERAN_MED_0_G( "No scell afc info to restore");
   }

}

static void ncell_bcch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   /* Log the metrics */
   l1_log_ncell_bcch_burst_metrics(rpt, gas_id);

   l1_sc_bcch_data_ptr->bcch.metrics_done = TRUE;
   if (l1_sc_bcch_data_ptr->bcch.data_done)
   {
      l1_sc_bcch_data_ptr->bcch.state = StateNull;
      l1_sci_receive_done(gas_id);
   }
}

static void ncell_bcch_abort_callback( gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
#ifdef FEATURE_GPRS_GBTA
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif /* FEATURE_GPRS_GBTA */

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      /* If aborting the ncell BCCH allow SUSPENSION_FRAME_DELAY frames before disabling the FW */
      if ( gl1_msg_get_multi_sim_mode() )
      {
        l1_fm_set_suspend_trm_release( SUSPENSION_FRAME_DELAY,
                                       FM_PRI_NCELL_BCCH, gas_id );
      }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   l1_fm_clear_required_frames(FM_PRI_NCELL_BCCH, gas_id);
   l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = FALSE;

#ifdef FEATURE_GPRS_GBTA
   /* After BCCH decode , reset back to GBTA_ALLOWED for idle sub*/
   if (IS_GBTA_ACTIVE() && 
      (gas_id != gl1_get_gas_id_for_ptm_client()))
   {
     gpl1_gbta_reset_active_state(gas_id);
     /* as we never got the lock but if it gets extended then usefull to know if that was gbta extension */
     grm_set_trm_status(GL1_TRM_NULL, l1_tskisr_blk->client_id, gas_id);
   }
#endif /* FEATURE_GPRS_GBTA */

   /* Why were we aborted? */
   if (l1_sc_bcch_data_ptr->bcch.state == StateRanOutOfFrames)
   {
      /* We ran out of frames */

      /* Retry? */
      if ( ++l1_sc_bcch_data_ptr->bcch.tries == L1_SC_BCCH_MAX_TRIES )
      {
         /* We've done all our retries, call it quits */
         l1_sc_bcch_data_ptr->bcch.state   = StateNull;
         l1_sc_bcch_data_ptr->bcch.success = FALSE;
#ifdef FEATURE_GPRS_GBTA
         l1_sc_bcch_data_ptr->bcch.scheduled_status = 0;
#endif /* FEATURE_GPRS_GBTA */
         l1_sci_receive_done(gas_id);
         MSG_GERAN_HIGH_2_G("BCCH (ARFCN=%d) abandoned (%d tries)", l1_sc_bcch_data_ptr->bcch.arfcn.num, L1_SC_BCCH_MAX_TRIES);
      }
      else
      {
         /* Work out the next frame to try on */
         setup_next_bcch_fn(gl1_get_FN( gas_id ), FALSE, gas_id);

         {
           l1_sc_bcch_data_ptr->bcch.state = StateWaiting;
         }
      }
   }
   else if (l1_sc_bcch_data_ptr->bcch.state == StateAborting)
   {
      /* All SCE receives were aborted */

      l1_sc_bcch_data_ptr->bcch.state = StateNull;

      /* Tell the SCE we're done */
      l1_sc_abort_receive_callback(gas_id);
   }
   else
   {
      /* This shouldn't happen */
      l1_sc_bcch_data_ptr->bcch.state = StateNull;
      l1_sc_abort_receive_callback(gas_id);
      MSG_GERAN_ERROR_1_G("Unexpected abort callback %d", l1_sc_bcch_data_ptr->bcch.state);
   }
   l1_sc_bcch_data_ptr->ncell_bcch_in_progress = FALSE;

}
/*
 *     Function name :  next_bcch_fn
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Calculates the serving cell frame number corresponding to the next BCCH
 *     block on a neighbor cell.  This functions uses values stored in the
 *     bcch struct, specifically: fn_lag and mask.  These must be set to values
 *     corresponding to the neighbor cell in question.
 *
 *     Parameters :
 *     ---------------
 *     IN  - serv_fn      - The current serving cell frame number.
 *
 *     Returns :
 *     -----------
 *     The number of the serving cell frame on which to start the BCCH reception.
 *
 */

static uint32 next_bcch_fn( uint32 serv_fn, gas_id_t gas_id )
{
   uint32  fn;
   uint8   fn_mod_51;
   uint8   tc;
   uint16  mask = 0;
   l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);

   /* for idle mode update qbit drift */
   if( l1_sc_globals_ptr->mode == L1SCModeIdle )
   {
     if( l1_sc_apply_timing_drift(gas_id) )
     {
       l1_sc_bcch_data_ptr->bcch.fn_lag = l1_sc_globals_ptr->receiving_cell->bs.fn_lag;
       l1_sc_bcch_data_ptr->bcch.start_qbit = l1_sc_globals_ptr->receiving_cell->bs.qbit_lag;
       MSG_GERAN_HIGH_3_G("BCCH arfcn %d updated fn_lag %d qbit_lag %d",l1_sc_bcch_data_ptr->bcch.arfcn.num,l1_sc_bcch_data_ptr->bcch.fn_lag,l1_sc_bcch_data_ptr->bcch.start_qbit);
     }
   }

   /* Work out the ncell frame number */
   /* Add 2 if in gprs 52 mode */

   if (l1_sc_globals_ptr->mode == L1SCModeGprsTrans)
   {
     fn = FRAMES_PER_HYPERFRAME + serv_fn - l1_sc_bcch_data_ptr->bcch.fn_lag;
   }
   else
   {
      fn             = FRAMES_PER_HYPERFRAME + serv_fn - l1_sc_bcch_data_ptr->bcch.fn_lag + 1;
   }

   fn_mod_51      = fn % 51;
   tc             = (fn / 51) & 7;

   if (fn_mod_51==2)
   {
     /*we can still read ext bcch if we are at fnmod51 = 2 in transfer*/
     if((l1_sc_globals_ptr->mode == L1SCModeGprsTrans) && 
        (bcch_mask_ext[gas_id] == TRUE))
     {
       serv_fn  += 6 - fn_mod_51;
       fn_mod_51 = 6;
       
       l1_sc_bcch_data_ptr->bcch.returned_tc = tc + 8;
       mask			  = 0x0100 << tc;
       MSG_GERAN_MED_0_G("next_bcch_fn : fn_mod_51(2) ext bcch mask set");
     }
     else
     {
       /* don't want to return current serv_fn */
       mask = 0;
     }
   }
   else if(fn_mod_51==6)
   {
      /* don't want to return current serv_fn */
      mask = 0;
   }
   else if (fn_mod_51 < 2)
   {
      serv_fn  += 2 - fn_mod_51;
      fn_mod_51 = 2;

      l1_sc_bcch_data_ptr->bcch.returned_tc = tc;
      mask = 0x0001 << tc;
   }
   else if (fn_mod_51 < 6)
   {
      serv_fn  += 6 - fn_mod_51;
      fn_mod_51 = 6;

      l1_sc_bcch_data_ptr->bcch.returned_tc = tc + 8;
      mask             = 0x0100 << tc;
   }
   else if (fn_mod_51 > 6)
   {
      serv_fn  += 51 - fn_mod_51 + 2;
      fn_mod_51 = 2;
      tc        = ( tc + 1 ) & 7;

      l1_sc_bcch_data_ptr->bcch.returned_tc = tc;
      mask = 0x0001 << tc;
   }

   /**********************************************************************************/
   /* It could be possible that this function is called with the BCCH mask not setup.*/
   /* This will cause the final while loop below to loop forever, so to stop this    */
   /* possibiltiy then mask is set here and is defaulted to BCCH_MASK_3              */
   /**********************************************************************************/
   if ( l1_sc_bcch_data_ptr->bcch.mask == 0 )
   {
     MSG_GERAN_ERROR_1_G("BCCH MASK not setup l1_sc_bcch_data_ptr->bcch.mask=%d default to SI3",l1_sc_bcch_data_ptr->bcch.mask);
     l1_sc_bcch_data_ptr->bcch.mask = BCCH_MASK_3;
   }

   while ( ( l1_sc_bcch_data_ptr->bcch.mask & mask ) == 0 )
   {
      if ( fn_mod_51 == 2 )
      {
         serv_fn  += 4;
         fn_mod_51 =  6;

         l1_sc_bcch_data_ptr->bcch.returned_tc = tc + 8;
         mask             = 0x0100 << tc;

      }
      else if ( fn_mod_51 == 6 )
      {
         fn_mod_51 = 2;
         serv_fn  += 47;
         tc        = ( tc + 1 ) & 7;

         l1_sc_bcch_data_ptr->bcch.returned_tc = tc;
         mask = 0x0001 << tc;
      }
   }

   if ( serv_fn >= FRAMES_PER_HYPERFRAME )
   {
      serv_fn -= FRAMES_PER_HYPERFRAME;
   }
   return serv_fn;
}


static void setup_next_bcch_fn(uint32 serv_fn, boolean pbcch_flag, gas_id_t gas_id)
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
  uint32     frames_reqd = L1_SC_DURATION_NEW_BCCH;     

   if (!pbcch_flag)
   {
      l1_sc_bcch_data_ptr->bcch.start_fn = next_bcch_fn (serv_fn, gas_id );
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif
   }

   if(!gl1_msg_get_multi_sim_mode())
   {
    l1_sc_bcch_data_ptr->bcch.drx_ref = gl1_drx_require_tick (l1_sc_bcch_data_ptr->bcch.start_fn, gas_id);
   }

   l1_fm_clear_required_frames(FM_PRI_NCELL_BCCH, gas_id);
   l1_fm_set_required_frame_number( FM_PRI_NCELL_BCCH, l1_sc_bcch_data_ptr->bcch.start_fn, frames_reqd, gas_id);
}


uint32 gpl1_gprs_get_bcch_fn(uint32 fn_lag, int16 qbit_lag, uint16 bcch_mask, gas_id_t gas_id)
{
    uint32 bcch_fn;
    l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

    l1_sc_bcch_data_ptr->bcch.fn_lag = fn_lag;
    l1_sc_bcch_data_ptr->bcch.mask = bcch_mask;

    bcch_fn = next_bcch_fn( gl1_get_FN( gas_id ), gas_id );

    while (qbit_lag < 0)
    {
       qbit_lag += QS_PER_FRAME;
       bcch_fn = SUB_FN(bcch_fn, 1);
    }
    while (qbit_lag >= QS_PER_FRAME)
    {
        qbit_lag -= QS_PER_FRAME;
        bcch_fn = ADD_FN(bcch_fn, 1);
    }

    return bcch_fn;
}

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER

static void ncell_bcch_data_callback_gprs( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  dBx16_T snr_array[4];
  uint8 i;
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

  ASSERT_INT();
 
 #ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  if(l1_sc_bcch_data_ptr->bcch.chan_type == GL1_MSG_CCCH)
  {
    /* Log the metrics for PCH in transfer*/
    l1_log_message_metrics(GL1_MSG_CCCH, rpt, gas_id);
  }
  else
  {
    /* Log the ncell BCCH metrics */
    l1_log_ncell_bcch_msg_metrics(&rpt->ccch, gas_id);
  }
 #else
  /* Log the ncell BCCH metrics */
  l1_log_ncell_bcch_msg_metrics(&rpt->ccch, gas_id);
 #endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/
 
 
  l1_sc_bcch_data_ptr->bcch.success = rpt->ccch.hdr.good_data ? TRUE : FALSE ;
 
  l1_sc_bcch_data_ptr->bcch.data_done = TRUE;
  if (l1_sc_bcch_data_ptr->bcch.metrics_done)
  {
    l1_sc_bcch_data_ptr->bcch.state = StateNull;
	
    l1_sci_receive_done(gas_id);
  }
 
  if (l1_sc_bcch_data_ptr->bcch.chan_type == GL1_MSG_BCCH)
  {
    if ( rpt->ccch.hdr.good_data )
    {
      if(ARFCNS_EQUAL(gl1_get_serving_cell_arfcn(gas_id), l1_sc_bcch_data_ptr->bcch.arfcn))
      {
        MSG_GERAN_MED_3_G("BCCH Decoded: ARFCN=%d TC=%d FN:%d", l1_sc_bcch_data_ptr->bcch.arfcn.num, l1_sc_bcch_data_ptr->bcch.returned_tc, gl1_get_FN( gas_id ));
      }
      else
      {
        MSG_GERAN_MED_3_G("Ncell BCCH Decoded: ARFCN=%d TC=%d FN:%d", l1_sc_bcch_data_ptr->bcch.arfcn.num, l1_sc_bcch_data_ptr->bcch.returned_tc, gl1_get_FN( gas_id ));
      }
    }
    else
    {
      if(ARFCNS_EQUAL(gl1_get_serving_cell_arfcn(gas_id), l1_sc_bcch_data_ptr->bcch.arfcn))
      {
        MSG_GERAN_MED_3_G("BCCH Failed (ARFCN=%d, TC=%d, FN=%d)...", l1_sc_bcch_data_ptr->bcch.arfcn.num, l1_sc_bcch_data_ptr->bcch.returned_tc, gl1_get_FN( gas_id ));
      }
      else
      {
        MSG_GERAN_MED_3_G("Ncell BCCH Failed (ARFCN=%d, TC=%d, FN=%d)...", l1_sc_bcch_data_ptr->bcch.arfcn.num, l1_sc_bcch_data_ptr->bcch.returned_tc, gl1_get_FN( gas_id ));
      }
    }
  }
 
#ifdef FEATURE_GPRS_GBTA
  l1_sc_bcch_data_ptr->bcch.scheduled_status = rpt->ccch.hdr.scheduled_status;
#endif /* FEATURE_GPRS_GBTA */
 
  /*if metrics not yet logged*/
  if (l1_sc_bcch_data_ptr->bcch.metrics_done)
  {
    uint8 valid_bursts = 0;
    
    for(i = 0; i < 4 ;i++)
    {
    /* Output a debug message showing the SNRs */
      if(l1_sc_bcch_data_ptr->bcch.rx_rpt.metrics[i]->valid == TRUE)
      {
        snr_array[i] = gl1_msg_SNR_to_dBx16(l1_sc_bcch_data_ptr->bcch.rx_rpt.metrics[i]->snr);
        valid_bursts++;
      }   
      else
      {
         snr_array[i] = 0;
      }
    }
    
    l1_snr_msg( snr_array, valid_bursts, gas_id );
  }
 
  gpl1_gprs_restart_ncell_sch(gas_id);
  

}

static void ncell_bcch_metrics_callback_gprs( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  dBx16_T snr_array[4];
  uint8 i;
  uint8 valid_bursts = 0;

  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
  if(l1_sc_bcch_data_ptr->bcch.chan_type == GL1_MSG_CCCH)
  {
    /* Log the metrics */
    l1_log_burst_metrics(GL1_MSG_CCCH, rpt, gas_id);
  }
  else
  {
    /* Log the metrics */
    l1_log_ncell_bcch_burst_metrics(rpt, gas_id);
  }
#else
  /* Log the metrics */
  l1_log_ncell_bcch_burst_metrics(rpt, gas_id);
#endif


  l1_sc_bcch_data_ptr->bcch.metrics_done = TRUE;
  if (l1_sc_bcch_data_ptr->bcch.data_done)
  {
     l1_sc_bcch_data_ptr->bcch.state = StateNull;
     l1_sci_receive_done(gas_id);
  }
 
  for(i = 0; i < 4 ;i++)
  {
    /* Output a debug message showing the SNRs */
    if(rpt[i]->valid == TRUE) 
    {
      snr_array[i] = gl1_msg_SNR_to_dBx16(rpt[i]->snr);
      valid_bursts++;
    }
    else
     snr_array[i] = 0;
  }  
   
  l1_snr_msg( snr_array, valid_bursts, gas_id );
}

#else /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

static void ncell_bcch_data_callback_gprs( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   ASSERT_INT();

   /* Log the metrics */
   l1_log_ncell_bcch_msg_metrics(&rpt->ccch, gas_id);

   l1_sc_bcch_data_ptr->bcch.success = rpt->ccch.hdr.good_data ? TRUE : FALSE ;

   l1_sc_bcch_data_ptr->bcch.data_done = TRUE;
   if (l1_sc_bcch_data_ptr->bcch.metrics_done)
   {
      l1_sc_bcch_data_ptr->bcch.state = StateNull;
      l1_sci_receive_done(gas_id);
   }

   if ( rpt->ccch.hdr.good_data )
   {
      MSG_GERAN_MED_3_G("Ncell BCCH Decoded: ARFCN=%d TC=%d FN:%d", l1_sc_bcch_data_ptr->bcch.arfcn.num, l1_sc_bcch_data_ptr->bcch.returned_tc, gl1_get_FN( gas_id ));
   }
   else
   {
      MSG_GERAN_MED_3_G("Ncell BCCH Failed (ARFCN=%d, TC=%d FN=%d)...", l1_sc_bcch_data_ptr->bcch.arfcn.num, l1_sc_bcch_data_ptr->bcch.returned_tc, gl1_get_FN( gas_id ));
   }

#ifdef FEATURE_GPRS_GBTA
   l1_sc_bcch_data_ptr->bcch.scheduled_status = rpt->ccch.hdr.scheduled_status;
#endif /* FEATURE_GPRS_GBTA */

   gpl1_gprs_restart_ncell_sch(gas_id);

}

static void ncell_bcch_metrics_callback_gprs( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   /* Log the metrics */
   l1_log_ncell_bcch_burst_metrics(rpt, gas_id);

   l1_sc_bcch_data_ptr->bcch.metrics_done = TRUE;
   if (l1_sc_bcch_data_ptr->bcch.data_done)
   {
      l1_sc_bcch_data_ptr->bcch.state = StateNull;
      l1_sci_receive_done(gas_id);
   }
}
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/

void gpl1_gprs_sc_start_bcch_trans( cell_T* cell, gl1_msg_chan_type chan_type,async_ccch_type_t async_ccch_type,gas_id_t gas_id  )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   gl1_msg_rx_cb_type           rx_cb;
   int                          i;

   ASSERT_INT();

   if ( l1_sc_bcch_data_ptr->bcch.state != StateNull )
   {
      MSG_GERAN_ERROR_0_G("ncell BCCH already in progress");
   }

   /* Set up info needed for the reception/decoding */
   l1_sc_bcch_data_ptr->bcch.arfcn          = cell->bs.arfcn;
   l1_sc_bcch_data_ptr->bcch.bsic           = cell->bs.bsic;
   l1_sc_bcch_data_ptr->bcch.agc            = &cell->bs.agc;

   l1_sc_bcch_data_ptr->bcch.tries          = 0;
   l1_sc_bcch_data_ptr->bcch.mask           = cell->bcch_mask;
   l1_sc_bcch_data_ptr->bcch.fn_lag         = cell->bs.fn_lag;

   /* Clear the flags so we know when both callbacks have been called */
   l1_sc_bcch_data_ptr->bcch.data_done      = FALSE;
   l1_sc_bcch_data_ptr->bcch.metrics_done   = FALSE;
   l1_sc_bcch_data_ptr->bcch.chan_type       = chan_type;
   l1_sc_bcch_data_ptr->bcch.async_ccch_type = async_ccch_type;

   l1_sc_bcch_data_ptr->bcch.afc_info_ptr = &( cell->afc_info );

   /* Set up stuff required by the gl1_msg layer             */
   /* Some of this doesn't really need to be done every time */
   l1_sc_bcch_data_ptr->bcch.rx_rpt.data = &l1_sc_bcch_data_ptr->bcch.decode_rpt;
   for (i=0; i<4; i++)
   {
      l1_sc_bcch_data_ptr->bcch.rx_rpt.metrics[i] = &l1_sc_bcch_data_ptr->bcch.metrics_rpts[i];
   }

   rx_cb.data_cb    = ncell_bcch_data_callback_gprs;
   rx_cb.metrics_cb = ncell_bcch_metrics_callback_gprs;
   rx_cb.log_cb     = NULL;
   rx_cb.abort_cb   = ncell_bcch_abort_callback;

   gl1_msg_cfg_ncell_bcch( l1_sc_bcch_data_ptr->bcch.bsic & 0x07, TRUE, &rx_cb,gas_id );

   /* Ensure qbit_lag is in range (Note tha bcch_fn has already been adjusted in gpl1_gprs_get_bcch_fn */
   l1_sc_bcch_data_ptr->bcch.start_qbit = cell->bs.qbit_lag;
   while (l1_sc_bcch_data_ptr->bcch.start_qbit < 0)
   {
       l1_sc_bcch_data_ptr->bcch.start_qbit += QS_PER_FRAME;
   }
   while (l1_sc_bcch_data_ptr->bcch.start_qbit >= QS_PER_FRAME)
   {
       l1_sc_bcch_data_ptr->bcch.start_qbit -= QS_PER_FRAME;
   }

   MSG_GERAN_MED_2_G("neigh_fn bcch.fn_lag %d %d",(l1_sc_bcch_data_ptr->bcch.start_fn - l1_sc_bcch_data_ptr->bcch.fn_lag),l1_sc_bcch_data_ptr->bcch.fn_lag);

   l1_sc_bcch_data_ptr->bcch.state = StateInit;
}


void gpl1_gprs_sci_tick_bcch_trans( gas_id_t gas_id )
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

   if (l1_sc_bcch_data_ptr->bcch.state == StateInit)
   {
      int     i;
      ARFCN_T arfcns[4];

      for (i=0; i<4; i++)
      {
         arfcns[i] = l1_sc_bcch_data_ptr->bcch.arfcn;
      }

      l1_sc_bcch_data_ptr->bcch.start_fn = l1_scdrv_get_fn(gas_id);

      gl1_msg_rx_ncell_bcch ( arfcns, l1_sc_bcch_data_ptr->bcch.agc, 
                             (uint16)l1_sc_bcch_data_ptr->bcch.start_qbit, &l1_sc_bcch_data_ptr->bcch.rx_rpt,
                             l1_sc_bcch_data_ptr->bcch.chan_type,l1_sc_bcch_data_ptr->bcch.async_ccch_type,l1_sc_bcch_data_ptr->bcch.afc_info_ptr->freq_error ,gas_id );

      gl1_set_ncell_block_start(TRUE,gas_id);

      l1_sc_bcch_data_ptr->bcch.state = StateActive;
   }
   else
   {
      MSG_GERAN_ERROR_1_G("Unexpected call to gpl1_gprs_sci_tick_bcch_trans %d", l1_sc_bcch_data_ptr->bcch.state);
   }
}

/*===========================================================================

FUNCTION l1_sc_schedule_retry

DESCRIPTION
        Function used to schedule a retry of an ncell bcch attempt. This will
        be due to the failure of the current attempt.
PARAMS
DEPENDENCIES

RETURN VALUE None

===========================================================================*/
static void l1_sc_schedule_retry ( uint32 frame_no, gas_id_t gas_id )
{
    l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
    l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];
    idle_data_T *l1_idle_data = &l1_idle_data_store[gas_id];

    if (++l1_sc_bcch_data_ptr->bcch.tries == L1_SC_BCCH_MAX_TRIES)
    {
        /* We used up all our scheduling tries so give up. */
        l1_sc_bcch_data_ptr->bcch.state = StateNull;
        l1_sc_bcch_data_ptr->bcch.success = FALSE;
#ifdef FEATURE_GPRS_GBTA
        l1_sc_bcch_data_ptr->bcch.scheduled_status = 0;
#endif /* FEATURE_GPRS_GBTA */
        l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = FALSE;

        l1_sci_receive_done(gas_id);
        l1_fm_clear_required_frames(FM_PRI_NCELL_BCCH, gas_id);
        MSG_GERAN_HIGH_2_G("BCCH (ARFCN=%d) abandoned (%d tries)",
                l1_sc_bcch_data_ptr->bcch.arfcn.num, L1_SC_BCCH_MAX_TRIES);
    }
    else
    {
      if( l1_sc_bcch_data_ptr->bcch.tries > L1_SC_BCCH_COUNT_FOR_PRI_INV )
      {
        l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = TRUE;
        MSG_GERAN_HIGH_0_G("Use high priority for NBCCH decode");
      }
      /* Work out the next frame to try on */
      setup_next_bcch_fn( frame_no, FALSE, gas_id);
    }

    if ( FALSE != l1_scell_afc_info_ptr->valid )
    {
       /* Reset the afc info for the serving cell */
       MSG_GERAN_ERROR_2_G("afc freq %d restored for scell %d",
                           l1_scell_afc_info_ptr->afc_freq,
                           l1_idle_data->campedon_cell_ARFCN.num);

       /* Initialise the AFC algorithm back to scell value */
       gl1_msg_acq_afc_init(l1_idle_data->campedon_cell_ARFCN, gas_id);

       gl1_msg_set_afc_info(l1_scell_afc_info_ptr->afc_freq, gas_id);

       l1_scell_afc_info_ptr->valid = FALSE;
    }
    else
    {
        MSG_GERAN_MED_0_G("No scell afc info to restore");
    }
}
boolean l1_sc_bcch_use_high_priority ( gas_id_t gas_id)
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];
  return l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority;
}

void l1_sc_bcch_reset_high_priority_flag ( gas_id_t gas_id)
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

  l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = FALSE;
}

void l1_sc_bcch_set_high_priority_flag ( gas_id_t gas_id)
{
  l1_sc_bcch_data_t *l1_sc_bcch_data_ptr = &l1_sc_bcch_data[gas_id];

  l1_sc_bcch_data_ptr->bcch.nbcch_use_high_priority = TRUE;
}

/* 
 *     --------------------------------- 
 *     Function name :  gl1_set_bcch_drx_ref
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Sets the drx_ref in the BCCH Data Structure
 *
 *     Parameters :
 *     ---------------
 *     drx_ref, gas_id
 *
 *     Returns :
 *     -----------
 *     None
 *
 */
void gl1_set_bcch_drx_ref(uint8 drx_ref, gas_id_t gas_id)
{
    l1_sc_bcch_data[gas_id].bcch.drx_ref = drx_ref;
    MSG_GERAN_MED_1_G("gl1_set_bcch_drx_ref: set to %d", drx_ref);
}
/* 
 *     --------------------------------- 
 *     Function name :  gl1_get_bcch_drx_ref
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Get the drx_ref in the BCCH Data Structure
 *
 *     Parameters :
 *     ---------------
 *     gas_id
 *
 *     Returns :
 *     -----------
 *     None
 *
 */
uint8 gl1_get_bcch_drx_ref(gas_id_t gas_id)
{
  return l1_sc_bcch_data[gas_id].bcch.drx_ref;
}

