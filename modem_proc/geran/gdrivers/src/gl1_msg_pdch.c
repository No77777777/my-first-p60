/*===========================================================================

        IMPLEMENTATION OF GPRS L1 MESSAGE LAYER CHANNEL FUNCTIONS

GENERAL DESCRIPTION
   This file implements functions that are common to all GPRS Logical channels

EXTERNALIZED FUNCTIONS

gl1_msg_pdch_init
gl1_msg_pdch_terminate


INITIALIZATION AND SEQUENCING REQUIREMENTS
The following is the desired order in which functions should be called

gl1_msg_pdch_init
gl1_msg_pdch_terminate

Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gdrivers/src/gl1_msg_pdch.c#1 $
$DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when        who     what, where, why
--------    ---     --------------------------------------------------------
08/06/16   hd       CR1023028 Use a new parameter to gl1_msg_snr_to_bep() indicate TCH channel mode
28/09/15   pg       CR909305 Update dynamic RxD switching and remove pwr_meas suspension  
29/05/15   pa       CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD. 
09/04/15   cja      CR819956 G2T changes to support new RF API's  FEATURE_NEW_RF_API
13/02/15   pa       CR792105: Allow either GBTA or IRAT. 
16/12/14   jk       CR753423:LIF for Idle,voice and data
31/10/14   xz        CR748816 Extend dual data space for building RF script variable 
12/09/14   fm        CR718734 to avoid RX window is stuck in close state, open rx window when GL1 is in L1_TRANSFER_INIT state 
31/07/14   xz        CR697883 Not allow GBTA if current idle frame is doing TDS IRAT in xfer mode
13/12/13    mc      CR.583042 : COEX : DSDA CS + PS - PS call drops in RxTx overlap scenario
7/16/13     zc      CR470197 Replace memcpy with memscpy
20/06/13    cs      Major Triton TSTS Syncup
20/05/13    cs      TSTS Updates
19/04/12    ws      CR 353512 - Increase SNR threashold to 5dB if AEQ is enabled for SRB loopback
10/04/13    ap      CR472264 DSDS: Fix the gas_id problems when the IQ logging is enabled
24/03/12    ws      CR 341661 - Check SAIC is enabled before checking if SNR is above threshold
                    for SRB loopback modulation sanity
16/02/12    ws      CR 332030. Implement storage of last good modulation for
                    SRB loopback
15/09/11    jj      CR 295428 correction  SNR gating algorithm
15/09/10    cja     Debug to be under feature DEBUG_GSM_SCHEDULER
13/09/10    cja     Add debug for CR251982 and CR251985
14/07/10    ws      Stop 2 x SRB loopback log packet being generated if ASRB is off
15/06/10    dv      case00155521 - GCF 21.9 failure when using ASRB
11/06/10    cs      Fix lint errors
07/05/10    dv      CR237642 - fix compile issue
06/05/10    dv      CR237642 - Add ASRB funtionality
17/03/10    ws      Added featurisation to seperate Data mover code from FEATURE_GSM_MDSP_IR
02/12/09    ws      Added FEATURE_GSM_GPRS_AGC_UPDATE for GPRS/EGPRS AGC
                    Improvements
23/09/09    ps      Resolve Klocwork Buffer Overflow errors
05/08/09    ws      Removed FEATURE_GSM_MDSP_DTM it's now mainlined
29/05/09    ws      FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
01/12/08    cs      Add new include gl1_hw_debug.h
09/09/08    nt      Fix Critical LINT errors
20/05/08    cs      Fix Klocwork array bounds check
14/01/08    cs      Allow for separate IQ logging and saving and fix ts to log
13/12/07    agv     CR 131982 Fix for EGPRS DM causing sleep nacks.
07/09/07    og      Added support for FTM BER calculation in loopback A, B, C
                    and SRB.
10/08/07    nf      Added GPLT support
24/07/06    zyu     Added gpl1_log_srb_hard_decisions(...)
12/08/05    gfr     Modify gl1_hw_save_iq_samples to work with L4
10/11/05    gfr     Delay applying mac mode until next block start
08/24/05    gfr     Schedule rx after getting metrics and data to allow time
                    for EDA cancelled receives
08/05/05    gfr     Added gl1_msg_pdch_get_mac
07/29/05    gfr     DTM/EDA/Class 12 support
06/21/05    gfr     Add support for storing the camped on timeslot
06/16/05    gfr     Added call to gl1_hw_set_iq_samples_tn
06/02/05    gfr     Make handler static
05/25/05    gfr     Featurized prototype for dl_pdch_save_iq_samples
05/25/05    gfr     Add channel-specific IQ sample saving enabling
05/05/05    yh      Code clean up
04/26/05    gfr     Handle invalid rx metrics gracefully
04/21/05    gfr     Move tags into local structure to avoid extra copy
                    Call sys_algo_gprs_agc_reset only when necessary
04/20/05    yh      Remove use of RF fifo
04/12/05    gfr     Consolidate GSM hw layer parameters, remove FT slam code
04/08/05    gfr     Combined GSM and GPRS channel types
04/05/05    gfr     Change type 1 header SNR threshold from 2 dB to 1 dB.
04/03/05    gfr     Use a header-type specific threshold for SNR based
                    EGPRS message discarding.
03/24/05    gfr     Discard EGPRS messages that pass CRC but with low SNR
02/05/05    gfr     Use the burst SNR to calculate GMSK BEP.
01/14/05    gfr     Use the burst info for BEP instead of message metrics.
01/13/05    gfr     Do not access metrics info after the callback
01/12/05    gfr     Lint cleanup
12/21/04    gfr     Add support for IQ sample logging if the CRC fails.
12/07/04    yhong   Lint cleanup
12/06/04    yhong   Support for FEATURE_GSM_MDSP_EGPRS
11/17/04    yhong   Add offset_to_sob and offset_to_burst calculation
10/01/04    gfr     Clean up AGC if data is never available.
09/08/04    gfr     Pack the USF into the EGPRS header.
08/26/04    gfr     Added first_burst parameter to gprs rx command.
07/22/04    gfr     EGPRS support.
01/27/04    gfr     Change usf parameters passed to ul_pdtch.
08/13/03    gfr     Pass correct metrics parameter to AGC
06/03/03    gfr     Pass last attempt parameter when looking for decoded data.
05/12/03    gfr     Only pass USF info to the ul handler when receiving PDTCH
                    and remove burst_schedule code.
05/01/03    gfr     Change featuring from FEATURE_GSM to FEATURE_GSM_GPRS
03/12/03    sg      Fixed incorrect loop parameter.
02/24/03    gfr     Lint cleanup.
02/04/03    gfr     Retrieve data in second frame instead of fourth and
                    remove handler once all data is retrieved.
01/15/03    gfr     Use new burst indication function.
11/15/02    gfr     Return the arfcn in the metrics structure.
10/11/02    gr      Replaced full abort with mdsp supported one.
10/10/02    gr      Modify abort to just zero out callbacks if the dl has
                    already started to avoid an mdsp bug.
10/09/02    gr      Make log callback one frame later instead of two.
10/09/02    gr      Moved decode back to fourth frame for upper layer.
09/06/02    gr      Added support for GPRS AGC.
09/03/02    gr      Modified data decode stage to pull decoded data in
                    frames 2,3 and 4 instead of just 4.
08/28/02    gr      Added abort functionality.
06/28/02    bk      Removed parameters for log callback.
06/28/02   thh      Featurize this whole module under FEATURE_GSM.  This is
                    required to allow dualmode build to make single mode
                    (WCDMA) executable.
04/16/02    bk      Added API to set MAC and getting USF only in 4th metrics
03/14/02    bk      Always get metrics whether metrics callback is installed or not
02/09/02    bk      Changed MSG_HIGH to MSG_LOW
            ...      ..many revisions later..
10/29/01    bk      Initial revision

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* definitions */

#include "geran_variation.h"
#ifdef CUST_H
#include "customer.h"
#endif


#include <stdio.h>
#include "comdef.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#include "gl1_defs.h"
#include "gl1_defs_g.h"
#include "gl1_msgi.h"
#include <stringl/stringl.h>

/* header files to support GPRS NPL1 channel operations */
#include "gl1_msg_pdch.h"

/* header file for agc algorithms */
#include "gl1_sys_algo_gprs.h"

#include "gl1_hw_debug.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"

/* Put in for SRB logging */
#include "gpl1_gprs_log.h"

#include "msg.h"

#ifdef  FEATURE_FTM_EGPRS_BER

#include "ftm_gsm_ber.h"

#endif


#define BITS_PER_WORD 16
#define EXTRACT_2MSB  0xC000

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/
#define FIRST_MSG               1
#define NUM_SM_STATES           9
#define NUM_SM                  3

#define SM_STATE_START_RX_BURST 9
#define SM_STATE_END_RX_BURST   6

