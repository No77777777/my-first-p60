/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====him*

            L 1   B C C H  LIST  S C H E D U L I N G

GENERAL DESCRIPTION
   This module contains the procedures to schedule parallel BCCH
   message decodes for the valid carriers in the list sent down by RR
   This procedure executes in the L1 ISR context.

EXTERNALIZED FUNCTIONS
  L1_get_bcch_list
    Schedules asynchronous BCCH receives for the valid carriers in the
    list

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_bcch_list.c#1 $
  $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
23/12/16   sk        CR1104531 Increase the blocked_count for BCCH in case of BPLMN acquisition also
23/02/17   ks        CR2009320 Do not restart Acquisition or bcch decode if W2G BPLMN search is suspended
01/02/18   km        CR2092516 Removal of misleading or redundant prints
31/10/17   sp        CR2125524 Increase number of bcch attempts to 14 for EMBMS case.
31/08/17   br        CR2101248 Abort ongoing ACQ in advance so that BCCH is resumed at the right time during cell selction
23/01/17   ksb       CR1108276 reverting CR566562 and increasing the  gap check fro acq during bcch list decode
03/04/17   br        CR2027088 Band based ASDIV support during ACQ
08/12/16   ksb       CR1008155 Change TRM reason to CM during BCCH list decode after 2 attempts so that it has a better chance to get/retain lock when other SUB's idle activities are colliding with it
06/09/15   br        CR1063442 FR 38050: AsDiv in Acquisition for CSFB in multisimode.
03/11/15   ss        CR916273 FR 29852: AsDiv in Acquisition
27/04/16   sk        CR1005946 Debug prints added for the issue "BCCH decode fails for some cells on multimode sub" 
30/12/15   db        CR956257 MPH_DECODE_BCCH_LIST_FAILURE_IND is sent repeatedly for same ARFCN
10/09/15   nk        CR895944 Count number of BCCH attempts made correctly
11/09/15   sjw       CR905858 avoid high volume of logging for MPH_ACQUISITION_STATE_IND
01/09/15   am        CR899297 Abort ongoing ACQ in advance such that BCCH is resumed at the right time
26/08/15   ws        CR896163 - Fix compile errors for undefining FEATURE_DUAL_SIM
18/08/15   am        CR892241 Resume BCCH when sufficient frames are left
05/05/15   cs        CR832756 Ensure correct start FN always used for acquisition
16/05/15   br        CR871734 GL1 to retain the afc while aborting the bcch to avoid subsequnt bcch failurers on that cell.
30/04/15   am        CR830972 Use individual subs for storing afc
13/07/15   ab        CR832798 FR 26893: BPLMN\OOS - GL1 COEX : Register ARFCN/FREQID and Power Update 
                     during OOS ACQ/IDLE MONITORS
22/05/15   ws        CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
27/05/15   ggu       CR843786 When resume the BCCH list, should check whether l1_BCCH_List_ptr->prev_bcch_fn 
                     is a valid FN to increase the blocking count
14/05/15   am        CR824101 Abort ACQ and continue with pending BCCH in find_bcch state
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
07/04/15   ab        CR 812042:Register ARFCN and Report Power to MCS in Early Camping Stage/ Reselection for COEX 
31/03/15   am        CR814147 Don't skip acq abort in find_bcch state
25/03/15   ap        CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
01/14/15   dp        CR774267 Make bcch blocked count handling more robust
12/09/14   nm        CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
08/08/14   gk        CR665330 Correct the l1_bcch_list_data[gas_id].acq_rpt.data, when needed
06/08/14   df        CR536059 Allow Asynchronous sleep for all idle activities
18/11/13   sp        CR543075: BPLMN optimizations - to handle Si3/Si4 colliding with either sub pagings.
12/11/13   zc        CR566562 Enable the wait_abort in l1_bcch_list_acq_init if doing BPLMN
04/10/13   sp        CR553749 GERAN L1 Changes to MAX_ACQ_FRAMES
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
07/16/13   zc        CR470197 Replace memcpy with memscpy
15/07/13   sp        CR508123: Update panic reset to use multi-sim data space
06/06/13   cs        TSTS Updates
20/12/12   am/ap     CR432888 Adjust BPLMN frames for acqusition of BCCH in DSDS mode
31/03/12   ws        CR466973 - Triton DSDS BU: Dual dataspace start_ACQ_FN
01/02/12   sjw       CR329729 - Explicit indication to GRR if BCCH is blocked
24/08/11   kb        CR302696 - Added Initialization function to initialize bcch list data values
08/06/12   pa        GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
15/05/11   jj        CR357981 Abort aquicition earlier if panic reset triggered
10/02/12   ws        CR 336194 Enable TD-SCDMA IRAT for GERAN
13/01/12   nt        CR329799 - Extend the value of MAX_ACQ_FRAMES.
14/06/11   cja       CR264690 Add multipass for XO
02/03/11   njt       DSDS CR276931 - BPLMN suspension changes
18/11/10   cs        Fix F3 for XO on LIST_REQ restore
15/04/10   dmb       Add the ability to force SI3-only parallel BCCH decodes
10-03-16   tjw       Renamed UNUSED to __UNUSED_ARG to avoid name clash.
03/07/09   sjw       Support Extended BCCH parameter for l1_compute_tc()
26/02/09   cs        Extra debug info for XO support
22/01/09   ap        CR167961 Ensure that upon aborting a BCCH decode for a
                     background PLMN search the AFC frequencies are all adjusted
                     back to their original settings.
02/10/08   cs        Lint fixes
27/06/08   nt        CR147984 - REV2:- Final changes for TCXO reset
16/06/08   nt        CR147984:- Changes for TCXO reset.
23/01/08   og        Work-around for L1 stuck in BCCH issue when the decode list
                     is large. Resolves CR134559.
25/07/07   og        Compilation error fix.
17/07/07   og        Prevent bcch carriers from being blocked during W2G BPLMN and
                     IDLE HPLMN searches. Resolves CR123274 (SR949815).
15/06/07   og        Prevent ncell bcch reads from corrupting the PCH freq base
                     during BPLMN search. Fixes CR121587.
06/12/06   og        Fix pch block decode failures due to interruption by BPLMN
                     search. Fixes CR107369.
12/10/06   og        Merge fix to CR101667 / SR860781. Ensure the correct freq err
                     is applied before attenpting to decode BCCH after pscan.
27/04/06   og        Merge of change 317522 from the raven 3.2 branch.
                     Fix to CR 91668 / SR 812423. Ensure the NCELL BCCH band change
                     is updated to the lower layers.
01/16/06   sv        Fix CR85766 - UE reset while Stability testing for Abort specific
                     BPLMN testing
01/05/06   sv        Set the main command to SUSPEND_COMMAND only when suspending for GSM
                     and not when stopping GSM mode.
01/03/06   sv        Check for whether in WTOG BPLMN mode when sending BCCH_LIST_CNF.
                     This was causing ERR_FATAL scenario during WPLMN scan in 2G.
12/14/05   sv        Support the changed prototype for MPH_DECODE_BCCH_LIST_CNF.
11/16/05   ws        More Lint Cleanup
11/15/05   sv        Lint cleanup.
11/07/05   sv        Add support for FEATURE_WTOG_BACKGROUND_PLMN_SEARCH.
08/17/05   gfr       Use gl1_msg_init_frame_tick_info to init the ftn structure
05/26/05   gw        Fixed up some frame number comparisons.
04/13/05   bm        Schedule the earliest of SI3 or SI4 for BCCH list decodes
03/16/05   dp        Fixed data abort during parallel decodes of cells not
                     in pscan list
02/16/05   sv        Fixed "stuck in BCCH list abort" problem.
02/05/05   bm        Background PLMN search optimizations
01/26/05   sv        Add support for Frame Manager.
10/01/04   bm        Enhanced Parallel BCCH decode support
01/26/05   dp        Fix to issue NPL1 BCCH abort only if handler installed
11/01/04   yhong     Changed NCell Abort API to take a parameter
06/14/04   gfr       Log ncell BCCH metrics.
06/07/04   gw        Changed MAX_BCCH_ATTEMPTS from 2 to 1.
05/26/04   sv        Lint Cleanup.
04/02/04   gw        Removed some lint errors.
03/18/04   gsc       Lint clean up.
02/25/04   gw        Lint clean up.
02/19/04   gfr       Compile fixes for no background HPLMN.
02/09/04   dp        Support for background HPLMN searching
11/05/03   gfr       Support for quad-band.
08/27/03   dp        Fix to wait till BCCH block has been aborted before
                     returning.   Implemented bcch_list_abort_callback
01/24/03   atm       Logging cleanup (removed obsolete logging calls)
10/07/02   dp        Added support for fully parallel decodes
09/04/02   dp        Fixed race condition when we get a select specific bcch
                     req from RR at the same time we're just completing
                     parallel decodes
09/04/02   dp        Added qbit_lag to BCCH decode debug msg
09/04/02   plm       Added externs for L1_send_MPH_DECODE_BCCH_LIST_CNF() and
                     L1_send_MPH_DECODE_BCCH_LIST_FAILURE_IND() to eliminate
                     compiler warnings. Also removed unused declaration of mask
                     variable in next_SI3_fn().
