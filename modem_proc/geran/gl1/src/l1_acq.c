/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                      L 1   A C Q U I S I T I O N

GENERAL DESCRIPTION
   This module contains the procedures to acquire the cell timing.  It
   initiates an acquisition (FCCH tone detection and SCH decode) and
   waits for the specified timeout periods (in Frames) until the FCCH
   and SCH are acquired.  This procedure executes in the ISR context and
   signals the L1 task of any status.

EXTERNALIZED FUNCTIONS
  L1_initial_acq
    Procedure to acquire the base station timing.

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_acq.c#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
09/04/18   sk        CR2217923 To limit the CR2212372 fix to Blind HO scenarios
23/02/17   ks        CR2009320 Do not restart Acquisition or bcch decode if W2G BPLMN search is suspended
31/01/18   rv        CR2019406 Ensure BPLMN is correctly suspensed for SSIM Acquisition
05/01/16   sk        CR896794 Ensure GL1 doesnt get stuck in NULL mode when list search gets aborted in ACQ_WAIT_STOP state
05/05/15   cs        CR832756 Ensure correct start FN always used for acquisition
13/07/15   ab        CR832798 : FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
22/05/15   ws        CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
29/05/15   am        CR843362 Remove CR823102
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
30/04/15   am        CR830972 Use individual subs for storing afc
30/04/15   og        CR823102. Set the afc freq before decoding every BCCH radio block.
12/12/14   cs        CR754853 Add support for Parallel SCH detection handling
09/12/14   am        CR768292 Ensure new ACQ doesn't start while in suspending state
12/04/14   am        CR765903 Ensure bplmn is aborted in suspending state
07/09/14   js        CR711466:Reset the main command after drx off in W2G handover.
09/04/14   am        CR639289 Report SNR in MPH_FCCH_SCH_DECODE_IND to GRR
04/04/14   cah       CR643764 VSTMR multi-sim restruct and FEATURE_MSIM_VSTMR removal
29/11/13   gk        CR551547:Go to the next arfcn if we see fcch missed and suspended the bplmn search
06/09/13   sjw       CR467545 - Pass updated RXLEV to GRR in FCCH_SCH_DECODE_IND
15/07/13   sp        CR508123: Update panic reset to use multi-sim data space
21/06/13   cs        TSTS Updates
01/04/13   cja       CR448231 Correct CR398356 for trans mode NCell SCH outside gap
31/03/12   ws        CR466973 - Triton DSDS BU: Dual dataspace start_ACQ_FN
24/08/11   kb        CR302696 - Added Initialization function to initialize acq data values.
29/01/12   ss        CR398356  Adjust qbit lag and fn lag when qbit lag is negetive or greater than 5000QS
28/11/12   br        CR383165 array intailsed properly to avoid compilation warnings
15/05/11   jj        CR357981 Abort aquicition earlier if panic reset triggered
14/06/11   cja       CR264690 Acquisition frequency capture range larger & multipass for XO
16/05/11   ap        CR 287985 - L1 stuck in FIND_BCCH mode during DL UL data transfer.
26/01/11   nt        CR272659 - do not send select_specific_cnf(false) if a retry is going to be attempted.
22/03/11   ap        CR279430 DSDS Timing value wraps casusing frame misalignment
27/04/10   cs        CR236460 Latest AFC\XO handling updates
08/02/10   cja       XO update
09-10-22   tjw       avoid compiler and lint warnings about unused variables when MSG
                     is defined away to nothing
29/05/09   ps        Zero Initialise memory relocation to Heap for selected data structure
02/10/08   cs        Lint fixes
16/09/08   cs        Add an extra frame delay when the sweep gives an offset of zero
                     to ensure we always have a fixed delay from SCH to start of measures
28/08/08   cs        Implement the Fast Rx Sweep functionality for FTM
27/06/08   nt        CR147984 - REV2:- Final changes for TCXO reset
16/06/08   nt        CR147984:- Changes for TCXO reset.
17/01/08   cs        Lint fixes
05/07/07   dv        CR122548: Initialise c0_agc with RSSI from FB/SB detection
26/04/07   ws        CR 114617 - Added L1_send_MPH_FCCH_SCH_DECODE_IND for UOOS
06/12/06   og        Fix pch block decode failures due to interruption by BPLMN
                     search. Fixes CR107369.
12/10/06   og        Merge fix to CR101667 / SR860781. Ensure the correct freq err
                     is applied before attenpting to decode BCCH after pscan.
01/16/06   sv        Fix CR85766 - UE reset while Stability testing for Abort specific
                     BPLMN testing
01/05/06   sv        Set the main command to SUSPEND_COMMAND only when suspending for GSM
                     and not when stopping GSM mode.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
08/17/05   gfr       Use gl1_msg_init_frame_tick_info to init the ftn structure
07/29/05   gfr       Support for NPL1 DTM interface changes
02/25/05   pp        Added support for GSM to GSM Blind Handovers
02/05/05   bm        Background PLMN search optimizations
10/01/04   bm        Enhanced Parallel BCCH decode support
11/01/04   yhong     Changed NCell Abort API to take a parameter
06/12/04   gfr       Control slew rate limiting in PL1.
08/20/04   dp        Fix to prevent race condition when aborting background
                     PLMN search when its scheduled in the task but the ISR
                     has not yet run
06/02/04   gw        Allow L1 to handle MPH_DECODE_BCCH_LIST_REQ after getting
                     MPH_SELECT_SPECIFIC_BCCH_REQ.
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters
                     for better code readability.
04/05/04   dp        Fixed bug in background HPLMN aborting
04/02/04   gw        Removed some lint errors.
03/31/04   dp        Fixed compiler warnings
03/03/04   gw        Initialize fcch_sch_ilde to TRUE and call abort function
                     only if fcch_sch_idle == TRUE.
