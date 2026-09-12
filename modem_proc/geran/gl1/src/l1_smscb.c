/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       L 1   I D L E - C B C H   H A N D L E R

GENERAL DESCRIPTION
  This module contains the procedures and global variables required to
  implement SMSCB processing.

EXTERNALIZED FUNCTIONS
  l1_smscb_init
  l1_smscb_process_scheduling_info
  l1_smscb_process_skip_info
  l1_smscb_process_deactivate_info
  l1_smscb_set_cb_supported
  l1_smscb_set_chan_type
  l1_smscb_get_chan_type
  l1_smscb_cbch_expected
  l1_smscb_gap_to_next_activity
  l1_smscb_decoding_basic_cbch
  l1_smscb_decoding extened_cbch
  l1_smscb_set_decoding_basic_cbch
  l1_smscb_set_decoding_extended_cbch
  l1_smscb_get_cbch_fn
  l1_smscb_set_arfcn
  l1_smscb_set_bsic
  l1_smscb_get_arfcn
  l1_smscb_get_bsic
  l1_smscb_is_active

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.
$Header: //components/rel/geran.mpss/7.4.0/gl1/src/l1_smscb.c#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
09/10/16   sp       CR1073155: Remove entries of SkipInfo which are earlier than next entry in DRX Sched info
06/09/16   sp       CR969186: While handling skip request in DRX mode, ensure to check if FN is already elapsed or not
16/05/16   db       CR1009490 Move CBCH priority handling from GRR to GL1
10/12/15   sp       CR948656 Set high prio CB only when CB mode is DRX mode
24/11/15   sp       CR924033 Apply RR requested CB priority inversion, only on next TC=0 occassion.
24/10/15   mko      CR921690 Update CB Skip Request handling, Check each entry of skip req and schedule intermediate TCs
07/09/15   sp       CR729269 Skip reserving CB activity for non TC0 occasions, if TC0 is not decoded yet.
01/10/15   hd       CR914811 Check for some valid entry not to be skipped within the boundaries of the CBCH skip list
07/09/15   hd       CR898517 Schedule next CBCH activity by going through the skip list instead of the last FN in the skip list
11/09/15   ws       CR906177 Change CBCH priority only when ISR is not running
12/08/15   hd       CR888214 Exchange the FM activity if SET_PRIORITY_IND has come to invert and a CBCH activity is already booked
14/07/15   hd       CR871015 DRX tick should be released when CBCH activity is cleared while TRM priority is changed.
19/05/15   sjw      CR840271 clear inactive CBCH FM entries when priority is changed
12/05/15   cs       CR653746 Remove unnecessary F3 when SCHED_REQ received in NON_DRX
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
24/03/15   sjw      CR811752: refactored priority inversion for CBCH
19/02/15   nk       CR780295 Clear CBCH scheduling information when clearing CBCH frames while handling CB_SKIP_REQ
18/02/15   am       CR770270 Ensure CBCH is not scheduled if it is cleared from frame manager
17/02/15   am       CR787188 Removing Deprecated TRM APIs in GERAN
29/09/14   sk       CR743686: Ensure Frames for the Cb activity are cleared when new scheduling/skip req received
15/09/14   sk       CR722967 Ensure CB schedule message processed when other RAT is in BG TRAFFIC
15/09/14   ws       CR724529 Resolve remaining P1 Klocwork issues
04/09/14   am       CR682760 Addressing KW GL1 P1 errors
20/06/14   jj       CR656872 fix critical gl1 kw error
20/11/13   sk       CR579082 Reset the scheduling mode when CB schedule request is ignored
25/02/13   ap       CR450915 DSDS: Not decoding PCH as per DRX
06/02/13   cs       DSDS CR438977: merged support for CBCH in DSDS mode
28/11/12   br       Fix for Klockwork error
01/05/12   ws       CR308681 - CBCH integration from MDM6610
04/01/12   cs       CR297422 defer the EXPIRY_IND until after last CB block decode
03/11/11   cs       CR316804 Do not perform TC0 alignment check when calculating valid gap
20/10/11   sjw      CR309794 reset non-DRX TCO flag to FALSE for all schedule requests
08/08/11   cs       Do not send a SKIP_CNF if not doing an ERASE_LAST_SCHEDULING
                    as the fn_list can be NULL
04/08/11   sjw      CR299790 - reset CBCH scheduling after reselection
22/07/11   cs       CR297420 Avoid missing the SKIP_REQ processing in CB DRX mode
18/07/11   cs       CR297428 Add in TC0 synchronisation for first CB block in Non-DRX
15/06/11   cs       DSDS Add a utility func to give correct FN
16/05/11   cs       DSDS CR282434 Reenable CBCH support on DSDS
17/11/10   cja      Fix Klocwork issues
14/07/10   cs       Only send the SCHED_CNF in CB_DRX mode
14/07/10   cs       Minor compile update
13/07/10   cs       Updates for CB_DRX support
04/03/08   ap       CR143065 Complete CR120987 as below fixes SDCCH/8(CBCH_TN>CCCH_TN)
                    but breaks SDCCH/8(CBCH_TN<=CCCH_TN) and SDCCH/4
28/09/07   ws       CR120987 remove Fn+1 offset in l1_smscb_cbch_expected() since
                    this results in SKIP_REQ msg being ignored
03/11/05   sv       Mainlined FEATURE_GSM_CB
09/24/04   bc       Fixed DRX gap calculation.
08/09/04   gsc      Added more error checking. Added support for CB_SKIP_CNF.
                    Included fixes for DRX mode for issues found during unit
                    testing
06/21/04   gsc      Fixed a type conversion bug in calc_non_drx_gap_using_skip_list
06/02/04   gw       Free memory allocated dynamically by CB task when processing
                    CB_SKIP_REQ.
05/26/04   sv       Lint Cleanup + Changed "l1_isrtsk_blk" structure parameters
                    for better code readability.
05/14/04   gsc      Cosmetic change to l1_smscb_get_cbch_fn function.
03/25/03   gsc      Added FEATURE_GSM_CB_DRX_SUPPORTED to turn off DRX mode.
01/13/03   gsc      Added some optimizations. Added more checks to validate
                    messages received from CB. Now using IS_FRAME_NUM_LATER and
                    ADD_FN macros. Quadband support. Return type for functions
                    calculating gap to next CB block is now changed to uint32.
10/22/03   gsc      Added support for skip request in non-drx mode.
                    Formatting changes.
10/09/03   gsc      First revision.

===========================================================================*/


/*===========================================================================

              INCLUDE FILES FOR MODULE

===========================================================================*/
#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#include "l1_smscb.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "l1i.h"
#include "l1_isr.h"
#include "msg.h"
#include "gs.h"
#include "l1_utils.h"
#include "gprsdef.h"
#include "gprsdef_g.h"

#include "l1_task.h"

#include "geran_dual_sim.h"

#include "gpl1_dual_sim.h"
#include "gl1_msg_g.h"
#include "l1_fm.h"

/*===========================================================================

           DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

/* How many frames ahead we have to program F/W to perform CBCH decode */
#define NUM_FRAMES_EARLY_CBCH_SCHEDULE 2

/* How many frames ahead we have to calculate DRX gap for CBCH decode */
#define NUM_FRAMES_EARLY_DRX_GAP       1

/*---------------------------------------------------------------------------
 *                       LOCAL FUNCTIONS
 *-------------------------------------------------------------------------*/
static void smscb_init( l1_smscb_cbch_info_type *cbch_info,
                        boolean                  basic_cbch );

static boolean cbch_expected( l1_smscb_cbch_info_type *cbch_info,
                              uint32                   fn , gas_id_t gas_id);

static boolean process_scheduling_info( l1_smscb_cbch_info_type *cbch_info,
                                        l1_smscb_mode_type       sched_mode,
                                        uint32                  *fn_list,
                                        uint8                    fn_list_len,
                                        gas_id_t                 gas_id );

static boolean process_skip_info( l1_smscb_cbch_info_type *cbch_info,
                                  uint32                  *fn_list,
                                  uint8                    fn_list_len,
                                  cb_skip_ind_type         skip_ind );

static uint32 cbch_gap_to_next_activity( l1_smscb_cbch_info_type *cbch_info,
                                         gas_id_t                 gas_id );

static uint32 calc_non_drx_gap_using_skip_list( l1_smscb_cbch_info_type *cbch_info,
                                                uint8                    cbch_fn,
                                                uint32                   this_FN,
                                                gas_id_t                 gas_id );

static uint32 calc_non_drx_gap( l1_smscb_cbch_info_type *cbch_info,
                                uint8                    cbch_fn,
                                uint32                   this_FN,
                                gas_id_t                 gas_id );

static l1_smscb_mode_type l1_smscb_map_cb_sched_to_l1_sched(
                           cb_scheduling_mode_type cb_sched_mode );

static uint32 l1_smscb_get_FN( gas_id_t gas_id );

static int l1_smscb_compare_skip_list( const void *a, const void *b );
void l1_reset_non_drx_tc0(gas_id_t gas_id);

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
static uint32 calc_drx_gap( l1_smscb_cbch_info_type *cbch_info,
                            uint32                   this_FN,
                            gas_id_t                 gas_id );

static cb_channel_ind_type map_l1_service_info_to_cb_channel_ind(
                            l1_smscb_service_type service_info,
                            gas_id_t              gas_id );

static void l1_smscb_send_expiry_ind_and_reset( l1_smscb_cbch_info_type *cbch_info,
                                                boolean                  deferred_expiry,
                                                gas_id_t                 gas_id );
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

/*---------------------------------------------------------------------------
 *                       GLOBAL DATA
 *-------------------------------------------------------------------------*/
static l1_smscb_cbch_info_type l1_basic_cbch_info[NUM_GERAN_DATA_SPACES];

static l1_smscb_cbch_info_type l1_extended_cbch_info[NUM_GERAN_DATA_SPACES];

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_TRIPLE_SIM)
static geran_priority_t        l1_cbch_priority[NUM_GERAN_DATA_SPACES] = { INITIAL_VALUE(GERAN_PRIORITY_NOMINAL) };
#endif /* FEATURE_DUAL_SIM || FEATURE_TRIPLE_SIM */

/*===========================================================================
FUNCTION: smscb_init

DESCRIPTION:
  This function intializes the L1 for CBCH processing. It puts the SMSCB
  state machine in Init mode.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_init( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  MSG_GERAN_MED_0_G( L1_CBCH_HDR"Initializing SMS CB" );

  smscb_init( l1_basic_cbch_info_p,    TRUE );

  smscb_init( l1_extended_cbch_info_p, FALSE );
}

/*===========================================================================
FUNCTION: smscb_init

DESCRIPTION:
  This is a local function used to initialize the basic_cbch_info and
  extended_cbch_info structures.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static ARFCN_T cbch_invalid_arfcn = { INVALID_ARFCN, SYS_BAND_NONE };

static void smscb_init( l1_smscb_cbch_info_type *cbch_info,
                        boolean                  basic_cbch )
{
  uint16 i;

  /*
   * Initialize the fn_list.
   */
  for ( i = 0; i < ARR_SIZE( cbch_info->fn_list ); i++ )
  {
    cbch_info->fn_list[i]       = 0;
  }

  for ( i = 0; i < ARR_SIZE( cbch_info->skip_fn_list ); i++ )
  {
    cbch_info->skip_fn_list[i]  = 0;
  }

  /*
   * Initialize cbch_info.
   */
  cbch_info->fn_list_len        = 0;
  cbch_info->skip_fn_list_len   = 0;
  cbch_info->curr_fn_index      = 0;
  cbch_info->curr_skip_fn_index = 0;
  cbch_info->curr_cbch_fn       = 0;

  if ( basic_cbch )
  {
    cbch_info->service          = L1_SMSCB_BASIC_CBCH;
  }
  else
  {
    cbch_info->service          = L1_SMSCB_EXTENDED_CBCH;
  }

  cbch_info->cbch_supported     = FALSE;
  cbch_info->decoding_cbch      = FALSE;
  cbch_info->chan               = SDCCH_4;
  cbch_info->sched_mode         = L1_SMSCB_INIT_MODE;
  cbch_info->arfcn              = cbch_invalid_arfcn;
  cbch_info->bsic               = 0;
  cbch_info->high_priority      = FALSE;
  cbch_info->non_drx_tc0        = FALSE;

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
  cbch_info->deferred_expiry    = FALSE;
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */
}

/*===========================================================================
FUNCTION: l1_smscb_process_scheduling_info

DESCRIPTION:
  This function handles the schedule request received from the CB module.
  The shedule request primitive either puts the CB state machine in NON-DRX
  mode or in DRX mode. In DRX mode this primitive contains a schedule
  that is followed by the L1 to wake up, schedule and decode CBCH.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_process_scheduling_info( cb_channel_ind_type      chan_ind,
                                       cb_scheduling_mode_type  sched_mode,
                                       uint32                  *fn_list,
                                       uint8                    fn_list_len,
                                       gas_id_t                 gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];
  l1_smscb_mode_type cb_sched_mode = l1_smscb_map_cb_sched_to_l1_sched( sched_mode );

  MSG_GERAN_HIGH_3_G( L1_CBCH_HDR"Rcvd sched req: Service %d Mode %d Fn_List_len %d",
                      chan_ind, cb_sched_mode, fn_list_len );

  /* reset high priority flags for the active stack */
  l1_extended_cbch_info_p->high_priority = FALSE;
  l1_basic_cbch_info_p->high_priority    = FALSE;

  /* reset the non-DRX TC0 alignment flag for new schedules */
  l1_extended_cbch_info_p->non_drx_tc0 = FALSE;
  l1_basic_cbch_info_p->non_drx_tc0    = FALSE;

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
  /* reset deferred expiry send to avoid spurious ind */
  l1_extended_cbch_info_p->deferred_expiry = FALSE;
  l1_basic_cbch_info_p->deferred_expiry    = FALSE;

  /*------------------------------------------------------------------------
   * Reject the new cbch schedule if the frame list contains frame number
   * greater than the FRAMES_PER_HYPERFRAME ( 26*51*2048) in DRX mode.
   ------------------------------------------------------------------------*/
  if ( L1_SMSCB_DRX_MODE == cb_sched_mode )
  {
    if ( !fn_list || !fn_list_len )
    {
      MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Invalid Sched in DRX mode fn_list %d list_len %d",
                           fn_list, fn_list_len );

      L1_send_CB_SCHEDULING_CNF( fn_list, gas_id );

      return;
    }
    else
    {
      uint16 i;

      for ( i = 0; i < fn_list_len; i++ )
      {
        if ( fn_list[i] >= FRAMES_PER_HYPERFRAME )
        {
          MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Invalid frame in the fn_list[%d] %d",
                               i, fn_list[i] );

          L1_send_CB_SCHEDULING_CNF( fn_list, gas_id );

          return;
        }
      }
    }
  }
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

  /*------------------------------------------------------------------------
   * If the channel indication is CBCH
   ------------------------------------------------------------------------*/
  if ( chan_ind == CBCH )
  {
    if ( (L1_SMSCB_DRX_MODE == cb_sched_mode) && (l1_basic_cbch_info_p->cbch_supported))
    {
       /* if the pseudo-DRX feature is defined, CBCH is prioritised over paging */
       l1_basic_cbch_info_p->high_priority = TRUE;

       MSG_GERAN_HIGH_1_G( L1_CBCH_HDR"Multi-SIM high priority basic CBCH decoding set to %d",
                           (int)l1_basic_cbch_info_p->high_priority );
    }
    if ( !l1_basic_cbch_info_p->cbch_supported )
    {
      MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Rcvd sched req, basic cbch not supported" );
    }
    else if ( !process_scheduling_info( l1_basic_cbch_info_p,
                                        cb_sched_mode,
                                        fn_list, fn_list_len, gas_id ) )
    {
      MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Rcvd invalid basic cbch sched info" );
    }
  }
  /*------------------------------------------------------------------------
   * If the channel indication is ECBCH
   ------------------------------------------------------------------------*/
  else if ( chan_ind == ECBCH )
  {
    if ( (L1_SMSCB_DRX_MODE == cb_sched_mode) && (l1_extended_cbch_info_p->cbch_supported))
    {
       /* if the pseudo-DRX feature is defined, CBCH is prioritised over paging */
       l1_extended_cbch_info_p->high_priority = TRUE;

       MSG_GERAN_HIGH_1_G( L1_CBCH_HDR"Multi-SIM high priority extended CBCH decoding set to %d",
                           (int)l1_extended_cbch_info_p->high_priority );
    }
    if ( !l1_extended_cbch_info_p->cbch_supported )
    {
      MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Rcvd sched req, ext cbch not supported" );
    }
    else if ( !process_scheduling_info( l1_extended_cbch_info_p,
                                        cb_sched_mode,
                                        fn_list, fn_list_len, gas_id ) )
    {
      MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Rcvd invalid ext cbch sched info" );
    }
  }

  GL1_ISR_LOCK(gas_id);

  /* Clear any pending reservations with TRM and clear old cbch scheduling if possible */
  l1_clear_inactive_cbch_entries( gas_id );
  l1_clear_cbch_scheduling( gas_id );

  GL1_ISR_UNLOCK(gas_id);

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
  /*------------------------------------------------------------------------
   * Only need to send the CNF response if in DRX
   *------------------------------------------------------------------------*/
  if ( L1_SMSCB_DRX_MODE == cb_sched_mode )
  {
    L1_send_CB_SCHEDULING_CNF( fn_list, gas_id );
  }
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */
}

/*===========================================================================
FUNCTION: process_scheduling_info

DESCRIPTION:
  This is a local function that handles the processing of scheduling info
  primitive received from the CB module for both basic and extended service
  type

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static boolean process_scheduling_info( l1_smscb_cbch_info_type *cbch_info,
                                        l1_smscb_mode_type       sched_mode,
                                        uint32                  *fn_list,
                                        uint8                    fn_list_len,
                                        gas_id_t                 gas_id )
{
  boolean return_success_state = FALSE;

#ifndef FEATURE_GSM_CB_DRX_SUPPORTED
  if ( L1_SMSCB_DRX_MODE == sched_mode )
  {
    MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Rcvd sched in DRX mode: DRX not supported" );
  }
  else
  {
    /*
     * Change the L1 SMS CB mode based on the node received from CB
     */
    cbch_info->sched_mode = sched_mode;

    return_success_state = TRUE;

    /* Start scheduling CBCH with low priority for first TC cycle */
    l1_fm_set_CBCH_pri(FALSE, gas_id);
  }
#else
  
  

  /*
   * Now if we are being asked to go to the DRX mode, then look at the
   * schedule and save it for basic CBCH.
   */
  if (sched_mode == L1_SMSCB_DRX_MODE )
  {
    uint16  i;
    uint32  cbch_fn;

    boolean done    = FALSE;
    uint32  this_FN = l1_smscb_get_FN( gas_id );

    MSG_GERAN_HIGH_1_G( L1_CBCH_HDR"Rcvd new schedule in DRX mode list length %d",
                        fn_list_len );

    if ( !fn_list_len ||
         ( fn_list_len > ARR_SIZE( cbch_info->fn_list ) ) )
    {
      MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Invalid sched list length %d in sched req max %d",
                           fn_list_len, ARR_SIZE( cbch_info->fn_list ) );

      return ( return_success_state );
    }

    for ( i = 0; i < fn_list_len; i++ )
    {
      if ( ( ( fn_list[i] % MULTIFRAME_51 ) != 32 ) &&
           ( ( fn_list[i] % MULTIFRAME_51 ) != 8 ) )
      {
        MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Invalid frame in sched_req_list[%d] %d",
                             i, fn_list[i] );

        return ( return_success_state );
      }
    }
	/*
   * Change the L1 SMS CB mode based on the node received from CB only if fn_lists are valid
   */
    cbch_info->sched_mode = sched_mode;
    /* New schedule request to replace any currently active */
    cbch_info->fn_list_len        = fn_list_len;
    cbch_info->curr_fn_index      = 0;
    cbch_info->curr_skip_fn_index = 0;
    cbch_info->skip_fn_list_len   = 0;

    for ( i = 0; i < fn_list_len; i++ )
    {
      cbch_info->fn_list[i] = fn_list[i];

      MSG_GERAN_HIGH_2_G( L1_CBCH_HDR"Rcvd new schedule in DRX mode new fn_list item sched_fn[%d] %d",
                          i, cbch_info->fn_list[i] );
    }

    while ( !done )
    {
      uint32 num_frames_early = NUM_FRAMES_EARLY_CBCH_SCHEDULE;

      cbch_fn = cbch_info->fn_list[cbch_info->curr_fn_index];

      /*
       * If we receive the scheduling information with the first CBCH decode
       * schedule to happen at this_FN + 2, we will not be able to decode
       * the CBCH block. The start of the CBCH block should atleast be at
       * this_FN + 3.
       */
      if ( IS_FRAME_NUM_LATER( this_FN,
                               SUB_FN( cbch_fn, num_frames_early ) ) )
      {
        cbch_info->curr_fn_index++;

        /*------------ -------------------------------------------------
         * Check to ensure that after advancing we have not gone past
         * the entire schedule. If we did, reset the schedule list and
         * send a schedule expiry indication to CB.
         *-------------------------------------------------------------*/
        if ( cbch_info->curr_fn_index >= cbch_info->fn_list_len )
        {
#ifdef GL1_EXTENDED_CBCH_DBG
          MSG_GERAN_HIGH_2_G( L1_CBCH_HDR"advanced past end of list %d in sched list len %d",
                              cbch_info->curr_fn_index, cbch_info->fn_list_len );
#endif /* GL1_EXTENDED_CBCH_DBG */

          /* Send expiry straight away as block not scheduled */
          l1_smscb_send_expiry_ind_and_reset( cbch_info, FALSE, gas_id );

          done               = TRUE;
        }
      }
      else
      {
        return_success_state = TRUE;

        /* All good */
        done                 = TRUE;
      }
    }
  }
  else
  {
    cbch_info->sched_mode = sched_mode;
    /* CB may send non-DRX schedule when in any DRX mode */
    return_success_state   = TRUE;
    /* Start scheduling CBCH with low priority for first TC cycle */
    l1_fm_set_CBCH_pri(FALSE, gas_id);

    MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"Rcvd sched in non-DRX mode: assume pseudo-DRX active");
  }
#endif /* !FEATURE_GSM_CB_DRX_SUPPORTED */

  return ( return_success_state );
  /*lint -esym(715, fn_list, fn_list_len) suppress 'fn_list' not accessed */
}

/*===========================================================================
FUNCTION: l1_smscb_process_skip_info

DESCRIPTION:
  This function handles the skip request received from the CB module. It is
  used by the CB to tell L1 to skip some CBCH decodes from the schedule list
  or eliminate the entire schedule.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_process_skip_info( cb_channel_ind_type  chan_ind,
                                 uint32              *fn_list,
                                 uint8                fn_list_len,
                                 cb_skip_ind_type     skip_ind,
                                 gas_id_t             gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  MSG_GERAN_HIGH_4_G( L1_CBCH_HDR"Rcvd skip req Service %d Skip Type %d, SkipLen %d FN %d",
                      chan_ind, skip_ind, fn_list_len, l1_smscb_get_FN( gas_id ) );

  /*------------------------------------------------------------------------
   * Reject the skip list if it contains frame number greater than the
   * FRAMES_PER_HYPERFRAME ( 26*51*2048)
   *------------------------------------------------------------------------*/
  if ( skip_ind != CB_ERASE_LAST_SCHEDULING )
  {
    if ( !fn_list || !fn_list_len )
    {
      MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Rcvd Invalid skip req fn_list %p list_len %d",
                           fn_list, fn_list_len );

      L1_send_CB_SKIP_CNF( fn_list, gas_id );

      return;
    }
    else
    {
      uint16 i;

      for ( i = 0; i < fn_list_len; i++ )
      {
        if ( fn_list[i] >= FRAMES_PER_HYPERFRAME )
        {
          MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Invalid frame in the skip fn_list[%d] %d",
                               i, fn_list[i] );

          L1_send_CB_SKIP_CNF( fn_list, gas_id );

          return;
        }
      }
    }
  }


  if ( chan_ind == CBCH )
  {
    /*------------------------------------------------------------------------
     * Reject the skip request if a request is received when CBCH is not
     * supported.
     ------------------------------------------------------------------------*/
    if ( !l1_basic_cbch_info_p->cbch_supported )
    {
      MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Rcvd skip req, basic cbch not supported" );
    }
    /*------------------------------------------------------------------------
     * Reject the skip request if the skip indication is asking to erase
     * the complete schedule list and we are not in DRX mode. Since there
     * is no schedule in non-DRX mode, this is not a valid request.
     ------------------------------------------------------------------------*/
    else if ( ( l1_basic_cbch_info_p->sched_mode != L1_SMSCB_DRX_MODE ) &&
              ( skip_ind == CB_ERASE_LAST_SCHEDULING ) )
    {
      MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Invalid skip request in mode %d skip_ind %d",
                           l1_basic_cbch_info_p->sched_mode, skip_ind );
    }
    else if ( !process_skip_info( l1_basic_cbch_info_p, fn_list,
                                  fn_list_len, skip_ind ) )
    {
      MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Invalid basic skip info" );
    }
  }
  else if ( chan_ind == ECBCH )
  {
    /*------------------------------------------------------------------------
     * Reject the skip request if a request is received when ECBCH is not
     * supported.
     *------------------------------------------------------------------------*/
    if ( !l1_extended_cbch_info_p->cbch_supported )
    {
      MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Rcvd skip req, ext cbch not supported" );
    }
    /*------------------------------------------------------------------------
     * Reject the skip request if the skip indication is asking to erase
     * the complete schedule list and we are not in DRX mode. Since there
     * is no schedule in non-DRX mode, this is not a valid request.
     * ------------------------------------------------------------------------*/
    else if ( ( l1_extended_cbch_info_p->sched_mode != L1_SMSCB_DRX_MODE ) &&
              ( skip_ind == CB_ERASE_LAST_SCHEDULING ) )
    {
      MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Invalid skip request in mode %d skip_ind %d",
                           l1_extended_cbch_info_p->sched_mode, skip_ind );
    }
    else if ( !process_skip_info( l1_extended_cbch_info_p, fn_list,
                                  fn_list_len, skip_ind ) )
    {
      MSG_GERAN_ERROR_0_G( L1_CBCH_HDR"Invalid extended skip info" );
    }
  }

  /* Clear any pending reservations with TRM */
  l1_clear_inactive_cbch_entries( gas_id );
  l1_clear_cbch_scheduling( gas_id );

  /* Do not send a CNF for the ERASE request as fn_list checking is not required */
  if ( skip_ind != CB_ERASE_LAST_SCHEDULING )
  {
    L1_send_CB_SKIP_CNF( fn_list, gas_id );
  }
}

/*===========================================================================
FUNCTION: process_skip_info

DESCRIPTION:
  This is a local function that handles the processing of skip info
  primitive received from the CB module for both basic and extended service
  type

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static boolean process_skip_info( l1_smscb_cbch_info_type *cbch_info,
                                  uint32                  *skip_list,
                                  uint8                    skip_list_len,
                                  cb_skip_ind_type         skip_ind )
{
  boolean return_success_state = FALSE;

  if ( skip_ind != CB_ERASE_LAST_SCHEDULING )
  {
    uint8 i;

    if ( !skip_list_len ||
         ( skip_list_len > ARR_SIZE( cbch_info->skip_fn_list ) )
       )
    {
      MSG_GERAN_ERROR_2( L1_CBCH_HDR"Invalid skip list length:%d in skip req max:%d",
                         skip_list_len, ARR_SIZE( cbch_info->skip_fn_list ) );

      return ( return_success_state );
    }

    for ( i = 0; i < skip_list_len; i++ )
    {
      if ( ( ( skip_list[i] % MULTIFRAME_51 ) != 32 ) &&
           ( ( skip_list[i] % MULTIFRAME_51 ) != 8 ) )
      {
        MSG_GERAN_ERROR_2( L1_CBCH_HDR"Invalid frame in skip_list[%d] %d",
                           i, skip_list[i] );

        return ( return_success_state );
      }
    }
  }
  else
  {
     MSG_GERAN_MED_3( L1_CBCH_HDR"Erase Last Sched FN list %d FN index %d TC0 %d",
                      cbch_info->fn_list_len,
                      cbch_info->curr_fn_index,
                      cbch_info->non_drx_tc0 );

     /* Erase schedule request for any currently active */
     cbch_info->fn_list_len        = 0;
     cbch_info->curr_fn_index      = 0;
     cbch_info->curr_skip_fn_index = 0;
     cbch_info->skip_fn_list_len   = 0;

     return_success_state          = TRUE;

     return ( return_success_state );
  }

  /*
   * If a skip request for basic cbch is received in INIT
   * mode, reject the request.
   */
  switch ( cbch_info->sched_mode )
  {
    case L1_SMSCB_INIT_MODE:
    {
      MSG_GERAN_ERROR_0( L1_CBCH_HDR"Rcvd Invalid Skip request in init mode" );
    }
    break;

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
    case L1_SMSCB_DRX_MODE:
      /* Fall through */
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */
    case L1_SMSCB_NON_DRX_MODE:
    {
      uint16 i;

#ifdef GL1_EXTENDED_CBCH_DBG
      MSG_GERAN_MED_1( L1_CBCH_HDR"Rcvd Skip request list_len %d",
                       skip_list_len );
#endif /* GL1_EXTENDED_CBCH_DBG */

      if ( skip_list_len == 1 )
      {
        cbch_info->skip_fn_list[0] = skip_list[0];
      }
      else
      {
      for ( i = 0; i < skip_list_len; i++ )
      {
        cbch_info->skip_fn_list[i] = skip_list[i];

        MSG_GERAN_MED_2( L1_CBCH_HDR"Rcvd Skip request skip_fn_list[%d] %d",
                         i, cbch_info->skip_fn_list[i] );
      }

        /* Sort the list into ascending FN order */
        qsort( cbch_info->skip_fn_list, skip_list_len,
               sizeof( skip_list[0] ), l1_smscb_compare_skip_list );
      }

#ifdef GL1_EXTENDED_CBCH_DBG
      for ( i = 0; i < skip_list_len; i++ )
      {
        MSG_GERAN_MED_2( L1_CBCH_HDR"Sorted Skip request skip_fn_list[%d] %d",
                         i, cbch_info->skip_fn_list[i] );
      }
#endif /* GL1_EXTENDED_CBCH_DBG */

      cbch_info->skip_fn_list_len   = skip_list_len;
      cbch_info->curr_skip_fn_index = 0;

      return_success_state          = TRUE;
    }
    break;

    default:
    break;
  }

  return ( return_success_state );
}

/*===========================================================================
FUNCTION: l1_smscb_process_deactivate_info

DESCRIPTION:
  This function handles the deactivate request received from the CB module.
  This reset the L1 CB state machine and wipes out any schedule that
  was currently active.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_process_deactivate_info( cb_channel_ind_type chan_ind,
                                       gas_id_t            gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  channel_type_T chan;
  boolean        cbch_supported;

  if ( chan_ind == CBCH )
  {
    MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"Rcvd deactivate req for CBCH" );

    /*
     * Save the chan (SDCCH_4/SDCCH_8) and cbch_supported flag before
     * initializing the basic_cbch_info structure.
     */
    chan           = l1_basic_cbch_info_p->chan;
    cbch_supported = l1_basic_cbch_info_p->cbch_supported;

    smscb_init( l1_basic_cbch_info_p, TRUE );

    /*
     * Now restore the chan (SDCCH_4/SDCCH_8) and cbch_supported flag
     */
    l1_basic_cbch_info_p->chan           = chan;
    l1_basic_cbch_info_p->cbch_supported = cbch_supported;
  }
  else if ( chan_ind == ECBCH )
  {
    MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"Rcvd deactivate req for ECBCH" );

    /*
     * Save the chan (SDCCH_4/SDCCH_8) and cbch_supported flag before
     * initializing the extended_cbch_info structure.
     */
    chan           = l1_extended_cbch_info_p->chan;
    cbch_supported = l1_extended_cbch_info_p->cbch_supported;

    smscb_init( l1_extended_cbch_info_p, FALSE );

    /*
     * Now restore the chan (SDCCH_4/SDCCH_8) and cbch_supported flag
     */
    l1_extended_cbch_info_p->chan           = chan;
    l1_extended_cbch_info_p->cbch_supported = cbch_supported;
  }
  /* Reset CBCH priority */
  l1_fm_set_CBCH_pri(FALSE, gas_id);
}

/*===========================================================================
FUNCTION: l1_smscb_set_cb_supported

DESCRIPTION:
  This function stores whether CBCH is supported in the current cell based on
  SI4 received from the network or not.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_set_cb_supported( boolean val, gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  /* In order to always disable CBCH in Multi-SIM use this */
#if ( defined ( FEATURE_DUAL_SIM ) || defined( FEATURE_TRIPLE_SIM ) ) \
    && !defined ( FEATURE_NAS_CBS_DSDS )
  if ( val && gl1_msg_get_multi_sim_mode() )
  {
    val = FALSE;

    MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"Disabled in Multi-SIM Mode");
  }
  else
#endif /* ( FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM ) & !FEATURE_NAS_CBS_DSDS */
  {
    MSG_GERAN_HIGH_1_G( L1_CBCH_HDR"Basic CBCH supported %d",
                        val  );
  }

  l1_basic_cbch_info_p->cbch_supported = val;

  if ( !val )
  {
    l1_basic_cbch_info_p->sched_mode = L1_SMSCB_INIT_MODE;
  }

  /*
   * Turn off extended CBCH for now
   */
  l1_extended_cbch_info_p->cbch_supported = FALSE;

  l1_extended_cbch_info_p->sched_mode     = L1_SMSCB_INIT_MODE;
}

/*===========================================================================
FUNCTION: l1_smscb_set_chan_type

DESCRIPTION:
  This function stores the channel type combination (SDCCH_4/SDDCH_8) based
  on SI4 received from the network.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_set_chan_type( channel_type_T chan, gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  if ( chan == SDCCH_4 )
  {
     MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"CBCH channel SDCCH_4" );
  }
  else
  {
     MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"CBCH channel SDCCH_8" );
  }

  l1_basic_cbch_info_p->chan = l1_extended_cbch_info_p->chan = chan;
}


/*===========================================================================
FUNCTION: l1_smscb_set_arfcn

DESCRIPTION:
  This function saves the ARFCN of the serving cell, so that a CBCH schedule
  can be associated with a cell.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_set_arfcn( ARFCN_T arfcn, gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  l1_basic_cbch_info_p->arfcn = l1_extended_cbch_info_p->arfcn = arfcn;
}

/*===========================================================================
FUNCTION: l1_smscb_get_arfcn

DESCRIPTION:
  This function returns the ARFCN of the cell in which we received a valid
  schedule.

DEPENDENCIES:
  None

RETURN VALUE:
  The ARFCN of the cell.

SIDE EFFECTS:
  None
===========================================================================*/
ARFCN_T l1_smscb_get_arfcn( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];

  return ( l1_basic_cbch_info_p->arfcn );
}

/*===========================================================================
FUNCTION: l1_smscb_set_bsic

DESCRIPTION:
  This function saves the BSIC of the serving cell, so that a CBCH schedule
  can be associated with a cell.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_set_bsic( uint8 bsic, gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  l1_basic_cbch_info_p->bsic = l1_extended_cbch_info_p->bsic = bsic;
}

/*===========================================================================
FUNCTION: l1_smscb_get_bsic

DESCRIPTION:
  This function returns the BSIC of the cell in which we received a valid
  schedule.

DEPENDENCIES:
  None

RETURN VALUE:
  The BSIC of the cell

SIDE EFFECTS:
  None
===========================================================================*/
uint8 l1_smscb_get_bsic( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];

  return ( l1_basic_cbch_info_p->bsic );
}

/*===========================================================================
FUNCTION: l1_smscb_is_active

DESCRIPTION:
  This function returns True if SMSCB is currently active i.e. either
  we are in NON-DRX mode or DRX mode with a valid schedule. This routine
  is used to determine if the SMSCB state machine needs to be reset, after
  a cell-reselection or handover when we end up in a new cell carrying the
  schedule received in the old cell which is no-longer valid.

DEPENDENCIES:
  None

RETURN VALUE:
  True, if CBCH is active; otherwise False

SIDE EFFECTS:
  None
===========================================================================*/
boolean l1_smscb_is_active( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  return ( ( l1_basic_cbch_info_p->sched_mode != L1_SMSCB_INIT_MODE ) ? TRUE :
           ( ( l1_extended_cbch_info_p->sched_mode != L1_SMSCB_INIT_MODE ) ? TRUE :
           FALSE ) );
}

/*===========================================================================
FUNCTION: l1_smscb_get_chan_type

DESCRIPTION:
  This function returns the channel type combination (SDCCH_4/SDDCH_8) used
  in the current cell for CBCH.

DEPENDENCIES:
  None

RETURN VALUE:
  Channel type (SDCCH_4/SDDCH_8)

SIDE EFFECTS:
  None
===========================================================================*/
channel_type_T l1_smscb_get_chan_type( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];

  /* Basic and extended use same channel type */
  return ( l1_basic_cbch_info_p->chan );
}

/*===========================================================================
FUNCTION: l1_smscb_decoding_basic_cbch

DESCRIPTION:
  This function returns True if we are decoding Basic CBCH, otherwise False

DEPENDENCIES:
  None

RETURN VALUE:
  True, if decoding basic CBCH; otherwise False.

SIDE EFFECTS:
  None
===========================================================================*/
boolean l1_smscb_decoding_basic_cbch( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];

  return ( l1_basic_cbch_info_p->decoding_cbch );
}

/*===========================================================================
FUNCTION: l1_smscb_decoding_extended_cbch

DESCRIPTION:
  This function is used to remember that we are decoding Extended CBCH

DEPENDENCIES:
  None

RETURN VALUE:
  True, if decoding extended CBCH; otherwise False.

SIDE EFFECTS:
  None
===========================================================================*/
boolean l1_smscb_decoding_extended_cbch( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  return ( l1_extended_cbch_info_p->decoding_cbch );
}

/*===========================================================================
FUNCTION: l1_smscb_set_decoding_basic_cbch

DESCRIPTION:
  This function is used to remember that we are decoding Basic CBCH

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_set_decoding_basic_cbch( boolean val, gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];

  l1_basic_cbch_info_p->decoding_cbch = val;
}

/*===========================================================================
FUNCTION: l1_smscb_set_decoding_extended_cbch

DESCRIPTION:
  This function returns True if we are decoding Extended CBCH,
  otherwise False

DEPENDENCIES:
  None

RETURN VALUE:
  True, if decoding extended CBCH; otherwise False.

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_set_decoding_extended_cbch( boolean val, gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  l1_extended_cbch_info_p->decoding_cbch = val;
}

/*===========================================================================
FUNCTION: l1_smscb_get_cbch_fn

DESCRIPTION:
  This function returns the frame number of the start of the CBCH
  block that was decoded. This frame number is sent to L2 in PH_DATA_IND.

DEPENDENCIES:
  None

RETURN VALUE:
  The starting frame number of the decoded CBCH block.

SIDE EFFECTS:
  None
===========================================================================*/
uint32 l1_smscb_get_cbch_fn( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  if ( l1_smscb_decoding_basic_cbch( gas_id ) )
  {
    return ( l1_basic_cbch_info_p->curr_cbch_fn );
  }
  else if ( l1_smscb_decoding_extended_cbch( gas_id ) )
  {
    return ( l1_extended_cbch_info_p->curr_cbch_fn );
  }
  else
  {
    return ( 0 );
  }
}

