/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     G L 1  M E S S A G E  L A Y E R -  N C E L L   FCCH   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for receiving the FCCH on a
   neighbor cell.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_ncell_fcch
   gl1_msg_rx_ncell_fcch
   ncell_fcch_rx_handler




INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_ncell_fcch.c#2 $
$DateTime: 2019/12/30 04:21:22 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
03/01/16   mn       CR1104539:Migrating to new API to expose the value of GPRS_ANITE_GCF in Gl1(rework of1094617)
22/05/15   ws       CR790493 Check BSIC decoded in SCH matches requested BSIC from RR
11/06/15   pg       CR850142 Featurise dedicated parallel SCH clock bump to avoid unnecessary MCPM/NPA calls 
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
28/01/14   jj       CR 787871  Limit the n cell acq offset in dtm 
18/12/14   cs       CR754853 Add support for Parallel SCH detection handling
13/08/13   dg       CR485683: In dedicated & packet transfer mode,Reject tone if its offset
                    is less than starting of FCCH window 
09/09/14   jk       CR697308:GL1 changes to support HLin LLin sawless feature
05/09/14   ws       CR 718881 Removing unused rfgsm_core_types.h
07/08/14   cja      CR643648 Desense for channel adjacent to spur
30/07/14   sp       CR676031:Use delayed abort for ncell fcch in TCH
04/03/14   sp       CR613435: Clear acq_in_progress if FCCH is aborted
23/10/13   sp       CR563626: Disable acq_in_progress in next frame after Stop_Acq is issued.
21/08/13   npt      CR531633 - Remove FEATURE_DIME_MODEM dependencies
20/06/13   cs       Major Triton TSTS Syncup
03/10/12   ky       Fixed compilation error
28/08/12   cja      CR386695 Use RFI API for Rx alpha
29/06/12   jj       CR374950 GL1 changes for 38 symbol HMSC monitor 
26/03/12   jj       CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33 
14/06/11   cja      CR264690 Acquisition frequency capture range larger
23/02/11   pg       Explicitly check num slots to determine if in HMSC mode before
                    applying min FCCH acq window size
14/02/11   cja      CR273625 Revert HMSC Rx alpha to old value
22/11/10   ky       Panic reset mechanism is changed. For all errors gl1 first
                    sees an nv item set or not. if this nv item is set then does
                    error fatal immediately other wise gl1 sends signal to grr for
                    recovery
02/11/10   cja      For HMSC 30-34 do not have minimum FCCH acq window size
26/10/10   cja      CR257870 Limit increase in FCCH acq window so that Tx in TS5 is not dicarded
14/10/10   pg       CR259000: Do not apply Acq window re-sizing to 9 slots in DTM mode
15/09/10   cja      CR255616. Ensure power measure start offset is in current frame
02/09/10   cja      Fix compilation warnings
23/08/10   cja      CR248465 Account for BB Rx clock gatig timing in TCH Ncell window
30/06/10   cja      CR243878 Correct scheduling of SCH if not 52 frames after FCCH detection
28/06/10   cja      CR242471 Ensure NCell acquisition window is at least 9 timeslots in dedicated mode
14/06/10   og       Schedule the stop acquisition a frame later in packet
                    transfer. Resolves CR240836.
27/04/10   cs       CR236460 Latest AFC\XO handling updates
08/02/10   cja      XO Update
25/01/10   cja      CR224507. For XO correct freq error setting for NCell.
05/06/09   cs       Ensure the fine_freq casting is consistent
26/05/09   cs       CR167848 fix for expanded afc_freq I/F to mDSP
09/04/09   og       Seperate the dedicated and transfer SCH scheduling decisions
                    CR174438.
20/03/09   cs       Fixes to the common 32-bit afc_freq mDSP I/F
12/03/09   og       FEATURE_XO support.
26/02/09   cs/hy    Add in initial XO support
17/09/08   nt       Re-Introduce the CR88026
22/04/08   og       Back out NCELL SCH changes for CR 88026.
11/04/08   og       NCELL SCH changes to support a wider range of offsets. CR 88026.
12/06/07   og       Upgrade the FEATURE_GSM_L1_HFREQ_ERR_NCELL_SCH_DECODE to
                    include dedicated TCH NCELL SCH attempts.
07/04/06   ws       Removed optimization to start window based on TA since mdsp
                    will silently cancel Transmits in idle -1 FN,
03/29/06   gfr      Add frequency offset to ncell functions to support non
                    spec-compliant networks in WCDMA mode.
26/01/06    og      Additions to allow TA to be included in NCELL ACQ offset
                    calculations.
11/10/05   gfr      Changed interface to take last timeslot instead of last
                    tx timeslot to give more flexibility to caller
