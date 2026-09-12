/*===========================================================================

        IMPLEMENTATION OF FUNCTIONS COMMON TO GSM GPRS L1 MESSAGE LAYER

GENERAL DESCRIPTION
   This file implements functions that are common the GSM and GPRS Non-Portable
   Layer 1 (NPL1) message layer functions.


EXTERNALIZED FUNCTIONS

gl1_msg_init
gl1_msg_terminate
gl1_msg_frame_tick_notification


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called

gl1_msg_init
gl1_msg_frame_tick_notification  (every frame )
gl1_msg_terminate


Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
23/08/16   bg       CR1045621 Updating Rx Div Ant parameters in Engineering mode
27/05/16   snjv     CR1000909 pass ACI400 switchpoint detection stat from GFW to GRF via GL1
11/10/15   tsk      CR937467: Relocate dynamic RxD handler to end of gstmr_frame_tick_process.
09/10/15   akh      CR912421 FN/QS not correct for active ttg, sleep ttg resp delayed by 1s
28/09/15   pg       CR909305 Update dynamic RxD switching and remove pwr_meas suspension  
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
21/04/15   cs       CR808438 Ensure any pending TT requests are sent before cell
                    information is updated
20/02/15   ab       CR773948 GL1 Support for provding Dumps and Metrics for QSH
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
16/03/15   js       CR633570 Issue cleanup in first frame if no power monitors are scheduled
03/04/15   br       CR763774 GL1 logging improvement to ease debugging
09/02/15   js       CR775188 GL1 changes to receive EPD preference 
30/10/14   cs       CR701894 Enhanced Page Detection Support
30/09/14   nk       CR732309 Update check in ft_remove_handler function to cater to boundary values
23/10/14   pa       CR744906: Allow one IRAT DRDSDS activity at a time. 
23/09/14   cja      CR728745 Always clear IR mem when enable IR
01/09/14  dg        CR716836 Add print inside function gl1_msgi_remove_ft_handler_by_index to track the handler being removed 
06/08/14   df       CR536059 Allow Asynchronous sleep for all idle activities
02/05/14   br       CR655411 Remove amr_tx_handler if any left active while moving out of dedicated mode.
01/04/14   pjr      CR637457 Remove FEATURE_DUAL_SIM around gl1_hw_data_init
19/12/13   jk       CR503359 - GL1 changes to support Rx diversity
12/12/13   ssh      CR588188: ISR overrun recovery mechanism
09/12/13   sp       CR586547: Update gl1_msg_get_all_subs_in_idle_mode to return based on RR Standby pref.
07/12/13   js       CR586603 GSM has not called Enter mode
02/12/13   zf       CR583263: Only enable G+G mode while multi-mode sub is G only
25/11/13   zf       CR581089: remove multisim lock when checking mode
19/11/13   ssh/ip   CR547956 DSDS/DSDA GL1 timeline Opt (parallelize the RF config. with GL1 warm-up)
21/11/13   cja      CR455527 Mainline or remove features to tidy up code.
30/10/13   npt      CR569433 Remove FEATURE_DUAL_SIM around gl1_hw_data_init
16/09/13   cs       CR503042 Updated XO RGS handling for Triton
05/09/13   nm       CR538507 Remove Null of handlers when calling gl1_msg_reset
                             and resetting of num_ft_handlers again as this is causing more crashes
23/07/13   dv       CR531410 Revert CR503902
23/08/13   sk       CR533467 Partial QBTA bringup on 2.0 in DSDS
15/08/13   sk       CR524039 GBTA bringup changes
29/07/13   sk       CR519663 Partial QBTA bringup changes
01/07/13   dv       CR503902 Remove Null of handlers when calling gl1_msg_reset
                             and resetting of num_ft_handlers
19/06/13   cs       Major Triton TSTS Syncup
17/05/13   cs       TSTS Updates
16/05/13   ab       CR487396 - Support EGPRS IR Reduced Window in GFW
20/05/13   npt      CR417466 - Ensure that opt3 is not kicking in
                    if change to GSM only mode is being processed
29/04/13   ws       CR481359 enable GTA support
25/04/13   pjr      CR475805 removed check in gl1_msg_set_multi_sim_mode
13/03/13   mc       CR459487 : COEX Support - GERAN arbitrator framework
14/02/13   pjr      CR452272 added support to set multisim mode at activation time
04/01/13   og       CR427574. Make sure the PDTCH handler is active before scheduling
                    G2T searches.
05/11/12   cja      CR414098 Tidy-up F3 for MCPM builds
17/07/12   jj       CR378753 - clear gl1_msgi_add_ft_handler at gl1_msg_reset
15/06/12   jj       CR363500 Clearing all active handlers if msg layer reset happens
31/05/12   cja      CR366518 Correct g2t startup/cleanup time decrement
28/05/12   ws       CR364339 Only suspend power meas for IDLE frame during WCDMA ID search
                    avoids Ncell fluctuations in PTM as power meas in IDLE+1 is ignored
25/05/12   cja      CR363431 Suspend G power measurements during T measuremente
24/04/12   npt      CR354790 - Remove old sleep debug (under DEBUG_GL1_SLEEP) as it is not needed
                    after FEATURE_GL1_SLEEP_DEBUG_STRUCT is introduced
14/03/12   pg       Add support for QuRT
12/03/12   cgc      CR339393 add gl1_dtm_speedup_notify()
08/03/12   ip       CR326699 - Code changed for wake up recovery mechanism
22/11/11   jj       CR319573 incorporate with review comments
16/11/11   jj       CR 319573   changes for  profiling the modem performance
08/02/11   cja      Initial changes for Nikel
11/03/11   pg       CR278827: Add RSSI signal info to Eng Mode IF
23/02/11   pg       CR276800 Add RSSI DIAG logging functionality
09/12/10   cs       CR264682 Add support for asynch dsp completion interrupt
30/11/10   ab       Support EGPRS WS REDUCTION during allocation of dynamic memory in start
                    gsm mode request when FEATURE_GSM_EGPRS_IR_OPT/PER TBF is defined.
02/08/10   ws       CR248480 - Resume power measurements in IRAT after W activity
22/07/10   ip       CR245474 Increased the scope of the sleep logging
04/05/10   cja      Fix compiler warning
06/04/10   cja      Setting of mdsp feature enable to match DSP functionalty.
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
09/06/09   nf       Added FEATURE_GSM_PCH_SINGLE_BURST_DEC
06/06/09   ab       Feature FEATURE_GSM_EGPRS_IR_OPT for LCU
29/05/09   ws       FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
13/02/09   ap       Lint fixes for high Warning
30/01/09   ip       CR 167135 Added sleep debug code
08/12/08   cs       Add in new split gl1_hw_clk_ctl.h from gl1_hw.h
03/12/08   ws       Added FEATURE_GSM_DYNAMIC_SAIC
28/10/08   agv      CR161919 - Fix for UL Transmit Slots not reset at end of
                    E/GPRS TBF for GSM call.
09/10/08   cs       Allow the RX Sweep acquisition to be restarted
20/05/08   cs       Fix Klocwork error
21/04/08   ws       CR 144248 Enable and disable ESAIC in gl1_msg_set_egprs_mode() and
                    improve F3 for SAICS status
07/04/08   cs       Avoid disabling all IQ channels on TCH release/handover
01/02/08   cs       Fix IQ logging via QXDM
14/12/07   ws       merged LCU Bringup and demo changes from 6260 branch
09/11/07   nf       Added FEATURE_GSM_R_SACCH
22/10/07   ws       Add support for ESAIC NV 2508 item control
17/09/07   agv      Added I/Q logging to code base.
03/08/07   agv      CR123023 - Combined fix for AFC/TT/AGC.
23/05/07   agv      CR118309 - Large RSSI variation initial fix.
21/09/06   og       Reset only the cs part of lower layers when connecting DTM
                    assignment in packet transfer (EDTM route).
14/08/06   ws       Added gl1_hw_register_edge_clk() and modified
                    gl1_msg_set_egprs_mode()
10/25/05   gfr      Renamed gl1_hw_edge_feature_disable_ir
10/12/05   gfr      Add support for GSTMR tracing
10/12/05   gfr      Add support for ft handlers to add other handlers
08/17/05   gfr      Added gl1_msg_init_frame_tick_info, QCLint cleanup
08/03/05   gfr      Extra sanity when adding and removing handlers
07/29/05   gfr      DTM support
06/21/05   gfr      Call gl1_msg_tt_do_pending_adjust at end of FT processing
06/10/05   gfr      Move clk_regime calls to gl1_hw_gprs.c
06/08/05   yh       Change API for changing mdsp speed.
06/02/05   gfr      Add time_stamp to interference measure handler
05/19/05   gfr      Let mDSP driver handle enabling/disabling mclk for IR DM
05/12/05   yh       Clean up wcdma variables upon initialization.
04/28/05   gfr      Remove sleep disabling if FEATURE_GSM_MDSP_DM_ACTIVE_INT
                    is defined
04/22/05   gfr      Call sys_algo_gprs_agc_reset in gl1_msg_reset
04/22/05   gfr      Move sys_algo_gprs_agc_reset to gl1_msg_pdch.c
04/21/05   yh       Support for panic reset
04/14/05   gfr      Changed retrieved TA to uint8
04/14/05   yh       Add mDSP optimization
04/11/05   gfr      Code beautification
03/07/05   gfr      Add gl1_msg_IR_set_tfi
01/31/05   gfr      Removed IR_config function, support for dynamic disabling of IR
01/12/05   gfr      Lint cleanup
12/07/04   yhong    Lint cleanup
12/06/04   yhong    Support for FEATURE_GSM_MDSP_EGPRS
09/03/04   yhong    Moved the store_ta and retrieve_ta from gl1_msg_com.c to here
07/22/04   gfr      EGPRS support.
06/15/04   gfr      Added init function for async ccch channel.
06/12/04   gfr      Added init functions for some GSM channels.
05/20/04   gfr      Mainlined FEATURE_COMMON_PWR_HANDLER
03/05/04   gfr      Lint cleanup.
02/11/04   gfr      New HW scheduler support.
10/16/03   jc       Prevent GL1 from having vocoder control when returning back to GSM
10/15/03   gfr      Fix gl1_msg_adjust_time_stamp to take negative offsets.
08/29/03   jc       Remove ypf functions no longer needed in PLT.
05/13/03   gfr      Common power handler support.
05/12/03   gfr      Remove burst_schedule code.
04/16/03   gfr      Validate the function pointer when removing the ncell
                    handler to avoid removing the wrong one.
04/04/03   gfr      Update the GPRS burst schedule structure.
02/25/03   gfr      Lint cleanup for GPRS.
02/24/03   gfr      Rename rxlev to pwr_meas for consistency with int_meas.
01/31/03   gfr      Keep track of first rx tn and burst count in a separate
                    structure.
01/15/03   gfr      Changed GPRS rxlev handler to take the first rx tn.
11/07/02   gfr      Rename internal GPRS functions to gl1_msgi_*.
11/07/02   gfr      Abort GPRS interference measures when aborting D/L.
11/03/02   tb       Added extra rx handler for INTERRAT_GTOW
10/22/0    gr       Moved power measure abort into the dl_abort function.
10/16/02   gr       Added gl1_msg_mon_abort for aborting power measures.
10/11/02   gr       Re-added mdsp abort dl functionality.
10/10/02   gr       Change DL abort function to not abort mdsp processing.
09/06/02   gr       Pass timestamp into GPRS rxlev handler for AGC
08/28/02   gr       Added GPRS DL abort functionality.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
06/04/02   bbd      Changed FEATURE_GPLT to FEATURE_GSM_PLT
                    Changed FEATURE_GPRS to FEATURE_GSM_GPRS
04/24/02   JC       Added an API to adjust the local time stamp when in DRX
                    so that we don't upset the slew rate fcn of AGC.
04/16/02   bk       Implemented GPRS med. acc. mode API
02/21/02   dp       Cleaned up warnings
02/14/02   gw       Replaced gl1_msg_reset_sync_rx_seq_nums with more general
                    gl1_msg_reset, to support AGC.
02/01/02   JC       Added local_time_stamp variable and removed parameter
                    passed into the gl1_msg_frame_tick_notification() fcn.
01/14/02   sg       Added GPRS power measurement support
12/18/01   dp       Added support to abort ncell rx.  Also renamed the ncell
                    handler type to include the acquisition handler
12/12/01   gw       Added support for returning the YPF word returned by the
                    mDSP with decoded data.  To use, FEATURE_GPLT must be
                    defined.
11/9/01    bk       Added GPRS support
10/31/01   gw       Added support for logging debug info.  Added support for
                    returning metrics in dedicated mode.
10/10/01   gw       Added support for Ncell SCH and BCCH reception.
08/31/01     jc     Initial version.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* type definitions */

