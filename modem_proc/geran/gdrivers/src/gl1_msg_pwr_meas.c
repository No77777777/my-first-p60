/*===========================================================================

     IMPLEMENTATION OF NPL1 POWER MEASUREMENT(Non-PDCH specific) FUNCTIONS

GENERAL DESCRIPTION
   This file implements NPL1 non-PDCH specific power measurement functions.

EXTERNALIZED FUNCTIONS
gl1_msg_cfg_pwr_meas
gl1_msg_do_pwr_meas
gl1_msg_suspend_pwr_meas
gl1_msg_resume_pwr_meas
gl1_msg_stop_pwr_meas
gl1_msg_abort_pwr_meas
gl1_msgi_init_pwr_meas
gl1_msgi_terminate_pwr_meas

INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the order in which functions should be called:
gl1_msgi_init_pwr_meas
  gl1_msg_cfg_pwr_meas
    gl1_msg_do_pwr_meas
      gl1_msg_suspend_pwr_meas
        gl1_msg_resume_pwr_meas
      gl1_msg_stop_pwr_meas
      gl1_msg_abort_pwr_meas
gl1_msgi_terminate_pwr_meas



Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_pwr_meas.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $


when       who      what, where, why
--------   ---      ----------------------------------------------------------
10/08/16   cws      CR1044687 Use ARFCN from pwr sm directly for pwr monitor log packet
28/07/16   og       CR1034290. Reject power measurement commands if both state machines
                    are busy.
06/07/17   mk       CR2071218 FR44121 -Synchronous Ncell Quality Monitor - run synq algo on pwr mon results
21/04/17   br       CR1041084 FR 36219: GDET – GMSK Modulation Detector
08/01/16   cws      CR949517 Force asdiv position when requested by RF at certain SAR stateNew
11/11/15   smd      CR929829 Check the correct flag for BPLMN active
12/08/15   cws      CR864883 FR27546+FR27544 Add MTPL/power imbalance factors into ASDIV algorithm
10/08/15   og       CR884495. Avoid resetting the campedon cell agc structure when dealing
                    with unfinished power monitors.
21/07/15   jj       CR875985 reset number_of_attempt_with_lower_delta flag at asdiv reset
23/07/15   cah      CR872243 LWG + G DR-DSDS : LTE Conn with G2G mobility failing, FC -> DTA Measurements 
26/05/15   akh      CR838149 Ncell pwr levels for idle sub are not updated during GBTA (G Tr HMSC + G Idle)
26/05/15   js       CR815068 Reset  meas flag when pwr measurements are aborted.
12/05/15   akh      CR813711 Improvements to power monitor debug structure
11/05/15   akh      CR834821 Fix regression from CR820145 (idle sub power monitors not done in GBTA)
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
01/05/15   cws      CR794322 Correct the switching logic to be consistent with CR766904
17/04/15   ak       CR813711 Debug code for SCE power monitors
16/04/15   ak       CR820145 Problems with empty BA list during ded mode and with HMSC tests
02/04/15   ab       CR815867 : Reset power meas handlers after callback is issued.
25/03/15   ap       CR799526 FR 22639 - Change Geran BPLMN Mechanism + Removal of dependency on X tech in BG Traffic 
09/03/15   cs       CR802822 Remove power measure cap for DESENSE channel
25/02/14   cah     CR787616 Updates to Persistent Denial mechanism. reduce amount of inter-band power monitors in good
18/02/15   zc       CR795410: Support asdiv type2 during data
20/02/15   zf       CR797700: Call abort CB when terminate pwr meas
18/02/15   am       CR775652 Mismatch in monitor reading
18/02/15   am       CR790448 Re-set meas_aborted_while_retrieving
18/02/15   am       CR777094 Keep mon write and read logging-indices aligned after mon is aborted
26/01/15   npt      CR786002 Fix compilation errors when FEATURE_DUAL_SIM is enabled
12/01/15   sjv      CR756154 Enhance Band Registration for Activities
07/01/15   jj       CR 766904 reset the type 1 asdiv algorithm during transition from dedicated to packet
19/12/14   cws      CR772318 ASDIV: Do not count in GBTA for ant meas
12/12/14   pa       CR763859:Handling of RxD clients in GL1 TRM structures.
29/05/14   mko      CR639716 GCF TC 21.1 failure, Limit the changes of CR592893 to idle mode only
26/11/14   jj       CR 760174 fix  compiler warning
19/11/14   zf       CR752040: Do not use empty power measurement results on calculating RSSI measurement
23/10/14   sjv      CR744425 Removing redundant debug F3s for PM_ACT_DBG
17/10/14   nm       CR729413 increment suspension timer for W2G BPLMN power scan if meas in progress true before terminating
09/08/14   zc       CR715095 GSM type1 ASDiv missed one probe cycle after switching
22/09/14   cah       CR723144: Bolt 2.0 DRDSDS - W+G - G2G interband ncell monitor activity denied while in W+G FC mode
17/09/14   sjv      CR724558 For PM, TRM band-reg and release at frame level granularity for perf. optimization
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
20/05/14   sk       CR659786 Ensure yielded monitor not considered for averaging
22/08/14   cja      CR643648 Desense for channel adjacent to spur
22/08/14   sp       CR697096: Suspend Pwr meas inside a TBF Suspension gap/reconfig
06/08/14   cah      CR707445 extra F3 for power monitor in progress
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
10/06/14   jj       CR668965 merge ASDIV from DI.3.0 to BO 2.0
17/04/14   jj       CR 650872 send switch complete based on  call back from rf in pch crc fail
11/04/14   sp       CR645132: Correct logging of ARFCN in retrieve data
08/04/14   gk       CR641603 Ignore yielded power monitor results
14/03/14   pg       CR630347 restrict CR578517 to multi-SIM
13/01/14   am       CR592893 Cap power measurement result on spur channels
14/01/14   cgc      CR582048 gl1_ded_late_start() mitigate late start in dedicated to prevent overruns
17/12/13   sk       CR578517 Ensure pwr measurement commands not issued during TBF reconfig
14/11/13   js       CR549415 - Do not do reselection meas on yielded arfcns
15/03/13   jj       CR 463546 tx -div- improvements
10/01/13   jj       CR 402579 fixing tx diversity merge errors
26/11/12   jj       CR 424199 Adding Scell measurement for tx-diversity when BA list is empty
10/09/13   sk       CR537393 - Removed ERR_FATAL introduced with CR513078
02/09/13   sp       CR536875 - Add F3s for Power meas stop/abort
07/08/13   zc       CR513078 - change to crash the phone on pwr measure reconfig fail
15/08/13   sk       CR524039 GBTA bringup changes
11/07/13   nm       CR510055 - trm_suspension when pwr meas aborted in Retrieving state
20/06/13   cs       Major Triton TSTS Syncup
06/06/13   cs       More TSTS Updates
22/05/13   og       CR443980: Reset mdsp read/write index while terminating power measure in GPRS.
21/05/13   cs       TSTS Updates
13/05/13   jk       CR451872 allow extra frame for RSSI results before switching state machines
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
17/01/11   cja      CR270529 Add recovery from extra pwr meas result (+ for missed Ncell for DTM mode).
24/12/09   cja      CR217174. Add message for monitor not scheduled in multi-frame.
13/02/09   ap       Lint fixes for high Warning
04/09/08   dv       CR153510: Fix for Manual NW search failure
28/08/08   cs       Implement the Fast Rx Sweep functionality for FTM
11/16/05   gfr      Issue abort callback when stopping power measures even if
                    we have not started scheduling them yet.
08/05/05   gfr      Only add handler if not already installed
07/29/05   gfr      DTM support
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
06/12/04   gfr      Slew rate limiting is now handled in the PL1.
05/20/04   gfr      Mainlined FEATURE_COMMON_PWR_HANDLER
03/09/04   gfr      Handle empty power scan requests gracefully.
02/09/04   dp       Added support for background HPLMN searching
11/05/03   gfr      Support for quad-band.
05/21/03   gfr      Minor lint fix.
05/20/03   gfr      Initial Version of GSM/GPRS combined API

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_sys_algo.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1_task.h"
#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "gpl1_dual_sim.h"
#include "gpl1_grm_intf.h"
#include "l1_utils.h"
#include "l1_sc.h"

#ifdef FEATURE_GL1_GPLT
#include "gplt_result.h"
#endif /* FEATURE_GL1_GPLT */

#define MAX_FRAMES_TO_WAIT_BEFORE_DISCARDING_PWR_MON_RESULTS  3
#define HIST_QUEUE_SIZE              3
#define INC_QUEUE_INDEX(x)           \
   {                                 \
      if (++(x) == HIST_QUEUE_SIZE)  \
         x = 0;                      \
   }
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
extern uint32  antenna_to_use[100];
extern int16 tx_div_min_delta_btw_antenna;
gl1_hw_asdiv_type1_T gl1_hw_asdiv_type1 = {0};

typedef struct {
int32 antenna1_avg_rssi_bckup1 ;
int32 antenna1_avg_rssi_bckup2 ;
int32 antenna1_avg_rssi_bckup3 ;
int32 antenna1_avg_rssi_bckup4 ;
int32 antenna1_avg_rssi_bckup5 ;

int32 antenna2_avg_rssi_bckup1 ;
int32 antenna2_avg_rssi_bckup2 ;
int32 antenna2_avg_rssi_bckup3 ;
int32 antenna2_avg_rssi_bckup4 ;
int32 antenna2_avg_rssi_bckup5 ;
}antenna_avg_backup_T;
antenna_avg_backup_T antenna_avg_backup ;

#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

/* NPL1 power measurement states transitions */
typedef enum
{
   SM_PWR_MEAS_NOT_SCHEDULED = 0,
   SM_PWR_MEAS_RECEIVED,
   SM_PWR_MEAS_SCHEDULING,
   SM_PWR_MEAS_RETRIEVING
} gl1_pwr_meas_state_type;



/* Info for one of two overlapping power measurements */
typedef struct
{
    /* parameters */
   uint32                       num_arfcns;
   ARFCN_T                      *arfcns;
   sys_algo_agc_T               **agc_ptrs;
   uint32                       max_meas_per_frame;
   uint32                       max_frames_to_measure;
   gl1_msg_pwr_meas_result_type *pwr_meas_results;
   gl1_hw_rx_tags_type          tags[HIST_QUEUE_SIZE * MAX_MEAS_PER_FRAME];

    /* vars for state processing */
   gl1_pwr_meas_state_type pwr_meas_state;
   boolean                 pwr_meas_stopped;
   uint32                  arfcn_sched_index;  /* index of next arfcn */
   uint32                  arfcn_fetch_index;  /* index for storing data */
   uint32                  meas_in_progress;   /* meas currently in progress */
   uint32                  frame_count;        /* count of frames passed */

    /*  used for result retrieval */
   uint32                  hist_wr;
   uint32                  hist_rd;
   uint32                  num_meas_hist[HIST_QUEUE_SIZE];
   boolean                 second_attempt[HIST_QUEUE_SIZE];    /* if we have polled mdsp yet */
   uint8                   num_attempts[HIST_QUEUE_SIZE];
   uint32                  FN_retreive_attempted[HIST_QUEUE_SIZE];

   boolean                 meas_aborted_while_retrieving;

  /* Callbacks */
  gl1_msg_pwr_meas_cb_type       pwr_meas_cb;
  gl1_msg_pwr_meas_abort_cb_type pwr_meas_abort_cb;
} sm_pwr_meas_type;