#define SM_STATE_OPEN_RX_WIN    8

#define SM_STATE_START_METRICS  6
#define SM_STATE_END_METRICS    3
#define SM_STATE_START_GET_DATA 3
#define SM_STATE_END_GET_DATA   1

#define SM_STATE_START_LOG_DATA 7
#define SM_STATE_END_LOG_DATA   4

#define SM_STATE_DONE           0

/* The tag index to use for getting decoded data */
#define DECODE_TAG 3

#define SNR_AVG_THRESHOLD  1000

typedef struct
{
   boolean                      activate_sm;
   uint8                        state;
   gl1_defs_rx_pkt_data_type    *data_mem[GL1_DEFS_MAX_DL_MSGS];
   gl1_defs_rx_pkt_metrics_type *metrics_mem[GL1_DEFS_FRAMES_IN_BLK_PERIOD][GL1_DEFS_MAX_DL_MSGS];
   sm_rx_params_type            rx_params;
   sm_cfg_params_type           cfg_params;
   gl1_defs_rx_pkt_alloc_type   usfs[GL1_DEFS_MAX_DL_MSGS];
   gl1_hw_rx_tags_type          tags[GL1_DEFS_FRAMES_IN_BLK_PERIOD][GL1_DEFS_MAX_DL_MSGS];
   gl1_defs_tn_type             camp_tn;
   uint16                       bep[GL1_DEFS_FRAMES_IN_BLK_PERIOD][GL1_DEFS_MAX_DL_MSGS];
   uint16                       snr[GL1_DEFS_FRAMES_IN_BLK_PERIOD][GL1_DEFS_MAX_DL_MSGS];
   boolean                      schedule_error[GL1_DEFS_FRAMES_IN_BLK_PERIOD][GL1_DEFS_MAX_DL_MSGS];
} gl1_msg_pdch_sm_type;



typedef struct
{
  gl1_msg_pdch_sm_type sm[NUM_SM];

  uint8 num_sm_active;
  uint8 curr_sm_num;

  uint8 timing_advance;

  /* MAC mode flags */
  gl1_defs_access_mode_type mac_mode;
  gl1_defs_access_mode_type pending_mac_mode;
  boolean set_mac_mode;

  /* Current camped on timeslot */
  gl1_defs_tn_type pdch_camp_tn;

} gl1_msg_pdch_params_sm_t;

static gl1_msg_pdch_params_sm_t gl1_msg_pdch_params_sm[NUM_GERAN_DATA_SPACES];

static boolean reg_rx_hdlr_flag[NUM_GERAN_DATA_SPACES] =
                 { INITIAL_VALUE( FALSE ) };

static boolean  rx_window_open[NUM_GERAN_DATA_SPACES]=
                 { INITIAL_VALUE( TRUE ) };

#ifdef FEATURE_GSM_MDSP_EGPRS
/* Structures for storing loopback data */
typedef struct
{
   boolean                  valid;
   gl1_defs_tn_type         tn;
   gl1_defs_modulation_type modulation;  /* GMSK or 8PSK */
   uint16                   hard_decisions[MDSP_MAX_HARD_DECISIONS];
   uint16                   snr;
   gl1_defs_modulation_type last_good_mod;  /* GMSK or 8PSK */

} srb_loopback_burst_type;

typedef struct
{
   srb_loopback_burst_type burst[GL1_DEFS_MAX_DL_MSGS];
} srb_loopback_frame_type;

typedef struct
{
   srb_loopback_frame_type frame[GL1_DEFS_FRAMES_IN_BLK_PERIOD];

  /* Flag if we are in srb loopback mode */
   boolean srb_loopback;
} srb_loopback_block_type;


typedef struct
{
   srb_loopback_frame_type frame[12];

   int32 next_loopback_wr_frame[GL1_DEFS_MAX_DL_MSGS];
   int32 next_loopback_wr_word[GL1_DEFS_MAX_DL_MSGS];
   uint16 next_loopback_data_gmsk[GL1_DEFS_MAX_DL_MSGS];
   int32 next_loopback_rd_frame[GL1_DEFS_MAX_DL_MSGS];

   boolean first_time;
   boolean sync_up;
   boolean asrb_gmsk_ul_enable;
}  asrb_loopback_block_type;

static asrb_loopback_block_type loopback_8psk[NUM_GERAN_DATA_SPACES];

static void gl1_msg_asrb_process_hard_decisions (srb_loopback_burst_type *burst,gas_id_t gas_id);


/* Storage for the loopback hard decisions */
static srb_loopback_block_type loopback_block[NUM_GERAN_DATA_SPACES];

#endif

/* SNR threshold below which we discard any headers that pass CRC, each
   header type has a different value, but we index by coding scheme for
   simplicity and possible later expansion. */

#define EGPRS_HDR_SNR_THRESHOLD_TYPE_3_AEQ  406  /* 2db dB */

#define EGPRS_HDR_SNR_THRESHOLD_TYPE_3      228  /* -0.5 dB */
#define EGPRS_HDR_SNR_THRESHOLD_TYPE_2      287  /* 0.5 dB */
#define EGPRS_HDR_SNR_THRESHOLD_TYPE_1      322  /* 1.0 dB */


const uint16 EGPRS_HDR_SNR_THRESHOLD_AEQ[] =
{
   EGPRS_HDR_SNR_THRESHOLD_TYPE_3_AEQ,  /* MCS1 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_3_AEQ,  /* MCS2 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_3_AEQ,  /* MCS3 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_3_AEQ,  /* MCS4 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_2,  /* MCS5 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_2,  /* MCS6 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_1,  /* MCS7 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_1,  /* MCS8 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_1   /* MCS9 */
};

const uint16 EGPRS_HDR_SNR_THRESHOLD[] =
{
   EGPRS_HDR_SNR_THRESHOLD_TYPE_3,  /* MCS1 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_3,  /* MCS2 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_3,  /* MCS3 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_3,  /* MCS4 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_2,  /* MCS5 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_2,  /* MCS6 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_1,  /* MCS7 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_1,  /* MCS8 */
   EGPRS_HDR_SNR_THRESHOLD_TYPE_1   /* MCS9 */
};



/* Prototypes */
static boolean gl1_msg_pbcch_pccch_pacch_pdtch_handler
(
   boolean  next_frame_is_chn_dl_block_boundary,
   uint32   time_stamp,
   gas_id_t gas_id
);


static void dl_pdch_process_egprs_data
(
   gl1_defs_rx_pkt_data_type *data_info,
   gl1_msg_pdch_sm_type *tsm,
   uint8 ind,
   gas_id_t gas_id
);

/*===========================================================================

FUNCTION gl1_msg_dm_complete

DESCRIPTION
  This function reads the exported value of dm_complete which tells
  the Host that Data Mover (DM) is complete.

DEPENDENCIES
  None

RETURN VALUE
  boolean         DM finished or not.

SIDE EFFECTS
  None
===========================================================================*/
#if defined (FEATURE_GSM_MDSP_IR)&& !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
boolean gl1_msg_dm_complete (gas_id_t gas_id)
{
  return (gl1_hw_dm_complete(gas_id));
}
#endif
/*===========================================================================
FUNCTION gl1_msgi_pdch_init

DESCRIPTION
   This function initializes data structures that are used by all PDCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_pdch_init (gas_id_t gas_id)
{
   uint8 sm_num = 0;

   /* Initialize the state machines */
   gl1_msg_pdch_params_sm[gas_id].num_sm_active = 0;
   gl1_msg_pdch_params_sm[gas_id].curr_sm_num   = 0;
   gl1_msg_pdch_params_sm[gas_id].timing_advance = 0;

   /* Start out in FA mode */
   gl1_msg_pdch_params_sm[gas_id].mac_mode = gl1_msg_pdch_params_sm[gas_id].pending_mac_mode = GL1_DEFS_FIXED_ALLOC;
   gl1_msg_pdch_params_sm[gas_id].set_mac_mode = TRUE;

   gl1_msg_pdch_params_sm[gas_id].pdch_camp_tn = GL1_DEFS_TN_0;

   for( sm_num = 0; sm_num < NUM_SM ; sm_num++ )
   {
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].state = 0;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.rx_pkt_cb.data_callback      = NULL;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.rx_pkt_cb.log_callback       = NULL;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.rx_pkt_cb.metrics_callback   = NULL;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.rx_pkt_cb.discarded_callback = NULL;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].rx_params.num_msgs = 0;
   }

   loopback_block[gas_id].srb_loopback = FALSE;
   loopback_8psk[gas_id].asrb_gmsk_ul_enable = FALSE;

   ALLOW_RX_HDLR_TO_BE_REG(gas_id);
   OPEN_RX_WINDOW(gas_id);
}

