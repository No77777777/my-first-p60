/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 M D S P   D T M   I N T E R F A C E
                    mDSP DTM Interface Module

GENERAL DESCRIPTION

   Implementation of the GSM mDSP DTM module.  Support for receive and transmit
   operation.


EXTERNALIZED FUNCTIONS

   mdsp_dtm_rx
   mdsp_dtm_get_rx_data
   msdp_dtm_get_rx_burst_metrics
   mdsp_gprs_get_rx_burst_schedule
   mdsp_gprs_get_rx_usfs
   mdsp_dtm_write_tx_data
   mdsp_dtm_tx
   mdsp_dtm_get_tx_schedule
   mdsp_gprs_convert_usf_to_pdch
   mdsp_dtm_abort_tx
   mdsp_dtm_abort_rx
   mdsp_dtm_process_frame_tick
   mdsp_dtm_init


INITIALIZATION AND SEQUENCING REQUIREMENTS


  Copyright (c) 2005-2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*===*/

/*=========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gmdsp6/src/mdsp_dtm.c#2 $
$DateTime: 2019/11/27 05:34:59 $ $Author: pwbldsvc $

when         who     what, where, why
----------   ------- ------------------------------------------------------
18//11/19   nv      CR2567070 FR51554: Recovery mechanism for RF scripting delays in X2Y IRAT scenarios for Habanero
09/02/17      bg     CR2004571 In FTM, do not set ACI detection always to 0 and also do not clear band supported information 
11/08/17      rc     CR2138578  ML logging - GL1 Changes
03/06/16      sk     CR1011063 Do not update average power if all the CCCH bursts have been yielded
08/02/15      mn     CR872002 GL1 changes for VSWR feature
28/09/15      pg     CR909305 Update dynamic RxD switching and remove pwr_meas suspension  
18/09/15      cws    CR907895 Change unnecessary F3 to LOW 
07/09/15      npt    CR898493 GL1 NV control of T2G acquisition GFW error recovery
20/08/15      npt    CR878988 GL1 NV control of GFW recovery features
14/08/14      ws     CR889273 Fix Static declarations of variables which are not Dual data spaced
14/02/15      smd    CR794015 Added support for RF self test
08/07/15      cja    CR868167 For dynamic EGPRS AEq reduce clock bump after block is complete
29/05/15      pa     CR826683: FR27452 GL1 Antenna Sharing Between WLAN and WAN RAT's. Dynamic Voice & Data RxD. 
22/04/15      ab     CR822921 : Provide Rx ARFCN in SYNC RX / ASYNC RX for PCHXFER Scheduling in GFW and RXLEV info for IQ Scaling in ALL ASYNC RX. 
07/04/15      og     CR814507. Further stack reduction in the GSM L1 ISR.
02/04/15      pg     CR810444 Adjust AEQ/RxD dynamic switching and add support for DRx IQ scaling 
30/03/15      mc     CR773788: COEX Enhancements : FACCH delay, RACH delay, PS monitor reduction and DTX timeline changes
16/03/15      pg     CR786609 Mainline FEATURE_GERAN_RFLM_FED and FEATURE_GSM_MULTI_THREAD_WAKEUP
27/02/15      cja    CR768169 Add RF Seq Number
27/02/15      am     CR646584 WLAN coex
15/12/14      pg     CR630281 implementation of RxD support for data/MSRD
11/12/14      sp     CR765250 Correct the propagtion error
03/12/14      npt    CR759467 Fix compiler warnings
07/11/14      pjr    CR752564 Calculate correct buffer index when retrieving AMR-PD metrics
03/12/14      pg     CR750607 Avoid IQ scaling in FTM for Rx Cal
17/11/14      npt    CR642426 Added power reporting log packet
30/10/14      ss     CR752151 enabled enableAeq2a only if enableAeq2 is enabled
30/10/14      cs     CR701894 Enhanced Page Detection Support
29/10/14      ab     CR745429 - Featurization correction for PCHXFER for SS / CR merges.
19/08/14      pjr    CR711506 Support for AMR Partial Decode and DTX detection FR19667
07/10/14      pg     CR730132 F3 cleanup
01/10/14      df     CR611845 SBD interface updates for missed wakeup
06/06/14      jk     CR668058:Adding EPD Log pakcet
08/10/14      cs     CR709170 Always clear enableEPD when GCF is enabled
17/09/14      df     CR725802 Update burst_num during error recovery
09/09/14      jk     CR697308:GL1 changes to support HLin LLin sawless feature
12/09/14      nm     CR723570 GL1 changes for FR21036 - Read own PCH in transfer to BO2.0(to be leveraged for TH/JO)
29/08/14      cos    CR 716812 - Adding missing changes for imrd framework
07/08/14      cja    CR643648 Desense for channel adjacent to spur
02/05/14      jj     CR 658008 turn off aci detection for ftm mode
11/02/14      jk      CR614254:GL1 changes to support IP2 calibration
11/07/14      pjr    CR 692177 Added support for RFACCH 2.0
06/08/14      df     CR536059 Full asynchronous sleep support in idle
10/07/14      jk     CR688587:GL1 changes to support LIF feature
18/06/14      cos    CR682031 - GL1 control algorithm and changes for supporting AEQ PHASE 4
02/07/14      mc     CR.687340 : BOLT 2.0 : COEX : Mass Merge of COEX changes from DI4.0
10/06/14      jj     CR 668965 merge ASDIV from DI.3.0 to BO 2.0
30/05/14      pg     CR655759 Feature cleanup of FEATURE_BOLT_MODEM and FEATURE_DIME_MODEM
28/05/14      cja    CR657368 Fix Klockwork issue of NULL data space pointer
17/03/14      jk     CR631867:Enable aeqPhase2a only during EGPRS mode
16/04/14      jk    CR650105:Only if NV67228 is set to VAMOS2 mode use VAMOS2 BFI handling
20/06/14      pjr    DATA+MMS feature - USF utilisation calculation
16/04/14      jk     CR650105:Only if NV67228 is set to VAMOS2 mode use VAMOS2 BFI handling
05/03/14      jj     CR 610751  dime 3.0 dsda Asdiv
13/03/14      jk     CR631073:Enable AEQ Phase 2 flag only for EDGE and if RxD is not enabled
03/02/14      pg     CR610253: Crash on disabling SRB loopback due to CR607716
03/02/14      cos    CR475069 GL1 to indicate rxlev to firmware; merged on Bolt
29/01/14      cs     CR605061:Set decode_flag based on feature Macro(FEATURE_DISABLE_EARLY_DECODE)
27/01/14      pa     CR607716:GL1 changes related to legacy GFW interface changes.
19/12/13      jk     CR503359 - GL1 changes to support Rx diversity
07/10/13      pjr    CR554814 added support for change in fw interface (CR551941): gsmTxCmd.ratmode
22/11/13      cja    CR455527 Remove featurisation from files in API directory
24/10/13      cos    CR564287 - TX frequency correction on Bolt
07/10/13      pjr    CR554814 added support for change in fw interface: gsmTxCmd.ratmode
24/09/13      sp     CR482765 - Update GFW-GL1 interface with EDGE Enhanced Info.
17/09/13      pjr    CR538893 Changes to enable CXM logging
13/09/13      jk     CR413653:Set flag spurChanFlag for desense channel
15/08/13      sk     CR524039 GBTA bringup changes
7/16/13       zc     CR470197 Replace memcpy with memscpy
29/07/13      sk     CR519663 Partial QBTA bringup changes
21/06/13      cs     Major Triton TSTS Syncup
09/05/13      cs     TSTS Updates
17/05/13      kb/npt CR385548 - Decode PCH after sleep wakeup error recovery.
25/04/13      pjr    CR480105 dual dataspaced missing variables
25/03/13      mc     CR464496 : COEX Support - GERAN arbitrator SW CXM integration
13/03/13      mc     CR459487 : COEX Support - GERAN arbitrator framework
19/02/13      mc     CR453970 Hard coding of FW interface COEX parameters for Rx, Tx and Monitors
18/01/13      ws     Removed mdsp_tx_buffer_free declaration has been moved to dula data space
05/12/12      pg     CR427361: GFW/RF shared memory interface cleanup
24/07/12      ky     Changes for new TX CCS interface changes
20/06/12      ky     Added support of using new CCS interface and passing new CCS
                     buffer pointers in RF task messages
20/06/12      cja    CR371336 If Rx data header only do not flag error.
25/05/12      pg     Add support for RF Task
26/03/12      jj     CR344695 rename feature name FEATURE_GSM_GPRS_MSC34 to FEATURE_GSM_GPRS_MSC33
09/03/12      dv     T2G:CR342537 Triple buffer all monitor buffers
13/03/12      ws     CR341080 Always set logIQ for all timeslots and channel types
                     when FEATURE_GSM_LOG_IQ_SAMPLES is enabled
28/02/12      ky     Vamos DTM set 2 fix
27/02/12      cs     CR295865 Enable FEATURE_GSM_MDSP_ACI_DETECTION_VOICE and
                     add SACCH to the channel list
27/01/12      jj     CR329722 add indication for  scheduleTimeError
13/01/12      ws     Added Vamos Feature
07/11/11      jj     CR315253 IQ capture capability for GSM via QRCT
22/09/11      jj     CR308254 GL1 Changes for GSM DC spur removal
19/09/11      jj     CR307435 I/Q logging bug fix
08/09/11      jj     CR302378  Added GL1 changes for RSB along with spectral inversion
19/07/11      pg     CR295424: Ensure dynamic AEQ PH2 does not override AEQ PH2 NV setting
25/05/11      cja    Ensure rotator is 0 for Sync Rx
28/04/11      cja    Add Nikel freq correction by GFW
11/04/11      pg     CR282109: Ensure AEQ PH2 switching only occurs at a block boundary
20/04/11      cja    Nikel GFW interface changes
08/03/11      cja    Initial IRAT for Nikel
27/01/11      cja    Changes for RxLM and TxLM for NikeL
24/01/11      dv     271432:For Fusion fix bug on MDSP interface to enable WB_AMR.
11/02/11      ky     265892: For Q6, corrected the number of tx bursts for RATSCCH,SID UPDATE
                     and SID FIRST messages
12/01/11      ky     Lint & compiler warnings fixes
14/10/10      ws     FEATURE_GSM_MDSP_AEQ_8PSK - support AEQ Ph2
01/10/10      ky     258042:During SRBL disconnect, when sw stops sending the tx command we need to
                     clear the num transactions in sbi and grfc buffers
09/08/10      ky     235724: TC 26.7.5.2 RFACCH Mdsp halt during the third MO call
29/07/10      ky     Synchronous copying of IQ samples from FW is introduced in QDSP6
13/07/10      ky     Fixed 9k warnings
18/06/10      pg     CR240452(cont) - expand dataBuffer ptr check for multislot SRBL
14/06/10      pg     CR240452 - Sanity check dataBuffer ptr before sending Tx cmd in SRBL
04/06/09      ws     CR 241282 - Fix enabling AEQ on PS slots in DTM mode
20/04/10      pg     Removed unintended Qshrink message conversions
15/04/10      pg     Check tag is not INVALID before indexing into rx burst metrics buffer
12/04/10      ky     Replacing mdsp_memcpy with memcpy_v function
17/03/10      ws     Added featurisation to seperate Data mover code from FEATURE_GSM_MDSP_IR
29/01/10      ky     control_field in mdsp_dtm_tx_ctl_buffer structure need to be reset before populating the values.
12/01/10      pg     Removed doubling of data_len before Rx data copy
23/12/09      ws     Added volatile type for accessing pMdspHostIfBaseStore
05/11/09      ky     changed memcpy to mdsp_memcpy for QDSP6
30/09/09      ws     Renamed mdsp_issue_command2 to mdsp_issue_qdsp6_command
21/08/09      ws     Added FEATURE_SEC around secutil_fill_rnd_data()
05/08/09      ws     Removed FEATURE_GSM_MDSP_DTM it's now mainlined
26/05/09      cs     CR167848 fix for expanded afc_freq I/F to mDSP
29/05/09      ws     FEATURE_GSM_MDSP_AEQ phase 1 for all GMSK bursts
20/04/09      ws     CR 178831 - Enable ACI detection on PTCCH channels
13/02/09      ap     Lint fixes for high Warning
24/11/08      og     FEATURE_GSM_GPRS_MSC33 support.
30/10/08      ws     CR 162112 - ESAIC Only reset IIR filter on PCH and NCELL BCCH
21/10/08      nt     Fix high Lint warnings
08/08/08      ws     Reinstated ESAIC on GPRS chanels since scheduling issue
                     in Firmware is resolved
12/05/08      dv     Added FEATURE_GSM_AMR_WB - L1 Component
27/03/08      nf     Added NV Item handling to R-SACCH and R-FACCH
19/03/08     agv     LCU RF Spur Interface Support.
05/03/08      ws     Added ESAIC functionality to Async Rx interfaces
16/01/08      ws     Added ESAIC EsaicDisableIir for PCH and BCCH channels using sync_rx
15/01/08      cs     Fix compile warning
24/12/07      nt     Added the GCF flag to the mDSP interface for async/sync_rx cmds
19/12/07      cs     Fix the previous checkin for builds that don't have DM fix
13/12/07      agv    CR 131982 Fix for EGPRS DM causing sleep nacks.
09/11/07      nf     Added FEATURE_GSM_R_SACCH
22/10/07      ws     Add support for ESAIC NV 2508 item control
27/03/07      dv     Added FEATURE_GSM_R_FACCH
05/01/07      agv    Switch SAIC off if Idle Mode to reduce current draw.
28/11/06      ws     High Lint warning fixes
02/11/06      agv    CR98176 - Added csprng seed for security util.
21/09/06      og     Reset only the cs part of lower layers when connecting DTM
                     assignment in packet transfer (EDTM route).
31/07/06      agv    CR85810 NB AB TSC, and TA fix.
07/06/06      og     Fixed MSG_ERROR order of arguments printout.
03/27/06      ws     Added missing initialisation to saic_control[2]
03/23/06      ws     Added Support for SDA algorithm SAIC Disable Algorithm
01/24/06      gfr    Disable SAIC for PTCCHD
12/14/05      gfr    Auto-clear tx data buffers one frame earlier to support
                     2 uplink slots in SRB loopback mode without having to add
                     another tx data buffer.
10/17/05      gfr    Skip saic if the mdsp_saic flag is not set
10/11/05      gfr    Added mdsp_dm_pdtch_decode_done to sanity check the counter
09/30/05      gfr    Remove abort tx message since NPL1 now aborts discarded
                     USF specific messages, causing a flood
09/30/05      gfr    Move ACI detection control to mDSP driver, support for
                     ACI detection on GSM voice channels
09/23/05      gfr    Optimize DM active period to just during PDTCH decoding
09/12/05      gfr    SAIC support
08/25/05      gfr    Add timeslot to stale rx debug message
08/17/05      gfr    QCLint cleanup - shorten lines to less than 90 chars
08/10/05      gfr    AMR half-rate uplink RATSCCH/SID_FIRST/SID_UPDATE
                     messages are 4 bursts.
08/10/05      gfr    Only set early decoding bypass bit if decode is set
08/05/05      gfr    Add support for SAIC control words and ACI detect flag.
05/10/05      gfr    Initial version.

==========================================================================*/


/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/
#include "geran_variation.h"
#include "customer.h"
#include "comdef.h"
#include "msg.h"
#include "mdsp_intf.h"
#include "mdsp_intf_g.h"
#include "mdsp_cmd.h"
#include "gtmrs.h"
#include "gtmrs_g.h"
#ifdef FEATURE_GSM_MDSP_RAND_GEN
#include "secutil.h"
#endif

#include "gl1_hw.h"
#include "gl1_hw_g.h"
#include "gl1_arbitrator_utils.h"
#include "gl1_defs.h"