static sm_pwr_meas_type    rxlev_SMs[NUM_GERAN_DATA_SPACES][2];

static uint32 cur_sm[NUM_GERAN_DATA_SPACES] =
                { INITIAL_VALUE( 0 ) };

static int unexpected_block_start[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( 0 ) };

#ifdef FEATURE_GSM_SCE_PWR_MEAS_DBG
/* -----------------------------------------------------------------------
** SCE debug
** ----------------------------------------------------------------------- */

struct gl1_msg_pwr_meas_dbg_store_S
{
   uint32                           FN;
   l1_sc_mode_T                     mode;
   boolean                          active;
   gl1_msg_pwr_meas_dbg_cmd_T       cmd;

   uint32                           num_arfcns;
   uint32                           max_meas_per_frame;
   uint32                           max_frames_to_measure;

   gl1_pwr_meas_state_type          cur_pwr_meas_state;   
   gl1_pwr_meas_state_type          other_pwr_meas_state;   
   gl1_msg_pwr_meas_cb_type         pwr_meas_cb;
   gl1_msg_pwr_meas_abort_cb_type   pwr_meas_abort_cb;

   uint32                           cur_sm;

#ifdef FEATURE_GPRS_GBTA
   grm_gbta_idle_event_t            gbta_idle_event;
   grm_gbta_idle_event_status_t     gbta_idle_event_status;
#endif
};

#define GL1_MSG_PWR_MEAS_DBG_CMD_STORE_MAX_IDX       49
static gl1_msg_pwr_meas_dbg_store_T gl1_msg_pwr_meas_dbg_cmd_store[NUM_GERAN_DATA_SPACES][GL1_MSG_PWR_MEAS_DBG_CMD_STORE_MAX_IDX+1];
static uint8 gl1_msg_pwr_meas_dbg_cmd_store_idx[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE( GL1_MSG_PWR_MEAS_DBG_CMD_STORE_MAX_IDX ) };

static l1_sce_dbg_mdsp_gprs_T * l1_sce_dbg_mdsp_gprs_update_gl1_params(l1_sce_dbg_mdsp_gprs_cmd_T cmd, sm_pwr_meas_type *sm_ptr, gas_id_t gas_id);

#define L1_SCE_DBG_MDSP_GPRS_UPDATE_GL1_PARAMS(cmd, sm_ptr, gas_id)  ((void) l1_sce_dbg_mdsp_gprs_update_gl1_params(cmd, sm_ptr, gas_id))

#else /* FEATURE_GSM_SCE_PWR_MEAS_DBG */

#define L1_SCE_DBG_MDSP_GPRS_UPDATE_GL1_PARAMS(cmd, sm_ptr, gas_id)

#endif /* FEATURE_GSM_SCE_PWR_MEAS_DBG */


/* forward function declarations */
boolean gl1_msg_pwr_meas_hdlr
   (
     boolean is_next_frame_dl_block_start,
     uint32  time_stamp,
     gas_id_t gas_id
   );
static void retrieve_data(sm_pwr_meas_type *sm_ptr, uint32 time_stamp,gas_id_t gas_id);
static void report_results(sm_pwr_meas_type *sm_ptr,gas_id_t gas_id);

static uint32 gl1_ded_late_start(uint32 in_num_meas, gas_id_t gas_id);
static uint16 gl1_dbg_late_count =0;

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
void gl1_hw_type1_switch_decision(int16 thresh, int16 imbl, boolean packet_trans, boolean lower_delta, gas_id_t gas_id);
void gl1_hw_clear_antenna_meas(gas_id_t gas_id);
void gl1_hw_push_antenna_avg_fifo (void);
void gl1_hw_clear_antenna_avg_fifo(void);
extern void gl1_hw_switch_antenna_tx_div (gas_id_t gas_id);
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */


/* Keep track of whether or not power measurements are suspended */
static boolean suspend_pwr_meas[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

/* =======================================================================
**              NPL1 FUNCTION DEFINITIONS FOR POWER MEASUREMENTS
** ======================================================================= */
/*===========================================================================
FUNCTION gl1_msgi_pwr_meas_init

DESCRIPTION
   This function initializes data structures used for power measurements.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_pwr_meas_init (gas_id_t gas_id)
{
   uint8 i = 0;

   suspend_pwr_meas[gas_id] = FALSE;
   cur_sm[gas_id]           = 0;

   for(i = 0; i < 2; i++)
   {
      L1_STATE_CHANGE(rxlev_SMs[gas_id][i].pwr_meas_state   = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");
      rxlev_SMs[gas_id][i].meas_in_progress = FALSE;
      rxlev_SMs[gas_id][i].pwr_meas_cb = NULL;
      rxlev_SMs[gas_id][i].pwr_meas_abort_cb = NULL;
   }
}


/*===========================================================================
FUNCTION gl1_msgi_pwr_meas_terminate

DESCRIPTION
   Terminates processing of any current power measurements

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_pwr_meas_terminate (gas_id_t gas_id)
{
   uint8 i;

   for(i = 0; i < 2; i++)
   {
     rxlev_SMs[gas_id][i].pwr_meas_cb = NULL;
     if (rxlev_SMs[gas_id][i].pwr_meas_abort_cb)
     {
       rxlev_SMs[gas_id][i].pwr_meas_abort_cb(gas_id);
       rxlev_SMs[gas_id][i].pwr_meas_abort_cb = NULL;
     }
   }

   suspend_pwr_meas[gas_id] = FALSE;

   /* Remove handler if necessary */
   if (rxlev_SMs[gas_id][0].pwr_meas_state != SM_PWR_MEAS_NOT_SCHEDULED ||
       rxlev_SMs[gas_id][1].pwr_meas_state != SM_PWR_MEAS_NOT_SCHEDULED)
   {
      MSG_GERAN_ERROR_3_G("Terminating active pwr meas: state=%d/%d, FN=%d", rxlev_SMs[gas_id][0].pwr_meas_state,
                rxlev_SMs[gas_id][1].pwr_meas_state, GSTMR_GET_FN_GERAN(gas_id));

      gl1_msgi_remove_ft_handler(gl1_msg_pwr_meas_hdlr,gas_id);

      mdsp_gprs_abort_power_measure( gas_id );

      /* terminate int also, otehrwise int can pick the buffer index which has been re-set */
      gl1_msgi_int_meas_terminate(gas_id);

      L1_STATE_CHANGE(rxlev_SMs[gas_id][0].pwr_meas_state = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");
      L1_STATE_CHANGE(rxlev_SMs[gas_id][1].pwr_meas_state = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");

      GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_TERMINATE_PWR_MEAS, gas_id);
   }
}


/*=============================================================================
FUNCTION gl1_msg_cfg_pwr_meas

DESCRIPTION
   Configures the message layer to retrieve RXLEV measurements. The message
   layer is now ready to be commanded to retrieve RXLEV measurements.

DEPENDENCIES
   The message layer must be initialized.

RETURN VALUE
   None

SIDE EFFECTS
   Sets static cb variable
============================================================================= */
boolean gl1_msg_cfg_pwr_meas
(
   gl1_msg_pwr_meas_cb_type pwr_meas_callback,
   gl1_msg_pwr_meas_abort_cb_type pwr_meas_abort_callback,
   gas_id_t gas_id
)
{
   if ( ! rxlev_SMs[gas_id][0].pwr_meas_state )
   {
     rxlev_SMs[gas_id][0].pwr_meas_cb = pwr_meas_callback;
     rxlev_SMs[gas_id][0].pwr_meas_abort_cb = pwr_meas_abort_callback;
   }
   else if ( ! rxlev_SMs[gas_id][1].pwr_meas_state )
   {
     rxlev_SMs[gas_id][1].pwr_meas_cb = pwr_meas_callback;
     rxlev_SMs[gas_id][1].pwr_meas_abort_cb = pwr_meas_abort_callback;
   }
   else
   {
     MSG_GERAN_ERROR_0_G("Pwr meas re-cfg while in progress.");

     return FALSE;
   }
   return TRUE;
}


/*=============================================================================
FUNCTION gl1_msg_do_pwr_meas

DESCRIPTION
   This function is to be called at least one frame prior to which RXLEV measure-
   ments are to be taken. The mDSP schedules the timeslots as well as the number of
   measurements that can be taken in a frame. The measurement results are copied
   into the results buffer and the callback is invoked when as many measurements
   as possible are taken.

   The results of the measurements are reported once the measurements for the
   specified frequencies are taken.

DEPENDENCIES
   The message layer must be configured for RXLEV measurements.

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_do_pwr_meas
(
   ARFCN_T                            *arfcns,
   sys_algo_agc_T                     **agc_ptrs,
   uint32                             num_arfcns,
   uint32                             max_meas_per_frame,
   uint32                             max_frames_to_measure,
   gl1_msg_pwr_meas_result_type       *pwr_meas_results,
   gas_id_t                           gas_id
)
{
   sm_pwr_meas_type  *sm_ptr, *other_sm_ptr;
   uint32  sm, i;

   /* If two concurrent measures are happening already, return */
   if (rxlev_SMs[gas_id][0].pwr_meas_state && rxlev_SMs[gas_id][1].pwr_meas_state)
   {
      GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_DO_PWR_MEAS_FAIL_SM_FULL, gas_id);
      MSG_GERAN_ERROR_2_G("Too many power measure requests (s1=%d, s2=%d)!",rxlev_SMs[gas_id][0].pwr_meas_state,rxlev_SMs[gas_id][1].pwr_meas_state);
      return;
   }

   /* If max meas per frame is 0, we can't do anything! */
   if (max_meas_per_frame == 0)
   {
      GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_DO_PWR_MEAS_FAIL_INVALID_MAX_MEAS, gas_id);
      MSG_GERAN_ERROR_0_G("Max meas per frame = 0?");
      return;
   }

   sm = (rxlev_SMs[gas_id][0].pwr_meas_state ? 1 : 0);
   sm_ptr = &rxlev_SMs[gas_id][sm];

   /* If both state machine instances are busy then we need to retry later */
   if( SM_PWR_MEAS_NOT_SCHEDULED != sm_ptr->pwr_meas_state )
   {
      MSG_GERAN_ERROR_2_G("busy 1st SM %d 2nd SM %d ", rxlev_SMs[gas_id][0].pwr_meas_state, rxlev_SMs[gas_id][1].pwr_meas_state );
      return;
   }

   other_sm_ptr = &rxlev_SMs[gas_id][sm == 0 ? 1 : 0];

   /* If the other sm is also idle then set cur_sm and add the handler */
   if (other_sm_ptr->pwr_meas_state == SM_PWR_MEAS_NOT_SCHEDULED)
   {
      cur_sm[gas_id] = sm;
      gl1_msgi_add_ft_handler(gl1_msg_pwr_meas_hdlr, GL1_MSG_FT_HDLR_PWR_MEAS,gas_id);
   }

   sm_ptr->num_arfcns = num_arfcns;
   sm_ptr->arfcns = arfcns;
   sm_ptr->agc_ptrs = agc_ptrs;
   sm_ptr->max_meas_per_frame = max_meas_per_frame;
   sm_ptr->max_frames_to_measure = max_frames_to_measure;
   sm_ptr->pwr_meas_results = pwr_meas_results;
   sm_ptr->pwr_meas_results->num_pwr_meas = 0;

   L1_STATE_CHANGE(sm_ptr->pwr_meas_state = SM_PWR_MEAS_RECEIVED, "SM_PWR_MEAS_RECEIVED");
   sm_ptr->pwr_meas_stopped = FALSE;
   sm_ptr->arfcn_sched_index = 0;
   sm_ptr->arfcn_fetch_index = 0;
   sm_ptr->meas_in_progress = 0;
   sm_ptr->frame_count = 0;

   for (i = 0; i < HIST_QUEUE_SIZE; i++)
   {
      sm_ptr->num_meas_hist[i] = 0;
      sm_ptr->num_attempts[i]  = 0;
      sm_ptr->second_attempt[i] = FALSE;

      /* used for counting num_attempts */
      sm_ptr->FN_retreive_attempted[i] = GSTMR_GET_FN_GERAN( gas_id);
   }

   /* The write pointer is set to two past the read pointer to trigger reading
      measurement attempts two frames after the first scheduled measure */
   sm_ptr->hist_rd = 0;
   sm_ptr->hist_wr = 2;

   unexpected_block_start[gas_id] = 0;