08/22/02   dp        Printing out the TC when a BCCH decode succeeds or fails
07/31/01   dp        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#include "comdef.h"
#include "l1i.h"
#include "l1_isr.h"
#include "gl1_msg_g.h"
#include "gl1_sys_algo.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "msg.h"
#include "l1_log.h"
#include "l1_utils.h"    /* for l1_state_change */
#include "l1_sc.h"  /* For BCCH mask */
#include "rr_l1.h"
#include "rr_l1_g.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "l1_fm.h"
#include <stringl/stringl.h>

#include "gpl1_dual_sim.h"
#include "geran_dual_sim_g.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT */

#ifdef FEATURE_GSM_COEX_SW_CXM
#include "gl1_arbitrator_cxm.h"
#include "gl1_arbitrator_interface.h"
#endif /* FEATURE_GSM_COEX_SW_CXM */

#ifdef FEATURE_GL1_ASDIV_ACQ
#include "gl1_asdiv.h"
#endif

#define MAX_BCCH_ATTEMPTS 4
#define MAX_BCCH_ATTEMPTS_IN_EMBMS 14

extern void write_arfcns(ARFCN_T rx_arfcns[], ARFCN_T arfcn );

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/
typedef struct
{
  /* To indicate if a BCCH decode is in progress */
  int                      find_bcch_rx_in_progress;
  boolean                  find_bcch_abort_in_progress ;
  gl1_msg_metrics_rpt      bcch_metrics_rpt[4];
  gl1_msg_rx_cb_type       rx_cb;
  uint32                   strongest_arfcn_bcch_fn;
  uint16                   strongest_arfcn_index ;

  boolean                  fcch_sch_state;

  sys_algo_agc_T          *agc_ptr;
  gl1_msg_decode_rpt       acq_decode_data_rpt;
  gl1_msg_rx_rpt           acq_rpt ;
     /* Two sets of report arrays are declared out of paranoia      */
     /* regarding reception of back-to-back blocks (extended BCCH). */
  gl1_msg_decode_rpt       data_rpts[2];

  boolean                  start_acq_handler;
  uint32                   next_available_FN;
  uint16                   num_scheduling_frames;
}l1_bcch_list_data_t;

#ifdef FEATURE_GL1_ASDIV_ACQ
   /** Variable to Track if BCCH Power Mon. were configured in
    *  FT properly or Not, as if they failed to be configured we
    *  cant perform any power meas. */
   boolean asdiv_cfg = FALSE;
#endif


/* Prototype for local functions */
static void bcch_list_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void bcch_list_abort_callback( gas_id_t gas_id );
static void bcch_list_metrics_callback( gl1_msg_metrics_rpt *rpt[],gas_id_t gas_id );
static uint32 next_earliest_SI3_SI4_fn( uint32 serv_fn, uint32 fn_lag, boolean si3_only );

/*
   Don't schedule a weaker cell if its BCCH occurs within
   8 frames of a stronger cell.  6 frames for decode + results from DSP
   + a 2 frame margin
*/
#define MIN_INTER_BCCH_DECODE_SEPARATION 8

/*Define to ensure that there is enough time to squeeze an ACQ in before a BCCH */
#define MAX_ACQ_FRAMES 15

/*
   Max number of carriers that can be processed in the scheduling loop
   per frame without overruning the frame tick
*/
#define MAX_CARRIERS_PER_FRAME 124

extern void L1_suspend_background_HPLMN_search( gas_id_t gas_id );
extern void L1_background_HPLMN_search_suspend_cb( gas_id_t gas_id );
extern void l1_set_background_HPLMN_search_suspended(boolean background_HPLMN_search_suspended, gas_id_t gas_id);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
extern void l1_ds_set_bplmn_state_to_null(gas_id_t gas_id);
extern background_hplm_state_t l1_ds_get_bplmn_state(gas_id_t gas_id);
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

extern boolean bcch_list_fcch_sch_running[];
extern void decode_bcch_list_acq_abort_callback(gas_id_t gas_id);
extern void decode_bcch_list_acq_bcch_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
extern void fcch_sch_abort_callback( gas_id_t gas_id );
extern void L1_acq_list_init( uint8 state, gas_id_t gas_id );

static l1_bcch_list_data_t  l1_bcch_list_data[NUM_GERAN_DATA_SPACES];
static byte acq_suspend_timer[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( 0 ) };

void l1_set_fcch_sch_state(boolean fcch_sch_state, gas_id_t gas_id );

/*===========================================================================

FUNCTION init_l1_bcch_list_data

DESCRIPTION
             Initializes bcch list data values.

PARAMS
             None.

DEPENDENCIES

RETURN VALUE None

===========================================================================*/

void init_l1_bcch_list_data( gas_id_t gas_id )
{
  int j;

  /*Do a sanity check first */
  gas_id = check_gas_id(gas_id);

  memset ( &l1_bcch_list_data[gas_id], 0 , sizeof (l1_bcch_list_data_t) );
  l1_bcch_list_data[gas_id].find_bcch_abort_in_progress = FALSE;
  l1_bcch_list_data[gas_id].strongest_arfcn_bcch_fn = MAX_GSM_FRAME_NUMBER + 1;
  l1_bcch_list_data[gas_id].strongest_arfcn_index = RR_MAX_CANDIDATE_FREQ_LIST + 1;
  l1_bcch_list_data[gas_id].agc_ptr = NULL;
  l1_bcch_list_data[gas_id].acq_rpt.data = &l1_bcch_list_data[gas_id].acq_decode_data_rpt;
  for( j = 0; j < GL1_DEFS_FRAMES_IN_BLK_PERIOD; j++ )
  {
    l1_bcch_list_data[gas_id].acq_rpt.metrics[j] = NULL;
  }
  l1_bcch_list_data[gas_id].start_acq_handler = FALSE;

  l1_set_fcch_sch_state( FALSE, gas_id );
}

void l1_set_fcch_sch_state(boolean fcch_sch_state, gas_id_t gas_id )
{
  l1_bcch_list_data_t *l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];

  l1_bcch_list_data_ptr->fcch_sch_state = fcch_sch_state;
}

/*===========================================================================

FUNCTION  l1_bcch_list_trigger_acq_after_bcch

DESCRIPTION
  This function triggers reading of SCH, incase if we have time before BCCH
  decode or no BCCH decodes are possible

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_bcch_list_trigger_acq_after_bcch(gas_id_t gas_id)
{
    idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];

    MSG_GERAN_HIGH_0_G("Schedule SCH before next BCCH");

    if ( l1_BCCH_List_ptr->current_sch_index == l1_BCCH_List_ptr->length )
    {
       /* Should never get here */
       MSG_GERAN_ERROR_0_G("No SCHs to decode");
    }
    else
    {
       L1_send_ISRTIM_ACQ_LIST_REQ( l1_BCCH_List_ptr->current_sch_index, gas_id );
    }

    if (l1_tskisr_blk->l1_state == L1_IDLE_MODE
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            || l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    )
    {
        if ( l1_idle_data->background_HPLMN_search_activity != NO_HPLMN_SEARCH )
        {
           L1_transfer_new_data( gas_id );
           l1_idle_data->background_HPLMN_search_activity = ACQ_BCCH_LIST;
           L1_acq_list_init ( L1_BCCH_LIST_WAITING, gas_id );
         }
    }
}