#ifdef FEATURE_GSM_COEX_FW_CXM
#include "gl1_arbitrator_utils.h"
#endif

#include "l1_utils.h"

#include "geran_dual_sim.h"
#include "geran_multi_sim.h"
#include "stringl.h"
#include "geran_dual_sim_g.h"

/*===========================================================================

                     DEFINITIONS FOR MODULE

===========================================================================*/

/* Define this feature to track buffer allocations and free ones that are
   erroneously abandoned by the upper layer */
#define DEBUG_STALE_BUFFERS
#ifdef DEBUG_STALE_BUFFERS
/* SACCH tx occurs every 26 frames, so need to allow that much space */
#define MAX_TX_IN_USE_COUNTER  30
/* All decoded data should be picked up within 8 frames */
#define MAX_RX_IN_USE_COUNTER   8
#endif

/* Define this to print out a lot of debug messages */
/* #define DEBUG_GSM_MDSP_DTM_RX */
/* #define DEBUG_GSM_MDSP_DTM_TX */

#ifdef  FEATURE_GSM_COEX_FW_CXM
#ifdef FEATURE_GSM_QSC_TX_DIV
extern garb_arbitrated_gas_t arbitrated_gas[NUM_GERAN_DATA_SPACES];
#endif
#endif

/* Structure storing information about messages being decoded */
typedef struct
{
   boolean            free_flag;
   uint16             seq_num;
   mdsp_timeslot_type timeslot;
   mdsp_channel_type  channel_type;
#ifdef DEBUG_STALE_BUFFERS
   uint16             in_use_counter;
#endif
} mdsp_dtm_rx_data_buffer_type;

/* State of data written for transmit */
typedef enum
{
   TX_DATA_STATE_CLEARED,
   TX_DATA_STATE_DONE,
   TX_DATA_STATE_ENCODING,
   TX_DATA_STATE_SCHEDULED,
   TX_DATA_STATE_READY
} mdsp_dtm_data_state_type;


/* Structure storing information about written tx data */
typedef struct
{
   boolean                      free_flag;
   mdsp_channel_type            channel_type;
   mdsp_specific_msg_type       msg_type;
   GfwGsmTxMsgCtrlWord          control_field;
   mdsp_dtm_data_state_type     data_state;
   uint16                       data_buffer;
   uint16                       bursts_remaining;
   boolean                      auto_clear;
#ifdef DEBUG_STALE_BUFFERS
   uint16                       in_use_counter;
#endif
} mdsp_dtm_tx_ctl_buffer_type;

/* Need to support 8 PDTCH (4 ping-pong style) + 1 PTCCH/SACCH */
#define MDSP_DTM_NUM_TX_CTL_BUFFERS 9

/* ----- Description of TX process -----
   1. Data is written to the driver by calling mdsp_dtm_write_tx_data.
      This allocates new data and control buffers and sets the data state
      READY, indicating that the data is ready for encoding.  A tag is
      returned to the caller.
   2. When mdsp_dtm_tx is called it is passed a set of tags.  For each tag
      the data state is checked.  If the state is READY than the data is
      marked for encoding, the state is changed to SCHEDULED, and the
      bursts_remaining field is set the number of bursts required to transmit
      the encoded data.
      The bursts_remaining field is decremented for all scheduled transmits.
   3. In mdsp_dtm_process_frame_tick the data state is decremented to
      ENCODING and then to DONE.
   4. The upper layer calls mdsp_dtm_get_tx_schedule which checks if the
      message was transmitted (in dynamic allocation it may not have been).
      If the message was transmitted or the discard_untxed flag is set the
      data buffer is cleared and the data state is set to CLEARED.  Otherwise
      the state goes back to READY and the data may be re-encoded.
      If the auto_clear flag was set for the data then
      mdsp_dtm_get_tx_schedule will be called in mdsp_dtm_process_frame_tick
      when the data state is DONE.  The discard_untxed flag will be set in
      this case.
   5. In mdsp_dtm_process_frame_tick when the data state is detected as CLEARED
      and bursts_remaining is 0 the control buffer is freed and the transmit
      is complete.
*/


/* Lookup table for coding scheme to data length */
extern const uint8 mdsp_cs_to_length_lut[];

/* Sequence number for debug data */
extern uint16   mdsp_sync_rx_burst_seq_num_fifo[3];

/* Flag indicating if ACI detection is enabled */
extern boolean mdsp_aci_detection;

#ifdef FEATURE_GSM_MDSP_EGPRS
/* Flag to store EGPRS mode */
extern boolean mdsp_egprs_mode;
#endif

/* USF list table */
extern uint32 mdsp_usf_list[];

/*===========================================================================

                             LOCAL MACROS

===========================================================================*/
/* Define the offsets into the rx structure */
#define MDSP_DTM_RX_DATA1_OFFSET MDSP_RX_HDR_SIZE
#ifdef FEATURE_GSM_MDSP_EGPRS
#define MDSP_DTM_RX_EGPRS_OFFSET \
  (MDSP_DTM_RX_DATA1_OFFSET + MAX_RX_DTM_DATA_PER_FRAME)
#define MDSP_DTM_RX_DATA2_OFFSET \
  (MDSP_DTM_RX_EGPRS_OFFSET + MDSP_RX_EGPRS_HDR_SIZE)
#endif

/* Define some offsets into the Tx data buffer */
#define MDSP_DTM_TX_GPRS_DATA_OFFSET   (2)
#define MDSP_DTM_TX_GSM_DATA_OFFSET    (2)
#define MDSP_DTM_TX_AMR_DATA_OFFSET    (1)
#ifdef FEATURE_GSM_MDSP_EGPRS
#define MDSP_DTM_TX_EGPRS_HDR_OFFSET   (2)
#define MDSP_DTM_TX_EGPRS_DATA1_OFFSET (MDSP_DTM_TX_EGPRS_HDR_OFFSET + 3)
#define MDSP_DTM_TX_EGPRS_DATA2_OFFSET \
  (MDSP_DTM_TX_EGPRS_DATA1_OFFSET + MAX_TX_GPRS_DATA_PER_FRAME)
#endif


/* Macro to determine if a given channel type is AMR */
#ifdef FEATURE_GSM_AMR_WB

#define IS_MDSP_AMR_CHANNEL(chan)  \
   (chan == MDSP_TCH_AFS   || chan == MDSP_TCH_AHS ||  \
    chan == MDSP_FACCH_AFS || chan == MDSP_FACCH_AHS || \
    chan == MDSP_TCH_WFS   || chan == MDSP_FACCH_WFS)

#ifdef FEATURE_GSM_MDSP_ACI_DETECTION_VOICE
/* Macro to determine if a given channel type is downlink voice or SACCH */
#define IS_MDSP_DL_VOICE_CHANNEL(chan)  \
   (chan == MDSP_TCH_FS  || chan == MDSP_TCH_EFS || \
    chan == MDSP_TCH_HS  || chan == MDSP_TCH_AFS || \
     chan == MDSP_TCH_AHS || chan == MDSP_TCH_WFS || \
     chan == MDSP_SACCH )
#endif

#else /* FEATURE_GSM_AMR_WB */

#define IS_MDSP_AMR_CHANNEL(chan)  \
   (chan == MDSP_TCH_AFS   || chan == MDSP_TCH_AHS ||  \
    chan == MDSP_FACCH_AFS || chan == MDSP_FACCH_AHS )

#ifdef FEATURE_GSM_MDSP_ACI_DETECTION_VOICE
/* Macro to determine if a given channel type is downlink voice or SACCH */
#define IS_MDSP_DL_VOICE_CHANNEL(chan)  \
   (chan == MDSP_TCH_FS  || chan == MDSP_TCH_EFS || \
     chan == MDSP_TCH_HS  || chan == MDSP_TCH_AFS || \
     chan == MDSP_SACCH )
#endif

#endif /* FEATURE_GSM_AMR_WB */

/* SAIC default values */
#define MDSP_SAIC_SHORT            0
#define MDSP_SAIC_LONG_MIMO_FILT   1
#define MDSP_SAIC_TWO_INPUT_MLSE   0
#define MDSP_SAIC_TS_LENGTH      146
#define MDSP_SAIC_TS_OFFSET      149
#define MDSP_SAIC_RF_GAIN_RANGE    1

#ifdef FEATURE_GSM_MDSP_RAND_GEN
#define MDSP_SWITCH_RANDOM_ON         1
#define MDSP_SWITCH_RANDOM_OFF        0
#define MDSP_RAN_GEN_READY            0x8000
#define MDSP_CSPRNG_CLEAR_DATA_READY  0x7FFF
#define MDSP_RAN_DATA_BYTE_LEN        8
#define MDSP_FOUR_SEC_TIMER           867
#define MAX_MDSP_GSM_FRAME_NUMBER     2715647
#endif

/*===========================================================================
  MULTI-DATASPACED PRIVATE DATA
===========================================================================*/
typedef struct
{
  mdsp_dtm_tx_ctl_buffer_type mdsp_dtm_tx_ctl_buffer[MDSP_DTM_NUM_TX_CTL_BUFFERS];
  uint32                      mdsp_gsm_txlm_buf_idx;
  uint32                      fn_save;
  boolean                     first_call;
  boolean                     header_only_received;
  boolean                     mdsp_tx_buffer_free[MDSP_DTM_NUM_TX_DATA_BUFFERS];
  mdsp_dtm_rx_data_buffer_type mdsp_dtm_rx_data_buffer[MDSP_DTM_NUM_RX_DATA_BUFFERS];
  uint16                       mdsp_dtm_rx_usfs_seq_num;
  uint16                       mdsp_dtm_rx_burst_seq_num[3];
} mdsp_dtm_private_data_t;

/*===========================================================================
  PUBLIC AND PRIVATE DATASPACES
===========================================================================*/
static mdsp_dtm_private_data_t mdsp_dtm_private_data[NUM_GERAN_DATA_SPACES];

/*===========================================================================

                           FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

                                FUNCTIONS

===========================================================================*/

/*===========================================================================

FUNCTION get_private_dataspace_ptr

DESCRIPTION
  This function gets the private data space for a specified GAS ID

===========================================================================*/
static mdsp_dtm_private_data_t* get_private_dataspace_ptr(const gas_id_t gas_id )
{
  mdsp_dtm_private_data_t* data_ptr;

  data_ptr =
      &(mdsp_dtm_private_data[GERAN_MAP_GAS_ID_TO_DATA_SPACE_INDEX(check_gas_id(gas_id))]);

  return ( data_ptr );
}

/*===========================================================================

FUNCTION init_private_data_space

DESCRIPTION
  This function initialises a public data space

===========================================================================*/
static void init_private_data_space( gas_id_t gas_id )
{
  mdsp_dtm_private_data_t* data = get_private_dataspace_ptr( gas_id);

  data->mdsp_gsm_txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID;
  data->fn_save     = 0;
  data->first_call = FALSE;
  data->header_only_received = FALSE;
}

#ifdef FEATURE_GSM_MDSP_RAND_GEN
/*===========================================================================

FUNCTION mdsp_random_timer_cmd

DESCRIPTION
  This function tells the mDSP every 4 seconds to generate the random seed,
  and sets bit 15 of the mDSP control field.

DEPENDENCIES
  None

RETURN VALUE
  uint16         Switches bit 15 on /off.

SIDE EFFECTS
  None
===========================================================================*/
uint16 mdsp_random_timer_cmd( gas_id_t gas_id )
{
          uint16  rand_switch = MDSP_SWITCH_RANDOM_OFF;
   static uint32  fn_save     = 0; /* static_dual_spaced_ignore */
   static boolean first_call = FALSE; /* static_dual_spaced_ignore */

   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   if ( !first_call )
   {
      /* Save the Fn for first time only. */
      fn_save    = GSTMR_GET_FN_GERAN(gas_id);
      first_call = TRUE;
   }

   /* Handle the Hyper Frame wrap scenario. */
   if (fn_save + MDSP_FOUR_SEC_TIMER >= MAX_MDSP_GSM_FRAME_NUMBER)
   {
     if (MDSP_FOUR_SEC_TIMER -
         (MAX_MDSP_GSM_FRAME_NUMBER - fn_save) >= GSTMR_GET_FN_GERAN(gas_id))
     {
       /* This is 4 sec so toggle the bit. */
       rand_switch = MDSP_SWITCH_RANDOM_ON;
       /* Save the Fn for next time. */
       fn_save = GSTMR_GET_FN_GERAN(gas_id);
     }
   }
   else if (GSTMR_GET_FN_GERAN(gas_id) >= fn_save + MDSP_FOUR_SEC_TIMER)
   {
       /* This is 4 sec so toggle the bit. */
       rand_switch = MDSP_SWITCH_RANDOM_ON;
       /* Save the Fn for next time. */
       fn_save = GSTMR_GET_FN_GERAN(gas_id);
   }

   return (rand_switch);
}

/*===========================================================================

FUNCTION mdsp_process_csprng_rnd_data

DESCRIPTION
  This function reads the 64bit Random seed from the mDSP and writes it to
  the security utility for it's processing.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mdsp_process_csprng_rnd_data( gas_id_t gas_id )
{
   MSG_GERAN_ERROR_0_G("CSPRNG Data not supported on Nikel");
}
#endif

/*===========================================================================

FUNCTION mdsp_dtm_use_aci_detection

DESCRIPTION
  This function determines if ACI detection should be used for the given
  channel.

===========================================================================*/
static boolean mdsp_dtm_use_aci_detection (mdsp_channel_type chan , gas_id_t gas_id )
{
   mdsp_gprs_public_data_t* mdsp_gprs_data_ptr = get_mdsp_gprs_public_dataspace_ptr( gas_id);

   /* First check global flag */


   if (!mdsp_gprs_data_ptr->mdsp_aci_detection) return FALSE;

#ifdef FEATURE_GSM_MDSP_ACI_DETECTION_VOICE
   /* Check if this is a voice channel */
   if (IS_MDSP_DL_VOICE_CHANNEL(chan)) return TRUE;
#endif

#ifdef FEATURE_GSM_MDSP_EGPRS
   /* Check for an EGPRS channel */
   if ( ( (chan == MDSP_PDTCHD) || (chan == MDSP_PTCCHD) ) && mdsp_gprs_data_ptr->mdsp_egprs_mode)
   {
     return TRUE;
   }
#endif

   return FALSE;
}

/*===========================================================================

FUNCTION mdsp_dtm_use_saic

DESCRIPTION
  This function determines if SAIC should be used for the given
  channel.

===========================================================================*/
boolean mdsp_dtm_use_saic (mdsp_channel_type chan, boolean saic_cfg , gas_id_t gas_id )
{
   boolean result = FALSE;

   /* First check global flag */
   if (!mdsp_get_enable_saic( gas_id )) return FALSE;

#ifdef FEATURE_GSM_MDSP_ESAIC
   /* We don't want SAIC and ESAIC to be enabled on EGPRS,
    * this will be controlled by saic_cfg flag in gl1_msg_set_egprs_mode()
    */

   if(saic_cfg == TRUE)
   {
     result = TRUE;
   }
#else
   /* This only allows SAIC in Dedicated Mode to reduce current draw. */
   if (chan != MDSP_PDTCHD && chan != MDSP_PTCCHD && saic_cfg == TRUE)
   {
      result = TRUE;
   }
#endif

   /* Use SAIC on all non-EGPRS channels */
   return (result);
}


#ifdef FEATURE_GSM_MDSP_ESAIC
/*===========================================================================

FUNCTION mdsp_dtm_use_esaic

DESCRIPTION
  This function determines if ESAIC should be used for the given
  channel.

===========================================================================*/
boolean mdsp_dtm_use_esaic (mdsp_channel_type chan, boolean saic_cfg , gas_id_t gas_id )
{
   boolean result = FALSE;

    /* First check global flag */
   if (!mdsp_get_enable_esaic( gas_id )) return FALSE;

   /* This only allows ESAIC in Dedicated Mode to reduce current draw. */
   if (saic_cfg == TRUE)
   {
      result = TRUE;
   }

   /* Use ESAIC on all non-EGPRS channels */
   return (result);
}