#include "geran_variation.h"

#ifdef CUST_H
#include "customer.h"
#endif

#include "comdef.h"

#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_hw_debug.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1_drx.h"
#include "gl1_mutex.h"

#include "gl1_hw_clk_ctl.h"

#ifdef FEATURE_GSM_MDSP_EGPRS
#include "mdsp_gprs.h"  /* IR functions */
#endif

#include "msg.h"

#include "l1_sc_int.h"
#include "l1i.h"
#include "l1_isr.h"

/* header file to support all 52 multi-frame channels */
#include "gl1_msg_pdch.h"
#include "gl1_sys_algo_gprs.h"

#include "geran_eng_mode_info.h"

#ifdef FEATURE_GSM_COEX
#include "gl1_hw_arbitration.h"
#endif

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#ifdef FEATURE_QSH_DUMP
#include "mdsp_intf_g.h"
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY
#include "gl1_hw_rxdiversity.h"
#endif /*FEATURE_GSM_RX_DIVERSITY*/

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

boolean gl1_msg_is_epd_allowed( gas_id_t gas_id );

/*****************************************************

 LOCAL STORAGE

******************************************************/

/* Structure containing the frame tick handler and type */
typedef struct
{
   gl1_msgi_ft_handler_type      type;
   gl1_msgi_ft_handler_func_type handler;
} ft_handler_type;

#define MAX_FRAME_TICK_HANDLERS 8

typedef struct
{
  ft_handler_type ft_handlers[MAX_FRAME_TICK_HANDLERS];
  uint8           num_ft_handlers;
  boolean         executing_ft_handlers;

  /* If a handler tries to add another handler while executing we need to
     save it and only add it the list after executing the remaining handlers */
  ft_handler_type ft_handlers_pending[MAX_FRAME_TICK_HANDLERS];
  uint8           num_ft_handlers_pending;

  /* multi-purpose arbitrary frame counter */
  uint32 local_time_stamp;

  uint8 tx_ta;

  /* CS timeslot in DTM */
  gl1_defs_tn_type cs_tn;

  boolean gl1_msg_ir_enabled_state;

#if defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
  boolean gl1_trigger_iq_state;
  uint32  gl1_iq_start_fn;
#endif  /* FEATURE_GSM_LOG_IQ_SAMPLES */
}gl1_msg_data_t;

static gl1_msg_data_t gl1_msg_data[NUM_GERAN_DATA_SPACES];

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
 sys_modem_device_mode_e_type gl1_msg_multi_sim_mode = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

boolean gl1_msg_acq_complete( gas_id_t gas_id );

boolean gl1_msg_asynch_int_dbg( gas_id_t gas_id );
boolean gl1_msg_is_asynch_sleep_enabled( gas_id_t gas_id );
boolean gl1_msg_is_sleep_opt3_enabled( gas_id_t gas_id );
boolean gl1_msg_is_sbd_enabled( gas_id_t gas_id );
boolean gl1_msg_is_early_decode_disabled( gas_id_t gas_id );

extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

/* ========================================================================

                          LOCAL FUNCTIONS

========================================================================= */

/*=============================================================================
FUNCTION gl1_msgi_init_msg_data

DESCRIPTION
  Initialises gl1_msg_data structure for given gas id
==============================================================================*/
static void gl1_msgi_init_msg_data(gas_id_t gas_id)
{
  memset(&gl1_msg_data[gas_id],NULL,sizeof(gl1_msg_data_t));
}

/*=============================================================================
FUNCTION gl1_msg_set_num_assigned_tx_slots

DESCRIPTION
   Sets the number of assigned uplink slots.  This is used for determining
   the power backoff for multi-slot uplink.  Note that "assigned" is used
   meaning the number of slots in the PUA, whereas "allocated" refers to
   the number of USFs allocated in dynamic allocation.

============================================================================= */
void gl1_msg_set_num_assigned_tx_slots (uint8 num_assigned_tx_slots,gas_id_t gas_id)
{
   gl1_hw_set_num_assigned_tx_slots (num_assigned_tx_slots,gas_id);
}