/*===========================================================================

FUNCTION  l1_bcch_list_acq_init

DESCRIPTION
  This function inits ACQ

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void l1_bcch_list_acq_init(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
  l1_bcch_list_data_t *l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];
  l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];
 


   L1_transfer_new_data( gas_id );

   /* Configure the message layer */
   gl1_msg_reset(gas_id);
   l1_bcch_list_data_ptr->rx_cb.metrics_cb = NULL;  /* not used */
   l1_bcch_list_data_ptr->rx_cb.log_cb     = NULL;
   l1_bcch_list_data_ptr->rx_cb.abort_cb   = decode_bcch_list_acq_abort_callback;

   l1_set_fcch_sch_state( TRUE, gas_id );

   /* L1_ACQ_INITIALISE_BCCH_LIST */
   /* retrieve AGC ptr from BCCH list, else use default */
   /* This is done during background HPLMN searching because */
   /* l1_tskisr_blk->p gets overwritten in l1_idle_mode_ISR */

   l1_bcch_list_data_ptr->agc_ptr = l1_get_agc_ptr ( l1_tskisr_blk->current_params.ARFCN /* ARFCN */, gas_id );

   if (l1_bcch_list_data_ptr->agc_ptr == NULL)
   {
      l1_bcch_list_data_ptr->agc_ptr = &(l1_BCCH_List_ptr->agc[ l1_BCCH_List_ptr->current_sch_index ] );
      sys_algo_agc_init( l1_bcch_list_data_ptr->agc_ptr, SYS_PARAM_INIT_EXP_PWR_RX_LVL_dBm_x16 );
   }

   //bm change call back function
   l1_bcch_list_data_ptr->rx_cb.data_cb     = decode_bcch_list_acq_bcch_callback;

   gl1_msg_cfg_fcch_sch_list( &l1_bcch_list_data_ptr->rx_cb, l1_tskisr_blk->current_params.ARFCN_index ,gas_id);


   if(( L1_IDLE_MODE == l1_tskisr_blk->l1_state )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            ||(l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    )
   {
     /* Get and save the current serving cell freq base value */
     gl1_msg_get_afc_info( &(l1_scell_afc_info_ptr->afc_freq),gas_id );

     l1_scell_afc_info_ptr->valid = TRUE;

     MSG_GERAN_MED_2_G( "Saved scell afc_freq %dXO FN %d",
              l1_scell_afc_info_ptr->afc_freq, GSTMR_GET_FN_GERAN( gas_id));

   }

   /* Init afc algorithm */
   gl1_msg_acq_afc_init( l1_tskisr_blk->current_params.ARFCN,gas_id );

   /* At every Initial Acquisition setup the PDM, and rotator for the first time through*/
   /* to make sure they are setup, and account for the ageing */
   if ( l1_BCCH_List_ptr->current_sch_index == 0 )
   {
     gl1_msg_acq_tcxo_and_rotator_init(gas_id);
   }

  

   /* Do a sanity check */
   if (l1_bcch_list_data[gas_id].acq_rpt.data != &l1_bcch_list_data[gas_id].acq_decode_data_rpt)
   {
     l1_bcch_list_data[gas_id].acq_rpt.data = &l1_bcch_list_data[gas_id].acq_decode_data_rpt;
     MSG_GERAN_ERROR_0_G("BCCH list report data was pointing to incorrect location, and has been corrected");
   }

#ifdef FEATURE_GSM_COEX_SW_CXM
    garb_cxm_power_indication_arfcn(l1_tskisr_blk->current_params.ARFCN,
                                    (((l1_BCCH_List_ptr->rx_power_instant[ l1_BCCH_List_ptr->current_sch_index ])*10) >> 4),
                                    gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

   /* Tell the message layer to start trying to acquire */
   /* Removed the Acquisition AGC when doing BCCH list decodes */
   gl1_msg_rx_fcch_sch( l1_tskisr_blk->current_params.ARFCN,                /* ARFCN          */
                        l1_bcch_list_data_ptr->agc_ptr,                     /* AGC Control    */
                        0,                                                  /* offset         */
                        l1_tskisr_blk->current_params.frames_for_FCBdetect, /* FCCH frames    */
                        l1_tskisr_blk->current_params.frames_for_SCHdecode, /* SCH frames     */
                        l1_tskisr_blk->current_params.max_SCH_attempts,     /* SCH attempts   */
                        TRUE,                                               /* do AFC         */
                        FALSE,                                              /* do AGC         */
                        TRUE,                              /* early_callback */
                        &l1_bcch_list_data_ptr->acq_rpt,                    /* report         */
                        GSM_INVALID_BSIC, //l1_tskisr_blk->current_params.bsic_expected,        /* expected BSIC */
                        gas_id);                                            /* subscription id */

   bcch_list_fcch_sch_running[gas_id] = TRUE;

   MSG_GERAN_MED_1_G("ACQ Started, ARFCN=%d",
        l1_tskisr_blk->current_params.ARFCN.num);

   l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_WAITING;

}


/*===========================================================================

FUNCTION  L1_get_bcch_list_fcch_sch_bcch

DESCRIPTION
  This function reads the BCCH data off a carrier where the SCH has just
  been decoded. And also performs SCH Decodes

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean L1_get_bcch_list_fcch_sch_bcch( gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
  l1_bcch_list_data_t *l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];
  l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];

   /* Locals */
   boolean                      idle = FALSE;
   boolean                      leaving_bcch_mode;

   gl1_msg_rx_rpt               rpt;

   ARFCN_T                      ARFCNs[4];

   gl1_msg_frame_tick_type      ftn;
   /* This indicates the start of a block _we_want_to_receive_ */
   boolean                      dl_block_start;
   boolean                      scheduling_complete;
   uint8                        max_bcch_attempts = MAX_BCCH_ATTEMPTS;

   leaving_bcch_mode = ( l1_tskisr_blk->main_command != L1_NULL_COMMAND );

   if (leaving_bcch_mode|| panic_reset_occured[gas_id])
   {
       if (l1_bcch_list_data_ptr->fcch_sch_state)
       {
          if (bcch_list_fcch_sch_running[gas_id])
          {
             /* Abort ACQ */
             gl1_msg_abort_rx_ncell_fcch_sch(DELAYED_ABORT,gas_id);
          }

          gl1_msg_init_frame_tick_info(&ftn);
          gl1_msg_frame_tick_notification ( &ftn,gas_id);

          idle = !bcch_list_fcch_sch_running[gas_id];
       }
       else
       {
         /* Abort only once */
         if ( l1_bcch_list_data_ptr->find_bcch_abort_in_progress == FALSE )
         {
            l1_bcch_list_data_ptr->find_bcch_abort_in_progress = TRUE;
            /* Abort BCCH Rx */
            gl1_msg_abort_rx_ncell_bcch (DELAYED_ABORT,gas_id);
            MSG_GERAN_MED_0_G("FindBcchList abort");
         }

         gl1_msg_init_frame_tick_info(&ftn);
         gl1_msg_frame_tick_notification ( &ftn,gas_id);

         /* Return TRUE only if a BCCH block is not in progress */
         /* Otherwise wait till we clean up */
         if ( l1_bcch_list_data_ptr->find_bcch_rx_in_progress == 0 )
         {
            idle = TRUE;
            l1_bcch_list_data_ptr->find_bcch_abort_in_progress = FALSE;

         #ifdef DEBUG_BCCH_LIST_DECODE_FAILURES
            bcch_list_decode_check_done();
         #endif
         }
       }

      return idle;
   }

   /* Code */
   switch ( l1_tskisr_blk->acq_bcch_state )
   {
      case L1_BCCH_LIST_INIT:
         {
            uint32 local_FN;

            acq_suspend_timer[gas_id] = 0;

            l1_set_fcch_sch_state( FALSE, gas_id );

            l1_BCCH_List_ptr->current_bcch_index = 0;

            /*
              Compute the number of frames it will take to schedule all the
              ARFCNs.  Need to factor this in when computing next BCCH FN
              -1 because we fall through to next state, so we start from
              current frame.  Currently we can schedule 124 freqs in a single
              frame without overrunning the frame tick.  The framework is in
              place just in case we need to handle a larger number in future
            */

            if (  l1_BCCH_List_ptr->num_valid_carriers )
            {
               l1_bcch_list_data_ptr->num_scheduling_frames = ( ( l1_BCCH_List_ptr->num_valid_carriers - 1 )/MAX_CARRIERS_PER_FRAME );

            }
            else
            {
               l1_bcch_list_data_ptr->num_scheduling_frames = 0;
            }

            /* Next avail FN = current FN + frames used up for scheduling */
            local_FN          = gl1_get_FN( gas_id );
            l1_bcch_list_data_ptr->next_available_FN = ADD_FN( local_FN, l1_bcch_list_data_ptr->num_scheduling_frames );

            /* Default value - Invalid GSM frame number */
            l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn = MAX_GSM_FRAME_NUMBER + 1;
            l1_bcch_list_data_ptr->strongest_arfcn_index = RR_MAX_CANDIDATE_FREQ_LIST + 1;

            l1_BCCH_List_ptr->all_arfcns_done = TRUE;

            l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_START;
            if(gl1_msg_get_multi_sim_mode() && grm_is_embms_active(gas_id))
            {
              max_bcch_attempts = MAX_BCCH_ATTEMPTS_IN_EMBMS;
            }
         }
         /*lint -fallthrough */

      case L1_BCCH_LIST_START:
         {
            /*
               Start from the top of the sorted list (in decreasing order
               of power ) and pick the next ARFCN which has its SCH decoded
               and the BCCH not yet decoded (attempted).  If the SCH has not
               been decoded, just skip it - can't decode the BCCH
            */

            scheduling_complete = TRUE;

            for ( ;
                l1_BCCH_List_ptr->current_bcch_index < l1_BCCH_List_ptr->length;
                l1_BCCH_List_ptr->current_bcch_index++
                )
            {
               if ( !l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] &&
                   l1_BCCH_List_ptr->bcch_blocked_cnt[ l1_BCCH_List_ptr->current_bcch_index ] >= max_bcch_attempts)
              {
                MSG_GERAN_HIGH_2_G("Attempts %d done for ARFCN=%d, Giving up now",
                                    l1_BCCH_List_ptr->bcch_blocked_cnt[ l1_BCCH_List_ptr->current_bcch_index],
                                    l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ].num );

                /* Send MPH_DECODE_BCCH_FAILURE (BCCH blocked) */
                L1_send_MPH_DECODE_BCCH_LIST_FAILURE_IND(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],
                                                        RR_L1_BCCH_BLOCKED, gas_id );

                l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] = TRUE;
#ifdef  FEATURE_GSM_COEX_SW_CXM
                /* Trigger for Re-freq reg */
                garb_intf_notify_find_bcch_decode_done(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */
              }

               if ( !l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] &&
                    l1_BCCH_List_ptr->sch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] )
               {
                  l1_BCCH_List_ptr->all_arfcns_done = FALSE;

                  /* If starting from scratch, compute next BCCH FN using the FN
                     lag, otherwise once computed we just need to tack on 204 to
                     get the next BCCH FN
                  */
                  if ( l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] ==
                       MAX_GSM_FRAME_NUMBER + 1 )
                  {
                     /* Schedule the earliest of SI3 or SI4 */

                     l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] =
                     next_earliest_SI3_SI4_fn (
                                               l1_bcch_list_data_ptr->next_available_FN,
                                               l1_BCCH_List_ptr->fn_lag[ l1_BCCH_List_ptr->current_bcch_index ] ,
                                               l1_BCCH_List_ptr->si3_only
                                              );

                     MSG_GERAN_HIGH_3_G("ARFCN=%d, index=%d, Next BCCH FN=%d ",l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ].num, l1_BCCH_List_ptr->current_bcch_index, l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ]);
                  }
                  else
                  {

                     while ( IS_FRAME_NUM_LATER(l1_bcch_list_data_ptr->next_available_FN, l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ]) )
                     {

                        l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] =
                        next_earliest_SI3_SI4_fn (
                                       l1_bcch_list_data_ptr->next_available_FN,
                                       l1_BCCH_List_ptr->fn_lag[ l1_BCCH_List_ptr->current_bcch_index ],
                                       l1_BCCH_List_ptr->si3_only
                                       );
                     MSG_GERAN_HIGH_3_G("ARFCN=%d, index=%d, new Next BCCH FN=%d ",l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ].num, l1_BCCH_List_ptr->current_bcch_index, l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ]);
                     }
                  }
                  /*
                     If default value, this is the first arfcn in the list
                     and hence the strongest of the ones not yet decoded
                  */
                  if ( l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn == MAX_GSM_FRAME_NUMBER + 1 )
                  {

                     l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn = l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ];
                     l1_bcch_list_data_ptr->strongest_arfcn_index = l1_BCCH_List_ptr->current_bcch_index;

                     /* If we're within 8 frames of scheduling the strongest carrier,
                        go schedule it
                     */
                     if ( SUB_FN ( l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn, l1_bcch_list_data_ptr->next_available_FN ) <= ( MIN_INTER_BCCH_DECODE_SEPARATION + l1_bcch_list_data_ptr->num_scheduling_frames ) )
                     {
                        MSG_GERAN_HIGH_2_G("schedule strongest ARFCN index=%d, Next BCCH FN=%d",l1_BCCH_List_ptr->current_bcch_index, l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ]);
                        break;
                     }
                  }

                  /* Otherwise pick a carrier which has its BCCH coming earlier but
                     which is not within 8 frames of the strongest guy, so we
                     don't preempt it
                  */
                  if ( IS_FRAME_NUM_LATER(l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn, 
                                          l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ]))
                  {
                     if ( SUB_FN ( l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn,
                                   l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ]
                                 ) > ( MIN_INTER_BCCH_DECODE_SEPARATION + l1_bcch_list_data_ptr->num_scheduling_frames ) )
                     {
                        /* This becomes the new strongest arfcn */
                        l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn = l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ];
                        l1_bcch_list_data_ptr->strongest_arfcn_index = l1_BCCH_List_ptr->current_bcch_index;
                        //break;
                     }
                     MSG_GERAN_HIGH_2_G("strongest ARFCN index=%d, Next BCCH FN=%d", l1_bcch_list_data_ptr->strongest_arfcn_index , l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn); 
                  }
               }

               if ( ( l1_BCCH_List_ptr->current_bcch_index + 1 ) % MAX_CARRIERS_PER_FRAME == 0 )
               {
                  l1_BCCH_List_ptr->current_bcch_index++;
                  scheduling_complete = FALSE;
                  break;
               }
            }

            if ( l1_BCCH_List_ptr->current_bcch_index == l1_BCCH_List_ptr->length )
            {
               scheduling_complete = TRUE;
            }

            if (scheduling_complete )
            {
               /* We're done !! */
               /* Only if all SCHs have been attempted.  With parallel decodes,
                  all_arfcns_done could be TRUE if for e.g the first SCH was
                  not decoded..in that case there would be no BCCH to decode either
               */
               if ( l1_BCCH_List_ptr->all_arfcns_done &&
                    l1_BCCH_List_ptr->current_sch_index == l1_BCCH_List_ptr->length )
               {
                  /*
                     If we're aborting (for e.g due to a
                     MPH_SELECT_SPECIFIC_BCCH_REQ from RR), don't switch to NULL
                     mode, because it will override the state change to FIND_BCCH
                     and we will not go onto acquisition
                  */

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
                 if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
                 {
                     l1_set_wtog_bplmn_task_substate(WTOG_BPLMN_ACTIVE,gas_id);
                     l1_set_wtog_bplmn_task_state(NO_HPLMN_SEARCH,gas_id);
                 }
                 else
#endif
                 {
                    if ( l1_idle_data->background_HPLMN_search_activity !=  NO_HPLMN_SEARCH )
                    {
                       /* Suspend HPLMN search if decodes are complete */
                       L1_suspend_background_HPLMN_search(gas_id);
                       l1_idle_data->background_HPLMN_search_activity = NO_HPLMN_SEARCH;
                       MSG_GERAN_HIGH_0_G("NO_HPLMN_SEARCH");
                       /* Send MPH_DECODE_BCCH_LIST_CNF to RR */
                       L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE, gas_id);
                       MSG_GERAN_HIGH_0_G("Completed List BCCH decodes ");
                       return (TRUE);
                    }
                    else
                    {
                       /* Go to NULL mode and wait for RR to kick off something new */
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                      if( gl1_msg_get_multi_sim_mode() )
                      {
                       l1_change_state( L1_MULTI_SIM_NULL, gas_id );
                      }
                      else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
                      {
                        l1_change_state( L1_NULL_MODE, gas_id );
                      }

                      l1_tskisr_blk->main_command = L1_NULL_COMMAND;
                    }
                 }

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
                 if ((l1_get_drx_interval(gas_id) < MIN_FRAMES_FOR_L1_TO_SUSPEND) && (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE))
                 {
                     /* Send MPH_DECODE_BCCH_LIST_CNF to RR */
                     L1_send_MPH_DECODE_BCCH_LIST_CNF(TRUE,gas_id);
                     l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
                 }
                 else
#endif
                 {
                     /* Send MPH_DECODE_BCCH_LIST_CNF to RR */
                     L1_send_MPH_DECODE_BCCH_LIST_CNF(FALSE,gas_id);
                 }

                 MSG_GERAN_HIGH_0_G("Completed List BCCH decodes ");

                 break;
               }
               else
               {
                  if ( l1_bcch_list_data_ptr->strongest_arfcn_index == RR_MAX_CANDIDATE_FREQ_LIST + 1 )
                  {
                     if ( l1_BCCH_List_ptr->current_sch_index < l1_BCCH_List_ptr->length )
                     {
                         /* No BCCH to schedule - so start scheduling Acquisition*/
                         l1_bcch_list_trigger_acq_after_bcch(gas_id);
                         l1_bcch_list_acq_init(gas_id);

                         l1_bcch_list_data_ptr->start_acq_handler = TRUE;
                         break;

                     }
                     else
                     {
                        MSG_GERAN_ERROR_0_G("No SCH and BCCH to schedule!");
                     }
                     break;
                  }
                  else
                  {
                     l1_BCCH_List_ptr->current_bcch_index = l1_bcch_list_data_ptr->strongest_arfcn_index;

                     /*
                        If not time yet for BCCH, try another SCH decode if there is
                        an SCH to be decoded
                     */
                     if ( SUB_FN ( l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn, l1_bcch_list_data_ptr->next_available_FN ) > ( MAX_ACQ_FRAMES ) &&
                          l1_BCCH_List_ptr->current_sch_index < l1_BCCH_List_ptr->length )
                     {
                        /*
                           Next BCCH is way off - tell task to schedule SCH in the
                           meanwhile
                        */
                        MSG_GERAN_MED_3_G("Next BCCH %d at %d,scheduling SCH MAX_ACQ_FRAMES:%d"
                                ,l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ].num
                                ,l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ]
                                ,MAX_ACQ_FRAMES
                               );

                        /* No BCCH to schedule - so start scheduling Acquisition */
                        l1_bcch_list_trigger_acq_after_bcch(gas_id);
                        l1_bcch_list_acq_init(gas_id);

                        l1_bcch_list_data_ptr->start_acq_handler = TRUE;
                        break;
                     }
                     else
                     {
                        /* Configure the message layer */
                        gl1_msg_reset(gas_id);

                        l1_bcch_list_data_ptr->rx_cb.data_cb    = bcch_list_rx_callback;
                        l1_bcch_list_data_ptr->rx_cb.metrics_cb = bcch_list_metrics_callback;
                        l1_bcch_list_data_ptr->rx_cb.abort_cb   = bcch_list_abort_callback;
                        l1_bcch_list_data_ptr->rx_cb.log_cb     = NULL;

                        /* -------------------------------------------------------- */
                        /* DESCRIPTION: For broadcast channels the TSC = BCC        */
                        /* Ensure that TSC is valid by masking all but the 3 LSBs   */
                        /* REF: (GSM 05.02, v. 4.10.1, 5.2.3)                       */
                        /* -------------------------------------------------------- */
                        /* Using Ncell interface since it supports passing in an offset */
                        gl1_msg_cfg_ncell_bcch ( l1_BCCH_List_ptr->bsic[ l1_BCCH_List_ptr->current_bcch_index ] & 0x0007, TRUE, &l1_bcch_list_data_ptr->rx_cb,gas_id );
#ifdef FEATURE_GL1_ASDIV_ACQ
                        if (gl1_asdiv_acq_enabled(gas_id) && ((l1_tskisr_blk->l1_state == L1_FIND_BCCH) ||(l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH) )&&
							gl1_acq_asdiv_band_supported(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],gas_id))
                        {
                          /*Configure AsDiv Power meas. here if AsDiv is enabled*/
                          asdiv_cfg = gl1_msg_cfg_pwr_meas(gl1_asdiv_acq_bcch_mon_cb, gl1_asdiv_acq_bcch_mon_abort_cb, gas_id);
                          MSG_GERAN_HIGH_1_G("GL1_ASDIV: Bcch Monitors configured: %d", asdiv_cfg);
                        }