02/25/04   gw        Lint clean up.
02/19/04   gfr       Fix for running without background HPLMN searching.
02/17/04   gfr       Inform drivers if this is the first acq in a series.
02/04/03   dp        Added support for background HPLMN searching
01/08/04   gw        Make sure frame tick notification is called when aborting.
12/17/03   gfr       Fix for sleeping too early after acquisition.
11/05/03   gfr       Support for quad-band.
16/07/03   kf        modified ncell pbcch reading.
07/11/03   gfr       Lint cleanup.
04/11/03   ATM       Moved Frame Number management entirely inside GTMRS
11/22/02   mk        Added multiband AFC support
11/04/02   ATM       Added more debug for CM handovers
10/30/02   dp        Fix to abort immediately if leaving this mode.  Don't
                     run through the state machine and reset any state
                     variables which might have been set by the task
10/22/02   dp        Fixed local_sch_FN computation in all callbacks to
                     account for the 2 frame delay introduced by acq AGC in
                     the acq handler.  Fixes acq failures in acq AGC build
10/17/02   JC        Added acquisition AGC support.
10/01/02   ATM       Added central get and set FN routines
09/10/02   ATM       BUG:Set AGC pointer correctly
08/27/02   ATM       BUG:Inverted FN change
08/13/02   dp        Added support for parallel acq/BCCH decodes
07/10/02   ATM       Added blind-handover support.
06/14/02   mk        Added call to gl1_msg_reset() to support AGC.
05/21/02   mk        Changed L1_ACQ_END state to handle only ACQ and BCCH
                     decode commands.
03/28/02   JC        Removed GSM_DEBUG macros.
02/03/05   JC        Added initial RxPWRx16 for debugging purposes (F3 screen)
02/27/20   gw        Added code to set current_timeslot = 0 on successful acq.
02/15/02   mk        Updated afc/tt.
02/14/02   gw        Added support for L1_SCAN_COMMAND in L1_initial_acq().
02/05/02   JC        Modified L1_initial_acq interface for AGC.
01/24/02   ATM       Added calls to l1_change_state() for logging
12/18/01   dp        Added support to use the handler for ncell acquisition
12/13/01   gw        Changed acquisition to use message/frame layer API.
12/10/01   thh       Hw_*() functions are obsolete.  Use the new interface
                     gl1_hw*() functions.
                     Removed redundant RF function calls.
09/25/01   mk        Deleted L1_get_bcch().
08/14/01   JC        Removal of KOVU compile flag and general cleanup.
05/14/01   JC        Debug variable cleanup.
05/07/01   jc        Add support for mdsp firmware 000300(PLT0300).
04/26/01   jc        Add support for mdsp firmware 010202(PLT02020) which uses
                     a new definition of time offset when SCH is decoded.
04/02/01   JC        Initial version (Sina emulaton board)

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Alphabetical Order */
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "gl1_msg_g.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"    /* for l1_idle_data */
#include "l1_utils.h"   /* for l1_state_change */
#include "msg.h"
#include "rr_l2.h"      /* For HANDOVER_IND reason */
#include "gl1_sys_params.h"
#include "l1_sc.h"

#include "geran_dual_sim.h"
#include "l1i.h"

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

extern boolean gl1_is_ok_to_increment_sch_index_during_abort (gas_id_t gas_id);

extern dedicated_data_T l1_dedicated_data[];

static void fcch_sch_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void interrat_acq_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
void fcch_sch_abort_callback(  gas_id_t gas_id );

static void gsm_blind_ho_acq_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void setup_target_cell_details( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );

static uint32 L1_acq_get_fn_offset( gl1_msg_decode_rpt *rpt, gas_id_t gas_id  );

typedef struct
{
 boolean                  fcch_sch_idle;
 gl1_msg_rx_cb_type       rx_cb;
 sys_algo_agc_T          *agc_ptr;
 gl1_msg_decode_rpt       acq_decode_data_rpt;
 gl1_msg_rx_rpt           acq_rpt;
 boolean                  start_acq_handler;
}l1_acq_data_t;

static l1_acq_data_t  l1_acq_data[NUM_GERAN_DATA_SPACES];

extern void l1_set_fcch_sch_state(boolean fcch_sch_state, gas_id_t gas_id );
boolean bcch_list_fcch_sch_running[NUM_GERAN_DATA_SPACES] =
           { INITIAL_VALUE( FALSE ) };

boolean last_acq_was_list = FALSE;

void decode_bcch_list_acq_abort_callback( gas_id_t gas_id );
extern void L1_background_HPLMN_search_suspend_cb( gas_id_t gas_id );
extern void L1_acq_list_init( uint8 state, gas_id_t gas_id );

extern void gl1_begin_INTERRAT_Handover( gas_id_t gas_id );
extern void gl1_begin_gsm_blind_handover( boolean acq_success, gas_id_t gas_id );

#undef DEBUG_GSM_BLIND_HANDOVER
#ifdef DEBUG_GSM_BLIND_HANDOVER
#error code not present
#endif