09/22/05   gfr      Removed FEATURE_SBI_MDSP_CONTROLLER
07/29/05   gfr      DTM support
06/21/05   gfr      Remove calls to tt_suspend and tt_resume
04/28/05   gfr      Add guard period for safety
04/21/05   yh       Support for panic reset
04/20/05   yh       Remove use of RF fifo
04/18/05   gfr      Added frame number to status printout
12/07/04   yhong    Lint cleanup
12/06/04   gfr      Featurize the mDSP SBI controller
11/01/04   yhong    Changed NCell Abort API to take a parameter
09/23/04   gfr      Suspend time tracking during activity.
08/12/04   gfr      Add abort message.
07/22/04   gfr      Removed QS_PER_FRAME definition.
06/09/04   gfr      Consolidate static variables for robustness.
06/02/04   gfr      Lint cleanup.
05/08/04   gfr      Removed broken legacy support for back-to-back operation.
05/07/04   dp       Fixed bug in ncell rx power computation due to using the
                    wrong gain state
05/06/04   gfr      Combine phase and freq value for acq results.
03/05/04   gfr      Lint cleanup.
02/17/04   gfr      GSM rotators support.
11/05/03   gfr      Support for quad-band.
08/05/03   gfr      Disable FEATURE_8_SLOT_ACQ_WINDOW
07/25/03   gfr      Add FEATURE_8_SLOT_ACQ_WINDOW to force an 8 slot acq window.
07/08/03   bk       Cleaned up some F3 messages.
07/08/03   gfr      Added support for partial tone detection
06/09/03   gw       Added check on number of messages in process at once.
                    Maximum allowed is one.
06/05/03   gfr      Fix the window calculation in ncell_sch_pos_in_acq_window.
06/04/03   gfr      Suspend PSBI transactions during ncell activity.
06/04/03   gfr      Dynamically calculate the acquisition window size.
05/12/03   gfr      Changed gl1_msgi_schedule_first_rx_tn to
                    gl1_hw_schedule_first_rx_tn
05/06/03   dp       Pulled out Cetecom hack to reduce tone offset by 120 qs.
                    Was being done twice (also in l1_sc_acq.c )
04/16/03   gfr      Stop the acquisition at the right time when aborting and
                    pass the handler pointer when removing it.
04/11/03   bk       Added a fix to issue the stop acquisition command correctly
                    during an abort
03/10/03   dp       Cetecom hack to decrease tone offsets by 120qs
01/31/03   gfr      Schedule acquisition start one frame earlier.
01/15/03   gfr      Remove need to pass in first_rx_tn.
09/17/02   plm      Initialized local variable qbit_lag to zero to eliminate
                    compiler warning in function ncell_fcch_rx_handler().
08/23/02   plm      Removed declaration of read_count variable in ncell_fcch_rx_handler()
                    that was not being used to eliminate compiler warning.
07/18/02   ATM      Added FCCH/SCH decode logging.
07/08/02   dp       Put back handling of negative tone offsets.  Need this to
                    handle corner cases after the acq window was moved for
                    GPRS
06/28/02   bk       Added support to slide the acq window used in GPRS.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
05/30/02   dp       Increased 9 slot acq window by a fudge factor (24 qs) as
                    a temp workaround to solve the firmware bug (not able to
                    detect corner case tones with 9 slot window )
05/09/02   dp       Fix to ignore negative tone offsets returned by DSP and
                    wait for it to fall into the acquisition window
03/26/02   dp       Added gl1_msg_ncell_sch_pos_in_acq_window
02/20/02   dp       Cleaned up tone offset computation
02/14/02   gw       Added support for AGC.
02/07/02   gw       Removed use of l1_scdrv_call_in_task().
01/16/02   mk       Added support for afc/tt.
12/18/01   dp       Added support to abort handler
12/13/01   gw       Changes to support message/frame layer acquisition.
11/6/01    bk       Changed NPL1 function calls to reflect change in interface
10/31/01   dp       Removed reference to gl1_hw_rx_hdr_struct
10/24/01   dp       Initial version.
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
#include "gl1_hwi.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1i.h"
#include "msg.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "err.h"
#include "geran_dual_sim.h"
#include "mcfg_nv_api.h"

/*****************************************************

 LOCAL STORAGE

******************************************************/

typedef enum
{
   NCELL_FCCH_INACTIVE,
   NCELL_FCCH_WAITING_FOR_BLOCK_START,
   NCELL_FCCH_IN_PROGRESS,
   NCELL_FCCH_STOP_ACQ,
   NCELL_FCCH_PROCESSING,
   NCELL_FCCH_GET_RESULTS,
   NCELL_FCCH_ABORTING,
   NCELL_FCCH_ABORTED
} ncell_fcch_state_type;


