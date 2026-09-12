/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                   H D R       M D S P     D R I V E R S

GENERAL DESCRIPTION

  This file contains the HDR modem DSP functions which are to be
  used for code download, DSP reset, MDSP command interrupt processing.


REFERENCES
  QDSP2 Firmware Interface Specification: 80-24597-1
  MSM3100 Device Specification: 93-V0104-1

EXTERNALIZED FUNCTIONS

  hdrmdsp_download
    Downloads MDSP firmware

  hdrmdsp_set_isr
    Sets HDR MDSP interrupt service routines for various F/W interrupts

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright(c) 2000-2018  by Qualcomm Technologies, Incorporated. All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/hdr.mpss/6.0/drivers/src/hdrmdsp.c#2 $

when       who     what, where, why
--------   ---     ---------------------------------------------
06/20/19   svu     Toggle FW DEMBACk/TDEC before moving FW 
                   to ACTIVE
05/01/18   vke     API added to return timing state status
08/24/17   vko     Add support for 1xSRLTE+W
04/28/14   wsh     Added NV control to pass MMSE config to FW
09/03/13   wsh     Replaced FEATURE_DIME/TRITON_BOLT_MODEM with HDR version
04/22/13   arm     Added changes for DSDA coexistence manager.
03/18/13   kss     Remove unused legacy code.
01/25/13   vke     Introduced mutex lock to prevent HW multi-thread to 
                   perform simultaneous operation of disabling and sending message
10/19/12   smd     Told fw whether the standby to active transition is for wakeup.
03/19/12   ljl     Fixed Dime compiler errors.
10/17/11   rmg     Removed calls to obsolete clk regim APIs.
06/17/11   smd     Added a flag for HDR RTC being started or not. 
01/18/11   smd     Added a check not to put fw into the same state.
08/16/10   lyl     Supported FW power management with suspend/resume.
08/02/10   smd     Added support for MEAS_CONNECTED state.
06/21/10   cnx     Fixed compiler warnings.
06/10/10   smd     Process state cfg response for MEAS state.
06/02/10   ljl     Removed mdsp.h.
04/21/10   smd     Added firmware MEAS state.
03/24/10   pxu     Removed FEATURE_MSM6500_DSP_SLOT_TIMER feature and 
                           FEATURE_HDR_IDLE_TIMELINE_V3 check. 
04/06/10   ljl     Added hdrmdsp_fws_enable_app().
03/17/10   ljl     Removed the obsolete include files.
03/11/10   ljl     Supported the fw timing state.
12/16/09   lyl     Waited for rsp before sending state config msg.
12/11/09   grl     Added FW state config support
11/20/09   kss     Revert last check-in.
11/16/09   kss     Restore removed types temporarily.
11/11/09   ljl     Removed the legacy mdsp service API. 
                   Waited for rsp after disabling hdr app.
