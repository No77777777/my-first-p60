/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  R A C H   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for transmitting on the RACH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

   gl1_msg_cfg_ul_rach
   gl1_msg_tx_rach
   rach_tx_handler





INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_rach.c#2 $
$DateTime: 2020/01/27 07:43:27 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
27/01/20   rks      CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
19/08/15   ab       CR892150 : GL1 RACH handling broken during CS handover due to RACH Delay COEX changes.
31/07/15   hd       CR882159 Do not abort the RACH state if there was RACH tx in the previous frame.
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
13/03/15   mc       CR773788 : COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
30/03/15   nm       CR765891 Allowing TA for TDS during PS RACH
22/05/14   nk       CR667961 Make ab_tsc crash code FTM_MODE proof
09/07/14   pjr      CR685199 Added extra check to ensure WTR hop is not triggered after ACCESS MODE is aborted
01/05/14   nk       CR656822 Debug prints/crash to catch when ab_tsc is greater than 2
01/10/13   sk       CR552259 CR sync from 1.0/1.1
03/10/13   ws       CR499846 - Bug fix for RACH PWR reduction feature
15/08/13   sk       CR524039 GBTA bringup changes
20/06/13   cs       Major Triton TSTS Syncup
13/03/13   mc       CR.459487 : COEX Support - GERAN arbitrator framework
05/08/09   ws       Removed FEATURE_GSM_MDSP_DTM it's now mainlined
20/05/08   cs       Fix for Klocwork uninitialised variable error
31/08/07   agv      CR127469 - Temp fix for Timing Alignment for Handover.
31/07/06   av       CR85810 NB AB TSC, and TA fix.
09/27/05   gfr      Add gl1_msg_rach_type to distinguish between normal,
                    handover and extended RACHs.
08/17/05   gfr      Cleanup, added gl1_msg_inc_rach_frame_cnt to replace global
                    variable, added explicity state names
07/29/05   gfr      DTM support
04/21/05   yh       Support for panic reset
04/12/05   gfr      Consolidate GSM hw layer parameters
07/22/04   gfr      EGPRS support.
06/02/04   gfr      Lint cleanup.
03/05/04   gfr      Lint cleanup.
11/05/03   gfr      Support for quad-band.
11/15/02   ws       changed rach_delay to in16 as can be -ve
10/26/02   dp       Removed extern declaration for rach_cnt to fix Leopard
                    compiler errors with ADS1.2  Has now been externed in 
                    gl1_msg_rach.c
10/01/02   ATM      Replaced references to L1's FN with GSTMR FN
08/29/02   JC       Changed gl1_msg_cfg_ul_rach and gl1_msg_tx_rach() APIs
                    to accomodate multi-band operation.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
03/20/02   dp       Changed rach_signal_strength to rach_power_index
03/06/02   dp       Support to frequency hop access bursts
02/20/02   dp       Added support for back to back RACHs as in the case of
                    handover access bursts
02/12/02   thh      Fixed the problem where RACH were occurring on non-RACH
                    frames.
                    Updated copyright date.
11/20/01   mk       Fixed gl1_msg_abort_rach().
11/6/01    bk       Changed NPL1 function calls to reflect change in interface
09/27/01     jc     Initial version.
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
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "geran_msgs.h"
#include "gtmrs.h"
#include "gtmrs_g.h"      /* Frame no: For debug msgs only */
#include "l1_utils.h"
#include "geran_dual_sim.h"

/*****************************************************

 LOCAL STORAGE

******************************************************/

typedef struct {
  /* Number of messages to send */
  uint8 tx_rach;
  rach_state_type rach_state;

  /* Configured Callback pointers  */

  gl1_msg_rach_cb_type   rach_callback;
  uint16                 rach_random_content;
  int16                  rach_delay;
  gl1_msg_rach_type      rach_type;

  gl1_hw_tx_signal_type  rach_signal_info;
  gl1_hw_tx_burst_type   rach_burst_info;
  gl1_hw_channel_type    rach_channel_type;

  gl1_hw_tx_tags_type    rach_tags;

  
  /* Counts of rach frames, used for calculating delay */
  uint16                 rach_cnt;
  uint16                 last_rach_cnt;

#ifdef GSM_HO_TIMING_ADJ
  boolean                handover_time_align;
#endif
#ifdef FEATURE_GSM_COEX_RACH_DELAY
  boolean                yield;
#endif

  uint32                 last_rach_tx_fn;

}rach_params_type;