/*===========================================================================

FUNCTION mdsp_dtm_disable_iir_filter

DESCRIPTION
  This function determines if we need to disable IIR filter based on chan type
  current design specifies PCH, NCELL BCCH and SCELL BCCH IIR filter should be
  disabled

===========================================================================*/
boolean mdsp_dtm_disable_iir_filter(mdsp_channel_type chan,boolean idle_mode)
{

  boolean result = FALSE;

  if( ( (chan == MDSP_PCH) && (idle_mode == TRUE) ) || (chan == MDSP_BCCH) )
  {
    result = TRUE;
  }

  return (result);

}


#endif /* FEATURE_GSM_MDSP_ESAIC */

/*===========================================================================

FUNCTION mdsp_dtm_use_aeq

DESCRIPTION
  This function determines if AEQ should be used for the given
  channel.

===========================================================================*/
boolean mdsp_dtm_use_aeq (mdsp_channel_type chan,boolean dedicated_mode, boolean saic_cfg , gas_id_t gas_id )
{
   boolean result = FALSE;

    /* First check global flag */
   if(!mdsp_get_enable_aeq( gas_id )) return FALSE;

   /* ESAIC needs to be enabled for AEQ */
   if(!mdsp_get_enable_esaic( gas_id )) return FALSE;

   /* If SAIC is disabled then return FALSE */
   if (saic_cfg == TRUE)
   {
      /* Don't enable AEQ for Voice if FEATURE_GSM_AEQ_VOICE is undefined */
      if(dedicated_mode)
      {
#ifdef FEATURE_GSM_AEQ_VOICE
        result = TRUE;
#else
        MSG_GERAN_LOW_0_G(" VOICE AEQ OFF!! ");
        result = FALSE;
#endif

      }
      /* Always check if we are on a packet channel since
       * DTM mode we will be in dedicated_mode and also Rxing PDCH.
       */
      if ((chan == MDSP_PDTCHD) || (chan == MDSP_PTCCHD))
      {
        result = TRUE;
      }
   } /* end of if (saic_cfg == TRUE) */

   MSG_GERAN_LOW_3_G("AEQ status saic_cfg %d result %d [%d]",saic_cfg,result,GSTMR_GET_FN_GERAN(gas_id));

   /* Use ESAIC on all non-EGPRS channels */
   return (result);
}

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
/*===========================================================================

FUNCTION mdsp_dtm_use_aeq_8psk

DESCRIPTION
  This function determines if AEQ Ph2 (8PSK) should be used for the given
  channel. Currently on used on PDTCH

===========================================================================*/
boolean mdsp_dtm_use_aeq_8psk (mdsp_channel_type chan, boolean saic_cfg , gas_id_t gas_id )
{
   boolean result = FALSE;

   /*Check 8PSK Ph2 global flag*/
   if (!get_mdsp_intf_public_dataspace_ptr( gas_id)->mdsp_aeq_8psk) return FALSE;

   /* If SAIC is disabled then return FALSE */
   if (saic_cfg == TRUE)
   {
     if (chan == MDSP_PDTCHD)
     {
       result = TRUE;
     }
   } /* end of if (saic_cfg == TRUE) */

   MSG_GERAN_LOW_3_G("AEQ ph2 status saic_cfg %d result %d [%d]",saic_cfg,result,GSTMR_GET_FN_GERAN(gas_id));

   /* Use ESAIC on all non-EGPRS channels */
   return (result);
}

#endif /* FEATURE_GSM_MDSP_AEQ_8PSK */

#ifdef FEATURE_GSM_RX_DIVERSITY
/*===========================================================================

FUNCTION mdsp_dedicated_use_aeq

DESCRIPTION
  This function determines if AEQ should be used in dedicated mode (TCH/DTM mode).

===========================================================================*/
boolean mdsp_dedicated_use_aeq (boolean dedicated_mode, boolean saic_cfg, gas_id_t gas_id)
{
   boolean result = FALSE;

    /* First check global flag */
   if(!mdsp_get_enable_aeq( gas_id )) return FALSE;

   /* ESAIC needs to be enabled for AEQ */
   if(!mdsp_get_enable_esaic( gas_id )) return FALSE;

   /* If SAIC is disabled then return FALSE */
   if (saic_cfg == TRUE)
   {
      /* Don't enable AEQ for Voice if FEATURE_GSM_AEQ_VOICE is undefined
       * Need to disable rx diversity for pcacket channels.
            */
      if(dedicated_mode)
      {
#ifdef FEATURE_GSM_AEQ_VOICE
        result = TRUE;
#else
        MSG_GERAN_LOW_0_G(" VOICE AEQ OFF!! ");
        result = FALSE;
#endif
      }
   } /* end of if (saic_cfg == TRUE) */

   MSG_GERAN_LOW_3_G("AEQ status saic_cfg %d result %d [%d]",saic_cfg,result,GSTMR_GET_FN_GERAN(gas_id));

   /* Use ESAIC on all non-EGPRS channels */
   return (result);
}


#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE
/*===========================================================================

FUNCTION mdsp_idle_use_aeq

DESCRIPTION
  This function checks if AEQ is used in idle mode
===========================================================================*/
boolean mdsp_idle_use_aeq (boolean idle_mode, boolean saic_cfg, gas_id_t gas_id)
{
   boolean result = FALSE;

    /* First check global flag */
   if(!mdsp_get_enable_aeq( gas_id )) return FALSE;

   /* ESAIC needs to be enabled for AEQ */
   if(!mdsp_get_enable_esaic( gas_id )) return FALSE;

   /* If SAIC is disabled then return FALSE */
   if (saic_cfg == TRUE)
   {
      if(idle_mode)
      {
        result = TRUE;
      }
   }
#ifdef FEATURE_GSM_RX_DIVERSITY_IDLE_DEBUG
   MSG_GERAN_HIGH_3_G(GL1_RXD"IMRD_AGC_LOOP AEQ status saic_cfg %d result %d [%d]",
                      saic_cfg,result,GSTMR_GET_FN_GERAN(gas_id));
#endif
   /* Use ESAIC on all non-EGPRS channels */
   return (result);
}
#endif /*FEATURE_GSM_RX_DIVERSITY_IDLE*/




#endif /* FEATURE_GSM_RX_DIVERSITY */