#ifdef FEATURE_GSM_SCE_PWR_MEAS_DBG
   {
      gl1_msg_pwr_meas_dbg_store_T *pl1_sce_dbg_pwr_meas;

      pl1_sce_dbg_pwr_meas = gl1_msg_pwr_meas_dbg_new_entry(GL1_MSG_PWR_MEAS_DBG_CMD_DO_PWR_MEAS, gas_id);

      pl1_sce_dbg_pwr_meas->num_arfcns = num_arfcns;
      pl1_sce_dbg_pwr_meas->max_meas_per_frame = max_meas_per_frame;
      pl1_sce_dbg_pwr_meas->max_frames_to_measure = max_frames_to_measure;
      pl1_sce_dbg_pwr_meas->cur_pwr_meas_state = sm_ptr->pwr_meas_state;
      pl1_sce_dbg_pwr_meas->other_pwr_meas_state = other_sm_ptr->pwr_meas_state;
      pl1_sce_dbg_pwr_meas->pwr_meas_cb = sm_ptr->pwr_meas_cb;
      pl1_sce_dbg_pwr_meas->pwr_meas_abort_cb = sm_ptr->pwr_meas_abort_cb;
      pl1_sce_dbg_pwr_meas->cur_sm = sm;

#ifdef FEATURE_GPRS_GBTA
      pl1_sce_dbg_pwr_meas->gbta_idle_event = grm_gbta_info[gas_id].idle_event;
      pl1_sce_dbg_pwr_meas->gbta_idle_event_status = grm_gbta_info[gas_id].idle_event_status;
#endif   
   }
#endif /* FEATURE_GSM_SCE_PWR_MEAS_DBG */

}


/*===========================================================================

FUNCTION gl1_msg_suspend_pwr_meas

DESCRIPTION
  This functions temporarily suspends power measurements.  The measurements that
  are suspended are scheduled later and are not thrown away but the total number
  of measurements are truncated

DEPENDENCIES
  gl1_msg_cfg_power_meas must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  Power measurements are suspended until explicitly resumed
===========================================================================*/
void gl1_msg_suspend_pwr_meas(gas_id_t gas_id)
{
   suspend_pwr_meas[gas_id] = TRUE;
}


/*===========================================================================

FUNCTION gl1_msg_resume_pwr_meas

DESCRIPTION
  This functions resumes the suspended power measurements.

DEPENDENCIES
  gl1_msg_cfg_power_meas must have been called at some time prior.

RETURN VALUE
  None

SIDE EFFECTS
  Power measurements are resumed
===========================================================================*/
void gl1_msg_resume_pwr_meas(gas_id_t gas_id)
{
   suspend_pwr_meas[gas_id] = FALSE;
}


/*===========================================================================
FUNCTION  gl1_msg_stop_pwr_meas

DESCRIPTION
  This API function provides the facility to stop the scheduling of power
  measures immediately.  The outstanding results from the mDSP will be
  collected and the results callback will be invoked with the measures
  done so far.

DEPENDENCIES
  On going power measure

RETURN VALUE
  None

SIDE EFFECTS
  Power measure scheduling is stopped

===========================================================================*/
void gl1_msg_stop_pwr_meas( gas_id_t gas_id )
{
   uint8 sm;
   boolean remove_handler = TRUE;

   MSG_GERAN_HIGH_0_G("DBG: Inside stop pwr meas");

   GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_STOP_PWR_MEAS, gas_id);

   /* Any state machine currently scheduling is moved to retrieve only
      mode.  All other state machines are simply stopped. */
   for (sm = 0; sm < 2; sm++)
   {
      switch (rxlev_SMs[gas_id][sm].pwr_meas_state)
      {
         case SM_PWR_MEAS_RECEIVED:
          MSG_GERAN_HIGH_0_G("DBG: pwr_meas_state is SM_PWR_MEAS_RECEIVED");
            L1_STATE_CHANGE(rxlev_SMs[gas_id][sm].pwr_meas_state = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");
            break;

         case SM_PWR_MEAS_SCHEDULING:
          MSG_GERAN_HIGH_0_G("DBG: pwr_meas_state is SM_PWR_MEAS_SCHEDULING");
            L1_STATE_CHANGE(rxlev_SMs[gas_id][sm].pwr_meas_state = SM_PWR_MEAS_RETRIEVING, "SM_PWR_MEAS_RETRIEVING");
            /*lint -fallthrough */

         case SM_PWR_MEAS_RETRIEVING:
          MSG_GERAN_HIGH_0_G("DBG: pwr_meas_state is SM_PWR_MEAS_RETRIEVING");
            remove_handler = FALSE;
            /*lint -fallthrough */

         case SM_PWR_MEAS_NOT_SCHEDULED:
          MSG_GERAN_HIGH_0_G("DBG: pwr_meas_state is SM_PWR_MEAS_SCHEDULED");
            MSG_GERAN_HIGH_1_G( "Skipped removal of gl1_msg_pwr_meas_hdlr ft hdlr sm=%d", sm);
            break;
      }

      /* Set a flag indicating that we were stopped */
      rxlev_SMs[gas_id][sm].pwr_meas_stopped = TRUE;
   }

   MSG_GERAN_HIGH_1_G("DBG: remove handler value- %d", remove_handler);
   /* If both machines are idle remove the handler */
   if (remove_handler)
   {
      gl1_msgi_remove_ft_handler(gl1_msg_pwr_meas_hdlr,gas_id);



      /* Call the abort callback now if we removed the handler */
      if(rxlev_SMs[gas_id][0].pwr_meas_abort_cb)
      {
         MSG_GERAN_HIGH_0_G("DBG: sm 0 aborting called");
        rxlev_SMs[gas_id][0].pwr_meas_abort_cb(gas_id);
        rxlev_SMs[gas_id][0].pwr_meas_abort_cb = NULL;
      }

      /* Call the abort callback */
      if(rxlev_SMs[gas_id][1].pwr_meas_abort_cb)
      {
        MSG_GERAN_HIGH_0_G("DBG: sm 1 aborting called");
        rxlev_SMs[gas_id][1].pwr_meas_abort_cb(gas_id);
        rxlev_SMs[gas_id][1].pwr_meas_abort_cb = NULL;
      }
   }

}


/*=============================================================================
FUNCTION gl1_msg_abort_pwr_meas

DESCRIPTION
   This function is will terminate any ongoing RXLEV measurements.
   The results buffer may be partially filled with results taken
   until that point in time
   NOTE: You *must* reset the RF and MDSP after calling this function as
   it leaves them in a bad state.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_abort_pwr_meas (gas_id_t gas_id)
{
   int i;
   boolean remove_handler = FALSE;
   boolean abort_flag = FALSE;

   MSG_GERAN_HIGH_2_G("gl1_msg_abort_pwr_meas: sm_states %d, %d", rxlev_SMs[gas_id][0].pwr_meas_state, rxlev_SMs[gas_id][1].pwr_meas_state);

   GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_ABORT_PWR_MEAS, gas_id);

   /* Reset everything and remove the handler if necessary */
   for (i = 0; i < 2; i++)
   {
      /*clear out any previous flags*/
      rxlev_SMs[gas_id][i].meas_aborted_while_retrieving = FALSE;

      switch (rxlev_SMs[gas_id][i].pwr_meas_state)
      {
         case SM_PWR_MEAS_NOT_SCHEDULED:
            break;

         case SM_PWR_MEAS_RETRIEVING:
            /*aborted while we are in retrieving state??  flag this up
             * we would need a TRM release suspension based on this*/
            rxlev_SMs[gas_id][i].meas_aborted_while_retrieving = TRUE;

         case SM_PWR_MEAS_SCHEDULING:
            abort_flag = TRUE;
            /*lint -fallthrough */

         case SM_PWR_MEAS_RECEIVED:
            remove_handler = TRUE;
            MSG_GERAN_HIGH_2_G("Aborting power measures: state=%d, FN=%d", rxlev_SMs[gas_id][i].pwr_meas_state, GSTMR_GET_FN_GERAN(gas_id));
            L1_STATE_CHANGE(rxlev_SMs[gas_id][i].pwr_meas_state = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");
            break;
      }
   }

   /* Abort if necessary */
   if (abort_flag)
   {
      gl1_hw_pwr_meas_abort(gas_id);

      /* terminate int also, otehrwise int can pick the buffer index which has been re-set */
      gl1_msgi_int_meas_terminate(gas_id);
   }

   /* Remove handler if necessary */
   if (remove_handler)
   {
      gl1_msgi_remove_ft_handler(gl1_msg_pwr_meas_hdlr,gas_id);
      rxlev_SMs[gas_id][0].meas_in_progress = 0;
      rxlev_SMs[gas_id][1].meas_in_progress = 0;
      MSG_GERAN_HIGH_2_G("Decrement meas in progress SM1=%d SM2=%d ", rxlev_SMs[gas_id][0].meas_in_progress,rxlev_SMs[gas_id][1].meas_in_progress);
   }

   /* Call the abort callback */
   if(rxlev_SMs[gas_id][0].pwr_meas_abort_cb)
   {
     rxlev_SMs[gas_id][0].pwr_meas_abort_cb(gas_id);
     rxlev_SMs[gas_id][0].pwr_meas_abort_cb = NULL;
   }

   /* Call the abort callback */
   if(rxlev_SMs[gas_id][1].pwr_meas_abort_cb)
   {
     rxlev_SMs[gas_id][1].pwr_meas_abort_cb(gas_id);
     rxlev_SMs[gas_id][1].pwr_meas_abort_cb = NULL;
   }
}