static rach_params_type rach_params[NUM_GERAN_DATA_SPACES];

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/

static boolean rach_tx_handler( boolean start_of_block,
                                uint32  time_stamp,gas_id_t gas_id );


/*===========================================================================
FUNCTION gl1_msgi_init_rach

DESCRIPTION
   This function initializes data structures that are used by all RACH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_init_rach(gas_id_t gas_id)
{
   rach_params[gas_id].tx_rach                 = 0;
   L1_STATE_CHANGE(rach_params[gas_id].rach_state              = RACH_STATE_NULL, "RACH_STATE_NULL");
   rach_params[gas_id].rach_callback.data_cb   = NULL;
   rach_params[gas_id].rach_callback.abort_cb  = NULL;
   rach_params[gas_id].last_rach_cnt           = 0;
   rach_params[gas_id].rach_cnt                = 0;
   rach_params[gas_id].last_rach_tx_fn           = L1_GPRS_INVALID_FN;
}

/*===========================================================================

FUNCTION gl1_msg_cfg_ul_rach

DESCRIPTION
  This function is used to configure the parameters that are used with the
  RACH burst.  The callback function will be invoked by the handler when
  the RACH burst has been transmitted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_cfg_ul_rach
(
  uint8                ab_tsc, /* Access Burst TSC */
  uint8                BSIC,
  gl1_msg_rach_type    type,
  gl1_msg_rach_cb_type *cb,
  gas_id_t             gas_id
)
{
  if ( ftm_get_mode() != FTM_MODE )
  {
    // ERR_FATAL if ab_tsc value passed is greater than 2
    if(ab_tsc > 2)
    {
#ifdef FEATURE_QSH_MDUMP
      QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
      ERR_GERAN_FATAL_1_G("AB_TSC: %d", ab_tsc);
    }
  }
#ifdef GSM_HO_TIMING_ADJ
  rach_params[gas_id].handover_time_align = FALSE;
#endif

   rach_params[gas_id].rach_callback.data_cb = cb->data_cb;

   memset(&rach_params[gas_id].rach_signal_info, 0, sizeof(rach_params[gas_id].rach_signal_info));
   memset(&rach_params[gas_id].rach_burst_info, 0, sizeof(rach_params[gas_id].rach_burst_info));
   rach_params[gas_id].rach_burst_info.burst_num = 0;
   rach_params[gas_id].rach_burst_info.ab_tsc  = ab_tsc; /* Access Burst TSC */
   rach_params[gas_id].rach_burst_info.bsic = BSIC;
   rach_params[gas_id].rach_type            = type;

   if (type == GL1_MSG_RACH_EXTENDED)
   {
      rach_params[gas_id].rach_channel_type = GL1_HW_PRACH_EXT;
   }
   else
   {
      rach_params[gas_id].rach_channel_type = GL1_HW_RACH;
   }

#ifdef GSM_HO_TIMING_ADJ
   /* Temp mode of Handover timing adjust. */
   if (type == GL1_MSG_RACH_HANDOVER)
   {
     rach_params[gas_id].handover_time_align = TRUE;
   }
#endif

   /* New batch of RACHes going out */
   MSG_5(MSG_SSID_DFLT, MSG_LEGACY_MED,
         "New RACH set (cnt=%d, ab_tsc=%d, bsic=%d, type=%d, rach_params.ab_tsc: %d)",
         rach_params[gas_id].rach_cnt, ab_tsc, BSIC, type, rach_params[gas_id].rach_burst_info.ab_tsc);  /*lint !e778*/ /*lint bug?*/

   rach_params[gas_id].last_rach_cnt = rach_params[gas_id].rach_cnt;
}



