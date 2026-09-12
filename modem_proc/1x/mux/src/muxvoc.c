/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

       M U X  V O C O D E R  I N T E R F A C E  S O U R C E   F I L E


GENERAL DESCRIPTION
  Contains logic surrounding Layer1 Mux's interaction with the vocoder.

EXPORTED FUNCTIONS:
  muxvoc_init
  muxvoc_tc_init
  muxvoc_clear_inhibit_mvs_enable_flag
  rxc_reset_voc_init
  muxvoc_config_vocoder
  muxvoc_rxc_voc_xchg
  muxvoc_rxc_set_time_cnt
  muxvoc_rxc_pri_voice
  muxvoc_set_frame_rate
  muxvoc_process_tx_frame
  muxvoc_init_tx_voice_to_silence
  muxvoc_reset_tx_voice_to_silence

INITIALIZATION AND SEQUENCING REQUIREMENTS

 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Copyright (c) 2011 - 2020 Qualcomm Technologies, Inc.
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

/*==========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/1x.mpss/8.0/mux/src/muxvoc.c#2 $ $DateTime: 2020/01/06 00:22:52 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/06/20   eye     Added API to get RX voc payload size.
04/17/18   eye     Added null so check for RX and TX VOC frame processing.
09/29/15   agh     Remove all references to MVS and cleanup WON feature
08/04/15   eye     Added NULL ptr check before memscpy.
06/16/15   pap     Adding changes for encrypted voice calls to work correctly.
11/12/14   eye     Added API to get and set MVS variable & reset in call end.
12/02/14   agh     Support for new customer specific voice service option
08/27/14   eye     Added mvs_standby variables reset after real mvs_enable.
08/21/14   eye     Added MVS_STANDBY during HHO with frame_offset change.
05/29/14   pap     Redistributing messages.
10/18/13   pap     Adding changes for VFR allocations.
08/27/13   pap     Mainlining feature FEATURE_MODEM_1X_INHIBIT_VOC_ENABLE.
04/02/13   pap     Feature cleanup
02/27/13   vks     Fix compiler error arising out of turning off voice support
02/19/13   vks     Set codec rate to 8000 only if won decides to do NB
02/19/13   vks     Set max/min rates to full if won decides to do full rate
02/15/13   srk     Replace mcs hwio with m1x hwio
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
10/30/12   srk     KW warning fix.
10/19/12   vks     Use 1x internal feature (FEATURE_MODEM_1X_VOC_4GV_WB_ON_NB)
                   for HDOn-EVRC feature (FEATURE_VOC_4GV_WB_ON_NB)
10/18/12   srk     Use Vocoder debug F3s instead of diag commands.
10/16/12   vks     Initial changes to support HDOn-EVRC feature
04/04/12   trc     Convert to m1x_time module
01/23/12   srk     Feature Cleanup
09/13/11   srk     Disable vfr strobe upon call termination in muxvoc_disable_rx()
09/02/11   sk      Rebased the code to the NikeL modem package by mainlining
                   FEATURE_MODEM_1X_NIKEL.
06/01/11   trc     Add NikeL VFR strobe selection for 1x
02/15/11   jtm     Fixed compiler errors when 1xA is disabled.
01/25/11   jtm     Initial implementation.

==========================================================================*/

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
#include "muxvoc.h"
#include "stringl.h" /* for memscpy */

#ifdef FEATURE_MODEM_1X_VOICE_SUPPORT

//cp
#include "mccdma_i.h"

//diag
#include "cdma2kdiagi.h"

//drivers
#include "m1x_hwio_mpss.h"

//mux
#include "mux.h"
#include "rxccommon_i.h"
#include "txccommon_i.h"
#include "txcmux.h"

//other
#include "bit.h"
#include "err.h"
#include "msg.h"
#include "m1x_diag.h"
#include "m1x_time_i.h"
#include "dsrlp_api.h"
#include "m1x_stmr_i.h"

/*===========================================================================

                   CONSTANTS AND MACROS

===========================================================================*/

#define VOC_DBG_MSG( ... ) \
    M1X_MSG( MUX, DBG_VOC, __VA_ARGS__ )

/* Vocoder buffer and frame rate to transmit (this frame and next frame) */
#define  MAX_VOC_BUFS 2

/* Critical Section */
#define MUXVOC_ENTER_CRITICAL_SECTION()  rex_enter_crit_sect(&muxvoc_crit_sect)
#define MUXVOC_LEAVE_CRITICAL_SECTION()  rex_leave_crit_sect(&muxvoc_crit_sect)

/*===========================================================================

                   INTERNAL TYPES

===========================================================================*/

typedef struct
{
  byte txc_bits[ENC_14400_FULL_RATE_SIZE];
  uint32 txc_voc_rate;

} txc_voc_type;

/*===========================================================================

                   LOCAL VARIABLES

 ===========================================================================*/

static struct
{
  uint16 rx_service_option;
  uint16 tx_service_option;

  uint32 rx_voc_rate;
  uint32 tx_voc_rate;

  txc_sig_frame_type signext;

  boolean is_rx_sync_check;
  boolean is_tx_sync_check;
  boolean is_first_rx_cb;
  boolean is_first_tx_cb;
  boolean dim_last;
  boolean inhibit_voc_enable;

  byte rx_voice_frames_processed;
  byte tx_voice_frames_processed;

  txc_voc_type *txc_voc_data_rd_ptr;
  txc_voc_type *txc_voc_data_wr_ptr;

  /* Pingpong buffers to store vocoder TX frame and rate. */
  txc_voc_type txc_voc_data[MAX_VOC_BUFS];

  /* Rx buffer to store vocoder data. */
  byte rxc_bits[ENC_14400_FULL_RATE_SIZE];

  /* stores the voc id being used for a call */
  uint32 mvs_mode_option;

  #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
  /* store min and max rates for a call */
  onex_voc_event_rqst_min_max_rate_t curr_min_max_rates;

  /* Stores open parameters registered by VA */
  onex_voc_cmd_open_t voc_open_val;
  #endif
} muxvoc_data;

#ifdef FEATURE_JCDMA
/* Voice frame activity test */
static struct {
   boolean enable;
   word    cnt;
   word    voc_1;
   word    voc_2;
   word    voc_8;
} muxvoc_tx_voc_activity = { FALSE, 0, 0, 0, 0 };
#endif /* FEATURE_JCDMA */

/* Internal value to represent vocoder type not set yet. This macro is not 
 * meant to be used by Audio and so is not accessible to them */
#define ONEX_VOC_ID_NONE ( 0x00000000 )

#else
/* Required in order to return null frames to the caller
   when no voice service is supported.*/
byte muxvoc_null_tc_frame[ENC_14400_FULL_RATE_SIZE];
#endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

/* Declare a critical section for MUXMDSP */
static rex_crit_sect_type muxvoc_crit_sect;

/*===========================================================================

          LOCAL VARIABLES ALSO NEEDED WHEN VOICE SUPPORT IS DISABLED

 ===========================================================================*/

static const byte muxvoc_eighth_rate_null_frame [3] = {0xFF, 0xFF, 0xFF};

#ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
/*===========================================================================

                    INTERNAL FUNCTION DECLARATIONS

===========================================================================*/

#ifdef FEATURE_VOICE_AGENT_ARCHITECTURE

