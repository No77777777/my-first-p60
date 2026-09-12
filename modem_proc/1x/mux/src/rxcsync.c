/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                 C D M A    S Y N C    M O D U L E

GENERAL DESCRIPTION
  Contains items associated with the sync channel

EXTERNALIZED FUNCTIONS
  rxc_sc_isr - interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Sync Channel
  rxc_sc_init - initializes the Sync channel processing state
  rxc_sc - Sync channel processing state

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 1990 - 2021 Qualcomm Technologies, Inc.
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

$Header: //components/rel/1x.mpss/8.0/mux/src/rxcsync.c#3 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/05/21   dmd     Compiler upgradation changes (8.5.01) - atherton
01/22/20   vaa     Implement QSH MDUMP framework
11/29/16   srk     Use new events for paging and sync SW DEMBACK decode.
07/08/16   nsh     QSH:Enable event diagnostic data to QSH
08/27/15   jh      Changed common SW Demback messages from Error to Medium
02/12/15   jh      Changes for SW Demback
                   Use DEC_CLK_SYNC clock state
03/05/14   srk     Message macro reduction
03/03/14   srk     Dynamically specify stack size for ISRs.
01/28/14   pap     Aggregating the variables into global structures.
12/07/13   cjb     Changes to support DEMBACK_1
10/25/13   cjb     Avoid registering for RX ISR when FW is disabled
10/18/13   cjb     Added support to verify LC scrambling on RUMI
10/15/13   cjb     Fixed F3s that printout RAW decode status and data
10/10/13   cjb     DEMBACK0 vs DEMBACK1: Use DEMBACK ID from FW shared memory
09/27/13   vks     Use rxccommon api to configure ch0 demod for sync/paging
09/13/13   vks     Update code beta information for sync
09/11/13   vks     Use muxmdsp api to update ch0 config
08/28/13   cjb     Mainline FEATURE_MODEM_1X_DIME
07/23/13   srk     Use TCB_PTR MACRO instead of rex_self() or tcb_ptr to make
                   the code compatible with RCINIT.
05/31/13   srk     Mainlined FEATURE_MODEM_1X_NON_SIG.
02/15/13   srk     Replace mcs hwio with m1x hwio
12/19/12   bph     Replace memcpy w/ struct assignment or memscpy
09/15/12   ag      Changes for 1xCP memory optimization.
07/16/12   srk     Removed FEATURE_IS2000_REL_A_CC
04/04/12   trc     Convert to m1x_time module
03/16/12   jtm     Return RXC_BADCMD_S for illegal commands from MC.
02/17/12   srk     Replaced the tramp api with DAL Interrupt Controller.
11/22/11   srk     Feature clean up: Mainlined and Internalized Features
09/28/11   srk     Mainlined Always On features and removed Always Off Features
05/09/11   trc     Remove unnecessary encoder coupling
04/22/11   jtm     CMI Phase 4 - Final SU API changes.
04/20/11   adw     Removed unnecessary modulator config from SYNC state.
                   Added additional debug messages.
03/31/11   adw     Clear decoder done interrupt status on NikeL.
03/30/11   adw     Initialize deint and dec upon entry to sync state.
01/28/11   jtm     Additional FTM decoupling.
01/19/11   vks     More register featurization for nikel.
01/18/11   jtm     Lint clean up.
12/15/10   jtm     Feature cleanup.
12/09/10   jtm     Mainlined FEATURE_SRCH_CLKREGIM_RM and cleaned up unneeded
                   legacy clock code.
12/09/10   jtm     Remove obsolete RXC_* commands.
11/15/10   jtm     Initial SU API split for CP.
11/05/10   jtm     Added more _i.h includes for Modem 1x SU API support.
10/30/10   jtm     Modem 1x SU API changes.
07/20/10   jtm/adw Enable decoder clocks in rxc_sc_init().
06/10/10   jtm     Eliminate dependency on log_dmss.h
02/04/10   jtm     Removed FEATURE_PLT.
01/09/09   bb      Code changes for Slew state optimization feature
10/21/09   jtm     Fixed ARM compiler warnings.
09/22/09   jtm     Corrected compiler warnings.
08/22/09   jtm     Lint Fixes.
08/21/09   jtm     Lint Fixes.
08/11/09   jtm     Replaced deprecated ts.h api's with new time api's.
08/05/09   jtm     Lint fixes.
08/04/09   jtm     Replaced calls to deprecated BIO funcs with new DAL API.
06/29/09   jtm     Added include for log_dmss.h.
05/20/09   jtm     Added include for rx_v.h for CMI clean up.
04/17/09   jtm     Added includes to fix compiler warnings.
04/01/09   jtm     Include clean up.
03/30/09   jtm     Eliminated implicit includes.
03/20/09   jtm     Cleaned up header file inclusions.
03/17/09   jtm     Replaced INTLOCK/INTFREE, INTLOCK_SAV/INTFREE_SAV, and
                   MSM_LOCK/MSM_UNLOCK with macros that can be mapped correctly
                   based on the compiler and controlled internally to RXC.
11/19/08   jtm     Split various header files into internal and external
                   declarations for Common Modem Interface.
07/21/08   jtm     Klockwork Fixes and removed dead code.
07/01/08   vlc     Merged code changes from old tip archive.
04/10/08   trc     Switch clock mgmt strategy via FEATURE_SRCH_CLKREGIM_RM
03/05/08   vlc     User clock resource manager to manage RX clocks
                   (FEATURE_CLKREGIM_RM.)
06/09/06   trc     Remove T_MSM6700 featurization
09/08/05   ejv     Use new srch_mux interface calls.
07/25/05   rkc     Fixed lint errors.
07/15/05   bn      Make sure to clear the Dec status bit after install the decoder synch isr
04/27/05   vlc     Main lined code in current targets, cleaned out obsolete targets,
                   cleaned out obsolete features.
02/24/05   vlc     Lock interrupt between clearing DEC_DONE_INT_STATUS and
                   installing RXC ISR.
07/29/04   bn,rz   Clear the DEC_DONE_INT_STATUS on power up
06/24/04   sr      Added support for 2nd level dec_done interrupt in ISR
04/20/04   jrp     Fixed lint warnings.
10/22/03   sr      Fixed compilation warning errors when using RVCT compiler
06/16/03   bn      Fixed the "out of buffer dec_free_q" problem.
03/20/03   bn,sr   Added logic to support simultaneously monitor BCCH and FCCCH
03/05/03   fc      Consolidated f-csh fade timer implementation.
10/11/02   sr      Initialized rxc_isr_q in rxc_sc_init
08/06/02   cr      Added MSM6100 changes.
08/13/02   sr      Set slotted_status to AWAKE before entering BCCH state
08/13/02   sr      Added call to rxcbcch_set_first_frame_ind() in rxc_sc()
                   under REL_A_CC
02/27/02   lad     Merged in IS2000 Release A Common Channels.
11/16/01   vlc     Added call to dec_clk_select () before switching rxc state.
06/15/01   day     Delete FEATURE_SPECIAL_MDR
04/20/01   sr      Merged in from MSM5100 archive
04/19/01   tc      Merged MSM_MUX1X.01.00.01.
           fc      Renamed mcc_report_event_msg_rxed to
                   mclog_report_event_msg_rxed.
03/27/01   vlc     Merged MSM_MUX1X.00.00.17.
           fc      Added support of events report: Message received.
03/01/01   tc      Merged MSM_MUX1X.00.00.15.
02/21/01   sr      Fixes for paging channel loss problem in rxc_sc_init()
02/16/01   bgc     Changed FEATURE_FACTORY_TESTMODE to enable a runtime switch.
01/18/01   ejv     Modified some combiner macros to match new names.
01/12/01   tc      Merged MSM_MUX1X.00.00.06.
11/23/00   tc      Fixed MSM5100 PLT ftm_sc_msg_sent.
12/07/00   bgc     Commented out some unused PLT code.
11/13/00   bgc     Moved FTM sync variables to here from common.
11/10/00   jc      This file was part of rxc.c. See rxccommon.c for full
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
#include "rxc.h"
#include "rxccommon.h"
#include "rxccommon_i.h"
#include "rxcsync.h"
#include "rxcpaging.h"
#include "rxcidle.h"
#include "rxc_i.h"
#include "rxcmc_i.h"
#include "mux_logtypes.h"
#include "muxmdsp_i.h"

//drivers
#include "dalint_1x.h"
#include "enc_v.h"
#include "enc_i.h"
#include "is2k_fl_decoder.h"
#include "msm_drv.h"
#include "msm_drv_i.h"
#include "dec5000_v.h"
#include "dmod_i.h"
#include "m1x_time.h"
#include "m1x_hwio_mpss.h"

//cp
#include "caix.h"
#include "caix_i.h"
#include "rx.h"
#include "rx_v.h"
#include "mccrxtx.h"
#include "mclog.h"
#include "mclog_v.h"
#include "rxtx_v.h"
#include "cai_v.h"
#ifdef FEATURE_1XCP_QSH_SUPPORT
#include "mccqsh_i.h"
#endif /* FEATURE_1XCP_QSH_SUPPORT */

