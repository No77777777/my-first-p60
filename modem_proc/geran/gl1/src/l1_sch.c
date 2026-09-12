/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

            L 1   SC H  R E C E I V E   S C H E D U L I N G

GENERAL DESCRIPTION
   This module contains the procedures to schedule a SCH decode
   This procedure executes in the L1 ISR context.

EXTERNALIZED FUNCTIONS
  L1_get_sch
    Procedure to schedule an asynchronous receive of the SCH message
    on the serving cell prior to decoding the BCCH (could be commanded
    by RR in some reselection cases).  This is different from
    acquisition where we do a combined FCCH+SCH


INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sch.c#1 $
  $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
10/08/15   jk        CR887760:Initialize DRx AGC pointer correctly for SCH
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
25/01/15   am        CR798569 Ensure timing errors from SCH re-confirmation is updated properly
19/12/13   jk        CR503359 - GL1 changes to support Rx diversity
17/12/13   mko       CR586179: Applying the timing error reported in SCH reconfirm during reselection
01/10/13   sk        CR552259 CR sync from 1.0/1.1
20/08/13   nn        CR518344 Use secapi_get_random() for generating random numbers.
10/06/13   cs        TSTS Updates
26/05/09   cs        CR167848 fix for expanded afc_freq I/F to mDSP
12/06/07   og        Upgrade the FEATURE_GSM_L1_HFREQ_ERR_NCELL_SCH_DECODE to
                     include dedicated TCH NCELL SCH attempts.
05/03/07   ws        Fixed filtered lint warnings
08/17/05   gfr       Use gl1_msg_init_frame_tick_info to init the ftn structure
08/16/05   sv        Clean up the abort scenario during SCH receive.
11/01/04   yhong     Changed NCell Abort API to take a parameter
07/15/04   gfr       Log ncell sch burst and msg metrics.
06/11/04   dp        Use AGC setting from ncell struct for SCH decode during reselection
05/26/04   sv        Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters for better
                     code readability.
02/25/04   gw        Lint clean up.
11/05/03   gfr       Support for quad-band.
09/24/02   dp        Workaround for mdsp SBI errors when Ncell SCH offset lies
                     near FTSM.  Problem is due to large ZIF tune time.  Will
                     be pulled out when ZIF tune time is fixed
06/24/02   dp        Initial version.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "customer.h"

#include "comdef.h"
#include "l1i.h"
#include "l1_isr.h"
#include "l1_task.h"
#include "gl1_msg_g.h"
#include "gl1_sys_algo.h"
#include "gl1_sys_params.h"
#include "msg.h"
#include "l1_log.h"
#include "l1_utils.h"    // for l1_state_change

#include "geran_dual_sim.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/* Prototype for local functions */
static void sch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id );
static void sch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id );
boolean get_sch_decoded( gas_id_t gas_id );
void set_sch_decoded(boolean sch_decoded,gas_id_t gas_id);

typedef struct
{
 gl1_msg_metrics_rpt      sch_metrics_rpt[4];
 boolean sch_decoded;
 /* Two sets of report arrays are declared out of paranoia      */
 /* regarding reception of back-to-back blocks (extended SCH). */
 gl1_msg_decode_rpt    data_rpt;
}l1_sch_data_t;

static l1_sch_data_t l1_sch_data[NUM_GERAN_DATA_SPACES];
// static l1_sch_data_t *l1_sch_data_ptr=l1_sch_data;

boolean get_sch_decoded( gas_id_t gas_id )
{
  l1_sch_data_t *l1_sch_data_ptr=&l1_sch_data[gas_id];
  return l1_sch_data_ptr->sch_decoded;
}

void set_sch_decoded(boolean sch_decoded,gas_id_t gas_id)
{
  l1_sch_data_t *l1_sch_data_ptr=&l1_sch_data[gas_id];
  l1_sch_data_ptr->sch_decoded = sch_decoded;
}

/*===========================================================================

FUNCTION  L1_GET_SCH

DESCRIPTION
  This function reads the SCH data off a carrier where the SCH has just
  been decoded.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void L1_get_sch(gas_id_t gas_id )
{
  idle_data_T               *l1_idle_data=&l1_idle_data_store[gas_id];
  l1_sch_data_t *l1_sch_data_ptr=&l1_sch_data[gas_id];
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  /* Locals */
  sys_algo_agc_T               *agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY
  sys_algo_agc_T 	           *agc_ptr_divrx;
#endif
  gl1_msg_rx_rpt               rpt;

  gl1_msg_rx_cb_type           rx_cb;
  gl1_msg_frame_tick_type      ftn;
  /* This indicates the start of a block _we_want_to_receive_ */
  boolean                      ncell_block_start;
  uint16                       timing_offset = 0;

  /* Use AGC setting from ncell struct for SCH decode during reselection */
  agc_ptr = &(l1_idle_data->campedon_cell_agc);

#ifdef FEATURE_GSM_RX_DIVERSITY  
	agc_ptr_divrx = &(l1_idle_data->campedon_cell_agc_divrx);