/*===========================================================================

FUNCTION gl1_msg_tx_rach

DESCRIPTION
  This function is used to transmit a message on the SDCCH on the next sdcch
  block boundary.  The message contents should span 104 frames ie 1 block.
  This function may be called at task level but should be called in the block
  before the data is to transmitted.

  gl1_msg_cfg_ul_rach() must have been called at some time prior.

===========================================================================*/
void gl1_msg_tx_rach
(
   ARFCN_T  arfcn,
   int16    txlev,
   uint16   delay,          /* rach frames to delay */
   uint16   random_content,
   gas_id_t gas_id
)
{
   int16 tx_pwr_idx = 0;

   /* Translate GSM TXLEV to HW power index */
   gl1_msgi_map_txlev_to_pwr_idx(arfcn, &txlev, &tx_pwr_idx, 1,gas_id);

   rach_params[gas_id].rach_signal_info.tx_power_index = tx_pwr_idx;
   rach_params[gas_id].rach_burst_info.arfcn           = arfcn;
   rach_params[gas_id].rach_burst_info.encode_flag     = TRUE;
   rach_params[gas_id].rach_burst_info.channel_type    = rach_params[gas_id].rach_channel_type;

   /* Get the timeslot - use CS slot for handover bursts, otherwise 0 */
   if (rach_params[gas_id].rach_type == GL1_MSG_RACH_HANDOVER)
   {
      rach_params[gas_id].rach_signal_info.tn = gl1_msg_get_cs_timeslot(gas_id);
   }

   MSG_GERAN_MED_4_G("RACH: delay=%d, tn=%d, content=%d, ab_tsc: %d", delay, rach_params[gas_id].rach_signal_info.tn, random_content, rach_params[gas_id].rach_burst_info.ab_tsc);

   /* We don't really support back-to-back rach calls since there is only
      a single copy of each variable - can remove the tx_rach variable
      etc...once it is confirmed that it is not needed. */
   if (rach_params[gas_id].rach_state != RACH_STATE_NULL)
   {
      MSG_GERAN_ERROR_2_G("Rach already active, state=%d, delay=%d",
                rach_params[gas_id].rach_state, rach_params[gas_id].rach_delay);
   }

   /* Calculate how many frames have already passed since the last rach
   ** Remember to take into account the RACH frames which have already passed
   ** since the last RACH.
   */
#ifdef FEATURE_GSM_COEX_RACH_DELAY
    if(rach_params[gas_id].yield)
    {
       MSG_GERAN_HIGH_0_G("RACH was YIELD'ed (blanked) due to COEX conflict, retry");
       rach_params[gas_id].rach_delay = 0;
    }
    else
#endif
   if (rach_params[gas_id].rach_cnt < rach_params[gas_id].last_rach_cnt)
   {
      /* Deal with RACH count rollover */
      rach_params[gas_id].rach_delay = (int16)(delay - (0xFFFF - rach_params[gas_id].last_rach_cnt + rach_params[gas_id].rach_cnt));
   }
   else
   {
      /* Total delay = requested delay - RACH frames that have passed */
      rach_params[gas_id].rach_delay = (int16)(delay - (rach_params[gas_id].rach_cnt - rach_params[gas_id].last_rach_cnt));
   }

   rach_params[gas_id].rach_random_content = random_content;

   /* Register a handler if one does not exist */
   if (rach_params[gas_id].tx_rach == 0)
   {
      gl1_msgi_add_ft_handler(rach_tx_handler, GL1_MSG_FT_HDLR_UL,gas_id);
      L1_STATE_CHANGE(rach_params[gas_id].rach_state = RACH_STATE_WAITING_FOR_BLOCK_START, "RACH_STATE_WAITING_FOR_BLOCK_START");
   }

   rach_params[gas_id].tx_rach++;
}


/*===========================================================================

FUNCTION gl1_msg_abort_rach

DESCRIPTION
  This function is used to terminate any RACH bursts pending.

===========================================================================*/
void gl1_msg_abort_rach ( gas_id_t gas_id )
{
   if (rach_params[gas_id].tx_rach > 0)
   {
      rach_params[gas_id].tx_rach = 0;
      L1_STATE_CHANGE(rach_params[gas_id].rach_state = RACH_STATE_NULL, "RACH_STATE_NULL");
      gl1_msgi_remove_ft_handler ( rach_tx_handler,gas_id );
   }
#ifdef FEATURE_GSM_COEX_RACH_DELAY
   /*Reset RACH Yield Flag */
   rach_params[gas_id].yield = FALSE;
#endif

}



/*===========================================================================

FUNCTION gl1_msg_inc_rach_frame_cnt

DESCRIPTION
  This function is used to increment the count of rach frames.

===========================================================================*/
void gl1_msg_inc_rach_frame_cnt ( gas_id_t gas_id )
{
   rach_params[gas_id].rach_cnt++;
}



/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/



