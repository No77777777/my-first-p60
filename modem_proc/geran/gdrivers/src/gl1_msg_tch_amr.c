/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

    G L 1  M E S S A G E  L A Y E R -  T R A F F I C   C H A N N E L

GENERAL DESCRIPTION
    This module contains the procedures used for communicating on the TCH.
    The procedures used herein are intended for use by the
    portable layer 1 software.

EXTERNALIZED FUNCTIONS
    gl1_msg_init_tch_amr
    gl1_msg_terminate_tch_amr_fr_hr
    gl1_msg_enable_vocoder_amr_fr_hr
    gl1_msg_disable_vocoder_amr_fr_hr

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

   EDIT HISTORY FOR MODULE

   This section contains comments describing changes made to the module.
   Notice that changes are listed in reverse chronological order.
$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_tch_amr.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who      what, where, why
--------   ---      ---------------------------------------------------------
17/08/15   pjr      CR891678 Remove references to mvs.h
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
10/04/14   pjr      CR636693 - Fix argument order when calling gl1_vs_get_mvs_mode
22/01/13   ws       CR602047 - Allow MVS/GHDI to operate on SUB2 if VS driver is not present
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
7/16/13    zc       CR470197 Replace memcpy with memscpy
21/05/13   cs       TSTS Updates
16/05/13   pjr      CR487402: Changes to improve interaction with Voice Services
07/03/13   dv       CR461080: Introduction of new GSM VS interface
16/12/11   ws       Free floating API change - provide API for NAS GSVOICE to copy vocoder
                    data from/to GL1 to MVS structures
26/04/11   ws       Removed deprecated header voc.h
02/08/10   tjw      Removed unused clk.h (CMI phase 1 re-clean up)
27/04/10   pg       CR232559 - Add missing FEATURE_MVS_UNDER_GHDI to avoid direct
                    MVS calls for AMR and sanity check gl1_vocoder_control
15/04/10   dv       Correct miss-spelling of FEATURE_GSM_AMR_WB
27/11/08   ws       Added FEATURE_MVS_UNDER_GHDI to remove all direct MVS calls.
02/10/08   cs       Lint fixes and AMR_WB loopback I
12/05/08   dv       Added FEATURE_GSM_AMR_WB - L1 component
11/07/05   gfr      QLint
07/29/05   gfr      DTM support
05/03/05   pp       Changes to support 14.10 test case
04/08/05   gfr      Combined GSM and GPRS channel types
03/29/05   dp       Inter-RAT audio optimization
01/05/05   dp       Merged audio optimizations to mainline
12/07/04   yhong    Lint cleanup
08/18/04   yhong    Split file into uplink and downlink
08/12/04   gsc      Added support for loopback I.
06/15/04   gfr      Added look_for_facch flag to rx.
06/25/04   gsc      Updated to include new BFI algorithm. Use computed BFI
                    in the loopbacks only.
06/02/04   gfr      Lint cleanup for Lint 7.50ad.
03/18/04   gsc      Lint Cleanup.
03/12/04   dp       Undid handover blanking changes
02/24/04   gsc      Vocoder blanking during handovers on the RX upon request
                    from RR.
02/18/04   gsc      Get sync rx data one frame earlier compared to before
                    for AMR Full Rate. Also, metrics and data callbacks
                    into PL1 happens one frame earlier.
01/23/04   gsc      Featurized call to mvs_enable under FEATURE_MVS.
01/20/04   dp       Fix to only do a mvs_enable across GSM handovers when
                    the mode changes (reduces handover mutes )
12/02/03   gsc      Minor loopback fixes to use CRC instead of BFI.
12/02/03   gsc      Fixed a bug in call to mvs_enable
11/24/03   gsc      Fix to only enable mvs (as opposed to disabling and doing
                    a complete reinit) across handovers if channel mode doesn't
                    change
11/05/03   gfr      Support for quad-band.
10/22/03   gsc      Added support for controlling enabling/disabling of
                    vocoder from Layer 1.