/*===========================================================================
FUNCTION gl1_msg_pdch_terminate

DESCRIPTION
   Terminate the Uplink and Downlink logical channels. No further channel
   operations or monitoring may take place after this function is called.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_pdch_terminate (gas_id_t gas_id)
{
   uint8 sm_num;

   if (IS_RX_HDLR_REG(gas_id))
   {
      gl1_msgi_remove_ft_handler(gl1_msg_pbcch_pccch_pacch_pdtch_handler,gas_id);
   }

   /* Reset the state machines */
   gl1_msg_pdch_params_sm[gas_id].num_sm_active = 0;
   gl1_msg_pdch_params_sm[gas_id].curr_sm_num   = 0;
   gl1_msg_pdch_params_sm[gas_id].timing_advance = 0;

   for( sm_num = 0; sm_num < NUM_SM ; sm_num++ )
   {
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].state = 0;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.rx_pkt_cb.data_callback      = NULL;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.rx_pkt_cb.log_callback       = NULL;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.rx_pkt_cb.metrics_callback   = NULL;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.rx_pkt_cb.discarded_callback = NULL;
      gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].rx_params.num_msgs = 0;
   }

   DONT_ALLOW_RX_HDLR_TO_BE_REG(gas_id);
   CLOSE_RX_WINDOW(gas_id);
}



/*===========================================================================

FUNCTION gl1_msgi_store_data_mem_info

DESCRIPTION
   Stores information about the data pointers. A 2-D array of data pointers is
   maintained. The row denotes a block number and the column denotes the
   messages in the block. Upto 3 block worth of information needs to be
   maintained.
   Every message has a corresponding data pointer associated with it. When a
   message is submitted in a particular block the data pointer is stored and
   can later be retrieved.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_pdch_store_sm_info
(
   const gl1_defs_rx_pkt_mem_type *mem_info,
   const sm_rx_params_type        *rx_params,
   const sm_cfg_params_type       *cfg_params,
   gas_id_t                        gas_id
)
{
    uint8 frame_num;
    uint8 msg_num = 0;
    gl1_msg_pdch_sm_type *curr_sm = &gl1_msg_pdch_params_sm[gas_id].sm[gl1_msg_pdch_params_sm[gas_id].curr_sm_num ];

    if ( IS_RX_WINDOW_OPEN(gas_id) == FALSE )
    {
       /* got receive command in frames other than 1, 2 or 3. */
       MSG_GERAN_ERROR_0_G("Got Receive Cmd while Rx window was closed\n");
       return;
    }

    if ( !cfg_params || !rx_params || !mem_info )
    {
       MSG_GERAN_ERROR_3_G("Invalid params %d %d %d", cfg_params, rx_params, mem_info);
       return;
    }

    /*--------------------------------------------------------------------------
     Incr the number of message received
    --------------------------------------------------------------------------*/
    msg_num       = curr_sm->rx_params.num_msgs;

    if ( msg_num >= GL1_DEFS_MAX_DL_MSGS )
    {
       MSG_GERAN_ERROR_2_G("Too many rx msgs, chan=%d, FN=%d",
                 cfg_params->chan_type, GSTMR_GET_FN_GERAN(gas_id));
       return;
    }

    curr_sm->rx_params.num_msgs++;

    /*--------------------------------------------------------------------------
     Copy the data pointers and metrics pointers for all four frames.
     -------------------------------------------------------------------------*/
    curr_sm->data_mem[ msg_num ]= mem_info->msg_data_mem;

    for( frame_num = 0; frame_num < GL1_DEFS_FRAMES_IN_BLK_PERIOD; frame_num++ )
    {
       curr_sm->metrics_mem[frame_num ] [ msg_num ]
           = mem_info->metrics_data_mem[ frame_num ];

       /* Copy the frequencies for all 4 frames */
       curr_sm->rx_params.arfcn[frame_num] = rx_params->arfcn[ frame_num ];
    }

    curr_sm->rx_params.search_width = rx_params->search_width;
    curr_sm->rx_params.agc_ptr = rx_params->agc_ptr;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
    curr_sm->rx_params.agc_ptr_divrx = rx_params->agc_ptr_divrx;
#endif

    /* signal info is always stored in index 0 of the array in the param */
    curr_sm->rx_params.signal_info[ msg_num ] = rx_params->signal_info[ 0 ];
    curr_sm->cfg_params                       = *cfg_params;

    /* Indicate that at the next block boundary, the sm should be activated */
    curr_sm->activate_sm                      = TRUE;

    /* Latch the camped timeslot */
    curr_sm->camp_tn                          = gl1_msg_pdch_get_camp_tn(gas_id);

    /*--------------------------------------------------------------------------
       Check if this is the first message received for this state machine, if
       so increment the number of state machines that are active.
    ---------------------------------------------------------------------------*/
    if ( curr_sm->rx_params.num_msgs == FIRST_MSG )
    {
       gl1_msg_pdch_params_sm[gas_id].num_sm_active++;
    }

   /*---------------------------------------------------------------------------
   * Check if the handler is installed, if not then install the handler.
   * and dont allow any further handlers to be registered
   ---------------------------------------------------------------------------*/
   if ( IS_RX_HDLR_REG(gas_id) == FALSE )
   {
      gl1_msgi_add_ft_handler
       (
         gl1_msg_pbcch_pccch_pacch_pdtch_handler,
         GL1_MSG_FT_HDLR_DL_PDCH,
         gas_id
       );

      DONT_ALLOW_RX_HDLR_TO_BE_REG(gas_id);
   }

}