static void muxvoc_send_voc_frame( void );
static void muxvoc_get_open_params( void );
static void muxvoc_set_min_max_rate( uint32 max_rate,
                                     uint32 min_rate );
static void muxvoc_reset_session( void );

#endif

/*===========================================================================

                    INTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================

FUNCTION MUXVOC_IS_VOICE_SO_SUPPORTED

DESCRIPTION
  This function performs a simple conditional check on the provided service
  option and returns a boolean representing if the the service option is
  supported based on the features defined.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if the service option is supported, FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
static boolean muxvoc_is_voice_so_supported
(
  uint16 srv_opt
)
{
  boolean so_is_supported = TRUE;

  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_1X_CUST_VOICE_SO
  /* This workaround has been added to handle NV dependent non-constant
   * SO while ensuring the existing switch construct is not disrupted */
  if (srv_opt == mcc_get_cust_so() && srv_opt != CAI_SO_NULL)
  {
    srv_opt = CAI_GEN_EVRC_CUST_SO;
  }
  #endif /* FEATURE_1X_CUST_VOICE_SO */

  switch (srv_opt)
  {
    case CAI_SO_VOICE_13K_IS733:
    case CAI_SO_VOICE_13K:
      break;

    case CAI_SO_VOICE_EVRC:
    #ifdef FEATURE_1X_CUST_VOICE_SO
    case CAI_GEN_EVRC_CUST_SO:
    #endif /* FEATURE_1X_CUST_VOICE_SO */
      break;

    case CAI_SO_VOICE_EVRC_NW:
      break;

    case CAI_SO_VOICE_4GV_NB:
      break;

    case CAI_SO_VOICE_4GV_WB:
      break;

    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "SO%d Not a valid voice SO.",
        srv_opt);
      so_is_supported = FALSE;
      break;
  }

  return so_is_supported;

} /* muxvoc_is_voice_so_supported */


/*===========================================================================
FUNCTION      muxvoc_check_for_dim_frame

DESCRIPTION   Check if we asked for a dim frame for the next frame but got
              less than full rate for the current frame and used up all the
              signaling bits waiting to be transmitted.

              If this is TRUE then we change the order for the next frame with
              the vocoder and don't affect voice quality over the next frame.

DEPENDENCIES  Do not allow this if DSRLP also requires bandwidth to
              send RLP signaling data (for SVD calls.)

RETURN VALUE  None.

SIDE EFFECTS  Vocoder may be updated for full range of voice if all pending
              signaling bits can be sent in the current frame.
===========================================================================*/
static void muxvoc_check_for_dim_frame
(
  cai_data_rate_type frame_rate,
  txc_sig_frame_type signext
)
{
  uint32 max_rate;
  uint32 min_rate;

  /*-----------------------------------------------------------------------*/

  if( ((signext == DIM) || (signext == BLANK))  && (frame_rate != CAI_FULL_RATE))
  {
    if(txc_sig_fits_in_frame( frame_rate ) == TRUE)
    {
      /* Signaling payload fits in this frame. Reconfigure the vocoder
         to provide the full voice frame range. */

      max_rate = ONEX_VOC_FRAME_RATE_FULL;

      /* if dipswitch 8 is ON force full rate voice,
         otherwise allow full range. */
      if ( cdma2kdiag_get_switch( CDMA2KDIAG_SW_FORCE_RL_FULL_RATE )  )
      {
        min_rate = ONEX_VOC_FRAME_RATE_FULL;
      }
      else
      {
        min_rate = ONEX_VOC_FRAME_RATE_EIGHTH;
      }

      #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
      muxvoc_set_min_max_rate(max_rate, min_rate);
      #endif

      M1X_MSG( MUX, LEGACY_MED,
        "Signaling satisfied in current frame. Requested full range.");
    }
  } /* end if((txtc.frame.sig == DIM)... */

} /* muxvoc_set_frame_rate */


/*===========================================================================
FUNCTION      MUXVOC_PROCESS_TX_FRAME

DESCRIPTION   Get traffic data (primary or secondary) from the vocoder.

DEPENDENCIES  The vocoder should be initialized already.

RETURN VALUE  Returns the address of the vocoder data in a pointer passed to
              the procedure and the supplied vocoder rate.

SIDE EFFECTS  None.
===========================================================================*/
static cai_data_rate_type muxvoc_process_tx_frame
(
  uint32 max_rate,
  uint32 min_rate,
  byte **data_ptr                /* Returns pointer to data */
)
{
  uint32 voc_rate;    /* Current vocoder data rate */
  cai_data_rate_type frame_rate; /* resulting frame rate */

  txc_voc_type *txc_voc_data_tmp_ptr = &muxvoc_data.txc_voc_data[0];

  /*-----------------------------------------------------------------------*/

  /* Get the data from what vocoder last sent, switch buffers */
  txc_voc_data_tmp_ptr = muxvoc_data.txc_voc_data_rd_ptr;
  muxvoc_data.txc_voc_data_rd_ptr = muxvoc_data.txc_voc_data_wr_ptr;
  muxvoc_data.txc_voc_data_wr_ptr = txc_voc_data_tmp_ptr;

  voc_rate = muxvoc_data.txc_voc_data_rd_ptr->txc_voc_rate;
  muxvoc_data.tx_voc_rate = voc_rate;

  /* If we get an Erasure or Blank from the vocoder, point
     the data_ptr to null and set the rate to 0_RATE (BLANK)
     Otherwise, point to the data provided by the vocoder. */
  if ( (voc_rate == ONEX_VOC_FRAME_RATE_ERASURE)
        #ifdef FEATURE_IS2000_REL_A_SVD
        || (voc_rate == ONEX_VOC_FRAME_RATE_BLANK)
        #endif // FEATURE_IS2000_REL_A_SVD
     )
  {
    *data_ptr = NULL;
    voc_rate = ONEX_VOC_FRAME_RATE_BLANK;
  }
  else
  {
    *data_ptr = muxvoc_data.txc_voc_data_rd_ptr->txc_bits;
  }

  TXC_ENTER_ISR_SECTION();
  /* Keep a count of the number of consecutive voice frames
     processed by TX task. This will be cleared by the
     MVS callback function. */
  muxvoc_data.tx_voice_frames_processed++;
  TXC_LEAVE_ISR_SECTION();

  /* Give vocoder voice bandwidth for next frame */
  #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
  /* Set max and min rate here for vocoder if its not blank,
     vocoder FW doesnt accept BLANK as rate */
  if( ( max_rate != ONEX_VOC_FRAME_RATE_BLANK ) &&
      ( min_rate != ONEX_VOC_FRAME_RATE_BLANK )
    )
  {
    muxvoc_set_min_max_rate(max_rate, min_rate);
  }
  #endif

  VOC_DBG_MSG(
    "voctxcmd,r=%d,w=%d,bits=%d,rate=%d,max=%d,min=%d",
    muxvoc_data.txc_voc_data_rd_ptr,
    muxvoc_data.txc_voc_data_wr_ptr,
    muxvoc_data.txc_voc_data_rd_ptr->txc_bits[0],
    muxvoc_data.txc_voc_data_rd_ptr->txc_voc_rate,
    max_rate,
    min_rate );

  /* Translate the MVS rate type to CAI rate type. */
  switch(voc_rate)
  {
    case ONEX_VOC_FRAME_RATE_BLANK:
      frame_rate = CAI_BLANK_RATE;
      break;

    case ONEX_VOC_FRAME_RATE_EIGHTH:
      frame_rate = CAI_EIGHTH_RATE;
      break;

    case ONEX_VOC_FRAME_RATE_QUARTER:
      frame_rate = CAI_QUARTER_RATE;
      break;

    case ONEX_VOC_FRAME_RATE_HALF:
      frame_rate = CAI_HALF_RATE;
      break;

    case ONEX_VOC_FRAME_RATE_FULL:
      frame_rate = CAI_FULL_RATE;
      break;

    #ifdef FEATURE_IS2000_1X_ADV
    case ONEX_VOC_FRAME_RATE_EIGHTH_NONCRITICAL:
      frame_rate = CAI_NON_CRITICAL_RATE;
      break;
    #endif /* FEATURE_IS2000_1X_ADV */

    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Bad MVS rate (%d). Default to Blank Rate",
        voc_rate);
      frame_rate = CAI_BLANK_RATE;
      break;
  }

  return frame_rate;

} /* muxvoc_process_tx_frame */