/* Ncell FCCH parameters structure */
typedef struct
{
   /* State variables */
   ncell_fcch_state_type state;


   /* Callbacks */
   gl1_msg_rx_cb_type    callbacks;
   gl1_msg_decode_rpt   *data;

   /* Calculated offsets */
   uint16 abort_stop_acq_offset;
   uint16 start_acq_offset;
   uint16 timing_offset;

   /* Configured rx parameters */
   sys_algo_agc_T  *agc_ptr;
   ARFCN_T         ARFCN;
   uint16          first_tn_offset;
   uint16          last_tn_offset;

   gl1_hw_rx_tags_type  tags;

} ncell_fcch_params_type;


/* Ncell FCCH parameters */
static ncell_fcch_params_type ncell_fcch_params[NUM_GERAN_DATA_SPACES];

/* Extra space to leave around the acquisition for safety */
#define NCELL_FCCH_START_GUARD_PERIOD  8
#define NCELL_FCCH_STOP_GUARD_PERIOD   8

extern volatile ISRTIM_CMD_BLK   *l1_isrtsk_blk;
extern volatile ISRTIM_CMD_BLK l1_tsk_buffer[];

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/
static boolean ncell_fcch_rx_handler( boolean start_of_block,
                                      uint32  time_stamp,gas_id_t gas_id );


/*===========================================================================

FUNCTION gl1_msg_init_ncell_fcch

DESCRIPTION
  This function is used to initialize the ncell fcch engine.

===========================================================================*/
void gl1_msgi_init_ncell_fcch (gas_id_t gas_id)
{
   L1_STATE_CHANGE(ncell_fcch_params[gas_id].state                = NCELL_FCCH_INACTIVE, "NCELL_FCCH_INACTIVE");
   
   ncell_fcch_params[gas_id].data                 = NULL;

   ncell_fcch_params[gas_id].callbacks.data_cb    = NULL;
   ncell_fcch_params[gas_id].callbacks.metrics_cb = NULL;
   ncell_fcch_params[gas_id].callbacks.log_cb     = NULL;
   ncell_fcch_params[gas_id].callbacks.abort_cb   = NULL;
}


/*===========================================================================

FUNCTION gl1_msg_cfg_dl_ncell_fcch

DESCRIPTION
  This function is used to configure the callbacks for the neighbor
  cell FCCH. The callbacks will be invoked by the frame tick handler as
  required.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cfg_ncell_fcch ( gl1_msg_rx_cb_type *cb,gas_id_t gas_id)
{
   ncell_fcch_params[gas_id].callbacks.data_cb    = cb->data_cb;
   ncell_fcch_params[gas_id].callbacks.metrics_cb = cb->metrics_cb;
   ncell_fcch_params[gas_id].callbacks.log_cb     = cb->log_cb;
   ncell_fcch_params[gas_id].callbacks.abort_cb   = cb->abort_cb;
}

/*===========================================================================

FUNCTION gl1_msg_rx_ncell_fcch

DESCRIPTION
  This function is used to receive the FCCH on a neighbor cell.

DEPENDENCIES
  gl1_msg_cfg_ncell_fcch() must have been called at some time prior.

===========================================================================*/
void gl1_msg_rx_ncell_fcch
(
   const gl1_defs_tn_struct* first_tn_ptr,
   const gl1_defs_tn_struct* last_tn_ptr,
   ARFCN_T                   ARFCN,
   sys_algo_agc_T*           agc_ptr,
   gl1_msg_rx_rpt*           rpt,
   gas_id_t                  gas_id
)
{
   /* Make sure we are not already active */
   if (ncell_fcch_params[gas_id].state == NCELL_FCCH_WAITING_FOR_BLOCK_START)
   {
      MSG_GERAN_ERROR_2_G( GL1_ACQ_HDR"Double NCell FCCH rx issued (arfcns=%d,%d)",
                 ncell_fcch_params[gas_id].ARFCN.num, ARFCN.num);

      /* Remove the handler to avoid spurious errors, it will be re-installed
         below */
      gl1_msgi_remove_ft_handler(ncell_fcch_rx_handler,gas_id);
   }
   else if (ncell_fcch_params[gas_id].state != NCELL_FCCH_INACTIVE)
   {
      MSG_GERAN_ERROR_2_G( GL1_ACQ_HDR"NCell FCCH collision (state=%d arfcn=%d)",
                 ncell_fcch_params[gas_id].state, ARFCN.num);

      /* At this point L1 is probably stuck, but to continue with this
         sch could kill the lower layers even worse.  To try and recover
         reinstall the handler in case somehow it got uninstalled and hope
         for the best. */
      gl1_msgi_add_ft_handler(ncell_fcch_rx_handler, GL1_MSG_FT_HDLR_ACQ_NCELL,gas_id);
      return;
   }

   /* Store the data */
   ncell_fcch_params[gas_id].agc_ptr        = agc_ptr;
   ncell_fcch_params[gas_id].ARFCN          = ARFCN;
   ncell_fcch_params[gas_id].data           = rpt->data;

   /* Save the ARFCN in the AGC structure for debugging */
   agc_ptr->arfcn                   = ARFCN;

   /* Store the rx tn information */
   ncell_fcch_params[gas_id].first_tn_offset = (uint16) first_tn_ptr->tn * QS_PER_SLOT;

   /* Check the type of activity occuring in this timeslot.
    * If there transmit activity then take the timing advance
    * into account ONLY for that timeslot.
    */
   if( FALSE != first_tn_ptr->transmit )
   {
     /* Get the current timing advance value
      * Shift by 2 to convert from symbols to quarter symbols.
      */
     uint8  timing_advance = gl1_msg_retrieve_ta(gas_id);

     /* In some cases when TA differs in (IDLE - 1)
      * radio block from (IDLE + 1) then this subtraction
      * is invalid because we have not set the TA yet.
      */
     ncell_fcch_params[gas_id].first_tn_offset -= ( timing_advance << 2 );
   }

   ncell_fcch_params[gas_id].last_tn_offset = ((uint16)(last_tn_ptr->tn) + 1) * QS_PER_SLOT;


   /* Add the handler */
   gl1_msgi_add_ft_handler(ncell_fcch_rx_handler, GL1_MSG_FT_HDLR_ACQ_NCELL,gas_id);

   L1_STATE_CHANGE(ncell_fcch_params[gas_id].state = NCELL_FCCH_WAITING_FOR_BLOCK_START, "NCELL_FCCH_WAITING_FOR_BLOCK_START");
}