/*===========================================================================

FUNCTION gl1_msg_pwr_meas_aborted_while_retrieving

DESCRIPTION
  This functions returns whether the last Pwr meas abort was done while it was in
  SM_PWR_MEAS_RETRIEVING state.

  DEPENDENCIES
    gl1_msg_abort_pwr_meas must have been called at some time prior.

  RETURN VALUE
    TRUE - if the last Pwr meas abort was done while it was in
               SM_PWR_MEAS_RETRIEVING state.
    FALSE - if not

  SIDE EFFECTS
    None

===========================================================================*/
boolean gl1_msg_pwr_meas_aborted_while_retrieving( gas_id_t gas_id )
{
   /*check if any of the 2 state machines was aborted while it was in SM_PWR_MEAS_RETRIEVING state
    * for this gas_id
    */
   return (rxlev_SMs[gas_id][0].meas_aborted_while_retrieving || rxlev_SMs[gas_id][1].meas_aborted_while_retrieving);
}

/*===========================================================================

FUNCTION gl1_msg_pwr_reset_meas_aborted_while_retrieving

DESCRIPTION
  This functions resets the flag meas_aborted_while_retrieving.

  DEPENDENCIES
    none

  RETURN VALUE
    none

  SIDE EFFECTS
    None

===========================================================================*/
void gl1_msg_pwr_reset_meas_aborted_while_retrieving( gas_id_t gas_id )
{
  rxlev_SMs[gas_id][0].meas_aborted_while_retrieving = FALSE;
  rxlev_SMs[gas_id][1].meas_aborted_while_retrieving = FALSE;
}

/*===========================================================================
FUNCTION gl1_ded_late_start

DESCRIPTION
   Inhibit power measurement if in dedicated and late in frame.

DEPENDENCIES
  None

RETURN VALUE
  number of measurements or 0 to inhibit.

SIDE EFFECTS
  None
===========================================================================*/

#define PWR_MEASURE_START_IS_LATE  (1850)

static uint32 gl1_ded_late_start(uint32 in_num_meas, gas_id_t gas_id)
{
    uint32 current_qs = gstmr_rd_qsymbol_count_geran(geran_map_gas_id_to_nas_id(gas_id));
    /*
    ** pwr meas in dedicated can take 2.8ms so do not start if Qs > 1850
    */
    if(    (l1_tsk_buffer[gas_id].l1_state == L1_DEDICATED_MODE)
        && ( current_qs > PWR_MEASURE_START_IS_LATE)
      )
    {
        gl1_dbg_late_count++;
        MSG_GERAN_ERROR_3_G("Late start, skip this pwr meas, FN=%d Qs=%d cnt=%d", GSTMR_GET_FN_GERAN(gas_id), current_qs, gl1_dbg_late_count);
        return 0;   /* inhibit this measurement */
    }
    else
    {
        return in_num_meas;
    }
}