#endif /*FEATURE_GL1_ASDIV_ACQ*/

                        l1_bcch_list_data_ptr->find_bcch_rx_in_progress = 0;
                        l1_bcch_list_data_ptr->find_bcch_abort_in_progress = FALSE;

                        MSG_GERAN_MED_3_G("BCCH(%d) next_fn=%d,offset=%d",
                                l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ].num,
                                l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ],
                                l1_BCCH_List_ptr->qbit_lag[ l1_BCCH_List_ptr->current_bcch_index ]);
                        l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_RUNNING;

                       
                       MSG_GERAN_MED_3_G("BCCH(%d) next_fn=%d,attempt=%d",l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ].num,
                               l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ],
                               l1_BCCH_List_ptr->bcch_blocked_cnt[ l1_BCCH_List_ptr->current_bcch_index ]);

                       /* Save off last BCCH fn to check later if rescheduled */
                       l1_BCCH_List_ptr->prev_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] = l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ];

                     }
                  }
               }

            }
            else
            {
               break;
            }
         }
         /*lint -fallthrough */

      case L1_BCCH_LIST_RUNNING:

         dl_block_start = FALSE;

         if ( gl1_get_FN( gas_id ) == l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] )
         {
            dl_block_start = TRUE;
         }

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
          /* if  we are suspending to release trm, we don't want to schedule a BCCH read */
          if ( (l1_tskisr_blk->multi_sim_acq_bcch_list_state == L1_MULTI_SIM_ACQ_BCCH_LIST_SUSPENDING) ||
                  (l1_ds_get_bplmn_state(gas_id) == L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING)
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH            
               || l1_get_bplmn_suspend_in_progress(gas_id) 
#endif
)
          {
             dl_block_start = FALSE;
          }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
         if ( dl_block_start )
         {
            /* Fill up the parameter and report arrays */

            write_arfcns( ARFCNs,  l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ] );

            rpt.metrics[0] = &l1_bcch_list_data_ptr->bcch_metrics_rpt[0];
            rpt.metrics[1] = &l1_bcch_list_data_ptr->bcch_metrics_rpt[1];
            rpt.metrics[2] = &l1_bcch_list_data_ptr->bcch_metrics_rpt[2];
            rpt.metrics[3] = &l1_bcch_list_data_ptr->bcch_metrics_rpt[3];

            rpt.data = &l1_bcch_list_data_ptr->data_rpts[0];

            if(( L1_IDLE_MODE == l1_tskisr_blk->l1_state )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            ||(l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
            )
            {
              /* Get and save the current serving cell freq base value */
              gl1_msg_get_afc_info( &(l1_scell_afc_info_ptr->afc_freq),gas_id );

              l1_scell_afc_info_ptr->valid = TRUE;

              MSG_GERAN_MED_2_G( "Saved scell afc_freq %dXO FN %d",
                       l1_scell_afc_info_ptr->afc_freq,
                       GSTMR_GET_FN_GERAN( gas_id));
            }

            if ( gl1_is_asynch_sleep_enabled( gas_id ) )
            {
              /* For a PCH/BCCH setup the asynch int possible flag */
              l1_set_asynch_int_possible( gas_id );
            }

            /* Incase of band change between various NCELL BCCH attempts
             * Initialise the afc loop so that the Hz-to-PDM conversion factors are
             * updated to the firmware.
             * This prevents BCCH decode failures.
             */
            gl1_msg_acq_afc_init( ARFCNs[0],gas_id );

            /* Do we have afc info available? If yes then use it to overcome
             * high frequency error conditions to ensure correct BCCH decode
             * for this radio block.
             */
            if( FALSE != l1_BCCH_List_ptr->init_acq_afc[ l1_BCCH_List_ptr->current_bcch_index ].valid )
            {
                  MSG_GERAN_MED_3_G( "L1_BCCH_LIST_RUNNING: re-installing afc_freq %dXO for ARFCN %d FN %d",
                           l1_BCCH_List_ptr->init_acq_afc[l1_BCCH_List_ptr->current_bcch_index ].word1,
                           l1_BCCH_List_ptr->arfcn[l1_BCCH_List_ptr->current_bcch_index ].num,
                           GSTMR_GET_FN_GERAN( gas_id) );

                  gl1_msgi_reset_acq_afc( l1_BCCH_List_ptr->init_acq_afc[ l1_BCCH_List_ptr->current_bcch_index ].word1,gas_id );

            }

#ifdef FEATURE_GSM_COEX_SW_CXM
            garb_cxm_power_indication_arfcn(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],
                                            (((l1_BCCH_List_ptr->rx_power_instant[ l1_BCCH_List_ptr->current_bcch_index ])*10)>>4),gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

            if ( l1_get_agc_ptr ( l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ], gas_id ) )
            {
               gl1_msg_rx_ncell_bcch ( ARFCNs,
                                       l1_BCCH_List_ptr->agc_ptrs[ l1_BCCH_List_ptr->current_bcch_index ],
                                       l1_BCCH_List_ptr->qbit_lag[ l1_BCCH_List_ptr->current_bcch_index ],
                                       &rpt,GL1_MSG_BCCH,ASYNC_ACTIVITY_SUSPENSION,0,gas_id );

            }
            else
            {
               /*
                  If BCCH decode list freq is not in power scanlist,
                  use agc struct with default values
               */
               gl1_msg_rx_ncell_bcch ( ARFCNs,
                                       l1_BCCH_List_ptr->agc + l1_BCCH_List_ptr->current_bcch_index ,
                                       l1_BCCH_List_ptr->qbit_lag[ l1_BCCH_List_ptr->current_bcch_index ],
                                       &rpt,GL1_MSG_BCCH,ASYNC_ACTIVITY_SUSPENSION,0,gas_id );
            }
