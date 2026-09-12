/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                    H D R    M O D U L A T O R    D R I V E R

GENERAL DESCRIPTION
  This module contains the High Data Rate Modulator driver.
  
EXTERNALIZED FUNCTIONS

  hdrmod_reset
    -Places modulator (and PA) in known state.
  hdrmod_set_pn_long_code_mask
    -Set the I&Q long code mask.
  hdrmod_set_tx_frame_start_offset
    -Set the slot offset on which transmit begins (relative to frame).
  hdrmod_set_tx_frame_interrupt_offset
    -Set the slot offset on which to receive Tx interrupt.
  hdrmod_set_rri
    -Set RRI for the next frame to be transmitted.    
  hdrmod_enter_access_mode
    -Sets up PA for an access attempt.
  hdrmod_exit_access_mode
    -Returns PA to normal state.
  hdrmod_setup_access_probe
    -Sets up PA for an access probe.
  hdrmod_begin_access_probe
    -Begins access probe transmission.
  hdrmod_end_access_probe
    -Ends access probe transmission.
  hdrmod_enable_pa
    -Turns on PA for reverse traffic.
  hdrmod_disable_pa
    -Turns off pa.
  hdrmod_pa_is_on
    -Returns current state of PA.
  hdrmod_reset_reverse_frame_count
    -Resets internal count of frames for each rate that have been sent.
  hdrmod_get_reverse_frame_count
    -Fills in structure with frame count data.
  
  
  Copyright (c) 2000 - 2015 by Qualcomm Technologies, Inc.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/drivers/src/hdrmod.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/10/15   vko     Removed baned function memcpy
04/08/15   wsh     Check for NULL before invoking hdrmod.isr_ptr
03/27/15   wsh     Fixed processing AMAC ISR after RMAC is active 
02/25/15   wsh     Added sync. to prevent DAC start/ASDiv race cond. 
02/19/15   wsh     Fixed crash when ASDiv is sent between DAC start/resp
12/23/14   sat     Changes to start Flex capture for RUMI Tx
12/16/14   wsh     Removed unused function to fix compile warning
10/14/14   wsh     Changes to support Jolokia bringup
11/27/13   arm     Fixed compilation error due to tDTX.
07/10/14   rmv     Check actual PA status before issuing DAC_STOP. Updated 
                   the condition for resetting request pending flag.
06/09/14   kss     Added support for handling TX_RESET_REQ_IND processing. 
05/27/14   wsh     Fixed compile error for Jolokia build
04/08/14   bb      Graceful handling of multiple PA_CTL_RSP while waiting
                   for PA OFF 
02/26/14   mbs     Corrected usage of frame offset for BOLT
02/11/14   mbs     Updated VSTMR event table when an interrupt offset changed
11/26/13   rmg     Supported enabling carriers in FW without a frame delay.
11/21/13   mbs     Added support for enabling VSTMR after STMR block is clocked
11/12/13   mbs     Protected VSTMR registrations
11/08/13   mbs     Supported rotation of all 42 bits in long code mask
10/08/13   mbs     Added support for BOLT interface changes
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
08/06/13   rmg     Added API to wait for PA off.
02/17/13   arm     Supported network based dtx. 
05/06/13   mbs     Fixed compiler errors for BOLT build
04/05/13   rmg     Added delay before prefill start.
03/26/13   smd     Fixed RevB MC call.
03/15/13   smd     Fixed compiler error for Triton.
03/15/13   smd     Waited 27ms after DAC_STOP (temp fix for Triton Tx).
03/12/13   smd     Added Triton fixes.
02/11/13   vko     Fixed compiler warnings
12/19/12   smd     Added Triton Tx fixes. 
01/17/13   rmg     Added support for seperating modulator config from PA ctrl.
                   Added change to send PA off in DAC stop only if immediate 
                   PA off command has not been sent before.
12/04/12   mbs     Added unified TX related changes for Triton
11/07/12   dsp     Removed prefill disable just before prefill start.
10/30/12   rmg     Added change to disable all carriers in PA off cmd before
                   DAC stop.
10/16/12   ljl     Prefill disable just before prefill start.
08/21/12   dsp     Bio switch change for Dime bringup.
08/15/12   ljl     Added FTM support.
04/26/12   rmg     Added wait for PA off response before turning off DAC.
04/25/12   kss     Postpone disable of carriers in modulator hw.
04/03/12   rmg     Shifted half frame ISR firing slot from 8 to 10.
03/20/12   cnx     Resolved TCB compiler errors for Dime. 
03/19/12   kss     Added updates for Dime. 
02/15/12   rmg     Added support for storing PA status reported in PA ON CTL
                   RSP.  Removed wait for pending PA ON CTL CMD to finish 
                   when a new command is queued.
01/05/12   wsh     Added support for SVLTE+SHDR
01/31/12   rmg     Changed translation of immediate PA enable from two slots 
                   to three.
01/19/12   kss     Removed MOD_STALL_SEL and MOD_TXC0_STB_SEL (set in TxLM). 
01/26/12   ljl     Replaced tramp with DAL.
01/04/12   rmg     Added support for HDR_FW_TX_PA_ON_CTL response.
12/01/11   rmg     Added workaround to compiler bug causing tx halfslot index
                   to be incorrectly supplied as 0 to FW in PA ON ctrl msg.
11/16/11   rmg     Added a 1 slot wait before DAC trigger.
08/05/11   rmg     Corrected carrier mask setting in timed_disable().
06/21/11   rmg     Made changes to process tx_start/stop commands in hdrtx ctxt
06/09/11   rmg     Use masked writes to MOD_STALL_SEL (for SVDO)
06/07/11   kss     Use masked writes to MOD_MODE (for SVDO)
05/26/11   rmg     Removed setting of REVMOD_PREFILL_CTL to 0 for prefill.
05/24/11   kss     Remove control of MODEM_PA_CTL/MODEM_TX_CTL.
05/19/11   kss     Corrected tx trigger time calculation.
05/19/11   rmg     Updated HDR MCPM interface.
04/27/11   rmg     Added HDR MCPM support changes.
04/27/11   kss     Added hdrmod_tx_start() and hdrmod_tx_stop(). 
04/28/11   kss     Renamed FW TX PA enable message.
04/16/11   kss     Supported FW-controlled Tx enable.
03/16/11   kss     Nikel modulator updates. 
02/01/11   rmg     Added HWIO changes for NikeL builds.
01/10/11   kss     Disable modulator DO mode in hdrmod_exit(). 
08/17/10   kss     Revert previous changes (TXR reset)
04/30/10   kss     Voyager 2.0 modulator changes. 
03/23/10   kss     Updated for Voyager 2.0 registers. 
12/20/09   kss     Fixed hdrmod_reset rename from 12/08.
12/14/09   kss     Configure TX_MODE for cdma. 
12/08/09   kss     Renamed hdrmod_reset() as hdrmod_enter() for clarity.
12/07/09   kss     Configured TX RAM clock source.
12/01/09   kss     Added modulator debug test control functionality.
10/04/09   kss     Removed MDSP mode config. This is the responsibility of 
                   the caller.  
07/13/09   kss     Update target checks for 9k.
05/17/09   rkc     Use diff registers for PA_ON_EN and TX_ON_EN for Poseidon2.
05/13/09   rkc     Used MODEM_TX_STATUS to get TX_ON status on Poseidon2.
04/24/09   rkc     Added Poseidon2 MODEM_TXDAC_SLEEP support.
04/16/09   rkc     Mainlined T_MSM6800B.
05/28/08   kss     Fixed problem with Tx_punc bit. Previous wkaround removed.
05/12/08   rmg     Removed unnecessary carrier 0 enables.
01/28/08   kss     Remove bring-up hack in hdrmod_reset().
09/25/07   rmg     Added changes to support RevB RMAC DRV.
09/14/06   kss     Added missing update of PA state in hdrmod_end_access_probe.
03/06/06   ljl     Updated to support T_MSM6800B
12/05/05   kss     Use T_MSM7500B for 7500 support.
11/07/05   kss     Turn on TX_ON before PA_ON.
10/10/05   kss     Added MSM7500 support.
10/10/05   kss     Disable/enable TX_DAC_SLEEP during reset and exit.
04/25/05   kss     Clean up register access calls (HWIO, etc.).
04/22/05   kss     Switch PA control back to 1x on exit.
03/29/05   kss     Use FEATURE_HDR_REVA_L1.
02/23/05   kss     Remove code related to second level frame interrupt.
                   (Support was added to tramp.)
02/10/05   kss     Initial changes for Rev A support.
11/12/04   kss     Changes for MSM6800 Rev 0 support.
01/05/03   kss     Make hdrmod_exit() a no-op; 1x enc driver will handle mode
                   switch to 1x when needed.
12/08/03   kss     Ensure PA_ON and TX_ON are off in hdrmod_reset.
11/14/03   kss     Corrected PA_PUNCT settings for disable PA functions.
08/29/03   kss     Corrected TX_PUNCT settings for disable PA functions.
02/11/02   kss     Added interface for setting LC mask to zero (for test).
09/12/01   kss     Added hdrmod_enable_pa_on2().
06/07/01   kss     Turn RRI off for probes; removed frame offset hacks.
04/24/01   kss     Ported for MSM5500.
12/19/00   kss     Added HDRMOD_USE_GPIO_FOR_PA
11/06/00   kss     Added interface for tracking frame rates.
10/23/00   kss     Corrected several frame size definitions.
05/24/00   kss     Created.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "hdr_variation.h"
#include "comdef.h"
#include "hdrmod.h"
#include "hdrmodi.h"

#ifndef FEATURE_HDR_VI_VERIFICATION
#ifdef FEATURE_HDR_BOLT_MODEM
#include "hdrts.h"
#else /* FEATURE_HDR_BOLT_MODEM */
#include "DALDeviceId.h"
#include "DDISoftwareSwitches.h"
#include "DALSys.h"               /* DAL_DeviceAttach() */
#include "DALStdErr.h"            /* DAL_SUCCESS */
#include "DDIInterruptController.h"
#include "DalDevice.h"
#include "tramp.h"
#endif /* FEATURE_HDR_BOLT_MODEM */
#include "msm.h"

#include "task.h"
#include "rex.h"
#include "hdrdebug.h"
#include "hdrtx.h"
#include "qw.h"
#include "err.h"
#include "hdrtrace.h"
#include "hdrsrchrf.h"
#include "txlm_intf.h"
#endif  /* FEATURE_HDR_VI_VERIFICATION */

#include "hdrmsg.h"                   /* F3 messaging */
#include "hdrfw_msg.h"                /* FW messaging */
#include "hdrmdspmcr.h"               /* HSTR read from FW */

#ifdef FEATURE_FACTORY_TESTMODE
#include "hdrhitmsg.h"
#include "ftm.h"
#endif

#ifdef FEATURE_HDR_SELECTIVE_TXD
#include "hdrsrchrftxd.h"
#endif /* FEATURE_HDR_SELECTIVE_TXD */

/* For defining slot times, in chips */
#define HDRMOD_ONE_SLOT_IN_CHIPS     ( 2048 )
#define HDRMOD_8_SLOTS_IN_CHIPS      ( HDRMOD_ONE_SLOT_IN_CHIPS * 8 )
#define HDRMOD_15_SLOTS_IN_CHIPS     ( HDRMOD_ONE_SLOT_IN_CHIPS * 15 )
#define HDRMOD_CHIPS_PER_MS          ( 1228 )

/* TBD this needs to be updated for Dime */
#define HDR_FRAME_INT_ID    106 /* TBD to confirm whether this is the correct vector may get from DAL */

#ifndef FEATURE_HDR_VI_VERIFICATION
/* Tracks number of frames sent for each rate */
uint32 hdrmod_frame_count[HDRMOD_NUM_REVERSE_RATES];

/* Task context block pointer. */
#define HDRMOD_TASK_PTR HDRTX_TASK_PTR

/* Task command queue function */
#define HDRMOD_QUEUE_CMD( protocol, cmd_ptr, size )\
  ( hdrtx_queue_cmd( protocol, cmd_ptr, size ) )

DalDeviceHandle                   *hdrmod_hInthandle;
  /* DAL devie handle */
#endif  /* FEATURE_HDR_VI_VERIFICATION */

/* Track last requested PA status */
LOCAL boolean hdrmod_pa_on = FALSE;

/* Track whether use of PA (tx) has been requested */
LOCAL boolean hdrmod_access_mode = FALSE;

/* Track whether use of PA (tx) has been requested */
LOCAL boolean hdrmod_pa_needed = FALSE;

/* Track last reported PA status in PA ON CTL response msg by FW */
typedef enum
{
  HDRMOD_PA_STATUS_OFF = 0,
  HDRMOD_PA_STATUS_ON  = 1,
} hdrmod_pa_status_type;

LOCAL hdrmod_pa_status_type hdrmod_pa_status = HDRMOD_PA_STATUS_OFF;