//diag
#include "cdma2kdiagi.h"
#include "onex_non_sig.h"

//srch
#include "srch_mux.h"

//other
#include "rex.h"
#include "bit.h"
#include "queue.h"
#include "err.h"
#include "log.h"
#include "crc.h"
#include "cmd.h"
#include "msg.h"
#include "m1x_diag.h"
#include "qw.h"
#include "srch_mux.h"
#include "ulpn.h"
#include "stringl.h" /* for memscpy */

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

boolean ftm_sc_msg_sent = FALSE;
/* Flag indicating if a message has been sent to PLT_MCC. The message
   notifies PLT_MCC that RXC has received a Sync Channel message */

/* Number of bytes in a Sync Channel subframe */
#define SC_SF_BYTES   ((DEC_96_BITS / 8) / 3 )

/* Number of bits in a Sync Channel subframe */
#define SC_SF_BITS    (SC_SF_BYTES * 8)

#ifdef FEATURE_SLEW_OPTIM
/* Following bit offsets and bit lengths are defined based
 * on Sync message format
 */

/* Number of bits in one Sync Channel subframe */
#define SC_SF_BITS_FRAME 93

/* Sync message length (in bits) */
#define MAX_SYNC_MSG_LEN  279

/* Maximum number of bits in Sync Message Capsule */
#define SC_SF_MAX_BITS (3 * SC_SF_BITS_FRAME )

/* system time bit offset in sync message*/
#define SYS_TIME_BIT_OFFSET  114

/* system time length (bits) in sync message */
#define SYS_TIME_BIT_LEN 36

/* Long code state bit offset in sync message */
#define LC_STATE_BIT_OFFSET 72

/* long code state (bits) in sync message */
#define LC_STATE_BIT_LEN 42

/* ULPN High Word initial value */
#define ULPN_INIT_HIGH_WORD_VALUE   0x200

#endif /* FEATURE_SLEW_OPTIM */

/* DEBUG CRC statistics */

/* number of Sync Channel messages with good CRCs */
dword rxc_sc_crc_good = 0;

/* number of Sync Channel messages with bad CRCs */
dword rxc_sc_crc_bad;

#ifdef FEATURE_SLEW_OPTIM
 /* indicates the number of decoder interrupts
    after MUX receives SC_F command from MC */
static byte decoder_int_cnt_after_sync;
#endif

/*-------------------------------------------------------------------------*/
/* States of Sync Channel parsing state machine                            */
/*-------------------------------------------------------------------------*/
typedef enum
{
  SC_HUNT_STATE,   /* Hunt for start of message */
  SC_MSG_STATE,    /* Collect bits of message */
  SC_FLUSH_STATE,  /* Discard remaining bits in subframe */

  #ifdef FEATURE_SLEW_OPTIM
  SC_SPEEDUP_STATE,/* Collect bits of message despite of SOM  bit */
  #endif /* FEATURE_SLEW_OPTIM */

  #ifdef FEATURE_MODEM_1X_RUMI
  SC_TEST_STATE,   /* Test state for 1x non-sig LC scrambling verification */
  #endif /* FEATURE_MODEM_1X_RUMI */

} rxc_sc_state_type;

/*-------------------------------------------------------------------------*/
/* Sync Channel State variables                                            */
/*-------------------------------------------------------------------------*/

struct
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*         TASK VARIABLES                                                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  byte            sync_blk[ DEC_96_BITS / 8 ];
    /* Sync channel frame read from decoder */
  rxc_sc_state_type  state;
    /* State of parsing state machine */
  word  msg_inx; /* index into message buffer */
  word  msg_len; /* number of bits in message rounded to 93 */
  byte  sav_inx; /* subframe index where start of message bit was
                     found -- passed back to the Main control task */
  #ifdef FEATURE_SLEW_OPTIM
  byte  sav_opt_inx; /* stores the index which indicates the number of
                        sync sub-frames has received before SOM bit of
                        sync message */
  #endif /* FEATURE_SLEW_OPTIM */
  rxtx_rx_msg_type *msg_ptr;
    /* pointer to buffer to place received message for Layer 2 task */

} rxc_sc_var;

#ifdef FEATURE_SLEW_OPTIM
struct
{
   byte  msg_bytes[(SC_SF_MAX_BITS + 7)/8];  /* storage for sync channel message sub-frames
                             before receiving SOM bit of sync channel
                             sub-frame */
   word msg_inx;         /* Index to Message bits */
} rxc_sc_opt_var;
#endif /* FEATURE_SLEW_OPTIM */

/* Symbol error rates returned from decoder */
dec_sc_status_type rxc_sc_status = {0, 0, 0}; /* Status(CRC + QUALITY), ser, energy */
rxc_sc_ser_type rxc_sc_ser_total = {0, 0};

/* Declarations for SW Demback */
static is2k_fl_decoder_type *pSyncDecoder = NULL;
static boolean rxc_sync_use_sw_demback = FALSE;

/****************************** RUMI ******************************/
#ifdef FEATURE_MODEM_1X_RUMI
typedef struct
{
  boolean        enable_lc_verify;
  boolean        som_detected;
  word           ser_thresh;
  caii_sync_type sync_msg;
} rxc_sc_test_type;

volatile rxc_sc_test_type rxc_sc_test_lc =
  {
    .enable_lc_verify = FALSE,
    .som_detected     = FALSE,
    .ser_thresh       = 6,
    .sync_msg         = { .msg_type = 0x1,
                          .p_rev = 0x6,
                          .min_p_rev = 0x1,
                          .sid = 0x4444,
                          .nid = 0x1111,
                          .pilot_pn = 0x0,
                          .lc_state = {0xA1D2E508, 0x00000102},
                          .sys_time = {0x7, 0x0},
                          .daylt = 0x1,
                          .cdma_freq = 758,
                          .ext_cdma_freq = 320,
                        },
  };
#endif /* FEATURE_MODEM_1X_RUMI */
/******************************************************************/

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION rxc_sc_check_pending_interrupts

DESCRIPTION
  This function handles the common processing for interrupts in the
  HW and SW ISRs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rxc_sc_check_pending_interrupts(void)
{
  boolean flag = FALSE;
  rxc_isr_cmd_type *cmd_ptr;  /* pointer to command received from the task */

  /* Check to see if there are pending commands for the interrupt handler */
  if( (cmd_ptr =
          (rxc_isr_cmd_type *)q_get(&rxc_common_data.rxc_isr_q) ) != NULL )
  {
    switch( cmd_ptr->cmd )
    {
      case RXC_SC_F:
      {
        M1X_MSG( MUX, LEGACY_MED,
          "Got RXC_SC_F");

        #ifdef FEATURE_SLEW_OPTIM
        /* reset the decoder interrupt value to zero */
        decoder_int_cnt_after_sync = 0;
        #endif /* FEATURE_SLEW_OPTIM */
        break;
      }

      default:
      {
        M1X_MSG( MUX, LEGACY_FATAL,
          "Illegal command %d",
          cmd_ptr->cmd );
      }
    } /* switch */
    /* return the memnory back to the free queue */
    q_put (&rxc_common_data.rxc_isr_free_q, &cmd_ptr->link);
    (void)rex_set_sigs ( RX_TCB_PTR, RXC_ISR_CMD_SIG );
    flag = TRUE;

  }

  return flag;

} /* rxc_sc_check_pending_interrupts */

