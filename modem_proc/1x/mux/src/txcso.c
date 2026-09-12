/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

   C D M A   T R A N S M I T   S E R V I C E   O P T I O N S   M O D U L E

GENERAL DESCRIPTION
  Provides an interface to service option specific functions

EXTERNALIZED FUNCTIONS
  txc_voc_traffic - Get traffic data (primary or secondary) from the vocoder
  txc_loopback_traffic -  Get loopback data bits (primary only) from the
  Loopback Service Option layer.
  txc_mar_traffic - Get traffic data (primary or secondary) from Markov frame
  error rate tests
  txc_tdso_traffic - Get traffic data (primary) from TDSO

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

  Copyright (c) 1991 - 2013 Qualcomm Technologies, Inc. 
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

$PVCSPath: O:/src/asw/COMMON/vcs/txcso.c_v   1.16   24 Jul 2002 14:49:40   hkulkarn  $
$Header: //components/rel/1x.mpss/8.0/mux/src/txcso.c#1 $ $DateTime: 2019/06/11 23:51:40 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/24/12   srk     Mainlined FEATURE_IS2000_P2
01/17/12   jtm     Revert REL_A feature clean up.
01/04/12   srk     Feature Cleanup.
05/20/11   jtm     Protect changes to SO and physical channel.
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
01/25/11   jtm     Added Mux Vocoder interface module.
12/17/10   jtm     Mainlined FEATURE_MVS_MIGRATE under FEATURE_MODEM_1X_VOICE_SUPPORT.
12/14/10   jtm     Featurized include of voc.h.
12/02/10   jtm     Added the capability to track the synchronization
                   of MVS and TXC and report errors.
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
10/30/10   jtm     Modem 1x SU API changes.
10/30/10   jtm     Integrate from rel/1h10.
08/10/10   vks     Cleanup Genesis Revisit feature.
08/06/10   jtm     Fixed unnecessary call to get the RL rate set for R-FCH when
                   DCCH is assigned instead during TDSO calls.
                   Added 1x Advanced support when REL_A is not enabled.
06/24/10   jtm     Use CAI_IS_SO_VOICE to check for valid voice service options.
04/12/10   jtm     1x Advanced: Corrected non-critical frame processing.
03/25/10   vks     Remove Geneis Revisit feature around MVS calls.
02/04/10   jtm     Removed FEATURE_PLT.
11/24/09   jtm     Moved dtx_non_crit_fch_frame to txtc structure.
                   Added dtx logic for markov testing.
10/27/09   jtm     Mux support for MSO74 and SO73.
09/23/09   jtm     Changed macro ENC_RC_TO_RATE_SET to become a function.
09/14/09   adw     Changes to support modem data header CMI refactoring.
09/11/09   mca     Temporary changes to get 9k linking
08/13/09   jtm     Removed FEATURE_1X_DISABLE_CMI.
08/05/09   jtm     Lint fixes.
08/04/09   jtm     Replaced calls to deprecated BIO funcs with new DAL API.
04/17/09   jtm     Added includes to fix compiler warnings.
04/13/09   adw     Replaced tdso.h with tdso_v.h include for CMI.
04/02/09   jtm     Added featurization (FEATURE_MODEM_1X_MVS_API_GEN2) to allow
                   support of the legacy MVS api required for 6x95. Generation 1
                   is legacy, generation 2 is 8650B.
03/31/09   jtm     Eliminated implicit includes.
03/26/09   jtm     Mainlined T_MSM8650B MVS support.
03/26/09   jtm     Added include for modem_1x_defs.h
03/03/09   jtm     Lint fixes.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
09/24/08   vlc     Call mvssup_voc_set_frame_rate() instead of
                   mvs_voc_set_frame_rate for older targets.
09/12/08   vlc     Fixed a typo.
09/12/08   vlc     Added support for FEATURE_MVS_MIGRATE.
07/21/08   jtm     Removed FEATURE_IS2000_REL_C_DV code as well as txc_voc_lrate
                   and txc_voc_hrate. These were replaced with defined constants.