#ifdef FEATURE_JCDMA
/*===========================================================================
FUNCTION      muxvoc_voc_activity_test

DESCRIPTION   If the voice activity test is enabled provide the
              voice frame rate to be used when requesting a frame from the
              vocoder.

DEPENDENCIES  None

RETURN VALUE  test voice frame rate.

SIDE EFFECTS  None.
===========================================================================*/
static uint32 muxvoc_voc_activity_test(void)
{
  uint32 voc_rate = ONEX_VOC_FRAME_RATE_FULL;;

  /* We look at the txc_voc_activity and report vocoder rate set */
  if (muxvoc_tx_voc_activity.cnt < muxvoc_tx_voc_activity.voc_1)
  {
    voc_rate = ONEX_VOC_FRAME_RATE_FULL;
  }
  else if (muxvoc_tx_voc_activity.cnt < (muxvoc_tx_voc_activity.voc_1 +
                                   muxvoc_tx_voc_activity.voc_2))
  {
    voc_rate = ONEX_VOC_FRAME_RATE_HALF;
  }
  else
  {
    voc_rate = ONEX_VOC_FRAME_RATE_EIGHTH;
  }

  muxvoc_tx_voc_activity.cnt = (++muxvoc_tx_voc_activity.cnt
                          % (muxvoc_tx_voc_activity.voc_1 +
                             muxvoc_tx_voc_activity.voc_2 +
                             muxvoc_tx_voc_activity.voc_8));

  return (voc_rate);
}
#endif /* FEATURE_JCDMA */

#endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

/*===========================================================================

                    EXTERNAL FUNCTION DEFINITIONS

===========================================================================*/

/*===========================================================================

FUNCTION MUXVOC_DISABLE_RX

DESCRIPTION
  This routine disables the Rx voice processing once the primary voice SO has
  been released. It also disables VFR interrupt generation.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void muxvoc_disable_rx(void)
{
  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  M1X_MSG( MUX, LEGACY_MED,
    "Disable Rx voice processing, "
    "Setting VFR strobe to ground");

  muxvoc_data.rx_service_option = CAI_SO_NULL;
  muxvoc_data.is_rx_sync_check = FALSE;

  RXC_ENTER_ISR_SECTION();
  muxvoc_data.rx_voc_rate = ONEX_VOC_FRAME_RATE_BLANK;
  memset(muxvoc_data.rxc_bits, 0xFF, sizeof(muxvoc_data.rxc_bits));

  /* Clear the count of the number of consecutive voice frames
     processed by RX task. This will be incremented when
     RX task demuxes the voice frame and is cleared by the
     MVS callback function. */
  muxvoc_data.rx_voice_frames_processed = 0;
  RXC_LEAVE_ISR_SECTION();

  #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
  muxvoc_reset_session();
  #endif

  /* Disable VFR strobe when finished with call */
  /* release the vocoder interrupt */
  m1x_stmr_release_vfr();

  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

} /* muxvoc_disable_rx */


/*===========================================================================

FUNCTION MUXVOC_DISABLE_TX

DESCRIPTION
  This routine disables the tx voice processing once the primary voice SO has
  been released.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void muxvoc_disable_tx(void)
{
  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT

  M1X_MSG( MUX, LEGACY_MED,
    "Disable Tx voice processing.");

  muxvoc_data.tx_voc_rate = ONEX_VOC_FRAME_RATE_EIGHTH;
  muxvoc_data.tx_service_option = CAI_SO_NULL;
  muxvoc_data.dim_last = FALSE;
  muxvoc_data.signext = NORM;
  muxvoc_data.is_tx_sync_check = FALSE;

  TXC_ENTER_ISR_SECTION();
  muxvoc_data.tx_voice_frames_processed = 0;
  TXC_LEAVE_ISR_SECTION();

  muxvoc_reset_tx_voice_to_silence();

  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

} /* muxvoc_disable_tx */

/*===========================================================================

FUNCTION MUXVOC_RX_TC_INIT

DESCRIPTION
  This routine initializes the muxvocoder module using the TCI command
  provided by MC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void muxvoc_rx_tc_init
(
  boolean inhibit_voc_enable
)
{
  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  muxvoc_data.rx_service_option = CAI_SO_NULL;

  RXC_ENTER_ISR_SECTION();
  memset(muxvoc_data.rxc_bits, 0xFF, sizeof(muxvoc_data.rxc_bits));
  muxvoc_data.rx_voc_rate = ONEX_VOC_FRAME_RATE_BLANK;

  /* Clear the count of the number of consecutive voice frames
     processed by RX task. This will be incremented when
     RX task demuxes the voice frame and is cleared by the
     MVS callback function. */
  muxvoc_data.rx_voice_frames_processed = 0;
  muxvoc_data.is_first_rx_cb = FALSE;
  RXC_LEAVE_ISR_SECTION();

  muxvoc_data.is_rx_sync_check = FALSE;

  /* Indicate that vocoder should OR
     should *NOT* be initialized at this time. */
  muxvoc_data.inhibit_voc_enable = inhibit_voc_enable;

  M1X_MSG( MUX, LEGACY_MED,
    "Configure Mux Vocoder module for rx traffic. inhibit_voc_enable=%d",
    muxvoc_data.inhibit_voc_enable);
  #else
  MODEM_1X_UNUSED(inhibit_voc_enable);
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

} /* muxvoc_rx_tc_init */

/*===========================================================================

FUNCTION MUXVOC_CLEAR_INHIBIT_VOC_ENABLE_FLAG

DESCRIPTION
  This routine sets the inhibit mvs enable flag to FALSE which will then allow
  subsequent calls to mvs_enable() to be performed.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void muxvoc_clear_inhibit_voc_enable_flag ( void )
{
  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  M1X_MSG( MUX, LEGACY_HIGH,
    "Clear inhibit_voc_enable.");
  muxvoc_data.inhibit_voc_enable = FALSE;
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

} /* muxvoc_clear_inhibit_mvs_enable_flag */