/*===========================================================================

FUNCTION mdsp_dtm_rx_qdsp6

DESCRIPTION
  This function instructs the QDSP6 mDSP to start receiving bursts.

===========================================================================*/
void mdsp_dtm_rx_qdsp6
(
   mdsp_dtm_rx_params_type *params,
   mdsp_dtm_rx_tags_type   *tags,
   int32                   phase_incrmt,
   ARFCN_T                 arfcn,
   uint32                  rf_seq_num,
   gas_id_t                gas_id
)
{
   GfwCmdBufType    cmd;
   uint8            i, j;
   boolean          decode_usfs = FALSE;
   mdsp_dtm_rx_slot_params_type  *slot_params;
#ifdef FEATURE_GSM_MDSP_RAND_GEN
   uint16           rand_switch;
   rand_switch = mdsp_random_timer_cmd( gas_id );
#endif

   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);
   mdsp_gprs_public_data_t* mdsp_gprs_public_data_ptr = get_mdsp_gprs_public_dataspace_ptr( gas_id);
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);


   /* Sanity */
   if(  (params->num_slots == 0)
#ifdef  FEATURE_GSM_GPRS_MSC33
      ||(params->num_slots > MDSP_MAX_DL_TS)
#else
      ||(params->num_slots > 4)
#endif
     )
   {
      MSG_GERAN_ERROR_2_G("Bad num_slots %d, FN=%d", params->num_slots, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }
   memset(&cmd,0,sizeof(GfwCmdBufType));
   /* Fill in the command fields */
   cmd.rxCmd.cmd = GFW_SYNC_RX_CMD;

   cmd.rxCmd.controlField.arfcn = arfcn.num;

#ifdef FEATURE_VAMOS
   cmd.rxCmd.controlField.tsc = params->tsc & 0x7 ;
#else
   cmd.rxCmd.controlField.tsc = params->tsc;
#endif
   cmd.rxCmd.controlField.uncWin = mdsp_get_sync_rx_search_width( gas_id );
#ifdef FEATURE_GSM_MDSP_RAND_GEN
   cmd.rxCmd.controlField.genCsprng = rand_switch;
#endif /*FEATURE_GSM_MDSP_RAND_GEN*/
   cmd.rxCmd.metricsBufIndex             = mdsp_current_buffer_mod3( gas_id );  /* Add 3 as shares with sync metric Buffer */
   cmd.rxCmd.rfBufIndex                  = mdsp_current_buffer_mod3( gas_id );
   cmd.rxCmd.schedulerResultBufIndex     = mdsp_current_buffer_mod3( gas_id );
   for (i=0; i < 8; i++)
   {
      cmd.rxCmd.pdchvsUSFH[i] = mdsp_gprs_public_data_ptr->mdsp_usf_list[i];
   }
   cmd.rxCmd.iqSampleBuffer = (GfwIqSamplesBuffer*)&(mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwIqBuffer[mdsp_current_buffer( gas_id )]);
   cmd.rxCmd.phaseIncrement = phase_incrmt;
   cmd.rxCmd.numSlots = params->num_slots;
   cmd.rxCmd.gfwDelWkupFrNo = (GfwRxDelWkupFno) params->num_of_page_bursts_missed;
   if (ftm_get_mode() == FTM_MODE)
   {
      cmd.rxCmd.rxLeveldB = NO_IQ_SCALING_IN_FTM;
      cmd.rxCmd.rxLevelDrxdB = NO_IQ_SCALING_IN_FTM;
   }
   else 
   {
      cmd.rxCmd.rxLeveldB = params->rx_lev_dBm >> 4;
#ifdef FEATURE_GSM_RX_DIVERSITY
      cmd.rxCmd.rxLevelDrxdB = params->rx_lev_drx_dBm >> 4;
#else
      cmd.rxCmd.rxLevelDrxdB = NO_IQ_SCALING_IN_FTM;
#endif
   }
#ifdef GERAN_L1_HLLL_LNASTATE
   cmd.rxCmd.doJdetReading = FALSE;
   if(gl1_hw_get_sawless_HLLL_support(arfcn.band,gas_id))
     {
     cmd.rxCmd.doJdetReading = TRUE;
     }
#endif /*GERAN_L1_HLLL_LNASTATE*/

   cmd.rxCmd.rFacchFN = (uint16)(ADD_MOD(frame_counters[gas_id].FNmod42432, 1, 42432) );

   /* Now fill in slot info */
   for (i = 0; i < params->num_slots; i++)
   {
      /* Shortcut */
      slot_params = &params->slot_params[i];

      /* Fill in the command */
      cmd.rxCmd.slotInfo[i].controlField.forceFacchCs1 = slot_params->force_facch;
      cmd.rxCmd.slotInfo[i].controlField.lookForFacch = slot_params->look_for_facch;
      /* GFW expects firstBurst to be set for the 1st burst sent over the air, not
         for the first burst GFW has had an opportunity to decode (if we miss one) */
      cmd.rxCmd.slotInfo[i].controlField.firstBurst = slot_params->burst_num == 0 ? 1 : 0;
      cmd.rxCmd.slotInfo[i].controlField.aciDetect = 0;
      /*For desense channels frequency error is corrected by GFW using rotator*/
      if (params->DesenseArfcn != DESENSE_CHAN)
      {
         cmd.rxCmd.slotInfo[i].controlField.usePhaseInc = FALSE;
      }
      else
      {
         cmd.rxCmd.slotInfo[i].controlField.usePhaseInc = TRUE;
      }
      cmd.rxCmd.slotInfo[i].controlField.cmiPhase = slot_params->amr_rx_phase;
      cmd.rxCmd.slotInfo[i].chanTsField.chanType = (GfwChanType)slot_params->channel_type;
      cmd.rxCmd.slotInfo[i].chanTsField.tsNum = slot_params->timeslot;
      cmd.rxCmd.slotInfo[i].offset = (uint16)
      (mdsp_get_sync_rx_time_offset( gas_id) + QS_PER_TS * (uint16)slot_params->timeslot);
      cmd.rxCmd.slotInfo[i].logNewIq  = mdsp_intf_data_ptr->gsm_Iq_Log.sync_rx_iq;
      cmd.rxCmd.iqBufferIndex = mdsp_current_buffer( gas_id );

      /* NULL page detection requested or not */
      if ( slot_params->sbd_enabled )
      {
        cmd.rxCmd.slotInfo[i].controlField.bSbDecNullPage = 1;
      }
      else
      {
        cmd.rxCmd.slotInfo[i].controlField.bSbDecNullPage = 0;
      }

      cmd.rxCmd.slotInfo[i].controlField.lookForRfacch = slot_params->look_for_rfacch;
      cmd.rxCmd.slotInfo[i].controlField.rFacchPingPong = slot_params->rfacch_fw_buffer_index;
      if (slot_params->decode)
      {
        MSG_GERAN_LOW_3_G("RFACCH 2.0 DEBUG lookForRfacch %d, rFacchPingPong %d, decode_flag %d",
                           slot_params->look_for_rfacch, slot_params->rfacch_fw_buffer_index, slot_params->decode);
      }

      cmd.rxCmd.slotInfo[i].controlField.lookForRsacch = slot_params->look_for_rsacch;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if (((slot_params->channel_type == MDSP_PDTCHD) || (slot_params->channel_type == MDSP_PTCCHD)) &&
         (slot_params->burst_num == 3 ) )
      {
        cmd.rxCmd.slotInfo[i].controlField.prioritySchedule  = 1;
      }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
      {
        cmd.rxCmd.slotInfo[i].controlField.prioritySchedule  = 0;
      }

#ifdef FEATURE_GSM_COEX_FW_CXM

#ifdef FEATURE_GSM_QSC_TX_DIV
      if (antenna_switch_burst [gas_id] )
      {
      cmd.rxCmd.slotInfo[i].cxmPriority  = arbitrated_gas[gas_id].cxm_activity_table[ACTIVITY_TIER_3].priority  ;// ASDIV
      cmd.rxCmd.slotInfo[i].cxmActAsDiv  = GFW_CXM_ASDIV_ACT_REG ;
      antenna_switch_burst [gas_id] = FALSE ;
      }
      else
#endif /* FEATURE_GSM_QSC_TX_DIV*/
      {
        cmd.rxCmd.slotInfo[i].cxmPriority = slot_params->coex_params.coex_priority;
      }
      cmd.rxCmd.slotInfo[i].cxmDesenseId = slot_params->coex_params.coex_desense_id;

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if (cmd.rxCmd.slotInfo[i].controlField.prioritySchedule)
      {
        MSG_GERAN_MED_4_G("CXM : GMDSP RX slot=%d coex_priority=%d, coex_desense_id=%u prioritySchedule %d",
                         i,cmd.rxCmd.slotInfo[i].cxmPriority, cmd.rxCmd.slotInfo[i].cxmDesenseId,cmd.rxCmd.slotInfo[i].controlField.prioritySchedule);
      }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG */
#else
      cmd.rxCmd.slotInfo[i].cxmPriority  = 0;
      cmd.rxCmd.slotInfo[i].cxmDesenseId = 0;
#endif /* FEATURE_GSM_COEX */

#ifdef FEATURE_GSM_LOG_IQ_SAMPLES
      cmd.rxCmd.slotInfo[i].logIq = TRUE;
#else
      cmd.rxCmd.slotInfo[i].logIq = FALSE;
#endif
      cmd.rxCmd.slotInfo[i].saicControl.spurChanFlag = params->DesenseArfcn;

#ifdef FEATURE_GSM_RX_DIVERSITY
      cmd.rxCmd.slotInfo[i].rxdControl.enableRxd     = slot_params->rxdiv_control.enableRxd;
      cmd.rxCmd.slotInfo[i].rxdControl.enableDrxOnly = slot_params->rxdiv_control.enableDrxOnly;
      cmd.rxCmd.slotInfo[i].rxdControl.enablePrxOnly = slot_params->rxdiv_control.enablePrxOnly;
#endif

      cmd.rxCmd.slotInfo[i].saicControl.enableAeq2a = FALSE;


#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
      cmd.rxCmd.slotInfo[i].controlField.dtxDetect = slot_params->dtx_detected;
      cmd.rxCmd.slotInfo[i].controlField.amrPd = slot_params->amr_partial_decode;
#endif /* FEATURE_GSM_AMR_PD_DTX_DETECT */

      /* Enable SAIC for non-data channels; added saic on/off*/
      if (mdsp_dtm_use_saic(slot_params->channel_type,params->saic , gas_id ))
      {
        cmd.rxCmd.slotInfo[i].saicControl.enableSaic = 1;
        cmd.rxCmd.slotInfo[i].saicControl.shortSaic = MDSP_SAIC_SHORT;
        cmd.rxCmd.slotInfo[i].saicControl.gainRangeFlag = params->rf_gain.range==1? 1 : 0;
#ifdef FEATURE_VAMOS
        if(params->dedicated == TRUE)
        {
          /* we may be in dtm or voice look for PDTCHD if it is there then set tsc set1 other wise choose either tsc set1/2
     depending upon the tsc value*/
          if(slot_params->channel_type == MDSP_PDTCHD)
          {
            cmd.rxCmd.slotInfo[i].saicControl.vamosTscSet = 0;
          }
          else
          {
            cmd.rxCmd.slotInfo[i].saicControl.vamosTscSet = (params->tsc & 0x8) >> 3 ;
          }
        }
        else
        {
          /* we are for sure in packet transfer or bcch channel so set always tsc set 1*/
          cmd.rxCmd.slotInfo[i].saicControl.vamosTscSet = 0;
        }
        if(cmd.rxCmd.slotInfo[i].saicControl.vamosTscSet == 1)
        {
          MSG_GERAN_LOW_0_G("vamos tsc set 2 is given to FW in rx command");
        }
#endif

#ifdef FEATURE_GSM_MDSP_ESAIC
          if(mdsp_dtm_use_esaic(slot_params->channel_type,params->saic , gas_id ))
          {
            cmd.rxCmd.slotInfo[i].saicControl.enableESaic = 1;
          }
          else
          {
            cmd.rxCmd.slotInfo[i].saicControl.enableESaic = 0;
          }

          if( mdsp_dtm_disable_iir_filter(slot_params->channel_type,params->idle) )
          {
            /* disable iir filter as we are scheduling PCH or BCCH */
            cmd.rxCmd.slotInfo[i].saicControl.suspendIir = 1;
          }
          else
          {
            cmd.rxCmd.slotInfo[i].saicControl.suspendIir = 0;
          }
#endif

          if(mdsp_dtm_use_aeq(slot_params->channel_type,params->dedicated,params->saic , gas_id ))
          {
            cmd.rxCmd.slotInfo[i].saicControl.enableAeq = 1;
#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
            l1_physical_layer_power_data(AEQ_STATUS, TRUE, gas_id);
#endif
          }
          else
          {
            cmd.rxCmd.slotInfo[i].saicControl.enableAeq = 0;
          }

#ifdef FEATURE_GSM_MDSP_AEQ_8PSK
          if(mdsp_dtm_use_aeq_8psk(slot_params->channel_type,params->saic , gas_id ))
          {
            cmd.rxCmd.slotInfo[i].saicControl.enableAeq2 = 1;
#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
            l1_physical_layer_power_data(AEQ_STATUS, TRUE, gas_id);
#endif
          }
          else
          {
            cmd.rxCmd.slotInfo[i].saicControl.enableAeq2 = 0;
          }
#endif  /* FEATURE_GSM_MDSP_AEQ_8PSK*/
      }
      else
      {
         //cmd.rx.slot_info[i].saic_control[0] = 0;
         //cmd.rx.slot_info[i].saic_control[1] = 0;
         //cmd.rx.slot_info[i].saic_control[2] = 0;
      }


      //Disable enableAeq2a flag in DTM mode. It needs to be enabled only in EGPRS mode if RxD is disabled
      if(!gl1_is_current_state_dtm(gas_id)  && cmd.rxCmd.slotInfo[i].saicControl.enableAeq2 )
      {
#ifdef FEATURE_GSM_RX_DIVERSITY
        if((mdsp_gprs_public_data_ptr->mdsp_egprs_mode) && (slot_params->channel_type == MDSP_PDTCHD)
           &&(!cmd.rxCmd.slotInfo[i].rxdControl.enableRxd))
        {
          cmd.rxCmd.slotInfo[i].saicControl.enableAeq2a = TRUE;
        }
        else
        {
          cmd.rxCmd.slotInfo[i].saicControl.enableAeq2a = FALSE;
          MSG_GERAN_LOW_3("Not setting enableAeq2a : egprs_mode = %d channel_type = %d Rxd =%d ", mdsp_gprs_public_data_ptr->mdsp_egprs_mode , slot_params->channel_type , cmd.rxCmd.slotInfo[i].rxdControl.enableRxd );
        }
#else /*FEATURE_GSM_RX_DIVERSITY*/
        if((mdsp_gprs_public_data_ptr->mdsp_egprs_mode) && (slot_params->channel_type == MDSP_PDTCHD))
        {
          cmd.rxCmd.slotInfo[i].saicControl.enableAeq2a = TRUE;
        }
        else
        {
          cmd.rxCmd.slotInfo[i].saicControl.enableAeq2a = FALSE;
          MSG_GERAN_LOW_2("Not setting enableAeq2a : egprs_mode = %d channel_type = %d ", mdsp_gprs_public_data_ptr->mdsp_egprs_mode , slot_params->channel_type);
        }
#endif /*FEATURE_GSM_RX_DIVERSITY*/
      }

#ifdef FEATURE_AEQ_PHASE4
      cmd.rxCmd.slotInfo[i].saicControl.enableAeq4 = params->enableAeq4;
#ifdef FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO
      if ( params->enableAeq4 == TRUE )
      {
         l1_physical_layer_power_data(AEQ_STATUS, TRUE, gas_id);
      }
#endif /* FEATURE_GERAN_PHYSICAL_LAYER_POWER_INFO */
#endif /*FEATURE_AEQ_PHASE4 */

      /* Check if enabling ACI detection */
      if (mdsp_dtm_use_aci_detection(slot_params->channel_type , gas_id))
      {
         cmd.rxCmd.slotInfo[i].controlField.aciDetect = 1;
      }

      /* Take special action if the decode flag is set */
      if (slot_params->decode)
      {
         /* Find a free decoded data buffer */
         for (j = 0; j < MDSP_DTM_NUM_RX_DATA_BUFFERS; j++)
         {
            if (private_data_ptr->mdsp_dtm_rx_data_buffer[j].free_flag) break;
         }
         if (j == MDSP_DTM_NUM_RX_DATA_BUFFERS)
         {
            MSG_GERAN_ERROR_1_G("Out of RX buffers, unable to decode, FN=%d",GSTMR_GET_FN_GERAN(gas_id));
            cmd.rxCmd.slotInfo[i].decodedDataBufferIndex = j;
            tags->data_tag[i]               = 0xFFFF;
         }
         else
         {
            cmd.rxCmd.slotInfo[i].decodedDataBufferIndex = j;
            /* reset databuffer to delete erroneous data from previous data blocks */
            memset((GfwDecodedDataStruct*)&(mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[j]), 0, sizeof(GfwDecodedDataStruct));
            cmd.rxCmd.slotInfo[i].controlField.decodeFlag = 1;
            tags->data_tag[i] = j;

            /* Set the decode bypass bit if this is part of an early decode */
            if ( !gl1_msg_is_early_decode_disabled( gas_id ) &&
                 slot_params->early_decoding )
            {
               cmd.rxCmd.slotInfo[i].controlField.earlyDecode = 1;
            }
            else
            {
               /* Never set early decode to TRUE for F/W if disable feature enabled */
               cmd.rxCmd.slotInfo[i].controlField.earlyDecode = 0;
            }

            /*
             * Set enhanced page decode to TRUE for F/W if GCF is disabled
             * and page sharing not indicataed by RR on any SUB
             */
            if ( slot_params->epd_allowed &&
                 !params->gl1_hw_gprs_anite_gcf )
            {
               cmd.rxCmd.slotInfo[i].controlField.enableEPD = 1;
            }
            else
            {
               /* Never set enhanced page decode to TRUE for F/W if GCF is enabled */
               cmd.rxCmd.slotInfo[i].controlField.enableEPD = 0;
            }

            /* Clear any previous data semaphores since an abort may leave them set */
            mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[j].len = 0;

            /* If the channel is PDTCH we need to decoded usfs */
            if (slot_params->channel_type == MDSP_PDTCHD)
            {
               decode_usfs = TRUE;

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
               /* This counter is used to enable DM when required, not needed
                * for Q6 targets
                */
               mdsp_dm_pdtch_decodes++;
#endif
            }

            private_data_ptr->mdsp_dtm_rx_data_buffer[j].free_flag = FALSE;
            private_data_ptr->mdsp_dtm_rx_data_buffer[j].timeslot = slot_params->timeslot;
            private_data_ptr->mdsp_dtm_rx_data_buffer[j].channel_type = slot_params->channel_type;
#ifdef DEBUG_STALE_BUFFERS
            private_data_ptr->mdsp_dtm_rx_data_buffer[j].in_use_counter = 0;
#endif
         }
      }
      else
      {
         cmd.rxCmd.slotInfo[i].controlField.earlyDecode = 0;
#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
         cmd.rxCmd.slotInfo[i].decodedDataBufferIndex = MDSP_DTM_NUM_RX_DATA_BUFFERS - 1;
#else
         cmd.rxCmd.slotInfo[i].decodedDataBufferIndex = MDSP_DTM_NUM_RX_DATA_BUFFERS;
#endif /*FEATURE_GSM_GPRS_READ_CCCH_IN_XFER*/
         tags->data_tag[i]               = 0xFFFF;
      }

      /* Clear the metrics buffer */
      mdsp_intf_data_ptr->pMdspHostResultsStore->gfwMetricsBuffers[mdsp_current_buffer_mod3( gas_id )].gfwBurstMetricsBuffers[i].len = 0;

      /* Save the tag for returning to the caller */
#ifdef  FEATURE_GSM_GPRS_MSC33
      tags->metrics_tag[i] = (mdsp_current_buffer_mod3( gas_id ) * MDSP_MAX_DL_TS) + i;
#else
      tags->metrics_tag[i] = (mdsp_current_buffer_mod3( gas_id ) * 4) + i;
#endif

#ifdef DEBUG_GSM_MDSP_DTM_RX
      #error code not present
#endif
   }

#ifdef DEBUG_GSM_SLEEP_ERROR_RECOVERY
   MSG_HIGH("mdsp_dtm_rx_qdsp6 control_field %04x missed_frames %d FN %d",
            cmd.rxCmd.slotInfo[0].controlField.ctrlField,
            (uint8)  cmd.rxCmd.gfwDelWkupFrNo,
            GSTMR_GET_FN_GERAN(gas_id));
#endif

   /* Clear the USF buffer */
   if (decode_usfs)
   {
     mdsp_intf_data_ptr->pMdspHostResultsStore->gfwUsfResultBuffer.len = 0;
   }

   /* Clear the burst schedule buffer */
   mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerResultBuffers[mdsp_current_buffer_mod3( gas_id )].len = 0;

   cmd.rxCmd.rfSeqNr = rf_seq_num;

   /* Issue the command to the mdsp */
   mdsp_issue_qdsp6_command(&cmd,GFW_SYNC_RX_CMD , gas_id);

   /* Save the sequence number of this command to use later */
   for (i = 0; i < params->num_slots; i++)
   {
      if (params->slot_params[i].decode && tags->data_tag[i] != 0xFFFF)
      {
         private_data_ptr->mdsp_dtm_rx_data_buffer[tags->data_tag[i]].seq_num = mdsp_get_current_seq_num( gas_id );
      }
   }

   /* Burst metrics and schedule sequence number */
   private_data_ptr->mdsp_dtm_rx_burst_seq_num[mdsp_current_buffer_mod3( gas_id )] = mdsp_get_current_seq_num( gas_id );

   /* USF buffer sequence number */
   if (decode_usfs)
   {
      private_data_ptr->mdsp_dtm_rx_usfs_seq_num = mdsp_get_current_seq_num( gas_id );
   }

   /* Debug seq num */
   mdsp_intf_data_ptr->mdsp_sync_rx_burst_seq_num_fifo[mdsp_current_buffer_mod3( gas_id )] = mdsp_get_current_seq_num( gas_id );
}


/*===========================================================================

FUNCTION mdsp_dtm_get_rx_data

DESCRIPTION
  This function retrieves a data packet from the mDSP if available. This
  function should be called once for each pdch that was issued as part of
  the mdsp_dtm_rx cmd.

  tag - the data tag as returned by the mdsp_dtm_rx command
  data - pointer to decoded data results
  pdch - the pdch number for validation
  last_attempt - if this is the last try to get the data

===========================================================================*/
boolean mdsp_dtm_get_rx_data
(
   uint16             tag,          /* Tag of this message */
   GfwDecodedDataStruct *hdr,         /* Header storage */
   uint16             *data[],      /* Data storage */
   boolean            last_attempt  /* last attempt to get data */
   , gas_id_t gas_id
)
{
   boolean results_avail = FALSE;
   uint16  data_len;
   mdsp_timeslot_type decoded_pdch;
   mdsp_coding_type cs;

   mdsp_dtm_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* Sanity check */
   if (tag >= MDSP_DTM_NUM_RX_DATA_BUFFERS || private_data_ptr->mdsp_dtm_rx_data_buffer[tag].free_flag)
   {
      MSG_GERAN_ERROR_2_G("Bad rx data tag (tag=%d), FN=%d", tag, GSTMR_GET_FN_GERAN(gas_id));
      return FALSE;
   }

   /* Read the length field (used as flag) */
   data_len = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].len;

   /* If the data is ready, pull it out */

   if (data_len == MDSP_RX_HDR_SIZE)
   {
      if (private_data_ptr->header_only_received == FALSE)
      {
         private_data_ptr->header_only_received = TRUE;
         MSG_GERAN_MED_3_G("Only header data available: tn=%d, len=%d, FN=%d",
                  private_data_ptr->mdsp_dtm_rx_data_buffer[tag].timeslot,
                data_len, GSTMR_GET_FN_GERAN(gas_id));
      }
   }
   else if (data_len > 0)
   {

      hdr->len = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].len;
      hdr->seqNum = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].seqNum;
      hdr->gfwDecodedCtrlWord1 = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].gfwDecodedCtrlWord1;
      hdr->gfwDecodedCtrlWord2 = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].gfwDecodedCtrlWord2;
      hdr->hardRxQual = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].hardRxQual;
      hdr->softRxQual = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].softRxQual;
      hdr->bVamosPaired = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].bVamosPaired;
      hdr->rfacchLogBuff = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].rfacchLogBuff;
      hdr->epdLogPacket = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].epdLogPacket;

#ifdef FEATURE_AEQ_PHASE4
    if(gl1_get_enableAeq4_status(gas_id))
      {
        hdr->aeq4LogPacket    = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].aeq4LogPacket;
        gl1_hw_log_aeq4_metrics(&(hdr->aeq4LogPacket),gas_id);
      }