/*===========================================================================

FUNCTION init_l1_acq_data

DESCRIPTION
             Initializes acq data values.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/

void init_l1_acq_data( gas_id_t gas_id )
{
  int j;
  memset ( &l1_acq_data[gas_id], 0 , sizeof ( l1_acq_data_t ) );
  l1_acq_data[gas_id].fcch_sch_idle = TRUE;
  l1_acq_data[gas_id].agc_ptr = NULL;
  l1_acq_data[gas_id].acq_rpt.data = &l1_acq_data[gas_id].acq_decode_data_rpt;
  for(j = 0; j < GL1_DEFS_FRAMES_IN_BLK_PERIOD; j++)
  {
    l1_acq_data[gas_id].acq_rpt.metrics[j] = NULL;
  }
  l1_acq_data[gas_id].start_acq_handler = FALSE;
}

/*===========================================================================

FUNCTION  L1_INITIAL_ACQ

DESCRIPTION
  This function finds the next  Frequency burst on the given carrier,
  then looks for a synch burst.  If this succeeeds, the system Frame Number
  is established.

DEPENDENCIES
  Cmd_Blk.sub_sub_state =  state

  Cmd_Blk.p             = pointer to AGC structure
  Cmd_Blk.parameter1    = number of frames the ISR has to wait for an FCB
  Cmd_Blk.parameter2    = number of frames the ISR has to wait for an SCH
  Cmd_Blk.parameter5    = ARFCN

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean L1_initial_acq ( gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_acq_data_t *l1_acq_data_ptr = &l1_acq_data[gas_id];
   BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];

   gl1_msg_frame_tick_type         ftn;
   boolean                         leaving_acq_mode;

   leaving_acq_mode = ( l1_tskisr_blk->main_command != L1_NULL_COMMAND );

   /* If we need to abort, abort and return right away */
   if ( leaving_acq_mode ||panic_reset_occured[gas_id])
   {
       if (!l1_acq_data_ptr->fcch_sch_idle)
       {
          /* Abort ACQ */
          gl1_msg_abort_rx_ncell_fcch_sch(DELAYED_ABORT, gas_id);
       }

       gl1_msg_init_frame_tick_info(&ftn);
       ftn.next_frame_is_acq_ncell_block_start = FALSE;
       gl1_msg_frame_tick_notification ( &ftn,gas_id);

       return ( l1_acq_data_ptr->fcch_sch_idle );
   }

   switch ( l1_tskisr_blk->sub_sub_state )
   {
      case L1_ACQ_INITIALISE:
      case L1_ACQ_INITIALISE_FOR_INTERRAT:
      case L1_ACQ_INITIALISE_GSM_BLIND_HO:

         /* Configure the message layer */
         gl1_msg_reset(gas_id);

         l1_acq_data_ptr->rx_cb.metrics_cb = NULL;  /* not used */
         l1_acq_data_ptr->rx_cb.log_cb     = NULL;
         l1_acq_data_ptr->rx_cb.abort_cb   = fcch_sch_abort_callback;

         /* Register the callback with the message layer */
         if( L1_ACQ_INITIALISE == l1_tskisr_blk->sub_sub_state)
         {
             last_acq_was_list = FALSE;
             l1_acq_data_ptr->agc_ptr           = l1_tskisr_blk->current_params.L1Data.pAgc_data;
             l1_acq_data_ptr->rx_cb.data_cb     = fcch_sch_callback;
             gl1_msg_cfg_fcch_sch( &l1_acq_data_ptr->rx_cb, gas_id );
         }
         else if( L1_ACQ_INITIALISE_FOR_INTERRAT == l1_tskisr_blk->sub_sub_state)
         {
             last_acq_was_list = FALSE;
             l1_acq_data_ptr->agc_ptr           = l1_tskisr_blk->current_params.L1Data.pAgc_data;
             l1_acq_data_ptr->rx_cb.data_cb     = interrat_acq_callback;
             gl1_msg_cfg_fcch_sch( &l1_acq_data_ptr->rx_cb , gas_id);
         }
         else if( L1_ACQ_INITIALISE_GSM_BLIND_HO == l1_tskisr_blk->sub_sub_state)
         {
             last_acq_was_list = FALSE;
             l1_acq_data_ptr->agc_ptr           = l1_tskisr_blk->current_params.L1Data.pAgc_data;
             l1_acq_data_ptr->rx_cb.data_cb     = gsm_blind_ho_acq_callback;
             gl1_msg_cfg_fcch_sch( &l1_acq_data_ptr->rx_cb, gas_id );
         }

         /* Init afc algorithm */
         gl1_msg_acq_afc_init( l1_tskisr_blk->current_params.ARFCN,gas_id );

         /* Tell the message layer to start trying to acquire */
         gl1_msg_rx_fcch_sch( l1_tskisr_blk->current_params.ARFCN,   /* ARFCN          */
                              l1_acq_data_ptr->agc_ptr,                           /* AGC Control    */
                              0,                                 /* offset         */
                              l1_tskisr_blk->current_params.frames_for_FCBdetect,  /* FCCH frames    */
                              l1_tskisr_blk->current_params.frames_for_SCHdecode,  /* SCH frames     */
                              l1_tskisr_blk->current_params.max_SCH_attempts,  /* SCH attempts   */
                              TRUE,                              /* do AFC         */
                              TRUE,                              /* do AGC         */
                              FALSE,                             /* early_callback */
                              &l1_acq_data_ptr->acq_rpt,         /* report         */
                              l1_tskisr_blk->current_params.bsic_expected, /* expected BSIC */
                              gas_id);                           /* subscription id */

         l1_tskisr_blk->sub_sub_state = L1_ACQ_START;
         l1_acq_data_ptr->fcch_sch_idle = FALSE;

         /* Initialise afc informaion */
         l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].valid = FALSE;
         l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].word1 = 0;

         break;

      case L1_ACQ_START:

         MSG_GERAN_MED_2_G("ACQ Started, ARFCN=%d, RxPWRx16=%d",
                 l1_tskisr_blk->current_params.ARFCN.num,
                 (dBx16_T)l1_acq_data_ptr->agc_ptr->pwr_dBm_x16);

         l1_acq_data_ptr->start_acq_handler = TRUE;
         l1_tskisr_blk->sub_sub_state = L1_ACQ_WAITING;
         break;

      case L1_ACQ_WAITING:
         l1_acq_data_ptr->start_acq_handler = FALSE;
         /* Wait while the message layer does its thing. */
         /* We'll get a callback when its done.          */
         break;

      case L1_ACQ_END:
      {
         /* If we abort before getting to L1_ACQ_WAITING */
         l1_acq_data_ptr->start_acq_handler = FALSE;
         /* Await further commands */

         if ( l1_tskisr_blk->command == L1_ACQUIRE_COMMAND)
         {
            l1_tskisr_blk->command       = L1_NULL_COMMAND;
            l1_tskisr_blk->sub_state     = L1_ACQUIRING;
            l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE;
            L1_transfer_new_data( gas_id );
         }
         else if ( l1_tskisr_blk->command == L1_ACQUIRE_DURING_RESELECTION_COMMAND)
         {
            l1_tskisr_blk->command       = L1_NULL_COMMAND;
            l1_tskisr_blk->sub_state     = L1_ACQUIRING_DURING_RESELECTION;
            l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE;
            L1_transfer_new_data( gas_id );
         }
         else if ( l1_tskisr_blk->command == L1_ACQUIRE_BCCH_LIST_COMMAND)
         {
            l1_tskisr_blk->command       = L1_NULL_COMMAND;
            l1_tskisr_blk->sub_state     = L1_ACQUIRING;
            l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE_BCCH_LIST;
            L1_transfer_new_data( gas_id );
         }
         else if ( l1_tskisr_blk->command == L1_DECODE_BCCH_COMMAND)
         {
            l1_tskisr_blk->command = L1_NULL_COMMAND;
            l1_tskisr_blk->sub_state = L1_GETTING_BCCH;
            l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
            L1_transfer_new_data( gas_id );
         }
         else if ( l1_tskisr_blk->command == L1_DECODE_BCCH_LIST_COMMAND)
         {
            l1_tskisr_blk->command = L1_NULL_COMMAND;
            l1_tskisr_blk->sub_state = L1_GETTING_BCCH_LIST;
            //l1_tskisr_blk->sub_sub_state = L1_BCCH_LIST_START;
            l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;
            L1_transfer_new_data( gas_id );
         }
         else if ( l1_tskisr_blk->command == L1_NULL_COMMAND)
         {
            MSG_GERAN_LOW_0_G("L1_NULL_COMMAND: Background HPLMN search");
         }
         else
         {
            MSG_GERAN_ERROR_1_G("Unexpected command %d", l1_tskisr_blk->command);
         }

      }
      break;

      default:
         MSG_GERAN_ERROR_1_G("Unexpected state %d", l1_tskisr_blk->sub_sub_state);
         break;
   }

   gl1_msg_init_frame_tick_info(&ftn);
   ftn.next_frame_is_acq_ncell_block_start = l1_acq_data_ptr->start_acq_handler;
   gl1_msg_frame_tick_notification ( &ftn,gas_id);

   return(l1_acq_data_ptr->fcch_sch_idle);
}