#ifdef FEATURE_GL1_ASDIV_ACQ
            if (gl1_asdiv_acq_enabled(gas_id) && ((l1_tskisr_blk->l1_state == L1_FIND_BCCH)||(l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH))&&
				gl1_acq_asdiv_band_supported(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],gas_id))
            {
              if (asdiv_cfg)
              {
                gl1_asdiv_acq_update_params(GL1_ASDIV_ACQ_EV_BCCH, ARFCNs[0], 0, 0,gas_id);
                MSG_GERAN_HIGH_0("asdiv: starting handler installation for asdiv");   
                gl1_asdiv_acq_do_pwr_meas (GL1_ASDIV_ACQ_EV_BCCH, gas_id);
              }
              else
              {
                MSG_GERAN_ERROR_0_G(GL1_ASDIV_HDR"AsDiv Power Meas. Config Failed");
              }
            }
#endif /*FEATURE_GL1_ASDIV_ACQ*/

            l1_bcch_list_data_ptr->find_bcch_rx_in_progress++;
         }
         else if(  ( 0 == l1_bcch_list_data_ptr->find_bcch_rx_in_progress )
                 &&( FALSE == l1_bcch_list_data_ptr->find_bcch_abort_in_progress )
                )
         {
            /* We need to do a check on whether we've missed
             * the scheduling frame for the BCCH block read.
             *
             * Start by getting the current frame number.
             */
            const uint32  current_fn = GSTMR_GET_FN_GERAN( gas_id );

            /* Use the macro to check if current fn is later
             * than the BCCH schedule FN.
             */
            if( FALSE != IS_FRAME_NUM_LATER( current_fn,
                                             l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] ) )
            {
               /* We missed the scheduling chance so fall back
                * to BCCH LIST INIT state to attempt recovery.
                */
               l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
               /* if in dual sim and we are suspending here go to L1_BCCH_LIST_WAITING rather than allowing
               a restart which could begin another decode */
               if ( gl1_msg_get_multi_sim_mode() &&
                    ((l1_tskisr_blk->multi_sim_acq_bcch_list_state == L1_MULTI_SIM_ACQ_BCCH_LIST_SUSPENDING) ||
                    (l1_ds_get_bplmn_state(gas_id) == L1_MULTI_SIM_BGRND_HPLMN_SUSPENDING)
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH            
                  || l1_get_bplmn_suspend_in_progress(gas_id) 
#endif
                 )
                  )
               {
                 l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_WAITING;
               }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

               /* Print out F3 debug warning messages */
               MSG_GERAN_ERROR_3_G(" arfcn %d missed bcch fn %d on fn %d ",
                         l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ].num,
                         l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ],
                         current_fn );

               MSG_GERAN_ERROR_3_G(" index %d l size %d ",
                         l1_BCCH_List_ptr->current_bcch_index,
                         l1_BCCH_List_ptr->length,
                         0 );
            }
         }

         gl1_msg_init_frame_tick_info(&ftn);
         ftn.next_frame_is_acq_ncell_block_start = dl_block_start;
#ifdef FEATURE_GL1_ASDIV_ACQ
         if (asdiv_cfg) 
         {
             ftn.next_frame_is_pwr_meas_block_start = dl_block_start; 
         }