/*===========================================================================

FUNCTION muxvoc_enable_tx_sync_check

DESCRIPTION
    Enable the logic used to determine if TX and MVS voice frame processing
    is in sync. This is meant to catch cases where the MVS callback occurs
    too late or too early, resulting in degraded speech.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_enable_tx_sync_check(void)
{
  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  M1X_MSG( MUX, LEGACY_MED,
    "Enable TX voice frame sync checking.");

  muxvoc_data.is_tx_sync_check = TRUE;

  TXC_ENTER_ISR_SECTION();
  muxvoc_data.is_first_tx_cb = TRUE;
  TXC_LEAVE_ISR_SECTION();
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */
}

/*===========================================================================

FUNCTION muxvoc_enable_rx_sync_check

DESCRIPTION
    Enable the logic used to determine if RX and MVS voice frame processing
    is in sync. This is meant to catch cases where the MVS callback occurs
    too late or too early, resulting in degraded speech.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_enable_rx_sync_check(void)
{
  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  M1X_MSG( MUX, LEGACY_MED,
    "Enable RX voice frame sync checking.");

  muxvoc_data.is_rx_sync_check = TRUE;

  RXC_ENTER_ISR_SECTION();
  muxvoc_data.is_first_rx_cb = TRUE;
  RXC_LEAVE_ISR_SECTION();

  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */
}

/*===========================================================================

FUNCTION RXC_RESET_VOC_INIT

DESCRIPTION
  ****This function is deprecated***

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/

void rxc_reset_voc_init ( void )
{
  /*-----------------------------------------------------------------------*/
  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  M1X_MSG( MUX, LEGACY_LOW,
    "rxc_reset_voc_init called");
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

} /* rxc_reset_voc_init */

/*===========================================================================

FUNCTION MUXVOC_CRIT_SECT_INIT

DESCRIPTION
  Initialize critical section for MUXVOC module

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void muxvoc_crit_sect_init ( void )
{

  /* Initialize critical section */
  rex_init_crit_sect( &muxvoc_crit_sect );

} /* muxvoc_crit_set_init */

/*===========================================================================

FUNCTION MUXVOC_CONFIG_VOCODER

DESCRIPTION
  If the vocoder has not been configured, then configure it to work
  with the current voice service option.  Note that the service option
  may be changed in a middle of a call as well.

DEPENDENCIES
  Called if voc config is not done and SO is voice.

RETURN VALUE
  TRUE = if the vocoder was configured. FALSE otherwise.

SIDE EFFECTS
  Queue request to Vocoder driver.
  Service option SMV is not supported by MVS.
  For any service option not supported (by lack of a defined feature or
  otherwise) the vocoder api will not be called.

===========================================================================*/
boolean muxvoc_config_vocoder
(
  uint16 srv_opt
)
{
  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  /* Voice service option to configure vocoder */
  word so = srv_opt;

  #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
  onex_voc_event_rqst_dtx_mode_t dtx_setting;
  onex_voc_event_rqst_start_t voc_id;
  onex_voc_event_rqst_avg_enc_rate_t avg_enc_rate;
  #endif

  /*-----------------------------------------------------------------------*/

  muxvoc_data.mvs_mode_option = ONEX_VOC_ID_NONE;
  if(muxvoc_data.inhibit_voc_enable)
  {
    /* MC has instructed us to delay enabling the vocoder
      until instructed to at a later time. */
    return FALSE;
  }

  #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
  muxvoc_get_open_params();
  #endif

  if( muxvoc_is_voice_so_supported(srv_opt) )
  {
    muxvoc_data.rx_service_option = srv_opt;
    muxvoc_data.tx_service_option = srv_opt;

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
        muxvoc_data.mvs_mode_option = ONEX_VOC_ID_QCELP13K;
        break;

      case CAI_SO_VOICE_EVRC:
      #ifdef FEATURE_1X_CUST_VOICE_SO
      case CAI_GEN_EVRC_CUST_SO:
      #endif /* FEATURE_1X_CUST_VOICE_SO */
        muxvoc_data.mvs_mode_option = ONEX_VOC_ID_EVRCA;
        break;

      case CAI_SO_VOICE_EVRC_NW:
        muxvoc_data.mvs_mode_option = ONEX_VOC_ID_EVRCNW;

        /* For SO73, ensure 4GV NW vocoder is NOT in DTX mode, as we want
           to receive the 1/8-rate non-critical frames in mux for 1x voice. */
        #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
        dtx_setting.enable_flag = FALSE;
        muxvoc_data.voc_open_val.event_cb( 
                 muxvoc_data.voc_open_val.session_context,
                 ONEX_VOC_EVENT_RQST_DTX_MODE,
                 (void*) &dtx_setting,
                 sizeof(dtx_setting));
        #endif
        break;

      case CAI_SO_VOICE_4GV_NB:
        muxvoc_data.mvs_mode_option = ONEX_VOC_ID_EVRCB;
        break;

      case CAI_SO_VOICE_4GV_WB:
        muxvoc_data.mvs_mode_option = ONEX_VOC_ID_EVRCWB;
        break;

      default:
        /* We should never get here based on the check for
           a valid SO prior to this switch statement. */
        M1X_MSG( MUX, LEGACY_ERROR,
          "Invalid SO%d",
          srv_opt);
        break;
    }

    /* Enable VFR strobe before activating vocoder */
    M1X_MSG( MUX, LEGACY_HIGH,
      "Setting VFR strobe to 1x mode");

    /* initiate the VFR interrupt */
    m1x_stmr_allocate_vfr( mcc_get_vstmr_id() );

    #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
    voc_id.vocoder_id = muxvoc_data.mvs_mode_option;

    /* send the min max rates */
    muxvoc_data.voc_open_val.event_cb( muxvoc_data.voc_open_val.session_context,
                                       ONEX_VOC_EVENT_RQST_MIN_MAX_RATE,
                                       (void*)&(muxvoc_data.curr_min_max_rates),
                                       sizeof(muxvoc_data.curr_min_max_rates) );
    
    /* send the avg rate by querying CP */
    avg_enc_rate.avg_enc_rate = mcc_get_avg_enc_rate();
    muxvoc_data.voc_open_val.event_cb( muxvoc_data.voc_open_val.session_context,
                                       ONEX_VOC_EVENT_RQST_AVG_ENC_RATE,
                                       (void*) &avg_enc_rate,
                                       sizeof(avg_enc_rate) );
    
    /* send the start cmd */
    muxvoc_data.voc_open_val.event_cb( muxvoc_data.voc_open_val.session_context,
                                       ONEX_VOC_EVENT_RQST_START,
                                       (void*) &voc_id,
                                       sizeof(voc_id) );
    #endif

    M1X_MSG( MUX, LEGACY_HIGH,
      "Called mvs_enable: mode_option=%d, srv_opt=%d",
      muxvoc_data.mvs_mode_option,
      srv_opt);

    /* Reset the mvs_standby variables */
    rxc_mvs_set_standby_progress_status( FALSE );
    rxc_mvs_set_standby_status( MVS_STANDBY_FAIL );
    rxc_set_mvs_status_on_tc_entry(TRUE);
  }
  else
  {
    /* This SO either is NOT voice or is not supported. Default to
       a NULL SO to continue gracefully. */
    muxvoc_data.rx_service_option = CAI_SO_NULL;
    muxvoc_data.tx_service_option = CAI_SO_NULL;
    M1X_MSG( MUX, LEGACY_HIGH,
      "Can't enable MVS with unsupported SO.");
  }
  #else
  MODEM_1X_UNUSED(srv_opt);
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

  return TRUE;

} /* muxvoc_config_vocoder */