10/07/09   ljl     Used hdrsrchmsg_send().
10/01/09   ljl     Removed hdr app enable from hdrmdsp_init().
09/30/09   kss     Removed obsolete types.
07/22/09   ljl     Initialized the pointers for unit tests.
07/15/09   etv     Moved sw module definitions to hdrmsg.h
06/30/09   lyl     Updated for generic responses process.
06/25/09   wsh     Fixed MOB compile error on ARM compiler
05/18/09   ljl/lyl Updated for firmware MSGR interface.
05/22/09   etv     Removed obsolete ISRs.
05/07/09   etv     Obsoleted hdrmdsp_disable_hdr_app_and_dec.
04/16/09   rkc     Removed T_QSC60X5 from mainline.
03/12/09   rkc     Merged clock resource control.
12/10/08   smd     Replaced assert.h with amssassert.h
11/16/08   kss     Removed/replaced intlocks.
11/13/08   kss     Don't uninstall dec ISR in app and dec disable function.
11/10/08   Vish    Added support for wideband saturation detection interrupt.
10/13/08   kss     Lint fixes.
06/12/08   rmg     Added support to write Tx carriers ordered by ranks.
12/05/07   etv     Added support for mDSP Rx and Tx buffer logging.
11/28/07   kss     Various integration fixes (typos, renamed vars, etc.)
10/20/07   rmg     Added Tx log buffer definition.
10/29/07   ljl     Updated for firmware release 0016.
09/27/07   ljl     Added hdrmdsp_handoff_c.
09/27/07   ljl     Removed best ASP change ISR.
09/21/07   etv     Removed hdrmdsp_rl_packet_order.
07/18/07   rmg     Added TxAGC control structure.
05/09/07   ljl     Added variable for the stubbed firmware registers.
04/18/07   rmg     Added MAC channel info structure.
04/17/07   jyw     Added RevB ASP firmware register definitions.
04/17/07   rmg     Added HDR_MDSP_LOG_ISR support.
04/13/07   kss     Added antenna structures.
04/09/07   rmg     RevB macro changes - MAC
04/02/07   ljl     Added RevB equalizer firmware register definitions.
03/31/07   rmg     Added RevB encoder firmware register definitions.
03/19/07   ljl     Added RevB finger firmware register definitions.
03/16/07   grl     Fixed RVCT warning.
01/17/07   jyw     Added controls to hdrmdsp related clocks for QSC60x5.
09/07/06   ljl     Added hdrmdsp_disable_hdr_app_and_dec().
08/23/06   grl     Removed the int clear when reging missed preamble int.
06/28/06   kss     Check for HDR tx logging as well as rx logging.
04/21/06   grl     Included hdrlogi.h to fix a dependency issue
04/18/06   ds      Replace hdr mdsp logging with mdsp services logging 
08/08/05   sq      Disable/Enable demod clock
07/05/05   ds      Added support for generic MDSP logging
09/01/04   bt      Don't enable SRCH4 application when HDR is being enabled
07/23/04   aaj     Enable SRCH4 application when HDR is being enabled
12/03/03   aaj     Enable HDR application before accessing dsp
11/04/03   bt      Call hdrmdsp_en/disable_clocks in hdrmdsp_en/disable_hdr_app
11/03/03   remi    Removed obsolete MDSP logging interface.
10/29/03   aaj     Use HDRMDSP API for enabling/disabling hdr application
10/06/03   aaj     Mainlined T_MSM6500 
09/21/03   bt      Hybrid mode Integration changes.
09/10/03   mpa     Converted F3 messages to use new MSG2.0 SSID
07/30/03   sq      Do not install timer callback if already exists
07/22/03   sq      Added support for FEATURE_MSM6500_DSP_SLOT_TIMER
03/14/03   kss     Added HDR_MDSP_SCC_LOST_ISR support.
05/20/02   sq      Leave CODECPLL on when TCXO is 19.8MHz.
05/13/02   ajn     Added INTLOCK's around critical sections.
03/25/02   aaj     Add dsp_clk_on flag to indicate status of dsp clocks
03/18/02   aaj     Remove/install isr handler during mdsp clock stop/start
03/04/02   kss     Added hdrmdsp_stop() and hdrmdsp_restart().
02/13/02   kss     Added hdrmdsp_enable_clocks() and hdrmdsp_disable_clocks()
                   for powering down the DSP.
09/13/01   aaj     Added support for BestASPChange Interrupt
08/22/01   aaj     Correct MEMA,B,C macros in DSP logging
05/03/01   aaj     Disable all MDSP interrupts at startup
04/18/01   aaj     Added Interrupt Table and MDSP log interface
04/09/01   aaj     Created this file
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "hdr_variation.h"
#include "comdef.h"        /* Definitions for byte, word, etc.        */

#include "hdrmdsp.h"
#include "mcpm_api.h"
#include "hdrdebug.h"
#include "rex.h"

#include "amssassert.h"
#include "err.h"
#include "msm.h"
#include "DALSys.h"

#include "hdrsrchdrv.h"    /* to know about active search status */
#include "hdrlogi.h"

#ifdef FEATURE_MDSP_USES_MODULES
/* This feature is needed for MSM6500 DSP services */
#include "task.h"
#include "hdrsrchtask.h"
#endif /* FEATURE_MDSP_USES_MODULES */

#include "hdrsrchmsg.h"
#include "hdrsrchstate.h"
#include "hdrmdspmcr.h"    /* MDSP access macros */
#include "hdrdec.h"

#include "fws.h"
#include "hdrmsg.h"
#include "hdrfw_msg.h"