/*===========================================================================
HELPER FUNCTION  get_bsic_from_sch_data

DESCRIPTION
  This function extracts the Base Station Identity Code (6 bits)
  from the sch_data (4 octets).

RETURN VALUE
  byte - bsic (bit 5-3=PLMN, 2-0=BS)

SIDE EFFECTS
  None

===========================================================================*/
uint8 get_BSIC_from_sch_data( const uint8 *sch_data )
{
    /* ---------------------------------------------------------------------*/
    /* DESCRIPTION:                                                         */
    /* The SCH contains the BSIC and RFN.  Get the T1, T2, and T3' counters */
    /* from the SCH frame and calculate the timebase counter FN.            */
    /* This should be subsequently incremented when TN changes from 7 to 0. */
    /*                                                                      */
    /* REF: (GSM 05.02, v. 4.10.1, 3.3.2.3), (GSM 05.10, v. 5.2.1, 4.0)     */
    /* ---------------------------------------------------------------------*/
    return ((sch_data[0] & 0xfc) >> 2);
}

/*===========================================================================
HELPER FUNCTION  get_RFN_from_sch_data

DESCRIPTION
  This function extracts and calculates the Reduced Frame Number (19 bits)
  from the sch_data (4 octets).

RETURN VALUE
  uint32 - Reduced Frame Number

SIDE EFFECTS
  None

===========================================================================*/
uint32 get_RFN_from_sch_data( const uint8 *sch_data )
{
    uint32    true_sch_FN;
    uint16    T1, T2, T3_prime, T3;

    /* ---------------------------------------------------------------------*/
    /* DESCRIPTION:                                                         */
    /* The SCH contains the BSIC and RFN.  Get the T1, T2, and T3' counters */
    /* from the SCH frame and calculate the timebase counter FN.            */
    /* This should be subsequently incremented when TN changes from 7 to 0. */
    /*                                                                      */
    /* REF: (GSM 05.02, v. 4.10.1, 3.3.2.3), (GSM 05.10, v. 5.2.1, 4.0)     */
    /* ---------------------------------------------------------------------*/

    /* FN = Number of 51-multiframes elapsed + current 51 multi-frame count + superframes elapsed */
    /* NOTE: It is possible to have the situation where T2=25 and T3=0 which will cause the first */
    /* component to result in a negative number like (-25%26).  I think that CCWW adds 26 to T3   */
    /* to handle this case.  Therefore, instead of: FN = 51*((T3-T2)%26) + T3 + (51*26*T1);       */
    /* Also, CCWW adds 1                                                                          */
    /* REF: (GSM 05.10, v. 5.2.1, 4.0)                                                            */

    /* Calculate the correct frame number for the SCH burst */
    T1            = ((sch_data[0] & 0x03) << 9) |   /* superframe    */
                    ((sch_data[1] & 0xff) << 1) |
                    ((sch_data[2] & 0x80) >> 7);

    T2            =  (sch_data[2] & 0x7c) >> 2;     /* 26 multiframe */

    T3_prime      = ((sch_data[2] & 0x03) << 1) |
                     (sch_data[3] & 0x01);

    T3            = (uint16)(10 * T3_prime + 1);    /* 51 multiframe */

    true_sch_FN   = 51L*((26+T3 - T2) % 26) +       /* Number of 51-multiframes */
                    T3                      +       /* Index into 51-multiframe */
                    (51L *26L *T1);                 /* superframe component     */

    return true_sch_FN;
}