/*===========================================================================

FUNCTION rach_tx_handler

DESCRIPTION
  This function should be called on every frame of the 51 multiframe.
  The handler is installed as part of a request to tx a msg by
  gl1_msg_tx_rach().
  The handler is told when the start of a RACH frame is to occur and will call
  the frame layer functions at the proper time.   When the handler senses
  that the RACH has been transmitted,  it will signal that
  it should be deregistered.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if RACH was sent
  FALSE if RACH has yet to be sent

SIDE EFFECTS
  None
===========================================================================*/
static boolean rach_tx_handler
(
   boolean  start_of_block,     /* next frame is RACH */
   uint32   timestamp,          /* currently not used */
   gas_id_t gas_id
)
{
   boolean remove_handler = FALSE;

   switch (rach_params[gas_id].rach_state)
   {
      case RACH_STATE_NULL:
      {
         MSG_GERAN_ERROR_1_G("Unexpected NULL state for RACH, FN=%d", GSTMR_GET_FN_GERAN(gas_id));
         remove_handler = TRUE;
         break;
      }

      case RACH_STATE_WAITING_FOR_BLOCK_START:
      {
         /* Is the next frame a rach frame */
         if (!start_of_block) break;

         /* Decrement delay count until we get to zero */
         if (rach_params[gas_id].rach_delay > 0)
         {
            rach_params[gas_id].rach_delay--;
            break;
         }

         /* Delay is zero, time to transmit */

         /* Write RACH data to frame layer */
         gl1_hw_write_tx_data
          (
            rach_params[gas_id].rach_channel_type,
            (uint8 *)&rach_params[gas_id].rach_random_content,
            2,       /* data len */
            NULL,    /* no secondary data */
            0,       /* secondary data len */
            rach_params[gas_id].rach_signal_info.tn,
            &rach_params[gas_id].rach_tags,
            gas_id
          );

         if (gl1_get_rach_pwr_reduction_first_rach(gas_id))
         {
            gl1_set_rach_pwr_reduction_first_rach(FALSE,gas_id);
         }
         else
         {
            gl1_set_rach_pwr_reduction(FALSE,gas_id);
         }

         /* Schedule transmit */
         gl1_hw_tx_dtm_gsm(&rach_params[gas_id].rach_signal_info, &rach_params[gas_id].rach_burst_info, &rach_params[gas_id].rach_tags,gas_id);

         MSG_GERAN_MED_2_G("RACH txed: cnt=%d, FN=%d", rach_params[gas_id].rach_cnt, GSTMR_GET_FN_GERAN(gas_id));
         rach_params[gas_id].last_rach_cnt = rach_params[gas_id].rach_cnt;
         rach_params[gas_id].last_rach_tx_fn = GSTMR_GET_FN_GERAN(gas_id);


#ifdef GSM_HO_TIMING_ADJ
         /* Handle mDSP scheduling alignment bug. */
         /* Only adjust timing once per Handover. */
         if (rach_params[gas_id].handover_time_align && gl1_msg_tt_ho_bursts(gas_id) > 0)
         {
           gl1_msg_tt_ho_adjust(gas_id);
           rach_params[gas_id].handover_time_align = FALSE;
         }
#endif

         /* Wait an extra frame to get the results of any Tx Conflict*/
#ifdef FEATURE_GSM_COEX_RACH_DELAY
         if((gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE) &&
	    (rach_params[gas_id].rach_type != GL1_MSG_RACH_HANDOVER))
         {
           L1_STATE_CHANGE(rach_params[gas_id].rach_state = RACH_STATE_CONFLICT_CHECK, "RACH_STATE_CONFLICT_CHECK");
           break;
         }
	 else
#endif /* FEATURE_GSM_COEX_RACH_DELAY */
         {
           L1_STATE_CHANGE(rach_params[gas_id].rach_state = RACH_STATE_TRANSMITTING, "RACH_STATE_TRANSMITTING");

           /* For handover access bursts, we need to support back to back
            access bursts and they don't need callbacks, so fall through
            so that we can complete the handler in a frame */
           if (rach_params[gas_id].rach_type != GL1_MSG_RACH_HANDOVER)
           {
              break;
           }
	 }
      }
      /* fall through */
      case RACH_STATE_TRANSMITTING:
      {
#ifdef FEATURE_GSM_COEX_RACH_DELAY
         if((gl1_msg_get_multi_sim_sys_mode() == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE) &&
	    (rach_params[gas_id].rach_type != GL1_MSG_RACH_HANDOVER))
         {
            /*Check the yield status and send an empty CNF back to RR if rach is denied*/
            rach_params[gas_id].yield = (mdsp_gprs_get_rach_burst_conflict(gas_id) != 0);
         }
         else
         {
            rach_params[gas_id].yield = FALSE;
         }
#endif /* FEATURE_GSM_COEX_RACH_DELAY */

         if (rach_params[gas_id].rach_callback.data_cb != NULL)
         {
            rach_params[gas_id].rach_callback.data_cb(rach_params[gas_id].rach_random_content, gas_id
#ifdef FEATURE_GSM_COEX_RACH_DELAY
           ,rach_params[gas_id].yield
#endif
            );
         }

         rach_params[gas_id].tx_rach--;
         if (rach_params[gas_id].tx_rach == 0)
         {
            L1_STATE_CHANGE(rach_params[gas_id].rach_state = RACH_STATE_NULL, "RACH_STATE_NULL");
            remove_handler = TRUE;
         }
         else
         {
            L1_STATE_CHANGE(rach_params[gas_id].rach_state = RACH_STATE_WAITING_FOR_BLOCK_START, "RACH_STATE_WAITING_FOR_BLOCK_START");
         }
         break;
      }

#ifdef FEATURE_GSM_COEX_RACH_DELAY
      case RACH_STATE_CONFLICT_CHECK:
      {
        L1_STATE_CHANGE(rach_params[gas_id].rach_state = RACH_STATE_TRANSMITTING, "RACH_STATE_TRANSMITTING");
        break;
      }
#endif /* FEATURE_GSM_COEX_RACH_DELAY */

      default:
      {
         MSG_GERAN_ERROR_1_G("Unexpected RACH state %d", rach_params[gas_id].rach_state);
         L1_STATE_CHANGE(rach_params[gas_id].rach_state = RACH_STATE_NULL, "RACH_STATE_NULL");
         remove_handler = TRUE;
         break;
      }
   }

   return(remove_handler);

/*lint -esym(715,timestamp)*/
}

