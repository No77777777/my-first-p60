/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     H D R   T R A N S M I T   T A S K

GENERAL DESCRIPTION
  The HDR Transmit task runs the following protocols:
   - Radio Link Protocol (RLP),
   - Signalinng Link Protocol (SLP),
   - Packet Consolidatio Protocol (PCP),
   - Access Channel MAC Protocol (ACMAC),
   - Reverse Traffic Channel MAC Protocol (RTCMAC).
   
EXTERNALIZED FUNCTIONS
  hdrtx_task
    Entry point to the HDR Transmit task.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  The hdrtx_task function contains the HDR Transmit task.  It must be
  specified in a call to rex_def_task and started by the Main Control task.

Copyright (c) 2000 - 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/mac/src/hdrtx.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
03/28/16   wsh     Resets tx power stats on enable/disable. Also added F3
12/14/15   rmv     Fix KW issues from checker ASSERT.WITH.SIDE.EFFECTS 
08/17/15   vlc     Added code to handle Run Control Event Notification errors.
                   Mainlined FEATURE_MODEM_RCINIT.
07/13/15   arm     Mainlined FEATURE_RF_SVDO_API feature.
06/18/15   tnp     Fix compiler warnings
05/21/15   kss     Added support for modem tx power stats.
05/11/15   wsh     Added support for RxTx Split and TRM grouped API
04/27/15   kss     Minor cleanup of wdog API.
02/13/15   mbs     Disable further processing on RF error
02/03/15   mbs     Forced 2HS interval between delivery of 2 FW cmds
01/08/14   sat     Changes to allocate TXLM buffer (RUMI).
01/05/15   sat     Changes to wait for TX_STOP response from FW (RUMI).
11/12/14   wsh     Fixed carrier being disabled after QTA  
10/14/14   wsh     Changes to support Jolokia bringup
09/22/14   arm     HDR QTA code cleanup
09/11/14   vko     Fix compilation error for LTE compilation out flavor
08/06/14   vke     On QTA gap avoid modulator initialization and 
                   setting to Rev0 mode
07/29/14   tnp     FR22375: Fix Medium compiler warnings in HDR
06/05/14   rmv     Delay setting MCPM state to HDRSRCHMCPM_STOP_TX_REQ, until 
                   searcher traffic exit in MCDO case  
06/04/14   arm     Changes to support DO to G QTA
06/25/14   wsh     Fixed compile issue on Jolokia due to missing FW API 
06/09/14   kss     Added support for handling TX_RESET_REQ_IND processing.
05/08/14   wsh     Fixed concurrent RF/ASDiv call that led to RF issue
03/14/14   dsp     FR19338:Reduce call setup time by skipping TX RF shutdown.
12/16/13   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/08/13   mbs     Handled FW TX mesg to SW
09/04/13   wsh     Changes to support Bolt RUMI
08/08/13   rmv     Featurized tmc APIs with ifndef FEATURE_MODEM_RCINIT
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
07/31/13   sat     Mainlining Modem Statistics Feature
07/08/13   rmg     Added ERR_FATAL triggered on RF tx enable failure.
06/26/13   wsh     Added Selective TxD feature
04/22/13   arm     Added changes for DSDA coexistence manager.
03/12/13   smd     Added Triton Tx Fixes.
03/04/13   rmg     Added change to pass channel info in STX_SET_TX_STATE ind.
03/01/13   dsp     Moved watchdog timer code to watchdog heart-beat model.
01/17/13   rmg     Changed HDR_FW_TX_PA_CTL_RSP to HDR_FW_TX_PA_CTL_RSP.
12/19/12   smd     Added RUMI TX support. 
10/25/12   rmg     Added tx power reporting for simultaneous tx support.
08/31/12   rmg     Added support for processing multiple back-to-back FW 
                   responses.
05/22/12   rmg     Removed tx block reset when disabling tx.
04/27/12   dsp     Fixed compiler warnings.
04/26/12   rmg     Made changes in wait_for_msg() to forward the received 
                   messages to appropriate destinations.
04/03/12   cnx     Register MSGR when task is initialized.
03/23/12   arm     Fixed featurization bug for RCINIT. 
03/23/12   smd     Removed use of obsolete AEEstd.h.      
03/20/12   cnx     Resolved TCB compiler errors for Dime. 
03/16/12   arm     Resolve TCB errors for Dime. 
03/05/12   rmg     Added check for msgr client registration check before 
                   deregistering it.
02/03/12   wsh     Added support for SVLTE+SHDR
01/19/12   rmg     Shifted deregistration for msgr messages from exit online
                   processing to task stop.
01/16/12   rmg     Shifted processing of ISR signal before event signal in 
                   the event they are both set.
01/04/12   rmg     Shifted fw msg receiving code from hdrrmac to hdrtx.
                   Added hdrtx_wait_for_msg().
10/20/11   wsh     Changed PCP init code path to avoid double init
10/17/11   rmg     Removed featurization around hdrsrchmcpm calls.
10/10/11   arm     Added RCINIT support.
07/27/11   cnx     Merged Optimized Handoff changes.
07/11/11   smd     Included hdrmrlp_api.h and hdrrlp_api.h. 
06/21/11   rmg     Added hdrtx_task_wait().
                   Added support for commands to be posted to hdrmod.
06/16/11   ssu     Removed inclusion of "hdrrlptxq.h" and "hdrmrlpspd.h". 
06/01/11   kss     Moved data mcpm calls to hdrtx.c from hdrmod.c.
                   Revert temp NikeL bringup changes.
05/03/11   kss     Removed hdrmod_tx_start(), added hdrmod_init() call. 
04/28/11   kss     Added support for hdrmod_tx_start() and hdrmod_tx_stop(). 
04/26/11   kss     Added check for KW. 
04/01/11   kss     Integrate TxLM APIs. 
03/16/11   kss     Updated RF SVDO API support.
12/17/10   kss     Switch to timer api (from clock callbacks).
10/24/10   pxu     Fixed compiler warnings. 
09/16/10   pxu     Memory heap changes.  
09/15/10   ljl     Supported dynamic wdog.
08/24/10   rmg     Added assert around HDRTX free queue full condition.
07/20/10   rmg     Added RMAC message client removal on HDR exit.
06/21/10   cnx     Fixed compiler warnings.
06/15/10   ms/rmg  Added signal for MRLP stats calculation.
12/08/09   kss     Add hdrmod_enter() and hdrmod_exit().
12/08/09   wsh     Fixed dog timeout after TASK_STOP_SIG 
11/11/09   etv     Initialize RMAC MSGR module only in online mode.
10/02/09   lyl     Updated for rumi bringup.
09/17/09   etv     Added support for RMAC0 Q6 interface changes.
06/22/09   etv     Fixed Lint warnings.
04/16/09   rkc     Mainlined T_MSM6500.
03/23/09   wsh     CMI: featurized CMI under FEATURE_CMI
02/10/09   pmk     Clean up the resources before sending task_stop
12/10/08   smd     Replaced assert.h with amssassert.h
10/13/08   kss     Lint fixes.
09/12/08   etv     Cleaned up RF Powering up call sequence.
09/09/08   etv     Added missing break in switch statement.
06/18/08   etv     Process indication for PHY module in hdrtx task context.
01/15/08   vish    Added support for MC RF Rx/Tx AGC APIs.
11/11/07   kss     7800 integration update.
10/25/07   kss     Use new RF API.
07/30/07   cc      Added protocol timer support (for rmac4).
12/21/06   spn     Added a signal for Stale Packet Dropping (SPD)
05/09/06   etv     Fixed lint comment.
08/02/05   kss     Track RF Tx status.
07/08/05   jyw     Added support for IS890A
06/29/05   jyw     Added waiting and processing HDRTX_IS890_RTAP_PKTGEN_SIG.
02/23/05   kss     Added tasklock to hdrtx_disable_tx().
01/25/05   etv     Cleaned lint errors. 
01/12/04   sq      Report Rx AGC & LNA state along with Tx AGC reporting
09/10/03   mpa     Converted F3 messages to use new MSG2.0 HDRPROT SSID
08/29/03   kss     Add support for new message macros.
07/31/03   dyc     Added support for FEATURE_MULTI_RX_CHAIN
07/30/03   kss     Add support for MSM6500.
07/23/02   om      Removed MSG macro for FTAP signal.
07/01/02   kss     Include hdrind.h, handle disable_tx even if the enable 
                   is not yet complete.
06/20/02   kss     Removed NV write/read code as it is not used.
05/06/02   om      Added handling of HDRTX_IS890_FTAP_LO_INIT_SIG signal
04/18/02   kss     Added Tx agc reporting.
01/03/02   kss     Added callback and timer mechanism for hdrtx_enable_tx().
10/08/01   om      Added HDR IS890 signal handling
09/21/01   om      Added indication handling for IS-890
09/17/01   kss     Increased event queue size.
09/05/01   dna     Added separate handling for in-use vs. in_config messages.
08/21/01   kss     Added indication forwarding for RMAC and ACMAC.
07/11/01   kss     Added hdrtx-context NV access and RF TX enable/disable
                   utility functions.
06/15/01   kss     Added hdrtx_subtask, enter/exit HDR signals, and dog
                   support.
06/07/01   kss     Use correct call in hdrtx_queue_ind().
05/07/01   kss     Changed tasks from Tx to Hdrtx.
03/28/01   vas     Removed HDR_ON_REXNT code
05/22/00   om      Created module.

===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "memory.h"
#include "amssassert.h"
#include "task.h"

#ifdef FEATURE_MEMCPY_REMOVAL
#include "stringl.h"
#endif /* FEATURE_MEMCPY_REMOVAL */

#include "hdrdebug.h"
#include "err.h"
#include "dog.h"
#include "dog_hb_rex.h"
#include "dsm.h"
#include "queue.h"
#include "mc.h"
#include "modem_mem.h"
#include "timer.h"

#ifdef FEATURE_CMI
#include "mc_v.h"
#endif

#include "rfm.h"

#include "lm_types.h"
#include "txlm_intf.h"

#include "hdrmod.h"

#include "hdrhai.h"
#include "hdrind.h"
#include "hdrtx.h"
#include "hdrtxi.h"

#include "hdramac.h"
#include "hdramaci.h"
#include "hdrrmac.h"     
#include "hdrpcp.h"
#include "hdrslp.h"
#include "hdrslpi.h"
#include "hdrlmac.h"
#include "hdrlmaci.h"
#include "hdrsrchrf.h"
#include "hdrphy.h"
#include "hdrmod.h"

#ifdef FEATURE_HDR_IS890
  #include "hdrftap.h"
  #include "hdrrtap.h"
#endif

#include "hdrrlp_api.h"
#include "hdrmrlp_api.h"
#include "hdrsrchmcpm.h"

#include "hdrmsg.h"
#include "msgr.h"
#include "msgr_rex.h"
#ifdef FEATURE_LTE_TO_HDR_OH
#include "hdrsaprx.h"
#include "hdrrxi.h"
#endif /* FEATURE_LTE_TO_HDR_OH */

#include "rcinit.h"
#define HDRTX_RCEVT_PREFIX "HDRTX:"
#define HDRTX_RCEVT_READY HDRTX_RCEVT_PREFIX "ready"
#define HDRMC_RCEVT_PREFIX "HDRMC:"
#define HDRMC_RCEVT_READY HDRMC_RCEVT_PREFIX "ready"

#include "cxm.h" /* CoeXistance Manager: for DSDA support. */

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchmac.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#include "hdrts.h"

/* Make sure header is available */
#if defined(__has_include)
#if __has_include("ds3g_ext.h")
#include "ds3g_ext.h"   /* For Modem Tx Power Stats */
#else
#define DS3G_TX_POWER_BIN_MAX 5
#endif
#else
#include "ds3g_ext.h"   /* For Modem Tx Power Stats */
#endif

/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
#ifdef TEST_FRAMEWORK
#error code not present
#endif /* TEST_FRAMEWORK */

#ifdef FEATURE_HDR_RUMI
volatile uint32 hdrtx_tx_start_rsp_rcved = 0;
volatile uint32 hdrtx_tx_stop_rsp_rcved = 0;
errno_enum_type estatus = 0;
#endif /* FEATURE_HDR_RUMI */

/*---------------------------------------------------------------------------
 Task timers and watchdog reporting.
---------------------------------------------------------------------------*/

rex_tcb_type*    hdrtx_tcb_ptr;

