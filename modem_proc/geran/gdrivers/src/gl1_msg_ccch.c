/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  C C C H   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for communicating
   on the CCCH/BCCH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_dl_ccch
   gl1_msg_rx_ccch

   gl1_msg_abort_rx_ccch




INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_ccch.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
18/01/16   zf       CR961805: Set first burst flag in x2g_startup cmd
16/02/17   km       CR2002660: Set yield flag as true if RX is knocked by TX
03/01/18   ksb       CR2164555 Geran support to send Global Time Services Information TO GPS module
25/11/17   sn       CR2148990 Fixing Compilation issue with CR1061730 with new build Flavor
08/09/17   sn        CR1061730 FR43082: TA during PS Procedures- GL1 changes
13/06/16   sk       CR1026984 Rework on CR1011063 (Data halt in DTM scenario and Call drop observed)
08/06/16   hd       CR1023028 Use a new parameter to gl1_msg_snr_to_bep() indicate TCH channel mode
03/06/16   sk       CR1011063 Do not update average power if all the CCCH bursts have been yielded
16/11/15   zf       CR939824: Clear entered_warmup_recovery flag after abort CCCH
31/08/15   mko      CR896740:sleep error recovery changes in QTA gap
19/08/15   mc       CR874662 : CXM Freq ID support for X2G startup and X2G preload
17/06/15   js       CR831513 Allow SBD even when power monitors are scheduled.
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
16/04/15   smd      CR822202 Dual space data for IRAT and QTA
16/03/15   js       CR633570 Issue cleanup in first frame if no power monitors are scheduled
12/03/15   pa       CR806245:Add support of RxD for Single and Dual Multimode in DSDS (except GBTA/TA).
23/02/15   jk       CR793098:LIF additional logging changes
19/02/15   ab       CR771720 GL1 needs to substitute with a known NULL pattern  when SBD occurs on 2nd or 3rd bursts 
07/01/15   fan      CR777742 Clear error recovery flag when decode is finished 
17/02/15   br       CR791092 Adding F3 to print serving cell TSC and CBCH TSC
11/02/15   js       CR775188 GL1 changes to receive EPD preference 
21/01/15   am       CR782577 Discard agc update when ccch rx burst is yielded
16/12/14   jk       CR753423:LIF for Idle,voice and data
03/12/14   npt      CR759467 Fix compiler warnings
13/11/14   pa       CR755557:Support for granted band/rf device in Idle mode RxD.
11/11/14   sjv/cja  CR754278 Fixing KW errors on BO.2.0.x  
10/10/14   fan      CR730446 Check for buffer overflow in the CCCH state machine
30/10/14   cs       CR701894 Enhanced Page Detection Support
19/08/14   pjr      CR711506 Support for AMR Partial Decode and DTX detection FR19667
21/10/14   fan      CR734490 Rework the wake up recovery mechanism
10/10/14   cos      CR736560 BO 2.x Idle RxD mirror combined exit/sleep requests according to the enter/wakeup modes
07/10/14   pg       CR730132 F3 cleanup
26/09/14   fan      CR730788 Moving the start RF timer next to setting RF_WAKEUP flag to the commit mask
                          and correctly abort PWR mon