/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/



/*===========================================================================

FUNCTION ncell_fcch_rx_handler

DESCRIPTION
  This function should be called every frame on the 26/51 frame multiframe. The
  handler is installed as part of a request to rx a msg by gl1_msg_rx_ncell_fcch().
  The handler is told when the start of a block is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that there are no more messages queued up to send, it will signal that
  it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if message reception is complete
  FALSE if messages are still being received

SIDE EFFECTS
  None
===========================================================================*/
static boolean ncell_fcch_rx_handler
(
   boolean start_of_block,   /* next frame is start of block */
   uint32  time_stamp,       /* currently not used           */
   gas_id_t gas_id
)
{
   acq_decode_rpt fcch_rpt;
   int32      max_start_acq_offset;
   boolean    decoded;
   int32 rx_alpha_qs;
   volatile ISRTIM_CMD_BLK   *l1_tskisr_blk = &l1_tsk_buffer[gas_id];

#ifdef GERAN_L1_HLLL_LNASTATE
   gl1_defs_metrics_lnastate gl1_metrics_lnastate;
   uint8 SawlessHLLLState[MAX_NUMBER_OF_CHANNELS] = {0};
#endif /*GERAN_L1_HLLL_LNASTATE*/

   rx_alpha_qs = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

   /* Tick the state machine */
   switch (ncell_fcch_params[gas_id].state)
   {
      case NCELL_FCCH_INACTIVE:
         MSG_GERAN_ERROR_0_G( GL1_ACQ_HDR"Ncell FCCH inactive");
         return TRUE;

      case NCELL_FCCH_WAITING_FOR_BLOCK_START:
         /* Wait for the start of the block to continue */
         if (!start_of_block) return FALSE;

         /* Get the acq window stop offset */
         ncell_fcch_params[gas_id].timing_offset = (uint16)
                               (2 * QS_PER_FRAME
                                - rx_alpha_qs
                                - (mdsp_get_sync_rx_search_width(gas_id) * 4 / 2)
                                + ncell_fcch_params[gas_id].first_tn_offset
/* @@TO DO: If BB RX added to 9k may need to add shut down time */
                                - NCELL_FCCH_STOP_GUARD_PERIOD);

         /* Find the real start acq position */
         ncell_fcch_params[gas_id].start_acq_offset =
           ncell_fcch_params[gas_id].last_tn_offset
           + rx_alpha_qs
           + NCELL_FCCH_START_GUARD_PERIOD;

         /* Ensure acq window is at least 9 timeslots, by moving start offset earlier */
         /* But not so early as to cause Tx in TS 5 to be discarded */
         /* Do not make this adjustment if we are in DTM mode or if total slots > 5 */
         max_start_acq_offset = ncell_fcch_params[gas_id].timing_offset - (9 * QS_PER_SLOT);
         if (max_start_acq_offset < (rx_alpha_qs + (6 * QS_PER_SLOT) + NCELL_FCCH_START_GUARD_PERIOD))
         {
            max_start_acq_offset = (rx_alpha_qs + (6 * QS_PER_SLOT) + NCELL_FCCH_START_GUARD_PERIOD);
         }
		 if (( l1_tskisr_blk->l1_state == L1_DTM_MODE)|| (gl1_hw_schedule_get_num_slots(gas_id) > 5))
		 {
		    if (max_start_acq_offset < (rx_alpha_qs + (8 * QS_PER_SLOT) + NCELL_FCCH_START_GUARD_PERIOD))
			{
			 max_start_acq_offset = (rx_alpha_qs + (8 * QS_PER_SLOT) + NCELL_FCCH_START_GUARD_PERIOD);
			}
		 
		 }
         if ( ncell_fcch_params[gas_id].start_acq_offset > max_start_acq_offset)
         {
             MSG_GERAN_MED_2_G("Move acq start offset from %d to %d", ncell_fcch_params[gas_id].start_acq_offset, max_start_acq_offset);
             ncell_fcch_params[gas_id].start_acq_offset = max_start_acq_offset;
         }

         /* Start the tone detect, mdsp correctly handles offsets > 5000 */
         gl1_hw_start_acquisition_geran
          (
            ncell_fcch_params[gas_id].ARFCN,
            ncell_fcch_params[gas_id].agc_ptr->pwr_dBm_x16,
            ncell_fcch_params[gas_id].start_acq_offset,
            0,      /* No frequency offset in GSM mode */
            gl1_hw_get_sch_decode_mode( FALSE ),  /* No SCH decode */
            GL1_HW_ACQ_AFC_DISABLED,
            &ncell_fcch_params[gas_id].tags,
            FALSE,
            geran_map_gas_id_to_nas_id(gas_id)
          );

         /* Calculate the earliest possible stop time for aborts in the
            idle frame, below we will subtract a frame since the initial
            abort offset is from the start of the idle frame, and the
            offset here is from idle-1 */
         ncell_fcch_params[gas_id].abort_stop_acq_offset =
           ncell_fcch_params[gas_id].start_acq_offset + QS_PER_SLOT;

         MSG_4(MSG_SSID_DFLT, MSG_LEGACY_MED,
               GL1_ACQ_HDR"NCell FCCH (arfcn=%d): window=%d-%d, FN=%d",
               ncell_fcch_params[gas_id].ARFCN.num, ncell_fcch_params[gas_id].start_acq_offset,
               ncell_fcch_params[gas_id].timing_offset, GSTMR_GET_FN_GERAN(gas_id));  /*lint !e778*/

         ncell_fcch_params[gas_id].state++;
#ifdef FEATURE_GSM_PARALLEL_SCH_DEDICATED
         /* Request faster Mcpm speed if Parallel SCH is enabled */
         gl1_par_sch_mcpm_change( TRUE, gas_id );
#endif /* FEATURE_GSM_PARALLEL_SCH_DEDICATED */
         /* Fall through */

      case NCELL_FCCH_IN_PROGRESS:
         /* If we are greater than a frame then reduce it */
         if (ncell_fcch_params[gas_id].abort_stop_acq_offset > QS_PER_FRAME)
         {
            ncell_fcch_params[gas_id].abort_stop_acq_offset -= QS_PER_FRAME;
         }
         else
         {
            ncell_fcch_params[gas_id].abort_stop_acq_offset = 0;
         }

         /* If the offset is greater than a frame we wait to stop the acq */
         if (ncell_fcch_params[gas_id].timing_offset >= QS_PER_FRAME)
         {
           ncell_fcch_params[gas_id].timing_offset -= QS_PER_FRAME;
           break;
         }

         /* Tell mdsp to stop the acquisition */
         gl1_hw_stop_acquisition_geran(ncell_fcch_params[gas_id].timing_offset,gas_id);

         /* CR563626 - Dont allow measurements to resume in the same frame, when StopAcquistion is issued*/
         gl1_hw_update_acq_in_progress(TRUE, gas_id);       

         ncell_fcch_params[gas_id].state++;
         break;

      case NCELL_FCCH_STOP_ACQ:
#ifdef FEATURE_GSM_PARALLEL_SCH_DEDICATED
         /* Frame when stop acq is actioned so free Mcpm speed bump if Parallel SCH is enabled */
         gl1_par_sch_mcpm_change( FALSE, gas_id );
#endif /* FEATURE_GSM_PARALLEL_SCH_DEDICATED */
         gl1_hw_update_acq_in_progress(FALSE, gas_id);
         ncell_fcch_params[gas_id].state++;
         break;

      case NCELL_FCCH_PROCESSING:
         /* Processing results */
         ncell_fcch_params[gas_id].state++;
         break;

      case NCELL_FCCH_GET_RESULTS:
         /* Check results */
         decoded = gl1_hw_gen_was_tone_detected
                    (
                      ncell_fcch_params[gas_id].ARFCN,
                      &fcch_rpt,
                      gas_id
                    );

         /* If data is valid */
         if (decoded)
         {
            /* The tone offset is relative to the start of the acq window,
               convert it to be relative to the start of the idle-1 frame */
              fcch_rpt.time_offset += ncell_fcch_params[gas_id].start_acq_offset;

            /* A tone was found, the offset should be relative to the idle frame */
            ncell_fcch_params[gas_id].data->fcch.tone_detected = TRUE;
            ncell_fcch_params[gas_id].data->fcch.tone_offset   = ( fcch_rpt.time_offset - QS_PER_FRAME );

            /* Fill in the afc_freq after working it out from the fine freq offset */
            /* coarse_freq and fine_freq is ok for XO */
            ncell_fcch_params[gas_id].data->fcch.coarse_freq = fcch_rpt.coarse_freq;

            /* Fill in the afc_freq after working it out from the fine freq offset */
            ncell_fcch_params[gas_id].data->fcch.fine_freq = fcch_rpt.fine_freq;

            /* Calculate delta freq error in XO */
            ncell_fcch_params[gas_id].data->fcch.afc_freq =
              gl1_hw_freqtonormxofreq( ncell_fcch_params[gas_id].ARFCN, fcch_rpt.fine_freq ,gas_id);

            /* If desense ARFCN then don't include current XO setting */
            if ( gl1_msg_is_arfcn_xo_desense( ncell_fcch_params[gas_id].ARFCN,gas_id ) != DESENSE_CHAN)
            {
              /* Find total XO setting for ncell */
              ncell_fcch_params[gas_id].data->fcch.afc_freq
                += gl1_hw_get_xo_acc_freq_err(gas_id);
            }

            MSG_GERAN_MED_2_G( GL1_ACQ_HDR"Ncell FCCH afc_freq %dXO fine_freq %dHz",
                     ncell_fcch_params[gas_id].data->fcch.afc_freq,
                     fcch_rpt.fine_freq);

#ifdef GERAN_L1_HLLL_LNASTATE
            gl1_metrics_lnastate.NoOfChannels = 0;
            if(gl1_hw_get_sawless_HLLL_support(ncell_fcch_params[gas_id].ARFCN.band,gas_id))
              {
              gl1_msg_compute_rx_power(fcch_rpt.rssi,
                                       &fcch_rpt.pwr_dBm_x16,
                                       &ncell_fcch_params[gas_id].tags.gain.range,
                                       &ncell_fcch_params[gas_id].tags.gain);
              gl1_metrics_lnastate.NoOfChannels              = 1;
              gl1_metrics_lnastate.gsm_metrics[0].arfcn      = ncell_fcch_params[gas_id].ARFCN;
              gl1_metrics_lnastate.gsm_metrics[0].rx_lvl_dbm = fcch_rpt.pwr_dBm_x16;
              gl1_metrics_lnastate.gsm_metrics[0].snr        = fcch_rpt.snr;
              gl1_metrics_lnastate.gsm_metrics[0].jdet_value = fcch_rpt.jdetReading;
              SawlessHLLLState[0]                            = ncell_fcch_params[gas_id].tags.sawless_lnastate;
              gl1_hw_rf_burst_metrics(&gl1_metrics_lnastate,gas_id);
              }
            if(gl1_metrics_lnastate.NoOfChannels)
              {
              gl1_hw_log_HLin_LLin_metrics(&gl1_metrics_lnastate,SawlessHLLLState,gas_id);
              }
#endif /*GERAN_L1_HLLL_LNASTATE*/

         }
         /* If the tone was not detected */
         else
         {
            ncell_fcch_params[gas_id].data->fcch.tone_detected = FALSE;
         }

         if (ncell_fcch_params[gas_id].callbacks.data_cb)
         {
            ncell_fcch_params[gas_id].callbacks.data_cb( ncell_fcch_params[gas_id].data,gas_id);
         }
         else
         {
            MSG_GERAN_HIGH_0_G( GL1_ACQ_HDR"No ncell fcch dl callback registered");
         }

         L1_STATE_CHANGE(ncell_fcch_params[gas_id].state = NCELL_FCCH_INACTIVE, "NCELL_FCCH_INACTIVE");
         
#ifdef FEATURE_GSM_PARALLEL_SCH_DEDICATED
         gl1_par_sch_mcpm_change( FALSE, gas_id );
#endif /* FEATURE_GSM_PARALLEL_SCH_DEDICATED */

         break;

      /* In this case only when abort is called */
      case NCELL_FCCH_ABORTING:
         /* If acq has been started, stop it */
             gl1_hw_stop_acquisition_geran(ncell_fcch_params[gas_id].abort_stop_acq_offset,gas_id);
             ncell_fcch_params[gas_id].abort_stop_acq_offset = 0;
         ncell_fcch_params[gas_id].state++;
         break;

      /* In this case only when abort is called */
      case NCELL_FCCH_ABORTED:

         L1_STATE_CHANGE(ncell_fcch_params[gas_id].state = NCELL_FCCH_INACTIVE, "NCELL_FCCH_INACTIVE");
		 
         gl1_hw_update_acq_in_progress(FALSE, gas_id);     

         if (ncell_fcch_params[gas_id].callbacks.abort_cb)
         {
             ncell_fcch_params[gas_id].callbacks.abort_cb(gas_id);
         }
         else
         {
             MSG_GERAN_HIGH_0_G( GL1_ACQ_HDR"No Ncell abort CB registered");
         }
         break;

      default:
         MSG_GERAN_HIGH_1_G( GL1_ACQ_HDR"Error in rx state machine %d",
                   ncell_fcch_params[gas_id].state);

         L1_STATE_CHANGE(ncell_fcch_params[gas_id].state = NCELL_FCCH_INACTIVE, "NCELL_FCCH_INACTIVE");
         break;
   }

   /* If we are inactive, remove the handler */
   return (ncell_fcch_params[gas_id].state == NCELL_FCCH_INACTIVE ? TRUE : FALSE);

/*lint -esym(715,time_stamp)*/ /* time_stamp parameter not used */
}