/* Track if PA on/off is pending - used in timed enable/disable and DAC stop.*/
LOCAL struct 
{
  boolean                pending;    /* TRUE if PA on/off is pending. */
  hdrmod_pa_status_type  request;    /* Whether PA on or PA off. */
  boolean                immediate;  /* TRUE if the last send request 
                                        had immediate field set. */
} hdrmod_pending_pa_request;

/*  Indicates the use of test Long Code Mask -- useful for test modes */
LOCAL boolean hdrmod_use_lc_mask = FALSE;

/* Test long code mask defaults to zero. */
typedef struct {
  uint32 i_hi;
  uint32 i_lo;
  uint32 q_hi;
  uint32 q_lo;
} hdrmod_test_lc_mask_type;

hdrmod_test_lc_mask_type hdrmod_test_lc_mask = {0x3ff, 0, 0x3fe, 1};

/* Track Rev A mode status */
LOCAL hdrmod_traffic_mode_enum_type hdrmod_traffic_mode = 
         HDRMOD_TRAFFIC_MODE_REV0;

/* REVISIT: MDM9K bringup controls. */
boolean hdrmod_use_pa_tx_on = TRUE;

/* Track whether tx has been started (triggered) */
boolean hdrmod_tx_triggered = FALSE;

/* Track whether we are waiting for DAC resp */
boolean hdrmod_dac_resp_pending = FALSE;

/* Track state of each carrier */
boolean hdrmod_carrier_enabled[3] = {FALSE, FALSE, FALSE};

#ifndef FEATURE_HDR_VI_VERIFICATION
/* General timer */
rex_timer_type hdrmod_wait_timer;
#endif  /* FEATURE_HDR_VI_VERIFICATION */

/* ISR enum type */
typedef enum {
  HDRMOD_FRAME_ISR,
  HDRMOD_SUBFRAME_ISR
} hdrmod_isr_enum_type;

/* ISR struct type */
typedef struct {
  boolean               installed; /* TRUE if an ISR has been installed */
  hdrmod_isr_enum_type  type;      /* ISR type: frame or subframe ISR */
  uint8                 offset;    /* Interrupt offset */
  void      (*isr_ptr)( uint32 );  /* ISR pointer */
} hdrmod_isr_struct_type;

/* ISR */
LOCAL hdrmod_isr_struct_type hdrmod_isr;

#if defined(FEATURE_HDR_UNIFIED_TX) || defined(FEATURE_HDR_BOLT_MODEM)
LOCAL uint8 hdrmod_rel0_tx_frame_offset = 0;
  /*  This variable is used  to store  the tx  frame offset for access and
   *  rel0 traffic. In RevA access, SW controls the tx frame offset to get
   *  4 slots preamble.
   */

#endif /* FEATURE_HDR_UNIFIED_TX || FEATURE_HDR_BOLT_MODEM */

#ifdef FEATURE_HDR_UNIFIED_TX
LOCAL hdrmod_isr_struct_type hdrmod_isr_backup;
  /* A backup copy of hdrmod_isr, needed to support TX enable callflow */
#endif /* !FEATURE_HDR_RUMI */

#ifdef FEATURE_HDR_BOLT_MODEM
extern volatile hdrfw_shared_memory_interface_t *hdrmdsp_shared_mem_interface;
#endif /* FEATURE_HDR_BOLT_MODEM */


/*===========================================================================

                LOCAL DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/
#ifdef FEATURE_HDR_BOLT_MODEM
/* ISR to be registered with vstmr to receive event notification */
void hdrmod_vstmr_isr
(
  vstmr_hdr_event_tbl_s *event_tbl,
  void *arg,
  uint32 pending
);
uint64 hdrmod_reverse_bits(uint64 bits);
#endif /* FEATURE_HDR_BOLT_MODEM */

/*===========================================================================
FUNCTION HDRMOD_INIT

DESCRIPTION
  Performs any one-time init needed by modulator driver.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
#ifndef FEATURE_HDR_VI_VERIFICATION
void hdrmod_init( void )
{
#ifdef FEATURE_HDR_BOLT_MODEM
  /* Register TX timeline */
  hdrts_register_timeline(HDRTS_MOD);
#else
  /* One time init of timer. */
  rex_def_timer(&hdrmod_wait_timer, HDRMOD_TASK_PTR, HDRTX_TIMER_SIG);
#endif /* FEATURE_HDR_BOLT_MODEM */

  /* Initialize entries in struct for ISR tracking */
  hdrmod_isr.installed = FALSE;
  hdrmod_isr.type      = HDRMOD_FRAME_ISR;
  hdrmod_isr.offset    = 0;
  hdrmod_isr.isr_ptr   = NULL;

#ifndef FEATURE_HDR_BOLT_MODEM
  /* One time init of timer. */
  rex_def_timer(&hdrmod_wait_timer, HDRMOD_TASK_PTR, HDRTX_TIMER_SIG);

  if ( ( DAL_DeviceAttach( DALDEVICEID_INTERRUPTCONTROLLER, 
                           &hdrmod_hInthandle ) != DAL_SUCCESS ) ||
       ( hdrmod_hInthandle == NULL ) )
  {
    ERR_FATAL( "DAL_DeviceAttach failed", 0, 0, 0);
  }
#endif /* FEATURE_HDR_BOLT_MODEM */
}
#endif  /* FEATURE_HDR_VI_VERIFICATION */

/*===========================================================================
FUNCTION HDRMOD_ENTER

DESCRIPTION
  Places the modulator into a known state by reseting the PA control bits
  and setting the frame tx start offset slot back to zero.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_enter( void )
{
#ifdef FEATURE_HDR_UNIFIED_TX

  /* Place modulator in HDR Mode Rev 0 */
  hdrmod_set_traffic_mode(HDRMOD_TRAFFIC_MODE_REV0);

  /* Initialize long code */
  HWIO_OUT(TX_DO_PN_STATE_0, PN_LONG_STATE_L);
  HWIO_OUT(TX_DO_PN_STATE_1, PN_LONG_STATE_H);
  HWIO_OUT(TX_UNIFIED_PN_LD, 1);


#elif defined(FEATURE_HDR_BOLT_MODEM)

  hdrmdsp_shared_mem_interface->write.pn_state_cfg.pn_code.bits_31to0 = PN_LONG_STATE_L;
  hdrmdsp_shared_mem_interface->write.pn_state_cfg.pn_code.bits_41to32 = PN_LONG_STATE_H;
  hdrmdsp_shared_mem_interface->write.pn_state_cfg.traffic_pn_mask_ctl = 0;
  /* Use PN generator 0 for all carriers */

  /* Enable timeline. Events can be enabled and generated after this point
   * from the respective timeline. ISR needs to be registered to ensure
   * any events are delivered as per the registration.
   */

#else

  /* Place modulator in HDR Mode Rev 0*/
  HWIO_OUTM(MOD_MODE, MOD_MODE_HDR_V, MOD_MODE_HDR_V);
  hdrmod_set_traffic_mode(HDRMOD_TRAFFIC_MODE_REV0);

  /* Initialize long code */
  HWIO_REVMOD_PN_LONG_STATE_H_OUTM( HWIO_REVMOD_PN_LONG_STATE_H_RMSK, 
    PN_LONG_STATE_H );
  HWIO_REVMOD_PN_LONG_STATE_L_OUTM( HWIO_REVMOD_PN_LONG_STATE_L_RMSK, 
    PN_LONG_STATE_L );

  /* Lock I and Q  into modulator */
  HDRMOD_PN_IQ_LOAD();

  /* Set up the REVMOD_PA_CTL register:             */
  /* DRC_DISABLE off, IQ_MASK off      */
  HWIO_REVMOD_PA_CTL_OUTM(HDRMOD_DRC_DISABLE_M | 
                          HDRMOD_MOD_MASK_M,
                          HDRMOD_DRC_DISABLE_OFF |
                          HDRMOD_MOD_MASK_OFF);

  /* Make sure Mod. Control is at default setting. */
  HWIO_OUT(REVMOD_MOD_CTL, 0);
#endif /* FEATURE_HDR_UNIFIED_TX */

#if defined(FEATURE_HDR_BOLT_MODEM) // BOLT or JOLOKIA
  hdrts_enable_timeline(HDRTS_MOD, hdrmod_vstmr_isr);
#endif /* FEATURE_HDR_BOLT_MODEM */
}

/*===========================================================================
FUNCTION HDRMOD_EXIT

DESCRIPTION
  Exits HDR mode.  Currently this is a no-op.  (The 1x encoder driver 
  places the modulator in 1x mode when activated.)

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_exit( void )
{
  /* Switch to Rev 0 */  
  hdrmod_set_traffic_mode(HDRMOD_TRAFFIC_MODE_REV0);

  /* Disable DO in modulator */
#ifdef FEATURE_HDR_UNIFIED_TX
  HDR_MSG_PROT_1(MSG_LEGACY_HIGH,
                 "DAC stop: set C2K_MODE=0 here sample_cnt=0x%x",
                 HWIO_IN( TX_UNIFIED_SAMPLE_CNT_READ ));
  HWIO_OUTM(TX_C2K_MODE, HWIO_FMSK(TX_C2K_MODE,MOD_DO_EN),
                         0 << HWIO_TX_C2K_MODE_MOD_DO_EN_SHFT);
#elif !defined(FEATURE_HDR_BOLT_MODEM)
  HWIO_OUTM(MOD_MODE, MOD_MODE_HDR_V, 0);
#endif /* FEATURE_HDR_UNIFIED_TX */

#if defined(FEATURE_HDR_BOLT_MODEM) // BOLT or JOLOKIA
  /* Disable timeline. No events will be generated after this point
   * from the respective timeline
   */
  hdrts_disable_timeline(HDRTS_MOD);
#endif /* FEATURE_HDR_BOLT_MODEM */
}

#ifndef FEATURE_HDR_BOLT_MODEM

/*===========================================================================
FUNCTION HDRMOD_BIO_SWITCH_IS_SET

DESCRIPTION
  Check if BIO switch is set

DEPENDENCIES
  None.

PARAMETERS
  sw - switch to check  

RETURN VALUE
  True - if the swtich is set
  False - otherwise

SIDE EFFECTS
  None.
===========================================================================*/
#ifndef FEATURE_HDR_VI_VERIFICATION
boolean hdrmod_bio_switch_is_set(uint32 sw)
{
  /* DAL API handle */
  static DalDeviceHandle* DAL_handle;
  static DALResult DAL_result = DAL_ERROR;
  uint32 DAL_state = 0;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* if haven't registered */
  if (DAL_result != DAL_SUCCESS)
  {
    DAL_result = DAL_DeviceAttach(DALDEVICEID_SOFTWARESWITCHES,
                                  &DAL_handle);
    if (DAL_result != DAL_SUCCESS)
    {
      ERR_FATAL("Cannot register DAL: result %d", 
                DAL_result, 0, 0);
    }    
  }

  (void) DalSoftwareSwitches_GetMask(DAL_handle, sw, &DAL_state);

  HDR_MSG_PROT_2(MSG_LEGACY_MED,
                 "Get DAL switch %d, state %d",
                 sw, DAL_state);

  return (DAL_state != 0);
}
#endif  /* FEATURE_HDR_VI_VERIFICATION */
#endif /* FEATURE_HDR_BOLT_MODEM */