10/17/03   gsc      Abort RATSCCH/SID_FIRST/SID_UPDATE right away without
                    waiting for complete transmission. Fix for DTX indicator
                    in the measurement reports.
10/01/03   drl      Renamed loopback_data to avoid naming collision with 1x
09/04/03   gsc      Added loopback A&B support.
08/11/03   gsc      Fixes based on Nortel IOT.
07/25/03   gfr      Removed the channel filter delay timing error adjustment.
07/14/03   gsc      Fixes based on Ericsson IOT.
07/01/03   gsc      Minor updates to gl1_msg_vocoder_rx_amr_hr and
                    gl1_msg_vocoder_tx_amr_hr. Interface change for
                    gl1_msg_tx_tch_amr_fr and  gl1_msg_tx_tch_amr_hr
05/12/03   gsc      Changed amr_codec_mode_type to mdsp_amr_codec_mode_type
05/07/03   gsc      First revision. Used gl1_msg_tch.c (67) as the base line

===========================================================================*/


/*===========================================================================

    INCLUDE FILES FOR MODULE

===========================================================================*/

#include "geran_variation.h"
#include "comdef.h"

#ifdef CUST_H
#include "customer.h"
#endif

#if  defined (FEATURE_GSM_AMR)

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_msg_g.h"
#include "gl1_msgi.h"
#include "msg.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

#include "sys_cnst.h"
#include "gl1_msgi_tch_amr.h"

#include <string.h>
#include <stringl/stringl.h>
#include "event.h"

/*****************************************************

 LOCAL STORAGE

******************************************************/
#define AMR_MAX_LOOPBACK_OCTETS 38

extern gl1_msg_voc_type       current_voc[NUM_GERAN_DATA_SPACES];

boolean amr_loopback_sid_update_saved[NUM_GERAN_DATA_SPACES]           =
                     { INITIAL_VALUE( FALSE ) };


boolean gl1_msg_loopback_I_prev_block_was_facch[NUM_GERAN_DATA_SPACES] =
                     { INITIAL_VALUE( FALSE ) };

extern gl1_defs_loopback_type loopback_type[NUM_GERAN_DATA_SPACES];

extern uint8                  gl1_loopback_data[NUM_GERAN_DATA_SPACES][MAX_LOOPBACK_OCTETS];

static uint16                 amr_loopback_sid_update_bfi[NUM_GERAN_DATA_SPACES] =
                                { INITIAL_VALUE( 0 ) };

static uint8                  amr_loopback_sid_update_buf[NUM_GERAN_DATA_SPACES][MAX_SID_UPDATE_OCTETS - 4];

/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/

static void amr_handle_loopback_A_fr (tch_facch_decode_rpt *rpt,gas_id_t gas_id );

static void amr_handle_loopback_B_fr (tch_facch_decode_rpt *rpt,gas_id_t gas_id );

static void amr_handle_loopback_I_fr_hr (tch_facch_decode_rpt *rpt ,gas_id_t gas_id);

static void amr_handle_loopback_A_hr (tch_facch_decode_rpt *rpt,gas_id_t gas_id );

static void amr_handle_loopback_B_hr (tch_facch_decode_rpt *rpt,gas_id_t gas_id );