/*===========================================================================

FUNCTION gl1_msg_pdch_4frame_handler

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
static boolean gl1_msg_pbcch_pccch_pacch_pdtch_handler
(
   boolean     next_frame_is_chn_dl_block_boundary,
   uint32      time_stamp,
   gas_id_t    gas_id
)
{
   /*---------------------------------------------------------------------------
   * By default initialize the decode flag to false.
   ---------------------------------------------------------------------------*/
   uint8   sm_num;
   boolean remove_hdlr  = FALSE;
   boolean data_complete;
   uint16 local_snr;
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
   boolean is_block_start = FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

   gl1_msg_pdch_sm_type *curr_sm = &gl1_msg_pdch_params_sm[gas_id].sm[gl1_msg_pdch_params_sm[gas_id].curr_sm_num ];

   /*---------------------------------------------------------------------------
   * Processing to be done in the 4th frame of the block
   ----------------------------------------------------------------------------*/
   if( next_frame_is_chn_dl_block_boundary )
   {
      if ( curr_sm->activate_sm == TRUE )
      {
         CLOSE_RX_WINDOW(gas_id);

         /* Activate the state machine and indicate the sm has been activated */
         curr_sm->state       = NUM_SM_STATES;
         curr_sm->activate_sm = FALSE;

         /* Now that this state machine is active, incr. to use the next one */
         gl1_msg_pdch_params_sm[gas_id].curr_sm_num++;
         if ( gl1_msg_pdch_params_sm[gas_id].curr_sm_num == NUM_SM )
         {
            gl1_msg_pdch_params_sm[gas_id].curr_sm_num = 0;
         }
      }
   }

   /*---------------------------------------------------------------------------
    Processing receives take upto 3 blocks, hence 3 state machines are
    maintained to handle back to back receives
   ---------------------------------------------------------------------------*/
   for ( sm_num = 0; sm_num < NUM_SM; sm_num++ )
   {
     uint8 burst_num;
     gl1_msg_pdch_sm_type *tsm = &gl1_msg_pdch_params_sm[gas_id].sm[sm_num];

     data_complete = FALSE;

     /* Check if this state machine is active */
     if  (tsm->state == SM_STATE_DONE)
     {
        continue;
     }

     /* If this is the 1st frame of the block, open the rx window */
     if (tsm->state == SM_STATE_OPEN_RX_WIN)
     {
        OPEN_RX_WINDOW(gas_id);
     }


     /*------------------------------------------------------------------------
        Processing Metrics Retrieval

        This should come before the rx scheduling to allow for the cancelling
        of receives in extended dynamic allocation
     ------------------------------------------------------------------------*/
     if(
         tsm->state <= SM_STATE_START_METRICS &&
         tsm->state >= SM_STATE_END_METRICS
       )
     {
        uint8 metrics_fn, i;
        uint8 num_metrics_msgs, num_discarded;
        gl1_defs_rx_pkt_metrics_callback metrics_cb;
        gl1_defs_rx_pkt_discarded_callback discarded_cb;
        gl1_defs_rx_pkt_metrics_type **metrics_info;
        boolean           get_usf_info = FALSE;
        boolean           msg_complete;
        gl1_defs_tn_type  discarded_tn[GL1_DEFS_MAX_DL_MSGS];

        /* From the sm state, derive the index into the array */
        metrics_fn = SM_STATE_START_METRICS - tsm->state;
        num_metrics_msgs = tsm->rx_params.num_msgs;
        metrics_cb       = tsm->cfg_params.rx_pkt_cb.metrics_callback;
        discarded_cb     = tsm->cfg_params.rx_pkt_cb.discarded_callback;

        /*--------------------------------------------------------------------
           Get the USF info while retrieving the 4th metrics of the block.
        --------------------------------------------------------------------*/
        if (tsm->state == SM_STATE_END_METRICS && gl1_msg_pdch_params_sm[gas_id].mac_mode != GL1_DEFS_FIXED_ALLOC)
        {
           get_usf_info = TRUE;
        }

        /*--------------------------------------------------------------------
           Always get the metrics, regardless whether the callback is installed
           or not. The is a AGC related frame layer requirement.
           Metrics need to be retrieved *before* checking for data.
        --------------------------------------------------------------------*/
        metrics_info =  tsm->metrics_mem[ metrics_fn ];

        /* Fill in the known values for the burst metrics, tn is used in
           gl1_hw_get_rx_metrics */
        for (i = 0; i < num_metrics_msgs; i++)
        {
           metrics_info[i]->arfcn = tsm->rx_params.arfcn[metrics_fn];
           metrics_info[i]->tn = tsm->rx_params.signal_info[i].tn;
           metrics_info[i]->camp_tn = tsm->camp_tn;
           metrics_info[i]->offset_to_burst = 2;
        }

        /* Retrieve metrics from the frame layer */
        gl1_hw_get_rx_metrics
                   (
                      metrics_info,
                      num_metrics_msgs,
                      get_usf_info,
                      loopback_block[gas_id].srb_loopback,
                      tsm->tags[metrics_fn],
#ifdef GERAN_L1_ENHANCED_RX
                      GL1_MSG_PS_PACCH_PDTCH,
#endif /*GERAN_L1_ENHANCED_RX*/
                      gas_id
                   );

        /* If this is the first frame of the block in extended dynamic
           allocation we check if the mDSP discarded any of the msgs */
        if (gl1_msg_pdch_params_sm[gas_id].mac_mode == GL1_DEFS_EXT_DYNAMIC_ALLOC &&
            tsm->state == SM_STATE_START_METRICS)
        {
           /* Messages will only get discarded from the end for EDA, so start
              from there and find the first non-discarded message.  Skip
              the first message since it should never get discarded - and
              frankly I'm not sure how the rest of the handler would deal
              with num_msgs = 0 anyway. */
           num_discarded = 0;

           /* Bounds check to resolve Klocwork error */
           for (i = num_metrics_msgs - 1;((i > 0) && (i < GL1_DEFS_MAX_DL_MSGS)); i--)
           {
              if (metrics_info[i]->schedule_status & GL1_DEFS_RX_SCHEDULED)
              {
                 break;
              }

              /* Save the timeslot for the callback */
              discarded_tn[i] = metrics_info[i]->tn;
              num_discarded++;
           }

           /* Check if any were discarded */
           if (num_discarded > 0)
           {
              /* Make the callback, shift the tn list to the proper start */
              if ((discarded_cb != NULL) && (i < GL1_DEFS_MAX_DL_MSGS-1))
              {
                 discarded_cb(&discarded_tn[i + 1], num_discarded);
              }

              /* Drop the discarded messages from all further processing */
              tsm->rx_params.num_msgs -= num_discarded;
              num_metrics_msgs = tsm->rx_params.num_msgs;
              MSG_GERAN_MED_2_G("%d Rx discarded by MDSP. Num Rx msgs now %d", num_discarded, tsm->rx_params.num_msgs);
           }
        }

        /* Check schedule status, save the SNR and calculate the BEP */
        for (i = 0; i < num_metrics_msgs; i++)
        {
           tsm->schedule_error[metrics_fn][i] = FALSE;
           /* At this point any unscheduled rx is an error */
           if((metrics_info[i]->schedule_status & GL1_DEFS_RX_SCHEDULED) == 0)
           {
              MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
                    "Rx PDTCH not scheduled: tn=%d (%d), reason=%04x, FN=%d",
                    metrics_info[i]->tn,metrics_info[i]->camp_tn,
                    metrics_info[i]->schedule_status, GSTMR_GET_FN_GERAN(gas_id));
              tsm->schedule_error[metrics_fn][i] = TRUE;
           }

           /* Save the SNR for use later when getting the decoded data */
           tsm->snr[metrics_fn][i] = (metrics_info[i]->valid ? metrics_info[i]->snr : 0);

           /* Calculate the BEP */
           tsm->bep[metrics_fn][i] =
             gl1_msg_snr_to_bep
              (
                tsm->snr[metrics_fn][i],
                metrics_info[i]->modulation,
                FALSE,
                gas_id
              );
        }

        /* Send to AGC.  We set the msg_complete flag to false if there
           is no data callback to trick the agc into ignoring the metrics
           if this is a PDTCH channel that the PL1 is not processing. */
        msg_complete = (tsm->state == SM_STATE_START_METRICS &&
             tsm->cfg_params.rx_pkt_cb.data_callback != 0);
        sys_algo_gprs_agc_update
          (
            tsm->rx_params.agc_ptr,
#if defined (FEATURE_GSM_RX_DIVERSITY_DATA)
            tsm->rx_params.agc_ptr_divrx,
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA */
            metrics_info,
            num_metrics_msgs,
            tsm->cfg_params.chan_type,
            time_stamp,
            msg_complete,
            gas_id
          );

#ifdef FEATURE_GSM_MDSP_EGPRS
        /* Fill in the the loopback information */
        if (loopback_block[gas_id].srb_loopback)
        {

#ifdef  FEATURE_FTM_EGPRS_BER
           /* Used to provide FTM with hard decisions in
            * 8bit word format.
            * Declare as three times the size to allow for 8PSK.
            */
           static  uint8  hard_decision_8bit_word[NUM_GERAN_DATA_SPACES][ (3*16) ];

           uint32  current_fn = GSTMR_GET_FN_GERAN(gas_id);
#endif
           if( loopback_8psk[gas_id].first_time && (metrics_info[0]->snr > 722) &&
               (metrics_info[0]->modulation == GL1_DEFS_MODULATION_8PSK) ) /* burst must have SNR of 4.5dB */
           {
              switch( frame_counters[gas_id].FNmod13 )
              {
                case 2:
                case 6:
                case 10:
                  loopback_8psk[gas_id].first_time = FALSE;
                  loopback_8psk[gas_id].sync_up    = TRUE;
                break;

                default:
                break;
              }
           }

           MSG_4(MSG_SSID_DFLT, MSG_LEGACY_MED,
                 "FN %d frame_counters[gas_id].FNmod13 %d loopback_8psk[gas_id].first_time %d synch_up %d",
                 GSTMR_GET_FN_GERAN(gas_id), frame_counters[gas_id].FNmod13, loopback_8psk[gas_id].first_time, loopback_8psk[gas_id].sync_up);

           for (i = 0; i < num_metrics_msgs; i++)
           {
              loopback_block[gas_id].frame[metrics_fn].burst[i].valid = TRUE;
              loopback_block[gas_id].frame[metrics_fn].burst[i].tn = metrics_info[i]->tn;
              loopback_block[gas_id].frame[metrics_fn].burst[i].snr = metrics_info[i]->pre_mimo_trained_complete_snr;
              loopback_block[gas_id].frame[metrics_fn].burst[i].modulation =
                metrics_info[i]->modulation;

              if(gl1_msg_saic_in_burst(metrics_info[i]->misc_flags))
              {
                local_snr = metrics_info[i]->pre_mimo_trained_complete_snr;
              }
              else
              {
                local_snr = metrics_info[i]->snr;
              }

              if ( IS_FTM_IN_TEST_MODE() )
              {
                /* If more than 5dB trust modulation */
                if(local_snr > FIVE_DB_SNR)
                {
                  loopback_block[gas_id].frame[metrics_fn].burst[i].last_good_mod =  metrics_info[i]->modulation;
                }
                else
                {
                  MSG_GERAN_HIGH_3_G("Bad SNR < 5db snr %d mod %d last good mod %d",
                           local_snr,
                           metrics_info[i]->modulation,loopback_block[gas_id].frame[metrics_fn].burst[i].last_good_mod);
                }
              }
              memscpy
               (
                 loopback_block[gas_id].frame[metrics_fn].burst[i].hard_decisions,
                 sizeof(loopback_block[gas_id].frame[metrics_fn].burst[i].hard_decisions),
                 metrics_info[i]->hard_decisions,
                 GL1_DEFS_MAX_HARD_DECISIONS * 2
               );

#ifndef FEATURE_GSM_PLT
              /* Log hard descisions for debug, MCS not used for now */
              gpl1_log_srb_hard_decisions(GSTMR_GET_FN_GERAN(gas_id), metrics_info[i]->hard_decisions ,metrics_info[i]->modulation,gas_id);
#endif

              if ( loopback_8psk[gas_id].asrb_gmsk_ul_enable && loopback_8psk[gas_id].sync_up )
              {
                gl1_msg_asrb_process_hard_decisions
                (
                  &loopback_block[gas_id].frame[metrics_fn].burst[i],gas_id
                );

#ifndef FEATURE_GSM_PLT
                /* Log hard descisions for debug, MCS not used for now. This logs re-packed UL burst*/
                gpl1_log_srb_hard_decisions(GSTMR_GET_FN_GERAN(gas_id),loopback_block[gas_id].frame[metrics_fn].burst[i].hard_decisions ,0,gas_id);
#endif
              }



#ifdef  FEATURE_FTM_EGPRS_BER
             switch( metrics_info[i]->modulation )
             {
             case  GL1_DEFS_MODULATION_GMSK:
             {
              /* local loop counter */
               uint32 count = 0;
               ftm_gsm_ber_rx_data_ind_type    rx_data_ind;


               while( count < 8 )
               {
                 /* copy from field n to fields 2n and 2n+1 */
                 /*   bits d(k) . . . d(k+7) d(k+8) . . . d(k+15) in one 16 bit word */
                 /*   store as d(k) . . . d(k+7) as 1 byte followed by
                  *            d(k+8) . . . d(k+15) in next byte
                  *   k = 16*n
                  */
                 hard_decision_8bit_word[gas_id][ (count<<1) ]       = (uint8) (metrics_info[i]->hard_decisions[ count ]>>8);
                 hard_decision_8bit_word[gas_id][ ((count<<1) + 1) ] = (uint8) metrics_info[i]->hard_decisions[ count ];

                 count++;
               }

               /* Mask off bits 4 - 7 (inclusive) of the 15th 8bit word (index 14)
                * and all bits in the 16th 8bit word (index 15).
                */
               hard_decision_8bit_word[gas_id][ 14 ] &= 0xF0;

               hard_decision_8bit_word[gas_id][ 15 ] &= 0x0;


               rx_data_ind.fn       = current_fn;                  /* fn */
               rx_data_ind.slot_num = ADD_MOD( (uint8)metrics_info[i]->tn,
                                               (uint8)metrics_info[i]->camp_tn,
                                               8);                     /* timeslot */





               rx_data_ind.bfi      = TRUE;                            /* bfi boolean     */
               rx_data_ind.num_bits = 116;                             /* uint16 num bits */
               rx_data_ind.data     = &(hard_decision_8bit_word[gas_id][0]);   /* uint8* data     */

               /* Send the hard decisions to FTM */
               ftm_gsm_ber_send_rx_data_to_ftm(  &rx_data_ind );

             }
               break;

             case  GL1_DEFS_MODULATION_8PSK:
             {
               /* local loop counter */
               uint32 count = 0;
               ftm_gsm_ber_rx_data_ind_type    rx_data_ind;

               /* We are looping 2 times more than needed, keep this way for
                * now since it simplifies the copying loop.
                */
               while( count < (GL1_DEFS_MAX_HARD_DECISIONS - 1) )
               {
                 /* copy from field n to fields 2n and 2n+1 */
                 hard_decision_8bit_word[gas_id][ (count<<1) ]       = (uint8) (metrics_info[i]->hard_decisions[ count ]>>8);
                 hard_decision_8bit_word[gas_id][ ((count<<1) + 1) ] = (uint8) metrics_info[i]->hard_decisions[ count ];

                 count++;   /* iterate again */

                 /* copy from field n to fields 2n and 2n+1 */
                 hard_decision_8bit_word[gas_id][ (count<<1) ]       = (uint8) (metrics_info[i]->hard_decisions[ count ]>>8);
                 hard_decision_8bit_word[gas_id][ ((count<<1) + 1) ] = (uint8) metrics_info[i]->hard_decisions[ count ];

                 count++;
               }

               /* Mask off bits 4-7 of the 44th 8bit word (index 43)
                * and all bits in words 45, 46, 47, 48.
                */
               hard_decision_8bit_word[gas_id][ 43 ] &= 0xF0;

               hard_decision_8bit_word[gas_id][ 44 ] &= 0x0;
               hard_decision_8bit_word[gas_id][ 45 ] &= 0x0;
               hard_decision_8bit_word[gas_id][ 46 ] &= 0x0;
               hard_decision_8bit_word[gas_id][ 47 ] &= 0x0;

               rx_data_ind.fn       = current_fn;                  /* fn */
               rx_data_ind.slot_num = ADD_MOD( (uint8)metrics_info[i]->tn,
                                               (uint8)metrics_info[i]->camp_tn,
                                               8);                     /* timeslot */





               rx_data_ind.bfi      = TRUE;                            /* bfi boolean     */
               rx_data_ind.num_bits = (116*3);                         /* uint16 num bits */
               rx_data_ind.data     = &(hard_decision_8bit_word[gas_id][0]);   /* uint8* data     */

               /* Send the hard decisions to FTM */
               ftm_gsm_ber_send_rx_data_to_ftm(  &rx_data_ind );

             }
               break;

             default:
               MSG_GERAN_ERROR_1_G(" Invalid modulation type %d ", metrics_info[i]->modulation);
               break;
             } /* End of switch statement on modulation scheme */

#endif  /* FEATURE_FTM_EGPRS_BER */

           } /* End of for( ) loop */
        }    /* End of if( ) srb is on */

#endif /* FEATURE_GSM_MDSP_EGPRS */

        /* Save usf info and provide it to the uplink handler */
        if (get_usf_info)
        {
           for (i = 0; i < num_metrics_msgs; i++)
           {
              tsm->usfs[i].tn = metrics_info[i]->tn;
              tsm->usfs[i].usf = metrics_info[i]->usf;
           }
           if (tsm->cfg_params.chan_type == GL1_HW_PDTCHD)
           {
              gl1_msg_set_usf_alloc_info(tsm->usfs, num_metrics_msgs,gas_id);
           }
        }

        /* Ship up the metrics - do this last since once we make the callback
           we should no longer read from the metrics_info pointer */
        if (metrics_cb)
        {
           metrics_cb(metrics_info, num_metrics_msgs, metrics_fn,gas_id);
        }
     }

     /*------------------------------------------------------------------------
                 Log Data
     ------------------------------------------------------------------------*/
     if (
         tsm->state <= SM_STATE_START_LOG_DATA &&
         tsm->state >= SM_STATE_END_LOG_DATA
        )
     {
        gl1_defs_rx_pkt_log_callback     log_cb;
        uint8 log_fn;

        log_fn = SM_STATE_START_LOG_DATA - tsm->state;
        log_cb  = tsm->cfg_params.rx_pkt_cb.log_callback;

        if (log_cb)
        {
           log_cb(log_fn);
        }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
        gl1_hw_get_iq_samples( GL1_HW_PDTCHD, sm_num, log_fn, tsm->cfg_params.tsc, gas_id );
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */
     }

     /*------------------------------------------------------------------------
                 Processing Data Retrieval
     ------------------------------------------------------------------------*/
     if (  tsm->state <= SM_STATE_START_GET_DATA &&
           tsm->state >= SM_STATE_END_GET_DATA )
     {
        uint8 ind, num_data_msgs;
        boolean last_attempt, data_available;
        gl1_defs_rx_pkt_data_type *data_info;
        gl1_defs_rx_pkt_data_callback data_cb;

        num_data_msgs = tsm->rx_params.num_msgs;
        last_attempt = (tsm->state == SM_STATE_END_GET_DATA);
        data_complete = TRUE;

        /* Grab the data for all the received messages */
        for ( ind = 0; ind < num_data_msgs; ind++ )
        {
           /* If we already retrieved this message, go to the next one */
           data_info = tsm->data_mem[ind];
           if (data_info == NULL) continue;

           data_available = gl1_hw_get_rx_data
               (
                  FALSE,  /* not PTCCH */
                  tsm->rx_params.signal_info[ind].tn,
                  &tsm->tags[DECODE_TAG][ind],
                  last_attempt,
                  data_info,
                  gas_id
               );

           /* Send up the data if it is available or this is the last try */
           if (data_available || last_attempt)
           {
              /* Set the offset-to-start-of-burst and timeslot */
              data_info->offset_to_sob = SM_STATE_START_RX_BURST - tsm->state;
              data_info->tn = tsm->rx_params.signal_info[ind].tn;
              data_info->camp_tn = tsm->camp_tn;

              if (data_available)
              {

                 /* Perform some EGPRS specific activity */
                 if (IS_EGPRS_CODING(data_info->coding_scheme))
                 {
                    dl_pdch_process_egprs_data(data_info, tsm, ind,gas_id);
                 }
              }
              else
              {
                 /* Unwind AGC */
                 sys_algo_agc_process_pdtch
                  (
                    tsm->rx_params.agc_ptr,
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
                    tsm->rx_params.agc_ptr_divrx,
#endif
                    data_info->tn,
                    0,     /* Pr is do not care */
                    FALSE, /* Not valid */
                    gas_id
                  );
              }

              /* Do not try and retrieve this message again */
              tsm->data_mem[ind] = NULL;

              /* Invoke the callback if it exists */
              data_cb = tsm->cfg_params.rx_pkt_cb.data_callback;
              if (data_cb)
              {
                 data_cb(data_info, data_available,gas_id);
              }
           }
           else
           {
              data_complete = FALSE;
           }
        }
     }


     /*------------------------------------------------------------------------
                 Processing Receive Bursts
     ------------------------------------------------------------------------*/
     if (tsm->state <= SM_STATE_START_RX_BURST &&
         tsm->state >= SM_STATE_END_RX_BURST)
     {
         gl1_hw_rx_pkt_burst_type  burst_info;

         burst_num = SM_STATE_START_RX_BURST - tsm->state;

         burst_info.arfcn        = tsm->rx_params.arfcn[burst_num];
         burst_info.tsc          = tsm->cfg_params.tsc;
         burst_info.channel_type = tsm->cfg_params.chan_type;
         burst_info.decode_flag  = (tsm->state == SM_STATE_END_RX_BURST ? TRUE : FALSE);
         burst_info.burst_num    = burst_num;
         burst_info.search_width = tsm->rx_params.search_width;

         /* Set the mac mode in the frame we set the decode flag, this is
            an mDSP/scheduler requirement since both will latch the value at
            the next frame tick (when the first tx block starts with this
            mac mode).  We set the mode even if it did not change to ensure
            that the lower layers remain in sync. */
         if (burst_info.decode_flag && (gl1_msg_pdch_params_sm[gas_id].pending_mac_mode != gl1_msg_pdch_params_sm[gas_id].mac_mode || gl1_msg_pdch_params_sm[gas_id].set_mac_mode))
         {
            if (gl1_msg_pdch_params_sm[gas_id].pending_mac_mode != gl1_msg_pdch_params_sm[gas_id].mac_mode)
            {
               MSG_GERAN_MED_3_G("MAC mode changed %d->%d, FN=%d", gl1_msg_pdch_params_sm[gas_id].mac_mode, gl1_msg_pdch_params_sm[gas_id].pending_mac_mode,GSTMR_GET_FN_GERAN(gas_id));
               gl1_msg_pdch_params_sm[gas_id].mac_mode = gl1_msg_pdch_params_sm[gas_id].pending_mac_mode;
            }
            gl1_hw_set_medium_access_mode(gl1_msg_pdch_params_sm[gas_id].mac_mode,gas_id);
            gl1_msg_pdch_params_sm[gas_id].set_mac_mode = FALSE;
         }

#if  defined ( FEATURE_GSM_LOG_IQ_SAMPLES )
         if ( ( tsm->rx_params.num_msgs > IQ_SAMPLES_TN_INDEX ) && ( gl1_hw_gprs_iq_slot[gas_id] <= tsm->rx_params.num_msgs ) )
         {
            gl1_hw_set_iq_samples_tn
             (
               GL1_HW_PDTCHD,
               tsm->rx_params.signal_info[ tsm->rx_params.num_msgs + ( gl1_hw_gprs_iq_slot[gas_id] - tsm->rx_params.num_msgs ) - 1 ].tn,
               gas_id
             );
         }
#endif /* FEATURE_GSM_SAVE_IQ_SAMPLES | FEATURE_GSM_LOG_IQ_SAMPLES */
#ifdef DEBUG_GSM_SCHEDULER
         MSG_GERAN_MED_2_G("Schedule Rx burst %d, %d msgs", burst_num, tsm->rx_params.num_msgs);
#endif

#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
         is_block_start = (burst_info.burst_num == 0) ? TRUE : FALSE;
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/

         gl1_hw_rx_dtm_gprs
          (
            tsm->rx_params.signal_info,
            tsm->rx_params.num_msgs,
            &burst_info,
            tsm->tags[burst_num],
#ifdef FEATURE_GSM_RX_DIVERSITY_DATA
            is_block_start,
#endif /*FEATURE_GSM_RX_DIVERSITY_DATA*/
            gas_id
          );
     }

     /* Set the state depending on whether or not we are finished */
     tsm->state = (data_complete ? SM_STATE_DONE : tsm->state - 1);

     /* If the state is done then clean up this state machine */
     if  ( tsm->state == SM_STATE_DONE )
     {
        --gl1_msg_pdch_params_sm[gas_id].num_sm_active;

        /* Reset the number of msgs processed in this sm */
        tsm->rx_params.num_msgs = 0;

        /* If none of the state machines are active then remove the handler */
        if ( !gl1_msg_pdch_params_sm[gas_id].num_sm_active )
        {
           OPEN_RX_WINDOW(gas_id);
           ALLOW_RX_HDLR_TO_BE_REG(gas_id);
           remove_hdlr = TRUE;
        }
     }
   }

   return remove_hdlr;

}