/*===========================================================================

FUNCTION gl1_msg_bfi_mod_2

DESCRIPTION
   This triggers bfi AGC updates every 8 frames in dedicated 26.

===========================================================================*/
boolean gl1_msg_bfi_mod_2( gas_id_t gas_id )
{
  boolean result = FALSE;
  static  uint8 update_bfi_agc_count[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( 0 ) };

  update_bfi_agc_count[gas_id]++;

  if (update_bfi_agc_count[gas_id] >= 2)
  {
    update_bfi_agc_count[gas_id] = 0;
    result = TRUE;
  }

  return (result);
}



#if defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
/*===========================================================================

FUNCTION gl1_msg_trigger_iq

DESCRIPTION
   This triggers IQ Samples if gl1_msg_data[gas_id].gl1_trigger_iq_state is set true.

===========================================================================*/
boolean gl1_msg_trigger_iq( gas_id_t gas_id )
{
#ifdef GL1_IQ_USE_DED_TRIGGER
  #error code not present
#else
  return ( TRUE );
#endif /* GL1_IQ_USE_DED_TRIGGER */
}

/*===========================================================================

FUNCTION gl1_msg_set_iq_state

DESCRIPTION
   This sets/resets gl1_trigger_iq_stateto trigger IQ Samples or not.

===========================================================================*/
void gl1_msg_set_iq_state( boolean state, gas_id_t gas_id )
{
  /* For nornal selective IQ logging this will break everything unless needed */
#ifdef GL1_IQ_USE_DED_TRIGGER
  #error code not present
#else
  gl1_msg_data[gas_id].gl1_trigger_iq_state = TRUE;
#endif /* GL1_IQ_USE_DED_TRIGGER */
}

#endif /* FEATURE_GSM_LOG_IQ_SAMPLES */

/*===========================================================================

FUNCTION gl1_msg_set_ho_pwr

DESCRIPTION
   This signals the Msg Layer the Handover AGC power to use.

===========================================================================*/
void gl1_msg_set_ho_pwr( int16 agc_pwr,gas_id_t gas_id )
{
  gl1_hw_set_ho_pwr(agc_pwr,gas_id);
}

/*===========================================================================

FUNCTION gl1_msg_com_store_ta,   gl1_msg_retrieve_ta

DESCRIPTION
  This function stores/retrieves the timing advance as provided by the base
  station.  The timing advance changes infrequently about every 104 frames.


DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_com_store_ta ( uint8 ta ,gas_id_t gas_id )
{
  gl1_msg_data[gas_id].tx_ta = ta;
}


uint8 gl1_msg_retrieve_ta ( gas_id_t gas_id )
{
  return gl1_msg_data[gas_id].tx_ta;
}



/*===========================================================================

FUNCTION gl1_msg_reset_ft_handlers

DESCRIPTION
   This function resets all the ft handlers.

===========================================================================*/
static void gl1_msgi_reset_ft_handlers (gas_id_t gas_id)
{
   uint8 i;

   /* Should never reset with active handlers */
   if (gl1_msg_data[gas_id].num_ft_handlers != 0)
   {
      MSG_GERAN_ERROR_3_G("Init with %d active handlers: hdlr=%08x type=%d",
                gl1_msg_data[gas_id].num_ft_handlers,
                gl1_msg_data[gas_id].ft_handlers[0].handler,
                gl1_msg_data[gas_id].ft_handlers[0].type);
   }

   /* Wipe them all out anyway - in theory the state is already all null */
   for (i = 0; i < MAX_FRAME_TICK_HANDLERS; i++)
   {
      gl1_msg_data[gas_id].ft_handlers[i].type    = GL1_MSG_FT_HDLR_NULL;
      gl1_msg_data[gas_id].ft_handlers[i].handler = NULL;
   }

   gl1_msg_data[gas_id].num_ft_handlers = 0;
}



/*===========================================================================

FUNCTION gl1_msg_init

DESCRIPTION
   This function initializes the message layers. Initialization includes
   the following:
   - Initialization of all internal variables
   - Initialization of the frame layer
   The message layer is now ready to handle channel operations and monitoring.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_init( gas_id_t gas_id )
{
   gl1_msgi_init_msg_data(gas_id);

   /* Initialize the handler function pointers */
   gl1_msgi_reset_ft_handlers(gas_id);

   /* Initialize GSM channels */
   gl1_msgi_init_ncell_bcch(gas_id);
   gl1_msgi_init_ncell_sch(gas_id);
   gl1_msgi_init_ncell_fcch(gas_id);
   gl1_msgi_init_fcch_sch(gas_id);
   gl1_msgi_init_ccch(gas_id);
   gl1_msgi_init_async_ccch(gas_id);
   gl1_msgi_init_dl_tch(gas_id);
   gl1_msgi_init_dl_tch_amr(gas_id);
   gl1_msgi_init_ul_tch(gas_id);
   gl1_msgi_init_ul_tch_amr(gas_id);
   gl1_msgi_init_sacch(gas_id);
   gl1_msgi_init_sdcch(gas_id);
   gl1_msgi_init_rach(gas_id);

   gl1_msgi_init_cbch(gas_id);

   /* Set the CS slot to zero */
   gl1_msg_data[gas_id].cs_tn = GL1_DEFS_TN_0;

   /* Initialize the GPRS channel controller */
   gl1_msgi_pdch_init(gas_id);

   /* Initialize the GPRS logical channels */
   gl1_msgi_pbcch_init();
   gl1_msgi_pccch_init();
   gl1_msgi_dl_ptcch_init(gas_id);
   gl1_msgi_dl_pacch_pdtch_init(gas_id);
   gl1_msgi_prach_init();
   gl1_msgi_ul_ptcch_init(gas_id);
   gl1_msgi_ul_pacch_pdtch_init(gas_id);

   /* Initialize GPRS interference measures */
   gl1_msgi_int_meas_init(gas_id);

   /* Initialize common power measure handler */
   gl1_msgi_pwr_meas_init(gas_id);

   /* Initialize wcdma handlers */
   gl1_msg_wcdma_init(gas_id);

   /* Initialize the DTM message layer */
   gl1_hw_dtm_init(gas_id);

   /* Reset scheduler */
   gl1_hw_schedule_init(gas_id);

   gl1_hw_data_init(gas_id);

#ifdef FEATURE_GSM_RX_DIVERSITY
   gl1_hw_rxdiv_data_init(gas_id);
#endif

#ifdef GERAN_L1_ENHANCED_RX
   gl1_hw_lif_data_init(gas_id);
#endif /*GERAN_L1_ENHANCED_RX*/

#ifdef FEATURE_GSM_COEX
   /* Initialize the arbitration interface */
   gl1_hw_arbitration_init(gas_id);
#endif

   /*Reset ACI Global Array in gl1*/
   gl1_hw_aci_reset_table(gas_id);

   gl1_msg_data[gas_id].local_time_stamp = 0;

   gl1_msg_data[gas_id].gl1_msg_ir_enabled_state = FALSE;
}



/*===========================================================================

FUNCTION gl1_msg_terminate

DESCRIPTION
   Terminates all message layer processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_terminate( gas_id_t gas_id )
{
   /* Terminate the GPRS logical channels */
   gl1_msgi_ul_pacch_pdtch_terminate(gas_id);
   gl1_msgi_ul_ptcch_terminate(gas_id);
   gl1_msgi_prach_terminate();
   gl1_msgi_dl_pacch_pdtch_terminate(gas_id);
   gl1_msgi_dl_ptcch_terminate(gas_id);
   gl1_msgi_pccch_terminate();
   gl1_msgi_pbcch_terminate();

   /* Terminate the GPRS channel controller */
   gl1_msgi_pdch_terminate(gas_id);

   /* Terminate GPRS power measures */
   gl1_msgi_int_meas_terminate(gas_id);

   /* Terminate common power measure handler */
   gl1_msgi_pwr_meas_terminate(gas_id);

   /* Initialize the DTM message layer */
   gl1_hw_dtm_terminate();

   /* Initialize the handler function pointers */
   gl1_msgi_reset_ft_handlers(gas_id);
}