/* Local task watchdog timer. */
LOCAL rex_timer_type hdrtx_rpt_timer;

/* Watchdog reporting macros. */
#undef  DOG_HDRTX_RPT
#define DOG_HDRTX_RPT hdrtx_dog_rpt_var
dog_report_type   hdrtx_dog_rpt_var = 0;
  /* Dog rpt */

/* Tx power report timer */
LOCAL rex_timer_type hdrtx_pwr_report_timer;

LOCAL boolean hdrtx_power_reporting_enabled = FALSE;
                                    /* TRUE if power reporting is enabled */

#define HDRTX_PWR_REPORT_TIMER_MSECS      50 /* Power report timer period */

/* Timer ids */
#define HDRTX_TIMER_ID_PWR_REPORT          1 /* Power report timer id     */


/*---------------------------------------------------------------------------
 Task event handling.
---------------------------------------------------------------------------*/

/* Queue of pending commands, indications, and messages. */

q_type hdrtx_event_q;

/* Buffer type definition. */

typedef struct
{
  hdrbuf_hdr_type hdr;                       /* Header for this item       */
  union
  {
    hdrtx_protocol_cmd_union_type  cmd;      /* Cmds for prot's in tx task */
    hdrbuf_ind_type                ind;      /* All ind's have this type   */
    dsm_item_type                 *item_ptr; /* Msg is a DSM item (chain)  */
    uint32                         timer_id; /* Timer ID */
  } item;
} hdrtx_buf_type;

/* Number of buffers available. */

#define HDRTX_EVENT_BUFS_NUM 8 

/* For Tx agc report */
timer_type hdrtx_agc_cb;

/* hdrtx type definition */
typedef struct
{
  msgr_client_t msgr_client_id;
    /**< Messenger client ID */

  hdrtx_msg_s_type q_items[ HDRTX_MSG_QUEUE_SIZE ];  
    /**< Pool of queue items */

  q_type msg_q;  
    /**< Message queue */

  q_type free_q;
    /**< Free queue */

  boolean msgr_client_registered;
    /**< Messenger client registered with message router */

  qword  prev_rmac_e_c_slot;
    /**< Slot number when previous RMAC_ENABLE_CARRIER cmd was sent to FW */

  rfm_device_enum_type rx_dev;
  rfm_device_enum_type tx_dev;
    /* Rx and Tx device ID used for wakeup */

  rex_crit_sect_type modem_tx_power_crit_sect; 
    /* Critical Section specifically for modem tx power stats */ 

} hdrtx_type;

LOCAL hdrtx_type hdrtx;

/* Block of responses/indications expected to be received via msg router */
msgr_umid_type hdrtx_rcv_msgs[] =
{
  HDR_FW_TX_PA_CTL_RSP,            /* PA ON control response */
  HDR_FW_RMAC0_FRAME_IND           /* Rel 0 RMAC frame indication */

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
  ,HDR_FW_FLEXCONN_REPOINT_IND
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX) */

#ifdef FEATURE_LTE_TO_HDR_OH
#ifdef FEATURE_IRAT_DSM_SUPPORT
  ,NAS_EMM_IRAT_HDR_UL_MSG_CNF,
  NAS_EMM_IRAT_HDR_FAILURE_IND
#else
  ,NAS_EMM_IRAT_UL_MSG_CNF,
  NAS_EMM_IRAT_FAILURE_IND
#endif /* FEATURE_IRAT_DSM_SUPPORT */
#endif /*FEATURE_LTE_TO_HDR_OH */

#if defined(FEATURE_HDR_BOLT_MODEM) && (!defined(FEATURE_HDR_UNIFIED_TX))
  ,HDR_FW_TX_DAC_START_RSP,
  HDR_FW_TX_DAC_STOP_RSP,
  HDR_FW_TX_RESET_REQ_IND
#endif /* FEATURE_HDR_BOLT_MODEM && !FEATURE_HDR_UNIFIED_TX */
};

/* Indicate Tx power-up status. */
static hdrtx_rf_status_enum_type hdrtx_rf_status = TX_OFF;

/* TxLM handle for RF */
static lm_handle_type hdrtx_txlm_handle;
static boolean        hdrtx_txlm_handle_valid = FALSE;

#define HDTTX_NUM_MODEM_TX_POWER_BINS 5
  /* Number of bins to use for Tx power log */

/* --- Modem Tx Power Stats Logging --- */
struct
{
  boolean enabled;
    /* Track whether Tx power duration logging is enabled. */

  uint32 tx_power_bins[HDTTX_NUM_MODEM_TX_POWER_BINS];
    /* Array for binning of durations by power level. */

  uint32 tx_overall;
    /* For debug/internal use, tracks total tx duration.  Should be sum
       of all bins */

} hdrtx_tx_power_duration_logging;

/* Bucket thresholds for binning.
   Bin index 0 holds values below thresh[0]
   Bin index 1 holds values between thresh[0] and thresh[1] etc
   Value are in whole dB units.
   Last value thresh[4] is purposely above possible values to capture > thresh[3]
    condition.
*/
int hdrtx_tx_power_bin_thresh[HDTTX_NUM_MODEM_TX_POWER_BINS] = 
  {0, 8, 15, 20, 100};

/*---------------------------------------------------------------------------
 Forward declaration of functions.
---------------------------------------------------------------------------*/

/* Message router interaction functions */
void hdrtx_msg_init ( void );
void hdrtx_msg_stop ( void );
void hdrtx_process_msgs ( void );

/* Timer functions */
void hdrtx_process_timer( uint32 timer_id );
void hdrtx_timer_cb( uint32 timer_id );

/* Functions for tx power reporting */
void hdrtx_enable_power_reporting( void );
void hdrtx_disable_power_reporting( void );

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_INIT

DESCRIPTION
  This procedure does any task specific initialization.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_init( void )
{
  timer_def2(&hdrtx_agc_cb, NULL);

  /* Initialize hdrtx critical section*/
  rex_init_crit_sect( &hdrtx.modem_tx_power_crit_sect );

  /* Init power logging flag */
  hdrtx_tx_power_duration_logging.enabled = FALSE;

  /* Initialize queues */
  ( void ) q_init( &hdrtx_event_q );

#ifndef FEATURE_RUMI3_BRINGUP
  #ifdef FEATURE_HDR_QOS
  /* Initialize the SPD */
  hdrmrlpspd_init();
  #endif
  
  /*Initialize RLP Txq module*/
  hdrrlptxq_timer_init();
#endif /* FEATURE_RUMI3_BRINGUP */

  /* Initialize PHY module */
  hdrphy_init();

  /* Initialize msgr client registration status flag */
  hdrtx.msgr_client_registered = FALSE;

  /* Initialize time to 0 as there is no valid time from
     hdrts module at this time. This is always less than
     the current time this is compared against */
  qw_set ( hdrtx.prev_rmac_e_c_slot, 0, 0 );
}/* hdrtx_init */


/*===========================================================================

FUNCTION HDRTX_PROCESS_WDOG

DESCRIPTION
  This function reports wdog and start the wdog timer.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_process_wdog( void )
{
  (void) rex_clr_sigs( HDRTX_TASK_PTR,  HDRTX_RPT_TIMER_SIG );
  dog_hb_report(DOG_HDRTX_RPT); 
} /* hdrtx_process_wdog */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_QUEUE_CMD

DESCRIPTION
  This function queues a command on the hdrtx_event_q and sets the signal.
  The command data is copied into the command buffer before it is queued.  
  This function is called in the task context of whoever is giving the command.

DEPENDENCIES
  This routine must be multi-thread safe, as two different tasks may give
  commands to protocols in the TX task at the same time.

  cmd_size must not be larger than hdrtx_protocol_cmd_union_type, because 
  this is the data type the command gets copied into.

PARAMETERS
  protocol_name - Who the command is for
  input_cmd_ptr - Pointer to a union of all protocols in HDRTX subtask
  cmd_size - Size of the command being given

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_queue_cmd
( 
  hdrhai_protocol_name_enum_type  protocol_name,
  void                        *input_cmd_ptr, 
  uint32                       cmd_size 
)
{
  hdrtx_buf_type              *buf_ptr;         /* buffer for command */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT_1 (MSG_LEGACY_LOW, "hdrtx_queue_cmd (%d)", protocol_name);
             

  if ( input_cmd_ptr == NULL )
  {
    ERR( "No command to queue!", 0, 0, 0 );
    return;
  }

  ASSERT (cmd_size <= sizeof(hdrtx_protocol_cmd_union_type) );

  /* Get a buffer from the heap */
  buf_ptr = ( hdrtx_buf_type * ) modem_mem_alloc( 
    sizeof( hdrtx_buf_type ), MODEM_MEM_CLIENT_HDR_CRIT );

  /* Error catching: HDRTX free queue should not be out of buffers */
  if( buf_ptr == NULL )
  {
    ERR_FATAL("hdrtx_queue_cmd: Failed to get buffer from heap", 0, 0, 0);
  }

  /* Fill in the buffer.  Make sure all fields are filled in. */
  buf_ptr->hdr.protocol_name = protocol_name;
  buf_ptr->hdr.type_of_item = HDRBUF_COMMAND;

#ifdef FEATURE_MEMCPY_REMOVAL
  (void) memscpy( &buf_ptr->item.cmd, sizeof(buf_ptr->item.cmd), 
                  input_cmd_ptr, cmd_size );
#else
  (void) memcpy( &buf_ptr->item.cmd, input_cmd_ptr, cmd_size );
#endif /* FEATURE_MEMCPY_REMOVAL */

  /* Queue the buffer and set a signal */
  ( void ) q_link( buf_ptr, &buf_ptr->hdr.link );
  q_put( &hdrtx_event_q, &buf_ptr->hdr.link );
  ( void ) rex_set_sigs( HDRTX_TASK_PTR, HDRTX_EVENT_Q_SIG );

}/* hdrtx_queue_cmd */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_QUEUE_IND

DESCRIPTION
  This function queues an indication on the hdrtx_event_q and sets the 
  signal.  If there is any data associated with the indication, it is copied 
  into the buffer before it is queued.  This function is called in the task 
  context of whoever is giving the indication.

DEPENDENCIES
  This routine must be multi-thread safe, as two different tasks may give
  an indication to protocols in the TX task at the same time.

  ind_data_size must not be larger than hdrind_ind_data_union_type, because 
  this is the data type the command gets copied into.

PARAMETERS
  protocol_name - Who the indication is for
  ind_name - Global enumeration of indications
  input_ind_ptr - Pointer to the indication data
  ind_data_size - The size of the data associated with this indication.
                  This value is undefined if the input_ind_ptr
RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_queue_ind
( 
  hdrhai_protocol_name_enum_type  protocol_name,
  hdrind_ind_name_enum_type       ind_name,
  void                        *input_ind_data_ptr
)
{
  hdrtx_buf_type *buf_ptr;                 /* Buffer for indication */
  uint32          ind_data_size;           /* Size of data with indication */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT_3 (MSG_LEGACY_LOW,  "hdrtx_queue_ind (%d, %d, %lx)",
                 protocol_name, ind_name, input_ind_data_ptr );

  /* Get a buffer from the heap */
  buf_ptr = ( hdrtx_buf_type * ) modem_mem_alloc( 
    sizeof( hdrtx_buf_type ), MODEM_MEM_CLIENT_HDR_CRIT );

  /* Error catching: HDRTX free queue should not be out of buffers */
  if( buf_ptr == NULL )
  {
    ERR_FATAL("hdrtx_queue_ind: Failed to get buffer from heap", 0, 0, 0);
  }

  /* Fill in the buffer.  Make sure all fields are filled in. */
  buf_ptr->hdr.protocol_name = protocol_name;
  buf_ptr->hdr.type_of_item = HDRBUF_INDICATION;
  buf_ptr->item.ind.ind_name = ind_name;
  if ( input_ind_data_ptr != NULL )
  {
    ind_data_size = hdrind_get_ind_data_size (ind_name);
    ASSERT (ind_data_size <= sizeof(hdrind_ind_data_union_type) );
#ifdef FEATURE_MEMCPY_REMOVAL
    (void) memscpy( &buf_ptr->item.ind.ind_data, 
                     sizeof(buf_ptr->item.ind.ind_data), input_ind_data_ptr, 
                     ind_data_size );
#else
    (void) memcpy( &buf_ptr->item.ind.ind_data, input_ind_data_ptr, 
                   ind_data_size );
#endif /* FEATURE_MEMCPY_REMOVAL */
  }

  /* Queue the buffer and set a signal */
  ( void ) q_link( buf_ptr, &buf_ptr->hdr.link );
  q_put( &hdrtx_event_q, &buf_ptr->hdr.link );
  ( void ) rex_set_sigs( HDRTX_TASK_PTR, HDRTX_EVENT_Q_SIG );

}/* hdrtx_queue_ind */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_QUEUE_MSG