/*===========================================================================
FUNCTION gl1_msg_pdch_store_ta

DESCRIPTION
  This function stores the ta.
  The timing advance changes infrequently about every 104 frames.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msg_pdch_store_ta ( uint8 ta,gas_id_t gas_id )
{
    gl1_msg_pdch_params_sm[gas_id].timing_advance = ta;
}


/*===========================================================================
FUNCTION gl1_msg_pdch_retrieve_ta

DESCRIPTION
  This function retieves the stored ta.

DEPENDENCIES
  none.

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
uint8 gl1_msg_pdch_retrieve_ta ( gas_id_t gas_id )
{
    return gl1_msg_pdch_params_sm[gas_id].timing_advance;
}

/*===========================================================================

FUNCTION gl1_msg_pdch_set_mac

DESCRIPTION
  This function sets the medium access mode to FIXED, DYNAMIC or EXT DYNAMIC.
  The mode change takes effect at the next decode.

===========================================================================*/
void gl1_msg_pdch_set_mac (gl1_defs_access_mode_type mode,gas_id_t gas_id)
{
    /* Store the mode to set at the next block start */
    gl1_msg_pdch_params_sm[gas_id].pending_mac_mode = mode;

    /* Force the update even if we think the mode has not changed in case
       some bug results in the lower layers getting out of sync */
    gl1_msg_pdch_params_sm[gas_id].set_mac_mode = TRUE;
}