01/10/14   df       CR611845 SBD interface updates for missed wakeup
06/06/14   jk       CR668058:Adding EPD Log pakcet
17/09/14   df       CR725802 Update burst_num during error recovery
12/09/14   fan      CR636420: Run Sleep commit after GSTMR ISR at wakeup
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14   nm       CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
29/08/14   cos      CR 716812 - Adding missing changes for imrd framework
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
10/07/14   jk       CR688587:GL1 changes to support LIF feature
10/07/14   js       CR593074- ROTA/QBTA Feature implementation
28/02/13   sp       CR609697: Burst validity to be ignored only if too low power level, Reverting CR588107
21/01/14   cs       CR601912 If abort in CCCH_RX_1 then avoid sleep for one frame
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
22/12/13   sp       CR588107: Ensure invalid metrics are not considered in afc/tt/agc/snr averaging/logging.
01/12/13   js       CR578403 Frequent SNR based cell reselections
01/10/13   sk       CR552259 CR sync from 1.0/1.1
7/16/13    zc       CR470197 Replace memcpy with memscpy
20/06/13   cs       Major Triton TSTS Syncup
24/05/13   ap       CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
09/05/13   kb/npt   CR385548 - Decode PCH after sleep wakeup error recovery
08/06/12   pa       GPLT: Replaced FEATUREN_GERAN_RF_SUPPORT with FEATURE_GL1_GPLT_CANNED_RSLT.
10/02/12   ws       CR 336194 Enable TD-SCDMA IRAT for GERAN
14/10/11   cja      CR312698 Remove workaround CR308333
22/09/11  og/cja    CR308333 Disable ealry decode (as workaorund for CR305076
09/12/10   cs       CR264682 Add support for asynch dsp completion interrupt
21/07/10   cs       CR246418 Allow disabling of SAIC F/W processing in idle
12/03/10   nf       SBD:Added second null burst pattern
23/09/09   ps       Resolve Klocwork Buffer Overflow errors
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
16/06/09   ap       Fix Lint/compile warnings
09/06/09   nf       Added FEATURE_GSM_PCH_SINGLE_BURST_DEC
14/01/08   cs       Allow for separate IQ logging and saving
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
23/05/07   agv      CR118309 - Large RSSI variation initial fix.
12/08/05   gfr      Modify gl1_hw_save_iq_samples to work with L4
11/07/05   gfr      QLint
08/31/05   gfr      Replace gl1_msg_metrics_rpt with gl1_defs_metrics_rpt to
                    remove need for double copy
08/05/05   gfr      Make abort clean - i.e. no need for gl1_msg_reset afterwards
07/29/05   gfr      DTM support
05/25/05   gfr      Add channel-specific IQ sample saving enabling
04/18/05   gfr      Rename tag to l1_tag to avoid confusion, fixed offset_to_sob
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/12/05   gfr      Consolidate GSM hw layer parameters
03/09/05   yh       API change for gl1_hw_get_sync_rx_data() and added bep
                    calculation.
01/12/05    gfr     Lint cleanup
11/01/05   gfr      Add frame number to abort message
06/15/04   gfr      Renamed gl1_msg_cfg_dl_ccch to gl1_msg_cfg_ccch and merged
                    regular and early decode rx.
06/09/04   gfr      Consolidate static variables for robustness.
06/22/04   gfr      Change some MSG_HIGH to MSG_ERROR.
06/02/04   gfr      Lint cleanup.
11/05/03   gfr      Support for quad-band.
09/22/03   gfr      Save the TSC with the I/Q samples.
09/10/03   gfr      Support for saving I/Q samples to flash.
08/06/03   gw       Added call to gl1_hw_abort_early_decode() when aborting
                    receive using early decode.  This makes sure that the
                    mDSP is in the correct state to receive the next block.
07/25/03   gfr      Removed the channel filter delay timing error adjustment.
07/23/03   gfr      Add the new channel filter delay to the timing error.
06/11/03   gw       Updated burst metrics handling to support logging with
                    early paging decode.
05/21/03   gw       Modified state machine to support early paging decode.
02/12/03   gw       Commented out code to adjust timing error to account
                    for new channel filter.
01/13/03   dp       Adjusted timing error to account for new channel filter
                    delay
01/10/03   gw       Fixed bug where metrics callback was not called in DRX mode.
01/02/03   gw       Added another parameter to gl1_msg_cfg_dl_ccch(), to
                    support getting PCH block from mDSP one frame earlier.
10/09/02   dp       Undid changes to delay log callback (consequence of
                    reduction in mdsp debug buffer size )
08/15/02   bd       Delaying log callback for GPRS
07/11/02   gw       Changed references to power_dBm_x16 in metrics reports to
                    pwr_dBm_x16.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
06/06/02   mk       Support for aborts on all channels.
05/08/02   JC       Fixed issue with burst metrics when rx_state = {2,6}
                    condition.  Now reporting metrics in offset 5 so that
                    the metrics of the other machine don't overwrite the
                    current machine metrics, since L1 dowsn't allocate
                    buffers for each machine.
04/30/02   JC       Added ARFCN info to CCCH burst metrics.  Not the cleanest
                    approach, needs further review.
04/24/02   gw       Re-enabled AGC slew rate limiting for xCCH receives.
04/24/02   gw       Disabled AGC slew rate limiting for xCCH receives.
04/19/02   JC       Added rx_gain_range info in metrics callback to enhance
                    logging capabilities.
03/27/02   JC       Moved enabling of slew rate limiting to gl1_msg_rx_ccch
                    since srl is applied on an AGC-by-AGC basis vs. globally.
03/05/02   JC       Applying AGC to xCCH burst in 51-multiframe.
02/14/02   gw       Hardcoded returned RSSI values as interim measure while
                    AGC is introduced.
02/05/02   JC       Added support for AGC; Also, call to
                    gl1_hw_get_syncrx_metrics in rx_handler to unwind HW
                    layer.
02/01/02   mk       Added snr to metrics.
12/12/01   gw       Changed the way the debug (logging) callback works.
11/6/01    bk       Changed NPL1 function calls to reflect change in interface
10/31/01   gw/dp    Added support for logging debug info. Made changes to pass
                    in the required number of data octets when retrieving
                    CCCH decoded data.
10/31/01   dp       Made changes to use new gl1_hw_get_syncrx_data interface
                    to eliminate multiple copies when retrieving DSP results
10/16/01   gw       Changes to support returning RSSI in dBm.
09/27/01   gw       Added support for passing the channel type into
                    gl1_msg_rx_ccch and returning it in the callback.
09/25/01   mk/gw    Initial version with syncrx metrics support.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include "comdef.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "gl1_sys_algo.h"
#include "msg.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "gpl1_dual_sim.h"
#include "geran_dual_sim.h"

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

#include "l1_utils.h"
#include <stringl/stringl.h>
#include "gl1_hw_sleep_ctl.h"
#include "l1_task.h"

extern void l1_sci_monscan_done_idle( gas_id_t gas_id );
extern uint8 l1_get_idle_monitors_in_progress( gas_id_t gas_id );
extern  void l1_reset_idle_monitors_in_progress( gas_id_t gas_id );

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

/*****************************************************

 LOCAL STORAGE

******************************************************/
#define NUM_CCCH_SM 2

/* CCCH states */
typedef enum
{
   CCCH_INACTIVE,
   CCCH_WAITING_FOR_BLOCK_START,
   CCCH_RECEIVE_1,
   CCCH_RECEIVE_2,
   CCCH_RECEIVE_3,
   CCCH_RECEIVE_4,
   CCCH_WAITING,
   CCCH_GET_DATA
} ccch_state_type;


/* Structure containing information for a single CCCH rx */
typedef struct
{
   /* State variables */
   ccch_state_type     state;
   uint8               burst_num;
   uint8               metrics_num;
   uint8               index;
   uint8               missed_frames;

   /* Configured rx parameters */
   sys_algo_agc_T      *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   sys_algo_agc_T      *agc_ptr_divrx_ccch; // added CCCH temporarily for diferentiating on the other agc pointers.
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
   ARFCN_T             ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD];
   uint8               l1_tag;
   boolean             own_paging_block;
   gl1_msg_chan_type   chan;

   /* Frame layer parameters */
   gl1_hw_rx_burst_type   burst_info;
   gl1_hw_rx_signal_type  signal_info;
   gl1_hw_rx_tags_type    tags[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

   /* Callback parameters */
   gl1_msg_rx_cb_type  callbacks;
   gl1_msg_rx_rpt      rpt;
   uint8               offset_to_sob;
} ccch_state_machine_type;

#define SLEEP_WAKEUP_MISSED_1_FRAME (1)
#define SLEEP_WAKEUP_MISSED_2_FRAME (2)
#define SLEEP_WAKEUP_MISSED_3_FRAME (3)
/* Structure containing all CCCH parameters */
typedef struct
{
   /* Flag indicating if another rx can be scheduled */
   boolean             window_open;

   /* Configured parameters common to all blocks */
   boolean             next_block_DRX;
   uint8               next_block_TSC;
   gl1_msg_rx_cb_type  next_block_callbacks;

   /* Array of state machines */
   uint8                   sm_cnt;
   uint8                   sm_rd_ptr;
   uint8                   sm_wr_ptr;
   ccch_state_machine_type sm[NUM_CCCH_SM];

} ccch_params_type;


/* Static local storage */
static ccch_params_type ccch_params[NUM_GERAN_DATA_SPACES];

const uint8 null_page_data_0[MAX_CCCH_OCTETS] =
    {0x15,0x06,0x21,0x00,0x01,0x00,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,
      0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B};

const uint8 null_page_data_1[MAX_CCCH_OCTETS] =
    {0x15,0x06,0x21,0x00,0x01,0xF0,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,
      0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B,0x2B};

#ifdef FEATURE_GSM_PCH_SINGLE_BURST_DEC_DEBUG
extern gl1_hw_print_radio_block( uint8 *msg,gas_id_t gas_id );
#endif

extern void gl1_drx_require_next_tick(gas_id_t gas_id);

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/
static boolean ccch_rx_handler( boolean start_of_block,
                                uint32  time_stamp, gas_id_t gas_id );


void gl1_hw_recovery_update_ccch_params(gas_id_t gas_id );
/*===========================================================================

FUNCTION gl1_msgi_init_ccch

DESCRIPTION
  This function is used to configure the TSC and callbacks for the CCCH. The
  callbacks will be invoked by the frame tick handler as required.

===========================================================================*/
void gl1_msgi_init_ccch (gas_id_t gas_id)
{
   uint8 i;

   ccch_params[gas_id].sm_cnt                          = 0;
   ccch_params[gas_id].sm_rd_ptr                       = 0;
   ccch_params[gas_id].sm_wr_ptr                       = 0;
   ccch_params[gas_id].window_open                     = TRUE;
   ccch_params[gas_id].next_block_callbacks.data_cb    = NULL;
   ccch_params[gas_id].next_block_callbacks.metrics_cb = NULL;
   ccch_params[gas_id].next_block_callbacks.abort_cb   = NULL;
   ccch_params[gas_id].next_block_callbacks.log_cb     = NULL;

   for (i = 0; i < NUM_CCCH_SM; i++)
   {
      L1_STATE_CHANGE(ccch_params[gas_id].sm[i].state = CCCH_INACTIVE, "CCCH_INACTIVE");
      ccch_params[gas_id].sm[i].burst_num               = 0;
      ccch_params[gas_id].sm[i].metrics_num             = 0;
      ccch_params[gas_id].sm[i].index                   = i;
      ccch_params[gas_id].sm[i].agc_ptr                 = NULL;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
      ccch_params[gas_id].sm[i].agc_ptr_divrx_ccch      = NULL; // CCCH AGC POINTER INITIALIZED HERE. NO NEED FOR DRX INIT.
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
      ccch_params[gas_id].sm[i].callbacks.data_cb       = NULL;
      ccch_params[gas_id].sm[i].callbacks.metrics_cb    = NULL;
      ccch_params[gas_id].sm[i].callbacks.abort_cb      = NULL;
      ccch_params[gas_id].sm[i].callbacks.log_cb        = NULL;
   }
}



/*===========================================================================

FUNCTION gl1_msg_cfg_ccch

DESCRIPTION
  This function is used to configure the TSC and callbacks for the CCCH. The
  callbacks will be invoked by the frame tick handler as required.

===========================================================================*/
void gl1_msg_cfg_ccch
(
   uint8 TSC,
   boolean DRX,
   gl1_msg_rx_cb_type *cb,
   gas_id_t gas_id
)
{
   /* Save the parameters for the next block */
   ccch_params[gas_id].next_block_callbacks.data_cb    = cb->data_cb;
   ccch_params[gas_id].next_block_callbacks.metrics_cb = cb->metrics_cb;
   ccch_params[gas_id].next_block_callbacks.log_cb     = cb->log_cb;
   ccch_params[gas_id].next_block_DRX                  = DRX;
   ccch_params[gas_id].next_block_TSC                  = TSC;
   MSG_GERAN_HIGH_1_G("CCCH config: Serving cell tsc:%d  ", ccch_params[gas_id].next_block_TSC);
}



/*===========================================================================

FUNCTION gl1_msg_rx_ccch

DESCRIPTION
  This function is used to receive a message on the CCCH on the next ccch
  block boundary. This function may be called at task level but should
  be called in the block before the data is to transmitted.

DEPENDENCIES
  gl1_msg_cfg_dl_ccch() must have been called at some time prior.

===========================================================================*/
void gl1_msg_rx_ccch
(
   ARFCN_T           ARFCN[GL1_DEFS_FRAMES_IN_BLK_PERIOD],
   sys_algo_agc_T    *agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   sys_algo_agc_T    *agc_ptr_divrx_ccch, // diferentiating wtr other agc pointers.
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
   uint8             tag,
   boolean           own_paging_block,
   gl1_msg_chan_type chan,
   gl1_msg_rx_rpt    *rpt,
   gas_id_t          gas_id
)
{
   ccch_state_machine_type *sm;
   uint16 i;

   /* Check if we have a free sm */
   if (ccch_params[gas_id].sm_cnt >= NUM_CCCH_SM)
   {
      MSG_GERAN_ERROR_0_G("CCCH queue full");

      /* Just in case the handler was uninstalled by accident, reinstall it */
      gl1_msgi_add_ft_handler(ccch_rx_handler, GL1_MSG_FT_HDLR_DL,gas_id);
      return;
   }

   /* Check that the window is open */
   if (!ccch_params[gas_id].window_open)
   {
      MSG_GERAN_ERROR_0_G("CCCH rx issued with window closed");

      /* Just in case the handler was uninstalled by accident, reinstall it */
      gl1_msgi_add_ft_handler(ccch_rx_handler, GL1_MSG_FT_HDLR_DL,gas_id);
      return;
   }

   /* Add to the end of the fifo */
   sm = &ccch_params[gas_id].sm[ccch_params[gas_id].sm_wr_ptr];
   ccch_params[gas_id].sm_wr_ptr = (ccch_params[gas_id].sm_wr_ptr + 1) % NUM_CCCH_SM;
   ccch_params[gas_id].sm_cnt++;

   /* Fill in the state machine parameters */
   L1_STATE_CHANGE(sm->state = CCCH_WAITING_FOR_BLOCK_START, "CCCH_WAITING_FOR_BLOCK_START");
   sm->agc_ptr           = agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
   sm->agc_ptr_divrx_ccch= agc_ptr_divrx_ccch; // diferentiating wtr the other agc pointers.
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

   sm->l1_tag            = tag;
   sm->own_paging_block  = own_paging_block;
   sm->chan              = chan;
   sm->callbacks         = ccch_params[gas_id].next_block_callbacks;
   sm->rpt.data          = rpt->data;

   /* Fill in static frame layer parameters */
   sm->signal_info.tn                = GL1_DEFS_TN_0;
   sm->burst_info.channel_type       = GL1_HW_PCH;
   sm->burst_info.tsc                = ccch_params[gas_id].next_block_TSC;
   sm->burst_info.early_decoding     = ccch_params[gas_id].next_block_DRX;
   sm->burst_info.search_width       = GL1_HW_DEFAULT_SEARCH_WIDTH;

   for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
   {
      sm->ARFCN[i] = ARFCN[i];
      sm->rpt.metrics[i] = rpt->metrics[i];
   }

   /* Only a single rx is allowed per block */
   ccch_params[gas_id].window_open = FALSE;

   /* Add the handler if necessary */
   if (ccch_params[gas_id].sm_cnt == 1)
   {
      gl1_msgi_add_ft_handler(ccch_rx_handler, GL1_MSG_FT_HDLR_DL,gas_id);
   }

}

/*===========================================================================

FUNCTION gl1_msg_abort_rx_ccch

DESCRIPTION
  This function is used to abort CCCH.

===========================================================================*/
void gl1_msg_abort_rx_ccch (gas_id_t gas_id )
{
   uint8 sm_num, cnt;
   ccch_state_machine_type *sm;

   /* If we have nothing to abort, clean up to be safe */
   if (ccch_params[gas_id].sm_cnt == 0)
   {
      ccch_params[gas_id].sm_rd_ptr = 0;
      ccch_params[gas_id].sm_wr_ptr = 0;
      ccch_params[gas_id].window_open = TRUE;
      return;
   }

   /* Abort all queued state machines */
   sm_num = ccch_params[gas_id].sm_rd_ptr;
   for (cnt = 0; cnt < ccch_params[gas_id].sm_cnt; cnt++)
   {
      sm = &ccch_params[gas_id].sm[sm_num];

      MSG_GERAN_HIGH_2_G("CCCH aborted: state=%d, FN=%d", sm->state,GSTMR_GET_FN_GERAN(gas_id));

      /* We may need to abort decoding */
      switch (ccch_params[gas_id].sm[sm_num].state)
      {
         /* Avoid sleep here if abort and RX config on the same frame */
         case CCCH_RECEIVE_1:
            MSG_GERAN_MED_0_G( "Abort received on config frame, stay awake for next frame" );

            gl1_drx_require_next_tick( gas_id );
         break;

         case CCCH_RECEIVE_2:
            if ( gl1_msg_is_sbd_enabled( gas_id ) &&
                 sm->burst_info.early_decoding )
            {
               MSG_GERAN_MED_0_G("SBD:EarlyDecode clean-up");
               gl1_hw_cleanup_early_decode(&sm->tags[0],gas_id);
            }
            break;

         case CCCH_RECEIVE_3:
            if (sm->burst_info.early_decoding)
            {
               gl1_hw_cleanup_early_decode(&sm->tags[1],gas_id);
            }
            break;

         case CCCH_RECEIVE_4:
            if (sm->burst_info.early_decoding)
            {
               gl1_hw_cleanup_early_decode(&sm->tags[1],gas_id);
               gl1_hw_cleanup_early_decode(&sm->tags[2],gas_id);
            }
            break;

         case CCCH_WAITING:
            if (sm->burst_info.early_decoding)
            {
               gl1_hw_cleanup_early_decode(&sm->tags[2],gas_id);
            }
            gl1_hw_cleanup_early_decode(&sm->tags[3],gas_id);
            break;

         case CCCH_GET_DATA:
            gl1_hw_cleanup_early_decode(&sm->tags[3],gas_id);
            break;

         default:
            break;
      }

      L1_STATE_CHANGE(sm->state = CCCH_INACTIVE, "CCCH_INACTIVE");

      sm_num = (sm_num + 1) % NUM_CCCH_SM;
   }

   /* Reset fifo pointers */
   ccch_params[gas_id].sm_cnt = 0;
   ccch_params[gas_id].sm_rd_ptr = 0;
   ccch_params[gas_id].sm_wr_ptr = 0;
   ccch_params[gas_id].window_open = TRUE;

   gl1_msgi_remove_ft_handler(ccch_rx_handler,gas_id);
}



/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/


/*===========================================================================

FUNCTION ccch_process_metrics

DESCRIPTION
   Retrieves burst metrics for the given state machine.

===========================================================================*/
static void ccch_process_metrics
(
   ccch_state_machine_type *sm,
   uint32 time_stamp,
   gas_id_t gas_id
)
{
   gl1_defs_metrics_rpt metrics, *metrics_ptr;
   boolean              valid;
#ifdef GERAN_L1_HLLL_LNASTATE
   gl1_defs_metrics_lnastate gl1_metrics_lnastate;
#endif
   boolean                        valid_sched;
   GfwSchedulerResultStruct       schedule_results = {0};
   uint16 schedule_status;

   /* Interferer Ind. */
   boolean              blocker = FALSE;

#ifdef GERAN_L1_ENHANCED_RX 
   gl1_msg_lif_chan_type channel_type = GL1_INVALID_CHAN;
   uint8 NoOfSlots = 0;
#endif /*GERAN_L1_ENHANCED_RX*/

   if (sm->metrics_num >= GL1_DEFS_FRAMES_IN_BLK_PERIOD)
   {
      /* Out of range, so do nothing */
      return;
   }

   /* Get storage for the metrics, we need to get them even if PL1 is not
      interested in the results */
   metrics_ptr = sm->rpt.metrics[sm->metrics_num];
   if (metrics_ptr == NULL)
   {
      metrics_ptr = &metrics;
   }

   /* Need to unwind the frame layer, regardless of whether or not a
      metrics callback is registered or not */
   blocker = gl1_hw_get_sync_rx_metrics(
               metrics_ptr, &sm->tags[sm->metrics_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
               LOG_PCH,
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE */
               gas_id);
   /* Assign blocker to itself to handle compile warning until blocker gets
    * implemented here
    */
   NOTUSED(blocker);
   
   /* Get the burst schedule results */
   valid_sched = mdsp_ccch_get_rx_burst_schedule(&schedule_results ,gas_id);
   /* Burst schedule */
   if (valid_sched)
   {
      schedule_status = schedule_results.reason[0];
      if(schedule_status & MDSP_GPRS_RX_BURST_SCHED_OK)
      {
        if(schedule_status & MDSP_GPRS_TX_KNOCK_RX)
        {
           metrics_ptr->yield = TRUE;
           MSG_GERAN_HIGH_0_G("The burst got yielded due to conflict with TX of other sub");
        }
        else
        {
           metrics_ptr->yield = FALSE;
        }
      }
      else
      {
        metrics_ptr->yield = TRUE;
        MSG_GERAN_LOW_0_G("The burst got yielded due to conflict with other sub"); 
      }
   }
   else
   {
      metrics_ptr->yield = FALSE;   
      MSG_GERAN_HIGH_0_G("The burst could not be scheduled");  
   }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
   gl1_hw_get_iq_samples( GL1_HW_PCH, sm->index, sm->metrics_num, sm->burst_info.tsc, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */

   /* Perform AGC on the burst */
   valid = (metrics_ptr->powerStatus == RF_IN_RANGE ? TRUE : FALSE);

   /* No slew for Idle; note blocker is also not slewed. */
   sm->agc_ptr->srl_enabled = FALSE;

   /* RSSI too low - could be a CXM yield or very low power -
    * burst valid needs to be set, else this will be considered in averaging -impacts reselection
    */
   metrics_ptr->valid = (( metrics_ptr->pwr_dBm_x16 > RFGSM_MIN_RX_POWER_REPORTED ) || !(metrics_ptr->yield));

   if ( metrics_ptr->valid )
   {
     /* Perform AGC on the burst */
     sys_algo_agc_update(sm->agc_ptr, metrics_ptr->pwr_dBm_x16,
                         valid, time_stamp,gas_id );
   }

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
   MSG_GERAN_HIGH_2_G(GL1_RXD"AGC_LOOP : pwr_dBm_x16=%i pwr_dBm_x16_divrx=%d", 
                      metrics_ptr->pwr_dBm_x16, 
                      metrics_ptr->pwr_dBm_x16_divrx);
#endif
   //  valid_divrx = (metrics_ptr->powerStatus_divrx == RF_IN_RANGE ? TRUE : FALSE);

   if(gl1_get_IMRD_ctl_nv(gas_id))
   {
     /* No slew for Idle; note blocker is also not slewed. */
     sm->agc_ptr_divrx_ccch->srl_enabled = FALSE;


     /* Perform AGC on the burst */
     /* AGC loop for diversity RX should be ON even if RX diveristy is disabled under good channel conditions
     * If Rx diversity is disabled for the burst, burst RSSI and pwr_dBm_x16_divrx are set to 0. If Rx diversity
     * is disabled, use received power from primary chain to maintain the AGC loop for diversity chain.
     */
     if((metrics_ptr->pwr_dBm_x16_divrx != 0) && gl1_get_rxdiv_agc_start(gas_id))
     {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
       MSG_GERAN_HIGH_3_G(GL1_RXD"AGC_LOOP :sys_algo_agc_update() agc maintained with DRX agc_ptr_divrx_ccch=%i >>>pwr_dBm_x16_divrx=%i, pwr_dBm_x16=%i , ",
                   sm->agc_ptr_divrx_ccch, metrics_ptr->pwr_dBm_x16_divrx, metrics_ptr->pwr_dBm_x16);
#endif /* FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
       sys_algo_agc_update(sm->agc_ptr_divrx_ccch, metrics_ptr->pwr_dBm_x16_divrx,
                   valid, time_stamp,gas_id );
     }
     /* use power from primary chain only if valid */
     else if (gl1_get_rxdiv_agc_start(gas_id) && metrics_ptr->valid )
     {
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
       MSG_GERAN_HIGH_3_G(GL1_RXD"AGC_LOOP :sys_algo_agc_update() agc maintained with PRX agc_ptr_divrx_ccch=%i pwr_dBm_x16_divrx=%i, >>>pwr_dBm_x16=%i , ",
                 sm->agc_ptr_divrx_ccch, metrics_ptr->pwr_dBm_x16_divrx, metrics_ptr->pwr_dBm_x16);
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG*/
       sys_algo_agc_update(sm->agc_ptr_divrx_ccch, metrics_ptr->pwr_dBm_x16,
                 valid, time_stamp,gas_id );

     }
   }
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/

   metrics_ptr->arfcn            = sm->ARFCN[sm->metrics_num];
   metrics_ptr->offset_to_sob    = 1;
   metrics_ptr->own_paging_block = sm->own_paging_block;

   /* Fill out the BEP values in data ptr */
   sm->rpt.data->ccch.hdr.bep[sm->metrics_num] =
     gl1_msg_snr_to_bep(metrics_ptr->snr, GL1_DEFS_MODULATION_GMSK,FALSE,gas_id);

#ifdef GERAN_L1_HLLL_LNASTATE
   if( metrics_ptr->valid && gl1_hw_get_sawless_HLLL_support(metrics_ptr->arfcn.band,gas_id) )
   {
     gl1_metrics_lnastate.NoOfChannels              = 1;
     gl1_metrics_lnastate.gsm_metrics[0].arfcn      = metrics_ptr->arfcn;
     gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = metrics_ptr->pwr_dBm_x16;
     gl1_metrics_lnastate.gsm_metrics[0].snr        = metrics_ptr->snr;
     gl1_metrics_lnastate.gsm_metrics[0].jdet_value = metrics_ptr->jdet_value;
     if(gl1_is_current_state_dtm(gas_id) == TRUE)
     {
       gl1_hw_update_dtm_metrics(&gl1_metrics_lnastate,GSTMR_GET_FN_GERAN(gas_id),UPDATE_CS_METRICS,gas_id);
     }
     else
     {
       gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
     }
   }
#endif /*GERAN_L1_HLLL_LNASTATE*/

#ifdef GERAN_L1_ENHANCED_RX
   if(gl1_hw_get_lif_feature_flag(gas_id) &&
      (gl1_msg_get_multi_sim_sys_mode( ) != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE)
#ifdef FEATURE_SGLTE
      && (gl1_hw_get_sglte_mode(gas_id) == FALSE)
#endif
      && (metrics_ptr->valid)
      )
    {
    NoOfSlots = 1;
    channel_type = (sm->chan == GL1_MSG_BCCH)?GL1_MSG_CS_BCCH:GL1_MSG_CS_CCCH;

    gl1_hw_update_lif_metrics(metrics_ptr,NULL,sm->ARFCN[sm->metrics_num],channel_type,NoOfSlots,
                              GSTMR_GET_FN_GERAN(gas_id),UPDATE_LIF_CS_METRICS,gas_id);
    }
#endif /*GERAN_L1_ENHANCED_RX*/

   /* We are done with these metrics */
   sm->metrics_num++;

   /* Same story for the log callback, except that we call */
   /* the callback after each frame.                       */
   if (sm->callbacks.log_cb)
   {
      sm->callbacks.log_cb();
   }

}


/*===========================================================================

FUNCTION ccch_process_data

DESCRIPTION
   Attempts to retrieve data for the given state machine.

===========================================================================*/
static boolean ccch_process_data
(
   ccch_state_machine_type *sm,
   uint8 decode_tag,
   boolean last_try,
   gas_id_t gas_id
)
{
   boolean data_done = FALSE;

   /* The data might be available, try to get it. */
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
   // In case of GPLT and no RF, return default/canned result
   MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_process_data: Not calling gl1_hw_get_sync_rx_data");
   sm->rpt.data->ccch.hdr.good_data = TRUE;
#else
   gl1_hw_get_sync_rx_data
    (
      GL1_HW_PCH,
      &sm->tags[decode_tag],
      &sm->rpt.data->ccch.hdr,
      sm->rpt.data->ccch.data,
      (uint16)MAX_CCCH_OCTETS,
      gas_id
    );
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

   /*FW wants GL1 to log EPD log packet(0x5097) even when CRC fails for SBD.*/
   if(sm->metrics_num == 1)
    {
    gl1_log_epd_metrics(&(sm->rpt.data->ccch.hdr.epdLogPacket),gas_id);
    }

   /* If this is the last chance or the decode was succesful then
    * finish this block.  */
   if (last_try || sm->rpt.data->ccch.hdr.good_data)
   {
      /* Fill in rest of callback report */
      sm->rpt.data->ccch.hdr.chan           = (int32)sm->chan;
      sm->rpt.data->ccch.tag                = sm->l1_tag;
      sm->rpt.data->ccch.own_paging_block   = sm->own_paging_block;
      sm->rpt.data->ccch.arfcn              = sm->ARFCN[0];
      sm->rpt.data->ccch.offset_to_sob      = sm->offset_to_sob;

      /* Make the data callback */
      if (sm->callbacks.data_cb)
      {
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
         MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_process_data: ");
#else
         /* SBD can occur on first burst after missed frames */
         if (( gl1_msg_is_sbd_enabled( gas_id ) &&
              ( sm->own_paging_block ) &&
              ( ( sm->metrics_num == ( sm->missed_frames + 1 )) ||   // First Burst PCH Decode.
                ( (sm->metrics_num == 2 ) && (sm->rpt.metrics[0]->valid == FALSE) ) ||  // First Burst YIELD , 2nd burst -  single burst PCH decode
                ( (sm->metrics_num == 3 ) && (sm->rpt.metrics[0]->valid == FALSE) && (sm->rpt.metrics[1]->valid == FALSE) ) // 1st/2nd Burst YIELD , 3rd burst single burst PCH decode
               )
            ) 
#if defined (FEATURE_GSM_SIMULATE_GL1_RF)  || defined ( FEATURE_QSH_EVENT_NOTIFY_HANDLER )
            #error code not present
#endif
          )
         {
            uint8 *null_page_data;
            uint16 pattern_detect = gl1_hw_sbd_get_null_pattern(gas_id);

            if(pattern_detect == 0)
            {
                null_page_data = (uint8 *)null_page_data_0;
            }
            else
            {
                null_page_data = (uint8 *)null_page_data_1;
            }

            MSG_GERAN_MED_4_G("SBD:replace burst data with a NULL Page pattern %d sm->metrics_num %d sm->missed_frames %d valid[0] %d",
               pattern_detect, sm->metrics_num, sm->missed_frames,sm->rpt.metrics[0]->valid);

            /*First burst CRC=Ok, this is a NULL Page*/
            memscpy(sm->rpt.data->ccch.data,sizeof(sm->rpt.data->ccch.data),null_page_data,MAX_CCCH_OCTETS);
         }
#endif /*(defined(FEATURE_GL1_GPLT))*/

         sm->callbacks.data_cb(sm->rpt.data,gas_id);
      }
      else
      {
#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
       MSG_GERAN_HIGH_1_G("No ccch dl callback registered. CCCH ERR  sm->burst_num %d  ", sm->burst_num );
#endif
      }

      L1_STATE_CHANGE(sm->state = CCCH_INACTIVE, "CCCH_INACTIVE");
      data_done = TRUE;

      /* Let drivers clean up after un-received bursts */
      if ( (!last_try) && (decode_tag < (GL1_DEFS_FRAMES_IN_BLK_PERIOD-1)) )
      {
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
         // In case of GPLT and no RF, return default/canned result
         MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_process_data: Not calling gl1_hw_cleanup_early_decode");
#else
         gl1_hw_cleanup_early_decode(&sm->tags[decode_tag + 1],gas_id);
#endif /*(defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
      }
   }


   return data_done;
}



/*===========================================================================

FUNCTION ccch_rx_handler

DESCRIPTION
  This function should be called every frame on the 51 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_ccch().
  The handler is told when the start of a block is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that there are no more messages queued up to send, it will signal that
  it should be deregistered.

RETURN VALUE
  TRUE if message reception is complete
  FALSE if messages are still being received

===========================================================================*/
static boolean ccch_rx_handler
(
   boolean start_of_block,  /* next frame is start of block */
   uint32  time_stamp,       /* currently not used           */
   gas_id_t gas_id
)
{
   ccch_state_machine_type *sm;
   uint8   sm_num, i, cnt, decode_tag = 0;
   boolean metrics_flag, receive_burst, get_data;
   boolean decode_flag, sbd_allowed, epd_allowed;
   boolean last_attempt, data_done;

#if defined (FEATURE_QBTA)
   boolean pwr_hndlr_regtd = gl1_msgi_check_ft_handler_registered(gas_id ,GL1_MSG_FT_HDLR_PWR_MEAS);
#endif
   
   /* Sanity check */
   if (ccch_params[gas_id].sm_cnt == 0)
   {
      MSG_GERAN_ERROR_0_G("No active xCCH state machines");

      return TRUE;
   }

   /* Go through the state machine fifo */
   sm_num = ccch_params[gas_id].sm_rd_ptr;

   for (cnt = 0; cnt < ccch_params[gas_id].sm_cnt; cnt++)
   {
      sm = &ccch_params[gas_id].sm[sm_num];

      metrics_flag  = FALSE;
      decode_flag   = FALSE;
      receive_burst = FALSE;
      get_data      = FALSE;
      last_attempt  = FALSE;
      data_done     = FALSE;
      sbd_allowed   = FALSE;
      epd_allowed   = FALSE;

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
      MSG_GERAN_HIGH_1_G("CCCH HANDLER: sm_num=%d", sm_num);
#endif

      /* Kick the state machine */
      switch (sm->state)
      {
         case CCCH_INACTIVE:
            MSG_GERAN_ERROR_0_G("CCCH inactive");
            break;

         case CCCH_WAITING_FOR_BLOCK_START:
            if (!start_of_block) break;

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
            MSG_GERAN_HIGH_0_G("CCCH CCCH_WAITING_FOR_BLOCK_START");
#endif

            /*
             * Always clear on first block to avoid any hangover if sleep
             * never happens
             */
            gl1_msg_set_mdsp_asynch_int( FALSE, gas_id );

            /* Open the window now */
            ccch_params[gas_id].window_open = TRUE;

            /* Missed frames due to sleep wakeup error recovery */
            sm->missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);

#ifdef DEBUG_GSM_SLEEP_ERROR_RECOVERY
            MSG_GERAN_ERROR_3_G("ccch_rx_handler missed_frames %d FN %d timetick %d",
                                sm->missed_frames,GSTMR_GET_FN_GERAN(gas_id),timetick_get_safe());
#endif

            /* Sleep wakeup error recovery missed frames only affect the first xCCH decode
             * after waking up. The following xCCH decode in the same wakeup period, if there is,
             * won't be affected as it is already awake. So clear the missed frames here. */
            gl1_hw_sleep_clear_missed_frames(gas_id);
            sm->burst_info.num_of_page_bursts_missed = sm->missed_frames;
            sm->burst_num   = sm->missed_frames;
            sm->metrics_num = sm->missed_frames;
            sm->state++;
            /* fall through */

         case CCCH_RECEIVE_1:
            receive_burst = TRUE;

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
            MSG_GERAN_HIGH_0_G("CCCH CCCH_RECEIVE_1");
#endif

            /* Can we disable SAIC as good hard_rxqual for a period */
            gl1_hw_cfg_saic( !gl1_msg_get_saic_disable_in_idle(gas_id) ,gas_id);

            /*
             * Always set on first block as SBD could occur for PCH
             */
            if ( gl1_msg_is_sbd_enabled( gas_id ) && sm->own_paging_block )
            {
              if ( !gl1_msg_is_early_decode_disabled( gas_id ) )
              {
                decode_flag = sm->burst_info.early_decoding;
              }

              /* Single set of SBD allowed for first burst setup */
              if ( ( sm->missed_frames == 0 ) ||
                   ( sm->missed_frames == SLEEP_WAKEUP_MISSED_1_FRAME ) ||
                   ( sm->missed_frames == SLEEP_WAKEUP_MISSED_2_FRAME )
                 )
              {
                sbd_allowed = decode_flag;

                /* Single set of EPD allowed for SBD and not page sharing setup */
                if ( gl1_msg_is_epd_allowed(gas_id) )
                {
                  epd_allowed = sbd_allowed;
                }
              }
            }

            /* Always reset here */
            sm->offset_to_sob = 0;

            MSG_GERAN_HIGH_3_G( "ccch_rx_handler own_paging_block:%d sbd_allowed:%d epd_allowed:%d",
                                sm->own_paging_block, sbd_allowed, epd_allowed );

            sm->state++;
            break;

         case CCCH_RECEIVE_2:
            receive_burst = TRUE;

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
            MSG_GERAN_HIGH_0_G("CCCH CCCH_RECEIVE_2");
#endif
            /* In idle mode, send GTS data after first burst of PCH */
            if(sm->own_paging_block)
            {
              gl1_gts_send_gts_after_wakeup(gas_id);
            }

            if ( !gl1_msg_is_early_decode_disabled( gas_id ) )
            {
              decode_flag = sm->burst_info.early_decoding;
            }

            /* Possible SBD decode so enable Asynch Sleep trigger */
            if ( gl1_msg_is_sbd_enabled( gas_id ) && sm->own_paging_block &&
                 ( ( sm->missed_frames == 0 ) ||
                   ( sm->missed_frames == SLEEP_WAKEUP_MISSED_1_FRAME ) ||
                   ( sm->missed_frames == SLEEP_WAKEUP_MISSED_2_FRAME )
                 )
                )
            {
              /* Enable asynch int handling here as Early Decode receive frame */
              gl1_msg_set_mdsp_asynch_int( decode_flag, gas_id );
            }

            sm->offset_to_sob = 0;

            if ( sm->missed_frames == SLEEP_WAKEUP_MISSED_2_FRAME )
            {
                sm->state          = CCCH_WAITING;
                sm->offset_to_sob += sm->missed_frames;

                /* RX burst setup already that needs cleanup */
                decode_tag        += sm->missed_frames;
            }
            else
            {
                sm->state++;
            }
            break;

         case CCCH_RECEIVE_3:
            receive_burst = TRUE;

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
            MSG_GERAN_HIGH_0_G("CCCH CCCH_RECEIVE_3");
#endif

            if ( !gl1_msg_is_early_decode_disabled( gas_id ) )
            {
              decode_flag = sm->burst_info.early_decoding;
            }

            /* Enable asynch int handling here as Early Decode receive frame */
            if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) &&
                 ( ( sm->missed_frames == 0 ) ||
                   ( sm->missed_frames == SLEEP_WAKEUP_MISSED_1_FRAME ) )
               )
            {
               gl1_msg_set_mdsp_asynch_int( decode_flag, gas_id );
            }

            metrics_flag  = TRUE;

            if ( gl1_msg_is_sbd_enabled( gas_id ) &&
                 sm->own_paging_block &&
                 !gl1_msg_is_early_decode_disabled( gas_id ) )
            {
                get_data = sm->burst_info.early_decoding;
            }

            sm->offset_to_sob++;

            if ( sm->missed_frames == SLEEP_WAKEUP_MISSED_1_FRAME )
            {
                sm->state          = CCCH_WAITING;
                sm->offset_to_sob += sm->missed_frames;

                /* RX burst setup already that needs cleanup */
                decode_tag        += sm->missed_frames;
            }
            else
            {
                sm->state++;
            }
            break;

         case CCCH_RECEIVE_4:
#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
            MSG_GERAN_HIGH_0_G("CCCH CCCH_RECEIVE_4");
#endif

            receive_burst = TRUE;
            decode_flag   = TRUE;
            get_data      = sm->burst_info.early_decoding;
            decode_tag    = 1;
            metrics_flag  = TRUE;

            /* Enable asynch int handling here as Early Decode receive frame */
            if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) &&
                 ( sm->missed_frames == 0 ) )
            {
              /* Enable asynch int handling for next decode burst */
              gl1_msg_set_mdsp_asynch_int( TRUE, gas_id );
            }

            sm->offset_to_sob++;
            sm->state++;
            break;

         case CCCH_WAITING:
#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
            MSG_GERAN_HIGH_0_G("CCCH CCCH_WAITING");
#endif

            get_data      = sm->burst_info.early_decoding;
            decode_tag    = 2;
            metrics_flag  = TRUE;

            /* Need to check if missed_frames handling can jump here from SM */
            if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) )
            {
              /* Enable asynch int handling for last decode burst */
              gl1_msg_set_mdsp_asynch_int( TRUE, gas_id );
            }

            sm->offset_to_sob++;
            sm->state++;
            break;

         case CCCH_GET_DATA:
#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
            MSG_GERAN_HIGH_0_G("CCCH CCCH_GET_DATA");
#endif

            get_data      = TRUE;
            last_attempt  = TRUE;
            metrics_flag  = TRUE;
            decode_tag    = 3;

            if ( gl1_msg_is_asynch_sleep_enabled( gas_id ) )
            {
              /* Always clear as last RX burst opportunity passed */
              gl1_msg_set_mdsp_asynch_int( FALSE, gas_id );
            }

            sm->offset_to_sob++;
            L1_STATE_CHANGE(sm->state = CCCH_INACTIVE, "CCCH_INACTIVE");
            break;

         default:
            MSG_GERAN_ERROR_1_G("Invalid CCCH state: %d",sm->state);
            L1_STATE_CHANGE(sm->state = CCCH_INACTIVE, "CCCH_INACTIVE");
            break;
      }
	  if((sm->metrics_num > 4)||(sm->offset_to_sob > 4))
	  {
  #ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
	    ERR_GERAN_FATAL_0_G("Buffer overflow of CCH parameters");
	  }

      /* Increment offset_to_sob for any bursts received */
      for (i = 0; i < sm->metrics_num; i++)
      {
         sm->rpt.metrics[i]->offset_to_sob++;
      }

#ifdef DEBUG_GSM_SLEEP_ERROR_RECOVERY
      MSG_GERAN_HIGH_3_G("ccch_rx_handler missed_frames %d FN %d decode_tag:%d",
                         sm->missed_frames, GSTMR_GET_FN_GERAN(gas_id), decode_tag);

      MSG_GERAN_HIGH_3_G("metrics_flag %d metrics_num %d offset_to_sob %d",
                          metrics_flag, sm->metrics_num, sm->offset_to_sob);