boolean  gl1_msg_is_rach_tx_on_next_frame( gas_id_t gas_id )
{

  if(  ( rach_params[gas_id].rach_state == RACH_STATE_WAITING_FOR_BLOCK_START )
	 &&( 0 == rach_params[gas_id].rach_delay )
//	 &&( gl1_msg_is_ft_handler_installed( rach_tx_handler ) )
	)

  {
    return  TRUE;
  }

  return  FALSE;
}

/*===========================================================================

FUNCTION gl1_msg_get_rach_state

DESCRIPTION
 Returns the state of the RACH state machine

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
rach_state_type gl1_msg_get_rach_state(gas_id_t gas_id)
{
  MSG_GERAN_HIGH_1_G("get_rach_state %d", rach_params[gas_id].rach_state);
  return rach_params[gas_id].rach_state;
}
/*===========================================================================

FUNCTION gl1_msg_set_rach_delay

DESCRIPTION
 sets the rach_delay

DEPENDENCIES
  None

RETURN VALUE
  none

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_set_rach_delay(int16 rach_delay, gas_id_t gas_id)
{
  rach_params[gas_id].rach_delay = rach_delay;
}

/*===========================================================================

FUNCTION gl1_msg_get_rach_delay

DESCRIPTION
 Returns the rach_delay

DEPENDENCIES
  None

RETURN VALUE
  rach_delay

SIDE EFFECTS
  None
===========================================================================*/
int16 gl1_msg_get_rach_delay(gas_id_t gas_id)
{
  return rach_params[gas_id].rach_delay;
}

/*===========================================================================

FUNCTION gl1_msg_check_rach_tx_in_prev_frame

DESCRIPTION
 Returns TRUE if there was RACH tx in the previous frame

DEPENDENCIES
  None

RETURN VALUE
  TRUE  -  if there was RACH tx in the previous frame
  FALSE - if there was NOT a RACH tx in the previous frame

SIDE EFFECTS
  None
===========================================================================*/
boolean gl1_msg_check_rach_tx_in_prev_frame(gas_id_t gas_id)
{
  if((rach_params[gas_id].last_rach_tx_fn != L1_GPRS_INVALID_FN) &&
     (((rach_params[gas_id].last_rach_tx_fn + 1) % FRAMES_IN_HYPERFRAME) == GSTMR_GET_FN_GERAN(gas_id))
    )
  {
    return TRUE;
  }

  return FALSE;
}

/*===========================================================================

FUNCTION gl1_msg_clear_last_rach_tx_fn

DESCRIPTION
 Clears the last_rach_tx_fn

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_clear_last_rach_tx_fn(gas_id_t gas_id)
{
  rach_params[gas_id].last_rach_tx_fn = L1_GPRS_INVALID_FN;
}