DESCRIPTION
  This function queues a message on the hdrtx_event_q and sets the signal.
  The queued buffer simply holds a pointer to the DSM item chain that is
  the message.  This function is called in the task context of whoever is 
  giving the message, which is always the task in which SLP runs.

DEPENDENCIES
  None

PARAMETERS
  protocol_name - Who the message is for
  protocol_instance - Instance of the protocol the message is for
  item_ptr - Pointer to the DSM item chain holding the message

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_queue_msg
( 
  hdrhai_protocol_name_enum_type  protocol_name,
  hdrhai_protocol_instance_enum_type protocol_instance,
  dsm_item_type               *item_ptr 
)
{
   hdrtx_buf_type *buf_ptr;        /* Buffer for msg (w/ptr to DSM item */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT_2 (MSG_LEGACY_LOW,  "hdrtx_queue_msg (%d, %lx)", 
                 protocol_name, item_ptr);

  /* Get a buffer from the heap */
  buf_ptr = ( hdrtx_buf_type * ) modem_mem_alloc( 
    sizeof( hdrtx_buf_type ), MODEM_MEM_CLIENT_HDR_CRIT );

  /* Error catching: HDRTX free queue should not be out of buffers */
  if( buf_ptr == NULL )
  {
    ERR_FATAL("hdrtx_queue_msg: Failed to get buffer from heap", 0, 0, 0);
  }

  /* Fill in the buffer.  Make sure all fields are filled in. */
  buf_ptr->hdr.protocol_name = protocol_name;
  buf_ptr->hdr.protocol_instance = protocol_instance;
  buf_ptr->hdr.type_of_item = HDRBUF_MESSAGE;
  buf_ptr->item.item_ptr = item_ptr;

  /* Queue the buffer and set a signal */
  ( void ) q_link( buf_ptr, &buf_ptr->hdr.link );
  q_put( &hdrtx_event_q, &buf_ptr->hdr.link );
  ( void ) rex_set_sigs( HDRTX_TASK_PTR, HDRTX_EVENT_Q_SIG );

}/* hdrtx_queue_msg */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_QUEUE_TIMER

DESCRIPTION
  This function queues a timer event on the hdrtx_event_q and sets the
  signal.

DEPENDENCIES
  None

PARAMETERS
  protocol_name - Who the timer is for
  timer_id - The ID of the timer that has expired

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_queue_timer
(
  hdrhai_protocol_name_enum_type protocol_name,
  uint32 timer_id
)
{
  hdrtx_buf_type *buf_ptr;        /* Buffer for timer */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 HDR_MSG_PROT_2 (MSG_LEGACY_LOW,  "hdrtx_queue_timer (%d, %lx)", 
                protocol_name, timer_id);

 /* Get a buffer from the heap */
 buf_ptr = ( hdrtx_buf_type * ) modem_mem_alloc( 
   sizeof( hdrtx_buf_type ), MODEM_MEM_CLIENT_HDR_CRIT );

 /* Error catching: HDRTX free queue should not be out of buffers */
 if( buf_ptr == NULL )
 {
   ERR_FATAL("hdrtx_queue_timer: Failed to get buffer from heap", 0, 0, 0);
 }

 /* Fill in the buffer.  Make sure all fields are filled in. */
 buf_ptr->hdr.protocol_name = protocol_name;
 buf_ptr->hdr.type_of_item  = HDRBUF_TIMER;
 buf_ptr->item.timer_id     = timer_id;

 /* Queue the buffer and set a signal */
 ( void ) q_link( buf_ptr, &buf_ptr->hdr.link );
 q_put( &hdrtx_event_q, &buf_ptr->hdr.link );
 ( void ) rex_set_sigs( HDRTX_TASK_PTR, HDRTX_EVENT_Q_SIG );

} /* hdrtx_queue_timer */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_WAIT

DESCRIPTION
  This procedure waits for specified signals to be set.  This routine also
  checks any queues that are being waited on.  If the queue has an item, 
  this routine sets the signal without waiting.  This allows for the queue
  signal to be cleared after each item is removed.  If a queue signal is set,
  this routine checks the queue and clears the signal if there is nothing
  on it.  This centralizes the queue checking.

DEPENDENCIES
  None.

RETURN VALUE
  For non-queue signals, a signal bit is set in the return mask if the signal 
  was in the requested mask and is also set in the REX TCB.
  
  For queue signals, a signal bit is set in the return mask if the signal was 
  in the requested mask and the queue associated with the signal has an item
  on it.

SIDE EFFECTS
  None.

===========================================================================*/
rex_sigs_type hdrtx_wait
(
  rex_sigs_type  requested_mask         /* Mask of REX signals to wait for */
)
{
  rex_sigs_type  rex_signals_mask; /* Signal mask returned by rex_get_sigs */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Loop until at least one requested signal is set
  -------------------------------------------------------------------------*/
  do {

    /* Actually wait for one of the requested signals to be set */
    rex_signals_mask = rex_wait( requested_mask | HDRTX_RPT_TIMER_SIG);

    /*-----------------------------------------------------------------------
     Watchdog report timer signal. Kick watchdog and reset timer.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRTX_RPT_TIMER_SIG) != 0 ) 
    {
      hdrtx_process_wdog();
    }

  } while ( (requested_mask & rex_signals_mask) == 0 );

  /*-------------------------------------------------------------------------
   Kick watchdog one last time on the way out.
  -------------------------------------------------------------------------*/
  (void) rex_clr_sigs( HDRTX_TASK_PTR,  HDRTX_RPT_TIMER_SIG );
  dog_hb_report( DOG_HDRTX_RPT );

  /*-------------------------------------------------------------------------
   Assemble return mask.  Only return signals that were requested.
  -------------------------------------------------------------------------*/
  return ( rex_signals_mask & requested_mask );

}/* hdrtx_wait */



/*===========================================================================

FUNCTION HDRTX_REPORT_AGC

DESCRIPTION
  This function is a clock callback that reports Tx agc values periodically. 
  No more, no less.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_report_agc
( 
  int32 ms 
  /*lint -esym(715, ms)
   * Ignore argument(ms) not used lint error #715 */
)
{
  int32 tx_adjust, pilot_power, total_power;

  rfm_device_enum_type rf_dev;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
   rf_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN);

  if (hdrmod_pa_is_on())
  {

    tx_adjust   = rfm_hdr_get_tx_adjust(rf_dev,
                                        RFM_HDR_CARRIER_ID__0,
                                        RFM_HDR_AGC_FORMAT__DBM256);

    pilot_power = rfm_hdr_get_tx_pilot_power(rf_dev, 
                                             RFM_HDR_CARRIER_ID__0,
                                             RFM_HDR_AGC_FORMAT__DBM256);

    total_power = rfm_hdr_get_tx_total_power(rf_dev, 
                                             RFM_HDR_CARRIER_ID__0,
                                             RFM_HDR_AGC_FORMAT__DBM256);

    tx_adjust   += (tx_adjust<0)?-128:128;
    pilot_power += (pilot_power<0)?-128:128;
    total_power += (total_power<0)?-128:128;
 
    HDR_MSG_PROT_3 (MSG_LEGACY_MED, 
                 "Tx Pilot=%d, Total=%d (dBm) Adjust=%d",
                 pilot_power/256, total_power/256, tx_adjust/256);

    hdrsrchrf_report_rx_power( );
      /* Report Rx AGC and LNA state as well  */
  }

}


/*===========================================================================

FUNCTION HDRTX_ENABLE_TX

DESCRIPTION
  This function takes care of enabling TX RF hardware. The function returns 
  immediately.  An optional callback function is called when the warmup is 
  complete.

DEPENDENCIES
  Must be called from the HDR Tx task.
  
RETURN VALUE
  RF status.

SIDE EFFECTS
  None.

===========================================================================*/
hdrtx_rf_status_enum_type hdrtx_enable_tx
(
  void (*callback_ptr)(hdrtx_rf_status_enum_type)
    /* Function to be called when RF is ready */ 
)
{
#ifdef FEATURE_HDR_RUMI
  cfw_tx_start_msg_t start_msg;
  cfw_tx_agc_cfg_msg_t tx_agc_cfg_msg;
#endif /* FEATURE_HDR_RUMI */
  boolean call_cb = TRUE;
#ifndef FEATURE_HDR_RUMI
  uint32 rf_status;
#endif /* #ifndef FEATURE_HDR_RUMI */

  HDR_MSG_PROT (MSG_LEGACY_HIGH, "Enabling TX power ");

  /* Make sure someone hasn't already started power up */
  if (hdrtx_rf_status == TX_ON)
  {
    HDR_MSG_PROT(MSG_LEGACY_MED, "TX_ON already, calling callback.");
  }
  else if (hdrtx_rf_status == TX_POWERING_UP)
  {
    HDR_MSG_PROT(MSG_LEGACY_MED, "TX_POWERING_UP already, callback delayed");

    /* Don't call the callback yet! */
    call_cb = FALSE;
  }
  else /* TX_OFF or TX_OFF_RF_ON */
  {
    /* Enable call back for HDR Tx AGC report */
    /* Ignore do ... while(0) lint error #717 
     * do ... while(0) is used in clk_reg macro */
    /*lint -save -e717 */
    timer_reg(&hdrtx_agc_cb,
              (timer_t2_cb_type) hdrtx_report_agc,
              (timer_cb_data_type) NULL,
              200,
              1000);
    /*lint -restore */
    if ( hdrtx_rf_status == TX_OFF )
    {
      /* Notify TX data enable status to MCPM */
      hdrsrchmcpm_set_state( HDRSRCHMCPM_START_TX_REQ );
#ifdef FEATURE_HDR_UNIFIED_TX
      hdrmod_clear_carrier_enabled();
        /* Clear TX_DO_CTL_Cm before start tx. HW requires TX_DO_CTL_Cm after 
           transmission stops. In TX stop sequence, transmission stops at the 
           next frame or subframe boundary after OFFL_EN is set to 0. We don't
           know exactly when transmissions stops, so we clear those register at
           the beginning of next tx start
         */ 
#endif /* FEATURE_HDR_UNIFIED_TX */
       /*-----------------------------------------------------------------------
          The RF API (rfm_enable_tx) could block the HDRTX task. So, it is 
          essential to maintain an intermediate RF status (TX_POWERING_UP) until 
          the rfm_enable_tx returns. Otherwise, a second concurrent call to this 
          API from HDRSRCH task could call the same API again causing 
          inconsistency.
        -----------------------------------------------------------------------*/
      HDR_MSG_PROT(MSG_LEGACY_MED, "Tx state from TX_OFF -> TX_POWERING_UP");
      hdrtx_rf_status = TX_POWERING_UP;

      /* Obtain RF device id */
      hdrtx.rx_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_PRI_CHAIN);
      hdrtx.tx_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN);

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
      /* Send band-chan change msg to MCS */
      hdrsrchrf_send_tx_freq_to_coex( hdrtx.tx_dev );    
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

    /* Fetch a buffer! */
    if (txlm_allocate_buffer(hdrsrchrf_get_txlm_chain(), LM_HDR, 
          &hdrtx_txlm_handle) == LM_SUCCESS)
    {
      hdrtx_txlm_handle_valid = TRUE;
        /* Mark handle as valid */
    }
    else
    {
      ERR ("Unable to allocate RxLM buffer!", 0, 0, 0 );
         /* Just err for now... */
    }

#ifndef FEATURE_HDR_RUMI
    /* Enable Tx */
#ifdef FEATURE_HDR_SELECTIVE_TXD
    hdrsrchmac_txd_rf_call_started( );
      /* this function request (and block waiting) ASDiv crit sect */

    HDR_MSG_PROT_2( MSG_LEGACY_MED, 
                    "Wakeup Tx, dev:%d rx:%d", 
                    hdrtx.tx_dev, hdrtx.rx_dev );

    rf_status = rfm_hdr_wakeup_tx_v2( hdrtx.tx_dev, 
                                      hdrtx.rx_dev, 
                                      hdrtx_txlm_handle, 
                                      NULL, 
                                      NULL );

    hdrsrchmac_txd_rf_call_finished( );
      /* this function will release on ASDiv crit sect */
#else
    rf_status = rfm_hdr_wakeup_tx_v2( hdrtx.tx_dev,
                                      hdrtx.rx_dev, 
                                      hdrtx_txlm_handle, 
                                      NULL, 
                                      NULL );
#endif /* FEATURE_HDR_SELECTIVE_TXD */
    if ( rf_status != RFM_CDMA_HEALTHY_STATE )
    {
      ERR_FATAL("rfm_hdr_wakeup_tx() failed", 0, 0, 0);
    }

    /* NOTE: Currently RF will not return from the above function until
       FW sends a response indicating that the Tx configuration is complete.
       The hdrmod_enter() function below is dependent on this sequencing. */
#endif /* FEATURE_HDR_RUMI */

    }
    else if ( hdrtx_rf_status == TX_OFF_RF_ON )
    {
#ifdef FEATURE_HDR_UNIFIED_TX
       hdrmod_clear_carrier_enabled();
        /* Clear TX_DO_CTL_Cm before start tx. HW requires TX_DO_CTL_Cm after 
           transmission stops. In TX stop sequence, transmission stops at the 
           next frame or subframe boundary after OFFL_EN is set to 0. We don't
           know exactly when transmissions stops, so we clear those register at
           the beginning of next tx start
         */ 
#endif /* FEATURE_HDR_UNIFIED_TX */
       HDR_MSG_PROT(MSG_LEGACY_MED, "Tx state from TX_OFF_RF_ON -> TX_ON");
    }

#ifdef FEATURE_HDR_RUMI
    hdrtx_txlm_handle_valid = TRUE;
      /* Mark handle as valid */

    hdrtx_tx_start_rsp_rcved = 0;

#ifndef FEATURE_HDR_JOLOKIA_MODEM
    /* Hack to set DP_IQ_GAIN */
    //HWIO_TXC_DP_IQ_GAIN_A0_OUT(901);
    * (volatile uint32 *) 0xecb770b8 = 901;
#endif /* FEATURE_HDR_JOLOKIA_MODEM */


    memset((void *)&tx_agc_cfg_msg, 0, sizeof(tx_agc_cfg_msg));
    msgr_init_hdr( &tx_agc_cfg_msg.hdr,
                   MSGR_HDR_TX,
                   HDR_FW_TX_AGC_CFG_CMD );

    estatus = msgr_send ( &tx_agc_cfg_msg.hdr, sizeof(cfw_tx_agc_cfg_msg_t) );
    ASSERT ( estatus == E_SUCCESS );

    /* Send TX_START msg to FW */
    memset((void *)&start_msg, 0, sizeof(start_msg));
    msgr_init_hdr( &start_msg.hdr,
                   MSGR_HDR_TX,
                   HDR_FW_TX_START_CMD );

    start_msg.start_rtc_cx1 = 0x1c00;
    start_msg.rf_script_idx = -1; // Set to -1 to disable script

    estatus = msgr_send ( &start_msg.hdr, sizeof(cfw_tx_start_msg_t) );
    ASSERT ( estatus == E_SUCCESS );

    while ( hdrtx_tx_start_rsp_rcved == 0 )
    {
  
    }

    HDR_MSG_PROT (MSG_LEGACY_HIGH, "TX_START_RSP received");

    hdrtx_tx_start_rsp_rcved = 1;

#endif /*FEATURE_HDR_RUMI */

    /* Enable HDR mode in modulator. */
    hdrmod_enter();

    /* Note that all changes to hdrtx_rf_status occur in hdrtx task. */
    hdrtx_rf_status = TX_ON;

    HDR_MSG_PROT (MSG_LEGACY_HIGH, "RF Tx enable complete");

    /* Enable periodic power reporting */
    hdrtx_enable_power_reporting();

  }

  /* Tx should be ON by now. Call callback immediately */
  if (call_cb && (callback_ptr != NULL) )
  {
    callback_ptr(hdrtx_rf_status);
  }

  return hdrtx_rf_status;
}

#ifdef FEATURE_HDR_QTA
/*===========================================================================

FUNCTION HDRTX_PSEUDO_ENABLE_TX


DESCRIPTION
  This function fakes enabling TX RF hardware as it is already enabled.
  The function returns immediately.  An optional callback function is
  called when the warmup is complete.

DEPENDENCIES
  Must be called from the HDR Tx task.
  
RETURN VALUE
  RF status.

SIDE EFFECTS
  None.

===========================================================================*/
hdrtx_rf_status_enum_type hdrtx_pseudo_enable_tx
(
  void (*callback_ptr)(hdrtx_rf_status_enum_type)
    /* Function to be called when RF is ready */ 
)
{

  uint8   ch;
  boolean call_cb = TRUE;

  rfm_device_enum_type              rf_device;

  cxm_stx_tech_state_chs_ind_s      tx_enabled_ind;

  hdrsrchrf_trm_freq_info_type *    tune_info_ptr;

  /* Error status */
  errno_enum_type                   estatus;
  
  HDR_MSG_PROT (MSG_LEGACY_HIGH, "Enabling TX power ");

  /* Make sure someone hasn't already started power up */
 
    /* Enable call back for HDR Tx AGC report */
    /* Ignore do ... while(0) lint error #717 
     * do ... while(0) is used in clk_reg macro */
    /*lint -save -e717 */
    timer_reg(&hdrtx_agc_cb,
              (timer_t2_cb_type) hdrtx_report_agc,
              (timer_cb_data_type) NULL,
              200,
              1000);
    /*lint -restore */

    /* Notify TX data enable status to MCPM */
    hdrsrchmcpm_set_state( HDRSRCHMCPM_START_TX_REQ );

    /*-----------------------------------------------------------------------
       The RF API (rfm_enable_tx) could block the HDRTX task. So, it is 
       essential to maintain an intermediate RF status (TX_POWERING_UP) until 
       the rfm_enable_tx returns. Otherwise, a second concurrent call to this 
       API from HDRSRCH task could call the same API again causing 
       inconsistency.
     -----------------------------------------------------------------------*/

    hdrtx_rf_status = TX_POWERING_UP;

    /* Obtain RF device id */    
    rf_device = hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN);

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    /* Send band-chan change msg to MCS */
    hdrsrchrf_send_tx_freq_to_coex( rf_device );    
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

    /* Note that all changes to hdrtx_rf_status occur in hdrtx task. */
    hdrtx_rf_status = TX_ON;

    HDR_MSG_PROT (MSG_LEGACY_HIGH, "RF Tx enable complete");

    /* Prepare and send tx enabled indication */

    memset ( &tx_enabled_ind, 0, sizeof(cxm_stx_tech_state_chs_ind_s) );
    msgr_init_hdr ( &tx_enabled_ind.hdr, MSGR_HDR_TX, 
      MCS_CXM_STX_SET_TX_STATE_WITH_CHS_IND );

    /* Obtain last tune info from srch */
    tune_info_ptr = hdrsrchrf_get_trm_freq_info();

    /* Sanity check */
    ASSERT ( tune_info_ptr != NULL );

    tx_enabled_ind.tech_id = CXM_TECH_HDR;
    tx_enabled_ind.tx_rfdev = rf_device;
    tx_enabled_ind.band = tune_info_ptr->freq_info.band;
    tx_enabled_ind.num_channels = tune_info_ptr->freq_info.num_channels;
    for ( ch = 0; ch < tx_enabled_ind.num_channels; ch++)
    {
      tx_enabled_ind.channels[ch] = tune_info_ptr->freq_info.channels[ch];
    }

    /* Send tx enabled indication to STX module */
    estatus = msgr_send ( &tx_enabled_ind.hdr, sizeof(tx_enabled_ind) );
    ASSERT ( estatus == E_SUCCESS );
    
    /* Enable periodic power reporting */
    hdrtx_enable_power_reporting();
  

  /* Tx should be ON by now. Call callback immediately */
  if (call_cb && (callback_ptr != NULL) )
  {
    callback_ptr(hdrtx_rf_status);
  }

  return hdrtx_rf_status;
}
#endif /* FEATURE_HDR_QTA */

#ifdef FEATURE_HDR_SELECTIVE_TXD
/* Revisit: might not be needed */
#if 0
/*===========================================================================
FUNCTION HDRTX_TXD_EXIT_TX_CB

DESCRIPTION
  Callback function to be called after TxD state machine has exited TX mode.
  Antenna switch must be completed before exiting TX mode.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrtx_txd_exit_tx_cb( void )
{
  rex_set_sigs( HDRTX_TASK_PTR, HDRTX_TXD_TX_SHUTDOWN_SIG );
}

/*===========================================================================
FUNCTION HDRTX_WAIT_FOR_TXD_EXIT_TX

DESCRIPTION
  Callback function to be called after TxD state machine has exited TX mode.
  Antenna switch must be completed before exiting TX mode.

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrtx_wait_for_txd_exit_tx( void )
{
  rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_TXD_TX_SHUTDOWN_SIG );
  hdrsrchmac_txd_reg_tx_shutdown_cb( hdrtx_txd_exit_tx_cb );

  if ( hdrsrchmac_txd_is_in_tx_mode() )
  {
    hdrtx_task_wait( HDRTX_TXD_TX_SHUTDOWN_SIG );
  }
}
#endif
#endif /* FEATURE_HDR_SELECTIVE_TXD */
      
/*===========================================================================

FUNCTION HDRTX_DISABLE_TX

DESCRIPTION
  This function takes care of disabling TX RF hardware.  Unlike the enable,
  no wait is involved.

DEPENDENCIES
  Must be called from the HDR Tx task.

PARAMETERS
  skip_tx_disable - Specifies if CP wants to skip disabling Tx, 
                    If true, then we skip disabling Tx RF and MCPM. 

RETURN VALUE
  RF Status

SIDE EFFECTS
  None.

===========================================================================*/
hdrtx_rf_status_enum_type hdrtx_disable_tx 
( 
  boolean skip_tx_disable 
)
{

#ifdef FEATURE_HDR_RUMI
  cfw_tx_stop_msg_t stop_msg;
#endif /* FEATURE_HDR_RUMI */
  rfm_wait_time_t rf_ret_val = RFM_CDMA_HEALTHY_STATE;
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Check that Tx is supposed to be On or Powering up or Skip state */
  if (hdrtx_rf_status != TX_OFF)
  {
    /* Remove callback for HDR Tx agc report */
    (void) timer_clr(&hdrtx_agc_cb, T_NONE);
  
    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Current Task: 0x%x", rex_self());

    ASSERT (hdrtx_txlm_handle_valid == TRUE);

#ifdef FEATURE_HDR_SELECTIVE_TXD    
    /* Wait for TxD to exit TX and reset antenna position. This is a FW
       requirement that antenna switch must happen while in TX mode. The
       request to exit TX mode (and switch ant) should have been sent
       already. Here we just wait for switch to complete */

    /* Revisit: may not be needed */
    /* hdrtx_wait_for_txd_exit_tx(); */
#endif /* FEATURE_HDR_SELECTIVE_TXD */

#ifdef FEATURE_HDR_RUMI

    hdrtx_tx_stop_rsp_rcved = 0;

    /* Send TX_STOP msg to FW */
    memset((void *)&stop_msg, 0, sizeof(stop_msg));
    msgr_init_hdr( &stop_msg.hdr,
                   MSGR_HDR_TX,
                   HDR_FW_TX_STOP_CMD );

    estatus = msgr_send ( &stop_msg.hdr, sizeof(cfw_tx_stop_msg_t) );
    ASSERT ( estatus == E_SUCCESS );

    /* Wait for TX_STOP response from FW */
    while ( hdrtx_tx_stop_rsp_rcved == 0 )
    {
      /*Need to wait for TX Stop response to avoid crash in FW 
        due to Clocks being turned off before TX stop processed*/
    }

    HDR_MSG_PROT (MSG_LEGACY_HIGH, "TX_STOP_RSP received");

    hdrtx_tx_stop_rsp_rcved = 1;

#else /* FEATURE_HDR_RUMI */
    if ( skip_tx_disable == FALSE )
    {
#ifdef FEATURE_HDR_SELECTIVE_TXD
      hdrsrchmac_txd_rf_call_started( );
        /* this function request (and block waiting) ASDiv crit sect */

      rf_ret_val = rfm_hdr_sleep_tx( hdrtx.tx_dev,
                               hdrtx_txlm_handle, 
                               NULL,
                               NULL );

      hdrsrchmac_txd_rf_call_finished( );
        /* this function will release on ASDiv crit sect */
#else
      rf_ret_val = rfm_hdr_sleep_tx( hdrtx.tx_dev,
                              hdrtx_txlm_handle, NULL, NULL);
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      HDR_MSG_PROT_1( MSG_LEGACY_MED, 
                      "Sleep Tx dev:%d", 
                      hdrtx.tx_dev );
    }
#endif /* FEATURE_HDR_RUMI */

    if ( rf_ret_val < RFM_CDMA_HEALTHY_STATE )
    {
      /* Error fatal */
      ERR_FATAL("Unexpected rf status %d: please check RF cal", 
                rf_ret_val, 0, 0);
    }

    if ( skip_tx_disable == FALSE )
    {
      /* De-allocate buffer and check result */
      if ( txlm_deallocate_buffer(hdrtx_txlm_handle) != LM_SUCCESS )
      {
        ERR ("Unable to deallocate TxLM buffer!", 0, 0, 0 );
           /* Just err for now... */
      }
      hdrtx_txlm_handle_valid = FALSE;    
    }

    if ( skip_tx_disable == FALSE )
    {
      /* HDR modulator clean up is paired with RF Tx disable. */
      hdrmod_exit();
      /* Todo note: move hdrmod_exit() before rf call as hdrmod_enter?  */

    /* In case of MCDO connection close, delay this step until searcher exits 
       traffic state. This gives sufficient clocks to FW to handle unexpected
       FTC packets arriving at deinterleaver, after connection got closed. */
    if ( hdrsrchmcpm_get_mcdo_status() != TRUE )
    {
      /* Notify TX data disable status to MCPM */
      hdrsrchmcpm_set_state( HDRSRCHMCPM_STOP_TX_REQ );
     }
    }
    /* Disable periodic power reporting */
    hdrtx_disable_power_reporting();

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
    if ( skip_tx_disable == FALSE )
    {
    /* Let MCS know no TX now */
    hdrsrchrf_send_tx_freq_to_coex( RFM_INVALID_DEVICE );
    }
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */

    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Tx state from %s -> TX_OFF", 
                   skip_tx_disable ? "TX_OFF_RF_ON": "TX_ON" );
    /* Setting the value of hdrtx_rf_status */
    hdrtx_rf_status = skip_tx_disable ? TX_OFF_RF_ON : TX_OFF;

  }

  return hdrtx_rf_status;

}


/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_PROTOCOL_CMD

DESCRIPTION
  All asynchronous commands for protocols that run in the TX task context 
  are routed through here. This function dispatches the command to the 
  protocol it is for.  Synchronous commands are processed in the context of
  the protocol issuing the command, so they are not queued and thus are not
  routed through here.
  
DEPENDENCIES
  None

PARAMETERS
  hdrtx_cmd_ptr - Command for a protocol in TX task 

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_protocol_cmd
( 
  hdrhai_protocol_name_enum_type    protocol_name,
  hdrtx_protocol_cmd_union_type *hdrtx_cmd_union_ptr 
)
{
  switch ( protocol_name )
  {
    case HDRHAI_AC_MAC_PROTOCOL:
    {
      hdramac_process_cmd( &hdrtx_cmd_union_ptr->amac );
      break;
    }

    case HDRHAI_RTC_MAC_PROTOCOL:
    {
      hdrrmac_process_cmd( &hdrtx_cmd_union_ptr->rmac );
      break;
    }

    case HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL:
    {
      hdrlmac_process_cmd( &hdrtx_cmd_union_ptr->lmac );
      break;
    }

    case HDRHAI_MODULATOR:
    {
      hdrmod_process_cmd( &hdrtx_cmd_union_ptr->mod );
      break;
    }

#ifdef FEATURE_LTE_TO_HDR_OH
    case HDRHAI_SAP_TX_MODULE:
    {
      hdrsaptx_process_cmd( &hdrtx_cmd_union_ptr->saptx );
      break;
    }
#endif /* FEATURE_LTE_TO_HDR_OH */

    default:
    {
      ERR( "CMD for unknown protocol %d", protocol_name, 0, 0 );
      break;
    }
  }

}/* hdrtx_protocol_cmd */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_PROTOCOL_IND

DESCRIPTION
  All indications for protocols that run in the TX task context are routed 
  through here. This function dispatches the indication to the protocol it 
  is for.

DEPENDENCIES
  None

PARAMETERS
  hdrtx_ind_ptr - Indication for a protocol in TX task

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_protocol_ind
( 
  hdrhai_protocol_name_enum_type  protocol_name,
  hdrbuf_ind_type             *hdrtx_ind_ptr 
)
{
  switch ( protocol_name )
  {
    case HDRHAI_SIGNALING_LINK_PROTOCOL:
    {
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrslp_process_ind( hdrtx_ind_ptr->ind_name,
                          &hdrtx_ind_ptr->ind_data );
      #endif
      break;
    }

    case HDRHAI_LINK_MANAGEMENT_AND_CONTROL_PROTOCOL:
    {
      hdrlmac_process_ind( hdrtx_ind_ptr->ind_name,
                           &hdrtx_ind_ptr->ind_data );
      break;
    }

    case HDRHAI_AC_MAC_PROTOCOL:
    {
      hdramac_process_ind( hdrtx_ind_ptr->ind_name,
                           &hdrtx_ind_ptr->ind_data );
      break;
    }

    case HDRHAI_RTC_MAC_PROTOCOL:
    {
      hdrrmac_process_ind( hdrtx_ind_ptr->ind_name,
                           &hdrtx_ind_ptr->ind_data );
      break;
    }

    case HDRHAI_IS890_FTAP:
    {
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrftap_process_ind( hdrtx_ind_ptr->ind_name,
                           &hdrtx_ind_ptr->ind_data );
      #endif
      break;
    }

    case HDRHAI_IS890_RTAP:
    {
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrrtap_process_ind( hdrtx_ind_ptr->ind_name,
                           &hdrtx_ind_ptr->ind_data );
      #endif
      break;
    }

    case HDRHAI_PHYSICAL_LAYER_PROTOCOL:
    {
      hdrphy_process_ind( hdrtx_ind_ptr->ind_name,
                          &hdrtx_ind_ptr->ind_data );
      break;
    }

#ifdef FEATURE_LTE_TO_HDR_OH
    case HDRHAI_SAP_TX_MODULE:
    {
      hdrsaptx_process_ind( hdrtx_ind_ptr->ind_name,
                            &hdrtx_ind_ptr->ind_data );
      break;
    }
#endif /* FEATURE_LTE_TO_HDR_OH */

    default:
    {
      ERR( "IND for unknown protocol %d", protocol_name, 0, 0 );
      break;
    }
  }
}/* hdrtx_protocol_ind */ 

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_PROTOCOL_MSG

DESCRIPTION
  All messages for protocols that run in the TX task context are routed 
  through here. This function dispatches the message to the protocol it 
  is for.

DEPENDENCIES
  None

PARAMETERS
  hdrtx_msg_ptr - Message for a protocol in TX task

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_protocol_msg 
( 
  hdrhai_protocol_name_enum_type  protocol_name,
  hdrhai_protocol_instance_enum_type protocol_instance,
  dsm_item_type               *item_ptr 
)
{
  switch ( protocol_name )
  {
    case HDRHAI_SIGNALING_LINK_PROTOCOL:
    {
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrslp_process_msg( item_ptr, protocol_instance );
      #endif
      break;
    }

    case HDRHAI_AC_MAC_PROTOCOL:
    {
      hdramac_process_msg( item_ptr, protocol_instance );
      break;
    }

    case HDRHAI_RTC_MAC_PROTOCOL:
    {
      hdrrmac_process_msg( item_ptr, protocol_instance );
      break;
    }

    default:
    {
      ERR( "MSG for unknown protocol %d", protocol_name, 0, 0 );
      break;
    }
  }

}/* hdrtx_protocol_msg */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_PROTOCOL_TIMER

DESCRIPTION
  All timer expiration events for protocols that run in the TX task context 
  are routed here. This function dispatches the timer expiry to the protocol it 
  is for.

DEPENDENCIES
  None

PARAMETERS
  protocol_name: Protocol that owns the timer
  timer_id: Timer Id

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_protocol_timer
(
  hdrhai_protocol_name_enum_type protocol_name,
  uint32 timer_id
)
{
  switch (protocol_name)
  {
    case HDRHAI_RTC_MAC_PROTOCOL:
      hdrrmac_process_timer(timer_id);
      break;

#ifdef FEATURE_LTE_TO_HDR_OH
    case HDRHAI_SAP_TX_MODULE:
      hdrsaptx_process_timer(timer_id);
      break;
#endif /* FEATURE_LTE_TO_HDR_OH */

    case HDRHAI_TX:
      hdrtx_process_timer(timer_id);
      break;

    default:
      ERR( "TIMER for unknown protocol %d", protocol_name, 0, 0 );
      break;
  }

} /* hdrtx_protocol_timer */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_PROTOCOL_ITEM

DESCRIPTION
  This function routes the item to the protocol it is for, according to
  whether it is a command, indication, or message

DEPENDENCIES
  None

PARAMETERS
  hdrtx_msg_ptr - Message for a protocol in TX task

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrtx_protocol_item
( 
  hdrtx_buf_type *hdrtx_buf_ptr 
)
{
  ASSERT (hdrtx_buf_ptr != NULL);

  switch ( hdrtx_buf_ptr->hdr.type_of_item )
  {
    case HDRBUF_COMMAND: /* The item is a command */
    {
      hdrtx_protocol_cmd( hdrtx_buf_ptr->hdr.protocol_name,
                          &hdrtx_buf_ptr->item.cmd );
      break;
    }

    case HDRBUF_INDICATION: /* The item is an indication */
    {
      hdrtx_protocol_ind( hdrtx_buf_ptr->hdr.protocol_name,
                          &hdrtx_buf_ptr->item.ind );
      break;
    }

    case HDRBUF_MESSAGE: /* The item is a message (DSM item */
    {
      hdrtx_protocol_msg( hdrtx_buf_ptr->hdr.protocol_name,  
                          hdrtx_buf_ptr->hdr.protocol_instance,  
                          hdrtx_buf_ptr->item.item_ptr );
      break;
    }

    case HDRBUF_TIMER:  /* The item is a timer */
    {
      hdrtx_protocol_timer( hdrtx_buf_ptr->hdr.protocol_name,
                            hdrtx_buf_ptr->item.timer_id );
  
      break;
    }

    default:
    {
      ERR( "BUF for unknown item %d", hdrtx_buf_ptr->hdr.type_of_item, 0, 0 );
      break;
    }
  }

}/* hdrtx_protocol_item */

/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_SUBTASK