/* <EJECT> */
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

  This section contains definitions for constants, macros, types, variables
  and other items needed by this module.

===========================================================================*/

#define HDRMDSP_STATE_CFG_TIMEOUT_US    10000
 /* Maximum wait time for HDR State to enable/disable */

#define HDR_NUM_TIMERS 4

#ifdef FEATURE_MDSP_USES_MODULES

#define HDRMDSP_NUM_DSP_INTR         1
  /* Three interrupts possible from MDSP to ARM */

#ifdef FEATURE_MDSPSVC_LOG  
#define HDRMDSP_LOG_POLL_TIME_IN_MS  10
  /* Time in milliseconds at which to poll the mDSP for new log chunks.*/
#endif /* FEATURE_MDSPSVC_LOG */
#endif /* FEATURE_MDSP_USES_MODULES */

/*==========================================================================

                     Firmware register definitions

==========================================================================*/

volatile hdrfw_shared_memory_interface_t *hdrmdsp_shared_mem_interface;
  /* Shared memory interface */


/*-------------------------------------------------------------------------
  HDR MDSP ISR Table :
  Single MDSP to ARM interrupt is shared for different events between
  MDSP and the ARM. ARM runs a small "tramplike" routine to register
  ISRs for different MDSP events. This table lists the MDSP Interrupt
  ISRs registered by the DMSS software
  -----------------------------------------------------------------------*/

typedef struct
{

  void          (*isr_ptr) ( void );    /* callback function */

} hdrmdsp_isr_tab_type;

/*-------------------------------------------------------------------------
   HDR Modem DSP General Purpose Timer Structure:
   HDR MDSP provides general purpose timers which can be used by the DMSS
   physical layer modules. These timers provide much higher accuracy than
   the conventional rex timers. Slot timer is accurate upto 0.8msec whereas
   rex timers are accurate upto 5msec
  -----------------------------------------------------------------------*/

typedef struct
{

  boolean    active;
    /* indicates if the timer is active */

  void       (*timer_cb) (void);
    /* timer callback */

} hdrmdsp_timer_struct_type;



/*-------------------------------------------------------------------------
   HDRMDSP data structure
  -----------------------------------------------------------------------*/

typedef struct
{

  hdrmdsp_isr_tab_type           isr_tab[HDR_MDSP_INT_CNT];
    /* HDR MDSP Interrupt Service Routine Table */

  hdrmdsp_timer_struct_type      timer_tab[HDR_NUM_TIMERS];
    /* General purpose timers provided by the DSP */

  boolean                        in_reset;
    /* Indicates whether the DSP is in reset (during sleep) */

  boolean                         dsp_clk_on;
    /* Indicates whether the DSP clock has been turned off */

  hdrfw_state_t                   hdr_app_state;
    /* HDR App state */

  uint16                          version;
    /* HDR firmware version */

  boolean                         fws_hdr_enabled;
    /* Flag whether fws hdr enabled or not */

  boolean                         fws_hdr_suspended;
    /* Flag to indicate whether HDR voted to suspend FW or not */

#if !( defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
 boolean                          is_hdr_rtc_started;
    /* Flag to indicate whether HDR RTC is started */
#endif /* T_MSM8960 */

 boolean                          mmse_rab;
 boolean                          mmse_arq;
   /* Flags to control MMSE detection:
      RAB: Enable MMSE for RAB
      ARQ: Enable MMSE for RPC/DRC/ARQ */

  rex_crit_sect_type             hdrmdspcntrl_crit_sect;
  /* Critical Section for protecting MDSP simultaneous control from HDR */

} hdrmdsp_struct_type;

/* <EJECT> */

/*===========================================================================

                           HDR MDSP STRUCT

===========================================================================*/

hdrmdsp_struct_type    hdrmdsp;
 /* data struct for HDR MDSP interface */

uint16 hdrmdsp_stub_var = 0;

hdrfw_state_cfg_msg_t  hdrmdsp_state_cfg_msg;
  /* HDR State enable message */

boolean                hdrmdsp_init_done = FALSE;
    /* Flag to indicate whether init is completed after bootup */


/*=========================================================================

FUNCTION     : HDRMDSP_UPDATE_FW_REG_ADDRESS

DESCRIPTION  : Update firmware register addresses.

DEPENDENCIES : None

INPUTS       : None

RETURN VALUE : None

=========================================================================*/