/*===========================================================================

FUNCTION  gl1_msg_abort_rx_ncell_fcch

DESCRIPTION
  This function aborts the currently ongoing ncell fcch rx and invokes the
  registered abort callback when done

===========================================================================*/
void gl1_msg_abort_rx_ncell_fcch(gl1_msg_ncell_abort_type type,gas_id_t gas_id)
{
   /* Informational message */
   MSG_GERAN_MED_3_G( GL1_ACQ_HDR"NCell FCCH aborted: state=%d ARFCN=%d FN=%d",
            ncell_fcch_params[gas_id].state, ncell_fcch_params[gas_id].ARFCN.num, GSTMR_GET_FN_GERAN(gas_id) );

   switch (type)
   {
      case IMMEDIATE_ABORT:
         L1_STATE_CHANGE(ncell_fcch_params[gas_id].state = NCELL_FCCH_ABORTED, "NCELL_FCCH_ABORTED");
         break;

      case DELAYED_ABORT:
         if (ncell_fcch_params[gas_id].state == NCELL_FCCH_IN_PROGRESS)
         {
           L1_STATE_CHANGE(ncell_fcch_params[gas_id].state = NCELL_FCCH_ABORTING, "NCELL_FCCH_ABORTING");           
         }
         else
         {
           L1_STATE_CHANGE(ncell_fcch_params[gas_id].state = NCELL_FCCH_ABORTED, "NCELL_FCCH_ABORTED");
         }
         break;
         
      default:
         if(geran_get_nv_recovery_restart_enabled(gas_id))
         {
           MSG_GERAN_ERROR_1_G( GL1_ACQ_HDR"Aborting fcch with invalid type %d",
                     type);
           gl1_hw_handle_panic(gas_id);
         }
         else
         {
           MSG_GERAN_FATAL_1_G( GL1_ACQ_HDR"Aborting fcch with invalid type %d",
                                type );
         }
         break;
   }
}