DESCRIPTION
  This function is
   the main entry point to the HDR subtask of the Main 
  Control task.  It provides the main loop to handle commands, indications,
  and messages for protocols running within the TX task context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_subtask( void )
{
  rex_sigs_type   rex_signals_mask;  /* Task signal mask.     */
  hdrtx_buf_type *hdrtx_buf_ptr;     /* Event buffer pointer. */
  boolean exit_hdr_mode = FALSE;     /* Exit flag */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_PROT (MSG_LEGACY_HIGH, "Entering HDR Tx...");

  /* HDR entry Tx task processing goes here */

  /*-------------------------------------------------------------------------
   Enter the task loop to handle task signals.
  -------------------------------------------------------------------------*/
  while ( !exit_hdr_mode ) 
  {
    rex_signals_mask = hdrtx_wait( HDRTX_EVENT_Q_SIG             |
                                   HDRTX_AMAC_ISR_SIG            |
                                   HDRTX_RMAC_ISR_SIG            |
                                   HDRTX_RMAC_RATELIM_TIMER_SIG  |
                                   HDRTX_SLP_CB_SIG              |
                                   HDRTX_EXIT_HDR_SIG            |
                                   HDRTX_IS890_RTAP_PKTGEN_SIG   |
                                   HDRTX_IS890_FTAP_MSG_SIG      |
                                   HDRTX_IS890_RTAP_MSG_SIG      |
                                   HDRTX_IS890_FTAP_LO_INIT_SIG  |
                                   HDRTX_MRLP_SPD_SIG            |
                                   HDRTX_MRLP_STATS_SIG          |
                                   HDRTX_RLP_TXQ_SIG             |
                                   HDRTX_MSG_SIG                 |
                                   TASK_OFFLINE_SIG              |
                                   TASK_STOP_SIG                 );
                                                       
    /*-----------------------------------------------------------------------
     Powerdown command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & TASK_STOP_SIG) != 0 ) 
    {
      HDR_MSG_PROT (MSG_LEGACY_MED, "TASK_STOP_SIG received");
                 
      (void) rex_clr_sigs( HDRTX_TASK_PTR, TASK_STOP_SIG );

      /* Deregister and delete message client */
      hdrtx_msg_stop();
    }

    /*-----------------------------------------------------------------------
     Offline command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & TASK_OFFLINE_SIG) != 0 ) 
    {
      HDR_MSG_PROT (MSG_LEGACY_MED, "TASK_OFFLINE_SIG received");  
                 
      (void) rex_clr_sigs( HDRTX_TASK_PTR, TASK_OFFLINE_SIG );
    }


    /*-----------------------------------------------------------------------
     Transmit interrupt signal. Clear signal and invoke protocol signal
     handling function.  

     NOTE: Processing of ISR signal must precede processing of EVENT_Q 
           signal. This is to take care of the case when a command to 
           deregister ISR ( setting of EVENT_Q_SIG ) and firing of ISR 
           ( setting of ISR_SIG ) happens simultaneously. In this case,
           processing EVENT_Q before ISR would result in ISR being 
           executed after it has been disabled.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRTX_AMAC_ISR_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_AMAC_ISR_SIG );
      hdramac_process_sig( HDRTX_AMAC_ISR_SIG );
    }

    if ( ( rex_signals_mask & HDRTX_RMAC_ISR_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_RMAC_ISR_SIG );
      hdrrmac_process_sig( HDRTX_RMAC_ISR_SIG );
    }


    /*-----------------------------------------------------------------------
     Event queue signal. Clear signal and handle all events on event queue.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRTX_EVENT_Q_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_EVENT_Q_SIG );
      
      HDR_MSG_PROT_1 (MSG_LEGACY_LOW,  "Processing %d events", 
                     q_cnt( &hdrtx_event_q ));

      while( q_cnt( &hdrtx_event_q ) != 0 )
      {
        hdrtx_buf_ptr = (hdrtx_buf_type*) q_get( &hdrtx_event_q );
        hdrtx_protocol_item( hdrtx_buf_ptr );
        modem_mem_free( hdrtx_buf_ptr, MODEM_MEM_CLIENT_HDR_CRIT );
      }
    }

    /*-----------------------------------------------------------------------
     Signal for Firmware messages. Clear signal and process all firmware 
     messages in queue
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRTX_MSG_SIG ) != 0 )
    {
      (void) rex_clr_sigs ( HDRTX_TASK_PTR, HDRTX_MSG_SIG );
      hdrtx_process_msgs();
    }

#if defined( FEATURE_HDR_IS890 ) || defined (FEATURE_HDR_IS890A)
    /*-----------------------------------------------------------------------
     RTAP packet generation signals. 
     Call into the RTAP protocol to generate a new tst pkt
    -----------------------------------------------------------------------*/

    if ( ( rex_signals_mask & HDRTX_IS890_RTAP_PKTGEN_SIG) != 0 )
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_IS890_RTAP_PKTGEN_SIG);
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrrtap_process_sig( HDRTX_IS890_RTAP_PKTGEN_SIG );
      #endif
    }

#endif /* FEATURE_HDR_IS890 or FEATURE_HDR_IS890A */

    /*-----------------------------------------------------------------------
     Protocol timer signals. Clear signal and invoke protocol signal
     handling function.
    -----------------------------------------------------------------------*/

    if ( ( rex_signals_mask & HDRTX_RMAC_RATELIM_TIMER_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_RMAC_RATELIM_TIMER_SIG );
      hdrrmac_process_sig( HDRTX_RMAC_RATELIM_TIMER_SIG );
    }

    /*-----------------------------------------------------------------------
     SLP call back signal. Clear signal and TBD.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRTX_SLP_CB_SIG ) != 0 )
    {
      ( void ) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_SLP_CB_SIG );
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrslp_process_sig(HDRTX_SLP_CB_SIG);
      #endif
    }
    
    /*-----------------------------------------------------------------------
     Exit HDR signal. Clear signal, and set flag to exit.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRTX_EXIT_HDR_SIG) != 0 ) 
    {
      HDR_MSG_PROT (MSG_LEGACY_MED, "HDRTX_EXIT_HDR_SIG received"); 
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_EXIT_HDR_SIG );
      exit_hdr_mode = TRUE;
    }

#if defined( FEATURE_HDR_IS890 ) || defined( FEATURE_HDR_IS890A )
    /*-----------------------------------------------------------------------
     IS-890 FTAP signal. Clear signal, and set flag to exit.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRTX_IS890_FTAP_MSG_SIG) != 0 ) 
    {
      HDR_MSG_PROT (MSG_LEGACY_MED, "HDRTX_IS890_FTAP_MSG_SIG received"); 
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_IS890_FTAP_MSG_SIG );
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrftap_process_sig( HDRTX_IS890_FTAP_MSG_SIG );
      #endif
    }

    /*-----------------------------------------------------------------------
     IS-890 FTAP signal. Clear signal, and set flag to exit.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRTX_IS890_FTAP_LO_INIT_SIG) != 0 ) 
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_IS890_FTAP_LO_INIT_SIG );
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrftap_process_sig( HDRTX_IS890_FTAP_LO_INIT_SIG );
      #endif
    }

    /*-----------------------------------------------------------------------
     IS-890 RTAP signal. Clear signal, and set flag to exit.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRTX_IS890_RTAP_MSG_SIG) != 0 ) 
    {
      HDR_MSG_PROT (MSG_LEGACY_MED, "HDRTX_IS890_RTAP_MSG_SIG received"); 
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_IS890_RTAP_MSG_SIG );
      #ifndef FEATURE_RUMI3_BRINGUP
      hdrrtap_process_sig( HDRTX_IS890_RTAP_MSG_SIG );
      #endif
    }
#endif  /* FEATURE_IS_890 or FEATURE_IS890A */

#ifdef FEATURE_HDR_QOS
    if ( (rex_signals_mask & HDRTX_MRLP_SPD_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_MRLP_SPD_SIG );
#ifndef FEATURE_RUMI3_BRINGUP
      hdrmrlpspd_process_sig( HDRTX_MRLP_SPD_SIG );
#endif /* FEATURE_RUMI3_BRINGUP */
    }
#endif /* FEATURE_HDR_QOS */
    if ( (rex_signals_mask & HDRTX_RLP_TXQ_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_RLP_TXQ_SIG );
#ifndef FEATURE_RUMI3_BRINGUP
      hdrrlptxq_process_signal( HDRTX_RLP_TXQ_SIG );
#endif /* FEATURE_RUMI3_BRINGUP */
    }

    if ( (rex_signals_mask & HDRTX_MRLP_STATS_SIG ) != 0 )
    {
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_MRLP_STATS_SIG );

      hdrrlpstats_process_sig( HDRTX_MRLP_STATS_SIG );
    }

  }/* while ( !exit_hdr_mode ) */

  /* Exit processing goes here */

} /* hdrtx_subtask */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_TASK

DESCRIPTION
  This function is the main entry point to the HDR subtask of the Main 
  Control task.  It provides the main loop to handle commands, indications,
  and messages for protocols running within the TX task context.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_task
( 
  dword           dummy              /* Required for REX, ignore */
  /*lint -esym(715,dummy)
   * Ignore argument(dummy) not used lint error #715 */
)
{
  rex_sigs_type   rex_signals_mask;  /* Task signal mask.     */

  hdrtx_msg_s_type *msg_ptr; /* Pointer to dequeued message item */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Perform task initialization.
  -------------------------------------------------------------------------*/
  rcinit_handshake_startup();

  /* Run time synch with other tasks is done here if needed, following APIs
  ** used: rcinit_lookup(),  rcevt_wait_name()
  */
  if ( rcevt_wait_name(HDRMC_RCEVT_READY) == RCEVT_NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }
  if ( rcevt_signal_name(HDRTX_RCEVT_READY) == RCEVT_NULL )
  {
    HDR_MSG_SRCH( MSG_LEGACY_ERROR,"Unexpected Run Control Event Notification err" );
  }

  /* RCINIT calls init function defined in task scons file in rcinit context
     All tcb related intializations should be done after rcinit starts our
     task */

  hdrtx_tcb_ptr = rex_self();

   /* Initialize task timers */
  rex_def_timer_ex( &hdrtx_pwr_report_timer,
    hdrtx_timer_cb,
    HDRTX_TIMER_ID_PWR_REPORT ); /* cb_param */

  hdrtx_dog_rpt_var = dog_hb_register_rex( (rex_sigs_type) HDRTX_RPT_TIMER_SIG );
   /* Register with dog HB */

  /* Init HDR modulator driver */
  hdrmod_init();

  /* Initialize RMAC */
  hdrrmac_init();

  /* Initialize message client and register for messages */
  hdrtx_msg_init();

  /*-------------------------------------------------------------------------
   Send the first watchdog report and set the timer for report interval.
  -------------------------------------------------------------------------*/
  hdrtx_process_wdog();

  /*-------------------------------------------------------------------------
   Enter the task loop to handle task signals.
  -------------------------------------------------------------------------*/
  for (;;) 
  {
    rex_signals_mask = hdrtx_wait( HDRTX_ENTER_HDR_SIG           |
                                   TASK_OFFLINE_SIG              |
                                   TASK_STOP_SIG                 |
                                   HDRTX_MSG_SIG );

    /*-----------------------------------------------------------------------
     Powerdown command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & TASK_STOP_SIG) != 0 ) 
    {
      HDR_MSG_PROT (MSG_LEGACY_MED, "TASK_STOP_SIG received"); 
      (void) rex_clr_sigs( HDRTX_TASK_PTR, TASK_STOP_SIG );

      /* Deregister and delete message client */
      hdrtx_msg_stop();
    }

    /*-----------------------------------------------------------------------
     Offline command signal. Clear signal, send ack to MC, and proceed.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & TASK_OFFLINE_SIG) != 0 ) 
    {
      HDR_MSG_PROT (MSG_LEGACY_MED, "TASK_OFFLINE_SIG received"); 
      (void) rex_clr_sigs( HDRTX_TASK_PTR, TASK_OFFLINE_SIG );
    }
    
    /*-----------------------------------------------------------------------
     Enter HDR signal. Clear signal, and enter subtask.
    -----------------------------------------------------------------------*/
    if ( (rex_signals_mask & HDRTX_ENTER_HDR_SIG) != 0 ) 
    {
      HDR_MSG_PROT (MSG_LEGACY_MED, "HDRTX_ENTER_HDR_SIG received"); 
      (void) rex_clr_sigs( HDRTX_TASK_PTR, HDRTX_ENTER_HDR_SIG );

      /* Enter HDR mode processing subtask */
      hdrtx_subtask();

    }

   /*-----------------------------------------------------------------------
     Signal for MSGR. Ingore since HDRTX is offline.
    -----------------------------------------------------------------------*/
    if ( ( rex_signals_mask & HDRTX_MSG_SIG ) != 0 )
    {
      HDR_MSG_PROT( MSG_LEGACY_ERROR, 
                    "Rx'ed HDRTX_MSG_SIG when HDRTX is offline" );

      (void) rex_clr_sigs ( HDRTX_TASK_PTR, HDRTX_MSG_SIG );

      while ( q_cnt ( &(hdrtx.msg_q) ) != 0 )
      {
        msg_ptr = (hdrtx_msg_s_type*) q_get( &(hdrtx.msg_q) );
        q_put ( &(hdrtx.free_q), &(msg_ptr->qlink) );
      }
    }

  } /* for(;;) */

}/* hdrtx_task */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_TASK_WAIT

DESCRIPTION
  This function is intended to be used by entities running within hdrtx 
  task context to block hdrtx task from running until a particular signal
  is set.

DEPENDENCIES
  None.

PARAMETERS
  Signal to wait for.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_task_wait
(
  uint32 sig
)
{
  if (rex_self () == HDRTX_TASK_PTR)
  {
    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "HDRTX task waiting for signal %x", sig);

    hdrtx_wait(sig);

    (void) rex_clr_sigs( HDRTX_TASK_PTR, sig );

    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Wait for signal %x complete.", sig);
  }
  else
  {
    HDR_MSG_PROT (MSG_LEGACY_ERROR, 
      "hdrtx_task_wait() calls are only supported from HDRTX task context");
  }

} /* hdrtx_task_wait */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_MSG_INIT