/*===========================================================================

FUNCTION gl1_msgi_remove_ft_handler_by_index

DESCRIPTION
  This function removes the given ft handler index.

===========================================================================*/
static void gl1_msgi_remove_ft_handler_by_index (uint32 handler_num,gas_id_t gas_id)
{
   uint32 i;

   /* Sanity check */
   if (gl1_msg_data[gas_id].num_ft_handlers == 0 || handler_num >= gl1_msg_data[gas_id].num_ft_handlers)
   {
      MSG_GERAN_ERROR_3_G("Unable to rm hdlr %d (num=%d), FN=%d",
                handler_num, gl1_msg_data[gas_id].num_ft_handlers, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }

   MSG_GERAN_HIGH_3_G("FT handler remove 0x%08x num_ft_handlers %d num %d",
      gl1_msg_data[gas_id].ft_handlers[handler_num].handler,gl1_msg_data[gas_id].num_ft_handlers
      ,handler_num);

   /* Shift handlers down, overwritting the deleted one */
   for (i = handler_num; i < gl1_msg_data[gas_id].num_ft_handlers - 1; i++)  /*lint !e574*/
   {
      gl1_msg_data[gas_id].ft_handlers[i] = gl1_msg_data[gas_id].ft_handlers[i + 1];
   }

   /* Clean out last handler just to be safe */
   gl1_msg_data[gas_id].ft_handlers[gl1_msg_data[gas_id].num_ft_handlers - 1].type = GL1_MSG_FT_HDLR_NULL;
   gl1_msg_data[gas_id].ft_handlers[gl1_msg_data[gas_id].num_ft_handlers - 1].handler = NULL;
   gl1_msg_data[gas_id].num_ft_handlers--;
}



/*===========================================================================

FUNCTION gl1_msgi_execute_ft_handler

DESCRIPTION
  This function executes frame tick handlers of the given type starting
  from the given one.
  It returns the index of the first un-executed handler.

===========================================================================*/
static uint32 gl1_msgi_execute_ft_handler
(
   gl1_msgi_ft_handler_type type,
   boolean block_start,
   uint32 next_hdlr,
   gas_id_t gas_id
)
{
   boolean remove_flag;

   while (next_hdlr < gl1_msg_data[gas_id].num_ft_handlers && gl1_msg_data[gas_id].ft_handlers[next_hdlr].type == type)
   {
#ifdef FEATURE_GSM_ISR_PROFILING
      if(type == GL1_MSG_FT_HDLR_DL)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(START_DL_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_DL_PDCH)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(START_DL_PDCH_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_UL)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(START_UL_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_UL_PDCH)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(START_UL_PDCH_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_ACQ_NCELL)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(START_ACQ_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_INT_MEAS)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(START_INT_MEAS_HANDLER,gas_id);
      }
#endif /*FEATURE_GSM_ISR_PROFILING */

      GSTMR_TRACE(gl1_msg_data[gas_id].ft_handlers[next_hdlr].handler);

      remove_flag = gl1_msg_data[gas_id].ft_handlers[next_hdlr].handler(block_start, gl1_msg_data[gas_id].local_time_stamp,gas_id);

#ifdef FEATURE_GSM_ISR_PROFILING
      if(type == GL1_MSG_FT_HDLR_DL)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_DL_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_DL_PDCH)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_DL_PDCH_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_UL)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_UL_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_UL_PDCH)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_UL_PDCH_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_ACQ_NCELL)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_ACQ_HANDLER,gas_id);
      }
      else if(type == GL1_MSG_FT_HDLR_INT_MEAS)
      {
         GL1_HANDLER_PERFORMANCE_LOGGING(STOP_INT_MEAS_HANDLER,gas_id);
      }
#endif /* FEATURE_GSM_ISR_PROFILING */

      if (remove_flag)
      {
         gl1_msgi_remove_ft_handler_by_index(next_hdlr,gas_id);
      }
      else
      {
         next_hdlr++;
      }
   }

   return next_hdlr;
}

/*===========================================================================

FUNCTION gl1_msgi_remove_ft_handler

DESCRIPTION
  This function removes the given ft handler.

===========================================================================*/
void gl1_msgi_remove_ft_handler (gl1_msgi_ft_handler_func_type handler,gas_id_t gas_id)
{
   uint8 i;

   MSG_GERAN_MED_2_G("FT handler remove 0x%08x num_ft_handlers %d",
                     handler,gl1_msg_data[gas_id].num_ft_handlers);

   /* Check if we are in currently executing a handler - trying to remove
      a handler while executing will mess up the gl1_msg_data[gas_id].ft_handlers array */
   if (gl1_msg_data[gas_id].executing_ft_handlers)
   {
      MSG_GERAN_ERROR_1_G("Cannot rm hdlr 0x%08x while executing!",handler);
      return;
   }

   /* Find the handler in the array */
   if ( gl1_msg_data[gas_id].num_ft_handlers <= MAX_FRAME_TICK_HANDLERS )
   {
     for (i = 0; i < gl1_msg_data[gas_id].num_ft_handlers; i++)
     {
       if (gl1_msg_data[gas_id].ft_handlers[i].handler == handler)
       {
         gl1_msgi_remove_ft_handler_by_index(i,gas_id);

         return;
       }
     }
   }

   MSG_GERAN_ERROR_1_G("FT handler 0x%08x not found",handler);
}

/*===========================================================================

FUNCTION gl1_msgi_check_ft_handler_registered

DESCRIPTION
  This function checks if particular type of handler is registered or not.

===========================================================================*/

boolean gl1_msgi_check_ft_handler_registered( gas_id_t gas_id , gl1_msgi_ft_handler_type type)
{
   uint8 i = 0;
   boolean found = FALSE;

   if ( gl1_msg_data[gas_id].num_ft_handlers < MAX_FRAME_TICK_HANDLERS )
   {
     for (i = 0; i < gl1_msg_data[gas_id].num_ft_handlers; i++)
     {
        if (gl1_msg_data[gas_id].ft_handlers[i].type == type)
        {
          found = TRUE;

          break;
        }
     }
   }

   return found;
}

/*===========================================================================

FUNCTION gl1_msgi_add_ft_handler

DESCRIPTION
  This function adds the given ft handler, sorted by type.

===========================================================================*/
void gl1_msgi_add_ft_handler
(
   gl1_msgi_ft_handler_func_type handler,
   gl1_msgi_ft_handler_type      type,
   gas_id_t                      gas_id
)
{
   uint32 i;

   /* Check for overflow */
   if (gl1_msg_data[gas_id].num_ft_handlers >= MAX_FRAME_TICK_HANDLERS)
   {
      MSG_GERAN_ERROR_2_G("Too many FT handlers installed (0x%08x of type %d)", handler, type);
      return;
   }

   /* Check handler does not already exist */
   for (i = 0; i < gl1_msg_data[gas_id].num_ft_handlers; i++)
   {
      if (gl1_msg_data[gas_id].ft_handlers[i].handler == handler)
      {
         MSG_GERAN_ERROR_2_G("FT handler 0x%08x already installed (type=%d)", handler, type);
         return;
      }
   }

   /* Check if we are in currently executing handlers - trying to add
      a handler while executing will mess up the gl1_msg_data[gas_id].ft_handlers array so we
      store it for later. */
   if (gl1_msg_data[gas_id].executing_ft_handlers)
   {
      gl1_msg_data[gas_id].ft_handlers_pending[gl1_msg_data[gas_id].num_ft_handlers_pending].handler = handler;
      gl1_msg_data[gas_id].ft_handlers_pending[gl1_msg_data[gas_id].num_ft_handlers_pending].type = type;
      gl1_msg_data[gas_id].num_ft_handlers_pending++;
      return;
   }

   /* Start at end of list and shift until we find the right insertion spot */
   for (i = gl1_msg_data[gas_id].num_ft_handlers; i > 0; i--)
   {
      if (type >= gl1_msg_data[gas_id].ft_handlers[i - 1].type)
      {
        break;
      }

      gl1_msg_data[gas_id].ft_handlers[i] = gl1_msg_data[gas_id].ft_handlers[i - 1];
   }

   /* Add it */
   gl1_msg_data[gas_id].ft_handlers[i].type = type;
   gl1_msg_data[gas_id].ft_handlers[i].handler = handler;
   gl1_msg_data[gas_id].num_ft_handlers++;

   MSG_GERAN_MED_3_G("FT handler add 0x%08x num_ft_handlers %d type:%d",
                      handler,gl1_msg_data[gas_id].num_ft_handlers,type);
}