/*===========================================================================
FUNCTION: l1_smscb_cbch_expected

DESCRIPTION:
  This function determines based on the frame number passed in whether cbch
  is expected in the next TDMA frame based on the Scheduling mode (DRX/NON-DRX)
  and cbch service type (Basic CBCH/Extended CBCH)

DEPENDENCIES:
  None

RETURN VALUE:
  True, if cbch is expected; otherwise False.

SIDE EFFECTS:
  None
===========================================================================*/
boolean l1_smscb_cbch_expected( uint32 fn, gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  boolean                  result                  = FALSE;


  if ( l1_basic_cbch_info_p->cbch_supported &&
       cbch_expected( l1_basic_cbch_info_p, fn, gas_id ) )
  {
    l1_basic_cbch_info_p->decoding_cbch = TRUE;

    l1_basic_cbch_info_p->curr_cbch_fn  = ADD_FN( fn, 1 );

    result                              = TRUE;

    MSG_GERAN_HIGH_1_G( L1_CBCH_HDR"Basic CBCH expected FN %d",
                        l1_basic_cbch_info_p->curr_cbch_fn );
  }
  else if ( l1_extended_cbch_info_p->cbch_supported &&
            cbch_expected( l1_extended_cbch_info_p, fn, gas_id ) )
  {
    l1_extended_cbch_info_p->decoding_cbch = TRUE;

    l1_extended_cbch_info_p->curr_cbch_fn  = ADD_FN( fn, 1 );

    result                                 = TRUE;

    MSG_GERAN_HIGH_1_G( L1_CBCH_HDR"Extended CBCH expected FN %d",
                        l1_extended_cbch_info_p->curr_cbch_fn );
  }

  return ( result );
}

/*===========================================================================
FUNCTION: cbch_expected

DESCRIPTION:
  This is a local function that determines if CBCH is expected based on
  the CB mode (DRX/NON-DRX) and the service type (Basic/Extended).

DEPENDENCIES:
  None

RETURN VALUE:
  True, if cbch is expected; otherwise False.

SIDE EFFECTS:
  None
===========================================================================*/
static boolean cbch_expected( l1_smscb_cbch_info_type *cbch_info,
                              uint32                   fn,
                              gas_id_t                 gas_id )
{
  boolean result          = FALSE;

  uint32  cbch_fn         = ADD_FN( fn, 1 );

  uint32  last_skip_fn    = cbch_info->skip_fn_list[cbch_info->skip_fn_list_len - 1];

  uint8   fn_div_51_mod_8 = (uint8)( ( cbch_fn / MULTIFRAME_51 ) % TIMESLOTS_IN_FRAME );

#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_HIGH_3_G( L1_CBCH_HDR"expected non_drx_tc0:%d TC:%d sched_mode:%d",
            cbch_info->non_drx_tc0, fn_div_51_mod_8, cbch_info->sched_mode );
#endif /* GL1_EXTENDED_CBCH_DBG */

  switch ( cbch_info->sched_mode )
  {
    case L1_SMSCB_INIT_MODE:
    break;

    case L1_SMSCB_NON_DRX_MODE:
    {
      /*
       * In NON-DRX mode we must wait for first TC0 block before allowing
       * any valid skip list with non-TCO FN periods
       */
      if ( !cbch_info->non_drx_tc0 && fn_div_51_mod_8 )
      {
#ifdef GL1_EXTENDED_CBCH_DBG
        MSG_GERAN_HIGH_0_G( L1_CBCH_HDR"NON DRX with non TC0 first CBCH decode" );
#endif /* GL1_EXTENDED_CBCH_DBG */

        break;
      }

      if ( cbch_info->service == L1_SMSCB_BASIC_CBCH )
      {
        /*--------------------------------------------------------------------
         * Is basic CBCH expected?
         --------------------------------------------------------------------*/
        switch ( fn_div_51_mod_8 )
        {
          case 0:
          case 1:
          case 2:
          case 3:
          {
            /*
             * If there is a valid skip list
             */
            if ( cbch_info->skip_fn_list_len &&
                 ( cbch_info->skip_fn_list_len <= MAX_CBCH_BLOCKS_IN_SKIP_REQ ) )
            {
              if( (cbch_info->skip_fn_list_len - 1) < MAX_CBCH_BLOCKS_IN_SKIP_REQ)
              {
                last_skip_fn = cbch_info->skip_fn_list[cbch_info->skip_fn_list_len - 1];
              }
              else
              {
                MSG_GERAN_ERROR_1_G("Invalid skip_fn_list index %d. Break from switch",
                                     cbch_info->skip_fn_list_len - 1 );
                break;
              }

              /*
               * Check to see if the skip list is still valid or has elapsed
			   * Clear List Len only if Skip list FN is elapsed.
               */
              if ( IS_FRAME_NUM_LATER ( cbch_fn, last_skip_fn ) )
              {
                cbch_info->skip_fn_list_len = 0;
                MSG_GERAN_HIGH_2_G( L1_CBCH_HDR"skip list elapsed CBCH: last_skip_fn %d cbch_fn %d",
                                    last_skip_fn, cbch_fn );
              }
            }

            result = TRUE;
          }
          break;

          default:
          break;
        }
      }
      else
      {
        /*--------------------------------------------------------------------
         * Is extended CBCH expected?
         --------------------------------------------------------------------*/
        switch ( fn_div_51_mod_8 )
        {
          case 4:
          case 5:
          case 6:
          case 7:
          {
            /*
             * If there is a valid skip list
             */
            if ( cbch_info->skip_fn_list_len )
            {
              if( (cbch_info->skip_fn_list_len - 1) < MAX_CBCH_BLOCKS_IN_SKIP_REQ)
              {
                last_skip_fn = cbch_info->skip_fn_list[cbch_info->skip_fn_list_len - 1];
              }
              else
              {
                MSG_GERAN_ERROR_1_G("Invalid skip_fn_list index %d. Break from switch",
                                     cbch_info->skip_fn_list_len - 1 );
                break;
              }

              /*
               * Check to see if the skip list is still valid or has elapsed
               */
              if ( IS_FRAME_NUM_LATER( cbch_fn, last_skip_fn ) )
              {
                cbch_info->skip_fn_list_len = 0;

                /*
                             * If the skip list is still valid, skip the CBCH block
                             */
                MSG_GERAN_HIGH_2_G( L1_CBCH_HDR"skip list elapsed ECBCH: last_skip_fn %d cbch_fn %d",
                                    last_skip_fn, cbch_fn );
              }
            }
            result = TRUE;
          }
          break;

          default:
          break;
        }
      }
    }
    break;

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
    case L1_SMSCB_DRX_MODE:
    {
#ifdef GL1_EXTENDED_CBCH_DBG
      MSG_GERAN_HIGH_3_G( L1_CBCH_HDR"expected cbch_fn:%d fn_list[%d]->%d",
                          cbch_fn, cbch_info->curr_fn_index,
                          cbch_info->fn_list[cbch_info->curr_fn_index] );
#endif /* GL1_EXTENDED_CBCH_DBG */

      if ( cbch_fn == cbch_info->fn_list[cbch_info->curr_fn_index] )
      {
        cbch_info->curr_fn_index++;

        if ( cbch_info->curr_fn_index >= cbch_info->fn_list_len )
        {
          /*
           * NOTE: Need to send a schedule expiry event to the CB.
           * The response from CB should be a new schedule req
           * that should request a new mode.
           * Allow the expiry ind to be deferred until after the last
           * scheduled block read is performed.
           */
          l1_smscb_send_expiry_ind_and_reset( cbch_info, TRUE, gas_id );
        }

        result = TRUE;
      }
    }
    break;
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

    default:
    {
      MSG_GERAN_ERROR_1_G( L1_CBCH_HDR"Invalid SMSCB mode %d",
                           cbch_info->sched_mode );
    }
    break;
  }

  return ( result );
}

/*===========================================================================
FUNCTION: l1_smscb_gap_to_next_activity

DESCRIPTION:
  This function determines the gap in terms of TDMA frame to the next CBCH
  activity so that we can wake up to decode cbch. The gap is computed
  based on the SMSCB mode (Non-DRX/DRX), Channel Combination Type
  (SDCCH_4/SDCCH_8) and the service type (Basic/Extended CBCH).

DEPENDENCIES:
  None

RETURN VALUE:
  The gap to the next cbch block.

SIDE EFFECTS:
  None
===========================================================================*/
uint32 l1_smscb_gap_to_next_activity( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  uint32 cbch_gap = MAX_CBCH_GAP_PERIOD;

  if ( l1_basic_cbch_info_p->cbch_supported )
  {
    cbch_gap = cbch_gap_to_next_activity( l1_basic_cbch_info_p, gas_id );

#ifdef GL1_EXTENDED_CBCH_DBG
    MSG_GERAN_HIGH_1_G( L1_CBCH_HDR"basic_cbch_gap:%d",
                        cbch_gap );
#endif /* GL1_EXTENDED_CBCH_DBG */
  }
  else if ( l1_extended_cbch_info_p->cbch_supported )
  {
    cbch_gap = cbch_gap_to_next_activity( l1_extended_cbch_info_p, gas_id );

#ifdef GL1_EXTENDED_CBCH_DBG
    MSG_GERAN_HIGH_1_G( L1_CBCH_HDR"extended_cbch_gap:%d",
                        cbch_gap );
#endif /* GL1_EXTENDED_CBCH_DBG */
  }

  /*CB Service deactivated*/
  if ( !l1_basic_cbch_info_p->cbch_supported &&
       !l1_extended_cbch_info_p->cbch_supported )
  {
    cbch_gap = MAX_CBCH_GAP_PERIOD;
  }

  return ( cbch_gap );
}

/*===========================================================================
FUNCTION: l1_skip_cbch_non_tc0_block

DESCRIPTION:
  This function determines if the next CBCH Occassion can be skipped and wait for only scheduling TC0 occasion.

DEPENDENCIES:
  None

RETURN VALUE:


SIDE EFFECTS:
  None
===========================================================================*/

boolean l1_skip_cbch_non_tc0_block(l1_smscb_cbch_info_type *cbch_info, uint32  fn, gas_id_t gas_id )
{
  boolean skip_cbch_occasion = FALSE;
  uint32  cbch_fn         = ADD_FN( fn, 1 );

  uint8   fn_div_51_mod_8 = (uint8)( ( cbch_fn / MULTIFRAME_51 ) % TIMESLOTS_IN_FRAME );

#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_HIGH_3_G( L1_CBCH_HDR"expected non_drx_tc0:%d TC:%d sched_mode:%d",
            cbch_info->non_drx_tc0, fn_div_51_mod_8, cbch_info->sched_mode );
#endif

    if (L1_SMSCB_NON_DRX_MODE == cbch_info->sched_mode)
    {
      /*
       * In NON-DRX mode we must wait for first TC0 block before allowing
       * any valid skip list with non-TCO FN periods
       */
      if ( !cbch_info->non_drx_tc0 && fn_div_51_mod_8 )
      {

        skip_cbch_occasion = TRUE;

      }
    }

    return skip_cbch_occasion;
}