/*===========================================================================
FUNCTION HDRMOD_SET_PN_LONG_CODE_MASK

DESCRIPTION
  Load the 42-bit I and Q long code masks into the modulator.
  
DEPENDENCIES
  None.

PARAMETERS
  i            - PN long code mask for I
  q            - PN long code mask for Q
  carrier_mask - Carriers for which PN generator is to be enabled

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_set_pn_long_code_mask( 
  qword i,
  qword q
#ifdef FEATURE_HDR_BOLT_MODEM
  ,uint8 carrier_mask
#endif /* FEATURE_HDR_BOLT_MODEM */
)
{
#ifdef FEATURE_HDR_BOLT_MODEM
  uint32 pn_mask_i_hi = 0, pn_mask_i_lo = 0;
  uint32 pn_mask_q_hi = 0, pn_mask_q_lo = 0;
  uint64 temp_word = 0;

  /* Sanity check - just bits 0, 1 and 2 should be set in carrier mask */
  ASSERT( carrier_mask <= 7 );
#endif /* FEATURE_HDR_BOLT_MODEM */


  /* Zero out mask if we are in a test mode */
#ifdef FEATURE_FACTORY_TESTMODE
  if ( ftm_get_mode() == FTM_MODE )
  { 
#ifdef FEATURE_HDR_RUMI
    HDR_MSG_DRIVERS_4( MSG_LEGACY_HIGH, 
                       "RUMI TX ONLY: PN LC Mask( %x %x, %x %x )",
                       qw_hi(i), qw_lo(i), qw_hi(q), qw_lo(q) );
      /* we don't want to override the PN Long code MASK coming from FTM command */
#else
    /* Always use lc mask in ftm mode */
    qw_set( i, hdrmod_test_lc_mask.i_hi, hdrmod_test_lc_mask.i_lo );
    qw_set( q, hdrmod_test_lc_mask.q_hi, hdrmod_test_lc_mask.q_lo );
    HDR_MSG_DRIVERS_4( MSG_LEGACY_HIGH, 
                       "HDR: PN LC Mask override! ( %x %x, %x %x )",
                       qw_hi(i), qw_lo(i), qw_hi(q), qw_lo(q) );
#endif /* FEATURE_HDR_RUMI*/
  }
#endif /* FEATURE_FACTORY_TESTMODE */

#ifdef FEATURE_HDR_UNIFIED_TX
  /* Initialize long code */
  /* Set PN long code values */
  HWIO_OUT(TX_DO_PN_STATE_0, PN_LONG_STATE_L);
  HWIO_OUT(TX_DO_PN_STATE_1, PN_LONG_STATE_H);
  HWIO_OUT(TX_UNIFIED_PN_LD, 1);

  /* Set PN mask for I and Q channels */
  HWIO_OUTI(TX_DO_I_PN_MASK_0_Cm, 0, qw_lo(i));
  HWIO_OUTI(TX_DO_I_PN_MASK_1_Cm, 0, qw_hi(i) & 0xFFFF);
  HWIO_OUTI(TX_DO_Q_PN_MASK_0_Cm, 0, qw_lo(q));
  HWIO_OUTI(TX_DO_Q_PN_MASK_1_Cm, 0, qw_hi(q) & 0xFFFF);
  HWIO_OUT(TX_UNIFIED_PN_MASK_LD, 1);

  /* Use same PN generator 0 for all FOC channels */
  HWIO_OUTI(TX_DO_FOC_PN_CTL_Cm, 0, 0);
  HWIO_OUTI(TX_DO_FOC_PN_CTL_Cm, 1, 0);
  HWIO_OUTI(TX_DO_FOC_PN_CTL_Cm, 2, 0);

  /* NoFeedbackMultiplexing: select PN generator 0 for traffic channels */
  HWIO_OUT(TX_DO_TRAFFIC_PN_CTL, 0);  
#elif defined(FEATURE_HDR_BOLT_MODEM)
  /* Initialize long code */
  /* Set PN long code values */
  hdrmdsp_shared_mem_interface->write.pn_state_cfg.pn_code.bits_31to0 = PN_LONG_STATE_L;
  hdrmdsp_shared_mem_interface->write.pn_state_cfg.pn_code.bits_41to32 = PN_LONG_STATE_H;
  hdrmdsp_shared_mem_interface->write.pn_state_cfg.traffic_pn_mask_ctl = 0;
  /* should this be 0xE000? */

  /* Set mask for enabling PN generators for specified carriers */
  hdrmdsp_shared_mem_interface->write.pn_state_cfg.pn_enabled_car_mask = 
    carrier_mask;

  temp_word = ((uint64) qw_hi(i)) << 32 | qw_lo(i);
  temp_word = hdrmod_reverse_bits(temp_word);
  temp_word >>= 22;

  pn_mask_i_lo = (uint32) (temp_word & 0xFFFFFFFF); // Need all 42 bits reversed
  pn_mask_i_hi = (uint32) ((temp_word >> 32) & 0x3FF);

  temp_word = ((uint64) qw_hi(q)) << 32 | qw_lo(q);
  temp_word = hdrmod_reverse_bits(temp_word);
  temp_word >>= 22;

  pn_mask_q_lo = (uint32) (temp_word & 0xFFFFFFFF);
  pn_mask_q_hi = (uint32) ((temp_word  >> 32) & 0x3FF);

  HDR_MSG_DRIVERS_5( MSG_LEGACY_HIGH, 
    "HDR: PN LC Mask(flipped)( 0x%x 0x%x, 0x%x 0x%x ),EnableCarrierMask: 0x%x",
    pn_mask_i_hi, pn_mask_i_lo, pn_mask_q_hi, pn_mask_q_lo, carrier_mask );


  /* Set PN mask for I and Q channels */
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[0].pn_i_mask_31to0 = pn_mask_i_lo;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[0].pn_i_mask_41to32 = pn_mask_i_hi;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[0].pn_q_mask_31to0 = pn_mask_q_lo;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[0].pn_q_mask_41to32 = pn_mask_q_hi;

  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[1].pn_i_mask_31to0 = pn_mask_i_lo;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[1].pn_i_mask_41to32 = pn_mask_i_hi;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[1].pn_q_mask_31to0 = pn_mask_q_lo;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[1].pn_q_mask_41to32 = pn_mask_q_hi;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[2].pn_i_mask_31to0 = pn_mask_i_lo;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[2].pn_i_mask_41to32 = pn_mask_i_hi;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[2].pn_q_mask_31to0 = pn_mask_q_lo;
  hdrmdsp_shared_mem_interface->write.pn_mask_cfg[2].pn_q_mask_41to32 = pn_mask_q_hi;

#else

  HWIO_REVMOD_PN_LONG_STATE_H_OUTM( HWIO_REVMOD_PN_LONG_STATE_H_RMSK, 
    PN_LONG_STATE_H );
  HWIO_REVMOD_PN_LONG_STATE_L_OUTM( HWIO_REVMOD_PN_LONG_STATE_L_RMSK, 
    PN_LONG_STATE_L );
  
  /* Write I and Q long code masks to PN generator 0 */
  HWIO_REVMOD_0_PN_I_LONG_MASK_H_OUT( (qw_hi(i) & 0xFFFF));
  HWIO_REVMOD_0_PN_I_LONG_MASK_L_OUT( (qw_lo(i)));

  HWIO_REVMOD_0_PN_Q_LONG_MASK_H_OUT( (qw_hi(q) & 0xFFFF));
  HWIO_REVMOD_0_PN_Q_LONG_MASK_L_OUT( (qw_lo(q)));

  /* NoFeedbackMultiplexing: select PN generator 0 for FOC channels 
     corresponding to all the carriers */
  HWIO_REVMOD_0_MCDO_FOC_PN_CTL_OUTM(HWIO_REVMOD_0_MCDO_FOC_PN_CTL_RMSK, 0 );
  HWIO_REVMOD_1_MCDO_FOC_PN_CTL_OUTM(HWIO_REVMOD_1_MCDO_FOC_PN_CTL_RMSK, 0 );
  HWIO_REVMOD_2_MCDO_FOC_PN_CTL_OUTM(HWIO_REVMOD_2_MCDO_FOC_PN_CTL_RMSK, 0 );

  /* NoFeedbackMultiplexing: select PN generator 0 for traffic channels */
  HWIO_OUT(REVMOD_MCDO_TRAFFIC_PN_CTL, 0);
  
  /* Lock I and Q long code masks into modulator */
  HDRMOD_PN_IQ_LOAD();
#endif /* FEATURE_HDR_UNIFIED_TX */

#ifndef FEATURE_HDR_VI_VERIFICATION
  /* Reduce message priority after verification */
  HDR_MSG_DRIVERS_4( MSG_LEGACY_HIGH, 
                     "HDR - PN LC Mask i: %x %x, q: %x %x",
                     qw_hi(i), qw_lo(i), qw_hi(q), qw_lo(q) );
#endif  /* FEATURE_HDR_VI_VERIFICATION */

}


/*===========================================================================
FUNCTION HDRMOD_SET_TX_FRAME_START_OFFSET

DESCRIPTION
  Sets the tx frame start offset to a designated slot.
  
DEPENDENCIES
  None.

PARAMETERS
  slot_offset - The slot on which transmit frames will start (0 - 15)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_set_tx_frame_start_offset(
  byte slot_offset 
)
{
#ifdef FEATURE_HDR_UNIFIED_TX

  /* Load slot offset on which TX begins */
  HWIO_OUT(TX_UNIFIED_FRAME_OFFSET, slot_offset);
  hdrmod_rel0_tx_frame_offset = slot_offset;
    /* Store the frame offset. Will pass it to FW in rel0_tx_cfg message later. */

  HWIO_OUT(TX_UNIFIED_FRAME_OFF_LD, 1);

#elif defined(FEATURE_HDR_BOLT_MODEM)

  hdrmod_rel0_tx_frame_offset = slot_offset;
  /* Store the frame offset. Will pass it to FW in rel0_tx_cfg message
   * later
   */

#else

  /* Load slot offset on which TX begins */
  HWIO_REVMOD_FRAME_OFFSET_OUT(slot_offset );

#endif /* FEATURE_HDR_UNIFIED_TX */
}



/*===========================================================================
FUNCTION HDRMOD_SET_TX_FRAME_INTERRUPT_OFFSET

DESCRIPTION
  Sets the tx frame interrupt to assert on a designated slot.

DEPENDENCIES
  None.

PARAMETERS
  offset - The slot on which to assert the transmit interrupt (0 - 15)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_set_tx_frame_interrupt_offset
(
  uint16 int_offset
)
{
#ifdef FEATURE_HDR_BOLT_MODEM
  void (*temp_ptr)( uint32 ) = NULL;
#endif /* FEATURE_HDR_BOLT_MODEM */

  /* Save ISR type and offset */
  hdrmod_isr.type   = HDRMOD_FRAME_ISR;
  hdrmod_isr.offset = (uint8) int_offset;

#ifdef FEATURE_HDR_BOLT_MODEM

  temp_ptr = hdrmod_isr.isr_ptr;
  hdrmod_disable_frame_isr ();
  hdrmod_enable_frame_isr ( temp_ptr );

#else
  /* Set modulator to frame resolution */ 
  /* Write offset value (in slots) to modulator */
  HWIO_OUT( RTC_REVMOD_FRAME_INT_OFFSET, int_offset ); 
#endif /* FEATURE_HDR_BOLT_MODEM */

  HDR_MSG_DRIVERS_2 ( MSG_LEGACY_HIGH,
                      "Set frame int offset to offset %d int_offset %d",
                      hdrmod_isr.offset, int_offset);
}


/*===========================================================================
FUNCTION HDRMOD_SET_TX_FRAME_INTERRUPT_OFFSET_INTERNAL

DESCRIPTION
  Sets the tx frame interrupt to assert on a designated slot.
  This function is only to be used by functions defined within this file.
  Unlike hdrmod_set_tx_frame_interrupt_offset(), this function does not 
  track the programmed offset. Using a seperate function by internal entities
  helps track frame interrupt offset programmed by external entities.

DEPENDENCIES
  None.

PARAMETERS
  offset - The slot on which to assert the transmit interrupt (0 - 15)

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_set_tx_frame_interrupt_offset_internal
(
  uint16 int_offset
)
{
  /* Set modulator to frame resolution */ 
  /* Write offset value (in slots) to modulator */

#ifdef FEATURE_HDR_BOLT_MODEM
  HDR_MSG_DRIVERS_2 ( MSG_LEGACY_HIGH,
                      "Set frame int offset to %d int_offset %d",
                      hdrmod_isr.offset, int_offset);

  hdrmod_isr.type   = HDRMOD_FRAME_ISR;
  hdrmod_isr.offset = (uint8) int_offset;
#else
  HWIO_OUT( RTC_REVMOD_FRAME_INT_OFFSET, int_offset ); 
#endif /* FEATURE_HDR_BOLT_MODEM */
}


/*===========================================================================
FUNCTION HDRMOD_ENTER_ACCESS_MODE

DESCRIPTION
  Enters access mode.  In preparation for an access probe, the DRC is turned
  off. Once access mode is entered, hdrmod_exit_access_mode() must be called
  to exit before a reverse traffic link can be established.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_enter_access_mode( void )
{
#ifdef FEATURE_HDR_UNIFIED_TX
  /* Disable RRI channel transmission */
  HWIO_OUT(TX_DO_REV0_RRI_CTL, 
            1  << HWIO_TX_DO_REV0_RRI_CTL_DISABLE_SHFT |
            0  << HWIO_TX_DO_REV0_RRI_CTL_RRI_SHFT
          );
#elif !defined(FEATURE_HDR_BOLT_MODEM)
#ifdef FEATURE_HDR_DIME_MODEM
 /* Disable DRC/RRI channel transmission */
  HWIO_OUTM(REVMOD_PA_CTL, HDRMOD_DRC_DISABLE_M | HDRMOD_RRI_DISABLE_M, 
                           HDRMOD_DRC_DISABLE_ON | HDRMOD_RRI_DISABLE_ON );
#endif /* FEATURE_HDR_DIME_MODEM */
#endif /* FEATURE_HDR_UNIFIED_TX */

  /* In access mode, so PA will be used. */
  hdrmod_pa_needed = TRUE;
  hdrmod_access_mode = TRUE;
}

/*===========================================================================
FUNCTION HDRMOD_EXIT_ACCESS_MODE

DESCRIPTION
  Disables access mode.  The PA control bits are returned to the default
  state, and DRC channel tranmission is re-eneabled.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_exit_access_mode( void )
{
#ifdef FEATURE_HDR_UNIFIED_TX
  /* Set PA configuration to:
  **   RRI Disable: off
  */
  HWIO_OUT(TX_DO_REV0_RRI_CTL, 
            0  << HWIO_TX_DO_REV0_RRI_CTL_DISABLE_SHFT |
            0  << HWIO_TX_DO_REV0_RRI_CTL_RRI_SHFT
          );