/*=============================================================================
FUNCTION  gl1_msgi_is_ft_handler_registered

DESCRIPTION
   This function returns whether handler is registered or not.

============================================================================= */
boolean gl1_msgi_is_ft_handler_registered
(
   gl1_msgi_ft_handler_func_type handler,
   gl1_msgi_ft_handler_type      type,
   gas_id_t                      gas_id
)
{
  uint16  handler_idx;
  boolean handler_found = FALSE;

  /* Null is not a valid handler */
  if ( !handler )
  {
    return FALSE;
  }

  /* No need to lock ISR as this function will be called in ISR context */
  /* Search the handler table for a match */
  for (handler_idx = 0; handler_idx < gl1_msg_data[gas_id].num_ft_handlers; handler_idx++)
  {
    if ((gl1_msg_data[gas_id].ft_handlers[handler_idx].handler == handler) &&
        (gl1_msg_data[gas_id].ft_handlers[handler_idx].type == type))
    {
      handler_found = TRUE;
      break;
    }
  }

  if ( !handler_found ) 
  {
    /* Now check in Pending handlers*/
    for (handler_idx = 0; handler_idx < gl1_msg_data[gas_id].num_ft_handlers_pending; handler_idx++)
    {
      if ((gl1_msg_data[gas_id].ft_handlers_pending[handler_idx].handler == handler) &&
          (gl1_msg_data[gas_id].ft_handlers_pending[handler_idx].type == type))
      {
        handler_found = TRUE;
        break;
      }
    }
  }

  return(handler_found);
}

/*===========================================================================

FUNCTION gl1_msg_init_frame_tick_info

DESCRIPTION
  Initializes the frame tick info structure passed in.  Used to allow
  upper layer to be oblivious to all block start flags except the ones
  it is setting to TRUE.

===========================================================================*/
void gl1_msg_init_frame_tick_info (gl1_msg_frame_tick_type *ftn)
{
   ftn->next_frame_is_dl_block_start             = FALSE;
   ftn->next_frame_is_ul_block_start             = FALSE;
   ftn->next_frame_is_ul_pdch_block_start        = FALSE;
   ftn->next_frame_is_dl_pdch_block_start        = FALSE;
   ftn->next_frame_is_ul_ptcch_sacch_block_start = FALSE;
   ftn->next_frame_is_dl_ptcch_sacch_block_start = FALSE;
   ftn->next_frame_is_acq_ncell_block_start      = FALSE;
   ftn->next_frame_is_pwr_meas_block_start       = FALSE;
   ftn->next_frame_is_int_meas_block_start       = FALSE;
}



/*===========================================================================

FUNCTION gl1_msg_frame_tick_notification

DESCRIPTION
   Used to notify NPL1 message layer of frame, block, PTCCH and 26/52
   multi-frame idle frame, 51 multi-frame idle frame and SACCH frame boundaries.
   For correct operation of the NPL1 drivers it is imperative that this
   function be called every frame tick. All frame layer processing is done in
   this function.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_frame_tick_notification
(
   const gl1_msg_frame_tick_type *frame_tick_info,
   gas_id_t gas_id
)
{
   uint32 next_hdlr = 0;

   /* Increment our local frame timer */
   gl1_msg_data[gas_id].local_time_stamp++;

#ifdef FEATURE_GSM_ISR_PROFILING
   GL1_HANDLER_PERFORMANCE_LOGGING(START_MSG_FRAME_NOTIFICATION_HANDLER);
#endif /*FEATURE_GSM_ISR_PROFILING*/

   /* Trace */
   GSTMR_TRACE(gl1_msg_data[gas_id].local_time_stamp);

   gl1_hw_schedule_frame_tick(gl1_msg_data[gas_id].local_time_stamp,gas_id);

   /* Execute frame tick handlers.  This works by going through the sorted
      array of handlers and executing each type in order.  The execute
      function takes the type of handler to execute and where in the array
      to start from.  It then runs handlers until the first mismatch and
      returns this index.  This index is used as the start position for
      the next call to execute with the next type. */

   /* Execute all the downlink/uplink message handlers */
   gl1_msg_data[gas_id].executing_ft_handlers = TRUE;
   gl1_msg_data[gas_id].num_ft_handlers_pending = 0;


   /* Downlink handlers */
   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_DL,
                  frame_tick_info->next_frame_is_dl_block_start,
                  next_hdlr,
                  gas_id
                );

   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_DL_PDCH,
                  frame_tick_info->next_frame_is_dl_pdch_block_start,
                  next_hdlr,
                  gas_id
                );

   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_DL_PTCCH_SACCH,
                  frame_tick_info->next_frame_is_dl_ptcch_sacch_block_start,
                  next_hdlr
                  ,gas_id
                );

   /* Uplink handlers */
   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_UL,
                  frame_tick_info->next_frame_is_ul_block_start,
                  next_hdlr,
                  gas_id

                );

   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_UL_PDCH,
                  frame_tick_info->next_frame_is_ul_pdch_block_start,
                  next_hdlr,
                  gas_id
                );

   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_UL_PTCCH_SACCH,
                  frame_tick_info->next_frame_is_ul_ptcch_sacch_block_start,
                  next_hdlr,
                  gas_id
                );

   GSTMR_TRACE(gl1_msg_data[gas_id].local_time_stamp);

   /* Tick the DTM manager - must come after the DL and UL handlers but
      before the ncell and power measure handlers */
   gl1_hw_dtm_frame_tick(gas_id);

   /* Execute the remaining handlers */
   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_ACQ_NCELL,
                  frame_tick_info->next_frame_is_acq_ncell_block_start,
                  next_hdlr,
                  gas_id
                );
   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_INT_MEAS,
                  frame_tick_info->next_frame_is_int_meas_block_start,
                  next_hdlr,
                  gas_id
                );
   next_hdlr = gl1_msgi_execute_ft_handler
                (
                  GL1_MSG_FT_HDLR_PWR_MEAS,
                  frame_tick_info->next_frame_is_pwr_meas_block_start,
                  next_hdlr,
                  gas_id
                );

   gl1_msg_data[gas_id].executing_ft_handlers = FALSE;

   /* Sanity check - should never happen or the sorting must be messed up */
   if (next_hdlr != gl1_msg_data[gas_id].num_ft_handlers)
   {
      MSG_GERAN_ERROR_2_G("FT handlers out of sync %d != %d!", next_hdlr, gl1_msg_data[gas_id].num_ft_handlers);
   }

   /* Add any pending handlers now */
   for (next_hdlr = 0; next_hdlr < gl1_msg_data[gas_id].num_ft_handlers_pending; next_hdlr++)
   {
      gl1_msgi_add_ft_handler(gl1_msg_data[gas_id].ft_handlers_pending[next_hdlr].handler,
                              gl1_msg_data[gas_id].ft_handlers_pending[next_hdlr].type,gas_id);
   }

   /* Perform any delayed time tracking adjustments */
   gl1_msg_tt_do_pending_adjust(gas_id);

#ifdef FEATURE_GSM_ISR_PROFILING
   GL1_HANDLER_PERFORMANCE_LOGGING(STOP_MSG_FRAME_NOTIFICATION_HANDLER);
#endif /*FEATURE_GSM_ISR_PROFILING */

   GSTMR_TRACE(gl1_msg_data[gas_id].local_time_stamp);
}

/*===========================================================================

FUNCTION gl1_msg_store_ta

DESCRIPTION
  This function stores the ta in the pdch module for GPRS and com module for GSM
  The timing advance changes infrequently about every 104 frames.


DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_store_ta ( uint8 ta,gas_id_t gas_id )
{
   geran_eng_mode_data_write(ENG_MODE_TIMING_ADVANCE,&ta,TRUE,gas_id);
   gl1_msg_pdch_store_ta( ta,gas_id );

   gl1_msg_com_store_ta( ta,gas_id );
}



/*=============================================================================
FUNCTION  gl1_msg_adjust_time_stamp

DESCRIPTION
   This function provides an interface to add an offset to our local
   copy of time stamp.  This is necessary to support DRX operation.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_adjust_time_stamp (int32 offset,gas_id_t gas_id)
{
   gl1_msg_data[gas_id].local_time_stamp += offset;  /*lint !e737*/ /* loss of sign ok */
}