/*===========================================================================
FUNCTION  gl1_msg_init_tch_amr

DESCRIPTION
   This function should be called to perform any initialization that need to
   be done when entering the traffic channel from idle, or rach states.
   The search window size is modified as is the burst size.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_init_tch_amr (gl1_msg_chan_type chan, gl1_msg_voc_type voc_type,gas_id_t gas_id)
{
   gl1_hw_channel_type hw_chan;

   switch (chan)
   {
      case GL1_MSG_TCH_AFS:
         hw_chan = GL1_HW_TCH_AFS;
         break;

#ifdef FEATURE_GSM_AMR_WB
      case GL1_MSG_TCH_WFS:
         hw_chan = GL1_HW_TCH_WFS;
         break;
#endif

      case GL1_MSG_TCH_AHS:
         hw_chan = GL1_HW_TCH_AHS;
         break;

      default:
         MSG_GERAN_ERROR_1_G("Invalid channel type %d",chan);
         hw_chan = GL1_HW_TCH_AFS;
         break;
   }

   current_voc[gas_id] = voc_type;

   gl1_hw_init_tch( hw_chan,gas_id );
}

/*===========================================================================
FUNCTION  gl1_msg_terminate_tch_amr_fr_hr

DESCRIPTION
   This function should be called to perform any actions that need to
   be done when exiting the traffic channel.

DEPENDENCIES
   None

RETURN VALUE
   None

SIDE EFFECTS
   None
===========================================================================*/
void gl1_msg_terminate_tch_amr_fr_hr (gas_id_t gas_id )
{
   gl1_hw_terminate_tch(gas_id);
   /* clear DTX_used flag for next report */
   gl1_reset_dtx_used_flag(gas_id);
}

/****************************************************************************
 ****************************************************************************
 ****************************************************************************
   LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/
/*===========================================================================
FUNCTION amr_handle_loopback_fr

DESCRIPTION
   This function handles loopbacks for AMR FR

DEPENDENCIES
   None

RETURN VALUE

SIDE EFFECTS
   None
===========================================================================*/
void amr_handle_loopback_fr (tch_facch_decode_rpt *rpt,gas_id_t gas_id)
{
   /* Only loopback A & B implemented in s/w. C still handled my mdsp */
   if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B )
   {
      amr_handle_loopback_B_fr( rpt,gas_id );
   }
   else if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A )
   {
      amr_handle_loopback_A_fr( rpt,gas_id );
   }
   else if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_I )
   {
      amr_handle_loopback_I_fr_hr( rpt,gas_id );
   }
}

/*===========================================================================
FUNCTION amr_handle_loopback_A_fr

DESCRIPTION
   This function handles loopback type A for AMR FR

DEPENDENCIES
   None

RETURN VALUE

SIDE EFFECTS
   None
===========================================================================*/
static void amr_handle_loopback_A_fr (tch_facch_decode_rpt *rpt,gas_id_t gas_id)
{
  uint16 octets_to_copy = MAX_TCH_OCTETS_AMR;

  if (!rpt)
  {
     MSG_GERAN_ERROR_0_G("Rcvd NULL rpt ptr");
     return;
  }

  switch (rpt->hdr.chan)
  {
    case GL1_MSG_FACCH_AFS:
#ifdef FEATURE_GSM_AMR_WB
    case GL1_MSG_FACCH_WFS:
#endif
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS );
      break;

#ifdef FEATURE_GSM_AMR_WB
    case GL1_MSG_TCH_WFS:
      octets_to_copy = MAX_TCH_OCTETS_AMR_WB;
      /* Fall through */ /*lint !e616*/
#endif
    case GL1_MSG_TCH_AFS:
      if ( rpt->hdr.bfi )
      {
        memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
      }
      else
      {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000)  /*lint !e553*/
        uint16 i;

        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        for (i=0; i<octets_to_copy-4; i++)
        {
          gl1_loopback_data[gas_id][i+4] = rpt->data[i];
        }
#else
        memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
               sizeof(gl1_loopback_data[gas_id]) - 4,
               (uint8 *) rpt->data,
               octets_to_copy-4);
#endif
      }
      break;

    case GL1_MSG_SID_UPDATE_FS:
    case GL1_MSG_RATSCCH_FS:
#ifdef FEATURE_GSM_AMR_WB
    case GL1_MSG_RATSCCH_WFS:
#endif
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
      if ( !rpt->hdr.bfi)
      {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
        uint16 i;

        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        for (i=0; i<MAX_SID_UPDATE_OCTETS-4; i++)
        {
          gl1_loopback_data[gas_id][i+4] = rpt->data[i];
        }
#else
        memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
               sizeof(gl1_loopback_data[gas_id]) - 4,
               (uint8 *) rpt->data,
               MAX_SID_UPDATE_OCTETS-4);