/*===========================================================================

FUNCTION RXC_SC_ISR

DESCRIPTION
  This function is the interrupt handler for the Decoder interrupt used
  when the Receive task is processing the Sync Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  rxc_sc_isr ( void )
{
  /* pointer to buffer to be placed on rxc_dec_q */
  rxc_dec_type *buf_ptr;
    /* pointer to buffer to be placed on rxc_dec_q */

  /*-----------------------------------------------------------------------*/

  uint32 dec_int_status = 0;

  dec_int_status = GET_DEMBACK_REG( DEC_DONE_INT_STATUS );

  /* Clear DEC DONE int status to make sure ISR is properly serviced */
  SET_DEMBACK_REG( DEC_CLR_INT_STATUS,
                   HWIO_FMSK( DEC_CLR_INT_STATUS, VD_CLR_STATUS ) );

  /* Check for Rx AGC delta */
  #if defined(FEATURE_1XCP_QSH_SUPPORT) &&  \
      defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)
  if( srch_mux_is_rx_div_enabled() )
  {
    rxc_check_rx_agc_delta();
  }
  #endif /* FEATURE_1XCP_QSH_SUPPORT && 
            FEATURE_QSH_EVENT_NOTIFY_TO_QSH */

  if(dec_int_status != 0x2)
  {
    M1X_MSG( MUX, LEGACY_ERROR,
      "DEC Done Int, but not SVD! %d",
      dec_int_status);
    return;
  }

  /* increment interrupt number and frame number */
  qw_inc( rxc_dec_frame, 1L );


  if ( FALSE == rxc_sc_check_pending_interrupts() )
  {
    /* Process Decoder data */
    /*------------------------------------------------------------------------*/

    /* Read the status information for Sync Channel */
    dec_read_sc_status(&rxc_sc_status);
    rxc_sc_ser_accum();

    M1X_MSG( MUX, LEGACY_MED,
      "  SC: ser=%lx, energy=%lx, status=%d",
      rxc_sc_status.ser,
      rxc_sc_status.energy,
      rxc_sc_status.status );

    /* Peform normal interrupt processing */
    if ( (buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_free_q)) ==
             NULL )
    {
      M1X_MSG( MUX, LEGACY_ERROR,
        "No buffers on rxc_dec_free_q" );
    }

    else
    {
      /*********************** READ SYNC CHANNEL DECODED BITS **************/
      /* get data from decoder */
      dec_read_sc_data( buf_ptr->buf.sc_buf );

      #ifdef FEATURE_MODEM_1X_DEBUG_MSGS
      RX_DBG_MSG( "SYNC Data Payload = [0x%08X] [0x%08X] [0x%08X]",
        (buf_ptr->buf.sc_buf[0] << 24) | (buf_ptr->buf.sc_buf[1] << 16) |
        (buf_ptr->buf.sc_buf[2] << 8)  | (buf_ptr->buf.sc_buf[3] << 0),
        (buf_ptr->buf.sc_buf[4] << 24) | (buf_ptr->buf.sc_buf[5] << 16) |
        (buf_ptr->buf.sc_buf[6] << 8)  | (buf_ptr->buf.sc_buf[7] << 0),
        (buf_ptr->buf.sc_buf[8] << 24) | (buf_ptr->buf.sc_buf[9] << 16) |
        (buf_ptr->buf.sc_buf[10] << 8) | (buf_ptr->buf.sc_buf[11] << 0)
      );
      #endif /* FEATURE_MODEM_1X_DEBUG_MSGS */

      buf_ptr->state = RXC_SC_STATE;
        /* indicate data is from Sync Channel */

      q_put( &rxc_common_data.rxc_dec_q, &buf_ptr->link );
        /* put data on queue for task */

      (void)rex_set_sigs ( RX_TCB_PTR, RXC_INT_SIG );
        /* signal task to indicate data is on queue */
    }

  } /* if */

  /* perform AGC/power control logging */
  srch_mux_log_agc_pctl();

} /* rxc_sc_isr */

/*===========================================================================

FUNCTION RXC_SC_SW_DECODE

DESCRIPTION
  This function performs a SW decode

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the frame data.

SIDE EFFECTS
  

===========================================================================*/
is2k_fl_frame_type* rxc_sc_sw_decode( void )
{
  const uint32 num_syms = 384;  /* TODO - decoder should provide API for number
                                   of symbols */
  is2k_fl_symbol_type sync_syms[num_syms];
  is2k_fl_frame_type *pOutFrame = NULL;

  if ( pSyncDecoder != NULL )
  {
    /* Read the symbols */
    (void)muxmdsp_extract_ch0_syms( TRUE, TRUE, sync_syms, num_syms );
    /* Process the extracted symbols */
    pOutFrame = is2k_fl_decode_frame( pSyncDecoder, sync_syms, num_syms );
    (void)is2k_fl_decode_perfdump(pSyncDecoder);

    if ( pOutFrame != NULL )
    {
      uint32 *pOut = (uint32 *)pOutFrame->pOutBuffer;
      M1X_MSG( DEC, LEGACY_MED,
               "DEBUG: SYNC = [0x%X] [0x%X] [0x%X]",
               pOut[0],pOut[1],pOut[2] );
    }
  }

  return pOutFrame;
} /* rxc_sc_sw_decode */

/*===========================================================================

FUNCTION RXC_SC_SW_ISR

DESCRIPTION
  ISR for SW demback mode of operation

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  

===========================================================================*/
void rxc_sc_sw_isr  (m1x_stmr_event_type events)
{
  uint32 cnt_cx1;
  rxc_dec_type *buf_ptr;

  /* get to cx1 time */
  cnt_cx1 = (uint32)( m1x_stmr_get_scomb_chipx8_count() >> 3 );

  /* In phase 0 and less than 20ms */
  if ( (cnt_cx1 & 0x18000) == 0 && (cnt_cx1 & 0x7FFF) < 0x6000 )
  {
    M1X_MSG( MUX, LEGACY_MED,
           "rxc_sc_sw_isr FIRED; cnt_cx1=0x%X",
           cnt_cx1);

    if(FALSE == rxc_sc_check_pending_interrupts())
    {
      if ( (buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_free_q)) ==
               NULL )
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "No buffers on rxc_dec_free_q" );
      }
      else
      {

          /* get data from decoder */
        buf_ptr->state = RXC_SC_STATE_SW;
          /* indicate data is from Sync Channel */
        q_put( &rxc_common_data.rxc_dec_q, &buf_ptr->link );
          /* put data on queue for task */

        (void)rex_set_sigs ( RX_TCB_PTR, RXC_INT_SIG );
          /* signal task to indicate data is on queue */
      }
    }
  }

} /* rxc_sc_sw_isr() */

/*===========================================================================

FUNCTION RXC_SYNC_STMR_INIT

DESCRIPTION
  This funtion initializes the m1x_stmr timer resource for the sync
  channel state.  Also allocates SW demback.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
static void rxc_sync_stmr_init (void)
{
  int   err;
  /* Configure FW for SW demback. HW configuration is done in
     dec_config_clk_resources and ISR. */
  muxmdsp_set_demback_mode( FALSE, FALSE );

  /* Init the SW decoder */
  if ( NULL == pSyncDecoder )
  {
    pSyncDecoder = is2k_fl_decoder_new( IS2K_FL_SYNC );
    M1X_MSG( MUX, LEGACY_MED, "Allocated pSyncDecoder" );
  }

  err = m1x_stmr_register( M1X_STMR_CLIENT_SW_DEM, M1X_STMR_DECODE_SYNCH, rxc_sc_sw_isr );
  if(err != M1X_STMR_SUCCESS && err != M1X_STMR_ERR_REG_DUP)
  {
    ONEX_ERR_FATAL("Unable to schedule the event with err: %d", err, 0, 0);
  }

} /* rxc_sync_stmr_init */

/*===========================================================================

FUNCTION rxc_sync_stmr_deinit

DESCRIPTION
  This funtion de-initializes the action timer resource for the sync
  channel state.  Also frees the SW demback memory

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_sync_stmr_deinit( void )
{
  /* Free up resources used by SW decoder */
  if ( pSyncDecoder )
  {
    is2k_fl_decoder_free(pSyncDecoder);
    pSyncDecoder = NULL;
    M1X_MSG( MUX, LEGACY_MED, "De-Allocated pSyncDecoder" );
  }

  m1x_stmr_deregister( M1X_STMR_CLIENT_SW_DEM, M1X_STMR_DECODE_SYNCH );

} /* rxc_sync_stmr_deinit */

/*===========================================================================

FUNCTION RXC_LOG_SC_MSG

DESCRIPTION
  This function logs messages received on the Sync Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  rxc_log_sc_msg
(
  qword ts,                  /* time stamp when message was received */
  cai_sc_ext_type *msg_ptr   /* pointer to received message */
)
{
  mux_log_sc_msg_type *log_ptr;

  /*-----------------------------------------------------------------------*/

  if (msg_ptr && msg_ptr->length != 0)
  {
    log_ptr = (mux_log_sc_msg_type *)
                log_alloc (LOG_SC_MSG_C,
                           msg_ptr->length + sizeof(log_ptr->hdr));
  }
  else
  {
    log_ptr = NULL;
  }

  if (log_ptr == NULL)
  {
    rxc_log_msg_drop++;
  }
  else
  {
    /* Record timestamp from actual SC generation */
    qw_equ( log_ptr->hdr.ts, ts );

    /* copy message into buffer */
    (void)memscpy( (void *)(log_ptr->msg),
                   msg_ptr->length,
                   (void *)&(msg_ptr->length),
                   msg_ptr->length);  //lint !e613 msg_ptr has been checked non-null

    log_commit(log_ptr);
  }

  /* Event report: Message received */
  if (msg_ptr)
  {
    mclog_report_event_msg_rxed(CAI_SYNCH, msg_ptr->body);
  }

} /* rxc_log_sc_msg */