#endif /*FEATURE_AEQ_PHASE4*/
      /* Validate the data sequence number and pdch */
      decoded_pdch = (mdsp_timeslot_type) mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].gfwDecodedCtrlWord2.pdch;

      if (mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].seqNum != private_data_ptr->mdsp_dtm_rx_data_buffer[tag].seq_num)
      {
          MSG_GERAN_ERROR_3_G("Rx data: bad seq_num %d (expected %d), FN=%d",
                    mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].seqNum, private_data_ptr->mdsp_dtm_rx_data_buffer[tag].seq_num,
                    GSTMR_GET_FN_GERAN(gas_id));
      }
      else if (decoded_pdch != private_data_ptr->mdsp_dtm_rx_data_buffer[tag].timeslot)
      {
          MSG_GERAN_ERROR_3_G("Rx data: bad pdch %d (expected %d), FN=%d",
                    decoded_pdch,private_data_ptr->mdsp_dtm_rx_data_buffer[tag].timeslot,
                    GSTMR_GET_FN_GERAN(gas_id));
      }
      else
      {
          results_avail = TRUE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
          #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
      }

#ifdef DEBUG_GSM_MDSP_DTM_RX
      #error code not present
#endif
   }
   else if (last_attempt && private_data_ptr->header_only_received == FALSE)
   {
      MSG_GERAN_ERROR_3_G("Rx data not available: tn=%d, chan=%d, FN=%d",
                private_data_ptr->mdsp_dtm_rx_data_buffer[tag].timeslot,
                private_data_ptr->mdsp_dtm_rx_data_buffer[tag].channel_type, GSTMR_GET_FN_GERAN(gas_id));
   }

   /* If results are available, process them */
   if (results_avail)
   {
      /* If this is PDTCHD data we figure out the data length from the coding
         scheme.  Otherwise use what the dsp told us. */
      if (private_data_ptr->mdsp_dtm_rx_data_buffer[tag].channel_type == MDSP_PDTCHD)
      {
         cs = (mdsp_coding_type)mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].gfwDecodedCtrlWord2.CCS;
         data_len = mdsp_cs_to_length_lut[cs];
      }
      else
      {
         cs = MDSP_CODING_CS1;
         data_len -= MDSP_RX_HDR_SIZE;
      }

      /* Sanity check the data length */
      if (data_len > MAX_RX_DTM_DATA_PER_FRAME)
      {
         MSG_GERAN_ERROR_3_G("Decoded data length too large: len=%d, chan=%d, FN=%d!",
                   data_len, private_data_ptr->mdsp_dtm_rx_data_buffer[tag].channel_type,
                   GSTMR_GET_FN_GERAN(gas_id));
         data_len = MAX_RX_DTM_DATA_PER_FRAME;
      }

      /* Read the first message */
      memscpy(data[0],
              (GL1_DEFS_MAX_DATA * sizeof(uint16)),
               (void*)mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].data1,
               (data_len * 2));

#ifdef FEATURE_GSM_MDSP_EGPRS
      /* Read the second message if it exists */
      if (cs >= MDSP_CODING_MCS7)
      {
         memscpy(data[1],
                 (GL1_DEFS_MAX_DATA * sizeof(uint16)),
                  (void*)mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].data2,
                  (data_len * 2));
      }

      /* Read the EGPRS header */
      if (cs >= MDSP_CODING_MCS1)
      {
         memscpy(hdr->egprs_hdr,
                 (MDSP_RX_EGPRS_HDR_SIZE * sizeof(uint16)),
                  (void*)mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].egprs_hdr,
                  (MDSP_RX_EGPRS_HDR_SIZE * 2));

         hdr->enhEdgeLogPkt = mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSyncRxDataBuffer[tag].enhEdgeLogPkt;

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
         /* Validate any IR data */
         mdsp_check_IR_data(hdr);
#endif
      }
#endif
   }

   /* Free the data buffer if we are done with it */
   if (results_avail || last_attempt)
   {
      private_data_ptr->mdsp_dtm_rx_data_buffer[tag].free_flag = TRUE;

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
      /* Decrement count of PDTCHD decodes if necessary */
      if (private_data_ptr->mdsp_dtm_rx_data_buffer[tag].channel_type == MDSP_PDTCHD)
      {
         mdsp_dm_pdtch_decode_done(TRUE , gas_id);
      }
#endif
      private_data_ptr->header_only_received = FALSE;
   }

#ifdef FEATURE_GSM_MDSP_RAND_GEN
   mdsp_process_csprng_rnd_data();
#endif

   return results_avail;
}



/*===========================================================================

FUNCTION mdsp_dtm_get_rx_burst_metrics

DESCRIPTION
  This function retrieves the rx metrics that are returned by the mdsp as part
  of a mdsp_dtm_rx request.   Metrics for 4 bursts are returned into memory
  supplied as argument.     Metrics are available in frame N+1 or N+2 were N is
  where the burst occured.

===========================================================================*/
boolean mdsp_dtm_get_rx_burst_metrics
(
   uint16                    tag,
   GfwBurstMetricsStruct    *metrics,
   boolean                   get_hard_decisions
   , gas_id_t gas_id
)
{
   uint16         len, buf_index, buf_num;
   boolean       valid = FALSE;
   GfwMetricsStruct *gfwMetricsBuffer;
   GfwBurstMetricsStruct *gfwBurstMetricsBuffer;

   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* if tag is INVALID, do not attempt to retrieve metrics data */
   if (tag != MDSP_INVALID_TAG)
   {

      /* Calculate where to find the data */
#ifdef  FEATURE_GSM_GPRS_MSC33
      buf_num =   tag / MDSP_MAX_DL_TS;
      buf_index = tag % MDSP_MAX_DL_TS;
#else
      buf_num =   tag / 4;
      buf_index = tag % 4;
#endif

      gfwMetricsBuffer = (GfwMetricsStruct*)&(mdsp_intf_data_ptr->pMdspHostResultsStore->gfwMetricsBuffers[buf_num]);
      gfwBurstMetricsBuffer = &(gfwMetricsBuffer->gfwBurstMetricsBuffers[buf_index]);

      gl1_hw_handle_seq_num_error (GL1_GFW_RECOVERY_RX, gfwBurstMetricsBuffer->scheduleTimeError, gas_id);

      /* The size of each metrics log in words */
      len = sizeof(GfwBurstMetricsStruct) / 2;

#ifdef FEATURE_GSM_MDSP_EGPRS
      /* Skip the hard decisions unless requested */
      if (!get_hard_decisions) len -= MDSP_MAX_HARD_DECISIONS;
#else
      if (get_hard_decisions)
      {
         MSG_GERAN_ERROR_0_G("Hard decisions not available without EGPRS support");
      }
#endif /* FEATURE_GSM_MDSP_EGPRS */

      memscpy( metrics,
               sizeof(GfwBurstMetricsStruct),
               gfwBurstMetricsBuffer,
               sizeof(GfwBurstMetricsStruct) );

      if (metrics->len == 0)
      {
         MSG_GERAN_ERROR_2_G("Rx metrics: not ready for tag %d FN=%d",tag,GSTMR_GET_FN_GERAN(gas_id));
      }
      else if (metrics->seqNum != private_data_ptr->mdsp_dtm_rx_burst_seq_num[buf_num])
      {
         MSG_GERAN_ERROR_3_G("Rx metrics: bad seq_num %d exp %d, FN=%d",
                   metrics->seqNum,
                   private_data_ptr->mdsp_dtm_rx_burst_seq_num[buf_num],GSTMR_GET_FN_GERAN(gas_id));
      }
      else
      {
         valid = TRUE;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
         #error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
         (void) mdsp_validate_rx_metrics(metrics , gas_id);
      }

   /* Clear the semaphore */
      gfwBurstMetricsBuffer->len = 0;

   }
   else
   {
       MSG_GERAN_ERROR_0_G("Attempt to retrieve Rx burst metrics with INAVLID tag");
   }

   return valid;
}

/*===========================================================================

FUNCTION mdsp_gprs_get_tx_yield_result

DESCRIPTION
  This function retrieves the results of a mdsp_dtm_tx cmd and indicates
  in the data if the burst was scheduled successfully or not. Failures will
  have a reason code in the upper byte of the reason word.  Burst scheduler
  results will be available in frame N+2. frame N is where the burst occured.

===========================================================================*/
uint16 mdsp_gprs_get_tx_yield_result(gas_id_t gas_id, uint16 buf_idx)
{
   uint16 i;
   uint16 blanked_bursts = 0;
   volatile GfwSchedulerTxResultStruct* data;
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* It's very safe to assume this will be within bounds - however, KLCKWRK is moaning..*/
   if(buf_idx >= 3)
   {
      return 0;
   }

   data = &mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerTxResultBuffers[buf_idx];
   if (data->len > 0)
   {
      for (i = 0; i < 10; i++)
      {
         if (data->reason[i] & 0x7FFF)
         {
            blanked_bursts |= 1 << i;
            MSG_GERAN_HIGH_2_G("USF DEBUG Tx Burst Blanked, burst Number: %d, Scheduler Result: %d", i, data->reason[i]);
         }
      }
      MSG_GERAN_HIGH_1_G("USF DEBUG Blanked burst mask: %d", blanked_bursts);
      data->len = 0;
   }

   return blanked_bursts;
}


#ifdef FEATURE_GSM_COEX_RACH_DELAY
/*===========================================================================
FUNCTION mdsp_gprs_get_rach_burst_schedule

DESCRIPTION
  Get tx burst conflict while UE is doing rach
===========================================================================*/
uint16 mdsp_gprs_get_rach_burst_conflict ( gas_id_t gas_id )
{   
   uint16 tx_buf_index = SUB_MOD(mdsp_current_buffer_mod3(gas_id), 2, 3);
   return mdsp_gprs_get_tx_yield_result(gas_id, tx_buf_index);
}
#endif


/*===========================================================================

FUNCTION mdsp_gprs_get_rx_burst_schedule

DESCRIPTION
  This function retrieves the results of a mdsp_dtm_rx cmd and indicates
  in the data if the burst was scheduled successfully or not.  Failures will
  have a reason code in the upper byte of the reason word.  Burst scheduler
  results will be available in frame N+2. frame N is where the burst occured.

===========================================================================*/
boolean mdsp_gprs_get_rx_burst_schedule (GfwSchedulerResultStruct *data , gas_id_t gas_id )
{
   boolean  valid = FALSE;

   uint16 i;

   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   *data = *(&(mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerResultBuffers[mdsp_current_buffer_mod3( gas_id )]));

   /* The results are only valid if the seq num in the request matches */
   if (data->len == 0)
   {
      MSG_GERAN_ERROR_1_G("Rx sched: not available FN=%d",GSTMR_GET_FN_GERAN(gas_id));
   }
   else if (data->seqNum != private_data_ptr->mdsp_dtm_rx_burst_seq_num[mdsp_current_buffer_mod3( gas_id )])
   {
      MSG_GERAN_ERROR_3_G("Rx sched: bad seq_num %d exp %d FN=%d", data->seqNum,
                private_data_ptr->mdsp_dtm_rx_burst_seq_num[mdsp_current_buffer_mod3( gas_id )],GSTMR_GET_FN_GERAN(gas_id));
   }
   else
   {
      valid = TRUE;
   }

   for (i=0;i<8;i++) 
   {
      if ( mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerResultBuffers[mdsp_current_buffer_mod3( gas_id )].reason[i] & ENTER_MODE_LATE_CANCEL_BURST )
      {
         gl1_hw_enter_mode_late_cancel_burst (gas_id);
      }
   }

   /* Clear the semaphore */
   mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerResultBuffers[mdsp_current_buffer_mod3( gas_id )].len = 0;

   return valid;
}

/*===========================================================================

FUNCTION mdsp_ccch_get_rx_burst_schedule

DESCRIPTION
  This function retrieves the results of a mdsp_dtm_rx cmd and indicates
  in the data if the burst was scheduled successfully or not.  Failures will
  have a reason code in the upper byte of the reason word.  Burst scheduler
  results will be available in frame N+2. frame N is where the burst occured.

===========================================================================*/
boolean mdsp_ccch_get_rx_burst_schedule (GfwSchedulerResultStruct *data , gas_id_t gas_id )
{
   boolean  valid = FALSE;

   uint16 i;

   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);
   uint8 mdsp_reported_buffer_mod3 = SUB_MOD(mdsp_current_buffer_mod3( gas_id ),2,3);
   
   *data = *(&(mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerResultBuffers[mdsp_reported_buffer_mod3]));
   /* The results are only valid if the seq num in the request matches */
   if (data->len == 0)
   {
      MSG_GERAN_ERROR_1_G("Rx sched: not available FN=%d",GSTMR_GET_FN_GERAN(gas_id));
   }
   else if (data->seqNum != private_data_ptr->mdsp_dtm_rx_burst_seq_num[mdsp_reported_buffer_mod3])
   {
      MSG_GERAN_ERROR_3_G("Rx sched: bad seq_num %d exp %d FN=%d", data->seqNum,
                private_data_ptr->mdsp_dtm_rx_burst_seq_num[mdsp_reported_buffer_mod3], GSTMR_GET_FN_GERAN(gas_id));
   }
   else
   {
      valid = TRUE;
   }

   for (i=0;i<8;i++) 
   {
      if ( mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerResultBuffers[mdsp_reported_buffer_mod3].reason[i] & ENTER_MODE_LATE_CANCEL_BURST )
      {
         gl1_hw_enter_mode_late_cancel_burst (gas_id);
      }
   }

   /* Clear the semaphore */
   mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerResultBuffers[mdsp_reported_buffer_mod3].len = 0;

   return valid;
}

#ifdef FEATURE_GSM_AMR_PD_DTX_DETECT
/*===========================================================================

FUNCTION mdsp_gprs_get_dtx_amr_pd_metrics

DESCRIPTION
  This function retrieves amr PD and DTX detect metrics.

===========================================================================*/
boolean mdsp_gprs_get_dtx_amr_pd_metrics(uint16 tag, GfwDtxAmrPdMetrics *data, gas_id_t gas_id )
{
   boolean  valid = TRUE;
   uint16   buf_num;
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* Calculate where to find the data */
#ifdef  FEATURE_GSM_GPRS_MSC33
   buf_num =   tag / MDSP_MAX_DL_TS;
#else
   buf_num =   tag / 4;
#endif

   if (buf_num >= TRIPPLEBUF)
   {  
      buf_num = 0;
   }

   *data = *(&(mdsp_intf_data_ptr->pMdspHostResultsStore->gfwDtxAmrPdLogBuffer[buf_num]));

   return valid;
}
#endif
/*===========================================================================

FUNCTION mdsp_gprs_get_rx_burst_schedule

DESCRIPTION
  This function retrieves the results of a mdsp_dtm_tx cmd and indicates
  in the data if the burst was scheduled successfully or not.  Failures will
  have a reason code in the upper byte of the reason word.  Burst scheduler
  results will be available in frame N+2. frame N is where the burst occured.

===========================================================================*/
uint16 mdsp_gprs_get_tx_burst_schedule ( gas_id_t gas_id )
{
   uint16 i,j;
   uint16 blanked_bursts = 0;

   volatile GfwSchedulerTxResultStruct* data;

   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* This function is called by gl1_msgi_ul_pacch_pdtch_hdlr on every frame, except frame 11 (mod 13)
    * the scheduler tx result for frame 10 and 11 are read during frame 12 and 0 */
   if (frame_counters[gas_id].FNmod13 == 12 || frame_counters[gas_id].FNmod13 == 0)
   {
     j = mdsp_current_buffer_mod3(gas_id);
   }
   else
   {
     j = SUB_MOD(mdsp_current_buffer_mod3(gas_id), 2, 3);
   }

   data = &mdsp_intf_data_ptr->pMdspHostResultsStore->gfwSchedulerTxResultBuffers[j];
   MSG_GERAN_LOW_3_G("USF DEBUG mdsp_gprs_get_tx_burst_schedule, Buffer: %d, seq num: %d, Num TX bursts: %d", j, data->seqNum, data->len);
   if (data->len > 0)
   {
     for (i = 0; i < 10; i++)
     {
       if (data->reason[i] & 0x7FFF)
       {
         blanked_bursts |= 1 << i;
         MSG_GERAN_LOW_2_G("USF DEBUG Tx Burst Blanked, burst Number: %d, Scheduler Result: %d", i, data->reason[i]);
       }
     }
     MSG_GERAN_HIGH_1_G("USF DEBUG Blanked burst mask: %d", blanked_bursts);
     data->len = 0;
   }

   return blanked_bursts;
}

