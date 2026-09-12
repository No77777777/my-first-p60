/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              l1_sc_utils.c

GENERAL DESCRIPTION
   This module contains utility routines for the SCE

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_sc_utils.c#2 $

when       who       what, where, why
--------   ---       ----------------------------------------------------------
27/01/20   rks       CR2079539 FR 42539: GERAN changes for Mini Dump Phase - 2 support [GL1]
24/11/15   js        CR930857 Debug crash code whenever big offset in SC 
30/07/15   am        CR882081 Ensure SCH reconfirmation happens as specified by 3GPP
29/06/15   am        CR861464 Ensure fn_when_surround_tick_changed is up-to-date
11/05/15   pjr       CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
27/11/14   ak        CR748234 Problems with legacy SCE Opt 16/17/20
17/04/09   nt        CR179739 - DTM/CS uses different acq window sizes
09/04/09   og        Seperate the dedicated and transfer SCH scheduling decisions
                     CR174438.
05/03/07   ws        Fixed filtered lint warnings
02/01/06   ip        CR85687 & CR85686 FCH and SCH corrections to receiver window
                     size in DTM mode
26/01/06   og        Neighbour cell acquisition changes to handle extended dynamic.
17/11/05   nt        Added setting of first_rx_tn=2 and last_tx_tn=5 for calls to
                     gl1_msg_ncell_sch_pos_in_acq_window under FEATURE_GSM_DTM.
03/18/04   gw        Removed lint errors in l1_sc_decode_sb().
03/02/04   gw        Backed out lint changes in l1_sc_decode_sb().
02/25/04   gw        Lint clean up.
07/08/03   bk        Made changes to sch_pos i/f as part of decoding all BSICs
04/11/03   ATM       Moved Frame Number management entirely inside GTMRS
10/01/02   ATM       Added central get and set FN routines
08/27/02   plm       Created from m10280.c

===========================================================================*/
#include "l1_sc.h"
#include "gtmrs.h"
#include "gtmrs_g.h"    /* for ADD_FN & SUB_FN macros */
#include "gl1_msg_g.h"

uint8  get_BSIC_from_sch_data( const uint8 *sch_data );
uint32 get_RFN_from_sch_data( const uint8 *sch_data );

/******************************************************************************
 *
 *     Function name :  l1_sc_adjust_lag
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Adjust the timing information of a base station according to a
 *     timing error.
 *
 *     Parameters :
 *     ---------------
 *     INOUT - bs           - Base station info to be adjusted.
 *     IN    - timing_error - The quarter bit timing error.
 *                            Positive means the block has been
 *                            received later than expected.
 *
 */

void l1_sc_adjust_lag( l1_sc_bs_T* bs, sword timing_error )
{
   int16 qb = bs->qbit_lag + timing_error;
   if ( qb >= 5000 )
   {
      if ( timing_error > 0 )
      {
         qb -= 5000;
         bs->fn_lag = ADD_FN( bs->fn_lag, 1 );
      }
      else
      {
         qb += 5000;
         bs->fn_lag = SUB_FN( bs->fn_lag, 1 );
      }
   }
   else if ( qb < 0 )
   {
      qb += 5000;
      bs->fn_lag = SUB_FN( bs->fn_lag, 1 );
      if(qb < 0)
      {
#ifdef FEATURE_QSH_MDUMP
        QSH_MDUMP_SET(QSH_MDUMP_CRASH_CAT_UNKNOWN,QSH_CLT_GL1,QSH_CLT_GL1)
#endif
        ERR_GERAN_FATAL_3_G(" Unusual qbit value %d for arfcn %d timing error %d", qb ,bs->arfcn.num ,timing_error );
      }
   }
   bs->qbit_lag = qb;
}

/******************************************************************************
 *
 *     Function name :  l1_sc_adjust_lag_tch
 *     ---------------------------------
 *
 *     Description :
 *     ---------------
 *     Adjust the timing information of a base station according to a
 *     timing error.  The fn_lag adjustment is different on the TCH
 *     because of the difference in frame structures, so we have a
 *     different function for this
 *
 *     Parameters :
 *     ---------------
 *     INOUT - bs           - Base station info to be adjusted.
 *     IN    - timing_error - The quarter bit timing error.
 *                            Positive means the block has been
 *                            received later than expected.
 *
 */