/*===========================================================================

FUNCTION RXC_SC_PARSE

DESCRIPTION
  This function collects the bits of a Sync Channel frame by parsing
  Sync Channel frames.  When it determines that an entire Sync Channel
  message has been received, and that the message CRC passes, it will place
  the message on a queue for the Layer 2 task and signal the Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  rxc_sc_parse
(
  byte                  sf_inx,     /* Subframe number */
  byte                  sframe[],   /* Address of 1st byte of subframe */
  word                  sframe_len  /* Number of bits in subframe */
)
{
  word  sframe_inx;      /* index into subframe */
  word  num_bits;        /* Number of bits to be copied to the message
                            buffer */
  static qword ts;       /* timestamp to use when logging message */
  mccrxtx_cmd_type *msg_ptr;
    /* pointer to buffer to place translated message in */
  word status;
    /* status returned by caix_ext_to_int */
  word msg_length;
    /* message length to send to caix_ext_to_int */

  #ifdef FEATURE_SLEW_OPTIM
  qword lc_state={0,0};       /* Long code state */
  qword sys_time={0,0};       /* System time */
  ulpn_type ulpn_state;   /* Long code state value */
  uint64  lc_advance = 0; /* stores the long code advance value */
  uint32  roll_overs = 0;
  boolean sys_time_changed = FALSE; /* indicates whether system time
                                       has changed or not */
  boolean lc_state_changed = FALSE; /* indicates whether long code
                                       state value has changed or not */
  word    tmp_msg_inx=0;   /* Temparory message index value */
  qword   orig_sys_time = {0,0}, orig_lc_state = {0,0}; /* holds original systime
                                                           and long code state values */
  #endif /* FEATURE_SLEW_OPTIM */

  /*-----------------------------------------------------------------------*/

 #ifdef FEATURE_SLEW_OPTIM
 /*
   After MUX receives the RXC_SC_F command from MC,
   decoder may have previously stored the sync channel
   frame bits. By considering 72 bit decoder delay for the
   newly received sync channel frame bits, ignore
   the first two decoder interrupts to store the sync
   channel message sub-frames for slew state optimization
   feature. Start storing the sync channel sub-frames, if
   decoder_int_cnt_after_sync is greater than 2.
 */
 if ( (decoder_int_cnt_after_sync == 0) ||
      (decoder_int_cnt_after_sync == 1) )
 {
   /* all sync sub-frames are parsed, increment
      decoder interrupt value */
   if((sf_inx == 2))
   {
     decoder_int_cnt_after_sync++;
   }
   return;
 }
 else
 {
   if (decoder_int_cnt_after_sync == 2)
   {
     /* initialize the slew state variables to zero */
     rxc_sc_opt_var.msg_inx = 0;
     rxc_sc_var.sav_opt_inx = 0;
     rxc_sc_var.state = SC_SPEEDUP_STATE;
     decoder_int_cnt_after_sync = 3;
     M1X_MSG( MUX, LEGACY_LOW,
       "enter the sync speed-up state");
   }
  }
  #endif /* FEATURE_SLEW_OPTIM */

  if ( sframe[0] & 0x80 )
  {
    #ifdef FEATURE_SLEW_OPTIM
    /* If SOM bit found, start parsing in the SPEEDUP
       state and don't need to change state to HUNT state,
       if the current is SPEED-UP state
     */
    if(  rxc_sc_var.state != SC_SPEEDUP_STATE)
    {
       rxc_sc_var.state = SC_HUNT_STATE;
    }
    #else /* !FEATURE_SLEW_OPTIM */
    /* Start of message bit found -- start parsing new message
       regardless of current state */
    rxc_sc_var.state = SC_HUNT_STATE;
    #endif /* FEATURE_SLEW_OPTIM */
  }

  sframe_inx = 1;  /* Skip over 1st bit (SOM). */

  while ( sframe_inx < sframe_len )  /* While still more bits. */
  {

    switch( rxc_sc_var.state )
    {
      /* ------------------------- SC_HUNT_STATE ------------------------- */
      case  SC_HUNT_STATE:
      {
        if( sframe[0] & 0x80 )
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "SOM detected" );

          /****************************** RUMI ******************************/
          #ifdef FEATURE_MODEM_1X_RUMI
          if( rxc_sc_test_lc.enable_lc_verify == TRUE )
          {
            if( rxc_sc_test_lc.sync_msg.cdma_freq == 0 )
            {
              M1X_MSG( MUX, LEGACY_ERROR,
                "SYNC-OTA copy not present. Skip test mode. "
                "Continue with regular flow" );
            }
            else if( onex_nonsig_is_ftm_mode() &&
                     ( rxc_sc_status.ser <= rxc_sc_test_lc.ser_thresh ) )
            {
              RX_DBG_MSG( "Using SYNC-OTA copy from SW. Move over to PCH" );

              /* save subframe start of message was found in */
              rxc_sc_var.sav_inx = sf_inx;

              /* Jump to TEST state */
              rxc_sc_var.state = SC_TEST_STATE;

              /* Remember SOM detection */
              rxc_sc_test_lc.som_detected = TRUE;

              break;
            }
          }
          #endif /* FEATURE_MODEM_1X_RUMI */
          /******************************************************************/

          rxc_sc_var.msg_inx = 0;

          if ( ( (rxc_sc_var.msg_len =
                  (word)(b_unpackw ( sframe, sframe_inx, 8 ) * 8)) > 0 ) &&
                 (rxc_sc_var.msg_len <= ( CAI_SC_EXT_SIZE * 8 ) ) )
          {
            /* adjust message length so that an even number of
               80 ms will be processed (80 ms = 93 bits ) */
            rxc_sc_var.msg_len = (word)( ( (rxc_sc_var.msg_len + 92)/93 ) * 93);

            M1X_MSG( MUX, LEGACY_MED,
              "  SC: length=%d",
              rxc_sc_var.msg_len );

            if  ( (rxc_sc_var.msg_ptr == NULL ) &&
                ( (rxc_sc_var.msg_ptr =
                    #ifdef FEATURE_1X_CP_MEM_OPT
                    (rxtx_rx_msg_type *)rxtx_alloc_queue_buf( RXTX_RX_Q ) ) ==
                        NULL ) )
                    #else /* !FEATURE_1X_CP_MEM_OPT */
                    (rxtx_rx_msg_type *)q_get( &rxtx_rx_free_q) ) == NULL ) )
                    #endif /* FEATURE_1X_CP_MEM_OPT */
            {
              M1X_MSG( MUX, LEGACY_ERROR,
                "No buffers on rxtx_rx_free_q" );
              /* out of buffers -- skip this frame and start looking
                for a new start of message */
              rxc_sc_var.state = SC_FLUSH_STATE;
            }
            else
            {
              /* start filling message buffer with length byte */
              b_copy ( sframe, sframe_inx,
                       (void *)(&rxc_sc_var.msg_ptr->msg.sc.length),
                       rxc_sc_var.msg_inx, 8 );
              /* copy length byte into message buffer */
              rxc_sc_var.msg_inx += 8;
              /* increment index into message buffer */
              sframe_inx += 8;
              /* increment index into subframe */
              rxc_sc_var.sav_inx = sf_inx;
                /* save subframe start of message was found in */

              rxc_sc_var.state = SC_MSG_STATE;

              (void) m1x_time_get( ts );  /* save timestamp for logging message */

            }
          }
          else
          { /* message length = 0 */

            M1X_MSG( MUX, LEGACY_MED,
              "  Bad length=%d",
              rxc_sc_var.msg_len );
            rxc_sc_var.state = SC_FLUSH_STATE;
          }

        }
        else
        { /* start of message bit not found */

          M1X_MSG( MUX, LEGACY_MED,
            "SOM not detected!" );

          rxc_sc_var.state = SC_FLUSH_STATE;
        }

        break;
      }

      /* ------------------------- SC_MSG_STATE -------------------------- */
      case SC_MSG_STATE:
      {
        M1X_MSG( MUX, LEGACY_LOW,
          "Copying SC payload" );

        num_bits = MIN ( (rxc_sc_var.msg_len - rxc_sc_var.msg_inx),
                         (sframe_len - sframe_inx) );
        /* calculate number of bits to copy into message buffer */

        b_copy ( sframe, sframe_inx, &rxc_sc_var.msg_ptr->msg.sc.length,
                  rxc_sc_var.msg_inx, num_bits );

        sframe_inx += num_bits;
        /* increment index into subframe */

        rxc_sc_var.msg_inx += num_bits;
        /* increment index into message buffer */

        #ifdef FEATURE_SLEW_OPTIM
        /* If there are any sync channel sub-frames received prior to
           SOM bit, combine them to the sync sub-frames received after
           SOM Bit after all sync channel sub-frames are received
         */

        /* ignore if only two sub-frames are received prior to the SOM bit.
         */
        if ( (rxc_sc_opt_var.msg_inx != 0) &&
                  (rxc_sc_opt_var.msg_inx <= ( (2 * SC_SF_BITS) - 2)) )
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Slew State Optim Feature has disabled");
          M1X_MSG( MUX, LEGACY_MED,
            "Number of bits received %d",
            rxc_sc_opt_var.msg_inx);

          /* Initialize the slew optimization feature variable */
          rxc_sc_opt_var.msg_inx = 0;
          rxc_sc_var.sav_opt_inx = 0;
        }

        /* Check whether complete sync message capsule has received or not */
        if ( (rxc_sc_opt_var.msg_inx != 0) &&
             ( (rxc_sc_opt_var.msg_inx + rxc_sc_var.msg_inx) >=
                   rxc_sc_var.msg_len) )
        {
          num_bits = MIN( rxc_sc_opt_var.msg_inx,
                          (rxc_sc_var.msg_len - rxc_sc_var.msg_inx) );

          M1X_MSG( MUX, LEGACY_MED,
            "Slew State Opt - Number of Bits %d Number of Frames Combined %d",
            num_bits,
            (num_bits / (SC_SF_BITS - 1)));

          /* Combine the sync message frames by copying into the
             common Sync message buffer */
          b_copy ( rxc_sc_opt_var.msg_bytes, 0,
                   &rxc_sc_var.msg_ptr->msg.sc.length,
                   rxc_sc_var.msg_inx, num_bits);

          /* store the message index value temparaily, this will be used
             to fall back on existing functionality if CRC check fails after
             combining the frames */
          tmp_msg_inx = rxc_sc_var.msg_inx;

          /* Update number of bits field */
          rxc_sc_var.msg_inx += rxc_sc_opt_var.msg_inx;

          /* Calculate the number of sub-frames received before SOM frame */
          rxc_sc_var.sav_opt_inx = (rxc_sc_opt_var.msg_inx * 3)/ SC_SF_BITS_FRAME;
          M1X_MSG( MUX, LEGACY_MED,
            "MSG - Number of subfarmes Combined %d",
            rxc_sc_var.sav_opt_inx);
        }
        #endif /* FEATURE_SLEW_OPTIM */

        if ( rxc_sc_var.msg_inx >= rxc_sc_var.msg_len )
        {

          #ifdef FEATURE_SLEW_OPTIM
          /* Slew state optimization feature has enabled and complete Sync
             message has received, before it pass it to CRC, just check
             if the system time or long code mask needs to modified
             based on number of sub-frames of previous sync message
             has combined
           */
          if (rxc_sc_var.sav_opt_inx != 0)
          {
             /* Check whether long code state needs to modify or not */
             if ( (rxc_sc_var.sav_opt_inx == 6) ||
                  (rxc_sc_var.sav_opt_inx == 3) ||
                  (rxc_sc_var.sav_opt_inx == 4) ||
                  (rxc_sc_var.sav_opt_inx == 5) )
             {
               lc_state_changed = TRUE;
             }
             /* Check whether system time needs to modify or not */
             if( (rxc_sc_var.sav_opt_inx == 3) ||
                 (rxc_sc_var.sav_opt_inx == 4) )
             {
               sys_time_changed = TRUE;
             }
          }

          /* Modify the system time */
          if (sys_time_changed == TRUE)
          {
            /* retrieve the system time value from packed sync message */
            b_unpackq( &rxc_sc_var.msg_ptr->msg.sc.length,
                       SYS_TIME_BIT_OFFSET,SYS_TIME_BIT_LEN,sys_time );

            /* retrieve the system time value from packed sync message */
            b_unpackq(&rxc_sc_var.msg_ptr->msg.sc.length,
                        SYS_TIME_BIT_OFFSET,SYS_TIME_BIT_LEN,orig_sys_time );

            /* Do changes to system time */
            qw_dec(sys_time,3);

            /* write back the modified system time value */
            b_packq( sys_time,&rxc_sc_var.msg_ptr->msg.sc.length,
                     SYS_TIME_BIT_OFFSET,SYS_TIME_BIT_LEN);
            M1X_MSG( MUX, LEGACY_HIGH,
              "changed system time");
          }

          /* Modify the long state value */
          if ( lc_state_changed == TRUE )
          {
            /* retrieve the long code state and system time value */
            b_unpackq( &rxc_sc_var.msg_ptr->msg.sc.length,
                       LC_STATE_BIT_OFFSET,LC_STATE_BIT_LEN,lc_state );
            b_unpackq( &rxc_sc_var.msg_ptr->msg.sc.length,
                       SYS_TIME_BIT_OFFSET,SYS_TIME_BIT_LEN,sys_time );

            /* retrieve the long code state and system time value */
            b_unpackq(&rxc_sc_var.msg_ptr->msg.sc.length,
                       LC_STATE_BIT_OFFSET,LC_STATE_BIT_LEN,orig_lc_state );

            /* load the initial long code state values */
            ulpn_state.high = ULPN_INIT_HIGH_WORD_VALUE;
            ulpn_state.low  = 0x0;
            lc_advance = QW_CVT_Q2N(sys_time);

            /* convert the time in slot-80ms to rolls*/
            lc_advance = 3 * lc_advance;

            /* convert into chip */
            lc_advance = lc_advance << 15;

            /* calculate the number of roll-over */
            roll_overs = (uint32 )(lc_advance >> 42);

            /* clear out the upper 22 bits, just leave the lower 42 bits */
            lc_advance &= 0x3FFFFFFFFFFULL;

            /* since our roll-overs were a bit more than the complete long code
             * cycle, specially 1 chip more than a complete cycle, we need to
             * advance the ULPN by extra chip per roll-over
             */
            lc_advance = lc_advance + roll_overs;
            ulpn_advance_chips(&ulpn_state,lc_advance);

            qw_set(lc_state,ulpn_state.high,ulpn_state.low);
            M1X_MSG( MUX, LEGACY_HIGH,
              "Modified LC state value - High %d low %d",
              ulpn_state.high,
              ulpn_state.low);

            /* Put back to the buffer */
            b_packq(lc_state,&rxc_sc_var.msg_ptr->msg.sc.length,
                                 LC_STATE_BIT_OFFSET,LC_STATE_BIT_LEN);
          }

          /* reset the slew state optimization variables */
          rxc_sc_opt_var.msg_inx = 0;
          sys_time_changed = FALSE;
          lc_state_changed = FALSE;
          #endif /* FEATURE_SLEW_OPTIM */

          /* entire message collected  -- calculate CRC */
          if ( (rxc_sc_var.msg_ptr != NULL) &&
               ( (crc_30_calc (&rxc_sc_var.msg_ptr->msg.sc.length,
                 (rxc_sc_var.msg_ptr->msg.sc.length * 8 ))) == CRC_30_OK))
          {

            rxc_sc_crc_good++;

            M1X_MSG( MUX, LEGACY_MED,
              "Good CRC: cnt=%d",
              rxc_sc_crc_good );

            /* log message */
            rxc_log_sc_msg( ts, &rxc_sc_var.msg_ptr->msg.sc );

            /* translate message to put on queue for MC */
            #ifdef FEATURE_1X_CP_MEM_OPT
            if ((msg_ptr =
                   (mccrxtx_cmd_type*) mc_alloc_queue_buf( MC_MCC_RXTX_Q )) == NULL)
            #else /* !FEATURE_1X_CP_MEM_OPT */
            if((msg_ptr = (mccrxtx_cmd_type *)q_get(&mcc_rxtx_free_q)) == NULL)
            #endif /* FEATURE_1X_CP_MEM_OPT */
            {
              #ifdef FEATURE_1X_CP_MEM_OPT
              M1X_MSG( MUX, LEGACY_ERROR,
                "No buffers on modem heap for MC_MCC_RXTX_Q" );
              #else /* !FEATURE_1X_CP_MEM_OPT */
              M1X_MSG( MUX, LEGACY_ERROR,
                "No buffers on mc_rxtx_free_q" );
              #endif /* FEATURE_1X_CP_MEM_OPT */
            }
            else
            {
              msg_length = (rxc_sc_var.msg_ptr->msg.sc.length * 8 ) -
                             CAI_SC_CRC_SIZE - CAI_SC_LGTH_SIZE;
              if ( (status = caix_ext_to_int(msg_length, CAIX_SC,
                    &rxc_sc_var.msg_ptr->msg, &msg_ptr->msg.msg ))
                    != CAIX_DONE_S )
              {
                M1X_MSG( MUX, LEGACY_MED,
                  "Bad SC translation: status %d",
                  status );

                (void) status; /* Added to satisfy compiler warning (set but not used) */

                /* Put item back on to free q */
                #ifdef FEATURE_1X_CP_MEM_OPT
                mc_free_queue_buf( msg_ptr, MC_MCC_RXTX_Q );
                #else /* !FEATURE_1X_CP_MEM_OPT */
                q_put( &mcc_rxtx_free_q, &msg_ptr->msg.hdr.cmd_hdr.link );
                #endif /* FEATURE_1X_CP_MEM_OPT */
              }
              else
              {
                if ((onex_nonsig_is_ftm_mode()) && ( !ftm_sc_msg_sent ))
                {
                  M1X_MSG( MUX, LEGACY_HIGH,
                    "RXC received Sync Channel msg" );
                }

                if ((!onex_nonsig_is_ftm_mode()) || ( !ftm_sc_msg_sent ))
                {
                  /* put the rest of the information on the queue for MC */
                  msg_ptr->msg.hdr.command = MCCRXTX_MSG_F;
                  msg_ptr->msg.chn = CAIX_SC; /* message is from sync channel */
                  msg_ptr->msg.frm.sframe = rxc_sc_var.sav_inx;

                  #ifndef FEATURE_1X_CP_MEM_OPT
                  msg_ptr->msg.hdr.cmd_hdr.done_q_ptr = &mcc_rxtx_free_q;
                  #endif /* !FEATURE_1X_CP_MEM_OPT */

                  msg_ptr->msg.hdr.cmd_hdr.task_ptr = NULL;

                  #ifdef FEATURE_SLEW_OPTIM
                  /* Copy the "number of frames combined informatio" on
                     4 MSB bits of sframe field and this information will be
                     used by searcher to speed up the slew state operations */
                  msg_ptr->msg.frm.sframe |= ( (rxc_sc_var.sav_opt_inx << 4) & 0xf0);

                  /* Move hyper space boundary by one slot to complete
                     the slew state operations by searcher and to avoid
                     TOD and system lost issues */
                  if ( (rxc_sc_var.sav_opt_inx == 3) )
                  {
                    qw_inc(msg_ptr->msg.msg.sync.sys_time,1);

                    /* Get the ULPN Value */
                    ulpn_state.high = (qw_hi(msg_ptr->msg.msg.sync.lc_state) & 0xFFFF);
                    ulpn_state.low = qw_lo(msg_ptr->msg.msg.sync.lc_state);

                    /* advance long code state by three rolls */
                    ulpn_adjust(&ulpn_state,3);
                    qw_set ( msg_ptr->msg.msg.sync.lc_state,
                             ulpn_state.high, ulpn_state.low );
                  }
                  #endif /* FEATURE_SLEW_OPTIM */

                  /* Put message on queue for MC */
                  mccrxtx_cmd( msg_ptr );
                  ftm_sc_msg_sent = TRUE;
                }
                else
                {
                  /* Put item back on to free q */
                  #ifdef FEATURE_1X_CP_MEM_OPT
                  mc_free_queue_buf( msg_ptr, MC_MCC_RXTX_Q );
                  #else /* !FEATURE_1X_CP_MEM_OPT */
                  q_put( &mcc_rxtx_free_q, &msg_ptr->msg.hdr.cmd_hdr.link );
                  #endif /* FEATURE_1X_CP_MEM_OPT */
                }

                /* For FTM, RXC should continue decoding Sync Channel frames
                   until it is told to go to some other state */
              } /* !(status = caix_ext_to_int() */
            } /* !(q_get(&mcc_rxtx_free_q))== NULL ) */
          } /* (rxc_sc_var.msg_ptr != NULL) */
          else
          {
            #ifdef FEATURE_SLEW_OPTIM
            /* if CRC has failed due to frame combining,
               then continue receiving rest of the sub-frame
               in the current sync message (fall back on
               existing functionality */
            if ( rxc_sc_var.sav_opt_inx != 0)
            {
              M1X_MSG( MUX, LEGACY_MED,
                "slew optim - crc is failed");
              /* restore the number of bits received in
                 current sync message and continue with
                 rest of the sync message sub-frames */
              rxc_sc_var.msg_inx = tmp_msg_inx;

              if ( sys_time_changed == TRUE )
              {
                /* set back original system time */
                /* write back the modified system time value */
                b_packq(orig_sys_time,&rxc_sc_var.msg_ptr->msg.sc.length,
                SYS_TIME_BIT_OFFSET,SYS_TIME_BIT_LEN);
              }

              if (lc_state_changed == TRUE )
              {
                /* Put back to the buffer */
                b_packq(orig_lc_state,&rxc_sc_var.msg_ptr->msg.sc.length,
                LC_STATE_BIT_OFFSET,LC_STATE_BIT_LEN);
              }
              sys_time_changed = FALSE;
              lc_state_changed = FALSE;

              rxc_sc_var.state = SC_MSG_STATE;

              /* reset the variables */
              rxc_sc_opt_var.msg_inx = 0;
              rxc_sc_var.sav_opt_inx = 0;
              break;
            }
            #endif /* FEATURE_SLEW_OPTIM */

            rxc_sc_crc_bad++;   /* bad CRC */

            M1X_MSG( MUX, LEGACY_MED,
              "Bad CRC, cnt=%d",
              rxc_sc_crc_bad );

          /* the buffer pointed to by rxc_sc_var will be filled up with the
             next message */
          } /* !(rxc_sc_var.msg_ptr != NULL) */

          rxc_sc_var.state = SC_FLUSH_STATE;

        } /* ( rxc_sc_var.msg_inx >= rxc_sc_var.msg_len ) */

        break;
      }

      #ifdef FEATURE_SLEW_OPTIM
      /* ------------------------ SC_SPEEDUP_STATE ----------------------- */
      case SC_SPEEDUP_STATE:
      {
        /* Check whether SOM bit is found or not */
        if( sframe[0] & 0x80 )
        {
          M1X_MSG( MUX, LEGACY_HIGH,
            "SPEEDUP - SOM bit found");
          rxc_sc_var.msg_inx = 0;
          if ( ( (rxc_sc_var.msg_len =
                 (word)(b_unpackw ( sframe, sframe_inx, 8 ) * 8)) > 0 ) &&
                 (rxc_sc_var.msg_len <= ( CAI_SC_EXT_SIZE * 8 ) ) )
          {
            M1X_MSG( MUX, LEGACY_HIGH,
              "retrieved message length %d",
              rxc_sc_var.msg_len);

            /* adjust message length so that an even number of
               80 ms will be processed (80 ms = 93 bits ) */
            rxc_sc_var.msg_len = (word)( ( (rxc_sc_var.msg_len + 92)/93 ) * 93);

            /* Check whether message length equals or not */
            if(rxc_sc_var.msg_len == MAX_SYNC_MSG_LEN)
            {
              if  ( (rxc_sc_var.msg_ptr == NULL ) &&
                    ( (rxc_sc_var.msg_ptr =
                      #ifdef FEATURE_1X_CP_MEM_OPT
                      (rxtx_rx_msg_type *)rxtx_alloc_queue_buf ( RXTX_RX_Q ) ) ==
                            NULL ) )
                      #else /* !FEATURE_1X_CP_MEM_OPT */
                      (rxtx_rx_msg_type *)q_get( &rxtx_rx_free_q) ) == NULL ) )
                      #endif /* FEATURE_1X_CP_MEM_OPT */
              {

                M1X_MSG( MUX, LEGACY_ERROR,
                  "No buffers on rxtx_rx_free_q" );

                /* out of buffers -- skip this frame and start looking
                   for a new start of message */
                rxc_sc_var.state = SC_FLUSH_STATE;
                break;

              } /* (rxc_sc_var.msg_ptr == NULL) */
              else
              {
                /* start filling message buffer with length byte */
                b_copy ( sframe, sframe_inx,
                         (void *)(&rxc_sc_var.msg_ptr->msg.sc.length),
                          rxc_sc_var.msg_inx, 8 );

                /* copy length byte into message buffer */
                rxc_sc_var.msg_inx += 8;

                /* increment index into message buffer */
                sframe_inx += 8;

                /* increment index into subframe */
                rxc_sc_var.sav_inx = sf_inx;

                /* save subframe start of message was found in */
                rxc_sc_var.state = SC_MSG_STATE;

                (void) m1x_time_get( ts );  /* save timestamp for logging message */
                break;
              } /* ! (rxc_sc_var.msg_ptr == NULL ) */
            } /* ( rxc_sc_var.msg_len == SC_SF_MAX_BITS ) */
          } /* ( rxc_sc_var.msg_len =) */

          /* Start copying sync message information */
          num_bits = (sframe_len - sframe_inx);

          M1X_MSG( MUX, LEGACY_MED,
            "Speed-up State Sframe Len %d sframe Index %d message Index(len) %d",
            sframe_len,
            sframe_inx,
            rxc_sc_opt_var.msg_inx);

          if ( (num_bits + rxc_sc_opt_var.msg_inx) < SC_SF_MAX_BITS)
          {
            /* calculate number of bits to copy into message buffer */
            b_copy ( sframe, sframe_inx, &rxc_sc_opt_var.msg_bytes,
                                   rxc_sc_opt_var.msg_inx, num_bits );

            sframe_inx += num_bits;

            /* increment index into subframe */
            rxc_sc_opt_var.msg_inx += num_bits;
          } /* ((num_bits + rxc_sc_opt_var.msg_inx) < SC_SF_MAX_BITS) */
          else
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "SOM bit wasn't found for 240ms back to SYNC state");

            /* Clear all varables */
            rxc_sc_opt_var.msg_inx = 0;

            /* start of message bit not found */
            rxc_sc_var.state = SC_FLUSH_STATE;
          } /* ! ((num_bits + rxc_sc_opt_var.msg_inx) < SC_SF_MAX_BITS) */
        } /* (sframe[0] & 0x80) */
        else /* SOM bit isn't found */
        {
          /* Start copying sync message information */
          num_bits = (sframe_len - sframe_inx);

          if ( (num_bits + rxc_sc_opt_var.msg_inx) < SC_SF_MAX_BITS)
          {
            /* calculate number of bits to copy into message buffer */
            b_copy ( sframe, sframe_inx, &rxc_sc_opt_var.msg_bytes,
                                       rxc_sc_opt_var.msg_inx, num_bits );

            sframe_inx += num_bits;
            /* increment index into subframe */
            rxc_sc_opt_var.msg_inx += num_bits;
          } /* (num_bits + rxc_sc_opt_var.msg_inx) < SC_SF_MAX_BITS) */
          else
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "SOM bit wasn't found for 240ms, back to sync state");

            /* Clear all varables */
            rxc_sc_opt_var.msg_inx = 0;

            /* start of message bit not found */
            rxc_sc_var.state = SC_FLUSH_STATE;
          } /* !(num_bits + rxc_sc_opt_var.msg_inx) < SC_SF_MAX_BITS) */
        } /* !(sframe[0] & 0x80) */

        break;
      } /* case end */
      #endif /* FEATURE_SLEW_OPTIM */

      /* ------------------------ SC_FLUSH_STATE ------------------------- */
      case SC_FLUSH_STATE:
      {
        sframe_inx = sframe_len; /* Indicate all bits used up. */
        rxc_sc_var.state = SC_HUNT_STATE;

        break;
      }

     /******************************** RUMI ********************************/
      #ifdef FEATURE_MODEM_1X_RUMI
      /* ------------------------- SC_TEST_STATE ------------------------- */
      case SC_TEST_STATE:
      {
        M1X_MSG( MUX, LEGACY_MED,
          "RUMI: SC_TEST_STATE handler" );

        /* log SW copy of fake SYNC-OTA message */
        M1X_MSG( MUX, LEGACY_MED,
          "********************** Fake SYNC-OTA *************************" );

        M1X_MSG( MUX, LEGACY_MED,
          "SID=0x%X, NID=0x%X, PILOT_PN=%u, cdma_freq=%u, ext_cdma_freq=%u",
          rxc_sc_test_lc.sync_msg.sid,
          rxc_sc_test_lc.sync_msg.nid,
          rxc_sc_test_lc.sync_msg.pilot_pn,
          rxc_sc_test_lc.sync_msg.cdma_freq,
          rxc_sc_test_lc.sync_msg.ext_cdma_freq );

        M1X_MSG( MUX, LEGACY_MED,
          "LC_STATE=[0x%08X%08X], SYS_TIME=[0x%08X%08X]",
          ((uint32*)rxc_sc_test_lc.sync_msg.lc_state)[1],
          ((uint32*)rxc_sc_test_lc.sync_msg.lc_state)[0],
          ((uint32*)rxc_sc_test_lc.sync_msg.sys_time)[1],
          ((uint32*)rxc_sc_test_lc.sync_msg.sys_time)[0] );

        M1X_MSG( MUX, LEGACY_MED,
          "**************************************************************" );

        if((!onex_nonsig_is_ftm_mode()) || ( !ftm_sc_msg_sent ))
        {
          /* translate message to put on queue for MC */
          #ifdef FEATURE_1X_CP_MEM_OPT
          if ((msg_ptr = (mccrxtx_cmd_type*) mc_alloc_queue_buf( MC_MCC_RXTX_Q )) == NULL)
          #else /* !FEATURE_1X_CP_MEM_OPT */
          if((msg_ptr = (mccrxtx_cmd_type *) q_get(&mcc_rxtx_free_q)) == NULL)
          #endif /* FEATURE_1X_CP_MEM_OPT */
          {
            #ifdef FEATURE_1X_CP_MEM_OPT
            M1X_MSG( MUX, LEGACY_ERROR,
              "No buffers on modem heap for MC_MCC_RXTX_Q" );
            #else /* !FEATURE_1X_CP_MEM_OPT */
            M1X_MSG( MUX, LEGACY_ERROR,
             "No buffers on mc_rxtx_free_q" );
            #endif /* FEATURE_1X_CP_MEM_OPT */
          }
          else
          {
            /* put the rest of the information on the queue for MC */
            msg_ptr->msg.hdr.command = MCCRXTX_MSG_F;
            msg_ptr->msg.chn = CAIX_SC; /* message is from sync channel */
            msg_ptr->msg.frm.sframe = rxc_sc_var.sav_inx;

            #ifndef FEATURE_1X_CP_MEM_OPT
            msg_ptr->msg.hdr.cmd_hdr.done_q_ptr = &mcc_rxtx_free_q;
            #endif /* !FEATURE_1X_CP_MEM_OPT */

            msg_ptr->msg.hdr.cmd_hdr.task_ptr = NULL;

            /* Use the SC message from the stored copy
               NOTE: We will reach here only if the SYNC-OTA copy is valid */
            msg_ptr->msg.msg.sync = rxc_sc_test_lc.sync_msg;

            /* Put message on queue for MC */
            mccrxtx_cmd( msg_ptr );
            ftm_sc_msg_sent = TRUE;

            /* For FTM, RXC should continue decoding Sync Channel frames
               until it is told to go to some other state */

          } /* !(q_get(&mcc_rxtx_free_q))== NULL ) */

        } /* (!onex_nonsig_is_ftm_mode()) || ( !ftm_sc_msg_sent ) */

        /* To break-out of while loop */
        rxc_sc_var.state = SC_FLUSH_STATE;

        break;
      }
      #endif /* FEATURE_MODEM_1X_RUMI */
      /*********************************************************************/

      /* ---------------------------- DEFAULT ---------------------------- */
      default:
      {
        ONEX_ERR_FATAL("Invalid sc parse substate %d",(int) rxc_sc_var.state,0,0 );
      }

    } /* switch */
  } /* while */

} /* rxc_sc_parse */