/*=============================================================================
FUNCTION gl1_msg_pwr_meas_hdlr

DESCRIPTION
   This function is invoked by gl1_msg_frame_tick_notification on each frame
   tick when registered.  This function processes the relev measurement state
   machines by scheduling the measures each frame and retrieving the results
   when ready in mDSP memory.

DEPENDENCIES
   None

RETURN VALUE
   TRUE to remove the handler, FALSE otherwise

SIDE EFFECTS
   None

============================================================================ */
boolean gl1_msg_pwr_meas_hdlr( boolean start_of_block, uint32 time_stamp,gas_id_t gas_id )
{
   uint32   num_meas, sm, other_sm, i, max_num_meas, meas_remaining;
   sm_pwr_meas_type  *sm_ptr, *other_sm_ptr;
   static uint32 ubs_sm[NUM_GERAN_DATA_SPACES] =
                    { INITIAL_VALUE( 0 ) };

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
    byte l1_state = l1_tskisr_blk->l1_state;
    idle_data_T      *l1_idle_data=&l1_idle_data_store[gas_id];
    sys_band_T  serving_cell_band, ncell_band;
    boolean interband_cell_detected = FALSE;

     if ( gl1_drdsds_enabled( gas_id ) )
     {
       /*capture serving cell band this is to check if we are making non-serving band measurements in the next three blocks.
        Non-serving cell band measurements must be checked with TRM before proceeding when DR-DSDS is enabled. RF tuning has an impact on the other tech
       */
       serving_cell_band = gl1_get_band(l1_idle_data->campedon_cell_ARFCN);
     }
#endif

   /* Cycle through the two state machines starting with the current one.
      We start there because otherwise in certain cases of back-to-back
      pwr measures we could miss a frame */
   sm = cur_sm[gas_id];

   /* if previous frame was an unexpected block start, switch state machines now */
   if (unexpected_block_start[gas_id]) /* we should wait for one extra frame for hmsc */
   {
      unexpected_block_start[gas_id] = 0;

      /* new sm becomes current */
      sm = cur_sm[gas_id] = ubs_sm[gas_id];
      sm_ptr = &rxlev_SMs[gas_id][sm];

      other_sm = (sm == 0 ? 1 : 0);
      other_sm_ptr = &rxlev_SMs[gas_id][other_sm];

      if (
         (other_sm_ptr->pwr_meas_state > SM_PWR_MEAS_RECEIVED)    /* power msr results still pending */
         && (other_sm_ptr->pwr_meas_cb == sm_ptr->pwr_meas_cb))   /* other sm still same as us */
      {
         /* make the old SM as done */
         L1_STATE_CHANGE(other_sm_ptr->pwr_meas_state = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");

         GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_UBS_STOP_PWR_MEAS, gas_id);
      }
   }

   for (i = 0; i < 2; i++)
   {
      /* Save a shortcut pointer */
      sm_ptr = &rxlev_SMs[gas_id][sm];

      switch (sm_ptr->pwr_meas_state)
      {
         case SM_PWR_MEAS_NOT_SCHEDULED:
         {
            break;
         }

         case SM_PWR_MEAS_RECEIVED:
         {
            /* Wait until block start */
            if (!start_of_block)
            {
               break;
            }

            /* Check that the other state machine is not already active */
            other_sm_ptr = &rxlev_SMs[gas_id][sm == 0 ? 1 : 0];
            if ((other_sm_ptr->pwr_meas_state > SM_PWR_MEAS_RECEIVED) &&
                (other_sm_ptr->pwr_meas_cb == sm_ptr->pwr_meas_cb))
            {
               /*unexpected block start, so store the next state machine index and only
                 switch SMs on the next frame, thereby allowing the current state machine
                 one more frame to collect the last measurement */
               MSG_GERAN_ERROR_1_G("Unexpected block start, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
               unexpected_block_start[gas_id]++;
               ubs_sm[gas_id] = sm;
            }
            else
            {
               /*  In GBTA , as the results of power mon are not yet retrieved, do not
                *  switch cur_sm to allow in next frame cur_sm(gbta) to run and fetch results.
                */
               if ( (other_sm_ptr->pwr_meas_cb != NULL) &&
                    (other_sm_ptr->pwr_meas_state > SM_PWR_MEAS_RECEIVED) &&
                    (other_sm_ptr->pwr_meas_cb != sm_ptr->pwr_meas_cb))
               {
                  MSG_GERAN_MED_4_G("Pwr Meas DBG do not switch cur_sm, FN=%d sm %d cur_sm %d other_sm_ptr->pwr_meas_cb %0x",
                     GSTMR_GET_FN_GERAN(gas_id),sm,cur_sm[gas_id],other_sm_ptr->pwr_meas_cb);
               }
               else
               {
                  /* We are now the current state machine */
                  cur_sm[gas_id] = sm;
                  MSG_GERAN_LOW_3_G("Pwr Meas DBG Switch cur_sm, FN=%d sm %d new_cur_sm %d", GSTMR_GET_FN_GERAN(gas_id),sm,cur_sm[gas_id]);
               }
            }

            /* advance to SCHEDULING state */
            L1_STATE_CHANGE(sm_ptr->pwr_meas_state = SM_PWR_MEAS_SCHEDULING, "SM_PWR_MEAS_SCHEDULING");

            /*lint -fallthrough to schedule first frame */
         }

         /*lint !e616*/ /* valid fall through case */
         case SM_PWR_MEAS_SCHEDULING:
         {
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
     // In case of GPLT and no RF, return default/canned result
            MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_pwr_meas_hdlr: Not calling retrieve_data");
#else
            /* Pick up any ready measurements */
            retrieve_data(sm_ptr, time_stamp,gas_id);
#endif /*(defined(FEATURE_GL1_GPLT))*/

            /* Set up info for this ARFCN to pass to frame layer */
            max_num_meas = num_meas = 0;
            if (!suspend_pwr_meas[gas_id])
            {
               meas_remaining = sm_ptr->num_arfcns - sm_ptr->arfcn_sched_index;
               max_num_meas = MIN(sm_ptr->max_meas_per_frame, meas_remaining);
            }

            /*
            ** mitigate late start in dedicated to prevent overruns
            */
            max_num_meas = gl1_ded_late_start(max_num_meas, gas_id);

            /* Issue the command if there are any to schedule */
            if (max_num_meas > 0)
            {
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )

             ncell_band =  gl1_get_band(sm_ptr->arfcns[sm_ptr->arfcn_sched_index]);

             /*if G is performing PM's on non-serving cell bands, the bands must be registered with TRM. TRM may deny the request
               This is OK as denial will cause arbitrator to skip the PM's on denied bands*/
             /*Band registration to be done only in DRDS in Idle / Transfer Mode*/
             
             if ( gl1_drdsds_enabled( gas_id ) && ((l1_state == L1_MULTI_SIM_IDLE)|| (l1_state == L1_TRANSFER_MODE) ) &&
                  ( gpl1_get_multi_sim_bplmn_state(gas_id) != L1_MULTI_SIM_BGRND_HPLMN_RUNNING ) )
             {
               /*loop through */
               for(i = 0; i < max_num_meas; i++ )
               {
                 ncell_band =  gl1_get_band(sm_ptr->arfcns[sm_ptr->arfcn_sched_index+i]);

                 if ( gl1_drdsds_enabled( gas_id ) && 
                    (serving_cell_band != ncell_band) && 
                    !grm_is_band_registered(ncell_band,l1_tskisr_blk->client_id,gas_id))
                 {
                   /*For power monitors we don't check the result we just need to update the list of denied bands. PM arbitrator will then check if PM is OK
                     or needs to be skipped due to band constraints*/
                   /*max_num_meas is 1 always, except in Idle case where it can be 5/6*/

                   /*here we add each band to current band info if different to the serving cell band*/
                   grm_add_sys_band_type_to_trm_freq_input_type(gl1_map_gsm_band_to_sys_band(ncell_band),
                                                                l1_tskisr_blk->client_id,
                                                                SC_PM,
                                                                gas_id);
                   interband_cell_detected = TRUE;
                 }
               }

               /*once we have processed all the arfcns for this frame then make one IFS request for all interbands*/
               if(interband_cell_detected)
               {
                  /*If state is Idle then PM can take 3 frames , and in Xfer PM can take 2 frames*/
                  if (l1_state == L1_MULTI_SIM_IDLE)
                  {

                    (void)grm_check_ifs_band_reg(SC_PM, SYS_BAND_NONE, GRM_FRM_REQ_PM_IDLE, DIVERSITY_FALSE, gas_id);
                  }
                  else
                  {

                    (void)grm_check_ifs_band_reg(SC_PM, SYS_BAND_NONE, GRM_FRM_REQ_PM_XFER, DIVERSITY_FALSE, gas_id);
                  }

#ifdef FEATURE_PERSISTENT_DENIAL_DEBUG
                 grm_denied_arfcn_processing(&sm_ptr->arfcns[sm_ptr->arfcn_sched_index],
                                           max_num_meas,
                                           GRM_COUNT_DENIED_ARFCNS,
                                           l1_tskisr_blk->client_id,
                                           gas_id);
#endif

               }

             }


#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

               num_meas = gl1_hw_pwr_meas_cmd
                (
                  &sm_ptr->arfcns[sm_ptr->arfcn_sched_index],
                  &sm_ptr->agc_ptrs[sm_ptr->arfcn_sched_index],
                  max_num_meas,
                  &sm_ptr->tags[sm_ptr->hist_wr * MAX_MEAS_PER_FRAME],
                  gas_id,
                  sm_ptr->arfcn_sched_index
                );
            }

            if (num_meas > 0)
            {
               sm_ptr->meas_in_progress++;
               sm_ptr->arfcn_sched_index += num_meas;
            }
            else
            {
               MSG_GERAN_MED_4_G("SCE-WARN: gl1_hw_pwr_meas_cmd() failed: FN=%d, num_meas=%d, max_num_meas=%d, sm=%d",
                  GSTMR_GET_FN_GERAN(gas_id), num_meas, max_num_meas, sm);
            }

            if(sm_ptr->meas_in_progress>3)
            {
               MSG_GERAN_HIGH_1_G("SCE: WARN: meas_in_progress (%d) > 3", sm_ptr->meas_in_progress);
            }

            /* Save the history */
            sm_ptr->num_meas_hist[sm_ptr->hist_wr] = num_meas;
            sm_ptr->num_attempts[sm_ptr->hist_wr]  = 0;
            sm_ptr->second_attempt[sm_ptr->hist_wr] = FALSE;
            /* used for counting num_attempts */
            sm_ptr->FN_retreive_attempted[sm_ptr->hist_wr] = GSTMR_GET_FN_GERAN( gas_id);
            INC_QUEUE_INDEX(sm_ptr->hist_wr);

            /* Keep track of how many frames have had measurements */
            sm_ptr->frame_count++;

            L1_SCE_DBG_MDSP_GPRS_UPDATE_GL1_PARAMS(L1_SEND_PWR_CMD_SCE_DBG, sm_ptr, gas_id);

            /* Check if it is time to stop scheduling either because there
               are no more arfcns or we are out of frames */
            if (sm_ptr->arfcn_sched_index == sm_ptr->num_arfcns ||
                sm_ptr->frame_count == sm_ptr->max_frames_to_measure)
            {
               /* Check that all arfcns have been scheduled */
               if (sm_ptr->num_arfcns > sm_ptr->arfcn_sched_index)
               {
                  MSG_GERAN_MED_2_G("Not all monitors scheduled, requested: %d, actual: %d",sm_ptr->num_arfcns,sm_ptr->arfcn_sched_index);
               }

               /* If there are pending measures, we keep retrieving */
               if (sm_ptr->meas_in_progress > 0)
               {
                  L1_STATE_CHANGE(sm_ptr->pwr_meas_state = SM_PWR_MEAS_RETRIEVING, "SM_PWR_MEAS_RETRIEVING");
               }
               /* Otherwise we must have suspended and are ready to go */
               else
               {
                  report_results(sm_ptr,gas_id);
                  L1_STATE_CHANGE(sm_ptr->pwr_meas_state = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");

                  GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_PENDING_STOP_PWR_MEAS, gas_id);
               }
            }

            break;
         }

         case SM_PWR_MEAS_RETRIEVING:
         {
            /* Retrieve any measures */
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
            // In case of GPLT and no RF, return default/canned result
            sm_ptr->meas_in_progress = 0;
            MSG_GERAN_HIGH_0_G("GPLT DBG: gl1_msg_pwr_meas_hdlr: Making sm_ptr->meas_in_progress = 0");
#else
            retrieve_data(sm_ptr, time_stamp,gas_id);
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

            /* If there is nothing left, then we are done */
            if (sm_ptr->meas_in_progress == 0)
            {
               report_results(sm_ptr,gas_id);
               L1_STATE_CHANGE(sm_ptr->pwr_meas_state = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");

               GL1_MSG_PWR_MEAS_DBG_NEW_ENTRY(GL1_MSG_PWR_MEAS_DBG_CMD_DONE_PWR_MEAS, gas_id);
            }
            else if (sm_ptr->pwr_meas_stopped)
            {
              MSG_GERAN_ERROR_1_G("pwr_meas_abort_cb not aborted as meas_in_progress %d", sm_ptr->meas_in_progress);
            }
            break;
         }

         default:
         {
            /* Error state, clear out the measure */
            MSG_GERAN_ERROR_1_G("pwr meas sm bad state: %d", sm_ptr->pwr_meas_state);
            L1_STATE_CHANGE(sm_ptr->pwr_meas_state = SM_PWR_MEAS_NOT_SCHEDULED, "SM_PWR_MEAS_NOT_SCHEDULED");
            break;
         }

      } /* switch */

      /* Switch to process the other state machine */
      sm = !sm;

   } /* for */

   return (!gl1_msg_meas_in_progress(gas_id));
}


/*=============================================================================
FUNCTION retrieve_data

DESCRIPTION
   This function is invoked by gl1_msg_pwr_meas_hdlr to get the measurement
   results from mDSP.

DEPENDENCIES
   None

RETURN VALUE
   Returns if there is any data left to retrieve anymore

SIDE EFFECTS
   None

============================================================================ */
static void retrieve_data
(
  sm_pwr_meas_type *sm_ptr,
  uint32 time_stamp,
  gas_id_t gas_id
)
{
   uint32                i, index, expected_results, retrieved_results;
   gl1_hw_measurement_T  measurements[MAX_MEAS_PER_FRAME];
   boolean               results_avail;
   boolean               valid;
   uint32                num_iterations;
   uint32                FN_cur;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#if defined ( FEATURE_PERSISTENT_DENIAL_DEBUG ) || defined ( FEATURE_GSM_QSC_TX_DIV )
   volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
#endif
#endif // FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
   idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
   boolean               store_ant_meas_result;
   gas_id_t        idle_gas_id;
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

#ifdef GERAN_L1_HLLL_LNASTATE
   gl1_defs_metrics_lnastate gl1_metrics_lnastate;
   uint8 SawlessHLLLState[MAX_NUMBER_OF_CHANNELS] = {0};
#endif /*GERAN_L1_HLLL_LNASTATE*/


   /* inc num of attempts for reading power monitor results */
   FN_cur = GSTMR_GET_FN_GERAN(gas_id);
   for(i=0; i<HIST_QUEUE_SIZE; i++)
   {
      if(sm_ptr->num_meas_hist[i])
      {
         /* some power monitor is scheduled */
         if(sm_ptr->FN_retreive_attempted[i] != FN_cur)
         {
            /* to avoid inc num_attemps in case where retrieve_data() is 
             * called multiple times from same FN 
             */
            sm_ptr->FN_retreive_attempted[i] = FN_cur;
            sm_ptr->num_attempts[i]++;

            /* discard this pwr monitor if we have tried many times */
            if(sm_ptr->num_attempts[i] >= MAX_FRAMES_TO_WAIT_BEFORE_DISCARDING_PWR_MON_RESULTS )
            {
               sm_ptr->second_attempt[i] = TRUE;
            }
         }
      }
   } /* for(i=0; i<HIST_QUEUE_SIZE; i++) */

 /* if we read results late, then at most 3 results can be available in fw (tripple buf) */
 for(num_iterations=0; (num_iterations<HIST_QUEUE_SIZE) && (sm_ptr->meas_in_progress > 0) ; num_iterations++)
 {
   /* If there are any results for this frame, get them */
   expected_results = sm_ptr->num_meas_hist[sm_ptr->hist_rd];
   if (expected_results > 0)
   {
#ifdef FEATURE_GSM_DED_SCELL_MONITOR
      /* if not dummy arfcn */
      if( sm_ptr->arfcns[sm_ptr->arfcn_fetch_index].num == DUMMY_ARFCN_FOR_SC_MEAS)
      {
         retrieved_results = 0;
         results_avail = TRUE;
      }
      else
#endif
      {
         /* read results from mdsp only if we have queued the power monitor */
         if( sm_ptr->second_attempt[sm_ptr->hist_rd]
            || !gl1_msg_int_retrieving_results(gas_id) )
         {
            /* Retrieve the measurements from the mdsp */
            results_avail = gl1_hw_pwr_meas_results
                            (
                              measurements,
                              expected_results,
                              &retrieved_results,
                              sm_ptr->second_attempt[sm_ptr->hist_rd],
                              &sm_ptr->tags[sm_ptr->hist_rd * MAX_MEAS_PER_FRAME],
                              gas_id
                            );
         }
         else
         {
            /* queued by another SM (interference measurement)*/
            retrieved_results = 0;
            results_avail = FALSE;
         }
      }

#ifdef FEATURE_GSM_SCE_PWR_MEAS_DBG
      {
         l1_sce_dbg_mdsp_gprs_T *pl1_sce_dbg_mdsp_gprs;

         pl1_sce_dbg_mdsp_gprs = l1_sce_dbg_mdsp_gprs_update_gl1_params(L1_GET_PWR_RESULTS_SCE_DBG, sm_ptr, gas_id);

         pl1_sce_dbg_mdsp_gprs->num_iterations = num_iterations;
         pl1_sce_dbg_mdsp_gprs->results_avail = results_avail;
         pl1_sce_dbg_mdsp_gprs->expected_results = expected_results;
         pl1_sce_dbg_mdsp_gprs->retrieved_results = retrieved_results;
         if (!results_avail && !sm_ptr->second_attempt[sm_ptr->hist_rd])
         {
            pl1_sce_dbg_mdsp_gprs->break_loop = TRUE;
         }
         else
         {
            pl1_sce_dbg_mdsp_gprs->break_loop = FALSE;
         }
      }
#endif

      /* try again later as results can be upto 3 frames late in hmsc */
      if (!results_avail && !sm_ptr->second_attempt[sm_ptr->hist_rd])
      {
         break;
      }

#ifdef GERAN_L1_HLLL_LNASTATE
      gl1_metrics_lnastate.NoOfChannels = 0;
#endif /*GERAN_L1_HLLL_LNASTATE*/

      /* Process the retrieved results */
      for (i = 0; i < retrieved_results; i++)
      {
         index = sm_ptr->arfcn_fetch_index++;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
#ifdef FEATURE_PERSISTENT_DENIAL_DEBUG
         /*process ARFCN results against denied band register*/
         grm_denied_arfcn_processing(&sm_ptr->arfcns[index],
                                     0,
                                     GRM_PROCESS_DENIED_ARFCNS,
                                     l1_tskisr_blk->client_id,
                                     gas_id);
#endif
#endif

         /*Only process data if there's no conflict on the pwr meas*/
         if(!measurements[i].yield)
         {

           /* Update the AGC structures */
           valid = (measurements[i].status == RF_IN_RANGE);
           sys_algo_agc_update
           (
              sm_ptr->agc_ptrs[index],
              measurements[i].dBm_x16,
              valid,
              time_stamp,
              gas_id
           );

           /* Save the arfcn to the agc pointer for logging */
           sm_ptr->agc_ptrs[index]->arfcn = sm_ptr->arfcns[index];
           
		   gl1_hw_log_monitor
           (
             measurements[i].dBm_x16,
             measurements[i].rssi,
             measurements[i].rx_gain_range,
             sm_ptr->agc_ptrs[index]->arfcn,
             FALSE,
             FALSE,
             gas_id
           );
           MSG_GERAN_LOW_3_G("Good band/arfcn for PwrMeas retrieval: %d,%d, index=%d", sm_ptr->agc_ptrs[index]->arfcn.band, sm_ptr->agc_ptrs[index]->arfcn.num, i);
           /* Save the value directly as well */
           if (sm_ptr->pwr_meas_results->pwr_dBm_x16 != NULL)
           {
             sm_ptr->pwr_meas_results->pwr_dBm_x16[index] =
               measurements[i].dBm_x16;
           }
#ifdef GL1_GMSK_MOD_DET
            if( sm_ptr->pwr_meas_results->decstat!= NULL)
            {
		   	  sm_ptr->pwr_meas_results->decstat[index] = ((measurements[i].decstat) + (measurements[i].decstat2))/2 ;
#ifdef FEATURE_GSM_SYNQ_SUPPORT	
              if(gl1_hw_get_synq_fr_enabled())
              {		  
                l1_sc_run_synq_algo(sm_ptr->arfcns[index], sm_ptr->pwr_meas_results->decstat[index], gas_id);
              }
#endif
            }
#endif			  
#ifdef GERAN_L1_HLLL_LNASTATE
          if(gl1_hw_get_sawless_HLLL_support(sm_ptr->arfcns[index].band,gas_id))
            {
            if(i > MAX_NUMBER_OF_CHANNELS)
              {
              MSG_GERAN_ERROR_2_G("NoOfChannels %d greater than max number allowed:%d",i,MAX_NUMBER_OF_CHANNELS);
              }
            gl1_metrics_lnastate.NoOfChannels              = gl1_metrics_lnastate.NoOfChannels + 1;
            gl1_metrics_lnastate.gsm_metrics[i].arfcn      = sm_ptr->arfcns[index];
            gl1_metrics_lnastate.gsm_metrics[i].rx_lvl_dbm = measurements[i].dBm_x16;
            gl1_metrics_lnastate.gsm_metrics[i].snr        = INVALID_SNR;
            gl1_metrics_lnastate.gsm_metrics[i].jdet_value = measurements[i].jdet_value;
            SawlessHLLLState[i]                            = measurements[i].SawlessHLLLState;
            }
#endif /*GERAN_L1_HLLL_LNASTATE*/

           if ( sm_ptr->pwr_meas_results->yielded != NULL )
           {
             sm_ptr->pwr_meas_results->yielded[index] = FALSE;
           }
         }
         else
         {
           if( sm_ptr->agc_ptrs[index] != &l1_idle_data_store[gas_id].campedon_cell_agc )
           {
             sm_ptr->agc_ptrs[index]->valid = FALSE;
             sm_ptr->agc_ptrs[index]->arfcn = sm_ptr->arfcns[index];
             sm_ptr->agc_ptrs[index]->FN = GSTMR_GET_FN_GERAN( gas_id);
             sm_ptr->agc_ptrs[index]->pwr_dBm_x16 = RX_POWER_FLOOR;
           }
           else
           {
             MSG_GERAN_MED_0_G("No update to idle campedon cell agc" );
           }

           if (sm_ptr->pwr_meas_results->pwr_dBm_x16 != NULL)
           {
             sm_ptr->pwr_meas_results->pwr_dBm_x16[index] = RX_POWER_FLOOR;
           }

           if ( sm_ptr->pwr_meas_results->yielded != NULL )
           {
             sm_ptr->pwr_meas_results->yielded[index] = TRUE;
           }

           MSG_GERAN_MED_3_G("Band/Arfcn for yield is: %d,%d, index=%d", sm_ptr->agc_ptrs[index]->arfcn.band, sm_ptr->agc_ptrs[index]->arfcn.num, i);
         }
#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING

#if defined(FEATURE_GPRS_GBTA)
       store_ant_meas_result = ((!gl1_check_other_subs_idle_event_status(GBTA_STATUS_PWR_MON_SCHEDULED, gas_id,&idle_gas_id))&&(!measurements[i].yield));
#else
         store_ant_meas_result = (!measurements[i].yield);
#endif
         if(store_ant_meas_result)
         {

#ifdef FEATURE_GSM_QSC_TX_DIV
           if( (sm_ptr->arfcns[index].num == l1_idle_data->campedon_cell_ARFCN.num) && (l1_tskisr_blk->l1_state == L1_TRANSFER_MODE))
           {
             gl1_trm_asdiv_type2_trans_store_rx_pwr(gas_id, measurements[i].dBm_x16);
           }
#endif

           if ((sm_ptr->arfcns[index].num ==l1_idle_data->campedon_cell_ARFCN.num)&& (antenna_to_use[index]==2))
           {

             if ( antenna_to_use_for_tx [gas_id]== 1)
             {
               MSG_GERAN_HIGH_3_G ("serving cell meas result antenna 1 = %d, sm_ptr->arfcns[index].num = %d, index = %d", 
			                        measurements[i].dBm_x16/16, sm_ptr->arfcns[index].num, index);
               gl1_hw_asdiv_type1.ant_1_rssi+= measurements[i].dBm_x16;
               gl1_hw_asdiv_type1.number_of_meas_ant_1++;
             }
             else if ( antenna_to_use_for_tx[gas_id] == 2)
             {
               MSG_GERAN_HIGH_3_G ("serving cell meas result antenna 2 = %d, sm_ptr->arfcns[index].num = %d, index = %d", 
			                        measurements[i].dBm_x16/16, sm_ptr->arfcns[index].num, index);
               gl1_hw_asdiv_type1.ant_2_rssi += measurements[i].dBm_x16;
               gl1_hw_asdiv_type1.number_of_meas_ant_2++;
       }
           }
           else if((sm_ptr->arfcns[index].num ==l1_idle_data->campedon_cell_ARFCN.num)&& (antenna_to_use[index]==1))
           {
             if ( antenna_to_use_for_tx[gas_id] == 1)
             {
               MSG_GERAN_HIGH_3_G ("serving cell meas result antenna 2 = %d, sm_ptr->arfcns[index].num = %d, index = %d",
 			                        measurements[i].dBm_x16/16, sm_ptr->arfcns[index].num, index);
               gl1_hw_asdiv_type1.ant_2_rssi += measurements[i].dBm_x16;
               gl1_hw_asdiv_type1.number_of_meas_ant_2++;
             }
             else if ( antenna_to_use_for_tx [gas_id]== 2)
             {
               MSG_GERAN_HIGH_3_G ("serving cell meas result antenna 1 = %d, sm_ptr->arfcns[index].num = %d, index = %d", 
			                        measurements[i].dBm_x16/16, sm_ptr->arfcns[index].num, index);
               gl1_hw_asdiv_type1.ant_1_rssi += measurements[i].dBm_x16;
               gl1_hw_asdiv_type1.number_of_meas_ant_1++;
       }
           }
         }
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING  */
      }

      /* Clean up any extra results that we did not get */
      for (i = retrieved_results; i < expected_results; i++)
      {
         index = sm_ptr->arfcn_fetch_index++;

         if( sm_ptr->agc_ptrs[index] != &l1_idle_data_store[gas_id].campedon_cell_agc )
         {
           sm_ptr->agc_ptrs[index]->valid = FALSE;
           sm_ptr->agc_ptrs[index]->arfcn = sm_ptr->arfcns[index];
           sm_ptr->agc_ptrs[index]->FN = GSTMR_GET_FN_GERAN( gas_id);
           sm_ptr->agc_ptrs[index]->pwr_dBm_x16 = RX_POWER_FLOOR;
         }
         else
         {
           MSG_GERAN_MED_0_G("No update to idle campedon cell agc" );
         }

         if ( sm_ptr->pwr_meas_results->yielded != NULL )
         {
           sm_ptr->pwr_meas_results->yielded[index] = TRUE;
         }

         /* Save our best guess at the value */
         if (sm_ptr->pwr_meas_results->pwr_dBm_x16 != NULL)
         {
            sm_ptr->pwr_meas_results->pwr_dBm_x16[index] =
              sm_ptr->agc_ptrs[index]->pwr_dBm_x16;
         }
#ifdef GL1_GMSK_MOD_DET
		 if( sm_ptr->pwr_meas_results->decstat!= NULL)
         {
		   	  sm_ptr->pwr_meas_results->decstat[index] = (measurements[index].decstat) + (measurements[index].decstat2) ;
			 
         }
#endif
#ifdef FEATURE_GSM_DED_SCELL_MONITOR
         /* if not dummy arfcn */
         if( sm_ptr->arfcns[index].num == DUMMY_ARFCN_FOR_SC_MEAS)
         {
            /* don't print anything as we don't expect to receive pwr msr results in this case */
         }
         else
#endif
         {
            MSG_GERAN_MED_2_G("SCE: WARN: Pwr meas results not retrieved, expected %d, retrieved %d",expected_results, retrieved_results);
         }

      }

      sm_ptr->pwr_meas_results->num_pwr_meas += expected_results;
      sm_ptr->num_meas_hist[sm_ptr->hist_rd] = 0;
      sm_ptr->meas_in_progress--;

#ifdef GERAN_L1_HLLL_LNASTATE
      if(gl1_metrics_lnastate.NoOfChannels)
        {
        gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
        gl1_hw_log_HLin_LLin_metrics(&gl1_metrics_lnastate,SawlessHLLLState,gas_id);
        }
#endif /*GERAN_L1_HLLL_LNASTATE*/
   }

   /* Inc the queue reading index */
   INC_QUEUE_INDEX(sm_ptr->hist_rd);
 } /* for(num_iterations=0; (num_iterations<TRIPPLEBUF) && (sm_ptr->meas_in_progress > 0) ; num_iterations++) */

}


/*===========================================================================
FUNCTION report_results

DESCRIPTION
   This function returns the measurement results to the upper layer.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static void report_results (sm_pwr_meas_type *sm_ptr,gas_id_t gas_id)
{

   /* If we have no callback installed, nothing to do */
   if (sm_ptr->pwr_meas_cb == NULL)
   {
      return;
   }

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
  // In case of GPLT and no RF, return default/canned result
  MSG_GERAN_HIGH_0_G("GPLT DBG: report_results: Returning power scan results");
  sm_ptr->agc_ptrs[0]->pwr_dBm_x16 = gplt_scan_power_data.pwr_dBm_x16;
  sm_ptr->pwr_meas_results->num_pwr_meas = gplt_scan_power_data.num_pwr_meas;
  sm_ptr->pwr_meas_stopped = FALSE;
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   /* Return memory to caller */
   sm_ptr->pwr_meas_results->arfcns     = sm_ptr->arfcns;
   sm_ptr->pwr_meas_results->agc_ptrs   = sm_ptr->agc_ptrs;
   sm_ptr->pwr_meas_results->num_arfcns = sm_ptr->arfcn_sched_index;
   sm_ptr->pwr_meas_results->stopped    = sm_ptr->pwr_meas_stopped;

   /* Send results to PL1 */
   sm_ptr->pwr_meas_cb(sm_ptr->pwr_meas_results,gas_id);

   /* If we were stopped, send the abort callback too */
   if (sm_ptr->pwr_meas_stopped && sm_ptr->pwr_meas_abort_cb)
   {
      sm_ptr->pwr_meas_abort_cb(gas_id);
   }

   sm_ptr->pwr_meas_cb       = NULL ; // Nullify After Use.
   sm_ptr->pwr_meas_abort_cb = NULL; // Nullify After Use.
}