/*===========================================================================

FUNCTION  fcch_sch_callback

DESCRIPTION
  This function is called when the message layer FCCH/SCH receive function
  completes.  If an SCH burst was received the MS's FN and timing offset
  are adjusted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void fcch_sch_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_acq_data_t *l1_acq_data_ptr = &l1_acq_data[gas_id];
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];

   if (rpt->acq.success)
   {
      /* Acquisition was successful */
      uint8     BSIC=0;
      uint32    local_sch_FN, true_sch_FN, FN_diff;

      /* Calculate the MS FN where the SCH burst was detected */
      local_sch_FN = L1_acq_get_fn_offset( rpt, gas_id );

      /* Calculate the correct frame number for the SCH burst */
      true_sch_FN  = get_RFN_from_sch_data( rpt->acq.sch_data );

      /* Work out the difference between the local frame number and the true */
      /* frame number.  Use the difference to adjust the local FN.           */
      /* The frame number variables are unsigned so we have to take care of  */
      /* the sign of the difference explicitly.  This is done in the macros  */
      /* ADD_FN and SUB_FN.)                                                 */
      FN_diff = SUB_FN( true_sch_FN, local_sch_FN );
      GSTMR_ADVANCE_FN_GERAN( FN_diff, gas_id );

      /* Now adjust the offset into the frame */
      rpt->acq.time_offset %= QS_PER_FRAME;  /* remaining qbits after FN adjustment */

      gl1_msg_correct_timing( rpt->acq.time_offset,gas_id );

      l1_set_current_timeslot(0, gas_id);

      /* Signal the ISR state machine to change states */
      l1_tskisr_blk->sub_sub_state = L1_ACQ_END;

      /* Tell the L1 task we succeeded */
      L1_send_ISR_SCH_CNF(TRUE, rpt->acq.sch_data, gas_id);

      BSIC = get_BSIC_from_sch_data( rpt->acq.sch_data );
      MSG_GERAN_MED_3_G("SCH Decoded, ARFCN=%d NCC=%d BCC=%d",
              l1_tskisr_blk->current_params.ARFCN.num, (BSIC>>3), (BSIC & 0x07)); /*lint -esym(550, BSIC) suppress 'BSIC' not accessed */
      /* avoid compiler and lint warnings about unused variables when MSG
         is defined away to nothing */
      (void)BSIC;

      /* Save afc information here for use later in BCCH decode under high freq err
       * conditions.
       */
      l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].valid = TRUE;
      l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].word1 = rpt->acq.afc_freq;

      MSG_GERAN_MED_3_G( "Saved ARFCN %d afc_freq %dXO in index %d",
               l1_tskisr_blk->current_params.ARFCN.num,
               l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].word1,
               l1_tskisr_blk->current_params.ARFCN_index );  /*lint !e778*/

      if( l1_BCCH_List_ptr->arfcn[ l1_tskisr_blk->current_params.ARFCN_index ].num != l1_tskisr_blk->current_params.ARFCN.num )
      {
          MSG_GERAN_HIGH_0_G( "Not equal arfcns !");
      }
   }
   else
   {
      /* Reset the afc information since no valid ones exist to be saved */
      l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].valid = FALSE;
      l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].word1 = 0;

      /* Couldn't acquire on this ARFCN */
      /* Tell the L1 task if this is the second time around that has failed ?*/
      if ( l1_tskisr_blk->command == L1_NULL_COMMAND )
      {
        L1_send_ISR_SCH_CNF(FALSE, NULL, gas_id);
      }
      else
      {
        MSG_GERAN_ERROR_1_G( "ACQ failure - reattempting %d",l1_tskisr_blk->command);
      }


      /* Get ready to try agian */
      l1_tskisr_blk->sub_sub_state = L1_ACQ_END;
   }

   l1_acq_data_ptr->fcch_sch_idle = TRUE;
}

/*===========================================================================

FUNCTION  interrat_acq_callback

DESCRIPTION
  This function is called when the message layer FCCH/SCH receive function
  completes.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void interrat_acq_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
  l1_acq_data_t *l1_acq_data_ptr = &l1_acq_data[gas_id];
  Frequency_List_T *l1_Frequency_List = l1_Frequency_List_Store[gas_id];

   if (rpt->acq.success)
   {
      /* Acquisition was successful */
      setup_target_cell_details(rpt, gas_id);

      /* Populate camped on cell details */
      l1_idle_data->campedon_cell_ARFCN     = l1_Frequency_List->ARFCN[l1_Frequency_List->length];
      l1_idle_data->campedon_cell_agc       = l1_Frequency_List->agc[l1_Frequency_List->length];
      sys_algo_agc_enable_slew_rate_limiting(&l1_idle_data->campedon_cell_agc);

      /* Transfer the AGC information from IDLE to DEDICATED */
      l1_dedicated_data_ptr->c0_agc = l1_idle_data->campedon_cell_agc;

      /* We should intialise c0_agc to what has been measured during the
       * FB/SB detection as this is the most recent and therefore accurate
       * value.
       */
      l1_dedicated_data_ptr->c0_agc.pwr_dBm_x16 = l1_dedicated_data_ptr->target_cell_details.rx_power;

      sys_algo_agc_enable_slew_rate_limiting(&l1_dedicated_data_ptr->c0_agc);

      l1_tskisr_blk->acq_for_blind_ho = TRUE;

      gl1_begin_INTERRAT_Handover(gas_id);
      l1_tskisr_blk->sub_state = L1_DED_SUSPEND;
      l1_tskisr_blk->main_command = L1_NULL_COMMAND;
      l1_tskisr_blk->command = L1_NULL_COMMAND;
   }
   else
   {  /* Couldn't acquire on this ARFCN */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
      if( gl1_msg_get_multi_sim_mode() )
      {
        l1_change_state( L1_MULTI_SIM_NULL, gas_id );
      }
      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
      {
        /* Wait in NULL mode for STOP_GSM_MODE command */
        l1_change_state( L1_NULL_MODE, gas_id );
      }

      l1_tskisr_blk->main_command = L1_NULL_COMMAND;

      /* Tell RR we failed */
      L1_send_MPH_HANDOVER_IND( OTHER_ERROR , gas_id);
   }

   l1_acq_data_ptr->fcch_sch_idle = TRUE;
}

