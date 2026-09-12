/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

         G L 1  M E S S A G E  L A Y E R -  T R A F F I C   C H A N N E L

GENERAL DESCRIPTION
   This module contains the procedures used for communicating on the TCH.
   The procedures used herein are intended for use by the
   portable layer 1 software.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_tch.c#2 $
$DateTime: 2019/12/30 04:21:22 $ $Author: pwbldsvc $

when       who      what, where, why
--------   ---      ----------------------------------------------------------
24/12/19    sc      CR2592509 FR47558/FR47064 Clock Plan 2.0 Interface changes to pass scenario info to mcpm
05/11/15   sp       CR933554 TCH Loopbacks clock bump to 384MHz
08/12/16   bala     CR1092776 Cryptographic Key Material Leaked in Debug Messages - GERAN
27/05/15   msa      CR844425 Incorporate new diag services API for logging events with subscription id
11/05/15   pjr      CR828213 Deprecate files geran_ghdi_api.h, gl1_msg.h, gl1_hw_vs.c and gl1_hw_vs_g.h
08/04/15   pjr      CR818535 FR25951 Changes to support Single Voice Session 
13/03/15   pjr      CR797534 Ensure DTX mode is not enabled by an Update req during loopback
11/03/15   pjr      CR806250 Fix featurisation for changes submitted under CR787199
24/02/15   pjr      CR787199 Acquire vocoder immediately after IMS releases it during srvcc handover
10/04/14   pjr      CR636693 - Fix argument order when calling gl1_vs_get_mvs_mode
23/01/13   ws       CR602047 - Further fixes for audio API's
22/01/13   ws       CR602047 - Allow MVS/GHDI to operate on SUB2 if VS driver is not present
18/11/13   pjr      CR563429 Fixed vocoder handover on sub1 while sub 2 was in a call
11/11/13   ws       CR 573757 Featurise GSMTR-1 registers and Voice services driver
21/10/13   pjr      CR564298 added function gl1_msg_is_l1_vocoder_control_enabled_multisim
28/08/13   dv       CR534855 TDS Audio mute
7/16/13    zc       CR470197 Replace memcpy with memscpy
20/05/13   cs       TSTS Updates
16/05/13   pjr      CR487402: Changes to improve interaction with Voice Services
03/04/13   pjr      CR470940 included gl1_hw_vs_g.h
07/03/13   dv       CR461080: Introduction of new GSM VS interface
28/11/12   br       CR383165 Include gl1_mutex.h to avoid compilation warnings
21/06/12   cja      CR372395 Clear cipher enabled flag on panic reset
06/03/12   pg       CR319120: Ensure latest DTX setting gets applied after vocoder init
24/02/12   pg       CR338211: Modification to CR331722 to account for variable MVS
                    execution timings
14/02/12   pg       CR313824: Init Rx vocoder packet as bad frame on vocoder init
19/01/12   pg       CR331722 Ensure GL1 maintains vocoder control throughout HO
16/12/11   ws       Free floating API change - provide API for NAS GSVOICE to copy vocoder
                    data from/to GL1 to MVS structures
26/04/11   ws       Removed deprecated header voc.h
04/01/11   dv       Featurise fix for CR265055
03/12/10   dv       CR265055 NAS/GL1 get out of sync due to codec change due to
                    HO occurring before call connect
02/08/10   tjw      Removed clk_busy_wait that was #if 0 prior to 2007 (clk.h clean up)
12/05/09   cja      Fix compiler warnings.
30/03/09   ws       Added FEATURE_GSM_GPRS_QDSP6 and FEATURE_GSM_GPRS_POSEIDON
                    to replace FEATURE_GSM_GPRS_MUSTANG
28/11/08   ws       Corrected Indentation
27/11/08   ws       Added FEATURE_MVS_UNDER_GHDI to remove all direct MVS calls.
26/08/08   og       Mustang GPLT changes.
06/11/07   ip       CR115620 Removed use of FEATURE_INTER_RAT_HO_OPT around
                    gl1_msg_disable_vocoder(), required for Worldphone targets
08/07/07   ws       CR - 124046 Open loopback mode C when we need to send FACCH
09/10/05   dp       Fix to disable L1 voc control when NAS releases vocoder
05/03/05   pp       Changes to support 14.10 test case
04/28/05   gfr      Wrap vocoder_acquired by FEATURE_INTER_RAT_HO_OPT
04/12/05   dp       Fix to invoke ghdi_mvs_release instead of mvs_release
04/08/05   gfr      Combined GSM and GPRS channel types
03/29/05   dp       Inter-RAT audio optimization
01/05/05   dp       Merged audio optimizations to mainline
08/18/04   yhong    Split file into uplink and downlink.
08/12/04   gsc      Added support for loopback I.
06/15/04   gfr      Added early decode and look_for_facch flag to rx.
06/02/04   gfr      Lint cleanup for Lint 7.50ad.
04/29/04   gfr      Lint cleanup.
03/31/04   dp       Fix to pass in flag to indicate if FACCH available in CSD
03/31/04   dp       Fix to factor hard RXQUAL into BFI computation
03/26/04   gsc      Fixes for Loopback E and F. Set sid equal to 0 for FACCH
                    frames. We don't get num_c1/num_c1c2 from the DSP for FACCHs
03/18/04   gsc      Lint Clean up. Only one error remains in CS Data Loopback.
03/12/04   dp       Undid handover blanking changes
03/11/04   gsc      Fix for loopbacks in half rate specific to ADS1.2 compiler
                    where the loopback data was not getting copied correctly.
02/18/04   gsc      Added APIs for RR to enable/disable vocoder blanking across
                    handovers.
01/20/04   dp       Fix to only do a mvs_enable across GSM handovers when
                    the mode changes (reduces handover mutes )
11/11/03   dp       Fix to only enable mvs (as opposed to disabling and doing
                    a complete reinit) across handovers if channel mode doesn't
                    change
11/05/03   gfr      Support for quad-band.
10/03/03   dp       Added functions to support vocoder control by
                    L1
10/01/03   drl      Renamed loopback_data to avoid naming collision with 1x
09/04/03   gsc      Removed static definition for loopback_type and loopback_data
                    so that they can be shared by AMR loopback code.
08/11/03   gsc      Removed the channel filter delay timing error adjustment
                    from hr rx handler.
08/11/03   gsc      Set ghdi dtx mode off or on while disabling/enabling DTX.
07/25/03   gfr      Removed the channel filter delay timing error adjustment.
05/12/03   gsc      Made gl1_dtx_used and gl1_voice_blanking non-statc as
                    they are used by gl1_msg_tch_amr.c
04/25/03   ATM      Fixed dtx_used logic per code review
04/24/03   dp       Fixed loopback bug in half rate
04/14/03   dp       Added gl1_msg_vocoder_released and
                    gl1_msg_is_vocoder_initialized.  Removed vocoder init from
                    L1.   Now handled by the NAS.
04/14/03   ATM      Added dtx_used logic: CR28812
04/11/03   jc       Use FEATURE_MVS instead of GPLT or WPLT flags.
03/26/03   gsc      Added compensatation for STAGE2 channel filter delay for
                    half rate.
03/21/03   gsc      Added BFI/UFI algorithm for Half Rate.
03/18/03   ATM      ADS1.2 compiler problem still exists - applied workaround
                    to both VOICE/DATA until solution can be found.
03/16/03   dp       Retrieve MAX_TCH_FS_OCTETS even for half rate.  This
                    ensures that we retrieve all FACCH octets even for half
                    rate
03/06/03   bbd      Cleaned up usage of PLT feature flags
02/26/03   dp       Added half rate support featurized under
                    FEATURE_GSM_HALF_RATE
02/04/03   jc/drl   Changes for 6300, ADS1.2 compiler problem. Temporary til a
                    more correct solution can be found.
01/23/03   djm      WCDMA modifications to build, MVS done with FEATURE_MVS wrap
01/13/03   bbd      Wrapped MVS changes with #ifndef FEATURE_GSM_PLT
01/13/03   dp       Adjusted timing error to account for new channel filter
                    delay
01/13/03   mk       Added support for ARM based (was in MDSP) BFI algorithm
                    (using crc, soft rx qual and snr).
01/09/03   dp       Put back tch_gating functions temporarily for 11.10
12/09/02   ATM      Mainline MVS and enable blanking.
11/20/02   dp       Modified cstch_rx_handler to always get the decoded data
                    first before scheduling a burst.  This prevents the "len"
                    semaphore from being overwritten and fixes the problem
                    with us dropping CSD FACCH blocks
11/12/02   ATM      Enabled MVS blanking.
10/17/02   ATM      Bugfix: added ymc fix to disable AMR during BHO
10/01/02   ATM      Added MVS blanking support.
10/10/02   dp       Added MAX_LOOPBACK_OCTETS to remove CSD dependencies
10/09/02   dp       Undid changes to delay log callback (consequence of
                    reduction in mdsp debug buffer size )
10/07/02   dp       Implemented TCH loopback A & B in S/W.  Featurized using
                    FEATURE_SW_LOOPBACK and enabled by default
10/02/02   ATM      Bugfix: mvssup_gsm_set_dtx_mode() returns void
09/25/02   ATM      Added MVS support from ymc.
09/23/02   dp       Removed 0.5 sec delay before enabling loopback C
09/09/02   ATM      Added temporary fix to switch from AMR to EFR/FR (for interRAT HO)
08/29/02   JC       Obsoleted call to gl1_msg_power_to_dBm(), cstch_rx_handler()
                    Also, changed TX() functions to accept TXLEV instead of
                    power index to support multi-band operation and because
                    of better abstraction.
08/15/02   bd       Delaying log callback for GPRS
08/13/02   dp       Resetting the TCH rx/tx fifos when aborting the TCH to
                    prevent frequency mismatches when setting up the next call
07/11/02   gw       Changed references to power_dBm_x16 in metrics reports to
                    pwr_dBm_x16.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
06/26/02   tb       Updated abort code to include CS Data.
06/14/02   dp       Changed tx aborts for TCH, FACCH to return
                    boolean when abort is complete.  i.e we complete txing
                    the block before returning
06/06/02   mk       Support for aborts on all channels.
06/04/02   bbd      Changed FEATURE_GPLT to FEATURE_GSM_PLT
05/17/02   dp       Changed gl1_msg_vocoder_rx interface to pass in and use
                    TAF flag
05/08/02   JC       Fixed issue with burst metrics when rx_state = {2,6}
                    condition.  Now reporting metrics in offset 5 so that
                    the metrics of the other machine don't overwrite the
                    current machine metrics, since L1 dowsn't allocate
                    buffers for each machine.
05/06/02   JC       Cleaned up ARFCN burst metric hack.  Original code was
                    working.
04/30/02   JC       Added ARFCN info to CCCH burst metrics.  Not the cleanest
                    approach, needs further review.
04/19/02   JC       Added rx_gain_range info in metrics callbacks to enhance
                    logging capabilities.
04/16/02   dp       Put back vocoder initialization in L1.  This is a temp fix
                    to solve the channel assignment failures in gm1009
03/29/02   dp       Merged BFI changes
03/20/02   dp       Changed txlevs to tx power index
03/18/02   jc       Add support for FEATURE_TCH_FIXED_PATTERN_TEST
02/13/02   jc       Add changes for DTX support and ciphering support
02/05/02   JC       Added support for AGC; Also, call to
                    gl1_hw_get_syncrx_metrics in rx_handler to unwind HW
                    layer.
02/01/02   mk       Added snr to metrics data.
12/12/01   jc       Add call to gl1_hw_terminate_tch(). Update the ciphering
                    functions but do not enable them in the handlers.
12/12/01   gw       Changed the way the debug (logging) callback works.
11/19/01   jc       Added support for voice on full stack on KOVU. Added tch
                    loopback support.
11/6/01    bk       Changed NPL1 function calls to reflect change in interface
10/31/01   gw/dp    Added support for logging debug info.  Added support for
                    returning metrics. Made changes to pass in the required number
                    of data octets when retrieving TCH decoded data.
10/31/01   dp       Made changes to use new gl1_hw_get_syncrx_data interface
                    to eliminate multiple copies when retrieving DSP results
08/31/01   jc       Initial version.
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
#include "gl1_msgi.h"
#include "msg.h"
#include "gl1_mutex.h"

#ifdef FEATURE_DATA_GCSD
#include "l1_ds.h"
#include "l1_ds_v.h"
#include "l1_ds_v_g.h"
#endif /* FEATURE_DATA_GCSD */

#include "l1i.h"
#include "sys_cnst.h"
#include "gl1_msgi_tch.h"
#include "event.h"

#include <string.h>
#include "gl1_mutex.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "gsmdiag.h"
#include <stringl/stringl.h>

#include "gl1_voice_server.h"
#include "geran_nv.h"

/*****************************************************

 LOCAL STORAGE

******************************************************/
static l1_vs_dtx_mode_t prior_dtx_mode [NUM_GERAN_DATA_SPACES] =
                           { INITIAL_VALUE( DTX_DISABLED ) };

/* Current state of ciphering */
static boolean  gl1_msg_ciphering_enabled[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( FALSE ) };

gl1_defs_loopback_type loopback_type[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( GL1_DEFS_NO_LOOPBACK ) };

gl1_hw_channel_type current_chan[NUM_GERAN_DATA_SPACES] =
                  { INITIAL_VALUE( GL1_HW_TCH_FS ) };

gl1_msg_voc_type current_voc[NUM_GERAN_DATA_SPACES];

#if defined (FEATURE_DUAL_SIM) && !defined (FEATURE_GSM_VS_DRIVER)
static gas_id_t dedicated_gas_id = GERAN_ACCESS_STRATUM_ID_1;
#endif
/*****************************************************

 LOCAL FUNCTION PROTOTYPES

******************************************************/


static void gl1_msg_log_align_vfr_event( boolean real_vfr,gas_id_t gas_id );

#if defined (FEATURE_DUAL_SIM) && !defined (FEATURE_GSM_VS_DRIVER)
/*===========================================================================

FUNCTION  gl1_msg_set_dedicated_gas_id

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void gl1_msg_set_dedicated_gas_id(gas_id_t gas_id)
{
  dedicated_gas_id = gas_id;
  MSG_GERAN_HIGH_0_G(" GL1_DED_GAS_ID");
}
#endif
/*===========================================================================

FUNCTION  gl1_msg_enable_ciphering,   gl1_msg_disable_ciphering

DESCRIPTION
  This function enables/disables ciphering on both the uplink and downlink
  by writing to the GSAC_CONFIG_x_RW registers and then telling the mdsp
  to turn on/off ciphering.

  key - pointer to cipher key

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void gl1_msg_enable_ciphering ( uint8              *key,
                                       gl1_defs_cipher_mode mode,
                                       gas_id_t gas_id)
{
  gl1_hw_enable_ciphering ( key, mode,gas_id );
  gl1_msg_ciphering_enabled[gas_id] = TRUE;

  if (TRUE == geran_ciphering_logging_enabled(gas_id))
  {
    MSG_GERAN_LOW_1_G("Ciphering enabled - mode = %d", mode);
  }

}


void gl1_msg_disable_ciphering ( gas_id_t gas_id )
{
  gl1_hw_disable_ciphering (gas_id );
  gl1_msg_ciphering_enabled[gas_id] = FALSE;

  MSG_GERAN_LOW_0_G("Ciphering disabled");

}

void gl1_msg_clear_ciphering_enabled ( gas_id_t gas_id )
{
  gl1_msg_ciphering_enabled[gas_id] = FALSE;
}

boolean gl1_msg_is_ciphering_enabled( gas_id_t gas_id )
{
  return (gl1_msg_ciphering_enabled[gas_id]);
}

/*===========================================================================

FUNCTION  gl1_msg_init_tch

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
void gl1_msg_init_tch (gl1_msg_chan_type chan, gl1_msg_voc_type voc_type,gas_id_t gas_id)
{
  switch (chan)
  {
    case GL1_MSG_TCH:
      if (voc_type == GL1_MSG_EFS)
      {
        current_chan[gas_id] = GL1_HW_TCH_EFS;
      }
      else if (voc_type == GL1_MSG_HS)
      {
        current_chan[gas_id] = GL1_HW_TCH_HS;
      }
      else
      {
        current_chan[gas_id] = GL1_HW_TCH_FS;
      }
      break;

    case GL1_MSG_DCCH:
      /* Consider channel mode change only for voice channels */
      current_chan[gas_id] = GL1_HW_FACCH;
      break;

#ifdef FEATURE_DATA_GCSD
    case GL1_MSG_TCH_F96:
      /* Consider channel mode change only for voice channels */
      current_chan[gas_id] = GL1_HW_TCH_F96;
      break;

    case GL1_MSG_TCH_F144:
      /* Consider channel mode change only for voice channels */
      current_chan[gas_id] = GL1_HW_TCH_F144;
      break;
#endif /* FEATURE_DATA_GCSD */

    default:
      MSG_GERAN_ERROR_1_G("Invalid channel type",chan);
      current_chan[gas_id] = GL1_HW_TCH_FS;
      break;

  }

  current_voc[gas_id] = voc_type;

  gl1_hw_init_tch( current_chan[gas_id],gas_id );
}

/*===========================================================================

FUNCTION  gl1_msg_terminate_tch

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
void gl1_msg_terminate_tch (gas_id_t gas_id )
{
   gl1_hw_terminate_tch(gas_id);
   /* clear DTX_used flag for next report */
   gl1_reset_dtx_used_flag(gas_id);
}



/*===========================================================================

FUNCTION gl1_msg_align_vfr

DESCRIPTION
  This function sets the hardware such that on the next frame tick, the
  Vocoder Frame Alignment should occur.  This function should be called
  every time the gstmr is reprogrammed to realign the VFR again.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_align_vfr( boolean real_vfr,gas_id_t gas_id )
{
    /* Generate align VFR event */
    gl1_msg_log_align_vfr_event( real_vfr,gas_id );

    gl1_hw_align_vfr(gas_id);
}


/*===========================================================================

FUNCTION gl1_msg_get_tch_loopback

DESCRIPTION
  This function gets the current TCH loopback in operation. If GL1_DEFS_NO_LOOPBACK
  returns then no loopback is currently active

DEPENDENCIES
  None

RETURN VALUE
  current loop_back type

SIDE EFFECTS
  None
===========================================================================*/
gl1_defs_loopback_type gl1_msg_get_tch_loopback(gas_id_t gas_id)
{
  return(loopback_type[gas_id]);
}

/*===========================================================================

FUNCTION gl1_msg_tch_loopback

DESCRIPTION
  This function tells the mdsp to put the TCH into loopback A,B or C or no
  loopback.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_tch_loopback( gl1_defs_loopback_type loopback,gas_id_t gas_id )

{

   loopback_type[gas_id] = loopback;

   /*
      This hack was put in to get fast BER to pass on the HP.  It seems to
      pass now on the HP without the fix but keeping the commented code in for now
      in case we have problems down the line
   */

   /* Only loopback A & B implemented in s/w. C still handled my mdsp */
   if (loopback == GL1_DEFS_LOOPBACK_TYPE_C ||
       loopback == GL1_DEFS_NO_LOOPBACK )
   {
      gl1_hw_tch_loopback(loopback,gas_id);
   }

   if (loopback == GL1_DEFS_LOOPBACK_TYPE_A)
   {
      MSG_GERAN_HIGH_0_G("L1: Loopback A");
   }
   else if (loopback == GL1_DEFS_LOOPBACK_TYPE_B)
   {
      MSG_GERAN_HIGH_0_G("L1: Loopback B");
   }
   else if (loopback == GL1_DEFS_LOOPBACK_TYPE_C)
   {
      MSG_GERAN_HIGH_0_G("L1: Loopback C");
   }
   else if (loopback == GL1_DEFS_LOOPBACK_TYPE_D)
   {
      MSG_GERAN_HIGH_0_G("L1: Loopback D");
   }
   else if (loopback == GL1_DEFS_LOOPBACK_TYPE_E)
   {
      MSG_GERAN_HIGH_0_G("L1: Loopback E");
   }
   else if (loopback == GL1_DEFS_LOOPBACK_TYPE_F)
   {
      MSG_GERAN_HIGH_0_G("L1: Loopback F");
   }