static void hdrmdsp_update_fw_reg_address( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
#if (defined (HDRSRCH_UT_PLATFORM) || defined(TEST_FRAMEWORK))
#error code not present
#endif /* (HDRSRCH_UT_PLATFORM) || (TEST_FRAMEWORK)*/

}  /* hdrmdsp_update_fw_reg_address */


/*===========================================================================

FUNCTION     : HDRMDSP_FWS_APP_IS_ENABLED

DESCRIPTION  : This function returns whether fws hdr app is enabled.

DEPENDENCIES : None

RETURN VALUE : TRUE:  enabled 
               FALSE: disabled 

SIDE EFFECTS : None

===========================================================================*/
boolean hdrmdsp_fws_app_is_enabled( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return hdrmdsp.fws_hdr_enabled;
} /* hdrmdsp_fws_app_is_enabled */


/*===========================================================================

FUNCTION     : HDRMDSP_IS_FW_IN_TIMING_STATE

DESCRIPTION  : This function returns whether hdr fws is in timing state

DEPENDENCIES : None

RETURN VALUE : TRUE:  FW in timing state 
               FALSE: FW in states other than timing state 

SIDE EFFECTS : None

===========================================================================*/
boolean hdrmdsp_is_fw_in_timing_state( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return ( hdrmdsp.hdr_app_state == HDRFW_STATE_TIMING );
} /* hdrmdsp_is_fw_in_timing_state */


/*===========================================================================

FUNCTION     : HDRMDSP_FWS_ENABLE_APP

DESCRIPTION  : This function enables fws hdr app.

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/
void hdrmdsp_fws_enable_app( void )
{
  uint32            err_mask = 0;
    /* Bitmask of applications which caused an error during enable or disable */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if ( hdrmdsp.fws_hdr_enabled == FALSE )
  {
    err_mask = fws_app_enable( FW_APP_HDR );
      /* Enable SRCH and HDR FW services */

    if ( err_mask & FW_APP_HDR )
    {
      ERR_FATAL( "FWS config failes: err mask 0x%x",
                 err_mask,
                 0,
                 0 );
    }

    hdrmdsp.fws_hdr_enabled = TRUE;
      /* fws hdr is enabled */
  }
}


/*===========================================================================

FUNCTION     : HDRMDSP_PROCESS_HDR_STATE_CFG_RSP

DESCRIPTION  : This function process HDR State Cfg rsp.

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/

void hdrmdsp_process_hdr_state_cfg_rsp
(
  hdrfw_state_cfg_rsp_msg_t *msg
)
{

/*-----------------------------------------------------------------------*/

  if ( msg != NULL ) 
  {
    hdrmdsp.version       = msg->hdrVersion;
    hdrmdsp.hdr_app_state = msg->state;

    hdrsrchmsg_process_rcvd_rsp( HDRSRCH_STATE_CFG_RSP );
      /* The cfg rsp has been received */

    if ( ( hdrmdsp.hdr_app_state == HDRFW_STATE_ACTIVE ) ||
         ( hdrmdsp.hdr_app_state == HDRFW_STATE_MEAS ) ||
         ( hdrmdsp.hdr_app_state == HDRFW_STATE_MEAS_CONNECTED ) )
    {
      hdrmdsp_shared_mem_interface = 
                ( hdrfw_shared_memory_interface_t *) msg->sharedMemoryAddr;
        /* Update the address of the shared memory */

      hdrsrchmsg_config_indications();
        /* Config indications for HDR SRCH */

      /* TBD config mac indication */

      /* TBD to make the old code fall through */
      hdrmdsp_update_fw_reg_address();
    }
    else
    {
       /* TBD */
       /* Set the shared memory address to NULL */
       /*hdrmdsp_shared_mem_interface = ( hdrfw_shared_memory_interface_t *) 0;*/
    }
  } /* if ( msg != NULL ) */
} /* hdrmdsp_process_hdr_state_cfg_rsp */

/*===========================================================================

FUNCTION     : HDRMDSP_SEND_STATE_CFG_MSG

DESCRIPTION  : This function sends the state config msg to HDR App.

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/

void hdrmdsp_send_state_cfg_msg
(
  hdrfw_state_t  state,
    /* State */

  boolean                    sleep_to_wakeup
    /* whether this is a sleep to wakeup transition. This is valid for active state only */

)
{
/*-----------------------------------------------------------------------*/

  HDR_MSG_DRIVERS_2( MSG_LEGACY_HIGH, "Config state %d, sleepToWakeup %d", 
                     state, sleep_to_wakeup );

  hdrmdsp_state_cfg_msg.state = state;
    /* Enable HDR App */

  if ( state == HDRFW_STATE_ACTIVE )
  {
    hdrmdsp_state_cfg_msg.sleepWakeUpOverride = sleep_to_wakeup;
  }
  else
  {
    hdrmdsp_state_cfg_msg.sleepWakeUpOverride = FALSE;
  }
  hdrsrchmsg_send_msg( &hdrmdsp_state_cfg_msg.hdr, 
                       sizeof( hdrfw_state_cfg_msg_t ) );

  hdrsrchmsg_set_rsp_waiting( HDRSRCH_STATE_CFG_RSP, TRUE );
    /* Wait for the rsp for the state cfg msg */

} /* hdrmdsp_send_state_cfg_msg */


/*===========================================================================

FUNCTION     : HDRMDSP_CONFIG_HDR_APP

DESCRIPTION  : This function configs HDR application

DEPENDENCIES : None.

RETURN VALUE : None.

SIDE EFFECTS : None.

===========================================================================*/

void hdrmdsp_config_hdr_app
( 
  hdrfw_state_t              state,
    /* State */

  boolean                    inline_wait,
    /* perform inline wait until DSP application is enabled */
  
  boolean                    sleep_to_wakeup
    /* whether this is a sleep to wakeup transition. This is valid for active state only */

)
{
  boolean                    rsp_received = FALSE;
    /* Whether the response is received */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

	HDR_MSG_DRIVERS_2( MSG_LEGACY_MED, 
					   "Setting FW %d -> %d", 
					   hdrmdsp.hdr_app_state, 
					   state );

  if ( hdrmdsp_fws_app_is_enabled() == TRUE )
  {
    if ( hdrmdsp.hdr_app_state == state )
    {
      HDR_MSG_DRIVERS_1( MSG_LEGACY_HIGH, 
                         "HDR App is already in state %d", 
                         state );

      return;
    }
    switch ( state )
    {
      case HDRFW_STATE_STANDBY:
  
        hdrmdsp_disable_clocks();
        break;
  
      case HDRFW_STATE_ACTIVE:
          /* Toggle Demback is required as DO FW shares registers with LTE
             This clears all the stale values in the registers*/
            HDR_MSG_SRCH( MSG_LEGACY_MED, "HDR FW entering ACTIVE. RESTART DEMBACK/TDEC" );
            MCPM_Toggle_Demback_And_Tdec();
         /*The below delay ensures that Dembaack/Tdec reset is completed */
            DALSYS_BusyWait(10);
      case HDRFW_STATE_MEAS:
      case HDRFW_STATE_MEAS_CONNECTED:
  
      default:
        break;
    }
  
    rsp_received = 
                 hdrsrch_wait_for_high_pri_rsp( HDRMDSP_STATE_CFG_TIMEOUT_US,
                                                HDRSRCH_STATE_CFG_RSP );
      /* Check whether we can send the app enable msg */
  
    if ( rsp_received )
    {
      hdrmdsp_send_state_cfg_msg( state, sleep_to_wakeup );
        /* Config the HDR SRCH Module in the firmware */
    }
    else
    {
      ERR_FATAL( "Previous HDR state cfg rsp not rcvd", 0, 0, 0 );
    }
  
    if ( inline_wait )
    {
      rsp_received = 
                  hdrsrch_wait_for_high_pri_rsp( HDRMDSP_STATE_CFG_TIMEOUT_US,
                                                 HDRSRCH_STATE_CFG_RSP );
       /* Wait for HDR APP to be enabled */
  
      if ( rsp_received == FALSE && ( hdrmdsp.hdr_app_state != state ) )
      {
        ERR_FATAL("HDR App cannot be enabled",0,0,0);
          /* HDR App is not enabled */
      }
    }
  
    switch ( state )
    {
      case HDRFW_STATE_STANDBY:
        break;
  
      case HDRFW_STATE_ACTIVE:
#if !( defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
         hdrmdsp_set_hdr_rtc_started();
           /* HDR RTC will be started when FW transits to active state in 
              the first time, and it won't be stopped untill power down 
           */
#endif /* T_MSM8960 */
      case HDRFW_STATE_MEAS:
      case HDRFW_STATE_MEAS_CONNECTED:

        hdrmdsp_enable_clocks();
        hdrmdsp_send_mmse_cfg_msg();
          /* Update cached MMSE cfg */
        break;
  
      default:
        break;
    }
  
  }
  else
  {
    HDR_MSG_DRIVERS( MSG_LEGACY_ERROR, "FWS HDR App is not enabled" );

    if ( ( state != HDRFW_STATE_STANDBY ) && ( state != HDRFW_STATE_INACTIVE ) )
    {
      ERR_FATAL("To state %d when FWS HDR App is not enabled",state,0,0);
    }
  }
}/* hdrmdsp_config_hdr_app */


/*===========================================================================

FUNCTION     : HDRMDSP_ENABLE_HDR_APP

DESCRIPTION  : This function enables HDR application

DEPENDENCIES : None.

RETURN VALUE : None.

SIDE EFFECTS : May perform rex_wait 
               HDRSRCH_ADHOC_GP_SIG may be set after a while

===========================================================================*/
/* TBD this function is to be removed after rf uses hdrmdsp_config_hdr_app */
void hdrmdsp_enable_hdr_app
( 
  boolean                    inline_wait
    /* perform inline wait until DSP application is enabled */
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrmdsp_config_hdr_app( HDRFW_STATE_ACTIVE, inline_wait, FALSE );
}  /* hdrmdsp_enable_hdr_app */
#if 0

/*===========================================================================

FUNCTION     : HDRMDSP_DISABLE_HDR_APP_AND_DEC

DESCRIPTION  : This function disables HDR application and turbo decoder.

DEPENDENCIES : HDR Decoder ISR should already have been uninstalled.

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/

void hdrmdsp_disable_hdr_app_and_dec( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  

  hdrmdsp_disable_clocks();

  hdrmdsp_send_disable_msg();
    /* Disable HDR App */

  DALSYS_BusyWait( 100 );
    /* Wait for HDR App to be disabled */

  hdrdec_deactivate( hdrsrch_dec_deactivate_cb );
    /* Reset and deactive decoder */

} /* hdrmdsp_disable_hdr_app_for_fast_deactivate */

#endif


/*===========================================================================

FUNCTION     : HDRMDSP_INIT

DESCRIPTION  : This function initializes the ARM DSP interface and downloads
               the DSP code once initialization is complete.

DEPENDENCIES : None.

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/

void hdrmdsp_init( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  hdrmdsp.fws_hdr_enabled = FALSE;
    /* fws hdr app is disabled */

  hdrmdsp.fws_hdr_suspended = FALSE;
    /* FW is not voted to suspend */

#if !( defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
  hdrmdsp.is_hdr_rtc_started = FALSE;
    /* HDR RTC has not been started */
#endif /* T_MSM8960 */
  hdrmdsp.hdr_app_state = HDRFW_STATE_INACTIVE;
    /* HDR App is not enabled */

  msgr_init_hdr( &hdrmdsp_state_cfg_msg.hdr, 
                 MSGR_HDR_SRCH, 
                 HDR_FW_STATE_CFG_CMD );
    /* Init message header */

  if(hdrmdsp_init_done == FALSE)
  {
    /* Initialize the flow control critical section variable */
    rex_init_crit_sect(&hdrmdsp.hdrmdspcntrl_crit_sect);

    /* After bootup the init is done */
    hdrmdsp_init_done = TRUE;
  }

  hdrmdsp.in_reset = FALSE;
   /* Init: not in reset */

  hdrmdsp.dsp_clk_on = FALSE;
    /* DSP clocks are off (?) TBD */

}/* hdrmdsp_init */



/*===========================================================================

FUNCTION     : HDRMDSP_DISABLE_CLOCKS

DESCRIPTION  : This function marks that the HDR DSP clocks are turned off

DEPENDENCIES : None.

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/

void hdrmdsp_disable_clocks( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

 /* Before disabling the flag, if the hdrmdspcntrl_crit_sect mutex is taken already 
          then pend on it till mutex is released (wait till msgr_send to be over) */
  rex_enter_crit_sect(&hdrmdsp.hdrmdspcntrl_crit_sect);

  /* hdrmdspcntrl_crit_sect mutex is relased (msg_send been over) and hence disable the flag */ 

    hdrmdsp.dsp_clk_on = FALSE;

 /* Leave Critical section */
  rex_leave_crit_sect(&hdrmdsp.hdrmdspcntrl_crit_sect);

}/* hdrmdsp_disable_clocks */


/*===========================================================================

FUNCTION     : HDRMDSP_ENABLE_CLOCKS

DESCRIPTION  : This function marks the HDR clock status as on

DEPENDENCIES : None.

RETURN VALUE : None.

SIDE EFFECTS : None.

===========================================================================*/

void hdrmdsp_enable_clocks( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   hdrmdsp.dsp_clk_on = TRUE;
     /* DSP clocks are on */

}/* hdrmdsp_enable_clocks */



/*===========================================================================

FUNCTION     : HDRMDSP_CHECK_DSP_CLK

DESCRIPTION  : This function returns the status of DSP clocks

DEPENDENCIES :

RETURN VALUE : True : if DSP clocks are ON.
               FALSE: if DSP clocks are OFF

SIDE EFFECTS : None.

===========================================================================*/

boolean hdrmdsp_check_dsp_clk( void )
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  return hdrmdsp.dsp_clk_on;

}/* hdrmdsp_check_dsp_clk */


/*===========================================================================

FUNCTION     : HDRMDSP_FWS_SUSPEND_APP

DESCRIPTION  : This function will send command to FWS to vote to suspend FW 
               if HDR is currently awake.

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/

void hdrmdsp_fws_suspend_app( void )
{
  uint32             err_mask;
    /* error mask: 0 means sucess */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrmdsp.fws_hdr_suspended == FALSE )
  {
    err_mask = fws_suspend( FW_APP_HDR );
      /* HDR app votes to put FW in suspend state */
  
    if ( err_mask )
    {
      HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Not able to put fw in suspend state" );
        /* May not be real err if there's another app is active */
    }

    hdrmdsp.fws_hdr_suspended = TRUE;
      /* Set the flag to true */
  }
 
} /* hdrmdsp_fws_suspend_app */


/*===========================================================================

FUNCTION     : HDRMDSP_FWS_RESUME_APP

DESCRIPTION  : This function will send command to FWS to resume/wake up FW if 
               HDR has voted to suspend previously.

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/

void hdrmdsp_fws_resume_app( void )
{

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  if ( hdrmdsp.fws_hdr_suspended == TRUE )
  {
    fws_resume( FW_APP_HDR );
      /* Resume and wake up FW if it's been vote to supend */

    hdrmdsp.fws_hdr_suspended = FALSE;
      /* set the flag to false */
  }
} /* hdrmdsp_fws_resume_app */
#if !( defined T_MSM8960 || defined FEATURE_HDR_DIME_MODEM )
/*===========================================================================

FUNCTION     : HDRMDSP_HDR_RTC_IS_STARTED

DESCRIPTION  : This function returns whether hdr rtc is started.

DEPENDENCIES : None

RETURN VALUE : TRUE:  being started
               FALSE: not being started

SIDE EFFECTS : None

===========================================================================*/
boolean hdrmdsp_hdr_rtc_is_started( void )
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return hdrmdsp.is_hdr_rtc_started;
} /* hdrmdsp_hdr_rtc_is_started */


/*===========================================================================

FUNCTION     : HDRMDSP_SET_HDR_RTC_STARTED

DESCRIPTION  : This function set the flag to indicate HDR RTC is started.

DEPENDENCIES : None

RETURN VALUE : None.

SIDE EFFECTS : None

===========================================================================*/
void hdrmdsp_set_hdr_rtc_started ( void )
{
  hdrmdsp.is_hdr_rtc_started = TRUE;
} /* hdrmdsp_set_hdr_rtc_started */
#endif /* T_MSM8960 */


/*===========================================================================

FUNCTION     : HDRMDSP_MSGR_SEND

DESCRIPTION  : This function sends message to MDSP.

DEPENDENCIES : None

RETURN VALUE : errno_enum_type

SIDE EFFECTS : None

===========================================================================*/
errno_enum_type hdrmdsp_msgr_send 
( 
  msgr_hdr_struct_type *msg,
    /* Firmware command to be sent */

  uint32  msg_size
  /* size of message to be sent */
)
{
  errno_enum_type estatus = E_FAILURE;
  /* Error status */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Before sending the message, if the hdrmdspcntrl_crit_sect mutex is taken already 
          then pend on it till mutex is released*/
  rex_enter_crit_sect(&hdrmdsp.hdrmdspcntrl_crit_sect);

  /* hdrmdspcntrl_crit_sect mutex is relased */
  if (hdrmdsp_check_dsp_clk())
  {
    estatus = msgr_send ( msg, msg_size );
      /* Send the message and as we can rely on flag condition 
                which is valid inside the mutex locked section */
  }

  /* Leave Critical section */
  rex_leave_crit_sect(&hdrmdsp.hdrmdspcntrl_crit_sect);

  return estatus;
}

#ifdef FEATURE_HDR_MODEM_COEXISTENCE_SW
/*===========================================================================

FUNCTION     : HDRMDSP_IS_MDSP_ACTIVE

DESCRIPTION  : This function returns if mdsp is active or not.

DEPENDENCIES : None

RETURN VALUE : TRUE if mdsp is active. 
               FALSE if not active. 

SIDE EFFECTS : None

===========================================================================*/
boolean hdrmdsp_is_mdsp_active ( void )
{

  return (hdrmdsp.hdr_app_state == HDRFW_STATE_ACTIVE );

} /* hdrmdsp_is_mdsp_active */

/*===========================================================================

FUNCTION     : HDRMDSP_SEND_MMSE_CFG_MSG

DESCRIPTION  : This function send the cached MMSE CFG msg to FW.

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

===========================================================================*/
void hdrmdsp_send_mmse_cfg_msg( void )
{
  hdrfw_fmac_detect_cfg_msg_t hdrmdsp_mmse_cfg_msg;
/*-----------------------------------------------------------------------*/

  HDR_MSG_DRIVERS_2( MSG_LEGACY_MED, "MMSE CFG: RAB %d ACQ %d", 
                     hdrmdsp.mmse_rab, hdrmdsp.mmse_arq );
  if ( ( hdrmdsp.mmse_rab != FALSE ) ||
       ( hdrmdsp.mmse_arq != FALSE ) )
  {
    /* Send command only if non-default. */

    memset( &hdrmdsp_mmse_cfg_msg, 0, sizeof(hdrmdsp_mmse_cfg_msg) );

    msgr_init_hdr( &hdrmdsp_mmse_cfg_msg.hdr, 
                   MSGR_HDR_SRCH,
                   HDR_FW_FMAC_DETECT_CFG_CMD );

    hdrmdsp_mmse_cfg_msg.mmseDetect.rab = hdrmdsp.mmse_rab;
    hdrmdsp_mmse_cfg_msg.mmseDetect.arq = hdrmdsp.mmse_arq;

    hdrsrchmsg_send_msg( &hdrmdsp_mmse_cfg_msg.hdr, 
                         sizeof( hdrfw_state_cfg_msg_t ) );
  }
    /* Not a critical msg and not waiting for RSP */
} /* hdrmdsp_send_state_cfg_msg */

/*===========================================================================

FUNCTION     : HDRMDSP_SET_MMSE_CFG

DESCRIPTION  : Function is called to set the MMSE CFG. The config is cached 
               within hdrmdsp module and pushed to FW after state cfg msg. 

DEPENDENCIES : None

RETURN VALUE : None

SIDE EFFECTS : None

===========================================================================*/
void hdrmdsp_set_mmse_cfg
( 
  boolean rab,
  boolean arq 
)
{
  hdrmdsp.mmse_rab = rab;
  hdrmdsp.mmse_arq = arq;
} /* hdrmdsp_set_mmse_cfg */
#endif /* FEATURE_HDR_MODEM_COEXISTENCE_SW */