#endif

      /* Process the metrics if necessary */
#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
      // In case of GPLT and no RF, return default/canned result
      if (metrics_flag && (sm->state == CCCH_INACTIVE) && get_data)
      {
         MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_rx_handler:Calling ccch_process_metrics");
#else
      if (metrics_flag)
      {
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/
         ccch_process_metrics(sm, time_stamp,gas_id);
      }

      /* Process the data if necessary */
      if (get_data)
      {
         data_done = ccch_process_data(sm, decode_tag, last_attempt,gas_id);

#ifdef FEATURE_GSM_PCH_SINGLE_BURST_DEC_DEBUG
         if ( gl1_msg_is_sbd_enabled( gas_id ) &&
              sm->rpt.data->ccch.hdr.good_data )
         {
            gl1_hw_print_radio_block(sm->rpt.data->ccch.data,gas_id);
         }
#endif
      }

      /* Send the metrics once the data is done - for early decode there
         will not be four results yet */
      if (data_done && sm->callbacks.metrics_cb != NULL)
      {
         /* Fill in any missing metrics */
         for (i = sm->metrics_num; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
         {
            sm->rpt.metrics[i]->valid = FALSE;
            sm->rpt.metrics[i]->yield = FALSE;
            sm->rpt.metrics[i]->arfcn = NULL_ARFCN;
            sm->rpt.metrics[i]->offset_to_sob = 0;
            sm->rpt.metrics[i]->own_paging_block = sm->own_paging_block;
         }
         /* Fill in any missing metrics due to sleep wakeup error recovery */
         for (i = 0; i < sm->missed_frames; i++)
         {
            sm->rpt.metrics[i]->valid = FALSE;
            sm->rpt.metrics[i]->yield = FALSE;
            sm->rpt.metrics[i]->arfcn = NULL_ARFCN;
            sm->rpt.metrics[i]->offset_to_sob = 0;
            sm->rpt.metrics[i]->own_paging_block = sm->own_paging_block;
         }

         /* Make the callback */
         sm->callbacks.metrics_cb(sm->rpt.metrics,gas_id);
         gl1_hw_sleep_set_entered_recovery(gas_id, FALSE);
      }

#if (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))
      // In case of GPLT and no RF, return default/canned result
      MSG_GERAN_HIGH_0_G("GPLT DBG: ccch_rx_handler: Not going to get bursts");
#else
      /* Now go get the bursts if there are any to get */
      if (receive_burst && !data_done)
      {
         /* Prepare dynamic parameters */
         sm->signal_info.rx_signal_strength = sm->agc_ptr->pwr_dBm_x16;

#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
         sm->signal_info.rx_signal_strength_divrx = sm->agc_ptr_divrx_ccch->pwr_dBm_x16;
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/


         sm->burst_info.arfcn               = sm->ARFCN[sm->burst_num];
         sm->burst_info.decode_flag         = decode_flag;
         sm->burst_info.burst_num           = sm->burst_num;

         sm->burst_info.sbd_enabled         = sbd_allowed;
         sm->burst_info.epd_allowed         = epd_allowed;

#ifdef FEATURE_GL1_SLEEP_DEBUG_STRUCT_F3
         MSG_GERAN_HIGH_1_G("CCCH sm->burst_num %d", sm->burst_num );
#endif

#if defined (FEATURE_QBTA)
                  if ( (gl1_check_qbta_allowed(gas_id)) &&
                       (sm->state > CCCH_RECEIVE_2 ||
                       !gl1_hw_get_qta_cm_fw_rf_init_status(gas_id) ) )
                  {
                    gl1_hw_qta_fw_rf_init(FALSE,GL1_FW_QBTA_CM_START_UP_OFFSET,TRUE,sm->burst_info.arfcn,FALSE,FALSE,gas_id);
                  }
#endif

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
         sm->burst_info.amr_partial_decode = FALSE;
         sm->burst_info.dtx_detected = FALSE;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

         /* Issue the receive */
         gl1_hw_rx_dtm_gsm
          (
            &sm->signal_info,
            &sm->burst_info,
            &sm->tags[sm->burst_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
            start_of_block,
#elif defined (FEATURE_GSM_RX_DIVERSITY) && !defined(FEATURE_GSM_RX_DIVERSITY_IDLE)
            FALSE,
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/
            gas_id
          );

#if defined (FEATURE_QBTA)
         if ( gl1_check_qbta_allowed(gas_id) &&
              (sm->state > CCCH_RECEIVE_2   || !pwr_hndlr_regtd  )
         )
         {
            gl1_hw_qta_fw_rf_deinit(gas_id,GL1_FW_QBTA_CM_CLEAN_UP_OFFSET, TRUE);
         }
#endif
         sm->burst_num++;
         sm->burst_info.num_of_page_bursts_missed = 0;
      }
#endif /* (defined(FEATURE_GL1_GPLT) && defined(FEATURE_GL1_GPLT_CANNED_RSLT))*/

      /* Next state machine */
      sm_num = (sm_num + 1) % NUM_CCCH_SM;
      }

      /* Clear out inactive state machines */
      while (ccch_params[gas_id].sm[ccch_params[gas_id].sm_rd_ptr].state == CCCH_INACTIVE)
      {
        ccch_params[gas_id].sm_rd_ptr = (ccch_params[gas_id].sm_rd_ptr + 1) % NUM_CCCH_SM;
        if (--ccch_params[gas_id].sm_cnt == 0) break;
      }

      /* If the queue is empty, remove the handler */
      return (ccch_params[gas_id].sm_cnt == 0 ? TRUE : FALSE);
 }

boolean  gl1_msg_ccch_is_rx_on_next_frame( gas_id_t gas_id )
{
  switch( ccch_params[gas_id].sm[ccch_params[gas_id].sm_rd_ptr].state )
  {
  case  CCCH_RECEIVE_1:
  case  CCCH_RECEIVE_2:
  case  CCCH_RECEIVE_3:
  case  CCCH_RECEIVE_4:
  {
    return  TRUE;
  }
  break;

  default:
    break;
  }

  return  FALSE;
}

/*===========================================================================
FUNCTION gl1_hw_recovery_update_ccch_params

DESCRIPTION
Function used as part of the recovery mechanism when wakeup takes to long or it is delayed,
to update CCCH parameters and send them to FW

===========================================================================*/
void gl1_msg_recovery_update_ccch_params(gas_id_t gas_id )
{
   ccch_state_machine_type *sm;
   uint8   sm_num;
   uint8   missed_frames = gl1_hw_sleep_get_missed_frames(gas_id);

   sm_num = ccch_params[gas_id].sm_rd_ptr;
   sm = &ccch_params[gas_id].sm[sm_num];

   /* Missed frames due to sleep wakeup error recovery */
   sm->missed_frames = missed_frames;
#ifdef DEBUG_GSM_SLEEP_ERROR_RECOVERY
   MSG_GERAN_ERROR_3_G("ccch_rx_handler missed_frames %d FN %d timetick %d",
       sm->missed_frames,GSTMR_GET_FN_GERAN(gas_id),timetick_get_safe());
#endif
   sm->burst_info.num_of_page_bursts_missed = sm->missed_frames;
   sm->burst_num                            = sm->missed_frames;
   sm->metrics_num                          = sm->missed_frames;
   /* burst_info.burst_num is used later to determine if the FirstBurst
      flag should be set when building a command for GFW */
   sm->burst_info.burst_num                 = sm->missed_frames;

   gl1_hw_sleep_clear_missed_frames(gas_id);
#ifdef DEBUG_GSM_SLEEP_ERROR_RECOVERY
   MSG_GERAN_ERROR_2_G("CCCH HANDLER: sm_num=%d, CCCH RECOVERY sm->burst_num %d", sm_num, sm->burst_num);
#endif

#ifdef FEATURE_QTA
   if( gl1_hw_qta_gap_active(gas_id) )
   {
#if defined (FEATURE_QBTA)
     if(gl1_hw_check_in_QBTA(gas_id))
     {
       gl1_hw_set_QBTA_flag(gas_id,FALSE);
       MSG_GERAN_ERROR_0_G("MAKING gap as partial QBTA issuing startup");
     }
#endif
     gl1_hw_qta_fw_rf_init(FALSE,GL1_FW_QBTA_CM_START_UP_OFFSET,FALSE,sm->burst_info.arfcn,FALSE,TRUE,gas_id);
   }
#endif

   gl1_hw_rx_dtm_gsm
   (
     &sm->signal_info,
     &sm->burst_info,
     &sm->tags[sm->burst_num],
#ifdef FEATURE_GSM_RX_DIVERSITY
     FALSE,
#endif
     gas_id
   );

   sm->burst_num++;
   sm->burst_info.num_of_page_bursts_missed = 0;
   }