#else
#ifndef FEATURE_HDR_BOLT_MODEM
  /* Set PA configuration to:
  **   DRC Disable: off; IQ_MASK: off
  **   RRI Disable: off
  */
  HWIO_OUTM(REVMOD_PA_CTL, HDRMOD_DRC_DISABLE_M   |
                           HDRMOD_MOD_MASK_M      |
                           HDRMOD_RRI_DISABLE_M,
                           HDRMOD_DRC_DISABLE_OFF |
                           HDRMOD_MOD_MASK_OFF    |
                           HDRMOD_RRI_DISABLE_OFF);
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */

  /* Track pa state */
  hdrmod_pa_on = FALSE;
  hdrmod_pa_needed = FALSE;
  hdrmod_access_mode = FALSE;
}
 


/*===========================================================================
FUNCTION HDRMOD_SETUP_ACCESS_PROBE

DESCRIPTION
  Sets up the modulator for an access probe. This function must be called 
  before each probe. 
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
#ifndef FEATURE_HDR_VI_VERIFICATION
void hdrmod_setup_access_probe( void )
{
  /* Track pa state */
  hdrmod_pa_on = FALSE;
}
 

/*===========================================================================
FUNCTION HDRMOD_BEGIN_ACCESS_PROBE

DESCRIPTION
  Turns on the PA and turns off IQ masking for an access probe.  This
  function should be called shortly before the frame in which the probe is
  to begin.  (The PA is turned on early to allow it to warm up.)
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_begin_access_probe( void )
{
  /* Track pa state */
  hdrmod_pa_on = TRUE;
}



/*===========================================================================
FUNCTION HDRMOD_END_ACCESS_PROBE

DESCRIPTION
  Ends an access probe by configuring the PA to turn off at the next frame
  boundary.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_end_access_probe( void )
{
  /* Track pa state */
  hdrmod_pa_on = FALSE;
}



/*===========================================================================
FUNCTION HDRMOD_ENABLE_PA

DESCRIPTION
  Turns on the PA for the reverse traffic channel.  The PA is turned on
  immediately.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_enable_pa( void )
{
  /* Track pa state */
  hdrmod_pa_on = TRUE;
  hdrmod_pa_needed = TRUE;
}


/*===========================================================================
FUNCTION HDRMOD_DISABLE_PA

DESCRIPTION
  Turns off the PA immediately.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_disable_pa( void )
{
  /* Track pa state */
  hdrmod_pa_on = FALSE;
  hdrmod_pa_needed = FALSE;
}


/*===========================================================================
FUNCTION HDRMOD_FRAME_SIZE_TO_RRI                                 

DESCRIPTION
  Converts a frame size to the corresponding modulator RRI value.
  
DEPENDENCIES
  None.

PARAMETERS
  frame_size - the frame size to find the RRI value for.

RETURN VALUE
  The RRI value.

SIDE EFFECTS
  None.
===========================================================================*/
hdrmod_rri_enum_type hdrmod_frame_size_to_rri( 
  uint16 frame_size
)
{
  
  hdrmod_rri_enum_type frame_type_val = REVERSE_RATE_PILOT; 

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Match number of bytes to reverse rate */
  switch (frame_size)
  {
  case 0:
    frame_type_val = REVERSE_RATE_PILOT;
    break;
  case REV_RATE_9600_SIZE: 
    frame_type_val = REVERSE_RATE_9600;
    break;
  case REV_RATE_19200_SIZE: 
    frame_type_val = REVERSE_RATE_19200;
    break;
  case REV_RATE_38400_SIZE:
    frame_type_val = REVERSE_RATE_38400;
    break;
  case REV_RATE_76800_SIZE:
    frame_type_val = REVERSE_RATE_76800;
    break;
  case REV_RATE_153600_SIZE:
    frame_type_val = REVERSE_RATE_153600;
    break;
  default:
    ERR_FATAL("Bad frame size sent to modulator.",0,0,0);
  }

  return frame_type_val;
}
#endif  /* FEATURE_HDR_VI_VERIFICATION */



/*===========================================================================
FUNCTION HDRMOD_SET_RRI

DESCRIPTION
  Sets the modulator reverse rate based on the RRI.
  
DEPENDENCIES
  None.

PARAMETERS
  RRI - the RRI index of the frame.  
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_set_rri(uint16 rri)
{
#if defined(FEATURE_HDR_UNIFIED_TX) || defined (FEATURE_HDR_BOLT_MODEM)
  hdrfw_msg_u fw_cmd;
    /* Firmware command */

  errno_enum_type estatus;
    /* Error status */
#endif /* defined(FEATURE_HDR_UNIFIED_TX) || defined (FEATURE_HDR_BOLT_MODEM) */

/*-----------------------------------------------------------------------*/

#ifdef FEATURE_HDR_UNIFIED_TX
  if(hdrmod_access_mode == TRUE)
  {
    HWIO_OUT(TX_DO_REV0_RRI_CTL,
              0x1 << HWIO_TX_DO_REV0_RRI_CTL_DISABLE_SHFT|
              rri << HWIO_TX_DO_REV0_RRI_CTL_RRI_SHFT);
  }
  else
  {
    HWIO_OUT(TX_DO_REV0_RRI_CTL, 
              0x0 << HWIO_TX_DO_REV0_RRI_CTL_DISABLE_SHFT|
              rri << HWIO_TX_DO_REV0_RRI_CTL_RRI_SHFT);
  }
#endif  /* FEATURE_HDR_TRITON_MODEM */

#if defined(FEATURE_HDR_UNIFIED_TX) || defined (FEATURE_HDR_BOLT_MODEM)

  /* Send rel0_tx_cfg message. REMOV_RATE_INDEX  is removed from Triton.
   * For rev0 and  access, FW depends on  SW to inform it  RRI and frame
   * offset. FW will  latch the requested RRI (sent in  this message) at
   * the  half slot  before frame  offset adjusted  frame boundary,  and
   * apply data  gains. In revA  access, SW  plays with frame  offset to
   * make fw transmit 4 slots preamble.
   */
  
  memset  ( &fw_cmd,  0,  sizeof(hdrfw_msg_u)  );
  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_AMAC, HDR_FW_REL0_TX_CFG_CMD );

  fw_cmd.rel0_tx_cfg.rriRequest = rri;
  fw_cmd.rel0_tx_cfg.txFrameOffset = hdrmod_rel0_tx_frame_offset;

   /* Send the message */
  estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_msg_u) );
  ASSERT ( estatus == E_SUCCESS );

  HDR_MSG_DRIVERS_2( MSG_LEGACY_HIGH, 
     "Send REL0_TX_CFG rri%d tx_frame_offset%d", rri,
     hdrmod_rel0_tx_frame_offset );

#else

  HWIO_REVMOD_RRI_REQUEST_OUT( rri );

#endif /* defined(FEATURE_HDR_UNIFIED_TX) || defined (FEATURE_HDR_BOLT_MODEM) */

  /* Update frame count */
  hdrmod_frame_count[rri]++;
}

/*===========================================================================
FUNCTION HDRMOD_PA_IS_ON

DESCRIPTION
  Indicates whether the PA is currently on.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE if PA is on, FALSE otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
#ifndef FEATURE_HDR_VI_VERIFICATION
boolean hdrmod_pa_is_on( void )
{
  return (hdrmod_pa_status);
}

/*===========================================================================
FUNCTION HDRMOD_RESET_REVERSE_FRAME_COUNT

DESCRIPTION
  Resets internal modulator count of frames sent.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_reset_reverse_frame_count( void )
{
  int i;

  /* Set all fields to zero. */
  for (i=0; i < (int)HDRMOD_NUM_REVERSE_RATES; i++) {
    hdrmod_frame_count[i] = 0;
  }

}


/*===========================================================================
FUNCTION HDRMOD_GET_REVERSE_FRAME_COUNT

DESCRIPTION
  Fills in a structure with the current count of frames of each rate that
  have been transmitted on the reverse link since the last
  hdrmod_reset_reverse_frame_count() call.
  
DEPENDENCIES
  None.

PARAMETERS
  frame_count - pointer to structer to fill in with count data.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_reset_frame_count(
  hdrmod_reverse_frame_count_type * frame_count_ptr
)
{
  /* Copy field data */
  frame_count_ptr->rate9600_cnt = hdrmod_frame_count[REVERSE_RATE_9600];
  frame_count_ptr->rate19200_cnt = hdrmod_frame_count[REVERSE_RATE_19200];
  frame_count_ptr->rate38400_cnt = hdrmod_frame_count[REVERSE_RATE_38400];
  frame_count_ptr->rate76800_cnt = hdrmod_frame_count[REVERSE_RATE_76800];
  frame_count_ptr->rate153600_cnt = hdrmod_frame_count[REVERSE_RATE_153600];

}


/*===========================================================================
FUNCTION HDRMOD_SET_ZERO_LC_MASK

DESCRIPTION
  Enables the long code mask to always be set to zero, for testing purposes.
  
DEPENDENCIES
  None.

PARAMETERS
  test_val - 1 uses zero for the PN long code mask, 
             anything else disables the "zero test mode".

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_set_zero_lc_mask(
  int8 test_val
)
{
  if (test_val == 1)
  {
    hdrmod_test_lc_mask.i_hi = 0;
    hdrmod_test_lc_mask.i_lo = 0;
    hdrmod_test_lc_mask.q_hi = 0;
    hdrmod_test_lc_mask.q_lo = 0;
    hdrmod_use_lc_mask = TRUE;
  }
  else
  {
    hdrmod_use_lc_mask = FALSE;
  }
}



/*===========================================================================
FUNCTION HDRMOD_ENABLE_FRAME_ISR

DESCRIPTION
  Enables the Tx frame interrupt. 
  
DEPENDENCIES
  None.

PARAMETERS
  isr_ptr - the routine to use as the ISR

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_enable_frame_isr
(
  void (*isr_ptr)( uint32 ) /* The routine to use as the ISR */
)
{   
#ifdef FEATURE_HDR_BOLT_MODEM
  uint16  slot_cnt = 0;
#endif /* FEATURE_HDR_BOLT_MODEM */

  if (isr_ptr)
  { 
    /* Save ISR pointer and set flag indicating that ISR has been installed */
    hdrmod_isr.isr_ptr   = isr_ptr;
    hdrmod_isr.installed = TRUE;

    /* Enable Tx frame interrupt */
#ifdef FEATURE_HDR_BOLT_MODEM
    switch (hdrmod_isr.type )
    {
      case HDRMOD_SUBFRAME_ISR:
        slot_cnt = 4;  // Every subframe
        break;
        
      case HDRMOD_FRAME_ISR:
        slot_cnt = 16;  // Every frame
        break;
        
      default:
        slot_cnt = 16;  // Every frame
        HDR_MSG_DRIVERS_1( MSG_LEGACY_ERROR, 
                                "Incorrect ISR type %d", hdrmod_isr.type );
        break;
        
    }
    HDR_MSG_DRIVERS_2( MSG_LEGACY_HIGH, 
                       "ISR slot %d offset %d",
                       slot_cnt,
                       hdrmod_isr.offset );

    /* After the timeline is enabled, events (both single and periodic)
     * can be activated on that timeline
     * MOD event to start on hdrmod_isr.offset
     * MOD event to occur every slot_cnt
     */
    hdrts_activate_periodic_event
    (
      HDRTS_MOD, hdrmod_isr.offset, slot_cnt
    );

#else
    DalInterruptController_RegisterISR( hdrmod_hInthandle,
                                        ( uint32 ) HDR_FRAME_INT_ID,
                                        ( DALIRQ ) isr_ptr,
                                        ( const DALIRQCtx )0,
                                        DALINTRCTRL_ENABLE_RISING_EDGE_TRIGGER );
#endif /* FEATURE_HDR_BOLT_MODEM */

    HDR_MSG_DRIVERS_1( MSG_LEGACY_HIGH, 
     "Frame ISR enabled %x", hdrmod_isr.isr_ptr );
  }
  else
  {
    HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, 
     "Frame ISR not enabled since ISR ptr is NULL" );
  }
}


#ifdef FEATURE_HDR_UNIFIED_TX
/*===========================================================================
FUNCTION HDRMOD_ENABLE_FRAME_ISR_INTERNAL

DESCRIPTION
  Enables the Tx frame interrupt.
  This function is only to be used by functions defined within this file.
  Unlike hdrmod_enable_frame_isr(), this function does not track the 
  installed ISR.  Using a seperate ISR install function by internal entities
  helps track ISRs installed by external entities.
  
DEPENDENCIES
  None.

PARAMETERS
  isr_ptr - the routine to use as the ISR

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_enable_frame_isr_internal
(
  void (*isr_ptr)( uint32 ) /* The routine to use as the ISR */
)
{   
  hdrmod_enable_frame_isr( isr_ptr );
}
#endif /* FEATURE_HDR_UNIFIED_TX */