/*===========================================================================

FUNCTION  gl1_msg_ncell_sch_pos_in_acq_window

DESCRIPTION
  This function determines if the passed in SCH offset lies within the
  Packet Transfer and DTM surround cell acquisition window

DEPENDENCIES
  None

PARAMETERS
  SCH offset in quarter symbols

RETURN VALUE
   -1 if offset lies before start of acq window
    0 if offset lies within acq window
   +1 if offset lies after the end of the acq window

SIDE EFFECTS
  None

===========================================================================*/
int16 gl1_msg_ncell_sch_pos_in_acq_window( int16 sch_offset,
                                         const gl1_defs_tn_struct* first_tn_ptr,
                                         const gl1_defs_tn_struct* last_tn_ptr,
                                         gas_id_t                  gas_id )
{
   int32 min_valid_offset, max_valid_offset;
   int32 rx_alpha_qs;
   rx_alpha_qs = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

   /* Make the sch offset relative to idle-1 */
   sch_offset += QS_PER_FRAME;
    /* Calculate the valid offset range */
   /* @@OG: Reduce the timeslot duration to half */

   if( FALSE == mcfg_gcf_nv_get_status())
   {
     min_valid_offset = ((int32)last_tn_ptr->tn) * QS_PER_SLOT
                      + rx_alpha_qs
                      + (QS_PER_SLOT/2)
                      + (GL1_HW_SCH_SEARCH_WIDTH_DEFAULT * 4 / 2);
   }
   else
   {
     min_valid_offset = ((int32)last_tn_ptr->tn) * QS_PER_SLOT
                        + rx_alpha_qs
                        + (QS_PER_SLOT)
                        + (GL1_HW_SCH_SEARCH_WIDTH_DEFAULT * 4 / 2);
   }

   if( FALSE != last_tn_ptr->transmit )
   {
     uint8  timing_advance = gl1_msg_retrieve_ta(gas_id);

     min_valid_offset -= (timing_advance << 2);
   }

   max_valid_offset = 2 * QS_PER_FRAME + (int32)first_tn_ptr->tn * QS_PER_SLOT
                        - (mdsp_get_sync_rx_search_width(gas_id) * 4 / 2)
                        - rx_alpha_qs
                        - (QS_PER_SLOT)
                        - (GL1_HW_SCH_SEARCH_WIDTH_DEFAULT * 4 / 2);

   if( FALSE != first_tn_ptr->transmit )
   {
     uint8  timing_advance = gl1_msg_retrieve_ta(gas_id);

     /* In some cases when TA differs in (IDLE - 1)
      * radio block from (IDLE + 1) then this subtraction
      * is invalid because we have not set the TA yet.
      */
     max_valid_offset -= (timing_advance << 2);
   }

   /* Ensure at least 1 frame of valid offsets by forcing min offset earlier */
   if ((max_valid_offset - min_valid_offset) < QS_PER_FRAME)
   {
      min_valid_offset = max_valid_offset - QS_PER_FRAME;
   }

   /* Validate the sch offset */
   if ( sch_offset >= max_valid_offset )
   {
      return ( 1 );
   }
   else if ( sch_offset <= min_valid_offset )
   {
      return ( -1 );
   }
   else
   {
      return ( 0 );
   }
}