/*===========================================================================

FUNCTION  gsm_blind_ho_acq_callback

DESCRIPTION
  This function is called when the message layer FCCH/SCH receive function
  completes.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void gsm_blind_ho_acq_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  l1_acq_data_t *l1_acq_data_ptr = &l1_acq_data[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   if (rpt->acq.success)
   {
      /* Acquisition was successful */
      setup_target_cell_details(rpt, gas_id);
      l1_tskisr_blk->acq_for_blind_ho = TRUE;
   }

   gl1_begin_gsm_blind_handover( rpt->acq.success, gas_id );

   l1_acq_data_ptr->fcch_sch_idle = TRUE;
}

/*===========================================================================

FUNCTION  setup_target_cell_details

DESCRIPTION
  This function is called after a successful FCCH/SCH acquisition. It
  adjusts the FN and Qbit lags and populates the target cell details.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void setup_target_cell_details( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   dedicated_data_T *l1_dedicated_data_ptr = &l1_dedicated_data[gas_id];
   uint8     BSIC=0;
   uint32    local_sch_FN, true_sch_FN, FN_lag;
   sint15    QB_lag;

   /* Calculate the MS FN where the SCH burst was detected */
   local_sch_FN = L1_acq_get_fn_offset( rpt, gas_id );

   /* Calculate the correct frame number for the SCH burst */
   true_sch_FN  = get_RFN_from_sch_data( rpt->acq.sch_data );

   /* Work out the difference between the local frame number and the true */
   /* frame number.  Use the difference to adjust the local FN.           */
   /* The frame number variables are unsigned so we have to take care of  */
   /* the sign of the difference explicitly.  This is done in the macros  */
   /* ADD_FN and SUB_FN.)                                                 */
   /* NOTE: For L1_sync_to_target, the FN lag is subtracted instead of    */
   /*       added so we will store (-FN_diff)                             */
   FN_lag = SUB_FN( local_sch_FN, true_sch_FN );

   /* Now adjust the offset into the frame */
   QB_lag = ( rpt->acq.time_offset % QS_PER_FRAME );  /* remaining qbits after FN adjustment */

   /* Also get the BSIC */
   BSIC = get_BSIC_from_sch_data( rpt->acq.sch_data );
   MSG_GERAN_HIGH_3_G("SCH Decoded, ARFCN=%d NCC=%d BCC=%d",
            l1_tskisr_blk->current_params.ARFCN.num, (BSIC>>3), (BSIC & 0x07));
   MSG_GERAN_HIGH_2_G("FN_LAG=%d QB_LAG=%d", FN_lag, QB_lag);
#ifdef DEBUG_GSM_BLIND_HANDOVER
   #error code not present
#endif

   /* Populate target_cell_details */
   l1_dedicated_data_ptr->target_cell_details.arfcn      = l1_tskisr_blk->current_params.ARFCN;
   l1_dedicated_data_ptr->target_cell_details.bsic_known = TRUE;
   l1_dedicated_data_ptr->target_cell_details.bsic       = BSIC;
   l1_dedicated_data_ptr->target_cell_details.fn_lag     = FN_lag;
   l1_dedicated_data_ptr->target_cell_details.qbit_lag   = QB_lag;
   l1_dedicated_data_ptr->target_cell_details.rx_power   = rpt->acq.pwr_dBm_x16;
}