/*===========================================================================
FUNCTION      MUXVOC_RXC_PRI_VOICE

DESCRIPTION   For voice service options, this procedure copies the received
              voice payload and rate to provide to the vocoder at a later
              time. It also calls other necessary routines (such as voice
              decription).

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void muxvoc_rxc_pri_voice
(
  byte bits[],                  /* Address of first byte of primary data   */
  word bit_pos,                 /* Position of first bit of data in buffer */
  word bit_len,                 /* Number of bits of data in buffer        */
  cai_data_rate_type frame_rate /* Rate of the current voice frame         */
)
{
  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  qword sys_time_in_20ms;       /* system time in 20ms units               */
  uint32 voc_rate;

  /*-----------------------------------------------------------------------*/

  /*-------------------------------------------------------------------------
    Copy vocoder bits from temp buffer to vocoder request buffer
    transfer frame to vocoder later.

    Also copy the current rate of the frame to exchange with the vocoder.
  -------------------------------------------------------------------------*/

  /* Translate the CAI rate type to MVS rate type. */
  switch(frame_rate)
  {
    case CAI_BLANK_RATE:
      voc_rate = ONEX_VOC_FRAME_RATE_BLANK;
      break;

    case CAI_EIGHTH_RATE:
      voc_rate = ONEX_VOC_FRAME_RATE_EIGHTH;
      break;

    case CAI_QUARTER_RATE:
      voc_rate = ONEX_VOC_FRAME_RATE_QUARTER;
      break;

    case CAI_HALF_RATE:
      voc_rate = ONEX_VOC_FRAME_RATE_HALF;
      break;

    case CAI_FULL_RATE:
      voc_rate = ONEX_VOC_FRAME_RATE_FULL;
      break;

    case CAI_NULL_RATE:
      voc_rate = ONEX_VOC_FRAME_RATE_ERASURE;
      break;

    #ifdef FEATURE_IS2000_1X_ADV
    case CAI_NON_CRITICAL_RATE:
      voc_rate = ONEX_VOC_FRAME_RATE_EIGHTH_NONCRITICAL;
      break;
    #endif /* FEATURE_IS2000_1X_ADV */

    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Bad frame rate (%d). Default to Zero Rate",
        frame_rate);
      voc_rate = ONEX_VOC_FRAME_RATE_BLANK;
      break;
  }

  RXC_ENTER_ISR_SECTION();
  b_copy( bits, bit_pos, muxvoc_data.rxc_bits, 0, bit_len );

  muxvoc_data.rx_voc_rate = voc_rate;

  /*Keep a count of the number of consecutive voice frames
    processed by RX task. This will be cleared by the
    MVS callback function. */
  muxvoc_data.rx_voice_frames_processed++;

  RXC_LEAVE_ISR_SECTION();

  VOC_DBG_MSG(
    "PriVoice,proc_cnt=%d,rate=%d,data=%d,%d,%d,%d,%d",
    muxvoc_data.rx_voice_frames_processed,
    (uint32)muxvoc_data.rx_voc_rate,
    muxvoc_data.rxc_bits[0],
    muxvoc_data.rxc_bits[1],
    muxvoc_data.rxc_bits[2],
    muxvoc_data.rxc_bits[3],
    muxvoc_data.rxc_bits[4] );

  /* Decrypt voice if voice encryption feature is enabled.  If feature is not
  ** enabled, buffer will remain unchanged.
  */
  m1x_time_get_20ms_frame_time(sys_time_in_20ms);  /* for decryption */

  #ifdef FEATURE_1X_CUST_VOICE_SO
  /* Use customer specific algorithm for customer specific voice SO
   * if the algorithm has been registered */
  if ( (rxc_so.pri == mcc_get_cust_so() && rxc_so.pri != CAI_SO_NULL) &&
       (cust_voice_decrypt_algorithm != NULL ) )
  {

    M1X_MSG( DCP, LEGACY_MED,
             "Using cust decrypt API, SO:%d", mcc_get_cust_so() );
    cust_voice_decrypt_algorithm(
      sys_time_in_20ms,    /* system time */
      muxvoc_data.rxc_bits,/* pointer to buffer to decrypt */
      0,                   /* buffer offset */
      (uint32) bit_len     /* number of bits to decrypt */
      );
  }
  else
  #endif /* FEATURE_1X_CUST_VOICE_SO */
  {
    mccdma_decrypt_voice_frame (
      sys_time_in_20ms,    /* system time */
      muxvoc_data.rxc_bits,/* pointer to buffer to decrypt */
      0,                   /* buffer offset */
      (uint32) bit_len     /* number of bits to decrypt */
      );
  }

  RXC_ENTER_ISR_SECTION();
  #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
  muxvoc_send_voc_frame();
  #endif
  RXC_LEAVE_ISR_SECTION();

  #else
  MODEM_1X_UNUSED(bits);
  MODEM_1X_UNUSED(bit_pos);
  MODEM_1X_UNUSED(bit_len);
  MODEM_1X_UNUSED(frame_rate);
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

} /* muxvoc_rxc_pri_voice */

/*===========================================================================
FUNCTION      MUXVOC_TX_TC_INIT

DESCRIPTION   Initialize voice frames to silence.

              Note that txc_voc_eighth_rate_null_frame is an eighth rate frame,
              therefore it is smaller than the voice buffer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  ALL TX voice buffers will be set to 1/8th rate null.

===========================================================================*/
void muxvoc_tx_tc_init( void )
{
  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT

  M1X_MSG( MUX, LEGACY_MED,
    "Configure Mux Vocoder module for tx traffic.");

  muxvoc_data.txc_voc_data_rd_ptr = &muxvoc_data.txc_voc_data[0];
  muxvoc_data.txc_voc_data_wr_ptr = &muxvoc_data.txc_voc_data[1];

  (void)memscpy( muxvoc_data.txc_voc_data_rd_ptr->txc_bits,
                 sizeof(muxvoc_data.txc_voc_data_rd_ptr->txc_bits),
                 muxvoc_eighth_rate_null_frame,
                 sizeof( muxvoc_eighth_rate_null_frame )
               );

  (void)memscpy( muxvoc_data.txc_voc_data_wr_ptr->txc_bits,
                 sizeof(muxvoc_data.txc_voc_data_wr_ptr->txc_bits),
                 muxvoc_eighth_rate_null_frame,
                 sizeof( muxvoc_eighth_rate_null_frame )
               );

  muxvoc_data.txc_voc_data_wr_ptr->txc_voc_rate = ONEX_VOC_FRAME_RATE_EIGHTH;
  muxvoc_data.txc_voc_data_rd_ptr->txc_voc_rate = ONEX_VOC_FRAME_RATE_EIGHTH;

  TXC_ENTER_ISR_SECTION();
  /* Clear the count of the number of consecutive voice frames
     processed by TX task. This will be incremented when
     TX task builds the voice frame and is cleared by the
     MVS callback function. */
  muxvoc_data.tx_voice_frames_processed = 0;
  muxvoc_data.is_first_tx_cb = FALSE;
  TXC_LEAVE_ISR_SECTION();

  muxvoc_data.is_tx_sync_check = FALSE;
  muxvoc_data.tx_voc_rate = ONEX_VOC_FRAME_RATE_EIGHTH;
  muxvoc_data.dim_last = FALSE;
  muxvoc_data.signext = NORM;
  muxvoc_data.tx_service_option = CAI_SO_NULL;

  VOC_DBG_MSG(
    "TrafInit,rd=%d,wr=%d",
    muxvoc_data.txc_voc_data_rd_ptr,
    muxvoc_data.txc_voc_data_wr_ptr);
  #else

  /* Initialize the null frame buffer when voice is not supported. */
  (void)memscpy( muxvoc_null_tc_frame,
                 sizeof(muxvoc_null_tc_frame),
                 muxvoc_eighth_rate_null_frame,
                 sizeof( muxvoc_eighth_rate_null_frame )
               );
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

} /* muxvoc_tx_tc_init */