/*===========================================================================

FUNCTION gl1_msg_get_meas_in_progress

DESCRIPTION
   returns if meas_in_progress is true for any of the state machines

DEPENDENCIES
  None

RETURN VALUE
  TRUE if any of the state machine has meas_in_progress

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_get_meas_in_progress(gas_id_t gas_id)
{
  return ( rxlev_SMs[gas_id][0].meas_in_progress || rxlev_SMs[gas_id][1].meas_in_progress );
}

#ifdef FEATURE_GSM_TX_DIV_ANTNA_SWITCHING
/*===========================================================================

FUNCTION start_tx_div_antenna_alg

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
void        stored_dtx_indicator

SIDE EFFECTS
  None
===========================================================================*/

void start_tx_div_antenna_alg( boolean packet_trans, gas_id_t gas_id)
{

  int16 imbl_ULDL = gl1_hw_asdiv_mtpl_store[gas_id].imb_ULDL;
#ifdef FEATURE_GERAN_SAR 
    if(gl1_hw_check_rf_switch_disable(gas_id))
  {
    gl1_hw_reset_typ1_asdiv_switch_algorithm(gas_id);
    return;
  }
#endif
  if(gl1_hw_asdiv_type1.wait_count<= 0)
  {
    gl1_hw_asdiv_type1.start_alg = TRUE;
  }
  else
  {
    gl1_hw_asdiv_type1.start_alg = FALSE;
  }
  MSG_GERAN_HIGH_5_G("start type1 algorithm =%d delay_for_switching =%d antenna_1_rssi =%d antenna_2_rssi =%d number_of_meas =%d",
                      gl1_hw_asdiv_type1.start_alg,
					  gl1_hw_asdiv_type1.wait_count, 
					  gl1_hw_asdiv_type1.ant_1_rssi/16,
					  gl1_hw_asdiv_type1.ant_2_rssi/16,
					  gl1_hw_asdiv_type1.number_of_meas_ant_1);

  if((gl1_hw_asdiv_type1.number_of_meas_ant_2 ==4)&&(gl1_hw_asdiv_type1.number_of_meas_ant_1 ==4))
  {

    if(gl1_hw_asdiv_type1.ant_1_rssi < 0 )
    {
      gl1_hw_asdiv_type1.ant_1_avg_rssi= gl1_hw_asdiv_type1.ant_1_rssi/gl1_hw_asdiv_type1.number_of_meas_ant_1;
    }

    if(gl1_hw_asdiv_type1.ant_2_rssi < 0 )
    {
      gl1_hw_asdiv_type1.ant_2_avg_rssi = gl1_hw_asdiv_type1.ant_2_rssi/gl1_hw_asdiv_type1.number_of_meas_ant_2;
    }

	gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch = 
    gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_accum/gl1_hw_asdiv_mtpl_store[gas_id].number_of_tx_pwr_avg;

    MSG_GERAN_HIGH_5_G("UL_delta = %d, DL_delta = %d, imbl_ULDL = %d, mtpl_antenna_1 = %d, mtpl_antenna_2 = %d", 
                       gl1_hw_asdiv_mtpl_store[gas_id].UL_delta,
                       gl1_hw_asdiv_mtpl_store[gas_id].DL_delta,
                       imbl_ULDL,
                       gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1,
                       gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2);

	/*if mtpl is not a legit value, set it to 40 dBm so the Tx condition will be bypass in the algorithm*/
    if(gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1 <= 250 )
    {
      gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1 = 400;
    }
	if(gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2 <= 250 )
    {
      gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2 = 400;
    }    

    MSG_GERAN_HIGH_4_G("antenna1_avg_rssi =%d antenna2_avg_rssi =%d tx_pwr_avg_over_sacch =%d with number %d",
	                 gl1_hw_asdiv_type1.ant_1_avg_rssi/16,gl1_hw_asdiv_type1.ant_2_avg_rssi/16,gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch,
	                 gl1_hw_asdiv_mtpl_store[gas_id].number_of_tx_pwr_avg);


    gl1_hw_type1_switch_decision(tx_div_min_delta_btw_antenna, imbl_ULDL, packet_trans, FALSE, gas_id);
    
    /* no switch in first 4 monitors */
    if(gl1_hw_asdiv_type1.start_alg)
    {

      if(gl1_hw_asdiv_type1.attempt_with_lower_delta == 1 )
      {
        /* averaging over 8 monitors */
        gl1_hw_asdiv_type1.ant_1_avg_rssi = ((gl1_hw_asdiv_type1.ant_1_avg_rssi +antenna_avg_backup.antenna1_avg_rssi_bckup1 )/2);
        gl1_hw_asdiv_type1.ant_2_avg_rssi = ((gl1_hw_asdiv_type1.ant_2_avg_rssi +antenna_avg_backup.antenna2_avg_rssi_bckup1 )/2);
        MSG_GERAN_HIGH_2_G("  average over 8 monitors  antenna1_avg_rssi = %d antenna2_avg_rssi =%d ",gl1_hw_asdiv_type1.ant_1_avg_rssi/16,gl1_hw_asdiv_type1.ant_2_avg_rssi/16);

        gl1_hw_type1_switch_decision(48, imbl_ULDL, packet_trans, TRUE, gas_id);

        if(gl1_hw_asdiv_type1.start_alg)
        {
          MSG_GERAN_HIGH_0_G("failed with lower delta 1st time");
        }

      }

      if(gl1_hw_asdiv_type1.attempt_with_lower_delta == 2 )
      {
        gl1_hw_asdiv_type1.ant_1_avg_rssi = ((gl1_hw_asdiv_type1.ant_1_avg_rssi +antenna_avg_backup.antenna1_avg_rssi_bckup1
                                             +antenna_avg_backup.antenna1_avg_rssi_bckup2 )/3);
        gl1_hw_asdiv_type1.ant_2_avg_rssi = ((gl1_hw_asdiv_type1.ant_2_avg_rssi +antenna_avg_backup.antenna2_avg_rssi_bckup1
                                             +antenna_avg_backup.antenna2_avg_rssi_bckup2 )/3);

        MSG_GERAN_HIGH_2_G("average over 12 monitors  antenna1_avg_rssi = %d antenna2_avg_rssi =%d ",gl1_hw_asdiv_type1.ant_1_avg_rssi/16,gl1_hw_asdiv_type1.ant_2_avg_rssi/16);

        gl1_hw_type1_switch_decision(32, imbl_ULDL, packet_trans, TRUE, gas_id); 
        
        if(gl1_hw_asdiv_type1.start_alg)
        {
          MSG_GERAN_HIGH_0_G("failed with lower delta 2nd time");
        }
      }
	  
      if(gl1_hw_asdiv_type1.attempt_with_lower_delta == 5 )
      {
        gl1_hw_asdiv_type1.ant_1_avg_rssi = ((gl1_hw_asdiv_type1.ant_1_avg_rssi +antenna_avg_backup.antenna1_avg_rssi_bckup1
                                             +antenna_avg_backup.antenna1_avg_rssi_bckup2
                                             +antenna_avg_backup.antenna1_avg_rssi_bckup3
                                             +antenna_avg_backup.antenna1_avg_rssi_bckup4
                                             +antenna_avg_backup.antenna1_avg_rssi_bckup5 )/6);
        gl1_hw_asdiv_type1.ant_2_avg_rssi = ((gl1_hw_asdiv_type1.ant_2_avg_rssi +antenna_avg_backup.antenna2_avg_rssi_bckup1
                                             +antenna_avg_backup.antenna2_avg_rssi_bckup2
                                             +antenna_avg_backup.antenna2_avg_rssi_bckup3
                                             +antenna_avg_backup.antenna2_avg_rssi_bckup4
                                             +antenna_avg_backup.antenna2_avg_rssi_bckup5 )/6);

        MSG_GERAN_HIGH_2_G("average over 24 monitors  antenna1_avg_rssi = %d antenna2_avg_rssi =%d ",gl1_hw_asdiv_type1.ant_1_avg_rssi/16,gl1_hw_asdiv_type1.ant_2_avg_rssi/16);

        gl1_hw_type1_switch_decision(16, imbl_ULDL, packet_trans, TRUE, gas_id);

        if(gl1_hw_asdiv_type1.start_alg)
		{
		  MSG_GERAN_HIGH_0_G("failed with lower delta 5th time");
		}

      }

      MSG_GERAN_HIGH_1_G("number_of_attempt_with_lower_delta = %d ",gl1_hw_asdiv_type1.attempt_with_lower_delta);
      gl1_hw_asdiv_type1.attempt_with_lower_delta++;
      gl1_hw_push_antenna_avg_fifo();
     }

	 if((!gl1_hw_asdiv_type1.start_alg)||gl1_hw_asdiv_type1.attempt_with_lower_delta >= 6 )
     {
       gl1_hw_asdiv_type1.attempt_with_lower_delta =0;
       MSG_GERAN_HIGH_0_G("clearing all monitor meas backup");
       gl1_hw_clear_antenna_avg_fifo();
     }

     gl1_hw_clear_antenna_meas(gas_id);

  }
  else
  {
    gl1_hw_clear_antenna_meas (gas_id);
    MSG_GERAN_HIGH_0_G("invalid num of meas");
  }
  if(!packet_trans)
  {

    if (gl1_hw_asdiv_type1.wait_count > -6 )
    {
      gl1_hw_asdiv_type1.wait_count -- ;
    }

  }

}