boolean l1_sc_adjust_lag_tch( l1_sc_bs_T* bs, sword timing_error, gas_id_t gas_id )
{
  l1_sc_globals_T   *l1_sc_globals_ptr  = gl1_ms_switch_l1_sc_globals_store(gas_id);
   int16 qb = bs->qbit_lag + timing_error;
   boolean sc_wrap = FALSE;
   int16 ncell_sch_pos_in_acq_window;

   /* Instantiate and initialise the lowest and highest timeslot
    * information structures.
    */
    gl1_defs_tn_struct  lowest_tn_info  = { GL1_DEFS_TN_0, /* relative timeslot */
                                                  FALSE };       /* transmit boolean  */
    gl1_defs_tn_struct  highest_tn_info = { GL1_DEFS_TN_3, /* relative timeslot */
                                                  FALSE };       /* transmit boolean  */

   /* gl1_msg_ncell_sch_pos_in_acq_window_dedic() determines the position
      of the SCH within the surround cell acquisition window on the
      TCH.  It returns 1 if lies after the end of the acq window,
      -1 if it lies before the start of the acq window and 0 if it
      lies within.  Thus 1 or -1 would indicate that the SCH lies
      outside the acquisition window and we need to wrap around
   */
   l1_sc_highest_lowest_timslot(&(lowest_tn_info), &(highest_tn_info), gas_id);
   highest_tn_info.tn = (gl1_defs_tn_type) ADD_MOD( (uint32) highest_tn_info.tn, 3, GL1_DEFS_SLOTS_IN_FRAME );

   /*The constraints apllied to the acquisition window differ for dedicated mode*/
   if ( l1_sc_globals_ptr->mode == L1SCModeDediTCh)
   {
     ncell_sch_pos_in_acq_window = gl1_msg_ncell_sch_pos_in_acq_window_dedic(
                                                                   qb,
                                                                   &(lowest_tn_info),
                                                                   &(highest_tn_info),
                                                                   gas_id);
   }
   else
   {
     ncell_sch_pos_in_acq_window = gl1_msg_ncell_sch_pos_in_acq_window(
                                                                   qb,
                                                                   &(lowest_tn_info),
                                                                   &(highest_tn_info),gas_id );
   }

   if ( ncell_sch_pos_in_acq_window == 1 )
   {
         /* Slide by a 51 multiframe to handle the wrap but we need
            to slide by 2*26 TCH multiframes to achieve this
         */
         qb -= 5000;
         bs->sch_wrap_direction = 1;

         sc_wrap = TRUE;
   }
   else if ( ncell_sch_pos_in_acq_window == -1 ) /* Rx and Tx TS */
   {
         /* Slide by a 51 multiframe to handle the wrap but we need
            to slide by 2*26 TCH multiframes to achieve this
         */
         qb += 5000;
         bs->fn_lag = SUB_FN( bs->fn_lag, 1 );
         bs->sch_wrap_direction = -1;

         sc_wrap = TRUE;
   }
   else if ( qb < 0 )
   {
      bs->fn_lag = SUB_FN( bs->fn_lag, 1 );
   }
   bs->qbit_lag = qb;

   if ( !sc_wrap )
   {
     bs->sch_wrap_direction = 0;
   }

   return ( sc_wrap );
}

/*
 *     Function name :  l1_sc_decode_sb
 *     --------------------------------
 *
 *     Description :
 *     ---------------
 *     Decode the data of an SB.
 *
 *     Parameters :
 *     ---------------
 *     IN  - sb_data - SB data to be decoded.
 *     OUT - bsic    - The BSIC encoded in the SB
 *
 *     Returns :
 *     -----------
 *     BS frame number encoded in the SB.
 *
 */
dword l1_sc_decode_sb( byte sb_data[4], byte* bsic )
{
   *bsic = get_BSIC_from_sch_data(sb_data);

   return get_RFN_from_sch_data(sb_data);
}

/*===========================================================================

FUNCTION l1_sci_inc_surround_tick

DESCRIPTION
  Increment surround_tick.

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

void l1_sci_inc_surround_tick( int proposed_surround_tick_inc, gas_id_t gas_id )

{
   l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);
   uint32 cur_fn;
   int surround_tick_inc;

   cur_fn = GSTMR_GET_FN_GERAN(gas_id);

   if( proposed_surround_tick_inc == 1)
   {
      /* dedicated / packet transfer mode */
      surround_tick_inc = proposed_surround_tick_inc;
   }
   else
   {
      /* idle mode */
      surround_tick_inc = ((int) (cur_fn - l1_sc_globals_ptr->fn_when_surround_tick_changed)) / 51;
   }

   /* should be in range */
   if( (surround_tick_inc > 0) && (surround_tick_inc <= 9) )
   {
      /* surround tick inc is within range */

      l1_sc_globals_ptr->surround_tick += surround_tick_inc;
      l1_sc_globals_ptr->fn_when_surround_tick_changed = cur_fn;
   }
   else if (surround_tick_inc != 0)
   {
       /* > 9 or < 0 means we don't know where we are,
        * so blindly inc surround count by number given by caller
        */
       l1_sc_globals_ptr->surround_tick += proposed_surround_tick_inc;
       l1_sc_globals_ptr->fn_when_surround_tick_changed = cur_fn;
   }
   else
   {
      /* we have been called this too soon (51 frames have not passed yet),
       * so don't increment surround tick */
   }
}

/*===========================================================================

FUNCTION l1_sci_update_surround_tick_fn

DESCRIPTION
  Decrement fn_when_surround_tick_changed by the input frames.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void l1_sci_update_surround_tick_fn( uint32 frame_decrease, gas_id_t gas_id )
{
  if ( frame_decrease )
  {
    l1_sc_globals_T *l1_sc_globals_ptr = gl1_ms_switch_l1_sc_globals_store(gas_id);

    l1_sc_globals_ptr->fn_when_surround_tick_changed =
                          SUB_FN( l1_sc_globals_ptr->fn_when_surround_tick_changed, frame_decrease );
  }
}