/*===========================================================================
FUNCTION: cbch_gap_to_next_activity

DESCRIPTION:
  This is a local function that determines the gap in terms of TDMA frame to
  the next CBCH activity so that we can wake up to decode cbch. The gap is
  computed based on the SMSCB mode (Non-DRX/DRX), Channel Combination Type
  (SDCCH_4/SDCCH_8) and the service type (Basic/Extended CBCH).

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static uint32 cbch_gap_to_next_activity( l1_smscb_cbch_info_type *cbch_info,
                                         gas_id_t                 gas_id )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint8  cbch_fn;
  byte   cbch_tn, ccch_tn;

  uint32 gap     = MAX_CBCH_GAP_PERIOD;

  uint32 this_FN = gl1_get_FN(gas_id);
  uint32 cb_exp_fn = MAX_CBCH_GAP_PERIOD;

  if ( !cbch_info->cbch_supported )
  {
    /*
     * If cbch not enabled return a huge gap
     */
    return ( gap );
  }

  switch ( cbch_info->sched_mode )
  {
    case L1_SMSCB_INIT_MODE:
    {
      /*
       * If L1 is in INIT mode, return a huge gap
       */
    }
    break;

    case L1_SMSCB_NON_DRX_MODE:
    {
      /*
       * If we are in NON-DRX mode
       */
      if ( cbch_info->chan == SDCCH_4 )
      {
        cbch_fn = 30;
      }
      else
      {
        idle_data_T *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;
        cbch_tn                    = idle_data_ptr->campedon_cell_data.cbch_channel_information.TN;
        ccch_tn                    = (byte)( idle_data_ptr->paging_data.ccch_group << 1 );

        if ( cbch_tn <= ccch_tn )
        {
          cbch_fn = 6;
        }
        else
        {
          cbch_fn = 7;
        }
      }

      /*
       * If the skip_list is not empty, use the skip_list to calculate the gap.
       */
      if ( cbch_info->skip_fn_list_len )
      {
        gap = calc_non_drx_gap_using_skip_list( cbch_info, cbch_fn, this_FN, gas_id );
      }
      /*
       * If the skip_list is empty, calculate the gap to the next cbch block
       */
      else
      {
        gap = calc_non_drx_gap( cbch_info, cbch_fn, this_FN, gas_id );
      }
    }
    break;

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
    case L1_SMSCB_DRX_MODE:
    {
      gap = calc_drx_gap( cbch_info, this_FN, gas_id );
    }
    break;
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

    default:
    {
      MSG_GERAN_ERROR_1_G( L1_CBCH_HDR"Invalid SMSCB sched_mode %d",
                           cbch_info->sched_mode );
    }
    break;
  }

  /*if TC0 is not received yet, then skip others in non drx mode*/
  if (gap != MAX_CBCH_GAP_PERIOD)
  {
    cb_exp_fn = ADD_FN(this_FN, gap);
    if (l1_skip_cbch_non_tc0_block(cbch_info, cb_exp_fn, gas_id))
    {
      gap = MAX_CBCH_GAP_PERIOD;
    }
  }
#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_HIGH_3_G( L1_CBCH_HDR"Gap to next activity %d for sched_mode %d this_FN %d",
                      gap, cbch_info->sched_mode, this_FN );
#endif /* GL1_EXTENDED_CBCH_DBG */


  return ( gap );
}

/*===========================================================================
FUNCTION: calc_non_drx_gap_using_skip_list

DESCRIPTION:
    This function calculates the next gap for CBCH activity for non DRX mode using skip list


DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static uint32 calc_non_drx_gap_using_skip_list( l1_smscb_cbch_info_type *cbch_info,
                                                uint8                    cbch_fn,
                                                uint32                   this_FN,
                                                gas_id_t                 gas_id  )
{
	uint32 gap = MAX_CBCH_GAP_PERIOD;
	uint32 index;

	uint8  this_skip_fn_div51_mod8;

	uint32 next_cbch_fn;
	uint8  next_cbch_tc;

	uint32 num_TC_to_skip = 0;

	/*
	 1) Calculate next CBCH FN from current FN.
	 2) If Skip List elapsed then return the gap.
	 3) Else Continue to check TC of each entry in Skip Request to next Calculated TC
	 */

	/*1) Calculate next CBCH FN from current FN.*/
	gap = calc_non_drx_gap( cbch_info, cbch_fn, this_FN, gas_id );

	next_cbch_fn = ADD_FN(gap, this_FN);

	next_cbch_tc = (next_cbch_fn / MULTIFRAME_51) % TIMESLOTS_IN_FRAME;

	MSG_GERAN_LOW_3_G(L1_CBCH_HDR"this_FN %d next_cbch_fn %d, next_cbch_tc %d",this_FN,next_cbch_fn,next_cbch_tc);

	/* 2) If Skip List elapsed then return the gap.*/
	if(IS_FRAME_NUM_LATER( this_FN, cbch_info->skip_fn_list[cbch_info->skip_fn_list_len - 1]))
	{
		cbch_info->skip_fn_list_len = 0;
#ifdef GL1_EXTENDED_CBCH_DBG
		MSG_GERAN_HIGH_1_G("NON DRX skip list gap %d ",gap);
#endif
		return gap;
	}

	/*
	3)  Skip list is valid, Find the next TC to be scheduled, find TC count
	     Ex: If skip_tc_count = 1 means it has to it has to skip next TC
	*/
	for (index = 0; index < cbch_info->skip_fn_list_len; index++)
	{
		this_skip_fn_div51_mod8 = (uint8) ((cbch_info->skip_fn_list[index] / MULTIFRAME_51) % TIMESLOTS_IN_FRAME);
#ifdef GL1_EXTENDED_CBCH_DBG
		MSG_GERAN_MED_4_G(L1_CBCH_HDR"SkipIndex: %d, SkipTC = %d, NextTC %d skipcount = %d ", index,this_skip_fn_div51_mod8, next_cbch_tc,num_TC_to_skip );
#endif
		if( (next_cbch_tc + num_TC_to_skip) == this_skip_fn_div51_mod8)
		{
		  /*if TC of next CBCH matches TC in Skip List entry, then increment counter to skip TC*/
		  num_TC_to_skip++;
		}
	}

	if(num_TC_to_skip)
	{
		//based on skip TC modify the next TC FN
        next_cbch_fn = ADD_FN(next_cbch_fn, num_TC_to_skip * MULTIFRAME_51);

		gap = calc_non_drx_gap( cbch_info, cbch_fn, next_cbch_fn, gas_id );
		//final gap calculation
		gap = ADD_FN(gap, SUB_FN(next_cbch_fn,this_FN));
	}

#ifdef GL1_EXTENDED_CBCH_DBG
	MSG_GERAN_HIGH_4_G("this_FN %d, gap = %d, NextTC_schedule %d skipcount = %d ", this_FN,gap,((this_FN+gap)/MULTIFRAME_51)%TIMESLOTS_IN_FRAME ,num_TC_to_skip );
#endif

	return gap;
}
/*===========================================================================
FUNCTION: calc_non_drx_gap

DESCRIPTION:

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static uint32 calc_non_drx_gap( l1_smscb_cbch_info_type *cbch_info,
                                uint8                    cbch_fn,
                                uint32                   this_FN,
                                gas_id_t                 gas_id )
{
  uint8  fn_div_51_mod_8 = (uint8)( ( this_FN / MULTIFRAME_51 ) % TIMESLOTS_IN_FRAME );

  uint32 gap             = MAX_CBCH_GAP_PERIOD;
  uint16 this_FN_mod51;

  this_FN_mod51 = this_FN%51;
#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_MED_4_G( L1_CBCH_HDR"NON DRX gap FNmod51 %d,this_FN_mod51 %d  TC %d cbch_fn %d",
                     frame_counters[gas_id].FNmod51, this_FN_mod51, fn_div_51_mod_8, cbch_fn );
#endif

  /*
   * If are trying to find gap to the next Basic CBCH activity
   */
  if ( cbch_info->service == L1_SMSCB_BASIC_CBCH )
  {
    switch ( fn_div_51_mod_8 )
    {
      /*
       * Write now we are in the Basic CBCH Page Block
       */
      case 0:
      case 1:
      case 2:
      case 3:
      {
        if ( this_FN_mod51 < cbch_fn )
        {
          gap = ( cbch_fn - this_FN_mod51 - NUM_FRAMES_EARLY_DRX_GAP );
        }
        else
        {
          if ( fn_div_51_mod_8 == 3 )
          {
            gap = ( MULTIFRAME_51 - this_FN_mod51 + ( MULTIFRAME_51 * 4 )
                    + cbch_fn - NUM_FRAMES_EARLY_DRX_GAP );
          }
          else
          {
            gap = ( MULTIFRAME_51 - this_FN_mod51 + cbch_fn - NUM_FRAMES_EARLY_DRX_GAP );
          }
        }
      }
      break;

      /*
       * Write now we are in Extended CBCH Page block
       */
      case 4:
      case 5:
      case 6:
      case 7:
      {
        /*
         * We are trying to find gap to the next basic CBCH activity
         */
        gap = ( MULTIFRAME_51 - this_FN_mod51 + ( ( 7 - fn_div_51_mod_8 ) * MULTIFRAME_51 )
                + cbch_fn - NUM_FRAMES_EARLY_DRX_GAP );
      }
      break;

      default:
      break;
    }
  }
  else
  {
    /*
     * We are trying to find gap to the next Extended CBCH activity
     */
    switch ( fn_div_51_mod_8 )
    {
      case 0:
      case 1:
      case 2:
      case 3:
      {
        /*
         * Write now we are in the Basic CBCH Page Block and trying to find
         * gap to the next Extended CBCH activity
         */
        gap = ( MULTIFRAME_51 - this_FN_mod51 + ( ( 3 - fn_div_51_mod_8 ) * MULTIFRAME_51 )
                + cbch_fn - NUM_FRAMES_EARLY_DRX_GAP );
      }
      break;

      case 4:
      case 5:
      case 6:
      case 7:
      {
        /*
         * Write now we are in one of the Extended CBCH Page block and we are
         * trying to find gap to the next Extended CBCH activity
         */
        if ( this_FN_mod51 < cbch_fn )
        {
          gap = ( cbch_fn - this_FN_mod51 - NUM_FRAMES_EARLY_DRX_GAP );
        }
        else
        {
          if ( fn_div_51_mod_8 == 7 )
          {
            gap = ( MULTIFRAME_51 - this_FN_mod51 + ( MULTIFRAME_51 * 4 )
                    + cbch_fn - NUM_FRAMES_EARLY_DRX_GAP );
          }
          else
          {
            gap = ( MULTIFRAME_51 - this_FN_mod51 + cbch_fn - NUM_FRAMES_EARLY_DRX_GAP );
          }
        }
      }
      break;

      default:
      break;
    }
  }