/*===========================================================================

FUNCTION  gl1_msg_ncell_sch_pos_in_acq_window_dedic

DESCRIPTION
  This function determines if the passed in SCH offset lies within the
  TCH surround cell acquisition window

DEPENDENCIES
  None

PARAMETERS
  SCH offset in quarter symbols

RETURN VALUE
   -1 if offset lies before start of acq window
    0 if offset lies within acq window
   +1 if offset lies after the end of the acq window

SIDE EFFECTS
  None

===========================================================================*/
int16 gl1_msg_ncell_sch_pos_in_acq_window_dedic( int16 sch_offset,
                                                 const gl1_defs_tn_struct* first_tn_ptr,
                                                 const gl1_defs_tn_struct* last_tn_ptr,
                                                 gas_id_t                  gas_id)
{
   int32 min_valid_offset, max_valid_offset;
   int32 rx_alpha_qs;

   rx_alpha_qs = gl1_hw_get_rf_gsm_rx_setup_time(gas_id);

   /* Make the sch offset relative to idle-1 */
   sch_offset += QS_PER_FRAME;

   /* Calculate the valid offset range */
   min_valid_offset = ((int32)last_tn_ptr->tn) * QS_PER_SLOT
                      + rx_alpha_qs
                      + (QS_PER_SLOT)
                      + (GL1_HW_SCH_SEARCH_WIDTH_DEFAULT * 4 / 2);

   if( FALSE != last_tn_ptr->transmit )
   {
     uint8  timing_advance = gl1_msg_retrieve_ta(gas_id);

     min_valid_offset -= (timing_advance << 2);
   }


   max_valid_offset = 2 * QS_PER_FRAME + (int32)first_tn_ptr->tn * QS_PER_SLOT
                      - (mdsp_get_sync_rx_search_width(gas_id) * 4 / 2)
                      - rx_alpha_qs
                      - (QS_PER_SLOT)
                      - (GL1_HW_SCH_SEARCH_WIDTH_DEFAULT * 4 / 2);


   if( FALSE != first_tn_ptr->transmit )
   {
     uint8  timing_advance = gl1_msg_retrieve_ta(gas_id);

     /* In some cases when TA differs in (IDLE - 1)
      * radio block from (IDLE + 1) then this subtraction
      * is invalid because we have not set the TA yet.
      */
     max_valid_offset -= (timing_advance << 2);
   }

   /* Validate the sch offset */
   if ( sch_offset >= max_valid_offset )
   {
      return ( 1 );
   }
   else if ( sch_offset <= min_valid_offset )
   {
      return ( -1 );
   }
   else
   {
      return ( 0 );
   }
}

/*===========================================================================

FUNCTION  gl1_get_ncell_start_acq_offset

DESCRIPTION
 This function returns the start offset value of NCELL FCCH detection

DEPENDENCIES
  None

PARAMETERS
  NA

RETURN VALUE
   Start offset value

SIDE EFFECTS
  None

===========================================================================*/
uint16 gl1_get_ncell_start_acq_offset(gas_id_t gas_id)
{
  return (ncell_fcch_params[gas_id].start_acq_offset);
}