/*===========================================================================
FUNCTION      MUXVOC_RESET_TX_VOICE_TO_SILENCE

DESCRIPTION   Reset voice frames to silence. This should be called only after
              the contents of the current read buffer have been copied.

              Note that txc_voc_eighth_rate_null_frame is an eighth rate frame,
              therefore it is smaller than the voice buffer.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  The current voice frame buffer will be set to 1/8th rate null.

===========================================================================*/
void muxvoc_reset_tx_voice_to_silence( void )
{
  /*-----------------------------------------------------------------------*/

  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  if( muxvoc_data.txc_voc_data_rd_ptr != NULL )
  {
    (void)memscpy( muxvoc_data.txc_voc_data_rd_ptr->txc_bits,
                   sizeof(muxvoc_data.txc_voc_data_rd_ptr->txc_bits),
                   muxvoc_eighth_rate_null_frame,
                   sizeof( muxvoc_eighth_rate_null_frame )
                 );
  
    muxvoc_data.txc_voc_data_rd_ptr->txc_voc_rate = ONEX_VOC_FRAME_RATE_EIGHTH;
  
    VOC_DBG_MSG(
      "FinishedResetBuf,rd_ptr=%d",
      muxvoc_data.txc_voc_data_rd_ptr);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,"NULL POINTER, Dont copy silence" );
  }
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */

} /* muxvoc_reset_tx_voice_to_silence */

/*===========================================================================
FUNCTION      MUXVOC_PRINT_VOC_FRAME_INFO

DESCRIPTION   Print debug information about the transmitted vocoder frame
              if debuging is enabled.

DEPENDENCIES  None

RETURN VALUE  None

SIDE EFFECTS  None

===========================================================================*/
void muxvoc_print_voc_frame_info
(
  enc_rate_type rate,
  byte *data
)
{
  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  if( data != NULL )
  {
    VOC_DBG_MSG(
      "EncFchData,rate=%d,data=%d,data=%d,data=%d,data=%d",
      rate,
      data[0],
      data[1],
      data[2],
      data[3] );
  }
  else
  {
    VOC_DBG_MSG(
      "NULL Vocoder frame: rate %d",
      rate );
  }
  #else
  MODEM_1X_UNUSED(rate);
  MODEM_1X_UNUSED(data);
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */
}

/*===========================================================================

FUNCTION MUXVOC_TX_VOC_TRAFFIC

DESCRIPTION
  Get traffic data (primary or secondary) from the vocoder.

DEPENDENCIES
  The vocoder should be initialized already.

RETURN VALUE
  Returns the address of the vocoder data in a pointer passed to the
  procedure.

  Returns the rate of the vocoder data

SIDE EFFECTS
  None

===========================================================================*/
cai_data_rate_type muxvoc_tx_voc_traffic
(
  byte **data_ptr,                /* Returns pointer to data        */
  txc_sig_frame_type signext     /* Signaling mode for next packet */
)
{
  cai_data_rate_type frame_rate; /* resulting frame rate */

  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT
  uint32 max_rate;    /* Maximum rate for next frame */
  uint32 min_rate;    /* Mimimum rate for next frame */

  #ifdef FEATURE_IS2000_REL_A_SVD
  boolean rlp_need_tx = FALSE;
  #endif /* FEATURE_IS2000_REL_A_SVD */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* save a copy of the signaling type.*/
  muxvoc_data.signext = signext;

  if (signext == BLANK)
  {
    max_rate = ONEX_VOC_FRAME_RATE_BLANK;  /* Nothing from vocoder, please */
    min_rate = ONEX_VOC_FRAME_RATE_BLANK;
    M1X_MSG( MUX, LEGACY_MED,
      "Request B&B");
  }
  else
  {
    /* Set up vocoder max and min for next packet */
    if ((signext == DIM) && (!muxvoc_data.dim_last))
    {
      max_rate = ONEX_VOC_FRAME_RATE_HALF; /* Dim-and-burst --> force half rate */
      min_rate = ONEX_VOC_FRAME_RATE_EIGHTH;
      muxvoc_data.dim_last = TRUE;
      M1X_MSG( MUX, LEGACY_MED,
        "Request D&B");
    }
    else
    {
      muxvoc_data.dim_last = FALSE;

      max_rate = ONEX_VOC_FRAME_RATE_FULL; /* Full range of rates */

      #ifdef FEATURE_IS2000_REL_A_SVD
      rlp_need_tx = txc_check_for_rlp_signaling();

      if(rlp_need_tx == TRUE)
      {
        max_rate = ONEX_VOC_FRAME_RATE_HALF; /* Pri-and-Sec frame, force half rate */
      }
      #endif /* FEATURE_IS2000_REL_A_SVD */

      min_rate = ONEX_VOC_FRAME_RATE_EIGHTH;

      /* if dipswitch 8 is ON force full rate voice */
      if ( cdma2kdiag_get_switch( CDMA2KDIAG_SW_FORCE_RL_FULL_RATE )  )
      {
        max_rate = ONEX_VOC_FRAME_RATE_FULL;
        min_rate = ONEX_VOC_FRAME_RATE_FULL;
      }
    }
  }

  #ifdef FEATURE_JCDMA
  if (muxvoc_tx_voc_activity.enable == TRUE)
  {
    max_rate = min_rate = muxvoc_voc_activity_test();
  }
  #endif /* FEATURE_JCDMA */

  frame_rate = muxvoc_process_tx_frame(max_rate, min_rate, data_ptr);

  #ifdef FEATURE_IS2000_REL_A_SVD
  if (rlp_need_tx == TRUE)
  {
    M1X_MSG( MUX, LEGACY_MED,
      "RLPSigDimVoc: dim_rate=%d, frame_rate=%d",
      max_rate,
      frame_rate);
  }
  else
  #endif /* FEATURE_IS2000_REL_A_SVD */
  {
    muxvoc_check_for_dim_frame(frame_rate, signext);
  }

  #else /* !FEATURE_MODEM_1X_VOICE_SUPPORT */

  *data_ptr = muxvoc_null_tc_frame;

  if (signext == BLANK)
  {
    frame_rate = CAI_BLANK_RATE;
  }
  else
  {
    frame_rate = CAI_EIGHTH_RATE;
  }
  #endif /* !FEATURE_MODEM_1X_VOICE_SUPPORT */

  return (frame_rate);

} /* txc_voc_traffic */