#endif

  /* Code */
  switch ( l1_tskisr_blk->sub_sub_state )
  {
  case L1_SCH_INIT:
  case L1_SCH_START:
    {
      /* Configure the message layer */
      gl1_msg_reset(gas_id);

      rx_cb.data_cb    = sch_rx_callback;
      rx_cb.metrics_cb = sch_metrics_callback;
      rx_cb.log_cb     = NULL;
      rx_cb.abort_cb   = NULL;

      gl1_msg_cfg_ncell_sch ( &rx_cb,gas_id );

      l1_sch_data_ptr->sch_decoded = FALSE;

      MSG_GERAN_MED_0_G("FindSCH init ...");

      l1_tskisr_blk->sub_sub_state = L1_SCH_RUNNING;
    }
    /* fall through */

  case L1_SCH_RUNNING:
    ncell_block_start = FALSE;

    /* 50 is the idle frame.  No SCH after it */
#ifdef ZIF_TUNE_TIME_FIXED
    #error code not present
#else
     /*
       ZIF_WORKAROUND.
       Wake up 2 frames before actual SCH burst. Workaround
       for mdsp SBI errors when Ncell SCH Offset is close
       to the FTSM.  Will be pulled out after ZIF tune time is
       reduced
     */
    if ( ( frame_counters[gas_id].FNmod51 % 10 == 9 ||
            frame_counters[gas_id].FNmod51 == 50 ) && frame_counters[gas_id].FNmod51 != 49 )
#endif
    {
        rpt.metrics[0] = &l1_sch_data_ptr->sch_metrics_rpt[0];
        rpt.metrics[1] = &l1_sch_data_ptr->sch_metrics_rpt[1];
        rpt.metrics[2] = &l1_sch_data_ptr->sch_metrics_rpt[2];
        rpt.metrics[3] = &l1_sch_data_ptr->sch_metrics_rpt[3];

        rpt.data = &l1_sch_data_ptr->data_rpt;

        MSG_GERAN_HIGH_1_G("SCH Pwr dBm x16 = %d",agc_ptr->pwr_dBm_x16);

#ifdef ZIF_TUNE_TIME_FIXED
        #error code not present
#else
        /*
          ZIF_WORKAROUND.
          Wake up 2 frames before actual SCH burst. Workaround
          for mdsp SBI errors when Ncell SCH Offset is close
          to the FTSM.  Will be pulled out after ZIF tune time is
          reduced.  Add 5000 since we're waking up 2 frames earlier
        */
        gl1_msg_rx_ncell_sch (
                                l1_tskisr_blk->current_params.ARFCN,
                                agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY 
                                agc_ptr_divrx,	   
#endif /*FEATURE_GSM_RX_DIVERSITY*/
                                timing_offset + 5000,
                                (int32)0,
                                &rpt,
                                gas_id
                             );

#endif
        ncell_block_start = TRUE;
    }


    gl1_msg_init_frame_tick_info(&ftn);
    ftn.next_frame_is_acq_ncell_block_start = ncell_block_start;
    gl1_msg_frame_tick_notification ( &ftn,gas_id );
    break;

  case L1_SCH_END:
    /* Now go find the BCCH */
    l1_change_state ( L1_FIND_BCCH, gas_id );
    l1_tskisr_blk->command = L1_NULL_COMMAND;
    l1_tskisr_blk->sub_state = L1_GETTING_BCCH;
    l1_tskisr_blk->sub_sub_state = L1_BCCH_START;
    L1_transfer_new_data( gas_id );

    /* Send reselection confirmation */
    L1_send_MPH_RESELECTION_CNF( l1_sch_data_ptr->sch_decoded, gas_id );

    break;

  default:
    MSG_GERAN_ERROR_0_G("FindSch: Bad sub_sub_state");
    break;

  } /* end of switch (subsubstate) */

}


/*===========================================================================

FUNCTION  SCH_RX_CALLBACK

DESCRIPTION
  Callback function called when a SCH block has been received.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void sch_rx_callback( gl1_msg_decode_rpt *rpt, gas_id_t gas_id )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  l1_sch_data_t *l1_sch_data_ptr=&l1_sch_data[gas_id];
  /* Log */
  l1_log_ncell_sch_msg_metrics(&rpt->sch, gas_id);

  /* send reports up the stack */
  if ( rpt->sch.hdr.good_data ) /* rpt->good_data is 2 if true */
  {
    l1_sch_data_ptr->sch_decoded = TRUE;
    MSG_GERAN_HIGH_0_G("SCH decoded");
  }
  else
  {
    l1_sch_data_ptr->sch_decoded = FALSE;

    MSG_GERAN_ERROR_0_G("SCH decode failed");
  }

  /* LOG MESSAGE METRICS */
  //l1_log_message_metrics ( GL1_MSG_SCH, rpt );

  l1_tskisr_blk->sub_sub_state = L1_SCH_END;

}


/*===========================================================================

FUNCTION  SCH_METRICS_CALLBACK

DESCRIPTION
  Callback function called when SCH burst metrics are available.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void sch_metrics_callback( gl1_msg_metrics_rpt *rpt[], gas_id_t gas_id )
{
	l1_sch_data_t *l1_sch_data_ptr = &l1_sch_data[gas_id];
    l1_sch_data_ptr->sch_metrics_rpt[0] = *rpt[0];

   /* Log - only a single burst */
   l1_log_ncell_sch_burst_metrics(rpt[0], gas_id);
}

/*===========================================================================

FUNCTION  l1_sch_adjust_timing_error

DESCRIPTION
  This is called to adjust timing error in SCH reconfirm during the reselection.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void l1_sch_adjust_timing_error( gas_id_t gas_id )
{
  int16 timing_offset;
  l1_sch_data_t *l1_sch_data_ptr=&l1_sch_data[gas_id];

  timing_offset = l1_sch_data_ptr->sch_metrics_rpt[0].timing_offset;

  if( timing_offset != 0 )
  {
    uint32 fn_lag = 0;

    if ( timing_offset < 0 )
    {
      timing_offset += 5000;
      fn_lag = NEG_FN(1);
      (void)L1_change_FN(fn_lag, gas_id);
    }

    gl1_msg_correct_timing(timing_offset,gas_id);

    l1_sc_set_sync(fn_lag, /*FN*/
                   timing_offset, /*qbit_offset*/
                   gas_id);

    MSG_GERAN_HIGH_1_G("Timing error adjusted in SCH %d",timing_offset);
  }
}