/*=============================================================================
FUNCTION  gl1_hw_msg_inc_fn

DESCRIPTION
   Increment frame number
DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_hw_msg_inc_fn ( int32 num,gas_id_t gas_id)
{
  gl1_drx_set_frame_number_adjustment (num,gas_id);
}

/*===========================================================================

FUNCTION gl1_msg_reset

DESCRIPTION
  Calls hw reset function to allow fifos in the drivers to be reset.
  This function should be called after any message layer abort functions
  have been invoked, before starting new message operations.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_reset( gas_id_t gas_id )
{
   /* Check if we have any active handlers as they will likely get messed up
      when everything is reset beneath them. */

   uint8 i;

   if (gl1_msg_data[gas_id].num_ft_handlers > 0)
   {
      MSG_GERAN_ERROR_3_G("Reset with %d active handlers: hdlr=%08x type=%d", gl1_msg_data[gas_id].num_ft_handlers,
                gl1_msg_data[gas_id].ft_handlers[0].handler, gl1_msg_data[gas_id].ft_handlers[0].type);

     for (i = 0; i < MAX_FRAME_TICK_HANDLERS; i++)
     {

       if (gl1_msg_data[gas_id].ft_handlers[i].type == GL1_MSG_FT_HDLR_PWR_MEAS )
       {

          /* Terminate common power measure handler */
          gl1_msgi_pwr_meas_terminate(gas_id);

          gl1_msgi_int_meas_terminate(gas_id);
          MSG_GERAN_HIGH_0_G( "terminate pending power measurements" );
       }

       if (gl1_msg_data[gas_id].ft_handlers[i].type == GL1_MSG_FT_HDLR_UL )
       {
          gl1_msg_abort_tx_tch_amr_hr ( gas_id );
          gl1_msg_abort_tx_tch_amr_fr ( gas_id );

          /* Terminate amr tx handler */
          MSG_GERAN_HIGH_0_G("Terminate amr tx handler" );
       }

       if (gl1_msg_data[gas_id].ft_handlers[i].type == GL1_MSG_FT_HDLR_INT_MEAS )
       {
          MSG_GERAN_HIGH_0_G("terminate pending interference measurement handler" );
       }
     }
   }

   /* Reset frame layer */
   gl1_hw_reset(gas_id);

   /* Reset GPRS frame layer */
   gl1_hw_gprs_reset(gas_id);

   /* Reset GPRS AGC */
   sys_algo_gprs_agc_reset(gas_id);
}

/*===========================================================================

FUNCTION gl1_msg_log_ft_handlers

DESCRIPTION
  Debug function which will log the FT handlers

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void gl1_msg_log_ft_handlers (gas_id_t gas_id)
{
   uint8 i;

   if (gl1_msg_data[gas_id].num_ft_handlers > 0)
   {
     MSG_GERAN_HIGH_1_G("FT Handers count %d",gl1_msg_data[gas_id].num_ft_handlers);

     for (i = 0; i < MAX_FRAME_TICK_HANDLERS; i++)
     {
       MSG_GERAN_HIGH_3_G("ft_handler[%d]:%08x type %d", i, gl1_msg_data[gas_id].ft_handlers[i].handler,
                           gl1_msg_data[gas_id].ft_handlers[i].type);
     }
   }
   else
   {
     MSG_GERAN_HIGH_0_G("FT Handlers count 0");
   }
}

/*===========================================================================

FUNCTION gl1_msg_reset_cs_only

DESCRIPTION
  Calls hw reset function to allow fifos in the drivers to be reset.
  This function should be called after any message layer abort functions
  have been invoked, before starting new message operations.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

void  gl1_msg_reset_cs_only( gas_id_t gas_id )
{
   /* Check if we have any active handlers as they will likely get messed up
      when everything is reset beneath them. */
   if (gl1_msg_data[gas_id].num_ft_handlers > 0)
   {
      MSG_GERAN_ERROR_3_G("Reset with %d active handlers: hdlr=%08x type=%d", gl1_msg_data[gas_id].num_ft_handlers,
                gl1_msg_data[gas_id].ft_handlers[0].handler, gl1_msg_data[gas_id].ft_handlers[0].type);
   }

   /* Reset frame layer */
   gl1_hw_reset_cs_only( gas_id);
}

#endif
#endif

/*=============================================================================
FUNCTION gl1_msg_set_cs_timeslot

DESCRIPTION
   This function sets the timeslot to be used for the CS channels (SACCH,
   SDCCH, TCH)

============================================================================= */
void gl1_msg_set_cs_timeslot (gl1_defs_tn_type tn,gas_id_t gas_id)
{
   if (gl1_msg_data[gas_id].cs_tn != tn)
   {
      MSG_GERAN_HIGH_3_G("CS slot set to %d from %d, FN=%d", tn, gl1_msg_data[gas_id].cs_tn, GSTMR_GET_FN_GERAN(gas_id));
      gl1_msg_data[gas_id].cs_tn = tn;

      if(tn==GL1_DEFS_TN_0)
      {
         /*
         ** CgC if we held DTM speed on exit from DTM
         ** notify speed can now be reduced
         */
         gl1_dtm_speedup_notify(gas_id);
     }
   }
}

/*=============================================================================
FUNCTION gl1_msg_get_cs_timeslot

DESCRIPTION
   This function gets the timeslot in use for the CS channels (SACCH,
   SDCCH, TCH)

============================================================================= */
gl1_defs_tn_type gl1_msg_get_cs_timeslot (gas_id_t gas_id)
{
   return gl1_msg_data[gas_id].cs_tn;
}

/*=============================================================================
FUNCTION gl1_msg_is_aeq_egprs_enabled

DESCRIPTION
   determines if AEQ for EGPRS has been disabled via NV item

RETURNS
  TRUE if AEQ is disabled FALSE otherwise
============================================================================= */
boolean gl1_msg_is_aeq_egprs_enabled(gas_id_t gas_id)
{
 boolean enabled = TRUE;

 if( gl1_hw_feature_disabled_aeq(gas_id) )
 {
   enabled = FALSE;
   MSG_GERAN_HIGH_0_G("AEQ disabled in NV");
 }
 else
 {
    if( gl1_hw_feature_disabled_esaic(gas_id) )
    {
       enabled = FALSE;
       MSG_GERAN_HIGH_0_G("ESAIC disabled in NV");
    }
 }

 return(enabled);
}
/*=============================================================================
FUNCTION gl1_msg_set_medium_access_mode

DESCRIPTION
   This function sets the medium access mode to FIXED, DYNAMIC or EXT DYNAMIC
   ALLOCATION.

============================================================================= */
void gl1_msg_set_medium_access_mode
(
   gl1_defs_access_mode_type mode,
   gas_id_t gas_id
)
{
   gl1_msg_pdch_set_mac( mode,gas_id );
}


/*=============================================================================
FUNCTION gl1_msg_IR_enable

DESCRIPTION
   This function enables or disables acknowledged mode, used for incremental
   redundancy.

============================================================================= */
void gl1_msg_IR_enable
(
   boolean enable,
   gas_id_t gas_id
)
{
#ifdef FEATURE_GSM_MDSP_IR
   if (enable && gl1_hw_feature_disabled_ir(gas_id))
   {
      MSG_GERAN_HIGH_0_G("*** IR manually disabled ***");
   }
   else
   {
      gl1_msg_data[gas_id].gl1_msg_ir_enabled_state = enable;
      mdsp_gprs_IR_enable(enable ,gas_id);
   }
#else
   if (enable)
   {
      MSG_GERAN_ERROR_0_G("MDSP does not support IR");
   }
#endif
}/*lint !e715*/ /*enable not referenced if FEATURE not defined */

/*=============================================================================
FUNCTION gl1_msg_is_IR_enabled

DESCRIPTION
   This function returns state of IR enabled/disabled

============================================================================= */
boolean gl1_msg_is_IR_enabled (gas_id_t gas_id)
{
   return (gl1_msg_data[gas_id].gl1_msg_ir_enabled_state);
}

/*=============================================================================
FUNCTION gl1_msg_IR_clear_memory

DESCRIPTION
   Clears a range of incremental redundancy memory.

============================================================================= */
void gl1_msg_IR_clear_memory
(
   uint16 starting_bsn,
   uint16 ending_bsn,
   gas_id_t gas_id
)
{
#ifdef FEATURE_GSM_MDSP_IR
   mdsp_gprs_IR_clear_memory(starting_bsn, ending_bsn ,gas_id);
#endif
}/*lint !e715*/ /*starting_bsn and ending_bsn not referenced if FEATURE not defined */



/*=============================================================================
FUNCTION gl1_msg_IR_set_active_window

DESCRIPTION
   Sets the active window for incremental redundancy.

============================================================================= */
void gl1_msg_IR_set_active_window
(
   uint16 starting_bsn,
   uint16 ending_bsn,
   gas_id_t gas_id
)
{
#ifdef FEATURE_GSM_MDSP_IR
   mdsp_gprs_IR_set_active_window(starting_bsn, ending_bsn ,gas_id);
#endif
}/*lint !e715*/ /*starting_bsn and ending_bsn not referenced if FEATURE not defined */