/*===========================================================================

FUNCTION  decode_bcch_list_acq_bcch_callback

DESCRIPTION
  This function is called when the message layer FCCH/SCH receive function
  completes.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void decode_bcch_list_acq_bcch_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];

   if (rpt->acq.success)
   {
      /* Acquisition was successful */
      uint8     BSIC=0;
      uint32    local_sch_FN, true_sch_FN, FN_diff;
      int32     qbit_lag;

      /* Calculate the MS FN where the SCH burst was detected */
      local_sch_FN = L1_acq_get_fn_offset( rpt, gas_id );

      /* Calculate the correct frame number for the SCH burst */
      true_sch_FN  = get_RFN_from_sch_data( rpt->acq.sch_data );

      /* Work out the difference between the local frame number and the true */
      /* frame number.  Use the difference to adjust the local FN.           */
      /* The frame number variables are unsigned so we have to take care of  */
      /* the sign of the difference explicitly.  This is done in the macros  */
      /* ADD_FN and SUB_FN.)                                                 */
      //FN_diff = SUB_FN( true_sch_FN, local_sch_FN );
      FN_diff = SUB_FN( local_sch_FN, true_sch_FN );
      //FN      = ADD_FN( FN, FN_diff );

      /* Now work out the offset into the frame.  (Always >= 0.) */
      qbit_lag = ( rpt->acq.time_offset % QS_PER_FRAME );  /* remaining qbits after FN adjustment */

      /*  Adjust FN lag and qbit lag if qbit lag is negetive*/
      if ( qbit_lag < 0 )
      {
         MSG_HIGH("Cell ARFCN num =  %d ; qbit_lag %d (< 0) ; fn_lag = %d", (l1_BCCH_List_ptr->arfcn->num), (l1_BCCH_List_ptr->qbit_lag[l1_BCCH_List_ptr->current_sch_index]), (l1_BCCH_List_ptr->fn_lag[ l1_BCCH_List_ptr->current_sch_index ]));
         qbit_lag += QS_PER_FRAME;
         FN_diff = SUB_FN ( FN_diff, 1 );
      }

      /*
         If this is not set, l1_initial_acq will start over again after
         it transitions to ACQ_END.  This way it will wait till the
         task moves it to another state
      */

      /* 
         Do not overwrite the command if main command is not NULL COMMAND, 
         which means we are aborting the list decode, 
         and updating here could lead to L1 getting stuck in NULL mode
      */
      if ( l1_tskisr_blk->main_command == L1_NULL_COMMAND )
      {
        l1_tskisr_blk->command = L1_NULL_COMMAND;
      }
      else
      {
        MSG_GERAN_HIGH_2_G( "Not updating the command here, main_command: %d, command : %d", l1_tskisr_blk->main_command, l1_tskisr_blk->command );
      }

      BSIC = get_BSIC_from_sch_data( rpt->acq.sch_data );

      /* Tell the L1 task we succeeded */

      /* If acquisition was successful, store FN and qbit lags */
      l1_BCCH_List_ptr->sch_decoded[ l1_BCCH_List_ptr->current_sch_index ] = TRUE;
      l1_BCCH_List_ptr->qbit_lag[ l1_BCCH_List_ptr->current_sch_index ] = (uint16)qbit_lag;
      l1_BCCH_List_ptr->fn_lag[ l1_BCCH_List_ptr->current_sch_index ] = FN_diff;
      l1_BCCH_List_ptr->bsic[ l1_BCCH_List_ptr->current_sch_index ] = BSIC;
      l1_BCCH_List_ptr->num_valid_carriers++;

      /*Once an SCH has been sucessfully decoded then this flag is set and is used
      in the BCCH_LIST_CNF to indicate confidence along with the RGS values.*/
      l1_BCCH_List_ptr->sch_found = TRUE;

      MSG_GERAN_MED_4_G( "SCH Decoded, ARFCN=%d FN_diff=%d BSIC=%d SNR %d",
               l1_tskisr_blk->current_params.ARFCN.num,
               FN_diff, BSIC, rpt->acq.snr );

      /* We have now decoded FCCH/SCH from ARFCN in BCCH list so inform GRR */
      L1_send_MPH_FCCH_SCH_DECODE_IND(rpt->acq.arfcn,
                                      BSIC,
                                      L1_map_dBm_x16_to_rxlev(rpt->acq.pwr_dBm_x16),
                                      rpt->acq.snr,
                                      gas_id);

      /* Save afc information here */
      l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].valid = TRUE;
      l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].word1 = rpt->acq.afc_freq;

      MSG_GERAN_MED_3_G( "Saved ARFCN %d afc_freq %dXO in index %d",
               l1_tskisr_blk->current_params.ARFCN.num,
               l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].word1,
               l1_tskisr_blk->current_params.ARFCN_index );

      if( l1_BCCH_List_ptr->arfcn[ l1_tskisr_blk->current_params.ARFCN_index ].num !=
            l1_tskisr_blk->current_params.ARFCN.num )
      {
        MSG_GERAN_HIGH_0_G( "Not equal arfcns !");
      }
   }
   else
   {
      /* Reset the afc information */
      l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].valid = FALSE;
      l1_BCCH_List_ptr->init_acq_afc[ l1_tskisr_blk->current_params.ARFCN_index ].word1 = 0;

      /* Couldn't acquire on this ARFCN */
      /* Tell the L1 task */
      l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_sch_index ] = TRUE; // This means this arfcn will never be attempted.

#ifdef  FEATURE_GSM_COEX_SW_CXM
      /* Trigger for Re- freq Reg */
      garb_intf_notify_find_bcch_decode_done(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_sch_index ],gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */
      
      L1_send_MPH_DECODE_BCCH_LIST_FAILURE_IND(
              l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_sch_index ],
              RR_L1_SCH_FAILED, gas_id );

   }

      /* Get ready to try agian */
   l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;

   l1_set_fcch_sch_state(FALSE, gas_id);

   bcch_list_fcch_sch_running[gas_id] = FALSE;

   l1_BCCH_List_ptr->current_sch_index++;

   if ( l1_BCCH_List_ptr->current_sch_index == l1_BCCH_List_ptr->length )
   {
      MSG_GERAN_MED_0_G("All parallel SCHs decoded");
   }

   if(( L1_IDLE_MODE == l1_tskisr_blk->l1_state )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            || ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    )
   {
       if ( l1_idle_data->background_HPLMN_search_activity != NO_HPLMN_SEARCH)
       {
         l1_idle_data->background_HPLMN_search_activity = ACQ_BCCH_LIST;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
         /* in case of in suspending state already, abort bplm so that a new acq doesn't start */
         if ( gpl1_get_multi_sim_bplmn_state(gas_id) == L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING )
         {
           /* don't schedule any more now, wait for BPLMN to get suspended */
           l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_WAITING;
         }
         else if ( !gl1_msg_get_multi_sim_mode() )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
         {
           /* don't schedule any more now, wait for BPLMN to get suspended */
           l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_WAITING;
           /* abort callback has not been called as the ACQ activity
            * was complete when hplmn was suspended hence data cb called. 
            * Ensure the HPLMN search is suspended 
            */ 
           L1_background_HPLMN_search_suspend_cb(gas_id);
         }
       }

       /* set the serving cell saved afc info */
       if( FALSE != l1_scell_afc_info_ptr->valid )
       {
         gl1_msg_acq_afc_init( l1_idle_data->campedon_cell_ARFCN,gas_id );

         gl1_msg_set_afc_info( l1_scell_afc_info_ptr->afc_freq,gas_id );

         MSG_GERAN_MED_3_G( "Restoring scell afc freq to %dXO ARFCN %d FN %d",
                  l1_scell_afc_info_ptr->afc_freq,
                  l1_idle_data->campedon_cell_ARFCN.num,
                  GSTMR_GET_FN_GERAN( gas_id) );

         l1_scell_afc_info_ptr->valid = FALSE;
       }

   }
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
    else if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
     {
        if (l1_get_bplmn_suspend_in_progress(gas_id))
        {
          MSG_GERAN_MED_0_G( "W2G BPLMN suspend in progress, don't schedule any more");
          /* don't schedule any more now, wait for BPLMN to get suspended */
          l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_WAITING;        
          l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
        }
     }