/*===========================================================================

FUNCTION mdsp_gprs_get_rx_usfs

DESCRIPTION
  This function retrieves the results on what decoded USFs match with the USF
  list provided.   This function should be called after a mdsp_gprs_sync_rx
  cmd has been issued with the decode flag set. This results provided are for
  the last mdsp_dtm_rx issued for a PDTCHD channel with the decode flag set.

===========================================================================*/
boolean mdsp_gprs_get_rx_usfs (GfwUsfBufStruct *data , gas_id_t gas_id )
{
   boolean valid = FALSE;

   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   *data = *(&(mdsp_intf_data_ptr->pMdspHostResultsStore->gfwUsfResultBuffer));

   /* The results are only valid if the seq num in the request matches */
   if (data->len == 0)
   {
      /* Not an error, let the upper layer handle this */
   }
   else if (data->seqNum != private_data_ptr->mdsp_dtm_rx_usfs_seq_num)
   {
      MSG_GERAN_ERROR_3_G("Rx decoded USFs: bad seq_num %d (expected %d), FN=%d", data->seqNum,
                private_data_ptr->mdsp_dtm_rx_usfs_seq_num,GSTMR_GET_FN_GERAN(gas_id));
   }
   else
   {
      valid = TRUE;
   }

   /* Clear semaphore */
   mdsp_intf_data_ptr->pMdspHostResultsStore->gfwUsfResultBuffer.len = 0;

   return valid;
}



/*===========================================================================

FUNCTION mdsp_dtm_write_tx_data

DESCRIPTION
  This function is used to write data into the mdsp TX buffers.
  Returns FALSE if the message could not be written to shared memory.

===========================================================================*/
boolean mdsp_dtm_write_tx_data
(
   mdsp_channel_type         chan,       /* logical channel     */
   mdsp_dtm_tx_msg_type      *msg,       /* ptr to tx data      */
   mdsp_specific_msg_type    msg_type,   /* usf or pdch         */
   mdsp_timeslot_type        timeslot,   /* time slot           */
   boolean                   auto_clear, /* whether caller will check sched */
   uint16                    *tag        /* tag used for checking schedule */
   , gas_id_t gas_id
)
{
   uint8     index, data_buffer;
   uint16   len;

   mdsp_dtm_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* Find an empty data buffer to use */
   for (data_buffer = 0; data_buffer < MDSP_DTM_NUM_TX_DATA_BUFFERS; data_buffer++)
   {
      if (private_data_ptr->mdsp_tx_buffer_free[data_buffer]) break;
   }
   if (data_buffer == MDSP_DTM_NUM_TX_DATA_BUFFERS)
   {
      MSG_GERAN_ERROR_0_G("No TX data buffers available");
      return FALSE;
   }

   /* Find a control structure as well */
   for (index = 0; index < MDSP_DTM_NUM_TX_CTL_BUFFERS; index++)
   {
      if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].free_flag) break;
   }
   if (index == MDSP_DTM_NUM_TX_CTL_BUFFERS)
   {
      MSG_GERAN_ERROR_0_G("No TX ctl buffers available");
      return FALSE;
   }

   /* Form the tx message control word */
   memset(&(private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].control_field),0,sizeof(GfwGsmTxMsgCtrlWord));
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].control_field.channelType = chan;
   if (msg_type != MDSP_USF_SPECIFIC)
   {
      private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].control_field.PDCH = timeslot;
      private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].control_field.M = 1;
   }

#ifdef FEATURE_GSM_MDSP_EGPRS
   /* Add puncturing schemes to the control word */
   if (IS_MDSP_EGPRS_CHANNEL(chan))
   {
      /* Add the first puncturing scheme */
      mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].egprs.controlField.ps1 = msg->egprs.puncturing[0];

      /* Write the EGPRS header */
      memscpy((void*)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].egprs.hdr,
               (3 * sizeof(uint16)),
               msg->egprs.hdr,
               (3 * sizeof(uint16)));
      /* Write the first EGPRS message */
      memscpy((void*)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].egprs.data_1,
              (MAX_TX_DTM_DATA_PER_FRAME * sizeof(uint16)),
               msg->egprs.data[0],
               (msg->egprs.len * 2));
      len = MDSP_DTM_TX_EGPRS_DATA1_OFFSET + msg->egprs.len;

      if (chan >= MDSP_PDTCHU_MCS7)
      {
         /* Add the second puncturing scheme */
         mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].egprs.controlField.ps2 = msg->egprs.puncturing[1];

         memscpy((void*)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].egprs.data_2,
                 (MAX_TX_DTM_DATA_PER_FRAME * sizeof(uint16)),
                  msg->egprs.data[1],
                  (msg->egprs.len * 2));
         len = MDSP_DTM_TX_EGPRS_DATA2_OFFSET + msg->egprs.len;
      }
   }
   else
#endif
   if (IS_MDSP_GPRS_CHANNEL(chan))
   {
      /* Write the GPRS data - no control word for GPRS */
      memscpy((void*)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.data,
              (MAX_TX_DTM_DATA_PER_FRAME * sizeof(uint16)),
               msg->gprs.data,
               (msg->gprs.len * 2));
      len = MDSP_DTM_TX_GPRS_DATA_OFFSET + msg->gprs.len;
   }
   else
   {
      /* Write the first msg data, AMR is hacky because the first word of the
         passed-in data is actually the control word */
      if (IS_MDSP_AMR_CHANNEL(chan))
      {
         uint16 *data = (uint16 *)msg->gsm.data[0];
         len = MDSP_DTM_TX_AMR_DATA_OFFSET + msg->gsm.len[0];
         /* For facch channels need to copy the data from data[0]+1 and for
            AMR channels need to copy from data[0]+2. The reason is AMR Channels
            has two control words in the buffer. This implementation is changed from
            Q4 to Q6. I have made a structure for these two controlwords into one.*/
         mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.sidFirstInhibit = ((data[0] & 0x0100 ) >> 8);
         mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.sidUpdateInhibit = ((data[0] & 0x0200 ) >> 9);
         mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.onset = ((data[0] & 0x0400 ) >> 10);
         mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.phase = ((data[0] & 0x0800 ) >>11);
         mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.cmi = ((data[0] & 0x3000 ) >> 12);
         mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.cmc = ((data[0] & 0xc000 ) >> 14);
#ifdef FEATURE_GSM_AMR_WB
         if(chan == MDSP_FACCH_AFS || chan == MDSP_FACCH_AHS || chan == MDSP_FACCH_WFS)
#else
         if(chan == MDSP_FACCH_AFS || chan == MDSP_FACCH_AHS)
#endif
         {
           msg->gsm.len[0] -= 1;
           memscpy((void*)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.data,
                  (MAX_TX_DTM_DATA_PER_FRAME * sizeof(uint16)),
                  (msg->gsm.data[0] + 1),
                  (msg->gsm.len[0] * 2));
         }
         else
         {
           mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.codecRate = (data[1] & 0x000F );
           mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.sidFirst = ((data[1] & 0x0100 ) >> 8);
           mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.sidUpdate = ((data[1] & 0x0200 ) >> 9);
           mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.ratscch = ((data[1] & 0x0400 ) >> 10);
           msg->gsm.len[0] -= 2;
           memscpy((void*)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.data,
                  (MAX_TX_DTM_DATA_PER_FRAME * sizeof(uint16)),
                (msg->gsm.data[0] + 2),
                (msg->gsm.len[0] * 2));
    }

      }
      else
      {
         len = MDSP_DTM_TX_GSM_DATA_OFFSET + msg->gsm.len[0];
         memscpy((void*)mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.data,
                  (MAX_TX_DTM_DATA_PER_FRAME * sizeof(uint16)),
                  msg->gsm.data[0],
                  (msg->gsm.len[0] * 2));
      }

      /* Write the second msg data if it exists */
      if (msg->gsm.len[1] > 0)
      {
         memscpy((void*)(mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.data +msg->gsm.len[0]),
                  (MAX_TX_DTM_DATA_PER_FRAME * sizeof(uint16)),
                  msg->gsm.data[1],
                  (msg->gsm.len[1] * 2));

         len += msg->gsm.len[1];
      }
   }

   /* Write the length field to the mdsp, used as a flag */
   mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.len = len +1;

   /* Mark the buffers as occupied and return the tag to the caller */
   private_data_ptr->mdsp_tx_buffer_free[data_buffer] = FALSE;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].free_flag = FALSE;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].msg_type = msg_type;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].channel_type = chan;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].data_buffer = data_buffer;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].data_state = TX_DATA_STATE_READY;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].bursts_remaining = 0;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].auto_clear = auto_clear;
#ifdef DEBUG_STALE_BUFFERS
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[index].in_use_counter = 0;
#endif
   *tag = index;

#ifdef DEBUG_GSM_MDSP_DTM_TX
   #error code not present
#endif

   /* Log the written data */
   mdsp_log_dtm_tx_trace_buf(chan, msg, msg_type, timeslot , gas_id);

   return TRUE;
}



/*===========================================================================

FUNCTION mdsp_num_tx_bursts

DESCRIPTION
  This function returns the number of transmit bursts required for a
  given channel type.  This will be either 1 (PTCCH/RACH), 2 (HS) or 4.

===========================================================================*/
static uint16 mdsp_num_tx_bursts (uint16 tag , gas_id_t gas_id )
{
   mdsp_dtm_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);


#ifdef FEATURE_GSM_MDSP_EGPRS
   /* Loopback data is a single burst */
   if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type == MDSP_LOOPBACK_SPECIFIC)
   {
      return 1;
   }
#endif

   /* Determine number of bursts based on channel type */
   switch (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].channel_type)
   {
      /* Access burst channels (RACH is the same channel type as PRACH) */
      case MDSP_PRACH:
      case MDSP_PRACH_EXT:
      case MDSP_PTCCHU:
      case MDSP_PTCCHU_EXT:
      {
         return 1;
      }

      /* Half-rate voice */
      case MDSP_TCH_HS:
      {
         return 2;
      }

      /* AMR half-rate voice */
      case MDSP_TCH_AHS:
      {
         /* We need to check if this is a SID_FIRST, SID_UPDATE or RATSCCH
            message which has 4 bursts despite being channel type AHS.  The
            vocoder frame type is stored in the third word of the data buffer. */
         uint16 data_buffer  = private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_buffer;
         if(mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.ratscch ||
            mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.sidUpdate ||
            mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].gsm_gprs.controlField.sidFirst)
         {
            return 4;
         }
         else
         {
         return 2;
      }
      }

      /* All other channel types */
      default:
      {
         return 4;
      }
   }
}



/*===========================================================================

FUNCTION mdsp_dtm_fill_tx_cmd_msgs

DESCRIPTION
  This function fills in the msg_info component of the DTM tx command with
  the messages of the given type.

===========================================================================*/
static void mdsp_dtm_fill_tx_cmd_msgs
(
   GfwGsmTxCmd*            GsmTxCmd_ptr,
   mdsp_dtm_tx_params_type *params
   , gas_id_t gas_id
)
{
   uint8  i, msg_cnt = 0, usf_index = 0, da_index, fa_index, index;
   uint16  data_buffer;
   uint16  tag;

   mdsp_dtm_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* mDSP requires that FA messages go first in the command, so we first
      go through and reserve space for all the fixed alloc messages.  Note
      that if something goes wrong when filling the command below and we
      skip an FA message the command will contain garbage if DA messages
      are also present...no nice way around that. */
   da_index = fa_index = 0;

   for (i = 0; i < params->num_msgs; i++)
   {
      tag = params->data_tags[i];
      if (tag >= MDSP_DTM_NUM_TX_CTL_BUFFERS || private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag)
      {
         continue;
      }

      /* Increment the DA index if we require space for an FA */
      if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type != MDSP_USF_SPECIFIC)
      {
         da_index++;
      }
   }

   /* Go through all scheduled transmit messages */
   for (i = 0; i < params->num_msgs; i++)
   {
      tag = params->data_tags[i];
      if (tag >= MDSP_DTM_NUM_TX_CTL_BUFFERS ||
          private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag)
      {
         MSG_GERAN_ERROR_1_G("Tx ctl buffer %d empty or invalid",tag);
         continue;
      }

#ifdef FEATURE_GSM_MDSP_EGPRS
      if (params->loopback &&
          private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type != MDSP_LOOPBACK_SPECIFIC)
      {
         MSG_GERAN_ERROR_3_G("Only loopback data allowed: chan=%d, type=%d, FN=%d",
                   private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].channel_type,
                   private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type, GSTMR_GET_FN_GERAN(gas_id));
         continue;
      }
#endif

      /* Figure out where in the command this will go based on alloc type */
      index = (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type == MDSP_USF_SPECIFIC ?
               da_index++ : fa_index++);

      /* Check if this burst requires encoding, data may be in ready state
         again after calling mdsp_dtm_get_tx_schedule so need to also check
         that previous message is complete */
      if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state == TX_DATA_STATE_READY &&
          private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].bursts_remaining == 0)
      {
         data_buffer = private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_buffer;

         /* Sanity check that this buffer contains valid data to tx */
         if (mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer].egprs.len == 0)
         {
            /* Very bad, data not present, try to clean up */
            MSG_GERAN_ERROR_2_G("Tx data buffer %d out of sync, FN=%d",
                      data_buffer,GSTMR_GET_FN_GERAN(gas_id));
            private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state = TX_DATA_STATE_CLEARED;
            private_data_ptr->mdsp_tx_buffer_free[data_buffer] = TRUE;
            private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].bursts_remaining = 0;
            continue;
         }

         /* For USF messages the PDCH field must be set to the msg index */
         if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type == MDSP_USF_SPECIFIC)
         {
            private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].control_field.PDCH = usf_index;
            usf_index++;
         }

         /* Set the control field with encode flag and the data buffer pointer */
         private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].control_field.E = 1;
         GsmTxCmd_ptr->msgInfo[index].dataBuffer = (GfwDtmTxStruct*)&(mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[data_buffer]);

         /* Update the data state */
         private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state = TX_DATA_STATE_SCHEDULED;
         private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].bursts_remaining = mdsp_num_tx_bursts(tag , gas_id);
      }
      else
      {
         /* No encoding required */
         private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].control_field.E = 0;
         GsmTxCmd_ptr->msgInfo[index].dataBuffer = NULL;
      }
      GsmTxCmd_ptr->msgInfo[index].controlField = private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].control_field;

      /* Decrement the bursts count */
      if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].bursts_remaining == 0)
      {
         MSG_GERAN_ERROR_3_G("No tx data left: chan=%d, state=%d, FN=%d",
                   private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].channel_type,
                   private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state, GSTMR_GET_FN_GERAN(gas_id));
      }
      else
      {
         private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].bursts_remaining--;
      }

#ifdef DEBUG_GSM_MDSP_DTM_TX
      #error code not present
#endif

#ifdef DEBUG_STALE_BUFFERS
      /* Reset in_use counter */
      private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].in_use_counter = 0;