/*=============================================================================
FUNCTION gl1_msg_IR_set_tfi

DESCRIPTION
   Sets the TFI for incremental redundancy.

============================================================================= */
void gl1_msg_IR_set_tfi
(
   uint8 tfi,
   gas_id_t gas_id
)
{
#ifdef FEATURE_GSM_MDSP_IR
   mdsp_gprs_IR_set_tfi((uint16)tfi ,gas_id);
#endif
}/*lint !e715*/ /*tfi not referenced if FEATURE not defined */


/*=============================================================================
FUNCTION gl1_msg_set_egprs_mode

DESCRIPTION
   This function enables or disables egprs mode.

============================================================================= */
void gl1_msg_set_egprs_mode
(
   boolean egprs_mode,
   boolean reg_clk,
   gas_id_t gas_id
)
{
   gl1_hw_set_egprs_mode(egprs_mode,reg_clk,gas_id);

   if (egprs_mode)
   {
      MSG_GERAN_HIGH_2_G("EGPRS mode enabled, reg clks %d FN=%d",reg_clk,GSTMR_GET_FN_GERAN(gas_id));

      /* Leave SAIC on all the time if AEQ and ESAIC are enabled */
#if defined (FEATURE_GSM_MDSP_ESAIC)
      /* Turn off SAIC when we setup for EGPRS as mDSP desn't currently support it */
      if(!gl1_msg_is_aeq_egprs_enabled(gas_id))
        gl1_hw_cfg_saic(FALSE,gas_id);
#endif
   }
   else
   {
      MSG_GERAN_HIGH_2_G("EGPRS mode disabled reg clks %d, FN=%d",reg_clk,GSTMR_GET_FN_GERAN(gas_id));

      /* Leave SAIC on all the time if AEQ and ESAIC are enabled */
#if defined (FEATURE_GSM_MDSP_ESAIC)

      if(!gl1_msg_is_aeq_egprs_enabled(gas_id))
        /* Turn SAIC back on after EGPRS TBF is released */
        gl1_hw_cfg_saic(TRUE,gas_id);
#endif

   }
}


#ifdef FEATURE_GSM_EGPRS_IR_WS_REDUCTION
/*===========================================================================

FUNCTION gl1_msg_get_ir_oper_win_size

DESCRIPTION
  return the size of phyical memory space allocated for edge IR  to pass
  to L1/RLC
  Also set the operational WS , can be updated after panic reset.
===========================================================================*/
uint16 gl1_msg_get_ir_oper_win_size(gas_id_t gas_id)
{
  return (mdsp_gprs_get_oper_win_size(gas_id));
} /* gl1_msg_set_ir_oper_win_size */

#endif /* FEATURE_GSM_EGPRS_IR_WS_REDUCTION */



#ifdef FEATURE_GSM_DYNAMIC_SAIC
/*===========================================================================

FUNCTION gl1_msg_get_saic_state

DESCRIPTION
  Informs Pl1 if SAIC is currently enalbed or disabled

DEPENDENCIES
  None

RETURN VALUE
  Boolean - TRUE = Enabled, FALSE  = disabled
SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_get_saic_state(gas_id_t gas_id)
{
  return(gl1_hw_get_saic_state(gas_id) );
}
#endif

#ifdef FEATURE_GSM_MDSP_ESAIC
/*===========================================================================

FUNCTION gl1_msg_clear_esaic_iir_filter

DESCRIPTION
  Tells the mDSP to Clear it's IIR filter for use in applying the ESAIC on
  Downlink channels.

  Currently needs to be called For Downlink DTX and Handover scenarios

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_clear_esaic_iir_filter(boolean inter_handover, boolean result,gas_id_t gas_id)
{
  boolean flush_esacch, restore_ersacch = FALSE;

  /* Don't flush ESAIC Filter if ESAIC is disabled */
  if(gl1_hw_feature_disabled_esaic(gas_id) == FALSE )
  {
    if(inter_handover == TRUE)
    {
      if(result == TRUE)
      {
        /* inter-cell Handover Passed */
        flush_esacch = TRUE;
        restore_ersacch = FALSE;
      }
      else
      {
         /* inter-cell Handover failed */
         flush_esacch = FALSE;
         restore_ersacch = TRUE;
      }
    }
    else
    {
      /* No inter-cell Handover */
      flush_esacch = FALSE;
      restore_ersacch = FALSE;
    }

    MSG_GERAN_HIGH_3_G("ESAIC: Flush IIR Filter! flush %d restore %d [%d]",flush_esacch,restore_ersacch,GSTMR_GET_FN_GERAN(gas_id) );

    gl1_hw_flush_esaic_iir_filter(flush_esacch, restore_ersacch,gas_id);
  }
  else
  {
    MSG_GERAN_HIGH_0_G("ESaic disabled");
  }
}
#endif

/*=============================================================================
FUNCTION gl1_msg_acch_get_nv_flag

DESCRIPTION
   This function returns the NV Item for ACCH
DEPENDENCIES

RETURN VALUE
   uint16

SIDE EFFECTS
   None
============================================================================= */
uint16 gl1_msg_acch_get_nv_flag (gas_id_t gas_id)
{
    return gl1_hw_acch_get_nv_flag(gas_id);
}

/*=============================================================================
FUNCTION  gl1_msg_get_all_subs_in_idle_mode

DESCRIPTION This function returns true if all the subs have G tech active (idle)

DEPENDENCIES

RETURN VALUE True if all the subs are in G idle mode

SIDE EFFECTS
   None
============================================================================= */
boolean gl1_msg_get_all_subs_in_idle_mode(void)
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
    uint8 count;
    uint8 num_subs_sys_pref = 0;
    uint8 num_subs_in_idle = 0;
    sys_modem_dual_standby_pref_e_type sys_pref = geran_get_sys_multi_sim_pref();

    /* Numb of subs in l1_state idle */
    for ( count = GERAN_DATA_SPACE_INDEX_1; count < NUM_GERAN_DATA_SPACES; count++ )
    {
      if ( l1_tsk_buffer[count].l1_state == L1_MULTI_SIM_IDLE )
      {
        num_subs_in_idle++;
      }
    }

    /* The device sys preference */
    switch (sys_pref)
    {
      case SYS_MODEM_DS_PREF_SINGLE_STANDBY:
         num_subs_sys_pref = 1;
         break;

      case SYS_MODEM_DS_PREF_DUAL_STANDBY:
      case SYS_MODEM_DS_PREF_DUAL_STANDBY_NO_TUNEAWAY:
         num_subs_sys_pref = 2;
         break;

      case SYS_MODEM_DS_PREF_TRIPLE_STANDBY:
      case SYS_MODEM_DS_PREF_TRIPLE_STANDBY_NO_TUNEAWAY:
         num_subs_sys_pref = 3;
         break;

      default:
         MSG_HIGH("Current SysMode Pref %d, defaulting to all subs active", sys_pref,0, 0);
         num_subs_sys_pref = 200; // Essentially making an invalid value
         break;
    }

    if (num_subs_in_idle == num_subs_sys_pref)
    {
       /*Check additionally if gs1 is G-only mode.*/
       if (l1_tsk_buffer[GERAN_ACCESS_STRATUM_ID_1].l1_state == L1_MULTI_SIM_IDLE)
       {
         if ( gl1_hw_get_gsm_only_mode( GERAN_ACCESS_STRATUM_ID_1 ) )
         {
           return TRUE;
         }
         else
         {
           return FALSE;
         }
       }
       return TRUE;
    }
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

    return FALSE;
}

/*=============================================================================
FUNCTION  gl1_msg_get_multi_sim_mode

DESCRIPTION This function returns true if multisim mode has been set to
            DSDS or DSDA, false otherwise

DEPENDENCIES

RETURN VALUE This function returns true if multisim mode has been set to
             DSDS or DSDA, false otherwise

SIDE EFFECTS
   None
============================================================================= */
boolean gl1_msg_get_multi_sim_mode(void)
{
   boolean result = FALSE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   result = ( gl1_msg_get_multi_sim_standby_mode()
              || ( gl1_msg_multi_sim_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE ) );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   return result;
}