/*===========================================================================

FUNCTION gl1_hw_type1_switch_decision

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
void

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_type1_switch_decision(int16 thresh, int16 imbl, boolean packet_trans, boolean lower_delta, gas_id_t gas_id)
{
  if((antenna_to_use_for_tx[gas_id] == 1)&&(gl1_hw_asdiv_type1.start_alg))
  {
    if((gl1_hw_asdiv_type1.ant_2_avg_rssi - gl1_hw_asdiv_type1.ant_1_avg_rssi >= (thresh + imbl))&&
	  (((gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch - (gl1_hw_asdiv_type1.ant_2_avg_rssi - gl1_hw_asdiv_type1.ant_1_avg_rssi)*5/8)< gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2)
		||(gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1 <= gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2))
	  )
    {
	  gl1_hw_switch_antenna_tx_div(gas_id);
	  antenna_to_use_for_tx[gas_id]= 2;
      gl1_hw_asdiv_type1.wait_count = 6 ;    // set delay for further switch

	  if(packet_trans)
	  {
	    gl1_hw_asdiv_type1.wait_count = 49;
	  }
	  
	  MSG_GERAN_HIGH_0_G("switch tx to antenna 2");
	  gl1_hw_asdiv_type1.start_alg = FALSE;

	  if(lower_delta)
	  {
	    gl1_hw_asdiv_type1.attempt_with_lower_delta = -1;
	  }
	  else
	  {
	    gl1_hw_asdiv_type1.attempt_with_lower_delta = 0;
	  }
    }
    else
    {
	  MSG_GERAN_HIGH_0_G("keep using antenna 1");
    }
    MSG_GERAN_HIGH_1_G("antenna_to_use_for_tx = %d",antenna_to_use_for_tx[gas_id]);
  }
  else if ((antenna_to_use_for_tx[gas_id] == 2)&&(gl1_hw_asdiv_type1.start_alg))
  {
    if ((gl1_hw_asdiv_type1.ant_1_avg_rssi - gl1_hw_asdiv_type1.ant_2_avg_rssi >= (thresh - imbl))||
		 ((gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch > (gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2 - 20))&&
		   (((gl1_hw_asdiv_type1.ant_2_avg_rssi - gl1_hw_asdiv_type1.ant_1_avg_rssi)*5/8) < (gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_1 - gl1_hw_asdiv_mtpl_store[gas_id].mtpl_antenna_2))) 
	   )
	{
	  gl1_hw_switch_antenna_tx_div (gas_id);
	  antenna_to_use_for_tx[gas_id] = 1;
      gl1_hw_asdiv_type1.wait_count = 6 ;	 // set delay for further switch

	  if(packet_trans)
	  {
	    gl1_hw_asdiv_type1.wait_count = 49 ;
	  }

	  MSG_GERAN_HIGH_0_G("switch tx to antenna 1");
	  gl1_hw_asdiv_type1.start_alg = FALSE;

	  if(lower_delta)
	  {
	    gl1_hw_asdiv_type1.attempt_with_lower_delta = -1;
	  }
	  else
	  {
	    gl1_hw_asdiv_type1.attempt_with_lower_delta = 0;
	  }	
	}
	else
	{
      MSG_GERAN_HIGH_0_G("keep using antenna 2");
	}
	MSG_GERAN_HIGH_1_G("antenna_to_use_for_tx = %d",antenna_to_use_for_tx[gas_id]);
  }

}

/*===========================================================================

FUNCTION gl1_hw_clear_antenna_meas

DESCRIPTION


DEPENDENCIES
  None

RETURN VALUE
void

SIDE EFFECTS
  None
===========================================================================*/