#endif

      msg_cnt++;
   }

   /* Save the new message count */
   GsmTxCmd_ptr->num_msgs = msg_cnt;

}


/*===========================================================================

FUNCTION mdsp_ab_ts_match

DESCRIPTION
  This function matches the Tn the AB Pdch Specific is on.

===========================================================================*/
static boolean mdsp_ab_ts_match(uint8 *pdch_num, uint8 tn)
{
   uint8   i;
   boolean result = FALSE;

   for (i = 0; i < MDSP_MAX_UL_TS; i++)
   {
      if (tn == pdch_num[i])
      {
         result = TRUE;
#ifdef DEBUG_GSM_MDSP_DTM_TX
         #error code not present
#endif
         break;
      }
   }

   return (result);
}

/*===========================================================================

FUNCTION mdsp_dtm_tx

DESCRIPTION
  This function tells the mDSP to start transmitting multiple burst.

      Design Note:
      The mDSP driver sends the cmd, and data to the mDSP in order, and
      the mDSP for the PDCH Specific 0x910d,and lookes at bits 12,13,and 14
      0x910d => [ 1 ][     001   ][  000     ][   1   ][  00001101  ]
                  P   Pdch/USFInd  Don't Care  Encode    Chan Type

      Description:
      P           =  PDCH_SPECIFIC
      Pdch/USFInd = PDCH Chan TS, or if [P] bit15 not set it's USF Index
      Don't Care  =
      Encode      = First frame of the block the mDSP encodes the data
                    sent in cmd->tx.msg_info[index].data_buffer.
      Chan Type   = All the channel Types.

      RF Driver for a RF Burst timing of -13 has aburst_timing_offset of 2337

        tn=0 is 2337 + (625 * 0) - (50 * 4) = 2137
        tn=1 is 2337 + (625 * 1) - (0 * 4)  = 2962; see the AB is on Tn1
        tn=2 is 2337 + (625 * 2) - (50 *4 ) = 3387

        CMU-200 Example 1:
        mDSPtx:tn=0,tag=0,cw=0x910d,tsc=5,ab_tsc=0,ta=50,off=2137,FN=32455
        mDSPtx:tn=1,tag=1,cw=0x0108,tsc=5,ab_tsc=0,ta=0,off=2962,FN=32455
        mDSPtx:tn=2,tag=2,cw=0x1108,tsc=5,ab_tsc=0,ta=50,off=3387,FN=32455

        8960 Example 2:
        mDSPtx:tn=1,tag=0,cw=0xa100,tsc=5,ab_tsc=0,ta=0,off=2962,FN=132638
        mDSPtx:tn=2,tag=1,cw=0x0129,tsc=5,ab_tsc=0,ta=0,off=3587,FN=132638

        8960 Example 3:
        mDSPtx:tn=1,tag=1,cw=0xa000,tsc=5,ab_tsc=0,ta=50,off=2762,FN=1181056
        mDSPtx:tn=2,tag=2,cw=0x0029,tsc=5,ab_tsc=0,ta=50,off=3387,FN=1181056


      This debug shows the 1st frame of the block where mDSP driver loads up
      the gsm_tx_cmd.

      mDSP processing does the following:
      If the burst is PDCH Specific burst it looks in bits 12,13,and 14, and
      places the data in the correct PDCH TS, and uses the offset for that
      timeslot, and attached the data that was sent in the index for the PDCH.
      If the PDCH Specific burst is an AB then TA of 0 is applied.

      Note: GSM frames are sent as PDCH Specific and the same as GPRS
      it carries the Timeslot number in bits 12,13, and 14 of the
      cmd.tx.msg_info[i].control_field  .


===========================================================================*/
void mdsp_dtm_tx
(
   mdsp_dtm_tx_params_type *params, ARFCN_T arfcn, uint16 pa_index[], uint32 rf_seq_num, gas_id_t gas_id
)
{
   static GfwGsmTxCmd  gsmTxCmd[3];
   uint16             tn;
   uint8               i;
   uint8               timing_advance;
#ifdef FEATURE_VSWR
   boolean             rach_burst = FALSE;
#endif /*FEATURE_VSWR*/
#ifdef  FEATURE_GSM_GPRS_MSC33
   uint8              pdch_num[MDSP_MAX_UL_TS];
#else
   uint8              pdch_num[4];
#endif

   mdsp_channel_type  channel= MDSP_PDTCHU_CS1;

#ifdef DEBUG_GSM_MDSP_DTM_TX
   #error code not present
#endif

#if defined(FEATURE_GSM_MDSP_EGPRS)
   boolean            abort_srbl_tx = FALSE;
#endif

   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);

   /* Sanity */
   if (params->num_slots == 0 ||
       params->num_msgs  == 0 ||
       params->num_slots > MDSP_MAX_UL_TS ||
       params->num_msgs  > MDSP_MAX_UL_TS)
   {
      MSG_GERAN_ERROR_3_G("Bad num tx slots (%d) or msgs (%d), FN=%d",
                params->num_slots, params->num_msgs, GSTMR_GET_FN_GERAN(gas_id));
      return;
   }
   /* Clear Q6 Tx command */
   memset( &gsmTxCmd[gas_id], 0, sizeof(GfwGsmTxCmd));

   /* Fill in the message parameters */
   mdsp_dtm_fill_tx_cmd_msgs(&gsmTxCmd[gas_id], params , gas_id);

   /* Fill in the message parameters */
   memset(pdch_num,0xff,sizeof(pdch_num));

   /* Generate the assigned pdch list */
   for (i = 0; i < params->num_slots; i++)
   {
      /* Get the channel being used to see if it's a Access Burst. */
      channel = (mdsp_channel_type)(gsmTxCmd[gas_id].msgInfo[i].controlField.channelType);
      /* Determine in PTM if the Slot is an Access Burst, and set TA.*/
      if (channel == MDSP_PACCHU || channel == MDSP_PACCHU_EXT ||
          channel == MDSP_PTCCHU || channel == MDSP_PTCCHU_EXT ||
          channel == MDSP_RACH   || channel == MDSP_PRACH      ||
          channel == MDSP_PRACH_EXT)
      {
         pdch_num[i] = gsmTxCmd[gas_id].msgInfo[i].controlField.PDCH;
#ifdef DEBUG_GSM_MDSP_DTM_TX
         #error code not present
#endif
      }
   }

   /* Generate the assigned pdch list */
   for (i = 0; i < params->num_slots; i++)
   {

#ifdef DEBUG_GSM_MDSP_DTM_TX
      #error code not present
#endif
#ifdef FEATURE_VSWR
      if (channel == MDSP_RACH || channel == MDSP_PRACH  ||channel == MDSP_PRACH_EXT )
      {
          rach_burst = TRUE;
      }
      else
      {
          rach_burst = FALSE;
      }
#endif /*FEATURE_VSWR*/
      /* Timeslot being processed in order. */
      tn = (uint16)params->timeslots[i];

      /* Set this for the mDSP here. */
      gsmTxCmd[gas_id].slotInfo[i].timeslot = tn;

      /* The TA for Access Bursts isn't sent correctly */
      /* If it's a AB then set timing advance to 0. */
      if (mdsp_ab_ts_match(pdch_num, tn))
      {
         timing_advance = 0;
      }
      /* Use the normal timing advance for NB's. */
      else
      {
         timing_advance = params->timing_advance;
      }

      /* Set the Offset appropriately for the mDSP. */
      gsmTxCmd[gas_id].slotInfo[i].offset   = (uint16)mdsp_intf_data_ptr->mdsp_tx_burst_offset
                                   + (uint16)(QS_PER_TS * tn)
                                   - (uint16)(timing_advance * 4);

#ifdef FEATURE_GSM_COEX_FW_CXM
#ifdef FEATURE_GSM_QSC_TX_DIV
    if (passive_sub_switch_notification [gas_id])
      {
 //     gsmTxCmd[gas_id].slotInfo[i].cxmActAsDiv      = GFW_CXM_ASDIV_ACT_BLANK;
      }

#endif /*  FEATURE_GSM_QSC_TX_DIV */

      gsmTxCmd[gas_id].slotInfo[i].cxmDesenseId     = params->coex_params[i].coex_desense_id;
      gsmTxCmd[gas_id].slotInfo[i].cxmPriority      = params->coex_params[i].coex_priority;
#else
      gsmTxCmd[gas_id].slotInfo[i].cxmDesenseId     = 0;
      gsmTxCmd[gas_id].slotInfo[i].cxmPriority      = 0;
#endif /* FEATURE_GSM_COEX */

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if (  (  ((channel >= MDSP_PDTCHU_MCS1) && (channel <= MDSP_PDTCHU_MCS9)) ||
               ((channel >= MDSP_PDTCHU_CS1 && (channel <= MDSP_PDTCHU_CS4) ))
             )
           &&
           ((params->high_prio_ctrl[i] == FALSE) && ( params->burst_num == 3 ) )
         )
      {
        gsmTxCmd[gas_id].slotInfo[i].prioritySchedule  = 1;
      }
      else if ((channel == MDSP_PDTCHU_CS1) && params->high_prio_ctrl[i] )
      {
        gsmTxCmd[gas_id].slotInfo[i].prioritySchedule  = 1;
      }
      else if (channel == MDSP_PACCHU)
      {
        gsmTxCmd[gas_id].slotInfo[i].prioritySchedule  = 1;
      }
      else if (channel == MDSP_PTCCHU)
      {
        gsmTxCmd[gas_id].slotInfo[i].prioritySchedule  = 1;
      }
      else
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER */
      {
        gsmTxCmd[gas_id].slotInfo[i].prioritySchedule  = 0;
      }

#ifdef FEATURE_GSM_GPRS_READ_CCCH_IN_XFER
      if (gsmTxCmd[gas_id].slotInfo[i].prioritySchedule)
      {
         MSG_GERAN_MED_4_G("CXM : GMDSP TX slot %d coex_priority =%d , coex_desense_id=%u prioritySchedule %d",
                         i,gsmTxCmd[gas_id].slotInfo[i].cxmPriority, gsmTxCmd[gas_id].slotInfo[i].cxmDesenseId,gsmTxCmd[gas_id].slotInfo[i].prioritySchedule);
      }
#endif /* FEATURE_GSM_GPRS_READ_CCCH_IN_XFER_DEBUG */

      gsmTxCmd[gas_id].slotInfo[i].arfcn = arfcn.num;
#ifdef FEATURE_GSM_COEX
      gsmTxCmd[gas_id].slotInfo[i].powerdBm = L1_convert_power_idx_to_dBm(arfcn.band, pa_index[i]);
#endif
#ifdef FEATURE_VAMOS
        if(gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_CS1 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_CS2 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_CS3 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_CS4 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PACCHU ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PACCHU_EXT ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PTCCHU ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PTCCHU_EXT ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS1 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS2 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS3 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS4 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS5 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS6 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS7 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS8 ||
           gsmTxCmd[gas_id].msgInfo[i].controlField.channelType == MDSP_PDTCHU_MCS9)
        {
          gsmTxCmd[gas_id].msgInfo[i].controlField.vamosTscSet = 0;
        }
        else
        {
          gsmTxCmd[gas_id].msgInfo[i].controlField.vamosTscSet = (params->tsc & 0x8) >> 3 ;
        }

        if(gsmTxCmd[gas_id].msgInfo[i].controlField.vamosTscSet == 1)
        {
          MSG_GERAN_LOW_0_G("at the point of assigning: vamos tsc set 2 is given to FW in tx");
        }
#endif

#ifdef DEBUG_GSM_MDSP_DTM_TX
#error code not present
#endif /* DEBUG_GSM_MDSP_DTM_TX */

   }

   /* Build the buffer header for the mDSP. */
   /* Note: Included are both NB and AB TSC */
   /* and for the AB a 41bit Sync Seq Bits. */

   gsmTxCmd[gas_id].cmd            = GFW_GSM_TX_CMD;
   gsmTxCmd[gas_id].controlField.bsic = mdsp_intf_data_ptr->mdsp_BSIC;
#ifdef FEATURE_VAMOS
   gsmTxCmd[gas_id].controlField.nbTsc = params->tsc & 0x7;
#else
   gsmTxCmd[gas_id].controlField.nbTsc = params->tsc;
#endif
   gsmTxCmd[gas_id].controlField.abTsc = params->ab_tsc;
   gsmTxCmd[gas_id].controlField.useRotator = mdsp_intf_data_ptr->mdsp_tx_rotator;

#ifdef FEATURE_GSM_COEX_FW_CXM
   /* Inform fw if we are not in G+G operation */
   gsmTxCmd[gas_id].ratmode = !garb_is_g_plus_g();
#endif

#ifdef FEATURE_GSM_MDSP_EGPRS
   gsmTxCmd[gas_id].controlField.loopBack = params->loopback;
#endif /* FEATURE_GSM_MDSP_EGPRS */

   gsmTxCmd[gas_id].rfBufIndex     = mdsp_current_buffer_mod3( gas_id );
   gsmTxCmd[gas_id].f_hz_q6        = params->f_hz_q6;
   gsmTxCmd[gas_id].inv_f_dl_lo    = params->inv_f_dl_lo;
   gsmTxCmd[gas_id].txlmBufIndex   = get_private_dataspace_ptr( gas_id)->mdsp_gsm_txlm_buf_idx;
   get_private_dataspace_ptr( gas_id)->mdsp_gsm_txlm_buf_idx = MDSP_TXLM_BUF_IDX_INVALID; /*Set index invalid after using, so only set for updates */
   gsmTxCmd[gas_id].num_slots      = params->num_slots;
   gsmTxCmd[gas_id].schedulerTxResultBufIndex = mdsp_current_buffer_mod3( gas_id );

   gsmTxCmd[gas_id].ppm            = gl1_hw_get_xo_acc_freq_err( gas_id );

   gsmTxCmd[gas_id].rfSeqNr        = rf_seq_num;

#ifdef FEATURE_GERAN_RF_SELF_TEST 
   if(params->selftest_flag_to_issue_TxCmd)
   {
     gsmTxCmd[gas_id].selfTest = mdsp_intf_data_ptr->gsm_selftest_params.selftest_flag;
     gsmTxCmd[gas_id].fbRxlmBufIndex = mdsp_intf_data_ptr->gsm_selftest_params.fbRxlBufIndex;
     gsmTxCmd[gas_id].selfTestCmdParam = &(mdsp_intf_data_ptr->gsm_selftest_params.selfTestCmdParam);
     gsmTxCmd[gas_id].selfTestconfig = &(mdsp_intf_data_ptr->gsm_selftest_params.selfTestconfig);
     MSG_GERAN_HIGH_4_G("selfTest_enable = %d, fbRxlmBufIndex = %d, selfTestCmdParam = %x, selfTestconfig = %x",
     	 gsmTxCmd[gas_id].selfTest,
     	 gsmTxCmd[gas_id].fbRxlmBufIndex,
     	 gsmTxCmd[gas_id].selfTestCmdParam,
     	 gsmTxCmd[gas_id].selfTestconfig);
   }
   else
   {
     gsmTxCmd[gas_id].selfTest = FALSE;
   }
#endif /*FEATURE_GERAN_RF_SELF_TEST*/
#ifdef FEATURE_VSWR
   if ( !rach_burst)
   {
     gsmTxCmd[gas_id].extendedTxAlpha = gl_hw_use_extended_tx_alpha[gas_id]; 
   }
   else
   {
     gsmTxCmd[gas_id].extendedTxAlpha = FALSE; 
   }
#endif /*FEATURE_VSWR*/
   #ifdef DEBUG_GSM_MDSP_DTM_TX
      #error code not present
#endif