/*=============================================================================
FUNCTION  gl1_msg_get_multi_sim_standby_mode

DESCRIPTION This function returns TRUE if multisim standby mode has been set to
            DSDS or TSTS, FALSE otherwise

DEPENDENCIES

RETURN VALUE This function returns TRUE if multisim standby mode has been set to
             DSDS or TSTS, FALSE otherwise

SIDE EFFECTS
   None
============================================================================= */
boolean gl1_msg_get_multi_sim_standby_mode( void )
{
   boolean result = FALSE;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   result = ( ( gl1_msg_multi_sim_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY )
#if defined ( FEATURE_TRIPLE_SIM )
              || ( gl1_msg_multi_sim_mode == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY )
#endif /* FEATURE_TRIPLE_SIM */
            );
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   return result;
}

/*=============================================================================
FUNCTION  gl1_msg_get_multi_sim_sys_mode

DESCRIPTION Returns the mode of operation set when GSM was activated

DEPENDENCIES

RETURN VALUE Returns the mode of operation set when GSM was activated

SIDE EFFECTS
   None
============================================================================= */
sys_modem_device_mode_e_type gl1_msg_get_multi_sim_sys_mode(void)
{
   sys_modem_device_mode_e_type result = SYS_MODEM_DEVICE_MODE_SINGLE_SIM;

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
   result = gl1_msg_multi_sim_mode;
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

   return result;
}


/*=============================================================================
FUNCTION  gl1_msg_set_multi_sim_sys_mode

DESCRIPTION Sets gl1_msg_multi_sim_mode value. This function should be called
            when GSM is activated and deactivated.

DEPENDENCIES

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
void gl1_msg_set_multi_sim_sys_mode(sys_modem_device_mode_e_type multi_sim_mode)
{
#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
  GL1_MULTI_SIM_MODE_LOCK();

  gl1_msg_multi_sim_mode = multi_sim_mode;

  GL1_MULTI_SIM_MODE_UNLOCK();
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

  return;
}


/*===========================================================================

FUNCTION gl1_msg_pass_xmsi_to_mdsp

DESCRIPTION

RETURN VALUE None
===========================================================================*/
void gl1_msg_pass_xmsi_to_mdsp (boolean imsi_valid,  uint8 *imsi_ptr,
                                boolean tmsi_valid,  uint8 *tmsi_ptr,
                                boolean ptmsi_valid, uint8 *ptmsi_ptr,
                                gas_id_t gas_id)
{
    gl1_hw_pass_xmsi_to_mdsp(imsi_valid, imsi_ptr,
                             tmsi_valid, tmsi_ptr,
                             ptmsi_valid, ptmsi_ptr,gas_id);
}

/*===========================================================================

FUNCTION gl1_msg_ncell_pch_low_monitor_active

DESCRIPTION
  Inform lower layers if the reduced power monitor state is active as this
  is used as a gating criteria for the asynch interrupt processing.

DEPENDENCIES
  None

RETURN
  Whether reduced ncell monitors are active

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_ncell_pch_low_monitor_active( gas_id_t gas_id )
{
  return ( l1_sc_ncell_pch_low_monitor_active(gas_id) );
}

/*===========================================================================

FUNCTION gl1_msg_get_dtm_expected_power

DESCRIPTION
  Get latest expected_power stored in gl1_hw_dtm_params

DEPENDENCIES
  None

RETURN VALUE
  uint16
SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_msg_get_dtm_expected_power(gas_id_t gas_id)
{
  return(gl1_hw_get_dtm_expected_power(gas_id));
}
/*===========================================================================

FUNCTION gl1_msg_get_dtm_expected_power_divrx

DESCRIPTION
  Get latest expected_power_divrx stored in gl1_hw_dtm_params

DEPENDENCIES
  None

RETURN VALUE
  uint16
SIDE EFFECTS
  None
===========================================================================*/
int32 gl1_msg_get_dtm_expected_power_divrx(gas_id_t gas_id)
{
  return(gl1_hw_get_dtm_expected_power_divrx(gas_id));
}

/*===========================================================================

FUNCTION gl1_msg_get_dtm_band

DESCRIPTION
  Get current dtm band stored in gl1_hw_dtm_params

DEPENDENCIES
  None

RETURN VALUE
  uint16
SIDE EFFECTS
  None
===========================================================================*/
uint8 gl1_msg_get_dtm_band(gas_id_t gas_id)
{
  return(gl1_hw_get_dtm_band(gas_id));
}
/*===========================================================================

FUNCTION gl1_msg_get_expected_power

DESCRIPTION
  Get current dtm ARFCN stored in gl1_hw_dtm_params

DEPENDENCIES
  None

RETURN VALUE
  uint16
SIDE EFFECTS
  None
===========================================================================*/
uint16 gl1_msg_get_dtm_arfcn(gas_id_t gas_id)
{
  return(gl1_hw_get_dtm_arfcn(gas_id));
}
/*===========================================================================

FUNCTION gl1_msg_is_asynch_int_possible

DESCRIPTION
  Inform lower layers if the asynch interrupt processing is allowed based
  on upper later conditions.

DEPENDENCIES
  None

RETURN
  Whether asynch interrupt processing is allowed

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_is_asynch_int_possible( gas_id_t gas_id )
{
  return ( l1_get_asynch_int_possible( gas_id ) );
}

/*===========================================================================

FUNCTION gl1_msg_set_mdsp_asynch_int

DESCRIPTION
  Set the enable trigger to drivers for the asynch interrupt

RETURN
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_set_mdsp_asynch_int( boolean asynch_int_enable,gas_id_t gas_id )
{
  gl1_hw_set_mdsp_asynch_int( asynch_int_enable,gas_id );
}

boolean gl1_msg_pdch_block_active( gas_id_t gas_id )
{
  uint16   i     = 0;
  boolean  found = FALSE;

  while( i < MAX_FRAME_TICK_HANDLERS )
  {
    if ( gl1_msg_data[gas_id].ft_handlers[i].type == GL1_MSG_FT_HDLR_DL_PDCH )
    {
      found = TRUE;

      break;
    }

    i++;
  }

  return  found;
}

/*===========================================================================

FUNCTION gl1_msg_get_rat_change_active

DESCRIPTION
  Inform lower layers if change to GSM only mode is being processed

DEPENDENCIES
  None

RETURN
  Whether change to GSM only mode is being processed

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_get_rat_change_active( gas_id_t gas_id )
{
   return ( l1_sc_get_rat_change_active (gas_id) );
}

/*===========================================================================

FUNCTION gl1_msg_acq_complete

DESCRIPTION
  Inform lower layers if cell acquisition completed.

DEPENDENCIES
  None

RETURN
  Whether cell acquisition is complete.

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_acq_complete( gas_id_t gas_id )
{
  return ( l1_is_cell_acq_complete( gas_id ) );
}
/* Run-time debug flag to enable extra asynch interrupt handling messaging */
boolean gl1_msg_asynch_int_dbg( gas_id_t gas_id )
{
  return ( gl1_asynch_int_dbg( gas_id ) );
}

/* Run-time debug flag to enable sleep */
boolean gl1_msg_is_asynch_sleep_enabled( gas_id_t gas_id )
{
  return ( gl1_is_asynch_sleep_enabled( gas_id ) );
}

/* Run-time debug flag to enable optin3 only */
boolean gl1_msg_is_sleep_opt3_enabled( gas_id_t gas_id )
{
  return ( gl1_is_sleep_opt3_enabled( gas_id ) );
}

/* Run-time debug flag to enable Single Burst Decode */
boolean gl1_msg_is_sbd_enabled( gas_id_t gas_id )
{
  return ( gl1_is_sbd_enabled( gas_id ) );
}

/* Run-time debug flag to disable early decode */
boolean gl1_msg_is_early_decode_disabled( gas_id_t gas_id )
{
  return ( gl1_is_early_decode_disabled( gas_id ) );
}

/* Run-time flag to enable Enhanced Page Decode */
boolean gl1_msg_is_epd_allowed( gas_id_t gas_id )
{
  /* Inverse of page sharing status */
  return ( l1_get_epd_pref(gas_id) );
}

#ifdef FEATURE_QSH_DUMP
/* get gfw qsh data dump */
void gl1_msg_qsh_get_gfw_data_dump(uint8 *dump_data_ptr1,uint8 *dump_data_ptr2,uint8  *gfw_result)
{
   mdsp_send_gfw_data_dump_req(dump_data_ptr1,dump_data_ptr2,gfw_result);
} /* gl1_msg_qsh_get_data_dump */
#endif /* FEATURE_QSH_DUMP */

/* EOF */