#endif
   else
   {
       /* (Re)-Initialize the SCE */
       l1_sc_init(gas_id);
   }
}



/*===========================================================================

FUNCTION  fcch_sch_abort_callback

DESCRIPTION
  This function is called when the message layer FCCH/SCH receive function
  completes.  If an SCH burst was received the MS's FN and timing offset
  are adjusted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void fcch_sch_abort_callback ( gas_id_t gas_id)
{
  l1_acq_data_t *l1_acq_data_ptr = &l1_acq_data[gas_id];
  MSG_GERAN_MED_0_G("ACQ abort");
  l1_acq_data_ptr->fcch_sch_idle = TRUE;
  bcch_list_fcch_sch_running[gas_id] = FALSE;
}

/*===========================================================================

FUNCTION  decode_bcch_list_abort_callback

DESCRIPTION
  This function is called when the message layer FCCH/SCH receive function
  completes.  If an SCH burst was received the MS's FN and timing offset
  are adjusted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void decode_bcch_list_acq_abort_callback(gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_acq_data_t *l1_acq_data_ptr = &l1_acq_data[gas_id];
  l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];

   MSG_GERAN_HIGH_0_G("ACQ list aborted ");

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
   if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
   {
     if (l1_get_bplmn_suspend_in_progress(gas_id))
     {
       l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
     }
   }
   else
#endif
   {
     L1_background_HPLMN_search_suspend_cb(gas_id);
   }

   l1_acq_data_ptr->fcch_sch_idle = TRUE;
   bcch_list_fcch_sch_running[gas_id] = FALSE;

   /* This is intended to avoid the re-start of FCCH decode on the same ARFCN after resumption,
    * in case the same has been attempted here and is missed due to exhaustion of number of allocated frames
    */
   if ( gl1_is_ok_to_increment_sch_index_during_abort(gas_id) )
   {
     BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
     l1_BCCH_List_ptr->current_sch_index++;
   }

   if( ( L1_IDLE_MODE == l1_tskisr_blk->l1_state )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
       || ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE )
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    )
   {
     /* Set the serving afc information */
     if( FALSE != l1_scell_afc_info_ptr->valid )
     {
       gl1_msg_acq_afc_init( l1_idle_data->campedon_cell_ARFCN,gas_id);

       gl1_msg_set_afc_info( l1_scell_afc_info_ptr->afc_freq,gas_id );

       MSG_GERAN_MED_3_G( "Restoring scell afc freq to %dXO ARFCN %d FN %d",
                l1_scell_afc_info_ptr->afc_freq,
                l1_idle_data->campedon_cell_ARFCN.num,
                GSTMR_GET_FN_GERAN( gas_id) );

       l1_scell_afc_info_ptr->valid = FALSE;
     }
   }
}

/*===========================================================================

FUNCTION  L1_acq_list_init

DESCRIPTION
  This function initializes the state variables with the passed in state and
  installs the right abort callbacks

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_acq_list_init( uint8 state, gas_id_t gas_id )
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_acq_data_t *l1_acq_data_ptr = &l1_acq_data[gas_id];
   l1_tskisr_blk->acq_bcch_state = state;
   /* Set this because if we suspend before we run L1_initial_acq the
    * abort_cb in NPL1 will point to the previous caller
    */
   l1_acq_data_ptr->rx_cb.abort_cb = decode_bcch_list_acq_abort_callback;

   gl1_msg_cfg_fcch_sch_list( &l1_acq_data_ptr->rx_cb, l1_tskisr_blk->current_params.ARFCN_index, gas_id );
}

/*===========================================================================

FUNCTION  L1_acq_get_fn_offset

DESCRIPTION
  This function calculates the FN offset for SCH from a given SCH report
  time offset in qsyms.

DEPENDENCIES
  None

RETURN VALUE
  FN offset based on start FN for ACQ state machine and returend report
  offset from firmware.

SIDE EFFECTS
  None

===========================================================================*/
static uint32 L1_acq_get_fn_offset(
                gl1_msg_decode_rpt *rpt,
                gas_id_t            gas_id  )
{
  /* Calculate the MS FN where the SCH burst was detected */
  uint32 offset_fn = (uint32)( rpt->acq.time_offset / QS_PER_FRAME );

  return ( ADD_FN( gl1_msg_get_start_acq_FN( gas_id ), offset_fn ) );
}