#if defined(FEATURE_GSM_MDSP_EGPRS)
   /* sanity check dataBuffer pointer is not NULL */
   for (i = 0; i < params->num_slots; i++)
   {
      if (gsmTxCmd[gas_id].msgInfo[i].dataBuffer == NULL)
      {
         abort_srbl_tx = TRUE;
      }
   }

   /* if SRB loopBack bit is set and any of the dataBuffer pointers is set to NULL, abort Tx and generate non-fatal error */
   if((gsmTxCmd[gas_id].controlField.loopBack == TRUE) && (abort_srbl_tx == TRUE))
   {
      MSG_GERAN_ERROR_0_G("Null dataBuffer ptr for SRBL data block - aborting Tx cmd");
   }
   else
#endif /*defined(FEATURE_GSM_MDSP_EGPRS)*/
   {
      mdsp_issue_qdsp6_command( (GfwCmdBufType *)&gsmTxCmd[gas_id],
                                 GFW_GSM_TX_CMD,
                                 gas_id
                              );
   }
}




/*===========================================================================

FUNCTION mdsp_dtm_get_tx_schedule

DESCRIPTION
   Determine if the message associated with the given tag was transmitted
   or not.  Will clear out the associated data buffer at the same time
   if the message was transmitted or the discard_untxed flag was set.

===========================================================================*/
boolean mdsp_dtm_get_tx_schedule
(
   uint16 tag,
   boolean discard_untxed
   , gas_id_t gas_id
)
{
   boolean msg_txed, msg_aborted = FALSE;

   mdsp_dtm_private_data_t* private_data_ptr   = get_private_dataspace_ptr( gas_id);
   mdsp_intf_public_data_t* mdsp_intf_data_ptr = get_mdsp_intf_public_dataspace_ptr( gas_id);


   /* msg will indicate if a msg was queued for tx */
   if (tag >= MDSP_DTM_NUM_TX_CTL_BUFFERS || private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag)
   {
      MSG_GERAN_ERROR_1_G("Scheduled request for invalid tag %d", tag);
      return FALSE;
   }

   /* Check we are in a valid state to check the schedule, but try anyway */
   if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state != TX_DATA_STATE_DONE)
   {
      MSG_GERAN_ERROR_2_G("Bad schedule request timing (state=%d,tag=%d)",
                private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state,tag);
   }

   /* Determine if the message was transmitted */
   msg_txed = (mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_buffer].egprs.len == 0 ? TRUE : FALSE);

   /* Drop untransmitted messages if requested */
   if (!msg_txed && discard_untxed)
   {
      mdsp_intf_data_ptr->pMdspHostIfBaseStore->gfwDtmTxBuffers[private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_buffer].egprs.len = 0;
      msg_aborted = TRUE;
   }

   /* If the message was transmitted or aborted the buffer is now free */
   if (msg_txed || msg_aborted)
   {
      /* We are done with this tx data buffer */
      private_data_ptr->mdsp_tx_buffer_free[private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_buffer] = TRUE;
      private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state = TX_DATA_STATE_CLEARED;
   }
   else
   {
      /* Otherwise the data is ready for re-encoding */
      private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state = TX_DATA_STATE_READY;
   }

   return msg_txed;
}



/*===========================================================================

FUNCTION mdsp_gprs_convert_usf_to_pdch

DESCRIPTION
   Converts the message associated with the given tag from dynamic to
   fixed allocation.

===========================================================================*/
void mdsp_gprs_convert_usf_to_pdch
(
   uint16 tag,
   mdsp_timeslot_type pdch
   , gas_id_t gas_id
)
{
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* Check the request makes sense */
   if (tag >= MDSP_DTM_NUM_TX_CTL_BUFFERS)
   {
      MSG_GERAN_ERROR_1_G("Invalid usf to pdch conversion tag %d",tag);
      return;
   }

   if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag ||
       private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type != MDSP_USF_SPECIFIC ||
       private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state != TX_DATA_STATE_READY)
   {
      MSG_GERAN_ERROR_3_G("Bad USF to PDCH conversion (state=%d, free=%d, type=%d)",
                private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state,
                private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag,
                private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type);
      return;
   }

   /* Mark this message as fixed alloc */
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].msg_type = MDSP_PDCH_SPECIFIC;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].control_field.PDCH = pdch;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].control_field.M = 1;
}



/*===========================================================================

FUNCTION mdsp_dtm_abort_tx

DESCRIPTION
  Aborts untransmitted messages

===========================================================================*/
boolean mdsp_dtm_abort_tx
(
   uint16 tag , gas_id_t gas_id
)
{
   uint16 data_buffer;
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   if (tag >= MDSP_DTM_NUM_TX_CTL_BUFFERS || private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag)
   {
      MSG_GERAN_ERROR_1_G("Invalid tx abort tag=%d",tag);
      return FALSE;
   }

   /* Abort the transmit and clean up */
   data_buffer = private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_buffer;
   private_data_ptr->mdsp_tx_buffer_free[data_buffer] = TRUE;
   private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag = TRUE;

   return TRUE;
}



/*===========================================================================

FUNCTION mdsp_dtm_abort_rx

DESCRIPTION
  Aborts undecoded messages

===========================================================================*/
void mdsp_dtm_abort_rx
(
   uint16 data_tag,
   gas_id_t gas_id
)
{
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* Sanity check */
   if (data_tag >= MDSP_DTM_NUM_RX_DATA_BUFFERS ||
       private_data_ptr->mdsp_dtm_rx_data_buffer[data_tag].free_flag)
   {
      MSG_GERAN_ERROR_1("Invalid rx abort tag=%d",data_tag);
      return;
   }

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
   /* The decode may not finish for a few frames, so we need to set a
      countdown timer to prevent early inactivation of DM */
   if (private_data_ptr->mdsp_dtm_rx_data_buffer[data_tag].channel_type == MDSP_PDTCHD)
   {
      mdsp_dm_pdtch_decode_done(FALSE);
   }
#endif

   /* Just need to mark the buffer as free */
   private_data_ptr->mdsp_dtm_rx_data_buffer[data_tag].free_flag = TRUE;
}



/*===========================================================================

FUNCTION mdsp_dtm_reset_rx

DESCRIPTION
  Resets the receive chain in case of aborts

===========================================================================*/
void mdsp_dtm_reset_rx (gas_id_t gas_id)
{
   uint16 i;
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* Mark all RX buffers as free */
   for (i = 0; i < MDSP_DTM_NUM_RX_DATA_BUFFERS; i++)
   {
#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
      /* If DM is enabled we can disable it once all aborted receives are
         finished in the firmware */
      if (!private_data_ptr->mdsp_dtm_rx_data_buffer[i].free_flag &&
          private_data_ptr->mdsp_dtm_rx_data_buffer[i].channel_type == MDSP_PDTCHD)
      {
         mdsp_dm_pdtch_decode_done(FALSE , gas_id);
      }
#endif

      private_data_ptr->mdsp_dtm_rx_data_buffer[i].free_flag = TRUE;
   }
}

/*===========================================================================

FUNCTION mdsp_dtm_reset_rx_cs_only

DESCRIPTION
  Resets the cs receive chain in case of aborts

===========================================================================*/
#ifdef  FEATURE_GSM_DTM
#ifdef  FEATURE_GSM_EDTM

void mdsp_dtm_reset_rx_cs_only (gas_id_t gas_id)
{
   uint32 i;
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* Mark all RX buffers as free */
   for (i = 0; i < MDSP_DTM_NUM_RX_DATA_BUFFERS; i++)
   {
     switch( private_data_ptr->mdsp_dtm_rx_data_buffer[i].channel_type )
     {
       case MDSP_PDTCHD:
       case MDSP_PTCCHD:

         break;

       default:
         private_data_ptr->mdsp_dtm_rx_data_buffer[i].free_flag = TRUE;
         break;
     }
   }
}

#endif
#endif

/*===========================================================================

FUNCTION mdsp_dtm_reset_tx

DESCRIPTION
  Resets the transmit chain variables

===========================================================================*/
void mdsp_dtm_reset_tx ( gas_id_t gas_id )
{
   uint16 i;
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* Mark all transmit data and control buffers as free */
   for (i = 0; i < MDSP_DTM_NUM_TX_DATA_BUFFERS; i++)
   {
      private_data_ptr->mdsp_tx_buffer_free[i] = TRUE;
   }
   for (i = 0; i < MDSP_DTM_NUM_TX_CTL_BUFFERS; i++)
   {
      private_data_ptr->mdsp_dtm_tx_ctl_buffer[i].free_flag = TRUE;
   }
}



/*===========================================================================

FUNCTION mdsp_dtm_process_frame_tick

DESCRIPTION
  This function performs any GPRS frame tick processing required.

===========================================================================*/
void mdsp_dtm_process_frame_tick ( gas_id_t gas_id )
{
   uint16 tag;
   mdsp_dtm_private_data_t* private_data_ptr = get_private_dataspace_ptr( gas_id);

   /* Tick the state of any written tx data */
   for (tag = 0; tag < MDSP_DTM_NUM_TX_CTL_BUFFERS; tag++)
   {
      if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag) continue;

      switch (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state)
      {
         /* Data waiting for encoding */
         case TX_DATA_STATE_READY:
            break;

         /* Data was scheduled and is now being encoded  */
         case TX_DATA_STATE_SCHEDULED:
            private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state = TX_DATA_STATE_ENCODING;
            break;

         /* Encoding is done, scheduled results are now ready */
         case TX_DATA_STATE_ENCODING:
            private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state = TX_DATA_STATE_DONE;
            /* fall through now that data is done */

         /* Data encoding is done, buffer can now be checked or auto-cleared */
         case TX_DATA_STATE_DONE:
            if (!private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].auto_clear) break;

            /* Auto call get_tx_schedule to clear the data */
            if (!mdsp_dtm_get_tx_schedule(tag, TRUE , gas_id ))
            {
               MSG_GERAN_ERROR_2_G("Tx data not sent: chan=%d, FN=%d",
                         private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].channel_type,
                         GSTMR_GET_FN_GERAN(gas_id));
            }
            /* fall through now that data is CLEARED */

         /* If the data is cleared and no bursts remain we can free
            this control structure */
         case TX_DATA_STATE_CLEARED:
            if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].bursts_remaining == 0)
            {
               private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag = TRUE;
            }
            break;

         default:
            /* This should not happen, if it does mark the data as done to
               force a schedule check next frame to ensure any stale data
               is cleared */
            MSG_GERAN_ERROR_2_G("Unexpected tx data state %d, FN=%d",
                      private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state, GSTMR_GET_FN_GERAN(gas_id));
            private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state = TX_DATA_STATE_DONE;
            private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].auto_clear = TRUE;
            break;
      }

#ifdef DEBUG_STALE_BUFFERS
      /* Check the in_use counter - in case someone forgets to clear
         the buffer somehow */
      private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].in_use_counter++;
      if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].in_use_counter >= MAX_TX_IN_USE_COUNTER)
      {
         MSG_5(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "Stale tx buffer (cnt=%d, state=%d, chan=%d, br=%d), FN=%d",
               private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].in_use_counter,
               private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state,
               private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].channel_type,
               private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].bursts_remaining, GSTMR_GET_FN_GERAN(gas_id));
         private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].free_flag = TRUE;
         if (private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_state != TX_DATA_STATE_CLEARED)
         {
            private_data_ptr->mdsp_tx_buffer_free[private_data_ptr->mdsp_dtm_tx_ctl_buffer[tag].data_buffer] = TRUE;
         }
      }
#endif
   }

#ifdef DEBUG_STALE_BUFFERS
   /* Check the in_use counter - in case someone forgets to clear
      the buffer somehow */
   for (tag = 0; tag < MDSP_DTM_NUM_RX_DATA_BUFFERS; tag++)
   {
      if (private_data_ptr->mdsp_dtm_rx_data_buffer[tag].free_flag) continue;

      private_data_ptr->mdsp_dtm_rx_data_buffer[tag].in_use_counter++;
      if (private_data_ptr->mdsp_dtm_rx_data_buffer[tag].in_use_counter >= MAX_RX_IN_USE_COUNTER)
      {
         MSG_4(MSG_SSID_DFLT, MSG_LEGACY_ERROR,
               "Stale rx buffer (cnt=%d, chan=%d, tn=%d), FN=%d",
               private_data_ptr->mdsp_dtm_rx_data_buffer[tag].in_use_counter,
               private_data_ptr->mdsp_dtm_rx_data_buffer[tag].channel_type,
               private_data_ptr->mdsp_dtm_rx_data_buffer[tag].timeslot, GSTMR_GET_FN_GERAN(gas_id));
               private_data_ptr->mdsp_dtm_rx_data_buffer[tag].free_flag = TRUE;

#if defined (FEATURE_GSM_MDSP_IR) && !defined (FEATURE_GSM_QDSP6_FW_HAS_IR_MEM)
         if (private_data_ptr->mdsp_dtm_rx_data_buffer[tag].channel_type == MDSP_PDTCHD)
         {
            mdsp_dm_pdtch_decode_done(TRUE , gas_id);
         }
#endif
      }
   }
#endif
}

/*===========================================================================

FUNCTION mdsp_set_mdsp_gsm_txlm_buf_idx(gas_id_t gas_id )

DESCRIPTION
  This function sets the mDSP frame-tick setup margin.

===========================================================================*/
void mdsp_set_mdsp_gsm_txlm_buf_idx (uint32 buf_idx , gas_id_t gas_id )
{
   get_private_dataspace_ptr( gas_id)->mdsp_gsm_txlm_buf_idx = buf_idx;
}

/*===========================================================================

FUNCTION mdsp_dtm_init

DESCRIPTION
  Performs initialization of static variables for the gprs functions.  Note
  that several pointers are duplicates of mdsp_intf.c but are duplicated here
  rather than make them global variables.

===========================================================================*/
void mdsp_dtm_init ( gas_id_t gas_id )
{
   init_private_data_space(gas_id);

   /* Reset receive variables */
   mdsp_dtm_reset_rx(gas_id);

   /* Reset transmit variables */
   mdsp_dtm_reset_tx( gas_id);
}

#ifdef GERAN_L1_IP2CAL
/*===========================================================================

FUNCTION mdsp_schedule_ip2cal

DESCRIPTION
  Send IP2Cal command to FW

===========================================================================*/
void mdsp_schedule_ip2cal(uint16 Offset,uint8 numIp2CalMeas,uint32 ip2CalStepSizeQS,gas_id_t gas_id)
{
  GfwCmdBufType  cmd;
  memset(&cmd,0,sizeof(GfwCmdBufType));

  /* Fill in the command fields */
  cmd.ip2CalCmd.cmd                          = GFW_IP2_CAL_CMD;
  cmd.ip2CalCmd.timeOffsetQs                 = Offset + mdsp_ftsm(gas_id);
  cmd.ip2CalCmd.ip2CalInfo.rxlmBufIndex      = gl1_hw_get_ip2cal_rxlm_buff_index(gas_id);
  cmd.ip2CalCmd.ip2CalInfo.txlmBufIndex      = gl1_hw_get_ip2cal_txlm_buff_index(gas_id);
  cmd.ip2CalCmd.ip2CalInfo.numIp2CalMeas     = numIp2CalMeas;
  cmd.ip2CalCmd.ip2CalInfo.ip2CalStepSizeQS  = ip2CalStepSizeQS;

  /* Issue the command to the mdsp */
  mdsp_issue_qdsp6_command(&cmd,GFW_IP2_CAL_CMD,gas_id);

  MSG_GERAN_HIGH_3_G("GFW_IP2_CAL_CMD timeOffsetQs:%d,numIp2CalMeas:%d,ip2CalStepSizeQS:%d",cmd.ip2CalCmd.timeOffsetQs,
                     numIp2CalMeas,ip2CalStepSizeQS);
}
#endif  /*GERAN_L1_IP2CAL*/


/*EOF */