/*===========================================================================

FUNCTION gl1_msg_pdch_get_mac

DESCRIPTION
  This function gets the medium access mode.

===========================================================================*/
gl1_defs_access_mode_type gl1_msg_pdch_get_mac (gas_id_t gas_id)
{
   return gl1_msg_pdch_params_sm[gas_id].mac_mode;
}



/*===========================================================================

FUNCTION gl1_msg_pdch_set/get_camp_tn

DESCRIPTION
  This function sets the timeslot that we are camped on.  This value is
  used to fill in the camp_tn field in the metrics and data callbacks.

===========================================================================*/
void gl1_msg_pdch_set_camp_tn (gl1_defs_tn_type tn,gas_id_t gas_id)
{
   gl1_msg_pdch_params_sm[gas_id].pdch_camp_tn = tn;
}

gl1_defs_tn_type gl1_msg_pdch_get_camp_tn (gas_id_t gas_id)
{
   return gl1_msg_pdch_params_sm[gas_id].pdch_camp_tn;
}



/*===========================================================================

FUNCTION gl1_msg_pdch_set_loopback

DESCRIPTION
  This function enables or disables EGPRS SRB loopback.

===========================================================================*/
void gl1_msg_pdch_set_loopback (boolean enable,gas_id_t gas_id)
{
#ifdef FEATURE_GSM_MDSP_EGPRS
   uint32 i, j;

   if (loopback_block[gas_id].srb_loopback == enable) return;
   loopback_block[gas_id].srb_loopback = enable;

   if (loopback_block[gas_id].srb_loopback)
   {
      MSG_GERAN_HIGH_1_G("SRB loopback enabled, FN=%d",GSTMR_GET_FN_GERAN(gas_id));

      /* Make sure we start with all data as invalid */
      for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD; i++)
      {
         for (j = 0; j < GL1_DEFS_MAX_DL_MSGS; j++)
         {
            loopback_block[gas_id].frame[i].burst[j].valid = FALSE;
            /* Preset last good modulation to GMSK in case first
             * Block received is below 1dB threshold
             */
            loopback_block[gas_id].frame[i].burst[j].last_good_mod = GL1_DEFS_MODULATION_GMSK;
         }
      }

      if ( loopback_8psk[gas_id].asrb_gmsk_ul_enable )
      {
         for (i = 0; i < GL1_DEFS_MAX_DL_MSGS; i++)
         {
            loopback_8psk[gas_id].next_loopback_wr_frame[i]  = 0;
            loopback_8psk[gas_id].next_loopback_rd_frame[i]  = 0;
            loopback_8psk[gas_id].next_loopback_wr_word[i]   = 0;
            loopback_8psk[gas_id].next_loopback_data_gmsk[i] = 0;
         }

         loopback_8psk[gas_id].first_time = TRUE;
         loopback_8psk[gas_id].sync_up    = FALSE;
      }
   }
   else
   {
      MSG_GERAN_HIGH_1_G("SRB loopback disabled, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
   }
#else
   MSG_GERAN_ERROR_0_G("MDSP does not support EGPRS");
#endif
}/*lint !e715 */ /*enable not referenced when FEATURE_GSM_MDSP_EGPRS is not defined */