/*===========================================================================
FUNCTION HDRMOD_DISABLE_FRAME_ISR

DESCRIPTION
  Disables the Tx frame interrupt.  
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_disable_frame_isr(void)
{
#ifdef FEATURE_HDR_BOLT_MODEM

  /* Disable timeline and stop all events activated on that timeline */
  hdrts_deactivate_timeline_event(HDRTS_MOD);

#else

  /* Use tramp to disable Tx frame interrupt */
  DalInterruptController_Unregister( hdrmod_hInthandle, HDR_FRAME_INT_ID );

#endif /* FEATURE_HDR_BOLT_MODEM */

  /* Reset entries in struct for ISR tracking */
  hdrmod_isr.installed = FALSE;
  hdrmod_isr.isr_ptr   = NULL;

  HDR_MSG_DRIVERS ( MSG_LEGACY_HIGH, "Frame ISR disabled" );
}


#ifdef FEATURE_HDR_UNIFIED_TX
/*===========================================================================

FUNCTION HDRMOD_BACKUP_FRAME_ISR

DESCRIPTION
  This function backs up the frame ISR structure is it can be
  temporarily overwritten for other purposes. Call
  hdrmod_restore_frame_isr() to restore the previous ISR

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrmod_backup_frame_isr( )
{
  HDR_MSG_DRIVERS_4( MSG_LEGACY_HIGH, 
                     "Backup ISR %x installed:%d type:%d offset:%d",
                     hdrmod_isr.isr_ptr, 
                     hdrmod_isr.installed,
                     hdrmod_isr.type,
                     hdrmod_isr.offset);

  hdrmod_isr_backup = hdrmod_isr;
}

/*===========================================================================
FUNCTION HDRMOD_DISABLE_FRAME_ISR_INTERNAL

DESCRIPTION
  Disables the Tx frame interrupt.
  This function is only to be used by functions defined within this file.
  Unlike hdrmod_disable_frame_isr(), this function does not update the struct 
  for tracking installed frame ISR. Using a seperate ISR disable function
  by internal entities helps track ISRs installed by external entities.
  
DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_disable_frame_isr_internal(void)
{
  hdrmod_disable_frame_isr();
}
#endif /* FEATURE_HDR_UNIFIED_TX */

#endif  /* FEATURE_HDR_VI_VERIFICATION */


/*===========================================================================
FUNCTION HDRMOD_SET_TRAFFIC_MODE

DESCRIPTION
  Sets the modulator to the specified traffic mode, either Rev 0 or Rev A.
  
DEPENDENCIES
  None.

PARAMETERS
  MODE - the mode to set the modulator.  
  
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_set_traffic_mode
(
  hdrmod_traffic_mode_enum_type mode
)
{
  /* Save the new traffic mode for later */
  hdrmod_traffic_mode = mode;

  /* set the modulator to the specified traffic mode */
#ifdef FEATURE_HDR_UNIFIED_TX

  if ( mode == HDRMOD_TRAFFIC_MODE_REVA || mode == HDRMOD_TRAFFIC_MODE_REVB )
  {
    /* Here we want to differentiate RevA and RevB call. So the two 
       enums HDRMOD_TRAFFIC_MODE_REVA and HDRMOD_TRAFFIC_MODE_REVB are different.
       However, in both cases TX_C2K_MODE is the same: 0x03. So we set mode to
       1 in both cases.
       For Rev0 traffic, TX_C2K_MODE should be 0x01.
    */
    mode = 1;
  }
  /* Triton unified TX */
  HWIO_OUTM(TX_C2K_MODE, HWIO_FMSK(TX_C2K_MODE,DO_REV) |
                         HWIO_FMSK(TX_C2K_MODE,MOD_DO_EN),
                         mode << HWIO_TX_C2K_MODE_DO_REV_SHFT |
                         1 << HWIO_TX_C2K_MODE_MOD_DO_EN_SHFT);
#else
#ifndef FEATURE_HDR_BOLT_MODEM
  HWIO_OUTM(MOD_MODE, HWIO_FMSK(MOD_MODE,DO_REV),
             ((int)mode << HWIO_SHFT(MOD_MODE,DO_REV)));
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */

#ifndef FEATURE_HDR_VI_VERIFICATION

  HDR_MSG_DRIVERS_1 ( MSG_LEGACY_HIGH,
                      "HDR Mod - Set traffic mode to %d.", mode );
#endif  /* FEATURE_HDR_VI_VERIFICATION */
}


/*===========================================================================
FUNCTION HDRMOD_SET_TX_SUBFRAME_INTERRUPT_OFFSET

DESCRIPTION
  Sets the tx frame interrupt to assert on a designated slot.

DEPENDENCIES
  None.

PARAMETERS
  offset - The slot (expressed in chipx1) on which to assert the transmit interrupt

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
#ifndef FEATURE_HDR_VI_VERIFICATION
void hdrmod_set_tx_subframe_interrupt_offset
(
  uint16 int_offset
)
{
#ifdef FEATURE_HDR_BOLT_MODEM
  void (*temp_ptr)( uint32 ) = NULL;
#endif /* FEATURE_HDR_BOLT_MODEM */

  /* Save ISR type and offset */
  hdrmod_isr.type   = HDRMOD_SUBFRAME_ISR;
  hdrmod_isr.offset = (uint8) int_offset;

#ifdef FEATURE_HDR_BOLT_MODEM

  /* Convert to slot from chipx1 */
  hdrmod_isr.offset = int_offset / HDRMOD_ONE_SLOT_IN_CHIPS;
  temp_ptr = hdrmod_isr.isr_ptr;
  hdrmod_disable_frame_isr ();
  hdrmod_enable_frame_isr ( temp_ptr );

#else

  /* Set modulator to 4-slot resolution and write offset value (in chips) */
  /* NOTE: This register is not readable; be careful using masked output. */
  HWIO_OUT(RTC_REVMOD_FRAME_INT_OFFSET, 
    (HWIO_FMSK(RTC_REVMOD_FRAME_INT_OFFSET,EN)) |
    (uint32)(int_offset << (HWIO_SHFT(RTC_REVMOD_FRAME_INT_OFFSET,CHIP))));

#endif /* FEATURE_HDR_BOLT_MODEM */

  HDR_MSG_DRIVERS_2 ( MSG_LEGACY_HIGH,
                      "Set subframe int offset to offset %d int offset %d",
                       hdrmod_isr.offset, int_offset);
}
#endif  /* FEATURE_HDR_VI_VERIFICATION */

/*===========================================================================
FUNCTION HDRMOD_CONFIG_CARRIER

DESCRIPTION
  Configures reverse link carrier.

DEPENDENCIES
  None.

PARAMETERS
  chan_idx - The MCDO carrier to be configured (0-2)
  enable   - TRUE  - enable transmission on the carrier specified by 
                     chan_idx
             FALSE - disable transmission on the carrier

  foc_mask - Specifies the forward overhead channel bit mask
    It is different for DIME and TRITON.
    DIME:
             000 - Transmit no FOC
             001 - Enable carrier 0 FOC transmission
             010 - Enable carrier 1 FOC transmission
             011 - Enable carrier 0 and carrier 1 FOC transmission
             100 - Enable carrier 2 FOC transmission
             101 - Enable carrier 0 and carrier 2 FOC transmission
             110 - Enable carrier 1 and carrier 2 FOC transmission
             111 - Enable carrier 0, 1 and 2 FOC transmission
     TRITON:
             001 - Enable carrier 0 FOC transmission
             010 - Enable carrier 1 FOC transmission
             100 - Enable carrier 2 FOC transmission
        if you want to enable two carriers on TRITON, you need to call this
        API two times.
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_config_carrier
(
  uint8   chan_idx, /* Physical channel index (0-2) */
  boolean enable,   /* Whether the channel should be enabled or disabled */
  uint8   foc_mask  /* FOC (Forward Overhead Channel) bit mask */
)
{
  /* Record status for each request. */
  hdrmod_carrier_enabled[chan_idx] = enable;

  /* When enabling, set immediately.  For disable, only record status.
     Modulator will be disabled later. */
  if (enable)
  {
#ifdef FEATURE_HDR_UNIFIED_TX
    /* convert foc_mask to carrier number:
         001  ->  0
         010  ->  1
         100  ->  2
       The input foc_mask could have one bit set only. This is ensured by mac already.
    */
    switch ( foc_mask )
    {
       case 0x01:
         foc_mask = 0;
         break;

       case 0x02:
         foc_mask = 1;
         break;

       case 0x04:
         foc_mask = 2;
         break;

       default:
         foc_mask = 0;
         break;
    }
    HWIO_OUTI(TX_DO_CTL_Cm, chan_idx,
                ( ( ((uint32)enable) << HWIO_TX_DO_CTL_Cm_EN_SHFT ) &
                    HWIO_TX_DO_CTL_Cm_EN_BMSK )   |
                ( ( foc_mask << HWIO_TX_DO_CTL_Cm_FOC_CTL_SHFT ) &
                  HWIO_TX_DO_CTL_Cm_FOC_CTL_BMSK )
             );
     /* To enable carrier:
          Enable carrier 0:  TX_DO_CTL_C0 = 0x00010000
          Enable carrier 1:  TX_DO_CTL_C1 = 0x00010001
          Enable carrier 1:  TX_DO_CTL_C2 = 0x00010002
     */

    HWIO_OUT(TX_UNIFIED_CARRIER_EN, 1);

#else
#ifndef FEATURE_HDR_BOLT_MODEM
    switch (chan_idx)
    {
      case 0:
        /* Configure the specified channel */
        HWIO_OUT( REVMOD_0_MCDO_CTL,
          ((((uint32) enable) << HWIO_REVMOD_0_MCDO_CTL_EN_SHFT) & 
           HWIO_REVMOD_0_MCDO_CTL_EN_BMSK) |
          (foc_mask & HWIO_REVMOD_0_MCDO_CTL_FOC_CTL_BMSK));
        break;
      case 1:
        /* Configure the specified channel */
        HWIO_OUT( REVMOD_1_MCDO_CTL,
          ((((uint32) enable) << HWIO_REVMOD_1_MCDO_CTL_EN_SHFT) & 
           HWIO_REVMOD_1_MCDO_CTL_EN_BMSK) |
          (foc_mask & HWIO_REVMOD_1_MCDO_CTL_FOC_CTL_BMSK));
        break;
      case 2:
        /* Configure the specified channel */
        HWIO_OUT( REVMOD_2_MCDO_CTL, 
          ((((uint32) enable) << HWIO_REVMOD_2_MCDO_CTL_EN_SHFT) & 
           HWIO_REVMOD_2_MCDO_CTL_EN_BMSK) |
          (foc_mask & HWIO_REVMOD_2_MCDO_CTL_FOC_CTL_BMSK));
        break;
      default:
        /* Do nothing for now */    
        break;
    }
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */
  }
  /*
     We don't disable a carrier by writting 0x0 to REVMOD_2_MCDO_CTL (DIME) 
     or TX_DO_CTL_cm (Triton). This will avoid the situation of DAC retriggering.
     HW will stop DAC automatically if all carriers are disabled. When enabling
     carrier again, DAC needs to be retriggerred. Since we don't disable carrier,
     we don't need to do DAC retrigger.
  */
#ifndef FEATURE_HDR_VI_VERIFICATION
  HDR_MSG_DRIVERS_3( MSG_LEGACY_HIGH, 
                     "HDR Mod Config: idx %x, enable %x, mask %x",
                     chan_idx, enable,
                     foc_mask
                    );
#endif  /* FEATURE_HDR_VI_VERIFICATION */

}     

/*===========================================================================
FUNCTION HDRMOD_DISABLE_INACTIVE_CARRIERS

DESCRIPTION
  Disables unused carriers. Should be called after the unused carriers
  have been disabled in FW.

DEPENDENCIES
  None.

PARAMETERS
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_disable_inactive_carriers( void )
{
#ifdef FEATURE_HDR_UNIFIED_TX
  uint8  num_enabled_carriers=0;
    /* number of enabled carriers */
  uint8  ch;
    /* loop index */

  if ( hdrmod_traffic_mode != HDRMOD_TRAFFIC_MODE_REVB )
  {
     HDR_MSG_DRIVERS( MSG_LEGACY_HIGH,
          " Rev0/A traffic: leave the carrier enabled");
     return;
  }
  /* Don't disable the last carrier otherwise DAC will be stopped automatically */
  for (ch = 0; ch < 3; ch++ )
  {
    if ( hdrmod_carrier_enabled[ch] == TRUE)
    {
      num_enabled_carriers ++;
    }
  }

  if ( num_enabled_carriers ==0 )
  {
    HDR_MSG_DRIVERS( MSG_LEGACY_HIGH,
           "RevB traffic: Don't disable the last carrier");
    return;
  }

  /* For each carrier, disable carrier if status is set to disabled. */
  if (hdrmod_carrier_enabled[0] == FALSE)
  {
     /* Configure the specified channel */
     HWIO_OUTI(TX_DO_CTL_Cm, 0, 0);
  }
  else if (hdrmod_carrier_enabled[1] == FALSE)
  {
     /* Configure the specified channel */
     HWIO_OUTI(TX_DO_CTL_Cm, 1, 0);
  }
  else if (hdrmod_carrier_enabled[2] == FALSE)
  {
     /* Configure the specified channel */
     HWIO_OUTI(TX_DO_CTL_Cm, 2, 0);
  }