void gl1_hw_clear_antenna_meas (gas_id_t gas_id)
{
  gl1_hw_asdiv_type1.ant_1_rssi = 0;
  gl1_hw_asdiv_type1.ant_2_rssi = 0;
  gl1_hw_asdiv_type1.number_of_meas_ant_1 =0;
  gl1_hw_asdiv_type1.number_of_meas_ant_2 =0; 
  gl1_hw_asdiv_type1.ant_1_avg_rssi = 0;
  gl1_hw_asdiv_type1.ant_2_avg_rssi = 0;

  gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_accum = 0; 
  gl1_hw_asdiv_mtpl_store[gas_id].number_of_tx_pwr_avg = 0; 
  gl1_hw_asdiv_mtpl_store[gas_id].tx_pwr_avg_over_sacch = 0;
}

void gl1_hw_push_antenna_avg_fifo ()
{
antenna_avg_backup.antenna1_avg_rssi_bckup5 = antenna_avg_backup.antenna1_avg_rssi_bckup4;
antenna_avg_backup.antenna1_avg_rssi_bckup4 = antenna_avg_backup.antenna1_avg_rssi_bckup3;
antenna_avg_backup.antenna1_avg_rssi_bckup3 = antenna_avg_backup.antenna1_avg_rssi_bckup2;
antenna_avg_backup.antenna1_avg_rssi_bckup2 = antenna_avg_backup.antenna1_avg_rssi_bckup1;
antenna_avg_backup.antenna1_avg_rssi_bckup1 = gl1_hw_asdiv_type1.ant_1_avg_rssi;

antenna_avg_backup.antenna2_avg_rssi_bckup5 = antenna_avg_backup.antenna2_avg_rssi_bckup4;
antenna_avg_backup.antenna2_avg_rssi_bckup4 = antenna_avg_backup.antenna2_avg_rssi_bckup3;
antenna_avg_backup.antenna2_avg_rssi_bckup3 = antenna_avg_backup.antenna2_avg_rssi_bckup2;
antenna_avg_backup.antenna2_avg_rssi_bckup2 = antenna_avg_backup.antenna2_avg_rssi_bckup1;
antenna_avg_backup.antenna2_avg_rssi_bckup1 = gl1_hw_asdiv_type1.ant_2_avg_rssi;
}
void gl1_hw_clear_antenna_avg_fifo ()
{
antenna_avg_backup.antenna1_avg_rssi_bckup5 = 0;
antenna_avg_backup.antenna1_avg_rssi_bckup4 = 0;
antenna_avg_backup.antenna1_avg_rssi_bckup3 = 0;
antenna_avg_backup.antenna1_avg_rssi_bckup2 = 0;
antenna_avg_backup.antenna1_avg_rssi_bckup1 = 0;

antenna_avg_backup.antenna2_avg_rssi_bckup5 = 0;
antenna_avg_backup.antenna2_avg_rssi_bckup4 = 0;
antenna_avg_backup.antenna2_avg_rssi_bckup3 = 0;
antenna_avg_backup.antenna2_avg_rssi_bckup2 = 0;
antenna_avg_backup.antenna2_avg_rssi_bckup1 = 0;
}
/*===========================================================================

FUNCTION gl1_hw_reset_typ1_asdiv_switch_algorithm

DESCRIPTION
 Reset ASDIV algorithm

DEPENDENCIES
  None

RETURN VALUE
void

SIDE EFFECTS
  None
===========================================================================*/
void gl1_hw_reset_typ1_asdiv_switch_algorithm (gas_id_t gas_id)
{
  gl1_hw_clear_antenna_avg_fifo ();
  gl1_hw_clear_antenna_meas (gas_id);
  gl1_hw_asdiv_type1.wait_count = 0;
  gl1_hw_asdiv_type1.attempt_with_lower_delta = 0;
}
#endif /* FEATURE_GSM_TX_DIV_ANTNA_SWITCHING */

/*===========================================================================

FUNCTION gl1_msg_meas_in_progress

DESCRIPTION
 Returns True if meas is in progress

DEPENDENCIES
  None

RETURN VALUE
  Returns True if meas is in progress

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_meas_in_progress(gas_id_t gas_id)
{
   if(   rxlev_SMs[gas_id][0].pwr_meas_state != SM_PWR_MEAS_NOT_SCHEDULED
      || rxlev_SMs[gas_id][1].pwr_meas_state != SM_PWR_MEAS_NOT_SCHEDULED )
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

#ifdef FEATURE_GSM_SCE_PWR_MEAS_DBG
/*===========================================================================

FUNCTION gl1_msg_pwr_meas_dbg_new_entry

DESCRIPTION
 Prepares new entry for storing debug data

DEPENDENCIES
  None

RETURN VALUE
  Pointer to debug structure for storing other debug data

SIDE EFFECTS
  None
===========================================================================*/
gl1_msg_pwr_meas_dbg_store_T *gl1_msg_pwr_meas_dbg_new_entry(gl1_msg_pwr_meas_dbg_cmd_T cmd, gas_id_t gas_id)
{
   
   l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   gl1_msg_pwr_meas_dbg_store_T *pl1_sce_dbg_pwr_meas;
   uint32 sm;
   sm_pwr_meas_type  *sm_ptr, *other_sm_ptr;

   if( gl1_msg_pwr_meas_dbg_cmd_store_idx[gas_id] < GL1_MSG_PWR_MEAS_DBG_CMD_STORE_MAX_IDX)
      gl1_msg_pwr_meas_dbg_cmd_store_idx[gas_id]++;
   else
      gl1_msg_pwr_meas_dbg_cmd_store_idx[gas_id]=0;

   pl1_sce_dbg_pwr_meas = &gl1_msg_pwr_meas_dbg_cmd_store[gas_id][gl1_msg_pwr_meas_dbg_cmd_store_idx[gas_id]];
   
   memset(pl1_sce_dbg_pwr_meas, 0, sizeof(gl1_msg_pwr_meas_dbg_store_T) );

   pl1_sce_dbg_pwr_meas->FN = GSTMR_GET_FN_GERAN(gas_id);
   pl1_sce_dbg_pwr_meas->mode = l1_sc_globals_ptr->mode;
   pl1_sce_dbg_pwr_meas->active = l1_sc_globals_ptr->active;
   pl1_sce_dbg_pwr_meas->cmd = cmd;

   sm = cur_sm[gas_id];
   sm_ptr = &rxlev_SMs[gas_id][sm];
   other_sm_ptr = &rxlev_SMs[gas_id][sm == 0 ? 1 : 0];

   pl1_sce_dbg_pwr_meas->cur_sm = sm;
   pl1_sce_dbg_pwr_meas->cur_pwr_meas_state = sm_ptr->pwr_meas_state;
   pl1_sce_dbg_pwr_meas->other_pwr_meas_state = other_sm_ptr->pwr_meas_state;
   pl1_sce_dbg_pwr_meas->pwr_meas_cb = sm_ptr->pwr_meas_cb;
   pl1_sce_dbg_pwr_meas->pwr_meas_abort_cb = sm_ptr->pwr_meas_abort_cb;

#ifdef FEATURE_GPRS_GBTA
   pl1_sce_dbg_pwr_meas->gbta_idle_event = grm_gbta_info[gas_id].idle_event;
   pl1_sce_dbg_pwr_meas->gbta_idle_event_status = grm_gbta_info[gas_id].idle_event_status;
#endif 

   return pl1_sce_dbg_pwr_meas;
}

static l1_sce_dbg_mdsp_gprs_T * l1_sce_dbg_mdsp_gprs_update_gl1_params(l1_sce_dbg_mdsp_gprs_cmd_T cmd, sm_pwr_meas_type *sm_ptr, gas_id_t gas_id)
{
   int i;
   l1_sce_dbg_mdsp_gprs_T *pl1_sce_dbg_mdsp_gprs;  

   pl1_sce_dbg_mdsp_gprs = l1_sce_dbg_mdsp_gprs_new_entry(cmd, gas_id);
   
   pl1_sce_dbg_mdsp_gprs->meas_in_progress = sm_ptr->meas_in_progress;
   pl1_sce_dbg_mdsp_gprs->arfcn_sched_index = sm_ptr->arfcn_sched_index;
   pl1_sce_dbg_mdsp_gprs->arfcn_fetch_index = sm_ptr->arfcn_fetch_index;
   pl1_sce_dbg_mdsp_gprs->hist_wr = sm_ptr->hist_wr;
   pl1_sce_dbg_mdsp_gprs->hist_rd = sm_ptr->hist_rd;

   for(i=0; i<HIST_QUEUE_SIZE; i++)
   {
      pl1_sce_dbg_mdsp_gprs->num_meas_hist[i] = sm_ptr->num_meas_hist[i];
      pl1_sce_dbg_mdsp_gprs->num_attempts[i]  = sm_ptr->num_attempts[i];
      pl1_sce_dbg_mdsp_gprs->second_attempt[i] = sm_ptr->second_attempt[i];
   }

   return pl1_sce_dbg_mdsp_gprs;
}
#endif /* FEATURE_GSM_SCE_PWR_MEAS_DBG */