/*===========================================================================

FUNCTION MUXVOC_SET_VOC_ACTIVITY

DESCRIPTION
  Enable or disable the voice activity test.

DEPENDENCIES
  Only applicable to JCDMA.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_set_voc_activity
(
  boolean enable,   /* enable/disable voice activity test */
  word voc_1,       /* # of full rate */
  word voc_2,       /* # of 1/2 rate */
  word voc_8        /* # of 1/8 rate */
)
{
  #if defined(FEATURE_JCDMA) && defined(FEATURE_MODEM_1X_VOICE_SUPPORT)
  muxvoc_tx_voc_activity.enable = enable;
  muxvoc_tx_voc_activity.voc_1 = voc_1;
  muxvoc_tx_voc_activity.voc_2 = voc_2;
  muxvoc_tx_voc_activity.voc_8 = voc_8;

  muxvoc_tx_voc_activity.cnt = 0;
  #else
  MODEM_1X_UNUSED(enable);
  MODEM_1X_UNUSED(voc_1);
  MODEM_1X_UNUSED(voc_2);
  MODEM_1X_UNUSED(voc_8);
  #endif /* FEATURE_JCDMA && FEATURE_MODEM_1X_VOICE_SUPPORT */
}

/*===========================================================================

FUNCTION MUXVOC_MVS_STANDBY

DESCRIPTION
  Calls MVS_STANDBY with MVS mode.

DEPENDENCIES
  Only for voice SO's.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_mvs_standby( void )
{

  #ifdef FEATURE_MODEM_1X_VOICE_SUPPORT

  #ifdef FEATURE_VOICE_AGENT_ARCHITECTURE

  if( muxvoc_data.voc_open_val.session_context == 0 )
  {
    return;
  }
  
  muxvoc_data.voc_open_val.event_cb( muxvoc_data.voc_open_val.session_context,
                                     ONEX_VOC_EVENT_RQST_STOP,
                                     NULL,
                                     NULL);
  #endif

  rxc_mvs_set_standby_progress_status( TRUE );

  M1X_MSG( MUX, LEGACY_HIGH,
           "Called mvs_standby: mode_option= ONEX_VOC_ID_NONE");
  #endif /* FEATURE_MODEM_1X_VOICE_SUPPORT */
} /* muxvoc_mvs_standby */

/*===========================================================================

FUNCTION MUXVOC_MVS_IS_NULL_SO

DESCRIPTION
  Checks if RX or TX side SO is NULL or not

DEPENDENCIES
  Only for voice SO's.

RETURN VALUE
  True if SO is NULL (RX or TX)

SIDE EFFECTS
  None
===========================================================================*/
boolean muxvoc_mvs_is_null_so( void )
{
  boolean is_null_so = TRUE;

  MUXVOC_ENTER_CRITICAL_SECTION();
  if( ( muxvoc_data.rx_service_option == CAI_SO_NULL ) ||
      ( muxvoc_data.tx_service_option == CAI_SO_NULL ) )
  {
      is_null_so = TRUE;
  }
  else
  {
      is_null_so = FALSE;
  }
  MUXVOC_LEAVE_CRITICAL_SECTION();

  M1X_MSG( MUX, LEGACY_LOW,
           "IS NULL SO RX %d TX %d ORed %d",muxvoc_data.rx_service_option,
           muxvoc_data.tx_service_option,is_null_so);

  return ( is_null_so );
} /* muxvoc_mvs_is_null_so */

/*===========================================================================

FUNCTION MUXVOC_GET_RXC_VOC_INPUT_SIZE

DESCRIPTION
  This function return's number of bytes to be transferred to
  vocoder on Rx side after stripping signalling message/s.

DEPENDENCIES
  None.

RETURN VALUE
  Number of vocoder input bytes

SIDE EFFECTS
  None
===========================================================================*/
uint32 muxvoc_get_rxc_voc_input_size( void )
{
  uint32 voc_size = 0;
  cai_radio_config_type curr_rc = CAI_INIT_RC;

  M1X_MSG( MUX, LEGACY_LOW,
    "Audio debug : FCH inc %d FCH RC %d DCCH inc %d DCCH RC %d Rx rate 0X%8X",
    rxc_so.for_fch.included,rxc_so.for_fch.rc,
    rxc_so.for_dcch.included,rxc_so.for_dcch.rc,
    muxvoc_data.rx_voc_rate );

  if( rxc_so.for_fch.included == TRUE )
  {
    curr_rc = rxc_so.for_fch.rc;
  }
  else if( rxc_so.for_dcch.included == TRUE )
  {
    curr_rc = rxc_so.for_dcch.rc;
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Wrong RC %d",curr_rc);
  }

  switch( curr_rc )
  {
    case CAI_RC_1 :
    case CAI_RC_3:
    case CAI_RC_4:
    case CAI_RC_11:
    {
      switch( muxvoc_data.rx_voc_rate )
      {
        case ONEX_VOC_FRAME_RATE_FULL:
          voc_size = ( ( MUX1_FULL_PRI_LEN+7 )/8 );
          break;
        case ONEX_VOC_FRAME_RATE_HALF:
          voc_size = ( ( MUX1_HALF_PRI_LEN+7 )/8 );
          break;
        case ONEX_VOC_FRAME_RATE_QUARTER:
          voc_size = ( ( MUX1_QTR_PRI_LEN+7 )/8 );
          break;
        case ONEX_VOC_FRAME_RATE_EIGHTH:
          voc_size = ( ( MUX1_EIGHTH_PRI_LEN+7 )/8 );
          break;
        default:
          M1X_MSG( MUX, LEGACY_ERROR,
            "RateSet1 rate 0X%8X",muxvoc_data.rx_voc_rate);
          voc_size = ( ( MUX1_EIGHTH_PRI_LEN+7 )/8 );
          break;
      }
      break;
    }
    case CAI_RC_2:
    case CAI_RC_5:
    {
      switch( muxvoc_data.rx_voc_rate )
      {
        case ONEX_VOC_FRAME_RATE_FULL:
          voc_size = ( ( MUX2_FULL_PRI_LEN+7 )/8 );
          break;
        case ONEX_VOC_FRAME_RATE_HALF:
          voc_size = ( ( MUX2_HALF_PRI_LEN+7 )/8 );
          break;
        case ONEX_VOC_FRAME_RATE_QUARTER:
          voc_size = ( ( MUX2_QTR_PRI_LEN+7 )/8 );
          break;
        case ONEX_VOC_FRAME_RATE_EIGHTH:
          voc_size = ( ( MUX2_EIGHTH_PRI_LEN+7 )/8 );
          break;
        default:
          M1X_MSG( MUX, LEGACY_ERROR,
            "RateSet2 rate 0X%8X",muxvoc_data.rx_voc_rate);
          voc_size = ( ( MUX2_EIGHTH_PRI_LEN+7 )/8 );
          break;
      }
      break;
    }
    default:
      M1X_MSG( MUX, LEGACY_ERROR,
        "Bad RC %d rate 0X%8X",
        curr_rc,muxvoc_data.rx_voc_rate);
      voc_size = ( ( MUX1_EIGHTH_PRI_LEN+7 )/8 );
      break;
  }

  M1X_MSG( MUX, LEGACY_MED,
    "Voc RX Rate 0X%8X RC %d Bytes %d",
    muxvoc_data.rx_voc_rate,curr_rc,voc_size);

  return ( voc_size );
}/* muxvoc_get_rxc_voc_input_size */