/*===========================================================================

FUNCTION RXC_SC_DATA

DESCRIPTION
  This procedure processes the bits received from the decoder while the
  Receive task is processing the Sync Channel.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  rxc_sc_data( void )
{
  byte sub_inx;                   /* index of subframe */
  rxc_dec_type *buf_ptr;          /* pointer to buffer received from the 
                                     rxc_dec_q */
  is2k_fl_frame_type* pFrame;     /* pointer to output frame */
  byte* pDecodedFrame = NULL;     /* pointer to SYNC frame data */

  /*-----------------------------------------------------------------------*/

  (void)rex_clr_sigs( RX_TCB_PTR, RXC_INT_SIG );
  while( (buf_ptr = (rxc_dec_type *)q_get( &rxc_common_data.rxc_dec_q ) ) !=
            NULL )
  {
    if( buf_ptr->state == RXC_SC_STATE_SW )
    {
      pFrame = rxc_sc_sw_decode();

      if (pFrame)
      {
        pDecodedFrame = pFrame->pOutBuffer;
        rxc_sc_status.ser    = (word)pFrame->symbol_errors;
        rxc_sc_status.energy = (int4)pFrame->symbol_energy;
      }
    }
    else if( buf_ptr->state == RXC_SC_STATE )
    {
      pDecodedFrame = buf_ptr->buf.sc_buf;
    }

    if (pDecodedFrame)
    {
      /* buffer is a Sync Channel buffer */
      /* Note that there is a 72 bit delay in the decoder so that we
         need to reassemble the frame by combining the first 72 bits
         of the current frame with the last 24 bits of the previous frame */
      b_copy ( pDecodedFrame, 0, rxc_sc_var.sync_blk, 24, 72 );
      /* Copy remaining portion of current frame to buffer. */
      for ( sub_inx = 0; sub_inx < 3; sub_inx++ )
      {
        /* Send bits to Sync Channel message processor */
        rxc_sc_parse( sub_inx,
                      &rxc_sc_var.sync_blk[ sub_inx * ((DEC_96_BITS/8)/3) ],
                      SC_SF_BITS );

        /****************************** RUMI ******************************/
        #ifdef FEATURE_MODEM_1X_RUMI
        if( rxc_sc_test_lc.som_detected == TRUE )
        {
          rxc_sc_test_lc.som_detected = FALSE;
          break;
        }
        #endif /* FEATURE_MODEM_1X_RUMI */
        /******************************************************************/
      }

      /* Copy first piece of next frame to buffer. */
      b_copy ( pDecodedFrame, 72, rxc_sc_var.sync_blk, 0, 24 );
    }

    /* Free up queue item */
    q_put( &rxc_common_data.rxc_dec_free_q, &buf_ptr->link );
    (void)rex_clr_sigs( RX_TCB_PTR, RXC_INT_SIG );
  }

} /* rxc_sc_data */

/*===========================================================================

FUNCTION RXC_SC_INIT_ISR

DESCRIPTION
  This function is registers the interrupt handler.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_sc_init_isr(boolean use_sw_demback)
{
  if (use_sw_demback == TRUE)   /* Setup SW demback */
  {
    rxc_sync_stmr_init(); 
  }
  else  /* Setup HW demback */
  {
    RXC_ENTER_ATOMIC_SECTION();

    /* register interrupt for Sync Channel data processing */
    rxc_register_isr( (dalint_1x_isr) rxc_sc_isr, DALINT_1X_STACK_4K );

    /* Clear DEC DONE int status to make sure ISR is properly serviced. */
    SET_DEMBACK_REG(DEC_CLR_INT_STATUS, HWIO_FMSK(DEC_CLR_INT_STATUS,VD_CLR_STATUS));

    RXC_LEAVE_ATOMIC_SECTION();

    deint_set_sc_mode();  /* Set deint/dec to sync mode */
  }

} /* rxc_sc_init_isr */

/*===========================================================================

FUNCTION RXC_SC_INIT

DESCRIPTION
  This funtion initializes the Sync channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rxc_sc_init
(
  rxc_state_type curr_state
)
{
  /*-----------------------------------------------------------------------*/

  dec_config_clk_resources(DEC_CLK_SYNC);

  rxc_sync_use_sw_demback = rxc_use_sw_demback();

  if (FALSE == rxc_sync_use_sw_demback )
  {
    /* Configure FW for HW demback. SW configuration is done in
       rxc_sync_stmr_init. */
    muxmdsp_set_demback_mode( TRUE, FALSE );

    /* Initialize the CDMA deinterleaver. Decoder will also be initialized. */
    deint_init();
  }

  /* Configure for demod channel 0 */
  rxc_config_demod_for_sync();

  switch( curr_state )
  {
    case RXC_SC_STATE:
      if(!onex_nonsig_is_ftm_mode())
      {
        ONEX_ERR_FATAL("Bad state %d", (int) curr_state, 0, 0 );
      }
      /* For FTM_MODE, fall through */
    case RXC_CDMA_STATE:
    case RXC_PC_STATE:
    case RXC_TC_STATE:
    {
      /* initialize variables for Sync Channel parsing state machine */
      rxc_sc_var.state = SC_HUNT_STATE;

      /* initialize sync channel crc count variables */
      rxc_sc_crc_good = 0;
      rxc_sc_crc_bad = 0;

      /* to appease the Lint Gods.*/
      MODEM_1X_UNUSED(rxc_sc_crc_good);
      MODEM_1X_UNUSED(rxc_sc_crc_bad);

      /* initialize ser totals */
      rxc_ser_reset();

      /* clear the frame offset */
      rxc_set_frame_offset(0);

      /* register interrupt for Sync Channel data processing */
      rxc_sc_init_isr(rxc_sync_use_sw_demback);


      /* Clear any unprocessed commands on the ISR queue.
       * This covers the case when an ISR command, say, RXC_WAKE_F,
       * has been queued up, but before it can be finished, i.e., before
       * SRCH reinstalls the decoder ISR, SRCH fails to reacquire after
       * sleep. In this case the command lingers on the queue and the
       * next ISR command (probably  the one below) is going to try to
       * queue up rxc_isr_cmd_buf again resulting in a messed up rxc_isr_q
       * Need to free all the lingering item on the queue
      */
      rxc_clear_isr_cmd_queue();

      rxc_common_data.rxc_isr_cmd_buf_ptr =
            (rxc_isr_cmd_type *) q_get (&rxc_common_data.rxc_isr_free_q);
      if (rxc_common_data.rxc_isr_cmd_buf_ptr != NULL)
      {
        rxc_common_data.rxc_isr_cmd_buf_ptr->cmd = RXC_SC_F;
        /* send command to interrupt handler to initialize SC mode */
        rxc_isr_cmd(rxc_common_data.rxc_isr_cmd_buf_ptr);
      }
      else
      {
        M1X_MSG( MUX, LEGACY_ERROR,
          "No buffers on rxc_isr_free_q" );
      }
      break;
    }

    default:
    {
      ONEX_ERR_FATAL("Bad state %d", (int) curr_state, 0, 0 );
    }
  } /* switch */

} /* rxc_sc_init */


/*===========================================================================

FUNCTION RXC_SC_EXIT

DESCRIPTION
  This funtion performs processing necessary to end the Sync channel state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void  rxc_sc_exit( void )
{
  /*-----------------------------------------------------------------------*/

  ftm_sc_msg_sent = FALSE;  /* Reset the flag */

  /* Put pending messages back on the free q */
  if (rxc_sc_var.msg_ptr != NULL)
  {
    #ifdef FEATURE_1X_CP_MEM_OPT
    RXTX_FREE_QUEUE_BUF( rxc_sc_var.msg_ptr, RXTX_RX_Q );
    #else /* !FEATURE_1X_CP_MEM_OPT */
    q_put(&rxtx_rx_free_q, &rxc_sc_var.msg_ptr->link );
    rxc_sc_var.msg_ptr = NULL ;
    #endif /* FEATURE_1X_CP_MEM_OPT */
  }

  /* Deinit the SW ISR */
  rxc_sync_stmr_deinit();

} /* rxc_sc_exit */


/*===========================================================================

FUNCTION RXC_SC

DESCRIPTION
  Sync channel processing state.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Next state to be processed.

===========================================================================*/
rxc_state_type  rxc_sc( void )
{
  /* next state to be processed */
  rxc_state_type next_state;

  /* pointer to command buffer received on rxc_cmd_q */
  rxc_cmd_msg_type *cmd_ptr;

  /*-----------------------------------------------------------------------*/

  next_state = RXC_SC_STATE;
  while (next_state == RXC_SC_STATE)
  {
    /* check for received interrupts */
    rxc_sc_data();

    /* Check for received messages */
    (void)rex_clr_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );

    cmd_ptr = (rxc_cmd_msg_type *) q_get (&rxc_common_data.rxc_cmd_q);

    if (cmd_ptr != NULL)
    {
      switch (cmd_ptr->hdr.command)
      {
        case RXC_EXIT_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_EXIT_F");
          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state = RXC_EXIT_STATE;
          break;
        }

        case RXC_IDLE_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_IDLE_F");
          rxcidle_init();
          if (cmd_ptr->idle.chan_info.chan_id == CAI_PCH)
          {
            /* Clean up SW Demback ISR */
            rxc_sync_stmr_deinit();

            rxc_pc_init (RXC_SC_STATE,
              cmd_ptr->idle.chan_info.config.pch.rate);
            next_state = RXC_PC_STATE;
            cmd_ptr->hdr.status = RXC_DONE_S;
          }
          else
          {
            M1X_MSG( MUX, LEGACY_ERROR,
              "Failed to transition to PC" );
            cmd_ptr->hdr.status = RXC_BADCMD_S;
          }

          break;
        }

        case RXC_CDMA_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_CDMA_F");
          /* --------------------------------------------------
          ** Transition back to CDMA init state without exiting
          ** rxc_subtask()
          ** -------------------------------------------------- */

          /* Clean up SW Demback ISR */
          rxc_sync_stmr_deinit();   
                 
          /* --------------------------------------
          ** Clean up before switching to CDMA mode
          ** -------------------------------------- */
          rxc_exit();

          cmd_ptr->hdr.status = RXC_DONE_S;
          next_state = RXC_CDMA_STATE;
          break;
        }

        case RXC_SC_F:
        {
          M1X_MSG( MUX, LEGACY_MED,
            "Got RXC_SC_F");
          if ( onex_nonsig_is_ftm_mode() )
          {
            /* --------------------------------------
            ** Clean up before switching to CDMA mode
            ** -------------------------------------- */

            cmd_ptr->hdr.status = RXC_DONE_S;

            /* Place item on queue if requested */
            cmd_done( &cmd_ptr->hdr.cmd_hdr );

            (void)rex_clr_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );

            /* Perform exit processing */
            rxc_sc_exit();

            rxc_sc_init( next_state );
            /* Initialize Sync Channel processing state. We will return
               to this state */

            return( next_state );
          }
          /* Fall through for mode == DMSS */
        }

        default:
        {
          M1X_MSG( MUX, LEGACY_ERROR,
            "Illegal RX command %d",
            cmd_ptr->hdr.command);
          cmd_ptr->hdr.status = RXC_BADCMD_S;
        }
      } /* switch */

      /* Place item on queue if requested */
      cmd_done( &cmd_ptr->hdr.cmd_hdr );

      (void)rex_clr_sigs( RX_TCB_PTR, RXC_CMD_Q_SIG );
    }
    else
    {

      (void)rxc_wait( RXC_CMD_Q_SIG | RXC_INT_SIG );
        /* wait for a command or for an interupt */
    }
  } /* while */

  /* Perform exit processing */
  rxc_sc_exit();

  return( next_state );

} /* rxc_sc */