#endif
      }
      break;

    default:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
      break;
  }
}


/*===========================================================================
FUNCTION amr_handle_loopback_B_fr

DESCRIPTION
   This function handles loopback type B for AMR FR

DEPENDENCIES
   None

RETURN VALUE

SIDE EFFECTS
   None
===========================================================================*/
static void amr_handle_loopback_B_fr (tch_facch_decode_rpt *rpt,gas_id_t gas_id)
{
  uint16 octets_to_copy = MAX_TCH_OCTETS_AMR;

  if (!rpt)
  {
     MSG_GERAN_ERROR_0_G("Rcvd NULL rpt ptr");
     return;
  }

  switch (rpt->hdr.chan)
  {
#ifdef FEATURE_GSM_AMR_WB
    case GL1_MSG_TCH_WFS:
    case GL1_MSG_FACCH_WFS:
      octets_to_copy = MAX_TCH_OCTETS_AMR_WB;
      /* Fall through */ /*lint !e616*/
#endif
    case GL1_MSG_TCH_AFS:
    case GL1_MSG_FACCH_AFS:
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
      {
        uint16 i;

        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        for (i=0; i<octets_to_copy-4; i++)
        {
          gl1_loopback_data[gas_id][i+4] = rpt->data[i];
        }
      }
#else
      memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
			 sizeof(gl1_loopback_data[gas_id]) - 4,
             (uint8 *) rpt->data,
             octets_to_copy-4);
#endif
      break;

    case GL1_MSG_SID_UPDATE_FS:
    case GL1_MSG_RATSCCH_FS:
#ifdef FEATURE_GSM_AMR_WB
    case GL1_MSG_RATSCCH_WFS:
#endif
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
      {
        uint16 i;

        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        for (i=0; i<MAX_SID_UPDATE_OCTETS-4; i++)
        {
          gl1_loopback_data[gas_id][i+4] = rpt->data[i];
        }
      }
#else
      memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
      		 sizeof(gl1_loopback_data[gas_id]) - 4,
             (uint8 *) rpt->data,
             MAX_SID_UPDATE_OCTETS-4);
#endif
      break;

    default:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
      break;
  }
}

/*===========================================================================
FUNCTION amr_handle_loopback_I_fr_hr

DESCRIPTION
   This function handles loopback type I for AMR FR and HR

DEPENDENCIES
   None

RETURN VALUE

SIDE EFFECTS
   None
===========================================================================*/
static void amr_handle_loopback_I_fr_hr (tch_facch_decode_rpt *rpt,gas_id_t gas_id)
{
  uint16 octets_to_copy = MAX_TCH_OCTETS_AMR;

  if (!rpt)
  {
     MSG_GERAN_ERROR_0_G("Rcvd NULL rpt ptr");
     return;
  }

  gl1_msg_loopback_I_prev_block_was_facch[gas_id] = FALSE;

  switch (rpt->hdr.chan)
  {
     case GL1_MSG_FACCH_AFS:
     case GL1_MSG_FACCH_AHS:
#ifdef FEATURE_GSM_AMR_WB
     case GL1_MSG_FACCH_WFS:
#endif
        memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
        gl1_msg_loopback_I_prev_block_was_facch[gas_id] = TRUE;
        break;

#ifdef FEATURE_GSM_AMR_WB
     case GL1_MSG_TCH_WFS:
        octets_to_copy = MAX_TCH_OCTETS_AMR_WB;
        /* Fall through */ /*lint !e616*/
#endif
     case GL1_MSG_TCH_AFS:
     case GL1_MSG_TCH_AHS:
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
      {
        uint16 i;

         /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        for (i=0; i<(octets_to_copy-4); i++)
        {
          gl1_loopback_data[gas_id][i+4] = 0xAA;
        }
      }
#else
      memset((uint8 *) (gl1_loopback_data[gas_id]+4), 0xAA, (octets_to_copy-4));
#endif
      break;

     default:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
      break;
  }
}