#endif
         gl1_msg_frame_tick_notification ( &ftn,gas_id );
         break;

      case L1_BCCH_LIST_WAITING:
         {
            l1_bcch_list_data_ptr->start_acq_handler = FALSE;

            if ( l1_tskisr_blk->command == L1_ACQUIRE_COMMAND)
            {
               l1_tskisr_blk->command       = L1_NULL_COMMAND;
               l1_tskisr_blk->sub_state     = L1_ACQUIRING;
               l1_tskisr_blk->sub_sub_state = L1_ACQ_INITIALISE;
               L1_transfer_new_data( gas_id );
            }
            else if ( l1_tskisr_blk->command == L1_ACQUIRE_BCCH_LIST_COMMAND)
            {
                //Don't have to do anything
            }
            else if ( l1_tskisr_blk->command == L1_DECODE_BCCH_COMMAND)
            {
               l1_tskisr_blk->command = L1_NULL_COMMAND;
               l1_tskisr_blk->sub_state = L1_GETTING_BCCH;
               l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
               L1_transfer_new_data( gas_id );
            }
            else if ( l1_tskisr_blk->command == L1_NULL_COMMAND)
            {
               MSG_GERAN_LOW_0_G("L1_NULL_COMMAND: Background HPLMN search");

               if ( l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn != MAX_GSM_FRAME_NUMBER + 1 ) 
               {
                 uint32 suspension_delay = ACQ_SUSPENSION_FRAME_DELAY + 1;

                 /* for W to G BPLMN abort 1 frame early as acq completion is delayed by 1 frame in this state */
                 if( l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE )
                 {
                   suspension_delay++;
                 }

                 /* ACQ has not finished within MAX_ACQ_FRAMES frames, 
                  * if BCCH frame is approaching and ACQ has not finished then abort ACQ
                  * so that BCCH reading is not delayed by rescheduling
                  */
                 if ( gl1_get_FN(gas_id) == SUB_FN(l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn,suspension_delay) )
                 {
                   boolean abort_acq = gl1_msg_getting_sch(gas_id);

                   if ( L1_background_HPLMN_search_in_progress(gas_id)
                        && ( grm_get_frame_duration(l1_tskisr_blk->client_id, gas_id) < ( MIN_INTER_BCCH_DECODE_SEPARATION +1 ) )
                      )
                   {
                     abort_acq = FALSE;
                   }

                   if ( abort_acq && l1_bcch_list_data_ptr->fcch_sch_state
                        && bcch_list_fcch_sch_running[gas_id]
                       )
                   {
                     MSG_GERAN_HIGH_1_G("Abort ongoing ACQ to do the pending BCCH FN %d",l1_bcch_list_data_ptr->strongest_arfcn_bcch_fn);

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
                     if ( gl1_msg_get_multi_sim_mode() && L1_background_HPLMN_search_in_progress(gas_id) )
                     {
                       /* before aborting acq suspend trm release */
                       l1_fm_set_suspend_trm_release( (SUSPENSION_FRAME_DELAY+1),
                                                       FM_PRI_BACKGROUND_PLMN, gas_id );
                     }
#endif
                 /* Abort ongoing ACQ to schdule high prio BCCH irrespctive of wether it is W2G BPLMN
				  or cell selection state */
                     if ( bcch_list_fcch_sch_running[gas_id])
					 {
                       gl1_msg_abort_rx_ncell_fcch_sch(DELAYED_ABORT,gas_id);
                     }

                     if( !L1_background_HPLMN_search_in_progress(gas_id) )
                     {
                       acq_suspend_timer[gas_id] = suspension_delay;
                     }
                   }
                 }
                 else if ( !L1_background_HPLMN_search_in_progress(gas_id) && ( acq_suspend_timer[gas_id] > 0 ) )
                 {
                   if( ( --acq_suspend_timer[gas_id] == 0 ) || ( bcch_list_fcch_sch_running[gas_id] == FALSE ) )
                   {
                     if ( bcch_list_fcch_sch_running[gas_id] == FALSE )
                     {
                       acq_suspend_timer[gas_id] = 0;
                       l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;
                       MSG_GERAN_HIGH_0_G("move to L1_BCCH_LIST_INIT");
                     }
                     else
                     {
                       MSG_GERAN_ERROR_0_G("ACQ hasn't stopped yet");
                     }
                   }
                 }
               }
            }
            else
            {
               MSG_GERAN_ERROR_1_G("Unexpected command %d", l1_tskisr_blk->command);
            }
         }
         break;

      default:
         MSG_GERAN_ERROR_1_G("FindBcchList: Bad sub_sub_state=%d", l1_tskisr_blk->acq_bcch_state);
         break;

   } /* end of switch (subsubstate) */

   if (l1_bcch_list_data_ptr->fcch_sch_state)
   {
      gl1_msg_init_frame_tick_info(&ftn);
      ftn.next_frame_is_acq_ncell_block_start = l1_bcch_list_data_ptr->start_acq_handler;
      gl1_msg_frame_tick_notification(&ftn,gas_id);
   }

   idle = FALSE;

   return(idle);
}

/*===========================================================================

FUNCTION  l1_bcch_current_bcch_fn

DESCRIPTION
  returns the frame number of current bcch search

DEPENDENCIES
  None

RETURN VALUE
  frame number

SIDE EFFECTS
  None

===========================================================================*/
uint32 l1_bcch_current_bcch_fn( gas_id_t gas_id )
{
  uint32 bcch_fn = NOT_A_FRAME_NUMBER;
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];

  if( l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] != MAX_GSM_FRAME_NUMBER + 1)
  {
    bcch_fn = l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ];
  }
  return bcch_fn;
}

/*===========================================================================

FUNCTION  BCCH_LIST_RX_CALLBACK

DESCRIPTION
  Callback function called when a BCCH block has been received.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void bcch_list_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
  l1_bcch_list_data_t *l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];
  l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];

   uint32 local_FN;
   uint8  si_tc;

   if (!rpt)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt pointer");
      return;
   }
   else
   {
      local_FN = gl1_get_FN( gas_id );
      si_tc = l1_compute_tc( SUB_FN( local_FN, l1_BCCH_List_ptr->fn_lag[ l1_BCCH_List_ptr->current_bcch_index ] ),
                             rpt->ccch.extbcch );
   }

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: bcch_list_rx_callback: Preparing Si3/SI4");
   rpt->ccch.hdr.good_data = TRUE;
   if (gplt_fbsb_data.si3only)
   {
      memscpy(rpt->ccch.data,sizeof(rpt->ccch.data),&gplt_si3_result, sizeof(gplt_si3_result));
   }
   else
   {
      if ((si_tc == 2) || (si_tc == 6) )
      {
         MSG_GERAN_HIGH_0_G("GPLT DBG: Making SI3");
         memscpy(rpt->ccch.data,sizeof(rpt->ccch.data),&gplt_si3_result, sizeof(gplt_si3_result));
      }

      if ((si_tc == 3) || (si_tc == 7) )
      {
         //si4
         MSG_GERAN_HIGH_0_G("GPLT DBG: Making SI4");
         memscpy(rpt->ccch.data,sizeof(rpt->ccch.data),&gplt_si4_result, sizeof(gplt_si4_result));
      }
   }
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   /* Log */
   l1_log_ncell_bcch_msg_metrics(&rpt->ccch, gas_id);

   /* send reports up the stack */
   if ( rpt->ccch.hdr.good_data ) /* rpt->good_data is 2 if true */
   {
      L1_send_sysinfo_PH_DATA_IND(TRUE,
                                  BCCH,  /* we only ever receive BCCH */
                                  FALSE, /* BCCH can never be own paging block */
                                  rpt->ccch.arfcn,
                                  si_tc,
                                  rpt->ccch.data,
                                  MAX_BCCH_OCTETS, gas_id);

      /* Mark ARFCN as attempted */
      l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] = TRUE;
#ifdef  FEATURE_GSM_COEX_SW_CXM
      /* Trigger for Re-freq reg */
      garb_intf_notify_find_bcch_decode_done(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */
      l1_BCCH_List_ptr->num_valid_carriers--;

      MSG_GERAN_MED_2_G("BCCH(%d) decoded (arfcn=%d)", si_tc, rpt->ccch.arfcn.num);
   }
   else
   {
      L1_send_PH_DATA_IND(FALSE,
                          BCCH,
                          FALSE,
                          rpt->ccch.arfcn,
                          NULL,
                          0, gas_id);

      if ( ++l1_BCCH_List_ptr->bcch_failed_cnt[ l1_BCCH_List_ptr->current_bcch_index ] )
      {
         /* Send MPH_DECODE_BCCH_FAILURE (BCCH fail) */
         L1_send_MPH_DECODE_BCCH_LIST_FAILURE_IND( l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],
                                                   RR_L1_BCCH_FAILED, gas_id );

         /* Mark ARFCN as attempted */
         l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] = TRUE;
#ifdef  FEATURE_GSM_COEX_SW_CXM
         /* Trigger for Re-freq reg */
         garb_intf_notify_find_bcch_decode_done(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

      }

      MSG_GERAN_ERROR_3_G("BCCH(tag=%d) decode failed (arfcn=%d TC=0x%02X)",
                rpt->ccch.tag,
                rpt->ccch.arfcn.num,
                si_tc);

   }
      /* Reset the afc information since it is now deemed to be invalid */
      l1_BCCH_List_ptr->init_acq_afc[l1_BCCH_List_ptr->current_bcch_index ].valid = FALSE;
      l1_BCCH_List_ptr->init_acq_afc[l1_BCCH_List_ptr->current_bcch_index ].word1 = 0;

   l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;


   if(( L1_IDLE_MODE == l1_tskisr_blk->l1_state )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            ||(l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    )
   {
     /* set the serving cell saved afc info */
     if( FALSE != l1_scell_afc_info_ptr->valid )
     {
       gl1_msg_acq_afc_init( l1_idle_data->campedon_cell_ARFCN,gas_id );

       gl1_msg_set_afc_info( l1_scell_afc_info_ptr->afc_freq,gas_id );

       MSG_GERAN_MED_3_G( "Restoring scell ARFCN %d afc_freq to %dXO FN %d",
                l1_idle_data->campedon_cell_ARFCN.num,
                l1_scell_afc_info_ptr->afc_freq,
                GSTMR_GET_FN_GERAN( gas_id) );

       l1_scell_afc_info_ptr->valid = FALSE;
     }
   }

 #ifdef FEATURE_GL1_ASDIV_ACQ
   else if ((l1_tskisr_blk->l1_state == L1_FIND_BCCH) ||(l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH))
   {
       if (gl1_asdiv_acq_is_mon_active(gas_id)) 
       {
           gl1_asdiv_acq_update_params(GL1_ASDIV_ACQ_EV_BCCH_END, l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ], 0,0,gas_id);
       }
   }