/*===========================================================================

FUNCTION gl1_msg_pdch_get_loopback_data

DESCRIPTION
  Retrieves the loopback data for the given frame and timeslot.  Returns
  the length of the data, or 0 if not found.

===========================================================================*/
uint8 gl1_msg_pdch_get_loopback_data
(
   uint8 frame_index,
   gl1_defs_tn_type tn,
   uint8 **data,
   gas_id_t gas_id
)
{
#ifdef FEATURE_GSM_MDSP_EGPRS
   uint32 i;

   /* Check that we are actually in loopback mode */
   if (!loopback_block[gas_id].srb_loopback)
   {
      MSG_GERAN_ERROR_0_G("Loopback not enabled, no data for uplink");
      return 0;
   }

   /* Find the data based on the timeslot */
   for (i = 0; i < GL1_DEFS_MAX_ASSIGNED_DL_TS; i++)
   {
      if (tn == loopback_block[gas_id].frame[frame_index].burst[i].tn &&
          loopback_block[gas_id].frame[frame_index].burst[i].valid)
      {
         /* Mark this data as no longer valid and return it to the caller */
         loopback_block[gas_id].frame[frame_index].burst[i].valid = FALSE;
         *data = (uint8 *)loopback_block[gas_id].frame[frame_index].burst[i].hard_decisions;

         if ( IS_FTM_IN_TEST_MODE() )
         {
           if(loopback_block[gas_id].frame[frame_index].burst[i].modulation != loopback_block[gas_id].frame[frame_index].burst[i].last_good_mod)
           {
              /* overwrite modulation as recevied modulation is unreliable SNR below 5 dB threshold*/
              loopback_block[gas_id].frame[frame_index].burst[i].modulation = loopback_block[gas_id].frame[frame_index].burst[i].last_good_mod;
           }
         }
         if (loopback_block[gas_id].frame[frame_index].burst[i].modulation ==
               GL1_DEFS_MODULATION_GMSK)
         {
            return MDSP_HARD_DECISIONS_GMSK;
         }
         else
         {
            return MDSP_HARD_DECISIONS_8PSK;
         }
      }
   }
#else
   MSG_GERAN_ERROR_0_G("MDSP does not support EGPRS");
#endif

   return 0;
}/*lint !e715 */

/*===========================================================================

FUNCTION gl1_msg_pdch_store_hard_decisions

DESCRIPTION
  Called internally to store the hard decisions in the loopback buffer.

===========================================================================*/
static void gl1_msg_asrb_process_hard_decisions (srb_loopback_burst_type *burst,gas_id_t gas_id)
{
   int32 i, j, burst_num, rd_frame, wr_frame, wr_word, wr_bit_no, bits_8psk;
   uint16 rd_bit, data_8psk, data_gmsk, gmsk_bit59;

   /* Find the burst, if already started */
   for (burst_num = 0; burst_num < GL1_DEFS_MAX_ASSIGNED_DL_TS; burst_num++)
   {
      if (( loopback_8psk[gas_id].next_loopback_rd_frame[burst_num] > 0 ) &&
          ( burst->tn == loopback_8psk[gas_id].frame[0].burst[burst_num].tn ))
      {
        break;
      }
   }

   /* Check if no information exists for this slot yet */
   if (burst_num == GL1_DEFS_MAX_ASSIGNED_DL_TS)
   {
      /* Find an empty spot */
      for (burst_num = 0; burst_num < GL1_DEFS_MAX_ASSIGNED_DL_TS; burst_num++)
      {
         if (loopback_8psk[gas_id].next_loopback_rd_frame[burst_num] == 0)
         {
           MSG_GERAN_MED_1_G("ASRB loopback block start, num %d", burst_num);

           break;
         }
      }

      if (burst_num == GL1_DEFS_MAX_ASSIGNED_DL_TS)
      {
         MSG_GERAN_ERROR_1_G("Out of 8PSK storage space for tn %d", burst->tn);

         return;  /* No way to handle this gracefully */
      }
   }

   /* Get read and write frame numbers (0 - 12) */
   rd_frame = loopback_8psk[gas_id].next_loopback_rd_frame[burst_num];
   wr_frame = loopback_8psk[gas_id].next_loopback_wr_frame[burst_num];

   /* If we have not filled up the 8PSK buffer, grab the hard decisions */
   if (wr_frame < 12)
   {
      /* If the modulation was GSMK then F3 message */
      if (burst->modulation == GL1_DEFS_MODULATION_GMSK)
      {
         MSG_GERAN_HIGH_1_G("Warning: GMSK data Rx, for asym loopback tn %d", burst->tn);
      }

      /* Set offset into first GMSK word 0, 4, 8 or 12 */
      wr_bit_no = rd_frame * 4;
      wr_word = loopback_8psk[gas_id].next_loopback_wr_word[burst_num];
      data_gmsk = loopback_8psk[gas_id].next_loopback_data_gmsk[burst_num];

      /* Copy the new hard decisions into the 8PSK buffer */
      for (i = 0; i < MDSP_HARD_DECISIONS_8PSK; i++)
      {
         /* Check for GMSK frames complete, there are 16 8PSK bits left over - do not want them copied */
         if ( wr_frame < 12 )
         {
            data_8psk = burst->hard_decisions [i];

            /* If last word of 8-PSK data, then only use 12 bits */
            if ( i == (MDSP_HARD_DECISIONS_8PSK -1 ) )
            {
               bits_8psk = 6;
            }
            else
            {
               bits_8psk = BITS_PER_WORD/2;
            }

            for (j = 0; j < bits_8psk; j++)
            {
               rd_bit = (data_8psk & EXTRACT_2MSB);
               data_8psk = data_8psk << 2;
               rd_bit = rd_bit >> (BITS_PER_WORD - 2);
               data_gmsk = data_gmsk << 2;
               data_gmsk |= rd_bit;
               wr_bit_no += 2;

               if ( wr_bit_no == BITS_PER_WORD )
               {
                  loopback_8psk[gas_id].frame[wr_frame].burst[burst_num].hard_decisions[wr_word] = data_gmsk;
                  wr_word++;

                  /* GMSK burst is 7.25 words, so offset bit count by 12 for last word */
                  if ( wr_word == ( MDSP_HARD_DECISIONS_GMSK - 1 ))
                  {
                     wr_bit_no = 12;
                  }
                  else
                  {
                     /* GMSK burst complete so shift last data (4 bits) to MSB, increment write frame number */
                     if ( wr_word == MDSP_HARD_DECISIONS_GMSK )
                     {
                        wr_word = 0;
                        loopback_8psk[gas_id].frame[wr_frame].burst[burst_num].hard_decisions[MDSP_HARD_DECISIONS_GMSK - 1] = (data_gmsk << 12) & 0xF000;
                        loopback_8psk[gas_id].frame[wr_frame].burst[burst_num].tn = burst->tn;
                        wr_frame++;
                     }

                     wr_bit_no = 0;
                  }

                  data_gmsk = 0;
               }

               /* Set gmsk stealing flags (bits 57,58) to 0, so adjust when next bit to write is 58 */
               if ( (wr_word == 3) && (wr_bit_no == 10) )
               {
                  gmsk_bit59 = data_gmsk & 0x1;
                  data_gmsk = data_gmsk << 2;
                  data_gmsk &= 0xFFF8;
                  data_gmsk |= gmsk_bit59;
                  wr_bit_no +=2 ;
               }

               /* Do not loopback bits 174,175 from 8-PSK data */
               if ( (i == 10) && (j == 6) )
               {
                  j++;
               }
            }
         }
      }

      loopback_8psk[gas_id].next_loopback_wr_frame[burst_num]  = wr_frame;
      loopback_8psk[gas_id].next_loopback_wr_word[burst_num]   = wr_word;
      loopback_8psk[gas_id].next_loopback_data_gmsk[burst_num] = data_gmsk;
   }

   /* Copy relevant data from 8PSK buffer to the main loopback structure, force Tx modulation type to GMSK */
   burst->modulation = GL1_DEFS_MODULATION_GMSK;
   burst->tn         = loopback_8psk[gas_id].frame[rd_frame].burst[burst_num].tn;

   memscpy( burst->hard_decisions,
   	       sizeof(burst->hard_decisions),
           loopback_8psk[gas_id].frame[rd_frame].burst[burst_num].hard_decisions,
           ( MDSP_HARD_DECISIONS_GMSK * 2 ) );

   loopback_8psk[gas_id].next_loopback_rd_frame[burst_num] = rd_frame + 1;

   /* If we have looped back all the 8PSK data then we are ready for more */
   if (loopback_8psk[gas_id].next_loopback_rd_frame[burst_num] == 12)
   {
      loopback_8psk[gas_id].next_loopback_rd_frame[burst_num]  = 0;
      loopback_8psk[gas_id].next_loopback_wr_frame[burst_num]  = 0;
      loopback_8psk[gas_id].next_loopback_wr_word[burst_num]   = 0;
      loopback_8psk[gas_id].next_loopback_data_gmsk[burst_num] = 0;
   }
}