/*===========================================================================
FUNCTION amr_handle_loopback_hr

DESCRIPTION
   This function handles loopbacks for AMR HR

DEPENDENCIES
   None

RETURN VALUE

SIDE EFFECTS
   None
===========================================================================*/
void amr_handle_loopback_hr (tch_facch_decode_rpt *rpt,gas_id_t gas_id)
{
   /* Only loopback A & B implemented in s/w. C still handled my mdsp */
   if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_B )
   {
      amr_handle_loopback_B_hr( rpt,gas_id );
   }
   else if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_A )
   {
      amr_handle_loopback_A_hr( rpt,gas_id );
   }
   else if ( loopback_type[gas_id] == GL1_DEFS_LOOPBACK_TYPE_I )
   {
      amr_handle_loopback_I_fr_hr( rpt,gas_id );
   }
}


/*===========================================================================
FUNCTION amr_handle_loopback_A_hr

DESCRIPTION
   This function handles loopback A for AMR HR

DEPENDENCIES
   None

RETURN VALUE

SIDE EFFECTS
   None
===========================================================================*/
static void amr_handle_loopback_A_hr (tch_facch_decode_rpt *rpt,gas_id_t gas_id)
{
   if (!rpt)
   {
      MSG_GERAN_ERROR_0_G("Rcvd NULL rpt ptr");
      return;
   }

  /*
   * Just loopback the saved sid_update payload.
   */
  if ( amr_loopback_sid_update_saved[gas_id] == TRUE )
  {
    amr_loopback_sid_update_saved[gas_id] = FALSE;
    memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
    if ( !amr_loopback_sid_update_bfi[gas_id] )
    {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
      uint16 i;
      /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
      for (i=0; i<MAX_SID_UPDATE_OCTETS-4; i++)
      {
        gl1_loopback_data[gas_id][i+4] = amr_loopback_sid_update_buf[gas_id][i];
      }
#else
      memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
			 sizeof(gl1_loopback_data[gas_id]) - 4,
             (uint8 *) amr_loopback_sid_update_buf[gas_id],
             MAX_SID_UPDATE_OCTETS-4);
#endif
    }
    return;
  }

  switch (rpt->hdr.chan)
  {
    case GL1_MSG_FACCH_AHS:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS );
      break;

    case GL1_MSG_TCH_AHS:
      if ( rpt->hdr.bfi )
      {
        memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
      }
      else
      {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
        uint16 i;
        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        for (i=0; i<MAX_TCH_OCTETS_AMR-4; i++)
        {
          gl1_loopback_data[gas_id][i+4] = rpt->data[i];
        }
#else
        memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
               sizeof(gl1_loopback_data[gas_id]) - 4,
               (uint8 *) rpt->data,
               MAX_TCH_OCTETS_AMR-4);
#endif
      }
      break;

    case GL1_MSG_SID_UPDATE_HS:
      {
        memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
        amr_loopback_sid_update_saved[gas_id] = TRUE;
        amr_loopback_sid_update_bfi[gas_id] = rpt->hdr.bfi;
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
        {
          uint16 i;
          /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
          for (i=0; i<MAX_SID_UPDATE_OCTETS-4; i++)
          {
            amr_loopback_sid_update_buf[gas_id][i] = rpt->data[i];
          }
        }
#else
        memscpy((uint8 *)amr_loopback_sid_update_buf[gas_id], sizeof(amr_loopback_sid_update_buf[gas_id]), rpt->data,
               MAX_SID_UPDATE_OCTETS - 4 );
#endif
        break;
      }

    case GL1_MSG_RATSCCH_MARKER:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS );
      break;


    case GL1_MSG_RATSCCH_HS:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
      if ( !rpt->hdr.bfi)
      {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
        uint16 i;
        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        for (i=0; i<MAX_RATSCCH_OCTETS-4; i++)
        {
          gl1_loopback_data[gas_id][i+4] = rpt->data[i];
        }
#else
        memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
               sizeof(gl1_loopback_data[gas_id])-4,
               (uint8 *) rpt->data,
               MAX_RATSCCH_OCTETS-4);