#endif /*FEATURE_GL1_ASDIV_ACQ*/
   if ( l1_bcch_list_data_ptr->find_bcch_rx_in_progress == 1 )
   {
      l1_bcch_list_data_ptr->find_bcch_rx_in_progress--;
   }
   else
   {
      MSG_GERAN_ERROR_0_G("Multiple BCCH decodes in progress");
   }
}

/*===========================================================================

FUNCTION  BCCH_LIST_ABORT_CALLBACK

DESCRIPTION
  Callback function called when a BCCH block has been aborted

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void bcch_list_abort_callback(gas_id_t gas_id)
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_bcch_list_data_t *l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];
  l1_ncell_afc_info_t *l1_scell_afc_info_ptr = &l1_scell_afc_info[gas_id];

   if ( l1_bcch_list_data_ptr->find_bcch_rx_in_progress )
   {
      l1_bcch_list_data_ptr->find_bcch_rx_in_progress = 0;
   }

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
     l1_fm_clear_required_frames(FM_PRI_BACKGROUND_PLMN, gas_id);
   }

   l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_WAITING;

   /* Ensure that upon aborting a BCCH decode for a background
    * PLMN search the AFC frequencies are all adjusted
    * back to their original settings.
    */
   if(( L1_IDLE_MODE == l1_tskisr_blk->l1_state )
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
            ||(l1_tskisr_blk->l1_state == L1_MULTI_SIM_IDLE)
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    )
   {
     /* set the serving cell saved afc info */
     if( FALSE != l1_scell_afc_info_ptr->valid )
     {
       gl1_msg_acq_afc_init( l1_idle_data->campedon_cell_ARFCN,gas_id );

       gl1_msg_set_afc_info( l1_scell_afc_info_ptr->afc_freq,gas_id );

       MSG_GERAN_MED_3_G( "Restoring scell ARFCN %d afc_freq to %dXO FN %d",
                l1_idle_data->campedon_cell_ARFCN.num,
                l1_scell_afc_info_ptr->afc_freq,
                GSTMR_GET_FN_GERAN( gas_id) );

       l1_scell_afc_info_ptr->valid = FALSE;
     }
   }

}


/*===========================================================================

FUNCTION  BCCH_LIST_METRICS_CALLBACK

DESCRIPTION
  Callback function called when BCCH block metrics are available.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void bcch_list_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
   BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
   dBx16_T             power_instant = 0;
   uint8               num_bursts;
   uint8               valid_num_bursts=0;

   /* Log */
   l1_log_ncell_bcch_burst_metrics(rpt, gas_id);

   /* get the rx power avg */
   valid_num_bursts = 0;
   for ( num_bursts = 0; num_bursts < 4; num_bursts++ )
   {
     if ( rpt[num_bursts]->valid )
     {
        power_instant +=  rpt[num_bursts]->pwr_dBm_x16;
        valid_num_bursts++;
     }
   }

   if (valid_num_bursts)
   {
#ifdef FEATURE_GL1_ASDIV_ACQ
     MSG_GERAN_HIGH_1_G(GL1_ASDIV_HDR"Number of BCCH Bursts:%d", valid_num_bursts);
#endif
     power_instant = power_instant/valid_num_bursts;
     l1_BCCH_List_ptr->rx_power_instant[ l1_BCCH_List_ptr->current_bcch_index ] = power_instant;
   }
}


/*===========================================================================

FUNCTION  l1_bcch_list_cell_rx_power

DESCRIPTION
   function get rx power for the cell in the bcch list .

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
int32 l1_bcch_list_cell_rx_power(ARFCN_T arfcn,gas_id_t gas_id)
{
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
  uint16 n =0;

  for( n = 0; n < l1_BCCH_List_ptr->length; n++ )
  {
    if( ARFCNS_EQUAL(l1_BCCH_List_ptr->arfcn[ n ], arfcn) )
    {
#ifdef FEATURE_GERAN_REDUCED_F3
       MSG_GERAN_MED_2_G( "arfcn %d  rx_power_instant %d ",arfcn.num, l1_BCCH_List_ptr->rx_power_instant[ n ] );
#endif
      return (l1_BCCH_List_ptr->rx_power_instant[ n ]);
    }
  }
  /* Return the pointer to caller */
  return  ((int32)(-32765));  // UNKNOWN_POWER
}

/*
 *     Function name :  next_SI3_fn
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Calculates the serving cell frame number corresponding to the next BCCH
 *     block(SI3) on a neighbor cell.  This functions uses values stored in the
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
static uint32 next_SI3_fn( uint32 serv_fn, uint32 fn_lag )
{
   uint32  fn;
   uint16  fn_mod_408;
   uint8   fn_mod_51, tc;

   /* Work out the ncell frame number */
   fn             = FRAMES_PER_HYPERFRAME + serv_fn - fn_lag + 1;
   fn_mod_408     = fn % 408;
   fn_mod_51      = fn_mod_408 % 51;
   tc             = (uint8)(fn_mod_408 / 51);

   /* Compute number of frames upto the next SI# (TC = 2 or 6) */
   /* 53 is 51 + 2, the frame where the BCCH occurs */
   /* tc + 1 because (52 - fn_mod_51) takes us to the next tc */
   /* We only care about TC=2 and 6 (SI3) */
   if ( (tc > 2) && (tc < 6) )
   {
      serv_fn =  ADD_FN( serv_fn, ( ( (53 - fn_mod_51) + ( 6 - ( tc + 1 ) ) * 51 ) % 204 ) );
   }
   else
   {
      serv_fn =  ADD_FN ( serv_fn, ( ( ( 53 - fn_mod_51 ) + ( ( 2 - ( tc + 1 ) + 8 ) % 8 ) * 51 ) % 204 ) );
   }

   return serv_fn;
}

/*
 *     Function name :  next_SI4_fn
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Calculates the serving cell frame number corresponding to the next BCCH
 *     block(SI4) on a neighbor cell.  This functions uses values stored in the
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
static uint32 next_SI4_fn( uint32 serv_fn, uint32 fn_lag )
{
   uint32  fn;
   uint16  fn_mod_408;
   uint8   fn_mod_51, tc;

   /* Work out the ncell frame number */
   fn             = FRAMES_PER_HYPERFRAME + serv_fn - fn_lag + 1;
   fn_mod_408     = fn % 408;
   fn_mod_51      = fn_mod_408 % 51;
   tc             = (uint8)(fn_mod_408 / 51);

   /* Compute number of frames upto the next SI# (TC = 3 or 7) */
   /* 53 is 51 + 2, the frame where the BCCH occurs */
   /* tc + 1 because (52 - fn_mod_51) takes us to the next tc */
   /* We only care about TC=3 and 7 (SI4) */
   if ( (tc > 3) && (tc < 7) )
   {
      serv_fn =  ADD_FN( serv_fn, ( ( (53 - fn_mod_51) + ( 7 - ( tc + 1 ) ) * 51 ) % 204 ) );
   }
   else
   {
      serv_fn =  ADD_FN ( serv_fn, ( ( ( 53 - fn_mod_51 ) + ( ( 3 - ( tc + 1 ) + 8 ) % 8 ) * 51 ) % 204 ) );
   }

   return serv_fn;
}

/*
 *     Function name :  next_earliest_SI3_SI4_fn
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Calculates the earliest of SI3 or SI4 frame number
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
static uint32 next_earliest_SI3_SI4_fn( uint32 serv_fn, uint32 fn_lag, boolean si3_only )
{
   uint32 si3_fn = next_SI3_fn(serv_fn, fn_lag);

   if ( si3_only )
   {
      return si3_fn;
   }
   else
   {
      uint32 si4_fn = next_SI4_fn(serv_fn, fn_lag);

      return ( (IS_FRAME_NUM_LATER( si3_fn, si4_fn )) ? (si4_fn) : (si3_fn));
   }
}

/*===========================================================================

FUNCTION  L1_bcch_list_suspend

DESCRIPTION
  This function suspends parallel BCCH decodes
  Called by the background HPLMN search engine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_bcch_list_suspend(void* dummy, gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK  *l1_tskisr_blk;
   BCCH_List_T              *l1_BCCH_List_ptr;
   l1_bcch_list_data_t      *l1_bcch_list_data_ptr;
   uint8 max_bcch_attempts = MAX_BCCH_ATTEMPTS;
   NOTUSED( dummy );

   l1_tskisr_blk = &l1_tsk_buffer[gas_id];
   l1_BCCH_List_ptr = &l1_BCCH_List[gas_id];
   l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];

   if(gl1_msg_get_multi_sim_mode() && grm_is_embms_active(gas_id))
   {
     max_bcch_attempts = MAX_BCCH_ATTEMPTS_IN_EMBMS;
   }

   if (l1_tskisr_blk->acq_bcch_state == L1_BCCH_LIST_INIT ||
       l1_tskisr_blk->acq_bcch_state == L1_BCCH_LIST_START ||
       l1_tskisr_blk->acq_bcch_state == L1_BCCH_LIST_RUNNING)
   {
       /* After 4 attempts (SI3 and SI4) we'll give up and
       * declare a failure
       */
       if ( (l1_BCCH_List_ptr->current_bcch_index < l1_BCCH_List_ptr->length)&&(l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] != TRUE)
           && ( l1_BCCH_List_ptr->bcch_blocked_cnt[ l1_BCCH_List_ptr->current_bcch_index ] >= max_bcch_attempts )
         )
      {
         /* Send MPH_DECODE_BCCH_FAILURE (BCCH blocked) */
         L1_send_MPH_DECODE_BCCH_LIST_FAILURE_IND(
                                                 l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],
                                                 RR_L1_BCCH_BLOCKED, gas_id );

         MSG_GERAN_HIGH_1_G("BCCH(%d) blocked ...giving up",l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index].num);
         /* Mark ARFCN as attempted so we won't reattempt */
         l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] = TRUE;