/*===========================================================================

FUNCTION dl_pdch_process_egprs_data

DESCRIPTION
  Does final processing of EGPRS data before shipping it up to PL1.

===========================================================================*/
static void dl_pdch_process_egprs_data
(
   gl1_defs_rx_pkt_data_type *data_info,
   gl1_msg_pdch_sm_type *tsm,
   uint8 ind,
   gas_id_t gas_id
)
{
   uint8 i;
   uint8 mcs = (uint8)GL1_DEFS_MCS1_CODING;
   boolean snr_valid = FALSE;
   uint16 snr_threshold;
   uint16 *hdr;

   /* The EGPRS header from the mDSP does not include the USF
      but the spec does (as the first 3 bits), so we add it here */
   hdr = data_info->egprs.hdr;
   hdr[2] =  ((hdr[2] << 3) & 0xFFF8) | ((hdr[1] >> 13) & 0x0007);
   hdr[1] =  ((hdr[1] << 3) & 0xFFF8) | ((hdr[0] >> 13) & 0x0007);
   hdr[0] =  ((hdr[0] << 3) & 0xFFF8);

   if ( ind < GL1_DEFS_MAX_DL_MSGS )
   {
     hdr[0] |= ((uint16)tsm->usfs[ind].usf & 0x0007);
   }

   /* Set the BEP with the calculated values and check the SNR */
   if ( data_info->coding_scheme >= GL1_DEFS_MCS1_CODING )
   {
     int snr_av =0;

     mcs = (uint8)(data_info->coding_scheme - GL1_DEFS_MCS1_CODING);  /*lint !e656*/

     if(gl1_hw_feature_disabled_aeq(gas_id) )
     {
       snr_threshold = EGPRS_HDR_SNR_THRESHOLD[mcs];
     }
     else
     {
       snr_threshold = EGPRS_HDR_SNR_THRESHOLD_AEQ[mcs];
     }

     if ( ind < GL1_DEFS_MAX_DL_MSGS )
     {

       for (i = 0; i < GL1_DEFS_FRAMES_IN_BLK_PERIOD ; i++)
       {
         data_info->egprs.bep[i]            = tsm->bep[i][ind];
         data_info->egprs.schedule_error[i] = tsm->schedule_error[i][ind];
         if (tsm->snr[i][ind] < SNR_AVG_THRESHOLD)
         {
           snr_av = snr_av +  tsm->snr[i][ind] ;
         }
         else
         {
           snr_valid = TRUE;
         }
       }
     snr_av = (snr_av >> 2) ;
     if (snr_av > snr_threshold)
        {
        snr_valid = TRUE;
        }

     }
   }

   /* If the SNR is really bad we force header CRC to fail */
   if (!snr_valid && data_info->egprs.hdr_crc_passed)
   {
      MSG_4(MSG_SSID_DFLT, MSG_LEGACY_MED,
            "Discarding EGPRS msg, SNR too low (MCS%d, tn=%d (%d), FN=%d)",
            mcs+1, data_info->tn, data_info->camp_tn, GSTMR_GET_FN_GERAN(gas_id));  /*lint !e778*/
      data_info->egprs.hdr_crc_passed = FALSE;
      data_info->msg[0].crc_passed = FALSE;
      data_info->msg[1].crc_passed = FALSE;
   }
}




/*===========================================================================

FUNCTION gl1_msg_pdch_abort_dl_pbcch_pccch_pacch_pdtch

DESCRIPTION
  This function immediately aborts the specified DL channel.
  If this function is invoked in the task context make sure that this function
  has interrupts locked.

===========================================================================*/
void gl1_msg_pdch_abort_dl_pbcch_pccch_pacch_pdtch
(
   gl1_hw_channel_type   chan_type,gas_id_t gas_id
)
{
   uint32 sm_num, i;

   /*-------------------------------------------------------------------------
     Iterate to through the 3 State machines and abort based on the specified
      channel type.
   --------------------------------------------------------------------------*/
   for (sm_num = 0; sm_num < NUM_SM; sm_num++)
   {
       if (gl1_msg_pdch_params_sm[gas_id].sm[sm_num].cfg_params.chan_type != chan_type) continue;

       /* If the state machine is marked to be activated or has already
          started then we abort it */
       if (gl1_msg_pdch_params_sm[gas_id].sm[sm_num].activate_sm || gl1_msg_pdch_params_sm[gas_id].sm[sm_num].state > 0)
       {
           /*-------------------------------------------------------------------
           At this point atleast one state machine should be active. However,
           confirm this and then if this is the last state machine remove the
           handler as well.
           --------------------------------------------------------------------*/
           if (gl1_msg_pdch_params_sm[gas_id].num_sm_active > 0)
           {
             --gl1_msg_pdch_params_sm[gas_id].num_sm_active;
             if (gl1_msg_pdch_params_sm[gas_id].num_sm_active == 0)
             {
                 /* This is the last active state machine, remove handler */
                 OPEN_RX_WINDOW(gas_id);
                 ALLOW_RX_HDLR_TO_BE_REG(gas_id);
                 gl1_msgi_remove_ft_handler( gl1_msg_pbcch_pccch_pacch_pdtch_handler,gas_id );
             }
           }
           else
           {
               MSG_GERAN_ERROR_1_G("No SM active, aborting DL chan=%d failed",
                         gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].cfg_params.chan_type);
           }

           /* Clean up frame layer if we started decoding */
           if (gl1_msg_pdch_params_sm[gas_id].sm[sm_num].state < SM_STATE_END_RX_BURST)
           {
              for (i = 0; i < gl1_msg_pdch_params_sm[gas_id].sm[sm_num].rx_params.num_msgs; i++)
              {
                 MSG_4(MSG_SSID_DFLT, MSG_LEGACY_HIGH,
                       "Aborting PDCH: chan=%d, tn=%d, state=%d, FN=%d",
                       chan_type, gl1_msg_pdch_params_sm[gas_id].sm[sm_num].rx_params.signal_info[i].tn,
                       gl1_msg_pdch_params_sm[gas_id].sm[sm_num].state,GSTMR_GET_FN_GERAN(gas_id));
                 gl1_hw_abort_rx_decode(&gl1_msg_pdch_params_sm[gas_id].sm[sm_num].tags[DECODE_TAG][i],gas_id);
              }
           }

           /* If we are aborting PDTCH, need to clean up AGC */
           if (chan_type == GL1_HW_PDTCHD)
           {
              sys_algo_gprs_agc_reset(gas_id);
           }

           /* Reset the SMs internal variables */
           gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].state              = 0;
           gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].rx_params.num_msgs = 0;
           gl1_msg_pdch_params_sm[gas_id].sm[ sm_num ].activate_sm        = FALSE;
       }

    }

}

/*===========================================================================

FUNCTION gl1_srb_set_ul_mod_mode

DESCRIPTION
  Sets the mode of the Uplink Modulation for ASRB/SRB loopback. This is
  described in 44014, sec.5.5.4. A value of 0 indicates that the UL
  modulation should be the same as that detected on the DL. A value of
  1 indicates that the UL modulation should be GMSK within the context
  of 8psk on the DL.
===========================================================================*/
void gl1_srb_set_ul_mod_mode(boolean srb_m_bit,gas_id_t gas_id)
{
   loopback_8psk[gas_id].asrb_gmsk_ul_enable = srb_m_bit;

   MSG_GERAN_HIGH_1_G( "SRB M bit value = %d",srb_m_bit);
}

/*===========================================================================

FUNCTION gl1_srb_get_loopback_state

DESCRIPTION
 Returns state of srb loopback mode. TRUE = enabled, FALSE = disabled
===========================================================================*/

boolean gl1_srb_get_loopback_state(gas_id_t gas_id)
{
   return(loopback_block[gas_id].srb_loopback);
}
/*===========================================================================

FUNCTION gl1_msgi_pdch_open_rx_win

DESCRIPTION
   This function open rx window for PDCH.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void gl1_msgi_pdch_open_rx_win( gas_id_t gas_id )
{
    OPEN_RX_WINDOW(gas_id);
}