#endif
      }
      break;

    default:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS );
      break;
  }
}


/*===========================================================================
FUNCTION amr_handle_loopback_B_hr

DESCRIPTION
   This function handles loopback B for AMR HR

DEPENDENCIES
   None

RETURN VALUE

SIDE EFFECTS
   None
===========================================================================*/
static void amr_handle_loopback_B_hr (tch_facch_decode_rpt *rpt,gas_id_t gas_id)
{
  if (!rpt)
  {
     MSG_GERAN_ERROR_0_G("Rcvd NULL rpt ptr");
     return;
  }

  /*
   * Just loopback the saved sid_update payload.
   */
  if ( amr_loopback_sid_update_saved[gas_id] == TRUE )
  {
    amr_loopback_sid_update_saved[gas_id] = FALSE;
    memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
    {
      uint16 i;
      /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
      for (i=0; i<MAX_SID_UPDATE_OCTETS-4; i++)
      {
        gl1_loopback_data[gas_id][i+4] = amr_loopback_sid_update_buf[gas_id][i];
      }
    }
#else
    memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
           sizeof(gl1_loopback_data[gas_id]) - 4,
           (uint8 *) amr_loopback_sid_update_buf[gas_id],
           MAX_SID_UPDATE_OCTETS-4);
#endif
    return;
  }

  switch (rpt->hdr.chan)
  {
    case GL1_MSG_FACCH_AHS:
    case GL1_MSG_TCH_AHS:
      {
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
        uint16 i;
        /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
        for (i=0; i<MAX_TCH_OCTETS_AMR-4; i++)
        {
          gl1_loopback_data[gas_id][i+4] = rpt->data[i];
        }
#else
        memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
               sizeof(gl1_loopback_data[gas_id])-4,
               (uint8 *) rpt->data,
               MAX_TCH_OCTETS_AMR-4);
#endif
        break;
      }

    case GL1_MSG_SID_UPDATE_HS:
      {
        memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
        amr_loopback_sid_update_saved[gas_id] = TRUE;
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
        {
          uint16 i;
          /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
          for (i=0; i<MAX_SID_UPDATE_OCTETS-4; i++)
          {
            amr_loopback_sid_update_buf[gas_id][i] = rpt->data[i];
          }
        }
#else
        memscpy((uint8 *)amr_loopback_sid_update_buf[gas_id],
                sizeof(amr_loopback_sid_update_buf[gas_id]),
			   rpt->data,
               MAX_SID_UPDATE_OCTETS - 4 );
#endif
        break;
      }

    case GL1_MSG_RATSCCH_MARKER:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS );
      break;

    case GL1_MSG_RATSCCH_HS:
      {
        memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS);
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 120000) /*lint !e553*/
        {
          uint16 i;
          /* ADS1.2 compiler issue - memcpy on non-aliged unions of structures */
          for (i=0; i<MAX_RATSCCH_OCTETS-4; i++)
          {
            gl1_loopback_data[gas_id][i+4] = rpt->data[i];
          }
        }
#else
        memscpy((uint8 *) gl1_loopback_data[gas_id]+4,
               sizeof(gl1_loopback_data[gas_id])-4,
               (uint8 *) rpt->data,
               MAX_RATSCCH_OCTETS-4);
#endif
        break;
      }

    default:
      memset((uint8 *) gl1_loopback_data[gas_id], NULL, AMR_MAX_LOOPBACK_OCTETS );
      break;
  }
}



#endif /* FEATURE_GSM && FEATURE_GSM_AMR */