#else
#ifndef FEATURE_HDR_BOLT_MODEM
  /* For each carrier, disable carrier if status is set to disabled. */
  if (hdrmod_carrier_enabled[0] == FALSE)
  {
      /* Configure the specified channel */
      HWIO_OUT( REVMOD_0_MCDO_CTL, 0 );
  }
  
  if (hdrmod_carrier_enabled[1] == FALSE)
  {
      /* Configure the specified channel */
      HWIO_OUT( REVMOD_1_MCDO_CTL, 0 );
  }

  if (hdrmod_carrier_enabled[2] == FALSE)
  {
      /* Configure the specified channel */
      HWIO_OUT( REVMOD_2_MCDO_CTL, 0 );
  }
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */

#ifndef FEATURE_HDR_VI_VERIFICATION
  HDR_MSG_DRIVERS_3( MSG_LEGACY_HIGH, 
                     "HDR Mod Disable: %x, %x, %x", hdrmod_carrier_enabled[0],
                     hdrmod_carrier_enabled[1], hdrmod_carrier_enabled[2] );
#endif  /* FEATURE_HDR_VI_VERIFICATION */

}     

#ifndef FEATURE_HDR_VI_VERIFICATION
/*===========================================================================

FUNCTION HDRMOD_PROCESS_PA_ON_CTRL_RSP

DESCRIPTION 
  Processes FW response to previously queued PA ON control command -
  marks the command as complete by resetting command pending status.

DEPENDENCIES
  None.

PARAMETERS
  pa_status - Status of PA returned by FW
              1 = Enabled
              0 = Disabled

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrmod_process_pa_on_ctrl_rsp
(
  int32 pa_status
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  if ( hdrmod_pending_pa_request.pending )
  {
    /* Mark pa status change as complete if request sent and response
       received match */
    if ( ( (hdrmod_pending_pa_request.request == HDRMOD_PA_STATUS_ON) &&
          (pa_status == 0x01) ) ||
          ( (hdrmod_pending_pa_request.request == HDRMOD_PA_STATUS_OFF) &&
          (pa_status == 0) ) )
    {
    hdrmod_pending_pa_request.pending = FALSE;
    }

    HDR_MSG_DRIVERS_1 (MSG_LEGACY_HIGH, 
      "PA ON ctrl response received.  PA status = %d",
      pa_status );

    /* Since PA has been disabled, it is safe to update disabled carriers. */
    if (pa_status == 0)
    {
      hdrmod_disable_inactive_carriers();
    }

#if defined FEATURE_HDR_REVB_DTX_MODE || defined FEATURE_HDR_TRAFFIC_DTX
    /* If we have a callback registered then notify about response received */
    if ( hdrmod_pa_mode_monitor != NULL )
    {
      (*hdrmod_pa_mode_monitor) ( (pa_status == 0x01) );
    }
#endif /* FEATURE_HDR_REVB_DTX_MODE || FEATURE_HDR_TRAFFIC_DTX*/
  }
  else
  {
    HDR_MSG_DRIVERS_1 ( MSG_LEGACY_ERROR, 
      "PA ON ctrl response received without PA pending status set. PA status = %d",
      pa_status );
  }

  /* Update PA status */
  hdrmod_pa_status = ( pa_status == 0 ) ? 
    HDRMOD_PA_STATUS_OFF : HDRMOD_PA_STATUS_ON;

} /* hdrmod_process_pa_on_ctrl_rsp */


/*===========================================================================

FUNCTION HDRMOD_SETUP_TX_TIMED_ENABLE

DESCRIPTION 
  Sends message to firmware to enable carrier and PA at the slot
  offset indicated.

DEPENDENCIES
  Time at which to enable Tx must be at least 2 slots in future.

PARAMETERS
  slot_offset - Should be 0-15; PA will enable on next occurence 
                (ie, when lower bits of hstr matches offset in halfslots).

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrmod_setup_tx_timed_enable
(
  boolean immediate,
    /* Whether to disable PA/carrier ASAP */
  uint8 slot_offset
    /* Slot (0-15) at which to turn on PA. */
)
{
  hdrfw_tx_pa_ctl_msg_t fw_cmd;
    /* Firmware command */

  errno_enum_type estatus;
    /* Error status */
 
  uint16 start_halfslot;
    /* Used for start time */

  uint16 hstr_now;
    /* Used for computing ASAP time */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Return if -
     ~ PA-enable has already been requested earlier and
       a response confirming PA-enable has arrived 
     OR
     ~ PA-enable has been requested earlier, a response for the same hasn't
       arrived yet, but the current command is not immediate */
  if ( ( hdrmod_pa_on && !(hdrmod_pending_pa_request.pending) ) ||
       ( hdrmod_pa_on && (hdrmod_pending_pa_request.pending) && !immediate ) )
  {
    HDR_MSG_DRIVERS_2 ( MSG_LEGACY_HIGH,
      "PA already enabled: tx_timed_enable (imm=%d) for slot %d skipped",
       immediate, 
       slot_offset);

     return;
  }

  /* Track the last requested pa state */
  hdrmod_pa_on = TRUE;
  hdrmod_pa_needed = TRUE;

  /* Print debug msg if the previously queued PA status change request is
     still pending. */
  if ( hdrmod_pending_pa_request.pending )
  {
    HDR_MSG_DRIVERS ( MSG_LEGACY_HIGH,
    "Response to previous timed PA cmd not received -- will be canceled");
  }

  /* Mark pa status change as pending */
  hdrmod_pending_pa_request.pending = TRUE;

  hstr_now = HDRHSTR_GET_COUNT();

  if (immediate == TRUE)
  {
    /* Translate immediate tx enable to tx enable scheduled 3 slots 
       in future */
    start_halfslot = (HDRHSTR_GET_COUNT() + 6) % 32;
  }
  else
  {
    /* Need offset in halfslots for calculations */
    start_halfslot = slot_offset * 2;
  }

  /* Initialize message for firmware. */
  memset ( &fw_cmd, 0, sizeof(hdrfw_tx_pa_ctl_msg_t) );
  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, 
                  HDR_FW_TX_PA_CTL_CMD );

  /* Set to enable at next tx_offset boundary */
  fw_cmd.immediate = 0;
  fw_cmd.txHalfSlotIndex = start_halfslot;
  fw_cmd.txPaState = 1;

  /* Store last PA request */
  hdrmod_pending_pa_request.request = HDRMOD_PA_STATUS_ON;
  hdrmod_pending_pa_request.immediate = FALSE;

  HDR_MSG_DRIVERS_1 ( MSG_LEGACY_MED,
    "txHalfSlotIndex = %x",
    fw_cmd.txHalfSlotIndex );

  /* Send the message */
  estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_tx_pa_ctl_msg_t) );
  ASSERT ( estatus == E_SUCCESS );

  HDR_MSG_DRIVERS_3 ( MSG_LEGACY_HIGH,
                     "tx_timed_enable@%x (imm=%d) for slot %d.", 
                     hstr_now, immediate, slot_offset);

}

/*===========================================================================

FUNCTION HDRMOD_SETUP_TX_TIMED_DISABLE

DESCRIPTION 
  Sends message to firmware to disable carrier and PA at the slot
  offset indicated, or immediately.

DEPENDENCIES
  If not immediate, slot_offset should be at least 2 slots in future.

PARAMETERS
  immediate   - Indicates that the disable should happen immediately.
  slot_offset - Should be 0-15; PA will disable on next occurence 
                (ie, when lower bits of hstr matches offset in halfslots).

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrmod_setup_tx_timed_disable
(
   boolean immediate,
     /* Whether to disable PA/carrier immediately */
   uint8 slot_offset
     /* Slot (0-15) at which to turn on PA. */
)
{
  hdrfw_tx_pa_ctl_msg_t fw_cmd;
    /* Firmware command */

  errno_enum_type estatus;
    /* Error status */
 
  uint16 start_halfslot;
    /* Used for start time */

  uint16 hstr_now;
    /* Time now */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Return if -
     ~ PA-disable has already been requested earlier and
       a response confirming PA-disable has arrived 
     OR
     ~ PA-disable has been requested earlier, a response for the same hasn't
       arrived yet, but the current command is not immediate */
  if ( ( !hdrmod_pa_on && !(hdrmod_pending_pa_request.pending) ) ||
       ( !hdrmod_pa_on && (hdrmod_pending_pa_request.pending) && !immediate ) )
  {
    HDR_MSG_DRIVERS_2 ( MSG_LEGACY_HIGH,
      "PA already disabled: tx_timed_disable (imm=%d) for slot %d skipped",
       immediate, 
       slot_offset);

     return;
  }

  /* Track the last requested pa state */
  hdrmod_pa_on = FALSE;
  hdrmod_pa_needed = FALSE;

  /* Print debug msg if the previously queued PA status change request is
     still pending. */
  if ( hdrmod_pending_pa_request.pending )
  {
    HDR_MSG_DRIVERS ( MSG_LEGACY_HIGH,
    "Response to previous timed PA cmd not received -- will be canceled");
  }

  /* Mark pa status change as pending */
  hdrmod_pending_pa_request.pending = TRUE;

  hstr_now = HDRHSTR_GET_COUNT();

  /* Need offset in halfslots for calculations */
  start_halfslot = slot_offset * 2;

  /* Initialize message for firmware. */
  memset ( &fw_cmd, 0, sizeof(hdrfw_tx_pa_ctl_msg_t) );
  msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, 
                  HDR_FW_TX_PA_CTL_CMD );

  /* Set to disable at next tx_frame_start_offset boundary */
  fw_cmd.immediate = immediate;
  fw_cmd.txHalfSlotIndex = start_halfslot;
  fw_cmd.txPaState = 0;

  /* Store last PA request */
  hdrmod_pending_pa_request.request = HDRMOD_PA_STATUS_OFF;
  hdrmod_pending_pa_request.immediate = immediate;

  /* Send the message */
  estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_tx_pa_ctl_msg_t) );
  ASSERT ( estatus == E_SUCCESS );

  HDR_MSG_DRIVERS_3 ( MSG_LEGACY_HIGH,
      "tx_timed_disable@%x (imm=%d), for slot %d.",
       hstr_now, immediate, slot_offset);

}

#ifdef FEATURE_HDR_UNIFIED_TX

/*===========================================================================

FUNCTION HDRMOD_TX_START_SECOND_HALF_OF_FRAME_ISR                  INTERNAL

DESCRIPTION 
  Helper ISR for tx start processing. The ISR is programmed to fire at 
  the beginning of the second half of frame. It sets signal to unblock 
  the waiting function - hdrmod_tx_start().
 
DEPENDENCIES
  None.
 
PARAMETERS
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrmod_tx_start_second_half_of_frame_isr
( 
  uint32 unused_param
    /* Parameter */
)
{
  unused_param=0;
    /* To avoid compiler warning */

  /* Disable frame ISR */
  hdrmod_disable_frame_isr();

  /* Set signal to continue with tx start processing */
  (void) rex_set_sigs(HDRMOD_TASK_PTR, HDRTX_MOD_TASK_DONE_SIG);

} /* hdrmod_tx_start_second_half_of_frame_isr() */



/*===========================================================================

FUNCTION HDRMOD_TX_START_FRAME_BOUNDARY_ISR

DESCRIPTION 
  Helper ISR for tx start processing. The ISR is programmed to fire at 
  the frame boundary. It sets signal to unblock the waiting function - 
  hdrmod_tx_start().
 
DEPENDENCIES
  None.
 
PARAMETERS
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrmod_tx_start_frame_boundary_isr
(
  uint32 unused_param
    /* parameter */
)
{
  unused_param=0;
    /* To avoid compiler warning */

  /* Disable frame ISR */
  hdrmod_disable_frame_isr();

  /* Set signal to continue with tx start processing */
  (void) rex_set_sigs(HDRMOD_TASK_PTR, HDRTX_MOD_TASK_DONE_SIG);

} /* hdrmod_tx_start_frame_boundary_isr() */



/*===========================================================================
FUNCTION HDRMOD_RESTORE_FRAME_ISR

DESCRIPTION
  This function restores previous backuped frame ISR
  
DEPENDENCIES
  Must not be called without a previous backup_frame_isr()

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_restore_frame_isr(void)
{
  hdrmod_disable_frame_isr();

  HDR_MSG_DRIVERS_8( MSG_LEGACY_HIGH, 
                     "Restore ISR %x=>%x installed:%d=>%d type:%d=>%d offset:%d=>%d",                     
                     hdrmod_isr.isr_ptr, 
                     hdrmod_isr_backup.isr_ptr,                     
                     hdrmod_isr.installed,
                     hdrmod_isr_backup.installed,                     
                     hdrmod_isr.type,
                     hdrmod_isr_backup.type,                     
                     hdrmod_isr.offset,
                     hdrmod_isr_backup.offset );

  hdrmod_isr = hdrmod_isr_backup;

  if ( hdrmod_isr.installed )
  {
    /* Enable ISR */
    hdrmod_enable_frame_isr( hdrmod_isr.isr_ptr );
  }
} /* hdrmod_restore_frame_isr() */
#endif /* FEATURE_HDR_UNIFIED_TX */

/*===========================================================================

FUNCTION HDRMOD_TX_START

DESCRIPTION 
  Prefills and triggers the TX DAC.  Call after enabling RF TX.

  Tx start processing is executed in HDRTX task context.  The caller, if
  executing from a non-HDRTX task, must wait for at least two frames 
  ( ~53 ms ) for tx start processing to complete when using this function.

  The function uses frame ISR to time sending of TX DAC start msg to 
  firmware and to wait till next frame boundary for DAC start to take 
  an effect.  If a frame ISR is registered with hdrmod, it restored at
  the end of this function.
 
DEPENDENCIES
  None.
 
PARAMETERS
  None.
 
RETURN VALUE
  None.

SIDE EFFECTS
  Uninstalls frame ISR.  The ISR is restored at the end of tx start 
  processing.
===========================================================================*/
void hdrmod_tx_start( void )
{
  cfw_tx_dac_start_msg_t fw_cmd;
    /* Firmware command */

  errno_enum_type estatus;
    /* Error status */
 
  uint16 frame_rtc = 0;
#ifdef FEATURE_HDR_UNIFIED_TX
  uint16 rtc_time = 0, time_to_trigger = 0;
    /* for time check */
#endif /* FEATURE_HDR_UNIFIED_TX */
  hdrmod_cmd_union_type hdrmod_cmd; 
    /* To build command into local struct */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* The function must be executed from HDRTX task context. 
     Post command to HDRTX task if called from a different task context. */
  if( rex_self() != HDRMOD_TASK_PTR )
  {
    memset ( &hdrmod_cmd, 0, sizeof (hdrmod_cmd_union_type) );

    /* Fill command fields */
    hdrmod_cmd.hdr.cmd    = HDRMOD_TX_START;
    hdrmod_cmd.hdr.sender = HDRHAI_MODULATOR;

    /* Post command */
    HDRMOD_QUEUE_CMD( HDRHAI_MODULATOR,
      &hdrmod_cmd,
      sizeof(hdrmod_cmd_union_type) );
  }
  else
  {
    /* Skip tx start processing if tx has already been triggered */
    if( !hdrmod_tx_triggered )
    {
#ifdef FEATURE_HDR_UNIFIED_TX
      hdrmod_backup_frame_isr( );
      /* backup original ISR structure */

      hdrmod_disable_frame_isr_internal();
      /* Disable frame ISR.  If one is installed, it will be restored at 
         the end of tx start processing */

      HWIO_OUT( TX_UNIFIED_SAMPLE_CNT_INIT_VAL, 0 );
      HWIO_OUT( TX_UNIFIED_SAMPLE_CNT_LD, 1 );

      switch( hdrmod_traffic_mode )
      { 
         case HDRMOD_TRAFFIC_MODE_REV0 :
           /* fall thru */           

         case HDRMOD_TRAFFIC_MODE_REVA:
           /* Enable carrier 0 before DAC trigger. This is required by Triton HW.
            If this register is set to 0x10000 at rev0 traffic (c2k_mode=0x01),
            the following dac trigger for RevA/B traffic won't work. That's why we
            touch this register only for RevA  traffic.
           */
           HWIO_OUTI(TX_DO_CTL_Cm, 0, 0x00010000);
           break;

         case HDRMOD_TRAFFIC_MODE_REVB: 
           /* Enable carrier 0/1/2 before DAC trigger. Otherwise RevB MC call won't 
              work in Triton.
           */
           HWIO_OUTI(TX_DO_CTL_Cm, 0, 0x00010000);
           HWIO_OUTI(TX_DO_CTL_Cm, 1, 0x00010001);
           HWIO_OUTI(TX_DO_CTL_Cm, 2, 0x00010002);
           break;
          /* To enable carrier:
             Enable carrier 0:  TX_DO_CTL_C0 = 0x00010000
             Enable carrier 1:  TX_DO_CTL_C1 = 0x00010001
             Enable carrier 1:  TX_DO_CTL_C2 = 0x00010002
           */

         default:
           HDR_MSG_DRIVERS( MSG_LEGACY_ERROR, "unexpected traffic mode");
           break;
      }

      HWIO_OUT(TX_UNIFIED_CARRIER_EN, 1);  

      HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "Start prefill");
        /* Don't remove this F3. Without it compiler might switch the order of 
           setting TX_DO_CTL and OFFL_EN. The order is critical.
        */

      /* Tx setup and prefill - for frame boundary */
      HWIO_OUT(TX_UNIFIED_OFFL_EN, 1);
#else /* if !UNIFIED_TX */
#ifndef FEATURE_HDR_BOLT_MODEM
      /* Tx setup and prefill - for frame boundary */
      HWIO_OUT(MOD_DO_STROBE_OFFSET, 0x41);
      HWIO_OUT(REVMOD_SAMPLE_COUNT_OFFSET, 0x4);
      HWIO_OUT(REVMOD_PREFILL_SLOT_NUM, 0x0);
      HWIO_OUT(REVMOD_PREFILL_FRAME_NUM, 0x0);
      HWIO_OUT(MOD_SB_DO_OFFSET, 0x0);

      HWIO_OUT(MP_PN_SLAM_LONG_STATE_03, PN_LONG_STATE_L); 
      HWIO_OUT(MP_PN_SLAM_LONG_STATE_45, PN_LONG_STATE_H);

      HWIO_OUT(MP_SHORT_PN_I_SLAM, 0x1);
      HWIO_OUT(MP_SHORT_PN_Q_SLAM, 0x1);

      /* Busy wait for 1 microsecond. */
      DALSYS_BusyWait(1);

      /* Start Prefill */
      HWIO_OUT(MOD_DO_PREFILL_MODE, 1);
#endif /* FEATURE_HDR_BOLT_MODEM */
#endif  /* FEATURE_HDR_UNIFIED_TX */

      /* A minimum 52 us wait is needed for prefill to take an effect */
   
      /* Busy wait for 1 slot ( 1.667 ms ).  This wait is needed between sending of TxStart 
         and TxDACTrigger messages to FW due to a FW limitation. */
      DALSYS_BusyWait(1667);

      /* Compute frame boundary in chipx2.  txRTCofs is in chipx8 */
      frame_rtc = ( hdrmdsp_shared_mem_interface->read.general_status.
        txRTCOfs ) >> 2;

#ifdef FEATURE_HDR_UNIFIED_TX
#ifdef FEATURE_HDR_BOLT_MODEM
      rtc_time = hdrts_get_rtc_cx2();
#else
      /* Time check. Trigger must only be called within 13.3 ms of next frame
         boundary.  RTC time is in chipx2 */
      rtc_time = HWIO_IN(HDR_RTC_CNT);
#endif /* FEATURE_HDR_BOLT_MODEM */

      /* We need to trigger in 2nd half of frame. */
      /* How long until trigger time? Subtract in cx2, convert to chips. */
      time_to_trigger = ((uint16)(frame_rtc - rtc_time)) >> 1;

      /* If needed, delay until 2nd half of frame so that 
         tx_dac_start message occurs within 13 ms of 
         frame boundary. */ 
      if( ( (time_to_trigger) > HDRMOD_8_SLOTS_IN_CHIPS ) ||
          ( (time_to_trigger) < HDRMOD_ONE_SLOT_IN_CHIPS ) )
      {
        /* TODO: Remove msg after verification */
        HDR_MSG_DRIVERS_3( MSG_LEGACY_HIGH, 
          "frame_rtc:%d rtc_time:%d time to trigger:%x. Wait till 2nd half of frame", 
          frame_rtc, rtc_time, time_to_trigger );

        /* Program ISR to fire some time within the 2nd half of frame.
           Slot 10 is chosen here */
        hdrmod_set_tx_frame_interrupt_offset_internal(10);
        hdrmod_enable_frame_isr_internal( 
          hdrmod_tx_start_second_half_of_frame_isr );

        /* Wait for the ISR to fire */
        hdrtx_task_wait( HDRTX_MOD_TASK_DONE_SIG );
      }

      /* Reduce message priority after verification */
      HDR_MSG_DRIVERS_2( MSG_LEGACY_HIGH, 
        "HDR - Tx start sent %x (cx1) at rtc_time %x (cx2).", 
        (frame_rtc >> 1), 
        rtc_time );
#endif /* FEATURE_HDR_UNIFIED_TX */

#ifdef FEATURE_HDR_RUMI_TX_ONLY
      /* Call function to trigger flex capture */
      HDR_MSG_DRIVERS( MSG_LEGACY_LOW, "Start Flex capture");
      hdrhitmsg_start_flex_capture();
#endif /* FEATURE_HDR_RUMI_TX_ONLY */

      /* Initialize message for firmware. */
      memset ( &fw_cmd, 0, sizeof(cfw_tx_dac_start_msg_t) );
      msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, 
        HDR_FW_TX_DAC_START_CMD );

      /* Load trigger time (in chips) */
      fw_cmd.start_rtc_cx1 = (frame_rtc >> 1);

#ifdef FEATURE_HDR_SELECTIVE_TXD
      hdrsrchrftxd_rf_call_started( );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      hdrmod_dac_resp_pending = TRUE;

#ifdef FEATURE_HDR_SELECTIVE_TXD
      hdrsrchrftxd_rf_call_finished( );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      /* Send the message */
      estatus = msgr_send ( &fw_cmd.hdr, sizeof(cfw_tx_dac_start_msg_t) );
      ASSERT ( estatus == E_SUCCESS );

      /* Track internally */
      hdrmod_tx_triggered = TRUE;

      HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "Sent Tx DAC start cmd to FW");

#ifndef FEATURE_HDR_UNIFIED_TX
      /* Wait for response */
      hdrtx_wait_for_msg(HDR_FW_TX_DAC_START_RSP);
#else /* FEATURE_HDR_UNIFIED_TX */
      /* Wait till frame boundary for Tx DAC start to take an effect */

      /* Program ISR to fire at frame boundary - 0th slot */
      hdrmod_set_tx_frame_interrupt_offset_internal(0);
      hdrmod_enable_frame_isr_internal( hdrmod_tx_start_frame_boundary_isr );

      /* Wait for the ISR to fire */
      hdrtx_task_wait( HDRTX_MOD_TASK_DONE_SIG );

      /* Restore frame ISR if one was installed before calling this 
         function. */
      hdrmod_restore_frame_isr();
#endif /* FEATURE_HDR_UNIFIED_TX */

      hdrmod_dac_resp_pending = FALSE;

      HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "Tx DAC started");

      /* For BO SW will wait for Tx_DAC_START_RESP mesg to confirm
       * that the DAC started
       */

    } /* If tx has not been triggered yet */
    else
    {
      HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "Tx DAC already started");
    }
  }

  return;

} /* hdrmod_tx_start() */

/*===========================================================================

FUNCTION HDRMOD_SEND_TX_STOP

DESCRIPTION 
  Sends the Tx DAC stop message to FW.
  Tx stop processing is executed in HDRTX task context.
 
DEPENDENCIES
  Assumes TX DAC was enabled.
 
PARAMETERS
  None. 
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrmod_send_tx_stop( void )
{
#ifdef FEATURE_HDR_UNIFIED_TX
  hdrfw_tx_dac_stop_msg_t fw_cmd;
    /* Firmware command */
#else
  cfw_tx_dac_stop_msg_t fw_cmd;
    /* Firmware command */
#endif /* FEATURE_HDR_UNIFIED_TX */
  errno_enum_type estatus;
    /* Error status */

  uint8 ch;
    /* Loop counter */

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

      /* All carriers should be disabled at this point.
         Assert if that isn't the case. */
      for (ch = 0; ch < 3; ch++ )
      {
        ASSERT( hdrmod_carrier_enabled[ch] == FALSE );
      }

      HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "Sending Tx DAC stop to FW");

#ifdef  FEATURE_HDR_UNIFIED_TX
       /* Initialize message for firmware. */
      memset ( &fw_cmd, 0, sizeof(hdrfw_tx_dac_stop_msg_t) );
      msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, 
                      HDR_FW_TX_DAC_STOP_CMD );
      /* pass traffic mode to FW in DAC stop message */
      fw_cmd.hdrTrafficMode = hdrmod_traffic_mode;
      /* Send the message */
      estatus = msgr_send ( &fw_cmd.hdr, sizeof(hdrfw_tx_dac_stop_msg_t) );

#else  /* FEATURE_HDR_UNIFIED_TX */
      /* Initialize message for firmware. */
      memset ( &fw_cmd, 0, sizeof(cfw_tx_dac_stop_msg_t) );
      msgr_init_hdr ( &fw_cmd.hdr, MSGR_HDR_RMAC, 
                      HDR_FW_TX_DAC_STOP_CMD );
  
      /* Send the message */
      estatus = msgr_send ( &fw_cmd.hdr, sizeof(cfw_tx_dac_stop_msg_t) );
#endif /* FEATURE_HDR_UNIFIED_TX */

      ASSERT ( estatus == E_SUCCESS );
  
}

/*===========================================================================

FUNCTION HDRMOD_TX_STOP

DESCRIPTION 
  Disables Tx DAC via FW. Call before disabling RF TX.
  Tx stop processing is executed in HDRTX task context.
 
DEPENDENCIES
  Assumes TX DAC was enabled.
 
PARAMETERS
  None. 
 
RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void hdrmod_tx_stop( void )
{

  hdrmod_cmd_union_type hdrmod_cmd; 
    /* To build command into local struct */

  uint8 pa_ctrl_rsp_cnt=0;
    /* PA_CTL_RSP counter to check PA OFF status */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* The function must be executed from HDRTX task context. 
     Post command to HDRTX task if called from a different task context. */
  if( rex_self() != HDRMOD_TASK_PTR )
  {
    memset ( &hdrmod_cmd, 0, sizeof (hdrmod_cmd_union_type) );

    /* Fill command fields */
    hdrmod_cmd.hdr.cmd    = HDRMOD_TX_STOP;
    hdrmod_cmd.hdr.sender = HDRHAI_MODULATOR;

    /* Post command */
    HDRMOD_QUEUE_CMD( HDRHAI_MODULATOR,
      &hdrmod_cmd,
      sizeof(hdrmod_cmd_union_type) );
  }
  else
  {
    if ( hdrmod_tx_triggered )
    {
      /* Make sure PA is turned off before turning off DAC. */

      if ( ( hdrmod_pending_pa_request.pending ) ||
           (hdrmod_pa_status == HDRMOD_PA_STATUS_ON) )
      {
        /* PA status change is pending. Or Actual PA status is still ON */

        HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, 
          "Tx DAC stop: PA off not complete.");

        /* Send PA off command if the last sent command was not
           immediate PA off. */
        if ( ( hdrmod_pending_pa_request.request != HDRMOD_PA_STATUS_OFF ) ||
          !( hdrmod_pending_pa_request.immediate ) )
        {
          /* Send PA disable immediate command. */
          hdrmod_setup_tx_timed_disable( TRUE, 0 );
        }

        /* Account for the case where the response received is 
           for a previously sent timed enable and not the just 
           sent timed disable */
        do
        {
          /* Wait for response */
          hdrtx_wait_for_msg(HDR_FW_TX_PA_CTL_RSP);

          pa_ctrl_rsp_cnt++;

          if ( pa_ctrl_rsp_cnt >= 3)
          {
             /* After waiting for three responses, Assert this time PA
                status is OFF */
             ASSERT( hdrmod_pa_status == HDRMOD_PA_STATUS_OFF );
          }
        }while(hdrmod_pa_status == HDRMOD_PA_STATUS_ON);
        
      }

#ifdef FEATURE_HDR_SELECTIVE_TXD
      hdrsrchrftxd_rf_call_started( );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      hdrmod_dac_resp_pending = TRUE;

#ifdef FEATURE_HDR_SELECTIVE_TXD
      hdrsrchrftxd_rf_call_finished( );
#endif /* FEATURE_HDR_SELECTIVE_TXD */

      /* Send Tx DAC stop to FW */
      hdrmod_send_tx_stop();
  
#ifndef FEATURE_HDR_UNIFIED_TX
      /* Wait for response */
      hdrtx_wait_for_msg(HDR_FW_TX_DAC_STOP_RSP);
#endif /* FEATURE_HDR_UNIFIED_TX */

      /* Track internally */
      hdrmod_tx_triggered = FALSE;
  
      /* Reduce message priority after verification */
      HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "Tx DAC stopped");
#ifdef FEATURE_HDR_UNIFIED_TX
       /* Wait for one slot here. FW needs to stop tx after receiving DAC STOP
          message. It should be done after one slot. Then we are safe to 
          do the next step: set TX_C2K_MODE to 0. Otherwise if we touch 
          it when transmission is still going on, the following DAC trigger
          for RevA/B traffic would fail.
       */
       DALSYS_BusyWait(1667);
       HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, 
          "DAC stop: wait for one slot after DAC stopped");
#endif /* FEATURE_HDR_UNIFIED_TX */

       hdrmod_dac_resp_pending = FALSE;
    }
    else
    {
      HDR_MSG_DRIVERS( MSG_LEGACY_HIGH, "Tx DAC already stopped");
    }

  }
}


/*===========================================================================
FUNCTION HDRRMOD_PROCESS_CMD                                        EXTERNAL

DESCRIPTION
  This function processes the commands for modulator that were queued
  on the HDRTX task command queue.

DEPENDENCIES
  None.

PARAMETERS
  cmd_ptr - pointer to received command buffer.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_process_cmd
(
  hdrmod_cmd_union_type * cmd_ptr
)
{
  HDRTRACE_CMD( HDRHAI_MODULATOR, (uint32) cmd_ptr->hdr.cmd,
    cmd_ptr->hdr.sender, 0 );

  switch ( cmd_ptr->hdr.cmd )
  {
    case HDRMOD_TX_START:
      hdrmod_tx_start();
      break;

    case HDRMOD_TX_STOP:
      hdrmod_tx_stop();
      break;

    default:
      HDR_MSG_DRIVERS_1( MSG_LEGACY_ERROR, 
        "Unknown cmd! %d ", cmd_ptr->hdr.cmd );
  }

} /* hdrmod_process_cmd() */


/*============================================================================
FUNCTION HDRMOD_PROCESS_FW_MSG

DESCRIPTION
  This function processes firmware indication/message destined for HDR MOD.

DEPENDENCIES
  None.

PARAMETERS
  msg_ptr - pointer to received message

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_process_fw_msg 
(
  hdrtx_msg_s_type * msg_ptr 
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Sanity check */
  ASSERT( msg_ptr != NULL );
 
  switch (msg_ptr->msg.hdr.id)
  {
    /* PA ON control response message */
    case HDR_FW_TX_PA_CTL_RSP:
    {
      hdrmod_process_pa_on_ctrl_rsp( 
        msg_ptr->msg.fw_msg.rsp.tx_pa_ctl.Status );
      break;
    }

#ifndef FEATURE_HDR_UNIFIED_TX
    /* DAC_START or DAC_STOP RESP control response message */
    case HDR_FW_TX_DAC_START_RSP:
    {
      HDR_MSG_DRIVERS ( MSG_LEGACY_LOW, "HDR_FW_TX_DAC_START_RSP");
      /* No processing needed for BOLT */
      break;
    }

    case HDR_FW_TX_DAC_STOP_RSP:
    {
      HDR_MSG_DRIVERS ( MSG_LEGACY_LOW, "HDR_FW_TX_DAC_STOP_RSP");
      /* No processing needed for BOLT */
      break;
    }
#endif /* FEATURE_HDR_UNIFIED_TX */

    default:
    {
      HDR_MSG_DRIVERS_1 ( MSG_LEGACY_ERROR, 
        "Unknown message! UMID: 0x%x", msg_ptr->msg.hdr.id );
    }

  } /* switch */
 
} /* hdrmod_process_fw_msg */

#ifdef FEATURE_HDR_UNIFIED_TX
/*============================================================================
FUNCTION HDRMOD_CLEAR_CARRIER_ENABLED

DESCRIPTION
  This function clears carrier enable registers.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/

void hdrmod_clear_carrier_enabled( void )
{
   HWIO_OUTI(TX_DO_CTL_Cm, 0, 0x0);  

   HWIO_OUTI(TX_DO_CTL_Cm, 1, 0x0);  

   HWIO_OUTI(TX_DO_CTL_Cm, 2, 0x0);  

}
#endif /* FEATURE_HDR_UNIFIED_TX */

#endif  /* FEATURE_HDR_VI_VERIFICATION */

#if defined FEATURE_HDR_REVB_DTX_MODE || defined FEATURE_HDR_TRAFFIC_DTX
/*===========================================================================
FUNCTION HDRMOD_REGISTER_PA_MODE_MONITOR                          EXTERNAL

DESCRIPTION
  This function is used to register callback to monitor changes to PA.
  Note only one callback can be registered at a time.
  

DEPENDENCIES
  None.

PARAMETERS
  cb - PA mode monitor

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_register_pa_mode_monitor (hdrmod_pa_mode_monitor_type cb)
{
  if ( hdrmod_pa_mode_monitor != NULL )
  {
     HDR_MSG_DRIVERS_1( MSG_LEGACY_ERROR,
                        "nDTX: Callback %d already registered! Overwriting..",
                        hdrmod_pa_mode_monitor );
  }
  hdrmod_pa_mode_monitor = cb;
 
} /* hdrmod_register_pa_mode_monitor */

/*===========================================================================
FUNCTION HDRMOD_DEREGISTER_PA_MODE_MONITOR                          EXTERNAL

DESCRIPTION
  This function is used to deregister callback to monitor changes to PA.
  

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void hdrmod_deregister_pa_mode_monitor ( void )
{
   hdrmod_pa_mode_monitor = NULL;
 
} /* hdrmod_register_pa_mode_monitor */
#endif  /* FEATURE_HDR_REVB_DTX_MODE || FEATURE_HDR_TRAFFIC_DTX */

/*===========================================================================
FUNCTION HDRMOD_WAIT_FOR_PA_OFF

DESCRIPTION
  If no PA state change request is pending, the function returns immediately
    If PA is off, the function returns success/TRUE.
    If PA is on, the function returns failure/FALSE.
  If PA state change request is pending, the function waits for PA response.
    If the response is PA off, the function returns success/TRUE.
    If the response is PA on, the function returns failure/FALSE.

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  TRUE  - if the wait has resulted in PA off response
  FALSE - otherwise

SIDE EFFECTS
  None.
 
NOTE 
  The function must be executed in HDRTX task context. 
===========================================================================*/
boolean hdrmod_wait_for_pa_off ( void )
{

  if ( hdrmod_pending_pa_request.pending )
  {
    /* PA state change request is pending */

    /* Wait for response */
    hdrtx_wait_for_msg(HDR_FW_TX_PA_CTL_RSP);
  }

  if ( hdrmod_pa_status == HDRMOD_PA_STATUS_OFF )
  {
    /* PA state is OFF */
    return TRUE;
  }
  else
  {
    /* PA state is ON */
    return FALSE;
  }
 
} /* hdrmod_wait_for_pa_off */


#ifdef FEATURE_HDR_BOLT_MODEM
/*===========================================================================
FUNCTION hdrmod_reverse_bits

DESCRIPTION
  Reverses long code and long code masks to machine required format

DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE
  reversed uint32 bits

SIDE EFFECTS
  None.
 
NOTE 
  None 
===========================================================================*/
uint64 hdrmod_reverse_bits(uint64 bits)
{
  uint32 max_shift = 63; // sizeof(bits) * 8 - 1
  uint64 reverse_bits = bits;
  
  HDR_MSG_DRIVERS_1 ( MSG_LEGACY_LOW, "Input 0x%x", bits );

  bits >>= 1;
  while(bits)
  {
    reverse_bits <<= 1;
    reverse_bits |= bits & 1;
    bits >>= 1;
    max_shift--;
  }
  reverse_bits <<= max_shift;

  HDR_MSG_DRIVERS_1 ( MSG_LEGACY_LOW, "Output 0x%x", bits );

  return reverse_bits;
}

/*===========================================================================

FUNCTION HDRMOD_VSTMR_ISR

DESCRIPTION
  This vstmr Interrupt Service Routine is used to route all frame
  boundary event requests while the chip is awake.

DEPENDENCIES
  This ISR will be installed after the module receives first request
  for frame boundary event

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void hdrmod_vstmr_isr
(
  vstmr_hdr_event_tbl_s *event_tbl,
  void *arg,
  uint32 pending
)
{
  if ( ( pending & 0x01 ) &&
       ( hdrmod_isr.isr_ptr != NULL) )
  {
    hdrmod_isr.isr_ptr(0);
  }
}
#endif /* FEATURE_HDR_BOLT_MODEM */

#ifdef FEATURE_HDR_SELECTIVE_TXD
/*===========================================================================

FUNCTION HDRMOD_DAC_RESP_IS_PENDING

DESCRIPTION
  This function returns if DAC resp is pending. It is pending if
  1) DAC start has been sent and resp hasn't been received.
  2) DAC stop has been sent and resp hasn't been received.

DEPENDENCIES
  None

RETURN VALUE
  True - If DAC start/stop resp is pending
  False - otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean hdrmod_dac_resp_is_pending( void )
{
  return hdrmod_dac_resp_pending;
}
#endif /* FEATURE_HDR_SELECTIVE_TXD */