#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_MED_2_G( L1_CBCH_HDR"NON DRX gap %d, this_FN %d",
                     gap,this_FN );
#endif /* GL1_EXTENDED_CBCH_DBG */

  return ( gap );
}

/*===========================================================================
FUNCTION: l1_smscb_map_cb_sched_to_l1_sched

DESCRIPTION:

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static l1_smscb_mode_type l1_smscb_map_cb_sched_to_l1_sched(
                           cb_scheduling_mode_type cb_sched_mode )
{
  l1_smscb_mode_type l1_sched_mode;

  switch ( cb_sched_mode )
  {
    case CB_DRX_MODE:
    {
      l1_sched_mode = L1_SMSCB_DRX_MODE;
    }
    break;

    case CB_NON_DRX_MODE:
    {
      l1_sched_mode = L1_SMSCB_NON_DRX_MODE;
    }
    break;

    default:
    {
      l1_sched_mode = L1_SMSCB_INIT_MODE;

      MSG_GERAN_ERROR_2( L1_CBCH_HDR"Unsupported cb_sched_mode %d l1_sched_mode %d",
                         cb_sched_mode, l1_sched_mode );
    }
    break;
  }

  return( l1_sched_mode );
}

/*===========================================================================
FUNCTION: l1_smscb_get_FN

DESCRIPTION:
  Return back the valid active FN depending on whether SYNC/ASYNC for Multi-SIM

DEPENDENCIES:
  None

RETURN VALUE:
  Current FN for SYNC SUB or adjusted FN for ASYNC

SIDE EFFECTS:
  None
===========================================================================*/
static uint32 l1_smscb_get_FN( gas_id_t gas_id )
{
  uint32 current_frame_number = gl1_get_FN( gas_id );

  return ( current_frame_number );
}

/*===========================================================================
FUNCTION: l1_smscb_use_high_priority

DESCRIPTION:
  This function returns the currently active high_priority flag

DEPENDENCIES:
  None

RETURN VALUE:
  TRUE / FALSE value of high_priority flag

SIDE EFFECTS:
  None
===========================================================================*/
boolean l1_smscb_use_high_priority( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];
  boolean                  hp_flag                 = FALSE;

  if (l1_basic_cbch_info_p && l1_extended_cbch_info_p)
  {
    /* check if high_priority flag is set for either basic or extended CBCH */
    if (l1_basic_cbch_info_p->high_priority || l1_extended_cbch_info_p->high_priority)
    {
        /*Use High prio in CB DRX Mode*/
        hp_flag = TRUE;
    }
  }

  return(hp_flag);
}

#if defined ( FEATURE_DUAL_SIM ) || defined ( FEATURE_TRIPLE_SIM )
/*===========================================================================
FUNCTION: l1_smscb_get_cbch_priority

DESCRIPTION:
  Returns the current priority setting for Cell Broadcast

DEPENDENCIES:
  Priority is set by GRR using MPH_SET_PRIORITY_IND

RETURN VALUE:
  GERAN_PRIORITY_NOMINAL or GERAN_PRIORITY_INVERTED

SIDE EFFECTS:
  None
===========================================================================*/
geran_priority_t l1_smscb_get_cbch_priority(const gas_id_t gas_id)
{
  return(l1_cbch_priority[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(gas_id)]);
}
#endif /* FEATURE_DUAL_SIM | FEATURE_TRIPLE_SIM */

/*===========================================================================
FUNCTION: l1_smscb_set_first_non_drx_block_read

DESCRIPTION:
  This function sets the TC0 align flag to TRUE once we have received the first
  valid CB block on a TC0 after a schedule req is sent in Non-DRX mode.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_set_first_non_drx_block_read( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  l1_smscb_cbch_info_type *cbch_info_p = l1_basic_cbch_info_p;

  if ( l1_smscb_decoding_extended_cbch(gas_id) )
  {
    cbch_info_p = l1_extended_cbch_info_p;
  }

#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_MED_3_G( L1_CBCH_HDR"set_first_drx_block_read basic_cb %d non_drx_tc0 %d mode %d",
                     l1_smscb_decoding_basic_cbch(gas_id), cbch_info_p->non_drx_tc0,
                     cbch_info_p->sched_mode );
#endif /* GL1_EXTENDED_CBCH_DBG */

  if ( !cbch_info_p->non_drx_tc0 &&
       ( cbch_info_p->sched_mode == L1_SMSCB_NON_DRX_MODE ) )
  {
    cbch_info_p->non_drx_tc0 = TRUE;
  }

#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_MED_1_G( L1_CBCH_HDR"set_first_drx_block_read non_drx_tc0 %d",
                     cbch_info_p->non_drx_tc0 );
#endif /* GL1_EXTENDED_CBCH_DBG */
}

/*===========================================================================
FUNCTION: l1_smscb_compare_skip_list

DESCRIPTION:
          Returns the difference in FN to detect ordering of
          the sorted skip_list in ascending FN order.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static int l1_smscb_compare_skip_list( const void *a, const void *b )
{
  return ( *(uint32*)a - *(uint32*)b );
}

#ifdef FEATURE_GSM_CB_DRX_SUPPORTED
/*===========================================================================
FUNCTION: calc_drx_gap

DESCRIPTION:

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
uint32 calc_drx_gap( l1_smscb_cbch_info_type *cbch_info,
                     uint32                   this_FN,
                     gas_id_t                 gas_id )
{
  volatile ISRTIM_CMD_BLK* l1_tskisr_blk = &l1_tsk_buffer[gas_id];
  uint32  cbch_fn, fn;
  uint8   cbch_tn, ccch_tn;

  uint32  gap  = MAX_CBCH_GAP_PERIOD;
  boolean done = FALSE;

  idle_data_T *idle_data_ptr = l1_tskisr_blk->current_params.L1Data.pIdle_data;

  if ( !idle_data_ptr )
  {
    MSG_GERAN_ERROR_1_G( L1_CBCH_HDR"Invalid NULL idle_data_ptr gap %d",
                         gap );

    return ( gap );
  }
  else
  {
    cbch_tn = idle_data_ptr->campedon_cell_data.cbch_channel_information.TN;
    ccch_tn = (uint8)( idle_data_ptr->paging_data.ccch_group << 1 );
  }

#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_MED_3_G( L1_CBCH_HDR"cbch_tn %d ccch_tn %d chan %d",
                     cbch_tn, ccch_tn, cbch_info->chan );
#endif /* GL1_EXTENDED_CBCH_DBG */

  while ( !done )
  {
    cbch_fn = cbch_info->fn_list[cbch_info->curr_fn_index];
    if (cbch_info->fn_list_len == 0)
     {
       MSG_GERAN_MED_0_G("CBCH fn_list is empty");
       break;
     }

#ifdef GL1_EXTENDED_CBCH_DBG
    MSG_GERAN_MED_3_G( L1_CBCH_HDR"cbch_fn %d curr_fn_index %d skip_fn_list_len %d",
                       cbch_fn, cbch_info->curr_fn_index,
                       cbch_info->skip_fn_list_len );
#endif /* GL1_EXTENDED_CBCH_DBG */

    /*---------------------------------------------------------------------
     * If the skip list is valid, use it to get to the next cbch fn that
     * needs to be decoded.
     *-------------------------------------------------------------------*/

   
   /*UE at this stage has Skip_fn_list (from skip req) and fn_list (DRx schedule)
        first clear out entries from skip_fn_list which are earlier than entries in fn_list*/
    if ( cbch_info->skip_fn_list_len )
    {
      /* Get the current frame to skip */
      uint32 curr_skip_fn = cbch_info->skip_fn_list[cbch_info->curr_skip_fn_index];

     while (cbch_info->skip_fn_list_len !=0 && IS_FRAME_NUM_LATER(cbch_fn, curr_skip_fn))
     {
        cbch_info->curr_skip_fn_index = cbch_info->curr_skip_fn_index+1;
#ifdef GL1_EXTENDED_CBCH_DBG
        MSG_GERAN_MED_4_G( L1_CBCH_HDR"cbch_fn_list:%d,fn_index:%d, curr_skip_fn %d curr_skip_fn_index %d",
                           cbch_fn, cbch_info->curr_fn_index, curr_skip_fn, cbch_info->curr_skip_fn_index );
#endif   
        curr_skip_fn = cbch_info->skip_fn_list[cbch_info->curr_skip_fn_index];

        if ( cbch_info->curr_skip_fn_index >= cbch_info->skip_fn_list_len )
        {
          cbch_info->curr_skip_fn_index = cbch_info->skip_fn_list_len = 0;
        }
     }
   }

    if ( cbch_info->skip_fn_list_len )
    {
      /* Get the current frame to skip */
      uint32 curr_skip_fn = cbch_info->skip_fn_list[cbch_info->curr_skip_fn_index];

#ifdef GL1_EXTENDED_CBCH_DBG
      MSG_GERAN_MED_4_G( L1_CBCH_HDR"cbch_fn_list:%d,fn_index:%d, curr_skip_fn %d curr_skip_fn_index %d",
                         cbch_fn, cbch_info->curr_fn_index, curr_skip_fn, cbch_info->curr_skip_fn_index );
#endif /* GL1_EXTENDED_CBCH_DBG */

      /*---------------------------------------------------------------------
       * Continue skipping frames in the schedule list as long as they match
       * frames in the skip list.
       *-------------------------------------------------------------------*/
      while ( IS_FRAME_NUM_LATER_OR_EQUAL(cbch_fn,curr_skip_fn) || 
              (cbch_fn == curr_skip_fn) )
      {
        /* Advance the current fn index */
        cbch_info->curr_fn_index++;

        /* If we have completed the schedule. reset the indecies and send
         * schedule expiry indication */
        if ( cbch_info->curr_fn_index >= cbch_info->fn_list_len )
        {
          /* Send expiry straight away as block not scheduled */
          l1_smscb_send_expiry_ind_and_reset( cbch_info, FALSE, gas_id );

          gap = MAX_CBCH_GAP_PERIOD;

          done = TRUE;

          break;
        }

        /* Advance the skip_fn index to point to the next frame to skip */
        if (cbch_fn == curr_skip_fn)
        {
           cbch_info->curr_skip_fn_index++;
        }

        /* Check to see if skip list has expired. */
        if ( cbch_info->curr_skip_fn_index >= cbch_info->skip_fn_list_len )
        {
          /* reset the skip_list length and index. */
          cbch_info->curr_skip_fn_index = cbch_info->skip_fn_list_len = 0;

          /* get the next frame that needs to be decoded. */
          /* For Klocwork ensure array not overrun */
          if ( cbch_info->curr_fn_index < MAX_CBCH_BLOCKS_IN_SCHED_PERIOD )
          {
             cbch_fn      = cbch_info->fn_list[cbch_info->curr_fn_index];
          }

#ifdef GL1_EXTENDED_CBCH_DBG
          MSG_GERAN_MED_3_G( L1_CBCH_HDR"Skip list expired cbch_fn %d curr_fn_index %d reset skip_index %d",
                             cbch_fn, cbch_info->curr_fn_index, cbch_info->curr_skip_fn_index );
#endif /* GL1_EXTENDED_CBCH_DBG */

          /* Break out of the inner loop if the skip list has elapsed.
           * Skip list expiration doesn't mean we are done with the schedule
           */
          break;
        }

        /* get the next frame that needs to be skipped. */
        /* For Klocwork ensure array not overrun */
        if (cbch_info->curr_skip_fn_index < MAX_CBCH_BLOCKS_IN_SKIP_REQ)
        {
           curr_skip_fn = cbch_info->skip_fn_list[cbch_info->curr_skip_fn_index];
        }
        else
        {
          break;
        }

        /* get the next frame that needs to be decoded. */
        /* For Klocwork ensure array not overrun */
        if ( cbch_info->curr_fn_index < MAX_CBCH_BLOCKS_IN_SCHED_PERIOD )
        {
           cbch_fn      = cbch_info->fn_list[cbch_info->curr_fn_index];
        }

#ifdef GL1_EXTENDED_CBCH_DBG
        MSG_GERAN_MED_3_G( L1_CBCH_HDR"cbch_fn %d curr_skip_fn_index %d curr_fn_index %d",
                           cbch_fn, cbch_info->curr_skip_fn_index,
                           cbch_info->curr_fn_index );
#endif /* GL1_EXTENDED_CBCH_DBG */
      }
    }

    /* If the schedule has expired, we are done so break from the outer loop */
    if ( done )
    {
      break;
    }

    if ( cbch_info->chan == SDCCH_4 )
    {
      fn = SUB_FN( cbch_fn, 2 );

#ifdef GL1_EXTENDED_CBCH_DBG
      MSG_GERAN_MED_2_G( L1_CBCH_HDR"SDCCH4 fn %d cbch_fn %d",
                         fn, cbch_fn );
#endif /* GL1_EXTENDED_CBCH_DBG */
    }
    else
    {
      if ( cbch_tn <= ccch_tn )
      {
        fn = SUB_FN( cbch_fn, 2 );
      }
      else
      {
        fn = SUB_FN( cbch_fn, 1 );
      }

#ifdef GL1_EXTENDED_CBCH_DBG
      MSG_GERAN_MED_3_G( L1_CBCH_HDR"SDCCH8 fn %d cbch_tn %d ccch_tn %d",
                         fn, cbch_tn, ccch_tn );
#endif /* GL1_EXTENDED_CBCH_DBG */
    }

    /*---------------------------------------------------------------------
     * If the fn is later than this_FN we will be able to schedule CBCH.
     *--------------------------------------------------------------------*/
    if ( IS_FRAME_NUM_LATER( fn, this_FN ) )
    {
      gap = ( fn - this_FN - NUM_FRAMES_EARLY_DRX_GAP );

      if ( this_FN >= fn )
      {
        gap += FRAMES_PER_HYPERFRAME;
      }

      done = TRUE;
    }
    else
    {
      /*-----------------------------------------------------------------
       * Looks like we received the schedule late and first few CBCH
       * blocks have elapsed.
       * Let us advance to the next CBCH block in the schedule
       *-------------------------------------------------------------*/
      cbch_info->curr_fn_index++;

      /*-------------------------------------------------------------
       * Check to ensure that after advancing we have not gone past
       * the entire schedule. If we did, reset the schedule list and
       * send a schedule expiry indication to CB.
       *-------------------------------------------------------------*/
      if ( cbch_info->curr_fn_index >= cbch_info->fn_list_len )
      {
        /* Send expiry straight away as block not scheduled */
        l1_smscb_send_expiry_ind_and_reset( cbch_info, FALSE, gas_id );

        gap  = MAX_CBCH_GAP_PERIOD;

        done = TRUE;
      }
    }
  }

#ifdef GL1_EXTENDED_CBCH_DBG
  MSG_GERAN_MED_1_G( L1_CBCH_HDR"calc_drx_gap %d",
                     gap );
#endif /* GL1_EXTENDED_CBCH_DBG */

  return ( gap );
}

/*===========================================================================
FUNCTION: l1_smscb_send_expiry_ind_and_reset

DESCRIPTION:
  Clear any locally active schedule info and allow for either a direct
  send of the EXPIRY_IND or defer until after the last CB block read has
  been performed.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static void l1_smscb_send_expiry_ind_and_reset( l1_smscb_cbch_info_type *cbch_info,
                                                boolean                  deferred_expiry,
                                                gas_id_t                 gas_id )
{
  MSG_GERAN_MED_1_G( L1_CBCH_HDR"Send expiry IND and reset cbch data deferred:%d",
                     deferred_expiry );

  cbch_info->curr_fn_index      = 0;
  cbch_info->fn_list_len        = 0;
  cbch_info->curr_skip_fn_index = 0;
  cbch_info->skip_fn_list_len   = 0;

  cbch_info->sched_mode         = L1_SMSCB_NON_DRX_MODE;

  /* clear the high_priority flag before going back to non-DRX mode */
  cbch_info->high_priority      = FALSE;

  cbch_info->deferred_expiry    = deferred_expiry;

  /* If not deferred then send expiry ind immediately */
  if ( !deferred_expiry )
  {
    L1_send_CB_SCHED_EXPIRY_IND(
      map_l1_service_info_to_cb_channel_ind( cbch_info->service, gas_id ), gas_id );
  }
}

/*===========================================================================
FUNCTION: l1_smscb_send_deferred_expiry_ind

DESCRIPTION:
  Send the EXPIRY_IND after the last CB block read has been performed.

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_send_deferred_expiry_ind( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  /* Align to active CB type */
  l1_smscb_cbch_info_type *cbch_info_p = l1_basic_cbch_info_p;

  if ( l1_smscb_decoding_extended_cbch( gas_id ) )
  {
    cbch_info_p = l1_extended_cbch_info_p;
  }

  /* Issue deferred EXPIRY_IND after last CB block read */
  if ( cbch_info_p->deferred_expiry )
  {
    MSG_GERAN_MED_1_G( L1_CBCH_HDR"Send deferred expiry IND FN:%d",
                       gl1_get_FN( gas_id ) );

    L1_send_CB_SCHED_EXPIRY_IND(
      map_l1_service_info_to_cb_channel_ind( cbch_info_p->service, gas_id ), gas_id );

    cbch_info_p->deferred_expiry = FALSE;
  }
}

/*===========================================================================
FUNCTION: map_l1_service_info_to_cb_channel_ind

DESCRIPTION:

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
static cb_channel_ind_type map_l1_service_info_to_cb_channel_ind(
                            l1_smscb_service_type service_info,
                            gas_id_t              gas_id )
{
  cb_channel_ind_type channel_ind;

  switch ( service_info )
  {
    case L1_SMSCB_BASIC_CBCH:
    {
      channel_ind = CBCH;
    }
    break;

    case L1_SMSCB_EXTENDED_CBCH:
    {
      channel_ind = ECBCH;
    }
    break;

    default:
    {
      channel_ind = CBCH;

      MSG_GERAN_ERROR_2_G( L1_CBCH_HDR"Unsupported service_info type %d chan_ind %d",
                           service_info, channel_ind );
    }
    break;
  }

  return( channel_ind );
}

/*===========================================================================
FUNCTION: l1_smscb_reset_schedule_info

DESCRIPTION:
  This function resets the CB DRX schedule in GL1: it is a wrapper for the
  local function process_skip_info() with the ERASE indication

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void l1_smscb_reset_schedule_info( gas_id_t gas_id )
{
  l1_smscb_cbch_info_type *l1_basic_cbch_info_p    = &l1_basic_cbch_info[gas_id];
  l1_smscb_cbch_info_type *l1_extended_cbch_info_p = &l1_extended_cbch_info[gas_id];

  /* Align to active CB type */
  l1_smscb_cbch_info_type *cbch_info_p = l1_basic_cbch_info_p;

  if ( l1_smscb_decoding_extended_cbch( gas_id ) )
  {
    cbch_info_p = l1_extended_cbch_info_p;
  }

  /* Always reset here so on reselection and select specific realign on TC0 */
  cbch_info_p->non_drx_tc0 = FALSE;

  /* if there is one, call utility function to erase the current schedule */
  if ( cbch_info_p->sched_mode == L1_SMSCB_DRX_MODE )
  {
    (void)process_skip_info( cbch_info_p,
                             NULL, 0,
                             CB_ERASE_LAST_SCHEDULING );
  }
}
#endif /* FEATURE_GSM_CB_DRX_SUPPORTED */

void l1_reset_non_drx_tc0(gas_id_t gas_id)
{
   l1_smscb_cbch_info_type *l1_basic_cbch_info_ptr    = &l1_basic_cbch_info[gas_id];
   l1_basic_cbch_info_ptr->non_drx_tc0 = FALSE; 
}


/* EOF */