06/04/08   vlc     Fixed post-increment issue in function txc_voc_traffic()
                   for FEATURE_JCDMA.
07/25/05   rkc     Fixed lint errors.
09/28/04   sr      Merged in changes from 6550 2009 build
07/30/04   vlc     Added support for rel C TDSO.
07/29/04   jrp     Properly checked return value of voc_frame_cmd.
06/16/04   sr      Merged in changes from 6500 5004 build
04/27/04   vlc     Following changes were done in function txc_voc_traffic(),
                   ret_val was renamed to rlp_need_tx for clarity sake, F3
                   message was moved to include more information, and if
                   rlp_need_tx is TRUE, do not increase vocoder bandwidth for
                   next frame.
04/20/04   jrp     Fixed lint warnings.
08/25/03   vlc     Removed shared rate_set from txc_so (txc_so_type) to avoid
                   confusion between FCH and DCCH rate sets.  Now calculate
                   rate set from RC as it is needed.
04/29/03   vlc     As an enhancement, do not dim vocoder for RLP signalling
                   frames if DCCH is also mapped for RLP data.
03/28/03   sy/vlc  Added logic to support different transmit schemes for
                   different types of RLP signalling frames in an SVD call.
07/24/02   hrk     Added support for VP2.
                   Introduced new function txc_tdso_sec_traffic().
05/15/02   bn      Initialize the FCH TDSO frame size correctly
04/04/02   hrk     Removed capping MAX_RATE to 1/2 in SVD. Implemented new algo.
03/15/02   hrk     Reducing vocoder MAX RATE if RLP has signalling to send.
02/27/02   hrk     Added SVD support.
06/15/01   day     Mainlined FEATURE_IS2000
                   Delete VOC_FULL_ONLY
06/07/01   kk      Mixed RS support for TDSO interface added.
05/18/01   kk      RS2 TDSO support.
05/11/01   fc      Added support for DCCH STDSO.
04/20/01   sr      Merged in from MSM5100 archive
04/18/01   sr      Merged with MSM_MUX1X.01.00.00
           hrk     Added support for FEATURE_IS2000_P2
03/20/01   ych     Merged JCDMA Features.
11/13/00   bgc     (merge) Seperated the FCH and SCH tdso blocks while
                   packing Reverse TDSO frames. Also fixed the fr_time
                   during FCH tdso frames.
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
#include "txctraffic_i.h"
#include "txcso.h"
#include "txcmc_i.h"
#include "txcmux.h"
#include "muxvoc.h"

//drivers
#include "enc_v.h"
#include "enc_i.h"

//diag
#include "cdma2kdiagi.h"

//cp
#include "cai.h"
#include "cai_v.h"
#include "mar_i.h"
#include "loopback.h"
#include "tdso_v.h"
#include "mctrans_i.h"

//other
#include "dsrlp_api.h"
#include "msg.h"
#include "m1x_diag.h"
#include "qw.h"


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

/* FULL Rate Frames Only Compile Option */
tdso_frame_block_type fch_tdso_frame_block;
tdso_frame_block_type sch_tdso_frame_block;

#ifdef FEATURE_IS2000_REL_A_SVD
/* txc_rlp_sig_priority is used to keep track of priorities in sending RLP control
** frames when RLP is the secondary SO in a SVD call.  The purpose of the
** implementation is to improve voice quality in a SVD call.
*/
txc_rlp_sig_priority_type txc_rlp_sig_priority;
#endif /* FEATURE_IS2000_REL_A_SVD */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/* <EJECT> */
/***************************************************************************/
/*                      TRAFFIC State Procedures                           */
/***************************************************************************/

/* <EJECT> */
/*===========================================================================

FUNCTION TX_LOOPBACK_TRAFFIC

DESCRIPTION
  Get loopback data bits (primary only) from the Loopback Service Option
  layer.

DEPENDENCIES
  The Loopback Service Option layer should be initialized already.

RETURN VALUE
  Returns the address of the loopback data bits in a pointer passed to the
  procedure.

  Returns the rate of the loopback data.

SIDE EFFECTS
  None

===========================================================================*/

cai_data_rate_type txc_loopback_traffic
(
  byte **data_ptr,
    /* Returns pointer to loopback data bits */
  txc_sig_frame_type signext
    /* Signaling mode for this frame */
)
{
  cai_data_rate_type max_rate;
    /* Maximum rate for this frame */
  cai_data_rate_type data_rate;
    /* Rate of current frame */
  static cai_frame_type data;
   /* Place to store loopback data bits */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (signext == BLANK)
  {
    /* Nothing from Loopback Service Option layer please */
    max_rate = CAI_BLANK_RATE;
  }
  else if (signext == DIM)
  {
    /* Dim-and-burst --> force half-rate */
    max_rate = CAI_HALF_RATE;
  }
  else
  {
    max_rate = CAI_FULL_RATE;
  }

  /* Get the loopback data bits from the Loopback Service Option layer */
  data_rate = loopback_tx( max_rate, &data ); //lint !e545 Suspicious use of &

  /* Set address of loopback data */
  *data_ptr = data;

  return (data_rate);

} /* txc_loopback_traffic */

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_MAR_TRAFFIC

DESCRIPTION
  Get traffic data (primary or secondary) from Markov frame error rate tests.

DEPENDENCIES
  The tx.frame_cnt has to be right.

RETURN VALUE
  Returns the address of the FER data in a variable passed to the
  procedure.

  Returns the rate of the FER data.

SIDE EFFECTS
  None

===========================================================================*/

cai_data_rate_type txc_mar_traffic
(
  byte **data_ptr
    /* Returns pointer to data */
)
{
  static byte        fer_data [ENC_14400_FULL_RATE_SIZE];
  dword              frame_num;
  byte               markov_rate;
  cai_data_rate_type data_rate;     /* Data rate of current frame */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  *data_ptr = fer_data;            /* return address of data */

  /*  Frame seed for Markov, frame number is +1 because we are building the
   *  frame before the tx_traffic_isr has occurred.
   */
  frame_num = qw_lo( txc_tx.frame_cnt );

/* txc_mar_traffic() is called before incrementing the frame count by 1 in
** txc isr. Hence we need to increment it by one for markov data generation.
*/
  frame_num++;

  markov_rate = mar_tx( fer_data, frame_num, FALSE );

  switch (markov_rate)
  {
#ifdef FEATURE_IS2000_1X_ADV
    case MAR_NC_8_RATE:
      data_rate = CAI_NON_CRITICAL_RATE;
      break;
#endif /* FEATURE_IS2000_1X_ADV */
    case MAR_8_RATE:
      data_rate = CAI_EIGHTH_RATE;
      break;
    case MAR_4_RATE:
      data_rate = CAI_QUARTER_RATE;
      break;
    case MAR_2_RATE:
      data_rate = CAI_HALF_RATE;
      break;
    case MAR_1_RATE:
      data_rate = CAI_FULL_RATE;
      break;
    default:
      data_rate = CAI_BLANK_RATE;
      break;
  }

  return (data_rate);

} /* txc_mar_traffic */

/* <EJECT> */
/*===========================================================================

FUNCTION TXC_TDSO_TRAFFIC

DESCRIPTION
  Get traffic data (primary) from TDSO

DEPENDENCIES

RETURN VALUE
  Returns the data rate

SIDE EFFECTS
  None

===========================================================================*/

cai_data_rate_type txc_tdso_traffic
(
  byte **data_ptr,
    /* Returns pointer to data */
  txc_sig_frame_type signext
    /* Signaling mode for next packet */
)
{
   cai_data_rate_type data_rate;   /* Data rate of current frame */

   if( signext == DIM )
   {
      // request 1 8TH rate frame from TDSO
      fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate = TDSO_RATE_EIGHTH;
      data_rate = CAI_EIGHTH_RATE;
   }
   else if( signext == BLANK )
   {
      fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate = TDSO_BLANK;
      data_rate = CAI_BLANK_RATE;
   }
   else
   {
     // TDSO rate defines if double or single size PDU
     // will be used. No matter what the rate set is
     // TDSO_RATE should be set to 1, which indicates
     // single pdu.

     fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate = TDSO_RATE_1;
     data_rate = CAI_FULL_RATE;
   }
   fch_tdso_frame_block.num_frames = 1;
   fch_tdso_frame_block.fr_time = qw_lo( txc_tx.frame_cnt ) + 1;

   if (txc_so.rev_dcch.included)
   {
#ifdef FEATURE_IS2000_REL_A_SVD
     if (txc_get_pri_phy_chan() & MCTRANS_PHY_CHAN_DCCH)
#endif /* FEATURE_IS2000_REL_A_SVD */
     {
       fch_tdso_frame_block.fdch_fr_mux =
         (tdso_mux_type) (txc_get_rev_link_rate_set( txc_so.rev_dcch.rc ));
       fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_channel = TDSO_DCCH;
     }
   }
   else
   {
#ifdef FEATURE_IS2000_REL_A_SVD
     if (txc_get_pri_phy_chan() & MCTRANS_PHY_CHAN_FCH)
#endif /* FEATURE_IS2000_REL_A_SVD */
     {
       fch_tdso_frame_block.fdch_fr_mux =
         (tdso_mux_type) (txc_get_rev_link_rate_set( txc_so.rev_fch.rc ));
       fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_channel = TDSO_FCH;
     }
   }

   if( fch_tdso_frame_block.fdch_fr_mux == TDSO_RATE_SET_1 )
   {
      fch_tdso_frame_block.frame_type.rev_arr[ 0 ].mux_pdu_type =
         TDSO_MUX_PDU_1;
   }
   else
   {
      fch_tdso_frame_block.frame_type.rev_arr[ 0 ].mux_pdu_type =
         TDSO_MUX_PDU_2;
   }

   fch_tdso_frame_block.frame_type.rev_arr[ 0 ].srvc_type =
      TDSO_PRIMARY_SRVC;

   fch_tdso_frame_block.frame_type.rev_arr[ 0 ].max_frame_size =
      tdso_data_len[ fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate ]
                   [ fch_tdso_frame_block.fdch_fr_mux ];

   if ( tdso_build_tx_frames(REV_LINK, &fch_tdso_frame_block ) != TDSO_VALID)
   {
     M1X_MSG( MUX, LEGACY_ERROR,
       "TDSO Build Failed");
   }
   *data_ptr = fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_item_ptr;

   if(fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate == TDSO_BLANK)
   {
     data_rate = CAI_BLANK_RATE;
     *data_ptr = NULL;
   }

   return (data_rate);
} /* txc_tdso_traffic */

#ifdef FEATURE_IS2000_REL_A_SVD
/* <EJECT> */
/*===========================================================================

FUNCTION TXC_TDSO_SEC_TRAFFIC

DESCRIPTION
  Get traffic data (Secondary) from TDSO

DEPENDENCIES
  Currently TDSO is supported on either R-FCH or R-DCCH.

RETURN VALUE
  Returns the data rate

SIDE EFFECTS
  None

===========================================================================*/

cai_data_rate_type txc_tdso_sec_traffic
(
  byte **data_ptr,
    /* Returns pointer to data */
  boolean msg_on_channel
    /* msg pending on signaling channel*/
)
{
   cai_data_rate_type data_rate;   /* Data rate of current frame */
   word  sec_num_bits = 0;

   sec_fch_num_bits  = 0;
   sec_dcch_num_bits  = 0;

   if (msg_on_channel)
   {
      // for SVD always blank S-TDSO data.
      fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate = TDSO_BLANK;
      data_rate = CAI_BLANK_RATE;
   }
   else
   {
     // TDSO rate defines if double or single size PDU
     // will be used. No matter what the rate set is
     // TDSO_RATE should be set to 1, which indicates
     // single pdu.
     // Even though Secondary traffic will be less than FULL RATE,
     // if a FRAME rate of less than FULL rate is specified,
     // TDSO will always build a FULL RATE frame.

     fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate = TDSO_RATE_1;
     data_rate = CAI_FULL_RATE;
   }

   fch_tdso_frame_block.num_frames  = 1;
   fch_tdso_frame_block.fr_time     = qw_lo( txc_tx.frame_cnt ) + 1;
   fch_tdso_frame_block.frame_type.rev_arr[ 0 ].srvc_type = TDSO_SECONDARY_SRVC;

   // initialize number of bits in sec. traffic
   if (txc_so.rev_dcch.included &&
       (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH))
   {
     fch_tdso_frame_block.fdch_fr_mux =
       (tdso_mux_type) (txc_get_rev_link_rate_set( txc_so.rev_dcch.rc ));
     fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_channel = TDSO_DCCH;
     sec_num_bits = txc_tdso_sec_frame_size (MCTRANS_PHY_CHAN_DCCH,
                                             msg_on_channel);
   }
   else if (txc_so.rev_fch.included)
   {
     if (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_FCH)
     {
       fch_tdso_frame_block.fdch_fr_mux =
         (tdso_mux_type) (txc_get_rev_link_rate_set( txc_so.rev_fch.rc ));
       fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_channel = TDSO_FCH;
       sec_num_bits = txc_tdso_sec_frame_size (MCTRANS_PHY_CHAN_FCH,
                                               msg_on_channel);
     }
   }

   if (fch_tdso_frame_block.fdch_fr_mux == TDSO_RATE_SET_1)
   {
     fch_tdso_frame_block.frame_type.rev_arr[ 0 ].mux_pdu_type = TDSO_MUX_PDU_1;
   }
   else
   {
     fch_tdso_frame_block.frame_type.rev_arr[ 0 ].mux_pdu_type = TDSO_MUX_PDU_2;
   }

   if (sec_num_bits == 0)
   {
     fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate = TDSO_BLANK;
     data_rate = CAI_BLANK_RATE;
   }

   // Even though FRAME RATE is specified as TDSO_RATE_1 (FULL rate),
   // specify the correct FRAME SIZE limit.
   fch_tdso_frame_block.frame_type.rev_arr[ 0 ].max_frame_size =
      sec_num_bits;

   if (tdso_build_tx_frames(REV_LINK, &fch_tdso_frame_block ) != TDSO_VALID)
   {
     M1X_MSG( MUX, LEGACY_ERROR,
       "TDSO Build Failed");
   }
   *data_ptr = fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_item_ptr;

   if (fch_tdso_frame_block.frame_type.rev_arr[ 0 ].fr_rate == TDSO_BLANK)
   {
     data_rate = CAI_BLANK_RATE;
     sec_num_bits = 0;
     *data_ptr = NULL;
   }

   if (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_DCCH)
   {
     sec_dcch_num_bits = sec_num_bits;
   }
   else if (txc_get_sec_phy_chan() & MCTRANS_PHY_CHAN_FCH)
   {
     sec_fch_num_bits = sec_num_bits;
   }

   return (data_rate);
} /* txc_tdso_sec_traffic */
#endif // FEATURE_IS2000_REL_A_SVD

/*===========================================================================

FUNCTION TXC_SET_VOC_ACTIVITY

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void txc_set_voc_activity
(
  boolean enable,   /* enable/disable voice activity test */
  word voc_1,       /* # of full rate */
  word voc_2,       /* # of 1/2 rate */
  word voc_8        /* # of 1/8 rate */
)
{
  muxvoc_set_voc_activity( enable, voc_1, voc_2, voc_8 );
}