DESCRIPTION
  Initializes message interface by doing the following. 
    - Creates a messenger Client ID for HDRTX. 
    - Initializes the free queue and event queue. 
    - Registers the rex queues for the client with the messenger. 
    - Registers to receive indication messages. 

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_msg_init ( void )
{

  errno_enum_type  estatus;
    /* Error status */

  msgr_id_t queue_id;
    /* Queue Identifier */

  uint8 ind;
    /* Loop index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( !hdrtx.msgr_client_registered )
  {
    /* Create client */
    estatus = msgr_client_create ( &hdrtx.msgr_client_id );
    ASSERT ( estatus == E_SUCCESS );
  
    /* Initialize queues */
    ( void ) q_init ( &(hdrtx.msg_q) );
    ( void ) q_init ( &(hdrtx.free_q) );
  
    /* Fill free_q */
    for ( ind = 0; ind < HDRTX_MSG_QUEUE_SIZE; ind++ )
    {
      q_put ( &(hdrtx.free_q), 
              q_link ( &(hdrtx.q_items[ind]), 
                       &(hdrtx.q_items[ind].qlink) ) );
    }
  
    /* Create the msgr client and associated with rex queues */
    estatus = msgr_client_add_rex_q ( &(hdrtx.msgr_client_id),
                                    HDRTX_TASK_PTR,
                                    HDRTX_MSG_SIG,
                                    &(hdrtx.msg_q),
                                    &(hdrtx.free_q),
                                    MSGR_OFFSETOF( hdrtx_msg_s_type, msg ),
                                    MSGR_NO_OFFSET,
                                    (uint16) sizeof( hdrtx_msg_s_type ),
                                    &queue_id );

  ASSERT ( estatus == E_SUCCESS );

  /* Register for messages */
  estatus = msgr_register_block( 
                MSGR_HDR_TX,
                &(hdrtx.msgr_client_id),
                MSGR_ID_REX,
                hdrtx_rcv_msgs,
                sizeof( hdrtx_rcv_msgs )/sizeof( msgr_umid_type ) );

  ASSERT ( estatus == E_SUCCESS );

    /* Set msgr client registered flag */
    hdrtx.msgr_client_registered = TRUE;

  }
  else
  {
    HDR_MSG_PROT ( MSG_LEGACY_HIGH, "MSGR client already registered" );
  }

} /* hdrtx_msg_init */


/*===========================================================================

FUNCTION HDRTX_MSG_STOP

DESCRIPTION
  Cleans up the message interface by doing the following. 
    - De-registers the registered messages. 
    - Deletes the messenger Client ID for Decoder module. 

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_msg_stop ( void )
{
  errno_enum_type  estatus;
    /* Error status */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrtx.msgr_client_registered )
  {
    /* Deregister from receiving messages */
    estatus = msgr_deregister_block ( 
                MSGR_HDR_TX,
                &(hdrtx.msgr_client_id),
                hdrtx_rcv_msgs,
                sizeof( hdrtx_rcv_msgs )/sizeof( msgr_umid_type ) );
  
    ASSERT ( estatus == E_SUCCESS );
  
    /* Delete client */
    estatus = msgr_client_delete ( &(hdrtx.msgr_client_id) );
  
    ASSERT ( estatus == E_SUCCESS );

    /* Reset msgr client registered flag */
    hdrtx.msgr_client_registered = FALSE;

  }
  else
  {
    HDR_MSG_PROT ( MSG_LEGACY_HIGH, "MSGR client not registered" );
  }

} /* hdrtx_msg_stop */


/*===========================================================================

FUNCTION HDRTX_PROCESS_MSGS

DESCRIPTION
  The function dequeues all messages from the message queue and processes 
  them one at a time until all messages have been processed.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_process_msgs ( void )
{
  hdrtx_msg_s_type *msg_ptr;
    /* Pointer to dequeued message item */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* While the msg_q is not empty */
  while ( q_cnt ( &(hdrtx.msg_q) ) != 0 )
  {
    /* Dequeue a message item from the queue */
    msg_ptr = (hdrtx_msg_s_type*) q_get( &(hdrtx.msg_q) );

    /* Check for NULL */
    if (msg_ptr != NULL)
    {
      HDR_MSG_PROT_1 ( MSG_LEGACY_HIGH, "Rcvd. MSGR msg 0x%x", 
        msg_ptr->msg.hdr.id );

       /* Forward messages to their appropriate destinations */
      switch (msg_ptr->msg.hdr.id)
      {
        case HDR_FW_TX_PA_CTL_RSP:
        {
          hdrmod_process_fw_msg( msg_ptr );
          break;
        }

#ifdef FEATURE_HDR_BOLT_MODEM
#ifndef FEATURE_HDR_UNIFIED_TX
        case HDR_FW_TX_RESET_REQ_IND:
        {
          /* Post system lost to hdrtx command queue */
          hdrlmac_trigger_system_lost();
          
          break;
        }
#endif /* FEATURE_HDR_UNIFIED_TX */
#endif /* FEATURE_HDR_BOLT_MODEM */

        case HDR_FW_RMAC0_FRAME_IND:
        {
          hdrrmac_process_fw_msg( msg_ptr );
          break;
        }

#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
        case HDR_FW_FLEXCONN_REPOINT_IND:
        {
          hdrrmac_process_fw_msg( msg_ptr );
          break;
        }
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX) */

#ifdef FEATURE_LTE_TO_HDR_OH
#ifdef FEATURE_IRAT_DSM_SUPPORT
        case NAS_EMM_IRAT_HDR_UL_MSG_CNF:
#else
        case NAS_EMM_IRAT_UL_MSG_CNF:
#endif /* FEATURE_IRAT_DSM_SUPPORT */
        {
          hdrsaptx_process_irat_ul_msg_cnf_ind( &(msg_ptr->msg.irat_ul_cnf) );
          break;
        }

#ifdef FEATURE_IRAT_DSM_SUPPORT
        case NAS_EMM_IRAT_HDR_FAILURE_IND:
#else
        case NAS_EMM_IRAT_FAILURE_IND:
#endif /* FEATURE_IRAT_DSM_SUPPORT */
        {
          hdrsaptx_process_irat_ul_msg_failure_ind( 
            &(msg_ptr->msg.irat_failure_ind) );
          break;    
        }
#endif /* FEATURE_LTE_TO_HDR_OH */

        default:
        {
          HDR_MSG_PROT_1 ( MSG_LEGACY_ERROR, 
            "Unknown message! UMID: 0x%x", msg_ptr->msg.hdr.id );
        }

      } /* switch - Received message */

      q_put ( &(hdrtx.free_q), &(msg_ptr->qlink) );

    } /* if msg_ptr is not NULL */

  } /* while msg_q non-empty */

} /* hdrtx_process_msgs */


/* <EJECT> */
/*===========================================================================

FUNCTION HDRTX_WAIT_FOR_MSG

DESCRIPTION
  This function is intended to be used by entities running within hdrtx 
  task context to block hdrtx task from running until a particular msg
  is received via msg router.

DEPENDENCIES
  None.

PARAMETERS
  Message to wait for.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrtx_wait_for_msg
( 
  uint32   req_msg
)
{
  boolean msg_rcvd = FALSE;

  hdrtx_msg_s_type * rcvd_msg_ptr;

  if (rex_self () == HDRTX_TASK_PTR)
  {
    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "HDRTX task waiting for msg %x", req_msg);

    while ( !msg_rcvd )
    {
      /* Wait for signal for incoming messages. */
      hdrtx_wait(HDRTX_MSG_SIG);
  
      /* Clear signal and process messages in queue */  
      (void) rex_clr_sigs ( HDRTX_TASK_PTR, HDRTX_MSG_SIG );
  
      /* Loop till there are messages in msg_q */
      while ( q_cnt ( &(hdrtx.msg_q) ) != 0 )
      {
        /* Dequeue a message item from the queue */
        rcvd_msg_ptr = (hdrtx_msg_s_type *) q_get( &(hdrtx.msg_q) );
  
        /* Check for NULL */
        if (rcvd_msg_ptr != NULL) 
        {
          HDR_MSG_PROT_1( MSG_LEGACY_HIGH, 
            "Msg wait: received msg: %x", 
            rcvd_msg_ptr->msg.hdr.id );

          /* Forward received message to its appropriate destination */
          switch (rcvd_msg_ptr->msg.hdr.id)
          {
#if defined(FEATURE_HDR_BOLT_MODEM) && (!defined(FEATURE_HDR_UNIFIED_TX))
            case HDR_FW_TX_DAC_START_RSP:
            case HDR_FW_TX_DAC_STOP_RSP:
#endif /* FEATURE_HDR_BOLT_MODEM && !FEATURE_HDR_UNIFIED_TX */
            case HDR_FW_TX_PA_CTL_RSP:
            {
              hdrmod_process_fw_msg( rcvd_msg_ptr );
              break;
            }

#ifndef FEATURE_HDR_UNIFIED_TX
            case HDR_FW_TX_RESET_REQ_IND:
            { 

              HDR_MSG_PROT(MSG_LEGACY_HIGH, 
                           "Rcvd TX_RESET_REQ" );              

              /* If RESET_REQ was received while waiting for DAC_STOP_RSP,
                 we need to resend the DAC_STOP (original was lost). */                
              if ( req_msg == HDR_FW_TX_DAC_STOP_RSP )
              {
                hdrmod_send_tx_stop();
              }

              /* We will continue waiting for the response in this function. 
                 Post system lost to hdrlmac (hdrtx command queue), 
                 to be handled after the DAC stop sequence finishes out */
              hdrlmac_trigger_system_lost();

              break;
            }
#endif /* FEATURE_HDR_UNIFIED_TX */
    
            case HDR_FW_RMAC0_FRAME_IND:
            {
              hdrrmac_process_fw_msg( rcvd_msg_ptr );
              break;
            }
    
#if defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX)
            case HDR_FW_FLEXCONN_REPOINT_IND:
            {
              hdrrmac_process_fw_msg( rcvd_msg_ptr );
              break;
            }
#endif /* defined(FEATURE_HDR_FLEX_CONN)||defined(FEATURE_HDR_DEVICE_ONLY_DTX) */

#ifdef FEATURE_LTE_TO_HDR_OH
#ifdef FEATURE_IRAT_DSM_SUPPORT
            case NAS_EMM_IRAT_HDR_UL_MSG_CNF:
#else
            case NAS_EMM_IRAT_UL_MSG_CNF:
#endif /* FEATURE_IRAT_DSM_SUPPORT */
            {
              hdrsaptx_process_irat_ul_msg_cnf_ind( 
                &(rcvd_msg_ptr->msg.irat_ul_cnf) );
              break;
            }

#ifdef FEATURE_IRAT_DSM_SUPPORT
            case NAS_EMM_IRAT_HDR_FAILURE_IND:
#else
            case NAS_EMM_IRAT_FAILURE_IND:
#endif /* FEATURE_IRAT_DSM_SUPPORT */
            {
              hdrsaptx_process_irat_ul_msg_failure_ind( 
                &(rcvd_msg_ptr->msg.irat_failure_ind) );
              break;    
            }
#endif /* FEATURE_LTE_TO_HDR_OH */

            default:
            {
              HDR_MSG_PROT_1 ( MSG_LEGACY_ERROR, 
                "Unknown message! UMID: 0x%x", rcvd_msg_ptr->msg.hdr.id );
            }

          } /* switch - Received message */


          if( rcvd_msg_ptr->msg.hdr.id == req_msg )
          {
            /* Set msg received flag when the msg being waited for is 
               received. */
            msg_rcvd = TRUE;
          }

          q_put ( &(hdrtx.free_q), &(rcvd_msg_ptr->qlink) );

        } /* if received msg pointer in non-null*/

      } /* loop till there are items in msg queue */
  
    } /* loop till the requested msg is received */
  
    HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "Wait for msg %x complete.", req_msg);
  }
  else
  {
    HDR_MSG_PROT (MSG_LEGACY_ERROR, 
      "hdrtx_wait_for_msg() calls are only supported from HDRTX task context");
  }

} /* hdrtx_wait_for_msg */