#ifdef  FEATURE_GSM_COEX_SW_CXM
         /* Trigger for Re-freq reg */ 
         garb_intf_notify_find_bcch_decode_done(l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ],gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */
      }
   }

  /* Abort NPL1 only if handler is installed.   BCCH_RUNNING does not imply
   * handler is running because we spin in BCCH_RUNNING till the BCCH FN
   * is reached and then install the handler.  find_bcch_rx_in_progress is
   * set after the handler is installed
   */
   if ( l1_bcch_list_data_ptr->find_bcch_rx_in_progress )
   {
     /* Abort BCCH Rx */
     gl1_msg_abort_rx_ncell_bcch (DELAYED_ABORT,gas_id);
     l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_RUNNING;
   }
   else
   {
     MSG_GERAN_MED_0_G("Aborting..BCCH handler not running");

#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
     if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
     {
        l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
     }
     else
#endif
     {
        L1_background_HPLMN_search_suspend_cb(gas_id);
     }

     l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_WAITING;
   }

   MSG_GERAN_MED_0_G("BCCH list abort");
}

/*===========================================================================

FUNCTION  L1_bcch_list_acq_bcch_suspend

DESCRIPTION
  This function suspends parallel BCCH decodes
  Called by the background HPLMN search engine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_bcch_list_acq_bcch_suspend(void* dummy, gas_id_t gas_id)
{
    volatile ISRTIM_CMD_BLK   *l1_tskisr_blk;
    l1_bcch_list_data_t *l1_bcch_list_data_ptr;

    NOTUSED( dummy );
    l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];

    if(l1_bcch_list_data_ptr->fcch_sch_state)
    {
        MSG_GERAN_HIGH_0_G("Aborting ACQ list ");
        l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_WAITING;

        /* Abort ACQ only if an ACQ is in progress.
         * Else just call the suspend callback function
         */
        if (bcch_list_fcch_sch_running[gas_id])
        {
           gl1_msg_abort_rx_ncell_fcch_sch(DELAYED_ABORT,gas_id);
        }
        else
        {
#ifdef FEATURE_WTOG_BACKGROUND_PLMN_SEARCH
           if (l1_tskisr_blk->l1_state == L1_WTOG_BPLMN_MODE)
           {
              l1_tskisr_blk->main_command = L1_SUSPEND_COMMAND;
           }
           else
#endif
           {
              L1_background_HPLMN_search_suspend_cb(gas_id);
           }
        }
    }
    else
    {
        L1_bcch_list_suspend(0, gas_id);
    }

#if      ( defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM ) )
   l1_ds_set_bplmn_state_to_null(gas_id);
#endif /* FEATURE_GSM_BACKGROUND_HPLMN_SEARCH & ( FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM  ) */
}


/*===========================================================================

FUNCTION  L1_bcch_list_resume

DESCRIPTION
  This function resumes parallel BCCH decodes
  Called by the background HPLMN search engine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_bcch_list_resume(gas_id_t gas_id)
{
  volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  BCCH_List_T *l1_BCCH_List_ptr=&l1_BCCH_List[gas_id];
  l1_bcch_list_data_t *l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];


   uint32 this_FN;

   MSG_GERAN_MED_0_G("BCCH list resume");

   /* Set this because if we suspend before we run L1_initial_acq the
    * abort_cb in NPL1 will point to the previous caller
    */
   l1_bcch_list_data_ptr->rx_cb.abort_cb   = bcch_list_abort_callback;

   l1_tskisr_blk->acq_bcch_state = L1_BCCH_LIST_INIT;

#ifdef FEATURE_GSM_COEX_SW_CXM
   /* bplmn_acq_bcch_list  = TRUE effective only in GARB_IDLE */
   garb_register_bcch_list_arfcns(TRUE,gas_id);
#endif /* FEATURE_GSM_COEX_SW_CXM */

   /* If we're starting from scratch and there's no previously scheduled
    * BCCH just return
    */
   if ( l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] ==
        MAX_GSM_FRAME_NUMBER + 1 )
   {
      return;
   }

       /* If its MAX_BCCH_ATTEMPTS, SI3 must be blocked because we
        * set bcch_decoded = TRUE in the rx_callback when it
        * becomes MAX_BCCH_ATTEMPTS.   Therefore try SI4.  SI4 is
        * one multiframe ( 51 frames ) after SI3
        */
       if (l1_BCCH_List_ptr->bcch_decoded[ l1_BCCH_List_ptr->current_bcch_index ] != TRUE) 
       { 
         this_FN = gl1_get_FN( gas_id );
         if ( IS_FRAME_NUM_LATER(this_FN, l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ]) )
         {
           l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] =
           next_earliest_SI3_SI4_fn ( this_FN,
                                      l1_BCCH_List_ptr->fn_lag[ l1_BCCH_List_ptr->current_bcch_index ],
                                      l1_BCCH_List_ptr->si3_only );

           /* Compare the next BCCH FN with the previously scheduled one.  If they
            * are different, then the one that was scheduled was blocked. 
            */
          if((l1_BCCH_List_ptr->current_bcch_index < l1_BCCH_List_ptr->length)&& (l1_BCCH_List_ptr->prev_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] != ( MAX_GSM_FRAME_NUMBER + 1 )) )
           {
             if ( l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] != l1_BCCH_List_ptr->prev_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ] )
             {
               l1_BCCH_List_ptr->bcch_blocked_cnt[ l1_BCCH_List_ptr->current_bcch_index ]++;

               MSG_GERAN_HIGH_4_G("ARFCN=%d, index=%d, new Next BCCH FN=%d blocked_cnt:%d ",
                          l1_BCCH_List_ptr->arfcn[ l1_BCCH_List_ptr->current_bcch_index ].num, 
                          l1_BCCH_List_ptr->current_bcch_index, 
                          l1_BCCH_List_ptr->next_bcch_fn[ l1_BCCH_List_ptr->current_bcch_index ],
                          l1_BCCH_List_ptr->bcch_blocked_cnt[ l1_BCCH_List_ptr->current_bcch_index ]);
             }
           }
         }
       }
     }

/*===========================================================================

FUNCTION  L1_bcch_list_acq_bcch_resume

DESCRIPTION
  This function resumes parallel BCCH decodes
  Called by the background HPLMN search engine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void L1_bcch_list_acq_bcch_resume( gas_id_t gas_id)
{
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

    MSG_GERAN_HIGH_0_G("Resuming L1_bcch_list_acq_bcch_resume");

    //Resume BCCH list decodes directly instead of interrupted acquisition
    L1_bcch_list_resume(gas_id);

    l1_set_fcch_sch_state( FALSE, gas_id );

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    if ( l1_tskisr_blk->l1_state == L1_MULTI_SIM_FIND_BCCH )
    {
      MSG_GERAN_MED_0_G("Multi-Sim bcch list resume");
    }
    else
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
    if (l1_tskisr_blk->l1_state != L1_WTOG_BPLMN_MODE)
    {
       l1_set_background_HPLMN_search_suspended(FALSE, gas_id);
    }
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================

FUNCTION  gpl1_multi_sim_bplmn_decode_bcch_inactive

DESCRIPTION
  Returns the status of the parrallel bcch decode state machine

DEPENDENCIES
  None

RETURN VALUE
  TRUE  = completed
  FALSE = ongoing

SIDE EFFECTS
  None

===========================================================================*/
boolean gpl1_multi_sim_bplmn_decode_bcch_inactive( gas_id_t gas_id  )
{
  l1_bcch_list_data_t *l1_bcch_list_data_ptr = &l1_bcch_list_data[gas_id];

  boolean completed = FALSE;

  if (l1_bcch_list_data_ptr->fcch_sch_state)
  {
    /* If TRUE then the process is completed or aborted */
    if ( !bcch_list_fcch_sch_running[gas_id])
    {
      completed = TRUE;
    }

    MSG_GERAN_HIGH_1_G("BPLMN ACQ competed = %d",!bcch_list_fcch_sch_running[gas_id]);
  }
  else
  {
    /* Return TRUE only if a BCCH block is not in progress */
    if ( l1_bcch_list_data_ptr->find_bcch_rx_in_progress == 0 )
    {
      completed = TRUE;
    }

    MSG_GERAN_HIGH_1_G("BPLMN BCCH running = %d",l1_bcch_list_data_ptr->find_bcch_rx_in_progress);
  }

  return completed;
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */
