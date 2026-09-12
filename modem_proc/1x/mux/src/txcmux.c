/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

             C D M A     M U L T I P L E X I N G    M O D U L E

GENERAL DESCRIPTION
  Contains frame building routines

EXTERNALIZED FUNCTIONS
  txc_pri - provides the primary traffic to the Multiplex sublayer
  dependant on what the current primary service option is
  txc_data_pump_build_frame - builds the next frame to be transmitted if
  data pump has been initialized
  txc_build_frame - builds the next frame to be transmitted

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  Copyright (c) 1991 - 2020 Qualcomm Technologies, Inc.
  All Rights Reserved Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies, Inc. and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies, Inc.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: //components/rel/1x.mpss/8.0/mux/src/txcmux.c#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/22/20   vaa     Implement QSH MDUMP framework
01/19/18   eye     F3 logging reduction.
09/29/15   agh     Remove all references to MVS and cleanup WON feature
04/21/15   srk     DRDSDS and BOLT REVISIT Feature cleanup
12/02/14   agh     Support for new customer specific voice service option
05/29/14   pap     Redistributing messages.
11/07/13   cjb     Revisit DCCH configuration for RC3/4 preamble case
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
10/19/12   vks     Use 1x internal feature (FEATURE_MODEM_1X_VOC_4GV_WB_ON_NB)
                   for HDOn-EVRC feature (FEATURE_VOC_4GV_WB_ON_NB)
10/16/12   vks     Initial changes to support HDOn-EVRC feature
07/17/12   eye     Intercepting out of bound MUX ID's and assigning them with
                   legacy equivalent.
04/04/12   trc     Convert to m1x_time module
02/24/12   srk     Mainlined FEATURE_IS2000_P2
01/23/12   srk     Feature Cleanup
01/17/12   jtm     Revert REL_A feature clean up.
01/17/12   ppr     Feature Cleanup.
01/06/12   jtm     Fix race condition between TXC task and encoder ISR when
                   release order is processed.
01/04/12   srk     Feature Cleanup.
11/11/11   trc     Ensure aligned time_type passed to time_get() in txc_log_mux1
09/19/11   jtm     Fix race condition between TXC task and encoder ISR when
                   processing service option and physical channels.
08/15/11   jtm     Klocwork Fixes.
08/04/11   jtm     Reset frame delay to TC value after the counter expires in
                   case we transitioned to traffic from TT.
07/26/11   vks     Fix compiler warnings.
05/23/11   jtm     Provided a mechanism to allow a configurable delay from TX on
                   until we send a signaling message.
05/20/11   jtm     Protect changes to SO and physical channel.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/14/11   jtm     Klocwork fixes.
01/25/11   jtm     Added Mux Vocoder interface module.
01/21/11   jtm     Klocwork fixes.
01/18/11   jtm     Lint clean up.
12/28/10   ag      Decoupling of SO73 from 1xAdv.
12/17/10   jtm     Mainlined FEATURE_MVS_MIGRATE under FEATURE_MODEM_1X_VOICE_SUPPORT.
12/15/10   jtm     Feature cleanup.
12/14/10   jtm     Removed MDR/SCCH support.
12/03/10   trc     Add support for pri payload override debug functionality
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
10/22/10   jtm     Updated names for CAI*_ID counters changed by CP.
08/10/10   vks     Cleanup Genesis Revisit feature.
08/03/10   jtm     Added logic to default case of txc_pri for graceful
                   processing of invalid SOs.
07/22/10   jtm     Fixed merge errors.
07/06/10   jtm     Fixed compiler warnings.
06/30/10   jtm     Apply non-critical blanking algorithm to a Null frames in RC8.
06/24/10   jtm     Use CAI_IS_SO_VOICE to check for valid voice service options.
                   Default unsupported SO handling to the same as CAI_SO_NULL.
06/15/10   jtm     For SMS SO's treat them as 1/8th rate Null frames. For RC8
                   the frames are non-critical and can be used for smart
                   blanking.
                   Relocated is_non_critical_frame flag to txc_build_frame().
                   Added SO75 support.
06/10/10   jtm     Eliminate dependency on log_dmss.h
04/12/10   jtm     1x Advanced: Corrected non-critical frame processing.
02/04/10   jtm     Removed FEATURE_PLT.
02/01/10   jtm     Removed feature TMOBILE.
11/24/09   jtm     Moved dtx_non_crit_fch_frame to txtc structure.
10/27/09   jtm     Mux support for MSO74 and SO73.
09/24/09   jtm     Klocwork fixes.
09/23/09   jtm     Changed macro ENC_RC_TO_RATE_SET to become a function.
09/14/09   adw     Changes to support modem data header CMI refactoring.
08/22/09   jtm     Lint Fixes.
08/21/09   jtm     Lint Fixes.
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's and
                   removed FEATURE_1X_DISABLE_CMI.
08/05/09   jtm     Lint fixes.
04/17/09   jtm     Added includes to fix compiler warnings.
04/13/09   adw     Replaced tdso.h with tdso_v.h include for CMI.
04/01/09   jtm     Include clean up.
03/31/09   jtm     Eliminated implicit includes.
03/10/09   vlc     In function txc_pri(), added check for the case of NULL
                   rate returned by vocoder driver.
03/03/09   jtm     Lint fixes.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
09/12/08   vlc     Added support for FEATURE_MVS_MIGRATE.
07/21/08   jtm     Removed FEATURE_MAX_PWR_PROTECTION_DEBUG and
                   FEATURE_IS2000_REL_C_DV code.
09/19/06   vlc     Fixed lint errors.
06/19/06   vlc     Moved prim_num_bits from txctraffic.c to this file.  It is
                   only used in this file.
                   Added inclusion of mccdma.h
                   Added support for voice encryption.
                   Moved function txc_pri_num_bits() outside FEATURE_IS2000_REL_A_SVD.
01/10/06   rkc     Change ERR_FATAL to MSG_ERROR when prim_data_ptr should not
                   be NULL.
07/25/05   rkc     Fixed lint errors.
04/29/05   vlc     Added 4GV support under FEATURE_VOC_4GV.
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
09/28/04   sr      Merged in changes from 6550 2009 build
07/30/04   vlc     Added support for rel C TDSO.
06/18/04   ank     Mainlined FEATURE_GPSONE_DBM.
04/20/04   jrp     Fixed lint warnings.
10/22/03   bn      Fixed compilation warning errors when using RVCT compiler
09/09/03   dna     Merged porting changes for MSM6500
08/25/03   vlc     Removed shared rate_set from txc_so (txc_so_type) to avoid
                   confusion between FCH and DCCH rate sets.  Now calculate
                   rate set from RC as it is needed.
04/29/03   vlc     Comment out debug statement occuring every frame.
03/28/03   vlc     Added logic in function txc_rlp_sec_data_rate() to support
                   continuous transmit priority for RLP signalling frames in
                   SVD calls.
                   Used new constant TXC_VOC_DIM_DELAY instead of hard coded
                   value.
02/25/03   jrp     Changed two ERR_FATAL to MSG_ERROR.
12/05/02   vlc     Fixed Markov SO handling in function txc_pri() when
                   FEATURE_IS2000_REL_A_SVD is enabled.
09/17/02   hrk     Before deciding max RLP rate on DCCH, making sure that MS
                   is not in control hold state.
09/04/02   bn,hrk  In Rel.A when FEATURE_IS2000_REL_A_SVD is turned off,
                   prim_dcch_data_ptr and prim_dcch_rate were not initialized.
07/24/02   hrk     Added support for VP2. Mainlined from -r1.28.1.7
06/25/02   HQ      Added FEATURE_CDSMS.
05/15/02   bn      Fixed the incorrect of tdso frame type
04/18/02   az      Added support for new voice service option SMV (SO 56)
04/18/02   lcc     When CAI_SO_NULL is active, set rate to 1/8 even if
                   txtc.nulls is not true.
03/04/02   bn      Set the DCCH mux category of DTX frame type correctly for RS1 or RS2
02/27/02   bn,hrk  Added SVD support.
02/11/02   hrk     Keeping R-FCH, R-DCCH enabled during preambles, no harm as
                   R-FCH, R-DCCH gain during preamble is set to 0.
10/30/01   bn      Added support for Release A RDCH logging
09/06/01   hrk     Fixed reporting of dim-and-burst frames in frame types logging.
                   Created new function - build_null_frame() to build null 1800/1200
                   primary traffic frame.
08/28/01   lh      Back out SI bits changes for Release A Addendum 2.
08/02/01   jrp     Added support for SO54, SO55.
06/15/01   day     Mainlined FEATURE_IS2000
05/11/01   vlc     Merged in datapump changes for PLT (ks.)
04/20/01   sr      Merged in from MSM5100 archive
04/19/01   tc      Merged with MSM_MUX1X.01.00.01
           lcc     Corrected some merge errors.
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           lcc     Added support for FEATURE_IS2000_P2
03/27/01   vlc     Merged to rev 1.10 of MSM archive.
           lcc     Corrected tranmission of preambles in PCG units so that it won't
                   be repeated for "n" number of frames.
           hrk     Enhancements for Max Power Protection.
           lcc     Merged in support for SO 17 (CAI_SO_VOICE_13K_IS733).
           hrk     Enhancements for Max Power Protection and added debug msgs.
03/22/01   lh      Moved si_adj setting to the beginning of build_blank_frame().
03/14/01   lh      Fixed wrong SI bits settings for Release A.
03/07/01   lh      Support for frame formatting based on p_rev_in_use. (LAC
                   changes for REL A)
03/01/01   tc      Merged MSM_MUX1X.00.00.15.
01/31/01   tc      Merged GPS support.
12/13/00   hrk     Enhancements for Max Power Protection.
11/12/00   sr      (merge) Added support for Full TDSO
11/10/00   jc      This file was part of txc.c. See txccommon.c for full
                   change history

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

//common
#include "1x_variation.h"
#include <memory.h>
#include "comdef.h"
#include "customer.h"
#include "modem_1x_defs.h"
#include "modem_1x_utils.h"

//mux
#include "mux.h"
#include "txccommon_i.h"
#include "txcmux.h"
#include "txctraffic_i.h"
#include "txcso.h"
#include "txcmc_i.h"
#include "mux_logtypes.h"
#include "muxvoc.h"

//drivers
#include "enc.h"
#include "enc_v.h"
#include "enc_i.h"

//cp
#include "cai.h"
#include "mar_i.h"
#include "parm_i.h"
#include "tdso_v.h"
#include "mctrans_i.h"
#include "mccdma_v.h"
#include "mccdma_i.h"

//other
#include "err.h"
#include "bit.h"
#include "log.h"
#include "msg.h"
#include "m1x_diag.h"
#include "m1x_time_i.h"
#include "queue.h"
#include "amssassert.h"

//feature dependent
#ifdef FEATURE_IS2000_REL_A_SVD
#include "dsrlp_api.h"
#endif /* FEATURE_IS2000_REL_A_SVD */

#include "stringl.h" /* for memscpy */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* Constants for building Traffic Channel Frames */
#define TC_BLANK 168             /* Signaling bits for blank and burst */
#define TC_DIM2   88             /* 1/2 rate dim-and-burst */
#define TC_DIM4  128             /* 1/4 rate dim-and-burst */
#define TC_DIM8  152             /* 1/8 rate dim-and-burst */

word prim_num_bits = 0;          /* Num of bits in FCH primary frame */

/* Log buffer for accumulated MUX frame log */
  txc_log_mux_buf_type txc_log_mux_buf;

/*lint -esym(752,txc_max_pwr_limited_dtx_sch) Variable used in some ifdef's*/
extern boolean txc_max_pwr_limited_dtx_sch;

uint16 build_null_frame ( void );
uint16 build_norm_frame ( void );

word txc_pri_num_bits (cai_data_rate_type);
extern txc_sig_frame_type next_fch_sig_type;

#ifdef FEATURE_IS2000_REL_A_SVD
cai_data_rate_type txc_det_frame_rate (void);
dsrlp_rate_enum_type txc_rlp_sec_data_rate (word, boolean);
#endif // FEATURE_IS2000_REL_A_SVD

/* Support overriding of voice bits w/ counter debug option */
uint8 txcmux_override_pri_bits = 0;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/* <EJECT> */
/*===========================================================================

FUNCTION TXC_LOG_MUX1

DESCRIPTION
  This function logs the Reverse Traffic Channel Mux data.

DEPENDENCIES
  None.
RETURN VALUE
  None.
SIDE EFFECTS
  None.
===========================================================================*/
void txc_log_mux1
(
  uint16 val  /* value to log */
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* increment relevant parameter */
  ( void )parm_inc( val, 1 );

  if (txc_log_mux_buf.count == 0)
  {
    /* Use a temp time_type (qword), since time_get() expects it to be
       properly aligned, while our log packet is not */
    time_type timestamp;
    (void) m1x_time_get(timestamp);
    qw_equ(txc_log_mux_buf.entry.hdr.ts,timestamp);
  }

  /* Fill in log data */
  txc_log_mux_buf.entry.vals[txc_log_mux_buf.count] = (byte) val;
  txc_log_mux_buf.count++;
  txc_log_mux_buf.entry.hdr.len += sizeof(txc_log_mux_buf.entry.vals[0]);

  if (txc_log_mux_buf.count == MUX_LOG_R_MUX1_CNT)
  {
    TXC_SEND_MUX1_LOG();  /*lint !e717 Ignore while (0) */
  }
} /* txc_log_mux1 */


/* <EJECT> */
/*===========================================================================

FUNCTION TXC_PRI

DESCRIPTION
  This function provides the primary traffic to the Multiplex sublayer
  dependant on what the current primary service option is set to.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_pri (void)
{
  static byte null_data[3] = {0xFF, 0xFF, 0xFF};
  uint16 pri_channel_mask;
  uint16 pri_service_op;
  uint16 sec_service_op;

  byte *fch_data_ptr;
  cai_data_rate_type fch_rate;
  uint16 num_fch_bits = 0;

  byte *dcch_data_ptr;
  cai_data_rate_type dcch_rate;
  word so;

  /* -------------------------------------------------------------------- */

  TXC_ENTER_ISR_SECTION();
  pri_channel_mask = txc_so.pri.phy_channels;
  pri_service_op = txc_so.pri.so;
  sec_service_op = txc_so.sec.so;
  TXC_LEAVE_ISR_SECTION();

  dcch_rate     = CAI_BLANK_RATE;
  dcch_data_ptr = NULL;

  /* initialize rate and data pointers based on current signaling mode */
  if (txtc.frame.sig == BLANK)
  {
    fch_rate = CAI_BLANK_RATE;
    fch_data_ptr = NULL;
  }
  else
  {
    fch_data_ptr = null_data;

    #ifdef FEATURE_IS2000_1X_ADV
    if( txc_so.rev_fch.rc == CAI_RC_8 )
    {
      fch_rate = CAI_NON_CRITICAL_RATE;
    }
    else
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      fch_rate = CAI_EIGHTH_RATE;
    }
  }

  so = pri_service_op;

  #ifdef FEATURE_1X_CUST_VOICE_SO
  /* This workaround has been added to handle NV dependent non-constant
   * SO while ensuring the existing switch construct is not disrupted */
  if (so == mcc_get_cust_so() && so != CAI_SO_NULL)
  {
    so = CAI_GEN_EVRC_CUST_SO;
  }
  #endif /* FEATURE_1X_CUST_VOICE_SO */
  switch (so)
  {
    case CAI_SO_VOICE_13K_IS733:
    case CAI_SO_VOICE_13K:
    case CAI_SO_VOICE_IS96A:
    case CAI_SO_VOICE_EVRC:
    #ifdef FEATURE_1X_CUST_VOICE_SO
    case CAI_GEN_EVRC_CUST_SO:
    #endif /* FEATURE_1X_CUST_VOICE_SO */
    case CAI_SO_VOICE_EVRC_NW:
    case CAI_SO_VOICE_SMV:
    case CAI_SO_VOICE_4GV_NB:
    case CAI_SO_VOICE_4GV_WB:
    {
      #ifdef FEATURE_IS2000_REL_A_SVD
      if (pri_channel_mask & MCTRANS_PHY_CHAN_DCCH)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Voice on DCCH not supported");
      }
      else if (pri_channel_mask & MCTRANS_PHY_CHAN_FCH)
      #endif // FEATURE_IS2000_REL_A_SVD
      {
        txc_sig_frame_type signext = txtc.frame.sig;

        #ifdef FEATURE_IS2000_REL_A_SVD
        if (txtc.cur_dsch == TXC_REV_DCCH)
        {
          /* signalling on DCCH, voice on FCH: don't DIM/BLANK vocoder */
          signext = NORM;
        }
        #endif // FEATURE_IS2000_REL_A_SVD

        fch_rate = muxvoc_tx_voc_traffic (&fch_data_ptr, signext);

        num_fch_bits = txc_pri_num_bits (fch_rate);
      }
      break;
    }

    #ifdef FEATURE_IS2000_1X_ADV
    case CAI_SO_LOOPBACK_SO75:
    #endif /* FEATURE_IS2000_1X_ADV */
    case CAI_SO_LOOPBACK_SO55:
    case CAI_SO_LOOPBACK_13K:
    case CAI_SO_LOOPBACK:

      #ifdef FEATURE_IS2000_REL_A_SVD
      if (sec_service_op != CAI_SO_NULL)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Loopback SO %d not supported in SVD",
          pri_service_op);
      }

      if (pri_channel_mask & MCTRANS_PHY_CHAN_DCCH)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Loopback SO %d not supported on DCCH",
          pri_service_op);
      }
      else if (pri_channel_mask & MCTRANS_PHY_CHAN_FCH)
      #endif // FEATURE_IS2000_REL_A_SVD
      {
        /* Fill up buffer with Loopback rx packet data bits, return rate */
        fch_rate = txc_loopback_traffic( &fch_data_ptr, txtc.frame.sig );
      }
      break;

    case CAI_SO_MARKOV_SO54:
    case CAI_SO_MARKOV_13K:
    case CAI_SO_MARKOV:
    case CAI_SO_RS1_MARKOV:
    case CAI_SO_RS2_MARKOV:
    #ifdef FEATURE_IS2000_1X_ADV
    case CAI_SO_MARKOV_SO74:
    #endif /* FEATURE_IS2000_1X_ADV */
    {
      /* fill up the buffer with Markov data, returns rate */
      #ifdef FEATURE_IS2000_REL_A_SVD
      if (sec_service_op != CAI_SO_NULL)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Markov SO %d not supported in SVD",
          pri_service_op);
      }

      if (pri_channel_mask & MCTRANS_PHY_CHAN_DCCH)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "Markov SO %d not supported on DCCH",
          pri_service_op);
      }
      else if (pri_channel_mask & MCTRANS_PHY_CHAN_FCH)
      #endif // FEATURE_IS2000_REL_A_SVD
      {
        /* Fill up buffer with Markov rx packet data bits, return rate */
        fch_rate = txc_mar_traffic( &fch_data_ptr );
      }
      break;
    }

    #ifdef FEATURE_CDSMS
    case CAI_SO_SMS:
    case CAI_SO_RS2_SMS:
    {

      #ifdef FEATURE_IS2000_REL_A_SVD
      if (sec_service_op != CAI_SO_NULL)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "SMS SO %d not supported in SVD",
          pri_service_op);
      }

      /* Make sure this is carried on the FCH */
      if (pri_channel_mask & MCTRANS_PHY_CHAN_DCCH)
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "SMS SO %d not supported on DCCH",
          pri_service_op);
      }
      else
      #endif // FEATURE_IS2000_REL_A_SVD
      {
        #ifdef FEATURE_IS2000_1X_ADV
        if( txc_so.rev_fch.rc != CAI_RC_8 )
        #endif /* FEATURE_IS2000_1X_ADV */
        {
          fch_rate = txc_mar_traffic( &fch_data_ptr );
        }
      }
      break;
    }
    #endif /* FEATURE_CDSMS */

    case CAI_SO_TDSO:
    case CAI_SO_FULL_TDSO:
    {
      txc_sig_frame_type signext = txtc.frame.sig;

      #ifdef FEATURE_IS2000_REL_A_SVD
      if (
          ((pri_channel_mask & MCTRANS_PHY_CHAN_FCH) &&
           (txtc.cur_dsch == TXC_REV_DCCH)) ||
          ((pri_channel_mask & MCTRANS_PHY_CHAN_DCCH) &&
           (txtc.cur_dsch == TXC_REV_FCH))
         )
      {
        /* signaling and pri on different physical channels
        ** don't DIM/BLANK
        */
        signext = NORM;
      }

      if (pri_channel_mask & MCTRANS_PHY_CHAN_FCH)
      {
        fch_rate = txc_tdso_traffic (&fch_data_ptr, signext);
      }
      else if (pri_channel_mask & MCTRANS_PHY_CHAN_DCCH)
      {
        dcch_rate = txc_tdso_traffic (&dcch_data_ptr, signext);
      }
      #else /*FEATURE_IS2000_REL_A_SVD */
      if (txc_so.rev_dcch.included)
      {
        dcch_rate = txc_tdso_traffic (&dcch_data_ptr, signext);
      }
      else
      {
        fch_rate = txc_tdso_traffic (&fch_data_ptr, signext);
      }
      #endif // FEATURE_IS2000_REL_A_SVD
      break;
    }

    #ifdef FEATURE_IS2000_REL_A_SVD
    case CAI_SO_PPP_PKT_DATA_3G:
    {
      dsrlp_rate_enum_type max_rlp_rate = DSRLP_RATE_BLANK; /* no data */

      if (txc_so.rev_fch.included)
      {
        #ifdef FEATURE_IS2000_REL_A
        if (pri_channel_mask & MCTRANS_PHY_CHAN_FCH)
        #endif /* FEATURE_IS2000_REL_A */
        {
          /* R-FCH assigned and RLP is primary SO on R-FCH
          */
          if (!(txc_tx.active_msg && (txtc.cur_dsch == TXC_REV_FCH)))
          {
            /* no msg pending on FCH, all bandwidth to data
            */
            max_rlp_rate = DSRLP_RATE_1;
          }
        }
      }
      txc_tx_process_fundich_data3g (MCTRANS_PHY_CHAN_FCH,
                                     max_rlp_rate,
                                     DSRLP_PRIMARY_SRVC);

      max_rlp_rate = DSRLP_RATE_BLANK; /* no data */

      if (txc_so.rev_dcch.included
          #ifdef FEATURE_IS2000_REL_A
          && (pri_channel_mask & MCTRANS_PHY_CHAN_DCCH)
          #endif /* FEATURE_IS2000_REL_A */
         )
      {
        /* R-DCCH assigned and RLP is primary SO on R-DCCH
        */
        boolean control_hold = txtc.gating_rate != CAI_GATING_RATE_NONE;

        if (!control_hold &&
            !(txc_tx.active_msg && (txtc.cur_dsch == TXC_REV_DCCH)))
        {
          /* MS not in control hold state and no msg pending on DCCH,
          ** all bandwidth to data
          */
          max_rlp_rate = DSRLP_RATE_1;
        }
      }
      txc_tx_process_fundich_data3g (MCTRANS_PHY_CHAN_DCCH,
                                     max_rlp_rate,
                                     DSRLP_PRIMARY_SRVC);
      break;
    }
    #endif /* FEATURE_IS2000_REL_A_SVD */

    case CAI_SO_NULL:

    case CAI_SO_RS1_OTAPA:
    case CAI_SO_RS2_OTAPA:

    #ifdef FEATURE_GPSONE
    case CAI_SO_RS1_PD:
    case CAI_SO_RS2_PD:
    #endif /* FEATURE_GPSONE */
    default:
    {
      /*In the case where the SO is valid but not supported due to a
        disabled feature, we should continue Tx safely by sending null
        data to do our part to keep the call alive.*/
      break;
    }
  }

  /* If in pri bits override mode, override with new pri data in here */
  if (txcmux_override_pri_bits > 0)
  {
    /* 1/8-rate frame payload, RS1 = 16 bits, RS2 = 20 bits, so use 3 bytes */
    static uint8 override_frame[3] = {0, 0, 0};

    /* Reset the count if coming through the first time, otherwise increment */
    if (txcmux_override_pri_bits > 1)
    {
      txcmux_override_pri_bits = 1;
      override_frame[0] = 0;
      M1X_MSG( MUX, LEGACY_HIGH,
        "TX pri override counter RESET");
    }
    else
    {
      ++override_frame[0];
    }

    /* Repoint the outgoing pri data pointer accordingly, set 1/8-rate */
    override_frame[2] = override_frame[1] = override_frame[0];
    fch_data_ptr = override_frame;
    fch_rate = CAI_EIGHTH_RATE;
    M1X_MSG( MUX, LEGACY_HIGH,
      "TX pri override payload: %x ",
      override_frame[0]);
  }

  /*************************************************************/

  TXC_ENTER_ISR_SECTION();
  prim_data_ptr = fch_data_ptr;
  prim_rate = txc_data_rate = fch_rate;
  prim_num_bits = num_fch_bits;

  prim_dcch_data_ptr = dcch_data_ptr;
  prim_dcch_rate = dcch_rate;

  #ifdef FEATURE_IS2000_REL_A_SVD
  if(pri_channel_mask & MCTRANS_PHY_CHAN_DCCH)
  #else
  if (txc_so.rev_dcch.included)
  #endif /* !FEATURE_IS2000_REL_A_SVD */
  {
    txc_data_rate = dcch_rate;
  }
  TXC_LEAVE_ISR_SECTION();
}

/*===========================================================================

FUNCTION TXC_PRI_NUM_BITS

DESCRIPTION
  This function returns the number of bits corresponding to the frame rate.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
word txc_pri_num_bits
(
 cai_data_rate_type rate
)
{
  /* Called for VOICE SOs, must be on FCH. */
  if (txc_get_rev_link_rate_set( txc_so.rev_fch.rc ) == ENC_RATE_9600)
  {
    if (rate == CAI_FULL_RATE)
    {
      return MUX1_FULL_PRI_LEN;
    }
    else if (rate == CAI_HALF_RATE)
    {
      return MUX1_HALF_PRI_LEN;
    }
    else if (rate == CAI_QUARTER_RATE)
    {
      return MUX1_QTR_PRI_LEN;
    }
    else if (rate == CAI_EIGHTH_RATE)
    {
      return MUX1_EIGHTH_PRI_LEN;
    }
#ifdef FEATURE_IS2000_1X_ADV
    else if (rate == CAI_NON_CRITICAL_RATE)
    {
      return MUX1_EIGHTH_PRI_LEN;
    }
#endif /* FEATURE_IS2000_1X_ADV */
    else
    {
      return 0;
    }
  }
  else
  {
    if (rate == CAI_FULL_RATE)
    {
      return MUX2_FULL_PRI_LEN;
    }
    else if (rate == CAI_HALF_RATE)
    {
      return MUX2_HALF_PRI_LEN;
    }
    else if (rate == CAI_QUARTER_RATE)
    {
      return MUX2_QTR_PRI_LEN;
    }
    else if (rate == CAI_EIGHTH_RATE)
    {
      return MUX2_EIGHTH_PRI_LEN;
    }
    else
    {
      return 0;
    }
  }
}

#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION TXC_DET_FRAME_RATE

DESCRIPTION
  This function determines the lowest frame rate on the R-FCH which can
  accomodate the primary and secondary traffic data based on the number of
  bits in the primary and secondary traffic stream.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
cai_data_rate_type
txc_det_frame_rate (void)
{
  /* Called while processing FCH */
  if (txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_9600)
  {
    if ((prim_num_bits + sec_fch_num_bits) <= MUX1_EIGHTH_PRI_LEN)
    {
      return CAI_EIGHTH_RATE;
    }
    else if ((prim_num_bits + sec_fch_num_bits) <= MUX1_QTR_PRI_LEN)
    {
      return CAI_QUARTER_RATE;
    }
    else if ((prim_num_bits + sec_fch_num_bits) <= MUX1_HALF_PRI_LEN)
    {
      return CAI_HALF_RATE;
    }
    else
      return CAI_FULL_RATE;
  }
  else
  {
    if ((prim_num_bits + sec_fch_num_bits) <= MUX2_EIGHTH_PRI_LEN)
    {
      return CAI_EIGHTH_RATE;
    }
    else if ((prim_num_bits + sec_fch_num_bits) <= MUX2_QTR_PRI_LEN)
    {
      return CAI_QUARTER_RATE;
    }
    else if ((prim_num_bits + sec_fch_num_bits) <= MUX2_HALF_PRI_LEN)
    {
      return CAI_HALF_RATE;
    }
    else
      return CAI_FULL_RATE;
  }
}

/*===========================================================================

FUNCTION TXC_SEC

DESCRIPTION
  This function provides the secondary traffic to the Multiplex sublayer
  dependant on what the current secondary service option is set to.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void txc_sec (void)
{
  uint16 sec_service_op   = txc_get_sec_so();

  /* initialize FCH secondary rate and data ptr
  */
  sec_fch_num_bits = 0;
  sec_fch_data_ptr = NULL;

  /* initialize DCCH secondary rate and data ptr
  */
  sec_dcch_num_bits = 0;
  sec_dcch_data_ptr = NULL;

  switch (sec_service_op)
  {
    case CAI_SO_TDSO:
    case CAI_SO_FULL_TDSO:
    {
      boolean msg_on_channel = FALSE;

      if (txc_so.rev_dcch.included &&
               (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH))
      {
        msg_on_channel = (txc_tx.active_msg && (txtc.cur_dsch == TXC_REV_DCCH));
        /* Ignoring data rate return value */
        (void) txc_tdso_sec_traffic (&sec_dcch_data_ptr, msg_on_channel);
        sec_fch_data_ptr = NULL;
      }
      else if (txc_so.rev_fch.included)
      {
        /* R-FCH assigned and TDSO is secondary SO on R-FCH
        */
        if (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_FCH)
        {
          msg_on_channel = (txc_tx.active_msg && (txtc.cur_dsch == TXC_REV_FCH));
          /* Ignoring data rate return value */
          (void) txc_tdso_sec_traffic (&sec_fch_data_ptr, msg_on_channel);
          sec_dcch_data_ptr = NULL;
        }
      }
      break;
    }

    case CAI_SO_NULL:
    {
      sec_fch_data_ptr  = NULL;

      sec_dcch_data_ptr = NULL;

      break;
    }

    case CAI_SO_PPP_PKT_DATA_3G:
    {
      boolean msg_on_channel = FALSE;
      dsrlp_rate_enum_type max_rlp_rate  = DSRLP_RATE_BLANK;

      if (txc_so.rev_fch.included)
      {
        if (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_FCH)
        {
          /* R-FCH assigned and RLP is secondary SO on R-FCH
          */
          msg_on_channel = (txc_tx.active_msg && (txtc.cur_dsch == TXC_REV_FCH));
          max_rlp_rate =
              txc_rlp_sec_data_rate (MCTRANS_PHY_CHAN_FCH, msg_on_channel);
        }
      }
      txc_tx_process_fundich_data3g (MCTRANS_PHY_CHAN_FCH,
                                     max_rlp_rate,
                                     DSRLP_SECONDARY_SRVC);

      max_rlp_rate  = DSRLP_RATE_BLANK;

      if (txc_so.rev_dcch.included &&
          (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH))
      {
        /* R-DCCH assigned and RLP is secondary SO on R-DCCH
        */
        boolean control_hold = txtc.gating_rate != CAI_GATING_RATE_NONE;

        if (!control_hold)
        {
          /* MS not in control hold state
          */
          msg_on_channel = (txc_tx.active_msg && (txtc.cur_dsch == TXC_REV_DCCH));
          max_rlp_rate =
            txc_rlp_sec_data_rate (MCTRANS_PHY_CHAN_DCCH, msg_on_channel);
        }
      }
      txc_tx_process_fundich_data3g (MCTRANS_PHY_CHAN_DCCH,
                                     max_rlp_rate,
                                     DSRLP_SECONDARY_SRVC);

      /* The pointers : sec_fch_data_ptr and sec_dcch_data_ptr
      ** are set after call to txc_tx_frame_proc_build_data3g().
      ** These pointers are currently set in txc_tx_frame_proc()
      ** but this will be moved to txc_tx_frame_proc_build_data3g().
      */

      break;
    }

    default:
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "Unsupported SO %d as Sec Traffic",
        sec_service_op);
      break;
    }
  }
}

/*===========================================================================

FUNCTION TXC_RLP_SEC_DATA_RATE

DESCRIPTION
  This function determines the RLP secondary data rate based on the rate of
  primary traffic size and signaling traffic.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
dsrlp_rate_enum_type
txc_rlp_sec_data_rate
(
  word    phy_channel,
  boolean msg_on_channel
)
{
  dsrlp_rate_enum_type sec_rlp_rate = DSRLP_RATE_BLANK;

  if ((txc_get_pri_so() == CAI_SO_NULL) ||
      (!(txc_get_pri_phy_chan() & phy_channel)))
  {
    /* No primary traffic assigned  OR
    ** primary traffic assigned but not on this physical channel.
    ** Secondary only traffic on this physical channel.
    */
    if (msg_on_channel)
    {
      /* Signaling msg pending and signaling on this channel,
      ** BLANK secondary source
      */
      M1X_MSG( MUX, LEGACY_HIGH,
        "Signaling traffic. No Sec data");
      sec_rlp_rate = DSRLP_RATE_BLANK;
    }
    else
    {
      /* All bandwidth for data.
      */
      sec_rlp_rate = DSRLP_RATE_1;
    }
  }
  else if (txc_get_pri_phy_chan() & phy_channel)
  {
    /* Primary + Secondary traffic type assigned on this physical channel
    */
    if ((msg_on_channel) && (prim_rate != CAI_FULL_RATE))
    {
      /* Signaling msg pending and signaling on this channel,
      ** and primary traffic source returned < FULL rate, send Pri + sig,
      ** BLANK secondary source
      */
      M1X_MSG( MUX, LEGACY_HIGH,
        "Signaling traffic. No Sec data");
      sec_rlp_rate = DSRLP_RATE_BLANK;
    }
    else if (txc_rlp_sig_priority.delay_fr_cnt > 0)
    {
      /* Here if Vocoder has been dimmed but dim count is still non-zero.
      ** Do not allow RLP to send frames until dim count is zero so
      ** continous TX for RLP is guaranteed.
      */
      M1X_MSG( MUX, LEGACY_HIGH,
        "Awaiting continous TX. No Sec data");
      sec_rlp_rate = DSRLP_RATE_BLANK;
    }
    else
    {
      /* Find out secondary data rate based on primary traffic size.
      */
      switch (prim_rate)
      {
        case CAI_FULL_RATE:
          //MSG_HIGH_0("Voice FULL rate. No Sec data");
          sec_rlp_rate = DSRLP_RATE_BLANK;
          break;

        case CAI_HALF_RATE:
          sec_rlp_rate = DSRLP_RATE_1_2;
          break;

        case CAI_QUARTER_RATE:
          sec_rlp_rate = DSRLP_RATE_3_4;
          break;

#ifdef FEATURE_IS2000_1X_ADV
        case CAI_NON_CRITICAL_RATE:
#endif /* FEATURE_IS2000_1X_ADV */
        case CAI_EIGHTH_RATE:
          sec_rlp_rate = DSRLP_RATE_7_8;
          break;

        case CAI_BLANK_RATE:
          /* If primary service SO is voice, should not get here.
          */
          M1X_MSG( MUX, LEGACY_HIGH,
            "Pri SO %d. No Sig. Got Blank frame",
            txc_get_pri_so());
          sec_rlp_rate = DSRLP_RATE_1;
          break;

        case CAI_NULL_RATE:
        default:
          /* Do nothing */
          break;

      }
    }
  }

  return sec_rlp_rate;
}

/*===========================================================================

FUNCTION TXC_TDSO_SEC_FRAME_SIZE

DESCRIPTION
  This function determines the size of the TDSO data to be built
  when TDSO is assigned as Secondary traffic stream.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
word
txc_tdso_sec_frame_size
(
  word    phy_channel,
  boolean msg_on_channel
)
{
  word tdso_sec_size = 0;
  enc_rate_set_type fundich_rate_set = ENC_RATE_9600;
  if (phy_channel == MCTRANS_PHY_CHAN_FCH)
  {
    fundich_rate_set = txc_get_rev_link_rate_set(txc_so.rev_fch.rc);
  }
  else
  {
    fundich_rate_set = txc_get_rev_link_rate_set(txc_so.rev_dcch.rc);
  }

  if (msg_on_channel)
  {
    /* Signaling msg pending and signaling on this channel,
    ** BLANK secondary source
    */
    M1X_MSG( MUX, LEGACY_MED,
      "Signaling traffic. No Sec data");
    tdso_sec_size = 0;
  }
  else if ((txc_get_pri_so() == CAI_SO_NULL) ||
           (!(txc_get_pri_phy_chan() & phy_channel)))
  {
    /* No primary traffic assigned  OR
    ** primary traffic assigned but not on this physical channel.
    ** Secondary only traffic on this physical channel.
    ** All bandwidth for secondary source.
    */
    if (fundich_rate_set == ENC_RATE_9600)
    {
      tdso_sec_size = TDSO_RS1_SECONDARY_03;
    }
    else
    {
      tdso_sec_size = TDSO_RS2_SECONDARY_03;
    }
  }
  else if (txc_get_pri_phy_chan() & phy_channel)
  {
    /* primary traffic assigned on this physical channel
    */
    if (fundich_rate_set == ENC_RATE_9600)
    {
      if (prim_rate == CAI_FULL_RATE)
      {
        tdso_sec_size = 0;
      }
      else if (prim_rate == CAI_HALF_RATE)
      {
        tdso_sec_size = TDSO_RS1_SECONDARY_00;
      }
      else if (prim_rate == CAI_QUARTER_RATE)
      {
        tdso_sec_size = TDSO_RS1_SECONDARY_01;
      }
      else if (prim_rate == CAI_EIGHTH_RATE)
      {
        tdso_sec_size = TDSO_RS1_SECONDARY_02;
      }
      else if (prim_rate == CAI_BLANK_RATE)
      {
        tdso_sec_size = TDSO_RS1_SECONDARY_03;
      }
      else
      {
        // Remove this later.
        ONEX_ERR_FATAL ("Should not come here!", 0, 0, 0);
      }
    }
    else
    {
      if (prim_rate == CAI_FULL_RATE)
      {
        tdso_sec_size = 0;
      }
      else if (prim_rate == CAI_HALF_RATE)
      {
        tdso_sec_size = TDSO_RS2_SECONDARY_00;
      }
      else if (prim_rate == CAI_QUARTER_RATE)
      {
        tdso_sec_size = TDSO_RS2_SECONDARY_01;
      }
      else if (prim_rate == CAI_EIGHTH_RATE)
      {
        tdso_sec_size = TDSO_RS2_SECONDARY_02;
      }
      else if (prim_rate == CAI_BLANK_RATE)
      {
        tdso_sec_size = TDSO_RS2_SECONDARY_03;
      }
      else
      {
        // Remove this later.
        ONEX_ERR_FATAL ("Should not come here!", 0, 0, 0);
      }
    }
  }

  return tdso_sec_size;
}

#endif // FEATURE_IS2000_REL_A_SVD

/* <EJECT> */
/*===========================================================================

FUNCTION BUILD_HRATE_FRAME

DESCRIPTION
  This function builds a half rate frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void build_hrate_frame( void )
{
  word left;
  int  si_adj = 0;
          /* SI adjustment to account for the 1 bit difference.
             Default to 0 for P_REV < 7 */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* -------------------------------------------------------
  ** Set format bits to indicate 9600 bps, dim-and-bust with
  ** Rate 1/2 primary and signaling traffic frame
  ** ------------------------------------------------------- */
  txtc.frame.data[0] = 0x80;

  b_copy( prim_data_ptr, 0, txtc.frame.data, 4, CAI_HALF_RATE_BITS );

  if (!txc_tx.buf.pos)
  {
    /* Set bit to indicate start-of-message */
    b_packb( 0xFF, txtc.frame.data, 84, 1 );
  }
  else
  {
    /* Clear start-of-message bit */
    b_packb( 0x0, txtc.frame.data, 84, 1 );
  }

  /* Calculate how much of the message is left to send */
  left = txc_tx.buf.len - txc_tx.buf.pos;

  /* --------------------------
  ** Copy from message to frame
  ** -------------------------- */

  if (left >= (TC_DIM2 - 1 - si_adj )) /* Subtract SOM/SI bit */
  {
    b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos,
            txtc.frame.data, (word) (85 + si_adj), (word) (TC_DIM2 - 1 - si_adj ) );
  }
  else
  {
    b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos,
            txtc.frame.data, (word) (85 + si_adj), left );
  }

  /* Move index to next section of message */
  txc_tx.buf.pos += (TC_DIM2 - 1 - si_adj);//lint !e734 Loss of precision (assignment) (31 bits to 16 bits)

} /* build_hrate_frame */

/* <EJECT> */
/*===========================================================================

FUNCTION BUILD_BLANK_FRAME

DESCRIPTION
  This function builds a blank-and-burst signaling frame.  Used both for FCH and
  DCCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
enc_rate_type blank_rate;

byte build_blank_frame ( txc_dsch_type dsch )
{
  int left;
  byte val;
  enc_rate_type rate_index;
  uint8 *frame_ptr;
  int si_adj = 0;
    /* Length adjustment for SI bits, default to 0 for p_rev < 7 */
  enc_rate_set_type fundich_rate_set = ENC_RATE_9600;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if(txtc.sig_frame_delay)
  {
    /* Need to hold off sending any signaling for
       a few frames after a HHO. */
    if(txtc.nulls)
    {
      val = build_null_frame();
    }
    else
    {
      val = build_norm_frame();
    }

    return val;
  }

  /* Store FCH signalling type for frame being built */
  next_fch_sig_type     = BLANK;

  /* Initialize val to avoid compiler warning */
  if ( dsch == TXC_REV_FCH )
  {
    val = CAI_MUX1_REV_5_ID;
    frame_ptr = txtc.frame.data;
    fundich_rate_set = txc_get_rev_link_rate_set(txc_so.rev_fch.rc);
  }
  else
  {
    val = CAI_MUX1_REV_DCCH_5_ID;
    frame_ptr = txtc.dcch_frame.data;
    fundich_rate_set = txc_get_rev_link_rate_set(txc_so.rev_dcch.rc);
  }


  if ( fundich_rate_set == ENC_RATE_9600 )
  {
    /* ----------------------------------------------------------------
    ** Set format bits to indicate blank-and-burst and set SOM bit if
    ** this is the first bit of the message to be sent.
    ** -------------------------------------------------------------- */

    frame_ptr[0] = 0xB0 | ( txc_tx.buf.pos ? 0 : 8 );

    /* ----------------------------------------------------------------
    ** Calculate how much of the message is left to send in this frame.
    ** Copy from message to frame and subtract 1 from len for SOM bit.
    ** -------------------------------------------------------------- */

    //
    // Remove after DEBUG
    //
    if (txc_tx.buf.len < txc_tx.buf.pos)
    {
      ONEX_ERR_FATAL ("Signaling buf in bad state", 0, 0, 0);
    }

    left = MIN (txc_tx.buf.len - txc_tx.buf.pos, TC_BLANK - 1 );

    b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos, frame_ptr, (word) 5, (word) left );

    /* Move index to next section of message */
    txc_tx.buf.pos += TC_BLANK - 1;

    blank_rate = ENC_FULL_RATE;
  }

  else if (fundich_rate_set == ENC_RATE_14400)
  {
    /* ----------------------------------------------------------------
    ** Calculate how much of the message is left to send in this frame.
    ** -------------------------------------------------------------- */
    left = txc_tx.buf.len - txc_tx.buf.pos;

    /*
    ** Look for which rate frame we can squeeze this message into.
    ** If the signalling message is too large for one frame just
    ** use full rate frame for the first frame and catch the rest
    ** on the next frame.
    */
    if ( dsch == TXC_REV_FCH )
    {
      for ( rate_index = ENC_8TH_RATE; rate_index > ENC_FULL_RATE; rate_index-- )
      {

        if ( left < ( mux2_sig_blank_len[rate_index] - si_adj ) )
          break;
      }
      blank_rate = rate_index;
    }
    else
    {
      blank_rate = ENC_FULL_RATE;
    }

    /*
    ** Convert the blank frame rate chosen to Frame Mode value
    ** set erase bit = 0 ( to be filled in by traffic_isr, set MM_bit
    ** and set and shift frame mode bits appropriately
    */
    frame_ptr[0] = (uint8) (0 | MUX2_MM_BIT | mux2_sig_blank_fm [blank_rate]);

    /*
    ** Set SOM bit if this is the first bit of the message to be sent.
    */
    frame_ptr[0] |=
         (uint8) ( txc_tx.buf.pos ? 0 : mux2_sig_blank_som [blank_rate]);

    /*
    ** Calculate length of message left to copy into frame buffer.
    ** Copy it and move index to next section of message.
    */
    left = MIN (left, mux2_sig_blank_len [blank_rate] - si_adj );

    b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos, frame_ptr,
             mux2_sig_blank_pos [blank_rate] + 1 + (word) si_adj, (word) left );

    txc_tx.buf.pos += mux2_sig_blank_len [blank_rate] - si_adj;//lint !e734 Loss of precision (assignment) (31 bits to 16 bits)

    /* set frame categorization for FCH frame statistics */
    /* For DCCH, it's already set earlier */
    if ( dsch == TXC_REV_FCH )
    {
      val = (byte) MUX2_BLANK [blank_rate];
    }
    else
    {
      val = CAI_MUX2_REV_DCCH_5_ID;
    }
  }

  txtc.rate = blank_rate;
  return ( val );

} /* build_blank_frame */



/* <EJECT> */
/*===========================================================================

FUNCTION BUILD_NORM_FRAME

DESCRIPTION
  This function builds a primary traffic only frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint16 build_norm_frame ( void )
{
  uint16 val;

  /* Store FCH signalling type for frame being built */
  next_fch_sig_type = NORM;

  /* Initialize val to avoid compiler warning */
  val = CAI_MUX1_REV_1_ID;

  if ( txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_9600 )
  {
    switch (prim_rate)
    {
      case CAI_FULL_RATE:
        txtc.rate = ENC_FULL_RATE;
        val = CAI_MUX1_REV_1_ID;
        /* Copy vocoder data to our out buffer according to rate */
        b_copy( prim_data_ptr, 0, txtc.frame.data, 1,
                CAI_FULL_RATE_BITS );
        /* Buffer has already been set to 0 which forced
            the mixed mode bit to 0 */
        break;

      case CAI_HALF_RATE:
        txtc.rate = ENC_HALF_RATE;
        val = CAI_MUX1_REV_6_ID;
        /* Copy vocoder data to our out buffer according to rate */
        (void) memscpy( txtc.frame.data,
                        sizeof(txtc.frame.data),
                        prim_data_ptr,
                        CAI_HALF_RATE_BITS / 8 );
        break;

      case CAI_QUARTER_RATE:
        txtc.rate = ENC_QTR_RATE;
        val = CAI_MUX1_REV_7_ID;
        /* Copy vocoder data to our out buffer according to rate */
        (void) memscpy( txtc.frame.data,
                        sizeof(txtc.frame.data),
                        prim_data_ptr,
                        CAI_QUARTER_RATE_BITS / 8 );
        break;

      case CAI_EIGHTH_RATE:
        txtc.rate = ENC_8TH_RATE;
        val = CAI_MUX1_REV_8_ID;
        /* Copy vocoder data to our out buffer according to rate */
        (void) memscpy( txtc.frame.data,
                        sizeof(txtc.frame.data),
                        prim_data_ptr,
                        CAI_EIGHTH_RATE_BITS / 8 );
        break;

#ifdef FEATURE_IS2000_1X_ADV
      case CAI_NON_CRITICAL_RATE:
        if( txc_frame_is_guaranteed( qw_lo( txc_tx.frame_cnt ),
                                     txc_rcpm_data.rev_fch_blanking_dutycycle ) )
        {
          val = CAI_REV_NC_TX_ID;
        }
        else
        {
          val = CAI_REV_NC_BLANK_ID;
        }
        txtc.rate = ENC_8TH_RATE;
        /* Copy vocoder data to our out buffer according to rate */
        (void) memscpy( txtc.frame.data,
                        sizeof(txtc.frame.data),
                        prim_data_ptr,
                        CAI_EIGHTH_RATE_BITS / 8 );
        break;
#endif /* FEATURE_IS2000_1X_ADV */

      default:                /* Should not happen... */
        if (!txc_tx.tx_blank)
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Bad transmit rate %d",
            prim_rate );
        }
        val = build_null_frame ();

        break;
    } /* end switch (prim_rate) */
  }
  else if ( txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_14400 )
  {
    switch (prim_rate)
    {
      case CAI_FULL_RATE:
        txtc.rate = ENC_FULL_RATE;
        val = CAI_MUX2_REV_1_ID;
        /* Copy vocoder data to our out buffer according to rate */
        b_copy( prim_data_ptr, 0,
                txtc.frame.data, MUX2_PRI_POS, MUX2_FULL_PRI_LEN );
        /* Buffer has already been set to 0 which forced
            the mixed mode bit to 0 */
        break;

      case CAI_HALF_RATE:
        txtc.rate = ENC_HALF_RATE;
        val = CAI_MUX2_REV_11_ID;
        /* Copy vocoder data to our out buffer according to rate */
        b_copy( prim_data_ptr, 0,
                txtc.frame.data, MUX2_PRI_POS, MUX2_HALF_PRI_LEN );
        /* Buffer has already been set to 0 which forced
            the mixed mode bit to 0 */
        break;

      case CAI_QUARTER_RATE:
        txtc.rate = ENC_QTR_RATE;
        val = CAI_MUX2_REV_19_ID;
        /* Copy vocoder data to our out buffer according to rate */
        b_copy( prim_data_ptr, 0,
                txtc.frame.data, MUX2_PRI_POS, MUX2_QTR_PRI_LEN );
        /* Buffer has already been set to 0 which forced
            the mixed mode bit to 0 */
        break;

      case CAI_EIGHTH_RATE:
        txtc.rate = ENC_8TH_RATE;
        val = CAI_MUX2_REV_24_ID;
        /* Copy vocoder data to our out buffer according to rate */
        b_copy( prim_data_ptr, 0,
                txtc.frame.data, MUX2_PRI_POS, MUX2_EIGHTH_PRI_LEN );
        /* Buffer has already been set to 0 which forced
            the mixed mode bit to 0 */
        break;

      default:                /* Should not happen... */
        if (!txc_tx.tx_blank)
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Bad transmit rate %d",
            prim_rate );
        }

        val = build_null_frame ();
        break;
    } /* end switch (prim_rate) */
  }

  return ( val );
}

#ifdef FEATURE_IS2000_REL_A_SVD

uint16 build_norm_pri_sec_frame ( void )
{
  uint16 val;

  /* Initialize val to avoid compiler warning */
  val = CAI_MUX1_REV_1_ID;

  if (txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_9600)
  {
    if (prim_rate == CAI_HALF_RATE)
    {
      txtc.frame.data[0] = 0xC0; // 0x80
      val = CAI_MUX1_REV_11_ID;
    }
    else if (prim_rate == CAI_QUARTER_RATE)
    {
      txtc.frame.data[0] = 0xD0; // 0x90
      val = CAI_MUX1_REV_12_ID;
    }
    else if (prim_rate == CAI_EIGHTH_RATE)
    {
      txtc.frame.data[0] = 0xE0; // 0xA0
      val = CAI_MUX1_REV_13_ID;
    }
#ifdef FEATURE_IS2000_1X_ADV
    else if (prim_rate == CAI_NON_CRITICAL_RATE)
    {
      txtc.frame.data[0] = 0xE0; // 0xA0
      val = CAI_REV_NC_PRI_SEC_ID;
    }
#endif /* FEATURE_IS2000_1X_ADV */


    /* A Rate Set 1 frame with Pri+Sec traffic will always be FULL rate
    */
    txtc.rate = ENC_FULL_RATE;

    /* Copy vocoder data to out buffer
    */
    b_copy (prim_data_ptr, 0, txtc.frame.data, 4, prim_num_bits);

    /* Copy Secondary data to out buffer
    */
    b_copy (sec_fch_data_ptr, 0, txtc.frame.data,
            prim_num_bits + 4, sec_fch_num_bits);
  }
  else if (txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_14400)
  {
    switch (prim_rate)
    {
      case CAI_FULL_RATE:
        txtc.rate = ENC_FULL_RATE;
        val = CAI_MUX2_REV_1_ID;

        txtc.frame.data[0] = 0x00;

        /* Copy vocoder data to our out buffer according to rate */
        b_copy( prim_data_ptr, 0,
                txtc.frame.data, MUX2_PRI_POS, MUX2_FULL_PRI_LEN );
        /* Buffer has already been set to 0 which forced
            the mixed mode bit to 0 */
        break;

      case CAI_HALF_RATE:
        txtc.rate = ENC_FULL_RATE;
        val = CAI_MUX2_REV_6_ID; /* Header = "X101 00" */

        /* Set Mixed Mode bit and Frame Mode bits in buffer
        ** For Half rate Primary (124) + 138 Secondary traffic bits.
        */
        txtc.frame.data[0] = 0x50;

        /* Copy vocoder data to our out buffer according to rate */
        b_copy( prim_data_ptr, 0, txtc.frame.data, 6, MUX2_HALF_PRI_LEN );

        /* Copy RLP data to out buffer */
        b_copy( sec_fch_data_ptr, 0,
                txtc.frame.data, MUX2_HALF_PRI_LEN + 6, sec_fch_num_bits );
        break;

      case CAI_QUARTER_RATE:
        if (sec_fch_num_bits == 208)
        {
          val = CAI_MUX2_REV_7_ID; /* Header = "X101 01" */

          /* Set Mixed Mode bit and Frame Mode bits in buffer
          ** For Qtr rate Primary (54) + 208 Secondary traffic bits.
          */
          txtc.frame.data[0] = 0x54;
          txtc.rate = ENC_FULL_RATE;

          /* Copy vocoder data to our out buffer according to rate */
          b_copy (prim_data_ptr, 0, txtc.frame.data, 6, MUX2_QTR_PRI_LEN);

          /* Copy RLP data to out buffer
          */
          b_copy (sec_fch_data_ptr, 0,
                  txtc.frame.data, MUX2_QTR_PRI_LEN + 6, sec_fch_num_bits);
        }
        else if (sec_fch_num_bits == 67)
        {
          val = CAI_MUX2_REV_15_ID; /* Header = "X101 1" */

          /* Set Mixed Mode bit and Frame Mode bits in buffer
          ** For Qtr rate Primary (54) + 67 Secondary traffic.
          */
          txtc.frame.data[0] = 0x58;
          txtc.rate = ENC_HALF_RATE;

          /* Copy vocoder data to our out buffer according to rate */
          b_copy (prim_data_ptr, 0, txtc.frame.data, 5, MUX2_QTR_PRI_LEN);

          /* Copy RLP data to out buffer
          */
          b_copy (sec_fch_data_ptr, 0,
                  txtc.frame.data, MUX2_QTR_PRI_LEN + 5, sec_fch_num_bits);
        }
        else if (sec_fch_num_bits == 138)
        {
          val = CAI_MUX2_REV_7_ID; /* Header = "X101 01" */

          /* Set Mixed Mode bit and Frame Mode bits in buffer
          ** For Qtr rate Primary (54) + 208 Secondary traffic bits.
          */
          txtc.frame.data[0] = 0x54;
          txtc.rate = ENC_FULL_RATE;

          /* Copy vocoder data to our out buffer according to rate */
          b_copy (prim_data_ptr, 0, txtc.frame.data, 6, MUX2_QTR_PRI_LEN);

          /* Copy RLP data to out buffer
          */
          b_copy (sec_fch_data_ptr, 0,
                  txtc.frame.data, MUX2_QTR_PRI_LEN + 6, sec_fch_num_bits);
        }
        break;

      case CAI_EIGHTH_RATE:
        if (sec_fch_num_bits == 242)
        {
          val = CAI_MUX2_REV_8_ID; /* Header = "X101 10" */

          /* Set Mixed Mode bit and Frame Mode bits in buffer
          ** For Eighth rate Primary (20) + 242 Secondary traffic bits.
          */
          txtc.frame.data[0] = 0x58;
          txtc.rate = ENC_FULL_RATE;

          /* Copy vocoder data to our out buffer according to rate */
          b_copy (prim_data_ptr, 0, txtc.frame.data, 6, MUX2_EIGHTH_PRI_LEN);

          /* Copy RLP data to out buffer
          */
          b_copy (sec_fch_data_ptr, 0,
                  txtc.frame.data, MUX2_EIGHTH_PRI_LEN + 6, sec_fch_num_bits);
        }
        else if (sec_fch_num_bits == 138)
        {
          val = CAI_MUX2_REV_8_ID; /* Header = "X101 10" */

          /* Set Mixed Mode bit and Frame Mode bits in buffer
          ** For Eighth rate Primary (20) + 101 Secondary traffic bits.
          */
          txtc.frame.data[0] = 0x58;
          txtc.rate = ENC_FULL_RATE;

          /* Copy vocoder data to our out buffer according to rate */
          b_copy (prim_data_ptr, 0, txtc.frame.data, 6, MUX2_EIGHTH_PRI_LEN);

          /* Copy RLP data to out buffer
          */
          b_copy (sec_fch_data_ptr, 0,
                  txtc.frame.data, MUX2_EIGHTH_PRI_LEN + 6, sec_fch_num_bits);
        }
        else if (sec_fch_num_bits == 101)
        {
          val = CAI_MUX2_REV_16_ID; /* Header = "X110 0" */

          /* Set Mixed Mode bit and Frame Mode bits in buffer
          ** For Eighth rate Primary (20) + 101 Secondary traffic bits.
          */
          txtc.frame.data[0] = 0x60;
          txtc.rate = ENC_HALF_RATE;

          /* Copy vocoder data to our out buffer according to rate */
          b_copy (prim_data_ptr, 0, txtc.frame.data, 5, MUX2_EIGHTH_PRI_LEN);

          /* Copy RLP data to out buffer
          */
          b_copy (sec_fch_data_ptr, 0,
                  txtc.frame.data, MUX2_EIGHTH_PRI_LEN + 5, sec_fch_num_bits);
        }
        else if (sec_fch_num_bits == 32)
        {
          val = CAI_MUX2_REV_22_ID; /* Header = "X110" */

          /* Set Mixed Mode bit and Frame Mode bits in buffer
          ** For Eighth rate Primary (20) + 32 Secondary traffic bits.
          */
          txtc.frame.data[0] = 0x60;
          txtc.rate = ENC_QTR_RATE;

          /* Copy vocoder data to our out buffer according to rate */
          b_copy (prim_data_ptr, 0, txtc.frame.data, 4, MUX2_EIGHTH_PRI_LEN);

          /* Copy RLP data to out buffer
          */
          b_copy (sec_fch_data_ptr, 0,
                  txtc.frame.data, MUX2_EIGHTH_PRI_LEN + 4, sec_fch_num_bits);
        }
        break;

      default:                /* Should not happen... */
        if (!txc_tx.tx_blank)
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Bad transmit rate %d",
            prim_rate );
        }
        txtc.rate = ENC_8TH_RATE;
        val = CAI_MUX2_REV_24_ID;
        /* put null primary traffic into an 1800 bps frame */
        txtc.frame.data[0] = 0x3F;
        txtc.frame.data[1] = 0xFF;
        txtc.frame.data[2] = 0xFC;
        break;
    } /* end switch (prim_rate) */
  }

  return ( val );
}


uint16 build_sec_frame ( void )
{
  uint16 val;

  /* Initialize val to avoid compiler warning */
  val = CAI_MUX1_REV_1_ID;

  if (txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_9600)
  {
    /* A RS1 frame with Sec only traffic will always be FULL rate
    */
    txtc.rate = ENC_FULL_RATE;

    txtc.frame.data[0] = 0xF0;
    val = CAI_MUX1_REV_14_ID;

    /* Copy Secondary data to out buffer
    */
    b_copy (sec_fch_data_ptr, 0, txtc.frame.data, 4, sec_fch_num_bits);
  }
  else if (txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_14400)
  {
    cai_data_rate_type frame_rate;

    /* secondary only frame, explicitly set prim_num_bits = 0
    */
    prim_num_bits = 0;

    frame_rate = txc_det_frame_rate ();

    switch (frame_rate)
    {
      case CAI_FULL_RATE:
        val = CAI_MUX2_REV_9_ID;

        /* Header = "X101 11"
        */
        txtc.frame.data[0] = 0x5C;

        /* Copy Secondary data to out buffer
        */
        b_copy( sec_fch_data_ptr, 0, txtc.frame.data, 6, sec_fch_num_bits );

        txtc.rate = ENC_FULL_RATE;
        break;

      case CAI_HALF_RATE:
        val = CAI_MUX2_REV_17_ID;

        /* Set MM bit and FM bits in buffer for 121 Secondary traffic bits.
        ** Header = "X110 1XXX"
        */
        txtc.frame.data[0] = 0x68;

        /* Copy Secondary data to out buffer
        */
        b_copy (sec_fch_data_ptr, 0, txtc.frame.data, 5, sec_fch_num_bits);

        txtc.rate = ENC_HALF_RATE;
        break;

      case CAI_QUARTER_RATE:
        val = CAI_MUX2_REV_23_ID;

        /* Set MM bit and FM bits in buffer for 52 Secondary traffic bits.
        ** Header = "X111 XXXX"
        */
        txtc.frame.data[0] = 0x70;

        /* Copy Secondary data to out buffer
        */
        b_copy (sec_fch_data_ptr, 0, txtc.frame.data, 4, sec_fch_num_bits);

        txtc.rate = ENC_QTR_RATE;
        break;

      case CAI_EIGHTH_RATE:
        val = CAI_MUX2_REV_8_ID;

        /* Set MM bit and FM bits in buffer for 20 Secondary traffic bits.
        ** Header = "X1XX XXXX"
        */
        txtc.frame.data[0] = 0x40;

        /* Copy RLP data to out buffer
        */
        b_copy (sec_fch_data_ptr, 0, txtc.frame.data, 2, sec_fch_num_bits);

        txtc.rate = ENC_8TH_RATE;
        break;

      default:                /* Should not happen... */
        ONEX_ERR_FATAL ("Should not happen!!!", 0, 0, 0);
    } /* end switch (prim_rate) */
  }

  return ( val );
}
#endif // FEATURE_IS2000_REL_A_SVD

/* <EJECT> */
/*===========================================================================

FUNCTION BUILD_NULL_FRAME

DESCRIPTION
  This function builds a primary traffic 8th rate null frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
uint16 build_null_frame ( void )
{
  uint16 val;

  if( txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_14400)
  {
    /* put null primary traffic into an 1800 bps frame */
    txtc.frame.data[0] = 0x3F;
    txtc.frame.data[1] = 0xFF;
    txtc.frame.data[2] = 0xFC;
    val = CAI_MUX2_REV_24_ID;
  }
  else
  {
    /* put null primary traffic into an 1200 bps frame */
    txtc.frame.data[0] = 0xFF;
    txtc.frame.data[1] = 0xFF;
    val = CAI_MUX1_REV_8_ID;
  }
  txtc.rate = ENC_8TH_RATE;

  /* Store signalling frame type for frame being built */
  next_fch_sig_type = NORM;

  return val;
}
/* <EJECT> */
/*===========================================================================

FUNCTION BUILD_DIM_FRAME

DESCRIPTION
  This function builds a dim-and-burst frame.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

uint16 build_dim_frame ( void )
{
  uint16 val;
  byte som;
  word left;
  int si_adj = 0;
          /* Adjustment for SI bit, default to 0 for p_rev < 7 */

  if(txtc.sig_frame_delay)
  {
    /* Need to hold off sending any signaling for
       a few frames after a HHO. */
    if(txtc.nulls)
    {
      val = build_null_frame();
    }
    else
    {
      val = build_norm_frame();
    }

    return val;
  }

  /* Store FCH signalling type for frame being built */
  next_fch_sig_type = DIM;

  /* Initialize val to avoid compiler warning */
  val = CAI_MUX1_REV_1_ID;

  if ( txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_9600 )
  {
    txtc.rate = ENC_FULL_RATE;       /* Full rate */

    switch (prim_rate)
    {
      case CAI_HALF_RATE:
        build_hrate_frame();
        val = CAI_MUX1_REV_2_ID;
        break;

      case CAI_QUARTER_RATE:
        /* -------------------------------------------------------
        ** Set format bits to indicate 9600 bps, dim-and-bust with
        ** Rate 1/4 primary and signaling traffic frame
        ** ------------------------------------------------------- */
        txtc.frame.data[0] = 0x90;

        b_copy( prim_data_ptr, 0, txtc.frame.data, 4,
                CAI_QUARTER_RATE_BITS );

        if (!txc_tx.buf.pos)
        {
          /* Set bit to indicate start-of-message */
          b_packb( 0xFF, txtc.frame.data, 44, 1 );
        }
        else
        {
          /* Clear start-of-message bit */
          b_packb( 0x0, txtc.frame.data, 44, 1 );
        }

        //
        // Remove after DEBUG
        //
        if (txc_tx.buf.len < txc_tx.buf.pos)
        {
          ONEX_ERR_FATAL ("Signaling buf in bad state", 0, 0, 0);
        }

        /* Calculate how much of the message is left to send */
        left = txc_tx.buf.len - txc_tx.buf.pos;

        /* --------------------------
        ** Copy from message to frame
        ** -------------------------- */

        if (left >= (TC_DIM4 - 1 - si_adj )) /* Subtract SOM/SI bit */
        {
          b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos,
                  txtc.frame.data, (word) (45 + si_adj), (word) (TC_DIM4 - 1 - si_adj) );
        }
        else
        {
          b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos,
                  txtc.frame.data, (word) (45 + si_adj), left );
        }

        /* Move index to next section of message */
        txc_tx.buf.pos += (TC_DIM4 - 1 - si_adj);//lint !e734 Loss of precision (31 bits to 16 bits)

        val = CAI_MUX1_REV_3_ID;
        break;

#ifdef FEATURE_IS2000_1X_ADV
      case CAI_NON_CRITICAL_RATE:
#endif /* FEATURE_IS2000_1X_ADV */
      case CAI_EIGHTH_RATE:
        /* -------------------------------------------------------
        ** Set format bits to indicate 9600 bps, dim-and-bust with
        ** Rate 1/8 primary and signaling traffic frame
        ** ------------------------------------------------------- */
        txtc.frame.data[0] = 0xA0;

        b_copy( prim_data_ptr, 0, txtc.frame.data, 4,
                CAI_EIGHTH_RATE_BITS );

        if (!txc_tx.buf.pos)
        {
          /* Set bit 21 to indicate start-of-message */
          b_packb( 0xFF, txtc.frame.data, 20, 1 );
        }
        else
        {
          /* Clear start-of-message bit */
          b_packb( 0x0, txtc.frame.data, 20, 1 );
        }

        /* Calculate how much of the message is left to send */
        left = txc_tx.buf.len - txc_tx.buf.pos;

        /* --------------------------
        ** Copy from message to frame
        ** -------------------------- */

        if (left >= (TC_DIM8 - 1 - si_adj )) /* Subtract SOM/SI bit */
        {
          b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos,
                  txtc.frame.data, (word) (21 + si_adj), (word) (TC_DIM8 - 1 - si_adj) );
        }
        else
        {
          b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos,
                  txtc.frame.data, (word) (21 + si_adj), left );
        }

        /* Move index to next section of message */
        txc_tx.buf.pos += (TC_DIM8 - 1 - si_adj);//lint !e734 Loss of precision (31 bits to 16 bits)

#ifdef FEATURE_IS2000_1X_ADV
        if(prim_rate == CAI_NON_CRITICAL_RATE)
        {
          val = CAI_REV_NC_PRI_SIG_ID;
        }
        else
#endif /* FEATURE_IS2000_1X_ADV */
        {
          val = CAI_MUX1_REV_4_ID;
        }
        break;

      default:
        /* Must be a blank - build a blank and burst */
        if (!txc_tx.tx_blank)
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Invalid voc rate %d",
            prim_rate );
        }
        val = build_blank_frame( TXC_REV_FCH );
        break;
    } /* switch */
  }
  else if ( txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_14400 )
  {
    uint8 dim_rate;
    uint8 pri_rate;
    uint8 rate_index;

    /* Translate primary traffic rate to an encoder rate type */
    if( prim_rate <= CAI_FULL_RATE )
    {
      pri_rate = (uint8) mux_cai_to_enc [prim_rate];
    }
    else
    {
  #ifdef FEATURE_IS2000_1X_ADV
      if(prim_rate == CAI_NON_CRITICAL_RATE)
      {
        pri_rate = (uint8) ENC_8TH_RATE;
      }
      else
  #endif /* FEATURE_IS2000_1X_ADV */
      {
        pri_rate = (uint8) mux_cai_to_enc [CAI_BLANK_RATE];
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid prim_rate (%d). Defaulting to BLANK_RATE.",
          (int) prim_rate);
      }
    }

    /* Range check pri_rate */
    if(pri_rate >= (uint8) ENC_NUM_FRAC_RATES)
    {
      pri_rate = 0;
      M1X_MSG( MUX, LEGACY_ERROR,
        "Invalid pri_rate %d, defaulting to full",
        pri_rate);
    }

    /* ----------------------------------------------------------------
    ** Calculate how much of the message is left to send in this frame.
    ** -------------------------------------------------------------- */
    left = txc_tx.buf.len - txc_tx.buf.pos;

    /*
    ** Look for which rate frame we can squeeze this message into.
    ** If the signalling message is too large for one frame just
    ** use full rate frame for the first frame and catch the rest
    ** on the next frame.
    */

    for (rate_index = (uint8) ENC_QTR_RATE; rate_index > (uint8) ENC_FULL_RATE; rate_index-- )
    {
      if ( left < (mux2_sig_dim_len [rate_index] [pri_rate] - si_adj) )
        break;
    }

    dim_rate = rate_index;

    /*
    ** Convert the blank frame rate chosen to Frame Mode value
    ** set erase bit = 0 ( to be filled in by traffic_isr, set MM_bit
    ** and set and shift frame mode bits appropriately
    */
    txtc.frame.data[0] = (byte)
       (0 | MUX2_MM_BIT | mux2_sig_dim_fm [dim_rate] [pri_rate]);

    /*
    ** Fill the primary traffic into the primary part of the frame.
    */
    b_copy( prim_data_ptr, 0, txtc.frame.data,
            mux2_pri_dim_pos [dim_rate] [pri_rate],
            mux2_pri_dim_len [dim_rate] [pri_rate] );

    /*
    ** Set SOM bit if this is the first bit of the message to be sent.
    */
    som = ( txc_tx.buf.pos ? 0 : 0xFF );

    b_packb( som, txtc.frame.data,
             mux2_sig_dim_pos [dim_rate] [pri_rate], (word) (1 + si_adj) );

    /*
    ** Calculate length of message left to copy into frame buffer.
    ** Copy it and move index to next section of message.
    */
    left = (word) MIN (left, mux2_sig_dim_len [dim_rate] [pri_rate] - si_adj );

    b_copy( txc_tx.buf.msg, (word) txc_tx.buf.pos, txtc.frame.data,
             (word) (mux2_sig_dim_pos [dim_rate][pri_rate] + 1 + si_adj), left );

    txc_tx.buf.pos += mux2_sig_dim_len [dim_rate][pri_rate] - si_adj;//lint !e734 Loss of precision (assignment) (31 bits to 16 bits)

    txtc.rate = (enc_rate_type) dim_rate;

    /* set frame categorization for frame statistics */
    val = (byte) MUX2_DIM [dim_rate] [pri_rate];
  }
  return ( val );
}

/*===========================================================================

FUNCTION TXC_BUILD_FRAME

DESCRIPTION
  This function builds the next frame to be transmitted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void txc_build_frame
(
  txc_sig_frame_type  signext
)
{
  static    uint16    val;          /* value for mux1 logging */
  qword sys_time_in_20ms;         /* system time in 20ms units */

  boolean pri_so_is_voice = CAI_IS_SO_VOICE(txc_get_pri_so());

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (txc_tx.tx_on)
  {

    /* Encrypt voice, if feature is enabled.  If feature is not enabled,
    ** buffer will remain unchanged.
    */
    if ( (pri_so_is_voice) && (prim_data_ptr) )
    {
      m1x_time_get_20ms_frame_time (sys_time_in_20ms);  /* for encryption */

      #ifdef FEATURE_1X_CUST_VOICE_SO
      /* Use customer specific algorithm for customer specific voice SO
       * if the algorithm has been registered */
      if ( (txc_get_pri_so() == mcc_get_cust_so()) &&
           (mcc_get_cust_so() != CAI_SO_NULL ) &&
           (cust_voice_encrypt_algorithm != NULL ) )
      {
        TX_DBG_MSG("Using cust encrypt API, SO:%d",
              mcc_get_cust_so());

        cust_voice_encrypt_algorithm(
          sys_time_in_20ms,                 /* system time */
          prim_data_ptr,                    /* pointer to buffer to encrypt */
          0,                                /* buffer offset */
          (uint32) prim_num_bits            /* number of bits to encrypt */
        );
      }
      else
      #endif /* FEATURE_1X_CUST_VOICE_SO */
      {
        mccdma_encrypt_voice_frame (
          sys_time_in_20ms,                 /* system time */
          prim_data_ptr,                    /* pointer to buffer to encrypt */
          0,                                /* buffer offset */
          (uint32) prim_num_bits            /* number of bits to encrypt */
        );
      }
    } /* if ( (pri_so_is_voice) && (prim_data_ptr) ) */

#ifdef FEATURE_IS2000_1X_ADV
    if( txc_so.rev_fch.rc == CAI_RC_8 )
    {
      if ( (prim_rate == CAI_NON_CRITICAL_RATE) || (txtc.nulls) )
      {
        txtc.rc8_status.is_non_critical_frame = TRUE;
      }
      else
      {
        txtc.rc8_status.is_non_critical_frame = FALSE;
      }
    }
#endif /* FEATURE_IS2000_1X_ADV */

    switch (signext)
    {
      case BLANK:
        if ( (prim_rate == CAI_BLANK_RATE) || (prim_data_ptr == NULL) )
        {
          txtc.dim_delay = TXC_VOC_DIM_DELAY;
          val = build_blank_frame( TXC_REV_FCH );
        }
        else if (prim_rate == CAI_FULL_RATE)
        {
          /* The vocoder didn't give us a blank frame! */

          if (txtc.dim_delay)
          {
            txtc.dim_delay--;
            val = build_norm_frame ();
          }
          else
          {
            if ( pri_so_is_voice )
            {
              M1X_MSG( MUX, LEGACY_ERROR,
                "Vocoder rate error" );
            }
            val = build_blank_frame( TXC_REV_FCH );
          }
        } /* end if (prim_rate == CAI_FULL_RATE) */
        else
        {
          #ifdef FEATURE_IS2000_REL_A_SVD
          if (txc_get_pri_so() == CAI_SO_NULL)
          {
            /* Secondary only traffic, blank secondary data
            */
            val = build_blank_frame( TXC_REV_FCH );
          }
          else
          #endif /* FEATURE_IS2000_REL_A_SVD */
          {
            val = build_dim_frame();
          }
        }

        break;

      case DIM:

        if ( (txtc.nulls) || (prim_data_ptr == NULL) )
        {
          /* Just switched to nulls - do blank-and-burst signaling */
          val = build_blank_frame( TXC_REV_FCH );
        }
        else
        {
          if ((prim_rate == CAI_FULL_RATE) || (prim_rate == CAI_BLANK_RATE))
          {
            /* The vocoder didn't give us a dimmed frame! */

            if (txtc.dim_delay)
            {
              txtc.dim_delay--;
              val = build_norm_frame();
            }
            else
            {
              if (pri_so_is_voice)
              {
                M1X_MSG( MUX, LEGACY_ERROR,
                  "Vocoder rate error" );
              }
              val = build_blank_frame( TXC_REV_FCH );
            }
          }
          else
          {
            #ifdef FEATURE_IS2000_REL_A_SVD
            if (txc_get_pri_so() == CAI_SO_NULL)
            {
              /* Secondary only traffic, BLANK secondary data */
              val = build_blank_frame( TXC_REV_FCH );
            }
            else
            #endif /* FEATURE_IS2000_REL_A_SVD */
            {
              txtc.dim_delay = 3;
              val = build_dim_frame ();
            }
          }
        }
        break;

      case NORM:

        if( txtc.preambles )
        {
          val = (txc_get_rev_link_rate_set(txc_so.rev_fch.rc) == ENC_RATE_14400) ?
                CAI_MUX2_REV_1_ID : CAI_MUX1_REV_1_ID;
          txtc.rate = ENC_FULL_RATE;

          /* No FCH/DCCH during RC3/4 preamble */
          if(
             txc_so.rev_dcch.included ||
             (txc_so.rev_fch.included && (txc_so.rev_fch.rc >= CAI_RC_3))
          )
          {
            enc_fch_enable( TRUE );

            if( txc_hho_preamble_pcg_units == TRUE )
            {
              ASSERT( txtc.num_preambles <= 16 );

               /* We want num_preambles 1's as the pilot gating pattern */
              M1X_MSG( MUX, LEGACY_HIGH,
                "Start %d-PCG preamble",
                txtc.num_preambles);
              enc_is95c_tx_patn
              (
                 (enc_gating_pattern_type) PREAMBLES_TO_GATING_PATN( txtc.num_preambles )
              );
              enc_gating_mode( ENC_HHO_PREAMBLE_MODE );

              /* In RC3/4, preamble can only be 1 frame long in the maximum.  So set
                 num_preambles here to 0 but don't set preambles here to FALSE yet.
                 We need to defer that until the preamble is sent.  After that, we need
                 to disable this pattern and enable FCH. */
              txtc.num_preambles = 0;
            }
            else
            {
              txtc.num_preambles--;
            }
          }
          else if ((--txtc.num_preambles) == 0)
          {
            txtc.preambles = FALSE;
          }
        }
        else if( txtc.nulls )
        {
          val = build_null_frame ();
          txc_data_rate = CAI_EIGHTH_RATE;
        }
        else
        {
          #ifdef FEATURE_IS2000_REL_A_SVD
          if ((txc_get_pri_phy_chan() & MCTRANS_PHY_CHAN_FCH) &&
              (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_FCH))
          {
            /* both primary and secondary traffic type assigned on R-FCH */
            if (prim_data_ptr && sec_fch_data_ptr)
            {
              if ((prim_rate == CAI_BLANK_RATE) ||
                  (prim_rate == CAI_NULL_RATE)  ||
                  (sec_fch_num_bits == 0))
              {
                M1X_MSG( MUX, LEGACY_ERROR,
                  "Incorrect Prim/Sec rate, "
                  "prim_rate=%d, sec_fch_num_bits=%d",
                  prim_rate,
                  sec_fch_num_bits);
                val = build_null_frame ();
                txc_data_rate = CAI_EIGHTH_RATE;
              }
              else
              {
                /* Pri + Sec traffic this frame */
                val = build_norm_pri_sec_frame ();
              }
            }
            else if (prim_data_ptr)
            {
              if ((prim_rate == CAI_BLANK_RATE) ||
                  (prim_rate == CAI_NULL_RATE)  ||
                  (sec_fch_num_bits != 0))
              {
                M1X_MSG( MUX, LEGACY_ERROR,
                  "Incorrect Prim/Sec rate, "
                  "prim_rate=%d, sec_fch_num_bits=%d",
                  prim_rate,
                  sec_fch_num_bits);
                val = build_null_frame ();
                txc_data_rate = CAI_EIGHTH_RATE;
              }
              else
              {
                /* Pri only traffic this frame */
                val = build_norm_frame ();
              }
            }
            else if (sec_fch_data_ptr)
            {
              if ((prim_rate != CAI_BLANK_RATE) ||
                  (sec_fch_num_bits == 0))
              {
                M1X_MSG( MUX, LEGACY_ERROR,
                  "Incorrect Prim/Sec rate, "
                  "prim_rate=%d, sec_fch_num_bits=%d",
                  prim_rate,
                  sec_fch_num_bits);
                val = build_null_frame ();
                txc_data_rate = CAI_EIGHTH_RATE;
              }
              else
              {
                /* Sec only traffic this frame */
                val = build_sec_frame ();
              }
            }
            else
            {
              /* prim_data_ptr and sec_fch_data_ptr is NULL. Why? */
              M1X_MSG( MUX, LEGACY_ERROR,
                "Pri and Sec ptrs == NULL");
              val = build_null_frame ();
              txc_data_rate = CAI_EIGHTH_RATE;
            }
          }
          else if (txc_get_pri_phy_chan() & MCTRANS_PHY_CHAN_FCH)
          {

            /* No secondary traffic type assigned on R-FCH */
            if (prim_data_ptr)
            {
              /* build Primary only frame */
              val = build_norm_frame ();
            }
            else
            {
              M1X_MSG( MUX, LEGACY_ERROR,
                "prim_data_ptr is NULL, build NULL frame");
              val = build_null_frame ();
              txc_data_rate = CAI_EIGHTH_RATE;
            }
          }
          else if (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_FCH)
          {
            /* No primary traffic type assigned on R-FCH */

            if (sec_fch_data_ptr)
            {
              if (sec_fch_num_bits == 0)
              {
                M1X_MSG( MUX, LEGACY_ERROR,
                  "No primary traffic and sec_fch_num_bits == 0");
                val = build_null_frame ();
                txc_data_rate = CAI_EIGHTH_RATE;
              }
              else
              {
                val = build_sec_frame ();
              }
            }
            else
            {
              /* No Primary service, no Secondary data. Send NULL frame. */
              val = build_null_frame ();
              txc_data_rate = CAI_EIGHTH_RATE;
            }
          }
          else
          {
            /* FCH assigned but no Primary, Secondary service on FCH.
            ** Send NULL frame.
            */
            val = build_null_frame ();
            txc_data_rate = CAI_EIGHTH_RATE;
          }
          #else /* !FEATURE_IS2000_REL_A_SVD */
          if (prim_data_ptr)
          {
            val = build_norm_frame ();
          }
          else
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "prim_data_ptr is NULL, build NULL frame");
            val = build_null_frame ();
            txc_data_rate = CAI_EIGHTH_RATE;
          }
          #endif /* !FEATURE_IS2000_REL_A_SVD */
        }
        break;

      default:
      {
        ONEX_ERR_FATAL( "Invalid signaling mode %d", (int) signext, 0, 0 );
      }
    } /* switch mode */

    if(txtc.sig_frame_delay > 0)
    {
      M1X_MSG( MUX, LEGACY_MED,
        "Delaying signaling frame, Delay = %d",
        txtc.sig_frame_delay);

      /* Need to hold off sending any signaling for
         a few frames after a HHO. */
      txtc.sig_frame_delay--;

      if( txtc.sig_frame_delay == 0)
      {
        /* Reset the frame delay value to the native 1x TC value. This is
           to cover the case where we came from TT and may do a normal HHO later
           on. */
        txc_sig_frame_delay_value = txc_sig_frame_delay_for_tc_value;
      }
    }
  }
  else
  {
    val = build_null_frame ();
    txc_data_rate = CAI_EIGHTH_RATE;
  }

  txc_log_mux1( val );             /* log the MUX1 data */

  mar_log_tx ( val );              /* log Markov stats */

  /* Whenever MUX ID value i.e. val is more than CAI_LAST_PARM_ID,
  ** Legacy equivalent of MUX ID is logged to avoid the truncation
  ** of MUX ID's which leads to wrong logging
  */
  switch(val)
  {
    case CAI_REV_NC_TX_ID:
      txtc.rev_mux = CAI_MUX1_REV_8_ID;
      break;

    case CAI_REV_NC_BLANK_ID:
      txtc.rev_mux = CAI_MUX1_REV_8_ID;
      break;

    case CAI_REV_NC_PRI_SEC_ID:
      txtc.rev_mux = CAI_MUX1_REV_13_ID;
      break;

    case CAI_REV_NC_PRI_SIG_ID:
      txtc.rev_mux = CAI_MUX1_REV_4_ID;
      break;

    default:
      txtc.rev_mux = val;              /* store for RDCH logging */
      break;
  }

  if (txc_tx.tx_blank)   /* one blank frame delay */
  {
    txc_tx.tx_blank = FALSE;
  }

  muxvoc_reset_tx_voice_to_silence();
}


#ifdef FEATURE_IS2000_REL_A_SVD
/*===========================================================================

FUNCTION TXC_CHECK_FOR_RLP_SIGNALING

DESCRIPTION
  Check with RLP to find out if signaling needs to be sent.

  Note: this should only be done if no L3 signaling is present.

DEPENDENCIES
  None

RETURN VALUE
  Boolean indicating if RLP signaling needs to be Txd.

SIDE EFFECTS
  None

===========================================================================*/
boolean txc_check_for_rlp_signaling(void)
{
  boolean rlp_need_tx = FALSE;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (
      (txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G) &&
      (!(txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH))
     )
  {
    /* dsrlp_any_sig_to_send() returns TRUE if RLP has signalling to send.
    ** Special handling is required when txc_rlp_sig_priority.curr_fr_val
    ** is:
    **
    ** DSRLP_TX_CONTINUOUS:    Initial delay OK then continuous TX
    **
    ** This action takes a minimum delay of 3 frames plus the frames
    ** required to transmit the RLP signalling sequence (1 frame or
    ** multiple frames, unknown to the MUX layer).
    */
    rlp_need_tx = dsrlp_any_sig_to_send (&txc_rlp_sig_priority.curr_fr_val);
    if (rlp_need_tx == TRUE)
    {
      /* Signaling required. Vocoder will be requested to Dim.
      */
      if( txc_rlp_sig_priority.delay_fr_cnt != 0)
      {
        txc_rlp_sig_priority.delay_fr_cnt--;
      }
      else if (
               (txc_rlp_sig_priority.curr_fr_val == DSRLP_TX_CONTINUOUS) &&
               (txc_rlp_sig_priority.curr_fr_val != txc_rlp_sig_priority.prev_fr_val)
              )
      {
        txc_rlp_sig_priority.delay_fr_cnt = TXC_VOC_DIM_DELAY;
      }

      M1X_MSG( MUX, LEGACY_MED,
        "RLPSigDimVoc: dly=%d,cur=%d,prv=%d",
        txc_rlp_sig_priority.delay_fr_cnt,
        txc_rlp_sig_priority.curr_fr_val,
        txc_rlp_sig_priority.prev_fr_val );
    }
    else
    {
      txc_rlp_sig_priority.delay_fr_cnt = 0;
      txc_rlp_sig_priority.curr_fr_val = DSRLP_TX_NONE;

    } /* end if (rlp_need_tx == TRUE) */

    /* Set current to previous.
    **  - In case RLP returns TRUE, if we don't set current to previous,
    **    then we set delay frame count to TXC_VOC_DIM_DELAY
    **    on the frame which delay frame count is decremented to zero.
    **  - In case RLP returns FALSE, current frame value is reset to NONE so
    **    previous should be reset to NONE as well.
    */
    txc_rlp_sig_priority.prev_fr_val = txc_rlp_sig_priority.curr_fr_val;

  } /* end if (txc_get_sec_so() == CAI_SO_PPP_PKT_DATA_3G) */

  return (rlp_need_tx);
}
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

FUNCTION TXC_SIG_FITS_IN_FRAME

DESCRIPTION
  Check if the remaining signalling payload will fit in the available
  space of the current voice frame.

DEPENDENCIES
  Intended for use during voice traffic when Dim and Burst is used.

RETURN VALUE
  Boolean indicating if the signaling payload will fit.
  True if yes, false otherwise.

SIDE EFFECTS
  None

===========================================================================*/
boolean txc_sig_fits_in_frame(cai_data_rate_type frame_rate)
{
  boolean sig_fits = FALSE;
  word sig_left;
  word space_avail;

  /*-----------------------------------------------------------------------*/

  /* Determine the remaining signalling payload. */
  sig_left = txc_tx.buf.len - txc_tx.buf.pos;

  /* Based on the curernt rate, determine the
     available space in the frame. */
  switch(frame_rate)
  {
    case CAI_HALF_RATE:
      space_avail = TC_DIM2;
      break;

    case CAI_QUARTER_RATE:
      space_avail = TC_DIM4;
      break;

    case CAI_EIGHTH_RATE:
      space_avail = TC_DIM8;
      break;

    default:
      space_avail = 0;
      break;
  }

  /* Check if remaining signalling will fit in
     the available space of this frame. */
  if(sig_left <= space_avail)
  {
    sig_fits = TRUE;
  }

  return (sig_fits);
}