/*===========================================================================
FUNCTION HDRTX_ENABLE_POWER_REPORTING

DESCRIPTION
  This function starts timer for reporting tx power to STX.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrtx_enable_power_reporting( void )
{
  /* Set power reporting enabled flag */
  hdrtx_power_reporting_enabled = TRUE;

  /* Start power report timer */
  (void) rex_set_timer( &hdrtx_pwr_report_timer, 
    HDRTX_PWR_REPORT_TIMER_MSECS );

  HDR_MSG_PROT( MSG_LEGACY_HIGH, 
    "HDR TX power report timer started" );

} /* hdrtx_enable_power_reporting */


/*===========================================================================
FUNCTION HDRTX_DISABLE_POWER_REPORTING

DESCRIPTION
  This function stops timer for reporting tx power to STX.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrtx_disable_power_reporting( void )
{
  /* Reset power reporting enabled flag */
  hdrtx_power_reporting_enabled = FALSE;

  /* Clear power report timer */
  (void) rex_clr_timer( &hdrtx_pwr_report_timer );

  HDR_MSG_PROT( MSG_LEGACY_HIGH, 
    "HDR TX power report timer stopped" );

} /* hdrtx_disable_power_reporting */


/*===========================================================================
FUNCTION HDRTX_TIMER_CB

DESCRIPTION
  This function is the timer callback function.

DEPENDENCIES
  None.

PARAMETERS
  timer_id - The ID of the timer that has expired

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrtx_timer_cb
(
  uint32 timer_id
)
{
  /* The callback is called in timer context when tx power report timer 
     expires. The timer expiry processing is queued to be processed by HDRTX
     task. The timer id is utilized for differentiating among different 
     HDRTX timers. */

  /* queue the timer to HDR TX */
  hdrtx_queue_timer(HDRHAI_TX, timer_id);

  return;

} /* hdrtx_timer_cb */


/*===========================================================================
FUNCTION HDRTX_REPORT_TX_POWER

DESCRIPTION
  This function reports tx power to STX and sets tx report timer if power
  reporting is enabled.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrtx_report_tx_power( void )
{
  rfm_device_enum_type      rf_device;

  cxm_stx_set_pwr_ind_s     pwr_report;

  /* Error status */
  errno_enum_type           estatus;

  /* Report power if power reporting is enabled */
  if ( hdrtx_power_reporting_enabled )
  {
    /* Initialize power report indication */
    memset ( &pwr_report, 0, sizeof(cxm_stx_set_pwr_ind_s) );
    msgr_init_hdr ( &pwr_report.hdr, MSGR_HDR_TX, 
      MCS_CXM_STX_SET_POWER_IND );
  
    /* Obtain RF device id */
    rf_device = hdrtx.tx_dev;
  
    /* Populate power report fields */
    pwr_report.tech_id = CXM_TECH_HDR;
  
    pwr_report.valid_pwrs = STX_TX_VALID_ONLY; /* Mark only tx power as valid */
  
    pwr_report.tx_pwr =                        /* Filtered total tx pilot     */
      rfm_hdr_get_filtered_tx_power(           /* power in dBm*10 units       */
      rf_device, 
      RFM_CDMA_CARRIER_CUMULATIVE,
      RFM_CDMA_AGC_FORMAT__DBM10 );
      
    /* Send power report */
    estatus = msgr_send ( &pwr_report.hdr, sizeof(pwr_report) );
    ASSERT ( estatus == E_SUCCESS );
  
    /* Set timer for the next report */  
    (void) rex_set_timer( &hdrtx_pwr_report_timer, 
      HDRTX_PWR_REPORT_TIMER_MSECS );
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_HIGH, 
      "hdrtx_report_tx_power: power reporting disabled" );
  }

} /* hdrtx_report_tx_power */


/*===========================================================================
FUNCTION HDRTX_PROCESS_TIMER

DESCRIPTION
  This function processes timer expiry event for this protocol.

DEPENDENCIES
  None.

PARAMETERS
  timer_id: Timer Id

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrtx_process_timer(uint32 timer_id)
{
  switch ( timer_id )
  {
    case HDRTX_TIMER_ID_PWR_REPORT:
      /* Send tx power report to STX */
      hdrtx_report_tx_power();
      break;

    default:
      HDR_MSG_PROT_1( MSG_LEGACY_ERROR, 
        "hdrtx_process_timer: Invalid timer id (%u)", 
        timer_id );
  }

} /* hdrtx_process_timer */

/*===========================================================================
FUNCTION HDRTX_PAUSE_FW_CMD_DELIVERY

DESCRIPTION
  This function spaces apart delivery of RMAC_ENABLE_CARRIER_CMD to FW
  such that two successive cmds are at least 2 half slots apart. This
  is needed to avoid inconsistent handling of two successive cmds in FW

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  May cause slight delay in TX establishment or teardown procedure
===========================================================================*/
void hdrtx_pause_fw_cmd_delivery()
{
  qword   curr_slot_time, cmp_result;

  /* get current time in slots */
  (void) hdrts_get_current_time (curr_slot_time, NULL);
  
  /* compare current slot time since the last call to this function */
  qw_sub ( cmp_result, curr_slot_time, hdrtx.prev_rmac_e_c_slot );

  HDR_MSG_PROT_5( MSG_LEGACY_HIGH,
                  "Current time in slots: 0x%x:%x,"
                  "prev time : 0x%x:%x cmp %d",
                  qw_hi(curr_slot_time),
                  qw_lo(curr_slot_time),
                  qw_hi(hdrtx.prev_rmac_e_c_slot),
                  qw_lo(hdrtx.prev_rmac_e_c_slot),
                  qw_lo(cmp_result));

  /* Store the current slot time for the next call */
  qw_equ ( hdrtx.prev_rmac_e_c_slot, curr_slot_time );
  
  /* Ensure this RMAC_ENABLE_CARRIER CMD is sent atleast 2slots
     after the previous one */
  if ( ( qw_hi (cmp_result ) == 0 ) && qw_lo ( cmp_result ) < 2 )
  {
    /* It is less than 2 slots since we sent last RMAC_ENABLE_CARRIER
       to FW. So let us wait to send next one */
    HDR_MSG_PROT( MSG_LEGACY_HIGH, "Wait for 1 slot");
    
    /* Busy wait for 1 slot ( 1.667 ms ).  This wait is needed
       between sending two successive RMAC_ENABLE_CARRIER messages
       to FW due to a FW limitation. */
    DALSYS_BusyWait(1667);
  }
  else
  {
    HDR_MSG_PROT( MSG_LEGACY_LOW, "No need to wait for 1 slot");
    /* It is more than 2 slots since we sent last RMAC_ENABLE_CARRIER
       to FW. So it is safe to send next one with no more delay */
  }
}

/*===========================================================================
FUNCTION HDRTX_CONFIG_MODEM_TX_POWER_STATS

DESCRIPTION
  This function enables or disables tx duration logging.
 
DEPENDENCIES
  None

PARAMETERS
  enable - Whether to start or stop logging

RETURN VALUE
  None

SIDE EFFECTS
  None
 ===========================================================================*/
void hdrtx_config_modem_tx_power_stats
(
  boolean enable
    /* Enable/disable statistic collection */  
)
{

  HDR_MSG_PROT_1(MSG_LEGACY_HIGH, "hdrtx_config_modem_tx_power_stats %d", 
                 enable );
 
  hdrtx_tx_power_duration_logging.enabled = enable;

  memset( hdrtx_tx_power_duration_logging.tx_power_bins, 0, 
          sizeof( hdrtx_tx_power_duration_logging.tx_power_bins ) );

  hdrtx_tx_power_duration_logging.tx_overall = 0;
}


/*===========================================================================
FUNCTION HDRTX_GET_MODEM_TX_POWER_STATS

DESCRIPTION
  This function fills Tx duration values into the provided table.
 
DEPENDENCIES
  None

PARAMETERS
  tx_power_bins - array of max_bins items, to be filled with duration stats.

RETURN VALUE
  None

SIDE EFFECTS
  None
 ===========================================================================*/
void hdrtx_get_modem_tx_power_stats
(
  uint32 tx_power_bins[]
    /* Pointer to the bin array */
)
{
  int i=0; 
  int num_bins = MIN ( DS3G_TX_POWER_BIN_MAX, HDTTX_NUM_MODEM_TX_POWER_BINS );

  if ( tx_power_bins == NULL  ) 
  {
    HDR_MSG_PROT(MSG_LEGACY_HIGH, "HDR Power Log: Error - NULL passed");

    return;
  }

  if ( hdrtx_tx_power_duration_logging.enabled == FALSE )
  {
     HDR_MSG_PROT(MSG_LEGACY_HIGH, "HDR Power Log: disabled");
     return;
  }

  rex_enter_crit_sect(&hdrtx.modem_tx_power_crit_sect);

  /* Cycle through bins and update appropriate one. */
  for (i=0; i<num_bins; i++)
  {
    tx_power_bins[i] = (hdrtx_tx_power_duration_logging.tx_power_bins[i] * 167 ) / 100;
      /* Convert slots to ms */  
      /* Overflow may occur if delta is > 7 hrs. This is known limitation of API. */
  }

  rex_leave_crit_sect(&hdrtx.modem_tx_power_crit_sect);

  HDR_MSG_PROT_6( MSG_LEGACY_MED, 
                  "HDR Power Log: bins:%d/%d/%d/%d/%d total:%d", 
                  tx_power_bins[0],
                  tx_power_bins[1],
                  tx_power_bins[2],
                  tx_power_bins[3],
                  tx_power_bins[4],
                  hdrtx_tx_power_duration_logging.tx_overall );

  memset( hdrtx_tx_power_duration_logging.tx_power_bins, 0,
          sizeof( hdrtx_tx_power_duration_logging.tx_power_bins ) );
  hdrtx_tx_power_duration_logging.tx_overall = 0;
    /* Clear table */  

}


/*===========================================================================
FUNCTION HDRTX_UPDATE_TX_POWER_DURATION_STATS

DESCRIPTION
  Update statistics with duration and Tx power level.
 
DEPENDENCIES
  None

PARAMETERS
  interval - Duration in slots to update.
  tx_power - Tx power level used.

RETURN VALUE
  None

SIDE EFFECTS
  None
 ===========================================================================*/
void hdrtx_update_tx_power_stats
(
   int interval,
     /* Duration in slots to update */

   int tx_power
     /* Tx power level used */
)
{
  int i=0;   /* Loop varaible */

  /* Skip if not enabled */
  if ( hdrtx_tx_power_duration_logging.enabled == FALSE )
  {
     return;
  }

  /* Find appropriate bin */
  for (i=0; i<HDTTX_NUM_MODEM_TX_POWER_BINS; i++)
  {
    if ( tx_power < hdrtx_tx_power_bin_thresh[i] )
    {
      break;
    }
  }

  /* No bin found. Unexpected, just set to max power bin. */
  if ( i==HDTTX_NUM_MODEM_TX_POWER_BINS )
  {
    i = HDTTX_NUM_MODEM_TX_POWER_BINS-1;
  }

  rex_enter_crit_sect(&hdrtx.modem_tx_power_crit_sect);

  /* Bin found, update it. */
  hdrtx_tx_power_duration_logging.tx_power_bins[i] += interval;
  hdrtx_tx_power_duration_logging.tx_overall += interval;
  
  rex_leave_crit_sect(&hdrtx.modem_tx_power_crit_sect);

}


/*===========================================================================
FUNCTION HDRTX_UPDATE_TX_POWER_DURATION_STATS

DESCRIPTION
  Update statistics with duration. Tx power level will be queried.
 
DEPENDENCIES
  None

PARAMETERS
   interval - Duration in slots to update.

RETURN VALUE
  None

SIDE EFFECTS
  None
 ===========================================================================*/
void hdrtx_update_tx_power_stats_with_duration
(
   int interval
     /* In slots */
)
{
  int tx_power = -128;              /* Tx total power */
  rfm_device_enum_type rf_dev;      /* RF device */

  if ( hdrtx_tx_power_duration_logging.enabled == FALSE )
  {
     return;
  }

  /* If Tx is on, read RF AGC. Api returns dB/10 units. */
  if ( hdrmod_pa_is_on() )
  {
    rf_dev = hdrsrchrf_get_rf_dev(HDRSRCHRF_TX_CHAIN);  
    tx_power = rfm_hdr_get_filtered_tx_power_v2( 
               rf_dev,
               RFM_TX_FILTER_POWER_WHEN_TX_ON );

    /* Update with dB units. */
    hdrtx_update_tx_power_stats( interval, tx_power / 10 );
  }

}