#ifdef FEATURE_GSM_AMR
   else if (loopback == GL1_DEFS_LOOPBACK_TYPE_I)
   {
      MSG_GERAN_HIGH_0_G("L1: Loopback I");
   }
#endif
   else if (loopback == GL1_DEFS_NO_LOOPBACK)
   {
      MSG_GERAN_HIGH_0_G("L1: No Loopback");
   }
   else
   {
      MSG_GERAN_HIGH_0_G("L1: Unknown Loopback mode");
   }


   /* If loopback is enabled , turn DTX off                     */
   /* When done with loopback put dtx back to it's former state */
   switch (loopback)
   {

      case GL1_DEFS_NO_LOOPBACK:
         /* MVS NOTE: this may be handled up in gsvoicif
                   -- it may not be needed here??? */
         if (prior_dtx_mode[gas_id] == DTX_ENABLED)
         {
            l1_vs_set_dtx_mode(DTX_ENABLED, gas_id);
         }
         else
         {
            l1_vs_set_dtx_mode(DTX_DISABLED, gas_id);
         }
         gl1_clkdata_speed(FALSE, L1_DED_SPEEDUP_LOOPBACK, L1_DED_DATA_NULL,L1_DED_LOOPBACK,gas_id);
		 
         break;
      default:
         prior_dtx_mode[gas_id] = l1_vs_get_dtx_mode(gas_id);
         l1_vs_set_dtx_mode(DTX_DISABLED, gas_id);
         gl1_clkdata_speed(TRUE, L1_DED_SPEEDUP_LOOPBACK, L1_DED_DATA_NULL,L1_DED_LOOPBACK,gas_id);
		 
         break;
   }
}

/****************************************************************************
 ****************************************************************************
 ****************************************************************************
                            LOCAL   FUNCTIONS
 ****************************************************************************
 ****************************************************************************
 ***************************************************************************/

/*=============================================================================
FUNCTION gl1_msg_get_current_voc_type

DESCRIPTION
   This function is used by PL1 to query the current vocoder type in use.

DEPENDENCIES

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
gl1_msg_voc_type gl1_msg_get_current_voc_type(gas_id_t gas_id)
{
   return current_voc[gas_id];
}

/*=============================================================================
FUNCTION gl1_msg_log_align_vfr_event

DESCRIPTION
   This function generates a DIAG event when the VFR is aligned
   after a handover

DEPENDENCIES

RETURN VALUE
   None

SIDE EFFECTS
   None
============================================================================= */
static void gl1_msg_log_align_vfr_event( boolean real_vfr,gas_id_t gas_id )
{
   MSG_GERAN_HIGH_2_G("EVENT_GSM_L1_ALIGN_VFR id=%x", EVENT_GSM_L1_ALIGN_VFR, 0 );
   GERAN_EVENT_REPORT_PAYLOAD(   EVENT_GSM_L1_ALIGN_VFR,
                           sizeof(byte),
                           &real_vfr );
}

/*===========================================================================

FUNCTION gl1_msg_tch_set_prior_dtx_mode

DESCRIPTION
  This function sets prior_dtx_mode variable. This variable determines if dtx
  mode is enabled/disabled when loopback finishes

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_tch_set_prior_dtx_mode(boolean dtx_enabled, gas_id_t gas_id)
{
  prior_dtx_mode[gas_id] = dtx_enabled ? DTX_ENABLED : DTX_DISABLED;
}

/*===========================================================================

FUNCTION gl1_msg_tch_get_loopback_type

DESCRIPTION
  This function returns the current loopback type. If loopback is not enabled
  function returns GL1_DEFS_NO_LOOPBACK

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
gl1_defs_loopback_type gl1_msg_tch_get_loopback_type(gas_id_t gas_id)
{
  return loopback_type[gas_id];
}