#ifdef FEATURE_VOICE_AGENT_ARCHITECTURE
/*===========================================================================

FUNCTION MUXVOC_SEND_VOC_FRAME

DESCRIPTION
  This function will be called when rxc wants to send the decoder frame from
  decoder to the vocoder using dl_cb.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_send_voc_frame( void )
{
  onex_vocoder_buffer_t buffer = {0};
  onex_voc_frame_info_t frame_info;
  byte rxc_bits[ENC_14400_FULL_RATE_SIZE] = { 0 };

  /*-----------------------------------------------------------------------*/

  if( ( muxvoc_data.voc_open_val.session_context == 0 )||
      ( TRUE == muxvoc_mvs_is_null_so() ) )
  {
    return;
  }

  memscpy ( rxc_bits,
            sizeof(muxvoc_data.rxc_bits),
            muxvoc_data.rxc_bits,
            sizeof(muxvoc_data.rxc_bits) );

  muxvoc_data.rx_voice_frames_processed = 0; /* Clear the processed frame count. */
  muxvoc_data.is_first_rx_cb = FALSE;

  frame_info.frame_rate =  muxvoc_data.rx_voc_rate ;

  buffer.frame      = rxc_bits;
  buffer.frame_info = ( void* )&frame_info;
  buffer.size       = muxvoc_get_rxc_voc_input_size();
  buffer.vocoder_id = muxvoc_data.mvs_mode_option ;
  buffer.flags      = TRUE;

  if( ( frame_info.frame_rate == ONEX_VOC_FRAME_RATE_BLANK   )||
      ( frame_info.frame_rate == ONEX_VOC_FRAME_RATE_ERASURE ) 
    )
  {
    buffer.size  = 0;
  }

  muxvoc_data.voc_open_val.dl_cb( muxvoc_data.voc_open_val.session_context,
                                  &buffer );
  M1X_MSG( MUX, LEGACY_MED,
           "send_voc_frame, size=%d,rate=%d,voc_id: %d, data=%x,%x,%x,%x,%x",
           buffer.size,
           frame_info.frame_rate,
           buffer.vocoder_id,
           rxc_bits[0],
           rxc_bits[1],
           rxc_bits[2],
           rxc_bits[3],
           rxc_bits[4] );

  return;
}/* muxvoc_send_voc_frame */

/*===========================================================================

FUNCTION MUXVOC_GET_VOC_FRAME

DESCRIPTION
  This function will be called when txc wants to get the vocoder frame from
  vocoder and send it the decoder.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_get_voc_frame( void )
{
  onex_vocoder_buffer_t buffer = {0};
  byte txc_bits[ENC_14400_FULL_RATE_SIZE];
  onex_voc_frame_info_t frame_info;

  /*-----------------------------------------------------------------------*/

  if( ( muxvoc_data.voc_open_val.session_context == 0 ) ||
      ( TRUE == muxvoc_mvs_is_null_so() ) )
  {
    return;
  }
  
  buffer.frame = txc_bits;
  buffer.size  = sizeof(txc_bits);
  buffer.frame_info = (void*)&frame_info;

  muxvoc_data.voc_open_val.ul_cb( muxvoc_data.voc_open_val.session_context,
                                  &buffer );

  muxvoc_data.txc_voc_data_wr_ptr->txc_voc_rate = ( ( onex_voc_frame_info_t* ) 
                                                        buffer.frame_info )->frame_rate;                                                      

  if( buffer.vocoder_id != muxvoc_data.mvs_mode_option ||
      buffer.flags == FALSE )
  {
    M1X_MSG( MUX, LEGACY_ERROR,
             "Error has occured while getting vocoder frame,"
             "flags %d, voc_id %d",
             buffer.flags,
             buffer.vocoder_id );
    return;
  }

  muxvoc_data.tx_voice_frames_processed = 0; /* Clear the processed frame count. */
  muxvoc_data.is_first_tx_cb = FALSE;

  if( buffer.size <= ENC_14400_FULL_RATE_SIZE )
  {
    /* Save voice frame in TXC local area */
    (void)memscpy (muxvoc_data.txc_voc_data_wr_ptr->txc_bits,
                   sizeof(muxvoc_data.txc_voc_data_wr_ptr->txc_bits),
                   buffer.frame,
                   buffer.size);
  }
  else
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "Vocoder output packet length %d,exceeds limit of %d bytes",
      buffer.size,
      ENC_14400_FULL_RATE_SIZE );
  }

  M1X_MSG( MUX, LEGACY_MED,
           "get_voc_frame, size=%d,rate=%d,voc_id: %d, data=%x,%x,%x,%x,%x",
           buffer.size,
           frame_info.frame_rate,
           buffer.vocoder_id,
           txc_bits[0],
           txc_bits[1],
           txc_bits[2],
           txc_bits[3],
           txc_bits[4] );
  
  return;
}/* muxvoc_get_voc_frame */

/*===========================================================================

FUNCTION MUXVOC_GET_OPEN_PARAMS

DESCRIPTION
  Get the onex_voc_cmd_open_t struct registered by voice adapter to CP before 
  a call begins.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_get_open_params( void )
{

  /*-----------------------------------------------------------------------*/

  if( muxvoc_data.voc_open_val.session_context != 0 )
  {
    return;
  }

  /* this function will collect and keep all the open parameters required, this
     function should be called from CP once it gets the open cdma_call */
  /* store uplink cb */
  /* store downlink cb */
  /* store event cb */

  muxvoc_data.voc_open_val = mcc_get_vadapter_params();
}/* muxvoc_get_open_params */

/*===========================================================================

FUNCTION MUXVOC_SET_MIN_MAX_RATE

DESCRIPTION
  Set the min and max rate.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_set_min_max_rate
( 
  uint32 max_rate,
  uint32 min_rate
)
{

  /*-----------------------------------------------------------------------*/

  if( muxvoc_data.voc_open_val.session_context == 0 )
  {
    return;
  }

  if( ( muxvoc_data.curr_min_max_rates.max_rate !=
                          max_rate ) ||
      ( muxvoc_data.curr_min_max_rates.min_rate != 
                          min_rate ) 
    )
  {
    muxvoc_data.curr_min_max_rates.max_rate = 
                                 max_rate;
    muxvoc_data.curr_min_max_rates.min_rate = 
                                 min_rate;
      
    muxvoc_data.voc_open_val.event_cb( muxvoc_data.voc_open_val.session_context,
                                       ONEX_VOC_EVENT_RQST_MIN_MAX_RATE,
                                       (void*)&(muxvoc_data.curr_min_max_rates),
                                       sizeof(muxvoc_data.curr_min_max_rates));
  }

  return;

}/* muxvoc_set_min_max_rate */

/*===========================================================================

FUNCTION MUXVOC_RESET_SESSION

DESCRIPTION
  Reset session context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
void muxvoc_reset_session( void )
{

  /*-----------------------------------------------------------------------*/

  muxvoc_data.voc_open_val.session_context = 0;
  return;
}/* muxvoc_reset_session */

#endif
